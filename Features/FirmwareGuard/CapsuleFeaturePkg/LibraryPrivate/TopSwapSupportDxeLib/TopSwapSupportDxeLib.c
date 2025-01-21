/** @file
  DXE Top Swap support library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/TopSwapSupportLib.h>
#include <Protocol/TopSwapControlProtocol.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Register/EspiRegs.h>

/**
  Set/Clear TopSwap bit by request.

  @param[in] Enable  TRUE: Set TopSwap bit; FALSE: Clear TopSwap bit.

  @retval EFI_SUCCESS  Triggered TopSwap enable/disable SMI by request.

**/
EFI_STATUS
EFIAPI
TopSwapControl (
  IN BOOLEAN  Enable
  )
{
#if (FixedPcdGetBool (PcdTopSwapSmiSupport) == 0x1)
  ASSERT (PcdGet8 (PcdTopSwapEnableSwSmi) != 0xFF);
  ASSERT (PcdGet8 (PcdTopSwapDisableSwSmi) != 0xFF);

  if (Enable) {
    DEBUG ((DEBUG_INFO, "Enable TopSwap via SwSmi (0x%x)\n", PcdGet8 (PcdTopSwapEnableSwSmi)));
    IoWrite8 (R_PCH_IO_APM_CNT, PcdGet8 (PcdTopSwapEnableSwSmi));
  } else {
    DEBUG ((DEBUG_INFO, "Disable TopSwap via SwSmi (0x%x)\n", PcdGet8 (PcdTopSwapDisableSwSmi)));
    IoWrite8 (R_PCH_IO_APM_CNT, PcdGet8 (PcdTopSwapDisableSwSmi));
  }

  return EFI_SUCCESS;

#else
  TOP_SWAP_CONTROL_PROTOCOL        *TopSwapControl;
  EFI_STATUS                       Status;
  //
  // Locate TopSwapControl Protocol
  //
  Status = gBS->LocateProtocol (
                  &gTopSwapControlProtocolGuid,
                  NULL,
                  (VOID **) &TopSwapControl
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Error : Locate TopSwapControl Protocol Failed\n"));
    return Status;
  }

  return (TopSwapControl->SetTopSwapStatus (TopSwapControl, Enable));
#endif
}

/**
  Returns Top Swap functionality enable state

  @retval TRUE    Top Swap is enabled
  @retval FALSE   Top Swap is disabled
**/
BOOLEAN
EFIAPI
IsTopSwapEnabled (
  VOID
  )
{
  return SpiIsTopSwapEnabled ();
}
