/** @file
  PCH Pcie SMM Driver Entry

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/
#include "PchInitSmm.h"
#include <PcieRegs.h>
#include <Register/PchRegs.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/PcieSipRegs.h>
#include <Library/PcieHelperLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/S3PciSegmentLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <Protocol/PchPolicy.h>
#include <PchPcieRpConfigHob.h>

GLOBAL_REMOVE_IF_UNREFERENCED PCIE_DEVICE_OVERRIDE          *mDevAspmOverride;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                        mNumOfDevAspmOverride;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                         mPchBusNumber;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                        mNumOfRootPorts;
//
// @note:
// These temp bus numbers cannot be used in runtime (hot-plug).
// These can be used only during boot.
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                         mTempRootPortBusNumMin;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                         mTempRootPortBusNumMax;

GLOBAL_REMOVE_IF_UNREFERENCED PCH_PCIE_ROOT_PORT_CONFIG     mPcieRootPortConfig[PCH_MAX_PCIE_ROOT_PORTS];

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                       mPchPciePmTrapExecuted = FALSE;

extern EFI_GUID gPchDeviceTableHobGuid;

/**
  Get PCIe port number for enabled port.
  @param[in] RpBase    Root Port pci segment base address

  @retval Root Port number (1 based)
**/
UINT32
PciePortNum (
  IN     UINT64  RpBase
  )
{
  return PciSegmentRead32 (RpBase + R_PCIE_CFG_LCAP) >> N_PCIE_CFG_LCAP_PN;
}

/**
  Get PCIe root port index

  @param[in] RpBase    Root Port pci segment base address

  @retval Root Port index (0 based)
**/
UINT32
PciePortIndex (
  IN     UINT64  RpBase
  )
{
  return PciePortNum (RpBase) - 1;
}

/**
  Enable Root Port as EOI target

  @param[in] RpBase  Address of PCIe device
**/
VOID
EFIAPI
PtlPcdEnableRpEoiTarget (
  IN  UINT64        RpBase
  )
{
  UINT32          RpIndex;

  RpIndex = PciePortIndex (RpBase);
  PtlPcdPsfEnableRpEoiTarget (RpIndex);
}

