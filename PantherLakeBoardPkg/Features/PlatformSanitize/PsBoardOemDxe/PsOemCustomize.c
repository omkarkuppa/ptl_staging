/** @file
  OEM Specific sanitize services.
  OEM can have their customized erase actions in this file.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include "PsBoardOemDxe.h"

/**
  This function invoked when "PS_OEM_CUSTOM_ACTION" corresponding to Bitmask 16 is triggered to perform
  OEM customized sanitize action.

  Note: Intel RVP-BIOS returns simply with EFI_SUCCESS.
        OEM can modify this function as per their need.

  @param[in] *This                - A pointer to the PS_BOARD_OEM_PROTOCOL instance
  @param[in] *PsOemHookTlvData    - A pointer to OEM TLV Data

  @retval EFI_SUCCESS           - The operation succeeds.
  @retval Others                - Other status errors
**/
EFI_STATUS
EFIAPI
PsOemCustomActionBitMask16 (
  IN PS_BOARD_OEM_PROTOCOL   *This,
  IN PS_OEM_HOOK_TLV_DATA    *PsOemHookTlvData
  )
{
  UINT32   Index;

  DEBUG ((DEBUG_INFO, "PS: OEM Custom Action.\n"));

  // OEM specific Sanitize Implementation.

  // ------ Dump OEM TLV Data for verification -------
  DEBUG ((DEBUG_INFO, "OEM Vendor ID         : 0x%X\n", PsOemHookTlvData->OemVid));
  DEBUG ((DEBUG_INFO, "OEM Parameter Type ID : 0x%X\n", PsOemHookTlvData->OemParameterTypeId));
  DEBUG ((DEBUG_INFO, "OEM OemDataLength     : %d bytes\n", PsOemHookTlvData->OemDataLength));
  DEBUG ((DEBUG_INFO, "OEM TLV Data:\n"));
  for (Index = 0; Index < PsOemHookTlvData->OemDataLength; Index++) {
    if (Index % 16 == 0) {
      DEBUG ((DEBUG_INFO, "\n%08x:", Index));
    } else if (Index % 8 == 0) {
      DEBUG ((DEBUG_INFO, " - "));
    }
    DEBUG ((DEBUG_INFO, "%02X", PsOemHookTlvData->OemData[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  // --------------------------------

  return EFI_SUCCESS;
}
