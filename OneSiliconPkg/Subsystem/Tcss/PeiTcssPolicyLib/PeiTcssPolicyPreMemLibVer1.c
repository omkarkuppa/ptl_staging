/** @file
  This file provides services for TCSS PreMem policy function

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
#include <Ppi/SiPolicy.h>
#include <TcssPeiPreMemConfig.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>

/**
  Print TCSS_PEI_PREMEM_CONFIG and serial out.

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
TcssPrintConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS               Status;
  TCSS_PEI_PREMEM_CONFIG   *TcssPeiPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  TcssPreMemConfigDump (TcssPeiPreMemConfig, GetPchMaxTypeCPortNum());
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TcssLoadConfigPreMemDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  TCSS_PEI_PREMEM_CONFIG      *TcssPeiPreMemConfig;
  UINTN                       Index;

  TcssPeiPreMemConfig = (TCSS_PEI_PREMEM_CONFIG *)ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "TcssPeiPreMemConfig->Header.GuidHob.Name = %g\n", &TcssPeiPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "TcssPeiPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", TcssPeiPreMemConfig->Header.GuidHob.Header.HobLength));

  TcssPeiPreMemConfig->TcssEnable = 1;
  // Default set all ports are Type C enabled
  for (Index = 0; Index < GetPchMaxTypeCPortNum(); Index++) {
    TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[Index] = FullFunction;
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mTcssBlocksPreMem = {
  &gTcssPeiPreMemConfigGuid,
  sizeof (TCSS_PEI_PREMEM_CONFIG),
  TCSS_PEI_PREMEM_CONFIG_REVISION,
  TcssLoadConfigPreMemDefault
};

/**
  Get TCSS PreMem config block table size.

  @retval      Size of config block
**/
UINT16
TcssGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return mTcssBlocksPreMem.Size;
}

/**
  Add TCSS PreMem ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
TcssAddConfigBlockPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mTcssBlocksPreMem, 1);
}