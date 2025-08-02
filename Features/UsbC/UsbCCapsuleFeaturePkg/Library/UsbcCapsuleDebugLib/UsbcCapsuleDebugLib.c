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
#ifndef MDEPKG_NDEBUG
  CHAR8                            LogStr[200];
  UINT32                           Argc;
  UINT32                           NumOfEntries;
  UINTN                            Arg0;
  UINTN                            Arg1;
#else
  USBC_CAPSULE_DEBUG_PROTOCOL      *CapsuleDebugProtocol;
  UINT8                            CapsuleReleaseEnable;
#endif
  EFI_STATUS                       Status;
  UINT32                           EvtId;
  const CAPSULE_LOG_MAPPING_ENTRY  *MappingTable;

#ifndef MDEPKG_NDEBUG
  ///
  /// Make sure Event ID is same with Log Mapping Entry number
  ///
  NumOfEntries = mUsbCCapsuleLogMappingEntries;
  EvtId = ((EventCode) >> EVT_CODE_EVT_ID_OFFSET);
  if (EvtId >= NumOfEntries) {
    DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Invalid Event ID 0x%x for Log Mapping table\n", EvtId));
    return EFI_UNSUPPORTED;
  }

  if ((EventCode & EVT_CODE_ARG0_STATUS) && (EvtArg0 != EFI_SUCCESS)) {
    Arg0 = ((UINTN) EvtArg0 | MAX_BIT);
  } else {
    Arg0 = (UINTN) EvtArg0;
  }

  if ((EventCode & EVT_CODE_ARG1_STATUS) && (EvtArg1 != EFI_SUCCESS)) {
    Arg1 = ((UINTN) EvtArg1 | MAX_BIT);
  } else {
    Arg1 = (UINTN) EvtArg1;
  }

  MappingTable = mUsbCCapsuleLogMappingTable;
  ///
  /// Get the argument count of event from Bit2-3 of Event code
  ///
  Argc = EVT_CODE_ARG_NUM_MASK (EventCode);

  switch (Argc) {
    case 0:
      Status = (LogDataToStr (LogStr, sizeof (LogStr), MappingTable[EvtId].LogStr) == 0) ? EFI_UNSUPPORTED : EFI_SUCCESS;
      break;
    case 1:
      Status = (LogDataToStr (LogStr, sizeof (LogStr), MappingTable[EvtId].LogStr, Arg0) == 0) ? EFI_UNSUPPORTED : EFI_SUCCESS;
      break;
    case 2:
      Status = (LogDataToStr (LogStr, sizeof (LogStr), MappingTable[EvtId].LogStr, Arg0, Arg1) == 0) ? EFI_UNSUPPORTED : EFI_SUCCESS;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Unsupported argument number %d\n", Argc));
      Status = EFI_UNSUPPORTED;
      break;
  }

  if (!EFI_ERROR (Status)) {
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
  return Status;
#else
  Status = EFI_SUCCESS;
  CapsuleReleaseEnable = PcdGet8 (PcdUsbCCapsuleDebugLevel);

  if (CapsuleReleaseEnable != USBC_CAPSULE_DBG_DISABLED) {
    Status = gBS->LocateProtocol (&gUsbCCapsuleDebugProtocolGuid, NULL, (VOID**) &CapsuleDebugProtocol);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    ///
    /// Make sure Event ID is same with Log Mapping Entry number
    ///
    EvtId = ((EventCode) >> EVT_CODE_EVT_ID_OFFSET);
    if (EvtId >= CapsuleDebugProtocol->LogMappingEntries) {
      DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Invalid Event ID 0x%x for Log Mapping table\n", EvtId));
      Status = EFI_UNSUPPORTED;
      return Status;
    }

    MappingTable = CapsuleDebugProtocol->LogMappingTable;
    ///
    /// Make sure Event code of Log mapping entry is same with Log data Event code
    ///
    if (MappingTable[EvtId].EvtCode != EventCode) {
      DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Event ID and Event code mismatch in Log mapping table\n"));
      Status = EFI_UNSUPPORTED;
      return Status;
    }

    ///
    /// Record debug messages when the BIOS is release build
    ///
    Status = CapsuleDebugProtocol->LogWrite (CapsuleDebugProtocol, LogLevel, EventCode, EvtArg0, EvtArg1);
  }
  return Status;
#endif // MDEPKG_NDEBUG
}