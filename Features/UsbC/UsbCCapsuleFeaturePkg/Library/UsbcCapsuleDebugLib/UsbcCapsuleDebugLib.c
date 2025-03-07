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
#include <Library/CapsuleDebugLib.h>
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
  IN UINT8   LogLevel,
  IN UINT32  EventCode,
  IN UINT32  EvtArg0,
  IN UINT32  EvtArg1
  )
{
  EFI_STATUS                       Status;
  USBC_CAPSULE_DEBUG_PROTOCOL      *CapsuleDebugProtocol;
  UINT8                            CapsuleReleaseEnable;
  CHAR8                            LogStr[200];
  UINT32                           EvtId;
  UINT32                           Argc;
  const CAPSULE_LOG_MAPPING_ENTRY  *Mappings;

  CapsuleReleaseEnable = PcdGet8 (PcdUsbCCapsuleDebugLevel);

  Status = gBS->LocateProtocol (&gUsbCCapsuleDebugProtocolGuid, NULL, (VOID**) &CapsuleDebugProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate gUsbCCapsuleDebugProtocolGuid (%r).\n", Status));
    return Status;
  }

  ///
  /// Make sure Event ID is same with Log Mapping Entry number
  ///
  EvtId = ((EventCode) >> 2);
  if (EvtId >= CapsuleDebugProtocol->LogMappingEntries) {
    DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Invalid Event ID 0x%x for Log Mapping table\n", EvtId));
    Status = EFI_UNSUPPORTED;
    return Status;
  }

  Mappings = CapsuleDebugProtocol->LogMappingTable;
  ///
  /// Make sure Event code of Log mapping entry is same with Log data Event code
  ///
  if (Mappings[EvtId].EvtCode != EventCode) {
    DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Event ID and Event code mismatch in Log mapping table\n"));
    Status = EFI_UNSUPPORTED;
    return Status;
  }

  if (CapsuleReleaseEnable != USBC_CAPSULE_DBG_DISABLED && (PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_DEBUG_PRINT_ENABLED) == 0) {
    ///
    /// Record debug messages when the BIOS is release build
    ///
    CapsuleDebugProtocol->LogWrite (CapsuleDebugProtocol, LogLevel, EventCode, EvtArg0, EvtArg1);
  } else if ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_DEBUG_PRINT_ENABLED) != 0) {
    ///
    /// Get the argument count of event from lower two bits of Event code
    ///
    Argc = EventCode & 0x3;

    switch (Argc) {
      case 0:
        if (LogDataToStr (LogStr, sizeof (LogStr), Mappings[EvtId].LogStr) == 0) {
          Status = EFI_UNSUPPORTED;
        }
        break;
      case 1:
        if (LogDataToStr (LogStr, sizeof (LogStr), Mappings[EvtId].LogStr, EvtArg0) == 0) {
          Status = EFI_UNSUPPORTED;
        }
        break;
      case 2:
        if (LogDataToStr (LogStr, sizeof (LogStr), Mappings[EvtId].LogStr, EvtArg0, EvtArg1) == 0) {
          Status = EFI_UNSUPPORTED;
        }
        break;
      default:
        DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Unsupported argument number %d\n", Argc));
        Status = EFI_UNSUPPORTED;
        break;
    }

    switch (LogLevel) {
      case USBC_CAPSULE_DBG_VERBOSE:
        DEBUG ((DEBUG_VERBOSE, LogStr));
        break;
      case USBC_CAPSULE_DBG_INFO:
        DEBUG ((DEBUG_INFO, LogStr));
        break;
      case USBC_CAPSULE_DBG_WARN:
        DEBUG ((DEBUG_WARN, LogStr));
        break;
      case USBC_CAPSULE_DBG_ERROR:
        DEBUG ((DEBUG_ERROR, LogStr));
        break;
      default:
        return EFI_INVALID_PARAMETER;
    }
  }

  return EFI_SUCCESS;
}