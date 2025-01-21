/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _CPU_UNIT_TEST_CAPABILITY_H_
#define _CPU_UNIT_TEST_CAPABILITY_H_

//
// Logical description of CPU characteristics
//
typedef struct {
  //
  // Basic
  //
  UINT32    FamilyModelStepping;
  UINT16    DeviceId0;
  UINT8     PowerUnit;          // Value programmed in MSR is in unit of 2^PowerUnit.
  UINT8     ThermalDesignPower; // Package TDP Power
  UINT16    MaxPower;           // Maximum PL1 Power Limit
  UINT16    MinPower;           // Minimum PL1 Power Limit
  UINT16    MaxTimeWindow;      // Maximum Time Window for PL1
  UINT8     MaxNonTurboRatio;   // P1 Ratio
  UINT8     MaxEfficiencyRatio; // Pn Ratio
} CPU_UNIT_TEST_BASIC;

typedef struct {
  //
  // Config TDP: Nominal, Level 1, Level 2
  // 0 scaling factor indicates no such level.
  // All 0 indicates no config TDP capability.
  //
  // Level[i].P1Ratio = MaxNonTurboRatio * ConfigTdpScalingFactor[i] / 100. All scaling factors <= 1.
  // Level[i].Tdp     = ThermalDesignPower * ConfigTdpScalingFactor[i] / ConfigTdpScalingFactor[0]. Nominal.Tdp = ThermalDesignPower.
  //
  UINT16    ConfigTdpScalingFactor[3];
} CPU_UNIT_TEST_CONFIG_TDP;

typedef struct {
  //
  // Turbo
  //
  //
  // TurboBins == 0 indicates Turbo is not supported: IA32_MISC_ENABLE[38] = 0 AND CPUID.06H.EAX[1] = 0.
  // (Turbo is not disabled in MSR but CPUID still claims no capability)
  //
  UINT8      TurboBins;             // P0 Ratio - P1 Ratio.
  BOOLEAN    TurboSupportedInCpuid; // CPUID.06H.EAX[1] when TurboBins != 0
} CPU_UNIT_TEST_TURBO;

typedef struct {
  //
  // Tdx Supported
  //
  BOOLEAN  TdxSupported;
} CPU_UNIT_TEST_TDX;

typedef struct {
  CPU_UNIT_TEST_BASIC         Basic;
  CPU_UNIT_TEST_CONFIG_TDP    ConfigTdp;
  CPU_UNIT_TEST_TURBO         Turbo;
  CPU_UNIT_TEST_TDX           Tdx;
} CPU_UNIT_TEST_CPU_CAPABILITY;

#endif
