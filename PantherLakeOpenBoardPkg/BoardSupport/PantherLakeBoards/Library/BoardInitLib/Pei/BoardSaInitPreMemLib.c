/**@file
  Source code for the board SA configuration Pcd init functions in Pre-Memory init phase.

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
#include <Ppi/SiPolicy.h>
#include <Pins/GpioPinsVer2Lp.h>
#include <PlatformBoardId.h>
#include <PlatformOpenBoardConfig.h>

/**
  MRC configuration init function for PEI pre-memory phase.

  @param[in]  BoardId           An unsigned integer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
PtlSaMiscConfigInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Board Memory Init related configuration init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
PtlMrcConfigInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Board SA related GPIO configuration init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
PtlSaGpioConfigInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  SA Display DDI configuration init function for PEI pre-memory phase.

  @param[in]  BoardId       An unsigned integer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
PtlSaDisplayConfigInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

