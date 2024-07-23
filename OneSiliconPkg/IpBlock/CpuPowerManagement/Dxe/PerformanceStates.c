/** @file
  This file contains P States and Turbo Power Management configuration functions for
  processors.

  <b>Acronyms:</b>
     - PPM:  Processor Power Management
     - TM:   Thermal Monitor
     - IST:  Intel(R) Speedstep technology
     - HT:   Hyper-Threading Technology

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#include "PowerMgmtInit.h"

//
// Update ACPI PerfomanceStates tables
//

/**
  This patches the PSS entry according to input.

  @param [in]  PssEntry                  PSS entry.
  @param [in]  IsTurboEnabled            Is turbo is enabled.
  @param [in]  IsFirstEntry              Is first PSS entry.
  @param [in]  HfmBusRatio               HFM bus ratio. Only valid when IsFirstEntry is TRUE.
  @param [in]  BusRatio                  Assigned bus ratio.
  @param [in]  Power                     Assigned power.
  @param [in]  MaximumNonTurboRatio      Max non turbo ratio.
  @param [in]  MaximumEfficiencyRatio    Max efficient ratio.
**/
STATIC
VOID
PatchPssEntry (
  PSS_PACKAGE_LAYOUT *PssEntry,
  BOOLEAN            IsTurboEnabled,
  BOOLEAN            IsFirstEntry,
  UINT32             HfmBusRatio,
  UINT32             BusRatio,
  UINT32             Power,
  UINT8              MaximumNonTurboRatio,
  UINT8              MaximumEfficiencyRatio
  )
{
  UINT16             PnPercent;
  ///
  /// Update the _PSS table. If Turbo mode is supported, add one to the Max Non-Turbo frequency
  ///
  if (IsTurboEnabled && IsFirstEntry) {
    PssEntry->CoreFrequency = (HfmBusRatio * 100) + 1;
  } else if (BusRatio < MaximumEfficiencyRatio) {
    //
    // If cTDP Down Ratio == LFM, set it to 1% lower than LFM.
    //
    PnPercent = (MaximumEfficiencyRatio * 100) / MaximumNonTurboRatio;
    PssEntry->CoreFrequency = (MaximumNonTurboRatio * (PnPercent - 1)); // Simplified Calculation.
  } else {
    PssEntry->CoreFrequency = BusRatio * 100;
  }
  PssEntry->Power = Power;
  ///
  /// If it's PSS table, Control is the PERF_CTL value.
  /// Status entry is the same as control entry.
  /// TransLatency uses 10
  ///
  PssEntry->TransLatency  = NATIVE_PSTATE_LATENCY;
  PssEntry->Control       = (UINT32) LShiftU64 (BusRatio, 8);
  //
  // Ensure any future OS would not look for the IA32_PERF_STATUS MSR to check if the value matches
  //
  if (BusRatio < MaximumEfficiencyRatio) {
    PssEntry->Status        = (UINT32) LShiftU64 (MaximumEfficiencyRatio, 8);
  } else {
    PssEntry->Status        = (UINT32) LShiftU64 (BusRatio, 8);
  }
  PssEntry->BmLatency     = PSTATE_BM_LATENCY;
}


/**
  Dump FVID Tables.

  @param[in out] FvidPointer         Pointer to a table to be updated
  @param[in]     LpssNumberOfStates  Number of entries in the table pointed to by FvidPointer for LPSS
  @param[in]     LpssNumberOfStates  Number of entries in the table pointed to by FvidPointer for TPSS
**/
VOID
DumpFvidTable (
  IN OUT FVID_TABLE *FvidPointer,
  IN UINT16         LpssNumberOfStates,
  IN UINT16         TpssNumberOfStates
  )
{
  UINTN             Index;

  //
  // Print LPSS and TPSS FVID Tables
  //
  DEBUG ((DEBUG_INFO, "LPSS FVID Acpi Pss Patched Table (%d)\n", LpssNumberOfStates));
  DEBUG ((DEBUG_INFO, "Index\tState\tRatio\tPower\n"));
  for (Index = 1; Index <= LpssNumberOfStates ; Index++) {
    DEBUG ((DEBUG_INFO, "%d\t%d\t%d\t%d\n",Index, FvidPointer[Index].FvidState.Limit16State, FvidPointer[Index].FvidState.Limit16BusRatio, FvidPointer[Index].FvidState.Limit16Power));
  }

  DEBUG ((DEBUG_INFO, "TPSS FVID Acpi Pss Patched Table (%d)\n", TpssNumberOfStates));
  DEBUG ((DEBUG_INFO, "Index\tState\tRatio\tPower\n"));
  for (Index = 1; Index <= TpssNumberOfStates ; Index++) {
    DEBUG ((DEBUG_INFO, "%d\t%d\t%d\t%d\n",Index, FvidPointer[Index].FvidState.State, FvidPointer[Index].FvidState.BusRatio, FvidPointer[Index].FvidState.Power));
  }
}

