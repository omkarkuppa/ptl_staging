/** @file
  Source code file for Set Cache Mtrr Pei module

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

#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MtrrLib.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Guid/SmramMemoryReserve.h>
#include <Core/Pei/PeiMain.h>


/**
  Initializes the valid bits mask and valid address mask for MTRRs.

  This function initializes the valid bits mask and valid address mask for MTRRs.

  @param[out]  MtrrValidBitsMask     The mask for the valid bit of the MTRR
  @param[out]  MtrrValidAddressMask  The valid address mask for the MTRR

**/
VOID
MtrrLibInitializeMtrrMask (
  OUT UINT64 *MtrrValidBitsMask,
  OUT UINT64 *MtrrValidAddressMask
  );

/**
  Convert variable MTRRs to a RAW MTRR_MEMORY_RANGE array.
  One MTRR_MEMORY_RANGE element is created for each MTRR setting.
  The routine doesn't remove the overlap or combine the near-by region.

  @param[in]   VariableSettings      The variable MTRR values to shadow
  @param[in]   VariableMtrrCount     The number of variable MTRRs
  @param[in]   MtrrValidBitsMask     The mask for the valid bit of the MTRR
  @param[in]   MtrrValidAddressMask  The valid address mask for MTRR
  @param[out]  VariableMtrr          The array to shadow variable MTRRs content

  @return      Number of MTRRs which has been used.

**/
UINT32
MtrrLibGetRawVariableRanges (
  IN  MTRR_VARIABLE_SETTINGS  *VariableSettings,
  IN  UINTN                   VariableMtrrCount,
  IN  UINT64                  MtrrValidBitsMask,
  IN  UINT64                  MtrrValidAddressMask,
  OUT MTRR_MEMORY_RANGE       *VariableMtrr
  );

/**
  Apply the variable MTRR settings to memory range array.

  @param[in]      VariableMtrr      The variable MTRR array.
  @param[in]      VariableMtrrCount The count of variable MTRRs.
  @param[in, out] Ranges            Return the memory range array with new MTRR settings applied.
  @param[in]      RangeCapacity     The capacity of memory range array.
  @param[in, out] RangeCount        Return the count of memory range.

  @retval RETURN_SUCCESS            The memory range array is returned successfully.
  @retval RETURN_OUT_OF_RESOURCES   The count of memory ranges exceeds capacity.
**/
RETURN_STATUS
MtrrLibApplyVariableMtrrs (
  IN     CONST MTRR_MEMORY_RANGE *VariableMtrr,
  IN     UINT32                  VariableMtrrCount,
  IN OUT MTRR_MEMORY_RANGE       *Ranges,
  IN     UINTN                   RangeCapacity,
  IN OUT UINTN                   *RangeCount
  );

/**
  This function attempts to set the attributes into MTRR setting buffer.

  @param[in, out] MtrrSetting   - A buffer holding all MTRRs content.
  @param[in]      Ranges        - Array holding memory type settings.
  @param[in]      RangeCount    - Memory range count in the array.

  @retval Count of used variable Mtrrs
**/
EFI_STATUS
EFIAPI
EOPSetMemoryAttributesInMtrrSettings (
  IN OUT MTRR_SETTINGS      *MtrrSetting,
  IN     MTRR_MEMORY_RANGE  *Ranges,
  IN     UINTN              RangeCount
  )
{
  EFI_STATUS        Status;
  UINTN             Index;
  UINTN             HighIndex;
  UINT64            TopHighMemory;

  Status = EFI_NOT_FOUND;

  for (Index = 0, HighIndex = 0xFF; Index < RangeCount; Index++) {
    //
    // Set Mtrr variables from 1M.
    //
    if (Ranges[Index].BaseAddress < 0x100000) {
      Ranges[Index].Length -= 0x100000;
      Ranges[Index].BaseAddress = 0x100000;
    }
    if ((Ranges[Index].BaseAddress >= SIZE_4GB) && (Ranges[Index].Type == CacheWriteBack)) {
      HighIndex = Index;                       // Set above 4G attributes at the latest step.
    } else {
      Status = MtrrSetMemoryAttributeInMtrrSettings (
                 MtrrSetting,
                 Ranges[Index].BaseAddress,
                 Ranges[Index].Length,
                 Ranges[Index].Type
                 );
      ASSERT_EFI_ERROR (Status);
    }
  }
  if (HighIndex != 0xFF) {
    TopHighMemory = Ranges[HighIndex].BaseAddress + Ranges[HighIndex].Length;
    //
    // Try to cover memory as mmuch as we can.
    // In later phase boot loader code can re-configure MTRR to exclude flash region and get back above 4GB coverage.
    //
    do {
      Status = MtrrSetMemoryAttributeInMtrrSettings (
                 MtrrSetting,
                 SIZE_4GB,
                 TopHighMemory - SIZE_4GB,
                 CacheWriteBack
                 );
      if (TopHighMemory > SIZE_4GB) {
        TopHighMemory = RShiftU64 (TopHighMemory, 1);
      }
    } while ((EFI_SUCCESS != Status) && (TopHighMemory > SIZE_4GB));
  }

  return Status;
}

