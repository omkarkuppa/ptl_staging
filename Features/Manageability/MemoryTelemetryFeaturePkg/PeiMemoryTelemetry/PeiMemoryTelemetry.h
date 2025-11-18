/** @file
  Pei Memory Telemetry Amt Ppr PEIM Internal Definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef _PEI_MEMORY_TELEMETRY_H_
#define _PEI_MEMORY_TELEMETRY_H_

#include <PiPei.h>
#include <AmtPprEnableVariable.h>
#include <MemoryConfig.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>

///
/// MRC Memory Types API matching definitions
///
#define NOREPAIR_PPR      0
#define HARD_PPR          2
#define PPR_TEST          6

/**
  Reads the AmtPprEnable UEFI variable and returns the variable data.

  @param[out] AmtPprVariableData  AMT and PPR enablement variable data

  @retval     EFI_SUCCESS         UEFI variable was found
  @retval     Others              UEFI variable not found
**/
EFI_STATUS
GetAmtPprEnableVar (
  IN OUT AMT_PPR_ENABLE    *AmtPprVariableData
  );

/**
  Update MRC PprEnable and and PprTestType policy

  @param[in]  AmtPprVariable    AMT PPR UEFI variable data to update MRC policy

  @retval     EFI_SUCCESS       Success in getting and setting policy data
  @retval     Others            Failed to get Si PreMem or MemConfig Policy
**/
EFI_STATUS
UpdatePprMrcPolicy (
  IN AMT_PPR_ENABLE       *AmtPprVariable
  );

/**
 Check if AMT PPR is supported on platform

 @retval TRUE   Platform supports AMT PPR
 @retval FALSE  Platform does not support AMT PPR
**/
BOOLEAN
IsAmtPprSupported (
  VOID
  );

/**
  Notification callback that executes when SiPreMemPolicy PPI becomes available.
  This ensures our AMT PPR processing happens early, before MeOnPolicyInstalled().

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The SiPreMemPolicy PPI that became available.

  @retval    EFI_SUCCESS          AMT PPR policy was processed successfully
  @retval    Others               Failed to process AMT PPR policy
**/
EFI_STATUS
EFIAPI
OnSiPreMemPolicyAvailable (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  );

/**
  Entry point of the Memory Telemetry AMT PPR PEIM
  Registers a notification to process AMT PPR when SiPreMemPolicy becomes available.
  This ensures execution before MeOnPolicyInstalled().

  @param[in] FileHandle       Handle of the file being invoked.
  @param[in] PeiServices      Describes the list of possible PEI Services.

  @retval     EFI_SUCCESS       Notification callback registered successfully
  @retval     Others            Failed to register notification callback
**/
EFI_STATUS
EFIAPI
PeiMemoryTelemetryEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

#endif // _PEI_MEMORY_TELEMETRY_H_
