/** @file
  Userial layer header.

  Copyright (c) 2017, Intel Mobile Communications GmbH. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef TL_H_
#define TL_H_

#include "bt_types.h"

INT8
tl_init (
  char  *device,
  int   ispeed
  );

INT8
tl_write (
  UINT8   *buffer,
  UINT16  len
  );

void
tl_recv_msg (
  UINT8  *buffer
  );

void
tl_close (
  );

#endif /* TL_H_ */
