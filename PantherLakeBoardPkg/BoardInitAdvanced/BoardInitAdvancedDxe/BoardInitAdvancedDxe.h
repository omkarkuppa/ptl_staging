/** @file
  Header file for Board Init Advanced Dxe Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#ifndef _BOARD_INIT_ADVANCED_DXE_H_
#define _BOARD_INIT_ADVANCED_DXE_H_

#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/HobLib.h>
#include <SetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/BootGuardRevocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Uefi/UefiBaseType.h>
#include <OemSetup.h>
#include <MemInfoHob.h>
#include <Library/PmcLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/PmcRegs.h>
#if FixedPcdGetBool(PcdSinitAcmBinEnable) == 1
#include <TxtInfoHob.h>
#include <Txt.h>
#include <Protocol/FirmwareVolume2.h>
#endif

//----------------------------------------------------------------------------
// VT-d Engine Configuration Register Details
//----------------------------------------------------------------------------
#define SA_VTD_RMRR_USB_LENGTH  0x20000

VOID
SaPlatformInitDxe (
  VOID
  );

#if FixedPcdGetBool (PcdSinitAcmBinEnable) == 1
/**

  This function looks for SINIT ACM in FVs and updates TXT HOB
  with SINIT ACM Base and Size.

  @retval EFI_SUCCESS     - SINIT ACM found and copied.
  @retval EFI_NOT_FOUND   - If TxtInfoHob is not found
  @retval EFI_UNSUPPORTED - TXT Device memory not available.

**/
EFI_STATUS
TxtSinitAcmLoad (
  VOID
  );
#endif

#endif
