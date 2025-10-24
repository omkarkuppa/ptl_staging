/** @file
  Initializes both PCH Host and Device USB Controllers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include "PtlPcdInitPei.h"

#include <Register/PchRegs.h>
#include <Register/UsbHostRegs.h>

#include <IpWrapperCntxtInfoClient.h>
#include <UsbConfig.h>
#include <IpUsb3.h>

#include <Library/UsbDeviceControllerInitLib.h>
#include <IntelRcStatusCode.h>
#include <Library/PchPciBdfLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PcdLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <Library/MmioAccessLib.h>
#include <Library/DciPrivateLib.h>
#include <Library/PchFiaLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/TimerLib.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdFiaSocLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <PcdSbPortIds.h>
#include <Register/GpioAcpiDefines.h>
#include <Library/GpioHelpersLib.h>

/**
  Configures GPIO pins for USB OverCurrent detection

  @param[in]  pInst             IP_USB3_INST structure pointer
  @param[in]  OvercurrentPin    Index of OverCurrent Pin to be enabled
  @param[in]  SbVwEnable        Flag to enable sideband messaging for physical OC pins in TCSS
**/
VOID
UsbEnableOvercurrentPin (
  IP_USB3_INST *pInst,
  UINT8        OverCurrentPin,
  BOOLEAN      SbVwEnable
  )
{
  GPIOV2_SERVICES  *GpioServices;
  EFI_STATUS       Status;

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    return;
  }

  PtlPcdGpioEnableUsbOverCurrent (GpioServices, OverCurrentPin, SbVwEnable);
}

/**
  Checks in FIA is given lane is USB owned

  @param[in]  UsbHandle         Pointer to USB_HANDLE instance
  @param[in]  LaneIndex         Index of lane to check
**/
BOOLEAN
IsFiaLaneOwnedByUsb (
  IP_USB3_INST  *pInst,
  UINT32        LaneNumber
  )
{
  return PtlPcdFiaIsUsb3PortConnected (LaneNumber);
}

/**
  Disables USB Device Controller
  @param[in]  UsbHandle         Pointer to USB_HANDLE instance
**/
VOID
UsbDeviceControllerDisable (
  IN    USB_HANDLE *UsbHandle
  )
{
  // Disable xDCI function.
  PtlPcdPsfDisableXdci ();
}

/**
  Delays execution for desired amount of us (microseconds)
  @param[in]  UsbHandle         Pointer to USB_HANDLE instance
**/
VOID
UsbDelayForUs (
  IN    USB_HANDLE        *UsbHandle,
  IN    UINT32            MicroSeconds
  )
{
  MicroSecondDelay (MicroSeconds);
}

/**
  Initializes USB controllers handle for both Host and Device controllers
  along with required callbacks for cross IP dependencies

  @param[in]  SiPolicy            The Silicon Policy PPI instance
  @param[out] UsbHandle           USB_HANDLE structure
  @param[out] UsbPrivateConfig    USB_PRIVATE_CONFIG structure
  @param[out] UsbCallback         USB_CALLBACK structure
**/
VOID
CreateUsbHandle (
  IN      SI_POLICY_PPI      *SiPolicy,
  IN OUT  USB_HANDLE         *UsbHandle,
  IN OUT  USB_PRIVATE_CONFIG *UsbPrivateConfig,
  IN OUT  USB_CALLBACK       *UsbCallback
  )
{
  EFI_STATUS  Status;
  USB_CONFIG  *UsbConfig;

  DEBUG ((DEBUG_VERBOSE, "%a entry\n", __FUNCTION__));

  // Create private config values
  UsbPrivateConfig->IsPortResetMessagingSupported = TRUE;
  UsbPrivateConfig->IpVersion = V19_3;
  UsbPrivateConfig->Location  = Standalone;

  switch (PchStepping ()) {
    case PCH_A0:
    default:
      UsbPrivateConfig->Stepping = A0;
      break;
  }

  UsbHandle->PrivateConfig = UsbPrivateConfig;

  // Populate callbacks pointers
  UsbCallback->UsbDeviceControllerDisable = UsbDeviceControllerDisable;
  UsbCallback->DelayForUs = UsbDelayForUs;
  UsbHandle->Callback = UsbCallback;

  // Set config pointer
  Status = GetConfigBlock ((VOID *)SiPolicy, &gUsbConfigGuid, (VOID *)&UsbConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - cannot get USB config block.\n", __FUNCTION__));
    UsbConfig = NULL;
  }

  UsbHandle->UsbConfig = UsbConfig;
}

