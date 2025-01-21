/** @file
  This file provides services for SPI policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <SpiConfig.h>

/**
  Print SPI_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
SpiPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS    Status;
  SPI_CONFIG    *SpiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSpiConfigGuid, (VOID *) &SpiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ SPI Config ------------------------\n"));
  DEBUG ((DEBUG_INFO, " ExtendedBiosDecodeRangeEnable:  0x%X\n", SpiConfig->ExtendedBiosDecodeRangeEnable));
  DEBUG ((DEBUG_INFO, " ExtendedBiosDecodeRangeBase:    0x%X\n", SpiConfig->ExtendedBiosDecodeRangeBase));
  DEBUG ((DEBUG_INFO, " ExtendedBiosDecodeRangeLimit:   0x%X\n", SpiConfig->ExtendedBiosDecodeRangeLimit));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
SpiLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  SPI_CONFIG        *SpiConfig;
  SpiConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SpiConfig->Header.GuidHob.Name = %g\n", &SpiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SpiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SpiConfig->Header.GuidHob.Header.HobLength));

  //
  // Disable extended BIOS Direct Reads decoding
  //
  SpiConfig->ExtendedBiosDecodeRangeEnable = FALSE;

  //
  // Before changing default range check Memory Map for available regions
  // Below range is recommendation based on Platform FAS
  //
  SpiConfig->ExtendedBiosDecodeRangeBase = 0xF8000000;
  SpiConfig->ExtendedBiosDecodeRangeLimit = 0xF9FFFFFF;
}

STATIC COMPONENT_BLOCK_ENTRY  mSpiBlocks = {
  &gSpiConfigGuid,
  sizeof (SPI_CONFIG),
  SPI_CONFIG_REVISION,
  SpiLoadConfigDefault
};

/**
  Get Spi config block table size.

  @retval      Size of config block
**/
UINT16
SpiGetConfigBlockTotalSize (
  VOID
  )
{
  return mSpiBlocks.Size;
}

/**
  Add Spi ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
SpiAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mSpiBlocks, 1);
}
