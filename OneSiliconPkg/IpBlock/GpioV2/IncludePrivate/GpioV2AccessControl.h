/** @file
  Header file for defines related to handling Access Control in GPIOV2 library
  
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

#ifndef _GPIOV2_ACCESS_CONTROL_H_
#define _GPIOV2_ACCESS_CONTROL_H_

typedef enum {
  GPIOV2_ACCESS_CONTROL_SAI_GROUPSMMACC_LOCK_REGISTERS = 2,
  GPIOV2_ACCESS_CONTROL_SAI_GROUP4_LOCK_REGISTERS = 4
} GPIOV2_ACCESS_CONTROL_SAI_GROUP;

#endif // _GPIOV2_ACCESS_CONTROL_H_
