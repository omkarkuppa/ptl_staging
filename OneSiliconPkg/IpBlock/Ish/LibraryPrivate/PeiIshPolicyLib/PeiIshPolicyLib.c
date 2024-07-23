/** @file
  This file provides services for Ish policy function

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchLimits.h>
#include <IshConfig.h>

/**
  Print ISH_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
IshPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS    Status;
  ISH_CONFIG    *IshConfig;
  UINT32        CtrlIndex;
  UINT32        CsIndex;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gIshConfigGuid, (VOID *) &IshConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ ISH Config ------------------\n"));

  for (CtrlIndex = 0; CtrlIndex < GetPchMaxIshSpiControllersNum (); CtrlIndex++) {
    DEBUG ((DEBUG_INFO, " SPI%d Enable      = %x\n", CtrlIndex, IshConfig->Spi[CtrlIndex].Enable));
    for (CsIndex =0; CsIndex < GetPchMaxIshSpiControllerCsPinsNum (); CsIndex++) {
      DEBUG ((DEBUG_INFO, " SPI%d Cs#%d Enable = %x\n", CtrlIndex, CsIndex, IshConfig->Spi[CtrlIndex].CsEnable[CsIndex]));
    }

    DEBUG ((DEBUG_INFO, " Pad termination:\n"));
    DEBUG ((DEBUG_INFO, "  Mosi = %d\n", IshConfig->Spi[CtrlIndex].PinConfig.Mosi.PadTermination));
    DEBUG ((DEBUG_INFO, "  Miso = %d\n", IshConfig->Spi[CtrlIndex].PinConfig.Miso.PadTermination));
    DEBUG ((DEBUG_INFO, "  Clk  = %d\n", IshConfig->Spi[CtrlIndex].PinConfig.Clk.PadTermination));
    for (CsIndex = 0; CsIndex < GetPchMaxIshSpiControllerCsPinsNum (); CsIndex++) {
      DEBUG ((DEBUG_INFO, "  Cs#%d = %d\n", CsIndex, IshConfig->Spi[CtrlIndex].PinConfig.Cs[CsIndex].PadTermination));
    }
  }
  for (CtrlIndex = 0; CtrlIndex < GetPchMaxIshUartControllersNum (); CtrlIndex++) {
    DEBUG ((DEBUG_INFO, " UART%d Enable     = %x\n", CtrlIndex, IshConfig->Uart[CtrlIndex].Enable));
    DEBUG ((DEBUG_INFO, " Pad termination:\n"));
    DEBUG ((DEBUG_INFO, "  Rx = %d\n",   IshConfig->Uart[CtrlIndex].PinConfig.Rx.PadTermination));
    DEBUG ((DEBUG_INFO, "  Tx = %d\n",   IshConfig->Uart[CtrlIndex].PinConfig.Tx.PadTermination));
    DEBUG ((DEBUG_INFO, "  Rts = %d\n",  IshConfig->Uart[CtrlIndex].PinConfig.Rts.PadTermination));
    DEBUG ((DEBUG_INFO, "  Cts = %d\n",  IshConfig->Uart[CtrlIndex].PinConfig.Cts.PadTermination));
  }
  for (CtrlIndex = 0; CtrlIndex < GetPchMaxIshI2cControllersNum (); CtrlIndex++) {
    DEBUG ((DEBUG_INFO, " I2C%d Enable      = %x\n", CtrlIndex, IshConfig->I2c[CtrlIndex].Enable));
    DEBUG ((DEBUG_INFO, " Pad termination:\n"));
    DEBUG ((DEBUG_INFO, "  Sda = %d\n",  IshConfig->I2c[CtrlIndex].PinConfig.Sda.PadTermination));
    DEBUG ((DEBUG_INFO, "  Scl = %d\n",  IshConfig->I2c[CtrlIndex].PinConfig.Scl.PadTermination));
  }
  for (CtrlIndex = 0; CtrlIndex < GetPchMaxIshGpNum (); CtrlIndex++) {
    DEBUG ((DEBUG_INFO, " GP_%d Enable      = %x\n", CtrlIndex, IshConfig->Gp[CtrlIndex].Enable));
    DEBUG ((DEBUG_INFO, " Pad termination:\n"));
    DEBUG ((DEBUG_INFO, "  Gpio = %d\n", IshConfig->Gp[CtrlIndex].PinConfig.PadTermination));
  }
  for (CtrlIndex = 0; CtrlIndex < GetPchMaxIshI3cControllersNum (); CtrlIndex++) {
    DEBUG ((DEBUG_INFO, " I3C%d Enable      = %x\n", CtrlIndex, IshConfig->I3c[CtrlIndex].Enable));
    DEBUG ((DEBUG_INFO, " Pad termination:\n"));
    DEBUG ((DEBUG_INFO, "  Sda = %d\n",  IshConfig->I3c[CtrlIndex].PinConfig.Sda.PadTermination));
    DEBUG ((DEBUG_INFO, "  Scl = %d\n",  IshConfig->I3c[CtrlIndex].PinConfig.Scl.PadTermination));
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
IshLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  ISH_CONFIG  *IshConfig;
  IshConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "IshConfig->Header.GuidHob.Name = %g\n", &IshConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IshConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IshConfig->Header.GuidHob.Header.HobLength));
}

STATIC COMPONENT_BLOCK_ENTRY  mIshBlocks = {
  &gIshConfigGuid,
  sizeof (ISH_CONFIG),
  ISH_CONFIG_REVISION,
  IshLoadConfigDefault
};

/**
  Get Ish config block table size.

  @retval      Size of config block
**/
UINT16
IshGetConfigBlockTotalSize (
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
IshAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mIshBlocks, 1);
}
