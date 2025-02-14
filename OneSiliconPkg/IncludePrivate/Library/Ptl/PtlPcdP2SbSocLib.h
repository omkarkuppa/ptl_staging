/** @file
  Header file for P2SB SoC library.

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

#ifndef _PTLPCD_P2SB_SOC_LIB_H_
#define _PTLPCD_P2SB_SOC_LIB_H_

#include <P2SbController.h>
#include <PcrDefine.h>
#include <Library/SiPolicyLib.h>

/**
  Get P2SB instance for SOC

  @param[in, out] P2SbController        P2SB controller pointer
  @param[in]      Pid                   P2SB 16 bits port ID

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - P2SbController NULL or wrong Segment
**/
EFI_STATUS
PtlPcdGetP2SbController (
  IN OUT P2SB_CONTROLLER         *P2SbController,
  IN     P2SB_PORT_16_ID         Pid
  );

/**
  Get P2SB first instance of P2SB
  First instance corresponds to segment ID 0xF3

  @param[in, out] P2SbController        P2SB controller pointer

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - P2SbController NULL
**/
EFI_STATUS
PtlPcdGetFirstP2SbController (
  IN OUT P2SB_CONTROLLER         *P2SbController
  );

/**
  Get P2SB second instance of P2SB
  Second instance corresponds to segment ID 0xF2

  @param[in, out] P2SbController        P2SB controller pointer

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - P2SbController NULL
**/
EFI_STATUS
PtlPcdGetSecondP2SbController (
  IN OUT P2SB_CONTROLLER         *P2SbController
  );

/**
  The function performs P2SB lock programming for SOC.

  @param[in] SiPolicy         The SI Policy PPI instance
**/
VOID
PtlPcdP2sbLock (
  IN  SI_POLICY_PPI           *SiPolicy
  );

/**
  The function performs P2SB lock programming for second P2SB instance.

  @param[in] SiPolicy         The SI Policy PPI instance
**/
VOID
PtlPcdSecondP2sbLock (
  IN  SI_POLICY_PPI           *SiPolicy
  );

/**
  Checks if second instance of P2SB is hidden.

  @return TRUE if P2SB is hidden.
**/
BOOLEAN
PtlPcdIsSecondP2SbHidden (
  VOID
  );

#endif // _PTLPCD_P2SB_SOC_LIB_H_
