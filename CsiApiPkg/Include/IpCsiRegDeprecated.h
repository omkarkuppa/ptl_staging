/** @file
  IP Common Services Interface (CSI)

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IP_CSI_REG_DEPRECATED_H_
#define IP_CSI_REG_DEPRECATED_H_

/** DEPRECATED INTERFACES:
 * - This file contains deprecated interfaces for backwards compatability.
 * - These interfaces should not be used in newer code
**/

// please use preferred name REG_POLL_FLD_US
#define REG_POLL_FLD(PINST, REG, FLD, DATA, MAXRETRY, USECDELAY, PCSISTS)                                      \
  do {                                                                                                         \
    REG_STRUCT (REG)                                                                                           \
    _Mask;                                                                                                     \
    _Mask.Data     = 0;                                                                                        \
    _Mask.Data     = ~_Mask.Data;                                                                              \
    _Mask.Bits.FLD = 0;                                                                                        \
    _Mask.Data     = ~_Mask.Data;                                                                              \
    REG_STRUCT (REG)                                                                                           \
    _Match;                                                                                                    \
    _Match.Data     = 0;                                                                                       \
    _Match.Bits.FLD = DATA;                                                                                    \
    _REG_DFX_FLD (PINST, FLAGS, (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD, DATA); \
    IpWrRegPoll (                                                                                              \
      (PINST)->REG_CNTXT (REG),                                                                                \
      REG_OFFSET (REG),                                                                                        \
      REG_BITS (REG),                                                                                          \
      _Mask.Data,                                                                                              \
      _Match.Data,                                                                                             \
      MAXRETRY,                                                                                                \
      (PINST)->TimeCntxt,                                                                                      \
      USECDELAY,                                                                                               \
      PCSISTS);                                                                                                \
  } while (0)

// please use preferred name REG_POLL_FLD_2VAL_US
#define REG_POLL_FLD_2VAL(PINST, REG, FLD, DATA1, DATA2, MAXRETRY, USECDELAY, PCSISTS)                          \
  do {                                                                                                          \
    REG_STRUCT (REG)                                                                                            \
    _Mask;                                                                                                      \
    _Mask.Data     = 0;                                                                                         \
    _Mask.Data     = ~_Mask.Data;                                                                               \
    _Mask.Bits.FLD = 0;                                                                                         \
    _Mask.Data     = ~_Mask.Data;                                                                               \
    REG_STRUCT (REG)                                                                                            \
    _Match1;                                                                                                    \
    _Match1.Data     = 0;                                                                                       \
    _Match1.Bits.FLD = DATA1;                                                                                   \
    REG_STRUCT (REG)                                                                                            \
    _Match2;                                                                                                    \
    _Match2.Data     = 0;                                                                                       \
    _Match2.Bits.FLD = DATA2;                                                                                   \
    _REG_DFX_FLD (PINST, FLAGS, (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD, DATA1); \
    _REG_DFX_FLD (PINST, FLAGS, (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD, DATA2); \
    IpWrRegPoll2Val (                                                                                           \
      (PINST)->REG_CNTXT (REG),                                                                                 \
      REG_OFFSET (REG),                                                                                         \
      REG_BITS (REG),                                                                                           \
      _Mask.Data,                                                                                               \
      _Match1.Data,                                                                                             \
      _Match2.Data,                                                                                             \
      MAXRETRY,                                                                                                 \
      (PINST)->TimeCntxt,                                                                                       \
      USECDELAY,                                                                                                \
      PCSISTS);                                                                                                 \
  } while (0)

#endif //IP_CSI_REG_DEPRECATED_H_

#ifdef __cplusplus
}
#endif
