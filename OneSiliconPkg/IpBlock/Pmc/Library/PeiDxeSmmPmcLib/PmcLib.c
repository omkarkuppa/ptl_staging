/** @file
  PCH PMC Library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PmcPrivateLib.h>
#include <Defines/PchReservedResources.h>

/**
  Get PCH ACPI base address.

  @retval Address                   Address of PWRM base address.
**/
UINT16
PmcGetAcpiBase (
  VOID
  )
{
  return PcdGet16 (PcdAcpiBaseAddress);
}

/**
  Get PCH PWRM base address.

  @retval Address                   Address of PWRM base address.
**/
UINTN
PmcGetPwrmBase (
  VOID
  )
{
  return PCH_PWRM_BASE_ADDRESS;
}

/**
  Get PCH Die PMC ACPI base.

  @retval Address                   Address of MTL PCH Acpi base address.
**/
UINT16
PchDiePmcGetAcpiBase (
  VOID
  )
{
  return 0;
}

/**
  Ensure Pm1En has synchronized between SOC and PCH
  @param[in] SocAcpiBaseAddr  AcpiBaseAddr of SOC
  @param[in] PchAcpiBaseAddr  AcpiBaseAddr of PCH

  @retval BOOLEAN             True: Synchronized
                              False : Not synchronized
**/
BOOLEAN
PchDieIsPmcPm1EnSync (
  UINT16                        SocAcpiBaseAddr,
  UINT16                        PchAcpiBaseAddr
  )
{
  return TRUE;
}

/**
  This function enables Power Button SMI
**/
VOID
PmcEnablePowerButtonSmi (
  VOID
  )
{
  IoOr16 (PmcGetAcpiBase () + R_ACPI_IO_PM1_EN, B_ACPI_IO_PM1_EN_PWRBTN);
}

/**
  This function disables Power Button SMI
**/
VOID
PmcDisablePowerButtonSmi (
  VOID
  )
{
  IoAnd16 (PmcGetAcpiBase () + R_ACPI_IO_PM1_EN, (UINT16)~B_ACPI_IO_PM1_EN_PWRBTN);
}

/**
  This function reads PM Timer Count driven by 3.579545 MHz clock

  @retval PM Timer Count
**/
UINT32
PmcGetTimerCount (
  VOID
  )
{
  return IoRead32 (PmcGetAcpiBase () + R_ACPI_IO_PM1_TMR) & B_ACPI_IO_PM1_TMR_TMR_VAL;
}

/**
  Get Sleep Type that platform has waken from

  @retval SleepType                Sleep Type
**/
PMC_SLEEP_STATE
PmcGetSleepTypeAfterWake (
  VOID
  )
{
  UINT16  AcpiBase;
  UINT32  PmconA;

  AcpiBase = PmcGetAcpiBase ();
  PmconA   = MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A);

  DEBUG ((DEBUG_INFO, "PWRM_PMCON_A = 0x%x\n", PmconA));

  //
  // If Global Reset Status, Power Failure. Host Reset Status bits are set, return S5 State
  //
  if ((PmconA & (B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS | B_PMC_PWRM_GEN_PMCON_A_PF | B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS)) != 0) {
    return PmcNotASleepState;
  }

  if (IoRead16 (AcpiBase + R_ACPI_IO_PM1_STS) & B_ACPI_IO_PM1_STS_WAK) {
    switch (IoRead16 (AcpiBase + R_ACPI_IO_PM1_CNT) & B_ACPI_IO_PM1_CNT_SLP_TYP) {
      case V_ACPI_IO_PM1_CNT_S0:
        return PmcInS0State;

      case V_ACPI_IO_PM1_CNT_S1:
        return PmcS1SleepState;

      case V_ACPI_IO_PM1_CNT_S3:
        return PmcS3SleepState;

      case V_ACPI_IO_PM1_CNT_S4:
        return PmcS4SleepState;

      case V_ACPI_IO_PM1_CNT_S5:
        return PmcS5SleepState;

      default:
        ASSERT (FALSE);
        return PmcUndefinedState;
    }
  } else {
    return PmcNotASleepState;
  }
}

/**
  Clear PMC Wake Status
**/
VOID
PmcClearWakeStatus (
  VOID
  )
{
  IoWrite16 (PmcGetAcpiBase () + R_ACPI_IO_PM1_STS, B_ACPI_IO_PM1_STS_WAK);
}

/**
  Configure sleep state

  @param[in] SleepState         S0/S1/S3/S4/S5, refer to PMC_SLEEP_STATE
**/
VOID
PmcSetSleepState (
  PMC_SLEEP_STATE  SleepState
  )
{
  UINT16  Data16;

  switch (SleepState) {
    case PmcInS0State:
      Data16 = V_ACPI_IO_PM1_CNT_S0;
      break;

    case PmcS1SleepState:
      Data16 = V_ACPI_IO_PM1_CNT_S1;
      break;

    case PmcS3SleepState:
      Data16 = V_ACPI_IO_PM1_CNT_S3;
      break;

    case PmcS4SleepState:
      Data16 = V_ACPI_IO_PM1_CNT_S4;
      break;

    case PmcS5SleepState:
      Data16 = V_ACPI_IO_PM1_CNT_S5;
      break;

    default:
      ASSERT (FALSE);
      return;

  }
  IoAndThenOr16 (PmcGetAcpiBase () + R_ACPI_IO_PM1_CNT, (UINT16) ~B_ACPI_IO_PM1_CNT_SLP_TYP, Data16);
}

