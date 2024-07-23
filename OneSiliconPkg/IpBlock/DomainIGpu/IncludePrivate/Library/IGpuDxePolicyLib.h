/** @file
  Header file for the IGpu DXE Policy Init library.

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

@par Specification Reference:
**/

#ifndef _IGPU_DXE_POLICY_LIB_H_
#define _IGPU_DXE_POLICY_LIB_H_

#include <Uefi.h>
#include <Protocol/IGpuPolicy.h>
#include <ConfigBlock.h>
#include <IGpuConfig.h>

#define DWORD_FIELD_VALID_BIT  BIT31

extern EFI_GUID gIGpuDxeConfigGuid;

/**
  This function prints the IGPU DXE phase policy.

  @param[in] IGpuPolicy - IGPU DXE Policy protocol
**/
VOID
IGpuDxePolicyPrint (
  IN  IGPU_POLICY_PROTOCOL      *IGpuPolicy
  );

/**
  This function Load default IGPU DXE policy.

  @param[in] ConfigBlockPointer    The pointer to add IGPU config block
**/
VOID
IGpuLoadDxeDefault (
  IN VOID    *ConfigBlockPointer
  );


/**
  Get DXE IGPU config block table total size.

  @retval     Size of DXE IGPU config block table
**/
UINT16
EFIAPI
IGpuGetDxeConfigBlockTotalSize (
  VOID
  );

/**
  IGpuAddDxeConfigBlocks add all DXE IGPU config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
IGpuAddDxeConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _IGPU_DXE_POLICY_LIB_H_
