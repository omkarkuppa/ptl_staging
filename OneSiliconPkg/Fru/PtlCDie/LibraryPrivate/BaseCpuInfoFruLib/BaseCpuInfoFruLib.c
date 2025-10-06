/** @file
  This file contains the Cpu Information for specific generation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#include <Register/CpuGenRegs.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Register/Cpuid.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuCommonLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Register/GenerationMsr.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/SmBios.h>
#include <Library/PcdLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/HobLib.h>
#include <OverclockingConfig.h>
#include <OcDataHob.h>
#include <Library/CpuLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Pi/PiMultiPhase.h>
#include <Protocol/MpService.h>
#include <CpuPowerManagementPrivateDataHob.h>
#include <Library/CpuMailboxLib.h>
#include <Library/FixedPointLib.h>
#include <Register/B2pMailbox.h>

STATIC CONST CHAR8 mCpuFamilyString[] = "PantherLake";
STATIC CONST CHAR8 mCpuFamilyStringWcl[] = "WildcatLake";
STATIC CHAR8 mPtlSocketTypeString[] = "BGA2540";
STATIC CHAR8 mWclSocketTypeString[] = "BGA1516";

#define NOCARE 0xFF

typedef struct {
  UINT32  CPUID;
  UINT8   SRID;
  CHAR8   String[16];
} CPU_REV;

GLOBAL_REMOVE_IF_UNREFERENCED CONST CPU_REV  mProcessorRevisionTable[] = {
  //
  //                        CPUID                              SRID      String
  //
  {CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE  + EnumPtlHA0,  NOCARE,   "A0"},
  {CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE  + EnumPtlUA0,  NOCARE,   "A0"},
  {CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE  + EnumPtlUHB0, NOCARE,   "B0"},
  {CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE  + EnumPtlUA1,  NOCARE,   "A1"},
