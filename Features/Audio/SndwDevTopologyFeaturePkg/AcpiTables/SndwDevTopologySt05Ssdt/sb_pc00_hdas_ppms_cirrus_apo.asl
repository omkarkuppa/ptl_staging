/** @file
  SoundWire topology definitions

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

If (Arg0 == ToUUID ("76171558-7b4d-42ee-8900-74b3ed69775a")) /* Cirrus MFX APO */
{
    Return (1)
}
If (Arg0 == ToUUID ("4dba4525-0cb8-4a09-b73d-e915fd8bc7bc")) /* Cirrus EFX APO */
{
    Return (1)
}
If (Arg0 == ToUUID ("c97d0473-6918-4173-8e42-59cf432d8533")) /* Cirrus EFX UAJ APO */
{
    Return (1)
}
