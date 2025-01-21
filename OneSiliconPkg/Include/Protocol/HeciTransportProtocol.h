/** @file
  ME HECI Transport Protocol - allows ME-UEFI FW communication over HECI in DXE and SMM.

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

#ifndef _HECI_TRANSPORT_PROTOCOL_H_
#define _HECI_TRANSPORT_PROTOCOL_H_

typedef struct _HECI_TRANSPORT_ HECI_TRANSPORT;

/**
  Maximum possible length of HECI packet not including header.

  Actually it is maximum length of HECI queue that can be configured by ME
  minus packet header size. Single packet must not exceed HECI queue size.
  Actual HECI queue size is configured by ME, so this macro does not define
  the current maximum packet length, but the maximum possible length.
  The current maximum packet length for the to host and from host directions.
**/
#define MAX_CIRCULAR_BUFFER_SIZE  (128 * sizeof (UNIT32))
#define HECI_PKT_MAXLEN           (MAX_CIRCULAR_BUFFER_SIZE - sizeof (HECI_PACKET_HEADER))

/**
  Default HECI communication timeouts for HECI stack
**/
#define HECI_TIMEOUT_TICK             100  ///< Timeout grain in us for the HECI functions
#define HECI_SEND_WAITTIME          10000  ///< 10 ms timeout in microseconds,
#define HECI_RESET_TIMEOUT        2000000  ///< 2 s timeout in microseconds
#define HECI_INIT_TIMEOUT        15000000  ///< 15 s timeout in microseconds
#define HECI_READ_TIMEOUT          500000  ///< 0.5 s timeout in microseconds
#define HECI_SEND_TIMEOUT          500000  ///< 0.5 s timeout in microseconds

/**
  ME HECI Protocol GUID
  This protocol provides an interface to communicate with Intel ME
  over HECI function on PCH ME Device.
**/
extern EFI_GUID gHeciTransportProtocolGuid;
extern EFI_GUID gHeciTransportSmmProtocolGuid;

///
/// HECI device information information
///
typedef struct {
  UINT32    SecurityEngine;
  UINTN     HeciSegment;
  UINTN     HeciBus;
  UINTN     HeciDevice;
  UINTN     HeciFunction;
} HECI_DEVICE_INFO;

/**
  Get HECI Device Information Record

  @param[in]      This              Pointer to HECI Transport device context structure
  @param[in, out] HeciDeviceInfo    Pointer to HECI device information structure

  @retval EFI_SUCCESS               HeciDeviceInfo has been updated and keeps correct HECI device information
  @retval EFI_INVALID_PARAMETER     Wrong parameters passed
**/
typedef
HECI_DEVICE_INFO *
(EFIAPI *HECI_TRANSPORT_GET_HECI_DEVICE_INFO) (
  IN      HECI_TRANSPORT        *This,
  IN OUT  HECI_DEVICE_INFO      *HeciDeviceInfo
  );

/**
  Function gets initialization state flag

  Initialization state flag should be set to true after interface
  is successfully initialized

  @param[in]  This    Pointer to HECI Transport device context structure

  @retval TRUE        Heci interface is initialized
  @retval FALSE       Heci interface is not initialized
**/
typedef
BOOLEAN
(EFIAPI *HECI_TRANSPORT_GET_INIT_STATE) (
  IN  HECI_TRANSPORT      *This
  );

/**
  Return Host Firmware status
  Corresponds to HECI HPS (part of) section 4.2.1

  @param[in]      This              HECI Transport pointer through which the message will be sent
  @param[in,out]  FirmwareStatus    Pointer to returned firmware status

  @retval EFI_SUCCESS               Returned valid firmware status
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid
  @retval EFI_DEVICE_ERROR          Error occurred
**/
typedef
EFI_STATUS
(EFIAPI *HECI_TRANSPORT_GET_FIRMWARE_STATUS) (
  IN      HECI_TRANSPORT    *This,
  IN OUT  UINT32            *FirmwareStatus
  );

