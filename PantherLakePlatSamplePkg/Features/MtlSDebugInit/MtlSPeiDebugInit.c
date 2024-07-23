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
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SerialPortLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/SerialPortParameterLib.h>
#include <Library/Usb3DebugPortLib.h>
#include <Library/SerialIoUartDebugHelperLib.h>
#include <Library/TraceHubDebugLib.h>

/**
  MtlSPeiDebugInitEntryPoint entry point

  @param[in]  FileHandle        Handle of the file being invoked.
  @param[in]  PeiServices       Pointer to PEI Services table.

  @return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MtlSPeiDebugInitEntryPoint (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
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