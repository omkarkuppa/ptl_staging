/** @file
  Header file for ASF functionality

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _DXE_ASF_HECI_LIB_H_
#define _DXE_ASF_HECI_LIB_H_

#include <AsfMsgs.h>

/**
  Clears boot options by sending a proper ASF command through HECI

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
ClearBootOptions (
  VOID
  );

/**
  Get boot options by sending a proper ASF command through HECI

  @param[out] AsfBootOptions       Boot Options returned from HECI

  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER    BootOptionsResponse is a NULL pointer
**/
EFI_STATUS
GetBootOptions (
  OUT ASF_BOOT_OPTIONS                 *AsfBootOptions
  );

/**
  Send ASF_ReportBiosStatus message

  @param[in] BiosStatus           Current bios status

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
ReportBiosStatus (
  IN UINT32                            BiosStatus
  );
#endif
