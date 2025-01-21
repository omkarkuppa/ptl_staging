/** @file
  Source code file for Multi-IBB initialization services

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/MtrrLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Ppi/MultiIbbConfig.h>
#include <Library/AccessNemConfigLib.h>
#include <Register/Intel/Cpuid.h>

/**
  Callback on memory available
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Notify on gEfiPeiMemoryDiscoveredPpiGuid
**/
static EFI_PEI_NOTIFY_DESCRIPTOR mMemDiscoveredNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) MemoryDiscoveredPpiNotifyCallback
};

/**
  Dump raw data.

  @param[in]  Data  raw data
  @param[in]  Size  raw data size

**/
VOID
InternalDumpData (
  IN UINT8   *Data8,
  IN UINTN   DataSize
  )
{
  DEBUG_CODE_BEGIN();

  UINTN      Index;

  for (Index = 0; Index < DataSize; Index++) {
    if (Index % 0x10 == 0) {
      DEBUG ((DEBUG_INFO, "\n%08X:", Index));
    }
    DEBUG ((DEBUG_INFO, "%02X", *Data8++));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG_CODE_END();
}

/**

  This function dump raw data with column format.

  @param  Data  raw data
  @param  Size  raw data size

**/
VOID
InternalDumpHex (
  IN UINT8  *Data,
  IN UINTN  Size
  )
{
  UINTN  Index;
  UINTN  Count;
  UINTN  Left;

  #define COLUMN_SIZE (16 * 3)

  Count = Size / COLUMN_SIZE;
  Left  = Size % COLUMN_SIZE;
  for (Index = 0; Index < Count; Index++) {
    DEBUG ((DEBUG_VERBOSE, "%04x: ", Index * COLUMN_SIZE));
    InternalDumpData (Data + Index * COLUMN_SIZE, COLUMN_SIZE);
    DEBUG ((DEBUG_VERBOSE, "\n"));
  }

  if (Left != 0) {
    DEBUG ((DEBUG_VERBOSE, "%04x: ", Index * COLUMN_SIZE));
    InternalDumpData (Data + Index * COLUMN_SIZE, Left);
    DEBUG ((DEBUG_VERBOSE, "\n"));
  }
}

/**
  Calculate SHA384 Hash

  @param[in]  Data   data
  @param[in]  Size   data size
  @param[out] Digest SHA384 digest
**/
VOID
CreateSha384Hash (
  IN  UINT8     *Data,
  IN  UINTN     Size,
  OUT UINT8     *Digest
  )
{
  UINTN       CtxSize;
  VOID        *HashCtx;

  CtxSize = Sha384GetContextSize ();
  HashCtx = AllocatePool (CtxSize);
  ASSERT (HashCtx != NULL);

  if (HashCtx != NULL){
    Sha384Init (HashCtx);
    Sha384Update (HashCtx, Data, Size);
    Sha384Final (HashCtx, Digest);
    FreePool (HashCtx);

    DEBUG((DEBUG_INFO, "Sha384 Hash  - \n"));
    InternalDumpHex((UINT8 *)(Digest), 48);
  }
  return ;
}

/**
  Get the information of all installed FV for checking
**/
VOID
GetAllFvInfo (
  VOID
  )
{
  EFI_STATUS                 Status;
  UINTN                      FvIndex;
  EFI_PEI_FV_HANDLE          VolumeHandle;
  EFI_FV_INFO                VolumeInfo;

  DEBUG ((DEBUG_INFO, "FV information:\n"));
  for (FvIndex = 0; ; FvIndex++) {
    Status = PeiServicesFfsFindNextVolume (FvIndex, &VolumeHandle);

    Status = PeiServicesFfsGetVolumeInfo (VolumeHandle, &VolumeInfo);
    if (EFI_ERROR (Status)) {
      break;
    }

    DEBUG ((DEBUG_INFO, "FV%d - BaseAddress:%x, size:%x.\n", FvIndex, (UINTN)VolumeInfo.FvStart, VolumeInfo.FvSize));
  }
}

/**
  Check the integrity of the new Ibb before installing it

  @param[in]  IbbIndex  Ibb index
  @param[in]  FvBuffer  Ibb base address
  @param[in]  FvSize    Ibb size

  @retval EFI_SUCCESS     - The integrity of the new Ibb is verified
  @retval EFI_LOAD_ERROR  - New Ibb is corrupted and lead to load error
**/
EFI_STATUS
EFIAPI
FvIntegrityCheck(
  IN UINTN                      IbbIndex,
  IN VOID                       *FvBuffer,
  IN UINTN                      FvSize
  )
{
  EFI_STATUS                                            Status;
  UINT8                                                 Sha384[SHA384_DIGEST_SIZE];
  EFI_PEI_FILE_HANDLE                                   DigestFileHandle;
  VOID                                                  *DigestFileBuffer;
  PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI                *IbbHashInfoPpi;

  Status = PeiServicesLocatePpi (
            &gPeiFirmwareVolumeVerifyHashFvPpiGuid,
            0,
            NULL,
            (VOID **)&IbbHashInfoPpi
            );

  Status = PeiServicesFfsFindFileByName (IbbHashInfoPpi->MultiIbbInfo[IbbIndex].HashGuid,
           (EFI_PEI_FV_HANDLE)(UINTN) FixedPcdGet32 (PcdFlashFvPreMemoryBase), &DigestFileHandle);
  if (!EFI_ERROR (Status)) {
    //
    // Search RAW section.
    //
    Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, DigestFileHandle, &DigestFileBuffer);
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Find the according digest for IBB%d on ROM:\n", IbbIndex));
      InternalDumpData (DigestFileBuffer, SHA384_DIGEST_SIZE);
    }
  }

  if (EFI_ERROR (Status) || (DigestFileBuffer == NULL)) {
    DEBUG ((DEBUG_ERROR, "Failed to located IBB digest file on ROM\n"));
    return Status;
  }

  ZeroMem (Sha384, SHA384_DIGEST_SIZE);
  DEBUG ((DEBUG_INFO, "\nCalculate the SHA384 digest for the upcoming IBB:\n"));
  CreateSha384Hash ((UINT8 *) FvBuffer, FvSize, Sha384);

  //
  // Check the integrity of the FV image
  //
  DEBUG ((DEBUG_INFO, "Check the integrity of the IBB image\n"));
  if (CompareMem (DigestFileBuffer, Sha384, SHA384_DIGEST_SIZE) != 0) {
      DEBUG ((DEBUG_ERROR, "IBB%d loaded from media is corrupted.\n", IbbIndex));
      return EFI_LOAD_ERROR;
  } else {
      DEBUG ((DEBUG_INFO, "-> The integrity of IBB%d is verified.\n", (IbbIndex)));
      return EFI_SUCCESS;
  }

}

