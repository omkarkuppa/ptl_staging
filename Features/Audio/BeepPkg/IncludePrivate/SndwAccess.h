/** @file
  Sndw Access definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par specification reference:
**/

#ifndef _SNDW_ACCESS_H_
#define _SNDW_ACCESS_H_

#include <SndwMipiCmd.h>

typedef union {
  UINT8       Data[6];
  struct {
    UINT8     UniqueId           : 4;
    UINT8     Version            : 4;
    UINT8     ManufacturerID[2];
    UINT8     PartId[2];
    UINT8     Class;
  } Encoding;
} SNDW_CODEC_ID;

typedef struct {
  UINT32         SndwLinkIndex;
  UINT32         PeripheralIndex;
  SNDW_CODEC_ID  CodecId;
} SNDW_CODEC_INFO;

typedef struct _SNDW_ACCESS SNDW_ACCESS;

/**
  Function operating on Sndw Fifo buffer for sending messages to codecs.

  @param[in]  This                Pointer to Sndw Access Protocol
  @param[in]  SndwCodecInfo       Sndw codec information
  @param[in]  TxCommands          Pointer to send messages
  @param[in]  TxSize              Size of messages to send

  @retval EFI_SUCCESS             Message sent successfully
  @retval EFI_NOT_FOUND           Given codec not found
**/
typedef
EFI_STATUS
(EFIAPI *SNDW_SEND) (
  IN  CONST SNDW_ACCESS             *This,
  IN  SNDW_CODEC_INFO               SndwCodecInfo,
  IN  SNDW_COMMAND                  *TxCommand,
  OUT UINTN                         TxSize
  );


/**
  Function sends one message through the Sndw interface and waits
  for the corresponding ACK message.

  @param[in]  This                Pointer to Sndw Access Protocol
  @param[in]  SndwCodecInfo       Sndw codec information
  @param[in]  TxCommands          Pointer to sending message
  @param[out] RxCommands          Pointer to receiving message

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           Given codec not found
  @retval EFI_TIMEOUT             Sending command has failed
  @retval EFI_DEVICE_ERROR        Response command has failed
**/
typedef
EFI_STATUS
(EFIAPI *SNDW_SEND_WITH_ACK) (
  IN  CONST SNDW_ACCESS         *This,
  IN  SNDW_CODEC_INFO           SndwCodecInfo,
  IN  SNDW_COMMAND              TxCommand,
  OUT SNDW_COMMAND*             RxCommand OPTIONAL
  );


/**
  Function operating on Sndw Fifo for receiving messages from codecs.

  @param[in]  This                Pointer to Sndw Access Protocol
  @param[in]  SndwCodecInfo       Sndw codec information
  @param[out] RxCommands          Pointer to pointer to received messages.
                                  Memory pool is allocated in this function,
                                  but the caller is responsible for
                                  freeing the memory with FreePool.
  @param[out] RxSize              Size of received messages

  @retval EFI_SUCCESS             Message received successfully
  @retval EFI_NOT_FOUND           Given codec not found
  @retval EFI_OUT_OF_RESOURCES    Memory allocation for response failed
**/
typedef
EFI_STATUS
(EFIAPI *SNDW_RECEIVE) (
  IN  CONST SNDW_ACCESS         *This,
  IN  SNDW_CODEC_INFO           SndwCodecInfo,
  OUT SNDW_COMMAND              **RxCommands,
  OUT UINTN*                    RxSize
  );


/**
  Function return first codec saved in codecs list.
  If it does not exist, function sets *CodecInfo to NULL.

  @param[in]  This                Pointer to Sndw Access Protocol
  @param[out] SndwCodecInfo           Pointer to pointer to Sndw codec information structure.
                                  The caller is responsible for freeing the memory.

  @retval EFI_SUCCESS             Codec found
  @retval EFI_NOT_FOUND           Codec not found
  @retval EFI_OUT_OF_RESOURCES    Memory allocation for codec info failed
**/
typedef
EFI_STATUS
(EFIAPI *SNDW_GET_FIRST_CODEC) (
  IN  CONST SNDW_ACCESS           *This,
  OUT       SNDW_CODEC_INFO       **SndwCodecInfo
  );


/**
  Function retrieves the next codec saved in codecs list that follows the given one.
  If it does not exist, function sets *NextCodecInfo to NULL.

  @param[in]  This                Pointer to Sndw Access Protocol
  @param[in]  SndwCodecInfo       Pointer to Sndw codec information structure.
  @param[out] NextCodecInfo       Pointer to pointer to Sndw codec information structure.
                                  The caller is responsible for freeing the memory.

  @retval EFI_SUCCESS             Codec found
  @retval EFI_NOT_FOUND           Codec not found
  @retval EFI_OUT_OF_RESOURCES    Memory allocation for codec info failed
**/
typedef
EFI_STATUS
(EFIAPI *SNDW_GET_NEXT_CODEC) (
  IN  CONST SNDW_ACCESS           *This,
  IN        SNDW_CODEC_INFO       *SndwCodecInfo,
  OUT       SNDW_CODEC_INFO       **NextCodecInfo
  );


/**
  Function enables Soundwire interface and enumerates attached codecs

  @param[in] This           Pointer to Sndw Access Protocol

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR
**/
typedef
EFI_STATUS
(EFIAPI *SNDW_ACCESS_ENABLE) (
    IN CONST SNDW_ACCESS        *This
  );


/**
  Function disables Soundwire interface

  @param[in] This           Pointer to Sndw Access Protocol

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR
**/
typedef
EFI_STATUS
(EFIAPI *SNDW_ACCESS_DISABLE) (
  IN CONST SNDW_ACCESS          *This
  );


///
/// Sndw Access
///
/// This ppi/protocol allows to initialize and communicate with the Sndw controllers.
///
struct _SNDW_ACCESS {
  ///
  /// Send message
  ///
  SNDW_SEND             Send;
  ///
  /// Send message and wait for corresponding ACK message
  ///
  SNDW_SEND_WITH_ACK    SendWithAck;
  ///
  /// Receive message
  ///
  SNDW_RECEIVE          Receive;
  ///
  /// Enable Sndw interface and enumerate attached codecs
  ///
  SNDW_ACCESS_ENABLE    Enable;
  ///
  /// Disable Sndw interface
  ///
  SNDW_ACCESS_DISABLE   Disable;
  ///
  /// Get first codec saved in codecs list created during the enumeration process
  ///
  SNDW_GET_FIRST_CODEC  GetFirstCodec;
  ///
  /// Retrieve the next codec saved in codecs list that follows the given one
  ///
  SNDW_GET_NEXT_CODEC   GetNextCodec;
};

#endif
