/** @file
  SMM Policy Analyzer (SPA) support functions

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

#if FixedPcdGetBool (PcdSpaEnable) == 1
#include "DgrSmm.h"
#include <Protocol/SmmSpaLogOutputProtocol.h>
#include <Uefi/UefiBaseType.h>
#include "SmmSpa.h"

UINTN            mSpaLogOutputProtocolBase [MAX_PROTOCOL_ENTRIES];
UINT8            mSpaNoHandles;
SPA_CTXT         *gSmmDgrSpaCtxt = NULL;
EFI_HANDLE       gSmmDgrSpaReadyToLockRegistration;

/**
  SPA Log output callback

  @param[in] LogString                   The log string output.
**/
VOID
EFIAPI
AsmSpaLogOutputCb (
  IN  CHAR16                      *LogString
  );

/**
  SPA Log output callback

  @param[in] LogString                   The log string output.
**/
VOID
EFIAPI
SpaLogOutputCb (
  IN  CHAR16                     *LogString
  )
{
  EFI_STATUS                      Status;
  EFI_SPA_LOG_OUTPUT_PROTOCOL     *SpaLogOutputProtocol = NULL;
  UINT8                           Index;

  if (LogString == NULL) {
    DEBUG ((DEBUG_INFO, "SpaLogOutputCb Invalid parameter.\n"));
    return;
  }

  // Invoking Protocol interface as per given entries.
  for (Index = 0; Index < mSpaNoHandles; Index++) {
    SpaLogOutputProtocol = (EFI_SPA_LOG_OUTPUT_PROTOCOL *) mSpaLogOutputProtocolBase [Index];

    if (SpaLogOutputProtocol != NULL) {
      Status = SpaLogOutputProtocol->SpaLogOutput (
                                       SpaLogOutputProtocol,
                                       LogString
                                       );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "SpaLogOutput protocol entry failed with return Status %s.\n", Status));
      }
    }
  }

  return ;
}

/**
  Get SPA Log output protocol entries which are installed.

  @retval EFI_SUCCESS              The function completed successfully
  @retval EFI_NOT_FOUND            Failed to locate Spa log output protocol
  @retval EFI_OUT_OF_RESOURCES     Insufficient resources to create buffer
**/
EFI_STATUS
SpaGetLogProtocolEntries (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINTN                           HandleBufferSize;
  EFI_HANDLE                      *HandleBuffer = NULL;
  UINT8                           Index;
  EFI_SPA_LOG_OUTPUT_PROTOCOL     *SpaLogOutputProtocol = NULL;

  HandleBufferSize = 0;
  mSpaNoHandles    = 0;
  ZeroMem (mSpaLogOutputProtocolBase, sizeof (mSpaLogOutputProtocolBase));

  Status = gSmst->SmmLocateHandle (
                    ByProtocol,
                    &gEfiSpaLogOutputProtocolGuid,
                    NULL,
                    &HandleBufferSize,
                    HandleBuffer
                  );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return EFI_NOT_FOUND;
  }

  HandleBuffer = AllocateZeroPool (HandleBufferSize);
  if (HandleBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gSmst->SmmLocateHandle (
                    ByProtocol,
                    &gEfiSpaLogOutputProtocolGuid,
                    NULL,
                    &HandleBufferSize,
                    HandleBuffer
                    );
  if (EFI_ERROR (Status)) {
    if (HandleBuffer != NULL) {
      FreePool(HandleBuffer);
    }
    return EFI_NOT_FOUND;
  }

  mSpaNoHandles = (UINT8) HandleBufferSize / sizeof (EFI_HANDLE);

  if (mSpaNoHandles > MAX_PROTOCOL_ENTRIES) {
    mSpaNoHandles = MAX_PROTOCOL_ENTRIES;
    DEBUG ((DEBUG_INFO, "mSpaNoHandles are higher than Max handles, hence limited to Max handles.\n"));
  }

  //
  // Get Protocol Entries. One of protocol instance is installed in Intel platform package.
  // OEM can install their owned protocol to get log string.
  //
  for (Index = 0; Index < mSpaNoHandles; Index++) {
    Status = gSmst->SmmHandleProtocol (
                      HandleBuffer [Index],
                      &gEfiSpaLogOutputProtocolGuid,
                      (VOID **)&SpaLogOutputProtocol
                      );
    if (EFI_ERROR (Status)) {
      continue;
    }

    mSpaLogOutputProtocolBase [Index] = (UINTN) SpaLogOutputProtocol;
  }

  if (HandleBuffer != NULL) {
    FreePool(HandleBuffer);
  }

  return EFI_SUCCESS;
}

