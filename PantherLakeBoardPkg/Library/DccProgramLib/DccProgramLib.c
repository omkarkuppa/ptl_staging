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

#include <Uefi.h>
#include <Ppi/Smbus2.h>
#include <Library/PmcLib.h>
#include <Library/SmbusLib.h>
#include <Library/DebugLib.h>
#include <Library/DccProgramLib.h>
#include <Library/PcdLib.h>
#include <PlatformBoardId.h>

//
// If Next Divider Frequency is equal to Current, divider won't be changed.
//
#define IS_DIVIDER_NEED_TO_CHANGE(int1, int2, frac1, frac2)         ((((int1) != (int2)) || ((frac1) != (frac2)))? TRUE : FALSE)

//
// If Next Divider Frequency is greater than Current, divider frequence is increasing, otherwise, it is decreasing.
//
#define IS_DIVIDER_INCREASING(nextint, curint, nextfrac, curfrac)     (((nextint) == (curint))? (((nextfrac) > (curfrac))? TRUE : FALSE) \
                                                                                                : (((nextint) > (curint))? TRUE : FALSE))
#define GET_QUOTIENT(a, b)                            (UINT8) ((((a) > 0) && ((b) > 0))? (a / b) : 0)
#define GET_REMAINDER(a, b)                           (UINT32) ((((a) > 0) && ((b) > 0))? (a % b) : 0)

/**
  Set register page address.

  @param[in]  Page              Page address of the register.

  @retval Status

**/
EFI_STATUS
DccRc260xSetPage (
  IN UINT8 Page
  )
{
  EFI_STATUS                  Status;
  EFI_SMBUS_DEVICE_ADDRESS    SmbusDeviceAddr;

  SmbusDeviceAddr.SmbusDeviceAddress = RENESAS_260X_ADDRESS;

  SmBusWriteDataByte (
    SMBUS_LIB_ADDRESS (SmbusDeviceAddr.SmbusDeviceAddress, RENESAS_260X_1B_MODE_SET_PAGE_CMD, 0, FALSE),
    Page,
    &Status
    );

  return Status;
}

/**
  Read data from RC260X register.

  @param[in]  Register          Register address

  @retval Data                  Data read from RC260X register.

**/
UINT8
DccRegisterValueRead (
  IN UINT8 Register
  )
{
  EFI_STATUS                  Status;
  EFI_SMBUS_DEVICE_ADDRESS    SmbusDeviceAddr;
  UINT8                       Data;

  SmbusDeviceAddr.SmbusDeviceAddress = RENESAS_260X_ADDRESS;

  Data = SmBusReadDataByte (
           SMBUS_LIB_ADDRESS (SmbusDeviceAddr.SmbusDeviceAddress, Register, 0, FALSE),
           &Status
           );

  return Data;
}

/**
  Write data to RC260X register.

  @param[in]  Register          Register address
  @param[in]  Data              Data that will be set.

**/
VOID
DccRegisterValueWrite (
  IN UINT8 Register,
  IN UINT8 Data
  )
{
  EFI_STATUS                  Status;
  EFI_SMBUS_DEVICE_ADDRESS    SmbusDeviceAddr;

  SmbusDeviceAddr.SmbusDeviceAddress = RENESAS_260X_ADDRESS;

  SmBusWriteDataByte (
    SMBUS_LIB_ADDRESS (SmbusDeviceAddr.SmbusDeviceAddress, Register, 0, FALSE),
    Data,
    &Status
    );

  return;
}

