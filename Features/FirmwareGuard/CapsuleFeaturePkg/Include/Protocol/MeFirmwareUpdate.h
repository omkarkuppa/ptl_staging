/** @file
  ME Firmware Update Protocol.

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

#ifndef __ME_FIRMWARE_UPDATE_PROTOCOL_H__
#define __ME_FIRMWARE_UPDATE_PROTOCOL_H__

#include <Uefi.h>

#define ME_FIRMWARE_UPDATE_PROTOCOL_GUID \
  { \
    0xF72BF4C2, 0x92C9, 0x4A49, { 0xA1, 0x11, 0x2D, 0x23, 0x66, 0xD0, 0xAD, 0x02 } \
  }

typedef struct _ME_FIRMWARE_UPDATE_PROTOCOL ME_FIRMWARE_UPDATE_PROTOCOL;

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

  @retval  0       If Update started successfully.
  @retval  Others  Error code otherwise.

**/
typedef
UINT32
(EFIAPI *ME_FIRMWARE_UPDATE_PROTOCOL_FULL_UPDATE_FROM_BUFFER)(
  IN UINT8     *Buffer,
  IN UINT32    BufferLength,
  IN EFI_GUID  *OemId,
  IN VOID      (*Func)(UINT32, UINT32)
  );

/**
  Check for Update progress.

  If in progress, return the current percent.
  If finished, return the status of the update, and the needed reset type
  after the update.
  This function should be called only after starting the update by calling
  FwuUpdateFull/Partial...

  @param[out]  InProgress       True if Update is still in progress.
                                False if Update finished. Caller allocated.
  @param[out]  CurrentPercent   Current percent of the update,
                                if Update is in progress. Caller allocated.
  @param[out]  FwUpdateStatus   FW error code status of the update,
                                if it finished (success or error code).
                                Caller allocated.
  @param[out]  NeededResetType  Needed reset type after the update,
                                if it finished. Caller allocated.
                                MFT_PART_INFO_EXT_UPDATE_ACTION_NONE         0
                                MFT_PART_INFO_EXT_UPDATE_ACTION_HOST_RESET   1
                                MFT_PART_INFO_EXT_UPDATE_ACTION_CSE_RESET    2
                                MFT_PART_INFO_EXT_UPDATE_ACTION_GLOBAL_RESET 3

  @retval  0       If Update is still in progress, or finished successfully.
  @retval  Others  Error code otherwise.

**/
typedef
UINT32
(EFIAPI *ME_FIRMWARE_UPDATE_PROTOCOL_CHECK_UPDATE_PROGRESS)(
  OUT BOOLEAN  *InProgress,
  OUT UINT32   *CurrentPercent,
  OUT UINT32   *FwUpdateStatus,
  OUT UINT32   *NeededResetType
  );

/**
  Get FW Update enabling state.

  @param[out]  EnabledState  FW Update enabling state. Caller allocated.
                             FW_UPDATE_DISABLED = 0.
                               - Full Disabled.
                               - Partial Enabled.
                             FW_UPDATE_ENABLED = 1.
                               - Full Enabled.
                               - Partial Enabled.
                             FW_UPDATE_FULL_AND_PARTIAL_DISABLED = 3.
                               - Full Disabled.
                               - Partial Disabled.

  @retval  0       If succeeded.
  @retval  Others  Error code otherwise.

**/
typedef
UINT32
(EFIAPI *ME_FIRMWARE_UPDATE_PROTOCOL_ENABLED_STATE)(
  OUT UINT16  *EnabledState
  );

/**
  Set FW Update enabling state.

  Supported only before EOP.

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

  @retval  0       If succeeded.
  @retval  Others  Error code otherwise.

**/
typedef
UINT32
(EFIAPI *ME_FIRMWARE_UPDATE_PROTOCOL_SET_ENABLED_STATE)(
  IN UINT32  EnabledState
  );

