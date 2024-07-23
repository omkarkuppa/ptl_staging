/** @file
  Header file for HSIO Modular PHY SoC layer library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _PTLPCD_HSIO_SOC_LIB_H_
#define _PTLPCD_HSIO_SOC_LIB_H_

#include <Ppi/SiPolicy.h>
#include <P2SbController.h>
#include <Library/P2SbSidebandAccessLib.h>

/**
  Enable dynamic power gating of the PHY

  @param[in out]  P2SbController Pointer to P2SB controller identification structure
  @param[in    ]  SbAccess       Pointer to Sideband access interface structure

**/
VOID
PtlPcdPhyPwrGatingProg (
  IN      P2SB_SIDEBAND_REGISTER_ACCESS  *SbAccess
  );

/**
  Enable the IOMT tool using the EOM bit that is reflected on CSE FW Status register
**/
VOID
PtlPcdPhyBeforeSaiPostBoot (
  IN      P2SB_SIDEBAND_REGISTER_ACCESS  *SbAccess
  );

#endif //_PTLPCD_HSIO_SOC_LIB_H_
