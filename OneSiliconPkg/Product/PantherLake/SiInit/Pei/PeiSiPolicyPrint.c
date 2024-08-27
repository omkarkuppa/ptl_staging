/** @file
  Source code file for PEI Silicon Policy Print function.

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
#include <Library/PcdLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/PeiRtcPolicyLib.h>
#include <Library/PeiThermalPolicyLib.h>
#include <Library/PeiCnviPolicyLib.h>
#include <Library/PeiEspiPolicyLib.h>
#include <Library/PeiGbePolicyLib.h>
#include <Library/PeiTsnPolicyLib.h>
#include <Library/PeiHdaPolicyLib.h>
#include <Library/PeiIehPolicyLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PeiIshPolicyLib.h>
#include <Library/PeiItssPolicyCommonLib.h>
#include <Library/PeiItssPolicyLib.h>
#include <Library/PeiP2sbPolicyLib.h>
#include <Library/PeiPchDmiPolicyLib.h>
#include <Library/PeiPmcPolicyLib.h>
#include <Library/PeiScsPolicyLib.h>
#include <Library/PeiUsbPolicyLib.h>
#include <Library/PeiThcPolicyLib.h>
#include <Library/PeiFusaPolicyLib.h>
#include <Library/PeiAmtPolicyLib.h>
#include <Library/PeiMePolicyLib.h>
#include <Library/IGpuPeiPolicyLib.h>
#include <Library/PeiTelemetryPolicyLib.h>
#include <Library/PeiHostBridgePolicyLib.h>
#include <Library/PeiNpuPolicyLib.h>
#include <Library/PeiVmdPolicyLib.h>
#include <Library/PeiIaxPolicyLib.h>
#include <Library/CpuPowerManagementPolicyLib.h>
#include <Library/PeiTcssPolicyLib.h>
#include <Library/PeiITbtPolicyLib.h>
#include <Library/PeiVrPolicyLib.h>
#include <Library/CpuInitPolicyLib.h>
#include <Library/PeiBiosGuardPolicyLib.h>
#include <Library/FspVPolicyLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiLpssI3cPolicyLib.h>
#include <Library/PeiLpssI2cPolicyLib.h>
#include <Library/PeiLpssSpiPolicyLib.h>
#include <Library/PeiLpssUartPolicyLib.h>

/**
  PPI function to print Silicon Policy config block settings

  @retval EFI_STATUS       - Locate SiPolicy PPI successfully.
  @retval others           - Locate SiPolicy PPI fail.
 **/
EFI_STATUS
EFIAPI
PeiSiPolicyPrint (
  VOID
  )
{
  EFI_STATUS    Status;
  SI_POLICY_PPI *SiPolicyPpi;

  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  if (Status == EFI_SUCCESS) {
    //
    // Print SI config blocks and serial out.
    //
    SiPrintPolicyPpi (SiPolicyPpi);

    //
    // Print PCH config blocks and serial out.
    //
    PtlPcdPrintPolicyPpi (SiPolicyPpi);
    //
    // Print IpBlocks config.
    //
    ThermalPrintConfig (SiPolicyPpi);
    RtcPrintConfig (SiPolicyPpi);
    CnviPrintConfig (SiPolicyPpi);
    GbePrintConfig (SiPolicyPpi);
    TsnPrintConfig (SiPolicyPpi);
    HdaPrintConfig (SiPolicyPpi);
    IshPrintConfig (SiPolicyPpi);
    EspiPrintConfig (SiPolicyPpi);
    IehPrintConfig (SiPolicyPpi);
    ItssPrintConfig (SiPolicyPpi);
    P2sbPrintConfig (SiPolicyPpi);
    ScsPrintConfig (SiPolicyPpi);
    UsbPrintConfig (SiPolicyPpi);
    ThcPrintConfig (SiPolicyPpi);
    PmcPrintConfig (SiPolicyPpi);
    FusaPrintConfig (SiPolicyPpi);
    //
    // Print AMT config blocks and serial out.
    //
    AmtPrintPolicyPpi (SiPolicyPpi);
    //
    // Print ME config blocks and serial out.
    //
    MePrintPolicyPpi (SiPolicyPpi);
    //
    // Print SA IpBlocks config.
    //
    IGpuPrintPolicyPpi (SiPolicyPpi);
    VmdPrintPeiPolicyPpi (SiPolicyPpi);
    TelemetryPrintPeiPolicyPpi (SiPolicyPpi);
    HostBridgePeiPrintConfig (SiPolicyPpi);
    NpuPrintPolicyPpi (SiPolicyPpi);
    IaxPrintPolicyPpi (SiPolicyPpi);
    LpssI3cPrintConfig (SiPolicyPpi);
    LpssSpiPrintConfig (SiPolicyPpi);
    LpssUartPrintConfig (SiPolicyPpi);
    LpssI2cPrintConfig (SiPolicyPpi);

    //
    // Print CPU config block and serial out.
    //
    CpuInitPrintPolicy (SiPolicyPpi);
    CpuPowerManagementPrintPolicy (SiPolicyPpi);
    TcssPrintConfig (SiPolicyPpi);

    FspVPostMemPrintConfig (SiPolicyPpi);
#if FixedPcdGetBool (PcdTcssSupport) == 1
    ITbtPrintPolicy (SiPolicyPpi);
#endif

#if FixedPcdGetBool(PcdBiosGuardEnable) == 1
    //
    // Print BIOS Guard config block
    //
    BiosGuardConfigPrint (SiPolicyPpi);
#endif
  }

  return Status;
}
