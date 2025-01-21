/** @file
  This file contains an implementation of the function call interfaces
  required by the main SiInit PEIM file. Hopefully, future platform porting
  tasks will be mostly limited to modifying the functions in this file.

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

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/SiPolicy.h>
#include <DprInfoHob.h>
#include <Library/CpuCommonLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesTablePointerLib.h>

/**
  Init and Install DPR Info Hob

  @param[out] DprInfoHob         - DPR_INFO_HOB instance installed by this function
  @retval EFI_SUCCESS
**/
EFI_STATUS
InstallDprInfoHob (
  OUT      DPR_INFO_HOB               **DprInfoHobOut
  )
{
  EFI_STATUS                   Status;
  DPR_INFO_HOB                 *DprInfoHob;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicy;
  CPU_SECURITY_PREMEM_CONFIG   *CpuSecurityPreMemConfig;
  TXT_PREMEM_CONFIG            *TxtPreMemConfig;

  ///
  /// Create HOB for DPR Info
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (DPR_INFO_HOB),
             (VOID **) &DprInfoHob
             );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Initialize default HOB data
  ///
  CopyGuid (&DprInfoHob->EfiHobGuidType.Name, &gDprInfoHobGuid);
  DEBUG ((DEBUG_INFO, "DprInfoHob->EfiHobGuidType.Name: %g\n", &DprInfoHob->EfiHobGuidType.Name));
  ZeroMem (&(DprInfoHob->DprDirectory[0]), sizeof (DPR_INFO_HOB) - sizeof (EFI_HOB_GUID_TYPE));

  ///
  /// Update SA Config HOB
  ///
  Status = PeiServicesLocatePpi(
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gTxtPreMemConfigGuid, (VOID *) &TxtPreMemConfig);

  ///
  /// TXT DPR Directory Entry
  ///
  DprInfoHob->DprDirectory[EnumDprDirectoryTxt].Type = DPR_DIRECTORY_TYPE_TXT;
  if (CpuSecurityPreMemConfig->Txt) {
    DprInfoHob->DprDirectory[EnumDprDirectoryTxt].Size = (UINT8) RShiftU64 ((UINT64) TxtPreMemConfig->TxtDprMemorySize, 20);
  }


  (*DprInfoHobOut) = DprInfoHob;
  DEBUG ((DEBUG_INFO, "DPR Info HOB installed\n"));

  return EFI_SUCCESS;
}

/**
  Determine the Total DPR memory size needed based on the DPR directory in the SA Data HOB.

  @param[in, out] Size        - The memory size in MB to return.
**/
VOID
EFIAPI
CalculateTotalDprMemorySize (
  IN OUT   UINT8             *Size
  )
{
  UINT8                           DprEntryIndex;
  DPR_INFO_HOB                    *DprInfoHob;
  DPR_DIRECTORY_ENTRY             *DirectoryEntry;
  EFI_STATUS                      Status;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi;
  CPU_SECURITY_PREMEM_CONFIG      *CpuSecurityPreMemConfig;
  UINT16                          BiosMemSizeInMb;

  *Size                   = 0;
  DprEntryIndex           = 0;
  DirectoryEntry          = NULL;
  DprInfoHob              = NULL;
  BiosMemSizeInMb         = 0;
  SiPreMemPolicyPpi       = NULL;
  CpuSecurityPreMemConfig = NULL;

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  Status  = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  if (CpuSecurityPreMemConfig->BiosGuard == TRUE) {
    ///
    /// Check if BiosGuardToolsInterface policy is enabled
    /// If Enabled, BGUP data will use the memory allocated within DPR
    /// If Disabled, the size allocated to BIOS Guard in DPR is 0MB, BGUP data will be stored in TSEG
    ///
    if (CpuSecurityPreMemConfig->BiosGuardToolsInterface == TRUE) {
      ///
      /// The size allocated for DPR needs to account for the size of the BIOS ROM plus the size of the
      /// BGUP data: BGUP Header, BGUP Script,and BGUP Certificate.
      /// The size allocated needs to aligned to 1MB due to 1MB granularity of DPR.
      ///
      if ((CpuSecurityPreMemConfig->BiosSize % 1024) == 0) {
        BiosMemSizeInMb = (CpuSecurityPreMemConfig->BiosSize / 1024) + 1;
      } else {
        ///
        /// For a BiosSize that is not exactly 1MB, additional space will need to be added
        /// With consideration of the BGUP data, we can add an additional 2MB for a BIOS
        /// with greater than X.5MB, where X represents whole number value of the BiosSize
        ///
        if ((CpuSecurityPreMemConfig->BiosSize % 1024) < 512) {
          BiosMemSizeInMb = (CpuSecurityPreMemConfig->BiosSize / 1024) + 1;
        } else {
          BiosMemSizeInMb = (CpuSecurityPreMemConfig->BiosSize / 1024) + 2;
        }
      }
      ///
      /// Upon MTRR shortage, BiosMemSizeInMb can be 2MB aligned at the expense of losing memory.
      ///
      if ((BiosMemSizeInMb % 2) == 1) {
        BiosMemSizeInMb = BiosMemSizeInMb + 1;
      }
    }
    *Size += (UINT8)BiosMemSizeInMb;
  }

  DprInfoHob = GetFirstGuidHob (&gDprInfoHobGuid);
  if (DprInfoHob != NULL) {
    DirectoryEntry = DprInfoHob->DprDirectory;
    while (DprEntryIndex < DPR_DIRECTORY_MAX) {
      *Size += DirectoryEntry->Size;
      DirectoryEntry++;
      DprEntryIndex++;
    }
  }
  return;
}

/**
  Calculates the bases for each technology consuming the DPR region
  and updates the SA Data HOB with the appropriate values in the Dpr
  directory

  @param[in] Base           - The memory base to return.
  @param[in] TotalDprSizeMB - The total DPR size in MB
**/
VOID
EFIAPI
UpdateDprHobInfo (
  IN       EFI_PHYSICAL_ADDRESS  Base,
  IN       UINT8                 TotalDprSizeMB
  )
{
  UINT32               TopOfDpr;
  UINT8                DprEntryIndex;
  DPR_INFO_HOB         *DprInfoHob;
  DPR_DIRECTORY_ENTRY  *DirectoryEntry;

  DprEntryIndex   = 0;
  DirectoryEntry  = NULL;
  DprInfoHob      = NULL;
  TopOfDpr        = (UINT32) Base + (UINT32) LShiftU64 (TotalDprSizeMB, 20);

  DprInfoHob = GetFirstGuidHob (&gDprInfoHobGuid);
  if (DprInfoHob != NULL) {
    DirectoryEntry = DprInfoHob->DprDirectory;
    while (DprEntryIndex < DPR_DIRECTORY_MAX) {
      switch (DirectoryEntry->Type) {
        case DPR_DIRECTORY_TYPE_TXT:
          DirectoryEntry->PhysBase = (UINT32) TopOfDpr - (UINT32) LShiftU64(DirectoryEntry->Size, 20);
          break;
        default:
          break;
      }
      DirectoryEntry++;
      DprEntryIndex++;
    }
  }
  return;
}