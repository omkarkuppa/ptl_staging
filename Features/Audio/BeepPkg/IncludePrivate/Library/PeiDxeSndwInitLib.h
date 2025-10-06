/** @file
  Header file for Sndw Audio Init Library

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

@par specification reference:
**/

#ifndef _PEI_DXE_SNDW_INIT_LIB_
#define _PEI_DXE_SNDW_INIT_LIB_

#include <IndustryStandard/Pci22.h>
#include <SndwAccess.h>

#define SNDW_ACCESS_SIGNATURE SIGNATURE_32 ('s', 'd', 'w', 'a')

typedef struct {
  LIST_ENTRY       ListEntry;
  SNDW_CODEC_INFO  CodecInfo;
  UINTN            SndwControllerMmioOffset;
} CODEC_LIST_ENTRY;

#define CODEC_LIST_ENTRY_FROM_LIST_ENTRY(a) BASE_CR (a, CODEC_LIST_ENTRY, ListEntry)

typedef struct {
  UINTN                 Signature;
  SNDW_ACCESS           SndwAccess;
  CODEC_LIST_ENTRY      *CodecListHead;
  UINTN                 EnableCounter;
  EFI_PCI_IO_PROTOCOL   *PciIo;
} SNDW_ACCESS_CONTEXT;

#define SNDW_ACCESS_CONTEXT_FROM_SNDW_ACCESS_PPI_PROTOCOL(a)  CR (a, SNDW_ACCESS_CONTEXT, SndwAccess, SNDW_ACCESS_SIGNATURE)

/**
  Function operating on Sndw Fifo buffer for sending messages to codecs.

  @param[in]  This                Pointer to Sndw Access PPI/Protocol
  @param[in]  SndwCodecInfo       Sndw codec information
  @param[in]  TxCommands          Pointer to send messages
  @param[in]  TxSize              Size of messages to send

  @retval EFI_SUCCESS             Message sent successfully
  @retval EFI_NOT_FOUND           Given codec not found
**/
EFI_STATUS
EFIAPI
SndwSend (
  IN  CONST SNDW_ACCESS    *This,
  IN  SNDW_CODEC_INFO      SndwCodecInfo,
  IN  SNDW_COMMAND         *TxCommand,
  IN  UINTN                TxSize
  );

/**
  Function sends one message through the Sndw interface and waits
  for the corresponding ACK message.

  @param[in]  This                Pointer to Sndw Access PPI/Protocol
  @param[in]  SndwCodecInfo       Sndw codec information
  @param[in]  TxCommands          Pointer to sending message
  @param[out] RxCommands          Pointer to receiving message

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           Given codec not found
  @retval EFI_TIMEOUT             Sending command has failed
  @retval EFI_DEVICE_ERROR        Response command has failed
**/
EFI_STATUS
EFIAPI
SndwSendWithAck (
  IN  CONST SNDW_ACCESS    *This,
  IN  SNDW_CODEC_INFO      SndwCodecInfo,
  IN  SNDW_COMMAND         TxCommand,
  OUT SNDW_COMMAND*        RxCommand OPTIONAL
  );

/**
  Function operating on Sndw Fifo for receiving messages from codecs.

  @param[in]  This                Pointer to Sndw Access PPI/Protocol
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
EFI_STATUS
EFIAPI
SndwReceive (
  IN  CONST SNDW_ACCESS           *This,
  IN  SNDW_CODEC_INFO             SndwCodecInfo,
  OUT SNDW_COMMAND                **RxCommands,
  OUT UINTN*                      RxSize
  );

/**
  Function disables Soundwire interface

  @param[in] This            Pointer to Sndw Access PPI

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR
**/
EFI_STATUS
EFIAPI
SndwAccessDisable (
  IN CONST SNDW_ACCESS       *This
  );

/**
  Function enables Soundwire interface and enumerates attached codecs

  @param[in] This               Pointer to Sndw Access PPI/Protocol

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR
**/
EFI_STATUS
EFIAPI
SndwAccessEnable (
  IN CONST SNDW_ACCESS          *This
  );

/**
  Function return first codec saved in codecs list.
  If it does not exist, function sets *CodecInfo to NULL.

  @param[in]  This                Pointer to Sndw Access PPI/Protocol
  @param[out] SndwCodecInfo       Pointer to pointer to Sndw codec information structure.
                                  The caller is responsible for freeing the memory.

  @retval EFI_SUCCESS             Codec found
  @retval EFI_NOT_FOUND           Codec not found
  @retval EFI_OUT_OF_RESOURCES    Memory allocation for codec info failed
**/
EFI_STATUS
EFIAPI
SndwGetFirstCodec (
  IN  CONST SNDW_ACCESS           *This,
  OUT SNDW_CODEC_INFO             **SndwCodecInfo
  );

/**
  Function retrieves the next codec saved in codecs list that follows the given one.
  If it does not exist, function sets *NextCodecInfo to NULL.

  @param[in]  This                Pointer to Sndw Access PPI/Protocol
  @param[in]  SndwCodecInfo       Pointer to Sndw codec information structure.
  @param[out] NextCodecInfo       Pointer to pointer to Sndw codec information structure.
                                  The caller is responsible for freeing the memory.

  @retval EFI_SUCCESS             Codec found
  @retval EFI_NOT_FOUND           Codec not found
  @retval EFI_OUT_OF_RESOURCES    Memory allocation for codec info failed
**/
EFI_STATUS
EFIAPI
SndwGetNextCodec (
  IN   CONST SNDW_ACCESS          *This,
  IN   SNDW_CODEC_INFO            *SndwCodecInfo,
  OUT  SNDW_CODEC_INFO            **NextSndwCodecInfo
  );

/**
  SndwAccess function prepares and sends address page information to a SoundWire codec
  device. It configures two address page registers (page 1 and page 2) by:
  1. Setting the device address based on the codec's peripheral index
  2. Breaking down the provided address parameter into appropriate bit fields
  3. Sending the configured registers to the SoundWire device with acknowledgment

  @param[in] Stream      Pointer to the SoundWire stream structure
  @param[in] CodecIndex  Index of the codec in the stream's codec information array
  @param[in] Address     The 32-bit address to be configured in the page registers
 **/
VOID
SetAddrPageRegisters (
  IN CONST SNDW_ACCESS  *SndwAccess,
  IN SNDW_CODEC_INFO    SndwCodecInfo,
  IN UINT32             LinearAddress
  );

/**
  Sends a sequence of initialization commands to the audio codec using the provided SNDW_STREAM.
  The function iterates through the PowerOn array, configuring the codec registers as specified.
  For register addresses above 0x7FFF, it sets the appropriate address pages before sending the command.
  Each command is sent using the SendWithAck method of the SndwAccess interface.

  @param[in] Stream                 Pointer to the SNDW_STREAM structure containing codec access and information.

  @retval EFI_SUCCESS               The initialization commands were sent successfully.
  @retval EFI_INVALID_PARAMETER     One or more input parameters are invalid.
**/
EFI_STATUS
SendSdcaCommand (
  IN CONST SNDW_ACCESS  *SndwAccess,
  IN SDCA_COMMAND       *SdcaCommand,
  IN UINTN              NumOfCommands,
  IN SNDW_CODEC_INFO    SndwCodecInfo
  );

#endif