/**
  Check if platform boots after shutdown caused by power button override event

  @retval  TRUE   Power Button Override occurred in last system boot
  @retval  FALSE  Power Button Override didn't occur
**/
BOOLEAN
PmcIsPowerButtonOverrideDetected (
  VOID
  )
{
  return ((IoRead16 (PmcGetAcpiBase () + R_ACPI_IO_PM1_STS) & B_ACPI_IO_PM1_STS_PRBTNOR) != 0);
}

/**
  This function sets tPCH25 timing

  @param[in] TimingValue       tPCH25 timing value (10us, 100us, 1ms, 10ms)
**/
VOID
PmcSetTPch25Timing (
  IN PMC_TPCH25_TIMING    TimingValue
  )
{
  ASSERT (TimingValue <= PmcTPch25_10ms);

  MmioAndThenOr32 (
    (UINTN) PmcGetPwrmBase () + R_PMC_PWRM_PM_CFG,
    (UINT32)~(B_PMC_PWRM_PM_CFG_TIMING_T581),
    TimingValue
    );
}

/**
  This function checks if RTC Power Failure occurred by
  reading RTC_PWR_FLR bit

  @retval RTC Power Failure state: TRUE  - Battery is always present.
                                   FALSE - CMOS is cleared.
**/
BOOLEAN
EFIAPI
PmcIsRtcBatteryGood (
  VOID
  )
{
  return ((MmioRead8 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B) & B_PMC_PWRM_GEN_PMCON_B_RPS) == 0);
}

/**
  This function checks if Power Failure occurred by
  reading PWR_FLR bit

  @retval Power Failure state
**/
BOOLEAN
PmcIsPowerFailureDetected (
  VOID
  )
{
  return ((MmioRead16 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A) & B_PMC_PWRM_GEN_PMCON_A_PF) != 0);
}

/**
  This function checks if Power Failure occurred by
  reading SUS_PWR_FLR bit

  @retval SUS Power Failure state
**/
BOOLEAN
PmcIsSusPowerFailureDetected (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A) & B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR) != 0);
}

/**
  This function clears Power Failure status (PWR_FLR)
**/
VOID
PmcClearPowerFailureStatus (
  VOID
  )
{
  //
  // Write 1 to clear PWR_FLR
  // Avoid clearing other W1C bits
  //
  MmioAndThenOr8 (
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 1,
    (UINT8) ~(B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS >> 8),
    B_PMC_PWRM_GEN_PMCON_A_PF >> 8
    );
}

/**
  This function clears Global Reset status (GBL_RST_STS)
**/
VOID
PmcClearGlobalResetStatus (
  VOID
  )
{
  //
  // Write 1 to clear GBL_RST_STS
  // Avoid clearing other W1C bits
  //
  MmioAndThenOr8 (
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 3,
    (UINT8) ~0,
    B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS >> 24
    );
}

/**
  This function clears Host Reset status (HOST_RST_STS)
**/
VOID
PmcClearHostResetStatus (
  VOID
  )
{
  //
  // Write 1 to clear HOST_RST_STS
  // Avoid clearing other W1C bits
  //
  MmioAndThenOr8 (
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 1,
    (UINT8) ~(B_PMC_PWRM_GEN_PMCON_A_PF >> 8),
    B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS >> 8
    );
}

/**
  This function clears SUS Power Failure status (SUS_PWR_FLR)
**/
VOID
PmcClearSusPowerFailureStatus (
  VOID
  )
{
  //
  // BIOS clears this bit by writing a '1' to it.
  // Take care of other fields, so we don't clear them accidentally.
  //
  MmioAndThenOr8 (
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 2,
    (UINT8) ~(B_PMC_PWRM_GEN_PMCON_A_MS4V >> 16),
    B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR >> 16
    );
}

/**
  This function sets state to which platform will get after power is reapplied

  @param[in] PowerStateAfterG3          0: S0 state (boot)
                                        1: S5/S4 State
                                        2: Depend on the platform state when the G3 occured
**/
VOID
PmcSetPlatformStateAfterPowerFailure (
  IN UINT8 PowerStateAfterG3
  )
{
  UINTN                PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  if (PowerStateAfterG3 == AFTER_G3_S5_S4) {
    MmioOr8 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A, B_PMC_PWRM_GEN_PMCON_A_AG3E);
  } else {
    MmioAnd8 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A, (UINT8)~B_PMC_PWRM_GEN_PMCON_A_AG3E);
  }

  if (PowerStateAfterG3 == AFTER_G3_LAST_STATE) {
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_PM_CFG, B_PMC_PWRM_PM_CFG_AG3_LS_EN);
  } else {
    MmioAnd32 (PchPwrmBase + R_PMC_PWRM_PM_CFG, (UINT32)~B_PMC_PWRM_PM_CFG_AG3_LS_EN);
  }
}

/**
  This function sets whether previous platform state will be considered when determining whether to power up after Type 8 Global Reset

  @param[in] PowerStateAfterT8Gr        0: Previous state will not be considered
                                        1: Previous state will be considered
**/
VOID
PmcSetPlatformStateAfterT8GlobalReset (
  IN UINT8 PowerStateAfterT8Gr
  )
{
  UINTN                PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  if (PowerStateAfterT8Gr) {
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_PM_CFG, B_PMC_PWRM_PM_CFG_A8GR_LS_EN);
  } else {
    MmioAnd32 (PchPwrmBase + R_PMC_PWRM_PM_CFG, (UINT32)~B_PMC_PWRM_PM_CFG_A8GR_LS_EN);
  }
}

