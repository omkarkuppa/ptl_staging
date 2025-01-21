/** @file
  Header file for common definition of USB4 Connection Manager driver implementation

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

#ifndef _USB4_COMMON_H_
#define _USB4_COMMON_H_

#include <Uefi.h>

#define CM_STATUS_WAITS_MAX        3000
#define CM_STATUS_POLL_US          10

#define USB4_TRAINING_WAITS        (1000000 / CM_STATUS_POLL_US)   // 1 second

//
// DP stream array size
//
#define USB4_DP_STREAM_MAX           8

#define USB4_HR_SUPPORT_MAX          4
#define USB4_ROUTER_SUPPORT_MAX     64   // Maximum routers can be supported in one USB4 domain
#define USB4_ADAPTER_SUPPORT_MAX    64

//
// USB4 CM version
//
#define USB4_CM_VERSION_1          1
#define USB4_CM_VERSION_2          2

typedef VOID *    USB4_HR_CONTEXT;
typedef VOID *    USB4_HI_CORE_CONTEXT;
typedef VOID *    USB4_DOMAIN_CONTEXT;

#endif