EFI_STATUS
EFIAPI
SetCacheMtrrAfterEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) SetCacheMtrrAfterEndOfPei
};


/**
  Update MTRR setting in EndOfPei phase.

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
SetCacheMtrrAfterEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_HOB_POINTERS                  Hob;
  UINTN                                 Index;
  UINT64                                SmramSize;
  UINT64                                SmramBase;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK        *SmramHobDescriptorBlock;
  MTRR_SETTINGS                         MtrrSetting;
  UINTN                                 RangeCount;
  UINT32                                VariableMtrrCount;
  UINT64                                MtrrValidBitsMask;
  UINT64                                MtrrValidAddressMask;
  MTRR_MEMORY_RANGE                     RawVariableRanges[MTRR_NUMBER_OF_VARIABLE_MTRR];
  MTRR_MEMORY_RANGE                     Ranges[MTRR_NUMBER_OF_VARIABLE_MTRR];
  MTRR_SETTINGS                         UCMtrrSetting;

  //
  // PI SMM IPL can't set SMRAM to WB because at that time CPU ARCH protocol is not available.
  // Set cacheability of SMRAM to WB here to improve SMRAM initialization performance.
  //
  SmramSize = 0;
  SmramBase = 0;
  Status = PeiServicesGetHobList ((VOID **) &Hob.Raw);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION) {
      if (CompareGuid (&Hob.Guid->Name, &gEfiSmmSmramMemoryGuid)) {
        SmramHobDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) (Hob.Guid + 1);
        for (Index = 0; Index < SmramHobDescriptorBlock->NumberOfSmmReservedRegions; Index++) {
          if (SmramHobDescriptorBlock->Descriptor[Index].PhysicalStart > 0x100000) {
            SmramSize += SmramHobDescriptorBlock->Descriptor[Index].PhysicalSize;
            if (SmramBase == 0 || SmramBase > SmramHobDescriptorBlock->Descriptor[Index].CpuStart) {
              SmramBase = SmramHobDescriptorBlock->Descriptor[Index].CpuStart;
            }
          }
        }
        break;
      }
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  MtrrGetAllMtrrs (&MtrrSetting);
  VariableMtrrCount = GetVariableMtrrCount ();
  MtrrLibInitializeMtrrMask (&MtrrValidBitsMask, &MtrrValidAddressMask);

  Ranges[0].BaseAddress = 0;
  Ranges[0].Length      = MtrrValidBitsMask + 1;
  Ranges[0].Type        = (MTRR_MEMORY_CACHE_TYPE) (MtrrSetting.MtrrDefType & 0x07); //[Bits 2:0] Default Memory Type.
  RangeCount = 1;

  MtrrLibGetRawVariableRanges (
    &MtrrSetting.Variables, VariableMtrrCount,
    MtrrValidBitsMask, MtrrValidAddressMask, RawVariableRanges
    );

  MtrrLibApplyVariableMtrrs (
    RawVariableRanges, VariableMtrrCount,
    Ranges, ARRAY_SIZE (Ranges), &RangeCount
    );

  //
  // Set SMRAM as CacheWriteBack for performance.
  //
  Ranges[RangeCount].BaseAddress = SmramBase;
  Ranges[RangeCount].Length      = SmramSize;
  Ranges[RangeCount].Type        = CacheWriteBack;
  RangeCount++;

  ZeroMem (&UCMtrrSetting, sizeof (MTRR_SETTINGS));
  UCMtrrSetting.MtrrDefType = MtrrSetting.MtrrDefType;
  Status = EOPSetMemoryAttributesInMtrrSettings (&UCMtrrSetting, Ranges, RangeCount);

  CopyMem (&MtrrSetting.Variables, &UCMtrrSetting.Variables, sizeof (MTRR_VARIABLE_SETTINGS));
  MtrrSetting.MtrrDefType = UCMtrrSetting.MtrrDefType;

  return Status;
}

/**
  Set Cache Mtrr PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
SetCacheMtrrPeiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                        Status;
  //
  // Performing PlatformInitEndOfPei after EndOfPei PPI produced
  //
  Status = PeiServicesNotifyPpi (&mEndOfPeiNotifyList);

  return Status;
}
