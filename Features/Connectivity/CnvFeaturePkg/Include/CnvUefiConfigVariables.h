/** @file
  Header file to define internal macro, structures for CNV variable implementation

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

@par Specification
**/

#ifndef __CNV_UEFI_VARIABLES_H__
#define __CNV_UEFI_VARIABLES_H__

//
// Statements that include other header files
//
#if (FixedPcdGet16 (PcdCnvUefiVarVersion) >= 2)
  #include <CnvUefiConfigVariablesVer2.h>
#endif
#include <PiDxe.h>
#include <Protocol/VariableLock.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/CnvUefiProtocol.h>
#include <Library/VariablePolicyHelperLib.h>

#define WIFI_DOMAIN_VALUE  0x07

/**
  Update/Create Connectivity UEFI variable

  @param[in] CnvVarSize      Connectivity variable size
  @param[in] VariableName    Pointer to variable name
  @param[in] VendorGuid      Pointer to variable vendor guid
  @param[in] NewData         Pointer to variable data
  @param[in] LockVariable    Variable to be kept locked/unlocked
  @param[in] CheckVarUpdate  Check if variable needs to be updated

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
UpdateVariables (
  IN UINTN     CnvVarSize,
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid,
  IN VOID      *NewData,
  BOOLEAN      LockVariable,
  IN BOOLEAN   CheckVarUpdate
  );

/**
  Connectivity Config Initialization

  @param[in] CnvConfig       Pointer to Connectivity Configurations

  @retval EFI_SUCCESS        The function completed successfully

**/
VOID
CnvConfigInit (
  IN UEFI_CNV_CONFIG  *CnvConfig
  );

/**
  Install CNV UEFI Variables

  @param[in] CnvConfig       Pointer to Connectivity Configurations

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallCnvUefiVariables (
  IN UEFI_CNV_CONFIG  *CnvConfig
  );

#endif
