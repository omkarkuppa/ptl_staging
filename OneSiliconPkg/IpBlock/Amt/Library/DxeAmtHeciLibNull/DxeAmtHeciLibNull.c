/** @file
  This is DxeAmtHeciLibNull library.

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
#include <Uefi/UefiBaseType.h>

//
// LAN Command
//

/**
  This message is sent to switch active LAN interface.

  @param[in] ActiveInterface      Active Lan Interface
                                    0: As defined in FIT
                                    1: Integrated LAN
                                    2: Discrete LAN

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
AmtSetActiveLanInterface (
  IN UINT8                        ActiveInterface
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This message is sent to get active LAN interface.

  @param[out] ActiveInterface     Active Lan Interface
                                    0: As defined in FIT
                                    1: Integrated LAN
                                    2: Discrete LAN

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
AmtGetActiveLanInterface (
  OUT UINT8                          *ActiveInterface
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Start ASF Watchdog Timer.
  The WDT will be started only if AMT Watchdog policy is enabled and corresponding timer value is not zero.

  @param[in] WatchDogType         ASF Watchdog Type: 0 - BIOS Watchdog
                                                     1 - OS Watchdog
**/
VOID
AsfStartWatchDog (
  IN  UINT8                       WatchDogType
  )
{
}

/**
  Stop ASF Watchdog Timer HECI message.
**/
VOID
AsfStopWatchDog (
  VOID
  )
{
}