/** @file
  Header file for USB4 host interface core implementation.

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

#ifndef _USB4_HI_CORE_H_
#define _USB4_HI_CORE_H_

#include <Uefi.h>
#include <Library/Usb4HiCoreDefs.h>

EFI_STATUS
Usb4CsIoInit (
  OUT USB4_CS_IO    *This
  );

#endif
