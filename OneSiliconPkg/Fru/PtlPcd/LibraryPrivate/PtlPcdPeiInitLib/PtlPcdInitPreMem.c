/** @file
  The PCH Init PEIM implements the PCH PEI Init PPI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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
#include <Library/ChipsetInitLib.h>
#include <IntelRcStatusCode.h>
#include <Library/OcWdtLib.h>
#include <Library/SpiLib.h>
#include <Library/PchDmiLib.h>
#include <Library/IocLib.h>
#include <Library/P2sbPrivateLib.h>
#include <Library/PeiRtcLib.h>
#include <Library/PeiWdtLib.h>
#include <Library/PeiSmbusLib.h>
#include <Library/PeiItssLib.h>
#include <PchPcieRpInfo.h>
#include <Register/PchPcieRpRegs.h>
#include <Library/PeiPcieRpInitLib.h>
#include <Library/SpiInitLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <Library/PeiSerialIoInitLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PmcSocLib.h>
#include <TcssPeiPreMemConfig.h>
#include <Library/PcdInfoLib.h>
#include <Library/PeiCpuInitFruLib.h>
#include <Register/DfxRegs.h>
#include <Register/PchRegs.h>
#include <Library/WdtCommonLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdSpiSocLib.h>
#include <Library/EspiPrivateLib.h>
#include <Library/PeiLpcLib.h>
#include <Library/PchPcrLib.h>
#include <SpiHandle.h>
#include <Library/PeiLpssI2cInitLib.h>
#include <Library/MeInitLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdFiaSocLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <PcdSbPortIds.h>

#include <Register/SpiRegs.h>
#include <Register/PchRegs.h>
#include <Register/Ptl/Cpu/CpuSbInfo.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/CpuRegbarAccessLib.h>

STATIC EFI_GUID mResetNotificationCallbackPtrGuid = {0xb0d9cc70, 0x5bbf, 0x4eb2, {0xbc, 0x34, 0x53, 0x1b, 0xfe, 0x7f, 0x59, 0xeb}};

typedef struct {
  EFI_RESET_SYSTEM  ResetFunction;
} RESET_CALLBACK_PTR_HOB;

VOID
EFIAPI
SpiDmaWaResetNotificationCallback (
  IN EFI_RESET_TYPE     ResetType,
  IN EFI_STATUS         ResetStatus,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  );

/**
  This function register reset notify ppi with SPI DMA WA in PEI
  @param[in]  PeiServices      Pointer to PEI Services Table.
  @param[in]  NotifyDescriptor Pointer to the descriptor for the Notification event that
                               caused this function to execute.
  @param[in]  Interface        Pointer to the PPI data associated with this function.
  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
SpiDmaWaResetNotify (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  )
{
  EFI_STATUS                      Status;
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetNotify;
  RESET_CALLBACK_PTR_HOB          *PtrHob;

  DEBUG ((DEBUG_INFO, "%a\n",__FUNCTION__));

  Status = PeiServicesLocatePpi (
            &gEdkiiPlatformSpecificResetNotificationPpiGuid,
            0,
            NULL,
            (VOID **) &ResetNotify
            );

  if (!EFI_ERROR(Status)) {
    PtrHob = BuildGuidHob (&mResetNotificationCallbackPtrGuid, sizeof (RESET_CALLBACK_PTR_HOB));
    if (PtrHob == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    PtrHob->ResetFunction = SpiDmaWaResetNotificationCallback;
    ResetNotify->RegisterResetNotify (ResetNotify, SpiDmaWaResetNotificationCallback);
  }
  return EFI_SUCCESS;
}

/**
  This function register reset notify ppi in PEI after memory is discovered

  @param[in]  PeiServices      Pointer to PEI Services Table.
  @param[in]  NotifyDescriptor Pointer to the descriptor for the Notification event that
                               caused this function to execute.
  @param[in]  Interface        Pointer to the PPI data associated with this function.
  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others

**/
STATIC
EFI_STATUS
EFIAPI
SpiDmaWaResetNotifyAfterMemoryDiscovered (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  )
{
  EFI_STATUS                      Status;
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetNotify;
  EFI_HOB_GUID_TYPE               *GuidHob;
  RESET_CALLBACK_PTR_HOB          *PtrHob;

  Status = PeiServicesLocatePpi (
            &gEdkiiPlatformSpecificResetNotificationPpiGuid,
            0,
            NULL,
            (VOID **) &ResetNotify
            );

  if (!EFI_ERROR(Status)) {
    //
    // After memory is discovered, we need to fixup pointer to the callback function.
    //
    GuidHob = GetFirstGuidHob (&mResetNotificationCallbackPtrGuid);
    if (GuidHob == NULL) {
      ASSERT (FALSE);
      return EFI_NOT_FOUND;
    }
    PtrHob = (RESET_CALLBACK_PTR_HOB *) GET_GUID_HOB_DATA (GuidHob);
    ResetNotify->UnregisterResetNotify (ResetNotify, PtrHob->ResetFunction);
    ResetNotify->RegisterResetNotify (ResetNotify, SpiDmaWaResetNotificationCallback);
  }
  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mSpiDmaWaResetNotify = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEdkiiPlatformSpecificResetNotificationPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) SpiDmaWaResetNotify
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mSpiDmaWaResetNotifyAfterMemoryDiscovered = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiMemoryDiscoveredPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) SpiDmaWaResetNotifyAfterMemoryDiscovered
};