/**
  Add Images into the Image structure database.

  @param[in]  ImageBase        Address of real image Base.
  @param[in]  ImageSize        Loaded image size.
  @param[in]  LoadedImageBase  Address of loaded image base.
  @param[in]  EntryPoint       Image entry point.
  @param[in]  NameString       Image name string pointer.
  @param[in]  Guid             Image Guid.
  @param[in]  ImageStruct      Pointer to Image structure database.
**/
VOID
AddImageStruct (
  IN UINTN          ImageBase,
  IN UINTN          ImageSize,
  IN UINTN          LoadedImageBase,
  IN UINTN          EntryPoint,
  IN CHAR16         *NameString,
  IN EFI_GUID       *Guid,
  IN IMAGE_STRUCT   *ImageStruct
  )
{
  ImageStruct->ImageBase = ImageBase;
  DEBUG ((DEBUG_INFO, "ImageBase = %x\n", ImageBase));

  ImageStruct->ImageSize = ImageSize;
  DEBUG ((DEBUG_INFO, "ImageSize = %x\n", ImageSize));

  ImageStruct->LoadedImageBase = LoadedImageBase;
  DEBUG ((DEBUG_INFO, "LoadedImageBase = %x\n", LoadedImageBase));

  ImageStruct->EntryPoint = EntryPoint;
  DEBUG ((DEBUG_INFO, "EntryPoint = %x\n", EntryPoint));

  if (NameString != NULL) {
    DEBUG ((DEBUG_INFO, "NameString = %s\n", NameString));
    StrnCpyS (ImageStruct->NameString, NAME_STRING_LENGTH + 1, NameString, NAME_STRING_LENGTH);
  }

  CopyGuid (&ImageStruct->FileGuid, Guid);
  DEBUG ((DEBUG_INFO, "Guid = %x\n", Guid));
}

/**
  Get loaded SMM driver Name details.
  The following methods will be tried orderly:
    1. Extracting from FFS UI section using Image Guid.
    2. Get image name as File name.
    3. Get image name as File path name.

  @param[in]  LoadedImage LoadedImage protocol.
  @param[out] Guid        Guid of the FFS.
  @param[out] CHAR16      Name string pointer.

  @retval EFI_SUCCESS             Unicode name string is stored in the mNameString.
  @retval EFI_NOT_FOUND           Device path or file name not found.
  @retval EFI_INVALID_PARAMETER   Input parameters are not valid
**/
EFI_STATUS
GetDriverNameString (
  IN  EFI_LOADED_IMAGE_PROTOCOL  *LoadedImage,
  OUT EFI_GUID                   *Guid,
  OUT CHAR16                     *mNameString
  )
{
  EFI_STATUS             Status;
  CHAR16                 *NameString;
  UINTN                  StringSize;
  EFI_GUID               *FileName = NULL;

  if ((LoadedImage == NULL) || (Guid == NULL) || (mNameString == NULL)) {
    return EFI_INVALID_PARAMETER;
  } else {
    ZeroMem (Guid, sizeof (EFI_GUID));
    ZeroMem (mNameString, NAME_STRING_LENGTH+1);
  }

  if (DevicePathType (LoadedImage->FilePath) != MEDIA_DEVICE_PATH) {
    return EFI_NOT_FOUND;
  }

  if (DevicePathSubType (LoadedImage->FilePath) == MEDIA_PIWG_FW_FILE_DP) {
    FileName = &((MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *)LoadedImage->FilePath)->FvFileName;
    if (FileName == NULL) {
      return EFI_NOT_FOUND;
    }

    CopyGuid (Guid, FileName);
    //
    // Try to get the image's FFS UI section by image GUID
    //
    NameString = NULL;
    StringSize = 0;
    Status = GetSectionFromAnyFv (
               FileName,
               EFI_SECTION_USER_INTERFACE,
               0,
               (VOID **) &NameString,
               &StringSize
             );
    if (EFI_ERROR (Status)) {
      UnicodeSPrint (mNameString, NAME_STRING_LENGTH, L"%g", FileName);
    } else {
      StrnCpyS (mNameString, NAME_STRING_LENGTH + 1, NameString, NAME_STRING_LENGTH);
      mNameString [NAME_STRING_LENGTH] = 0;
      FreePool (NameString);
    }
  } else if (DevicePathSubType (LoadedImage->FilePath) == MEDIA_FILEPATH_DP) {
    StrnCpyS (
      mNameString,
      NAME_STRING_LENGTH + 1,
      ((FILEPATH_DEVICE_PATH *) LoadedImage->FilePath)->PathName,
      NAME_STRING_LENGTH
    );
  }

  return EFI_SUCCESS;
}

