/** @file
  VR post initializations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/HobLib.h>
#include <Register/VoltageRegulatorRegs.h>
#include <Library/PeiVrLib.h>
#include <VrDataHob.h>
#include <Library/PeiVrDomainLib.h>
#include <Library/CpuCommonLib.h>
#include <IntelRcStatusCode.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/FixedPointLib.h>
#include <Guid/CpuPmData.h>
#include <Library/PeiServicesLib.h>
#include <Register/B2pMailbox.h>

///
/// Vsys Demotion
///
typedef union {
  UINT32 Data;
  struct {
    UINT32 VsysData1    : 4;
    UINT32 VsysData2    : 4;
    UINT32 Reserved1    : 24;
  } Fields;
} MAILBOX_DATA_FORMAT_VSYS;

/**
  Return TRUE when the Fast Vmode IccLimit is supported in the specific VR.

  @param[IN] - VrAddress - It represent address of VR domain

  @retval TRUE     FVM IccLimit is supported.
  @retval FALSE    FVM IccLimit is not supported.
 */
BOOLEAN
IsFastVmodeIccLimitSupport (
  IN UINT8    VrAddress
  )
{
  EFI_STATUS                 Status;
  PCODE_MAILBOX_INTERFACE    MailboxCommand;
  UINT32                     MailboxData;
  UINT32                     MailboxStatus;

  MailboxData = 0;
  MailboxStatus = 0;

  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
  MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_GET_FAST_VMODE_ICC_LIMIT;
  MailboxCommand.Fields.Param2 = VrAddress & VR_ADDRESS_MASK;
  DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Read Command = 0x%08x\n", MailboxCommand.InterfaceData));
  Status = MailboxRead (MailboxCommand.InterfaceData, &MailboxData, &MailboxStatus);
  if (EFI_ERROR (Status) || MailboxStatus != PCODE_MAILBOX_CC_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "IsFastVmodeIccLimitSupport: CPU Unit doesn't support FVM. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    return FALSE;
  }
  return TRUE;
}

