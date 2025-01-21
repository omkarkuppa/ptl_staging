/** @file
  Heci driver core. For PEI/Dxe Phase, determines the HECI device and initializes it.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/HeciTraceLib.h>
#include <Library/HeciUtilsLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <MkhiMsgs.h>
#include <AsfMsgs.h>

#define HECI_TRANSPORT_NUM_DEBUG   "[HECI Transport (%d)]"

/**
  Check if this message is sensitive.

  @param[in]  MsgHeader   Pointer to the HECI message header.
  @param[in]  MsgBody     Pointer to the message body.

  @retval TRUE            This message is sensitive.
  @retval FALSE           This message is not sensitive.
**/
BOOLEAN
CheckMessageSensitivity (
  IN  HECI_PACKET_HEADER    MsgHeader,
  IN  UINT8                 *MsgBody
  )
{
  MKHI_MESSAGE_HEADER  *MkhiHeader;

  MkhiHeader = NULL;

  //
  // Hide MEBx password when send command
  //
  if (MsgHeader.Fields.EngineAddress == HECI_MKHI_MESSAGE_ADDR &&
      MsgHeader.Fields.Length > sizeof (MKHI_MESSAGE_HEADER) &&
      MsgHeader.Fields.MessageComplete) {

    MkhiHeader = (MKHI_MESSAGE_HEADER *) MsgBody;
    if (MkhiHeader->Fields.GroupId == MKHI_PWD_GROUP_ID &&
        MkhiHeader->Fields.IsResponse == 0 &&
        (MkhiHeader->Fields.Command == PWD_CHANGE_CMD || MkhiHeader->Fields.Command == PWD_VALIDATE_CMD)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Print HECI message to the terminal.

  Normally just trace the header, if needed dump whole message to terminal.

  @param[in]  HeciTransport           HECI Transport pointer through which the message will be sent
  @param[in]  Prefix                  Text prefix, whether it is receive or send
  @param[in]  Header                  Packet header
  @param[in]  Body                    Pointer to the packet body
  @param[in]  BodyLen                 The length of packet body
  @param[in]  DebugLevel              Used debug level
  @param[in]  UsedPacketBodyLength    Buffer length to print
**/
VOID
HeciPktTraceDisplay (
  IN  HECI_TRANSPORT_PRIVATE    *HeciTransport,
  IN  CHAR8                     *Prefix,
  IN  HECI_PACKET_HEADER        PacketHeader,
  IN  UINT8                     *PacketBody,
  IN  UINT32                    PacketBodyLength,
  IN  UINT32                    DebugLevel,
  IN  UINT32                    UsedPacketBodyLength
  )
{
  UINT32    LineBreak;
  UINT32    Index;
  UINTN     HeciNumber;

  GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);

  DEBUG ((DebugLevel, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DebugLevel, " %a%08X\n", Prefix, PacketHeader.Data));

  if (PacketBody != NULL && PacketBodyLength > 0) {
    LineBreak = 0;
    Index = 0;

    while (UsedPacketBodyLength-- > 0) {
      if (LineBreak == 0) {
        DEBUG ((DebugLevel, "%02X: ", (Index & 0xF0)));
      }
      DEBUG ((DebugLevel, "%02X ", PacketBody[Index++]));
      LineBreak++;
      if (LineBreak == 16) {
        DEBUG ((DebugLevel, "\n"));
        LineBreak = 0;
      }
      if ((LineBreak == 8) && (UsedPacketBodyLength > 0)) {
        DEBUG ((DebugLevel, "- "));
      }
    }
    DEBUG ((DebugLevel, "\n"));
  }
}

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
  )
{
  UINT32    DebugLevel;
  UINT32    UsedPacketBodyLength;
  BOOLEAN   IsMsgSensitive;

  IsMsgSensitive       = FALSE;
  DebugLevel           = DEBUG_INFO;
  IsMsgSensitive       = CheckMessageSensitivity (PacketHeader, PacketBody);
  UsedPacketBodyLength = IsMsgSensitive ? sizeof (UINT32) : PacketBodyLength;

  HeciPktTraceDisplay (
    HeciTransport,
    Prefix,
    PacketHeader,
    PacketBody,
    PacketBodyLength,
    DebugLevel,
    UsedPacketBodyLength
    );

  if (IsMsgSensitive) {
    DEBUG ((DebugLevel, "Message is trimmed to hide information!\n"));
  }
}
