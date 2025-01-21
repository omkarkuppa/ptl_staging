/** @file
  ME Display Message Protocol definitions

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

#ifndef _ME_DISPLAY_MSG_PROTOCOL_H_
#define _ME_DISPLAY_MSG_PROTOCOL_H_

#define ME_MSG_DISPLAY                 2000000  ///< show warning msg and stay for 2 seconds.

///
/// ME Message ID
///
typedef enum {
  MsgEopError,
  MsgHmrfpoLockFailure,
  MsgHmrfpoUnLockFailure,
  MsgIllegalCpuPluggedIn,
  MsgPlatDisableWait
} ME_MSG_ID;

/**
  This function is used to display message strings in local language in the console.
  To display the message, the routine will check the message ID and ConOut
  the message strings.

  @param[in] MsgId                ME message ID for displaying on screen message
**/
typedef
VOID
(EFIAPI *ME_DISPLAY_MSG) (
  IN ME_MSG_ID                  MsgId
  );

/**
  ME Display Message Protocol

  The interface function is used for displaying messages to screen.
  It is platform vendor's responsibility to implement the function instance.
**/
typedef struct {
  ME_DISPLAY_MSG      DisplayMeMsg;
} ME_DISPLAY_MSG_PROTOCOL;

extern EFI_GUID gMeDisplayMsgProtocolGuid;

#endif // _ME_DISPLAY_MSG_PROTOCOL_H_
