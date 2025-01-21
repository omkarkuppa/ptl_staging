/** @file
 Intel PEI PCH Pre-Memory Policy update by board configuration

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

#include "PeiPolicyBoardConfig.h"
#include <PchPcieRpConfig.h>

VOID
UpdatePcieClockInfo (
  PCH_PCIE_RP_PREMEM_CONFIG  *PcieRpPreMemConfig,
  UINTN                      Index,
  UINT64                     Data
  )
{
  PCD64_BLOB Pcd64;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID       *FspmUpd;
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#endif

  Pcd64.Blob = Data;
  DEBUG ((DEBUG_INFO, "UpdatePcieClockInfo ClkIndex %x ClkUsage %x, Supported %x\n", Index, Pcd64.PcieClock.ClockUsage, Pcd64.PcieClock.ClkReqSupported));

  UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.PcieClkSrcUsage[Index], PcieRpPreMemConfig->PcieClock[Index].Usage, (UINT8)Pcd64.PcieClock.ClockUsage);
  UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.PcieClkSrcClkReq[Index], PcieRpPreMemConfig->PcieClock[Index].ClkReq, Pcd64.PcieClock.ClkReqSupported ? (UINT8)Index : 0xFF);
}

/**
  This function performs PEI PCH Pre-Memory Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyBoardConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                         Status;
  PCH_PCIE_RP_PREMEM_CONFIG          *PcieRpPreMemConfig;

  DEBUG ((DEBUG_INFO, "Updating PCH Policy by board config in Pre-Mem\n"));

  Status = EFI_SUCCESS;
  PcieRpPreMemConfig = NULL;

#if FixedPcdGet8(PcdFspModeSelection) == 0
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  UpdatePcieClockInfo (PcieRpPreMemConfig, 0, PcdGet64  (PcdPcieClock0));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 1, PcdGet64  (PcdPcieClock1));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 2, PcdGet64  (PcdPcieClock2));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 3, PcdGet64  (PcdPcieClock3));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 4, PcdGet64  (PcdPcieClock4));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 5, PcdGet64  (PcdPcieClock5));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 6, PcdGet64  (PcdPcieClock6));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 7, PcdGet64  (PcdPcieClock7));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 8, PcdGet64  (PcdPcieClock8));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 9, PcdGet64  (PcdPcieClock9));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 10, PcdGet64 (PcdPcieClock10));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 11, PcdGet64 (PcdPcieClock11));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 12, PcdGet64 (PcdPcieClock12));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 13, PcdGet64 (PcdPcieClock13));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 14, PcdGet64 (PcdPcieClock14));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 15, PcdGet64 (PcdPcieClock15));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 16, PcdGet64 (PcdPcieClock16));
  UpdatePcieClockInfo (PcieRpPreMemConfig, 17, PcdGet64 (PcdPcieClock17));

  return Status;
}

