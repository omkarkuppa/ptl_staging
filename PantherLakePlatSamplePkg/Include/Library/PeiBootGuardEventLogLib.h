/** @file
  This file contains functions for creating TPM Event Log

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <Protocol/Tcg2Protocol.h>

#if FixedPcdGetBool(PcdBootGuardPerformanceEnable) == 1

//
// BootGuard Performance data definitions
//

extern EFI_GUID gAcmDataHobGuid;

#define BTG_PERF_MAX_RECORDS 0x40

#endif

typedef struct {
  UINT64 Address;
  UINT8  Model          : 4;
  UINT8  Family         : 4;
  UINT8  ProcessorType  : 4;
  UINT8  ExtModel       : 4;
  UINT8  ModelMask      : 4;
  UINT8  FamilyMask     : 4;
  UINT8  TypeMask       : 4;
  UINT8  ExtModelMask   : 4;
  UINT16 Version;
  UINT8  FitType        : 7;
  UINT8  C_V            : 1;
  UINT8  ExtFamily      : 4;
  UINT8  ExtFamilyMask  : 4;
} FIT_TYPE2_VER_200_ENTRY;


/**
   This is the library's callback entry point that will handle the TCG event logging
   depending on the Boot Guard configuration.

   @param[in]  PeiServices  Pointer to PEI Services Table.
   @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                            caused this function to execute.
   @param[in]  Ppi          Pointer to the PPI data associated with this function.

   @retval     EFI_SUCCESS  The function completes successfully
   @retval     Others       Bootguard not supported
**/
EFI_STATUS
EFIAPI
CreateBootguardEventLogEntriesCallback (
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDesc,
  IN  VOID                          *InvokePpi
  );
