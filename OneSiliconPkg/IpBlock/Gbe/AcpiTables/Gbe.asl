/**@file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

  Method (_DSM, 4, serialized) {
    if (PCIC (Arg0)) {
      return (PCID (Arg0, Arg1, Arg2, Arg3))
    };
    Return (Buffer () { 0 })
  }

  Method (_DSW, 3) {}