/**
  Set divide value to divider register.

  @param[in]  FodSelect         Choose Fractional Output Divider that will be set.
  @param[in]  Integer           The integer parts of divider register.
  @param[in]  Fractional        The fractional parts of divider register.

**/
VOID
SetDivider (
  IN UINT8                 FodSelect,
  IN REGISTER_INTEGER      Integer,
  IN REGISTER_FRACTIONAL   Fractional
  )
{
  UINT8                    Fod_Frac_Bit_Clear;

  Fod_Frac_Bit_Clear = 0;

  DEBUG ((DEBUG_INFO, "Set Divider Start\n"));

  switch (FodSelect) {
    case CPU_BCLK_OC_FOD:
      DEBUG ((DEBUG_INFO, "Set FOD0 for CPU\n"));
      DccRc260xSetPage (RENESAS_260X_PAGE_REG_1);
      DccRegisterValueWrite (FOD0_INT_CNFG_0, Integer.Parts.Byte0);                         //Bit[7:0]
      DccRegisterValueWrite (FOD0_INT_CNFG_1, (Integer.Parts.Byte1 & FOD_INT_CNFG_1_MASK)); //Bit[8:8]

      Fod_Frac_Bit_Clear = DccRegisterValueRead (FOD0_INT_CNFG_3);
      DccRegisterValueWrite (FOD0_INT_CNFG_3, Fod_Frac_Bit_Clear & FOD_INT_CNFG_3_MASK);    //Bit[31:26] must be 0
      DccRegisterValueWrite (FOD0_INT_CNFG_4, Fractional.Parts.Byte0);                      //Bit[39:32]
      DccRegisterValueWrite (FOD0_INT_CNFG_5, Fractional.Parts.Byte1);                      //Bit[47:40]
      DccRegisterValueWrite (FOD0_INT_CNFG_6, Fractional.Parts.Byte2);                      //Bit[55:48]
      DccRegisterValueWrite (FOD0_INT_CNFG_7, (Fractional.Parts.Byte3 & FOD_INT_CNFG_7_MASK));  //Bit[59:56] this byte should be written for latest

      break;
    case PEG_DMI_CLK_FOD:
      DEBUG ((DEBUG_INFO, "Set FOD1 for PEG\n"));
      DccRc260xSetPage (RENESAS_260X_PAGE_REG_1);
      DccRegisterValueWrite (FOD1_INT_CNFG_0, Integer.Parts.Byte0);                         //Bit[7:0]
      DccRegisterValueWrite (FOD1_INT_CNFG_1, (Integer.Parts.Byte1 & FOD_INT_CNFG_1_MASK)); //Bit[8:8]

      Fod_Frac_Bit_Clear = DccRegisterValueRead (FOD1_INT_CNFG_3);
      DccRegisterValueWrite (FOD1_INT_CNFG_3, Fod_Frac_Bit_Clear & FOD_INT_CNFG_3_MASK);    //Bit[31:26] must be 0
      DccRegisterValueWrite (FOD1_INT_CNFG_4, Fractional.Parts.Byte0);                      //Bit[39:32]
      DccRegisterValueWrite (FOD1_INT_CNFG_5, Fractional.Parts.Byte1);                      //Bit[47:40]
      DccRegisterValueWrite (FOD1_INT_CNFG_6, Fractional.Parts.Byte2);                      //Bit[55:48]
      DccRegisterValueWrite (FOD1_INT_CNFG_7, (Fractional.Parts.Byte3 & FOD_INT_CNFG_7_MASK)); //Bit[59:56] this byte should be written for latest
      break;
  }
  DEBUG ((DEBUG_INFO, "Set Divider End\n"));
  return;
}

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
  )
{
  switch (FodSelect) {
    case CPU_BCLK_OC_FOD:
      DccRc260xSetPage (RENESAS_260X_PAGE_REG_1);
      //
      // FOD0
      // Read FOD_INT
      //
      Integer->Parts.Byte1 = (DccRegisterValueRead (FOD0_INT_CNFG_1) & FOD_INT_CNFG_1_MASK);  //Bit[8:8]
      Integer->Parts.Byte0 = DccRegisterValueRead (FOD0_INT_CNFG_0);                          //Bit[7:0]
      //
      // Read FOD_FRAC
      //
      Fractional->Parts.Byte3 = (DccRegisterValueRead (FOD0_INT_CNFG_7) & FOD_INT_CNFG_7_MASK); //Bit[59:56]
      Fractional->Parts.Byte2 = DccRegisterValueRead (FOD0_INT_CNFG_6);                         //Bit[55:48]
      Fractional->Parts.Byte1 = DccRegisterValueRead (FOD0_INT_CNFG_5);                         //Bit[47:40]
      Fractional->Parts.Byte0 = DccRegisterValueRead (FOD0_INT_CNFG_4);                         //Bit[39:32]
      break;
    case PEG_DMI_CLK_FOD:
      DccRc260xSetPage (RENESAS_260X_PAGE_REG_1);
      //
      // FOD1
      // Read FOD_INT
      //
      Integer->Parts.Byte1 = (DccRegisterValueRead (FOD1_INT_CNFG_1) & FOD_INT_CNFG_1_MASK);    //Bit[8:8]
      Integer->Parts.Byte0 = DccRegisterValueRead (FOD1_INT_CNFG_0);                            //Bit[7:0]
      //
      // Read FOD_FRAC
      //
      Fractional->Parts.Byte3 = (DccRegisterValueRead (FOD1_INT_CNFG_7) & FOD_INT_CNFG_7_MASK); //Bit[59:56]
      Fractional->Parts.Byte2 = DccRegisterValueRead (FOD1_INT_CNFG_6);                         //Bit[55:48]
      Fractional->Parts.Byte1 = DccRegisterValueRead (FOD1_INT_CNFG_5);                         //Bit[47:40]
      Fractional->Parts.Byte0 = DccRegisterValueRead (FOD1_INT_CNFG_4);                         //Bit[39:32]
      break;
  }
  return;
}

