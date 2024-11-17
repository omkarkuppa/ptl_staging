/** @file
  Google Test mocks for UsbcCapsuleDebugLib

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

#ifndef MOCK_USBC_CAPSULE_DEBUG_LIB_H
#define MOCK_USBC_CAPSULE_DEBUG_LIB_H

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>

extern "C" {
#include <Uefi.h>
#include <Usb4Cm/Usb4Debug/Usb4DebugProtocol.h>
#include <Usb4Cm/Usb4Debug/Usb4LogEvents.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>
#include <Library/UsbcCapsuleDebugLib.h>
}

struct MockUsbcCapsuleDebugLib {
  MOCK_INTERFACE_DECLARATION (MockUsbcCapsuleDebugLib);

  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    CapsuleLogWrite,
    (IN   UINT8                    LogLevel,
     IN   UINT16                   EventCode,
     IN   UINT32                   EvtArg0,
     IN   UINT32                   EvtArg1)
    );
};

#endif
