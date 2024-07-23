/** @file
  Serial IO Private Uart Lib implementation MeteorLake specific.
  All functions in this library are available in PEI,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <GpioV2Functionalities.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/SerialIoUartSocLib.h>
#include <Ppi/SiPolicy.h>
#include <Register/SerialIoPciConfigControlRegs.h>
#include <Register/PchRegs.h>

/**
  Initialize the Uart controller

  @param[in, out] UartHandle       The SerialIo UART Handle instance
  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in]      SerialIoConfig   SerialIo Config Block
**/
VOID
SerialIoUartSocInit (
  IN OUT SERIAL_IO_UART_HANDLE   *UartHandle,
  IN     SI_POLICY_PPI           *SiPolicy,
  IN     SERIAL_IO_CONFIG        *SerialIoConfig
  )
{

}
