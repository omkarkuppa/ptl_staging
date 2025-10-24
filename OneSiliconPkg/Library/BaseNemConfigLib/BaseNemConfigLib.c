/**@file
  NEM config library provides APIs for MTRR operation to change NEM (No-Evict Mode) range.

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

#include <Uefi.h>
#include <Register/Intel/Cpuid.h>
#include <Register/Intel/Msr.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include <Register/Intel/Cpuid.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <Library/IoLib.h>
#include <Library/BootGuardLib.h>
#include <Library/NemConfigLib.h>
#include <Library/PcdLib.h>

typedef struct {
  UINTN    Cr4;
  BOOLEAN  InterruptState;
} MTRR_CONTEXT;

#ifndef DIVIDEFLOOR
#define DIVIDEFLOOR(a, b)  ((a) / (b))
#endif

#ifndef DIVIDECEIL
#define DIVIDECEIL(a, b)   (((a) + (b) - 1) / (b))
#endif

/**
  Decode ACM real module size

  @param[out] AcmSize    The real size of the ACM.

**/
VOID
EFIAPI
GetAcmSize (
  OUT UINT32    *AcmSize
  )
{
  UINT64                                 FitPointer;
  FIRMWARE_INTERFACE_TABLE_ENTRY         *FitEntry;
  UINT32                                 EntryNum;
  UINTN                                  Index;
  UINTN                                  AcmAddress;
  UINTN                                  AcmSizeOffset;

  AcmSizeOffset = 0x18;
  *AcmSize = 0;

  FitPointer = *(UINT64 *) (UINTN) FIT_POINTER_ADDRESS;
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *) (UINTN) FitPointer;
  EntryNum = *(UINT32 *)(&FitEntry[0].Size[0]) & 0xFFFFFF;

  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == FIT_TYPE_02_STARTUP_ACM) {
      AcmAddress = (UINT32) (FitEntry[Index].Address + AcmSizeOffset);
      *AcmSize = (UINT32) MmioRead64 (AcmAddress) * 4;
    }
  }
}

