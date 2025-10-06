/** @file
  This file is PeiCpuPolicy library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#include "PeiTxtPolicyLibrary.h"
#include <Library/PciSegmentLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PostCodeLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/MsrFruLib.h>
#include <Library/CpuInfoFruLib.h>
#include "TxtConfig.h"
#include <Txt.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadTxtConfigDefaultPreMem (
  IN VOID          *ConfigBlockPointer
  )
{
  TXT_PREMEM_CONFIG  *TxtPreMemConfig;
  CPUID_VERSION_INFO_ECX      Ecx;
  UINT32                      TxtCapableChipset;
  TxtPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "TxtPreMemConfig->Header.GuidHob.Name = %g\n", &TxtPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "TxtPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", TxtPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU TXT PreMem configuration
  ********************************/
  ///
  /// Execute CPUID and GETSEC[CAPABILITIES] to check both processor and chipset are TXT capable.
  ///
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);
  TxtCapableChipset = CheckSmxCapabilities();
  if ((Ecx.Bits.SMX == 1) && (TxtCapableChipset & BIT0)) {
     TxtPreMemConfig->SinitMemorySize    = TXT_SINIT_MEMORY_SIZE;
     TxtPreMemConfig->TxtHeapMemorySize  = TXT_HEAP_MEMORY_SIZE;
     TxtPreMemConfig->TgaSize            = TXT_TGA_MEMORY_SIZE;
     TxtPreMemConfig->TxtLcpPdBase       = TXT_LCP_PD_BASE;
     TxtPreMemConfig->TxtLcpPdSize       = TXT_LCP_PD_SIZE;
  }

  //
  // Initialize CMOS TXT Offset field
  //
  TxtPreMemConfig->CmosTxtOffset    = FIT_REC_TXT_POLICY_TYPE_A;

#if FixedPcdGetBool(PcdTxtEnable) == 1
  TxtPreMemConfig->TxtImplemented = 0;
#endif
}

static COMPONENT_BLOCK_ENTRY  mTxtIpBlocksPreMem [] = {
  {&gTxtPreMemConfigGuid,         sizeof (TXT_PREMEM_CONFIG),         TXT_PREMEM_CONFIG_REVISION,         LoadTxtConfigDefaultPreMem},
};

/**
  Get CPU PREMEM config block table total size.

  @retval Size of CPU PREMEM config block table
**/
UINT16
EFIAPI
TxtGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mTxtIpBlocksPreMem[0], sizeof (mTxtIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  CpuAddPreMemConfigBlocks add all CPU PREMEM config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add CPU PREMEM config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
TxtAddConfigBlockPreMem (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;
  DEBUG((DEBUG_INFO, "TXT Pre-Mem Entry \n"));
  //PostCode (0xC00);

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mTxtIpBlocksPreMem[0], sizeof (mTxtIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
  DEBUG((DEBUG_INFO, "TxtAddPreMemConfigBlocks Done \n"));
  //PostCode (0xC0F);

  return Status;
}

/**
  Print CPU_TXT_PREMEM_CONFIG and serial out.

  @param[in] CpuTxtPreMemConfig     Pointer to a CPU_TXT_PREMEM_CONFIG
**/
VOID
TxtConfigPrintPreMem (
  IN CONST TXT_PREMEM_CONFIG        *TxtPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ TXT PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : CmosTxtOffset = 0x%x\n", TxtPreMemConfig->CmosTxtOffset));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : SinitMemorySize = 0x%x\n", TxtPreMemConfig->SinitMemorySize));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : TxtHeapMemorySize = 0x%x\n", TxtPreMemConfig->TxtHeapMemorySize));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : TgaSize = 0x%x\n", TxtPreMemConfig->TgaSize));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : TxtLcpPdBase = 0x%x\n", TxtPreMemConfig->TxtLcpPdBase));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : TxtLcpPdSize = 0x%x\n", TxtPreMemConfig->TxtLcpPdSize));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : TxtDprMemoryBase = 0x%x\n", TxtPreMemConfig->TxtDprMemoryBase));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : TxtDprMemorySize = 0x%x\n", TxtPreMemConfig->TxtDprMemorySize));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : BiosAcmSize = 0x%x\n", TxtPreMemConfig->BiosAcmSize));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : BiosAcmBase = 0x%x\n", TxtPreMemConfig->BiosAcmBase));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : ApStartupBase = 0x%x\n", TxtPreMemConfig->ApStartupBase));
  DEBUG ((DEBUG_INFO, "TXT_PREMEM_CONFIG : TxtImplemented = 0x%x\n", TxtPreMemConfig->TxtImplemented));
}


/**
  Print Txt Config blocks of SI_PREMEM_POLICY_PPI and serial out in PreMem.

  @param[in] SiPreMemPolicyPpi The SI Pre-Mem Policy PPI instance
**/
VOID
TxtPrintPolicyPreMem (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  EFI_STATUS                    Status;
  TXT_PREMEM_CONFIG             *TxtPreMemConfig;

  DEBUG ((DEBUG_INFO, "\n------------------------ TXT - SiPreMemPolicyPpi Print Begin in PreMem -----------------\n"));

  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPreMemPolicyPpi->TableHeader.Header.Revision));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTxtPreMemConfigGuid, (VOID *) &TxtPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  TxtConfigPrintPreMem(TxtPreMemConfig);

  DEBUG ((DEBUG_INFO, "\n------------------------ TXT - SiPreMemPolicyPpi Print End -----------------\n\n"));
DEBUG_CODE_END();
}

