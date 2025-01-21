/** @file
  File to support address decoding and encoding

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>   //gBs
#include <Protocol/MemAddrDecodeEncode.h>


#ifndef ADDRESSDECODE_NULL_IMPLEMENT
extern EFI_STATUS
EFIAPI MrcMemoryAddressEncode (
    IN  DRAM_ADDRESS *DramAddress,
    OUT UINT64       *MemoryAddress
    );

extern EFI_STATUS
EFIAPI MrcMemoryAddressDecode (
    IN  UINT64          MemoryAddress,
    OUT DRAM_ADDRESS    *DramAddress
    );

#else
EFI_STATUS
EFIAPI MrcMemoryAddressEncode (
    IN  DRAM_ADDRESS *DramAddress,
    OUT UINT64       *MemoryAddress
    )
{
  DEBUG((DEBUG_INFO, "TO BE IMPLEMENTED\n"));
  return EFI_UNSUPPORTED;
}
EFI_STATUS
EFIAPI MrcMemoryAddressEncode (
    IN  DRAM_ADDRESS *DramAddress,
    OUT UINT64       *MemoryAddress
    )
{
  DEBUG((DEBUG_INFO, "TO BE IMPLEMENTED\n"));
  return EFI_UNSUPPORTED;
}
#endif

static GLOBAL_REMOVE_IF_UNREFERENCED MEMORY_ADDRESS_ENCODE_DECODE_PROTOCOL mMemoryAddressEncodeDecodeProtocol = {
  MEMORY_ADDRESS_ENCODE_DECODE_PROTOCOL_REVISION,
  (MEMORY_ADDRESS_ENCODE) MrcMemoryAddressEncode,
  (MEMORY_ADDRESS_DECODE) MrcMemoryAddressDecode
};

/**
   This function initializes and installs Address Decode/Encode protocol.

   @param[in] ImageHandle  The firmware allocated handle for the EFI image.

   @retval EFI_SUCCESS     The entry point is executed successfully.
   @retval Others          Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
InstallMemoryAddressDecodeEncodeProtocol (
  IN EFI_HANDLE         ImageHandle
  )
{
  EFI_STATUS                    Status;

  DEBUG((DEBUG_INFO,"Install %s\n", __FUNCTION__));
  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gMemoryAddressEncodeDecodeProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mMemoryAddressEncodeDecodeProtocol
                  );
  ASSERT_EFI_ERROR (Status);
  return EFI_SUCCESS;
}