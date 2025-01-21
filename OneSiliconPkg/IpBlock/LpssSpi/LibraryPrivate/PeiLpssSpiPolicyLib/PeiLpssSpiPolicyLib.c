/** @file
  This file provides services for Lpss Spi policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchLimits.h>
#include <LpssSpiConfig.h>

/**
  Print LPSS_SPI_CONFIG.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
LpssSpiPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS         Status;
  LPSS_SPI_CONFIG    *SpiConfig;
  UINT8              Index;
  UINT8              CsIndex;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gLpssSpiConfigGuid, (VOID *) &SpiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ LPSS SPI Config ------------------\n"));

  for (Index = 0; Index < PCH_MAX_LPSS_SPI_CONTROLLERS; Index++) {
    DEBUG ((DEBUG_INFO, " SPI[%d].Mode                 = %d\n", Index, SpiConfig->SpiDeviceConfig[Index].Mode));
    DEBUG ((DEBUG_INFO, " SPI[%d].DefaultCsOutput      = %d\n", Index, SpiConfig->SpiDeviceConfig[Index].DefaultCsOutput));
    DEBUG ((DEBUG_INFO, " SPI[%d].CsMode               = %d\n", Index, SpiConfig->SpiDeviceConfig[Index].CsMode));
    DEBUG ((DEBUG_INFO, " SPI[%d].CsState              = %d\n", Index, SpiConfig->SpiDeviceConfig[Index].CsState));
    DEBUG ((DEBUG_INFO, " SPI[%d].PinMux.Clk           = 0x%x\n", Index, SpiConfig->SpiDeviceConfig[Index].PinMux.Clk));
    DEBUG ((DEBUG_INFO, " SPI[%d].PinMux.Miso          = 0x%x\n", Index, SpiConfig->SpiDeviceConfig[Index].PinMux.Miso));
    DEBUG ((DEBUG_INFO, " SPI[%d].PinMux.Mosi          = 0x%x\n", Index, SpiConfig->SpiDeviceConfig[Index].PinMux.Mosi));
    for (CsIndex = 0; CsIndex < PCH_MAX_LPSS_SPI_CHIP_SELECTS; CsIndex++) {
      DEBUG ((DEBUG_INFO, " SPI[%d].CsEnable[%d]          = %d\n", Index, CsIndex, SpiConfig->SpiDeviceConfig[Index].CsEnable[CsIndex]));
      DEBUG ((DEBUG_INFO, " SPI[%d].CsPolarity[%d]        = %d\n", Index, CsIndex, SpiConfig->SpiDeviceConfig[Index].CsPolarity[CsIndex]));
      DEBUG ((DEBUG_INFO, " SPI[%d].PinMux.Cs[%d]         = 0x%x\n", Index, CsIndex, SpiConfig->SpiDeviceConfig[Index].PinMux.Cs[CsIndex]));
    }
  }

  DEBUG ((DEBUG_INFO, "------------------------------------------------\n"));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LpssSpiLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINT8                       Index;
  LPSS_SPI_CONFIG*            LpssSpiConfig;

  LpssSpiConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "LpssSpiConfig->Header.GuidHob.Name = %g\n", &LpssSpiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "LpssSpiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", LpssSpiConfig->Header.GuidHob.Header.HobLength));

  for (Index = 0; Index < PCH_MAX_SERIALIO_SPI_CONTROLLERS; Index++) {
    LpssSpiConfig->SpiDeviceConfig[Index].Mode        = LpssSpiDisabled;
    LpssSpiConfig->SpiDeviceConfig[Index].CsEnable[0] = 1;
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mLpssSpiBlocks = {
  &gLpssSpiConfigGuid,
  sizeof (LPSS_SPI_CONFIG),
  LPSS_SPI_CONFIG_REVISION,
  LpssSpiLoadConfigDefault
};

/**
  Get Lpss Spi config block table size.

  @retval      Size of config block
**/
UINT16
LpssSpiGetConfigBlockTotalSize (
  VOID
  )
{
  return mLpssSpiBlocks.Size;
}

/**
  Add LPss Spi ConfigBlock.

  @param[in] ConfigBlockTableAddress       The pointer to config block table

  @retval    EFI_SUCCESS                   The policy default is initialized.
  @retval    EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
LpssSpiAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mLpssSpiBlocks, 1);
}