/**
  Creates USB Host controller handle to pass to xHCI library with all necessary
  data for proper controller initialization

  @param[in]    PcieConfigAccess    PCIe Config Space access
  @param[in]    MmioAccess          Memory Mappied IO access
  @param[in]    SbMmioAccess        Sideband MMIO Access protocol
  @param[in]    P2SbController      Primary to Sideband controller
  @param[out]   UsbController       Instance of UsbController
**/
STATIC
VOID
CreateUsbDeviceController (
  IN  PCI_CONFIG_SPACE_ACCESS         *PcieConfigAccess,
  IN  MMIO_ACCESS                     *MmioAccess,
  IN  P2SB_SIDEBAND_REGISTER_ACCESS   *SbMmioAccess,
  IN  P2SB_CONTROLLER                 *P2SbController,
  OUT USB_CONTROLLER                  *UsbController
  )
{
  EFI_STATUS      Status;
  P2SB_PORT_16_ID Pid = { .Pid16bit = PTL_SID_F2_PID_XDCI };

  // PCIe config space access
  PciConfigAccessInit (
    DEFAULT_PCI_SEGMENT_NUMBER_PCH,
    DEFAULT_PCI_BUS_NUMBER_PCH,
    PchXdciDevNumber (),
    PchXdciFuncNumber (),
    PcieConfigAccess
    );
  UsbController->PcieConfigAccess = (REGISTER_ACCESS *) PcieConfigAccess;

  // Memory Mapped IO access
  MmioAccessInit (PcdGet32 (PcdSiliconInitTempMemBaseAddr), MmioAccess);
  UsbController->MmioAccess = (REGISTER_ACCESS *) MmioAccess;
  UsbController->ReservedMmio = PcdGet32 (PcdSiliconInitTempMemBaseAddr);

  // Get Sideband bridge data
  Status = PtlPcdGetP2SbController (P2SbController, Pid);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - error while getting P2Sb controller - %r\n", __FUNCTION__, Status));
    return;
  }

  BuildP2SbSidebandAccess (
    P2SbController,
    Pid.PortId.LocalPid,
    0,
    P2SbPrivateConfig,
    P2SbMmioAccess,
    FALSE,
    SbMmioAccess
    );
  UsbController->SbMmioAccess = (REGISTER_ACCESS *) SbMmioAccess;
}

/**
  Creates a Pcie Config Access Context for IP Wrapper Register Access

  @param[in,out]  PcieConfigAccessCntxt   Pointer to context structure
  @param[in]      Seg                     PCIe device segment
  @param[in]      Bus                     PCIe device Bus number
  @param[in]      Dev                     PCIe device Device number
  @param[in]      Fun                     PCIe device Function number
**/
STATIC
VOID
CreatePcieConfigAccessContext (
  IP_WR_REG_INFO *PcieConfigAccessCntxt,
  UINT8          Seg,
  UINT8          Bus,
  UINT8          Dev,
  UINT8          Fun
  )
{
  DEBUG ((DEBUG_VERBOSE, "%a entry\n", __FUNCTION__));

  PcieConfigAccessCntxt->AccessType    = IpWrRegAccessCfg;
  PcieConfigAccessCntxt->AccessSubType = IpWrRegSubTypeNone;
  PcieConfigAccessCntxt->OverrideAccessType = IpWrRegAccessDefault;
  PcieConfigAccessCntxt->RegType.Pci.Seg    = Seg;
  PcieConfigAccessCntxt->RegType.Pci.Bus    = Bus;
  PcieConfigAccessCntxt->RegType.Pci.Dev    = Dev;
  PcieConfigAccessCntxt->RegType.Pci.Fun    = Fun;
}

