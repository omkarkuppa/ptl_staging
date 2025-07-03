/** @file
  ITBT init Dxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/TbtCommonLib.h>
#include <ITbtInfoHob.h>
#include <Protocol/ITbtPolicy.h>
#include <Protocol/ITbtNvsArea.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi/UefiSpec.h>
#include <Library/PcdLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/PcdLib.h>
#include <Library/DxeTbtDisBmeLib.h>
#include <Library/ItbtPcieRpLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/TcssInfoLib.h>
#include <Protocol/TbtDisBmeProtocol.h>
#include <Protocol/PlatformTseExcludeProtocol.h>
#include <MeState.h>
#include <MkhiMsgs.h>
#include <MeBiosPayloadHob.h>
#include <TcssDataHob.h>
#include <TcssInfo.h>
#include <Register/HostDmaRegs.h>
#include <Register/ItbtPcieRegs.h>

GLOBAL_REMOVE_IF_UNREFERENCED ITBT_NVS_AREA_PROTOCOL                    mITbtNvsAreaProtocol;

/**
  DisableITbtBmeCallBackFunction

  Disable BME on iTBT tree

  @param[in] Event     - @todo Unused and needs to be removed from next platform
  @param[in] Context   - @todo Unused and needs to be removed from next platform
**/
VOID
EFIAPI
DisableITbtBmeCallBackFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                Status;
  UINT8                     Index;
  SBDF                      Sbdf = {0,0,0,0,0};
  UINTN                     RpDev;
  UINTN                     RpFunc;
  UINTN                     RpSegment;
  UINTN                     RpBus;

  DEBUG((DEBUG_INFO, "[TBT] Disable iTBT BME Start\n"));

  Status      = EFI_SUCCESS;
  Index       = 0;
  RpDev       = 0;
  RpFunc      = 0;
  RpSegment   = 0;
  RpBus       = 0;

  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
      Status = GetItbtPcieRpInfo ((UINTN) Index, &RpSegment, &RpBus, &RpDev, &RpFunc);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Failed to get iTBT PCIe root port[%d] info, Status: %r\n", Index, Status));
        continue;
      }

      Sbdf.Dev = (UINT32) RpDev;
      Sbdf.Func = (UINT32) RpFunc;
      Sbdf.Bus = (UINT32) RpBus;
      Sbdf.Seg = (UINT32) RpSegment;
      DEBUG ((DEBUG_INFO, "iTBT Root Port: %02x:%02x:%02x:%02x\n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
      RecursiveTbtHierarchyConfiguration (Sbdf);
  }

  DEBUG((DEBUG_INFO, "[TBT] Disable iTBT BME End\n"));
}

EFI_DISABLE_TBT_BME_PROTOCOL mDisableITbtBmeProtocol = {
  DisableITbtBmeCallBackFunction,
};

/**
  The function install DisableBme protocol for iTBT Shell validation
**/
VOID
InstallITbtDisableBmeProtocol (
  VOID
  )
{
  EFI_STATUS        Status;
  EFI_HANDLE        Handle;

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gDisableITbtBmeProtocolGuid,
                  &mDisableITbtBmeProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ITbtDisableBmeProtocol Failed. Status: %d\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "ITbtDisableBmeProtocol Installed\n"));
  }
}

