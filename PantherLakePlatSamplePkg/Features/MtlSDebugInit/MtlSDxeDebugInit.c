/** @file

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/SerialPortParameterLib.h>
#include <Library/Usb3DebugPortLib.h>
#include <Library/SerialIoUartDebugHelperLib.h>
#include <Library/TraceHubDebugLib.h>
#include <Library/UefiDriverEntryPoint.h>

/**
  MtlSDxeDebugInitEntryPoint Entry Point

  @param[in]  ImageHandle       Handle of the file being invoked.
  @param[in]  SystemTable       Pointer to system table.

  @return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MtlSDxeDebugInitEntryPoint (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  )
{
  UINT8                     StatusCodeFlags;

  StatusCodeFlags = GetDebugInterface ();

  if (StatusCodeFlags & STATUS_CODE_USE_ISA_SERIAL) {
    SerialPortInitialize ();
  }

  if (StatusCodeFlags & STATUS_CODE_USE_USB3) {
    Usb3DebugPortInitialize ();
  }

  return EFI_SUCCESS;
}