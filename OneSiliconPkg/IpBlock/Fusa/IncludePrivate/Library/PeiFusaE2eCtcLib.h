/** @file
  Header file for Fusa E2e Ctc Lib functions.

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
#ifndef _FUSA_E2E_CTC_LIB_H_
#define _FUSA_E2E_CTC_LIB_H_

#include <PiPei.h>
#include "FusaInfoHob.h"

/** status code return from internal routines*/
typedef enum
{
  FusaNoError = 0U,                 /**<No error*/
  FusaDeviceError = 0x80000001,     /**<Device configuration issue*/
  FusaInvalidParam = 0x80000002,    /**<Invalid input parameter such as input pointer being NULL or failing range check*/
  FusaInternalError = 0x80000004    /**<Error caused by internal implementation or defensive programming path taken*/
} FUSA_LIB_STATUS;

/**
  Calculate CRC32 value of a buffer.

  @param[in] *pBuffer   - input buffer for the CRC32 calculation
  @param[in] Len        - length of the buffer to be
        CRC32-calculated
  @param[in] InitVal    - init value use for the CRC32
        calculation

  @retval calculated CRC32 value
**/
UINT32
AsmCrc32Calc (
  IN UINT8 *pBuffer,
  IN UINT32 Len,
  IN UINT32 InitVal
  );

/**
  Init and Install Fusa Info Hob

  @retval EFI_OUT_OF_RESOURCES if the HOB resource allocation
          fails
  @retval EFI_SUCCESS otherwise
**/
EFI_STATUS
InstallFusaInfoHob (
  VOID
  ) ;

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
  );

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
  IN  CONST EFI_PEI_SERVICES  **PeiServices,
  IN  UINT32 FusaStartupFileAddr
  );

VOID
FspDxCheckEndofPei (
  IN  CONST EFI_PEI_SERVICES  **PeiServices
  );

#endif // _FUSA_E2E_CTC_LIB_H_
