/** @file
  ACPI EC LID APIs that consumed by IGFX.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
  RLDS        Get Lid status.

  @param      None.
  @retval     Return the LID status: 1 - LID open, 0 - LID close.
**/
Method (RLDS)
{
  Return (ECRD (RefOf (LSTE)))
}

/**
  UPLD      Update the Lid status in EC ram.

  @param    Arg0  - 1 byte date to update the Lid status.
  @retval   None.

**/
Method (UPLD, 1, Serialized)
{
  ECWT (Arg0, RefOf (^LSTE))
}
