/** @file
  This file provides services for Espi policy function

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
#include <Ppi/SiPolicy.h>
#include <EspiConfig.h>

/**
  Print PCH_ESPI_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
EspiPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS        Status;
  PCH_ESPI_CONFIG   *EspiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ eSPI Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " LGMR Enable %x\n", EspiConfig->LgmrEnable));
  DEBUG ((DEBUG_INFO, " Host C10 Report Enable %x\n", EspiConfig->HostC10ReportEnable));
  DEBUG ((DEBUG_INFO, " BME for host and device Enabled %x\n", EspiConfig->BmeHostDeviceEnabled));
  DEBUG ((DEBUG_INFO, " Lock Link Configuration %x\n", EspiConfig->LockLinkConfiguration));
  DEBUG ((DEBUG_INFO, " Espi Pm HAE %x\n", EspiConfig->EspiPmHAE));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
EspiLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_ESPI_CONFIG  *EspiConfig;
  EspiConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "EspiConfig->Header.GuidHob.Name = %g\n", &EspiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "EspiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", EspiConfig->Header.GuidHob.Header.HobLength));

  EspiConfig->BmeHostDeviceEnabled = TRUE;
  EspiConfig->LockLinkConfiguration = TRUE;
  EspiConfig->EspiPmHAE = TRUE;
  EspiConfig->HideNonFatalErrors = FALSE;
}

STATIC COMPONENT_BLOCK_ENTRY  mEspiBlocks = {
  &gEspiConfigGuid,
  sizeof (PCH_ESPI_CONFIG),
  PCH_ESPI_CONFIG_REVISION,
  EspiLoadConfigDefault
};

/**
  Get Espi config block table size.

  @retval      Size of config block
**/
UINT16
EspiGetConfigBlockTotalSize (
  VOID
  )
{
  return mEspiBlocks.Size;
}

/**
  Add Espi ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EspiAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mEspiBlocks, 1);
}
