/** @file
  A library for Setup variable initialization in PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _SETUP_INIT_LIB_H_
#define _SETUP_INIT_LIB_H_

#include <PiPei.h>
#include <Uefi.h>

#define SIZE_OF_FIELD(TYPE, Field) (sizeof (((TYPE *)0)->Field))

#define SIZE_OF_TABLE(TABLE, TYPE) (sizeof (TABLE) / sizeof (TYPE))

/**
  Updates the Setup variable revision for the Setup structures.

  @retval         EFI_SUCCESS    The revision for the Setup structures was updated successfully.
**/
EFI_STATUS
EFIAPI
UpdateAllSetupVariableRevisions (
  VOID
  );

/**
  Prints all Setup UEFI variables located in the UEFI variable store.

  @retval EFI_SUCCESS   An error did not occur located the UEFI variables and printing the contents.
  @retval Others        An error occurred locating the UEFI variables.

**/
EFI_STATUS
EFIAPI
PrintSetupStructures (
  VOID
  );

/**
  Checks if Setup UEFI variables are changed in the UEFI variable store.

  @retval BOOLEAN   Indicates whether the UEFI variable was changed, if changed, return false.

**/
BOOLEAN
EFIAPI
IsSetupStructuresUnChanged (
  VOID
  );
#endif
