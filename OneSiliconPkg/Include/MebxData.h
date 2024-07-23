/** @file
  MEBx setup data definitions

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
#ifndef _MEBX_DATA_H_
#define _MEBX_DATA_H_

#define MEBX_DATA_VARIABLE_NAME L"MebxData"

#pragma pack(1)

typedef struct {
  UINT8 AmtSol;      ///< Serial Over Lan State. 0: Disabled; 1: Enabled
  UINT8 AmtSrou;     ///< Storage Redirection Over USB-R State. 0: Disabled; 1: Enabled
  UINT8 Reserved[2]; ///< Reserved for future use
} MEBX_DATA;

#pragma pack()

#endif // _MEBX_DATA_H_
