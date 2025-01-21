/** @file
  Mother Board drive Health implementation head file.

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

#ifndef _DXE_MOTHER_BOARD_H_
#define _DXE_MOTHER_BOARD_H_

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/DevicePath.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/DevicePath.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <TelemetryCommon.h>

///
/// structure Revision (as defined in Telemetry Gen 2.0 spec.)
///
#define MOTHER_BOARD_AIP_HEADER_REVISION  0x01
#define MOTHER_BOARD_AIP_DATA_REVISION    0x01

typedef struct {
  EFI_HANDLE                                  Handle;
  EFI_ADAPTER_INFORMATION_PROTOCOL            AdapterInfo;
  EFI_DEVICE_PATH_PROTOCOL                    *MBHealthDevicePath;
} MOTHER_BOARD_HEALTH_DEV;

#endif // #ifndef _DXE_MOTHER_BOARD_H_

