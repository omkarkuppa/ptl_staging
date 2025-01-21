/**@file
    NvmePassThruProtocol Mock for NvmeEraseDxe driver.

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

#include "NvmePassThruProtocolMock.h"

#define IDENTFY_DATA_LEN  4096
#define MAX_NAMESPACE_ID  1
NVME_ADMIN_CONTROLLER_DATA *gIdentifyControlData = NULL;

/**
  Mock functionality for the PassThru of the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
  Mock the NvmeCompletion of the Packet and return status

  @param[in]     This                A pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL instance.
  @param[in]     NamespaceId         A 32 bit namespace ID as defined in the NVMe specification to which the NVM Express Command
                                     Packet will be sent.  A value of 0 denotes the NVM Express controller, a value of all 0xFF's
                                     (all bytes are 0xFF) in the namespace ID specifies that the command packet should be sent to
                                     all valid namespaces.
  @param[in,out] Packet              A pointer to the NVM Express Command Packet.
  @param[in]     Event               If non-blocking I/O is not supported then Event is ignored, and blocking I/O is performed.
                                     If Event is NULL, then blocking I/O is performed. If Event is not NULL and non-blocking I/O
                                     is supported, then non-blocking I/O is performed, and Event will be signaled when the NVM
                                     Express Command Packet completes.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
EFIAPI
MockNvmePassThruProtocol_PassThru(
  IN     EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL          *This,
  IN     UINT32                                      NamespaceId,
  IN OUT EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET    *Packet,
  IN     EFI_EVENT                                   Event OPTIONAL
  )
{
  EFI_NVM_EXPRESS_COMMAND *Command;
  NVME_CQ                 *Cq;

  if ((Packet == NULL) || (Packet->NvmeCmd == NULL) || (Packet->NvmeCompletion == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Command = Packet->NvmeCmd;
  Cq      = (NVME_CQ*) Packet->NvmeCompletion;

  if (Command->Nsid > MAX_NAMESPACE_ID) {
    Cq->Sct = 0;     // refer to the NVMe Spec, Generic Command Status
    Cq->Sc  = 0x0B;  // refer to the NVMe Spec, Invalid Namespace or Format
    return EFI_DEVICE_ERROR;
  }

  switch (Command->Cdw0.Opcode) {
    case NVME_ADMIN_IDENTIFY_CMD:
      if ((Packet->TransferBuffer == NULL) || (Packet->TransferLength != IDENTFY_DATA_LEN)) {
        return EFI_INVALID_PARAMETER;
      }

      if (Command->Flags & CDW10_VALID) {
        if (((Command->Cdw10 & 0xFF) == 1) && (gIdentifyControlData != NULL)) {
          CopyMem (Packet->TransferBuffer, gIdentifyControlData, Packet->TransferLength);
          Cq->Sct = 0;  // refer to the NVMe Spec, Generic Command Status
          Cq->Sc  = 0;  // refer to the NVMe Spec, Successful Completion
          return EFI_SUCCESS;
        }
      }
      Cq->Sct = 0;     // refer to the NVMe Spec, Generic Command Status
      Cq->Sc  = 0x04;  // refer to the NVMe Spec, Data Transfer Error
      return EFI_DEVICE_ERROR;
    case NVME_ADMIN_FORMAT_NVM_CMD:
      if (Command->Flags & CDW10_VALID) {
        if ((gIdentifyControlData != NULL) && ((gIdentifyControlData->Oacs & FORMAT_NVM_SUPPORTED) != 0)) {
          if ((Command->Cdw10 & 0x00000E00) == 0x0200) {
            return EFI_SUCCESS;
          } else if ((Command->Cdw10 & 0x00000E00) == 0x0400) {
            if ((gIdentifyControlData->Fna & BIT2) != 0)
              return EFI_SUCCESS;
          }
        }
      }
      Cq->Sct = 0;     // refer to the NVMe Spec, Generic Command Status
      Cq->Sc  = 0x04;  // refer to the NVMe Spec, Data Transfer Error
      return EFI_DEVICE_ERROR;
    default:
      return EFI_UNSUPPORTED;
  }
}

/**

  Mock functionality for the GetNextNamespace of the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.

  The EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.GetNextNamespace() function retrieves the next valid
  namespace ID on this NVM Express controller.

  @param[in]     This           A pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL instance.
  @param[in,out] NamespaceId    On input, a pointer to a legal NamespaceId for an NVM Express
                                namespace present on the NVM Express controller. On output, a
                                pointer to the next NamespaceId of an NVM Express namespace on
                                an NVM Express controller. An input value of 0xFFFFFFFF retrieves
                                the first NamespaceId for an NVM Express namespace present on an
                                NVM Express controller.

  @retval EFI_SUCCESS           The Namespace ID of the next Namespace was returned.
  @retval EFI_NOT_FOUND         There are no more namespaces defined on this controller.
  @retval EFI_INVALID_PARAMETER NamespaceId is an invalid value other than 0xFFFFFFFF.

**/
EFI_STATUS
EFIAPI
MockNvmePassThruProtocol_GetNextNamespace (
  IN     EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *This,
  IN OUT UINT32                              *NamespaceId
  )
{
  UINT32                        NextNamespaceId;
  EFI_STATUS                    Status;

  if ((This == NULL) || (NamespaceId == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_SUCCESS;

  //
  // If the NamespaceId input value is 0xFFFFFFFF, then get the first valid namespace ID
  //
  if (*NamespaceId == 0xFFFFFFFF) {
    //
    // Start with the first namespace ID
    //
    NextNamespaceId = 1;
    *NamespaceId = NextNamespaceId;
  } else {
    NextNamespaceId = *NamespaceId + 1;
    if (NextNamespaceId > 1) {
      return EFI_NOT_FOUND;
    }
    *NamespaceId = NextNamespaceId;
  }

  return Status;
}

/**
  Construct the Identify controller data.
  @retval   Point to the NVME_ADMIN_CONTROLLER_DATA.
**/
NVME_ADMIN_CONTROLLER_DATA*
EFIAPI
IdentifyControllerDataConstructMock (
  )
{
  if (gIdentifyControlData == NULL) {
    gIdentifyControlData = (NVME_ADMIN_CONTROLLER_DATA*) AllocateZeroPool (sizeof (NVME_ADMIN_CONTROLLER_DATA));
  }
  return gIdentifyControlData;
}

/**
  Destruct the Identify controller data.
**/
VOID
EFIAPI
IdendifyControllerDataDestructorMock (
  )
{
  if (gIdentifyControlData != NULL) {
    FreePool (gIdentifyControlData);
    gIdentifyControlData = NULL;
  }
}