/**
  Calculate the divider value that will be set.
  DividerFreq = ChipFreq / TargetFreq.

  @param[in]       TargetFreq        Frequency of the target device that will be set.
  @param[in, out]  Integer           The integer parts of divider register.
  @param[in, out]  Fractional        The fractional parts of divider register.

**/
VOID
GetNextDividerSettings (
  IN UINT16                TargetFreq,
  IN REGISTER_INTEGER      *Integer,
  IN REGISTER_FRACTIONAL   *Fractional
  )
{
  UINT32                   Remainder;

  Integer->Uint16 = GET_QUOTIENT (RENESAS_260X_CHIP_FREQUENCY, TargetFreq);
  Remainder = GET_REMAINDER (RENESAS_260X_CHIP_FREQUENCY, TargetFreq);

  if (Remainder > 0) {
    //
    // Fractional method:
    // Example: 100/110 = 0.909090909.... equals to 0.e8ba2e8 (in hex)
    // 1. (100 *  16) / 110 = 0xe [27:24],  remainder is (100 *  16) % 110 = 60
    // 2. (60  * 256) / 110 = 0x8b [23:16], remainder is (60  * 256) % 110 = 70
    // 3. (70  * 256) / 110 = 0xa2 [15:8],  remainder is (70  * 256) % 110 = 100
    // 4. (100 * 256) / 110 = 0xe8 [7:0],   remainder is (100 * 256) % 110 = 80
    // 5. 0xe8ba2e8 in [27:0] = 0x0e8ba2e8 in [31:0].
    //    Because the RC260X chip only prepare 28 bits for fractional, [31:28] is reserved.
    //
    Fractional->Parts.Byte3 = GET_QUOTIENT ((Remainder * 16), TargetFreq);
    Remainder = GET_REMAINDER ((Remainder * 16 ), TargetFreq);

    Fractional->Parts.Byte2 = GET_QUOTIENT ((Remainder * 256), TargetFreq);
    Remainder = GET_REMAINDER ((Remainder * 256 ), TargetFreq);

    Fractional->Parts.Byte1 = GET_QUOTIENT ((Remainder * 256), TargetFreq);
    Remainder = GET_REMAINDER ((Remainder * 256), TargetFreq);

    Fractional->Parts.Byte0 = GET_QUOTIENT ((Remainder * 256), TargetFreq);

  }
}

/**
  Program chip to adjust clock frequencey for target device.

  @param[in]  FodSelect         Choose Fractional Output Divider which will be modified.
  @param[in]  TargetFreq        Frequency of the target device that will be set.

**/
VOID
ProgramBclkFreq (
  IN UINT8              FodSelect,
  IN UINT16             TargetFreq
  )
{
  REGISTER_INTEGER      NextInteger;
  REGISTER_INTEGER      CurInteger;
  REGISTER_FRACTIONAL   NextFractional;
  REGISTER_FRACTIONAL   CurFractional;

  DEBUG ((DEBUG_INFO, "Program DCC CLK Start\n"));

  NextInteger.Uint16 = 0;
  NextFractional.Uint32 = 0;
  CurInteger.Uint16 = 0;
  CurFractional.Uint32 = 0;

  GetNextDividerSettings (TargetFreq, &NextInteger, &NextFractional);
  GetCurrentDividerSettings (FodSelect, &CurInteger, &CurFractional);

  DEBUG ((DEBUG_INFO, "BCLK NextInteger.Uint16 = 0x%02X\n", NextInteger.Uint16));
  DEBUG ((DEBUG_INFO, "BCLK NextFractional.Uint32 = 0x%08X\n", NextFractional.Uint32));
  DEBUG ((DEBUG_INFO, "BCLK CurInteger.Uint16 = 0x%02X\n", CurInteger.Uint16));
  DEBUG ((DEBUG_INFO, "BCLK CurFractional.Uint32 = 0x%08X\n", CurFractional.Uint32));

  //
  // If the next divider frequency is not equal to current frequency, divider need to be modified.
  //
  if (IS_DIVIDER_NEED_TO_CHANGE (NextInteger.Uint16, CurInteger.Uint16, NextFractional.Uint32, CurFractional.Uint32)) {
    DEBUG ((DEBUG_INFO, "(DCC) Divider need to be changed\n"));
    //
    // Program FOD Registers
    //
    SetDivider (FodSelect, NextInteger, NextFractional);

  }

  DEBUG ((DEBUG_INFO, "Program DCC CLK End\n"));
}

