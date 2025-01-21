/** @file
  This file contains functions to get CPGC Offsets
  used memory training.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#ifndef _MRC_CPGC_OFFSETS_H_
#define _MRC_CPGC_OFFSETS_H_

#include "MrcCommon.h"

typedef enum {
  MrcClMuxRd,
  MrcClMuxWr,
  MrcClMuxMax
} MRC_CL_MUX_DIR;

/**
  This function returns the offset of the error status register.

  @param[in]  Controller - 0-based index selecting Controller to program.
  @param[in]  Channel - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetTestErrStatOffset (
  IN  UINT32                Controller,
  IN  UINT32                Channel
  );

/**
  This function returns the offset of the error control register.

  @param[in]  Controller - 0-based index selecting Controller to program.
  @param[in]  Channel - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetTestErrCtlOffset (
  IN  UINT32                Controller,
  IN  UINT32                Channel
  );

/**
  This function returns the offset of the disabling errors per bit in byte 0-3 register.

  @param[in]  Controller - 0-based index selecting Controller to program.
  @param[in]  Channel - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetCpgcErrCheckingLowOffset (
  IN  UINT32                Controller,
  IN  UINT32                Channel
  );

/**
  This function returns the offset of the disabling errors per bit in byte 4-7 register for DDR4.  Same register offset is used as Chunk check Enabler for LPDDR4.

  @param[in]  Controller - 0-based index selecting Controller to program.
  @param[in]  Channel    - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetCpgcErrCheckingHighOffset (
  IN  UINT32                Controller,
  IN  UINT32                Channel
  );

/**
  This function returns the register offset for the ECC error mask and Rank error mask register.

  @param[in]  Controller - 0-based index specifying which Controller control to program.
  @param[in]  Channel - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetEccErrMskRankErrMskOffset (
  IN  UINT32                Controller,
  IN  UINT32        const   Channel
  );

/**
  This function returns the register offset for the CPGC seq config register.

  @param[in]  Controller - 0-based index specifying which Controller control to program.
  @param[in]  Channel - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetCpgcSeqCfgOffset (
  IN  UINT32                Controller,
  IN  UINT32        const   Channel
  );

/**
  This returns the read or write offset for the Cacheline Mux Pattern Buffer.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[in]  Direction - Specifies which traffic Mux to program.
  @param[in]  Index     - 0-based index specifying which Mux to program.

  @retval - MCHBAR Register Offset.

UINT32
MrcGetClMuxPbOffset (
  IN  MrcParameters   *const  MrcData,
  IN  MRC_CL_MUX_DIR  const   Direction,
  IN  UINT32          const   Index
  );
*/
#endif //_MRC_CPGC_OFFSETS_H_

