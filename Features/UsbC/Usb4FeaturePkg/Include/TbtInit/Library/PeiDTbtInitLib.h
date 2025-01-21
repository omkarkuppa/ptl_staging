/**@file
  PEI DTBT Init Dispatch library Header file

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

#ifndef __PEI_DTBT_INIT_LIB_H__
#define __PEI_DTBT_INIT_LIB_H__

#include <Library/TbtCommonLib.h>
#include <Library/DTbtCommonLib.h>
#include <Library/PeiTbtTaskDispatchLib.h>
#include <DTbtInfoHob.h>
#include <Library/PmcLib.h>

extern TBT_CALL_TABLE_ENTRY DTbtCallTable[];

/**
  Execute Mail box command "Set Max Payload Size".

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSetMaxPayloadSize (
  IN  VOID  *PeiTbtConfig
);

/**
  To retrieve RP bus/dev/func number.

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
RetrieveDTbtRpBdf (
  IN  VOID  *PeiTbtConfig
);

/**
  set tPCH25 Timing to 10 ms for DTBT.

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSetTPch25Timing (
  IN  VOID  *PeiTbtConfig
);

/**
  Clear VGA Registers for DTBT.

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtClearVgaRegisters (
  IN  VOID  *PeiTbtConfig
);

/**
  Execute Mail box command "Security Level".

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSLCommandExecution (
  IN  VOID  *PeiTbtConfig
);

/**
  Execute Mail box command "Sx Exit".

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSxExitFlow (
  IN  VOID  *PeiTbtConfig
);

/**
  This function pass PEI DTbt Policy to Hob.

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
PassDTbtPolicyToHob(
  IN  VOID  *PeiTbtConfig
);
#endif