/**
  Program Dcc chip for Cpu Bclk

  @param[in]  TargetFreq        Frequency of the target device that will be set.

**/
VOID
ProgramCpuBclkFreq (
  IN UINT16   TargetFreq
  )
{
  DEBUG ((DEBUG_INFO, "Program CPU Bclk Start\n"));
  ProgramBclkFreq (CPU_BCLK_OC_FOD, TargetFreq);
  DEBUG ((DEBUG_INFO, "Program CPU Bclk End\n"));
}


/**
  Program Dcc chip for Peg/Dmi Clk

  @param[in]  TargetFreq        Frequency of the target device that will be set.

**/
VOID
ProgramPegDmiClkFreq (
  IN UINT16   TargetFreq
  )
{
  DEBUG ((DEBUG_INFO, "Program Peg/Dmi Clk Start\n"));
  ProgramBclkFreq (PEG_DMI_CLK_FOD, TargetFreq);
  DEBUG ((DEBUG_INFO, "Program Peg/Dmi Clk End\n"));
}

/**
  Check if Renesas RC260X exist or not.

**/
BOOLEAN
CheckDccClkChipExist (
  VOID
  )
{
  //
  // If set page success, it means RC260X chip exists.
  //
  return (DccRc260xSetPage (RENESAS_260X_PAGE_REG_2) == EFI_SUCCESS)? TRUE : FALSE;
}

/**
  Disable Output Clock on all Out[X]/Out[X]b

**/
VOID
DisableExternalClockOutput (
  VOID
  )
{
  REGISTER_ODRV_EN    OutPutState;
  UINT8               Output;
  UINT8               OutDrvEn;

  DccRc260xSetPage (RENESAS_260X_PAGE_REG_2);

  for (Output = 0; Output <= 11; Output++) {
    OutDrvEn = 0x40 + (4 * Output);
    OutPutState.Uint8 = DccRegisterValueRead (OutDrvEn);
    DEBUG ((DEBUG_INFO, "OUT[%d] = 0x%02X\n", Output, OutPutState.Uint8));

    if (OutPutState.Bits.OutDis == 1) {
      DEBUG ((DEBUG_INFO, "OUT[%d] is already Disabled\n", Output));
      continue;
    }

    OutPutState.Uint8 |= RENESAS_260X_OUTPUT_DISABLE;
    DccRegisterValueWrite (OutDrvEn, OutPutState.Uint8);

    OutPutState.Uint8 = DccRegisterValueRead (OutDrvEn);
    DEBUG ((DEBUG_INFO, "OUT[%d] after set as Disabled = 0x%02X\n", Output, OutPutState.Uint8));
  }
}

/**
  Enable Output Clock on all Out[X]/Out[X]b

**/
VOID
EnableExternalClockOutput (
  VOID
  )
{
  REGISTER_ODRV_EN    OutPutState;
  UINT8               Output;
  UINT8               OutDrvEn;

  DccRc260xSetPage (RENESAS_260X_PAGE_REG_2);

  for (Output = 0; Output <= 11; Output++) {
    OutDrvEn = 0x40 + (4 * Output);
    OutPutState.Uint8 = DccRegisterValueRead (OutDrvEn);
    DEBUG ((DEBUG_INFO, "OUT[%d] = 0x%02X\n", Output, OutPutState.Uint8));

    if (OutPutState.Bits.OutDis == 0) {
      DEBUG ((DEBUG_INFO, "OUT[%d] is already Enabled\n", Output));
      continue;
    }

    OutPutState.Uint8 &= ~RENESAS_260X_OUTPUT_DISABLE;
    DccRegisterValueWrite (OutDrvEn, OutPutState.Uint8);

    OutPutState.Uint8 = DccRegisterValueRead (OutDrvEn);
    DEBUG ((DEBUG_INFO, "OUT[%d] after set as Enabled = 0x%02X\n", Output, OutPutState.Uint8));
  }
}

/**
  Detect clock source from Integrated CPU BCLK or discrete BCLK

  @reval  TRUE               BCLK from discrete clock
  @reval  FALSE              BCLK from integrated clock
**/
BOOLEAN
ExternalClockSupport (
  VOID
)
{
  if (CheckDccClkChipExist () == FALSE) {
    DEBUG ((DEBUG_INFO, "Discrete Clock Controller doesn't exist ... \n"));
    return FALSE;
  }

  if (PmcDetectIntegratedClockSource () == TRUE) {
    DEBUG ((DEBUG_INFO, "CPU BCLK Source is from integrated clock\n"));
    DisableExternalClockOutput ();
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "Discrete Clock Supported\n"));
  return TRUE;
}
