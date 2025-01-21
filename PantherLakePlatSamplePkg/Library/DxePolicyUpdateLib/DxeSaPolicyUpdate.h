/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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
#ifndef _DXE_SA_POLICY_UPDATE_H_
#define _DXE_SA_POLICY_UPDATE_H_

#include <PiDxe.h>
#include <CpuRegs.h>
#include <SetupVariable.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/IGpuPolicy.h>
#include <Library/CpuPlatformLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/LocalApicLib.h>

//
// Haddock Creek
//
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

#endif
