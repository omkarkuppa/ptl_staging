/** @file
  Test case to validate microcode.

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

#include <Library/BaseLib.h>
#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include <Register/Intel/Cpuid.h>
#include <Library/FspVInfoLib.h>
#include <Library/FspVTestCaseLib.h>

/**
  Get microcode update signature of currently loaded microcode update.
  @return  Microcode signature.
**/
UINT32
GetCurrentMicrocodeSignature (
  VOID
  )
{
  MSR_IA32_BIOS_SIGN_ID_REGISTER  BiosSignIdMsr;

  AsmWriteMsr64 (MSR_IA32_BIOS_SIGN_ID, 0);
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, NULL, NULL);
  BiosSignIdMsr.Uint64 = AsmReadMsr64 (MSR_IA32_BIOS_SIGN_ID);
  return BiosSignIdMsr.Bits.MicrocodeUpdateSignature;
}

VOID
ValidateMicrocodeVersion (
  UINTN  MinimumExpectedMicrocodeVersion
  )
{
  UINTN    ActualMicrocodeVersion;
  BOOLEAN  IsTestPassed;

  IsTestPassed = TRUE;
  DEBUG ((DEBUG_INFO, "ValidateMicrocodeVersion Test\n"));

  MinimumExpectedMicrocodeVersion = 0x1; // Expected value to be passed as a parameter
  ActualMicrocodeVersion = GetCurrentMicrocodeSignature ();

  if (!(ActualMicrocodeVersion >= MinimumExpectedMicrocodeVersion)) {
    DEBUG ((DEBUG_INFO, "\n Test FAIL Microcode Revision@ [0x%08x] \n\n", ActualMicrocodeVersion));
    IsTestPassed = FALSE;
  }
  CreateFspVResultHob (
    L"ValidateMicrocodeRevision",
    IsTestPassed,
    IsTestPassed ? L"No Failure" : L"Test FAIL Microcode Revision"
    );
}
