/** @file
  This file defines the function to initialize default silicon policy PPI.

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

@par Specification Reference:
**/
#ifndef _PEI_SI_DEFAULT_POLICY_INIT_PPI_H_
#define _PEI_SI_DEFAULT_POLICY_INIT_PPI_H_

//
// Forward declaration for the PEI_SI_DEFAULT_POLICY_INIT_PPI.
//
typedef struct _PEI_SI_DEFAULT_POLICY_INIT_PPI PEI_SI_DEFAULT_POLICY_INIT_PPI;

/**
  Initialize and install default silicon policy PPI
**/
typedef
EFI_STATUS
(EFIAPI *PEI_POLICY_INIT) (
  VOID
  );

///
/// This PPI provides function to install default silicon policy
///
struct _PEI_SI_DEFAULT_POLICY_INIT_PPI {
  PEI_POLICY_INIT   PeiPolicyInit;    ///< PeiPolicyInit()
};

extern EFI_GUID gSiDefaultPolicyInitPpiGuid;

#endif // _PEI_SI_DEFAULT_POLICY_INIT_PPI_H_
