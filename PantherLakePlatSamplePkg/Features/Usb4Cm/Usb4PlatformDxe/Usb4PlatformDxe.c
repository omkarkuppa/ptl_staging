/** @file
  USB4 Connection Manager Platform DXE driver.

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

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/TimerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/IUsb4HrLib.h>
#include <Library/TbtCommonLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/ResetNotification.h>
#include <IndustryStandard/Pci22.h>
#include <SetupVariable.h>
#include <TcssInfo.h>
#include <Library/Usb4CmMode.h>
#include <Library/Usb4PlatformHob.h>
#include "Usb4PlatformDxe.h"
#include <ITbtInfoHob.h>

/**
  Check All ITBT FW ready bit is set

  @retval EFI_SUCCESS         - All ITBT controller force power set successfully.
  @retval EFI_NOT_FOUND       - ITbt hob not found.

**/
EFI_STATUS
EFIAPI
CheckAllItbtFPReadyBit (
  VOID
  )
{
  EFI_STATUS     Status;
  UINT8          Index;
  UINT16         Timeout;
  UINT64         DevAddr;
  ITBT_INFO_HOB  *ITbtInfoHob;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  //
  // Check if iTBT exists
  //
  ITbtInfoHob = NULL;
  ITbtInfoHob = GetFirstGuidHob (&gITbtInfoHobGuid);
  if (ITbtInfoHob == NULL) {
    DEBUG ((DEBUG_INFO, "No integrated USB4 host router found!\n"));
    return EFI_NOT_FOUND;
  }

  Timeout = ITbtInfoHob->ITbtForcePowerOnTimeoutInMs;

  //
  // Start set un-force power for all ITBT host router
  //
  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index++) {
    if (0xFFFF != PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, HOST_DMA_BUS_NUM, HOST_DMA_DEV_NUM, HOST_DMA0_FUN_NUM + Index, PCI_VENDOR_ID_OFFSET))) {
      DevAddr = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, HOST_DMA_BUS_NUM, HOST_DMA_DEV_NUM, HOST_DMA0_FUN_NUM + Index, 0);
      Status = ITbtCheckFwReadyBit (DevAddr, Timeout);
      if (Status == EFI_TIMEOUT) {
        DEBUG ((DEBUG_ERROR, "Set power fail (%r) at Integrated USB4 HR %d BDF = (%d,%d,%d)\n",
          Status,
          Index,
          HOST_DMA_BUS_NUM,
          HOST_DMA_DEV_NUM,
          HOST_DMA0_FUN_NUM + Index
          ));
      }
    }
  }
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  This function disable force power all ITBT controller.

  @retval EFI_SUCCESS           - Disable all ITBT controller force power successfully.

**/
EFI_STATUS
EFIAPI
AllItbtForcePowerDisable (
  )
{
  EFI_STATUS  Status;
  UINT8       Index;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  //
  // Start set un-force power for all ITBT host router
  //
  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index++) {
    if (0xFFFF != PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, HOST_DMA_BUS_NUM, HOST_DMA_DEV_NUM, HOST_DMA0_FUN_NUM + Index, PCI_VENDOR_ID_OFFSET))) {
      Status = ITbtUnsetHrForcePower (SA_SEG_NUM, HOST_DMA_BUS_NUM, HOST_DMA_DEV_NUM, HOST_DMA0_FUN_NUM + Index);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Set Un-power fail (%r) at Integrated USB4 HR %d BDF = (%d,%d,%d)\n",
          Status,
          Index,
          HOST_DMA_BUS_NUM,
          HOST_DMA_DEV_NUM,
          HOST_DMA0_FUN_NUM + Index
          ));
        return Status;
      }
    }
  }
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  This function un-power ITBT controller when ExitBootService.

  @param[in] Event   - Event handle.
  @param[in] Context - Context for ExitBootService handler.