/**
  Program Common Clock and ASPM of Downstream Devices

  @param[in] PortIndex                  Pcie Root Port Number
  @param[in] RpDevice                   Pcie Root Pci Device Number
  @param[in] RpFunction                 Pcie Root Pci Function Number

  @retval EFI_SUCCESS                   Root port complete successfully
  @retval EFI_UNSUPPORTED               PMC has invalid vendor ID
**/
EFI_STATUS
PchPcieSmi (
  IN  UINT8                             PortIndex,
  IN  UINT8                             RpDevice,
  IN  UINT8                             RpFunction
  )
{
  UINT8                         SecBus;
  UINT8                         SubBus;
  UINT64                        RpBase;
  UINT64                        EpBase;
  UINT8                         EpPcieCapPtr;
  UINT8                         EpMaxSpeed;
  BOOLEAN                       DownstreamDevicePresent;
  UINT32                        Timeout;
  UINT32                        MaxLinkSpeed;
  UINT32                        OldDeviceState;
  PCIE_FEATURE_CONFIGURATION    FeatureConfiguration;
  UINT8                         CurrentLinkSpeed;

  RpBase   = PCI_SEGMENT_LIB_ADDRESS (
               DEFAULT_PCI_SEGMENT_NUMBER_PCH,
               mPchBusNumber,
               (UINT32) RpDevice,
               (UINT32) RpFunction,
               0
               );

  FeatureConfiguration.ConfigureMrrs = FALSE;
  FeatureConfiguration.ConfigureDynamicLinkThrottling = FALSE;
  FeatureConfiguration.Callback.EnableRpEoiTarget = PtlPcdEnableRpEoiTarget;
  if (PciSegmentRead16 (RpBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return EFI_SUCCESS;
  }
  OldDeviceState = PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_PMCS) & B_PCH_PCIE_CFG_PMCS_PS_MASK;
  PciSegmentAnd32 (RpBase + R_PCH_PCIE_CFG_PMCS, ~(UINT32) B_PCH_PCIE_CFG_PMCS_PS_MASK);
  //
  // Check presence detect state. Here the endpoint must be detected using PDS rather than
  // the usual LinkActive check, because PDS changes immediately and LA takes a few milliseconds to stabilize
  //
  DownstreamDevicePresent = !!(PciSegmentRead16 (RpBase + R_PCIE_CFG_SLSTS) & B_PCIE_SLSTS_PDS);

  if (DownstreamDevicePresent) {
    ///
    /// Make sure the link is active before trying to talk to device behind it
    /// Wait up to 100ms, according to PCIE spec chapter 6.7.3.3
    ///
    Timeout = 100 * 1000;
    while ((PciSegmentRead16 (RpBase + R_PCIE_CFG_LSTS) & B_PCIE_LSTS_LA) == 0 ) {
      MicroSecondDelay (10);
      Timeout-=10;
      if (Timeout == 0) {
        return EFI_NOT_FOUND;
      }
    }
    SecBus  = PciSegmentRead8 (RpBase + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
    SubBus  = PciSegmentRead8 (RpBase + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
    ASSERT (SecBus != 0 && SubBus != 0);
    RootportDownstreamConfiguration (
      DEFAULT_PCI_SEGMENT_NUMBER_PCH,
      DEFAULT_PCI_BUS_NUMBER_PCH,
      RpDevice,
      RpFunction,
      mTempRootPortBusNumMin,
      mTempRootPortBusNumMax,
      EnumPchPcie,
      &FeatureConfiguration
      );
    RootportDownstreamPmConfiguration (
      DEFAULT_PCI_SEGMENT_NUMBER_PCH,
      DEFAULT_PCI_BUS_NUMBER_PCH,
      RpDevice,
      RpFunction,
      mTempRootPortBusNumMin,
      mTempRootPortBusNumMax,
      &mPcieRootPortConfig[PortIndex].PcieRpCommonConfig,
      mNumOfDevAspmOverride,
      mDevAspmOverride,
      &FeatureConfiguration
    );
    //
    // Perform Equalization
    //
    EpBase = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, SecBus, 0, 0, 0);
    EpPcieCapPtr = PcieFindCapId (DEFAULT_PCI_SEGMENT_NUMBER_PCH, SecBus, 0, 0, EFI_PCI_CAPABILITY_ID_PCIEXP);
    EpMaxSpeed = PciSegmentRead8 (EpBase + EpPcieCapPtr + R_PCIE_LCAP_OFFSET) & B_PCIE_LCAP_MLS;
    MaxLinkSpeed = MIN (GetMaxLinkSpeed (RpBase), EpMaxSpeed);

    CurrentLinkSpeed = PciSegmentRead8 (RpBase + R_PCIE_CFG_LSTS) & B_PCIE_CFG_LSTS_CLS;
    if (CurrentLinkSpeed != MaxLinkSpeed) {
      PciSegmentAndThenOr16 (RpBase + R_PCIE_CFG_LCTL2,(UINT16) ~B_PCIE_LCTL2_TLS,(UINT16) MaxLinkSpeed);
      if (EpMaxSpeed >= 3) {
        PciSegmentOr32 (RpBase + R_PCIE_CFG_LCTL3, B_PCIE_CFG_LCTL3_PE);
        PciSegmentOr32 (RpBase + R_PCIE_CFG_LCTL, B_PCIE_CFG_LCTL_RL);
      }
    }
    ///
    ///  Follow PCIe BWG section 6.15 to restore root port D-state before back to OS.
    ///
    if (OldDeviceState == V_PCH_PCIE_CFG_PMCS_PS_D3H) {
      PciSegmentOr32 ((RpBase + R_PCH_PCIE_CFG_PMCS), B_PCH_PCIE_CFG_PMCS_PS_MASK);
    }
  }

  return EFI_SUCCESS;
}

/**
  PCIE Hotplug SMI call back function for each Root port

  @param[in] DispatchHandle             Handle of this dispatch function
  @param[in] RpContext                  Rootport context, which contains RootPort Index,
                                        and RootPort PCI BDF.
**/
VOID
EFIAPI
PchPcieSmiRpHandlerFunction (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  PCH_PCIE_SMI_RP_CONTEXT           *RpContext
  )
{
  PchPcieSmi (RpContext->RpIndex, RpContext->DevNum, RpContext->FuncNum);
}

/**
  PCIE Link Active State Change Hotplug SMI call back function for all Root ports

  @param[in] DispatchHandle             Handle of this dispatch function
  @param[in] RpContext                  Rootport context, which contains RootPort Index,
                                        and RootPort PCI BDF.
**/
VOID
EFIAPI
PchPcieLinkActiveStateChange (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  PCH_PCIE_SMI_RP_CONTEXT           *RpContext
  )
{
  return;
}

/**
  PCIE Link Equalization Request SMI call back function for all Root ports

  @param[in] DispatchHandle             Handle of this dispatch function
  @param[in] RpContext                  Rootport context, which contains RootPort Index,
                                        and RootPort PCI BDF.
**/
VOID
EFIAPI
PchPcieLinkEqHandlerFunction (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  PCH_PCIE_SMI_RP_CONTEXT           *RpContext
  )
{
  ///
  /// From PCI Express specification, the PCIe device can request for Link Equalization. When the
  /// Link Equalization is requested by the device, an SMI will be generated  by PCIe RP when
  /// enabled and the SMI subroutine would invoke the Software Preset/Coefficient Search
  /// software to re-equalize the link.
  ///

  return;

}

/**
  An IoTrap callback to config PCIE power management settings
**/
VOID
PchPciePmIoTrapSmiCallback (
  VOID
  )
{
  UINT32                        PortIndex;
  UINT64                        RpBase;
  PCIE_FEATURE_CONFIGURATION    FeatureConfiguration;

  FeatureConfiguration.ConfigureDynamicLinkThrottling = FALSE;

  for (PortIndex = 0; PortIndex < mNumOfRootPorts; PortIndex++) {
    RpBase = PchPcieRpPciCfgBase (PortIndex);

    if (PciSegmentRead16 (RpBase) != 0xFFFF) {
      RootportDownstreamPmConfiguration (
        DEFAULT_PCI_SEGMENT_NUMBER_PCH,
        DEFAULT_PCI_BUS_NUMBER_PCH,
        PchPcieRpDevNumber (PortIndex),
        PchPcieRpFuncNumber (PortIndex),
        mTempRootPortBusNumMin,
        mTempRootPortBusNumMax,
        &mPcieRootPortConfig[PortIndex].PcieRpCommonConfig,
        mNumOfDevAspmOverride,
        mDevAspmOverride,
        &FeatureConfiguration
      );

    }
  }
}

/**
  An IoTrap callback to config PCIE power management settings

  @param[in] DispatchHandle  - The handle of this callback, obtained when registering
  @param[in] DispatchContext - Pointer to the EFI_SMM_IO_TRAP_DISPATCH_CALLBACK_CONTEXT

**/
VOID
EFIAPI
PchPcieIoTrapSmiCallback (
  IN  EFI_HANDLE                            DispatchHandle,
  IN  EFI_SMM_IO_TRAP_CONTEXT                *CallbackContext,
  IN OUT VOID                               *CommBuffer,
  IN OUT UINTN                              *CommBufferSize
  )
{
  if (CallbackContext->WriteData == PchPciePmTrap) {
    if (mPchPciePmTrapExecuted == FALSE) {
      PchPciePmIoTrapSmiCallback ();
      mPchPciePmTrapExecuted = TRUE;
    }
  } else {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
  }
}

/**
  This function clear the Io trap executed flag before enter S3

  @param[in] Handle    Handle of the callback
  @param[in] Context   The dispatch context

  @retval EFI_SUCCESS  PCH register saved
**/
EFI_STATUS
EFIAPI
PchPcieS3EntryCallBack (
  IN  EFI_HANDLE                        Handle,
  IN CONST VOID                    *Context OPTIONAL,
  IN OUT VOID                      *CommBuffer OPTIONAL,
  IN OUT UINTN                     *CommBufferSize OPTIONAL
  )
{
  mPchPciePmTrapExecuted = FALSE;
  return EFI_SUCCESS;
}
/**
  Register PCIE Hotplug SMI dispatch function to handle Hotplug enabling

  @param[in] ImageHandle          The image handle of this module
  @param[in] SystemTable          The EFI System Table

  @retval EFI_SUCCESS             The function completes successfully
**/
EFI_STATUS
EFIAPI
InitializePchPcieSmm (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS                            Status;
  UINT32                                PortIndex;
  UINT8                                 Data8;
  UINT32                                Data32Or;
  UINT64                                RpBase;
  EFI_HANDLE                            PcieHandle;
  PCH_PCIE_SMI_DISPATCH_PROTOCOL        *PchPcieSmiDispatchProtocol;
  EFI_HANDLE                            PchIoTrapHandle;
  EFI_SMM_IO_TRAP_REGISTER_CONTEXT      PchIoTrapContext;
  EFI_SMM_SX_REGISTER_CONTEXT           SxDispatchContext;
  PCH_PCIE_IOTRAP_PROTOCOL              *PchPcieIoTrapProtocol;
  EFI_HANDLE                            SxDispatchHandle;
  PCH_POLICY_PROTOCOL                   *PchPolicy;
  PCIE_RP_DXE_CONFIG                    *PchPcieRpDxeConfig;
  UINTN                                 PcieDeviceTableSize;
  PCIE_DEVICE_OVERRIDE                  *DevAspmOverride;
  UINTN                                 Count;
  VOID                                  *HobPtr;
  PCIE_RP_CONFIG_HOB                    *PcieRpConfigHob;


  if (IsPchServer ()) {
    // Do not install BIOS SMI handlers for server's PCIe Root Ports
    // PCIe features like Hot Plug are handled by PCI Express Native Control feature in operating systems
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "InitializePchPcieSmm () Start\n"));

  mNumOfRootPorts    = GetPchMaxPciePortNum ();

  //
  // Locate Pch Pcie Smi Dispatch Protocol
  //
  Status = gSmst->SmmLocateProtocol (&gPchPcieSmiDispatchProtocolGuid, NULL, (VOID**) &PchPcieSmiDispatchProtocol);
  ASSERT_EFI_ERROR (Status);

  mPchBusNumber = DEFAULT_PCI_BUS_NUMBER_PCH;
  mTempRootPortBusNumMin = PcdGet8 (PcdSiliconInitTempPciBusMin);
  mTempRootPortBusNumMax = PcdGet8 (PcdSiliconInitTempPciBusMax);

  HobPtr = GetFirstGuidHob (&gPchPcieRpConfigHobGuid);
  ASSERT (HobPtr != NULL);
  PcieRpConfigHob = GET_GUID_HOB_DATA (HobPtr);
  CopyMem (
    mPcieRootPortConfig,
    PcieRpConfigHob->RootPort,
    sizeof (mPcieRootPortConfig)
    );

  DevAspmOverride                   = NULL;
  mDevAspmOverride                  = NULL;
  mNumOfDevAspmOverride             = 0;

  Status = gBS->LocateProtocol (&gPchPolicyProtocolGuid, NULL, (VOID **) &PchPolicy);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID*) PchPolicy, &gPchPcieRpDxeConfigGuid, (VOID*) &PchPcieRpDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DevAspmOverride = PchPcieRpDxeConfig->PcieDeviceOverrideTablePtr;

  Count = 0;
  if (DevAspmOverride != NULL) {
    for (Count = 0; DevAspmOverride[Count].DeviceId != 0; Count++) {}
  }

  if (Count !=0) {
    PcieDeviceTableSize = Count * sizeof (PCIE_DEVICE_OVERRIDE);
    Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      PcieDeviceTableSize,
                      (VOID **) &mDevAspmOverride
                      );
    if (!EFI_ERROR (Status)) {
      CopyMem (mDevAspmOverride, DevAspmOverride, PcieDeviceTableSize);
      mNumOfDevAspmOverride = (UINT32) Count;
    } else {
      mNumOfDevAspmOverride = 0;
      mDevAspmOverride = NULL;
      ASSERT_EFI_ERROR (Status);
    }
  }
  //
  // Throught all PCIE root port function and register the SMI Handler for enabled ports.
  //
  for (PortIndex = 0; PortIndex < mNumOfRootPorts; PortIndex++) {
    RpBase = PchPcieRpPciCfgBase (PortIndex);
    //
    // Skip the root port function which is not enabled
    //
    if (PciSegmentRead32 (RpBase) == 0xFFFFFFFF) {
      continue;
    }

    //
    // Register SMI Handlers for Hot Plug and Link Active State Change
    //
    Data8 = PciSegmentRead8 (RpBase + R_PCIE_CFG_SLCAP);
    if (Data8 & B_PCIE_SLCAP_HPC) {
      PcieHandle = NULL;
      Status = PchPcieSmiDispatchProtocol->HotPlugRegister (
                                             PchPcieSmiDispatchProtocol,
                                             PchPcieSmiRpHandlerFunction,
                                             PortIndex,
                                             &PcieHandle
                                             );
      ASSERT_EFI_ERROR (Status);

      Status = PchPcieSmiDispatchProtocol->LinkActiveRegister (
                                             PchPcieSmiDispatchProtocol,
                                             PchPcieLinkActiveStateChange,
                                             PortIndex,
                                             &PcieHandle
                                             );
      ASSERT_EFI_ERROR (Status);

      Data32Or  = B_PCH_PCIE_CFG_MPC_HPME;
      S3PciSegmentOr32 (RpBase + R_PCH_PCIE_CFG_MPC, Data32Or);
    }

    //
    // Register SMI Handler for Link Equalization Request from Gen 3 Devices.
    //
    Data8 = PciSegmentRead8 (RpBase + R_PCIE_CFG_LCAP);
    if ((Data8 & B_PCIE_LCAP_MLS) == V_PCIE_LCAP_MLS_GEN3) {
      Status = PchPcieSmiDispatchProtocol->LinkEqRegister (
                                             PchPcieSmiDispatchProtocol,
                                             PchPcieLinkEqHandlerFunction,
                                             PortIndex,
                                             &PcieHandle
                                             );
      ASSERT_EFI_ERROR (Status);
    }
  }

  ASSERT_EFI_ERROR (Status);

  PchIoTrapContext.Type     = WriteTrap;
  PchIoTrapContext.Length   = 4;
  PchIoTrapContext.Address  = 0;
  Status = mPchIoTrap->Register (
                         mPchIoTrap,
                         (EFI_SMM_HANDLER_ENTRY_POINT2) PchPcieIoTrapSmiCallback,
                         &PchIoTrapContext,
                         &PchIoTrapHandle
                         );
  ASSERT_EFI_ERROR (Status);

  //
  // Install the PCH Pcie IoTrap protocol
  //
  Status = (gBS->AllocatePool) (EfiBootServicesData, sizeof (PCH_PCIE_IOTRAP_PROTOCOL), (VOID **)&PchPcieIoTrapProtocol);
  ASSERT_EFI_ERROR (Status);
  PchPcieIoTrapProtocol->PcieTrapAddress = PchIoTrapContext.Address;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gPchPcieIoTrapProtocolGuid,
                  PchPcieIoTrapProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register the callback for S3 entry
  //
  SxDispatchContext.Type  = SxS3;
  SxDispatchContext.Phase = SxEntry;
  Status = mSxDispatch->Register (
                          mSxDispatch,
                          PchPcieS3EntryCallBack,
                          &SxDispatchContext,
                          &SxDispatchHandle
                          );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "InitializePchPcieSmm, IoTrap @ %x () End\n", PchIoTrapContext.Address));

  return EFI_SUCCESS;
}
