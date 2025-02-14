/** @file
  Library for installing BT NHLT UEFI variable

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Uefi.h>
#include <Library/InstallBtConfigurationVariable.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Nhlt.h>

/**
  Install NHLT BT UEFI variable
  @param[in]                 NhltBtActiveConfig   Value of active BT configuration
  @retval EFI_SUCCESS        The function completed successfully
  @retval Others             Failed to install UEFI variable
**/
EFI_STATUS
InstallNhltBtVariable (
  IN UINT8                   NhltBtConfigurationValue
  )
{
  NHLT_BT_CONFIGURATION_VARIABLE  NhltBtVar;
  NHLT_BT_CONFIGURATION_VARIABLE  NhltBtVarGlobal;
  UINT32                          NhltBtVarAttr;
  EFI_STATUS                      Status;
  UINTN                           VariableSize;

  VariableSize  = sizeof (NHLT_BT_CONFIGURATION_VARIABLE);
  ZeroMem (&NhltBtVarGlobal, sizeof (NhltBtVarGlobal));

  NhltBtVar.Revision           = NHLT_BT_CONFIGURATION_VARIABLE_REVISION;
  NhltBtVar.NhltBtActiveConfig = NhltBtConfigurationValue;

  Status = gRT->GetVariable (
                NHLT_BT_CONFIGURATION_VARIABLE_NAME,
                &gNhltBtConfigurationVariableGuid,
                &NhltBtVarAttr,
                &VariableSize,
                &NhltBtVarGlobal
                );

  if (Status == EFI_NOT_FOUND || (NhltBtVar.NhltBtActiveConfig != NhltBtVarGlobal.NhltBtActiveConfig)) {
    NhltBtVarAttr = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
    Status = gRT->SetVariable (
                    NHLT_BT_CONFIGURATION_VARIABLE_NAME,
                    &gNhltBtConfigurationVariableGuid,
                    NhltBtVarAttr,
                    VariableSize,
                    &NhltBtVar
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a () End. Error when installing/updating BT NHLT variable. Status: %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  return EFI_SUCCESS;
}
