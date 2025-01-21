/** @file
  This file contains function definitions that can determine
  the TXT capabilities of a platform during DXE and perform
  certain specific platform tasks that are required for TXT
  during DXE.

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
#ifndef _TXT_DXE_LIB_H_
#define _TXT_DXE_LIB_H_

#include <Protocol/MpService.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <TxtInfoHob.h>
#include <Library/CpuCommonLib.h>
#include <Library/TimerLib.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <Library/DxeSaNvsInitLib.h>

///
/// The following switch is used in EfiScriptLib.h file. If enabled -
/// forces linking to EfiScriptLib library
///
#define BASE_ADDR_MASK                      0xFFFFF000
#define TEST_PATTERN                        0x5A5A5A5A5A5A5A5A
#define MP_TIMEOUT_FOR_STARTUP_ALL_APS      0           ///< Set 0 for BSP always wait for APs

///
/// Chispet register
///
#define TXT_OPT_IN_VMX_AND_SMX_MSR_VALUE    0xFF03

#define LOCAL_APIC_THERMAL_DEF              0x330

#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND  1
#endif
#ifndef STALL_ONE_MILLI_SECOND
#define STALL_ONE_MILLI_SECOND  1000
#endif

#pragma pack(push, 1)
typedef struct _TXT_DXE_LIB_CONTEXT_ {
  EFI_HANDLE               ImageHandle;
  EFI_SYSTEM_TABLE         *SystemTable;
  EFI_MP_SERVICES_PROTOCOL *MpService;
  UINTN                    CpuCount;
  TXT_INFO_DATA            *TxtInfoData;
} TXT_DXE_LIB_CONTEXT;
#pragma pack(pop)

/**
  This routine initializes and collects all Protocols and data required
  by the routines in this file.

  @param[in]      ImageHandle      A pointer to the Image Handle for this file.
  @param[in]      SystemTable      A pointer to the EFI System Table
  @param[in][out] TxtDxeCtx        A pointer to a caller allocated data structure that contains
                                   all of the Protocols and data required by the routines
                                   in this file.

  @retval         EFI_SUCCESS      Return EFI_SUCCESS if no error happen
  @retval         EFI_NOT_FOUND    If TxtInfoHob is not found
**/
EFI_STATUS
InitializeTxtDxeLib (
  IN EFI_HANDLE              ImageHandle,
  IN EFI_SYSTEM_TABLE        *SystemTable,
  IN OUT TXT_DXE_LIB_CONTEXT *TxtDxeCtx
  );

/**
  Determines whether or not the current processor is TXT Capable.

  @retval TRUE   If the current processor supports TXT
  @retval FALSE  If the current processor does not support TXT
**/
BOOLEAN
IsTxtProcessor (
  VOID
  );

/**
  Determines whether or not the platform has executed an TXT launch by
  examining the TPM Establishment bit.

  @param[in]  TxtDxeCtx  A pointer to an initialized TXT DXE Context data structure

  @retval     TRUE       If the TPM establishment bit is asserted.
  @retval     FALSE      If the TPM establishment bit is unasserted.
**/
BOOLEAN
IsTxtEstablished (
  IN TXT_DXE_LIB_CONTEXT *TxtDxeCtx
  );

/**
  Determines whether or not the platform has executed an TXT launch by
  examining the TPM Establishment bit.

  @param[in]  TxtDxeCtx A pointer to an initialized TXT DXE Context data structure

  @retval     TRUE      If the TPM establishment bit is asserted.
  @retval     FALSE     If the TPM establishment bit is unasserted.
**/
BOOLEAN
IsTxtResetSet (
  IN TXT_DXE_LIB_CONTEXT *TxtDxeCtx
  );

/**
  Determines whether or not the platform requires initialization for TXT use.

  @param[in]  TxtDxeCtx   A pointer to an initialized TXT DXE Context data structure

  @retval     TRUE        If the the platoform should be configured for TXT.
  @retval     FALSE       If TXT is not to be used.
**/
BOOLEAN
IsTxtEnabled (
  IN TXT_DXE_LIB_CONTEXT *TxtDxeCtx
  );

/**
  Allocates 1 MB of 1MB-aligned memory for use as TXT Device Memory.  Records
  the location of TXT Device Memory in TXT Chipset registers and then adds
  programming instructions for these registers into BootScript.

  @param[in]  TxtDxeCtx     A pointer to an initialized TXT DXE Context data structure

  @retval     EFI_SUCCESS   TXT Device memory has been successfully initialized.
  @retval     EFI_ERROR     TXT Device memory not awailable.
**/
EFI_STATUS
SetupTxtDeviceMemory (
  IN TXT_DXE_LIB_CONTEXT *TxtDxeCtx
  );

