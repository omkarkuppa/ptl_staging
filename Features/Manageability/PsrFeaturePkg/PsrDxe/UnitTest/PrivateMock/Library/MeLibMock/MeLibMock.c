/** @file
  Mock instance of MeLib library class.

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

@par Specification
**/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <PsrMsgs.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

/**
  Mock HeciSendPsrCapabilitySet: Inform CSME of Psr Capability Set.

  @param[in] CapabilityMask       Bit mask of all capabilities supported by PSR feature.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendPsrCapabilitySet (
  IN UINT32 CapabilityMask
  )
{
  check_expected (CapabilityMask);
  return mock_type (EFI_STATUS);
}

/**
  Mock HeciGetPsrRecord: Get Platform Service Record from CSME

  @param[in, out] PsrData         Pointer to PSR data struct

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetPsrRecord (
  IN OUT PSR_DATA *PsrData
  )
{
  check_expected_ptr (PsrData);

  CopyMem (PsrData, mock_ptr_type (PSR_DATA*), sizeof (PSR_DATA));

  return mock_type (EFI_STATUS);
}

/**
  Mock HeciSendChassisIntrDetEvent: Inform CSME of Chassis Intrusion Detected Event

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendChassisIntrDetEvent (
  VOID
  )
{
  return mock_type (EFI_STATUS);
}

/**
  Mock HeciSendCriticalTempShutdownEvent: Inform CSME of Critical Temperature Shutdown Event

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendCriticalTempShutdownEvent (
  VOID
  )
{
  return mock_type (EFI_STATUS);
}

/**
  Mock HeciEventReport: PSR Report Event

  @param[in] EventId       Different event ID for each event
  @param[in] EventData     Data payload

  @retval EFI_UNSUPPORTED  Current ME mode doesn't support this function
  @retval EFI_SUCCESS      Command succeeded
**/
EFI_STATUS
HeciEventReport (
  IN UINT32 EventId,
  IN UINT32 EventData
  )
{
  check_expected (EventId);
  check_expected (EventData);

  return mock_type (EFI_STATUS);
}