/**
  Callback once there is main memory

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                                            Status;
  UINTN                                                 IbbNextIndex;
  PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI                *IbbHashInfoPpi;
  UINTN                                                 FvBaseAddress;
  UINTN                                                 FvSize;

  DEBUG((DEBUG_INFO, "Memory discovery callback to install the last IBB in MultiIbbService.\n"));

  Status = PeiServicesLocatePpi (
            &gPeiFirmwareVolumeVerifyHashFvPpiGuid,
            0,
            NULL,
            (VOID **)&IbbHashInfoPpi
            );

#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 0
  IbbNextIndex = 0; // Only includes FvIbbnp1
  DEBUG ((DEBUG_INFO, "IBB%d as FvIbbn+1 is going to be executed\n", IbbNextIndex));
#else
  for (UINTN Index = 0; Index < MULTI_IBB_INFO_COUNT; Index++ ) {
    if (IbbHashInfoPpi->MultiIbbInfo[Index].CurrentlyCached == FALSE) {
        IbbNextIndex = Index;
        DEBUG ((DEBUG_INFO, "IbbNextIndex is %x.\n", IbbNextIndex));
        break;
    }
  }

  // Find the next IBB waiting for execution, IbbCurrentIndex=0 is Ibb0
  DEBUG ((DEBUG_INFO, "IBB%d is going to be executed\n", IbbNextIndex));
#endif

  if (IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].FvInstallNeeded == TRUE) {
    FvBaseAddress = (UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].Base;
    FvSize = (UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].Length;

    // Cacheability via MTRR is not needed due to memory is up already.
    IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].CurrentlyCached = TRUE;
    Status = FvIntegrityCheck (IbbNextIndex, (VOID *) FvBaseAddress, FvSize);
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Verified the integrity of new IBB.\n"));
      if (IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].FvInstallNeeded == TRUE) {
        FvBaseAddress = (UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].Base;
        FvSize = (UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].Length;
        DEBUG ((DEBUG_INFO, "Install IBB%d - 0x%p, 0x%x\n", IbbNextIndex, FvBaseAddress, FvSize));
        PeiServicesInstallFvInfo2Ppi (
          &(((EFI_FIRMWARE_VOLUME_HEADER *)FvBaseAddress)->FileSystemGuid),
          (VOID *) FvBaseAddress,
          (UINT32) FvSize,
          NULL,
          NULL,
          0
          );
      }
    } else { // Verify error
      DEBUG ((DEBUG_INFO, "New IBB is corrupted, skip it.\n"));
    }
  }
#if FixedPcdGetBool (PcdCacheMissDetectEnable) == 0
  DEBUG ((DEBUG_INFO, "Check the Fv_info again.\n"));
  GetAllFvInfo ();
#endif
  return EFI_SUCCESS;
}

#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 1
EFI_STATUS
EFIAPI
EndOfIbbNotifyCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN VOID                      *Ppi
  );

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR mEndOfIbbNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfIbbSignalPpiGuid,
  EndOfIbbNotifyCallback
};

EFI_PEI_PPI_DESCRIPTOR gEndOfIbbSignalPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfIbbSignalPpiGuid,
  NULL
};

/**
  Change variable MTRR setting for multi-Ibb

  @param[in]  FvBuffer  Ibb base address
  @param[in]  FvSize    Ibb size
  @param[in]  Type      Mtrr cache type

**/
VOID
MtrrChange (
  IN VOID                     *FvBuffer,
  IN UINTN                    FvSize,
  IN MTRR_MEMORY_CACHE_TYPE   Type
  )
{
  EFI_STATUS                  Status;
  MTRR_SETTINGS               MtrrSetting;

  ZeroMem (&MtrrSetting, sizeof (MtrrSetting));
  MtrrGetAllMtrrs (&MtrrSetting);
  DEBUG ((DEBUG_INFO, "Mtrr set attribute\n"));

  Status = MtrrSetMemoryAttributeInMtrrSettings (
                &MtrrSetting,
                (UINTN)FvBuffer,
                FvSize,
                Type
                );
  ASSERT_EFI_ERROR (Status);

  BridgeTo1sLibPpiSetup (MtrrRangeChange, (UINTN *)&MtrrSetting.Variables, NULL, NULL, NULL);
}