/**
  Initialize HECI interface.

  This function initializes host side of HECI interface. If timeout is
  greater than zero it also waits until ME is ready to receive messages.

  @param[in]      This            HECI Transport pointer through which the message will be sent
  @param[in,out]  Timeout         On input timeout in us, on exit time left

  @retval EFI_SUCCESS             Initialization succeeded
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_UNSUPPORTED         Device not supported for this operation
  @retval EFI_DEVICE_ERROR        Device not present or HECI MBar can't be initialized
**/
typedef
EFI_STATUS
(EFIAPI *HECI_TRANSPORT_INIT) (
  IN      HECI_TRANSPORT    *This,
  IN OUT  UINT32            *Timeout
  );

/**
  Public functions.
**/

/**
  Reset HECI queue.

  This function triggers HECI queue reset. If timeout is greater than zero it
  also waits until ME is ready to receive messages.

  @param[in]      This            HECI Transport pointer through which the message will be sent
  @param[in,out]  Timeout         On input timeout in us, on exit time left

  @retval EFI_SUCCESS             Reset HECI queue succeeded
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_DEVICE_ERROR        HECI MBar can't be initialized
  @retval EFI_UNSUPPORTED         Device not supported for this operation
  @retval EFI_TIMEOUT             Timeout occurs during operation
**/
typedef
EFI_STATUS
(EFIAPI *HECI_TRANSPORT_RESET) (
  IN      HECI_TRANSPORT    *This,
  IN OUT  UINT32            *Timeout
  );

/**
  Send request message to HECI queue, wait for response if needed.

  This function writes one message to HECI queue and - if receive buffer
  was provided and timeout is greater than zero - waits for response message.
  The request can be retried if Retries > 0 (0 means one attempt, no retry),
  but if retries are requested, the ReqMsg and RspMsg pointers must point
  to different buffers. This is required to make sure request is unchanged.
  Size of receive message buffer is given in bytes in (*RspLen) on input.
  On exit (*RspLen) provides the number of bytes written to the message
  buffer. If buffer is too short the message is truncated.

  @param[in]      This            HECI Transport pointer through which the message will be sent
  @param[in,out]  Timeout         On input timeout in us, on exit time left
  @param[in]      Retries         The number of retries requested (0 => no retry)
  @param[in]      HostAddress     Host address
  @param[in]      EngineAddress   Engine address
  @param[in]      ReqMsg          Request message
  @param[in]      ReqLen          Request message length in bytes
  @param[out]     RspMsg          Buffer for the response message
  @param[in,out]  RspLen          On input buffer size, on exit message, in bytes
  @param[in,out]  SendStatus      Send operation status
  @param[in,out]  ReceiveStatus   Receive operation status

  @retval EFI_SUCCESS             Message request has been completed successfully
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_DEVICE_ERROR        HECI MBar can't be initialized
  @retval EFI_UNSUPPORTED         Device not supported for this operation
  @retval EFI_TIMEOUT             Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE     Message is too big
**/
typedef
EFI_STATUS
(EFIAPI *HECI_TRANSPORT_SEND_AND_RECEIVE) (
  IN      HECI_TRANSPORT    *This,
  IN OUT  UINT32            *Timeout,
  IN      UINT8             Retries,
  IN      UINT8             HostAddress,
  IN      UINT8             EngineAddress,
  IN      UINT32            *ReqMsg,
  IN      UINT32            ReqLen,
     OUT  UINT32            *RspMsg,
  IN OUT  UINT32            *RspLen,
  IN OUT  EFI_STATUS        *SendStatus,
  IN OUT  EFI_STATUS        *ReceiveStatus
  );