/**
  The function used to get P2SB access of Espi
  @return                             Espi P2SB Access.
**/
STATIC
VOID
GetEspiP2SbAccess (
  IN OUT P2SB_CONTROLLER                *P2SbController,
  IN OUT P2SB_SIDEBAND_REGISTER_ACCESS  *P2SbSidebandAccess
  )
{
  EFI_STATUS                          Status;
  P2SB_PORT_16_ID                    P2SBPid;

  ZeroMem (P2SbController, sizeof (P2SB_CONTROLLER));
  ZeroMem (P2SbSidebandAccess, sizeof (P2SB_SIDEBAND_REGISTER_ACCESS));

  P2SBPid.Pid16bit = PTL_SID_F2_PID_SPBC;
  Status = PtlPcdGetP2SbController (P2SbController, P2SBPid);

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to get P2SbController"));
    return;
  }

  BuildP2SbSidebandAccess (P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, P2SbSidebandAccess);
}

/**
  Workaround for SPI DMA. Disable Power and Clock gating to prevent the bug.
  Use this function before you use SPI-DMA
**/
VOID
PtlPcdSpiDmaWaStart (
  VOID
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;
  UINT32                         AndData32;
  UINT32                         OrData32;
  UINT32                         Data32;

  DEBUG ((DEBUG_INFO, "PtlSocSpiDmaWaStart()\n"));
  GetEspiP2SbAccess(&P2SbController, &EspiPcrAccess);

  // Clearing bit 3 and bit 4 back before SPI DMA is initiated
  // [ 5] PLCC_DCGE: Primary Logic, Core Clock, Dynamic Clock Gate Enable
  // [ 4] PLPC_DCGE: Primary Logic, Primary Clock, Dynamic Clock Gate Enable
  AndData32 = (UINT32)~(
    BIT5                                                    |
    BIT4
    );
  // Setting Clock Gate HoldOff Timeout value to 0xF
  OrData32 = (UINT32) (0);

  Data32 = EspiPcrAccess.Access.Read32(&EspiPcrAccess.Access,
                               R_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL);
  DEBUG ((DEBUG_ERROR, "SPI Clock Power Gate Control Before WA: 0x%x\n", Data32));

  // Disable ClockGate settings
  EspiPcrAccess.Access.AndThenOr32 (
                               &EspiPcrAccess.Access,
                               R_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL,
                               AndData32,
                               OrData32
                               );

  Data32 = EspiPcrAccess.Access.Read32(&EspiPcrAccess.Access,
                               R_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL);
  DEBUG ((DEBUG_ERROR, "SPI Clock Power Gate Control After WA: 0x%x\n", Data32));

  PtlPcdPsfDisableClockGating ();

  // PSF Power Gate Config
  AndData32 = ~(UINT32)(0xF);

  // Set to 0x80000006h
  OrData32 = (UINT32)(0x80000006);
  MmioAndThenOr32 (PmcGetPwrmBase () + R_PMC_PWRM_FAB_PG_CTL_PSF, AndData32, OrData32);
}

