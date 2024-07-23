/** @file
  Dummy file to support FspVPolicyLib Null library.

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

#include <PiPei.h>
#include <Ppi/SiPolicy.h>

/**
  NULL library instabce. Do nothing.

**/
VOID
FspVPostMemPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  return;
}

/**
  NULL library instabce. Do nothing.

**/
VOID
FspVPreMemPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  return;
}

/**
  NULL library instance. Return size as 0.

  @retval      Return FALSE from NULL library.
**/
UINT16
FspVPostMemGetConfigBlockTotalSize (
  VOID
  )
{
  return FALSE;
}

/**
  NULL library instance. Return size as 0.

  @retval      Return FALSE from NULL library.
**/
UINT16
FspVPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return FALSE;
}

/**
  Add FspV Post-Mem ConfigBlock.

  @param[in] ConfigBlockTableAddress  The pointer to config block table

  @retval EFI_SUCCESS                 Always return EFI_SUCCESS..
**/
EFI_STATUS
FspVPostMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return EFI_SUCCESS;
}

/**
  Add FspV Pre-Mem ConfigBlock.

  @param[in] ConfigBlockTableAddress  The pointer to config block table

  @retval EFI_SUCCESS                 Always return EFI_SUCCESS.
**/
EFI_STATUS
FspVPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return EFI_SUCCESS;
}
