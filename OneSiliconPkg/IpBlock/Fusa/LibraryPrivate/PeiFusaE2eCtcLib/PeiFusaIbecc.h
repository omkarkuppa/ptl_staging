/** @file
  PEI FUSA IBECC Header File

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

#ifndef _FUSA_IBECC_H_
#define _FUSA_IBECC_H_

#include "PeiFusaResultReportingLib.h"
#include <Protocol/MpService.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include "PeiFusaPrivateLibInternal.h"

#define MC0_IBECC_ACTIVATE_REG                                         (0x0000D400)
#define MC0_IBECC_PARITY_CONTROL_REG                                   (0x0000D498)
#define MC0_IBECC_PARITY_ERR_LOG_REG                                   (0x0000D578)
#define MC0_IBECC_PARITY_ERR_INJ_REG                                   (0x0000D590)

#define MC0_IBECC_ECC_ERROR_LOG_REG                                    (0x0000D570)
#define MC0_IBECC_ECC_INJ_ADDR_BASE_REG                                (0x0000D588)
#define MC0_IBECC_ECC_INJ_ADDR_MASK_REG                                (0x0000D580)
#define MC0_IBECC_ECC_INJ_CONTROL_REG                                  (0x0000D598)

#define MRC_MSR_MC5_STATUS                     0x400 + ((5*4)+1)
#define MRC_MSR_MC5_STATUS_MCACOD_MASK         0xFFFF
#define MRC_MSR_MC5_STATUS_MSCOD_MASK          0xFFFF0000

typedef enum {
  WR_DATA_ERR,
  RD_ADDR_ERR,
  WR_HDR_ERR,
  RD_DATA_ERR,
  RD_HDR_ERR,
  IbeccCorr,
  IbeccUncorr
} IbeccParityEccTest;

typedef enum {
  ParityTrafficWrite,
  ParityTrafficRead
} ParityTestTrafficMode;

typedef union {
  struct {
    UINT32 RADDR_PARITY_CHK_EN                     :  1;  // Bits 0:0
    UINT32 RDATA_PARITY_CHK_EN                     :  1;  // Bits 1:1
    UINT32 RHEADER_PARITY_CHK_EN                   :  1;  // Bits 2:2
    UINT32 WHEADER_PARITY_CHK_EN                   :  1;  // Bits 3:3
    UINT32 WDATA_PARITY_CHK_EN                     :  1;  // Bits 4:4
    UINT32 RSVD0                                   :  3;  // Bits 7:5
    UINT32 DIS_PARITY_PCIE_ERROR                   :  1;  // Bits 8:8
    UINT32 DIS_PARITY_LOG                          :  1;  // Bits 9:9
    UINT32 RSVD1                                   :  21;  // Bits 30:10
    UINT32 PARITY_CHK_EN                           :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_PARITY_CONTROL_STRUCT;

typedef union {
  struct {
    UINT32 WR_DATA_ERR_EN                          :  8;  // Bits 7:0
    UINT32 RD_ADDR_ERR_EN                          :  1;  // Bits 8:8
    UINT32 WR_HDR_ERR_EN                           :  2;  // Bits 10:9
    UINT32 RD_DATA_ERR_EN                          :  8;  // Bits 18:11
    UINT32 RD_HDR_ERR_EN                           :  2;  // Bits 20:19
    UINT32 RSVD3                                   :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_PARITY_ERR_INJ_STRUCT;

typedef union {
  struct {
    UINT32 RSVD1                                   :  4;  // Bits 3:0
    UINT32 PERR_OVERFLOW                           :  1;  // Bits 4:4
    UINT32 ERR_ADDRESSLow                          :  27;  // Bits 31:5
    UINT32 ERR_ADDRESSHigh                         :  7;  // Bits 38:32
    UINT32 RSVD2                                   :  21;  // Bits 59:39
    UINT32 ERR_TYPE                                :  3;  // Bits 62:60
    UINT32 ERR_STS                                 :  1;  // Bits 63:63
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_PARITY_ERR_LOG_STRUCT;

typedef union {
  struct {
    UINT32 RSVD1                                   :  5;  // Bits 4:0
    UINT32 ADDRESSLow                              :  27;  // Bits 31:5
    UINT32 ADDRESSHigh                             :  7;  // Bits 38:32
    UINT32 RSVD2                                   :  25;  // Bits 63:39
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_ECC_INJ_ADDR_MASK_STRUCT;

typedef MC0_IBECC_ECC_INJ_ADDR_MASK_STRUCT MC0_IBECC_ECC_INJ_ADDR_BASE_STRUCT;

typedef union {
  struct {
    UINT32 ECC_INJECT                              :  3;  // Bits 2:0
    UINT32 RSVD1                                   :  5;  // Bits 7:3
    UINT32 COUNT                                   :  8;  // Bits 15:8
    UINT32 RSVD2                                   :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_IBECC_ECC_INJ_CONTROL_STRUCT;

typedef union {
  struct {
    UINT32 RSVD0                                   :  3;  // Bits 2:0
    UINT32 CERR_OVERFLOW                           :  1;  // Bits 3:3
    UINT32 MERR_OVERFLOW                           :  1;  // Bits 4:4
    UINT32 ERRADDLow                               :  27;  // Bits 31:5
    UINT32 ERRADDHigh                              :  7;  // Bits 38:32
    UINT32 RSVD1                                   :  7;  // Bits 45:39
    UINT32 ERRSYND                                 :  16;  // Bits 61:46
    UINT32 CERRSTS                                 :  1;  // Bits 62:62
    UINT32 MERRSTS                                 :  1;  // Bits 63:63
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_IBECC_ECC_ERROR_LOG_STRUCT;

VOID
FspDxCheckIbeccParity (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckIbeccEccInj (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

#endif // _FUSA_IBECC_H_
