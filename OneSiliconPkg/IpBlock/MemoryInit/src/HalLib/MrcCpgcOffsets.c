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
#include "MrcCpgcOffsets.h" // for function prototypes and interface definitions
#include "CMcAddress.h"

/**
  This function returns the offset of the error status register.

  @param[in]  Controller - 0-based index selecting Controller to program.
  @param[in]  Channel    - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetTestErrStatOffset (
  IN  UINT32                Controller,
  IN  UINT32                Channel
  )
{
  return OFFSET_CALC_MC_CH (
          MC0_CH0_CR_CPGC_ERR_TEST_ERR_STAT_REG,
          MC1_CH0_CR_CPGC_ERR_TEST_ERR_STAT_REG, Controller,
          MC0_CH1_CR_CPGC_ERR_TEST_ERR_STAT_REG, Channel);
}

/**
  This function returns the offset of the error control register.

  @param[in]  Controller - 0-based index selecting Controller to program.
  @param[in]  Channel    - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetTestErrCtlOffset (
  IN  UINT32                Controller,
  IN  UINT32                Channel
  )
{
  return OFFSET_CALC_MC_CH (
          MC0_CH0_CR_CPGC_ERR_CTL_REG,
          MC1_CH0_CR_CPGC_ERR_CTL_REG, Controller,
          MC0_CH1_CR_CPGC_ERR_CTL_REG, Channel);
}

/**
  This function returns the offset of the disabling errors per bit in byte 0-3 register.

  @param[in]  Controller - 0-based index selecting Controller to program.
  @param[in]  Channel    - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetCpgcErrCheckingLowOffset (
  IN  UINT32                Controller,
  IN  UINT32                Channel
  )
{
  return OFFSET_CALC_MC_CH (
          MC0_CH0_CR_CPGC_ERR_LNEN_LO_REG,
          MC1_CH0_CR_CPGC_ERR_LNEN_LO_REG, Controller,
          MC0_CH1_CR_CPGC_ERR_LNEN_LO_REG, Channel);
}

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
  )
{
  return OFFSET_CALC_MC_CH (
          MC0_CH0_CR_CPGC_ERR_LNEN_HI_REG,
          MC1_CH0_CR_CPGC_ERR_LNEN_HI_REG, Controller,
          MC0_CH1_CR_CPGC_ERR_LNEN_HI_REG, Channel);
}

/**
  This function returns the register offset for the ECC error mask and Rank error mask register.

  @param[in]  Controller - 0-based index specifying which Controller control to program.
  @param[in]  Channel    - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetEccErrMskRankErrMskOffset (
  IN  UINT32                Controller,
  IN  UINT32        const   Channel
  )
{
  return OFFSET_CALC_MC_CH (
          MC0_CH0_CR_CPGC_ERR_XLNEN_REG,
          MC1_CH0_CR_CPGC_ERR_XLNEN_REG, Controller,
          MC0_CH1_CR_CPGC_ERR_XLNEN_REG, Channel);
}

/**
  This function returns the register offset for the CPGC Sequence Config register.
  Lp Channel expected to be taken care of outside of the function.

  @param[in]  Controller - 0-based index specifying which Controller control to program.
  @param[in]  Channel    - 0-based index specifying which channel control to program.

  @retval - MCHBAR Register Offset.
**/
UINT32
MrcGetCpgcSeqCfgOffset (
  IN  UINT32                Controller,
  IN  UINT32        const   Channel
  )
{
  return OFFSET_CALC_MC_CH (
          MC0_REQ0_CR_CPGC_SEQ_CFG_A_REG,
          MC1_REQ0_CR_CPGC_SEQ_CFG_A_REG, Controller,
          MC0_REQ1_CR_CPGC_SEQ_CFG_A_REG, Channel);
}

