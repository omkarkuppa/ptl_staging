/** @file
  HECI Access driver for Dxe and Smm

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PerformanceLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HeciUtilsLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/HeciAccessCoreLib.h>
#include <IndustryStandard/Pci22.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmReadyToLock.h>
#include <Protocol/HeciAccessProtocol.h>
#include <HeciDeviceConfig.h>

#define HECI_ACCESS_DEBUG   "[HECI Access]"

extern BOOLEAN    mGotSmmReadyToLockEvent;

/**
  Get next HECI Access

  @param[in]  This    Pointer to HECI Access protocol instance from which the next instance should be returned.

  @retval Pointer to the next HeciAccess protocol instance
**/
HECI_ACCESS *
EFIAPI
GetNextHeciAccess (
  IN  HECI_ACCESS   *This
  )
{
  if (This == NULL) {
    return NULL;
  }

  return ((HECI_ACCESS_PRIVATE *) This)->HeciAccessNext;
}

/**
  Smm ReadyToLock event notification handler.
  Installed for SMM only.

  Update HECI Memory BAR after PCI enumerator

  @param[in]  Protocol        Points to the protocol's unique identifier.
  @param[in]  Interface       Points to the interface instance.
  @param[in]  NotifyHandle    The handle on which the interface was installed.

  @retval EFI_SUCCESS         Notification handler runs successfully.
 **/
EFI_STATUS
EFIAPI
SmmReadyToLockEventNotify (
  IN  CONST EFI_GUID    *Protocol,
  IN  VOID              *Interface,
  IN  EFI_HANDLE        NotifyHandle
  )
{
  HECI_ACCESS   *HeciAccess;
  UINTN         MemoryBar;
  UINTN         Counter;

  DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG));
  DEBUG ((DEBUG_INFO, " %a () - Start.\n", __FUNCTION__));

  Counter = 0;

  if (!mGotSmmReadyToLockEvent) {
    HeciAccess = GetFirstHeciAccess ();
    while (HeciAccess != NULL) {
      MemoryBar = GetHeciBar ((HECI_ACCESS_PRIVATE *) HeciAccess);
      if (MemoryBar != 0) {
        ((HECI_ACCESS_PRIVATE *) HeciAccess)->DeviceConfig.TempMemoryBar = MemoryBar;
        Counter++;
      }
      HeciAccess = HeciAccess->GetNextHeciAccess (HeciAccess);
    }
    // Lock Heci MemoryBar configuration
    mGotSmmReadyToLockEvent = TRUE;
  }

  DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG));
  DEBUG ((DEBUG_INFO, " %a () - End (updated %d Memory Bar(s)).\n", __FUNCTION__, Counter));

  return EFI_SUCCESS;
}

/**
  HECI driver entry point used to initialize support for the HECI device.

  @param[in]  ImageHandle         Standard entry point parameter.
  @param[in]  SystemTable         Standard entry point parameter.

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES    Can't allocate PPI data
**/
EFI_STATUS
EFIAPI
HeciAccessEntryPoint (
  IN  EFI_HANDLE          ImageHandle,
  IN  EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_SMM_SYSTEM_TABLE2   *Smst2;
  HECI_ACCESS_PRIVATE     *HeciAccess;
  HECI_DEVICE_CONFIG      *HeciDeviceConfig;
  HECI_ACCESS_PRIVATE     *HeciAccessRoot;
  HECI_ACCESS_PRIVATE     *HeciAccessPrevious;
  EFI_HOB_GUID_TYPE       *GuidHob;
  EFI_GUID                *SelectedHeciGuid;
  BOOLEAN                 InstalledHeciAccessCount;
  BOOLEAN                 IsSmm;
  VOID                    *Registration;

  Status                    = EFI_SUCCESS;
  HeciAccessRoot            = NULL;
  HeciAccessPrevious        = NULL;
  InstalledHeciAccessCount  = 0;
  IsSmm                     = MeIsInSmm (&Smst2);
  SelectedHeciGuid          = IsSmm ? &gHeciAccessSmmProtocolGuid : &gHeciAccessProtocolGuid;

  DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG));
  DEBUG ((DEBUG_INFO, " %a () - Start\n", __FUNCTION__));

  GuidHob = GetFirstGuidHob (&gHeciDeviceConfigHobGuid);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "No HECI devices configurations found!\n"));
    return EFI_ABORTED;
  }

  //
  // Install HECI access protocol
  //
  while (GuidHob != NULL) {
    HeciDeviceConfig = (HECI_DEVICE_CONFIG *) (GET_GUID_HOB_DATA (GuidHob));
    HeciAccess = NULL;

    HeciAccess = HeciAllocateMemory (sizeof (HECI_ACCESS_PRIVATE));
    if (HeciAccess == NULL) {
      DEBUG ((DEBUG_ERROR, HECI_ACCESS_DEBUG));
      DEBUG ((DEBUG_ERROR, " Memory allocation failed!\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    Status = HeciSetNewDevice (HeciAccess, HeciDeviceConfig);
    if (!EFI_ERROR (Status)) {
      Status = InstallHeciProtocol (
                 SelectedHeciGuid,
                 (VOID *) HeciAccess
                 );
      if (!EFI_ERROR (Status)) {
        if (HeciAccessRoot == NULL) {
          HeciAccessRoot = HeciAccess;
        }
        HeciAccess->HeciAccessRoot = HeciAccessRoot;
        if (HeciAccessPrevious != NULL) {
          HeciAccessPrevious->HeciAccessNext = HeciAccess;
        }
        HeciAccessPrevious = HeciAccess;
        InstalledHeciAccessCount++;
      } else {
        DEBUG ((DEBUG_ERROR, HECI_ACCESS_DEBUG));
        DEBUG ((DEBUG_ERROR, " Protocol installation failed!\n"));
        HeciFreeMemory (HeciAccess);
        HeciAccess = NULL;
      }
    } else {
      HeciFreeMemory (HeciAccess);
      HeciAccess = NULL;
    }

    GuidHob = GET_NEXT_HOB (GuidHob);
    GuidHob = GetNextGuidHob (&gHeciDeviceConfigHobGuid, GuidHob);
  }

  if (IsSmm) {
    // Register ReadyToLockEvent notification handler for MemoryBar update
    Registration = NULL;
    Status = Smst2->SmmRegisterProtocolNotify (
                      &gEfiSmmReadyToLockProtocolGuid,
                      SmmReadyToLockEventNotify,
                      &Registration
                      );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, HECI_ACCESS_DEBUG));
      DEBUG ((DEBUG_ERROR, " Register protocol notify failed!\n"));
    }
  }

  DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG));
  DEBUG ((DEBUG_INFO, " Installed %d Protocols\n", InstalledHeciAccessCount));
  DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG));
  DEBUG ((DEBUG_INFO, " %a () - End\n", __FUNCTION__));

  return EFI_SUCCESS;
}
