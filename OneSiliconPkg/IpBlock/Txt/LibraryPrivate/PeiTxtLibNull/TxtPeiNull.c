/** @file
  This is the main PEIM file for TXT. It represents an abstract outline of the
  steps required during PEI for enabling TXT. Each individual step is further
  abstracted behind a function call interface. This is intended to minimize
  the need to modify this file when porting TXT to future platforms.

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

@par Specification
**/
#include <Uefi.h>
#include <DprInfoHob.h>


/**
  TXT init after memory PEI module

  @retval EFI_SUCCESS  - always return EFI_SUCCESS
**/
EFI_STATUS
TxtInit (
  VOID
  );

/**
  AliasCheck for MRC

  @retval EFI_SUCCESS  - always return EFI_SUCCESS
**/

EFI_STATUS
EFIAPI
AliasCheck (
  VOID
  );

/**
  This is ClearSecretsBit entry point for MRC call.
**/
EFI_STATUS
EFIAPI
ClearSecretsBit (
  VOID
  );

/**
  Determines whether or not SECRETS.STS bit is set in E2STS status register
**/
BOOLEAN
IsTxtSecretsSet (
  VOID
  );

/**
 TXT init after memory PEI module

 @retval EFI_SUCCESS  - always return EFI_SUCCESS
**/
EFI_STATUS
TxtInit (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  AliasCheck for MRC
**/

EFI_STATUS
EFIAPI
AliasCheck (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This is ClearSecretsBit entry point for MRC call.
**/
EFI_STATUS
EFIAPI
ClearSecretsBit (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Set Power Down Request.

  @param[in] PdrSrc             Power down type
                            0 - No Power Down Request
                            1 - MRC requested Power Down due to scrubbing failure
                            2 - BIOS setup requested power down
                            3 - Power down through MOR interface

  @exception EFI_UNSUPPORTED  - If the platform is not TXT capable.
  @retval EFI_SUCCESS         - In all other cases not listed above.
**/
EFI_STATUS
EFIAPI
SetPowerDownRequest (
  UINT8 PdrSrc
  )
{
  return EFI_SUCCESS;
}

/**
  Determines whether or not SECRETS.STS bit is set in E2STS status register
**/
BOOLEAN
IsTxtSecretsSet (
  VOID
  )
{
  return FALSE;
}

/**
  This routine provides alias check request status.
  @retval TRUE - If acheck is requested, otherwise FALSE
**/
BOOLEAN
IsTxtACheckRequested (
  VOID
  )
{
  return FALSE;
}

/**
  Determines whether or not the current processor is TXT Capable.

  @retval TRUE          - If the current processor supports TXT
  @retval FALSE         - If the current processor does not support TXT
**/
BOOLEAN
IsTxtProcessor (
  VOID
  )
{
  return FALSE;
}