/**
  This function will set the DISB - DRAM Initialization Scratchpad Bit.
**/
VOID
PmcSetDramInitScratchpad (
  VOID
  )
{
  //
  // Set B_CNL_PCH_PWRM_GEN_PMCON_A_DISB.
  // NOTE: Byte access and not clear BIT18 and BIT16 (W1C bits)
  //
  MmioAndThenOr8 (
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 2,
    (UINT8) ~((B_PMC_PWRM_GEN_PMCON_A_MS4V | B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR) >> 16),
    B_PMC_PWRM_GEN_PMCON_A_DISB >> 16
    );
}

/**
  This function sends PMC IPC CrashLog Discovery Command

  @param[out] DiscoveryBuffer     Pointer to CrashLog Discovery Data

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_INVALID_PARAMETER   NULL argument
  @retval EFI_DEVICE_ERROR        Crash Log Discovery command failed with an error
  @retval EFI_TIMEOUT             Crash Log Discovery command did not complete
**/
EFI_STATUS
PmcCrashLogDiscovery (
  OUT PMC_IPC_DISCOVERY_BUF     *DiscoveryBuffer
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;
  PMC_IPC_COMMAND_BUFFER    Rbuf;

  if (DiscoveryBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  ZeroMem (&Rbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CRASHLOG, V_PMC_PWRM_IPC_CMD_CMD_ID_CRASHLOG_DISCOVERY, 4, &Wbuf, &Rbuf);
  if (!EFI_ERROR (Status)) {
    DiscoveryBuffer->Uint64 = (LShiftU64 (Rbuf.Buf1, 32) | (Rbuf.Buf0));
  }
  return Status;
}

/**
  This function sends PMC IPC CrashLog Disable Command

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log Disable command failed with an error
  @retval EFI_TIMEOUT             Crash Log Disable command did not complete
**/
EFI_STATUS
PmcCrashLogDisable (
  VOID
  )
{
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  return PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CRASHLOG, V_PMC_PWRM_IPC_CMD_CMD_ID_CRASHLOG_DISABLE, 4, &Wbuf, NULL);
}

/**
  This function sends PMC IPC to clear CrashLog from PMC SSRAM area

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log Clear command failed with an error
  @retval EFI_TIMEOUT             Crash Log Clear command did not complete
**/
EFI_STATUS
PmcCrashLogClear (
  VOID
  )
{
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  return PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CRASHLOG, V_PMC_PWRM_IPC_CMD_CMD_ID_CRASHLOG_ERASE, 4, &Wbuf, NULL);
}

/**
  This function sends PMC IPC to populate CrashLog on all reboot. The SSRAM area will be cleared on G3 by PMC automatically

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log command failed with an error
  @retval EFI_TIMEOUT             Crash Log command did not complete
**/
EFI_STATUS
PmcCrashLogOnAllReset (
  VOID
  )
{
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  return PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CRASHLOG, V_PMC_PWRM_IPC_CMD_CMD_ID_CRASHLOG_ON_RESET, 4, &Wbuf, NULL);
}

/**
  This function sends PMC IPC Crashlog Re-arm command

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log command failed with an error
  @retval EFI_TIMEOUT             Crash Log command did not complete
**/
EFI_STATUS
PmcCrashLogReArm (
  VOID
  )
{
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  return PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CRASHLOG, V_PMC_PWRM_IPC_CMD_CMD_ID_CRASHLOG_RE_ARM, 4, &Wbuf, NULL);
}

