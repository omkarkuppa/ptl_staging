/** @file
  HECI Flow Protocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _HECI_FLOW_PROTOCOL_H
#define _HECI_FLOW_PROTOCOL_H

#include <MeDefines.h>

typedef struct _HECI_FLOW_PROTOCOL  HECI_FLOW_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *HECI_FLOW_SEND_AND_RECEIVE) (
  IN       HECI_DEVICE             HeciDev,
  IN OUT   VOID                    *Message,
  IN       UINT32                  Length,
  IN OUT   UINT32                  *RecLength,
  IN CONST EFI_GUID                *Guid
  );

typedef
EFI_STATUS
(EFIAPI *HECI_FLOW_SEND) (
  IN       HECI_DEVICE             HeciDev,
  IN OUT   VOID                    *Message,
  IN OUT   UINT32                  Length,
  IN CONST EFI_GUID                *Guid
  );

/*
  This protocol allows sending HECI messages, over a connection with flow control, to CSME clients
  identified by GUID rather than hardcoded address. This simple API wraps all connection-related complexity,
  such as search for GUIDs, establishing connections, flow control token management and disconnection.
  HECI_FLOW_PROTOCOL is an extension of HECI_PROTOCOL. They can be used together, at the same time.
  Limitation:
  Except for flow control tokens, no response is expected for Send and only a single response message is expected for
  SendAndReceive. It is not allowed to use HECI_FLOW_PROTOCOL to create connections that will result in CSME sending
  more messages than that. Unexpected response message will not be consumed and will jam the underlying HECI_PROTOCOL.
*/
struct _HECI_FLOW_PROTOCOL {
  HECI_FLOW_SEND_AND_RECEIVE SendAndReceive; ///< Send HECI message with flow control and wait for response
  HECI_FLOW_SEND             Send;           ///< Send HECI message with flow control, do not expect response
};

extern EFI_GUID gHeciFlowProtocolGuid;

#endif // _HECI_FLOW_PROTOCOL_H
