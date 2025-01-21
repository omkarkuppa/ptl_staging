/** @file
  Header file for PmcLib.

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
#ifndef _PMC_LIB_H_
#define _PMC_LIB_H_

#include <PmConfig.h>

#pragma pack(1)

typedef enum {
  PmcTPch25_10us = 0,
  PmcTPch25_100us,
  PmcTPch25_1ms,
  PmcTPch25_10ms,
} PMC_TPCH25_TIMING;

typedef enum {
  PmcNotASleepState,
  PmcInS0State,
  PmcS1SleepState,
  PmcS2SleepState,
  PmcS3SleepState,
  PmcS4SleepState,
  PmcS5SleepState,
  PmcUndefinedState,
} PMC_SLEEP_STATE;

typedef struct {
  UINT32    Buf0;
  UINT32    Buf1;
  UINT32    Buf2;
  UINT32    Buf3;
} PMC_IPC_COMMAND_BUFFER;

//
// Structure to Check different attributes for CrashLog supported by PMC.
//
typedef union {
  struct {
    UINT32  Avail      : 1;        ///< CrashLog feature availability bit
    UINT32  Dis        : 1;        ///< CrasLog Disable bit
    UINT32  Rsvd       : 2;        ///< Reserved
    UINT32  Size       : 12;       ///< CrasLog data size. (If it is zero, use default size 0xC00)
    UINT32  BaseOffset : 16;       ///< Start offset of CrashLog in PMC SSRAM
  } Bits;
  struct {
    UINT32  Avail      : 1;           ///< CrashLog feature availability bit
    UINT32  Dis        : 1;           ///< CrasLog Disable bit
    UINT32  Mech       : 2;           ///< CrashLog mechanism
    UINT32  ManuTri    : 1;           ///< Manul trigger command.
    UINT32  Clr        : 1;           ///< Clear Command
    UINT32  AllReset   : 1;           ///< Trigger on all reset command
    UINT32  ReArm      : 1;           ///< Re-arm command
    UINT32  GlbRstTriggerMask : 1;    ///< Global reset trigger mask supported
    UINT32  Rsvd       : 18;          ///< Pch Specific reserved
    UINT32  GlbRstTriggerMaskSts : 1; ///< Global reset trigger mask status
    UINT32  CrashLogReq: 1;           ///< Crash log requestor flow
    UINT32  TriArmedSts: 1;           ///< Trigger armed status, re-arm indication bit.
    UINT32  TriAllReset: 1;           ///< Trigger on all resets status
    UINT32  CrashDisSts: 1;           ///< Crash log disabled status
    UINT32  PchRsvd    : 16;          ///< Pch Specific reserved
    UINT32  DesTableOffset: 16;       ///< Descriptor Table offset
  } Bits64;
  UINT32  Uint32;
  UINT64  Uint64;
} PMC_IPC_DISCOVERY_BUF;

typedef union {
  struct {
    UINT32 Offset   : 16;
    UINT32 Size     : 16;
  } Info;
  UINT32 Uint32;
} PMC_CRASHLOG_RECORDS;

typedef struct PmcCrashLogLink {
  PMC_CRASHLOG_RECORDS      Record;
  UINT64                    AllocateAddress;
  struct PmcCrashLogLink    *Next;
} PMC_CRASHLOG_LINK;

#pragma pack()

#define AFTER_G3_S5_S4      1
#define AFTER_G3_LAST_STATE 2

/**
  Get PCH ACPI base address.

  @retval Address                   Address of PWRM base address.
**/
UINT16
PmcGetAcpiBase (
  VOID
  );

/**
  Get PCH PWRM base address.

  @retval Address                   Address of PWRM base address.
**/
UINTN
PmcGetPwrmBase (
  VOID
  );

/**
  Get MTL PCH PMC ACPI base.

  @retval Address                   Address of MTL PCH Acpi base address.
**/
UINT16
PchDiePmcGetAcpiBase (
  VOID
  );

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
  );

/**
  This function sets tPCH25 timing

  @param[in] TimingValue       tPCH25 timing value (10us, 100us, 1ms, 10ms)
**/
VOID
PmcSetTPch25Timing (
  IN PMC_TPCH25_TIMING    TimingValue
  );

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
  );

/**
  This function checks if Power Failure occurred by
  reading PWR_FLR bit

  @retval Power Failure state
**/
BOOLEAN
PmcIsPowerFailureDetected (
  VOID
  );

/**
  This function checks if Power Failure occurred by
  reading SUS_PWR_FLR bit

  @retval SUS Power Failure state
**/
BOOLEAN
PmcIsSusPowerFailureDetected (
  VOID
  );

/**
  This function clears Power Failure status (PWR_FLR)
**/
VOID
PmcClearPowerFailureStatus (
  VOID
  );

/**
  This function clears Global Reset status (GBL_RST_STS)
**/
VOID
PmcClearGlobalResetStatus (
  VOID
  );

/**
  This function clears Host Reset status (HOST_RST_STS)
**/
VOID
PmcClearHostResetStatus (
  VOID
  );

