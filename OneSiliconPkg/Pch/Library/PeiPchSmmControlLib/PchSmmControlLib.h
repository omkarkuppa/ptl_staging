/** @file
  Header file for SMM Control PEI Library.

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

@par Specification
**/
#ifndef _PCH_SMM_CONTROL_LIB_H_
#define _PCH_SMM_CONTROL_LIB_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PmcLib.h>
#include <Ppi/SmmControl.h>

/**
  Clear the SMI status


  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_DEVICE_ERROR        Something error occurred
**/
EFI_STATUS
EFIAPI
SmmClear (
  VOID
  );

//
// Prototypes
//
/**
  This routine generates an SMI

  @param[in] PeiServices                General purpose services available to every PEIM.
  @param[in] This                       The EFI SMM Control ppi instance
  @param[in, out] ArgumentBuffer        The buffer of argument
  @param[in, out] ArgumentBufferSize    The size of the argument buffer
  @param[in] Periodic                   Periodic or not
  @param[in] ActivationInterval         Interval of periodic SMI

  @retval EFI Status                    Describing the result of the operation
  @retval EFI_INVALID_PARAMETER         Some parameter value passed is not supported
**/
EFI_STATUS
EFIAPI
PeiActivate (
  IN     EFI_PEI_SERVICES    **PeiServices,
  IN     PEI_SMM_CONTROL_PPI *This,
  IN OUT INT8                *ArgumentBuffer OPTIONAL,
  IN OUT UINTN               *ArgumentBufferSize OPTIONAL,
  IN     BOOLEAN             Periodic OPTIONAL,
  IN     UINTN               ActivationInterval OPTIONAL
  );

/**
  This routine clears an SMI

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] This                 The EFI SMM Control ppi instance
  @param[in] Periodic             Periodic or not

  @retval EFI Status              Describing the result of the operation
  @retval EFI_INVALID_PARAMETER   Some parameter value passed is not supported
**/
EFI_STATUS
EFIAPI
PeiDeactivate (
  IN EFI_PEI_SERVICES    **PeiServices,
  IN PEI_SMM_CONTROL_PPI *This,
  IN BOOLEAN             Periodic OPTIONAL
  );

#endif
