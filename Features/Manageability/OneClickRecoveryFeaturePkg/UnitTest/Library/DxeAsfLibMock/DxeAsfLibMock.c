/** @file
  Mock DXE ASF Library for One Click Recovery Unit Testing.

  Provides controllable stub implementations of DxeAsfLib functions used by
  the WifiProfileSync production code under test.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2026 Intel Corporation.

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
#include <Library/DebugLib.h>

///
/// Controllable state used by test cases to drive AsfIsBootOptionsPresent() behavior.
/// Set to TRUE  to simulate an AMT boot option present in FWSTS1 bit 24.
/// Set to FALSE to simulate no AMT boot option (the common non-OCR boot path).
///
BOOLEAN  mAmtBootOptionsPresent = FALSE;

/**
  Check if ASF boot options is present (FWSTS1 bit 24 - MeBootOptionsPresent).

  This mock returns the value of the controllable global mAmtBootOptionsPresent.
  Test cases must set this global before calling any function that exercises
  IsProfileSyncSupported().

  @retval TRUE    A mock AMT boot option is present.
  @retval FALSE   No mock AMT boot option is present.
**/
BOOLEAN
AsfIsBootOptionsPresent (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[%a] Mock returning %a\n", __FUNCTION__,
    mAmtBootOptionsPresent ? "TRUE" : "FALSE"));
  return mAmtBootOptionsPresent;
}
