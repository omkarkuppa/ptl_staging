/** @file
  This file provides necessary GPIO functionality for PTL PCD.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef _PTL_PCD_MINIMAL_GPIO_H_
#define _PTL_PCD_MINIMAL_GPIO_H_

#include <GpioV2ControllerInterface.h>
#include <Register/GpioV2PcdPins.h>
#include <GpioV2Signals.h>


typedef struct _GPIOV2_MIN_NATIVE_SIGNAL_DATA {
  GPIOV2_SIGNAL                   Signal;
  GPIOV2_PAD                      Pad;
  GPIOV2_PAD_MODE                 PadMode;
  GPIOV2_IOSTANDBY_STATE          IosState;
  GPIOV2_IOSTANDBY_TERM           IosTerm;
  BOOLEAN                         VwmMap;
  UINT8                           VwIndex;
  UINT8                           BitPosition;
} GPIOV2_MIN_NATIVE_SIGNAL_DATA;

/**
  Get the appropriate GPIO native signal data array based on WCL support.

  @param[out] SignalData          Pointer to the signal data array
  @param[out] SignalDataArraySize Size of the signal data array

  @retval EFI_SUCCESS           Signal data retrieved successfully
  @retval EFI_INVALID_PARAMETER Invalid parameters provided

  **/
EFI_STATUS
GetPcdMinNativeSignalData (
  OUT GPIOV2_MIN_NATIVE_SIGNAL_DATA **SignalData,
  OUT UINT32                         *SignalDataArraySize
  );

#endif // _PTL_PCD_MINIMAL_GPIO_H_
