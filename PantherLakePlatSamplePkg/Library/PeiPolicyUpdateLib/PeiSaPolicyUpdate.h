/** @file

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
#ifndef _PEI_SA_POLICY_UPDATE_H_
#define _PEI_SA_POLICY_UPDATE_H_

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//
#include <SetupVariable.h>
#include <Ppi/SiPolicy.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Setup/SaSetup.h>
#include <Ppi/Wdt.h>
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>
#include "PeiPchPolicyUpdate.h"
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#include <FspsUpd.h>
#endif

///
/// DIMM SMBus addresses for Haddock Creek
///
#define DIMM_SMB_SPD_P0C0D0_HC 0xA2
#define DIMM_SMB_SPD_P0C0D1_HC 0xA0
#define DIMM_SMB_SPD_P0C1D0_HC 0xA6
#define DIMM_SMB_SPD_P0C1D1_HC 0xA4
#define DIMM_SMB_SPD_P0C0D2_HC 0xAA
#define DIMM_SMB_SPD_P0C1D2_HC 0xA8

//
// Sawtooth Peak
// Single SPD EEPROM at 0xA2 serves both C0D0 and C1D0 (LPDDR is 1DPC only)
//
#define DIMM_SMB_SPD_P0C0D0_STP 0xA2
#define DIMM_SMB_SPD_P0C0D1_STP 0xA0
#define DIMM_SMB_SPD_P0C1D0_STP 0xA2
#define DIMM_SMB_SPD_P0C1D1_STP 0xA0


#define WDT_TIMEOUT 60

// BClk Frequency Limitations (in Hz)
#define BCLK_MAX                600000000
#define BCLK_100                100000000
#define BCLK_GRANULARITY        1000000
#define MAX_BCLK_RFI_POINTS     4
#ifndef MAX_IBECC_REGIONS
#define MAX_IBECC_REGIONS       8  ///< Maximum number of protection regions handled by IBECC
#endif

#define VGA_LOGO_WIDTH 200
#define VGA_LOGO_HEIGHT 58
extern CONST UINT8  VgaPlanarImage200x58[4][58][25];
extern CONST UINT8  C8x16_Character_Set[256][16];

#endif