/**
  Workaround for SPI DMA.
  Re-enable Power and Clock gating after SPI-DMA usage
**/
VOID
PtlPcdSpiDmaWaEnd (
  VOID
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;
  UINT32                         AndData32;
  UINT32                         OrData32;
  UINT32                         Data32;

  DEBUG ((DEBUG_INFO, "PtlSocSpiDmaWaEnd()\n"));
  GetEspiP2SbAccess(&P2SbController, &EspiPcrAccess);

  // Setting bit 5 and bit 4 after SPI DMA is initiated
  // [ 5] PLCC_DCGE: Primary Logic, Core Clock, Dynamic Clock Gate Enable
  // [ 4] PLPC_DCGE: Primary Logic, Primary Clock, Dynamic Clock Gate Enable
  // Reset Clock Gate HoldOff to the previous value
  AndData32 = (UINT32)~(0);

  OrData32 = (UINT32) (
    BIT5                                                                     |
    BIT4
    );

  Data32 = EspiPcrAccess.Access.Read32(&EspiPcrAccess.Access,
                               R_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL);
  DEBUG ((DEBUG_INFO, "SPI Clock Power Gate Control Before WA: 0x%08X\n", Data32));

  // Re-enable Clock Gate Settings
  EspiPcrAccess.Access.AndThenOr32 (
                               &EspiPcrAccess.Access,
                               R_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL,
                               AndData32,
                               OrData32
                               );

  Data32 = EspiPcrAccess.Access.Read32(&EspiPcrAccess.Access,
                               R_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL);
  DEBUG ((DEBUG_INFO, "SPI Clock Power Gate Control After WA: 0x%x\n", Data32));

  PtlPcdPsfEnableClockGating ();

  // PSF Power Gate Config
  AndData32 = ~(UINT32)(0xF);
  // Set to 0x80000002
  OrData32 = (UINT32)(0x80000002);

  MmioAndThenOr32 (PmcGetPwrmBase () + R_PMC_PWRM_FAB_PG_CTL_PSF, AndData32, OrData32 );
}

VOID
PtlPcdOnMemoryDiscovered (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "%a - Start\n",__FUNCTION__));
  PtlPcdSpiDmaWaStart ();
  PeiServicesNotifyPpi (&mSpiDmaWaResetNotify);
  PeiServicesNotifyPpi (&mSpiDmaWaResetNotifyAfterMemoryDiscovered);
  DEBUG ((DEBUG_INFO, "%a - End\n",__FUNCTION__));
}

/**
 * SPI DMA WA call back function for Reset notification.
  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
SpiDmaWaResetNotificationCallback (
  IN EFI_RESET_TYPE     ResetType,
  IN EFI_STATUS         ResetStatus,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  )
{
  DEBUG ((DEBUG_INFO, "%a\n",__FUNCTION__));
  PtlPcdSpiDmaWaEnd ();
}

/**
  Check some PCH policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] SiPreMemPolicy  The SI PreMem Policy PPI instance

**/
VOID
PtlPcdValidatePolicy (
  IN  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  )
{
  EFI_STATUS                  Status;
  PCH_GENERAL_PREMEM_CONFIG   *PchGeneralPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  ASSERT (SiPreMemPolicy->TableHeader.Header.Revision == SI_PREMEM_POLICY_REVISION);
}

