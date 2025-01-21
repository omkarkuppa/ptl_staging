/** @file
  DXE driver for platform ME FwUpdate implementation.

  Note:
    Any function is not supported by platform, bypass it in here.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
//
// Advanced Feature Package Foundation.
//
#include <Protocol/MeFirmwareUpdate.h>
//
// Platform Package Foundation.
//
#include <Library/FWUpdateLib.h>

/**
  Starting a Full FW Update from a buffer.

  @param[in]  Buffer        Buffer of Update Image read from Update Image file.
  @param[in]  BufferLength  Length of the buffer in bytes.
  @param[in]  OemId         OEM ID to compare with OEM ID in FW (if exist).
                            Can be NULL.
  @param[in]  Func          A callback function that reports the progress of
                            sending the buffer to FW (not the progress of
                            the update itself). Can be NULL.
                            VOID *Func (
                                   UINT32 BytesSentToFw,
                                   UINT32 TotalBytesToSendToFw
                                   );

  @retval  SUCCESS  If Update started successfully.
  @retval  Others   Error code otherwise.

**/
UINT32
EFIAPI
PlatformMeFwuFullUpdateFromBuffer (
  IN UINT8     *Buffer,
  IN UINT32    BufferLength,
  IN EFI_GUID  *OemId,
  IN VOID      (*Func)(UINT32, UINT32)
  )
{
  return FwuFullUpdateFromBuffer (
           Buffer,
           BufferLength,
           (_UUID *)OemId,
           Func
           );
}

/**
  Check for Update progress.

  If in progress, return the current percent.
  If finished, return the status of the update, and the needed reset type
  after the update.
  This function should be called only after starting the update by calling
  FwuUpdateFull/Partial...

  @param[in, out]  InProgress       True if Update is still in progress.
                                    False if Update finished. Caller allocated.
  @param[in, out]  CurrentPercent   Current percent of the update,
                                    if Update is in progress. Caller allocated.
  @param[in, out]  FwUpdateStatus   FW error code status of the update,
                                    if it finished (success or error code).
                                    Caller allocated.
  @param[in, out]  NeededResetType  Needed reset type after the update,
                                    if it finished. Caller allocated.
                                    MFT_PART_INFO_EXT_UPDATE_ACTION_NONE         0
                                    MFT_PART_INFO_EXT_UPDATE_ACTION_HOST_RESET   1
                                    MFT_PART_INFO_EXT_UPDATE_ACTION_CSE_RESET    2
                                    MFT_PART_INFO_EXT_UPDATE_ACTION_GLOBAL_RESET 3

  @retval  SUCCESS  If Update is still in progress, or finished successfully.
  @retval  Others   Error code otherwise.

**/
UINT32
EFIAPI
PlatformMeFwuCheckUpdateProgress (
  IN OUT BOOLEAN  *InProgress,
  IN OUT UINT32   *CurrentPercent,
  IN OUT UINT32   *FwUpdateStatus,
  IN OUT UINT32   *NeededResetType
  )
{
  return FwuCheckUpdateProgress (
           InProgress,
           CurrentPercent,
           FwUpdateStatus,
           NeededResetType
           );
}

/**
  Get FW Update enabling state.

  @param[in, out]  EnabledState  FW Update enabling state. Caller allocated.
                                 FW_UPDATE_DISABLED = 0.
                                   - Full Disabled.
                                   - Partial Enabled.
                                 FW_UPDATE_ENABLED = 1.
                                   - Full Enabled.
                                   - Partial Enabled.
                                 FW_UPDATE_FULL_AND_PARTIAL_DISABLED = 3.
                                   - Full Disabled.
                                   - Partial Disabled.

  @retval  SUCCESS  If succeeded.
  @retval  Others   Error code otherwise.

**/
UINT32
EFIAPI
PlatformMeFwuEnabledState (
  IN OUT UINT16  *EnabledState
  )
{
  return FwuEnabledState (EnabledState);
}

/**
  Set FW Update enabling state. (Supported only before EOP.)

  @param[in]  EnabledState  FW Update enabling state.
                            FW_UPDATE_DISABLED = 0.
                              - Full Disabled.
                              - Partial Enabled.
                            FW_UPDATE_ENABLED = 1.
                              - Full Enabled.
                              - Partial Enabled.
                            FW_UPDATE_FULL_AND_PARTIAL_DISABLED = 3.
                              - Full Disabled.
                              - Partial Disabled.

  @return  SUCCESS  If succeeded.
  @retval  Others   Error code otherwise.

*/
UINT32
EFIAPI
PlatformMeFwuSetEnabledState (
  IN UINT32  EnabledState
  )
{
  return FwuSetEnabledState (EnabledState);
}