/**
  Any NVMe device subjected to runtime BDF change is not supported by TSE design currently.
  The function is for checking if the device is under a hotplug able daisy chain, such as TBT
  daisy chain typically.

  @param[in]   This               The Platform TSE Exclude Protocol Instance.
  @param[in]   PciIo              Target Device PciIo Protocol
  @param[out]  IsTseSupported     A pointer to a Boolean determining TSE is supported or not

  @retval  EFI_SUCCESS            Successfully check whether device is under TBT daisy chain.
  @retval  EFI_NOT_FOUND          Failed to get DTbtInfoHob.
  @retval  EFI_INVALID_PARAMETER  Invalid parameter passed in.

**/
EFI_STATUS
EFIAPI
PlatformITbtTseExcludeCheck (
  IN   PLATFORM_TSE_EXCLUDE_PROTOCOL  *This,
  IN   EFI_PCI_IO_PROTOCOL            *PciIo,
  OUT  BOOLEAN                        *IsTseSupported
  )
{
  EFI_STATUS     Status;
  UINTN          TargetSegment;
  UINTN          TargetBus;
  UINTN          TargetDevice;
  UINTN          TargetFunction;
  UINT8          Index;
  UINTN          RpSegment;
  UINTN          RpBus;
  UINTN          RpDev;
  UINTN          RpFunc;
  UINT8          SecondaryBus;
  UINT8          SubordinateBus;

  if ((This == NULL) || (PciIo == NULL) || (IsTseSupported == NULL)) {
    DEBUG ((DEBUG_ERROR, "Invalid parameter\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = PciIo->GetLocation (PciIo, &TargetSegment, &TargetBus, &TargetDevice, &TargetFunction);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *IsTseSupported = TRUE;

  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    Status = GetItbtPcieRpInfo ((UINTN) Index, &RpSegment, &RpBus, &RpDev, &RpFunc);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to get iTBT PCIe root port[%d] info, Status: %r\n", Index, Status));
      continue;
    }

    SecondaryBus   = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDev, RpFunc, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
    SubordinateBus = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDev, RpFunc, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
    if ((SecondaryBus <= TargetBus) && (TargetBus <= SubordinateBus)) {
      DEBUG ((DEBUG_INFO, "Bus:0x%x Device:0x%x Function:0x%x is under iTBT device, not supported with TSE\n", TargetBus, TargetDevice, TargetFunction));
      *IsTseSupported = FALSE;
    }
  }

  return EFI_SUCCESS;
}

PLATFORM_TSE_EXCLUDE_PROTOCOL mPlatformITbtTseExcludeProtocol = {
  PlatformITbtTseExcludeCheck
};

/**
  The function installs Platform TSE Exclude protocol for iTBT check

**/
VOID
EFIAPI
InstallPlatformITbtTseExcludeProtocol (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS        Status;
  EFI_HANDLE        Handle;

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gPlatformTseExcludeProtocolGuid,
                  &mPlatformITbtTseExcludeProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to Install Platform TSE Exclude Protocol for iTBT with Status = %r\n", __FUNCTION__, Status));
  } else {
    DEBUG ((DEBUG_INFO, "gPlatformTseExcludeProtocolGuid Installed Successfully for iTBT\n"));
  }
}

