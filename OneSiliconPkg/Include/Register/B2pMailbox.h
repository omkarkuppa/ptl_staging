/** @file
  Header file for CPU/PPM/VR Mailbox Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _PM_MAILBOX_H_
#define _PM_MAILBOX_H_

///
/// RFI/EMI register definitions
///
#define R_RFI_EMI_CONTROL_0_0_0_MCHBAR_PCU                                 0x00005A08

///
/// VR mailbox commands definitions
///
#define VR_ADDRESS_MASK                                                    0x0000000F

///
/// PCODE Mailbox Commands
///
#define MAILBOX_VR_CMD_ACOUSTIC_MITIGATION_CONFIG                          0x0A
#define MAILBOX_VR_SUBCMD_GET_HYST_WINDOW_PARAMS                           0x01
#define MAILBOX_VR_SUBCMD_SET_HYST_WINDOW_PARAMS                           0x00
#define MAILBOX_PCODE_CMD_TBT_PERF_BOOST                                   0x00000022
#define MAILBOX_PCODE_CMD_TBT_PERF_BOOST_PARAMS                            0x00000041
///
/// Hysteresis window
///
typedef union {
  UINT32 MailboxData;
  struct {
    UINT32 Window      : 8;
    UINT32 Reserved1   : 24;
  } Fields;
} MAILBOX_DATA_FORMAT_HYSTERESIS;

#define HYSTERESIS_PCORE  0
#define HYSTERESIS_ECORE  1

#define MAILBOX_PCODE_CMD_READ_AUTONOMOUS_PARAMS                           0x00000010
#define MAILBOX_PCODE_CMD_WRITE_AUTONOMOUS_PARAMS                          0x00000011
typedef union {
  struct {
    UINT32  Reserved1                :29;  ///< [28:0] : Reserved
    UINT32  EppGroupingDisable       :1;   ///< [29] : EPP_GROUPING_DISABLE
    UINT32  PcpsDisable              :1;   ///< [30] : PCPS_DISABLE
    UINT32  Reserved2                :1;   ///< [31] : Reserved
  } Fields;
  UINT32    Data32;
} MAILBOX_DATA_AUTONOMOUS_PARAMS;

#define MAILBOX_VR_CMD_READ_VR_TDC_CONFIG                                  0x00000019
#define MAILBOX_VR_CMD_WRITE_VR_TDC_CONFIG                                 0x0000001A
typedef union {
  struct {
    UINT32  CurrentLimit             :15;  ///< [14:0]  : Current Limit
    UINT32  TdcEnable                :1;   ///< [15:15] : TDC Enable
    UINT32  Reserved0                :1;   ///< [16:16] : Reserved 0
    UINT32  TimeWindow               :7;   ///< [23:17] : Time Window
    UINT32  Reserved1                :5;   ///< [28:24] : Reserved 1
    UINT32  IrmsSupported            :1;   ///< [29:29] : Irms Supported
    UINT32  UseIrms                  :1;   ///< [30:30] : Use Irms
    UINT32  Lock                     :1;   ///< [31:31] : Lock
  } Fields;
  UINT32    Data32;
} MAILBOX_DATA_VR_TDC_CONFIG;

///
/// VCCSA Shutdown Configuration
/// Project specific opcode for VCCSA VR shutdown control in PKGC10.3
/// Note: Must be configured before MRC_DONE and requires DFD mailbox coordination
///
#define MAILBOX_VR_CMD_PROJECT_SPECIFIC                                    0x00000001
#define WRITE_VCCSA_SHUTDOWN_ENABLE                                        0x00000004
#define READ_VCCSA_SHUTDOWN_ENABLE                                         0x00000005
typedef union {
  struct {
    UINT32  VccsaShutdownEnable      :1;   ///< [0:0]   : VCCSA Shutdown Enable
    UINT32  Reserved                 :31;  ///< [31:1]  : Reserved
  } Fields;
  UINT32    Data32;
} MAILBOX_DATA_VCCSA_SHUTDOWN_CONFIG;

//
// Cd Clock
//
#define MAILBOX_PCODE_CMD_SET_DISPLAY_FREQUENCY                             0x00000001
#define MAILBOX_SUBCMD_SET_DISPLAY_FREQUENCY_PARAM1                         0x00000007

//
// Get IPC Scaling Factor per module
//
#define MAILBOX_BIOS_CMD_READ_CORE_SCALING_FACTOR                          0x00000021
typedef union {
  struct {
    UINT16    LpId       : 3; // Core ID and Thread ID
    UINT16    ModuleId   : 8; // Module ID
    UINT16    PmDomainId : 5; // Die ID
  } InputFields;
  struct {
    UINT32    ScalingFactor       : 16;   ///< Scaling Factor
    UINT32    NumEquivalentModule : 8;    ///< Returns number of modules with consecutive id that has same scaling factor
    UINT32    Reserved            : 8;    ///< Reserved bit
  } Fields;
  UINT32    Data;
} MAILBOX_DATA_FORMAT_SCALING_FACTOR;

///
/// MAILBOX_BIOS_CMD_MISC_ALG_CONFIG_INTERFACE command set
///
#define MAILBOX_PCODE_CMD_MISC_ALG_CONFIG_INTERFACE                        0x0000002B
#define MAILBOX_PCODE_SUBCMD_READ_HDC_DIS_WHEN_OSREQ_ABOVE_PE_SUBCOMMAND   0x00000000
#define MAILBOX_PCODE_SUBCMD_WRITE_HDC_DIS_WHEN_OSREQ_ABOVE_PE_SUBCOMMAND  0x00000001
#define MAILBOX_PCODE_SUBCMD_READ_EPB_PECI_CONTROL_SUBCOMMAND              0x00000002
#define MAILBOX_PCODE_SUBCMD_WRITE_EPB_PECI_CONTROL_SUBCOMMAND             0x00000003
#define MAILBOX_PCODE_SUBCMD_READ_RING_DISTRESS_DISABLE_SUBCOMMAND         0x00000004
#define MAILBOX_PCODE_SUBCMD_WRITE_RING_DISTRESS_DISABLE_SUBCOMMAND        0x00000005
#define MAILBOX_PCODE_SUBCMD_READ_FORCEPR_DEMOTION_DISABLE_SUBCOMMAND      0x0000000C
#define MAILBOX_PCODE_SUBCMD_WRITE_FORCEPR_DEMOTION_DISABLE_SUBCOMMAND     0x0000000D
#define MAILBOX_PCODE_SUBCMD_READ_VRALERT_DEMOTION_DISABLE_SUBCOMMAND      0x0000000E
#define MAILBOX_PCODE_SUBCMD_WRITE_VRALERT_DEMOTION_DISABLE_SUBCOMMAND     0x0000000F

///
/// Power Floor Management
///
#define MAILBOX_PCODE_CMD_READ_POWER_FLOOR_CONFIG                          0x0000003C
#define MAILBOX_PCODE_CMD_WRITE_POWER_FLOOR_CONFIG                         0x0000003D
typedef union {
  UINT32 Data;
  struct {
    UINT32 PowerFloorManagementDisable          : 1;  ///< [0] Power Floor Management Disable
    UINT32 PowerFloorDisplayDisconnectDisable   : 1;  ///< [1] Power Floor Display Disconnect Disable
    UINT32 Rsvd1                                : 17; ///< [18:2] Reserved
    UINT32 PowerFloorPcieGenXDowngradeDisable   : 1;  ///< [19] Returns number of modules with consecutive id that has same scaling factor
    UINT32 Rsvd2                                : 12; ///< [31:20] Reserved
  } Fields;
} MAILBOX_DATA_POWER_FLOOR_CONFIG;

///
/// BCLK Config
///
#define MAILBOX_PCODE_CMD_BCLK_CONFIG                                      0x0000003F
#define MAILBOX_PCODE_BCLK_CONFIG_SET_SSC_CONTROL_SUBCOMMAND               0x00000004
#define MAILBOX_PCODE_BCLK_CONFIG_GET_SSC_CONTROL_SUBCOMMAND               0x00000003
#define SSC_PLL_SELECT_SOC_BCLK                                            0x00000000
#define SSC_PLL_SELECT_CPU_BCLK                                            0x00000001
typedef union {
  UINT32 Data32;
  struct {
    UINT32 SscEnable                 :1;  ///< [0:0] Enable SSC
    UINT32 Rsvd                      :30; ///< [31:1] Reserved
  } Fields;
} MAILBOX_DATA_SSC_CONTROL;

//
// MAILBOX_BIOS_DFD_POLICY
//
#define MAILBOX_PCODE_CMD_BIOS_WRITE_DFD_POLICY                            0x00000042
#define MAILBOX_PCODE_CMD_BIOS_READ_DFD_POLICY                             0x00000043
#define V_MAILBOX_DFD_EN                                                   0x00000000
#define V_MAILBOX_DFD_DIS                                                  0x00000001

//
// Power limit 4 boost configure
//
#define MAILBOX_BIOS_CMD_PLATFORM_CONFIG                                   0x00000044
#define MAILBOX_BIOS_SUBCMD_READ_PL4_BOOST_POWER                           0x00000000
#define MAILBOX_BIOS_SUBCMD_WRITE_PL4_BOOST_POWER                          0x00000001
typedef union {
  UINT32 Data32;
  struct {
    UINT32 Pl4Boost                  :13; ///< [12:0] Power Floor Management Disable
    UINT32 Rsvd                      :19; ///< [31:13] Reserved
  } Fields;
} MAILBOX_DATA_PL4_BOOST_POWER;

#define MAILBOX_BIOS_SUBCMD_HUB_CORE_INFO                                  0x00000002
typedef union {
  UINT32 Data32;
  struct {
    UINT32 NumCdieLinks              :8;  ///< [7:0]   Number of Compute Complex Links (not number of Cdies)
    UINT32 Rsvd1                     :8;  ///< [15:8]  Reserved
    UINT32 NumLpCore                 :8;  ///< [23:16] Number of LP HUB core count (based on current FUSE count of HUB Atoms)
    UINT32 Rsvd2                     :8;  ///< [31:24] Reserved
  } Fields;
} MAILBOX_DATA_HUB_CORE_INFO;

#define MAILBOX_BIOS_SUBCMD_COMPUTE_COMPLEX_CORE_INFO                      0x00000003
typedef union {
  UINT32 Data32;
  struct {
    UINT32 PerfCoreCount            :8;  ///< [7:0]   Performance Core count for Compute Complex (IF CDIE is enabled, Fused Pcount from Dcode->Pcode info, else 0). This field return fuse enabled P-core count
    UINT32 ECoreCount               :8;  ///< [15:8]  Efficient Core count for Compute Complex (IF CDIE is enabled, Fused Ecount from Dcode->Pcode info, else 0). This field return fuse enabled E-core count
    UINT32 LpCoreCount              :8;  ///< [23:16] LP core count
    UINT32 Rsvd                     :8;  ///< [31:24] Reserved
  } Fields;
} MAILBOX_DATA_COMPUTE_COMPLEX_CORE_INFO;

#define MAILBOX_PCODE_CMD_DLVR_SOC                                         0x00000048
#define MAILBOX_PCODE_DLVR_PHASE_SSC_SET_SUBCOMMAND                        0x00000000
typedef union {
  UINT32 Data32;
  struct {
    UINT32 EnableDlvrSscMode         :1;  ///< [0] Enable DLVR phase SSC Mode
    UINT32 Rsvd                      :31; ///< [31:1] Reserved
  } Fields;
} MAILBOX_DATA_SET_DLVR_SOC;

#define MAILBOX_PCODE_DLVR_PHASE_SSC_GET_SUBCOMMAND                        0x00000001
typedef union {
  UINT32 Data32;
  struct {
    UINT32 PhaseSscSupported         :1;  ///< [0] Phase SSC supported
    UINT32 PhaseSscStatus            :1;  ///< [1] Phase SSC status
    UINT32 Rsvd                      :30; ///< [31:2] Reserved
  } Fields;
} MAILBOX_DATA_GET_DLVR_SOC;
//
// Get of P0,P1 and Pn per logical CCP ID
//
#define MAILBOX_BIOS_CMD_DFX                                               0x00000049
#define MAILBOX_BIOS_SUB_COMMAND_DFX_GET_FREQUENCIES                       0x00000000
#define MAILBOX_BIOS_SUB_COMMAND_DFX_GET_FREQUENCIES_DIE                   0x00000001
typedef union {
  struct {
    UINT16  DieSelection             :4; ///< [3:0]  Die selection
    UINT16  LogicalCcpId             :8; ///< [11:4] Logical CCP ID Selecion
    UINT16  Reserved                 :4; ///< [15:12] Reserved
  } Bits;
  UINT16    Uint16;
} MAILBOX_BIOS_CMD_DFX_PARAM2;
typedef union {
  struct {
    UINT16    LpId       : 3; // Core ID and Thread ID
    UINT16    ModuleId   : 8; // Module ID
    UINT16    PmDomainId : 5; // Die ID
  } InputBits;
  struct {
    UINT32    P0       : 8;
    UINT32    P1       : 8;
    UINT32    Pn       : 8;
    UINT32    CcpType  : 3;
    UINT32    Reserved : 4;
    UINT32    Enabled  : 1;
  } Bits;
  UINT32    Uint32;
} MAILBOX_BIOS_SUB_COMMAND_DFX_GET_FREQUENCIES_DATA;

//
// Mailbox BIOS command for PATH_TO_MEM_AVAILABLE
//
#define MAILBOX_PCODE_CMD_READ_MISC_CONFIG_STATUS                          0x0000004A
typedef union {
  UINT32 Data32;
  struct {
    UINT32 PatchToMemAvailable       :1;  ///< [0] Patch to Memory Available
    UINT32 Rsvd                      :31; ///< [31:1] Reserved
  } Fields;
} MAILBOX_DATA_MISC_CONFIG_STATUS;

//
// Mailbox BIOS command 0x1F to read CPU soft straps value
//
#define MAILBOX_PCODE_CMD_READ_SOFT_STRAPS                                 0x0000001F
#define CPU_SOFTSTRAP_SET1_HIGH                                            0x00000001
#define CPU_SOFTSTRAP_SET2_LOW                                             0x00000002
#define CPU_SOFTSTRAP_SET2_HIGH                                            0x00000003
#define CPU_SOFTSTRAP_SET3_LOW                                             0x00000004
#define CPU_SOFTSTRAP_SET3_HIGH                                            0x00000005
#define V_STRAP_RAW_VALUE                                                  0
#define V_STRAP_RESOLVED_VALUE                                             1

//
// VR mailbox commands
//
#define MAILBOX_VR_CMD_EARLY_VR_DONE                                       0x00000046

//
// MAILBOX_VR_CMD_SVID_VR_HANDLER command set.
//
#define MAILBOX_VR_CMD_SVID_VR_HANDLER                                     0x00000018
#define MAILBOX_VR_SUBCMD_SVID_GET_STRAP_CONFIGURATION                     0x00000000
#define MAILBOX_VR_SUBCMD_SVID_GET_ACDC_LOADLINE                           0x00000001
#define MAILBOX_VR_SUBCMD_SVID_SET_ACDC_LOADLINE                           0x00000002
typedef union {
  struct {
    UINT32 AcLoadLine               :16; ///< [15:0]  AC Load Line
    UINT32 DcLoadLine               :16; ///< [31:16] DC Load Line
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_ACDC_LOADLINE;

#define MAILBOX_VR_SUBCMD_SVID_SET_PS_CUTOFF                               0x00000003
#define MAILBOX_VR_SUBCMD_SVID_GET_PS_CUTOFF                               0x00000013
typedef union {
  struct {
    UINT32 Ps1CutOff                :10; ///< [9:0]   PS1 Max Current
    UINT32 Ps2CutOff                :10; ///< [19:10] PS2 Max Current
    UINT32 Ps3CutOff                :10; ///< [29:20] PS3 Max Current
    UINT32 Ps3Disable               :1;  ///< [30:30] Disable PS3
    UINT32 Ps4Disable               :1;  ///< [31:31] Disable PS4
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_PS_CUTOFF;

#define MAILBOX_VR_SUBCMD_SVID_SET_IMON_CONFIG                             0x00000004
#define MAILBOX_VR_SUBCMD_SVID_GET_IMON_CONFIG                             0x00000014
typedef union {
  struct {
    UINT32 ImonOffset               :16; ///< [15:0]  IMON Offset
    UINT32 ImonSlope                :16; ///< [31:16] IMON Slope
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_IMON_CONFIG;

#define MAILBOX_VR_SUBCMD_SVID_GET_MAX_ICC                                 0x00000005
#define MAILBOX_VR_SUBCMD_SVID_SET_MAX_ICC                                 0x00000006
typedef union {
  struct {
    UINT32 MaxIcc                   :11; ///< [10:0]  Max ICC
    UINT32 Reserved                 :21; ///< [31:11] Reserved
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_ICC_MAX;

#define MAILBOX_VR_SUBCMD_SVID_GET_VOLTAGE_LIMIT                           0x00000007
#define MAILBOX_VR_SUBCMD_SVID_SET_VOLTAGE_LIMIT                           0x00000008
typedef union {
  struct {
    UINT32 VoltageLimit             :16; ///< [15:0] VR Voltage Limit
    UINT32 Reserved                 :16; ///< [31:16] Reserved
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_VOLTAGE_LIMIT;

#define MAILBOX_VR_SUBCMD_SVID_SET_PMON_CONFIG                             0x00000009
#define MAILBOX_VR_SUBCMD_SVID_GET_PMON_CONFIG                             0x00000019

typedef union {
  struct {
    UINT32 PmonOffset               :16; ///< [15:0]  PMON Offset
    UINT32 PmonSlopeCorrection      :16; ///< [31:16] PMON Slope Correction
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_PMON_CONFIG;

#define MAILBOX_VR_SUBCMD_SVID_GET_PMON_PMAX                               0x0000000A
#define MAILBOX_VR_SUBCMD_SVID_SET_PMON_PMAX                               0x0000000B
typedef union {
  struct {
    UINT32 PmonPmax                 :16; ///< [15:0] PMON PMAX
    UINT32 Reserved                 :16; ///< [31:16] Reserved
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_PMON_PMAX;

#define MAILBOX_VR_SUBCMD_SVID_SET_VR_SLEW_RATE                            0x0000000C
#define MAILBOX_VR_SUBCMD_SVID_GET_VR_SLEW_RATE                            0x0000001C
typedef union {
  struct {
    UINT32 SlowSlewRate             :2;  ///< [1:0] Slow SLEW Rate
    UINT32 Reserved                 :30; ///< [31:2] Reserved
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_SLEW_RATE;

#define MAILBOX_VR_SUBCMD_SVID_SET_DISABLE_FAST_PKGC_RAMP                  0x0000000D
typedef union {
  struct {
    UINT32 FastPkgcRampDisable      :1;  ///< [0:0] 0: Fast PKGC Ramp is Enabled (Default), 1: Fast PKGC Ramp is Disabled
    UINT32 Reserved                 :31; ///< [31:21 Reserved
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_FAST_PKGC_RAMP;

#define MAILBOX_VR_SUBCMD_SVID_SET_VR                                      0x00000011
#define MAILBOX_VR_SUBCMD_SVID_SET_PSYS_VR                                 0x0000000D
#define MAILBOX_VR_SUBCMD_SVID_SET_VR_VSYS_MODE                            0x00000034
#define MAILBOX_VR_SUBCMD_SVID_SET_VR_CRIT_THRESHOLD                       0x0000004A
#define MAILBOX_VR_SUBCMD_SVID_SET_VR_CONFIG2                              0x0000004F
#define MAILBOX_VR_SUBCMD_SVID_SET_VR_CONFIG1                              0x00000049

#define MAILBOX_VR_SUBCMD_SVID_GET_FAST_VMODE_ICC_LIMIT                    0x00000024
#define MAILBOX_VR_SUBCMD_SVID_SET_FAST_VMODE_ICC_LIMIT                    0x00000025
#define MAILBOX_VR_SUBCMD_SVID_GET_MISC_CONFIG                             0x00000026
#define MAILBOX_VR_SUBCMD_SVID_SET_MISC_CONFIG                             0x00000027
typedef union {
  struct {
    UINT32 FastVModeIccLimit        :11; ///< [10:0]  Fast VMode ICC Limit
    UINT32 Reserved                 :21; ///< [31:11] Reserved
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_FAST_VMODE_ICC_LIMIT;

#define MAILBOX_VR_SUBCMD_SVID_GET_VSYS_MAX                                0x00000028
#define MAILBOX_VR_SUBCMD_SVID_SET_VSYS_MAX                                0x00000029
typedef union {
  struct {
    UINT32 VsysMax                  :16; ///< [15:0] VSYS Max, Power supply output voltage
    UINT32 Reserved                 :16; ///< [31:16] Reserved
  } Fields;
  UINT32 Data32;
} MAILBOX_DATA_VSYS_MAX;
#endif