#if FixedPcdGetBool (PcdCacheMissDetectEnable) == 1
/**
  This function is mainly for the debugging purpose that allows user to observe if any cache miss happen.
  It cannot be executed together with GetAllFvInfo() or it will cause an error.

  The Cache Miss debugging mechanism is to cache the previous IBB region as WB and corrupted its content,
  so that any callback to this corrupted IBB will be easily observed as system hang.

  @param[in]  CurrentIbbIndex  //The index of current IBB

  @retval EFI_SUCCESS     - The Cache Miss check enabled successfully.
**/
EFI_STATUS
EFIAPI
MultiIbbCacheMissDebugCheck(
  IN UINTN                      CurrentIbbIndex
  )
{
  EFI_STATUS                                            Status;
  VOID                                                  *FvBaseAddress;
  UINTN                                                 FvSize;
  PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI                *IbbHashInfoPpi;


  Status = PeiServicesLocatePpi (
            &gPeiFirmwareVolumeVerifyHashFvPpiGuid,
            0,
            NULL,
            (VOID **)&IbbHashInfoPpi
            );

  if (IbbHashInfoPpi->MultiIbbInfo[CurrentIbbIndex-1].IbbCacheAgent == IBB_CACHE_AGENT_BIOS) {
    // Cache previous IBB as WB
    FvBaseAddress = (VOID *) ((UINTN) IbbHashInfoPpi->MultiIbbInfo[CurrentIbbIndex-1].Base);
    FvSize = (UINTN) (IbbHashInfoPpi->MultiIbbInfo[CurrentIbbIndex-1].Length);

    MtrrChange (FvBaseAddress, FvSize, CacheWriteBack);
    DEBUG ((DEBUG_INFO, "Corrupt previous IBB content to check any unexpected callback happen that leads to cache miss\n"));
    CopyMem ( // Copy the current IBB content to replace previous IBB content
          FvBaseAddress,
          (VOID *) (UINTN) IbbHashInfoPpi->MultiIbbInfo[CurrentIbbIndex].Base,
          ((UINTN) IbbHashInfoPpi->MultiIbbInfo[CurrentIbbIndex-1].Length) // Need to match previous IBB size
    );
  }
  if (CurrentIbbIndex >= 3 && IbbHashInfoPpi->MultiIbbInfo[CurrentIbbIndex-2].IbbCacheAgent == IBB_CACHE_AGENT_BIOS) {
    // Release MTRRs used for previous IBB cache miss check
    FvBaseAddress = (VOID *) (UINTN) IbbHashInfoPpi->MultiIbbInfo[CurrentIbbIndex-2].Base;
    FvSize = (UINTN) IbbHashInfoPpi->MultiIbbInfo[CurrentIbbIndex-2].Length;
    MtrrChange (FvBaseAddress, FvSize, CacheUncacheable);
    WriteBackInvalidateDataCacheRange (FvBaseAddress, FvSize);  // Clflush all cache lines [note: this will not touch data cache]
    DEBUG ((DEBUG_INFO, "Release the MTRRs use for cache miss testing.\n"));
  }
  return Status;
}
#endif