/**
  Patch the native _PSS package with the EIST values
  Uses ratio/VID values from the FVID table to fix up the control values in the _PSS.

  (1) Find _PSS package:
    (1.1) Find the _PR_CPU0 scope.
    (1.2) Save a pointer to the package length.
    (1.3) Find the _PSS AML name object.
  (2) Resize the _PSS package.
  (3) Fix up the _PSS package entries
    (3.1) Check Turbo mode support.
    (3.2) Check Dynamic FSB support.
  (4) Fix up the Processor block and \_PR_CPU0 Scope length.
  (5) Update SSDT Header with new length.

  @retval EFI_SUCCESS   - on success
  @retval EFI_NOT_FOUND - if _PR_.CPU0 scope is not foud in the ACPI tables
**/
EFI_STATUS
AcpiPatchPss (
  VOID
  )
{
  UINT8              *CurrPtr;
  UINT8              *EndOfTable;
  UINT16             Index;
  UINT16             NewPackageLength;
  UINT16             LpssMaxPackageLength;
  UINT16             TpssMaxPackageLength;
  UINT16             Temp;
  UINT16             *PackageLength;
  UINT16             *ScopePackageLengthPtr;
  UINT32             *Signature;
  PSS_PACKAGE_LAYOUT *PssPackage;
  UINT8               MaximumEfficiencyRatio;
  UINT8               MaximumNonTurboRatio;
  BOOLEAN             IsTurboEnabled;

  ScopePackageLengthPtr = NULL;
  PssPackage            = NULL;

  IsTurboEnabled = (gCpuNvsAreaProtocol->Area->PpmFlags & PPM_TURBO) != 0;

  //
  // Get Maximum Efficiency bus ratio (LFM) from Platform Info MSR Bits[47:40]
  // Get Maximum Non Turbo bus ratio from Platform Info MSR Bits[15:8]
  //
  GetBusRatio ((UINT8 *) &MaximumNonTurboRatio, (UINT8 *) &MaximumEfficiencyRatio);
  mNumberOfStates = mFvidPointer[0].FvidHeader.EistStates;

  ///
  /// Calculate new package length
  ///
  NewPackageLength      = Temp = (UINT16) (mNumberOfStates * sizeof (PSS_PACKAGE_LAYOUT) + 3);
  LpssMaxPackageLength  = (UINT16) (LPSS_FVID_MAX_STATES * sizeof (PSS_PACKAGE_LAYOUT) + 3);
  TpssMaxPackageLength  = (UINT16) (TPSS_FVID_MAX_STATES * sizeof (PSS_PACKAGE_LAYOUT) + 3);
  ///
  /// Locate the SSDT package in the IST table
  ///
  CurrPtr     = (UINT8 *) mCpu0IstTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  EndOfTable  = (UINT8 *) mCpu0IstTable + mCpu0IstTable->Length;
  for (; CurrPtr < EndOfTable; CurrPtr++) {
    Signature = (UINT32 *) (CurrPtr + 1);
    ///
    /// If we find the _SB_PR00 scope, save a pointer to the package length
    ///
    if ((*CurrPtr == AML_SCOPE_OP) &&
        (*(Signature + 1) == SIGNATURE_32 ('_', 'S', 'B', '_')) &&
        (*(Signature + 2) == SIGNATURE_32 ('P', 'R', '0', '0'))
        ) {
      ScopePackageLengthPtr = (UINT16 *) (CurrPtr + 1);
    }
    ///
    /// Patch the LPSS package with 16 P-states for _PSS Method
    ///
    if ((*CurrPtr == AML_NAME_OP) && (*Signature == SIGNATURE_32 ('L', 'P', 'S', 'S'))) {
      /*
        Check table dimensions.
        PSS package reserve space for LPSS_FVID_MAX_STATES number of P-states so check if the
        current number of P- states is more than LPSS_FVID_MAX_STATES. Also need to update the SSDT contents
        if the current number of P-states is less than LPSS_FVID_MAX_STATES.
      */
      PackageLength  = (UINT16 *) (CurrPtr + 6);

      if (mNumberOfStates > LPSS_FVID_MAX_STATES) {
        *(CurrPtr + 8) = (UINT8) LPSS_FVID_MAX_STATES;
        ///
        /// Update the Package length in AML package length format
        ///
        *PackageLength = ((LpssMaxPackageLength & 0x0F) | 0x40) | ((LpssMaxPackageLength << 4) & 0x0FF00);
      } else {
        *(CurrPtr + 8) = (UINT8) mNumberOfStates;
        ///
        /// Update the Package length in AML package length format
        ///
        *PackageLength = ((NewPackageLength & 0x0F) | 0x40) | ((Temp << 4) & 0x0FF00);
        ///
        /// Move SSDT contents
        ///
        CopyMem ((CurrPtr + NewPackageLength), (CurrPtr + LpssMaxPackageLength), EndOfTable - (CurrPtr + LpssMaxPackageLength));

        ///
        /// Save the new end of the SSDT
        ///
        EndOfTable = EndOfTable - (LpssMaxPackageLength - NewPackageLength);
      }

      PssPackage = (PSS_PACKAGE_LAYOUT *) (CurrPtr + 9);
      if (mNumberOfStates > LPSS_FVID_MAX_STATES) {
        for (Index = 1; Index <= LPSS_FVID_MAX_STATES; Index++) {
          PatchPssEntry ( PssPackage,
                          IsTurboEnabled,
                          Index == 1,
                          Index == 1 ? mFvidPointer[Index + 1].FvidState.Limit16BusRatio : 0,
                          mFvidPointer[Index].FvidState.Limit16BusRatio,
                          mFvidPointer[Index].FvidState.Limit16Power,
                          MaximumNonTurboRatio,
                          MaximumEfficiencyRatio
                          );
          PssPackage++;
        }
      } else {
        for (Index = 1; Index <= mNumberOfStates; Index++) {
          PatchPssEntry ( PssPackage,
                          IsTurboEnabled,
                          Index == 1,
                          Index == 1 ? mFvidPointer[Index + 1].FvidState.BusRatio : 0,
                          mFvidPointer[Index].FvidState.BusRatio,
                          mFvidPointer[Index].FvidState.Power,
                          MaximumNonTurboRatio,
                          MaximumEfficiencyRatio
                          );
          PssPackage++;
        }
      }
    }

    ///
    /// Patch the TPSS package with no limit P-states for _PSS Method
    ///
    if ((*CurrPtr == AML_NAME_OP) && (*Signature == SIGNATURE_32 ('T', 'P', 'S', 'S'))) {
      ASSERT (mNumberOfStates <= TPSS_FVID_MAX_STATES);
      if (mNumberOfStates <= TPSS_FVID_MAX_STATES) {

        *(CurrPtr + 8)  = (UINT8) mNumberOfStates;
        PackageLength   = (UINT16 *) (CurrPtr + 6);
        ///
        /// Update the Package length in AML package length format
        ///
        *PackageLength = ((NewPackageLength & 0x0F) | 0x40) | ((Temp << 4) & 0x0FF00);
        ///
        /// Move SSDT contents
        ///
        CopyMem ((CurrPtr + NewPackageLength), (CurrPtr + TpssMaxPackageLength), EndOfTable - (CurrPtr + TpssMaxPackageLength));
        ///
        /// Save the new end of the SSDT
        ///
        EndOfTable = EndOfTable - (TpssMaxPackageLength - NewPackageLength);
      }

      PssPackage = (PSS_PACKAGE_LAYOUT *) (CurrPtr + 9);
      for (Index = 1; Index <= mNumberOfStates; Index++) {
        PatchPssEntry ( PssPackage,
                        IsTurboEnabled,
                        Index == 1,
                        Index == 1 ? mFvidPointer[Index + 1].FvidState.BusRatio : 0,
                        mFvidPointer[Index].FvidState.BusRatio,
                        mFvidPointer[Index].FvidState.Power,
                        MaximumNonTurboRatio,
                        MaximumEfficiencyRatio
                        );
        PssPackage++;
      }
    }
  }

  //
  // Dump FVID table for LPSS and TPSS
  //
  DumpFvidTable (mFvidPointer, LPSS_FVID_MAX_STATES, mNumberOfStates);

  ASSERT (ScopePackageLengthPtr != NULL);
  if (ScopePackageLengthPtr == NULL) {
    return EFI_NOT_FOUND;
  }
  ///
  /// Update the Package length in AML package length format
  ///
  CurrPtr                 = (UINT8 *) ScopePackageLengthPtr;
  NewPackageLength        = Temp = (UINT16) (EndOfTable - CurrPtr);
  *ScopePackageLengthPtr  = ((NewPackageLength & 0x0F) | 0x40) | ((Temp << 4) & 0x0FF00);
  mCpu0IstTable->Length   = (UINT32) (EndOfTable - (UINT8 *) mCpu0IstTable);

  return EFI_SUCCESS;
}