/**
  Programs the common VR mailbox

  @param[in]  CpuPowerMgmtVrConfig   Vr config block
**/
VOID
SetVrCommon (
  IN CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig
  )
{
  EFI_STATUS                        Status;
  PCODE_MAILBOX_INTERFACE           MailboxCommand;
  UINT32                            MailboxStatus;
  RFI_EMI_CONTROL_0_0_0_MCHBAR      RfiEmiControlRegister;
  UINT64                            MchBar;
  MAILBOX_DATA_FORMAT_VSYS          VsysData;
  SI_PREMEM_POLICY_PPI*             SiPreMemPolicyPpi;
  CPU_POWER_DELIVERY_CONFIG         *CpuPowerDeliveryConfig;
  MAILBOX_DATA_PMON_CONFIG          PmonMailboxData;
  MAILBOX_DATA_PMON_PMAX            PmonPmaxMailboxData;
  MAILBOX_DATA_VSYS_MAX             VsysMaxMailboxData;
  EFI_HOB_GUID_TYPE                 *GuidHob;
  CPU_PM_DATA                       *CpuPmData;
  MAILBOX_DATA_FORMAT_HYSTERESIS    HysteresisWindowData;

  SiPreMemPolicyPpi = NULL;
  GuidHob           = NULL;
  CpuPmData         = NULL;

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuPowerDeliveryConfigGuid, (VOID *) &CpuPowerDeliveryConfig);
  ASSERT_EFI_ERROR (Status);

  GuidHob = GetFirstGuidHob (&gCpuPmDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Get CpuPmData failed\n"));
    return;
  }

  CpuPmData = (CPU_PM_DATA *) (GET_GUID_HOB_DATA (GuidHob));
  ASSERT (CpuPmData != NULL);

  ///
  /// Configure Platform Level controls
  /// PSYS Config
  /// -PsysOffset is defined as S16.7.8 fixed point
  /// -PsysSlope is defined as U16.1.15 fixed point
  /// -Policy Psys offset is defined in 1/1000 increments
  /// -Policy Psys slope is defined in 1/100 increments
  /// -Mailbox PsysOffset = (PlatPolicyPsysOffset * 2^8)/1000
  /// -Mailbox PsysSlope = (PlatPolicyPsysSlope * 2^15)/100
  /// -Adding half of divisor to dividend to account for rounding errors in fixed point arithmetic.
  ///
  if (CpuPowerMgmtVrConfig->PsysOffset != 0 || CpuPowerMgmtVrConfig->PsysSlope != 0 ) {
    PmonMailboxData.Data32 = 0;
    PmonMailboxData.Fields.PmonOffset = ToSignedFixedPoint16 (CpuPowerMgmtVrConfig->PsysOffset, 1000, 7, 8, NULL);
    PmonMailboxData.Fields.PmonSlopeCorrection = ToUnsignedFixedPoint16 (CpuPowerMgmtVrConfig->PsysSlope, 100, 1, 15, NULL);
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_PMON_CONFIG;
    MailboxCommand.Fields.Param2 = 0;
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = WRITE_PSYS_CONFIG_CMD\n"));
    DEBUG ((DEBUG_INFO, "(MAILBOX) PsysOffset            = %d (1/1000)\n", CpuPowerMgmtVrConfig->PsysOffset));
    DEBUG ((DEBUG_INFO, "(MAILBOX) PsysSlope             = %d (1/100)\n", CpuPowerMgmtVrConfig->PsysSlope));
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Data    = %d\n", PmonMailboxData.Data32));
    Status = MailboxWrite (MailboxCommand.InterfaceData, PmonMailboxData.Data32, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "VR: Error Writing PSYS Config. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }
  }

  ///
  /// PSYS Pmax
  /// -Mailbox Data PMax is defined as U16.10.6 fixed point
  /// -Policy Pmax is defined in 1/8 W increments
  ///
  if ((CpuPowerMgmtVrConfig->PsysPmax != 0) || (CpuPmData->PmData.PsysPmax != 0)) {
    PmonPmaxMailboxData.Data32 = 0;
    PmonPmaxMailboxData.Fields.PmonPmax = CpuPowerMgmtVrConfig->PsysPmax ? ToUnsignedFixedPoint16 (CpuPowerMgmtVrConfig->PsysPmax, 8, 10, 6, NULL) : ToUnsignedFixedPoint16 (CpuPmData->PmData.PsysPmax, 8, 10, 6, NULL);
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_PMON_PMAX;
    MailboxCommand.Fields.Param2 = 0;
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = WRITE_PSYS_PMAX_CMD\n"));
    DEBUG ((DEBUG_INFO, "(MAILBOX) PsysPmax              = %d (1/8 Watt)\n", CpuPowerMgmtVrConfig->PsysPmax));
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Data    = %d\n", PmonPmaxMailboxData.Data32));
    Status = MailboxWrite (MailboxCommand.InterfaceData, PmonPmaxMailboxData.Data32, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "VR: Error Writing Psys PMAX. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }
  }

  ///
  /// Vsys/Psys 0:Disable 1:Psys 2:Vsys
  ///
  if (CpuPowerMgmtVrConfig->EnableVsysCritical < 3) {
    Status                        = EFI_SUCCESS;
    MailboxStatus                 = PCODE_MAILBOX_CC_SUCCESS;
    ///
    /// Set Vsys/Psys mode
    ///
    VsysData.Data                 = 0;
    if (CpuPowerMgmtVrConfig->EnableVsysCritical == 2) { // 2:Vsys critical
      VsysData.Fields.VsysData1 = BIT3;  // Vsys Set Bit3 to 1
    }
    MailboxCommand.InterfaceData  = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1  = MAILBOX_VR_SUBCMD_SVID_SET_VR;
    MailboxCommand.Fields.Param2  = (MAILBOX_VR_SUBCMD_SVID_SET_VR_VSYS_MODE << 4) | MAILBOX_VR_SUBCMD_SVID_SET_PSYS_VR;
    ///
    /// Set Vsys/Psys mode
    ///
    Status = MailboxWrite (MailboxCommand.InterfaceData, VsysData.Data, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "Mailbox Command Write Vsys mode failed. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }

    if (CpuPowerMgmtVrConfig->EnableVsysCritical == 2) { //2:Vsys critical
      ///
      /// Set Vsys/Psys Critical Threshold when EnableVsysCritical is not disable
      ///
      VsysData.Data = (CpuPowerMgmtVrConfig->VsysCriticalThreshold * 0xFF) / CpuPowerMgmtVrConfig->VsysFullScale;
    } else if (CpuPowerMgmtVrConfig->EnableVsysCritical == 1) { //1:Psys
      VsysData.Data = (CpuPowerMgmtVrConfig->PsysCriticalThreshold * 0xFF) / CpuPowerMgmtVrConfig->PsysFullScale;
    } else {
      VsysData.Data = 0;
    }
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_VR;
    MailboxCommand.Fields.Param2 = (MAILBOX_VR_SUBCMD_SVID_SET_VR_CRIT_THRESHOLD << 4) | MAILBOX_VR_SUBCMD_SVID_SET_PSYS_VR;
    ///
    /// Set Vsys/Psys Critical Threshold
    ///
    Status = MailboxWrite (MailboxCommand.InterfaceData, VsysData.Data, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "Mailbox Command Write Vsys Critical Threshold failed. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }

    ///
    /// Set Vsys/Psys Assertion Deglitch
    ///
    VsysData.Data                 = 0;
    VsysData.Fields.VsysData1     = CpuPowerMgmtVrConfig->VsysAssertionDeglitchExponent;
    VsysData.Fields.VsysData2     = CpuPowerMgmtVrConfig->VsysAssertionDeglitchMantissa;
    MailboxCommand.InterfaceData  = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1  = MAILBOX_VR_SUBCMD_SVID_SET_VR;
    MailboxCommand.Fields.Param2  = (MAILBOX_VR_SUBCMD_SVID_SET_VR_CONFIG2 << 4) | MAILBOX_VR_SUBCMD_SVID_SET_PSYS_VR;
    ///
    /// Set Vsys/Psys Assertion Deglitch
    ///
    Status = MailboxWrite (MailboxCommand.InterfaceData, VsysData.Data, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "Mailbox Command Write Vsys Assertion Deglitch failed. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }

    ///
    /// Set Vsys/Psys De-Assertion Deglitch
    ///
    VsysData.Data                 = 0;
    VsysData.Fields.VsysData1     = CpuPowerMgmtVrConfig->VsysDeassertionDeglitchExponent;
    VsysData.Fields.VsysData2     = CpuPowerMgmtVrConfig->VsysDeassertionDeglitchMantissa;
    MailboxCommand.InterfaceData  = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1  = MAILBOX_VR_SUBCMD_SVID_SET_VR;
    MailboxCommand.Fields.Param2  = (MAILBOX_VR_SUBCMD_SVID_SET_VR_CONFIG1 << 4) | MAILBOX_VR_SUBCMD_SVID_SET_PSYS_VR;
    ///
    /// Set Vsys/Psys De-Assertion Deglitch
    ///
    Status = MailboxWrite (MailboxCommand.InterfaceData, VsysData.Data, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "Mailbox Command Write Vsys De-Assertion Deglitch failed. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }
  }

  if (CpuPowerDeliveryConfig->ThETAIbattEnable) {
    ///
    /// Vsys Max
    /// -VsysMax is defined as U16.10.6 fixed point
    /// -VsysMax is defined in 1/1000 increments
    ///
    VsysMaxMailboxData.Data32 = 0;
    VsysMaxMailboxData.Fields.VsysMax = CpuPowerDeliveryConfig->VsysMax ? ToUnsignedFixedPoint16 (CpuPowerDeliveryConfig->VsysMax, 1000, 10, 6, NULL) : ToUnsignedFixedPoint16 (CpuPmData->PmData.VsysMax, 1000, 10, 6, NULL);
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_VSYS_MAX;
    MailboxCommand.Fields.Param2 = 0;
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = MAILBOX_VR_SUBCMD_SVID_SET_VSYS_MAX\n"));
    DEBUG ((DEBUG_INFO, "(MAILBOX) VsysMax = %d (1/1000)\n", VsysMaxMailboxData.Fields.VsysMax));
    Status = MailboxWrite (MailboxCommand.InterfaceData, VsysMaxMailboxData.Data32, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "VR: Error Writing Vsys Maximum value. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }
  }

  ///
  /// Acoustic Noise Mitigation
  ///
  if (CpuPowerMgmtVrConfig->AcousticNoiseMitigation) {
    ///
    /// Hysteresis Window programming for Pcore Acoustic Noise Mitigation.
    ///
    if (CpuPowerMgmtVrConfig->PcoreHysteresisWindow != 0) {
      HysteresisWindowData.MailboxData = 0;
      MailboxCommand.InterfaceData  = 0;
      MailboxCommand.Fields.Command = MAILBOX_VR_CMD_ACOUSTIC_MITIGATION_CONFIG;
      MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SET_HYST_WINDOW_PARAMS;
      MailboxCommand.Fields.Param2 = HYSTERESIS_PCORE;
      HysteresisWindowData.Fields.Window = CpuPowerMgmtVrConfig->PcoreHysteresisWindow;
      DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = PCORE - MAILBOX_VR_SUBCMD_SET_HYST_WINDOW_PARAMS\n"));
      DEBUG ((DEBUG_INFO, "(MAILBOX) Hysteresis Window Mailboxdata for Pcore = %d ms\n", HysteresisWindowData.Fields.Window));
      Status = MailboxWrite (MailboxCommand.InterfaceData, HysteresisWindowData.MailboxData, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "Error Writing Hysteresis Window. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
      }
    }
    ///
    /// Hysteresis Window programming for Ecore Acoustic Noise Mitigation.
    ///
    if (CpuPowerMgmtVrConfig->EcoreHysteresisWindow != 0) {
      HysteresisWindowData.MailboxData = 0;
      MailboxCommand.InterfaceData  = 0;
      MailboxCommand.Fields.Command = MAILBOX_VR_CMD_ACOUSTIC_MITIGATION_CONFIG;
      MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SET_HYST_WINDOW_PARAMS;
      MailboxCommand.Fields.Param2 = HYSTERESIS_ECORE;
      HysteresisWindowData.Fields.Window = CpuPowerMgmtVrConfig->EcoreHysteresisWindow;
      DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = ECORE - MAILBOX_VR_SUBCMD_SET_HYST_WINDOW_PARAMS\n"));
      DEBUG ((DEBUG_INFO, "(MAILBOX) Hysteresis Window Mailboxdata for Ecore = %d ms\n", HysteresisWindowData.Fields.Window));
      Status = MailboxWrite (MailboxCommand.InterfaceData, HysteresisWindowData.MailboxData, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "Error Writing Hysteresis Window. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
      }
    }
  }

  ///
  /// RFI Mitigation
  ///
  if (CpuPowerMgmtVrConfig->DlvrRfiEnable) {
    MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
    RfiEmiControlRegister.Data = MmioRead32 (MchBar + R_RFI_EMI_CONTROL_0_0_0_MCHBAR_PCU);
    DEBUG ((DEBUG_INFO, "Read value from MchBar + R_RFI_EMI_CONTROL_0_0_0_MCHBAR_PCU register : 0x%X\n", RfiEmiControlRegister.Data));
    RfiEmiControlRegister.Field.SscFreqSprd = CpuPowerMgmtVrConfig->DlvrSpreadSpectrumPercentage;
    RfiEmiControlRegister.Field.SscMode = 0;  // 0: Down spread.
    RfiEmiControlRegister.Field.RfiFreqSelect = CpuPowerMgmtVrConfig->DlvrRfiFrequency;
    RfiEmiControlRegister.Field.RfiEnable = TRUE;
    RfiEmiControlRegister.Field.SpreadSpecControlLock = FALSE;
    DEBUG ((DEBUG_INFO, "Write value to MchBar + R_RFI_EMI_CONTROL_0_0_0_MCHBAR_PCU register : 0x%X\n", RfiEmiControlRegister.Data));
    MmioWrite32 (MchBar + R_RFI_EMI_CONTROL_0_0_0_MCHBAR_PCU, RfiEmiControlRegister.Data);
  }
}

/**
  Programs the VR mailbox using Override Values for the selected VR Address.

  @param[in]  CpuPowerMgmtVrConfig   Vr config block
  @param[in]  TempVrAddress          Selected VR Address.
  @param[in]  VrIndex                Selected VR Index.
  @param[in]  IccMax                 IccMax override value
  @param[in]  IccLimit               IccLimit override value
  @param[in]  TdcCurrentLimi         TdcCurrentLimit override value
  @param[in]  TdcTimeWindow          TdcTimeWindow override value
  @param[in]  AcLoadline             AcLoadline override value
  @param[in]  DcLoadline             DcLoadline override value
  @param[in]  VoltageLimit           VoltageLimit override value
**/
VOID
SetVrOverrideValues (
  IN CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig,
  IN UINT8                       TempVrAddress,
  IN UINTN                       VrIndex,
  IN UINT16                      *IccMax,
  IN UINT16                      *IccLimit,
  IN UINT16                      *TdcCurrentLimit,
  IN UINT32                      *TdcTimeWindow,
  IN UINT16                      *AcLoadline,
  IN UINT16                      *DcLoadline,
  IN UINT16                      *VoltageLimit
  )
{
  EFI_STATUS                  Status;
  UINT32                      MailboxData;
  PCODE_MAILBOX_INTERFACE     MailboxCommand;
  UINT32                      MailboxStatus;
  UINT64                      TempDcLoadline;
  UINT64                      TempAcLoadline;
  UINT8                       ConvertedTimeWindow;
  MAILBOX_DATA_VR_TDC_CONFIG  VrTdcMailboxData;
  MAILBOX_DATA_ACDC_LOADLINE  AcDcLoadlineMailboxData;
  MAILBOX_DATA_VOLTAGE_LIMIT  VrVoltageLimitMailboxData;

  ///
  /// AC/DC Loadline
  ///
  if (AcLoadline[VrIndex] != 0 || DcLoadline[VrIndex] != 0) {
    ///
    ///  Loadline is 1/100 mOhm units. Mailbox interface requires Loadline in U-4.20 Ohms format.
    ///  After multiplying by 2^20, we must divide the result by 100,000 to convert to Ohms.
    ///  Adding half of divisor to dividend to account for rounding errors in fixed point arithmetic.
    ///
    TempAcLoadline = MultU64x64 (AcLoadline[VrIndex], LShiftU64 (1, 20));
    TempAcLoadline = DivU64x32 (TempAcLoadline + 50000, 100000);
    TempDcLoadline = MultU64x64 (DcLoadline[VrIndex], LShiftU64 (1, 20));
    TempDcLoadline = DivU64x32 (TempDcLoadline + 50000, 100000);

    AcDcLoadlineMailboxData.Data32 = 0;
    AcDcLoadlineMailboxData.Fields.AcLoadLine = (UINT16) TempAcLoadline;
    AcDcLoadlineMailboxData.Fields.DcLoadLine = (UINT16) TempDcLoadline;
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_ACDC_LOADLINE;
    MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = WRITE_ACDC_LOADLINE_CMD\n"));
    DEBUG ((DEBUG_INFO, "(MAILBOX) AcLoadline            = %d (1/100 mOhms)\n", AcLoadline[VrIndex]));
    DEBUG ((DEBUG_INFO, "(MAILBOX) DcLoadline            = %d (1/100 mOhms)\n", DcLoadline[VrIndex]));
    Status = MailboxWrite (MailboxCommand.InterfaceData, AcDcLoadlineMailboxData.Data32, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "VR: Error Writing AC/DC Loadline. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }
  }

  ///
  /// Icc Max
  /// When Overclocking is enabled, OCMB interface is used to override ICC Max
  ///
  if ((IccMax[VrIndex] != 0) && (IsOcEnabled () == FALSE)) {
    MailboxData = (UINT32)IccMax[VrIndex];
    //
    // Config ICCMAX via PCODE Mailbox
    //
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_MAX_ICC;
    MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = WRITE_VR_ICC_MAX_CMD\n"));
    DEBUG ((DEBUG_INFO, "(MAILBOX) IccMax                = %d (1/4 A)\n", MailboxData));
    Status = MailboxWrite (MailboxCommand.InterfaceData, MailboxData, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "VR: Error Writing IccMax[%d]. EFI_STATUS = %r, Mailbox Status = %X\n", VrIndex, Status, MailboxStatus));
    }
  }

  ///
  /// Fast Vmode ICC Limit.
  ///
  //
  // Config Fast Vmode ICC Limit via PCODE Mailbox.
  // A value of 0 corresponds to feature disabled (no reactive protection).
  //
  if (IccLimit[VrIndex] != 0) {
    MailboxData = (UINT32)IccLimit[VrIndex];
    MailboxCommand.InterfaceData  = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1  = MAILBOX_VR_SUBCMD_SVID_SET_FAST_VMODE_ICC_LIMIT;
    MailboxCommand.Fields.Param2  = (TempVrAddress & VR_ADDRESS_MASK);
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = 0x%08x\n", MailboxCommand.InterfaceData));
    DEBUG ((DEBUG_INFO, "(MAILBOX) IccLimit              = %d (1/4 A)\n", MailboxData));
    Status = MailboxWrite (MailboxCommand.InterfaceData, MailboxData, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "VR: Error Writing IccLimit[%d]. EFI_STATUS = %r, Mailbox Status = %X\n", VrIndex, Status, MailboxStatus));
    }
  } else if ((IccLimit[VrIndex] == 0) && CpuPowerMgmtVrConfig->EnableFastVmode[VrIndex] == 1) {
    DEBUG ((DEBUG_WARN, "VR: Fast Vmode for VR Domain Index[%d] is not enabled due to no IccLimit[%d] setting!\n", VrIndex, VrIndex));
  }

  ///
  /// VR Voltage Limit
  /// -Mailbox Voltage Limit defined as U16.3.13, Range 0.001 - 7.999 V
  /// -Voltage Limit is defined in mV, Range 0 - 7999 mV (1/1000 increments)
  ///
  if (VoltageLimit[VrIndex] != 0) {
    VrVoltageLimitMailboxData.Data32 = 0;
    VrVoltageLimitMailboxData.Fields.VoltageLimit = ToUnsignedFixedPoint16 (VoltageLimit[VrIndex], 1000, 3, 13, NULL);
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_VOLTAGE_LIMIT;
    MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = WRITE_VR_VOLTAGE_LIMIT_CMD\n"));
    DEBUG ((DEBUG_INFO, "(MAILBOX) VrVoltageLimit        = %d mV\n", VoltageLimit[VrIndex]));
    Status = MailboxWrite (MailboxCommand.InterfaceData, VrVoltageLimitMailboxData.Data32, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "VR: Error Writing VR Voltage Limit[%d]. EFI_STATUS = %r, Mailbox Status = %X\n", VrIndex, Status, MailboxStatus));
    }
  }

  if (CpuPowerMgmtVrConfig->TdcEnable[VrIndex] == 1) {
    ///
    /// VR TDC Settings
    /// -Mailbox TDC Current Limit defined as U15.12.3, Range 0-4095A
    ///    -Policy defined in 1/8 A increments
    /// -Time window mailbox format, in binary, is xxyyyyy, time window = (1+X/4)*pow(2,Y)
    ///    -Set to 1ms default (mailbox value = 0000000b)
    ///

    ///
    /// Ensure Time Window Value is within the supported range.
    ///
    ConvertedTimeWindow = 0;
    if (TdcTimeWindow[VrIndex] != 0) {
      ConvertedTimeWindow = GetConvertedTime (TdcTimeWindow[VrIndex]);
    }
    DEBUG ((DEBUG_INFO, "(MAILBOX) ConvertedTimeWindow        = %xh \n", ConvertedTimeWindow));
    if (TdcCurrentLimit[VrIndex] != 0) {
      VrTdcMailboxData.Data32 = 0;
      VrTdcMailboxData.Fields.CurrentLimit = TdcCurrentLimit[VrIndex];
      VrTdcMailboxData.Fields.TdcEnable = CpuPowerMgmtVrConfig->TdcEnable[VrIndex];
      VrTdcMailboxData.Fields.TimeWindow = ConvertedTimeWindow;
      VrTdcMailboxData.Fields.UseIrms = CpuPowerMgmtVrConfig->TdcMode[VrIndex];
      VrTdcMailboxData.Fields.Lock = CpuPowerMgmtVrConfig->TdcLock[VrIndex];
      MailboxCommand.InterfaceData = 0;
      MailboxCommand.Fields.Command = MAILBOX_VR_CMD_WRITE_VR_TDC_CONFIG;
      MailboxCommand.Fields.Param1 = (TempVrAddress & VR_ADDRESS_MASK);

      DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = WRITE_VR_TDC_CONFIG_CMD\n"));
      DEBUG ((DEBUG_INFO, "(MAILBOX) TdcPowerLimit         = %d (1/8A)\n", TdcCurrentLimit[VrIndex]));
      Status = MailboxWrite (MailboxCommand.InterfaceData, VrTdcMailboxData.Data32, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "VR: Error Writing VR TDC Config. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
      }
    }
  }
}

/**
  Programs the VR mailbox without using Override Values for the selected VR Address.

  @param[in]  CpuPowerMgmtVrConfig   Vr config block
  @param[in]  TempVrAddress          Selected VR Address.
  @param[in]  VrIndex                Selected VR Index.
**/
VOID
SetVrNonOverrideValues (
  IN CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig,
  IN UINT8                       TempVrAddress,
  IN UINTN                       VrIndex
  )
{
  EFI_STATUS                  Status;
  PCODE_MAILBOX_INTERFACE     MailboxCommand;
  UINT32                      MailboxStatus;
  MAILBOX_DATA_IMON_CONFIG    ImonMailboxData;
  MAILBOX_DATA_PS_CUTOFF      PsCutoffMailboxData;
  MAILBOX_DATA_SLEW_RATE      VrSlewRateMailboxData;
  MAILBOX_DATA_FAST_PKGC_RAMP FastPkgcRampMailboxData;

  ///
  /// PS Cutoff Current
  ///
  PsCutoffMailboxData.Data32 = 0;
  ///
  /// Mailbox encoding is reversed for PS3/PS4 as 0 - Enable, 1 = Disable
  ///
  PsCutoffMailboxData.Fields.Ps3Disable = !CpuPowerMgmtVrConfig->Ps3Enable[VrIndex];
  DEBUG ((DEBUG_INFO, "(MAILBOX) PS3Enable            = %d\n", CpuPowerMgmtVrConfig->Ps3Enable[VrIndex]));

  PsCutoffMailboxData.Fields.Ps4Disable = !CpuPowerMgmtVrConfig->Ps4Enable[VrIndex];
  DEBUG ((DEBUG_INFO, "(MAILBOX) PS4Enable            = %d\n", CpuPowerMgmtVrConfig->Ps4Enable[VrIndex]));

  if (CpuPowerMgmtVrConfig->Ps1Threshold[VrIndex] != 0) {
    PsCutoffMailboxData.Fields.Ps1CutOff = CpuPowerMgmtVrConfig->Ps1Threshold[VrIndex];
    DEBUG ((DEBUG_INFO, "(MAILBOX) PS1Threshold         = %d (1/4 Amp)\n", CpuPowerMgmtVrConfig->Ps1Threshold[VrIndex]));
  }
  if (CpuPowerMgmtVrConfig->Ps2Threshold[VrIndex] != 0) {
    PsCutoffMailboxData.Fields.Ps2CutOff = CpuPowerMgmtVrConfig->Ps2Threshold[VrIndex];
    DEBUG ((DEBUG_INFO, "(MAILBOX) PS2Threshold         = %d (1/4 Amp)\n", CpuPowerMgmtVrConfig->Ps2Threshold[VrIndex]));
  }
  if ((CpuPowerMgmtVrConfig->Ps3Threshold[VrIndex] != 0) && (CpuPowerMgmtVrConfig->Ps3Enable[VrIndex] != 0)) {
    PsCutoffMailboxData.Fields.Ps3CutOff = CpuPowerMgmtVrConfig->Ps3Threshold[VrIndex];
    DEBUG ((DEBUG_INFO, "(MAILBOX) PS3Threshold         = %d (1/4 Amp)\n", CpuPowerMgmtVrConfig->Ps3Threshold[VrIndex]));
  }

  DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = WRITE_PS_CUTOFF_CMD\n"));
  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
  MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_PS_CUTOFF;
  MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
  Status = MailboxWrite (MailboxCommand.InterfaceData, PsCutoffMailboxData.Data32, &MailboxStatus);
  if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_ERROR, "VR: Error Writing PS Cutoff Current. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
  }

  /// -ImonOffset is defined as S16.7.8 fixed point
  /// -ImonSlope is defined as U16.1.15 fixed point
  /// -Policy Imon offset is defined in 1/1000 increments
  /// -Policy Imon slope is defined in 1/100 increments
  /// -Mailbox ImonOffset = (PlatPolicyImonOffset * 2^8)/1000
  /// -Mailbox ImonSlope = (PlatPolicyImonSlope * 2^15)/100
  /// -Adding half of divisor to dividend to account for rounding errors in fixed point arithmetic.
  ///
  if (CpuPowerMgmtVrConfig->ImonOffset[VrIndex] != 0 || CpuPowerMgmtVrConfig->ImonSlope[VrIndex] != 0) {
    ImonMailboxData.Data32 = 0;
    ImonMailboxData.Fields.ImonOffset = ToSignedFixedPoint16 (CpuPowerMgmtVrConfig->ImonOffset[VrIndex], 1000, 7, 8, NULL);
    ImonMailboxData.Fields.ImonSlope = ToUnsignedFixedPoint16 (CpuPowerMgmtVrConfig->ImonSlope[VrIndex], 100, 1, 15, NULL);
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_IMON_CONFIG;
    MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);

    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = WRITE_IMON_CONFIG_CMD\n"));
    DEBUG ((DEBUG_INFO, "(MAILBOX) ImonOffset            = %d (1/1000)\n", CpuPowerMgmtVrConfig->ImonOffset[VrIndex]));
    DEBUG ((DEBUG_INFO, "(MAILBOX) ImonSlope             = %d (1/100)\n", CpuPowerMgmtVrConfig->ImonSlope[VrIndex]));
    Status = MailboxWrite (MailboxCommand.InterfaceData, ImonMailboxData.Data32, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "VR: Error Writing IMON Config. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }
  }

  ///
  /// Core, GT and Atom domains support Slow Slew Rate and Fast Pkg Ramp features
  ///
  if ((VrIndex < 2) || (VrIndex == 3)) {
    ///
    /// Set Fast and Slow Slew Rate for Deep Package C States.
    ///
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
    MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_DISABLE_FAST_PKGC_RAMP;
    MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
    FastPkgcRampMailboxData.Data32 = 0;
    FastPkgcRampMailboxData.Fields.FastPkgcRampDisable = CpuPowerMgmtVrConfig->FastPkgCRampDisable[VrIndex];
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = WRITE_SVID_SET_DISABLE_FAST_PKGC_RAMP_CMD\n"));
    Status = MailboxWrite (MailboxCommand.InterfaceData, FastPkgcRampMailboxData.Data32, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "VR: Error Writing disable Fast Deep Package C States for VrIndex = %x. EFI_STATUS = %r, Mailbox Status = %X\n", VrIndex , Status, MailboxStatus));
    }

    if (CpuPowerMgmtVrConfig->FastPkgCRampDisable[VrIndex] == 1) {
      MailboxCommand.InterfaceData = 0;
      MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
      MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_SET_VR_SLEW_RATE;
      MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
      VrSlewRateMailboxData.Data32 = 0;
      VrSlewRateMailboxData.Fields.SlowSlewRate = CpuPowerMgmtVrConfig->SlowSlewRate[VrIndex];
      DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = WRITE_SVID_SET_VR_SLEW_RATE_CMD\n"));
      DEBUG ((DEBUG_INFO, "(MAILBOX) SlowSlewRate[%x] = %d (0: Fast/2</b>; 1: Fast/4; 2: Fast/8; 3: Fast/16)\n", VrIndex, CpuPowerMgmtVrConfig->SlowSlewRate[VrIndex]));
      Status = MailboxWrite (MailboxCommand.InterfaceData, VrSlewRateMailboxData.Data32, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "VR: Error Writing Slow Slew Rate for VrIndex = %x. EFI_STATUS = %r, Mailbox Status = %X\n", VrIndex , Status, MailboxStatus));
      }
    }
  }
}

