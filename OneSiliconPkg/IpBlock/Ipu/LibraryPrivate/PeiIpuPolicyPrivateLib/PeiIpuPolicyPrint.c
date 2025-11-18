/** @file
  This file provides services for IPU PEI policy default print function

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
#include <Ppi/SiPolicy.h>
#include <IpuPreMemConfig.h>

#include <Library/DebugLib.h>
#include <Library/PeiIpuPolicyPrivateLib.h>

/**
  This function prints the PEI phase PreMem policy for IPU.

  @param[in] SiPolicyPreMemPpi - Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
IpuPrintPolicyPpiPreMem (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                            Status;
  IPU_PREMEM_CONFIG                     *IpuPreMemPolicy;
  UINT8                                 Index;

  //
  // Get IPU Config Blocks which needs to be used here
  //
  IpuPreMemPolicy = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPreMemPpi, &gIpuPreMemConfigGuid, (VOID *) &IpuPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  if (IpuPreMemPolicy != NULL) {
    DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
    if (IpuPreMemPolicy->Header.Revision == IPU_PREMEM_CONFIG_REVISION) {
      DEBUG ((DEBUG_INFO, "Revision : %d\n", IpuPreMemPolicy->Header.Revision));
      DEBUG ((DEBUG_INFO, "IpuEnable : 0x%x\n", IpuPreMemPolicy->IpuEnable));
      for (Index = 0; Index < GPIO_IMGUCLK_NUMBER_OF_PINS; Index ++) {
        DEBUG ((DEBUG_INFO, "ImguClkOutEn[%x] : %x\n", Index, IpuPreMemPolicy->ImguClkOutEn[Index]));
      }
    } else {
      DEBUG ((DEBUG_INFO, "IPU Config Block Revision = %d \n", IpuPreMemPolicy->Header.Revision));
      DEBUG ((DEBUG_INFO, "IPU_PREMEM_CONFIG_REVISION = %d \n", IPU_PREMEM_CONFIG_REVISION));
      DEBUG ((DEBUG_INFO, "IPU Config Block Version doesn't match.\n"));
    }
    DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  }

  DEBUG_CODE_END ();
  return;
}