/**
  Check ACM setups initial NEM or not.

  @param[out] AcmEnable             Return True if ACM sets up initial NEM.

  @retval EFI_SUCCESS               Function is executed successfully.
  @retval EFI_INVALID_PARAMETER     AcmEnable is NULL.
**/
EFI_STATUS
EFIAPI
AcmSetNemCheck (
  OUT BOOLEAN    *AcmEnable
  )
{
  UINT32       MsrValue;

  if (AcmEnable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Read Boot Guard S-ACM Info MSR
  MsrValue = (UINT32) AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO);
  // Check Bit 0 of BOOT_GUARD_SACM_INFO MSR if system is in Boot Guard boot mode
  // Bit 0 of BOOT_GUARD_SACM_INFO (B_BOOT_GUARD_SACM_INFO_NEM_ENABLED):
  //   - 0 : NEM was NOT initiated by the Boot Guard S-ACM. The
  //         platform Firmware/BIOS initialized NEM.
  //   - 1 : NEM was initiated by the Boot Guard S-ACM (ACM configured NEM).
  if ((MsrValue & B_BOOT_GUARD_SACM_INFO_NEM_ENABLED) == 0) {
    *AcmEnable = FALSE;
  } else {
    *AcmEnable = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  Calculate Effective Cache size and Effective WayCache size.

  @param[out] EffCacheSize        The size of the EffCache.
  @param[out] EffWayCacheSize     The size of the EffWayCache.

  @retval  UINT8                  The number of ACMWay that have been used.

**/
UINT16
EFIAPI
GetEffCacheSize (
  OUT UINT32    *EffCacheSize,
  OUT UINT32    *EffWayCacheSize
  )
{
  CPUID_CACHE_PARAMS_EAX  Eax;
  CPUID_CACHE_PARAMS_EBX  Ebx;
  UINT32                  Ecx;
  UINT32                  CacheLevel;
  UINT32                  CacheSize;
  UINT32                  WayCnt;
  UINT32                  WayCacheSize;
  UINT32                  SetCnt;
  UINT32                  EffSetCnt;
  UINT32                  RealAcmSize;
  UINT8                   ACMWay;
  UINT32                  MTRR0Size;
  BOOLEAN                 AcmEnable;

  AcmEnable = FALSE;
  WayCnt = 0;
  CacheSize = 0;
  WayCacheSize = 0;
  *EffCacheSize = 0;
  *EffWayCacheSize = 0;
  CacheLevel = 0;

  AcmSetNemCheck (&AcmEnable);

  // Dynamically search for LLC.
  do {
    CacheLevel += 1;
    AsmCpuidEx (CPUID_CACHE_PARAMS, CacheLevel, &Eax.Uint32, &Ebx.Uint32, &Ecx, NULL);
  } while (Eax.Uint32 != 0);

  CacheLevel -= 1;
  ASSERT (CacheLevel > 0);

  AsmCpuidEx (CPUID_CACHE_PARAMS, CacheLevel, &Eax.Uint32, &Ebx.Uint32, &Ecx, NULL);

  // Get Cache parameters
  //
  // This Cache Size in Bytes = (Ways + 1) * (Partitions + 1) * (Line_Size + 1) * (Sets + 1)
  //  = (EBX[31:22] + 1) * (EBX[21:12] + 1) * (EBX[11:0] + 1) * (ECX + 1)
  CacheSize = (Ebx.Bits.Ways + 1) * (Ebx.Bits.LinePartitions + 1) * (Ebx.Bits.LineSize + 1) * (Ecx + 1);

  WayCnt = Ebx.Bits.Ways + 1;
  WayCacheSize = CacheSize / WayCnt;
  SetCnt = (Ecx + 1);

  // ACM is in L2. If NEM is allocated from L3: ACMways = 0.
  if ((AcmEnable == FALSE) || (CacheLevel == 3)) {
    ACMWay = 0;
  } else {
    GetAcmSize (&RealAcmSize);
    MTRR0Size = GetPowerOfTwo32(RealAcmSize)*2;
    ACMWay = (UINT8)(MTRR0Size / WayCacheSize);
  }
  EffSetCnt = GetPowerOfTwo32 (SetCnt);

  *EffCacheSize = CacheSize / SetCnt * EffSetCnt;
  *EffWayCacheSize = WayCacheSize / SetCnt * EffSetCnt;

  return ACMWay;
}

/**
  Calculate overall Guard Band size.

  @param[in] EffWayCacheSize        The size of the EffWayCache.
  @param[out] AllGbSize             The size of all the GuardBand add up.

**/
VOID
EFIAPI
GetGbSize (
  IN  UINT32    EffWayCacheSize,
  OUT UINTN     *AllGbSize
  )
{
  MTRR_SETTINGS      *Mtrrs;
  MTRR_SETTINGS      LocalMtrrs;
  UINTN              RangeCount;
  UINTN              Index;
  UINTN              GuardBand;
  UINT64             EndAddress;
  EFI_STATUS         Status;
  MTRR_MEMORY_RANGE  Ranges[
                            MTRR_NUMBER_OF_FIXED_MTRR * sizeof (UINT64) + 2 * ARRAY_SIZE (Mtrrs->Variables.Mtrr) + 1
  ];

  *AllGbSize = 0;
  GuardBand = 0;
  ZeroMem (&LocalMtrrs, sizeof (LocalMtrrs));
  MtrrGetAllMtrrs (&LocalMtrrs);
  Mtrrs = &LocalMtrrs;
  RangeCount = ARRAY_SIZE (Ranges);

  Status = MtrrGetMemoryAttributesInMtrrSettings (Mtrrs, Ranges, &RangeCount);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Can't get the memory range from current Mtrr setting\n"));
  }

  for (Index = 0; Index < RangeCount-1; Index++) {
    if (Ranges[Index].Type != Ranges[Index + 1].Type) {
      if (Ranges[Index].Type != CacheUncacheable) {  // If it's UC, then don't need to check
        // Check base address is WayCacheSize aligned or not
        GuardBand = (UINTN) (Ranges[Index].BaseAddress - (DIVIDEFLOOR (Ranges[Index].BaseAddress, EffWayCacheSize) * EffWayCacheSize));
        *AllGbSize = *AllGbSize + GuardBand;

        EndAddress = Ranges[Index].BaseAddress + Ranges[Index].Length;
        if (EndAddress <= 0xFFFFFFFF) { // It not, it should be on 4G
          GuardBand = (UINTN) ((DIVIDECEIL (EndAddress, EffWayCacheSize) * EffWayCacheSize) - EndAddress);
          *AllGbSize = *AllGbSize + GuardBand;
        }
      }
    }
  }
  DEBUG ((DEBUG_INFO, "Overall GB is %x\n", *AllGbSize));
}

/**
  Calculate the available NEM size

  @param[in]  DataStackSize    The size of the data stack.
  @param[in]  IbbSize          The size of the common IBB.
  @param[in]  SizeOfNewRange   The size of the new NEM range.

  @retval     UINTN            The size of the remaining NEM range.

**/
UINTN
EFIAPI
NemSizeCalculate (
  IN  UINTN          DataStackSize,
  IN  UINTN          IbbSize,
  IN  UINTN          *SizeOfNewRange
  )
{
  UINT32                  EffCacheSize;
  UINTN                   AvailableCacheSize;
  UINT64                  GbSize;
  UINTN                   NemSize;
  UINTN                   RestOfNemSize;
  UINT32                  EffWayCacheSize;
  UINTN                   SecondaryDataStackSize;

  EffCacheSize = 0;
  AvailableCacheSize = 0;
  NemSize = 0;
  RestOfNemSize = 0;
  GbSize = 0;

  DEBUG ((DEBUG_INFO, "%a << Begin\n", __FUNCTION__));

  if (SizeOfNewRange == NULL) {
    DEBUG ((DEBUG_INFO, "Input parameter in not correct."));
    return 0;
  }
  GetEffCacheSize (&EffCacheSize, &EffWayCacheSize);
  GetGbSize (EffWayCacheSize, &GbSize);

  // Code cache size = Total NEM size - DataStack size - Guard band size - Secondary DataStack size
  AvailableCacheSize = EffCacheSize - GbSize;
  NemSize = AvailableCacheSize - DataStackSize;

  // Account for secondary data stack if configured
  if (PcdGet32 (PcdSecondaryDataStackSize) != 0) {
    SecondaryDataStackSize = PcdGet32 (PcdSecondaryDataStackSize);
    NemSize -= SecondaryDataStackSize;
  }

  RestOfNemSize = NemSize - *SizeOfNewRange - IbbSize;

  if (*SizeOfNewRange == 0) {
      DEBUG ((DEBUG_INFO, "Original max NEM size can be around %x\n", RestOfNemSize));
  } else {
    // If RestOfNemSize is negative
    if (RestOfNemSize > NemSize) {
      DEBUG ((DEBUG_INFO, "The new config range is too big to be accommodated, please adjust it.\n"));
    } else {
      DEBUG ((DEBUG_INFO, "After adding the new range %x, the remaining NEM size is %x\n", *SizeOfNewRange, RestOfNemSize));
    }
  }
  DEBUG ((DEBUG_INFO, "%a << End\n", __FUNCTION__));
  return RestOfNemSize;
}

/**
  Configure the variable MTRRs to change NEM range

  @param[in]  VariableSettings   A buffer to hold variable MTRRs content.

**/
VOID
EFIAPI
SetVariableMtrrForNemRange (
  IN MTRR_VARIABLE_SETTINGS         *VariableSettings
  )
{
  MTRR_CONTEXT  MtrrContext;
  UINT32        Index;
  UINT32        VariableMtrrCount;

  MtrrContext.InterruptState = SaveAndDisableInterrupts ();
  MtrrContext.Cr4 = AsmReadCr4 ();
  CpuFlushTlb ();

  //
  // Set variable MTRRs
  //
  VariableMtrrCount = GetVariableMtrrCount ();
  ASSERT (VariableMtrrCount <= ARRAY_SIZE (VariableSettings->Mtrr));

  for (Index = 0; Index < VariableMtrrCount; Index++) {
    AsmWriteMsr64 (
      MSR_IA32_MTRR_PHYSBASE0 + (Index << 1),
      VariableSettings->Mtrr[Index].Base
      );

    AsmWriteMsr64 (
      MSR_IA32_MTRR_PHYSMASK0 + (Index << 1),
      VariableSettings->Mtrr[Index].Mask
      );
  }

  CpuFlushTlb ();
  AsmWriteCr4 (MtrrContext.Cr4);
  SetInterruptState (MtrrContext.InterruptState);

}

/**
  Get remaining cache size for a given NEM buffer list.

  @param[in]  NemBufferList      A NEM buffer list containing all NEM buffers already allocated or plan
                                 to allocate.
  @param[out] RemainingNemSize   The remaining cache size after all NEM buffers in list allocated. This
                                 value is only valid when EFI_SUCCESS is returned.

  @retval EFI_SUCCESS            The remaining cache size already calculated successfully.
  @retval EFI_OUT_OF_RESOURCES   There's no enough cache size for the given NEM buffer list.

**/
EFI_STATUS
GetRemainingNemSize (
  IN   NEM_BUFFER_LIST      *NemBufferList,
  OUT  UINT32               *RemainingNemSize
  )
{
  UINT32                  Index;
  UINT32                  TotalSize;
  UINT32                  EffCacheSize;
  UINT32                  EffWayCacheSize;

  TotalSize = 0;
  for (Index = 0; Index < NemBufferList->Count; Index ++) {
    TotalSize += NemBufferList->NemBufferList[Index].NemBufferSize;
  }

  GetEffCacheSize (&EffCacheSize, &EffWayCacheSize);

  if (EffCacheSize >= TotalSize) {
    *RemainingNemSize = EffCacheSize - TotalSize;
    return EFI_SUCCESS;
  } else {
    *RemainingNemSize = 0;
    return EFI_OUT_OF_RESOURCES;
  }
}

/**
  Convert a flash region list to a NEM buffer list.

  @param[in]  FlashRegionList    A list which contains all regions which are expected to be loaded to NEM,
                                 caller should be clear of which regions should be loaded to NEM when this
                                 API is called. All flash regions in list should be reorganized in ascending
                                 order on flash region base, overlaps are not allowed between different flash
                                 regions.
  @param[in]  DataStackBase      The base address of data stack allocated by bios. Must be aligned to
                                 Effective WayCacheSize.
  @param[in]  DataStackSize      The size of data stack allocated by bios. Must be aligned to Effective
                                 WayCacheSize.
  @param[out] NemBufferList      Corresponding NEM buffer list for the given flash region list. Each NEM buffer
                                 is created as EffWayCacheSize aligned.

  @retval EFI_SUCCESS            NEM buffer list is calculated successfully with the given flash regions.
  @retval EFI_DEVICE_ERROR       Unexpected error occur when checking effective WayCacheSize.
  @retval EFI_INVALID_PARAMETER  One or more input parameters are not valid.

**/
EFI_STATUS
ConvertFlashRegionToNemBuffer (
  IN   FLASH_REGION_LIST    *FlashRegionList,
  IN   UINT32               DataStackBase,
  IN   UINT32               DataStackSize,
  OUT  NEM_BUFFER_LIST      *NemBufferList
  )
{
  UINT32                  Index;
  UINT32                  EffCacheSize;
  UINT32                  EffWayCacheSize;
  UINT32                  AlignedBase;
  UINT32                  AlignedSize;
  UINT64                  AlignedTop;   /// AlignedTop may reach top address 4G, need a UINT64 value
  UINT32                  CurrentFlashRegionBase;
  UINT32                  CurrentFlashRegionSize;
  UINT32                  CurrentFlashRegionType;
  UINT32                  LastNemBufferBase;
  UINT32                  LastNemBufferSize;
  UINT32                  LastNemBufferType;
  UINT64                  LastNemBufferTop;   /// LastNemBufferTop may reach top address 4G, need a UINT64 value
  UINT32                  SecondaryDataStackBase;
  UINT32                  SecondaryDataStackSize;

  GetEffCacheSize (&EffCacheSize, &EffWayCacheSize);
  if (EffWayCacheSize == 0) {
    return EFI_DEVICE_ERROR;
  }

  if (DataStackBase % EffWayCacheSize != 0 || DataStackSize % EffWayCacheSize != 0) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (NemBufferList, sizeof (NEM_BUFFER_LIST));

  //
  // The first NEM Buffer should be set to DataStack
  //
  NemBufferList->Count = 1;
  NemBufferList->NemBufferList[0].NemBufferType = CacheWriteBack;
  NemBufferList->NemBufferList[0].NemBufferBase = DataStackBase;
  NemBufferList->NemBufferList[0].NemBufferSize = DataStackSize;

  //
  // Add secondary data stack for FSP-M decompression if configured
  //
  if (PcdGet32 (PcdSecondaryDataStackSize) != 0) {
    SecondaryDataStackBase = PcdGet32 (PcdSecondaryDataStackBase);
    SecondaryDataStackSize = PcdGet32 (PcdSecondaryDataStackSize);

    // Validate alignment
    if (SecondaryDataStackBase % EffWayCacheSize != 0 || SecondaryDataStackSize % EffWayCacheSize != 0) {
      DEBUG ((DEBUG_ERROR, "Secondary data stack not aligned to EffWayCacheSize: Base=0x%x, Size=0x%x, EffWayCacheSize=0x%x\n",
              SecondaryDataStackBase, SecondaryDataStackSize, EffWayCacheSize));
      return EFI_INVALID_PARAMETER;
    }

    NemBufferList->NemBufferList[NemBufferList->Count].NemBufferType = CacheWriteBack;
    NemBufferList->NemBufferList[NemBufferList->Count].NemBufferBase = SecondaryDataStackBase;
    NemBufferList->NemBufferList[NemBufferList->Count].NemBufferSize = SecondaryDataStackSize;
    NemBufferList->Count++;

  }

  for (Index = 0; Index < FlashRegionList->Count; Index ++) {

    CurrentFlashRegionBase = FlashRegionList->FlashRegionList[Index].FlashRegionBase;
    CurrentFlashRegionSize = FlashRegionList->FlashRegionList[Index].FlashRegionSize;
    CurrentFlashRegionType = FlashRegionList->FlashRegionList[Index].TargetNemBufferType;

    LastNemBufferBase      = NemBufferList->NemBufferList[NemBufferList->Count-1].NemBufferBase;
    LastNemBufferSize      = NemBufferList->NemBufferList[NemBufferList->Count-1].NemBufferSize;
    LastNemBufferTop       = (UINT64) LastNemBufferBase + (UINT64) LastNemBufferSize;
    LastNemBufferType      = NemBufferList->NemBufferList[NemBufferList->Count-1].NemBufferType;

    //
    // Only EffWayCacheSize aligned NEM buffer will be created, adjust the flash region
    // [CurrentFlashRegionBase :: CurrentFlashRegionBase + CurrentFlashRegionSize] to align EffWayCacheSize
    // to [AlignedBase :: AlignedBase + AlignedSize].
    //
    AlignedBase = CurrentFlashRegionBase - CurrentFlashRegionBase % EffWayCacheSize;
    AlignedSize = CurrentFlashRegionSize % EffWayCacheSize == 0 ?
                    CurrentFlashRegionSize :
                    CurrentFlashRegionSize - (CurrentFlashRegionSize % EffWayCacheSize) + EffWayCacheSize;
    if ((UINT64) AlignedBase + (UINT64) AlignedSize < (UINT64) CurrentFlashRegionBase + (UINT64) CurrentFlashRegionSize) {
      //
      // EffWayCacheSize aligned buffer is not big enough to cover the whole flash region
      //
      AlignedSize += EffWayCacheSize;
    }
    AlignedTop  = (UINT64) AlignedBase + (UINT64) AlignedSize;

    //
    // The top of new NEM buffer should fall in the region [LastNemBufferTop :: BASE_4GB]
    //
    if (AlignedTop > BASE_4GB || AlignedTop < LastNemBufferTop) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // Compute NEM buffer for current flash region.
    // * If the new NEM buffer Base is below the last NEM buffer Top, that means these two NEM buffers will have overlaps:
    //   - If they should be programed to the same cache type, combine them.
    //   - If they should be programed to different cache types, we don't allow overlaps and report error.
    // * If the new NEM buffer Base equals to the last NEM buffer Top:
    //   - If they should be programed to the same cache type, combine them.
    //   - If they should be programed to different cache types, create a new NEM buffer.
    // * If the new NEM buffer Base above the last NEM buffer Top, that means new NEM buffer should be created:
    //   - We don't need to care cache types now, always create a new NEM buffer.
    //
    if ((UINT64) AlignedBase < LastNemBufferTop) {
      if (CurrentFlashRegionType == LastNemBufferType) {
        if (AlignedTop > LastNemBufferTop) {
          NemBufferList->NemBufferList[NemBufferList->Count-1].NemBufferSize = (UINT32) (AlignedTop - LastNemBufferBase);
        }
      } else {
        return EFI_INVALID_PARAMETER;
      }
    } else {
      if ((UINT64) AlignedBase == LastNemBufferTop && CurrentFlashRegionType == LastNemBufferType) {
        NemBufferList->NemBufferList[NemBufferList->Count-1].NemBufferSize = (UINT32) (AlignedTop - LastNemBufferBase);
      } else {
        NemBufferList->NemBufferList[NemBufferList->Count].NemBufferType = CurrentFlashRegionType;
        NemBufferList->NemBufferList[NemBufferList->Count].NemBufferBase = AlignedBase;
        NemBufferList->NemBufferList[NemBufferList->Count].NemBufferSize = AlignedSize;
        NemBufferList->Count ++;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Change MTRR settings for a given base and size. Expanded base and size will be tried
  when this config failed. Platform should provide upper and lower limits for this auto
  adjustment.

  @param[in]  MtrrBase           Region base address for MTRR config.
  @param[in]  MtrrSize           Region size for MTRR config.
  @param[in]  MtrrType           MTRR cache type.
  @param[in]  MtrrBaseLimit      Lower limit for MTRR base adjustment, must be below 4G address.
  @param[in]  MtrrTopLimit       Upper limit for MTRR base adjustment, should not exceed 4G address.

  @retval EFI_SUCCESS            MTRR has been configured successfully.
  @retval EFI_DEVICE_ERROR       Unexpected error occurs.
  @retval EFI_OUT_OF_RESOURCES   There's no enough MTRR pairs for this config.

**/
EFI_STATUS
NemMtrrConfig (
  IN   UINT32               MtrrBase,
  IN   UINT32               MtrrSize,
  IN   UINT32               MtrrType,
  IN   UINT32               MtrrBaseLimit,
  IN   UINT64               MtrrTopLimit
  )
{
  EFI_STATUS                Status;
  MTRR_SETTINGS             MtrrSetting;
  UINT32                    EffCacheSize;
  UINT32                    EffWayCacheSize;
  UINT32                    Index;
  UINT64                    MtrrTop;

  GetEffCacheSize (&EffCacheSize, &EffWayCacheSize);
  if (EffWayCacheSize == 0) {
    return EFI_DEVICE_ERROR;
  }
  MtrrTop = (UINT64) MtrrBase + (UINT64) MtrrSize;

  Index = 1;
  do {
    ZeroMem (&MtrrSetting, sizeof (MtrrSetting));
    MtrrGetAllMtrrs (&MtrrSetting);

    //
    // Looping to try appropriate Mtrrs by expanding region size with multiple of EffWayCacheSize
    // EffWayCacheSize should always be a power of 2 number.
    //
    MtrrBase = MtrrBase - MtrrBase % (Index * EffWayCacheSize);
    MtrrTop  = MtrrTop % (Index * EffWayCacheSize) == 0 ?
                 MtrrTop :
                 MtrrTop - (MtrrTop % (Index * EffWayCacheSize)) + (Index * EffWayCacheSize);
    if (MtrrBase < MtrrBaseLimit || MtrrTop > MtrrTopLimit) {
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }

    MtrrSize = (UINT32) (MtrrTop - MtrrBase);
    Index    = Index * 2;
    //
    // Calling MtrrSetMemoryAttributeInMtrrSettings() to config MTRRs.
    // EFI_OUT_OF_RESOURCES error may return, which indicates system may not have enough MTRR pairs to do this
    // configuration, then the loop will continue to try with larger range of MTRR base and size within the range of
    // [MtrrBaseLimit :: MtrrTopLimit].
    //
    Status = MtrrSetMemoryAttributeInMtrrSettings (
               &MtrrSetting,
               (UINTN) MtrrBase,
               MtrrSize,
               MtrrType
               );
    if (!EFI_ERROR (Status)) {
      SetVariableMtrrForNemRange (&MtrrSetting.Variables);
    }
  } while (Status == EFI_OUT_OF_RESOURCES);

  return Status;
}

/**
  This API will not perform any load operations, but only calculate if there are enough NEM size
  for a given flash region expect to load to NEM, and config MTRR for this region.
  Load operation will be actually performed by cache mechanism, after MTRR is correctly configured.

  @param[in, out]  FlashRegionsOnNem    Existing list which contains all regions were already
                                        loaded on NEM, caller should be clear of this information.
                                        All flash regions in list should be reorganized in ascending
                                        order on flash region base, overlaps are not allowed between
                                        different flash regions.
                                        On OUT, this list will be updated if this flash region is
                                        allowed to load to NEM.
  @param[in]       NewFlashRegionToNem  Region expected to load to NEM. Caller must make sure this
                                        region has no overlaps with existing regions in FlashRegionsOnNem.
  @param[in]       DataStackBase        The base address of data stack allocated by bios. Must be
                                        aligned to Effective WayCacheSize.
  @param[in]       DataStackSize        The size of data stack allocated by bios. Must be aligned to
                                        Effective WayCacheSize.

  @retval EFI_SUCCESS                   MTRR has been configured successfully.
  @retval EFI_INVALID_PARAMETER         One or more parameters are not valid.
  @retval EFI_OUT_OF_RESOURCES          There's no enough MTRR pairs or no enough cache size.

**/
EFI_STATUS
EFIAPI
AllocateNemForFlashRegion (
  IN OUT  FLASH_REGION_LIST       *FlashRegionsOnNem,
  IN      FLASH_REGION_SEGMENT    *NewFlashRegionToNem,
  IN      UINT32                  DataStackBase,
  IN      UINT32                  DataStackSize
  )
{
  EFI_STATUS                Status;
  UINT32                    Index;
  FLASH_REGION_SEGMENT      FlashRegionTemp;
  FLASH_REGION_SEGMENT      NewFlashRegionToNemTemp;
  NEM_BUFFER_LIST           NemBufferList;
  UINT32                    NemBufferBase;
  UINT32                    NemBufferSize;
  UINT64                    NemBufferTop;
  UINT32                    MtrrBaseLimit;
  UINT64                    MtrrTopLimit;
  FLASH_REGION_LIST         FlashRegionsOnNemTemp;
  UINT32                    SecondaryDataStackBase;
  UINT32                    SecondaryDataStackSize;
  UINT32                    SecondaryDataStackTop;

  if (FlashRegionsOnNem->Count == FLASH_REGION_COUNT_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Copy to Temp and update only when flash region is loaded successfully
  //
  CopyMem ((VOID *) &FlashRegionsOnNemTemp, (VOID *) FlashRegionsOnNem, sizeof (FlashRegionsOnNemTemp));
  CopyMem ((VOID *) &NewFlashRegionToNemTemp, (VOID *) NewFlashRegionToNem, sizeof (NewFlashRegionToNemTemp));

  //
  // Construct new flash region list for new flash region to load.
  // Insert the new flash region into flash region list to get the corresponding NEM buffer list.
  // This new flash region should not overlap with existing flash regions on NEM.
  //
  for (Index = 0; Index < FlashRegionsOnNemTemp.Count; Index ++) {
    if (FlashRegionsOnNemTemp.FlashRegionList[Index].FlashRegionBase > NewFlashRegionToNem->FlashRegionBase) {
      if (FlashRegionsOnNemTemp.FlashRegionList[Index].FlashRegionBase <
        (UINT64) NewFlashRegionToNem->FlashRegionBase + (UINT64) NewFlashRegionToNem->FlashRegionSize) {
          //
          // Overlap is found here!
          //
          DEBUG ((DEBUG_INFO, "Overlaps are not allowed in flash region list!\n"));
          return EFI_INVALID_PARAMETER;
      }

      //
      // Swap
      //
      CopyMem ((VOID *) &FlashRegionTemp, (VOID *) &NewFlashRegionToNemTemp, sizeof (FlashRegionTemp));
      CopyMem ((VOID *) &NewFlashRegionToNemTemp, (VOID *) &(FlashRegionsOnNemTemp.FlashRegionList[Index]), sizeof (NewFlashRegionToNemTemp));
      CopyMem ((VOID *) &(FlashRegionsOnNemTemp.FlashRegionList[Index]), (VOID *) &FlashRegionTemp, sizeof (FlashRegionTemp));
    }
  }

  CopyMem ((VOID *) &(FlashRegionsOnNemTemp.FlashRegionList[FlashRegionsOnNemTemp.Count]), (VOID *) &NewFlashRegionToNemTemp, sizeof (NewFlashRegionToNemTemp));
  FlashRegionsOnNemTemp.Count ++;

  Status = ConvertFlashRegionToNemBuffer (&FlashRegionsOnNemTemp, DataStackBase, DataStackSize, &NemBufferList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Can't convert flash regions to NEM buffers: %r.\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }

  MtrrBaseLimit = DataStackBase + DataStackSize;

  // Consider secondary data stack if configured
  if (PcdGet32 (PcdSecondaryDataStackSize) != 0) {
    SecondaryDataStackBase = PcdGet32 (PcdSecondaryDataStackBase);
    SecondaryDataStackSize = PcdGet32 (PcdSecondaryDataStackSize);
    SecondaryDataStackTop = SecondaryDataStackBase + SecondaryDataStackSize;

    // Update MtrrBaseLimit to account for both data stacks
    if (SecondaryDataStackTop > MtrrBaseLimit) {
      MtrrBaseLimit = SecondaryDataStackTop;
    }
  }

  MtrrTopLimit  = BASE_4GB;

  for (Index = 0; Index < NemBufferList.Count; Index ++) {

    NemBufferBase = NemBufferList.NemBufferList[Index].NemBufferBase;
    NemBufferSize = NemBufferList.NemBufferList[Index].NemBufferSize;
    NemBufferTop  = (UINT64) NemBufferBase + (UINT64) NemBufferSize;

    if (NemBufferList.NemBufferList[Index].NemBufferType != NewFlashRegionToNem->TargetNemBufferType) {
      if (NemBufferTop <= NewFlashRegionToNem->FlashRegionBase) {
        //
        // MtrrBaseLimit should not be lower than a previous NEM buffer TOP with different cache type
        //
        MtrrBaseLimit = (UINT32) NemBufferTop;
        DEBUG ((DEBUG_INFO, "MtrrBaseLimit Changed: 0x%x\n", MtrrBaseLimit));
      }
      if (NemBufferBase > NewFlashRegionToNem->FlashRegionBase) {
        //
        // MtrrTopLimit should not be higher than a later NEM buffer BASE with different cache type
        //
        MtrrTopLimit  = NemBufferBase;
        DEBUG ((DEBUG_INFO, "MtrrTopLimit Changed: 0x%x\n", MtrrTopLimit));
        break;
      }
    }
  }

  Status = NemMtrrConfig (
             NewFlashRegionToNem->FlashRegionBase,
             NewFlashRegionToNem->FlashRegionSize,
             NewFlashRegionToNem->TargetNemBufferType,
             MtrrBaseLimit,
             MtrrTopLimit
             );
  if (!EFI_ERROR (Status)) {
    //
    // NEM has been allocated for this new flash region, update FlashRegionsOnNem.
    //
    CopyMem ((VOID *) FlashRegionsOnNem, (VOID *) &FlashRegionsOnNemTemp, sizeof (FlashRegionsOnNemTemp));
  }

  return Status;
}