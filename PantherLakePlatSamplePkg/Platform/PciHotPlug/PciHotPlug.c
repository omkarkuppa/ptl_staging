/** @file
  Pci Hotplug Driver : This file will perform specific PCI-EXPRESS
  Devics resource configuration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2006 Intel Corporation.

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

//
// Statements that include other files
//
#include "PciHotPlug.h"
#include "PciHotPlugHelpers.h"

#include <Protocol/ITbtPolicy.h>
#if FixedPcdGetBool (PcdDTbtEnable) == 1
#include <Protocol/DxeDTbtPolicy.h>
#endif
#include <Library/PchPcieRpLib.h>
#include <Library/TbtCommonLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/ItbtPcieRpLib.h>
#include <Library/TcssInfoLib.h>
#include <Library/PchInfoLib.h>
#include <TcssDataHob.h>
#include <PcieRegs.h>
#include <Register/PchRegs.h>
#include <Library/PciExpressLib.h>

#define ITBT_CONTROLLER                   0x80
#define INVALID_RP_CONTROLLER_TYPE        0xFF

#define PCIE_NUM  (12)

#define PEG_NUM   (0)

#define PADDING_BUS (1)
#define PADDING_NONPREFETCH_MEM (1)
#define PADDING_PREFETCH_MEM (1)
#define PADDING_IO (1)
#define PADDING_NUM (PADDING_BUS + PADDING_NONPREFETCH_MEM + PADDING_PREFETCH_MEM + PADDING_IO)

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HPC_LOCATION          mPcieLocation[PCIE_NUM + PEG_NUM + MAX_ITBT_PCIE_PORT];
GLOBAL_REMOVE_IF_UNREFERENCED P2PB_RESOURCE_RECORD      mP2pbResourceRecord[MAX_P2PB_OF_TBT_DAISY_CHAIN];
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                     mP2pbResourceRecordCount = 0;
// Record the number of HotPlug capability Bridge on the same Bus.
GLOBAL_REMOVE_IF_UNREFERENCED NUMBER_OF_HOTPLUG_BRIDGE  mNumOfHotPlugBridge[MAX_P2PB_OF_TBT_DAISY_CHAIN];

GLOBAL_REMOVE_IF_UNREFERENCED UINTN   mHpcCount = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32  mRemainderBus = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN   gItbtSegment = 0;

GLOBAL_REMOVE_IF_UNREFERENCED PCIE_HOT_PLUG_DEVICE_PATH mHotplugPcieDevicePathTemplate = {
  ACPI,
  PCI(0xFF, 0xFF), // Dummy Device no & Function no
  END
};

/**
  Entry point for the driver.

  This routine reads the PlatformType GPI on FWH and produces a protocol
  to be consumed by the chipset driver to effect those settings.

  @param[in]  ImageHandle    An image handle.
  @param[in]  SystemTable    A pointer to the system table.

  @retval     EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
PciHotPlug (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                       Status;
  PCI_HOT_PLUG_INSTANCE            *PciHotPlug;
  UINTN                            Index;
  UINTN                            RpDev;
  UINTN                            RpFunc;
  PCIE_HOT_PLUG_DEVICE_PATH        *HotplugPcieDevicePath;
  UINTN                            VariableSize;
  PCH_SETUP                        PchSetup;
  ITBT_INFO_HOB                    *ITbtInfoHob;
  UINTN                            RpBus;
  UINTN                            RpSegment;

  RpSegment = 0;
  RpDev = 0;
  RpFunc = 0;

  //
  // Get TBT INFO HOB
  //
  ITbtInfoHob = NULL;
  ITbtInfoHob = (ITBT_INFO_HOB *) GetFirstGuidHob (&gITbtInfoHobGuid);
  if (ITbtInfoHob == NULL) {
    DEBUG ((DEBUG_INFO, "ITbtInfoHob not found\n"));
  }
  ZeroMem (mP2pbResourceRecord, sizeof (mP2pbResourceRecord));
  ZeroMem (mNumOfHotPlugBridge, sizeof (mNumOfHotPlugBridge));

  VariableSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &PchSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // PCH Rootports Hotplug device path creation
  //
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    if (PchSetup.PcieRootPortHPE[Index]) { // Check the Rootport no's hotplug is set
      Status = GetPchPcieRpDevFun (Index, &RpDev, &RpFunc); // Get the actual device/function no corresponding to the Rootport no provided
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
      }

      HotplugPcieDevicePath = NULL;
      HotplugPcieDevicePath = AllocatePool (sizeof (PCIE_HOT_PLUG_DEVICE_PATH));
      ASSERT (HotplugPcieDevicePath != NULL);
      if (HotplugPcieDevicePath == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CopyMem (HotplugPcieDevicePath, &mHotplugPcieDevicePathTemplate, sizeof (PCIE_HOT_PLUG_DEVICE_PATH));
      HotplugPcieDevicePath->PciRootPortNode.Device = (UINT8) RpDev; // Update real Device no
      HotplugPcieDevicePath->PciRootPortNode.Function = (UINT8) RpFunc; // Update real Function no

      mPcieLocation[mHpcCount].HpcDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)HotplugPcieDevicePath;
      mPcieLocation[mHpcCount].HpbDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)HotplugPcieDevicePath;
      mHpcCount++;

      DEBUG ((DEBUG_INFO, "(%02d) PciHotPlug (PCH RP#) : Bus 0x00, Device 0x%x, Function 0x%x is added to the Hotplug Device Path list \n", mHpcCount, RpDev, RpFunc));
    }
  }

  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    if((ITbtInfoHob != NULL) && (ITbtInfoHob->ITbtRootPortConfig[Index].ITbtPcieRootPortEn)) {
      Status = GetItbtPcieRpInfo((UINTN) Index, &RpSegment, &RpBus, &RpDev, &RpFunc);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
      }
      gItbtSegment = RpSegment;
      HotplugPcieDevicePath = NULL;
      HotplugPcieDevicePath = AllocatePool (sizeof (PCIE_HOT_PLUG_DEVICE_PATH));
      ASSERT (HotplugPcieDevicePath != NULL);
      if (HotplugPcieDevicePath == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CopyMem (HotplugPcieDevicePath, &mHotplugPcieDevicePathTemplate, sizeof (PCIE_HOT_PLUG_DEVICE_PATH));
      HotplugPcieDevicePath->PciRootPortNode.Device = (UINT8) RpDev; // Update real Device no
      HotplugPcieDevicePath->PciRootPortNode.Function = (UINT8) RpFunc; // Update real Function no

      mPcieLocation[mHpcCount].HpcDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)HotplugPcieDevicePath;
      mPcieLocation[mHpcCount].HpbDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)HotplugPcieDevicePath;
      mHpcCount++;

      DEBUG ((DEBUG_INFO, "(%02d) CPU PCIe HotPlug (ITBT RP#) : Bus 0x00, Device 0x%x, Function 0x%x is added to the Hotplug Device Path list \n", mHpcCount, RpDev, RpFunc));
    }
  }
  PciHotPlug = AllocatePool (sizeof (PCI_HOT_PLUG_INSTANCE));
  ASSERT (PciHotPlug != NULL);
  if (PciHotPlug == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize driver private data.
  //
  ZeroMem (PciHotPlug, sizeof (PCI_HOT_PLUG_INSTANCE));

  PciHotPlug->Signature                               = PCI_HOT_PLUG_DRIVER_PRIVATE_SIGNATURE;
  PciHotPlug->HotPlugInitProtocol.GetRootHpcList      = GetRootHpcList;
  PciHotPlug->HotPlugInitProtocol.InitializeRootHpc   = InitializeRootHpc;
  PciHotPlug->HotPlugInitProtocol.GetResourcePadding  = GetResourcePadding;

  Status = gBS->InstallProtocolInterface (
                  &PciHotPlug->Handle,
                  &gEfiPciHotPlugInitProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &PciHotPlug->HotPlugInitProtocol
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

/**
  This procedure returns a list of Root Hot Plug controllers that require
  initialization during boot process

  @param[in]  This      The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol.
  @param[out] HpcCount  The number of Root HPCs returned.
  @param[out] HpcList   The list of Root HPCs. HpcCount defines the number of elements in this list.

  @retval EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
GetRootHpcList (
  IN EFI_PCI_HOT_PLUG_INIT_PROTOCOL    *This,
  OUT UINTN                            *HpcCount,
  OUT EFI_HPC_LOCATION                 **HpcList
  )
{
  *HpcCount = mHpcCount;
  *HpcList  = mPcieLocation;

  return EFI_SUCCESS;
}

/**
  This procedure Initializes one Root Hot Plug Controller
  This process may casue initialization of its subordinate buses

  @param[in]  This            The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol.
  @param[in]  HpcDevicePath   The Device Path to the HPC that is being initialized.
  @param[in]  HpcPciAddress   The address of the Hot Plug Controller function on the PCI bus.
  @param[in]  Event           The event that should be signaled when the Hot Plug Controller initialization is complete. Set to NULL if the caller wants to wait until the entire initialization process is complete. The event must be of the type EFI_EVT_SIGNAL.
  @param[out] HpcState        The state of the Hot Plug Controller hardware. The type EFI_Hpc_STATE is defined in section 3.1.

  @retval   EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
InitializeRootHpc (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL      *This,
  IN  EFI_DEVICE_PATH_PROTOCOL            *HpcDevicePath,
  IN  UINT64                              HpcPciAddress,
  IN  EFI_EVENT                           Event, OPTIONAL
  OUT EFI_HPC_STATE                       *HpcState
  )
{
  if (Event) {
    gBS->SignalEvent (Event);
  }

  *HpcState = EFI_HPC_STATE_INITIALIZED;

  return EFI_SUCCESS;
}

/**
  Selects the proper TBT Root port to assign resources

  @param[in]  SetupData          Pointer to Setup data
  @param[in]  RpIndex            TBT PCIe Root Port Index
  @param[in]  TbtType            TBT Root Port Type

  @retval     TbtSelectorChosen  Root port number.
**/
UINT8
GetRootPortToSetResourcesForTbt (
  IN SETUP_DATA *SetupData,
  IN UINTN      RpIndex,
  IN UINT8      TbtType
  )
{
  UINT8              TbtSelectorChosen;
  TCSS_DATA_HOB      *TcssHob;
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  UINT8              Index;
  DTBT_INFO_HOB      *DTbtInfoHob;
#endif

  TbtSelectorChosen = 0xFF;

  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    DEBUG ((DEBUG_ERROR, "GetRootPortToSetResourcesForTbt(): Failed to get TCSS data HOB\n"));
  } else {
    if (TbtType == ITBT_CONTROLLER && RpIndex < MAX_ITBT_PCIE_PORT) {
      if (TcssHob->TcssData.ItbtPcieRpEn[RpIndex]) {
        TbtSelectorChosen = (UINT8) RpIndex;
      }
    }
  }