/**
  Program the crashlog global reset trigger enable mask

  @param[in] Buf0  Global reset mask
  @param[in] Buf1  Global reset mask 1
  @param[in] Buf2  Global reset mask 2

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcCrashLogProgramTrigger (
  IN UINT32  Buf0,
  IN UINT32  Buf1,
  IN UINT32  Buf2
  )
{
  PMC_IPC_COMMAND_BUFFER  Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = Buf0;
  Wbuf.Buf1 = Buf1;
  Wbuf.Buf2 = Buf2;
  return PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CRASHLOG, V_PMC_PWRM_IPC_CMD_CMD_ID_CRASHLOG_PROGRAM_TRIGGER_ENABLES, 12, &Wbuf, NULL);
}

/**
  Read the PMC global reset mask

  @param[out] Buf0  Global reset mask
  @param[out] Buf1  Global reset mask 1
  @param[out] Buf2  Global reset mask 2

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcCrashLogReadTrigger (
  OUT UINT32  *Buf0,
  OUT UINT32  *Buf1,
  OUT UINT32  *Buf2
  )
{
  PMC_IPC_COMMAND_BUFFER  Wbuf;
  PMC_IPC_COMMAND_BUFFER  Rbuf;
  EFI_STATUS              Status;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  ZeroMem (&Rbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CRASHLOG, V_PMC_PWRM_IPC_CMD_CMD_ID_CRASHLOG_READ_TRIGGER_ENABLES, 12, &Wbuf, &Rbuf);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Buf0 != NULL) {
    *Buf0 = Rbuf.Buf0;
  }
  if (Buf1 != NULL) {
    *Buf1 = Rbuf.Buf1;
  }
  if (Buf2 != NULL) {
    *Buf2 = Rbuf.Buf2;
  }
  return EFI_SUCCESS;
}

/**
  Program the crashlog global reset trigger enable mask

  @param[in] PmcGlobalResetMask   Global reset mask
  @param[in] PmcGlobalResetMask1  Global reset mask 1

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcCrashLogProgramGlobalResetTriggerEnableMask (
  IN PMC_GLOBAL_RESET_MASK   PmcGlobalResetMask,
  IN PMC_GLOBAL_RESET_MASK1  PmcGlobalResetMask1
  )
{
  PMC_IPC_COMMAND_BUFFER  Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = PmcGlobalResetMask.Value;
  Wbuf.Buf1 = PmcGlobalResetMask1.Value;
  return PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CRASHLOG, V_PMC_PWRM_IPC_CMD_CMD_ID_CRASHLOG_PROGRAM_TRIGGER_ENABLES, 2, &Wbuf, NULL);
}

/**
  Restore the default value of the global reset trigger enable mask

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcCrashLogRestoreGlobalResetTriggerMask (
  VOID
  )
{
  PMC_IPC_COMMAND_BUFFER  Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = 0xFFFFFFFF;
  Wbuf.Buf1 = 0xFFFFFFFF;
  return PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CRASHLOG, V_PMC_PWRM_IPC_CMD_CMD_ID_CRASHLOG_PROGRAM_TRIGGER_ENABLES, 2, &Wbuf, NULL);
}

/**
  Read the PMC global reset mask

  @param[out] PmcGlobalResetMask   Global reset mask
  @param[out] PmcGlobalResetMask1  Global reset mask 1

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcCrashLogReadGlobalResetTriggerMask (
  OUT PMC_GLOBAL_RESET_MASK   *PmcGlobalResetMask,
  OUT PMC_GLOBAL_RESET_MASK1  *PmcGlobalResetMask1
  )
{
  PMC_IPC_COMMAND_BUFFER  Wbuf;
  PMC_IPC_COMMAND_BUFFER  Rbuf;
  EFI_STATUS              Status;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  ZeroMem (&Rbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CRASHLOG, V_PMC_PWRM_IPC_CMD_CMD_ID_CRASHLOG_READ_TRIGGER_ENABLES, 2, &Wbuf, &Rbuf);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PmcGlobalResetMask->Value = Rbuf.Buf0;
  PmcGlobalResetMask1->Value = Rbuf.Buf1;
  return EFI_SUCCESS;
}

/**
  This function sends PMC IPC CrashLog Discovery Command

  @param[out] DiscoveryBuffer     Pointer to CrashLog Discovery Data

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_INVALID_PARAMETER   NULL argument
  @retval EFI_DEVICE_ERROR        Crash Log Discovery command failed with an error
  @retval EFI_TIMEOUT             Crash Log Discovery command did not complete
**/
EFI_STATUS
PchPmcCrashLogDiscovery (
  OUT PMC_IPC_DISCOVERY_BUF     *DiscoveryBuffer
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This function sends PMC IPC CrashLog Disable Command

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log Disable command failed with an error
  @retval EFI_TIMEOUT             Crash Log Disable command did not complete
**/
EFI_STATUS
PchPmcCrashLogDisable (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This function sends PMC IPC to clear CrashLog from PMC SSRAM area

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log Clear command failed with an error
  @retval EFI_TIMEOUT             Crash Log Clear command did not complete
**/
EFI_STATUS
PchPmcCrashLogClear (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This function sends PMC IPC to populate CrashLog on all reboot. The SSRAM area will be cleared on G3 by PMC automatically

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log command failed with an error
  @retval EFI_TIMEOUT             Crash Log command did not complete
**/
EFI_STATUS
PchPmcCrashLogOnAllReset (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This function sends PMC IPC Crashlog Re-arm command

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log command failed with an error
  @retval EFI_TIMEOUT             Crash Log command did not complete
**/
EFI_STATUS
PchPmcCrashLogReArm (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Program the crashlog global reset trigger enable mask

  @param[in] Buf0  Global reset mask
  @param[in] Buf1  Global reset mask 1
  @param[in] Buf2  Global reset mask 2

  @retval Result of the IPC command send
**/
EFI_STATUS
PchPmcCrashLogProgramTrigger (
  IN UINT32  Buf0,
  IN UINT32  Buf1,
  IN UINT32  Buf2
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Read the PMC global reset mask

  @param[out] Buf0  Global reset mask
  @param[out] Buf1  Global reset mask 1
  @param[out] Buf2  Global reset mask 2

  @retval Result of the IPC command send
**/
EFI_STATUS
PchPmcCrashLogReadTrigger (
  OUT UINT32  *Buf0,
  OUT UINT32  *Buf1,
  OUT UINT32  *Buf2
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Program the crashlog global reset trigger enable mask

  @param[in] PmcGlobalResetMask   Global reset mask
  @param[in] PmcGlobalResetMask1  Global reset mask 1

  @retval Result of the IPC command send
**/
EFI_STATUS
PchPmcCrashLogProgramGlobalResetTriggerEnableMask (
  IN PMC_GLOBAL_RESET_MASK   PmcGlobalResetMask,
  IN PMC_GLOBAL_RESET_MASK1  PmcGlobalResetMask1
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Restore the default value of the global reset trigger enable mask

  @retval Result of the IPC command send
**/
EFI_STATUS
PchPmcCrashLogRestoreGlobalResetTriggerMask (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Read the PMC global reset mask

  @param[out] PmcGlobalResetMask   Global reset mask
  @param[out] PmcGlobalResetMask1  Global reset mask 1

  @retval Result of the IPC command send
**/
EFI_STATUS
PchPmcCrashLogReadGlobalResetTriggerMask (
  OUT PMC_GLOBAL_RESET_MASK   *PmcGlobalResetMask,
  OUT PMC_GLOBAL_RESET_MASK1  *PmcGlobalResetMask1
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Check global SMI enable is set

  @retval TRUE  Global SMI enable is set
          FALSE Global SMI enable is not set
**/
BOOLEAN
PmcIsGblSmiEn (
  VOID
  )
{
  return !!(IoRead32 (PmcGetAcpiBase () + R_ACPI_IO_SMI_EN) & B_ACPI_IO_SMI_EN_GBL_SMI_EN);
}

/**
  This function checks if SMI Lock is set

  @retval SMI Lock state
**/
BOOLEAN
PmcIsSmiLockSet (
  VOID
  )
{
  return ((MmioRead8 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B) & B_PMC_PWRM_GEN_PMCON_B_SMI_LOCK) != 0);
}

/**
  This function checks if Debug Mode is locked

  @retval Debug Mode Lock state
**/
BOOLEAN
PmcIsDebugModeLocked (
  VOID
  )
{
  //
  // Get lock info from PWRMBASE + PM_CFG
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_PM_CFG) & B_PMC_PWRM_PM_CFG_DBG_MODE_LOCK) != 0);
}

/**
  This function checks PMC Set Strap Message interface lock

  @retval State of PMC Set Strap Message Interface lock
**/
BOOLEAN
PmcIsSetStrapMsgInterfaceLocked (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_SSML) & B_PMC_PWRM_SSML_SSL) != 0);
}

/**
  This function verify VR Config Bit is Locked

  @retval TRUE       VR Config Bit is Locked.
  @retval FALSE      VR Config Bit is Unlocked.
**/
BOOLEAN
PmcIsFivrConfigLocked (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B) & B_PMC_PWRM_GEN_PMCON_B_VR_CONFIG_LOCK) != 0);
}