/**
  Cacheability set-up for the upcoming IBB

  @param[in]  IbbIndex   Currently processing IBB index
  @param[in]  CacheType  Cache type for new IBB

  @retval EFI_SUCCESS    - The function completed successfully.
**/
EFI_STATUS
EFIAPI
IbbCacheabilityType(
  IN UINTN                      IbbIndex,
  IN MTRR_MEMORY_CACHE_TYPE     CacheType
  )
{
  EFI_STATUS                                            Status;
  VOID                                                  *FvBaseAddress;
  UINTN                                                 FvSize;
  PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI                *IbbHashInfoPpi;
  MTRR_MEMORY_CACHE_TYPE                                Type;
  UINTN                                                 RemainedNem;
  UINTN                                                 NumberOfGB;

  RemainedNem = 0;
  NumberOfGB = 0;
  Status = PeiServicesLocatePpi (
            &gPeiFirmwareVolumeVerifyHashFvPpiGuid,
            0,
            NULL,
            (VOID **)&IbbHashInfoPpi
            );

  // IBB0 a.k.a. common IBB is always present in cache until the memory is up and the BIOS modules are shadowed.
  // IBB1 ... IBBn are loaded, executed and removed from cache one after another
  // If IbbIndex = 1 means the previous IBB is Ibb0 which doesn't need to be unloaded from cache

  if (IbbIndex >= BIOS_CACHE_SECOND_IBB_INDEX) { // Recover previous MTRR configuration
    DEBUG ((DEBUG_INFO, "Next one is IBB%d, recover previous MTRR setting first.\n", IbbIndex));

    // Previous one is IbbIndex-1
    FvBaseAddress = (VOID *) (UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbIndex-1].Base;
    FvSize = (UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbIndex-1].Length;

    if (IbbHashInfoPpi->MultiIbbInfo[IbbIndex-1].IbbCacheAgent == IBB_CACHE_AGENT_BIOS) {
      // Uncache the previous FV region first
      MtrrChange (FvBaseAddress, FvSize, CacheUncacheable);
      WriteBackInvalidateDataCacheRange (FvBaseAddress, FvSize);  // Clflush all cache lines [note: this will not touch data cache]
    } else {
      DEBUG ((DEBUG_INFO, "Not controlled by Multi-IBB service, don't need to recover its setting.\n"));
    }

    // Debug function for cache miss testing
#if FixedPcdGetBool (PcdCacheMissDetectEnable) == 1
    BridgeTo1sLibPpiSetup (NemSizeCalculation, &FvSize, &RemainedNem, NULL, NULL);
    DEBUG ((DEBUG_INFO, "RemainedNem is %x\n", RemainedNem));
    if (RemainedNem > FvSize) {
      DEBUG ((DEBUG_INFO, "Enough cache to do cache miss examination\n"));
      MultiIbbCacheMissDebugCheck(IbbIndex);
    } else {
      DEBUG ((DEBUG_INFO, "Not enough cache for cache miss testing.\n"));
    }
#endif
  }
  if (IbbIndex > ACM_CACHE_INITIAL_IBB_INDEX && IbbIndex < MULTI_IBB_INFO_COUNT) { // IBB0 is already been handled by ACM.
    Type = MtrrGetMemoryAttribute((UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbIndex].Base);
    DEBUG ((DEBUG_INFO, "Original cache type of this BaseAddress is %d\n", Type));
    if (Type == CacheType) {
      DEBUG ((DEBUG_INFO, "This segment is already cached, don't need to do anything.\n"));
      IbbHashInfoPpi->MultiIbbInfo[IbbIndex].CurrentlyCached = TRUE;
      Status = EFI_SUCCESS;
    } else {
      // Get the current IBB information
      FvBaseAddress = (VOID *) (UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbIndex].Base;
      FvSize = (UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbIndex].Length;
      DEBUG ((DEBUG_INFO, "\nCache ready for IBB%d - BaseAddress:%x, size:%x.\n", IbbIndex, FvBaseAddress, FvSize));
      // Program the cache type for the given FV region
      MtrrChange (FvBaseAddress, FvSize, CacheType);
      BridgeTo1sLibPpiSetup (NemSizeCalculation, &FvSize, &RemainedNem, NULL, NULL);
      IbbHashInfoPpi->MultiIbbInfo[IbbIndex].CurrentlyCached = TRUE;
      Status = FvIntegrityCheck (IbbIndex, FvBaseAddress, FvSize);
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Cached success and verified the integrity of new IBB.\n"));
      } else { // Verify error
        DEBUG ((DEBUG_INFO, "New IBB is corrupted, skip it.\n"));
      }
    }
    return Status;
  }
  return EFI_SUCCESS;
}

