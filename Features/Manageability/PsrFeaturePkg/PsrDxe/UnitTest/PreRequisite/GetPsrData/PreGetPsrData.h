/** @file
  PreRequisite functions of the GetPsrData function.

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

#include <Uefi.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Protocol/PsrProtocol.h>

/**
  Get PSR blob from CSME via HECI cmds.

  @param[in]      *This                  Pointer of PLATFORM_SERVICE_RECORD_PROTOCOL.
  @param[in]      PsrDataBlobSize        PSR Data Buffer Size.
  @param[in, out] *PsrDataBlob           Pointer of the PSR Data Blob.

  @retval         EFI_SUCCESS            Retrieved PSR from CSME successfully.
  @retval         Others                 Failed to retrieve the record.
**/
EFI_STATUS
EFIAPI
GetPsrData (
  IN     PLATFORM_SERVICE_RECORD_PROTOCOL *This,
  IN     UINTN                            PsrDataBlobSize,
  IN OUT UINT8                            *PsrDataBlob
  );

/**
  Set up services for GetPsrData function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupGetPsrDataEnvironment (
  VOID
  );

/**
  Set up the environment and execute GetPsrData function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreGetPsrData (
  IN UNIT_TEST_CONTEXT  Context
  );
