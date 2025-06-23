/** @file
  This file Implement the public interface for  Fusa
   E2E CTC implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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


/**
  Check if the system is under Fusa diagnostic mode

  @note Bootloader does not rely on this routine for the
        diagnostic mode detection but the existence of the
        gSiFusaInfoGuid HOB

  @retval TRUE if system is under Fusa diagnostic mode
  @retval FALSE otherwise

**/
BOOLEAN
FspDxDiagnosticModeGet (
  VOID
  )
{
  return FALSE;
}

/**
  Init and Install Fusa Info Hob

  @retval EFI_OUT_OF_RESOURCES if the HOB resource allocation
          fails
  @retval EFI_SUCCESS otherwise
**/
EFI_STATUS
InstallFusaInfoHob (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Main check the checker routine that will perform all the check
  the checker test (exclude MemSS CTC) and update the test
  result HOB.

  If systematic fault exists in such the MpServices PPI was not
  installed, the test result HOB is still being produced with
  zero content from this test, which the bootloader stage can
  detect the issue.

  @note This routine shall be called before BIOS_DONE or CPU
        PRMRR setup, or another word, prior to CpuInit(). Some
        of the test are using memory reference and thus it shall
        be called after system memory is avaialable.

  @pre  MpService PPI was installed

  @param[in] PeiServices    Pointer to PEI Services Table

**/
VOID
FspDxCheck (
  IN  CONST EFI_PEI_SERVICES  **PeiServices
  )
{
}