#if FixedPcdGetBool (PcdDTbtEnable) == 1
  //
  // Get DTBT INFO HOB
  //
  DTbtInfoHob = (DTBT_INFO_HOB *) GetFirstGuidHob (&gDTbtInfoHobGuid);
  if (DTbtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "DTbtInfoHob not found!!\n"));
  } else {
    if (TbtType == PCIE_RP_TYPE_CPU) {
      for (Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
        if (DTbtInfoHob->DTbtControllerConfig[Index].DTbtControllerEn == 1) {
          //
          // Check if root port type is PCH or CPU PCIe
          //
          if (DTbtInfoHob->DTbtControllerConfig[Index].RpType == TbtType) {
            //
            // Check if Controller is Enabled or not.
            //
            if ((SetupData->DTbtController[Index] == 0x01) && ((DTbtInfoHob->DTbtControllerConfig[Index].PcieRpNumber - 1) == (UINT8) RpIndex)) {
              TbtSelectorChosen = Index;
              break;
            }
          }
        }
      }
    }
  }
#endif

  DEBUG ((DEBUG_INFO, "TbtSelectorChosen = %x\n", TbtSelectorChosen));

  return TbtSelectorChosen;
}

/**
  Update P2P Bridge Resource Record

  @param[in] *HpcDevicePath       The Device Path to the HPC that is being initialized.
  @param[in] HpcPciAddress        The address of the Hot Plug Controller function on the PCI bus.
  @param[in] RsvdExtraBusNum      Reserved Bus resource Number.
  @param[in] RsvdPcieMegaMem      Reserved Alignment of MEM resource.
  @param[in] PcieMemAddrRngMax    Alignment of MEM resource.
  @param[in] RsvdPciePMegaMem     Reserved Pcie Kilo Io Number.
  @param[in] PciePMemAddrRngMax   Alignment of PMEM resource.
  @param[in] RsvdPcieKiloIo       Reserved Pcie Kilo Io Number.

  @retval    EFI_SUCCESS.
**/
EFI_STATUS
UpdateP2pbResourceRecord (
  IN     EFI_DEVICE_PATH_PROTOCOL        *HpcDevicePath,
  IN     UINT64                          HpcPciAddress,
  IN     UINT8                           RsvdExtraBusNum,
  IN     UINT16                          RsvdPcieMegaMem,
  IN     UINT8                           PcieMemAddrRngMax,
  IN     UINT32                          RsvdPciePMegaMem,
  IN     UINT8                           PciePMemAddrRngMax,
  IN     UINT8                           RsvdPcieKiloIo
  )
{
  UINT16 Index;

  for (Index = 0; Index < MAX_P2PB_OF_TBT_DAISY_CHAIN; Index++) {
    if (mP2pbResourceRecord[Index].HpcPciAddress == 0) {
      continue;
    }
    if (mP2pbResourceRecord[Index].HpcPciAddress == HpcPciAddress) {
      //
      // Record already existed, so ignore it.
      //
      return EFI_ABORTED;
    }
  }
  //
  // Record the current HPC's resource assignment
  //
  mP2pbResourceRecord[mP2pbResourceRecordCount].HpcDevicePath = HpcDevicePath;
  mP2pbResourceRecord[mP2pbResourceRecordCount].HpcPciAddress = HpcPciAddress;
  mP2pbResourceRecord[mP2pbResourceRecordCount].RsvdExtraBusNum = RsvdExtraBusNum;
  mP2pbResourceRecord[mP2pbResourceRecordCount].RsvdPcieMegaMem = RsvdPcieMegaMem;
  mP2pbResourceRecord[mP2pbResourceRecordCount].PcieMemAddrRngMax = PcieMemAddrRngMax;
  mP2pbResourceRecord[mP2pbResourceRecordCount].RsvdPciePMegaMem = RsvdPciePMegaMem;
  mP2pbResourceRecord[mP2pbResourceRecordCount].PciePMemAddrRngMax = PciePMemAddrRngMax;
  mP2pbResourceRecord[mP2pbResourceRecordCount].RsvdPcieKiloIo = RsvdPcieKiloIo;
  DEBUG ((DEBUG_INFO, "mP2pbResourceRecord[%x].HpcDevicePath = %s \n", mP2pbResourceRecordCount, ConvertDevicePathToText (mP2pbResourceRecord[mP2pbResourceRecordCount].HpcDevicePath, TRUE, TRUE)));
  DEBUG ((DEBUG_INFO, "mP2pbResourceRecord[%x].HpcPciAddress = %lx \n", mP2pbResourceRecordCount, mP2pbResourceRecord[mP2pbResourceRecordCount].HpcPciAddress));
  DEBUG ((DEBUG_INFO, "mP2pbResourceRecord[%x].RsvdExtraBusNum = %x \n", mP2pbResourceRecordCount, mP2pbResourceRecord[mP2pbResourceRecordCount].RsvdExtraBusNum));
  DEBUG ((DEBUG_INFO, "mP2pbResourceRecord[%x].RsvdPcieMegaMem = %x \n", mP2pbResourceRecordCount, mP2pbResourceRecord[mP2pbResourceRecordCount].RsvdPcieMegaMem));
  DEBUG ((DEBUG_INFO, "mP2pbResourceRecord[%x].PcieMemAddrRngMax = %x \n", mP2pbResourceRecordCount, mP2pbResourceRecord[mP2pbResourceRecordCount].PcieMemAddrRngMax));
  DEBUG ((DEBUG_INFO, "mP2pbResourceRecord[%x].RsvdPciePMegaMem = %x \n", mP2pbResourceRecordCount, mP2pbResourceRecord[mP2pbResourceRecordCount].RsvdPciePMegaMem));
  DEBUG ((DEBUG_INFO, "mP2pbResourceRecord[%x].PciePMemAddrRngMax = %x \n", mP2pbResourceRecordCount, mP2pbResourceRecord[mP2pbResourceRecordCount].PciePMemAddrRngMax));
  DEBUG ((DEBUG_INFO, "mP2pbResourceRecord[%x].RsvdPcieKiloIo = %x \n", mP2pbResourceRecordCount, mP2pbResourceRecord[mP2pbResourceRecordCount].RsvdPcieKiloIo));

  return EFI_SUCCESS;
}

