/** @file
  Fru library to return reserved 64-bit MMIO space.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

/**
  Return the MMIO size.

  @retval The size of 64 bit MMIO that need to reserve.
**/

#include <Base.h>
#include <Defines/PchReservedResources.h>
#include <Library/PcdReserveMmio64SizeLib.h>

UINT64
ReserveMmio64SizeFru (
  VOID
  )
{
  return 0;
}