/**
  Updates the Vr Config block with Intel default override values if needed

  @param[in]      CpuPowerMgmtVrConfig    CPU Power Management VR Config Block
  @param[in, out] OverrideIccMax          On input, the IccMax setting in CPU Power Management VR Config Block.
                                          On output, the override IccMax setting.
  @param[out]     IccMaxItrip             IccMaxItrip setting.
  @param[in, out] OverrideIccLimit        On input, the IccLimit setting in CPU Power Management VR Config Block.
                                          On output, the override IccLimit setting.
  @param[in, out] OverrideTdcCurrentLimit On input, the TdcCurrentLimit setting in CPU Power Management VR Config Block.
                                          On output, the override TdcCurrentLimit setting.
  @param[in, out] OverrideTdcTimeWindow   On input, the TdcTimeWindow setting in CPU Power Management VR Config Block.
                                          On output, the override TdcTimeWindow setting.
  @param[in, out] OverrideAcLoadline      On input, the AcLoadline setting in CPU Power Management VR Config Block.
                                          On output, the override AcLoadline setting.
  @param[in, out] OverrideDcLoadline      On input, the DcLoadline setting in CPU Power Management VR Config Block.
                                          On output, the override DcLoadline setting.
  @param[in, out] OverrideVoltageLimit    On input, the VoltageLimit setting in CPU Power Management VR Config Block.
                                          On output, the override VoltageLimit setting.
**/
EFI_STATUS
UpdateVrOverrides (
  IN     CPU_POWER_MGMT_VR_CONFIG *CpuPowerMgmtVrConfig,
  IN OUT UINT16                   *OverrideIccMax,
  OUT    UINT16                   *IccMaxItrip,
  IN OUT UINT16                   *OverrideIccLimit,
  IN OUT UINT16                   *OverrideTdcCurrentLimit,
  IN OUT UINT32                   *OverrideTdcTimeWindow,
  IN OUT UINT16                   *OverrideAcLoadline,
  IN OUT UINT16                   *OverrideDcLoadline,
  IN OUT UINT16                   *OverrideVoltageLimit,
  IN     VR_DOMAIN_TOPOLOGY       *VrDomainTopology
  )
{
  UINTN                                 Index;
  VOLTAGE_REGULATOR_PARAM               *VrData;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  CPU_PM_DATA                           *CpuPmData;
  BOOLEAN                               FastVmodeSupported;

  ASSERT (CpuPowerMgmtVrConfig    != NULL);
  ASSERT (OverrideIccMax          != NULL);
  ASSERT (IccMaxItrip             != NULL);
  ASSERT (OverrideIccLimit        != NULL);
  ASSERT (OverrideTdcCurrentLimit != NULL);
  ASSERT (OverrideTdcTimeWindow   != NULL);
  ASSERT (OverrideAcLoadline      != NULL);
  ASSERT (OverrideDcLoadline      != NULL);
  ASSERT (OverrideVoltageLimit    != NULL);

  GuidHob = GetFirstGuidHob (&gCpuPmDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Get CpuPmData failed\n"));
    return EFI_NOT_FOUND;
  }

  CpuPmData = (CPU_PM_DATA *) (GET_GUID_HOB_DATA (GuidHob));
  ASSERT (CpuPmData != NULL);

  ASSERT (CpuPmData->VrDataCount <= MAX_NUM_VRS);
  for (Index = 0; Index < CpuPmData->VrDataCount; Index++) {
    FastVmodeSupported = IsFastVmodeIccLimitSupport (VrDomainTopology->VrAddress[Index]);
    VrData = &CpuPmData->VrData[Index];

    ///
    /// ICC Max Overrides
    ///
    if (OverrideIccMax[Index] == 0){
      if ((FastVmodeSupported) &&
          (VrData->IccMaxFvm != 0) &&
          (CpuPowerMgmtVrConfig->EnableFastVmode[Index] == 1)) {
        //
        // Update ICCMAX in FVM enable case.
        //
        OverrideIccMax[Index] = VrData->IccMaxFvm;
      } else {
        //
        // Update ICCMAX in FVM disable case.
        //
        OverrideIccMax[Index] = VrData->IccMax;
      }
    }

    ///
    /// Record the IccMaxItrip.
    ///
    IccMaxItrip[Index] = VrData->IccItripFvm;

    ///
    /// ICC Limit Overrides
    ///
    if (OverrideIccLimit[Index] == 0) {
      if ((FastVmodeSupported) &&
          (VrData->IccItripFvm != 0) &&
          (CpuPowerMgmtVrConfig->EnableFastVmode[Index] == 1)) {
        //
        // Override the IccLimit to the default defined in VR.
        //
        OverrideIccLimit[Index] = VrData->IccItripFvm;
      }
    }

    ///
    /// VR TDC Overrides
    ///
    if (CpuPowerMgmtVrConfig->TdcMode[Index] == 1) {
      if (OverrideTdcCurrentLimit[Index] == 0) {
        OverrideTdcCurrentLimit[Index] = VrData->TdcLimitIrms;
      }
      if (OverrideTdcTimeWindow[Index] == 0) {
        OverrideTdcTimeWindow[Index] = VrData->TdcTimeLimitIrms * 1000;
      }
    } else {
      if (OverrideTdcCurrentLimit[Index] == 0) {
        OverrideTdcCurrentLimit[Index] = VrData->TdcLimit;
      }
      if (OverrideTdcTimeWindow[Index] == 0) {
        OverrideTdcTimeWindow[Index] = VrData->TdcTimeLimit * 1000;
      }
    }

    ///
    /// AC/DC Loadlines
    ///
    if (OverrideAcLoadline[Index] == 0) {
      OverrideAcLoadline[Index] = VrData->AcLoadLine;
    }
    if (OverrideDcLoadline[Index] == 0) {
      OverrideDcLoadline[Index] = VrData->DcLoadLine;
    }

    ///
    /// VoltageLimit Overrides
    ///
    if (OverrideVoltageLimit[Index] == 0) {
      OverrideVoltageLimit[Index] = VrData->VoltageLimit;
    }
  }
  return EFI_SUCCESS;
}

