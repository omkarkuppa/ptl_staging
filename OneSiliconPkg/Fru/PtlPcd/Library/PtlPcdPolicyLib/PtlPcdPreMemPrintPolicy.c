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

**/
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataSocLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/PchPcieRpLib.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <PchPcieRpConfig.h>
#include <FlashProtectionConfig.h>
#include <LockDownConfig.h>
#include <HsioConfig.h>
#include <LpcConfig.h>
#include <HsioPcieConfig.h>

/**
  Print PCH_GENERAL_PREMEM_CONFIG and serial out.

  @param[in] PchGeneralPreMemConfig     Pointer to a PCH_GENERAL_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintGeneralPreMemConfig (
  IN CONST PCH_GENERAL_PREMEM_CONFIG    *PchGeneralPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH General PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Port80Route= %x\n", PchGeneralPreMemConfig->Port80Route));
  DEBUG ((DEBUG_INFO, " GpioOverride= %x\n", PchGeneralPreMemConfig->GpioOverride));
  DEBUG ((DEBUG_INFO, " IoeDebugEn= %x\n", PchGeneralPreMemConfig->IoeDebugEn));
  DEBUG ((DEBUG_INFO, " PmodeClkEn= %x\n", PchGeneralPreMemConfig->PmodeClkEn));
  DEBUG ((DEBUG_INFO, " DisableResets = %x\n", PchGeneralPreMemConfig->DisableResets));
}


/**
  Print PCH_LPC_PREMEM_CONFIG and serial out.

  @param[in] LpcPreMemConfig                  Pointer to a PCH_LPC_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintLpcPreMemConfig (
  IN CONST PCH_LPC_PREMEM_CONFIG              *LpcPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH LPC PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "EnhancePort8xhDecoding= %x\n", LpcPreMemConfig->EnhancePort8xhDecoding));
}

/**
  Print PCH_HSIO_PCIE_PREMEM_CONFIG and serial out.

  @param[in] HsioPciePreMemConfig     Pointer to a PCH_HSIO_PCIE_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintHsioPciePreMemConfig (
  IN CONST PCH_HSIO_PCIE_PREMEM_CONFIG *HsioPciePreMemConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ HSIO PCIE PreMem Config ------------------\n"));
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioRxSetCtleEnable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioRxSetCtleEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioRxSetCtle= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioRxSetCtle));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen1DownscaleAmpEnable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen1DownscaleAmp= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DownscaleAmpEnable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DownscaleAmp= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen3DownscaleAmpEnable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen3DownscaleAmp= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen1DeEmphEnable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmphEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen1DeEmph= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmph));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DeEmph3p5Enable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5Enable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DeEmph3p5= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DeEmph6p0Enable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0Enable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DeEmph6p0= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0));
  }
}

/**
  Print PCH_PCIE_RP_PREMEM_CONFIG and serial out.

  @param[in] PcieRpPreMemConfig        Pointer to a PCH_PCIE_RP_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintPcieRpPreMemConfig (
  IN CONST PCH_PCIE_RP_PREMEM_CONFIG    *PcieRpPreMemConfig
  )
{
  UINT32 Index;
  DEBUG ((DEBUG_INFO, "------------------ PCH PCIe RP PreMem Config ------------------\n"));

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    DEBUG ((DEBUG_INFO, " Port[%d] RpEnabled= %x\n", Index, (PcieRpPreMemConfig->RpEnabledMask & (UINT32) (1 << Index)) != 0 ));
  }
  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index++) {
    DEBUG ((DEBUG_INFO, " Clock[%d] Usage= %x\n", Index, PcieRpPreMemConfig->PcieClock[Index].Usage));
    DEBUG ((DEBUG_INFO, " Clock[%d] ClkReq= %x\n", Index, PcieRpPreMemConfig->PcieClock[Index].ClkReq));
  }
  DEBUG ((DEBUG_INFO, " SkipExtGfxScan: %d\n", PcieRpPreMemConfig->SkipExtGfxScan));
}

/**
  Print whole PCH_POLICY_PPI and serial out.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance

**/
VOID
EFIAPI
PtlPcdPreMemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                      Status;
  PCH_GENERAL_PREMEM_CONFIG       *PchGeneralPreMemConfig;
  PCH_LPC_PREMEM_CONFIG           *LpcPreMemConfig;
  PCH_HSIO_PCIE_PREMEM_CONFIG     *HsioPciePreMemConfig;
  PCH_PCIE_RP_PREMEM_CONFIG       *PcieRpPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gLpcPreMemConfigGuid, (VOID *) &LpcPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHsioPciePreMemConfigGuid, (VOID *) &HsioPciePreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print PreMemPolicy Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPreMemPolicyPpi->TableHeader.Header.Revision));

  PchPrintGeneralPreMemConfig (PchGeneralPreMemConfig);
  PchPrintLpcPreMemConfig (LpcPreMemConfig);
  PchPrintHsioPciePreMemConfig (HsioPciePreMemConfig);
  PchPrintPcieRpPreMemConfig (PcieRpPreMemConfig);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print PreMemPolicy End --------------------------\n"));
  DEBUG_CODE_END ();
}
