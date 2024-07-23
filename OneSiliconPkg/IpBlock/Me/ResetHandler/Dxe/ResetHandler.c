/** @file
  Platform Specific Reset system handler

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeMeLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/HobLib.h>
#include <PchPolicyCommon.h>
#include <Protocol/PlatformSpecificResetHandler.h>
#include <PchResetPlatformSpecific.h>
#include <Register/EspiRegs.h>
#include <PchConfigHobCommon.h>

/**
  Dummy Reset the system

  @param[in] ResetType            Warm or cold
  @param[in] ResetStatus          Possible cause of reset
  @param[in] DataSize             Size of ResetData in bytes
  @param[in] ResetData            Optional Unicode string
**/
VOID
EFIAPI
NullPlatformResetSystem (
  IN EFI_RESET_TYPE               ResetType,
  IN EFI_STATUS                   ResetStatus,
  IN UINTN                        DataSize,
  IN VOID                         *ResetData OPTIONAL
  )
{
  DEBUG ((DEBUG_INFO, "%a was called to skip resets\n", __FUNCTION__));
}


/**
  Print reset message for debug build readability
**/
VOID
PrintResetMessage (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "******************************\n"));
  DEBUG ((DEBUG_INFO, "**    SYSTEM REBOOT !!!     **\n"));
  DEBUG ((DEBUG_INFO, "******************************\n"));
}

/**
  Wait until CSME completes unconfiguration process.

  @retval EFI_SUCCESS             Unconfiguration not in progress
  @retval EFI_UNSUPPORTED         Unconfiguration error
  @retval EFI_TIMEOUT             Timeout exceeded
**/
EFI_STATUS
WaitForUnconfigFinish (
  VOID
  )
{
  EFI_STATUS Status;
  UINT8      UnconfigStatus;
  UINT8      Timeout;

  Timeout = ME_UNCONFIG_TIMEOUT;

  do {
    Status = HeciGetUnconfigureStatus (&UnconfigStatus);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    switch (UnconfigStatus) {
      case ME_UNCONFIG_IN_PROGRESS:
        MicroSecondDelay (1000000);
        break;

      case ME_UNCONFIG_NOT_IN_PROGRESS:
      case ME_UNCONFIG_FINISHED:
        return EFI_SUCCESS;

      default:
        return EFI_UNSUPPORTED;
    }
  } while (Timeout--);

  return EFI_TIMEOUT;
}

/**
   This function sends reset command to PCH for Global Reset.

   @param[in] ResetType     Type of reset to perform.
   @param[in] ResetStatus   Status code of the reset.
   @param[in] DataSize      Size of ResetData, in bytes.
   @param[in] ResetData     Optional Null-terminated string which can be used to introduce a platform specific reset.
**/
VOID
EFIAPI
PlatformResetSystem (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  EFI_GUID *GuidPtr;

  PrintResetMessage ();
  if (ResetType != EfiResetPlatformSpecific || (ResetData == NULL)) {
    return;
  }

  GuidPtr = (EFI_GUID *) ((UINT8 *) ResetData + DataSize - sizeof (EFI_GUID));
  if (!CompareGuid (GuidPtr, &gPchGlobalResetGuid)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "Enable CF9 Global Reset - DXE\n"));
  PmcEnableCf9GlobalReset ();

  //
  // Let ME do global reset if Me Fw is available
  //
  ResetStatus = HeciSendCbmResetRequest (CBM_RR_REQ_ORIGIN_BIOS_POST, CBM_HRR_GLOBAL_RESET);
  if (EFI_ERROR (ResetStatus)) {
    //
    // Check if ME is undergoing unconfiguration. If yes, then wait for the queued message to be executed.
    // Platform should reset during function execution. If not, attempt to do CF9 reset
    //
    WaitForUnconfigFinish ();
  }
  //
  // Allow 1 second delay so CSME can perform the global reset within this period.
  //
  MicroSecondDelay (1000000);
  //
  // ME Global Reset will fail after EOP is sent. Use PCH Reset instead.
  //
  IoWrite8 (R_PCH_IO_RST_CNT, V_PCH_IO_RST_CNT_FULLRESET);

  //
  // BIOS should stop the execution after initiating the system reset
  //
  CpuDeadLoop ();
}

/**
  This function hooks system reset to allow performing correct reset.

  @param[in] Event      A pointer to the Event that triggered the callback.
  @param[in] Context    A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
OnResetHandlerInstall (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                                      Status;
  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PROTOCOL  *ResetHandler;

  Status = gBS->LocateProtocol (
                  &gEdkiiPlatformSpecificResetHandlerProtocolGuid,
                  NULL,
                  (VOID **) &ResetHandler
                  );

  if (!EFI_ERROR (Status)) {
    ResetHandler->RegisterResetNotify (ResetHandler, PlatformResetSystem);
    gBS->CloseEvent (Event);
  }
}

/**
  This function is a ResetHandler entry point.

  @param[in] ImageHandle    Handle for this drivers loaded image protocol.
  @param[in] SystemTable    EFI system table.

  @retval EFI_SUCCESS       Driver initializes successfully
**/
EFI_STATUS
EFIAPI
ResetHandlerEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  VOID                    *Registration;
  PCH_GENERAL_CONFIG_HOB  *GeneralConfigHob;
  EFI_PEI_HOB_POINTERS    HobList;

  HobList.Guid = GetFirstGuidHob (&gPchGeneralConfigHobGuid);
  if (HobList.Guid != NULL) {
    GeneralConfigHob = (PCH_GENERAL_CONFIG_HOB *) GET_GUID_HOB_DATA (HobList.Guid);
    if (GeneralConfigHob->DisableResets == FORCE_ENABLE) {
      gRT->ResetSystem = NullPlatformResetSystem;
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));
  EfiCreateProtocolNotifyEvent (
    &gEdkiiPlatformSpecificResetHandlerProtocolGuid,
    TPL_CALLBACK,
    OnResetHandlerInstall,
    NULL,
    &Registration
    );

  return EFI_SUCCESS;
}