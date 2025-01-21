/** @file
  Header file for Boot Guard Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#ifndef _BOOT_GUARD_LIB_H_
#define _BOOT_GUARD_LIB_H_

#define MSR_BOOT_GUARD_SACM_INFO                                      0x0000013A
#define B_BOOT_GUARD_SACM_INFO_NEM_ENABLED                            BIT0
#define V_TPM_PRESENT_MASK                                            0x06
#define B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS                            BIT3
#define B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT                          BIT5
#define B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT                          BIT6
#define B_BOOT_GUARD_SACM_INFO_CAPABILITY                             BIT32

#define B_CPU_BOOT_GUARD_BOOTSTATUS_S3_TPM_STARTUP_FAILED             BIT46
#define R_CPU_BOOT_GUARD_NEM_MAP_STRUCTURE_ADDRESS                    0x320

#define R_CPU_BOOT_GUARD_BOOTSTATUS                                   0xA0
#define R_CPU_BOOT_GUARD_ACM_STATUS                                   0x328
#define R_CPU_ACM_POLICY_STATUS                                       0x378
#define V_CPU_BOOT_GUARD_LOAD_ACM_SUCCESS                             0x8000000000000000
#define B_BOOT_GUARD_ACM_ERRORCODE_MASK                               0x00007FF0

#define TXT_PUBLIC_BASE                                               0xFED30000
#define MMIO_ACM_POLICY_STATUS                                        (TXT_PUBLIC_BASE + R_CPU_ACM_POLICY_STATUS)

///
/// The TPM category, TPM 1.2, TPM 2.0 and PTT are defined.
///
typedef enum {
  TpmNone = 0, ///< 0: No TPM device present on system
  dTpm12,      ///< 1: TPM 1.2 device present on system
  dTpm20,      ///< 2: TPM 2.0 device present on system
  Ptt,         ///< 3: PTT present on system
  TpmTypeMax   ///< 4: Unknown device
} TPM_TYPE;

///
/// Information related to Boot Guard Configuration.
///
typedef struct {
  BOOLEAN  MeasuredBoot;        ///< Report Measured Boot setting in Boot Guard profile. 0: Disable; 1: Enable.
  /**
  ByPassTpmInit is set to 1 if Boot Guard ACM does TPM initialization successfully.
  - 0: No TPM initialization happen in Boot Guard ACM.
  - 1: TPM initialization is done by Boot Guard ACM.
  **/
  BOOLEAN  BypassTpmInit;
  TPM_TYPE TpmType;             ///< Report what TPM device is available on system.
  BOOLEAN  BootGuardCapability; ///< Value is set to 1 if chipset is Boot Guard capable.
  /**
  Value is set to 1 if microcode failed to load Boot Guard ACM or ENF Shutdown path is taken by ME FW.
  - 0: BIOS TPM code continue with TPM initization based on MeasuredBoot.
  - 1: BIOS TPM code is not to do any futher TPM initization and extends.
  **/
  BOOLEAN  DisconnectAllTpms;
  /**
  It is indicated BIOS TPM code is not to create DetailPCR or AuthorityPCR event log if Sx resume type is S3,
  Deep-S3, or iFFS Resume.
  - 0: Create TPM event log if not Sx Resume Type.
  - 1: Bypass TPM Event Log if Sx Resume Type is identified.
  **/
  BOOLEAN  ByPassTpmEventLog;
  /**
  This field indicates that the ACM's Tpm2Startup (State) command failed during S3 resume.
  - 0: Successful Tpm2Startup (State)
  - 1: Failure during Tpm2Startup (State). BIOS will need to perform a cold reset to handle the error.
  **/
  BOOLEAN  TpmStartupFailureOnS3;

} BOOT_GUARD_INFO;

/**
  Determine if Boot Guard is supported

  @retval TRUE  - Processor is Boot Guard capable.
  @retval FALSE - Processor is not Boot Guard capable.

**/
BOOLEAN
EFIAPI
IsBootGuardSupported (
  VOID
  );

/**
  Handle Tpm2Startup (State) failures reported by ACM.

  The function checks for the TpmStartupFailureOnS3 flag if TRUE it will
  request the system to perform a cold reset.

  @param[in] *BootGuardInfo - Pointer to BootGuardInfo.
**/
VOID
EFIAPI
HandleTpmStartupFailureOnS3 (
  IN BOOT_GUARD_INFO *BootGuardInfo
);

/**
  Report platform specific Boot Guard information.

  @param[out] *BootGuardInfo - Pointer to BootGuardInfo.
**/
VOID
EFIAPI
GetBootGuardInfo (
  OUT BOOT_GUARD_INFO *BootGuardInfo
  );

/**
  Stop PBE timer if system is in Boot Guard boot

  @retval EFI_SUCCESS        - Stop PBE timer
  @retval EFI_UNSUPPORTED    - Not in Boot GuardSupport mode.
**/
EFI_STATUS
EFIAPI
StopPbeTimer (
  VOID
  );

#endif