/**
 Clear any SMI status or wake status.
**/
VOID
ClearSmiAndWake (
  VOID
  )
{
  UINT16              ABase;
  UINT16              Pm1Sts;

  ABase = PmcGetAcpiBase ();

  //
  // Clear any SMI or wake state from the boot
  //
  Pm1Sts = B_ACPI_IO_PM1_STS_PWRBTN;

  IoWrite16 (ABase + R_ACPI_IO_PM1_STS, Pm1Sts);

  //
  // Clear the GPE and PM enable
  //
  IoWrite16 (ABase + R_ACPI_IO_PM1_EN, 0);
  IoWrite32 (ABase + R_ACPI_IO_GPE0_EN_127_96, 0);
}

/**
  Function to check if Dirty Warm Reset occurs
  (Global Reset has been converted to Host Reset)

  @reval TRUE DWR occurs
  @reval FALSE Normal boot flow
**/
BOOLEAN
PmcIsDwrBootMode (
  VOID
  )
{
  UINTN      PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();
  ASSERT (PchPwrmBase != 0);

  return !!(MmioRead32 (PchPwrmBase + R_PMC_PWRM_HPR_CAUSE0) & B_PMC_PWRM_HPR_CAUSE0_GBL_TO_HOST);
}

/**
  Sets the GPE Ctrl bit to generate a _GPE._L62 SCI to an ACPI OS

  @param  VOID

  @retval VOID
**/
VOID
PmcSetSwGpeSts (
  VOID
  )
{
  //
  // Check SCI enable bit
  //
  if (PmcIsSciEnabled ()) {

    //
    // Set SWGPE Status
    //
    PmcTriggerSwGpe ();
  }
}

/**
  This function sets Group to GPE0 configuration

  @param[in] GpeDw0Value       GPIO Group to GPE_DW0 assignment
  @param[in] GpeDw1Value       GPIO Group to GPE_DW1 assignment
  @param[in] GpeDw2Value       GPIO Group to GPE_DW2 assignment
**/
VOID
PmcSetGpioGpe (
  IN UINT32    GpeDw0Value,
  IN UINT32    GpeDw1Value,
  IN UINT32    GpeDw2Value
  )
{
  UINT32               Data32Or;
  UINT32               Data32And;

  //
  // Program GPIO_CFG register
  //
  Data32And = (UINT32) ~(B_PMC_PWRM_GPIO_CFG_GPE0_DW2 | B_PMC_PWRM_GPIO_CFG_GPE0_DW1 | B_PMC_PWRM_GPIO_CFG_GPE0_DW0);
  Data32Or  = (UINT32) ((GpeDw2Value << N_PMC_PWRM_GPIO_CFG_GPE0_DW2) |
                        (GpeDw1Value << N_PMC_PWRM_GPIO_CFG_GPE0_DW1) |
                        (GpeDw0Value << N_PMC_PWRM_GPIO_CFG_GPE0_DW0));

  MmioAndThenOr32 (
    (PmcGetPwrmBase () + R_PMC_PWRM_GPIO_CFG),
    Data32And,
    Data32Or
    );
}

/**
  This function sets Group to GPE0 configuration, based on Power Managment Controller (PMC) PWRM address.
  Input address gives opportiunity to configure PMC located inside SOC and PCH as well.

  @param[in] PwrmAddress       PMC PWRM address
  @param[in] GpeDw0Value       GPIO Group to GPE_DW0 assignment
  @param[in] GpeDw1Value       GPIO Group to GPE_DW1 assignment
  @param[in] GpeDw2Value       GPIO Group to GPE_DW2 assignment
**/
VOID
PwrmPmcSetGpioGpe (
  IN UINTN     PwrmAddress,
  IN UINT32    GpeDw0Value,
  IN UINT32    GpeDw1Value,
  IN UINT32    GpeDw2Value
  )
{
  UINT32               Data32Or;
  UINT32               Data32And;

  //
  // Program GPIO_CFG register
  //
  Data32And = (UINT32) ~(B_PMC_PWRM_GPIO_CFG_GPE0_DW2 | B_PMC_PWRM_GPIO_CFG_GPE0_DW1 | B_PMC_PWRM_GPIO_CFG_GPE0_DW0);
  Data32Or  = (UINT32) ((GpeDw2Value << N_PMC_PWRM_GPIO_CFG_GPE0_DW2) |
                        (GpeDw1Value << N_PMC_PWRM_GPIO_CFG_GPE0_DW1) |
                        (GpeDw0Value << N_PMC_PWRM_GPIO_CFG_GPE0_DW0));

  MmioAndThenOr32 (
    (PwrmAddress + R_PMC_PWRM_GPIO_CFG),
    Data32And,
    Data32Or
    );
}

