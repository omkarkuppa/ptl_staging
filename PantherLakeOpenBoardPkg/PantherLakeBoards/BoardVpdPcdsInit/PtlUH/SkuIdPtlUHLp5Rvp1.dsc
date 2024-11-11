## @file
#  Board description file initializes configuration (PCD) settings for the project.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
# @par Specification
##
[Defines]
  PIN_GPIO_ACTIVE_HIGH            = 1
  PIN_GPIO_ACTIVE_LOW             = 0
  BOARD_REAL_BATTERY_SUPPORTED    = 1
  BOARD_VIRTUAL_BATTERY_SUPPORTED = 2
  BOARD_NO_BATTERY_SUPPORT        = 0

[Packages]
  PantherLakeOpenBoardPkg/OpenBoardPkg.dec

#####################################################################
#  PTL UH LP5x T3 - RVP1
#     Board ID: 0x01 (BoardIdPtlUHLp5Rvp1)
#   Display ID: 0x00
#       SKU ID: 0x00000001 (SkuIdPtlUHLp5Rvp1)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlUHLp5Rvp1]

  # MRC UserBd
  gBoardModuleTokenSpaceGuid.VpdPcdSaMiscUserBd|*|0x05

  # Mrc Spd Data Size
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdDataSize|*|512

  # Mrc LP5 CCC Configuration. 0 for Ascending and 0xFF for Descending
  gBoardModuleTokenSpaceGuid.VpdPcdMrcLp5CccConfig|*|0xFF

  # Mrc Cmd Mirror Configuration
  gBoardModuleTokenSpaceGuid.VpdPcdMrcCmdMirror|*|0x0

  # Mrc Spd Address Table
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdAddressTable| * |{CODE(
    { 16,
      { 0xA8 ,   // Port 0
        0x00 ,   // Port 1
        0xA8 ,   // Port 2
        0x00 ,   // Port 3
        0xA8 ,   // Port 4
        0x00 ,   // Port 5
        0xA8 ,   // Port 6
        0x00 ,   // Port 7
        0xA8 ,   // Port 8
        0x00 ,   // Port 9
        0xA8 ,   // Port 10
        0x00 ,   // Port 11
        0xA8 ,   // Port 12
        0x00 ,   // Port 13
        0xA8 ,   // Port 14
        0x00     // Port 15
      }
    })}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqsMapCpu2Dram| * |{CODE({
      //
      // PTL-UH LPDDR5x T3 DQS byte swizzling between CPU and DRAM
      //
      // Ch 0     1         2         3
      {{ 1, 0 }, { 0, 1 }, { 0, 1 }, { 0, 1 },  // Controller 0
       { 0, 1 }, { 0, 1 }, { 1, 0 }, { 0, 1 }}  // Controller 1
      })}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdData|*|{CODE(
      {
        // LPDDR5x 315b 16Gb die, DDP 1Rx16, 4GB size per package
        // Hynix H58G56BK7BX068
        // 7500, ??-??-??-??
        // 4 Banks, 4 bank groups, 16Gb SDRAM density
        // 16 Row bits, 10 Column bits
        // Non-Monolithic DRAM Device, 2 dies, 2 Channels per package
         1,
         {0x23,                                 ///< 0   384 SPD bytes used, 512 total
          0x10,                                 ///< 1   SPD Revision 1.0
          0x15,                                 ///< 2   DRAM Type: LPDDR5x SDRAM
          0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
          0x86,                                 ///< 4   4 Bank Groups, 4 Banks, 16 Gb SDRAM density
          0x21,                                 ///< 5   16 Rows, 10 Columns
          0x95,                                 ///< 6   Non-Monolithic DRAM Device, 2 die, 2 Channels per package, Signal Loading Matrix 1
          0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
          0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
          0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
          0x00,                                 ///< 10  Reserved
          0x00,                                 ///< 11  Module Nominal Voltage: Reserved
          0x02,                                 ///< 12  Module Organization: 1 Rank, x16 Device Width per Channel
          0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
          0x00,                                 ///< 14  Module Thermal Sensor: none
          0x00,                                 ///< 15  Extended Module Type: Reserved
          0x48,                                 ///< 16  Signal Loading: Data/Strobe/Mask: 2 loads, CAC: 2 loads, CS: 1 load
          0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
          0x09,                                 ///< 18  tCKAVGmin =  1.067 ns (LPDDR5X-7500 4:1)
          0xFF,                                 ///< 19  tCKAVGmax = 32.002 ns
          0x92,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
          0x55,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20, 16
          0x05,                                 ///< 22  CAS Latencies supported (Third Byte) : 36, 32
          0x00,                                 ///< 23  CAS Latencies supported (Fourth Byte):
          0xAA,                                 ///< 24  Minimum CAS Latency (tAAmin) = 21.25 ns
          0x00,                                 ///< 25  Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
          0x90,                                 ///< 26  Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
          0xA8,                                 ///< 27  Row precharge time for all banks (tRPab) = 21 ns
          0x90,                                 ///< 28  Minimum row precharge time (tRPpb) = 18 ns
          0xC0,                                 ///< 29  tRFCab = 280 ns (16 Gb)
          0x08,                                 ///< 30  tRFCab MSB
          0x60,                                 ///< 31  tRFCpb = 140 ns (16 Gb)
          0x04,                                 ///< 32  tRFCpb MSB
          0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
          0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
          0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
          0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
          0x00,                                 ///< 123 FTB for tAAmin = 21.25 ns
          0x7F,                                 ///< 124 FTB for tCKAVGmax = 32.002 ns
          0xC6,                                 ///< 125 FTB for tCKAVGmin = -58ps
          0x00,                                 ///< 126 CRC A
          0x00,                                 ///< 127 CRC B
          0, 0,                                 ///< 128 - 129
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
          0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
          0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
          0x00,                                 ///< 322 Module Manufacturing Location
          0x00,                                 ///< 323 Module Manufacturing Date Year
          0x00,                                 ///< 324 Module Manufacturing Date Week
          0x20,                                 ///< 325 Module ID: Module Serial Number
          0x00,                                 ///< 326 Module Serial Number B
          0x00,                                 ///< 327 Module Serial Number C
          0x00,                                 ///< 328 Module Serial Number D
          0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
          0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
          0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
          0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
          0x00,                                 ///< 349 Module Revision Code
          0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
          0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
          0x00,                                 ///< 352 DRAM Stepping
          0, 0, 0, 0, 0, 0, 0,                  ///< 353 - 359
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 360 - 369
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 370 - 379
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 380 - 389
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 390 - 399
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 400 - 409
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 410 - 419
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 420 - 429
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 430 - 439
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 440 - 449
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 450 - 459
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 460 - 469
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 470 - 479
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 480 - 489
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 490 - 499
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 500 - 509
          0, 0                                  ///< 510 - 511
      }})}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqMapCpu2Dram|*|{CODE({{
      //
      // PTL-UH Lp5 DQ bit swizzling between CPU and DRAM
      //

      //Controller 0
      {{  13,  14,  12, 15, 11, 10, 8,  9  },   // Byte 0
       {  7,   5,   4,  6,  0,  3,  1,  2  }},  // Byte 1
      {{  1,   3,   0,  2,  7,  4,  6,  5  },   // Byte 2
       {  12,  13,  14, 15, 11, 10, 9,  8  }},  // Byte 3
      {{  0,   2,   1,  3,  6,  4,  7,  5  },   // Byte 4
       {  14,  13,  15, 12, 8,  11, 10, 9  }},  // Byte 5
      {{  6,   5,   7,  4,  2,  3,  1,  0  },   // Byte 6
       {  10,  8,   11, 9,  12, 15, 13, 14 }},  // Byte 7

      //Controller 1
      {{  2,   1,   3,  0,  4,  7,  5,  6  },   // Byte 0
       {  15,  14,  12, 13, 9,  11, 10, 8  }},  // Byte 1
      {{  6,   5,   7,  4,  3,  1,  0,  2  },   // Byte 2
       {  10,  9,   11, 8,  13, 14, 12, 15 }},  // Byte 3
      {{  9,   10,  11, 8,  14, 12, 13, 15 },   // Byte 4
       {  0,   1,   2,  3,  5,  7,  4,  6  }},  // Byte 5
      {{  0,   1,   2,  3,  7,  5,  6,  4  },   // Byte 6
       {  14,  13,  15, 12, 10, 8,  11, 9  }},  // Byte 7
  }})}