/**
  Get OEM ID from flash.

  @param[out]  OemId  OEM ID from flash. Caller allocated.

  @retval  0       If succeeded.
  @retval  Others  Error code otherwise.

**/
typedef
UINT32
(EFIAPI *ME_FIRMWARE_UPDATE_PROTOCOL_OEM_ID)(
  OUT EFI_GUID  *OemId
  );

/**
  Get FW Type, from the flash image.

  @param[out]  FwType  FW Type. Caller allocated.
                       FWU_FW_TYPE_INVALID   0
                       FWU_FW_TYPE_RESERVED  1
                       FWU_FW_TYPE_CONSUMER  3
                       FWU_FW_TYPE_CORPORATE 4

  @retval  0       If succeeded.
  @retval  Others  Error code otherwise.

**/
typedef
UINT32
(EFIAPI *ME_FIRMWARE_UPDATE_PROTOCOL_FW_TYPE)(
  OUT UINT32  *FwType
  );

/**
  Set ISH configuration file.

  Receive PDT file payload, create bios2ish file as a composition of
  bios2ish header (with PDT Update data type) and PDT file payload
  and send it to FW to set file.
  Supported only before EOP.

  @param[in]  Buffer        Buffer of PDT file payload.
  @param[in]  BufferLength  Length of the buffer in bytes.

  @retval  0       If succeeded.
  @retval  Others  Error code otherwise.

**/
typedef
UINT32
(EFIAPI *ME_FIRMWARE_UPDATE_PROTOCOL_SET_ISH_CONFIG)(
  IN UINT8   *Buffer,
  IN UINT32  BufferLength
  );

/**
  Get the recovery image from the FW using DMA buffer.

  @param[in]  DmaBuffer        DMA-able buffer for the recovery image.
                               The buffer should be pre-allocated
                               by the caller.
  @param[in]  DmaBufferLength  Length of the buffer in bytes.
  @param[out] ImageLength      Length of the recovery image
                               that was written in the buffer.

  @return  0       If succeeded.
  @retval  Others  Error code otherwise.

**/
typedef
UINT32
(EFIAPI *ME_FIRMWARE_UPDATE_PROTOCOL_GET_RECOVERY_TO_DMA)(
  IN     EFI_PHYSICAL_ADDRESS  DmaBuffer,
  IN     UINT32                DmaBufferLength,
     OUT UINT32                *ImageLength
  );

///
/// FIRMWARE_UPDATE_PROTOCOL
/// The protocol providing the different generation FwUpdateLib implementation for ME capsule update
/// as the following services.
/// - Starting a Full FW Update from a buffer.
/// - Check for Update progress.
/// - Set FW update enabling state.
/// - Get FW Update enabling state.
/// - Get OEM ID from flash.
/// - Get FW type from flash.
/// - Set ISH configuration file.
/// - Get recovery image to DMA buffer.
///
struct _ME_FIRMWARE_UPDATE_PROTOCOL {
  ME_FIRMWARE_UPDATE_PROTOCOL_FULL_UPDATE_FROM_BUFFER    FullUpdateFromBuffer;
  ME_FIRMWARE_UPDATE_PROTOCOL_CHECK_UPDATE_PROGRESS      CheckUpdateProgress;
  ME_FIRMWARE_UPDATE_PROTOCOL_SET_ENABLED_STATE          SetEnabledState;
  ME_FIRMWARE_UPDATE_PROTOCOL_ENABLED_STATE              EnabledState;
  ME_FIRMWARE_UPDATE_PROTOCOL_OEM_ID                     OemId;
  ME_FIRMWARE_UPDATE_PROTOCOL_FW_TYPE                    FwType;
  ME_FIRMWARE_UPDATE_PROTOCOL_SET_ISH_CONFIG             SetIshConfig;
  ME_FIRMWARE_UPDATE_PROTOCOL_GET_RECOVERY_TO_DMA        GetRecoveryImageToDmaBuffer;
};

extern EFI_GUID  gMeFwuProtocol;

#endif
