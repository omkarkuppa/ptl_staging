/** @file
  Storage sanitize common definition

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

@par Specification Reference:
**/

#ifndef _STORAGE_SANITIZE_DEFINITIONS_H_
#define _STORAGE_SANITIZE_DEFINITIONS_H_

#define MANUFACURER_MAX_LEN  20
#define SN_MAX_LEN           20
#define MN_MAX_LEN           40
#define PASSWORD_MAX_LENGTH  32

#define SANITIZE_COUNT         4
#define SANITIZE_CLEAR         0x01
#define SANITIZE_OVR_ERASE     0x02
#define SANITIZE_BLOCK_ERASE   0x03
#define SANITIZE_CRYPTO_ERASE  0x04

#endif  // _STORAGE_SANITIZE_DEFINITIONS_H_
