/** @file
 Intel PEI SA Policy update by board configuration

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

/**
  This function performs PEI SI Policy update by board configuration.

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSiPolicyBoardConfig (
  VOID
  )
{
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID        *FspsUpd;
  EFI_STATUS  Status;
  VOID        *MpServices2Ppi;

  FspsUpd = NULL;

  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);

  //
  // Bootloader passes the CpuMpPpi pointer to FSP in API mode.
  //
  Status = PeiServicesLocatePpi (&gEdkiiPeiMpServices2PpiGuid, 0, NULL, &MpServices2Ppi);
  if (Status != EFI_NOT_FOUND){
    DEBUG ((DEBUG_INFO, "PeiCpuPolicyBoardConfig: MpServices2 = %p\n", MpServices2Ppi));
    ASSERT_EFI_ERROR(Status);
    ((FSPS_UPD *) FspsUpd)->FspsConfig.CpuMpPpi = (UINTN) MpServices2Ppi;
  }
#endif
  return EFI_SUCCESS;
}
