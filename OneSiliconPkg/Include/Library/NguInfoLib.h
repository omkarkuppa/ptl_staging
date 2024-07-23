/** @file
  @ Header file to get NGU information.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _NGU_INFO_LIB_H_
#define _NGU_INFO_LIB_H_

#include <Library/Ptl/Noc/NguInfoFruLib.h>


#include <Uefi.h>

typedef enum {
// IMR
  NPK = 0,         // 0
  PRMRR,           // 1
  TSE,             // 2
  STREAM_TRACER,   // 3
  SEAMRR,          // 4
//IMR_1M
  GSM,          // 5
  DSM,          // 6
  CCS,          // 7
  DPR,          // 8
  TSEG,         // 9
  CSE,
} NGU_IMR;

typedef enum {
  IMR_BASE,
  IMR_BASE_RAW,
  IMR_MASK,
  IMR_LIMIT,
  IMR_SIZE,
  IMR_BASE_BYTE,
  IMR_MASK_BYTE,
  IMR_LIMIT_BYTE,
  IMR_SIZE_BYTE
} NGU_IMR_DATATYPE;

/**
  GetImrPrmrrBaseOffset: Get IMR PRMRR Base Offset value

  @retval IMR PRMRR Base Offset
**/
UINT32
EFIAPI
GetImrPrmrrBaseOffset (
  VOID
  );

/**
  GetImrPrmrrBaseOffset: Get IMR PRMRR Mask Offset value

  @retval IMR PRMRR Mask Offset
**/
UINT32
EFIAPI
GetImrPrmrrMaskOffset (
  VOID
  );

/**
  GetImrGsmBaseOffset: Get IMR GSM Base Offset value

  @retval IMR GSM Base Offset
**/
UINT64
EFIAPI
GetImrGsmBaseOffset (
  VOID
  );

/**
  GetImrDsmBaseOffset: Get IMR DSM Base Offset value

  @retval IMR DSM Base Offset
**/
UINT64
EFIAPI
GetImrDsmBaseOffset (
  VOID
  );

/**
  GetImrTsegBaseOffset: Get IMR TSEG Base Offset value

  @retval IMR TSEG Base Offset
**/
UINT64
EFIAPI
GetImrTsegBaseOffset (
  VOID
  );

/**
  GetImr1m10BaseOffset: Get IMR1M10 Base Offset value

  @retval IMR1M10 Base Offset
**/
UINT32
EFIAPI
GetImr1m10BaseOffset (
  VOID
  );

/**
  GetImr1m10MaskOffset: Get IMR1M10 Mask Offset value

  @retval IMR1M10 Mask Offset
**/
UINT32
EFIAPI
GetImr1m10MaskOffset (
  VOID
  );

/**
  GetImrGsmMask: Get IMR GSM mask value

  @retval IMR GSM mask
**/
UINT64
EFIAPI
GetImrGsmMask (
  VOID
  );

/**
  GetImrDsmMask: Get IMR DSM mask value

  @retval IMR DSM mask
**/
UINT64
EFIAPI
GetImrDsmMask (
  VOID
  );

/**
  GetImrCcsBase: Get IMR CCS Base address

  @retval IMR CCS Base address
**/
UINT64
EFIAPI
GetImrCcsBase (
  VOID
  );

/**
  GetImrTsegMask: Get IMR TSEG mask value

  @retval IMR TSEG mask
**/
UINT64
EFIAPI
GetImrTsegMask (
  VOID
  );

/**
  GetImrCcsMask: Get IMR CCS mask value

  @retval IMR CCS mask
**/
UINT64
EFIAPI
GetImrCcsMask (
  VOID
  );

/**
  GetImrDprBase: Get IMR DPR Base address

  @retval IMR DPR Base address
**/
UINT64
EFIAPI
GetImrDprBase (
  VOID
  );

/**
  GetImrDprMask: Get IMR DPR mask value

  @retval IMR DPR mask
**/
UINT64
EFIAPI
GetImrDprMask (
  VOID
  );

/**
  This function returns the Remap Base value

  @retval Remap Base
**/
UINT64
EFIAPI
GetRemapBase (
  VOID
  );

/**
  This function returns the Remap Limit value

  @retval Remap Limit
**/
UINT64
EFIAPI
GetRemapLimit (
  VOID
  );

/**
  This function is provide Imr related details (Base, Mast, Limit).

  @param[in] ImrName     - IMR name or ID
  @param[in] ImrDataType - Requested data (Base, Mast, Limit)

  @retval UINT64           - Register Details based on DataType

**/
UINT64
EFIAPI
GetImrRegisterData (
  IN NGU_IMR              ImrName,
  IN NGU_IMR_DATATYPE     ImrDataType
  );

#endif /* _NGU_INFO_LIB_H_ */
