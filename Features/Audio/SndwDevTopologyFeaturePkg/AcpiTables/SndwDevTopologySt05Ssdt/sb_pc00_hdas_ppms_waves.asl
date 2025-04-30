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

If (Arg0 == ToUUID ("faacc8cc-b365-4964-b4b8-bd4deb18d922")) /* Waves Reduced */
{
    Return (1)
}
If (Arg0 == ToUUID ("52983d04-2414-88b4-a2a2-c1397e13b022")) /* Waves Full */
{
    Return (1)
}
