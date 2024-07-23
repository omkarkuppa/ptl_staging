/** @file
  This file contains define definitions specific to processor

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _POWER_MGMT_DEFINITIONS_H_
#define _POWER_MGMT_DEFINITIONS_H_

#define CSTATE_SUPPORTED          0x1
#define ENHANCED_CSTATE_SUPPORTED 0x2
#define C6_C7_SHORT_LATENCY_SUPPORTED 0x01
#define C6_C7_LONG_LATENCY_SUPPORTED  0x02
#define C7s_SHORT_LATENCY_SUPPORTED   0x03
#define C7s_LONG_LATENCY_SUPPORTED    0x04
//
// Voltage offset definitions
//
#define OC_LIB_OFFSET_ADAPTIVE  0
#define OC_LIB_OFFSET_OVERRIDE  1
//
// Platform Power Management Flags Bit Definitions:
//   These defines are also used in CPU0CST.ASL to check platform configuration
//   and build C-state table accordingly.
//
#define PPM_EIST                BIT0   ///< Enhanced Intel Speed Step Technology.
#define PPM_C1                  BIT1   ///< C1 enabled, supported.
#define PPM_C1E                 BIT2   ///< C1E enabled.
#define PPM_C6                  BIT4   ///< C6 enabled, supported.
#define PPM_C7                  BIT5   ///< C7 enabled, supported.
#define PPM_C7S                 BIT6   ///< C7S enabled, supported
#define PPM_TM                  BIT7   ///< Adaptive Thermal Monitor.
#define PPM_TURBO               BIT8   ///< Long duration turbo mode
#define PPM_CMP                 BIT9   ///< CMP.
#define PPM_TSTATES             BIT10  ///< CPU throttling states
#define PPM_MWAIT_EXT           BIT11  ///< MONITIOR/MWAIT Extensions supported.
#define PPM_EEPST               BIT12  ///< Energy efficient P-State Feature enabled
#define PPM_TSTATE_FINE_GRAINED BIT13  ///< Fine grained CPU Throttling states
#define PPM_CD                  BIT14  ///< Deep Cstate - C8/C9/C10
#define PPM_TIMED_MWAIT         BIT15  ///< Timed Mwait support
#define C6_LONG_LATENCY_ENABLE  BIT16  ///< 1=C6 Long and Short,0=C6 Short only
#define C7_LONG_LATENCY_ENABLE  BIT17  ///< 1=C7 Long and Short,0=C7 Short only
#define C7s_LONG_LATENCY_ENABLE BIT18  ///< 1=C7s Long and Short,0=C7s Short only
#define PPM_C8                  BIT19  ///< 1= C8 enabled/supported
#define PPM_C9                  BIT20  ///< 1= C9 enabled/supported
#define PPM_C10                 BIT21  ///< 1= C10 enabled/supported
#define PPM_HWP                 BIT22  ///< 1= HWP enabled/supported
#define PPM_OC_UNLOCKED         BIT24  ///< 1= Overclocking fully unlocked
#define PPM_TURBO_BOOST_MAX     BIT25  ///< 1= Intel Turbo Boost Max Technology 3.0

#define PPM_C_STATES            0x72    ///< PPM_C1 + PPM_C6 + PPM_C7 + PPM_C7S

//
// C-State Latency (us):
//  Though _CST object is core level, it is recommended that System BIOS report the
//  Package level C state latencies, so that Operating System can account for worst case
//  latency before using a specific C state. Unlike legacy platforms,
//  BIOS is no longer required to program the C_STATE_LATENCYCONTROL_n MSRs (0x60B, 0x60C, 0x633, 0x634, 0x635).
//
#define C1_LATENCY                        1
#define C6_LATENCY                        127
#define C7_LATENCY                        253
#define C8_LATENCY                        260
#define C9_LATENCY                        487
#define C10_LATENCY                       1048

//
// C-State Power (mW)
//
#define C1_POWER                          0x3E8
#define C3_POWER                          0x1F4
#define C6_POWER                          0x15E
#define C7_POWER                          0xC8
#define C8_POWER                          0xC8
#define C9_POWER                          0xC8
#define C10_POWER                         0xC8

#define MAX_POWER_LIMIT_1_TIME_IN_SECONDS 32767
#ifndef AUTO
#define AUTO                              0
#endif
#define END_OF_TABLE                      0xFF

#define CONFIG_TDP_NOMINAL                0
#define CONFIG_TDP_LEVEL1                 1
#define CONFIG_TDP_LEVEL2                 2
#define CONFIG_TDP_DEACTIVATE             0xFF
//
// MMIO definitions
//
#define MMIO_DDR_RAPL_LIMIT                        0x58E0
#define MMIO_TURBO_POWER_LIMIT                     0x59A0
#define MMIO_TURBO_POWER_LIMIT_HIGH                0x59A4
#define MMIO_ISYS_CONTROL_0_0_0_MCHBAR             0x5E90
#define MMIO_PLATFORM_TEMP_CONTROL_0_0_0_0_MCHBAR  0x5B20
#define MMIO_PLATFORM_TEMP_CONTROL_1_0_0_0_MCHBAR  0x5B28
#define MMIO_PLATFORM_TEMP_CONTROL_2_0_0_0_MCHBAR  0x5B30

#define ISYS_CURRENT_L1_TAU_MASK                   0x7F0000;
#define ISYS_CURRENT_LIMIT_L2_MASK                 0x7FFF00000000;

# define SKIN_TEMP_CONTROL_SENSOR                  3

#define MAX_OVERCLOCKING_BINS                      0x7

///
/// For Mobile and Halo, default PL1 time window value is 28 seconds. For 125 Watt Sku it is 56 seconds.
///
#define MB_POWER_LIMIT1_TIME_DEFAULT   28
#define MB_POWER_LIMIT1_TIME_15_WATT   42
///
/// For Desktop, default PL1 time window value is 28 second
///
#define DT_POWER_LIMIT1_TIME_DEFAULT   28
#define DT_POWER_LIMIT1_TIME_125_WATT  56

#define PROCESSOR_FLAVOR_MOBILE       0x04
#define PROCESSOR_FLAVOR_DESKTOP      0x00
#define PROCESSOR_FLAVOR_MASK         (BIT3 | BIT2)

//
// Power definitions (Based on EMTS V1.0 for standard voltage 2.4-2.6 GHz dual-core parts.)
//
#define FVID_MAX_POWER      35000
#define FVID_TURBO_POWER    35000
#define FVID_SUPERLFM_POWER 12000
//
// Power definitions for LFM and Turbo mode TBD.
//
#define FVID_MIN_POWER  15000

///
/// Limit the number of P-states for different generation OS.
///
#define TPSS_FVID_MAX_STATES                   40
#define LPSS_FVID_MAX_STATES                   16

#define READ_PL1_DUTY_CYCLE_CLAMP_ENABLE  0x00000015
#define WRITE_PL1_DUTY_CYCLE_CLAMP_ENABLE 0x00000016

#define SVID_GET_REGISTER           0x80001018
#define SVID_SET_REGISTER           0x80001118
#define SVID_SET_EXCLUSIVE          0x80001218
#define SVID_READ_REG_ALLOWEDLIST   0x80001518
#define SVID_WRITE_REG_ALLOWEDLIST  0x80001618


#define SVID_REGISTER_VR_ID_OFFSET      16
#define SVID_REGISTER_VR_ID_VIRTUAL_0   4
#define SVID_REGISTER_VR_ID_VIRTUAL_1   5
#define SVID_REGISTER_ADDR_OFFSET       20
#define SVID_ALLOWEDLIST_DWORD_OFFSET   20

#define SVID_EXCLUSIVE_DISABLE  0
#define SVID_EXCLUSIVE_ENABLE   1

#define CPU_TDP_5_WATTS        500
#define CPU_TDP_7_WATTS        700
#define CPU_TDP_8_WATTS        800
#define CPU_TDP_9_WATTS        900
#define CPU_TDP_15_WATTS       1500
#define CPU_TDP_17_WATTS       1700
#define CPU_TDP_12_WATTS       1250
#define CPU_TDP_10_WATTS       1000
#define CPU_TDP_25_WATTS       2500
#define CPU_TDP_28_WATTS       2800
#define CPU_TDP_35_WATTS       3500
#define CPU_TDP_45_WATTS       4500
#define CPU_TDP_55_WATTS       5500
#define CPU_TDP_58_WATTS       5800
#define CPU_TDP_62_WATTS       6200
#define CPU_TDP_65_WATTS       6500
#define CPU_TDP_80_WATTS       8000
#define CPU_TDP_91_WATTS       9100
#define CPU_TDP_95_WATTS       9500
#define CPU_TDP_120_WATTS      12000
#define CPU_TDP_125_WATTS      12500
#define CPU_TDP_150_WATTS      15000

#endif
