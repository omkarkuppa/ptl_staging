/** @file
  Header file of USBC Capsule Debug Protocol Library functions

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <Library/Usb4DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>

/**
  Write Log data to the next available Log entry in Log buffer
  Protocol DebugLevel controls what level of Log data can be written to Log buffer

  @param[in] LogLevel  - Log level associated with Log data
  @param[in] EventCode - Event code of Log data
  @param[in] EvtArg0   - Argument 0 of Log data
  @param[in] EvtArg1   - Argument 1 of Log data

  @retval EFI_SUCCESS           - Write Log data to Log buffer successfully
  @retval EFI_UNSUPPORTED       - Unable to support log write
  @retval EFI_OUT_OF_RESOURCES  - Log buffer is full

**/
EFI_STATUS
EFIAPI
CapsuleLogWrite (
  IN UINT8                      LogLevel,
  IN UINT16                     EventCode,
  IN UINT32                     EvtArg0,
  IN UINT32                     EvtArg1
  )
{
  EFI_STATUS               Status;
  EFI_USB4_DEBUG_PROTOCOL  *Usb4DebugProtocol;

  Status = gBS->LocateProtocol (&gEfiUsbCCapsuleDebugProtocolGuid, NULL, (VOID**) &Usb4DebugProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate gEfiUsbCCapsuleDebugProtocolGuid (%r).\n", Status));
    return Status;
  }

  return Usb4DebugProtocol->Usb4LogWrite (Usb4DebugProtocol, LogLevel, EventCode, EvtArg0, EvtArg1);
}