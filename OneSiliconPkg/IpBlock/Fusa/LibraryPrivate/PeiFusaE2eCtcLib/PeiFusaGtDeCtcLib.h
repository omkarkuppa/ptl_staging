/** @file

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
**/
#ifndef _FUSA_GT_DE_CTC_H_
#define _FUSA_GT_DE_CTC_H_

#include "PeiFusaResultReportingLib.h"
#include <Protocol/MpService.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include "PeiFusaPrivateLibInternal.h"
#include <Library/PeiFusaLib.h>

#define L3BANKLOCK_LBCF_GFX_GT_REG       (0x0000b160)
#define FORCE_WAKE_GPM_GRP_GCD           (0x0000a188)
#define DEV_ERR_ROUTING_CTRL             (0x00100170)
#define LBCFLOCKMSGREG_LBCF_GFX          (0x0000b1b4)
#define ERR_STAT_GT_CORRECTABLE_2        (0x001002a8)
#define ERR_STAT_GT_FATAL_2              (0x00100268)
#define DEVICECTL_0_2_0_PCI              (0x78)
/**
  Perform GT L3 ECC Ctc
**/
VOID
FspDxCheckGtL3EccCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

#endif // _FUSA_GT_DE_CTC_H_
