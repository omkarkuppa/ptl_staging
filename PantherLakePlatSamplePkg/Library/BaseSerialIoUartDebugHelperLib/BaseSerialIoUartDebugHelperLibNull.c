/** @file
  Implementation of BaseSerialIoUartDebugHelperLib.

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

#include <Base.h>

/**
  Initialize SerialIo UART for debug.

**/
VOID
EFIAPI
SerialIoUartDebugInit (
  VOID
  )
{
  return;
}

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]  Buffer           Data pointer
  @param[in]  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval                      Actual number of bytes written to serial device.
**/
UINTN
EFIAPI
SerialIoUartDebugWrite (
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  )
{
  return 0;
}
