/** @file
  Definitions for HECI Trace library

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
#ifndef _HECI_TRACE_LIB_H_
#define _HECI_TRACE_LIB_H_

#include <Library/HeciTransportCoreLib.h>

/**
  Print HECI message to the terminal.

  Normally just trace the header, if needed dump whole message to terminal.

  @param[in]  HeciTransport   HECI transport pointer the message will be sent
  @param[in]  Prefix          Text prefix, whether it is receive or send
  @param[in]  Header          Packet header
  @param[in]  Body            Pointer to the packet body
  @param[in]  BodyLen         The length of packet body
**/
VOID
HeciPktTrace (
  IN  HECI_TRANSPORT_PRIVATE    *HeciTransport,
  IN  CHAR8                     *Prefix,
  IN  HECI_PACKET_HEADER        PacketHeader,
  IN  UINT8                     *PacketBody,
  IN  UINT32                    PacketBodyLength
  );

#endif // _HECI_TRACE_LIB_H_
