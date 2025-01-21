/** @file
 General Header file for PCT initialization.

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

#ifndef _PLATFORM_CONFIG_INIT_H_
#define _PLATFORM_CONFIG_INIT_H_

#include <Uefi.h>
#include "PlatformConfigToolGuids.h"
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BoardConfigLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include "PlatformConfigToolStructures.h"
#include <Include/Pi/PiFirmwareFile.h>
#include <Include/PlatformBoardId.h>

extern  UINTN                       PctConfigFileCount;
extern  UINTN                       PctConfigDataTypeGuidsCount;
extern  CHAR8                       *PctConfigFileNames[];
extern  EFI_GUID                    *PctConfigFileGuids[];
extern  PCT_CONFIG_DATA_TYPE_GUID   PctConfigDataTypeGuids[];

extern  UINTN                       DebugOutputGpioCount[];
extern  GPIO_PIN_STRING             *GpioConfiguration [];

typedef enum {
  GpioConfigTypeChipsetId,
  GpioConfigTypeGroup,
  GpioConfigTypePadMode,
  GpioConfigTypeOwner,
  GpioConfigTypeDirection,
  GpioConfigTypeOutput,
  GpioConfigTypeInterrupt,
  GpioConfigTypePower,
  GpioConfigTypeElectrical,
  GpioConfigTypeLock,
  GpioConfigTypeOther
} DEBUG_GPIO_CONFIG_TYPE;

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
  );

/**
  OutGpios

  Output GPIO configuration to serial in raw format.

  @param    GpioPointer          Address of byte buffer.

  @param    GpioCount            Number of bytes.

**/
VOID
OutGpios (
  UINT8     *GpioPointer,
  UINTN     GpioCount
  );

/**
  OutGpiosVerbose

  Output GPIO configuration to serial formatted
  with GPIO Config field strings.

  @param    GpioPointer          Address of byte buffer.

  @param    GpioCount            Number of bytes.

**/
VOID
OutGpiosVerbose (
  UINT8     *GpioPointer,
  UINTN     GpioCount
  );


#endif // _PLATFORM_CONFIG_INIT_H_
