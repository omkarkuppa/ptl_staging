/** @file
  Source code file for PEI Silicon Pre Memory Policy Print function.

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

#include <Library/PeiServicesLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/PeiMePolicyLib.h>
#include <Library/PeiMemPolicyLib.h>
#include <Library/PeiIpuPolicyPrivateLib.h>
#include <Library/IGpuPeiPolicyLib.h>
#include <Library/PeiTcssPolicyLib.h>
#include <Library/PeiVtdPolicyLib.h>
#include <Library/PeiTelemetryPolicyLib.h>
#include <Library/PeiHostBridgePolicyLib.h>
#include <Library/PeiDciPolicyLib.h>
#include <Library/PeiHdaPolicyLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PeiIshPolicyLib.h>
#include <Library/PeiPsfPolicyLib.h>
#include <Library/PeiTraceHubPolicyLib.h>
#include <Library/PeiPchDmiPolicyLib.h>
#include <Library/PeiSmbusPolicyLib.h>
#include <Library/PeiWdtPolicyLib.h>
#include <Library/PeiOcPolicyLib.h>
#include <Library/PeiHybridGraphicsPolicyLib.h>
#include <Library/PeiCnviPolicyLib.h>
#include <Library/CpuInitPolicyLib.h>
#include <Library/BootGuardPolicyLib.h>
#include <Library/PeiVrPolicyLib.h>
#include <Library/PeiSpiPolicyLib.h>
#include <Library/CpuPowerManagementPolicyLib.h>
#include <Library/FspVPolicyLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/MemorySubSystemPolicyLib.h>

/**
  PPI function to print Silicon PreMem Policy settings.

  @retval EFI_STATUS       - Locate SiPreMemPolicy PPI successfully.
  @retval others           - Locate SiPreMemPolicy PPI fail.
**/
EFI_STATUS
EFIAPI
PeiPreMemSiPolicyPrint (
  VOID
  )
{
  EFI_STATUS                Status;
  SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi;
  CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig;

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuPowerDeliveryConfigGuid, (VOID *) &CpuPowerDeliveryConfig);
  ASSERT_EFI_ERROR (Status);

  if (Status == EFI_SUCCESS) {
    //
    // Print SI PreMem config block
    //
    SiPreMemPrintPolicyPpi (SiPreMemPolicyPpi);
    //
    // Print PCH PreMem config blocks
    //
    PtlPcdPreMemPrintPolicyPpi (SiPreMemPolicyPpi);
    //
    // Print SPI PreMem config blocks
    //
    SpiPrintConfig ((SI_POLICY_PPI *) SiPreMemPolicyPpi);
    //
    // Print ME PreMem config blocks
    //
    MePrintPolicyPpiPreMem (SiPreMemPolicyPpi);
    //
    // Print Memory SubSystem PreMem config blocks
    //
    MemorySubSystemPrintPolicyPpi (SiPreMemPolicyPpi);
    //
    // Print Mrc PreMem config blocks
    //
    MrcPrintPolicyPpiPreMem (SiPreMemPolicyPpi);
    //
    //
    // Print IPU PreMem config blocks
    //
    IpuPrintPolicyPpiPreMem (SiPreMemPolicyPpi);
    IGpuPrintPolicyPpiPreMem (SiPreMemPolicyPpi);
    TcssPrintConfigPreMem (SiPreMemPolicyPpi);
    VtdPrintPolicyPpi (SiPreMemPolicyPpi);
    TelemetryPrintPeiPolicyPpiPreMem (SiPreMemPolicyPpi);
    HostBridgePreMemPrintConfig (SiPreMemPolicyPpi);
    //
    // Print CPU PreMem config blocks
    //
    CpuInitPreMemPrintPolicy (SiPreMemPolicyPpi);
    CpuPowerMgmtVrConfigPrint (SiPreMemPolicyPpi);
    CpuPowerDeliveryConfigPrint (CpuPowerDeliveryConfig);
    BootGuardPrintPolicy (SiPreMemPolicyPpi);
    IpuPrintPolicyPpiPreMem (SiPreMemPolicyPpi);
    DciPreMemPrintConfig (SiPreMemPolicyPpi);
    HdaPreMemPrintConfig (SiPreMemPolicyPpi);
    IshPreMemPrintConfig (SiPreMemPolicyPpi);
    PchPsfPrintConfig (SiPreMemPolicyPpi);
    CnviPreMemPrintConfig (SiPreMemPolicyPpi);
    SocTraceHubPreMemPrintConfig (SiPreMemPolicyPpi);
    SmbusPreMemPrintConfig (SiPreMemPolicyPpi);
    WdtPreMemPrintConfig (SiPreMemPolicyPpi);
    FspVPreMemPrintConfig (SiPreMemPolicyPpi);
  #if FixedPcdGetBool(PcdOverclockEnable) == 1
    OcPreMemPrintConfig (SiPreMemPolicyPpi);
  #endif
  }

  return Status;
}