/**@file
    AtaPassThruProtocol Mock

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

@par Specification
**/
#ifndef _ATA_PASSTHRU_PROTOCOL_MOCK_H_
#define _ATA_PASSTHRU_PROTOCOL_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/UnitTestLib.h>
#include <Protocol/AtaPassthru.h>
#include <IndustryStandard/Atapi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

extern UINT16 gMasterPassword[16];
extern UINT16 gUserPassword[16];
extern ATA_IDENTIFY_DATA *gIdentifyData;

/**
  Mock functionality for the PassThru of the EFI_ATA_PASS_THRU_PROTOCOL.

  @param[in]      This               A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param[in]      Port               The port number of the ATA device to send the command.
  @param[in]      PortMultiplierPort The port multiplier port number of the ATA device to send the command.
                                     If there is no port multiplier, then specify 0xFFFF.
  @param[in, out] Packet             A pointer to the ATA command to send to the ATA device specified by Port
                                     and PortMultiplierPort.
  @param[in]      Event               If non-blocking I/O is not supported then Event is ignored, and blocking
                                     I/O is performed. If Event is NULL, then blocking I/O is performed. If
                                     Event is not NULL and non blocking I/O is supported, then non-blocking
                                     I/O is performed, and Event will be signaled when the ATA command completes.

  @retval EFI_SUCCESS                The ATA command was sent by the host. For bi-directional commands,
                                     InTransferLength bytes were transferred from InDataBuffer. For write and
                                     bi-directional commands, OutTransferLength bytes were transferred by OutDataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The ATA command was not executed. The number of bytes that could be transferred
                                     is returned in InTransferLength. For write and bi-directional commands,
                                     OutTransferLength bytes were transferred by OutDataBuffer.
  @retval EFI_NOT_READY              The ATA command could not be sent because there are too many ATA commands
                                     already queued. The caller may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the ATA command.
  @retval EFI_INVALID_PARAMETER      Port, PortMultiplierPort, or the contents of Acb are invalid. The ATA
                                     command was not sent, so no additional status information is available.

**/
EFI_STATUS
EFIAPI
MockAtaPassThru_PassThru (
  IN     EFI_ATA_PASS_THRU_PROTOCOL        *This,
  IN     UINT16                            Port,
  IN     UINT16                            PortMultiplierPort,
  IN OUT EFI_ATA_PASS_THRU_COMMAND_PACKET  *Packet,
  IN     EFI_EVENT                         Event OPTIONAL
  );

/**
  Construct the Identify data.
  @retval   Point to the ATA_IDENTIFY_DATA.
**/
ATA_IDENTIFY_DATA*
EFIAPI
IdentifyDataConstructMock (
  );

/**
  Destruct the Identify data.
**/
VOID
EFIAPI
IdendifyDataDestructorMock (
  );

#endif /*_ATA_PASSTHRU_PROTOCOL_MOCK_H_*/
