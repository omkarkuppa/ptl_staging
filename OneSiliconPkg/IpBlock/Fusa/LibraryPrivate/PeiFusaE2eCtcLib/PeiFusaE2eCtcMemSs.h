/** @file
  PEI FUSA E2E CTC Memory Subsystem Header File

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef _PEI_FUSA_E2E_CTC_MEM_SUBSYSTEM_H_
#define _PEI_FUSA_E2E_CTC_MEM_SUBSYSTEM_H_

typedef union {
  struct {
    UINT32 DATA_ERR_EN                             :  8;  // Bits 7:0
    UINT32 REQ_ADDR_ERR_EN                         :  1;  // Bits 8:8
    UINT32 RWD_HEADER_ERR_EN                       :  2;  // Bits 10:9
    UINT32 RSVD                                    :  5;  // Bits 15:11
    UINT32 ERR_INJ_MASK                            :  6;  // Bits 21:16
    UINT32 RSVD2                                   :  9;  // Bits 30:22
    UINT32 DATA_ERR_INJ_SRC                        :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_PARITY_ERR_INJ_STRUCT;

typedef union {
  struct {
    UINT32 ERR_ADDRESSLow                          :  32;  // Bits 31:0
    UINT32 ERR_ADDRESSHigh                         :  7;  // Bits 38:32
    UINT32 RESERVED                                :  20;  // Bits 58:39
    UINT32 ERR_SRC                                 :  1;  // Bits 59:59
    UINT32 ERR_TYPE                                :  2;  // Bits 61:60
    UINT32 ERR_STS_OVERFLOW                        :  1;  // Bits 62:62
    UINT32 ERR_STS                                 :  1;  // Bits 63:63
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_PARITYERRLOG_STRUCT;

#define MRC_MSR_MCA5_STATUS                     0x00000415
#define MRC_MSR_MCA5_STATUS_MCACOD_MASK         0xFFFF
#define MRC_MSR_MCA5_STATUS_MSCOD_MASK          0xFFFF0000
#define MC0_PARITY_ERR_INJ_REG                                         (0x0000D9A8)
#define MC1_PARITY_ERR_INJ_REG                                         (0x0001D9A8)
#define MC0_PARITYERRLOG_REG                                           (0x0000D9A0)
#define MC1_PARITYERRLOG_REG                                           (0x0001D9A0)

typedef enum {
  AddrErrEn,
  BeErrEn,
  DataErrEn,
  DataChErrEn,
  IbeccCorr,
  IbeccUncorr
} ParityTest;

typedef enum {
  McBeErr0,
  McBeErr1,
  McDataErr0,
  McDataErr1,
  McDataErr2,
  McDataErr3,
  McDataErr4,
  McDataErr5,
  McDataErr6,
  McDataErr7,
  McAddrErr0
} McParityTestList;

#define OFFSET_CALC_CH(CH0, CH1, CH) (((CH0) + (((CH1) - (CH0)) * (CH))))
#ifndef ARRAY_COUNT
#define ARRAY_COUNT(a) (sizeof (a) / sizeof (a[0]))
#endif

#endif // _PEI_FUSA_E2E_CTC_MEM_SUBSYSTEM_H_
