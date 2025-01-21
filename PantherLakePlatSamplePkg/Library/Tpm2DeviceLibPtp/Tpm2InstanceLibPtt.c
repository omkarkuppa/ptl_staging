/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/PciSegmentLib.h>

#include <Library/PttPtpLib.h>
#include <Guid/PttPTPInstanceGuid.h>

#define R_PTT_HCI_BASE_ADDRESS             0xFED40000

/**
  This service enables the sending of commands to the TPM2.

  @param[in]  InputParameterBlockSize  Size of the TPM2 input parameter block.
  @param[in]  InputParameterBlock      Pointer to the TPM2 input parameter block.
  @param[in]  OutputParameterBlockSize Size of the TPM2 output parameter block.
  @param[in]  OutputParameterBlock     Pointer to the TPM2 output parameter block.

  @retval EFI_SUCCESS            The command byte stream was successfully sent to the device and a response was successfully received.
  @retval EFI_DEVICE_ERROR       The command was not successfully sent to the device or a response was not successfully received from the device.
  @retval EFI_BUFFER_TOO_SMALL   The output parameter block is too small.
**/
EFI_STATUS
EFIAPI
PttSubmitCommand (
  IN UINT32            InputParameterBlockSize,
  IN UINT8             *InputParameterBlock,
  IN OUT UINT32        *OutputParameterBlockSize,
  IN UINT8             *OutputParameterBlock
  )
{
  return PttHciSubmitCommand(InputParameterBlock, InputParameterBlockSize, OutputParameterBlock, OutputParameterBlockSize);
}

EFI_STATUS
EFIAPI
PttRequestUseTpm (
  VOID
  )
{

  BOOLEAN IsPttEnabled;
  UINT32 Data32;

  IsPttEnabled = FALSE;

  //
  // Check if it's SV ME FW first
  // Read FirmwareStatus register D22:F0:40h BIT[3:0]
  //
  Data32 = (PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (0, 0, 22, 0, 0x40))) & 0x0F;

  //
  // Return error if SV ME FW detected
  // ME_STATE_SV_FW = 0x0F
  //
  if (Data32 == 0x0F) {
    return EFI_DEVICE_ERROR;
  }

  if ((*(UINT8 *) (UINTN) (R_PTT_HCI_BASE_ADDRESS + R_TPM_INTERFACE_ID) != 0xFF)) {
    IsPttEnabled = TRUE;
  }

  if(!IsPttEnabled)
  {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

TPM2_DEVICE_INTERFACE  mPttInternalTpm2Device = {
  TPM_DEVICE_INTERFACE_TPM20_PTT_PTP,
  PttSubmitCommand,
  PttRequestUseTpm,
};

EFI_STATUS
EFIAPI
Tpm2InstanceLibPttConstructor (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = Tpm2RegisterTpm2DeviceLib (&mPttInternalTpm2Device);
  if ((Status == EFI_SUCCESS) || (Status == EFI_UNSUPPORTED)) {
    //
    // Unsupported means platform policy does not need this instance enabled.
    //
    return EFI_SUCCESS;
  }
  return Status;
}