/**
  ITBT NVS Area Initialize
**/
VOID
ITbtNvsAreaInit (
  IN  VOID              **mITbtNvsAreaPtr
  )
{
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  EFI_STATUS                    Status;
  UINT8                         Index;
  ITBT_NVS_AREA_PROTOCOL        *ITbtNvsAreaProtocol;
  ITBT_POLICY_PROTOCOL          *ITbtPolicy;
  DXE_ITBT_CONFIG               *DxeITbtConfig;
  TCSS_DATA_HOB                 *TcssHob;
  ITBT_INFO_HOB                 *ITbtInfoHob;
  UINT32                        MeMode;

  ITbtNvsAreaProtocol = NULL;
  ITbtPolicy = NULL;
  DxeITbtConfig = NULL;
  TcssHob = NULL;
  ITbtInfoHob = NULL;

  DEBUG ((DEBUG_INFO, "ITbtNvsAreaInit Start\n"));

  Status = gBS->LocateProtocol (&gITbtPolicyProtocolGuid, NULL, (VOID **) &ITbtPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) ITbtPolicy, &gDxeITbtConfigGuid, (VOID *)&DxeITbtConfig);
  ASSERT_EFI_ERROR (Status);

  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  ITbtInfoHob = (ITBT_INFO_HOB *) GetFirstGuidHob (&gITbtInfoHobGuid);

  Pages = EFI_SIZE_TO_PAGES (sizeof (ITBT_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);

  *mITbtNvsAreaPtr = (VOID *) (UINTN) Address;
  SetMem (*mITbtNvsAreaPtr, sizeof (ITBT_NVS_AREA), 0);

  //
  // ITBTNvsAreaProtocol default value init here
  //
  ITbtNvsAreaProtocol = (ITBT_NVS_AREA_PROTOCOL *) &Address;

  //
  // Initialize default values for ITBT NVS
  //
  ITbtNvsAreaProtocol->Area->OsNativeResourceBalance = DxeITbtConfig->OsNativeResourceBalance;
  ITbtNvsAreaProtocol->Area->ITbtRtd3              = (UINT8) DxeITbtConfig->ITbtGenericConfig.ITbtRtd3;
  ITbtNvsAreaProtocol->Area->ITbtRtd3ExitDelay     = (UINT16) DxeITbtConfig->ITbtGenericConfig.ITbtRtd3ExitDelay;
  ITbtNvsAreaProtocol->Area->PcieRtd3LinkActiveTimeout = (UINT16) DxeITbtConfig->PcieRtd3LinkActiveTimeout;

  if (ITbtInfoHob != NULL) {
    for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index ++) {
      if (ITbtInfoHob->ITbtRootPortConfig[Index].ITbtPcieRootPortEn == 1) {
        ITbtNvsAreaProtocol->Area->IntegratedTbtSupport = 1;
        break;
      }
    }
    ITbtNvsAreaProtocol->Area->ITbtPcieRootPortEn0   = ITbtInfoHob->ITbtRootPortConfig[0].ITbtPcieRootPortEn;
    ITbtNvsAreaProtocol->Area->ITbtPcieRootPortEn1   = ITbtInfoHob->ITbtRootPortConfig[1].ITbtPcieRootPortEn;
    ITbtNvsAreaProtocol->Area->ITbtPcieRootPortEn2   = ITbtInfoHob->ITbtRootPortConfig[2].ITbtPcieRootPortEn;
    ITbtNvsAreaProtocol->Area->ITbtPcieRootPortEn3   = ITbtInfoHob->ITbtRootPortConfig[3].ITbtPcieRootPortEn;
    ITbtNvsAreaProtocol->Area->ITbtForcePowerOnTimeoutInMs = ITbtInfoHob->ITbtForcePowerOnTimeoutInMs;
    ITbtNvsAreaProtocol->Area->ITbtConnectTopologyTimeoutInMs = ITbtInfoHob->ITbtConnectTopologyTimeoutInMs;
  }
  if (TcssHob != NULL) {
    ITbtNvsAreaProtocol->Area->IntegratedTbtDma0   = TcssHob->TcssData.ItbtDmaEn[0];
    ITbtNvsAreaProtocol->Area->IntegratedTbtDma1   = TcssHob->TcssData.ItbtDmaEn[1];
    ITbtNvsAreaProtocol->Area->ITbtImrValid        = (UINT8) ((TcssHob->TcssData.TbtImrStatus.Bits.Valid) & (TcssHob->TcssData.TbtImrStatus.Bits.Done));
  }

  if (DxeITbtConfig->ITbtGenericConfig.ITbtVproDockSupport == 1) {
    Status = MeBiosGetMeMode (&MeMode);
    if (!EFI_ERROR (Status) && MeMode == ME_MODE_NORMAL) {
      ITbtNvsAreaProtocol->Area->ITbtVproDockSupport = 1;
    }
  }

  DEBUG ((DEBUG_INFO, "ITbtNvsAreaInit End\n"));
}