/**
  This function performs basic initialization for PCH in PEI phase after Policy produced.
  If any of the base address arguments is zero, this function will disable the corresponding
  decoding, otherwise this function will enable the decoding.
  This function locks down the AcpiBase.

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
EFIAPI
PtlPcdOnPolicyInstalled (
  IN  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                  Status;
  PCH_GENERAL_PREMEM_CONFIG   *PchGeneralPreMemConfig;
  P2SB_HANDLE                 P2SbHandle;
  P2SB_PRIVATE_CONFIG         P2SbPrivate;
  P2SB_CONTROLLER             P2SbController;
  P2SB_CALLBACK               P2SbCallback;
  PCI_CONFIG_SPACE_ACCESS     P2SbPcieConfigAccess;
  UINTN                       HpetMmio;

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREMEM_START);
  DEBUG ((DEBUG_INFO, "PchOnPolicyInstalled() - Start\n"));
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  BuildPchGeneralConfigHobPreMem (SiPreMemPolicyPpi);

  ///
  /// Set PWRM Base in PMC device
  ///
  Status = PmcSetPwrmBase (PmcPciCfgBase (), PCH_PWRM_BASE_ADDRESS);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Set ACPI Base in PMC device
  ///
  PtlPcdPsfSetPmcAbase (PcdGet16 (PcdAcpiBaseAddress));

  //
  // Set Deferred Write Buffer abd hysteresis value for trunk clock gating in PSF
  //
  PtlPcdPsfProgramDWB (SiPreMemPolicyPpi);
  PtlPcdPsfConfigureClkreqHys ();

  //
  // Set PMC Privacy Consent
  //
  PmcSendPrivacyConsent (PmcGetPwrmBase (), PchGeneralPreMemConfig->PmcPrivacyConsent & !(IsEom ()));

  //
  // Check to disable and lock WDT according to Policy.
  //
  WdtDisableAndLock (SiPreMemPolicyPpi);

  PtlPcdPcieRpPreMemInit (SiPreMemPolicyPpi);

  PtlPcdExtendedBiosDecodeRangeRegistration (SiPreMemPolicyPpi);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREMEM_INIT_DCI);
  PtlPcdDciInit (SiPreMemPolicyPpi);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREMEM_INIT_TRACEHUB);
  PtlPcdTraceHubInitPreMem (SiPreMemPolicyPpi);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREMEM_INIT_SMBUS);
  Status = SmbusInit (SiPreMemPolicyPpi);

  if (PchGeneralPreMemConfig->Port80Route == PchReservedPageToLpc) {
    IocSetReservedPageRegToLpc ();
  } else {
    IocSetReservedPageRegToPcieRootPort ();
  }

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREMEM_INIT_HPET);
  PtlPcdP2SbHandleInit (NULL, &P2SbHandle, &P2SbPrivate, &P2SbController, &P2SbCallback, &P2SbPcieConfigAccess);

  Status = PchHpetBaseGet (&HpetMmio);
  if (Status == EFI_SUCCESS) {
    P2sbHpetInit (&P2SbHandle, HpetMmio);
    P2sbHpetBuildMemResHob (&P2SbController, HpetMmio);
  }

  LpcOnPolicyConfigure (SiPreMemPolicyPpi);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREMEM_INIT_CNVI);

  PtlPcdCnviPreMemInit (SiPreMemPolicyPpi);

  PtlPcdAcePreMemInit (SiPreMemPolicyPpi);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREMEM_DONE);
  DEBUG ((DEBUG_INFO, "PchOnPolicyInstalled() - End\n"));
}

/**
  This code is intended to handle all cases where IP could be fuse disabled, static power gated
  or disabled due to soft strap lane assignment

  @retval EFI_SUCCESS
**/
VOID
PtlPcdEarlyDisabledDeviceHandling (
  VOID
  )
{
  UINT8                           Index;

  DEBUG ((DEBUG_INFO, "PchEarlyDisabledDeviceHandling() - Start\n"));

  ///
  /// BWG 5.3.1 Fuse Disabled, Static Disabled, non-Static function disable Device Handling
  ///

  //
  // OTG fused
  //
  if (!PmcIsXdciSupported ()) {
    DEBUG ((DEBUG_INFO, "xDCI (OTG) Fuse disabled - performing PSF disable\n"));
    PtlPcdPsfDisableXdci ();
  }

  //
  // Serial Io fused
  //
  if (!PmcIsSerialIoSupported (PmcGetPwrmBase ())) {
    DEBUG ((DEBUG_INFO, "Serial Io Fuse disabled - performing PSF disable\n"));
    for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
      PtlPcdPsfDisableLpssI2c (Index);
    }
    for (Index = 0; Index < GetMaxSpiInterfacesNum (); Index++) {
      PtlPcdPsfDisableLpssSpi (Index);
    }
    for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
      PtlPcdPsfDisableLpssUart (Index);
    }
  }

  for (Index = 0; Index < PchGetMaxUfsNum (); Index++) {
    if (!PmcIsScsUfsSupported (Index)) {
      DEBUG ((DEBUG_INFO, "SCS UFS[%d] Fuse disabled - performing PSF disable\n", Index));
      PtlPcdPsfDisableScsUfs (Index);
    }
  }

  //
  // ISH fused
  //
  if (!PmcIsIshSupported ()) {
    DEBUG ((DEBUG_INFO, "ISH Supported and Fuse disabled - performing PSF disable\n"));
    PtlPcdPsfDisableIsh ();
  }

  //
  // Gbe fused
  //
  if (IsPchGbeSupported () && !PmcIsGbeSupported (PmcGetPwrmBase ())) {
    DEBUG ((DEBUG_INFO, "GBE Supported and Fuse disabled - performing PSF disable\n"));
    PtlPcdPsfDisableGbe ();
  }

  if (!PmcIsCnviSupported ()) {
    DEBUG ((DEBUG_INFO, "CNVi WiFi / Bluetooth function Fuse disabled - performing PSF disable\n"));
    PtlPcdPsfDisableCnvi ();
    PtlPcdPsfDisableCnviBt ();
  }

  DEBUG ((DEBUG_INFO, "PchEarlyDisabledDeviceHandling() - End\n"));
}