###
### !!! GPIOs designated to Native Functions shall not be configured by Platform Code.
### Native Pins shall be configured by Silicon Code (based on BIOS policies setting) or soft straps(set by CSME in FITc).
###
###
gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTable|*|{CODE(
  {
  //
  // x1 Slot
  //
    {GPIOV2_PTL_PCD_XXGPP_A_8,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // X1_PCIE_SLOT_PWR_EN
    {GPIOV2_PTL_PCD_XXGPP_D_19,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // X1_DT_PCIE_RST_N
    {GPIOV2_PTL_PCD_XXGPP_B_25,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock}},  // X1_SLOT_WAKE_N

  //
  // x8 Slot \ PEG
  //
    {GPIOV2_PTL_PCD_XXGPP_E_19,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_DGPU_SEL_N
    {GPIOV2_PTL_PCD_XXGPP_F_10,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_PWR_EN_N
    {GPIOV2_PTL_PCD_XXGPP_D_9,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_RST_N
    {GPIOV2_PTL_PCD_XXGPP_D_24,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault, GpioV2Unlock,  GpioV2Lock}}, // PEG_SLOT_WAKE_N
    {GPIOV2_PTL_PCD_XXGPP_E_20,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirIn,    GpioV2StateDefault,  GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_DGPU_PWR_OK

  //
  // M.2 SSD Gen4
  //
    {GPIOV2_PTL_PCD_XXGPP_B_10,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,     GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}}, //GEN4_SSD_PWREN
    {GPIOV2_PTL_PCD_XXGPP_B_9,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // M2_GEN4_SSD_RESET_N

  //
  // M.2 SSD Gen5
  //
    {GPIOV2_PTL_PCD_XXGPP_B_16,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}}, // GEN5_SSD_PWREN
    {GPIOV2_PTL_PCD_XXGPP_E_3,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // M2_GEN5_SSD_RESET_N

  //
  // M.2 WLAN
  //
    {GPIOV2_PTL_PCD_XXGPP_F_6,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirIn,     GpioV2StateDefault, GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // WLAN_WWAN_COEX3
    {GPIOV2_PTL_PCD_XXGPP_A_12,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock}}, // WIFI_WAKE_N
    {GPIOV2_PTL_PCD_XXGPP_A_16,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHostDeep,  GpioV2TermDefault}}, // BT_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_17,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHostDeep,  GpioV2TermDefault}}, // WIFI_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_11,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // WLAN_RST_N

  //
  // Camera Conn 1
  //
    {GPIOV2_PTL_PCD_XXGPP_C_5,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // CRD1_PWREN
    {GPIOV2_PTL_PCD_XXGPP_H_1,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // CRD_CAM_STROBE
    {GPIOV2_PTL_PCD_XXGPP_C_15,  {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // CRD1_CLK_EN
    {GPIOV2_PTL_PCD_XXGPP_E_6,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirIn,     GpioV2StateDefault, GpioV2IntLevel,  GpioV2ResetHost,    GpioV2TermDefault}},  // SECURE_CAM_SW
    {GPIOV2_PTL_PCD_XXGPP_F_20,  {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // GPP_PRIVACY_LED_CAM1_CVS_HST_WAKE
    {GPIOV2_PTL_PCD_XXGPP_E_10,  {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // CRD1_RST_N

  //
  // Camera Conn 2
  //
    {GPIOV2_PTL_PCD_XXGPP_C_8,  {GpioV2PadModeGpio,   GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // CRD2_PWREN
    {GPIOV2_PTL_PCD_XXGPP_E_1,  {GpioV2PadModeGpio,   GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateHigh,   GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // CRD2_RST_N
    {GPIOV2_PTL_PCD_XXGPP_F_19, {GpioV2PadModeGpio,   GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // GPP_PRIVACY_LED_CAM2

  //
  // TPM
  //
    { GPIOV2_PTL_PCD_XXGPP_D_15, {GpioV2PadModeGpio, GpioV2HostOwnGpio,    GpioV2DirInInv,    GpioV2StateDefault,  GpioV2IntEdge|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermNone,  GpioV2TermDefault}},  // SPI_TPM_INT_N

  //
  // Touch panel-1
  //
    {GPIOV2_PTL_PCD_XXGPP_F_8,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,     GpioV2StateHigh,     GpioV2IntDis,     GpioV2ResetHost,     GpioV2TermDefault}},  // TCH_PNL1_PWR_EN

  //
  // Touch pad
  //
    {GPIOV2_PTL_PCD_XXGPP_F_18,  {GpioV2PadModeGpio,  GpioV2HostOwnGpio,  GpioV2DirInInv,   GpioV2StateDefault, GpioV2IntEdge|GpioV2IntApic, GpioV2ResetHost,     GpioV2TermNone,  GpioV2Unlock,  GpioV2Lock}},  // TCH_PAD_INT_N

  //
  // TCSS
  //
    {GPIOV2_PTL_PCD_XXGPP_V_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetResume,  GpioV2TermNone}}, // GPP_V17_TCP_RT_S0IX_ENTRY_EXIT_N

  //
  // Type-C , TBT Re-Timers
  //
    {GPIOV2_PTL_PCD_XXGPP_B_21, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,  GpioV2StateLow,    GpioV2IntDis,  GpioV2ResetHostDeep,  GpioV2TermDefault}},  // TCP_RETIMER_FORCE_PWR

  //
  // Lid Switch Wake Gpio
  //
    {GPIOV2_PTL_PCD_XXGPP_F_23, {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirIn,  GpioV2StateDefault,  GpioV2IntDis, GpioV2ResetHostDeep,  GpioV2TermDefault}}, // SMC_LID

  //
  // Only for PNP
  //
    {GPIOV2_PTL_PCD_XXGPP_H_2,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateHigh,     GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // DEBUG_TRACE_PNP

  //
  // Not Used pins set to High Impedance
  //
    {GPIOV2_PTL_PCD_XXGPP_A_13, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_2,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_2,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_15, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_0,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_V_7,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC

    { 0x0 } // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTablePreMem|*|{CODE({
    {0x0}  // terminator
  })}

  # PCIE CLOCK USGAGE for PTL H
  gBoardModuleTokenSpaceGuid.VpdPcdPcieClkUsageMap|*|{CODE(
    {{
      PCIE_PCH + 10,  // X8 CEM Slot
      PCIE_PCH + 8,   // Gen5 SSD / x4 PCIe slot
      PCIE_PCH + 2,   // x1 Pcie Slot
      LAN_CLOCK,      // GBE LAN
      PCIE_PCH + 3,   // M.2 WLAN
      PCIE_PCH + 1,   // M.2 WWAN
      PCIE_PCH + 4,   // Gen4 M.2 SSD
      NOT_USED,
      NOT_USED
    }}
    )}

  gBoardModuleTokenSpaceGuid.VpdPcdPPcieClkUsageMap| * |{CODE(
    {0x0}
  )}