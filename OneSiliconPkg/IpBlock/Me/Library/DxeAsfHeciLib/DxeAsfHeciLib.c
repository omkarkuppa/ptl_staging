/** @file
  This is a library for Asf Heci Message functionality.

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
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PostCodeLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/HeciControlProtocol.h>
#include <AsfMsgs.h>

//
// ME Client - ASF
//

//
// ASF Configuration Command
//

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
  )
{
  EFI_STATUS                           Status;
  UINT32                               Length;
  UINT32                               MeMode;
  CLEAR_BOOT_OPTIONS                   ClearBootOptions;

  DEBUG ((DEBUG_INFO, "Clearing boot options\n"));

  ClearBootOptions.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  ClearBootOptions.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (CLEAR_BOOT_OPTIONS);
  ClearBootOptions.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_CLEAR_BOOT_OPT;
  ClearBootOptions.AsfHeader.Fields.VersionNumber = ASF_VERSION;
  Length                                          = sizeof (ClearBootOptions);

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  Status = HeciWrapperSend (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &ClearBootOptions,
             Length
             );

  DEBUG ((DEBUG_INFO, "Clear boot options status : %r\n", Status));

  return Status;
}

/**
  Get boot options by sending a proper ASF command through HECI

  @param[out] AsfBootOptions      Boot Options returned from HECI

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER   BootOptionsResponse is a NULL pointer
**/
EFI_STATUS
GetBootOptions (
  OUT ASF_BOOT_OPTIONS                 *AsfBootOptions
  )
{
  EFI_STATUS                           Status;
  UINT32                               RecvLength;
  UINT32                               MeMode;
  GET_BOOT_OPTIONS_BUFFER              GetBootOptions;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (AsfBootOptions == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&GetBootOptions, sizeof (GET_BOOT_OPTIONS_BUFFER));
  GetBootOptions.Request.AsfHeader.Data                 = 0;
  GetBootOptions.Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  GetBootOptions.Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (GET_BOOT_OPTIONS);
  GetBootOptions.Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_RETURN_BOOT_OPT;
  GetBootOptions.Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;
  RecvLength                                            = sizeof (GET_BOOT_OPTIONS_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &GetBootOptions.Request,
             sizeof (GET_BOOT_OPTIONS),
             (UINT32 *) &GetBootOptions.Response,
             &RecvLength
             );
  if (!EFI_ERROR (Status)) {
    CopyMem (AsfBootOptions, &GetBootOptions.Response.AsfBootOptions, sizeof (ASF_BOOT_OPTIONS));
  }

  return Status;
}


/**
  Send ASF_ReportBiosStatus message

  @param[in] BiosStatus           Current Bios status

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
ReportBiosStatus (
  IN UINT32                            BiosStatus
  )
{
  EFI_STATUS                           Status;
  REPORT_BIOS_STATUS                   ReportBiosStatus;
  UINT32                               MeMode;

  DEBUG ((DEBUG_INFO, "%a() enter, BiosStatus = %x\n", __FUNCTION__, BiosStatus));

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ReportBiosStatus.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  ReportBiosStatus.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (REPORT_BIOS_STATUS);
  ReportBiosStatus.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_REPORT_BIOS_STATUS;
  ReportBiosStatus.AsfHeader.Fields.VersionNumber = ASF_VERSION;
  ReportBiosStatus.AsfBiosStatus.Data             = BiosStatus;

  Status = HeciWrapperSend (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &ReportBiosStatus,
             sizeof (REPORT_BIOS_STATUS)
             );

  return Status;
}