/**
  Show PCH related information
**/
VOID
PtlPcdShowInformation (
  VOID
  )
{
  DEBUG_CODE_BEGIN ();
  CHAR8           Buffer[PTL_SOC_INFO_STEPPING_STR_MIN_LENGTH];
  EFI_STATUS      Status;

  DEBUG ((DEBUG_INFO, "SoC Series   : %a\n", PtlPcdGetSeriesStr ()));
  Status = PtlPcdGetSteppingStr (Buffer, sizeof (Buffer));
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SoC Stepping : %a\n", Buffer));
  }
  DEBUG ((DEBUG_INFO, "SoC SKU      : %a\n", PtlPcdGetSkuStr ()));
  PmcPrintResetReason ();
  DEBUG_CODE_END ();
}

/**
  Disables EISS (InSMM.STS) requirement in both SPI and eSPI
**/
STATIC
VOID
SpiEspiDisableEiss (
  VOID
  )
{
  SPI_HANDLE  SpiHandle;
  EFI_STATUS  Status;

  Status = PtlPcdGetSpiHandle (&SpiHandle, NULL);
  ASSERT_EFI_ERROR (Status);

  SpiDisableEiss (&SpiHandle);
  EspiDisableEiss ();
}

/**
  PCH init pre-memory entry point
**/
VOID
EFIAPI
PtlPcdInitPrePolicy (
  VOID
  )
{
  EFI_STATUS              Status;
  P2SB_CONTROLLER         P2SbController;
  PCI_CONFIG_SPACE_ACCESS P2SbPcieConfigAccess;
  UINT8                   PcieRpFuncNumTable[PCH_MAX_PCIE_ROOT_PORTS];
  UINT32                  PcieRpFuncNumTableSize;
  UINT32                  Index;

  //
  // Dump PCH information
  //
  DEBUG_CODE_BEGIN ();
  PtlPcdShowInformation ();
  PtlPcdFiaPrintLosRegisters ();
  DEBUG_CODE_END ();

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREPOLICY_START);
  DEBUG ((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));

  //
  // Check if Pch is supported
  //
  if (!PtlPcdIsSupported ()) {
    DEBUG ((DEBUG_ERROR, "PCH SKU is not supported due to no proper PCH LPC found!\n"));
    ASSERT (FALSE);
  }

  //
  // Check if SBREG has been set.
  //
  Status = PtlPcdGetFirstP2SbController (&P2SbController);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
  }
  PciConfigAccessInit (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, P2sbDevNumber (), P2sbFuncNumber (), &P2SbPcieConfigAccess);
  if (!P2sbIsSbregReady (&P2SbPcieConfigAccess.RegAccess)) {
    DEBUG ((DEBUG_INFO, "SBREG should be programmed before here\n"));
    ASSERT (FALSE);
  }

  //
  // Check if PCH PWRM Base has been set
  //
  DEBUG ((DEBUG_INFO, "PCH PWRM Base needs to be programmed before here\n"));
  ASSERT (PtlPcdPsfGetPmcPwrmBar0 () != 0);

  //
  // Build Memory Mapped IO Resource which is used to build E820 Table in LegacyBios.
  // the resource range should be preserved in ACPI as well.
  //
  PchPreservedMmioResource ();

  //
  // Clear WdtAllowKnownReset that may persist after previous boot.
  //
  WdtClearAllowKnownReset ();

  //
  // If there was unexpected reset but no WDT expiration and no resume from S3/S4,
  // clear unexpected reset status and enforce expiration. This is to inform Firmware
  // which has no access to unexpected reset status bit, that something went wrong.
  //
  OcWdtResetCheck();

  //
  // Initialize WDT and install WDT PPI
  //
  WdtInstallResetCallback ();
  OcWdtInit ();

  //
  // Installs PCH SPI PPI
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREPOLICY_INIT_SERVICE_SPI);
  SpiServiceInit ();

  //
  // Perform PSF early initialization.
  //
  PcieRpFuncNumTableSize = PtlPcdPsfGetMaxPsfDecodedPciePortNum ();
  for (Index = 0; Index < PcieRpFuncNumTableSize; Index++) {
    PcieRpFuncNumTable[Index] = PchPcieRpFuncNumber (Index);
  }
  PtlPcdPsfEarlyInit (PcieRpFuncNumTable, PcieRpFuncNumTableSize);

  //
  // Before any interrupt is enabled, set host message enable.
  //
  PtlPcdItssSetMasterMessageEnable ();
  //
  // The 8259 PIC is still functional and not masked by default,
  // even if APIC is enabled.
  // So need to set all 8259 interrupts to disabled.
  //
  ItssMask8259Interrupts ();

  IocEnableUCodePatchRegion ();

  //
  // Do P2SB early configuration.
  //
  PtlPcdP2sbEarlyConfig ();

  //
  // Configura RTC when power failure case.
  //
  if (!PmcIsRtcBatteryGood ()) {
    RtcPowerFailureConfiguration ();
  }

  //
  // Clear CF9GR if it's set in previous boot.
  //
  PmcDisableCf9GlobalReset (PmcGetPwrmBase ());

  //
  // Enhance port 8xh LPC decoding
  //
  LpcOnEarlyPeiConfigure ();

  //
  // Handle all static disabled controllers.
  //
  PtlPcdEarlyDisabledDeviceHandling ();

  PtlPcdPciePrePolicyInit ();

  //
  // Disable EISS for variable service
  //
  SpiEspiDisableEiss ();

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREPOLICY_DONE);
  DEBUG ((DEBUG_INFO, "PchInitPrePolicy() - End\n"));
}

