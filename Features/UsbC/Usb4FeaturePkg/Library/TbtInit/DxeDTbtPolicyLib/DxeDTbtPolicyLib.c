/** @file
  This file is DxeDTbtPolicyLib library.

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

@par Specification
**/

#include <DxeDTbtPolicyLibrary.h>
#include <Protocol/DxeDTbtPolicy.h>
#include <Library/PcdLib.h>

/**
  Print DXE DTBT Policy
**/
VOID
DTbtPrintDxePolicyConfig (
  VOID
  )
{
  EFI_STATUS                       Status;
  UINT8                            Index;
  DXE_DTBT_POLICY_PROTOCOL         *DxeDTbtConfig;

  DEBUG ((DEBUG_INFO, "DTbtPrintDxePolicyConfig Start\n"));

  DxeDTbtConfig = NULL;
  Status = EFI_NOT_FOUND;
  Status = gBS->LocateProtocol (
                &gDxeDTbtPolicyProtocolGuid,
                NULL,
                (VOID **) &DxeDTbtConfig
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, " gDxeDTbtPolicyProtocolGuid Not installed!!!\n"));
    return;
  }
  //
  // Print DTBT Policy
  //
  DEBUG ((DEBUG_ERROR, " ========================= DXE DTBT POLICY ========================= \n"));
  if (DxeDTbtConfig != NULL) {
    for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++) {
      DEBUG ((DEBUG_INFO, "DxeDTbtConfig->DTbtResourceConfig[%x].DTbtPcieExtraBusRsvd = %x\n", Index, DxeDTbtConfig->DTbtResourceConfig[Index].TbtPcieExtraBusRsvd));
      DEBUG ((DEBUG_INFO, "DxeDTbtConfig->DTbtResourceConfig[%x].DTbtPcieMemRsvd = %x\n", Index, DxeDTbtConfig->DTbtResourceConfig[Index].TbtPcieMemRsvd));
      DEBUG ((DEBUG_INFO, "DxeDTbtConfig->DTbtResourceConfig[%x].DTbtPcieMemAddrRngMax = %x\n", Index, DxeDTbtConfig->DTbtResourceConfig[Index].TbtPcieMemAddrRngMax));
      DEBUG ((DEBUG_INFO, "DxeDTbtConfig->DTbtResourceConfig[%x].DTbtPciePMemRsvd = %x\n", Index, DxeDTbtConfig->DTbtResourceConfig[Index].TbtPciePMemRsvd));
      DEBUG ((DEBUG_INFO, "DxeDTbtConfig->DTbtResourceConfig[%x].DTbtPciePMemAddrRngMax = %x\n", Index, DxeDTbtConfig->DTbtResourceConfig[Index].TbtPciePMemAddrRngMax));
    }
    DEBUG ((DEBUG_INFO, "DxeDTbtConfig->TbtCommonConfig.TbtVtdBaseSecurity = %x\n", DxeDTbtConfig->TbtCommonConfig.TbtVtdBaseSecurity));
    DEBUG ((DEBUG_INFO, "DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3 = %x\n", DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3));
    DEBUG ((DEBUG_INFO, "DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3OffDelay = %x\n", DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3OffDelay));
    DEBUG ((DEBUG_INFO, "DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3ClkReq = %x\n", DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3ClkReq));
    DEBUG ((DEBUG_INFO, "DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3ClkReqDelay = %x\n", DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3ClkReqDelay));
  }
  return;
}

/**
  DTbtInstallPolicyProtocol installs DTBT Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
DTbtInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle
  )
{
  EFI_STATUS                    Status;
  DXE_DTBT_POLICY_PROTOCOL      *DxeTbtPolicy;

  DEBUG ((DEBUG_INFO, "Install DXE DTBT Policy\n"));

  DxeTbtPolicy = NULL;
  //Alloc memory for DxeTbtPolicy
  DxeTbtPolicy = (DXE_DTBT_POLICY_PROTOCOL *) AllocateZeroPool (sizeof (DXE_DTBT_POLICY_PROTOCOL));
  if (DxeTbtPolicy == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gDxeDTbtPolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  DxeTbtPolicy
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Install DXE DTBT Policy failed\n"));
  }
  return Status;
}