/**
  This function clears SUS Power Failure status (SUS_PWR_FLR)
**/
VOID
PmcClearSusPowerFailureStatus (
  VOID
  );

/**
  This function sets state to which platform will get after power is reapplied

  @param[in] PowerStateAfterG3          0: S0 state (boot)
                                        1: S5/S4 State
                                        2: Depend on the platform state when the G3 occured
**/
VOID
PmcSetPlatformStateAfterPowerFailure (
  IN UINT8 PowerStateAfterG3
  );

/**
  This function sets whether previous platform state will be considered when determining whether to power up after Type 8 Global Reset

  @param[in] PowerStateAfterT8Gr        0: Previous state will not be considered
                                        1: Previous state will be considered
**/
VOID
PmcSetPlatformStateAfterT8GlobalReset (
  IN UINT8 PowerStateAfterT8Gr
  );

/**
  This function enables Power Button SMI
**/
VOID
PmcEnablePowerButtonSmi (
  VOID
  );

/**
  This function disables Power Button SMI
**/
VOID
PmcDisablePowerButtonSmi (
  VOID
  );

/**
  This function reads PM Timer Count driven by 3.579545 MHz clock

  @retval PM Timer Count
**/
UINT32
PmcGetTimerCount (
  VOID
  );

/**
  Get Sleep Type that platform has waken from

  @retval SleepType                Sleep Type
**/
PMC_SLEEP_STATE
PmcGetSleepTypeAfterWake (
  VOID
  );

/**
  Clear PMC Wake Status
**/
VOID
PmcClearWakeStatus (
  VOID
  );

/**
  Configure sleep state

  @param[in] SleepState         S0/S1/S3/S4/S5, refer to PMC_SLEEP_STATE
**/
VOID
PmcSetSleepState (
  PMC_SLEEP_STATE  SleepState
  );

/**
  Check if platform boots after shutdown caused by power button override event

  @retval  TRUE   Power Button Override occurred in last system boot
  @retval  FALSE  Power Button Override didn't occur
**/
BOOLEAN
PmcIsPowerButtonOverrideDetected (
  VOID
  );

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
  );

/**
  This function sends PMC IPC CrashLog Disable Command

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log Disable command failed with an error
  @retval EFI_TIMEOUT             Crash Log Disable command did not complete
**/
EFI_STATUS
PmcCrashLogDisable (
  VOID
  );

/**
  This function sends PMC IPC to clear CrashLog from PMC SSRAM area

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log Clear command failed with an error
  @retval EFI_TIMEOUT             Crash Log Clear command did not complete
**/
EFI_STATUS
PmcCrashLogClear (
  VOID
  );

/**
  This function sends PMC IPC to populate CrashLog on all reboot. The SSRAM area will be cleared on G3 by PMC automatically

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log command failed with an error
  @retval EFI_TIMEOUT             Crash Log command did not complete
**/
EFI_STATUS
PmcCrashLogOnAllReset (
  VOID
  );

/**
  This function sends PMC IPC Crashlog Re-arm command

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log command failed with an error
  @retval EFI_TIMEOUT             Crash Log command did not complete
**/
EFI_STATUS
PmcCrashLogReArm (
  VOID
  );

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
  );

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
  );

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
  );

/**
  Restore the default value of the global reset trigger enable mask

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcCrashLogRestoreGlobalResetTriggerMask (
  VOID
  );

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
  );

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
  );

/**
  This function sends PMC IPC CrashLog Disable Command

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log Disable command failed with an error
  @retval EFI_TIMEOUT             Crash Log Disable command did not complete
**/
EFI_STATUS
PchPmcCrashLogDisable (
  VOID
  );

/**
  This function sends PMC IPC to clear CrashLog from PMC SSRAM area

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log Clear command failed with an error
  @retval EFI_TIMEOUT             Crash Log Clear command did not complete
**/
EFI_STATUS
PchPmcCrashLogClear (
  VOID
  );

/**
  This function sends PMC IPC to populate CrashLog on all reboot. The SSRAM area will be cleared on G3 by PMC automatically

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log command failed with an error
  @retval EFI_TIMEOUT             Crash Log command did not complete
**/
EFI_STATUS
PchPmcCrashLogOnAllReset (
  VOID
  );

/**
  This function sends PMC IPC Crashlog Re-arm command

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Crash Log command failed with an error
  @retval EFI_TIMEOUT             Crash Log command did not complete
**/
EFI_STATUS
PchPmcCrashLogReArm (
  VOID
  );

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
  );

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
  );

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
  );

/**
  Restore the default value of the global reset trigger enable mask

  @retval Result of the IPC command send
**/
EFI_STATUS
PchPmcCrashLogRestoreGlobalResetTriggerMask (
  VOID
  );

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
  );

/**
  This function will set the DISB - DRAM Initialization Scratchpad Bit.
**/
VOID
PmcSetDramInitScratchpad (
  VOID
  );