/**
  Creates a MMIO (Memory Mapped IO) Access Context for IP Wrapper Register Access

  @param[in,out]  MmioAccessCntxt    Pointer to context structure
  @param[in]      MmioBar            MMIO Bar
**/
STATIC
VOID
CreateMmioAccessContext (
  IP_WR_REG_INFO *MmioAccessCntxt,
  UINT64         MmioBar
  )
{
  DEBUG ((DEBUG_VERBOSE, "%a entry\n", __FUNCTION__));

  MmioAccessCntxt->AccessType    = IpWrRegAccessMem;
  MmioAccessCntxt->AccessSubType = IpWrRegSubTypeNone;
  MmioAccessCntxt->OverrideAccessType = IpWrRegAccessDefault;
  MmioAccessCntxt->RegType.MmioBase   = MmioBar;
}

/**
  Create a SbMmioAccess Context for IP Wrapper to consume

  @param  SbMmioAccessCntxt   Context to initialize
  @param  Pid                 Sideband Port ID

  @retval EFI_SUCCESS           - Completed successfully
          EFI_OUT_OF_RESOURCES  - Not enough resources to allocate P2SbCtrl for the context.
          EFI_INVALID_PARAMETER - P2SbController NULL or wrong Segment
**/
STATIC
EFI_STATUS
CreateSbMmioAccessContext (
  IP_WR_REG_INFO  *SbMmioAccessCntxt,
  P2SB_PORT_16_ID Pid
  )
{
  EFI_STATUS       Status;
  P2SB_CONTROLLER  P2SbController;

  DEBUG ((DEBUG_VERBOSE, "%a entry\n", __FUNCTION__));

  Status = PtlPcdGetP2SbController (&P2SbController, Pid);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SbMmioAccessCntxt->AccessType    = IpWrRegAccessSb;
  SbMmioAccessCntxt->AccessSubType = IpWrRegAccessP2Sb;
  SbMmioAccessCntxt->RegType.P2Sb.AccessMethod = IpWrP2SbMmioAccess;
  SbMmioAccessCntxt->RegType.P2Sb.P2SbPid = (IP_WR_P2SB_PID)Pid.PortId.LocalPid;
  SbMmioAccessCntxt->RegType.P2Sb.RegisterSpace = IpWrP2SbPrivateConfig;
  SbMmioAccessCntxt->RegType.P2Sb.P2SbCtrl = (IP_WR_P2SB_CONTROLLER *)AllocateZeroPool (sizeof (IP_WR_P2SB_CONTROLLER));
  if (SbMmioAccessCntxt->RegType.P2Sb.P2SbCtrl == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SbMmioAccessCntxt->RegType.P2Sb.P2SbCtrl->PciCfgBaseAddr = P2SbController.PciCfgBaseAddr;
  SbMmioAccessCntxt->RegType.P2Sb.P2SbCtrl->Mmio = P2SbController.Mmio;
  SbMmioAccessCntxt->RegType.P2Sb.P2SbCtrl->P2sb20bPcrSupported = P2SbController.P2sb20bPcrSupported;

  return EFI_SUCCESS;
}

/**
  Create a DCI SbMmioAccess Context for IP Wrapper to consume

  @param  DciSbMmioAccessCntxt   Context to initialize
  @param  Pid                    Sideband Port ID

  @retval EFI_SUCCESS           - Completed successfully
          EFI_OUT_OF_RESOURCES  - Not enough resources to allocate P2SbCtrl for the context.
          EFI_INVALID_PARAMETER - P2SbController NULL or wrong Segment
**/
STATIC
EFI_STATUS
CreateDciSbMmioAccessContext (
  IP_WR_REG_INFO  *DciSbMmioAccessCntxt,
  P2SB_PORT_16_ID Pid
  )
{
  EFI_STATUS       Status;
  P2SB_CONTROLLER  *P2SbController;

  DEBUG ((DEBUG_VERBOSE, "%a entry\n", __FUNCTION__));

  DciSbMmioAccessCntxt->AccessType    = IpWrRegAccessSb;
  DciSbMmioAccessCntxt->AccessSubType = IpWrRegAccessP2Sb;
  DciSbMmioAccessCntxt->RegType.P2Sb.P2SbPid = (IP_WR_P2SB_PID)Pid.PortId.LocalPid;
  DciSbMmioAccessCntxt->RegType.P2Sb.P2SbCtrl = (IP_WR_P2SB_CONTROLLER *)AllocateZeroPool (sizeof (IP_WR_P2SB_CONTROLLER));
  if (DciSbMmioAccessCntxt->RegType.P2Sb.P2SbCtrl == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  P2SbController = (P2SB_CONTROLLER *) DciSbMmioAccessCntxt->RegType.P2Sb.P2SbCtrl;

  Status = PtlPcdGetP2SbController (P2SbController, Pid);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  BuildP2SbSidebandAccess (
    P2SbController,
    Pid.PortId.LocalPid,
    0,
    P2SbMemory,
    P2SbMsgAccess,
    FALSE,
    (P2SB_SIDEBAND_REGISTER_ACCESS *) &DciSbMmioAccessCntxt->RegType.P2Sb
    );
  DciSbMmioAccessCntxt->RegType.P2Sb.Bar = 5;

    return EFI_SUCCESS;
}

/**
  Initializes USB3 IP Instance structure with proper context data for IPSD usage

  @param[in,out]  pInst         USB3 IP instance structure
  @param[in]      SiPolicy      The Silicon Policy PPI instance
**/
STATIC
VOID
InitIpInstance (
  IP_USB3_INST  *pInst,
  SI_POLICY_PPI *SiPolicy
  )
{
  EFI_STATUS                      Status;
  USB_CONFIG                      *UsbConfig;
  UINT8                           Index;
  P2SB_PORT_16_ID                 PortId;
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   ExiSbMmioAccess;

  DEBUG ((DEBUG_VERBOSE, "%a entry\n", __FUNCTION__));

  // Config block locate
  Status = GetConfigBlock ((VOID *)SiPolicy, &gUsbConfigGuid, (VOID *)&UsbConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - cannot get USB config block.\n", __FUNCTION__));
    UsbConfig = NULL;
  }

  // Print context init
  pInst->PrintError   = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoError;
  pInst->PrintWarning = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoWarning;
  pInst->PrintLevel1  = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoLevel1;

  // Allocate pointer types data
  pInst->RegCntxtPci = (IP_WR_REG_CNTXT)(UINTN) AllocateZeroPool (sizeof (IP_WR_REG_INFO));
  pInst->RegCntxtMem = (IP_WR_REG_CNTXT)(UINTN) AllocateZeroPool (sizeof (IP_WR_REG_INFO));
  pInst->RegCntxtSbMmio = (IP_WR_REG_CNTXT)(UINTN) AllocateZeroPool (sizeof (IP_WR_REG_INFO));
  pInst->RegCntxtDciSbMmio = (IP_WR_REG_CNTXT)(UINTN) AllocateZeroPool (sizeof (IP_WR_REG_INFO));

  // PTL specific values
  pInst->MmioBaseAddress      = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
  pInst->IpVersion            = IpUsb3IpVersion19p3;
  pInst->Integration          = IpUsb3IntegrationStandalone;
  pInst->Usb2PortCount        = 8;
  pInst->UsbRPortCount        = 0;
  pInst->Usb3LanesCount       = 2;
  pInst->FabricPllAdvanceWake = 0x19;

  pInst->LtrEnable                      = TRUE;
  pInst->NdeSbEnable                    = FALSE;
  pInst->HsiiEnable                     = FALSE;
  pInst->LtrHighIdleTime                = pInst->LtrEnable ? 0x02000400 : 0x00050002;
  pInst->LtrMedIdleTime                 = pInst->LtrEnable ? 0x01320132 : 0x00050002;
  pInst->LtrLowIdleTime                 = pInst->LtrEnable ? 0x00320032 : 0x00050002;
  pInst->LtvLimit                       = 0xBD0;
  pInst->InterruptPin                   = ItssGetDevIntPin (SiPolicy, PchXhciDevNumber (), PchXhciFuncNumber ());
  pInst->MaxPayloadSize                 = 0x7;
  pInst->RequestBoundaryCrossingControl = 0x1;

  if (UsbConfig != NULL) {
    // Values from config block
    pInst->PdoProgramming        = !!UsbConfig->PdoProgramming;
    pInst->OverCurrentEnable     = !!UsbConfig->OverCurrentEnable;
    pInst->OverCurrentLock       = !!UsbConfig->XhciOcLock;
    pInst->UsbAudioOffloadEnable = !!UsbConfig->UaolEnable;
    pInst->Usb31SpeedAdvertise   = UsbConfig->Usb31Speed;
    pInst->IsDwbEnabled          = !!UsbConfig->DwbEnable;

    for (Index = 0; Index < pInst->Usb2PortCount; Index++) {
      pInst->Usb2PortConfig[Index].Enable = !!UsbConfig->PortUsb20[Index].Enable;
      pInst->Usb2PortConfig[Index].OverCurrentPin = (UINT8)UsbConfig->PortUsb20[Index].OverCurrentPin;
      pInst->Usb2PortConfig[Index].PortResetMessageEnable = !!UsbConfig->PortUsb20[Index].PortResetMessageEnable;
      pInst->Usb2PortConfig[Index].SwDeviceModeEnable     = !!UsbConfig->PortUsb20[Index].SwDeviceModeEnable;
      pInst->Usb2PortConfig[Index].Usb3CompatiblePort     = (UINT8)UsbConfig->PortUsb20[Index].Usb3CompatiblePort;
      pInst->Usb2PortConfig[Index].Usb3CompatiblePortNum  = (UINT8)UsbConfig->PortUsb20[Index].Usb3CompatiblePortNum;
    }

    for (Index = 0; Index < pInst->Usb3LanesCount; Index++) {
      pInst->Usb3PortConfig[Index].Enable = !!UsbConfig->PortUsb30[Index].Enable;
      pInst->Usb3PortConfig[Index].OverCurrentPin = (UINT8)UsbConfig->PortUsb30[Index].OverCurrentPin;
    }

    pInst->DisableHostController = !!UsbConfig->ControllerDisable;
  }

  // Callbacks
  pInst->cEnableOvercurrentPin = UsbEnableOvercurrentPin;
  pInst->cIsUsbOwningLane = IsFiaLaneOwnedByUsb;

  if (pInst->RegCntxtPci != (IP_WR_REG_CNTXT)0) {
    CreatePcieConfigAccessContext (
      (IP_WR_REG_INFO *)(UINTN)pInst->RegCntxtPci,
      DEFAULT_PCI_SEGMENT_NUMBER_PCH,
      DEFAULT_PCI_BUS_NUMBER_PCH,
      PchXhciDevNumber (),
      PchXhciFuncNumber ()
      );
  }
  if (pInst->RegCntxtMem != (IP_WR_REG_CNTXT)0) {
    CreateMmioAccessContext (
      (IP_WR_REG_INFO *)(UINTN)pInst->RegCntxtMem,
      PcdGet32 (PcdSiliconInitTempMemBaseAddr)
      );
  }
  if (pInst->RegCntxtSbMmio != (IP_WR_REG_CNTXT)0) {
    PortId.Pid16bit = PTL_SID_F2_PID_XHCI;
    Status = CreateSbMmioAccessContext (
      (IP_WR_REG_INFO *)(UINTN)pInst->RegCntxtSbMmio,
      PortId
      );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - SB MMIO access context failed\n", __FUNCTION__));
    }
  }
  if (pInst->RegCntxtDciSbMmio != (IP_WR_REG_CNTXT)0) {
    PortId.Pid16bit = PTL_SID_F2_PID_DBC;
    Status = CreateDciSbMmioAccessContext (
      (IP_WR_REG_INFO *)(UINTN)pInst->RegCntxtDciSbMmio,
      PortId
      );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - DCI SB MMIO access context failed\n", __FUNCTION__));
    }
  }

  pInst->IsDciEnabled = FALSE;
  pInst->SkipD3HE = FALSE;

  PortId.Pid16bit = PTL_SID_F2_PID_EXI;
  Status = PtlPcdGetP2SbController (&P2SbController, PortId);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a - DCI debug enabled can not be read\n", __FUNCTION__));
    DEBUG ((DEBUG_INFO, "%a - DCI Skip D3 Hot %x\n", __FUNCTION__, pInst->SkipD3HE));
    return;
  }

  BuildP2SbSidebandAccess (
    &P2SbController,
    PortId.PortId.LocalPid,
    0,
    P2SbPrivateConfig,
    P2SbMsgAccess,
    FALSE,
    &ExiSbMmioAccess
    );

  pInst->IsDciEnabled = IsDciDebugEnabled ((REGISTER_ACCESS *) &ExiSbMmioAccess);
  pInst->SkipD3HE = (
    pInst->IsDciEnabled &&
    IsDciHostConnected ((REGISTER_ACCESS *) &ExiSbMmioAccess)
    );
}