/**
  Get Root Port physical Number by CPU or PCH Pcie Root Port Device and Function Number

  @param[in]  RpDev                 Root port device number.
  @param[in]  RpFun                 Root port function number.
  @param[out] RpNumber              Return corresponding physical Root Port index (0-based)
**/
VOID
GetPcieRpNumber (
  IN  UINTN   RpDev,
  IN  UINTN   RpFun,
  OUT UINTN   *RpNumber
  )
{
  UINT64   RpBase;
  UINT8    RpPcieCapPtr;
  UINT32   RegVal32;

  *RpNumber = 0xFF;
  RpBase = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, RpDev, RpFun, 0);
  if (PciSegmentRead32 (RpBase + PCI_VENDOR_ID_OFFSET) != 0xFFFFFFFF) {
    RpPcieCapPtr = PcieBaseFindCapId (RpBase, EFI_PCI_CAPABILITY_ID_PCIEXP);
    if (RpPcieCapPtr != 0) {
      RegVal32 = PciSegmentRead32 (RpBase + RpPcieCapPtr + R_PCIE_LCAP_OFFSET);
      *RpNumber = ((PCI_REG_PCIE_LINK_CAPABILITY *) &RegVal32)->Bits.PortNumber - 1;
    }
  }
}

/**
  Handle the Resource Padding For RootPort

  @param[in] *HpcDevicePath       The Device Path to the HPC that is being initialized.
  @param[in] HpcPciAddress        The address of the Hot Plug Controller function on the PCI bus.
  @param[in] RsvdExtraBusNum      Reserved Bus resource Number.
  @param[in] RsvdPcieMegaMem      Reserved Alignment of MEM resource.
  @param[in] PcieMemAddrRngMax    Alignment of MEM resource.
  @param[in] RsvdPciePMegaMem     Reserved Pcie Kilo Io Number.
  @param[in] PciePMemAddrRngMax   Alignment of PMEM resource.
  @param[in] RsvdPcieKiloIo       Reserved Pcie Kilo Io Number.

**/
VOID
HandleResourcePaddingForRootPort (
  IN     EFI_DEVICE_PATH_PROTOCOL        *HpcDevicePath,
  IN     UINT64                          HpcPciAddress,
  OUT    UINT8                           *RsvdExtraBusNum,
  OUT    UINT16                          *RsvdPcieMegaMem,
  OUT    UINT8                           *PcieMemAddrRngMax,
  OUT    UINT32                          *RsvdPciePMegaMem,
  OUT    UINT8                           *PciePMemAddrRngMax,
  OUT    UINT8                           *RsvdPcieKiloIo
  )
{
  EFI_STATUS                Status;
  PCH_SETUP                 PchSetup;
  SETUP_DATA                SetupData;
  UINTN                     VariableSize;
  UINTN                     RpIndex;
  UINTN                     RpBus;
  UINTN                     RpDev;
  UINTN                     RpFunc;
  UINT8                     PcieRpType;
  UINT8                     TbtController;
  ITBT_POLICY_PROTOCOL      *ITbtPolicy;
  DXE_ITBT_CONFIG           *DxeITbtConfig;
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  DXE_DTBT_POLICY_PROTOCOL  *DxeDTbtConfig;
#endif

  RpIndex         = 0xFF;
  PcieRpType      = INVALID_RP_CONTROLLER_TYPE;
  *RsvdPcieKiloIo = 0; // Do not assign any IO Resource for any RP by default.

  Status = gBS->LocateProtocol (&gITbtPolicyProtocolGuid, NULL, (VOID **) &ITbtPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ITbtPolicy, &gDxeITbtConfigGuid, (VOID *) &DxeITbtConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Check if PCIe Root HPC Controller need to reserve bus for docking downstream P2P hotplug
  //
  VariableSize = sizeof(SETUP_DATA);
  Status = gRT->GetVariable(
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  VariableSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &PchSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  RpBus  = (UINTN) ((HpcPciAddress >> 24) & 0xFF);
  RpDev  = (UINTN) ((HpcPciAddress >> 16) & 0xFF);
  RpFunc = (UINTN) ((HpcPciAddress >> 8) & 0xFF);

  // Get the actual Rootport no corresponding to the device/function no provided
  if (RpDev == GetITbtPcieDevNumber ()) {
    GetItbtPciePortIndex (gItbtSegment, RpBus, RpDev, RpFunc, &RpIndex);
    PcieRpType = ITBT_CONTROLLER;
    DEBUG ((DEBUG_INFO, "GetResourcePadding : ITBT Rootport no %02d Bus 0x%x, Device 0x%x, Function 0x%x \n", RpIndex, RpBus, RpDev, RpFunc));
  } else if ((RpBus == 0) && ((RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1) || (RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9))) {
    // CPU PCIe
    PcieRpType = PCIE_RP_TYPE_CPU;
    GetPcieRpNumber (RpDev, RpFunc, &RpIndex);
    DEBUG ((DEBUG_INFO, "GetResourcePadding : CPU PCIe root port# %02d Bus 0x%x, Device 0x%x, Function 0x%x \n", RpIndex, RpBus, RpDev, RpFunc));
    if (RpIndex < GetPchMaxPciePortNum ()) {
      *RsvdExtraBusNum = PchSetup.PcieExtraBusRsvd[RpIndex];
      *RsvdPcieMegaMem = PchSetup.PcieMemRsvd[RpIndex];
      *RsvdPcieKiloIo  = PchSetup.PcieIoRsvd[RpIndex];
    }
  } else {
    // Non-Rootport
    DEBUG ((DEBUG_INFO, "GetResourcePadding : Non-Rootport no %02d Bus 0x%x, Device 0x%x, Function 0x%x \n", RpIndex, RpBus, RpDev, RpFunc));
  }

  if (SetupData.EnablePcieTunnelingOverUsb4 == 1) {
    TbtController = GetRootPortToSetResourcesForTbt (&SetupData, RpIndex, PcieRpType);
    if ((SetupData.IntegratedTbtSupport == 1)
        && (PcieRpType == ITBT_CONTROLLER))
    {
      if (TbtController < MAX_ITBT_PCIE_PORT) {
        if (DxeITbtConfig == NULL) {
          DEBUG ((DEBUG_ERROR, "DxeITbtConfig is NULL \n"));
          return;
        }
        *RsvdExtraBusNum    = DxeITbtConfig->ITbtResourceConfig[TbtController].TbtPcieExtraBusRsvd;
        *RsvdPcieMegaMem    = DxeITbtConfig->ITbtResourceConfig[TbtController].TbtPcieMemRsvd;
        *PcieMemAddrRngMax  = DxeITbtConfig->ITbtResourceConfig[TbtController].TbtPcieMemAddrRngMax;
        *RsvdPciePMegaMem   = DxeITbtConfig->ITbtResourceConfig[TbtController].TbtPciePMemRsvd;
        *PciePMemAddrRngMax = DxeITbtConfig->ITbtResourceConfig[TbtController].TbtPciePMemAddrRngMax;
        *RsvdPcieKiloIo     = 0;
      }
    }

#if FixedPcdGetBool (PcdDTbtEnable) == 1
    if ((SetupData.DiscreteTbtSupport == 1) &&
        (PcieRpType == PCIE_RP_TYPE_CPU)) {
      if (TbtController < MAX_DTBT_CONTROLLER_NUMBER) {
        Status = gBS->LocateProtocol(
                        &gDxeDTbtPolicyProtocolGuid,
                        NULL,
                        (VOID **) &DxeDTbtConfig
                        );
        ASSERT_EFI_ERROR (Status);
        if (DxeDTbtConfig == NULL) {
          DEBUG ((DEBUG_ERROR, "DxeDTbtConfig is NULL \n"));
          return;
        }
        *RsvdExtraBusNum    = DxeDTbtConfig->DTbtResourceConfig[TbtController].TbtPcieExtraBusRsvd;
        *RsvdPcieMegaMem    = DxeDTbtConfig->DTbtResourceConfig[TbtController].TbtPcieMemRsvd;
        *PcieMemAddrRngMax  = DxeDTbtConfig->DTbtResourceConfig[TbtController].TbtPcieMemAddrRngMax;
        *RsvdPciePMegaMem   = DxeDTbtConfig->DTbtResourceConfig[TbtController].TbtPciePMemRsvd;
        if (SetupData.DTbthostRouterPortNumber[TbtController] == 2) {
          // For BR Controller only, reserved PciePMem memory is set by port
          // For 2 ports, the memory reservation is twice the value used for one port.
          *RsvdPciePMegaMem = 2 * DxeDTbtConfig->DTbtResourceConfig[TbtController].TbtPciePMemRsvd;
        }
        *PciePMemAddrRngMax = DxeDTbtConfig->DTbtResourceConfig[TbtController].TbtPciePMemAddrRngMax;
        *RsvdPcieKiloIo     = 0;
      }
    }
#endif
  }
}

/**
  Handle the Resource Padding For Non-RootPort

  @param[in] *HpcDevicePath       The Device Path to the HPC that is being initialized.
  @param[in] HpcPciAddress        The address of the Hot Plug Controller function on the PCI bus.
  @param[in] RsvdExtraBusNum      Reserved Bus resource Number.
  @param[in] RsvdPcieMegaMem      Reserved Alignment of MEM resource.
  @param[in] PcieMemAddrRngMax    Alignment of MEM resource.
  @param[in] RsvdPciePMegaMem     Reserved Pcie Kilo Io Number.
  @param[in] PciePMemAddrRngMax   Alignment of PMEM resource.
  @param[in] RsvdPcieKiloIo       Reserved Pcie Kilo Io Number.

**/
VOID
HandleResourcePaddingForNonRootPort (
  IN  EFI_DEVICE_PATH_PROTOCOL        *HpcDevicePath,
  IN  UINT64                          HpcPciAddress,
  OUT UINT8                           *RsvdExtraBusNum,
  OUT UINT16                          *RsvdPcieMegaMem,
  OUT UINT8                           *PcieMemAddrRngMax,
  OUT UINT32                          *RsvdPciePMegaMem,
  OUT UINT8                           *PciePMemAddrRngMax,
  OUT UINT8                           *RsvdPcieKiloIo
  )
{
  UINT8       RpSeg;
  UINTN       RpBus;
  UINTN       RpDev;
  UINTN       RpFunc;
  UINT64      ParentPciAddress;
  EFI_STATUS  Status;
  UINT8       Index;
  UINT8       ConsumedBusNumber;

  RpSeg  = (UINT8) ((HpcPciAddress >> 32) & 0x01);
  RpBus  = (UINTN) ((HpcPciAddress >> 24) & 0xFF);
  RpDev  = (UINTN) ((HpcPciAddress >> 16) & 0xFF);
  RpFunc = (UINTN) ((HpcPciAddress >> 8) & 0xFF);

  // Non-Rootport
  DEBUG ((DEBUG_INFO, "GetResourcePadding : Non-Rootport Seg 0x%x, Bus 0x%x, Device 0x%x, Function 0x%x \n", RpSeg, RpBus, RpDev, RpFunc));

  *RsvdPcieKiloIo  = 0;
  ConsumedBusNumber = 0;

  ParentPciAddress = GetParentPciAddress (RpSeg, RpBus);

  GetParentResourceSize (ParentPciAddress, RsvdExtraBusNum, RsvdPcieMegaMem, RsvdPciePMegaMem, RsvdPcieKiloIo);
  DEBUG ((DEBUG_INFO, "Parent's resource as below \n"));
  DEBUG ((DEBUG_INFO, "BusNum = %x \n", *RsvdExtraBusNum));
  DEBUG ((DEBUG_INFO, "PcieMegaMem = %x \n", *RsvdPcieMegaMem));
  DEBUG ((DEBUG_INFO, "PciePMegaMem = %x \n", *RsvdPciePMegaMem));
  DEBUG ((DEBUG_INFO, "PcieKiloIo = %x \n", *RsvdPcieKiloIo));

  CalculateBusConsumptionBeforeAssign (ParentPciAddress, HpcPciAddress, &ConsumedBusNumber);

  Index = 0;
  Status = GetCurrentBusP2pbHotPlug(RpSeg, (UINT8)RpBus, &Index);
  if (Status == EFI_SUCCESS) {
    // If Reserved Bus >= (UpStream Bus + DownStream Bus + Total Non-HotPlug Bus + Total HotPlug P2pb)
    if (*RsvdExtraBusNum >= (ConsumedBusNumber + mNumOfHotPlugBridge[Index].NumOfP2pbNonHotPlug + mNumOfHotPlugBridge[Index].NumOfP2pbHotPlug)){
      // RsvdExtraBusNum = (Reserved Bus - UpStream Bus - DownStream Bus - Total Non-HotPlug Bus) / Total HotPlug P2pb
      if (mNumOfHotPlugBridge[Index].NumOfP2pbHotPlug != 0) {
        *RsvdExtraBusNum = (UINT8) DivU64x32Remainder (((UINT64) (*RsvdExtraBusNum) - ConsumedBusNumber - mNumOfHotPlugBridge[Index].NumOfP2pbNonHotPlug), (UINT32)mNumOfHotPlugBridge[Index].NumOfP2pbHotPlug, &mRemainderBus);
      }

      // If mRemainderBus not equal zero, check this device is the last one or not.
      if (mRemainderBus != 0) {
        DEBUG((DEBUG_INFO, "Remainder Bus = %x in the Seg:%x Bus:%x layer\n", mRemainderBus, RpSeg, RpBus));
        if (IsLastP2pbDevice(HpcPciAddress)) {
          DEBUG((DEBUG_INFO, "This device is the last P2pb in the same layer, so we need to add Remainder Bus\n"));
          *RsvdExtraBusNum = (UINT8)(*RsvdExtraBusNum + mRemainderBus);
        }
      }
    // If Reserved Bus < (UpStream Bus + DownStream Bus + Total Non-HotPlug Bus + Total HotPlug P2pb), stop allocation and return RsvdExtraBusNum = 0
    } else {
      *RsvdExtraBusNum  = 0;
    }
    *RsvdPcieMegaMem = (*RsvdPcieMegaMem - mNumOfHotPlugBridge[Index].NumOfP2pbNonHotPlug)/mNumOfHotPlugBridge[Index].NumOfP2pbHotPlug;
    *RsvdPciePMegaMem = (*RsvdPciePMegaMem)/mNumOfHotPlugBridge[Index].NumOfP2pbHotPlug;
    *RsvdPcieKiloIo = (*RsvdPcieKiloIo)/mNumOfHotPlugBridge[Index].NumOfP2pbHotPlug;
    DEBUG ((DEBUG_INFO, "Current resource as below \n"));
    DEBUG ((DEBUG_INFO, "BusNum = %x \n", *RsvdExtraBusNum));
    DEBUG ((DEBUG_INFO, "PcieMegaMem = %x \n", *RsvdPcieMegaMem));
    DEBUG ((DEBUG_INFO, "PciePMegaMem = %x \n", *RsvdPciePMegaMem));
    DEBUG ((DEBUG_INFO, "PcieKiloIo = %x \n", *RsvdPcieKiloIo));
  } else {
    DEBUG ((DEBUG_INFO, "GetCurrentBusP2pbHotPlug Status =%r\n", Status));
  }
}

/**
  Returns the resource padding required by the PCI bus that is controlled by the specified Hot Plug Controller.

  @param[in]  This           The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol. initialized.
  @param[in]  HpcDevicePath  The Device Path to the Hot Plug Controller.
  @param[in]  HpcPciAddress  The address of the Hot Plug Controller function on the PCI bus.
  @param[out] HpcState       The state of the Hot Plug Controller hardware. The type EFI_HPC_STATE is defined in section 3.1.
  @param[out] Padding        This is the amount of resource padding required by the PCI bus under the control of the specified Hpc. Since the caller does not know the size of this buffer, this buffer is allocated by the callee and freed by the caller.
  @param[out] Attribute      Describes how padding is accounted for.

  @retval     EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
GetResourcePadding (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL  *This,
  IN  EFI_DEVICE_PATH_PROTOCOL        *HpcDevicePath,
  IN  UINT64                          HpcPciAddress,
  OUT EFI_HPC_STATE                   *HpcState,
  OUT VOID                            **Padding,
  OUT EFI_HPC_PADDING_ATTRIBUTES      *Attributes
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *PaddingResource;
  EFI_STATUS                        Status;
  UINT8                             RsvdExtraBusNum = 0;
  UINT16                            RsvdPcieMegaMem = 10;
  UINT8                             PcieMemAddrRngMax = 1;
  UINT32                            RsvdPciePMegaMem = 10;
  UINT8                             PciePMemAddrRngMax = 1;
  UINT8                             RsvdPcieKiloIo = 4;
  UINTN                             RpBus;
  CHAR16                            *DevicePathStr;

  PaddingResource = AllocatePool (PADDING_NUM * sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR));
  ASSERT (PaddingResource != NULL);
  if (PaddingResource == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *Padding = (VOID *) PaddingResource;

  // Record the segment number in Bit[32] of HpcPciAddress.
  DevicePathStr = ConvertDevicePathToText (HpcDevicePath, TRUE, TRUE);
  if (DevicePathStr != NULL) {
    if (StrnCmp (DevicePathStr, PCI_ROOT_0_STRING, StrLen (PCI_ROOT_0_STRING)) == 0) {
      DEBUG ((DEBUG_INFO, "Segment0 \n"));
      HpcPciAddress &= ~BIT32;
    } else if (StrnCmp (DevicePathStr, PCI_ROOT_1_STRING, StrLen (PCI_ROOT_1_STRING)) == 0) {
      DEBUG ((DEBUG_INFO, "Segment1 \n"));
      HpcPciAddress |= BIT32;
    }
  } else {
      return EFI_OUT_OF_RESOURCES;
  }

  RpBus  = (UINTN) ((HpcPciAddress >> 24) & 0xFF);

  if (RpBus == 0x00) {
    //
    // Handle the GetResourcePadding for RootPort
    //
    HandleResourcePaddingForRootPort (
      HpcDevicePath,
      HpcPciAddress,
      &RsvdExtraBusNum,
      &RsvdPcieMegaMem,
      &PcieMemAddrRngMax,
      &RsvdPciePMegaMem,
      &PciePMemAddrRngMax,
      &RsvdPcieKiloIo
    );
  } else {
    //
    // Handle the GetResourcePadding for Non-RootPort
    //
    HandleResourcePaddingForNonRootPort (
      HpcDevicePath,
      HpcPciAddress,
      &RsvdExtraBusNum,
      &RsvdPcieMegaMem,
      &PcieMemAddrRngMax,
      &RsvdPciePMegaMem,
      &PciePMemAddrRngMax,
      &RsvdPcieKiloIo
    );
  }

  Status = UpdateP2pbResourceRecord (
    HpcDevicePath,
    HpcPciAddress,
    RsvdExtraBusNum,
    RsvdPcieMegaMem,
    PcieMemAddrRngMax,
    RsvdPciePMegaMem,
    PciePMemAddrRngMax,
    RsvdPcieKiloIo
    );
  if (Status == EFI_SUCCESS) {
    mP2pbResourceRecordCount ++;
  }

  //
  // Padding for bus
  //
  ZeroMem (PaddingResource, PADDING_NUM * sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR));
  *Attributes                   = EfiPaddingPciBus;

  PaddingResource->Desc         = 0x8A;
  PaddingResource->Len          = 0x2B;
  PaddingResource->ResType      = ACPI_ADDRESS_SPACE_TYPE_BUS;
  PaddingResource->GenFlag      = 0x0;
  PaddingResource->SpecificFlag = 0;
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrRangeMax = 0;
  PaddingResource->AddrLen      = RsvdExtraBusNum - 1; // Subordinate Bus Number = Secondary Bus Number + Reserved Bus Number, but Secondary Bus Number already consumed 1 bus number itself.

  //
  // Padding for non-prefetchable memory
  //
  PaddingResource++;
  PaddingResource->Desc                 = 0x8A;
  PaddingResource->Len                  = 0x2B;
  PaddingResource->ResType              = ACPI_ADDRESS_SPACE_TYPE_MEM;
  PaddingResource->GenFlag              = 0x0;

  PaddingResource->AddrSpaceGranularity = 64;
  PaddingResource->SpecificFlag         = 0;
  //
  // Pad non-prefetchable
  //
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrLen      = (UINT64) RsvdPcieMegaMem * 0x100000;
  PaddingResource->AddrRangeMax = (1 << PcieMemAddrRngMax) - 1;

  //
  // Padding for prefetchable memory
  //
  PaddingResource++;
  PaddingResource->Desc                 = 0x8A;
  PaddingResource->Len                  = 0x2B;
  PaddingResource->ResType              = ACPI_ADDRESS_SPACE_TYPE_MEM;
  PaddingResource->GenFlag              = 0x0;
  PaddingResource->AddrSpaceGranularity = 64;
  PaddingResource->SpecificFlag         = 06;
  //
  // Padding for prefetchable memory
  //
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrLen      = (UINT64) RsvdPciePMegaMem * 0x100000;
  //
  // Pad 16 MB of MEM
  //
  PaddingResource->AddrRangeMax = (1 << PciePMemAddrRngMax) - 1;

  //
  // Alignment
  //
  // Padding for I/O
  //
  PaddingResource++;
  PaddingResource->Desc         = 0x8A;
  PaddingResource->Len          = 0x2B;
  PaddingResource->ResType      = ACPI_ADDRESS_SPACE_TYPE_IO;
  PaddingResource->GenFlag      = 0x0;
  PaddingResource->SpecificFlag = 0;
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrLen      = RsvdPcieKiloIo * 0x400;
  //
  // Pad 4K of IO
  //
  PaddingResource->AddrRangeMax = 1;
  //
  // Alignment
  //
  // Terminate the entries.
  //
  PaddingResource++;
  ((EFI_ACPI_END_TAG_DESCRIPTOR *) PaddingResource)->Desc     = ACPI_END_TAG_DESCRIPTOR;
  ((EFI_ACPI_END_TAG_DESCRIPTOR *) PaddingResource)->Checksum = 0x0;

  *HpcState = EFI_HPC_STATE_INITIALIZED | EFI_HPC_STATE_ENABLED;

  return EFI_SUCCESS;
}
