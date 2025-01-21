/** @file
  This file contains functions to get CKD Offsets for CKD Initialization

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

@par Specification Reference: DDR5CK01 Rev0.7
**/

#ifndef _MRC_CKD_OFFSETS_H_
#define _MRC_CKD_OFFSETS_H_

#include "MrcCommon.h"

typedef enum {
  ckdRW0,
  ckdRW1,
  ckdRW2,
  ckdRW3,
  ckdRW4,
  ckdRW5,
  ckdRW6,
  ckdRW7,
  ckdRW29 = 0x29,
  ckdMax
} MrcCkdOffset;

typedef enum {
  ckdIndexRW00,
  ckdIndexRW01,
  ckdIndexRW02,
  ckdIndexRW03,
  ckdIndexRW04,
  ckdIndexRW05,
  ckdIndexRW06,
  ckdIndexRW07,
  ckdIndexRW29,
  ckdIndexMax
} MrcCkdIndex;

/**
  This function converts CKD Control Word Offset to CKD Index to be used in CkdBuffer
  @param[in]  CkdOffset   - CKD Control Word Offset
  @retval Control Word Index. 0xFF Read-Only Control Word
**/
UINT8
MrcCkdOffsetToIndex (
  IN OUT  MrcCkdOffset CkdOffset
  );

/**
  This function converts CKD Index used in CkdBuffer to CKD Control Word Offset
  @param[in]  CkdIndex    - CKD Control Word Offset
  @retval Control Word Offset. 0xFF Read-Only Control Word
**/
UINT8
MrcCkdIndexToOffset (
  IN OUT  MrcCkdIndex CkdIndex
  );

/**
  Function used to get the CR Offset for CKD.
  @param[in]  Group        - PMA Group being accessed.
  @param[in]  Rank         - 0-based index of Rank to access.
  @retval Control Word Offset.
**/
UINT32
MrcGetCkdRegOffset (
  IN  GSM_GT        Group,
  IN  UINT32  const Rank
  );
#endif //_MRC_CKD_OFFSETS_H_
