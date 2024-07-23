/** @file
  Platform Initialization Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include "PlatformSetup.h"
#include "KeyboardSetup.h"

EFI_STATUS
EFIAPI
Ps2InitHardware (
  IN  EFI_HANDLE  Handle
  )
/*++

Routine Description:

  Perform additional PS/2 hardware init.
  - Send a 0xF6 command to set the kbd in the default state. If the PS/2 keyboard driver takes a short-cut that skips the kbd
    reset command 0xff and the kbd enabling command 0xf4, then the command 0xf6 becomes mandatory to make KBD to work with Emerald Lake
    CRB EC FW. To avoid changes in the generic PS/2 keyboard driver, we send 0xF6 command in this function as a platform specific hook.
  - PS/2 Swap feature is not supported.

Arguments:

  Handle  - Handle for the protocol instance.

Returns:

  EFI_SUCCESS

--*/
{
  EFI_STATUS              Status;
  UINT8                   Buf8;
  UINT64                  Buf64;
  UINT32                  TimeOut;
  UINT32                  SumTimeOut;
  STATIC  BOOLEAN         Initialized;

  //
  // Only do once
  //
  if (!Initialized) {
    Initialized = TRUE;
  } else {
    return EFI_SUCCESS;
  }

  //
  // Based on assumption that output buffer clearing is done by Keyboard driver before calling this function,
  // we don't do output buffer clearing here to save boot time.
  // If the assumption does not hold true, we must clear output buffer here.
  //

  //
  // Wait for input buffer empty
  //
  Status = mPciRootBridgeIo->PollIo(mPciRootBridgeIo,
                                    EfiPciWidthUint8,
                                    KEYBOARD_8042_STATUS_REGISTER,
                                    KEYBOARD_STATUS_REGISTER_HAS_INPUT_DATA,
                                    0,
                                    KEYBOARD_TIMEOUT * 10,
                                    &Buf64
                                    );

  if (EFI_SUCCESS != Status) {
    goto Done;
  }

  //
  // Write the 0xF6 command
  //
  Buf8 = KEYBOARD_8048_COMMAND_SET_DEFAULT;
  Status = mPciRootBridgeIo->Io.Write (mPciRootBridgeIo,
                                     EfiPciWidthUint8,
                                     KEYBOARD_8042_DATA_REGISTER,
                                     1,
                                     &Buf8
                                     );

  //
  // Wait for ACK
  //
  TimeOut = 0;
  SumTimeOut = 0;
  while (1) {
    //
    // Read from data port
    //
    for (TimeOut = 0; TimeOut < KEYBOARD_TIMEOUT; TimeOut += 30) {
      Status = mPciRootBridgeIo->Io.Read(mPciRootBridgeIo,
                                EfiPciWidthUint8,
                                KEYBOARD_8042_STATUS_REGISTER,
                                1,
                                &Buf8
                                );
      ASSERT_EFI_ERROR (Status);
      if (Buf8 & KEYBOARD_STATUS_REGISTER_HAS_OUTPUT_DATA) {
        Status = mPciRootBridgeIo->Io.Read(mPciRootBridgeIo,
                                  EfiPciWidthUint8,
                                  KEYBOARD_8042_DATA_REGISTER,
                                  1,
                                  &Buf8
                                  );
        ASSERT_EFI_ERROR (Status);
        break;
      }
      gBS->Stall (30);
    }

    //
    // Is it an ACK?
    //
    if (TimeOut < KEYBOARD_TIMEOUT && Buf8 == KEYBOARD_8048_RETURN_8042_ACK) {
      Status = EFI_SUCCESS;
      break;
    }

    SumTimeOut += TimeOut;
    if (SumTimeOut >= KEYBOARD_WAITFORVALUE_TIMEOUT) {
      Status = EFI_DEVICE_ERROR;
      break;
    }
  }

Done:
  return  Status;
}
