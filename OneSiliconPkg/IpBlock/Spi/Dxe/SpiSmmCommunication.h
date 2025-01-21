/** @file
  The common header file for SMM SPI module and SMM SPI DXE Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _SPI_SMM_COMMUNICATION_H_
#define _SPI_SMM_COMMUNICATION_H_

#include <Protocol/Spi.h>

#pragma pack(1)

#define SPI_FUNCTION_FLASH_READ                  1
#define SPI_FUNCTION_FLASH_WRITE                 2
#define SPI_FUNCTION_FLASH_ERASE                 3
#define SPI_FUNCTION_FLASH_READ_SFDP             4
#define SPI_FUNCTION_FLASH_READ_JEDEC_ID         5
#define SPI_FUNCTION_FLASH_WRITE_STATUS          6
#define SPI_FUNCTION_FLASH_READ_STATUS           7
#define SPI_FUNCTION_GET_REGION_ADDRESS          8
#define SPI_FUNCTION_READ_PCH_SOFTSTRAP          9
#define SPI_FUNCTION_READ_CPU_SOFTSTRAP          10

typedef struct {
  UINTN       Function;
  EFI_STATUS  ReturnStatus;
  UINT8       Data[1];
} SMM_SPI_COMMUNICATE_FUNCTION_HEADER;

//
// Size of SMM communicate header, without including the payload.
//
#define SMM_COMMUNICATE_HEADER_SIZE  (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data))

//
// Size of SMM SPI communicate function header, without including the payload.
//
#define SMM_SPI_COMMUNICATE_HEADER_SIZE  (OFFSET_OF (SMM_SPI_COMMUNICATE_FUNCTION_HEADER, Data))

typedef struct {
  FLASH_REGION_TYPE  FlashRegionType;
  UINT32             Address;
  UINT32             ByteCount;
  UINT8              *Buffer;
} SMM_SPI_FLASH_READ;

typedef struct {
  FLASH_REGION_TYPE  FlashRegionType;
  UINT32             Address;
  UINT32             ByteCount;
  UINT8              *Buffer;
} SMM_SPI_FLASH_WRITE;

typedef struct {
  FLASH_REGION_TYPE  FlashRegionType;
  UINT32             Address;
  UINT32             ByteCount;
} SMM_SPI_FLASH_ERASE;

typedef struct {
  UINT8              ComponentNumber;
  UINT32             Address;
  UINT32             ByteCount;
  UINT8              *SfdpData;
} SMM_SPI_FLASH_READ_SFDP;

typedef struct {
  UINT8              ComponentNumber;
  UINT32             ByteCount;
  UINT8              *JedecId;
} SMM_SPI_FLASH_READ_JEDEC_ID;

typedef struct {
  UINT32             ByteCount;
  UINT8              *StatusValue;
} SMM_SPI_FLASH_WRITE_STATUS;

typedef struct {
  UINT32             ByteCount;
  UINT8              *StatusValue;
} SMM_SPI_FLASH_READ_STATUS;

typedef struct {
  FLASH_REGION_TYPE  FlashRegionType;
  UINT32             *BaseAddress;
  UINT32             *RegionSize;
} SMM_SPI_GET_REGION_ADDRESS;

typedef struct {
  UINT32             SoftStrapAddr;
  UINT32             ByteCount;
  VOID               *SoftStrapValue;
} SMM_SPI_READ_PCH_SOFTSTRAP;

typedef struct {
  UINT32             SoftStrapAddr;
  UINT32             ByteCount;
  VOID               *SoftStrapValue;
} SMM_SPI_READ_CPU_SOFTSTRAP;

#pragma pack()

#endif // _SPI_SMM_COMMUNICATION_H_