**/
VOID
EFIAPI
ItbtSwCmExitBootService (
  EFI_EVENT  Event,
  VOID       *Context
  )
{
  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  AllItbtForcePowerDisable ();

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
   This function un-power ITBT controller before system reset.

   @param[in] ResetType   - Type of reset to perform.
   @param[in] ResetStatus - Status code of the reset.
   @param[in] DataSize    - Size of ResetData, in bytes.
   @param[in] ResetData   - Optional Null-terminated string which can be used to introduce a platform specific reset.

**/
VOID
EFIAPI
ItbtHrPlatformResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID            *ResetData OPTIONAL
  )
{
  DEBUG ((DEBUG_INFO, "%a Entry - ResetType = %x\n", __FUNCTION__, ResetType));

  AllItbtForcePowerDisable ();

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
  This function hooks system reset to allow performing correct reset.

  @param[in] Event   - A pointer to the Event that triggered the callback.
  @param[in] Context - A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
ItbtSwCmResetHandlerInstall (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                       Status;
  EFI_RESET_NOTIFICATION_PROTOCOL  *ResetNotify;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));
  Status = gBS->LocateProtocol (
                  &gEfiResetNotificationProtocolGuid,
                  NULL,
                  (VOID **) &ResetNotify
                  );

  if (!EFI_ERROR (Status)) {
    ResetNotify->RegisterResetNotify (ResetNotify, ItbtHrPlatformResetSystem);
    DEBUG ((DEBUG_INFO, "ItbtHrPlatformResetSystem() registered\n"));
    gBS->CloseEvent (Event);
  }
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
  Register event callback for each USB4 host router instance in platform.

**/
VOID
RegisterSwCmItbtCallback (
  VOID
  )
{
  VOID        *ResetRegistration;
  EFI_STATUS  Status;
  EFI_EVENT   ExitBootServiceEvent;

  DEBUG ((DEBUG_INFO, "Register reset hook for SwCM call back\n"));
  EfiCreateProtocolNotifyEvent (
             &gEfiResetNotificationProtocolGuid,
             TPL_CALLBACK,
             ItbtSwCmResetHandlerInstall,
             NULL,
             &ResetRegistration
             );

  //
  // Create event to stop the disable Force power in the exit boot service.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  ItbtSwCmExitBootService,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &ExitBootServiceEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RegisterSwCmItbtCallback: Create ExitBootService event failure, %r\n", Status));
  }
}

/**
  USB4 Platform Dxe driver entry

  @param[in] ImageHandle     - Handle for the image of this driver
  @param[in] SystemTable     - Pointer to the EFI System Table

  @retval EFI_SUCCESS           - The function completes successfully
**/
EFI_STATUS
EFIAPI
Usb4PlatformDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
#if FixedPcdGetBool(PcdTcssSupport) == 1
  EFI_STATUS          Status;
  USB4_PLATFORM_INFO  Usb4PlatformInfo;

  ZeroMem (&Usb4PlatformInfo, sizeof (Usb4PlatformInfo));

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  Status           = EFI_SUCCESS;

  //
  // Query USB4 platform information to register force power
  //
  Status = Iusb4PlatformInfoQuery (&Usb4PlatformInfo);
  if (Status == EFI_SUCCESS) {
    //
    // Update the CM mode. If the CM Mode is SW CM then turn on the ITBT DXE FP setting.
    // If the CmModeOption is Debug, ITBT switch to SW CM but the USB4 CM driver didn't active.
    // If the CmModeOption is OS Mode, The USB4 CM driver is active with CmMode option is SW CM.
    //
    if (Usb4PlatformInfo.CmModeOption != USB4_SETUP_CM_MODE_DEBUG) {
      if (Usb4PlatformInfo.CmMode == USB4_CM_MODE_SW_CM) {
        //
        // Register reset hook and create event to un-force power in the exit boot service
        //
        RegisterSwCmItbtCallback ();

        //
        // Check all ITBT FW ready bit is set, cause by set all ITBT host router force power in PEI phase
        //
        Status = CheckAllItbtFPReadyBit ();
      }
    }
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return Status;
#else
  return EFI_SUCCESS;
#endif
}