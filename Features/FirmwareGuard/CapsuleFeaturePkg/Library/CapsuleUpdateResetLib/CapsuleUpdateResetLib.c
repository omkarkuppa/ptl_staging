/** @file
  Library instance to handle specific reset requirements after firmware update.

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
#include <Library/CapsuleUpdateResetLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PlatformEcResetLib.h>
#include <Library/PlatformWdtLib.h>

#include <Protocol/PlatformSpecificResetHandler.h>
#include <Protocol/PlatformSpecificResetFilter.h>
#include <Protocol/ResetNotification.h>

#include <PchResetPlatformSpecific.h>

#define PLATFORM_SPECIFIC_EC_RESET_STRING         L"EC_RESET"
#define PLATFORM_RESET_DATA_STRING_MAX_LENGTH     16

typedef struct _PLATFORM_RESET_DATA {
  CHAR16   Description[PLATFORM_RESET_DATA_STRING_MAX_LENGTH];
  EFI_GUID Guid;
} PLATFORM_RESET_DATA;

UINT8                                             mWdtTimeOut = 0;

/**
  Change ResetType to EfiResetPlatformSpecific with gPchGlobalResetGuid to indicate a PCH global reset.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
PlatformResetFilterGlobalReset (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  PCH_RESET_DATA              NewResetData;

  //
  // The assumption is that EC reset should be deeper than PCH global reset.
  // So if ResetType is already EfiResetPlatformSpecific, return directly without checking ResetData.
  //
  if (ResetType == EfiResetPlatformSpecific) return;

  DEBUG ((DEBUG_INFO, "CapsuleUpdateResetLib: PlatformResetFilterGlobalReset() Executed\n"));
  DataSize = sizeof (PCH_RESET_DATA);
  CopyMem (&NewResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
  StrCpyS (NewResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);

  gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, DataSize, &NewResetData);
}

/**
  Change ResetType to EfiResetPlatformSpecific with gPlatformEcResetGuid to indicate a platform EC reset.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
PlatformResetFilterEcReset (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  PLATFORM_RESET_DATA         NewResetData;
  EFI_GUID                    *GuidPtr;

  //
  // The assumption is that EC reset is the deepest reset and should override other platform
  // platform specific reset type like PCH global reset.
  //
  if ((ResetType == EfiResetPlatformSpecific) && (ResetData != NULL)) {
    GuidPtr = (EFI_GUID *) ((UINT8 *) ResetData + DataSize - sizeof (EFI_GUID));
    if (CompareGuid (GuidPtr, &gPlatformEcResetGuid)) {
      return;
    }
  }

  DEBUG ((DEBUG_INFO, "CapsuleUpdateResetLib: PlatformResetFilterEcReset() Executed\n"));
  DataSize = sizeof (PLATFORM_RESET_DATA);
  CopyMem (&NewResetData.Guid, &gPlatformEcResetGuid, sizeof (EFI_GUID));
  StrCpyS (NewResetData.Description, PLATFORM_RESET_DATA_STRING_MAX_LENGTH, PLATFORM_SPECIFIC_EC_RESET_STRING);

  gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, DataSize, &NewResetData);
}

/**
  Sent EC reset command to reset EC in normal mode

  @param[in] ResetType            Warm or cold
  @param[in] ResetStatus          Possible cause of reset
  @param[in] DataSize             Size of ResetData in bytes
  @param[in] ResetData            Optional Unicode string
**/
VOID
EFIAPI
PlatformEcResetHandler (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  EFI_GUID            *GuidPtr;

  if (ResetType != EfiResetPlatformSpecific) return;
  if (ResetData == NULL) return;

  GuidPtr = (EFI_GUID *) ((UINT8 *) ResetData + DataSize - sizeof (EFI_GUID));
  if (CompareGuid (GuidPtr, &gPlatformEcResetGuid)) {
    DEBUG ((DEBUG_INFO, "CapsuleUpdateResetLib: PlatformEcReset\n"));
    PlatformEcReset ();
  }
}