/**
  Write a message to HECI queue, if necessary perform fragmentation.

  This function puts one message to HECI queue. If the message is longer than
  HECI queue size and timeout is greater than zero the function will split
  the message into several packets and wait for space in the queue to send all
  packets/fragments. Each packet has HECI packet header added.

  @param[in]      This            HECI Transport pointer through which the message will be sent
  @param[in,out]  Timeout         On input timeout in us, on exit time left
  @param[in]      HostAddress     Host address
  @param[in]      EngineAddress   Engine address
  @param[in]      MessageBody     Body of the message to send
  @param[in]      MessageLength   Length of the message to send in bytes

  @retval EFI_SUCCESS             Message has been sent successfully
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_UNSUPPORTED         Device not supported for this operation
  @retval EFI_DEVICE_ERROR        HECI MBar can't be initialized
  @retval EFI_TIMEOUT             Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE     Packet is bigger than queue length
**/
typedef
EFI_STATUS
(EFIAPI *HECI_TRANSPORT_SEND) (
  IN     HECI_TRANSPORT      *This,
  IN OUT UINT32              *Timeout,
  IN     UINT8               HostAddress,
  IN     UINT8               EngineAddress,
  IN     UINT32              *MsgBody,
  IN     UINT32              MsgLen
  );

/**
  Read a message from HECI queue, if necessary perform reassembly.

  This function reads one message from HECI queue. If the message is
  fragmented and timeout is greater than zero the function will wait for
  subsequent packets (fragments) until full message is completed.

  @param[in]      This            HECI Transport pointer from which the message should be received
  @param[in,out]  Timeout         On input timeout in us, on exit time left
  @param[out]     HostAddress     Host address
  @param[out]     EngineAddress   Engine address
  @param[out]     MessageBody     Body of the message to receive
  @param[in,out]  MessageLength   On input buffer size, on exit message body size in bytes

  @retval EFI_SUCCESS             Message has been received successfully
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_UNSUPPORTED         Device not supported for this operation
  @retval EFI_DEVICE_ERROR        HECI MBar can't be initialized
  @retval EFI_TIMEOUT             Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE     Message exceeds buffer size
**/
typedef
EFI_STATUS
(EFIAPI *HECI_TRANSPORT_RECEIVE) (
  IN     HECI_TRANSPORT      *This,
  IN OUT UINT32              *Timeout,
     OUT UINT8               *HostAddress,
     OUT UINT8               *EngineAddress,
     OUT UINT32              *MsgBody,
  IN OUT UINT32              *MsgLen
  );

/**
  Get next HECI Transport

  @param[in]  This    Pointer to HECI Transport protocol instance from which the next instance should be returned.

  @retval Pointer to the next HeciTransport protocol instance
**/
typedef
HECI_TRANSPORT *
(EFIAPI *HECI_TRANSPORT_GET_NEXT_HECI) (
  IN HECI_TRANSPORT     *This
  );

/**
  EFI protocol for ME-UEFI FW communication over HECI.

  This protocol defines operations for communication with ME using HECI
  (Host Embedded Controller Interface). There are more HECI functions in
  ME device on PCI. HECI is designated for ME-UEFI FW boot flow communication.
**/
typedef struct _HECI_TRANSPORT_ {
  HECI_TRANSPORT_GET_INIT_STATE         GetInitializationState; /// < Get HECI Transport initialization state
  HECI_TRANSPORT_GET_HECI_DEVICE_INFO   GetHeciDeviceInfo;      /// < Get HECI information record
  HECI_TRANSPORT_GET_FIRMWARE_STATUS    GetHeciFirmwareStatus;  /// < Get HECI Firmware Status
  HECI_TRANSPORT_GET_NEXT_HECI          GetNextHeciTransport;   /// < HECI Transport protocol walker
  HECI_TRANSPORT_INIT                   HeciInit;               /// < Initialize HECI interface
  HECI_TRANSPORT_SEND_AND_RECEIVE       HeciSendAndReceive;     /// < Send HECI message and receive Ack
  HECI_TRANSPORT_SEND                   HeciSend;               /// < Send HECI message
  HECI_TRANSPORT_RECEIVE                HeciReceive;            /// < Receive HECI message
  HECI_TRANSPORT_RESET                  HeciReset;              /// < Reset HECI interface
} HECI_TRANSPORT;

#endif // _HECI_TRANSPORT_PROTOCOL_H_