/**
  Callback for the cacheability changes before processing the next IBB

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The gEfiEndOfIbbSignalPpiGuid PPI. Not used.

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval Others               - An error occurred when processing the callback
**/
EFI_STATUS
EFIAPI
EndOfIbbNotifyCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS                                            Status;
  UINTN                                                 IbbNextIndex;
  PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI                *IbbHashInfoPpi;
  UINTN                                                 Index;
  UINTN                                                 FvBaseAddress;
  UINTN                                                 FvSize;

  IbbNextIndex = 0;
  DEBUG ((DEBUG_INFO, "!! Callback: the notify function in MultiIbbService (EndOfIbbNotifyCallback)\n"));

  Status = PeiServicesLocatePpi (
            &gPeiFirmwareVolumeVerifyHashFvPpiGuid,
            0,
            NULL,
            (VOID **)&IbbHashInfoPpi
            );

  for (Index = 0; Index < MULTI_IBB_INFO_COUNT; Index++ ) {
    if (IbbHashInfoPpi->MultiIbbInfo[Index].CurrentlyCached == FALSE) {
        IbbNextIndex = Index;
        DEBUG ((DEBUG_INFO, "IbbNextIndex is %x, arrange Variable MTRRs to cache it.\n", IbbNextIndex));
        break;
    }
  }

  // Find the next IBB waiting for execution, IbbCurrentIndex=0 is Ibb0
  if (IbbNextIndex > ACM_CACHE_INITIAL_IBB_INDEX) {
     DEBUG ((DEBUG_INFO, "IBB%d is going to be executed\n", IbbNextIndex));

     Status = IbbCacheabilityType (IbbNextIndex, IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].CacheType);
     if (!EFI_ERROR (Status)) { // Integrity verify success
       if (IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].FvInstallNeeded == TRUE) {
         FvBaseAddress = (UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].Base;
         FvSize = (UINTN) IbbHashInfoPpi->MultiIbbInfo[IbbNextIndex].Length;
         DEBUG ((DEBUG_INFO, "Install IBB%d - 0x%p, 0x%x\n", IbbNextIndex, FvBaseAddress, FvSize));
         PeiServicesInstallFvInfo2Ppi (
            &(((EFI_FIRMWARE_VOLUME_HEADER *)FvBaseAddress)->FileSystemGuid),
            (VOID *) FvBaseAddress,
            (UINT32) FvSize,
            NULL,
            NULL,
            0
            );
       } else {
         DEBUG ((DEBUG_INFO, "This IBB will be installed by other PEIM.\n"));
       }
     } else { // Integrity verify failed
       DEBUG ((DEBUG_INFO, "IBB%d integrity verification failed and don't install\n", IbbNextIndex));
     }