/**
  This function gets registered as a callback to patch TBT ASL code

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
ITbtAcpiEndOfDxeCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                            Status;
  UINT32                                Address;
  UINT16                                Length;
  UINT64                                SsdtOemTableId;

  SsdtOemTableId = SIGNATURE_64 ('T','c','s','s','S','s','d','t');

  Address = (UINT32) (UINTN) mITbtNvsAreaProtocol.Area;
  Length  = (UINT16) sizeof (ITBT_NVS_AREA);
  DEBUG ((DEBUG_INFO, "Patch ITBT NvsAreaAddress: ITBT NVS Address %x Length %x\n", Address, Length));
  Status = UpdateSsdtNameAslCode ((UINT8*) &SsdtOemTableId, sizeof (SsdtOemTableId), SIGNATURE_32 ('I','T','N','B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status = UpdateSsdtNameAslCode ((UINT8*) &SsdtOemTableId, sizeof (SsdtOemTableId), SIGNATURE_32 ('I','T','N','L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);

  return;
}

/**
  This function gets registered as a callback on End of DXE for iTBT

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
ITbtEndOfDxeCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  ITbtAcpiEndOfDxeCallback (Event, Context);

  gBS->CloseEvent (Event);
}

/**
  This function clears some of PCIe errors in TCSS PCIe root ports when detected.
**/
VOID
ClearTcPcieRpErrors (
  VOID
  )
{
  SBDF   PcieRpSbdf;
  UINT8  RpIndex;

  DEBUG((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  ZeroMem (&PcieRpSbdf, sizeof (SBDF));
  for (RpIndex = 0; RpIndex < MAX_ITBT_PCIE_PORT; RpIndex++) {
    PcieRpSbdf.Seg  = SA_SEG_NUM;
    PcieRpSbdf.Bus  = ITBT_PCIE_BUS_NUM;
    PcieRpSbdf.Dev  = (UINT32) GetITbtPcieDevNumber ();
    PcieRpSbdf.Func = RpIndex;
    ClearPcieRpErrors (PcieRpSbdf);
  }
}

/**
  ITbtExitBootServiceCallback

  This is a callback function triggered by gEfiEventExitBootServicesGuid for iTBT.
**/
VOID
EFIAPI
ITbtExitBootServiceCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  //
  // Disable BME on iTBT tree at ExitBootServices to hand off security of TBT hierarchies to OS.
  //
  DisableITbtBmeCallBackFunction (Event, Context);
  if (IsGlobalPcieAerEnabled () == TRUE) {
    ClearTcPcieRpErrors ();
  }

  gBS->CloseEvent (Event);
}

/**
  Initialize Thunderbolt(TM) SSDT ACPI tables

  @retval EFI_SUCCESS    ACPI tables are initialized successfully
  @retval EFI_NOT_FOUND  ACPI tables not found
**/

EFI_STATUS
EFIAPI
ITbtDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle;
  EFI_EVENT               EndOfDxeEvent;
  EFI_EVENT               ExitBootServiceEvent;
  DXE_ITBT_CONFIG         *DxeITbtConfig;
  ITBT_POLICY_PROTOCOL    *ITbtPolicy;
  BOOLEAN                 ITbtExisted;
  UINT8                   Index;
  VOID                    *Registration;

  Status                  = EFI_SUCCESS;
  Handle                  = NULL;
  EndOfDxeEvent           = NULL;
  ExitBootServiceEvent    = NULL;
  DxeITbtConfig           = NULL;
  ITbtPolicy              = NULL;
  ITbtExisted             = FALSE;

  DEBUG ((DEBUG_INFO, "[TBT] ITbtDxeEntryPoint START\n"));

  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index ++) {
    if (0xFFFF != PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (0, HOST_DMA_BUS_NUM, HOST_DMA_DEV_NUM, HOST_DMA0_FUN_NUM + Index, PCI_VENDOR_ID_OFFSET))) {
      ITbtExisted = TRUE;
    }
  }

  if (ITbtExisted == FALSE) {
    DEBUG ((DEBUG_ERROR, "ITBT DMA controller is not existed\n"));
    goto Exit;
  }

  //
  // Initialize ITbt Nvs Area
  //
  ITbtNvsAreaInit ((VOID **) &mITbtNvsAreaProtocol.Area);

  //
  // [ACPI] ITBT ACPI table
  //

  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gITbtNvsAreaProtocolGuid,
                  &mITbtNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register an end of DXE event for iTBT
  // To patch TBT ASL code and install TSE Exclude protocol
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ITbtEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Register a End Of Dxe Event for iTBT, Status: %r\n", Status));
    gBS->CloseEvent (EndOfDxeEvent);
    goto Exit;
  }

  Status = gBS->LocateProtocol (
              &gITbtPolicyProtocolGuid,
              NULL,
              (VOID **) &ITbtPolicy
              );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get ITbtPolicy, Status: %d\n", Status));
    goto Exit;
  }

  Status = GetConfigBlock ((VOID *) ITbtPolicy, &gDxeITbtConfigGuid, (VOID *)&DxeITbtConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get DxeITbtConfig, Status: %d\n", Status));
    goto Exit;
  }

  //
  // Register a Exit Boot Service callback function for iTBT
  //
  DEBUG ((DEBUG_INFO, "Register a Exit Boot Service callback function for iTBT\n"));

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ITbtExitBootServiceCallback,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &ExitBootServiceEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Register a Exit Boot Service callback function for iTBT, Status: %d\n", Status));
    gBS->CloseEvent (ExitBootServiceEvent);
    goto Exit;
  }

  //
  // Install iTBT Disable Bme protocol for Shell testing purpose
  //
  InstallITbtDisableBmeProtocol ();

  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    InstallPlatformITbtTseExcludeProtocol,
    NULL,
    &Registration
    );

Exit:
  DEBUG ((DEBUG_INFO, "[TBT] ITbtDxeEntryPoint END\n"));
  return Status;
}