/**
  Get all images loaded at SMM and update the image database structure.

  @retval EFI_SUCCESS              The function completed successfully
  @retval EFI_NOT_FOUND            Failed to locate loaded Image protocol handler
  @retval EFI_OUT_OF_RESOURCES     Insufficient resources to create buffer
**/
EFI_STATUS
SpaGetLoadedImageSmmLock (
  VOID
  )
{
  EFI_STATUS                 Status;
  UINTN                      NoImageHandles;
  UINTN                      HandleBufferSize;
  EFI_HANDLE                 *HandleBuffer;
  UINTN                      Index;
  EFI_LOADED_IMAGE_PROTOCOL  *LoadedImage;
  CHAR16                     NameString [NAME_STRING_LENGTH + 1];
  UINTN                      EntryPoint;
  UINTN                      RealImageBase;
  EFI_GUID                   Guid;
  IMAGE_HANDLE_DB            *ImageHandleDb = NULL;
  UINTN                      ImageStructCount;

  HandleBufferSize = 0;
  HandleBuffer = NULL;

  Status = gSmst->SmmLocateHandle (
                    ByProtocol,
                    &gEfiLoadedImageProtocolGuid,
                    NULL,
                    &HandleBufferSize,
                    HandleBuffer
                  );
  if (Status != EFI_BUFFER_TOO_SMALL) {
  return EFI_NOT_FOUND;
  }

  HandleBuffer = AllocateZeroPool (HandleBufferSize);
  if (HandleBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gSmst->SmmLocateHandle (
                    ByProtocol,
                    &gEfiLoadedImageProtocolGuid,
                    NULL,
                    &HandleBufferSize,
                    HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "SpsGetLoaderImageSmmLock failed to locate loaded Image protocol handler.\n"));
    if (HandleBuffer != NULL) {
      FreePool (HandleBuffer);
    }
    return EFI_NOT_FOUND;
  }

  NoImageHandles = HandleBufferSize / sizeof (EFI_HANDLE);

  //
  // Allocate memory for Image Handler DB structure. Size should be Max number of Image handles of Image structure.
  // Remove one Image structure size since its already considered in image_handle_db.
  //
  ImageHandleDb = AllocateZeroPool (sizeof (IMAGE_HANDLE_DB) + ((NoImageHandles-1) * sizeof (IMAGE_STRUCT)));
  if (ImageHandleDb == NULL) {
    gSmst->SmmFreePool (HandleBuffer);   // Clear Handler buffer which was allocated earlier in this function.
    return EFI_OUT_OF_RESOURCES;
  }

  ImageStructCount = 0;
  ImageHandleDb->ImageStructCountMax = NoImageHandles;
  DEBUG((DEBUG_INFO, "Max no of Image Handles = %d\n", NoImageHandles));

  for (Index = 0; Index < NoImageHandles; Index++) {
    Status = gSmst->SmmHandleProtocol (
                      HandleBuffer [Index],
                      &gEfiLoadedImageProtocolGuid,
                      (VOID **) &LoadedImage);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = GetDriverNameString (LoadedImage, &Guid, NameString);

    EntryPoint = 0;
    RealImageBase = (UINTN) LoadedImage->ImageBase;

    AddImageStruct (
      RealImageBase,
      (UINTN) LoadedImage->ImageSize,
      (UINTN) LoadedImage->ImageBase,
      EntryPoint,
      NameString,
      &Guid,
      &ImageHandleDb->ImageHandles [ImageStructCount]
      );

    ImageStructCount++;
  }

  ImageHandleDb->ImageStructCount = ImageStructCount;
  gSmmDgrSpaCtxt->ImageHandleDb = (UINTN) ImageHandleDb;

  gSmst->SmmFreePool (HandleBuffer);
  return EFI_SUCCESS;
}

/**
  SMM Policy Analyzer (SPA) callback function at SMM Ready to Lock.
  Initializes the SPA Context structure with reserved memory for SPA.

  @param[in] Protocol   Points to the protocol's unique identifier
  @param[in] Interface  Points to the interface instance
  @param[in] Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS   Notification handler runs successfully.
**/
EFI_STATUS
EFIAPI
SpaSmmReadyToLockCallback (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  EFI_STATUS  Status;
  UINTN       *PageTablePool = NULL;
  UINTN       *UsermodePage = NULL;

  // Update for SPA log output protocol no of entries and Entries address
  Status = SpaGetLogProtocolEntries ();
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Failed to get spa log output protocol entries.\n"));
    return Status;
  }

  Status = SpaGetLoadedImageSmmLock ();
  if (EFI_ERROR (Status)) {
    // This is only SMM Driver information. There is no harm if continued.
    // But driver details will not be available in SPA log.
    DEBUG(( DEBUG_ERROR, "Failed to get SMM driver loaded information. Continue in case of Release build.\n"));
    ASSERT_EFI_ERROR (FALSE);
  }
  //
  // Allocate 1 MB of Page table pool for SPA Usage.
  //
  PageTablePool = AllocateZeroPool (SPA_PAGE_TABLE_POOL_SIZE);
  if (PageTablePool == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Allocate 1 Page for user mode page. This is used for passing log data to user mode callback.
  //
  UsermodePage = AllocateZeroPool (EFI_PAGE_SIZE);
  if (UsermodePage == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gSmmDgrSpaCtxt->UsermodePage = (UINTN) UsermodePage;
  gSmmDgrSpaCtxt->SpaPageTablePool = (UINTN) PageTablePool;
  gSmmDgrSpaCtxt->SpaPageTablePoolSize = SPA_PAGE_TABLE_POOL_SIZE;
  gSmmDgrSpaCtxt->SpaLogOutputCallback = (UINTN) AsmSpaLogOutputCb;
  gSmmDgrSpaCtxt->Valid = 1;

  return EFI_SUCCESS;
}
#endif
