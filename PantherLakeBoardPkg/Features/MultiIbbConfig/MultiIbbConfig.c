/** @file
  Configuration for Multi-IBB services.

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

#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MtrrLib.h>
#include <Library/BaseLib.h>
#include <Core/Pei/PeiMain.h>
#include <Include/Ppi/MultiIbbConfig.h>
#include <Register/GenerationMsr.h>
#include <Library/NemConfigLib.h>
#include <Library/AccessNemConfigLib.h>
#include <Library/NemMapLib.h>
#include <Ppi/MultiIbbConfig.h>

#define COMMON_IBB_BASE \
  FixedPcdGet32 (PcdFlashFvFspTBase)

#if FixedPcdGetBool (PcdFspWrapperResetVectorInFsp) == 1
  #define COMMON_IBB_SIZE \
  FixedPcdGet32 (PcdFlashFvPreMemorySize) + FixedPcdGet32 (PcdFlashFvFspTSize) + FixedPcdGet32 (PcdFlashFvFspOSize)
#else
  #define COMMON_IBB_SIZE \
  FixedPcdGet32 (PcdFlashFvPreMemorySize) + FixedPcdGet32 (PcdFlashFvFspTSize)
#endif

EFI_STATUS
EFIAPI
MtrrRangeChangeNotifyCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN VOID                      *Ppi
  );

EFI_STATUS
EFIAPI
NemSizeCalculateNotifyCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN VOID                      *Ppi
  );

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR mMtrrRangeChangeNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gMtrrRangeChangePpiGuid,
  MtrrRangeChangeNotifyCallback
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR mNemSizeCalculateNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gNemSizeCalculatePpiGuid,
  NemSizeCalculateNotifyCallback
};

GLOBAL_REMOVE_IF_UNREFERENCED MULTI_IBB_INFO  MultiIbbInfo[MULTI_IBB_INFO_COUNT] = {
#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 0
  {
    FixedPcdGet32 (PcdFlashFvIbbnp1Base),                   // Base
    FixedPcdGet32 (PcdFlashFvIbbnp1Size),                   // Length
    3,                                                      // IbbIndex
    &gFvIbbnp1Sha384HashGuid,                               // HashGuid
    TRUE,                                                   // CurrentlyCached
    TRUE,                                                   // FvInstallNeeded
    IBB_CACHE_AGENT_BIOS,                                   // IbbCacheAgent
    CacheWriteProtected                                     // CacheType
  }
#else
  {
    FixedPcdGet32 (PcdFlashFvPreMemoryBase),                // Base
    FixedPcdGet32 (PcdFlashFvPreMemorySize),                // Length
    0,                                                      // IbbIndex
    NULL,                                                   // HashGuid
    TRUE,                                                   // CurrentlyCached
    FALSE,                                                  // FvInstallNeeded
    IBB_CACHE_AGENT_ACM,                                    // IbbCacheAgent
    CacheInvalid                                            // CacheType
  },
  {
    FixedPcdGet32 (PcdFlashFvIbb1Base),                     // Base
    FixedPcdGet32 (PcdFlashFvIbb1Size),                     // Length
    1,                                                      // IbbIndex
    &gFvIbb1Sha384HashGuid,                                 // HashGuid
    FALSE,                                                  // CurrentlyCached
    TRUE,                                                   // FvInstallNeeded
    IBB_CACHE_AGENT_BIOS,                                   // IbbCacheAgent
    CacheWriteProtected                                     // CacheType
  },
#if FixedPcdGetBool (PcdSignedFspEnable) == 1
  {
    FixedPcdGet32 (PcdFlashFvFspMBase),                     // Base
    FixedPcdGet32 (PcdFlashFvFspMSize),                     // Length
    2,                                                      // IbbIndex
    &gFvFspMSha384HashGuid,                                 // HashGuid
    FALSE,                                                  // CurrentlyCached
    FALSE,   //It's installed by FspmWrapperPeim.efi        // FvInstallNeeded
    IBB_CACHE_AGENT_BIOS,                                   // IbbCacheAgent
    CacheWriteBack                                          // CacheType
  },
#else
  {
    FixedPcdGet32 (PcdFlashFvFspMBase),                     // Base
    FixedPcdGet32 (PcdFlashFvFspMSize),                     // Length
    2,                                                      // IbbIndex
    &gFvFspMSha384HashGuid,                                 // HashGuid
    FALSE,                                                  // CurrentlyCached
    FALSE,   //It's installed by FspmWrapperPeim.efi        // FvInstallNeeded
    IBB_CACHE_AGENT_BIOS,                                   // IbbCacheAgent
    CacheWriteProtected                                     // CacheType
  },
#endif
  {
    FixedPcdGet32 (PcdFlashFvIbbnp1Base),                   // Base
    FixedPcdGet32 (PcdFlashFvIbbnp1Size),                   // Length
    3,                                                      // IbbIndex
    &gFvIbbnp1Sha384HashGuid,                               // HashGuid
    FALSE,                                                  // CurrentlyCached
    TRUE,                                                   // FvInstallNeeded
    IBB_CACHE_AGENT_BIOS,                                   // IbbCacheAgent
    CacheWriteProtected                                     // CacheType
  }
#endif
};

/**
  Install Multi-IBB info Ppi

  @retval EFI_SUCCESS     MultiIbbHashPpi install successfully
**/
EFI_STATUS
InitMultiIbbInfo (
  VOID
  )
{
  PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI   *MultiIbbHashPpi;
  EFI_PEI_PPI_DESCRIPTOR                   *PpiDescriptor;
  EFI_STATUS                               Status;
  UINT32                                   TempRamSize;

  DEBUG ((DEBUG_INFO, "InitMultiIbbInfo enter\n"));
  DEBUG ((DEBUG_INFO, "There are %d IBBs\n", FixedPcdGet32(PcdMultiIbbInfoCount)));

  MultiIbbHashPpi = (PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI *) AllocateZeroPool (sizeof (PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI));

  if (MultiIbbHashPpi == NULL) {
    DEBUG ((DEBUG_INFO, "Failing to allocate resources for MultiIbbHashPpi\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  GetTempRamBaseAndSize (NULL, &TempRamSize);
  CopyMem ((void*) (MultiIbbHashPpi->MultiIbbInfo), (void*) MultiIbbInfo, sizeof (MultiIbbInfo));
  MultiIbbHashPpi->CommonIbbBase = COMMON_IBB_BASE;
  MultiIbbHashPpi->CommonIbbSize = COMMON_IBB_SIZE;
  MultiIbbHashPpi->DataStackSize = TempRamSize;

  // Install MultiIbbHashPpi
  PpiDescriptor = AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PpiDescriptor == NULL) {
    DEBUG ((EFI_D_ERROR, "Failing to allocate resources for PpiDescriptor\n"));
    FreePool (MultiIbbHashPpi);
    return EFI_OUT_OF_RESOURCES;
  }

  PpiDescriptor->Guid  = &gPeiFirmwareVolumeVerifyHashFvPpiGuid;
  PpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PpiDescriptor->Ppi   = (VOID *) MultiIbbHashPpi;

  Status = PeiServicesInstallPpi (PpiDescriptor);
  DEBUG ((DEBUG_INFO, "Install MultiIbbHashPpi\n"));
  if (EFI_ERROR (Status)) {
    FreePool (PpiDescriptor);
    FreePool (MultiIbbHashPpi);
  }
  return Status;
}

/**
  Callback for accessing SetVariableMtrrForNemRange() to change NEM Range.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The NEM_DATA_INPUT_STRUCT contains necessary parameters for function call.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_INVALID_PARAMETER   The Ppi is NULL.
**/
EFI_STATUS
EFIAPI
MtrrRangeChangeNotifyCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN VOID                      *Ppi
  )
{
  NEM_DATA_INPUT_STRUCT          *DataStruct;

  DEBUG ((DEBUG_INFO, "%a to access BaseNemConfigLib\n", __FUNCTION__));
  if (Ppi == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  DataStruct = (NEM_DATA_INPUT_STRUCT*)Ppi;

  // DataStruct->Data1 stands for &MtrrSetting.Variables
  SetVariableMtrrForNemRange ((MTRR_VARIABLE_SETTINGS *)DataStruct->Data1);

  return EFI_SUCCESS;
}

/**
  Callback for accessing NemSizeCalculate() to calculate remaining NEM size.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The NEM_DATA_INPUT_STRUCT contains necessary parameters for function call.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_INVALID_PARAMETER   The Ppi is NULL.
**/
EFI_STATUS
EFIAPI
NemSizeCalculateNotifyCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN VOID                      *Ppi
  )
{
  NEM_DATA_INPUT_STRUCT           *DataStruct;
  UINTN                           RemainedNem;
  UINT32                          TempRamSize;

  DEBUG ((DEBUG_INFO, "%a to access BaseNemConfigLib\n", __FUNCTION__));
  if (Ppi == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  DataStruct = (NEM_DATA_INPUT_STRUCT*) Ppi;
  GetTempRamBaseAndSize (NULL, &TempRamSize);
  RemainedNem = NemSizeCalculate (TempRamSize, COMMON_IBB_SIZE, (UINTN *)DataStruct->Data1); // FvSize = *DataStruct->Data1

  if (DataStruct->Data2 != NULL) {
    *DataStruct->Data2 = RemainedNem;
  }
  return EFI_SUCCESS;
}

/**
  Multi-Ibb Configuration and Initialization.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @retval    EFI_SUCCESS  MultiIbbInfo created and cacheability set-up for Ibb1 successfully
**/
EFI_STATUS
EFIAPI
MultiIbbConfigEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                 Status;
#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 1
  BOOLEAN                    AcmEnable;
#endif

  DEBUG ((DEBUG_INFO, "Enter MultiIbb Config entry point (MultiIbbConfigEntryPoint).\n"));
  Status = InitMultiIbbInfo ();

#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 1
  AcmEnable = FALSE;
  AcmSetNemCheck (&AcmEnable);
  if (AcmEnable == FALSE) {
    DEBUG ((DEBUG_INFO, "Set PcdAcmSetNem as FALSE\n"));
    PcdSetBoolS (PcdAcmSetNem, FALSE);
  }
  Status = PeiServicesNotifyPpi (&mMtrrRangeChangeNotifyList);
  Status = PeiServicesNotifyPpi (&mNemSizeCalculateNotifyList);
#endif
  ASSERT_EFI_ERROR (Status);

  return Status;
}