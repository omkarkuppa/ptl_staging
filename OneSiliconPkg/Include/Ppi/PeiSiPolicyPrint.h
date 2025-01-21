/** @file
  This file defines the PPI to print PEI Silicon Policy settings.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef PEI_SI_POLICY_PRINT_PPI_H_
#define PEI_SI_POLICY_PRINT_PPI_H_

//
// This PPI provides a function to print Si PreMem policy settings
//
typedef
EFI_STATUS
(EFIAPI *PEI_POLICY_PRINT) (
  VOID
  );

typedef struct _PEI_SI_POLICY_PRINT_PPI {
  PEI_POLICY_PRINT PeiPolicyPrint;
} PEI_SI_POLICY_PRINT_PPI;

extern EFI_GUID gSiPolicyPrintPpiGuid;

#endif // PEI_SI_POLICY_PRINT_PPI_H_