/**
  Check global SMI enable is set

  @retval TRUE  Global SMI enable is set
          FALSE Global SMI enable is not set
**/
BOOLEAN
PmcIsGblSmiEn (
  VOID
  );

/**
  This function checks if SMI Lock is set

  @retval SMI Lock state
**/
BOOLEAN
PmcIsSmiLockSet (
  VOID
  );

/**
  This function checks if Debug Mode is locked

  @retval Debug Mode Lock state
**/
BOOLEAN
PmcIsDebugModeLocked (
  VOID
  );

/**
  This function checks PMC Set Strap Message interface lock

  @retval State of PMC Set Strap Message Interface lock
**/
BOOLEAN
PmcIsSetStrapMsgInterfaceLocked (
  VOID
  );

/**
  This function verify VR Config Bit is Locked

  @retval TRUE       VR Config Bit is Locked.
  @retval FALSE      VR Config Bit is Unlocked.
**/
BOOLEAN
PmcIsFivrConfigLocked (
  VOID
  );

/**
  Clear any SMI status or wake status left from boot.
**/
VOID
ClearSmiAndWake (
  VOID
  );

/**
  Function to check if Dirty Warm Reset occurs
  (Global Reset has been converted to Host Reset)

  @reval TRUE DWR occurs
  @reval FALSE Normal boot flow
**/
BOOLEAN
PmcIsDwrBootMode (
  VOID
  );

/**
  Sets the GPE Ctrl bit to generate a _GPE._L62 SCI to an ACPI OS
**/
VOID
PmcSetSwGpeSts (
  VOID
  );

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
  );

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
  );

/**
  Read Cpu BCLK source comes from

  @reval  TRUE                      BCLK from integrated clock
  @reval  FALSE                     BCLK from discrete clock
**/
BOOLEAN
PmcDetectIntegratedClockSource (
  VOID
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
);

/**
  This function disables the power button from triggering a global reset warn condition.
**/
VOID
PmcPowerButtonGlobalResetDisable (
  VOID
  );

/**
  This function checks if Boot Mode is in Asynchronous Warm Reset

  @retval TRUE/FALSE
**/
BOOLEAN
PmcIsAwrBootMode (
  VOID
  );

/**
  This function checks Host Reset status (HOST_RST_STS)

  @retval TRUE  Warm boot
          FALSE Cold/G3 boot
**/
BOOLEAN
PmcIsWarmBoot (
  VOID
  );

/**
  Based on power source type update Psys telemetry it receives is in "Watts" or "% Capacity"

  @param[in] PmcPlatformAtxTelemetryUnit  This bit indicates whether Psys telemetry is in "Watts" or "% Capacity
**/
VOID
EFIAPI
ProgramTelemetryTypeforPsys (
  IN UINT32 PmcPlatformAtxTelemetryUnit
  );

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
  );

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
 );

/**
  Check if SOC PMC SSRAM device is disabled

  @retval FALSE - device is disabled
  @retval TRUE  - device is enabled
**/
BOOLEAN
IsPmcSsramDeviceEnabled (
  VOID
  );

/**
  Init PMC SSRAM MMIO Bar

  @param[out] SavedStsCmd - the saved value of PMC SSRAM device status and command register

  @retval FALSE          - PMC SSRAM MMIO Bar already asigned
  @retval TRUE           - PMC SSRAM MMIO Bar asigned by this routine
**/
BOOLEAN
InitPmcSsramMmioBar (
  OUT UINT16    *SavedStsCmd
  );

/**
  Clear PMC SSRAM MMIO Bar and restore the PMC SSRAM device status and command register value

  @param[in] SavedStsCmd  - the restore value of PMC SSRAM device status and command register
  @param[in] ClearBar     - if TRUE need to clear the bar, else don't
**/
VOID
ClearPmcSsramMmioBar (
  IN UINT16     SavedStsCmd,
  IN BOOLEAN    ClearBar
  );

/**
  This function checks if SCS UFS device is supported (not disabled by fuse)

  @param[in] UfsNum     SCS UFS Device

  @retval SCS device support state
**/
BOOLEAN
PmcIsScsUfsSupported (
  IN UINT32   UfsNum
  );

/**
  Get PMC SSRAM MMIO BAR

  @retval SOC PMC SSRAM MMIO base address
**/
UINT64
GetPmcSsramMmioBar (
  VOID
  );

/**
  Returns the PMC register contents for PM CON, Global Reset, and HPR.

  @param[in,out]  GenPmConA   - Gen PM Con A register value
  @param[in,out]  HprCause0   - HPR Cause 0 register value
  @param[in,out]  GblCause0   - Global Reset Cause 0 register value
  @param[in,out]  GblCause1   - Global Reset Cause 1 register value
**/
VOID
GetPmcResetRegisters (
  IN OUT UINT32 GenPmConA,
  IN OUT UINT32 HprCause0,
  IN OUT UINT32 GblCause0,
  IN OUT UINT32 GblCause1
  );
#endif // _PMC_LIB_H_
