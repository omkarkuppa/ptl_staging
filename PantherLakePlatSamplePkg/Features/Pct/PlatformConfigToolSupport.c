/** @file
  Source code for Platform Configuration Tool in Pre/Post Mem.

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

@par Specification Reference:
**/

#include "PlatformConfigToolInit.h"
#include "PlatformConfigToolStructures.h"
#include <Library/EcMiscLib.h>
#include <Include/PlatformBoardId.h>
#include <Library/UefiLib.h>

CHAR8 UnknownStr[] = "Unknown";


/**
  OutData

  Output debug bytes to serial port.

  @param    DataPointer          Address of byte buffer.

  @param    Count                Number of bytes.

**/

VOID
OutData (
  UINT8     *DataPointer,
  UINT16    Count
  )
{
  UINT16    Index;

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "PCT_DBG: Debug out, count = %d", Count));

  for (Index = 0; Index < Count; Index++) {
    if ((Index % 16) == 0) {
      DEBUG ((DEBUG_INFO, "\n"));
    }
    DEBUG ((DEBUG_INFO, "%02x ", DataPointer[Index]));
  }

  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG_CODE_END ();

  return;
}

CHAR8*
ReturnGpioConfigString (
  UINTN  GpioConfigType,
  UINTN  GpioConfigTypeData
  )
{
  UINTN  Index;
  CHAR8  *ReturnString;

  Index = 0;
  ReturnString = UnknownStr;

  while ((Index < DebugOutputGpioCount[GpioConfigType]) &&
          (GpioConfiguration[GpioConfigType][Index].GpioPinData != GpioConfigTypeData)) Index++;

  if (Index < DebugOutputGpioCount[GpioConfigType]) {
    ReturnString = GpioConfiguration[GpioConfigType][Index].GpioPinDataString;
  }

  return ReturnString;
}

/**
  OutGpios

  Output GPIO configuration to serial in raw format.

  @param    GpioPointer          Address of byte buffer.

  @param    GpioCount            Number of bytes.

**/
VOID
OutGpios (
  UINT8   *GpioPointer,
  UINTN   GpioCount
  )
{
  UINTN   Index;

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "\nGPIO set, Count = %d\n", GpioCount));
  DEBUG ((DEBUG_INFO, "Index   Pad    , PM, H, Dr, O, Icg, Pc, Ecg, L, Ost\n"));

  for (Index = 0; Index < GpioCount; Index++, GpioPointer += sizeof (GPIOV2_INIT_CONFIG)) {
    DEBUG ((DEBUG_INFO, "%04d,  %8x, %2x, %1x, %2x, %1x, %3x, %2x, %3x, %1x, %3x\n",
    Index,
    ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioPad,
    ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.PadMode,
    ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.HostOwn,
    ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.Direction,
    ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.OutputState,
    ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.InterruptConfig,
    ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.ResetConfig,
    ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.TerminationConfig,
    ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.LockConfig,
    ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.OtherSettings
    ));
  }

  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG_CODE_END ();

  return;
}

/**
  OutGpiosVerbose

  Output GPIO configuration to serial formatted
  with GPIO Config field strings.

  @param    GpioPointer          Address of byte buffer.

  @param    GpioCount            Number of bytes.

**/
VOID
OutGpiosVerbose (
  UINT8   *GpioPointer,
  UINTN   GpioCount
  )
{
  UINTN   Index;

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "\nGPIO set, Count = %d, Addr = %p\n", GpioCount, GpioPointer));
  DEBUG ((DEBUG_INFO, "Index,  {Group, Pin, {Pad Mode, Ownership, Direction, Output State, Interrupt Source, "));
  DEBUG ((DEBUG_INFO, "Interrupt Type, Reset Config, Termination, Lock Config, Lock Output, Other}}\n\n"));
  for (Index = 0; Index < GpioCount; Index++, GpioPointer += sizeof (GPIOV2_INIT_CONFIG)) {
    DEBUG ((DEBUG_INFO, "%04d, {%a%a, %02d, {GpioPadMode%a, GpioHostOwn%a, GpioDir%a, GpioOut%a, GpioInt%a, ",
      Index,
      ReturnGpioConfigString (GpioConfigTypeChipsetId, (((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioPad) >> 24),
      ReturnGpioConfigString (GpioConfigTypeGroup, (((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioPad) >> 16),
      (((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioPad) & 0xFF,
      ReturnGpioConfigString (GpioConfigTypePadMode, ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.PadMode),
      ReturnGpioConfigString (GpioConfigTypeOwner, ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.HostOwn),
      ReturnGpioConfigString (GpioConfigTypeDirection, ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.Direction),
      ReturnGpioConfigString (GpioConfigTypeOutput, ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.OutputState),
      ReturnGpioConfigString (GpioConfigTypeInterrupt,
        ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.InterruptConfig)
      ));
    DEBUG ((DEBUG_INFO, "GpioInt%a, GpioTerm%a, Gpio%aReset, Gpio%aLock, Gpio%aLock, RwRaw1%a}}\n",
      ReturnGpioConfigString (GpioConfigTypeInterrupt,
        ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.InterruptConfig),
      ReturnGpioConfigString (GpioConfigTypeElectrical,
        ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.TerminationConfig),
      ReturnGpioConfigString (GpioConfigTypePower, ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.ResetConfig),
      ReturnGpioConfigString (GpioConfigTypeLock,
        ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.LockConfig),
      ReturnGpioConfigString (GpioConfigTypeLock,
        ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.LockConfig),
      ReturnGpioConfigString (GpioConfigTypeOther,
        ((GPIOV2_INIT_CONFIG *)GpioPointer)->GpioConfig.OtherSettings)
      ));
  }

  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG_CODE_END ();

  return;
}


