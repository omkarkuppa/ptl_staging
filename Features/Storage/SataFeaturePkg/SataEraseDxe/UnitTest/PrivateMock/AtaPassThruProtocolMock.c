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

#include "AtaPassThruProtocolMock.h"
#include <Library/DebugLib.h>

#define ATA_TRANSFER_DATA_LEN 512

UINT16 gMasterPassword[16] = {0};
UINT16 gUserPassword[16]   = {0};
ATA_IDENTIFY_DATA *gIdentifyData = NULL;

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
  )
{
  EFI_ATA_COMMAND_BLOCK  *Acb;
  UINT16                 *DataBuffer;

  if (Packet == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Acb = Packet->Acb;
  switch (Acb->AtaCommand) {
    case ATA_CMD_IDENTIFY_DRIVE:
    {
      if ((Packet->Protocol != EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN)
          || (Packet->InDataBuffer == NULL)
          || (Packet->InTransferLength != ATA_TRANSFER_DATA_LEN)) {
        return EFI_INVALID_PARAMETER;
      }

      if (gIdentifyData != NULL) {
        CopyMem (Packet->InDataBuffer, gIdentifyData, Packet->InTransferLength);
        return EFI_SUCCESS;
      } else {
        Packet->Asb->AtaStatus = BIT0;  // refer to ATA spec, Error bit set
        Packet->Asb->AtaError  = BIT2;  // refer to ATA spec, Abort bit set
        return EFI_DEVICE_ERROR;
      }
    }
    case ATA_CMD_SECURITY_SET_PASSWORD:
    {
      if ((Packet->Protocol != EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT)
          || (Packet->OutDataBuffer == NULL)
          || (Packet->OutTransferLength != ATA_TRANSFER_DATA_LEN)) {
        return EFI_INVALID_PARAMETER;
      }

      DataBuffer = Packet->OutDataBuffer;
      //
      //  device support security feature and not in frozen mode
      //
      if ((gIdentifyData != NULL)
          && (((gIdentifyData->security_status & BIT0) != 0))
          && ((gIdentifyData->security_status & BIT3) == 0) ) {
        if ((DataBuffer[0] & BIT0) == 0) {
          //
          // set user password
          //
          CopyMem (gUserPassword, (UINT16*)Packet->OutDataBuffer + 1, 32);
          //
          // Set security feature enable, active user password
          //
          gIdentifyData->security_status |= BIT1;
          gIdentifyData->command_set_feature_enb_85 |= BIT1;
          //
          // set security level = Maximum
          //
          if ((DataBuffer[0] & BIT8) != 0x0) {
            gIdentifyData->security_status |= BIT8;
          }
        } else {
          //
          // check master password identifier valid or not
          //
          if ((gIdentifyData->master_password_identifier == 0)
              || (gIdentifyData->master_password_identifier == 0xffff)) {
            if ((DataBuffer[17] != 0x0) && (DataBuffer[17] != 0xffff)) {
              return EFI_ACCESS_DENIED;
            }
          } else {
            gIdentifyData->master_password_identifier = DataBuffer[17];
          }
          //
          // set master password
          //
          CopyMem (gMasterPassword, (UINT16*)Packet->OutDataBuffer + 1, 32);
        }
        return EFI_SUCCESS;
      }

      Packet->Asb->AtaStatus = BIT0;  // refer to ATA spec, Error bit set
      Packet->Asb->AtaError  = BIT2;  // refer to ATA spec, Abort bit set
      return EFI_DEVICE_ERROR;
    }
    case ATA_CMD_SECURITY_DISABLE_PASSWORD:
    {
      if ((Packet->Protocol != EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT)
          || (Packet->OutDataBuffer == NULL)
          || (Packet->OutTransferLength != ATA_TRANSFER_DATA_LEN)) {
        return EFI_INVALID_PARAMETER;
      }

      DataBuffer = Packet->OutDataBuffer;
      //
      //  device support security feature, but not in frozen mode, not in locked
      //
      if ((gIdentifyData != NULL) &&
         ((gIdentifyData->security_status & (BIT0|BIT2|BIT3)) == (BIT0))) {
        if ((DataBuffer[0] & BIT0) == 0) {
          //
          // if security disable, identify is user, should abort command
          //
          if ((gIdentifyData->security_status & BIT1) != 0) {
            //
            // remove user password
            //
            SetMem (gUserPassword, 32, 0);
            //
            // clear security feature enable and master password capability
            //
            gIdentifyData->security_status &= ~(BIT1|BIT8);
            gIdentifyData->command_set_feature_enb_85 &= ~BIT1;
            return EFI_SUCCESS;
          }
        } else {
          if ((gIdentifyData->security_status & BIT8) == 0) {
            //
            // clear master password
            //
            SetMem (gMasterPassword, 32, 0);
            //
            // clear security feature enable and master password capability
            //
            gIdentifyData->security_status &= ~(BIT1|BIT8);
            gIdentifyData->command_set_feature_enb_85 &= ~BIT1;
            return EFI_SUCCESS;
          }
        }
      }

      Packet->Asb->AtaStatus = BIT0;  // refer to ATA spec, Error bit set
      Packet->Asb->AtaError  = BIT2;  // refer to ATA spec, Abort bit set
      return EFI_DEVICE_ERROR;
    }
    case ATA_CMD_SECURITY_ERASE_PREPARE:
    {
      if ((Packet->Protocol != EFI_ATA_PASS_THRU_PROTOCOL_ATA_NON_DATA)) {
        return EFI_INVALID_PARAMETER;
      }
      //
      //  In frozen mode, abort
      //
      if ((gIdentifyData != NULL) && ((gIdentifyData->security_status & BIT3) != 0)) {
        Packet->Asb->AtaStatus = BIT0;  // refer to ATA spec, Error bit set
        Packet->Asb->AtaError  = BIT2;  // refer to ATA spec, Abort bit set
        return EFI_DEVICE_ERROR;
      }
      return EFI_SUCCESS;
    }
    case ATA_CMD_SECURITY_ERASE_UNIT:
    {
      if ((Packet->Protocol != EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT)
          || (Packet->OutDataBuffer == NULL)
          || (Packet->OutTransferLength != ATA_TRANSFER_DATA_LEN)) {
        return EFI_INVALID_PARAMETER;
      }

      DataBuffer = Packet->OutDataBuffer;
      //
      //  device support security feature, but not in frozen mode
      //
      if ((gIdentifyData != NULL) &&
         ((gIdentifyData->security_status & (BIT0|BIT3)) == (BIT0))) {
        if ((DataBuffer[0] & BIT0) == 0) {
          //
          // user password, if security disable, identify is user, should abort command
          //
          if (((gIdentifyData->security_status & BIT1) != 0)
             && (CompareMem (gUserPassword, &DataBuffer[1], 32) == 0)) {
            //
            // clear security feature enable and master password capability
            //
            gIdentifyData->security_status &= ~(BIT1|BIT8);
            gIdentifyData->command_set_feature_enb_85 &= ~BIT1;
            return EFI_SUCCESS;
          }
        } else if (CompareMem (gMasterPassword, &DataBuffer[1], 32) == 0){
          //
          // master password
          // clear security feature enable and master password capability
          //
          gIdentifyData->security_status &= ~(BIT1|BIT8);
          gIdentifyData->command_set_feature_enb_85 &= ~BIT1;
          return EFI_SUCCESS;
        }
      }

      Packet->Asb->AtaStatus = BIT0;  // refer to ATA spec, Error bit set
      Packet->Asb->AtaError  = BIT2;  // refer to ATA spec, Abort bit set
      return EFI_DEVICE_ERROR;
    }
    case 0xB4: //ATA_SANITIZE_CMD
    {
      if ((Packet->Protocol != EFI_ATA_PASS_THRU_PROTOCOL_ATA_NON_DATA)) {
        return EFI_INVALID_PARAMETER;
      }
      //
      //  Sanitize support
      //
      if ((gIdentifyData != NULL) && ((gIdentifyData->multi_sector_setting & BIT12) != 0)) {
        return EFI_SUCCESS;
      } else {
        Packet->Asb->AtaStatus = BIT0;  // refer to ATA spec, Error bit set
        Packet->Asb->AtaError  = BIT2;  // refer to ATA spec, Abort bit set
        return EFI_DEVICE_ERROR;
      }
    }
    default:
      return EFI_UNSUPPORTED;
  }
}

/**
  Construct the Identify data.
  @retval   Point to the ATA_IDENTIFY_DATA.
**/
ATA_IDENTIFY_DATA*
EFIAPI
IdentifyDataConstructMock (
  )
{
  if (gIdentifyData == NULL) {
    gIdentifyData = (ATA_IDENTIFY_DATA*) AllocateZeroPool (sizeof (ATA_IDENTIFY_DATA));
  }
  return gIdentifyData;
}

/**
  Destruct the Identify data.
**/
VOID
EFIAPI
IdendifyDataDestructorMock (
  )
{
  if (gIdentifyData != NULL) {
    FreePool (gIdentifyData);
    gIdentifyData = NULL;
  }
}
