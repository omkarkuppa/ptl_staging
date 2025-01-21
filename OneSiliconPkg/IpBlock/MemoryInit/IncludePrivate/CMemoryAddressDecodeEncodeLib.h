/** @file
  This Library provides the decode and encode memory addresses protocol installer.

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

@par Specification Reference:
**/
#ifndef _MEMORY_ADDRESS_ENCODE_DECODE_LIB_H_
#define _MEMORY_ADDRESS_ENCODE_DECODE_LIB_H_

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
  );

#endif  //_MEMORY_ADDRESS_ENCODE_DECODE_LIB_H_