#if FixedPcdGetBool (PcdCacheMissDetectEnable) == 0
       DEBUG ((DEBUG_INFO, "Check the Fv_info again.\n"));
       GetAllFvInfo ();
#endif
  } else {
    DEBUG ((DEBUG_INFO, "Don't need to change any MTRRs setting.\n"));
  }
  return Status;
}

/**
  Adjust initial NEM range to cover Common IBB only if it is not set by ACM at the beginning.

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval Others               - An error occurred when processing the function.
**/
EFI_STATUS
EFIAPI
AdjustInitNemRange (
  VOID
  )
{
  EFI_STATUS                                            Status;
  MTRR_MEMORY_CACHE_TYPE                                CacheType;
  MTRR_SETTINGS                                         CurrentMtrrs;
  MTRR_SETTINGS                                         NewMtrrs;
  UINTN                                                 Index;
  UINTN                                                 AllMtrr;
  UINT64                                                ValidBitsMask;
  UINT64                                                ValidAddressMask;
  UINT64                                                Length;
  UINT64                                                CommonIbbBase;
  UINT64                                                CommonIbbSize;
  VOID                                                  *OldMtrrBase;
  UINT64                                                MtrrUsedSize;
  PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI                *IbbHashInfoPpi;

  DEBUG ((DEBUG_INFO, "%a Begins...\n", __FUNCTION__));

  Index = 0;
  AllMtrr = 0;
  ValidBitsMask = 0;
  ValidAddressMask = 0;
  Length = 0;
  MtrrUsedSize = 0;
  CommonIbbBase = 0;
  CommonIbbSize = 0;

  // Get Current Mtrr Setting
  MtrrGetAllMtrrs (&CurrentMtrrs);
  // Reset New MTRR setting.
  ZeroMem (&NewMtrrs, sizeof (MTRR_SETTINGS));
  // Initialize Mtrr Mask
  BridgeTo1sLibPpiSetup (MtrrInitMask, (UINTN *)&ValidBitsMask, (UINTN *)&ValidAddressMask, NULL, NULL);

  Status = PeiServicesLocatePpi (
          &gPeiFirmwareVolumeVerifyHashFvPpiGuid,
          0,
          NULL,
          (VOID **)&IbbHashInfoPpi
          );

  if (Status != EFI_SUCCESS) {
    return Status;
  }

  // Config Common IBB range to WriteProtected
  CommonIbbBase = IbbHashInfoPpi->CommonIbbBase;
  CommonIbbSize = IbbHashInfoPpi->CommonIbbSize;
  CacheType     = CacheWriteProtected;

  AllMtrr = GetVariableMtrrCount ();
  // Count overall MtrrUsedSize
  for (Index = 1; Index < AllMtrr; Index++) { // Mtrr0 is for Data stack
    Length = ((~(CurrentMtrrs.Variables.Mtrr[Index].Mask & ValidAddressMask)) & ValidBitsMask) + 1;
    MtrrUsedSize += Length;
  }
  DEBUG ((DEBUG_INFO, "All MtrrUsedSize: %x\n", MtrrUsedSize));

  // Mtrr[0] is for Data Stack same as original setting, Mtrr[1] for Common IBB
  NewMtrrs.Variables.Mtrr[0].Base = CurrentMtrrs.Variables.Mtrr[0].Base;
  NewMtrrs.Variables.Mtrr[0].Mask = CurrentMtrrs.Variables.Mtrr[0].Mask;
  NewMtrrs.Variables.Mtrr[1].Base = (CommonIbbBase & ValidAddressMask) | CacheType;
  NewMtrrs.Variables.Mtrr[1].Mask = ((~(CommonIbbSize - 1)) & ValidAddressMask) | BIT11;

  OldMtrrBase = (VOID *) (UINTN) CurrentMtrrs.Variables.Mtrr[1].Base;
  // CurrentMtrrs = &NewMtrrs;
  BridgeTo1sLibPpiSetup (MtrrRangeChange, (UINTN *)&NewMtrrs.Variables, NULL, NULL, NULL);

  // Need to CLFLUSH the rest of non-Common Ibb region
  WriteBackInvalidateDataCacheRange (OldMtrrBase, (UINTN) (MtrrUsedSize - CommonIbbSize));

  DEBUG ((DEBUG_INFO, "%a Finished.\n", __FUNCTION__));
  return EFI_SUCCESS;
}
#endif // PcdMultiIbbFeatureEnable == 1

