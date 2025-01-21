/** @file
  ME HECI Control Protocol - allows ME-UEFI FW communication over HECI in DXE and SMM.

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

@par Specification
**/

#ifndef _HECI_CONTROL_PROTOCOL_H_
#define _HECI_CONTROL_PROTOCOL_H_

#include <MeState.h>

///
/// Default number of retries to be used HeciControl API calls
///
#define HECI_DEFAULT_RETRY_NUMBER              3        ///< Retries number

/**
  Default HECI Control PCH ID,
  leave the decision to HeciControl on how to send the message
**/
#define HECI_DEFAULT_PCH_ID                    0xDEFA001D

/**
  Broadcast HECI Control PCH ID,
  Message has to be sent to legacy PCH and all IO Extenders
**/
#define HECI_BROADCAST_PCH_ID                  0xBA0AD01D

/**
  ME HECI Protocol GUID
  This protocol provides an interface to communicate with Intel ME
  over HECI function on PCH ME Device.
**/
extern EFI_GUID gHeciControlProtocolGuid;
extern EFI_GUID gHeciControlSmmProtocolGuid;


typedef struct _HECI_CONTROL_ HECI_CONTROL;


/**
  Reset HECI queue.

  This function triggers HECI queue reset. If timeout is greater than zero it
  also waits until ME is ready to receive messages.

  @param[in,out] This           HeciControl instance
  @param[in]     SecurityEngine Engine where HECI device is located
  @param[in]     HeciDevice     HECI device that should be reset
  @param[in,out] Timeout        On input timeout in us, on exit time left

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_SUCCESS           Reset HECI queue succeed
**/
typedef EFI_STATUS (EFIAPI *HECI_CONTROL_RESET) (
  IN OUT HECI_CONTROL         *This,
  IN     UINT32               SecurityEngine,
  IN     UINT32               HeciDevice,
  IN OUT UINT32               *Timeout
  );

/**
  Send request message to HECI queue, wait for response if needed.

  This function writes one message to HECI queue and - if receive buffer
  was provided and timeout is greater than zero - waits for response message.
  The request can be retried if Retries > 0 (0 means one attempt, no retry),
  but if retries are requested, the pReqMsg and pRspMsg pointers must point
  to different buffers. This is required to make sure request is unchanged.
  Size of receive message buffer is given in bytes in (*RspLen) on input.
  On exit (*RspLen) provides the number of bytes written to the message
  buffer. If buffer is too short the message is truncated.

  @param[in,out] This           HeciControl instance
  @param[in]     SecurityEngine Engine to communicate with
  @param[in]     HeciDevice     HECI device to communicate through
  @param[in,out] Timeout        On input timeout in us, on exit time left
  @param[in]     Retries        The number of retries requested (0 => no retry)
  @param[in]     HostAddress    Host address
  @param[in]     EngineAddress  Engine (ME/IE) address
  @param[in]     ReqMsg         Request message
  @param[in]     ReqLen         Request message length in bytes
  @param[out]    RspMsg         Buffer for the response message
  @param[in,out] RspLen         On input buffer size, on exit message, in bytes
  @param[in,out] SendStatus     Send operation status
  @param[in,out] ReceiveStatus  Receive operation status

  @retval EFI_SUCCESS           Message request has been completed successfully
  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
  @retval EFI_ABORTED           This message is not supported for given Engine HECI device or in current boot path
**/
typedef EFI_STATUS (EFIAPI *HECI_CONTROL_SEND_AND_RECEIVE) (
  IN OUT HECI_CONTROL         *This,
  IN     UINT32               SecurityEngine,
  IN     UINT32               HeciDevice,
  IN OUT UINT32               *Timeout,
  IN     UINT8                Retries,
  IN     UINT8                HostAddress,
  IN     UINT8                EngineAddress,
  IN     UINT32               *ReqMsg,
  IN     UINT32               ReqLen,
     OUT UINT32               *RspMsg,
  IN     UINT32               *RspLen,
  IN OUT EFI_STATUS           *SendStatus,
  IN OUT EFI_STATUS           *ReceiveStatus
  );

