/** @file
  Platform GUID forward implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include "GuidForwardHandlerRuntimeDxe.h"
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <PlatformNvRamHookLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>

/**
  Get a human readable module name and module guid for the given image handle.
  If module name can't be found, "" string will return.
  If module guid can't be found, Zero Guid will return.

  @param[in]  Handle        Image handle or Controller handle.
  @param[out] ModuleGuid    Point to the guid buffer to store the got module guid value.

  @retval EFI_SUCCESS     Successfully get module name and guid.
  @retval EFI_INVALID_PARAMETER  The input parameter NameString is NULL.
  @retval other value  Module Name can't be got.
**/
EFI_STATUS
EFIAPI
GetModuleGuidFromHandle (
  IN EFI_HANDLE        Handle,
  OUT EFI_GUID         *ModuleGuid
  )
{
  EFI_STATUS                  Status;
  EFI_LOADED_IMAGE_PROTOCOL   *LoadedImage;
  EFI_GUID                    *TempGuid;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *FvFilePath;

  Status = EFI_INVALID_PARAMETER;
  LoadedImage     = NULL;

  //
  // Initialize GUID as zero value.
  //
  TempGuid    = &gZeroGuid;

  if (Handle != NULL) {
    //
    // Try Handle as ImageHandle.
    //
    Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID**) &LoadedImage
                  );
  }

  if (!EFI_ERROR (Status) && LoadedImage != NULL) {
    //
    // Get Module Guid from DevicePath.
    //
    if (LoadedImage->FilePath != NULL &&
        LoadedImage->FilePath->Type == MEDIA_DEVICE_PATH &&
        LoadedImage->FilePath->SubType == MEDIA_PIWG_FW_FILE_DP
       ) {
      //
      // Determine GUID associated with module logging performance
      //
      FvFilePath      = (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *) LoadedImage->FilePath;
      TempGuid        = &FvFilePath->FvFileName;
    }
  }

  //
  // Copy Module Guid
  //
  if (ModuleGuid != NULL) {
    CopyGuid (ModuleGuid, TempGuid);
  }

  return Status;
}

/**
  Convert status code value and GUID,  write data to Scratch register.

  @param[in]  CodeType     Indicates the type of status code being reported.
  @param[in]  Value        Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param[in]  Instance     The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param[in]  CallerId     This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param[in]  Data         This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to port code successfully.

**/
EFI_STATUS
EFIAPI
GuidForwardWorker (
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN EFI_GUID                       *CallerId,
  IN EFI_STATUS_CODE_DATA           *Data OPTIONAL
  )
{
  UINT32 PostCodeValue;
  UINT8  Port80;
  UINT8  Port81;
  EFI_STATUS Status;
  UINT64 MchBar;
  UINT64 *ScratchPad;
  EFI_GUID Protocol;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  PostCodeValue = GetPostCodeFromStatusCode (CodeType, Value);

  if ((PostCodeValue == 0) &&
      ((CodeType&EFI_STATUS_CODE_TYPE_MASK) == EFI_DEBUG_CODE) &&
    ((Value&(EFI_STATUS_CODE_CLASS_MASK|EFI_STATUS_CODE_SUBCLASS_MASK)) != EFI_SOFTWARE_DXE_BS_DRIVER) &&
    ((Value&(EFI_STATUS_CODE_CLASS_MASK|EFI_STATUS_CODE_SUBCLASS_MASK)) != EFI_SOFTWARE_SMM_DRIVER)
   ) {
    PostCodeValue = Value;
    PostCode (PostCodeValue);
  }

  if (PostCodeValue != 0) {
    DEBUG ((DEBUG_INFO, "Port80 forward DXE POSTCODE=<%02x>\n", PostCodeValue));
    ///
    /// Saving Post Code value to Scratch Pad
    ///
    ScratchPad  = (UINT64 *) ((UINTN) MchBar + NCDECS_CR_SCRATCHPAD_NCU_0_REG);
    *ScratchPad = PostCodeValue;
  }

  do {
    GetPostCodeBreak( &Port80, &Port81 );
    if (Port80 != DEFAULT_VALUE || Port81 != DEFAULT_VALUE) {
      MicroSecondDelay (100000);
    }
  } while ((PostCodeValue != 0) && ((UINT32)(Port80 | (Port81 << 8)) == PostCodeValue));

  if ((CodeType==EFI_PROGRESS_CODE)&&(Value==(EFI_SOFTWARE_DXE_CORE | EFI_SW_PC_INIT_BEGIN))&&(Data != NULL)) {
    ScratchPad  = (UINT64 *)((UINTN) (Data) + Data->HeaderSize);
      Status = GetModuleGuidFromHandle(*((EFI_HANDLE *)ScratchPad), &Protocol);
      if (!EFI_ERROR (Status)) {
        DEBUG((DEBUG_INFO, "Guid Forward DXE: %g\n", &Protocol));
        ScratchPad  = (UINT64 *) ((UINTN) MchBar + NCDECS_CR_SCRATCHPAD_NCU_2_REG);
        *ScratchPad = Protocol.Data1;
      }
  }

  return EFI_SUCCESS;
}

