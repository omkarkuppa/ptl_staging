/** @file
  PEI FUSA CFI Parity Header File

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

#ifndef _FUSA_CFI_PARITY_H_
#define _FUSA_CFI_PARITY_H_

#include "PeiFusaResultReportingLib.h"
#include <Protocol/MpService.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include "PeiFusaPrivateLibInternal.h"
#include <Library/PeiFusaLib.h>

#define MSCOD_MASK              0xFFFF0000ULL
#define MCACOD_MASK             0xFFFFULL

typedef enum {
  RX0,
  RX1,
  RX2,
  RX3
} CfiParityRx;

#define MRC_MSR_MCA8_STATUS                     (0x400 + (4 * 8) + 1)
#define MRC_MSR_MCA8_STATUS_MCACOD_MASK         0xFFFF
#define MRC_MSR_MCA8_STATUS_MSCOD_MASK          0xFFFF0000

/**
  Perform CFI Parity Ctc.
**/
VOID
FspDxCheckHbo0E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckHbo1E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckSNCUE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckSVTUE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckPunitE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckIPU_PB_PIDE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckVPU_PB_PIDE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckMEDIA_PBE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckGT_PBE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckIAX_PBE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckDNI2CFIE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckCCE0E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckCCE1E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckCCF_SANTA1E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );
  
VOID
FspDxCheckCCF_SANTA0E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckIOCE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckIOCCEE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

VOID
FspDxCheckIVTUE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
);

VOID
FspDxCheckIdiBE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
);

typedef union {
  struct {
    UINT32 addr_chk_sts        :  1;  // Bits 0:0
    UINT32 be_chk_sts          :  1;  // Bits 1:1
    UINT32 nc_be_chk_sts       :  1;  // Bits 2:2
    UINT32 Rsvd3_15            :  13; // Bits 3:15
    UINT32 addr_chk_en         :  1;  // Bits 16:16
    UINT32 be_chk_en           :  1;  // Bits 17:17
    UINT32 nc_be_chk_en        :  1;  // Bits 18:18
    UINT32 Rsvd19_31           :  13; // Bits 19:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FUSA_IDI_PARITY_STS_CTL_0_STRUCT;

typedef union {
  struct {
    UINT32 addr_fault_res      :  1;  // Bits 0:0
    UINT32 be_fault_res        :  1;  // Bits 1:1
    UINT32 nc_be_fault_res     :  1;  // Bits 2:2
    UINT32 Rsvd3_15            :  13; // Bits 3:15
    UINT32 addr_chk_fault_inj  :  1;  // Bits 16:16
    UINT32 be_chk_fault_inj    :  1;  // Bits 17:17
    UINT32 nc_be_chk_fault_inj :  1;  // Bits 18:18
    UINT32 Rsvd19_31           :  13; // Bits 19:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FUSA_IDI_PARITY_CTC_CTL_0_STRUCT;

typedef enum {
  IdiE2eIdiParityCtc_Addr,
  IdiE2eIdiParityCtc_Be,
  IdiE2eIdiParityCtc_Nc_Be
} IdiE2eIdiParityCtc;

/**
  Perform Idi Parity Ctc.
**/
VOID
FspDxCheckIdiBE2eIdiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

// CCF cNCU Injection
VOID
FspDxCheckCCFE2eNCUParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );  

// IDIB cNCU Injection
VOID
FspDxCheckIDIBE2eNCUParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );  
#endif // _FUSA_CFI_PARITY_H_
