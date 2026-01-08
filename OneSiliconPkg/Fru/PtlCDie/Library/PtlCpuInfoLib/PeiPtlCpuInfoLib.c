/** @file
  Information library for PantherLake Cpu.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include <PiPei.h>
#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/IntelDieInfo.h>

/**
  This function installs instance of IntelDieInfoPpi.

  @param[in] Type           GUID specifying type of the die for which PPI is being installed
  @param[in] DieIndex       Index of the die
  @param[in] DieId          Die specific identifier
  @param[in] SteppingId     Stepping ID of the die
  @param[in] GetNameStr     Pointer to the function that returns name string
  @param[in] GetSteppingStr Pointer to the function that returns stepping string
  @param[in] GetSkuStr      Pointer to the function that returns SKU string

  @retval EFI_SUCCESS if instance has been installed successfully
**/
EFI_STATUS
CpuInstallDieInfoInternal (
  IN  EFI_GUID                         Type,
  IN  UINT32                           DieIndex,
  IN  UINT64                           DieId,
  IN  UINT32                           SteppingId,
  IN  EDKII_INTEL_DIE_INFO_GET_DIE_NAME_STR  GetNameStr,
  IN  EDKII_INTEL_DIE_INFO_GET_STEPPING_STR  GetSteppingStr,
  IN  EDKII_INTEL_DIE_INFO_GET_SKU_STR       GetSkuStr
  )
{
  EFI_PEI_PPI_DESCRIPTOR   *DieInfoPpiDesc;
  EDKII_INTEL_DIE_INFO_PPI *DieInfoPtr;
  EFI_STATUS               Status;

  DieInfoPtr = (EDKII_INTEL_DIE_INFO_PPI *) AllocateZeroPool (sizeof (EDKII_INTEL_DIE_INFO_PPI));
  if (DieInfoPtr == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  DieInfoPtr->Revision = DIE_INFO_PROTOCOL_REVISION;
  DieInfoPtr->Type = Type;
  DieInfoPtr->DieIndex = DieIndex;
  DieInfoPtr->DieId = DieId;
  DieInfoPtr->SteppingId = SteppingId;
  DieInfoPtr->GetNameStr = GetNameStr;
  DieInfoPtr->GetSteppingStr = GetSteppingStr;
  DieInfoPtr->GetSkuStr = GetSkuStr;

  DieInfoPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (DieInfoPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  DieInfoPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  DieInfoPpiDesc->Guid  = &gIntelDieInfoPpiGuid;
  DieInfoPpiDesc->Ppi   = DieInfoPtr;

  Status = PeiServicesInstallPpi (DieInfoPpiDesc);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}