/**
  Read Cpu BCLK source comes from

  @reval  TRUE                      BCLK from integrated clock
  @reval  FALSE                     BCLK from discrete clock
**/
BOOLEAN
PmcDetectIntegratedClockSource (
  VOID
  )
{
  EFI_STATUS              Status;
  PMC_IPC_COMMAND_BUFFER  Rbuf;
  PMC_IPC_COMMAND_BUFFER  Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  ZeroMem (&Rbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = R_PMC_PWRM_PMC_SOFT_STRAP_7;

  Status = PmcSendCommand (
             PmcGetPwrmBase (),
             V_PMC_PWRM_IPC_CMD_COMMAND_GEN_COMM,
             V_PMC_PWRM_IPC_SUBCMD_GEN_COMM_READ,
             0,
             &Wbuf,
             &Rbuf
             );

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "PmcDetectIntegratedClockSource fail, Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return TRUE;
  }

  if ((Rbuf.Buf0 & (B_PMC_PWRM_PMC_SOFT_STRAP_7_DEF_CPU_BCLK_CFG | B_PMC_PWRM_PMC_SOFT_STRAP_7_DEF_SOC_BCLK_CFG)) == V_PMC_PWRM_PMC_SOFT_STRAP_7_DEF_CPU_BCLK_CFG_INTEGRATED) {
    return TRUE;
  }

  return FALSE;
}

/**
  Read Low Power Mode Requirement registers by PMC IPC interface,
  and store read data to the output buffer.

  @param[OUT] LpmReqData            Point to buffer (caller allocated) where LPM requirement data being stored.
  @param[IN]    BufLen                    Length of Buffer which param LpmReqData point to

  @reval  EFI_SUCCESS                    Function was executed successfully
  @reval  EFI_INVALID_PARAMETER   Parameters are invalid
  @reval  EFI_DEVICE_ERROR           Fail to read LPM requirement data
**/
EFI_STATUS
PmcReadLpmReqData (
  OUT UINT32 *LpmReqData,
  IN  UINT32 BufLen
  )
{
  EFI_STATUS              Status;
  UINT8                   Index = 0;
  UINT16                  Offset;
  PMC_IPC_COMMAND_BUFFER  Rbuf;
  PMC_IPC_COMMAND_BUFFER  Wbuf;

  if (!LpmReqData) {
    return EFI_INVALID_PARAMETER;
  }

  // Read and store the LPM Req Register to System memory
  for (Offset = R_PMC_PWRM_LPM0_CSOFF_REQ; Offset <= R_PMC_PWRM_LPM7_REQ_5; Offset += 0x4) {
    ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
    ZeroMem (&Rbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

    Wbuf.Buf0 = Offset;
    Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_GEN_COMM,
      V_PMC_PWRM_IPC_SUBCMD_GEN_COMM_READ, 0, &Wbuf, &Rbuf);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    if (Index < BufLen) {
      LpmReqData[Index++] = Rbuf.Buf0;
      DEBUG ((DEBUG_INFO, "LPM register Lpmreg[%d] 0ffset[0x%x] : %x\n", Index - 1, Wbuf.Buf0, LpmReqData[Index - 1]));
    } else {
      DEBUG ((DEBUG_WARN, "Overflow LpmReqData buffer\n"));
      return EFI_INVALID_PARAMETER;
    }

    //Each LPM register length is 4 byte. Register layout is not always adjacent, but "reserve"
    //0x18 bytes every 6 register.
    if ((Index % 6) == 0) {
      Offset += 0x18;
    }
  }

  DEBUG ((DEBUG_INFO, "LPM register Read End \n"));
  return EFI_SUCCESS;
}

/**
  Reads an 8-bit PMIC register of CastroCove.

  Reads the 8-bit PMIC register specified by RegisterOffset input field.
  The operation status is returned.

  @param[in]  RegisterOffset  - The PMIC register to read
  @param[out] Value           - The value read from the PMIC register

  @retval EFI_SUCCESS         - Read bytes from PMIC device successfully
  @retval Others              - Status depends on IPC operation
**/
EFI_STATUS
EFIAPI
PmicRead8 (
  IN UINT32    RegisterOffset,
  OUT UINT8    *Value,
  IN UINT8     PmicId
  )
{
  EFI_STATUS                Status = EFI_INVALID_PARAMETER;
  PMC_IPC_COMMAND_BUFFER    Wbuf;
  PMC_IPC_COMMAND_BUFFER    Rbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  ZeroMem (&Rbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  *Value = 0;

  Wbuf.Buf0 = RegisterOffset;
  if (PmicId == 0x1) {
    Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_PMIC, V_PMC_PWRM_IPC_CMD_CMD_ID_READ_PMIC1_I2C, 2, &Wbuf, &Rbuf);
  } else if (PmicId == 0x2) {
    Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_PMIC, V_PMC_PWRM_IPC_CMD_CMD_ID_READ_PMIC2_I2C, 2, &Wbuf, &Rbuf);
  } else if (PmicId == 0x3) {
    Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_PMIC, V_PMC_PWRM_IPC_CMD_CMD_ID_READ_PMIC3_I2C, 2, &Wbuf, &Rbuf);
  }
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR: CastroCovePmicRead8, Status: %r, Rbuf.Buf0 = 0x%08X\n", Status, Rbuf.Buf0));
    ASSERT_EFI_ERROR (Status);
  } else {
    *Value = (UINT8)Rbuf.Buf0;
  }

  return Status;
}

