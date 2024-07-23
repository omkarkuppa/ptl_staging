/** @file
  Modular USBC IO DXE driver

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

/**
  EP function for Modular USBC IO DXE driver

  @param[in]  ImageHandle  Not used.
  @param[in]  SystemTable  Global system service table.

  @retval     EFI_SUCCESS  This EP always returns EFI_SUCCESS if gTcssStrapDataHobGuid is
                           not available. It will ASSERT on errors during variable update.
**/
EFI_STATUS
EFIAPI
ModularUsbCIoDxeEntryPoint (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  ) {
  EFI_STATUS            Status;
  VOID                  *Hob;
  UINT32                CurrentTcssStrapConfig;

  Status = EFI_SUCCESS;
  Hob    = GetFirstGuidHob (&gTcssStrapDataHobGuid);

  if (Hob != NULL) {
    CurrentTcssStrapConfig = *(UINT32 *) GET_GUID_HOB_DATA (Hob);
    Status = gRT->SetVariable (
                    L"TcssStrapData",
                    &gTcssStrapDataVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    sizeof (UINT32),
                    &CurrentTcssStrapConfig
                    );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, "[TCSS] Update CurrentTcssStrapConfig to 0x%08x\n", CurrentTcssStrapConfig));
  }

  return Status;
}
