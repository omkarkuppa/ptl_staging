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

#include <Uefi.h>
#include <Library/DccProgramLib.h>

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
  return;
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
  return;
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
  return;
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
  return FALSE;
}