/**
  Provide Multi-Ibb service.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @retval    EFI_SUCCESS  MultiIbbInfo created and cacheability set-up for Ibb1 successfully
**/
EFI_STATUS
EFIAPI
MultiIbbServiceEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                 Status;

  DEBUG ((DEBUG_INFO, "Enter MultiIBB Service entry point (MultiIbbServiceEntryPoint).\n"));

  // To be notified after the memory is discovered.
  Status = PeiServicesNotifyPpi (&mMemDiscoveredNotifyList);

#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 1
#if FixedPcdGetBool (PcdCacheMissDetectEnable) == 0
    DEBUG ((DEBUG_INFO, "Cache miss detecting PCD is disabled\n"));
#else
    DEBUG ((DEBUG_INFO, "Cache miss detecting PCD is enabled\n"));
    if (PcdGetBool(PcdAcmSetNem) == FALSE) {
      DEBUG ((DEBUG_INFO, "Multi-IBB rearrange MTRRs to cache Common IBB only at initial state.\n"));
      AdjustInitNemRange ();
    }
#endif

  // To be notified at the end of each IBB, and invoke the callback into MtrrChange() function to get ready for next IBB
  Status = PeiServicesNotifyPpi (&mEndOfIbbNotifyList);

  // This install will trigger at end of IBB0 a.k.a. common IBB to facilitate the IBB1 loading
  Status = PeiServicesInstallPpi (&gEndOfIbbSignalPpi);

  ASSERT_EFI_ERROR (Status);
#endif // PcdMultiIbbFeatureEnable

  return Status;
}