/**
  Invokes TxtDxeLibLaunchBiosAcm to execute the LockConfig function.

  @param[in]  TxtDxeCtx    A pointer to an initialized TXT DXE Context data structure

  @retval     EFI_SUCCESS  Always.
**/
EFI_STATUS
DoLockConfig (
  IN TXT_DXE_LIB_CONTEXT *TxtDxeCtx
  );

/**
  Invokes TxtDxeLibLaunchBiosAcm to reset the TPM's establishment bit.

  @param[in]  TxtDxeCtx    A pointer to an initialized TXT DXE Context data structure

  @retval     EFI_SUCCESS  Always.
**/
EFI_STATUS
ResetTpmEstBit (
  IN TXT_DXE_LIB_CONTEXT *TxtDxeCtx
  );

/**
  Sets up the system and then launches the TXT BIOS ACM to run the function
  requested by AcmFunction.

  @param[in]  AcmBase      Base address of BIOS ACM location
  @param[in]  Funct        Function number of BIOS ACM to be executed

  @retval     EFI_SUCCESS  Always.
**/
VOID
LaunchBiosAcm (
  IN UINT64 AcmBase,
  IN UINT64 Funct
  );

/**
  Sets up the system and then launches the TXT BIOS ACM to run the function
  requested by AcmFunction.

  @param[in]  AcmFunction    Constant that represents the function from the BIOS ACM
                             that should be executed.
  @param[in]  TxtDxeCtx      A pointer to an initialized TXT DXE Context data structure

  @retval     EFI_SUCCESS    Always.
**/
EFI_STATUS
TxtDxeLibLaunchBiosAcm (
  IN TXT_DXE_LIB_CONTEXT *TxtDxeCtx,
  IN UINT64              AcmFunction
  );

/**
  AP initial routine executed through MP service for TXT SCHECK

  @param[in]  Buffer  Not used.
**/
VOID
EFIAPI
DoApInit (
  IN VOID *Buffer
  );

/**
  Save AP configuration routine executed through MP service for TXT SCHECK

  @param[in]  Buffer  Not used.
**/
VOID
EFIAPI
ApSaveConfig (
  IN VOID *Buffer
  );

/**
  Restore AP configuration routine executed through MP service for TXT SCHECK

  @param[in]  Buffer  Not used.
**/
VOID
EFIAPI
ApRestoreConfig (
  IN VOID *Buffer
  );

/**
  Disable or restore possible SMI sources before or after POST SCHECK

  @param[in]  TxtDxeCtx   A pointer to an initialized TXT DXE Context data structure
  @param[in]  Operation   Boolean value telling what operation is requested:
                          TRUE  - to save and then disable possible SMI sources
                          FALSE - to restore original SMI settings

  @retval    EFI_SUCCESS  always return EFI_SUCCESS
**/
EFI_STATUS
DisableSmiSources (
  IN TXT_DXE_LIB_CONTEXT *TxtDxeCtx,
  IN BOOLEAN             Operation
  );

/**
  This function gets called before/after run the SCHECK function, intend to avoid platform specific SMIs to interfere in BIOS POST
  if BIOS SMM MP services or synchonization code is not well considered that some of APs are not waken up from Wait-for-SIPI state.
  Function should preserve original SMI enabling setting in augument is TRUE, and then restore it in augurment is FALSE.
  The caller is DisableSmiSources() in TxtDxeLib.c

  @param[in]  TxtDxeCtx
  @param[in]  Operation = TRUE     Calling before SCHECK to saved and disable platform specific SMIs setting
                       = FALSE    Calling after SCHECK to restore platform specific SMIs setting

  @retval     EFI_SUCCESS          Always.
**/
extern
EFI_STATUS
EFIAPI
TxtDxeOemDisableSmi (
  IN TXT_DXE_LIB_CONTEXT *TxtDxeCtx,
  IN BOOLEAN             Operation
  );

/**
  Read policy protocol to reset AUX content

  @param[in]  TxtDxeCtx       A pointer to an initialized TXT DXE Context data structure

  @retval     EFI_SUCCESS     No error happend
  @retval     EFI_NOT_FOUND   TxtPolicyProtocol is not found
**/
EFI_STATUS
ResetTpmAux (
  IN TXT_DXE_LIB_CONTEXT *TxtDxeCtx
  );

/**
  This is the entry point to the TXT DXE Driver for installing SSDT ACPI table

  @param[in]  ImageHandle       A handle for this module
  @param[in]  SystemTable       A pointer to the EFI System Table

  @retval     EFI_SUCCESS       If SSDT installation succeed
**/
EFI_STATUS
EFIAPI
InitializeTxtAcpi (
  IN EFI_HANDLE       ImageHandle
  );

#endif