/**
  Programs the VR mailbox for each VR.

  @param[in]  CpuPowerMgmtVrConfig   Vr config block
**/
VOID
SetVrPerVr(
  IN CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig
  )
{
  VR_DATA_HOB                 VrDataHob;
  VR_DOMAIN_TOPOLOGY          VrDomainTopology;
  UINTN                       VrIndex;
  UINT8                       TempVrAddress;
  UINT8                       SvidEnabled;
  VOID                        *Hob;
  UINT16                      IccMax[MAX_NUM_VRS];
  UINT16                      IccMaxItrip[MAX_NUM_VRS];
  UINT16                      IccLimit[MAX_NUM_VRS];
  UINT16                      TdcCurrentLimit[MAX_NUM_VRS];
  UINT32                      TdcTimeWindow[MAX_NUM_VRS];
  UINT16                      AcLoadline[MAX_NUM_VRS];
  UINT16                      DcLoadline[MAX_NUM_VRS];
  UINT16                      VoltageLimit[MAX_NUM_VRS];
  UINTN                       MaxNumVrs;

  DEBUG ((DEBUG_INFO, "VR: Update VR overrides\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_VR_UPDATE_OVEERRIDES); //PostCode (0xC41)
  CopyMem (IccMax, CpuPowerMgmtVrConfig->IccMax, sizeof (IccMax));
  ZeroMem (IccMaxItrip, sizeof (IccMaxItrip));
  CopyMem (IccLimit, CpuPowerMgmtVrConfig->IccLimit, sizeof (IccLimit));
  CopyMem (TdcCurrentLimit, CpuPowerMgmtVrConfig->TdcCurrentLimit, sizeof (TdcCurrentLimit));
  CopyMem (TdcTimeWindow, CpuPowerMgmtVrConfig->TdcTimeWindow, sizeof (TdcTimeWindow));
  CopyMem (AcLoadline, CpuPowerMgmtVrConfig->AcLoadline, sizeof (AcLoadline));
  CopyMem (DcLoadline, CpuPowerMgmtVrConfig->DcLoadline, sizeof (DcLoadline));
  CopyMem (VoltageLimit, CpuPowerMgmtVrConfig->VrVoltageLimit, sizeof (VoltageLimit));

  ///
  /// Get CPU VR topology domains which are fru dependent
  ///
  DEBUG ((DEBUG_INFO, "VR: Get CPU VR Topology\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_GETCPU_VR_TOPOLOGY); //PostCode (0xC42)
  GetCpuVrTopologyFru (&VrDomainTopology);

  UpdateVrOverrides (
    CpuPowerMgmtVrConfig,
    IccMax,
    IccMaxItrip,
    IccLimit,
    TdcCurrentLimit,
    TdcTimeWindow,
    AcLoadline,
    DcLoadline,
    VoltageLimit,
    &VrDomainTopology
    );

  ///
  /// Initialize VR_DATA_HOB hob.
  ///
  ZeroMem (&VrDataHob, sizeof (VR_DATA_HOB));

  MaxNumVrs = GetMaxNumVrsFru ();

  //
  // Avoid array overflow while looping through VR arrays.
  //
  ASSERT (MaxNumVrs <= ARRAY_SIZE (VrDomainTopology.VrAddress));

  for (VrIndex = 0; VrIndex < MaxNumVrs; VrIndex++) {
    TempVrAddress = VrDomainTopology.VrAddress[VrIndex];
    SvidEnabled = VrDomainTopology.SvidEnabled[VrIndex];

    DEBUG ((DEBUG_INFO, " VR Address for Vr Index %x = 0x%x\n", VrIndex, VrDomainTopology.VrAddress[VrIndex]));
    DEBUG ((DEBUG_INFO, " VR SvidEnabled for Vr Index %x = 0x%x\n", VrIndex, VrDomainTopology.SvidEnabled[VrIndex]));

    DEBUG ((DEBUG_INFO, " Override IccMax[%x] = %d\n", VrIndex, IccMax[VrIndex]));
    DEBUG ((DEBUG_INFO, " IccMaxItrip[%x] = %d\n", VrIndex, IccMaxItrip[VrIndex]));
    DEBUG ((DEBUG_INFO, " Override IccLimit[%x] = %d\n", VrIndex, IccLimit[VrIndex]));
    DEBUG ((DEBUG_INFO, " Override TdcCurrentLimit[%x] = %d\n", VrIndex, TdcCurrentLimit[VrIndex]));
    DEBUG ((DEBUG_INFO, " Override TdcTimeWindow[%x] = %d\n", VrIndex, TdcTimeWindow[VrIndex]));
    DEBUG ((DEBUG_INFO, " Override AcLoadline[%x] = %d\n", VrIndex, AcLoadline[VrIndex]));
    DEBUG ((DEBUG_INFO, " Override DcLoadline[%x] = %d\n", VrIndex, DcLoadline[VrIndex]));
    DEBUG ((DEBUG_INFO, " Override VoltageLimit[%x] = %d\n", VrIndex, VoltageLimit[VrIndex]));

    ///
    /// Update Vr IccMaxItrip for VR Data HOB
    ///
    VrDataHob.IccMaxItrip[VrIndex] = IccMaxItrip[VrIndex];

    ///
    /// Update FastVmodeSupported for VR Data HOB
    ///
    VrDataHob.FastVmodeSupported[VrIndex] = (UINT8) IsFastVmodeIccLimitSupport (TempVrAddress);
    DEBUG ((DEBUG_INFO, " FastVmodeSupported[%d] = %d\n", VrIndex, VrDataHob.FastVmodeSupported[VrIndex]));

    ///
    /// Update Vr Address for VR Data HOB
    ///
    VrDataHob.VrAddress[VrIndex] = VrDomainTopology.VrAddress[VrIndex];
    VrDataHob.SvidEnabled = VrDataHob.SvidEnabled + (VrDomainTopology.SvidEnabled[VrIndex] << VrIndex);

    if (CpuPowerMgmtVrConfig->VrConfigEnable[VrIndex] == 1 && SvidEnabled == 1) {
      SetVrNonOverrideValues (CpuPowerMgmtVrConfig, TempVrAddress, VrIndex);
      SetVrNonOverrideValuesFru (CpuPowerMgmtVrConfig, TempVrAddress, VrIndex);
      SetVrOverrideValues (
          CpuPowerMgmtVrConfig,
          TempVrAddress,
          VrIndex,
          IccMax,
          IccLimit,
          TdcCurrentLimit,
          TdcTimeWindow,
          AcLoadline,
          DcLoadline,
          VoltageLimit
          );
    }
    SetVrMiscConfig (CpuPowerMgmtVrConfig, TempVrAddress, VrIndex);
  }

  ///
  /// Update Max value for CPU Data HOB
  ///
  VrDataHob.MaxNumVrs = (UINT8) MaxNumVrs;

  ///
  /// Build Vr Data HOB
  ///
  Hob = BuildGuidDataHob (
          &gVrDataHobGuid,
          &VrDataHob,
          (UINTN) sizeof (VR_DATA_HOB)
          );
  ASSERT (Hob != NULL);

}

/**
  Programs the VR parameters for the external VR's which support SVID communication.

  @param[in]  SiPreMemPolicyPpi   The Silicon PreMem Policy PPI instance
**/
VOID
EFIAPI
ConfigureSvidVrs (
  IN OUT SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                  Status;
  CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig;

  DEBUG ((DEBUG_INFO, "ConfigureSvidVrs Start \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_CFG_SVIDVRS_START); //PostCode (0xC40)

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuPowerMgmtVrConfigGuid, (VOID *)&CpuPowerMgmtVrConfig);
  if (CpuPowerMgmtVrConfig == NULL) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  //
  // Begin VR set flow
  //
  SetVrCommon (CpuPowerMgmtVrConfig);
  SetVrPerVr (CpuPowerMgmtVrConfig);

  DEBUG ((DEBUG_INFO, "ConfigureSvidVrs Done\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_CFG_SVIDVRS_DONE); //PostCode (0xC4E)
}

/**
  Send Early PM Configuration done through mailbox
**/
VOID
EFIAPI
SendPowerDeliveryDone (
  VOID
  )
{
  EFI_STATUS Status;
  UINT32                        MailboxStatus;
  UINT32                        MailboxData;
  PCODE_MAILBOX_INTERFACE       MailboxCommand;

  Status                        = EFI_SUCCESS;
  MailboxStatus                 = PCODE_MAILBOX_CC_SUCCESS;

  ///
  /// Early VR done configuration
  ///
  MailboxData = 0;
  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.Command = MAILBOX_VR_CMD_EARLY_VR_DONE;
  MailboxCommand.Fields.Param1 = 0;
  MailboxCommand.Fields.Param2 = 0;
  DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = MAILBOX_VR_CMD_EARLY_VR_DONE\n"));
  Status = MailboxWrite (MailboxCommand.InterfaceData, MailboxData, &MailboxStatus);
  if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_ERROR, "VR: Error Writing Early PM done configuration. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
  }
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_SEND_EARLY_PM_DONE); //PostCode (0xC4F)
}
