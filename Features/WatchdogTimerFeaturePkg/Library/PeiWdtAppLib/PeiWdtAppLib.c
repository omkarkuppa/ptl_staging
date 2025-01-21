/** @file
  Platform-specific Wdt code. Offers more convenient and foolproof API
  for manipulating WDT than low-level WDT driver which is part of Reference
  Code. WdtApp is too platform-specific to be included in reference code.
  However, it is expected that each client will need to write something
  similar on his own.
  This implementation simply kicks Wdt once before MRC execution starts,
  with such timeout value that allows BIOS to arrive in DXE phase where
  the Dxe part of this driver takes control of Wdt. There are other
  possibilities. For example, a more robust way would be to kick WDT more
  often, after each PEI module is loaded, and use shorter timeout values.

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

#include <Library/PeiWdtAppLib.h>

/**
  Turns on WDT during PEI phase according to requests made by
  OS overclocking application (through WDT status) and BIOS modules (through flash variable)

  @param[in]  WdtPei

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
PeiWdtApp (
  IN WDT_PPI    *WdtPei
  )
{
  EFI_STATUS                      Status;
  EFI_GUID                        EfiPeiReadOnlyVariable2PpiGuid = EFI_PEI_READ_ONLY_VARIABLE2_PPI_GUID;
  WDT_PERSISTENT_DATA             WdtStateData;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *ReadOnlyVariable;
  UINTN                           VariableSize;

  DEBUG ((DEBUG_INFO, "(PeiWdtAppLib) %a Start\n", __FUNCTION__));

  if (WdtPei == NULL) {
    DEBUG ((DEBUG_ERROR, "(PeiWdtApp) Wdt PPI was uninstalled. Disabling PeiWdtApp\n"));
    return EFI_SUCCESS;
  }

  Status = PeiServicesLocatePpi (
             &EfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &ReadOnlyVariable
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (WdtStateData);

  Status = ReadOnlyVariable->GetVariable (
                               ReadOnlyVariable,
                               WDT_PERSISTENT_DATA_C_NAME,
                               &gWdtPersistentDataGuid,
                               NULL,
                               &VariableSize,
                               &WdtStateData
                               );

  if (EFI_ERROR (Status)) {
    WdtStateData.Enable = 0;
  }

  if ((WdtPei->IsWdtRequired() == TRUE) || (WdtPei->IsWdtEnabled() == TRUE) || (WdtStateData.Enable == 1)) {
    WdtPei->ReloadAndStart (PcdGet8 (PcdWdtTimeoutBetweenPeiDxe));
  } else {
    WdtPei->Disable ();
  }

  DEBUG ((DEBUG_INFO, "(PeiWdtAppLib) %a End\n", __FUNCTION__));

  return EFI_SUCCESS;
}
