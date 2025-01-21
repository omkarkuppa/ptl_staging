/** @file
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
#include <Ppi/SiPolicy.h>
#include <Library/PchLimits.h>
#include <IshConfig.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PtlPcdIshLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  ISH_CONFIG  *IshConfig;
  UINT8        Index;
  IshConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "IshConfig->Header.GuidHob.Name = %g\n", &IshConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IshConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IshConfig->Header.GuidHob.Header.HobLength));

  IshConfig->Spi[0].Enable = 0;

  for (Index = 0; Index < PCH_MAX_ISH_SPI_CS_PINS; Index++) {
    IshConfig->Spi[0].CsEnable[Index] = 0;
  }

  IshConfig->Uart[0].Enable = 0;
  IshConfig->Uart[1].Enable = 0;

  IshConfig->I2c[0].Enable = 1;
  IshConfig->I2c[1].Enable = 1;
  IshConfig->I2c[2].Enable = 1;

  IshConfig->I3c[0].Enable = 1;
  IshConfig->I3c[1].Enable = 1;

  IshConfig->Gp[0].Enable = 1;
  IshConfig->Gp[1].Enable = 1;
  IshConfig->Gp[2].Enable = 1;
  IshConfig->Gp[3].Enable = 1;
  IshConfig->Gp[4].Enable = 1;
  IshConfig->Gp[5].Enable = 1;
  IshConfig->Gp[6].Enable = 1;
  IshConfig->Gp[7].Enable = 1;
  IshConfig->Gp[8].Enable = 0;
  IshConfig->Gp[9].Enable = 0;
  IshConfig->Gp[10].Enable = 0;
  IshConfig->Gp[11].Enable = 1;

  IshConfig->PdtUnlock = 0;
  IshConfig->MsiInterrupt = 1;
}

STATIC COMPONENT_BLOCK_ENTRY  mIshBlocks = {
  &gIshConfigGuid,
  sizeof (ISH_CONFIG),
  ISH_CONFIG_REVISION,
  PtlPcdIshLoadConfigDefault
};

/**
  Get Ish config block table size.

  @retval      Size of config block
**/
UINT16
PtlPcdIshGetConfigBlockTotalSize (
  VOID
  )
{
  return mIshBlocks.Size;
}

/**
  Add Ish ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PtlPcdIshAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mIshBlocks, 1);
}