/**
  Initializes SoC USB3 controller using USB3 IPSD API

  @param[in] SiPolicy     The Silicon Policy PPI instance
**/
STATIC
VOID
PtlPcdUsbHostControllerInit (
  SI_POLICY_PPI *SiPolicy
  )
{
  IP_USB3_INST    IpUsb3Inst;
  IP_CSI_STATUS   Status;

  IP_CSI_TUNING_PARAM_ENTRY  TuningList[] = {
    { IpCsiTuningParamIdReleaseInfo,          0x20230124 },
    { IpUsb3TuningParamIdCp13Deemphasis,      0x00000E45 },
    { IpUsb3TuningParamIdCp14Deemphasis,      0x00008D80 },
    { IpUsb3TuningParamIdCp15Deemphasis,      0x00008C45 },
    { IpUsb3TuningParamIdCp16Deemphasis,      0x00000F80 },
    { IpUsb3TuningParamIdLinkPollingMinTime,  0x2},
    { IpUsb3TuningParamIdGapTimeAfterLsTx,    0x16},
    { IpUsb3TuningParamIdU2ExitLatency,       0x3E8},
    { IpUsb3TuningParamIdHbufWaterMark,       0x2},
    { IpUsb3TuningParamIdIdleHysteresisScale, 0x1},
    { IpUsb3TuningParamIdMinIdleSpan,         0x1E},
  };

  IP_USB3_FEATURE_LIST_ENTRY  FeatureList[] = {
    { IpUsb3FeatIdHhhFlushFix,            IpUsb3FeatValHhhFlushFixDis          },
    { IpUsb3FeatIdB2bRetryFix,            IpUsb3FeatValB2bRetryFixEn           },
    { IpUsb3FeatIdReadAfterWriteDelay,    IpUsb3FeatValReadAfterWriteDelayEn   },
    { IpUsb3FeatIdHSAsyncActiveExposure,  IpUsb3FeatValHSAsyncActiveExposureEn },
  };

  DEBUG ((DEBUG_VERBOSE, "%a entry\n", __FUNCTION__));

  // Clear structures data
  ZeroMem (&IpUsb3Inst, sizeof (IP_USB3_INST));

  // Init Ip instance with config block data and SoC specific settings
  InitIpInstance (&IpUsb3Inst, SiPolicy);

  IpUsb3Inst.TuningList = TuningList;
  IpUsb3Inst.TuningListEntryCount = ARRAY_SIZE (TuningList);
  IpUsb3Inst.FeatureList = FeatureList;
  IpUsb3Inst.FeatureListEntryCount = ARRAY_SIZE (FeatureList);

  // Calling instance init to setup register access
  if (IpUsb3InstInit (&IpUsb3Inst) != IpCsiStsSuccess) {
    DEBUG ((DEBUG_WARN, "Error during instance initialization\n"));
    return;
  }

  DEBUG ((DEBUG_INFO, "USB3 IPSD version: %08X\n", IpUsb3GetVersion (&IpUsb3Inst, IpCsiVerIdIpFw)));
  DEBUG ((DEBUG_INFO, "CSI version: %08X\n", IpUsb3GetVersion (&IpUsb3Inst, IpCsiVerIdCsi)));

  // Calling single full init API instead of various function calls
  Status = IpUsb3HostControllerFullInit (&IpUsb3Inst);
  if (Status != IpCsiStsSuccess) {
    DEBUG ((DEBUG_WARN, "Error during IpUsb3HostControllerFullInit\n"));
    return;
  }
}

