/** @file
  This file defines the PCH SPI DMA PPI which implements the
  Intel(R) PCH SPI DMA Read Interface.

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

#ifndef _PCH_SPI_DMA_PPI_H_
#define _PCH_SPI_DMA_PPI_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>

/**
  This function copies data using SPI DMA from flash to memory

  @param FlashAddress       -  Pointer to Flash Address to copy from.
  @param Destination        -  Pointer in Memory we are copying to.
  @param ReadSize           -  Number of bytes to be read.

  @retval   EFI_INVALID_PARAMETER   Invalid BIOS region chosen
  @retval   EFI_ACCESS_DENIED       SPI controller not available or DMA operation
                                    already ongoing
  @retval   EFI_TIMEOUT             Timed out on waiting on DMA pointer reset or
                                    DMA operation timed out
  @retval   EFI_DEVICE_ERROR        One of status bits reported an error
  @retval   EFI_SUCCESS             SPI DMA copied the data successfully
**/
typedef
EFI_STATUS
(EFIAPI *SPI_DMA_READ) (
  IN VOID                     *FlashAddress,
  IN VOID                     *Destination,
  IN UINTN                    ReadSize
  );

typedef struct {
  SPI_DMA_READ  SpiDmaRead;
} SPI_DMA_READ_PPI;

#endif // _PCH_SPI_DMA_PPI_H_