/**
  Write a message to HECI queue, if necessary perform fragmentation.

  This function puts one message to HECI queue. If the message is longer than
  HECI queue size and timeout is greater than zero the function will split
  the message into several packets and wait for space in the queue to send all
  packets/fragments. Each packet has HECI packet header added.

  @param[in,out]   This            HeciControl instance
  @param[in]       SecurityEngine  Engine to communicate with
  @param[in]       HeciDevice      HECI device to communicate through
  @param[in,out]   Timeout         On input timeout in us, on exit time left
  @param[in]       HostAddress     Host address
  @param[in]       EngineAddress   Engined (ME/IE) address
  @param[in]       MessageBody     Body of the message to send
  @param[in]       MessageLength   Length of the message to send in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Packet is bigger than queue length
  @retval EFI_SUCCESS           Message has been sent successfully
**/
typedef EFI_STATUS (EFIAPI *HECI_CONTROL_SEND) (
  IN OUT HECI_CONTROL           *This,
  IN     UINT32                 SecurityEngine,
  IN     UINT32                 HeciDevice,
  IN OUT UINT32                 *Timeout,
  IN     UINT8                  HostAddress,
  IN     UINT8                  EngineAddress,
  IN     UINT32                 *MessageBody,
  IN     UINT32                 MessageLength
  );

/**
  Read a message from HECI queue, if necessary perform reassembly.

  This function reads one message from HECI queue. If the message is
  fragmented and timeout is greater than zero the function will wait for
  subsequent packets (fragments) until full message is completed.

  @param[in,out] This           HeciControl instance
  @param[in]     SecurityEngine Engine to communicate with
  @param[in]     HeciDevice     HECI device to communicate through
  @param[in,out] Timeout        On input timeout in us, on exit time left
  @param[out]    HostAddress    Host address
  @param[out]    EngineAddress  Engine (ME/IE) address
  @param[out]    MessageBody    Body of the message to send
  @param[in,out] MessageLength  On input buffer size, on exit message body size in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message exceeds buffer size
  @retval EFI_SUCCESS           Message has been received successfully
**/
typedef EFI_STATUS (EFIAPI *HECI_CONTROL_RECEIVE) (
  IN OUT HECI_CONTROL         *This,
  IN     UINT32               SecurityEngine,
  IN     UINT32               HeciDevice,
  IN OUT UINT32               *Timeout,
     OUT UINT8                *HostAddress,
     OUT UINT8                *EngineAddress,
     OUT UINT32               *MessageBody,
  IN OUT UINT32               *MessageLength
  );

/**
  Read HECI Firmware status

  This function reads HECI firmware status from given PCH and HECI interface

  @param[in]     This           HeciControl instance
  @param[in]     HeciNumber     Heci number to firmware status read [1..4]
  @param[in]     FilterFunction Heci filtering function to be installed
  @param[in,out] FirmwareStatus Pointer to returned firmware status

  @retval EFI_SUCCESS           Returned valid firmware status
  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      Error occurred
**/
typedef EFI_STATUS (EFIAPI *HECI_CONTROL_READ_FIRMWARE_STATUS) (
  IN     HECI_CONTROL        *This,
  IN     UINT32              HeciPchId,
  IN     UINT8               HeciNumber,
  IN OUT UINT32              *FirmwareStatus
  );

/**
  EFI protocol for ME-UEFI FW communication over HECI.

  This protocol defines operations for communication with ME using HECI
  (Host Embedded Controller Interface). There are more HECI functions in
  ME device on PCI. HECI is designated for ME-UEFI FW boot flow communication.
**/
typedef struct _HECI_CONTROL_ {
  HECI_CONTROL_SEND_AND_RECEIVE     HeciSendAndReceive;     /// < Send HECI message and receive Ack
  HECI_CONTROL_SEND                 HeciSend;               /// < Send HECI message
  HECI_CONTROL_RECEIVE              HeciReceive;            /// < Receive HECI message
  HECI_CONTROL_RESET                HeciReset;              /// < Reset HECI interface
  HECI_CONTROL_READ_FIRMWARE_STATUS HeciFirmwareStatus;     /// < Reads HECI Firmware Status
} HECI_CONTROL;

#endif // _HECI_CONTROL_PROTOCOL_H_