/**
  Get OEM ID from flash.

  @param[in, out]  OemId  OEM ID from flash. Caller allocated.

  @retval  SUCCESS  If succeeded.
  @retval  Others   Error code otherwise.

**/
UINT32
EFIAPI
PlatformMeFwuOemId (
  IN OUT EFI_GUID  *OemId
  )
{
  return FwuOemId ((_UUID *)OemId);
}

/**
  Set ISH configuration file. (Supported only before EOP.)

  Receive PDT file payload, create bios2ish file as a composition of
  bios2ish header (with PDT Update data type) and PDT file payload
  and send it to FW to set file.

  @param[in]  Buffer        Buffer of PDT file payload.
  @param[in]  BufferLength  Length of the buffer in bytes.

  @retval  SUCCESS  If succeeded.
  @retval  Others   Error code otherwise.

**/
UINT32
EFIAPI
PlatformMeFwuSetIshConfig (
  IN  UINT8   *Buffer,
  IN  UINT32  BufferLength
  )
{
  return FwuSetIshConfig (Buffer, BufferLength);
}

/**
  Get the recovery image from the FW using DMA buffer.

  @param[in]       DmaBuffer        DMA-able buffer for the recovery image.
                                    The buffer should be pre-allocated
                                    by the caller.
  @param[in]       DmaBufferLength  Length of the buffer in bytes.
  @param[in, out]  ImageLength      Length of the recovery image
                                    that was written in the buffer.

  @retval  SUCCESS  If succeeded.
  @retval  Others   Error code otherwise.

**/
UINT32
EFIAPI
PlatformMeFwuGetRecoveryImageToDmaBuffer (
  IN     EFI_PHYSICAL_ADDRESS  DmaBuffer,
  IN     UINT32                DmaBufferLength,
  IN OUT UINT32                *ImageLength
  )
{
  return FwuGetRecoveryImageToDmaBuffer (DmaBuffer, DmaBufferLength, ImageLength);
}

///
/// Protocol to expose the services.
///
ME_FIRMWARE_UPDATE_PROTOCOL  mFirmwareUpdateContext = {
  PlatformMeFwuFullUpdateFromBuffer,        // ME_FIRMWARE_UPDATE_PROTOCOL_FULL_UPDATE_FROM_BUFFER
  PlatformMeFwuCheckUpdateProgress,         // ME_FIRMWARE_UPDATE_PROTOCOL_CHECK_UPDATE_PROGRESS
  PlatformMeFwuSetEnabledState,             // ME_FIRMWARE_UPDATE_PROTOCOL_SET_ENABLED_STATE
  PlatformMeFwuEnabledState,                // ME_FIRMWARE_UPDATE_PROTOCOL_ENABLED_STATE
  PlatformMeFwuOemId,                       // ME_FIRMWARE_UPDATE_PROTOCOL_OEM_ID
  PlatformMeFwuSetIshConfig,                // ME_FIRMWARE_UPDATE_PROTOCOL_SET_ISH_CONFIG
  PlatformMeFwuGetRecoveryImageToDmaBuffer  // ME_FIRMWARE_UPDATE_PROTOCOL_GET_RECOVERY_TO_DMA
};

/**
  The module entry point of DXE driver.

  @param[in]  ImageHandle  Firmware allocated handle for the EFI image.
  @param[in]  SystemTable  Pointer to the EFI System Table.

  @retval  EFI_SUCCESS          The entry point is executed successfully.
  @retval  EFI_ALREADY_STARTED  Protocol instance had been started.

**/
EFI_STATUS
EFIAPI
MeFwuDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                   Status;
  ME_FIRMWARE_UPDATE_PROTOCOL  *MeFwu;

  //
  // Only allow a single gMeFwuProtocol Protocol instance to be installed.
  //
  Status = gBS->OpenProtocol (
                  ImageHandle,
                  &gMeFwuProtocol,
                  (VOID **)&MeFwu,
                  NULL,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }

  //
  // Install gMeFwuProtocol Protocol
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gMeFwuProtocol,
                  &mFirmwareUpdateContext,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Protocol install error. Status = %r.\n", __func__, Status));
  }

  return EFI_SUCCESS;
}
