/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef _DCC_PROGRAM_H_
#define _DCC_PROGRAM_H_

#pragma pack(1)

typedef union {
  struct {
    //
    // OUTx and/or OUTxb Driver disable
    // 0x0 = OUTx and/or OUTxb Driver is enabled if not disabled by other means
    // 0x1 = OUTx and/or OUTxb Driver is disabled
    //
    UINT8  OutDis            : 1;

    //
    // OUT Driver disabled state
    // Controls the state of OUTx / OUTxb when the output driver is
    // disabled.
    //
    UINT8  OutDisState       : 2;

    //
    // Output Driver OE Group Select and Global Output Enable Exclusion
    // Sets which OE group this driver is in, and if not assigned to a group,
    // can also exclude global output enables from applying to the clock.
    //
    UINT8  OutDisGroup       : 3;

    //
    // Output Driver OE Mode
    // Controls whether the output enable acts synchronously or
    // asynchronously with respect to the output divider clock. Must be set to
    // asynchronous mode when outputting SYSREF.
    //
    UINT8  OutOeMode         : 1;

    //
    // Output Driver Power Down
    // Powers down the output clock driver.
    // 0x0 = output driver is powered up
    // 0x1 = output driver is powered down
    //
    UINT8  OutPd             : 1;
  } Bits;
  UINT8 Uint8;
} REGISTER_ODRV_EN;

typedef union {
  //
  // The RC260X provide 9 bits to apply integer settings.
  //
  struct {
    UINT8  Byte0             : 8;
    UINT8  Byte1             : 1;
    UINT8  Rsvd              : 7;
  } Parts;
  UINT16  Uint16;
} REGISTER_INTEGER;

typedef union {
  //
  // The RC260X provide 28 bits to apply fraction settings.
  //
  struct {
    UINT8  Byte0             : 8;
    UINT8  Byte1             : 8;
    UINT8  Byte2             : 8;
    UINT8  Byte3             : 4;
    UINT8  Rsvd              : 4;
  } Parts;
  UINT32  Uint32;
} REGISTER_FRACTIONAL;

//
// Structure of CrashLog Hob data
//
typedef struct {
  REGISTER_INTEGER     Fod0IntegerDividerValue;
  REGISTER_FRACTIONAL  Fod0FractionalDividerValue;
  REGISTER_INTEGER     Fod1IntegerDividerValue;
  REGISTER_FRACTIONAL  Fod1FractionalDividerValue;
} DISCRETE_CLOCL_CHIP_HOB;
#pragma pack()

#define CPU_BCLK_OC_FOD                               0
#define PEG_DMI_CLK_FOD                               1
#define RENESAS_260X_1B_MODE_SET_PAGE_CMD             0xFD
#define RENESAS_260X_ADDRESS                          0x09
#define RENESAS_260X_CHIP_FREQUENCY                   10000     // 10Ghz = 10000Mhz
#define RENESAS_260X_OUTPUT_DISABLE                   BIT0

#define RENESAS_260X_PAGE_REG_1                       0x01
#define RENESAS_260X_PAGE_REG_2                       0x02
#define FOD0_INT_CNFG_0                               0xE0      // FOD0: INT[7:0]
#define FOD0_INT_CNFG_1                               0xE1      // FOD0: INT[8:8]
#define FOD0_INT_CNFG_3                               0xE3      // FOD0: FRAC[31:26]
#define FOD0_INT_CNFG_4                               0xE4      // FOD0: FRAC[39:32]
#define FOD0_INT_CNFG_5                               0xE5      // FOD0: FRAC[47:40]
#define FOD0_INT_CNFG_6                               0xE6      // FOD0: FRAC[55:48]
#define FOD0_INT_CNFG_7                               0xE7      // FOD0: FRAC[59:56]
#define FOD1_INT_CNFG_0                               0xF0      // FOD1: INT[7:0]
#define FOD1_INT_CNFG_1                               0xF1      // FOD1: INT[8:8]
#define FOD1_INT_CNFG_3                               0xF3      // FOD1: FRAC[31:26]
#define FOD1_INT_CNFG_4                               0xF4      // FOD1: FRAC[39:32]
#define FOD1_INT_CNFG_5                               0xF5      // FOD1: FRAC[47:40]
#define FOD1_INT_CNFG_6                               0xF6      // FOD1: FRAC[55:48]
#define FOD1_INT_CNFG_7                               0xF7      // FOD1: FRAC[59:56]

#define FOD_INT_CNFG_1_MASK                           0x01
#define FOD_INT_CNFG_3_MASK                           0x03
#define FOD_INT_CNFG_7_MASK                           0x0F

/**
  Get original divider setting from a register.

  @param[in]       FodSelect         Choose Fractional Output Divider that will be read.
  @param[in, out]  Integer           The integer parts of divider register.
  @param[in, out]  Fractional        The fractional parts of divider register.

**/
VOID
GetCurrentDividerSettings (
  IN UINT8                 FodSelect,
  IN REGISTER_INTEGER      *Integer,
  IN REGISTER_FRACTIONAL   *Fractional
  );

/**
  Program Dcc chip for Cpu Bclk

  @param[in]  TargetFreq        Frequency of the target device that will be set.

**/
VOID
ProgramCpuBclkFreq (
  IN UINT16   TargetFreq
  );

/**
  Program Dcc chip for Peg/Dmi Clk

  @param[in]  TargetFreq        Frequency of the target device that will be set.

**/
VOID
ProgramPegDmiClkFreq (
  IN UINT16   TargetFreq
  );

/**
  Detect clock source from Integrated CPU BCLK or discrete BCLK

  @reval  TRUE               BCLK from discrete clock
  @reval  FALSE              BCLK from integrated clock
**/
BOOLEAN
ExternalClockSupport (
  VOID
);
#endif