/**
  Initializes SoC USB3 Device Controller

  @param[in] SiPolicy     The Silicon Policy PPI instance
**/
STATIC
VOID
PtlPcdUsbDeviceControllerInit (
  SI_POLICY_PPI *SiPolicy
  )
{
  UINT8                          InterruptPin;
  UINT8                          Irq;
  USB_CONTROLLER                 DeviceController;
  USB_PRIVATE_CONFIG             UsbPrivateConfig;
  USB_CALLBACK                   UsbCallback;
  USB_HANDLE                     UsbHandle;
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  XdciSbMmioAccess;
  PCI_CONFIG_SPACE_ACCESS        XdciPcieConfigAccess;
  MMIO_ACCESS                    XdciMmioAccess;

  DEBUG ((DEBUG_VERBOSE, "%a entry\n", __FUNCTION__));

  // Clear structures data
  ZeroMem (&DeviceController, sizeof (USB_CONTROLLER));
  ZeroMem (&UsbPrivateConfig, sizeof (USB_PRIVATE_CONFIG));
  ZeroMem (&UsbCallback, sizeof (USB_CALLBACK));
  ZeroMem (&UsbHandle, sizeof (USB_HANDLE));
  ZeroMem (&XdciSbMmioAccess, sizeof (P2SB_SIDEBAND_REGISTER_ACCESS));
  ZeroMem (&XdciPcieConfigAccess, sizeof (PCI_CONFIG_SPACE_ACCESS));
  ZeroMem (&XdciMmioAccess, sizeof (MMIO_ACCESS));

  CreateUsbHandle (SiPolicy, &UsbHandle, &UsbPrivateConfig, &UsbCallback);
  if (UsbHandle.UsbConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - couldn't locate config block, aborting.\n", __FUNCTION__));
    return;
  }

  CreateUsbDeviceController (
    &XdciPcieConfigAccess,
    &XdciMmioAccess,
    &XdciSbMmioAccess,
    &P2SbController,
    &DeviceController
    );
  UsbHandle.DeviceController = &DeviceController;

  XdciConfigure (&UsbHandle);
  if (UsbHandle.UsbConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - couldn't locate config block, aborting.\n", __FUNCTION__));
    return;
  }
  if (UsbHandle.UsbConfig->XdciConfig.Enable) {
    // Configure xDCI interrupt
    ItssGetDevIntConfig (
      SiPolicy,
      PchXdciDevNumber (),
      PchXdciFuncNumber (),
      &InterruptPin,
      &Irq
      );

    // Set Interrupt Pin and IRQ number
    XdciConfigureInterrupt (&UsbHandle, InterruptPin, Irq);
  }
}

/**
  Performs PCH USB Controllers initialization

  @param[in] SiPolicy     The Silicon Policy PPI instance
**/
VOID
PtlPcdUsbInit (
  SI_POLICY_PPI *SiPolicy
  )
{
  DEBUG ((DEBUG_VERBOSE, "%a entry\n", __FUNCTION__));

  // Configure xHCI - USB Host Controller
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_USB_HOST);
  PtlPcdUsbHostControllerInit (SiPolicy);

  // Configure xDCI - USB Device Controller
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_USB_DEVICE);
  PtlPcdUsbDeviceControllerInit (SiPolicy);
}