/**
  Hook the system reset with intended reset type.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OnResetFilterInstall (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{
  EFI_STATUS                                      Status;
  EDKII_PLATFORM_SPECIFIC_RESET_FILTER_PROTOCOL   *ResetFilter;

  DEBUG ((DEBUG_INFO, "CapsuleUpdateResetLib OnResetFilterInstall() entry\n"));

  Status = gBS->LocateProtocol (
              &gEdkiiPlatformSpecificResetFilterProtocolGuid,
              NULL,
              (VOID **) &ResetFilter
              );

  if (EFI_ERROR (Status)) {
    return;
  }

  if (Context == NULL) {
    DEBUG ((DEBUG_ERROR, "Null Context is unexpected.\n"));
  } else {
    Status = ResetFilter->RegisterResetNotify (ResetFilter, (EFI_RESET_SYSTEM) (UINTN) Context);
    DEBUG ((DEBUG_INFO, "Register ResetFilter notify with Context 0x%x, %r\n", Context, Status));
  }

  gBS->CloseEvent (Event);
}

/**
  Hook the system reset to platform specific reset function.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OnResetHandlerInstall (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{
  EFI_STATUS                                     Status;
  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PROTOCOL *ResetHandler;

  DEBUG ((DEBUG_INFO, "CapsuleUpdateResetLib OnResetHandlerInstall() entry\n"));

  Status = gBS->LocateProtocol (
              &gEdkiiPlatformSpecificResetHandlerProtocolGuid,
              NULL,
              (VOID **) &ResetHandler
              );

  if (EFI_ERROR (Status)) {
    return;
  }

  if (Context == NULL) {
    DEBUG ((DEBUG_ERROR, "Null Context is unexpected.\n"));
  } else {
    Status = ResetHandler->RegisterResetNotify (ResetHandler, (EFI_RESET_SYSTEM) (UINTN) Context);
    DEBUG ((DEBUG_INFO, "Register ResetHandler notify with Context 0x%x, %r\n", Context, Status));
  }

  gBS->CloseEvent (Event);
}

/**
  Register a ResetFilter notification function to change system reset type according
  to given CapsuleResetType.

  @param[in] CapsuleResetType   The platform specific reset type that the caller intends
                                to change to. This is different to EFI_RESET_TYPE.

  @retval EFI_SUCCESS           ResetFilter notification is registered successfully.
  @retval Other                 Failed to register ResetFilter notify function.

**/
EFI_STATUS
EFIAPI
CapsuleUpdateRegisterResetFilter (
  UINT8                CapsuleResetType
  )
{
  EFI_STATUS                      Status;
  VOID                            *NotifyContext;
  VOID                            *Registration;

  DEBUG ((DEBUG_INFO, "CapsuleUpdateRegisterResetFilter entry\n"));
  DEBUG ((DEBUG_INFO, "Request reset type is 0x%x\n", CapsuleResetType));

  Status        = EFI_UNSUPPORTED;
  NotifyContext = NULL;
  Registration  = NULL;

  switch (CapsuleResetType) {
    case CAPSULE_UPDATE_RESET_TYPE_GLOBAL_RESET:
      NotifyContext = (VOID *)(UINTN) PlatformResetFilterGlobalReset;

      break;
    case CAPSULE_UPDATE_RESET_TYPE_EC_RESET:
      NotifyContext = (VOID *)(UINTN) PlatformResetFilterEcReset;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Unsupported reset type\n"));
      Status = EFI_UNSUPPORTED;
      break;
  }

  if (NotifyContext != NULL) {
    DEBUG ((DEBUG_INFO, "Register ResetFilterProtocol notify with Context 0x%x\n", NotifyContext));
    EfiCreateProtocolNotifyEvent (
      &gEdkiiPlatformSpecificResetFilterProtocolGuid,
      TPL_CALLBACK,
      OnResetFilterInstall,
      NotifyContext,
      &Registration
      );

    if (Registration != NULL) {
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_UNSUPPORTED;
    }
  }

  return Status;
}

/**
  Register ResetHandler notification function to handle specific platform reset functionality
  according to CapsuleResetType.

  @param[in] CapsuleResetType   The platform specific reset type that the caller intends to support.
                                This is different to EFI_RESET_TYPE.

  @retval EFI_SUCCESS           ResetHandler notification is registered successfully.
  @retval Other                 Failed to register ResetHandler notify function.

**/
EFI_STATUS
EFIAPI
CapsuleUpdateRegisterResetHandler (
  UINT8                CapsuleResetType
  )
{
  EFI_STATUS                      Status;
  VOID                            *NotifyContext;
  VOID                            *Registration;

  DEBUG ((DEBUG_INFO, "CapsuleUpdateRegisterResetHandler entry\n"));
  DEBUG ((DEBUG_INFO, "Request reset type is 0x%x\n", CapsuleResetType));

  Status        = EFI_UNSUPPORTED;
  NotifyContext = NULL;
  Registration  = NULL;

  switch (CapsuleResetType) {
    case CAPSULE_UPDATE_RESET_TYPE_GLOBAL_RESET:
      DEBUG ((DEBUG_INFO, "The global reset handler is handled by PCH ME driver.\n"));
      Status = EFI_SUCCESS;
      break;
    case CAPSULE_UPDATE_RESET_TYPE_EC_RESET:
      NotifyContext = (VOID *)(UINTN) PlatformEcResetHandler;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Unsupported reset type.\n"));
      Status = EFI_UNSUPPORTED;
      break;
  }

  if (NotifyContext != NULL) {
    DEBUG ((DEBUG_INFO, "Register ResetHandlerProtocol notify with Context 0x%x\n", NotifyContext));
    EfiCreateProtocolNotifyEvent (
      &gEdkiiPlatformSpecificResetHandlerProtocolGuid,
      TPL_CALLBACK,
      OnResetHandlerInstall,
      NotifyContext,
      &Registration
      );

    if (Registration != NULL) {
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_UNSUPPORTED;
    }
  }

  return Status;
}

/**
  PlatformWdtResetCallback call back function for Reset notification.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
PlatformWdtResetCallback (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  DEBUG((DEBUG_INFO, "Arm Platform WDT with timeout %d seconds\n", mWdtTimeOut));
  ArmPlatformWdt (mWdtTimeOut);
}

/**
  Register a reset notification to arm platform WDT right before system reset.
  This WDT is intended to deal with the case when BIOS/ACM/uCode is broken
  and BIOS cannot even be loaded after capsule update.

  With the assistance of platform WDT, TopSwap would be set and system would have a chance to recover.

  @param[in] WdtTimeOut       The timeout value (seconds) set to WDT.

  @retval EFI_SUCCESS           ResetNotification is registered successfully.
  @retval Other                 Failed to register ResetNotification function.

**/
EFI_STATUS
EFIAPI
CapsuleUpdateArmWdtBeforeReset (
  UINT8                WdtTimeOut
  )
{
  EFI_STATUS                               Status;
  STATIC EFI_RESET_NOTIFICATION_PROTOCOL   *ResetNotify = NULL;

  mWdtTimeOut = WdtTimeOut;

  Status = gBS->LocateProtocol (&gEfiResetNotificationProtocolGuid, NULL, (VOID **) &ResetNotify);
  if (!EFI_ERROR (Status)) {
    Status = ResetNotify->RegisterResetNotify (ResetNotify, PlatformWdtResetCallback);
    DEBUG ((DEBUG_INFO, "Install PlatforWdtResetCallbacks () - %r \n", Status));
  }

  return Status;
}
