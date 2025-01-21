/** @file
  This protocol provides the API to decode and encode memory addresses.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#ifndef _MEM_ADDR_DECODE_ENCODE_H_
#define _MEM_ADDR_DECODE_ENCODE_H_

#define MEMORY_ADDRESS_ENCODE_DECODE_PROTOCOL_REVISION  0x03

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gMemoryAddressEncodeDecodeProtocolGuid;

//
// The detail information of DramAddress
//
typedef struct {
  UINT32  Socket;        ///< Zero based Processor Socket number.
  UINT32  Controller;    ///< Zero based Controller number.
  UINT32  ChannelNumber; ///< Zero based Channel number.
  UINT32  DimmNumber;    ///< Zero based DIMM number.
  UINT32  Rank;          ///< Zero based Rank number.
  UINT32  BankGroup;     ///< Zero based Bank group number.
  UINT32  Bank;          ///< Zero based Bank number.
  UINT32  Ras;           ///< Zero based RAS number.
  UINT32  Cas;           ///< Zero based CAS number.
} DRAM_ADDRESS;

/**
  Address decode function
  Converts memory address to DRAM address

  @param[in]      MemoryAddress  - The 39-bit memory address to decode.
  @param[out]     DramAddress    - The DRAM address struct that the memory address decodes to.

  @retval Returns EFI_SUCCESS if successful, EFI_UNSUPPORTED otherwise.
**/
typedef
EFI_STATUS
(EFIAPI *MEMORY_ADDRESS_DECODE) (
  IN  UINT64       MemoryAddress,
  OUT DRAM_ADDRESS *DramAddress
  );

/**
  Address encode function (reverse address decode)
  DRAM address to memory address conversion

  @param[in]      DramAddress    - The DRAM address that is to be encoded.
  @param[out]     MemoryAddress  - The 39-bit memory address to convert to.

  @retval Returns EFI_SUCCESS if successful, EFI_UNSUPPORTED otherwise.
**/
typedef
EFI_STATUS
(EFIAPI *MEMORY_ADDRESS_ENCODE) (
  IN  DRAM_ADDRESS *DramAddress,
  OUT UINT64       *MemoryAddress
  );

///
/// Memory Encode/Decode Protocol definition
///
typedef struct _MEMORY_ADDRESS_ENCODE_DECODE_PROTOCOL {
  UINT32                 Revision;            ///< Protocol revision
  MEMORY_ADDRESS_ENCODE  MemoryAddressEncode; ///< Memory Encode Function. Converts Memory Address to DRAM Address
  MEMORY_ADDRESS_DECODE  MemoryAddressDecode; ///< Memory Decode Function. Converts DRAM Address to Memory Address
} MEMORY_ADDRESS_ENCODE_DECODE_PROTOCOL;

#endif // _MEM_ADDR_DECODE_ENCODE_H_