/**
  Writes an 8-bit PMIC register of CastroCove with a 8-bit value.

  Writes the 8-bit PMIC register specified by RegisterOffset input field with the value specified
  by Value and return the operation status.

  @param[in]  RegisterOffset  - The PMIC register to write.
  @param[in]  Value           - The value to write to the PMIC register

  @retval EFI_SUCCESS         - Write bytes to PMIC device successfully
  @retval Others              - Status depends on IPC operation
**/
EFI_STATUS
EFIAPI
PmicWrite8 (
  IN UINT32    RegisterOffset,
  IN UINT8     Value,
  IN UINT8     PmicId
  )
{
  EFI_STATUS                Status = EFI_INVALID_PARAMETER;
  PMC_IPC_COMMAND_BUFFER    Wbuf;
  PMC_IPC_COMMAND_BUFFER    Rbuf;

  DEBUG ((DEBUG_INFO, "CastroCovePmicWrite8 Begin\n"));

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  ZeroMem (&Rbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = (Value << 16) | RegisterOffset;
  if (PmicId == 0x1) {
    Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_PMIC, V_PMC_PWRM_IPC_CMD_CMD_ID_WRITE_PMIC1_I2C, 3, &Wbuf, &Rbuf);
  } else if (PmicId == 0x2) {
    Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_PMIC, V_PMC_PWRM_IPC_CMD_CMD_ID_WRITE_PMIC2_I2C, 3, &Wbuf, &Rbuf);
  } else if (PmicId == 0x3) {
    Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_PMIC, V_PMC_PWRM_IPC_CMD_CMD_ID_WRITE_PMIC3_I2C, 3, &Wbuf, &Rbuf);
  }
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR: CastroCovePmicWrite8, Status: %r, Rbuf.Buf0 = 0x%08X\n", Status, Rbuf.Buf0));
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Reads QDF information
  @param[in/out]  QdfInfo     - QDF Information
  @retval EFI_SUCCESS         - Write bytes to PMIC device successfully
  @retval Others              - Status depends on IPC operation
**/
EFI_STATUS
EFIAPI
PmcReadQdf (
  IN OUT CHAR8 QdfInfo[5]
  )
{
  EFI_STATUS                Status = EFI_INVALID_PARAMETER;
  PMC_IPC_COMMAND_BUFFER    Wbuf;
  PMC_IPC_COMMAND_BUFFER    Rbuf;

  DEBUG ((DEBUG_INFO, "PmcReadQdf Begin\n"));

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  ZeroMem (&Rbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = 3; //QDF Index
  Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_READ_QDF, 0, 4, &Wbuf, &Rbuf);

  if (!EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "Rbuf.Buf0 = 0x%0x\n", Rbuf.Buf0));
    if(0 != Rbuf.Buf0){
      QdfInfo[0] = ((Rbuf.Buf0 >> 24) & 0xFF);
      QdfInfo[1] = ((Rbuf.Buf0 >> 16) & 0xFF);
      QdfInfo[2] = ((Rbuf.Buf0 >> 8) & 0xFF);
      QdfInfo[3] = (Rbuf.Buf0 & 0xFF);
      QdfInfo[4] = '\0';
    } else {
      QdfInfo[0] = 'N';
      QdfInfo[1] = '/';
      QdfInfo[2] = 'A';
      QdfInfo[3] = '\0';
      DEBUG ((DEBUG_INFO, "QDF info not available\n"));
    }
  } else {
    DEBUG ((DEBUG_ERROR, "ERROR: QDF read Status: %r, Rbuf.Buf0 = 0x%08X\n", Status, Rbuf.Buf0));
  }

  DEBUG ((DEBUG_INFO, "PmcReadQdf End\n"));

  return Status;
}

/**
  Set OTP memory is write-locked

  @param[in]                  - None

  @retval EFI_SUCCESS         - Write bytes to PMIC device successfully
  @retval Others              - Status depends on IPC operation
**/
EFI_STATUS
EFIAPI
PmicWriteLock (
  VOID
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;
  PMC_IPC_COMMAND_BUFFER    Rbuf;

  DEBUG ((DEBUG_INFO, "CastroCovePmicWriteLock Begin\n"));

  ZeroMem(&Wbuf, sizeof(PMC_IPC_COMMAND_BUFFER));
  ZeroMem(&Rbuf, sizeof(PMC_IPC_COMMAND_BUFFER));

  Status = PmcSendCommand(PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_PMIC, V_PMC_PWRM_IPC_CMD_CMD_ID_WRITE_LOCK_ENABLE_PMIC_I2C, 0, &Wbuf, &Rbuf);

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR: CastroCovePmicWriteLock, Status: %r \n", Status));
    ASSERT_EFI_ERROR(Status);
  }
  DEBUG ((DEBUG_INFO, "CastroCovePmicWriteLock End, Status:%r\n", Status));

  return Status;
}

