/** @file
  This file defines the USB PORT MAP DATA VAR.

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

#ifndef __USB_PORT_MAP_DXE__H__
#define __USB_PORT_MAP_DXE__H__
#ifdef __cplusplus
extern "C"
{
#endif

#include <PiDxe.h>
#include <Protocol/AcpiTable.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Protocol/FirmwareVolume2.h>
#include <IndustryStandard/Acpi.h>
#include <Library/DxeServicesLib.h>
#include <Library/PcdLib.h>
#include <UsbPortMapNvs.h>
#include <DxeITbtConfig.h>

#define USB_PORT_MAP_DATA_VAR         L"UsbPortMap"

#ifdef __cplusplus
}
#endif
#endif
