/**@file
    NvmePassThruProtocol Mock for the Power Loss Notify Dxe driver.

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
#include "PowerLossNotify.h"

UINT32 gNvmePlnSetting = 0xFF;

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
  EFI_STATUS Status;
  EFI_NVM_EXPRESS_COMMAND *Command;
  EFI_NVM_EXPRESS_COMPLETION *Completion;
  UINT32 *DW0;
  UINT32 *DW3;

  Status = mock_type (EFI_STATUS);
  Completion = mock_type (VOID*);

  Command = Packet->NvmeCmd;
  switch (Command->Cdw0.Opcode) {
    case NVME_ADMIN_SET_FEATURES_CMD:
      if ((Command->Cdw10 & PLN_FEATURE_ID) == PLN_FEATURE_ID) {
        DW0 = &Completion->DW0;
        ((SAVE_FEATURE_DW0*)DW0)->PlnEnable = Command->Cdw11 & BIT0;
        DW3 = &Completion->DW3;
        if ((Status == EFI_SUCCESS) && (((SAVE_FEATURE_DW3 *)DW3)->StatusCode == 0)) {
          gNvmePlnSetting = ((SAVE_FEATURE_DW0*)DW0)->PlnEnable;
        }
      }
      break;
    case NVME_ADMIN_GET_FEATURES_CMD:
      if (((Command->Cdw10 & PLN_FEATURE_ID) == PLN_FEATURE_ID)
         && (Status == EFI_SUCCESS)) {
        DW0 = &Completion->DW0;
        gNvmePlnSetting = ((GET_FEATURE_ATTR_CUR*)DW0)->PlnEnable;
      }
      break;
    default:
      return EFI_UNSUPPORTED;
  }

  CopyMem (Packet->NvmeCompletion, Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));
  return Status;
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