/**
  This function is used to notify, that workaround is required for the a particular CPU stepping.
  The S3M ROM workaround is to add fixed timeouts of 15 seconds to two of the early handshakes between the CPU and PMC.

  @param[in]  WaRequired     - 1 WA required
                             - 0 WA not required

  @retval EFI_SUCCESS         - Write bytes to PMIC device successfully
  @retval Others              - Status depends on IPC operation
**/
EFI_STATUS
PmcS3mWaTimeouts (
  IN UINT8     WaRequired
)
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = WaRequired;

  Status = PmcSendCommand (
    PmcGetPwrmBase (),
    V_PMC_PWRM_IPC_CMD_COMMAND_GENERIC_WORKAROUND,
    V_PMC_PWRM_IPC_CMD_SUBCMD_S3M_ROM_WA_ADD_TIMEOUTS,
    1,
    &Wbuf,
    NULL);

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR: PMC did not support the S3M ROM workaround, Status: %r \n", Status));
  }

  return Status;
}

/**
  This function disables the power button from triggering a global reset warn condition.
**/
VOID
PmcPowerButtonGlobalResetDisable (
  VOID
  )
{
  IoOr16 (PmcGetAcpiBase () + R_ACPI_IO_PM1_EN, B_ACPI_IO_PM1_EN_PWRBTN_GBLRST_DIS);
}

/**
  This function checks if Boot Mode is in Asynchronous Warm Reset

  @retval TRUE/FALSE
**/
BOOLEAN
PmcIsAwrBootMode (
    VOID
    )
{
  UINTN      PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();
  ASSERT(PchPwrmBase != 0);

  return ((MmioRead32(PchPwrmBase + R_PMC_PWRM_HPR_CAUSE0) & B_PMC_PWRM_HPR_CAUSE0_AWR) != 0);
}

/**
  This function checks Host Reset status (HOST_RST_STS)

  @retval TRUE  Warm boot
          FALSE Cold/G3 boot
**/
BOOLEAN
PmcIsWarmBoot (
  VOID
  )
{
  UINT32  PmconA;
  PmconA = MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A);

  //
  // If Host Reset Status and Memory self-refresh bits are set, return TRUE (warm boot)
  //
  if (((PmconA & B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS) != 0) && ((PmconA & B_PMC_PWRM_GEN_PMCON_A_MEM_SR) != 0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Based on power source type update Psys telemetry it receives is in "Watts" or "% Capacity"

  @param[in] PmcPlatformAtxTelemetryUnit  This bit indicates whether Psys telemetry is in "Watts" or "% Capacity
**/
VOID
EFIAPI
ProgramTelemetryTypeforPsys (
  IN UINT32 PmcPlatformAtxTelemetryUnit
  )
{
  UINTN         PwrmBase;

  PwrmBase = PmcGetPwrmBase ();
  if (PmcPlatformAtxTelemetryUnit == 1) {
    ///
    /// Set Bit0 as 1 for Percentage, 0 for Watts
    ///
    MmioOr32 (PwrmBase + R_PMC_PWRM_SUS_SPARE_GCR_0, B_PMC_PWRM_SUS_SPARE_GCR_0_TELEMETRY_TYPE);
  }
}

/**
  This function reads NAC MBIST status and stores it in the output variable

  @param[OUT] NacMbistStatus      Pointer to the variable (caller allocated) where NAC MBIST status will be stored.
                                  NAC MBIST status is valid only if the function ends with success.

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        IPC command failed with an error
  @retval EFI_TIMEOUT             IPC command did not complete after 1s
**/
EFI_STATUS
PmcReadNacMbistStatus (
  OUT UINT32 *NacMbistStatus
  )
{
  EFI_STATUS              Status;
  PMC_IPC_COMMAND_BUFFER  Rbuf;
  PMC_IPC_COMMAND_BUFFER  Wbuf;

  ZeroMem (&Rbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Status = PmcSendCommand (
             PmcGetPwrmBase (),
             V_PMC_PWRM_IPC_CMD_COMMAND_NAC_STATUS,
             V_PMC_PWRM_IPC_CMD_CMD_ID_NAC_MBIST_STATUS,
             4,
             &Wbuf,
             &Rbuf
             );

  if (!EFI_ERROR(Status)) {
    *NacMbistStatus = Rbuf.Buf0;
    DEBUG ((DEBUG_INFO, "NAC MBIST status: 0x%04X\n", Rbuf.Buf0));
  } else if (Status == EFI_INVALID_PARAMETER) {
    DEBUG ((DEBUG_ERROR, "PmcReadNacMbistStatus passed invalid parameters to PmcSendCommand\n"));
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
  } else {
    DEBUG ((DEBUG_ERROR, "PmcReadNacMbistStatus ended with error: %r\n", Status));
  }

  return Status;
}

/**
  Send IPC Start Time Sync notification

  @param[in] PwrmBase  PWRM base address.
  @param[in] Consent   1 will set the consent
                       0 will clear the consent.

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcSendPrivacyConsent (
  IN UINTN   PwrmBase,
  IN BOOLEAN Consent
  )
{
  PMC_IPC_COMMAND_BUFFER  Wbuf;
  EFI_STATUS              Status;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = Consent;
  Wbuf.Buf1 = 1;
  Status = PmcSendCommand (PwrmBase, V_PMC_PWRM_IPC_CMD_COMMAND_GENERIC_WORKAROUND, 0, 8, &Wbuf, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR PmcCommand Status = %r\n", __FUNCTION__, Status));
  }
  return Status;
}