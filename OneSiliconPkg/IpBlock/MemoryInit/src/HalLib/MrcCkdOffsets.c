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

#include "MrcCkdOffsets.h" // for function prototypes and interface definitions
#include "MrcDdr5Ckd.h"

/**
  This function converts CKD Control Word Offset to CKD Index to be used in CkdBuffer
  @param[in]  CkdOffset   - CKD Control Word Offset

  @retval Control Word Index. 0xFF Read-Only Control Word
**/
UINT8
MrcCkdOffsetToIndex (
  IN  MrcCkdOffset CkdOffset
  )
{
  UINT8 CkdIndex;

  switch (CkdOffset) {
    case ckdRW0:
    case ckdRW1:
    case ckdRW2:
    case ckdRW3:
    case ckdRW4:
    case ckdRW5:
    case ckdRW6:
    case ckdRW7:
      CkdIndex = CkdOffset;
      break;

    case ckdRW29:
      CkdIndex = ckdIndexRW29;
      break;

    default:
      CkdIndex = MRC_IGNORE_ARG_8;
      break;
  }
  return CkdIndex;
}

/**
  This function converts CKD Index used in CkdBuffer to CKD Control Word Offset
  @param[in]  CkdIndex    - CKD Control Word Offset

  @retval Control Word Offset. 0xFF Read-Only Control Word
**/
UINT8
MrcCkdIndexToOffset (
  IN  MrcCkdIndex CkdIndex
  )
{
  UINT8 CkdOffset;

  switch (CkdIndex) {
    case ckdIndexRW00:
    case ckdIndexRW01:
    case ckdIndexRW02:
    case ckdIndexRW03:
    case ckdIndexRW04:
    case ckdIndexRW05:
    case ckdIndexRW06:
    case ckdIndexRW07:
      CkdOffset = CkdIndex;
      break;

    case ckdIndexRW29:
      CkdOffset = ckdRW29;
      break;

    default:
      CkdOffset = MRC_IGNORE_ARG_8;
      break;
  }
  return CkdOffset;
}

/**
  Function used to get the CR Offset for CKD.

  @param[in]  Group        - PMA Group being accessed.
  @param[in]  Rank         - 0-based index of Rank to access.

  @retval CR Offset.
**/
UINT32
MrcGetCkdRegOffset (
  IN  GSM_GT        Group,
  IN  UINT32  const CkdPin
  )
{
  UINT32    Offset;

  switch (Group) {
    case GsmCkdPllMode:
    case GsmCkdIctInputClkTerm:
    case GsmCkdQckClkDisable:
      Offset = CKD_CONFIGURATION_REG;
      break;

    case GsmCkdQckOutputDelayEnable:
      Offset = CKD_OUTPUT_DELAY_REG;
      break;

    case GsmCkdQckDrive:
      Offset = CKD_QCK_SIGNALS_DRIVER_REG;
      break;

    case GsmCkdQckSlewRate:
      Offset = CKD_QCK_SLEW_RATE_REG;
      break;

    case GsmCkdQckOutputDelay:
      Offset = OFFSET_CALC_CH (CKD_QCK0_OUTPUT_DELAY_RANGE_REG, CKD_QCK1_OUTPUT_DELAY_RANGE_REG, CkdPin);
      break;

    case GsmCkdErrorParityCheck:
    case GsmCkdErrorPacketCheck:
      Offset = CKD_I2C_I3C_ERROR_STATUS_REG;
      break;

    case GsmCkdClearParityErrSts:
    case GsmCkdClearPacketErrSts:
      Offset = CKD_I2C_I3C_CLEAR_ERROR_STATUS_REG;
      break;

    default:
      Offset = MRC_UINT32_MAX;
      break;
  }
  return Offset;
}