/**
  Dfd miscellaneous configurations

  @param[in] SiPreMemPolicy  The SI PreMem Policy PPI instance

**/
VOID
DfdConfiguration (
  IN  SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                    Status;
  CPU_INIT_PREMEM_CONFIG        *CpuInitPreMemConfig;
  PCH_GENERAL_PREMEM_CONFIG     *PchGeneralPreMemConfig;
  P2SB_PORT_16_ID               P2SBPid;
  P2SB_CONTROLLER               P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS DfxGpepPcrAccess;

  CpuInitPreMemConfig = NULL;
  PchGeneralPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  //
  // Configure DFD
  //
  PeiCpuDfdEnable ((BOOLEAN)(CpuInitPreMemConfig->DfdEnable));
  //
  // Configure Pmode clock
  //
  P2SBPid.Pid16bit = PTL_SID_F2_PID_DFX_GPEP;
  Status = PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to get P2SbController for DFX_GPEP."));
    return;
  }
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMsgAccess, FALSE, &DfxGpepPcrAccess);
  DfxGpepPcrAccess.Bar = 7;
  if ((BOOLEAN)(PchGeneralPreMemConfig->PmodeClkEn)) {
    DfxGpepPcrAccess.Access.AndThenOr32 (&DfxGpepPcrAccess.Access, R_DFX_GPEP_PCR_SURVIVABILITY_DFD_0, (UINT32)~B_DFX_GPEP_PCR_SURVIVABILITY_DFD_0_SD0OUT_1, 0);
  } else {
    DfxGpepPcrAccess.Access.AndThenOr32 (&DfxGpepPcrAccess.Access, R_DFX_GPEP_PCR_SURVIVABILITY_DFD_0, ~0u, B_DFX_GPEP_PCR_SURVIVABILITY_DFD_0_SD0OUT_1);
  }
}
