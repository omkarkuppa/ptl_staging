/** @file
  This file implements platform firmware version information driver.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <IndustryStandard/FirmwareVersionInfo.h>
#include <SiFvi.h>
#include <Library/TelemetryFviLib.h>
#include <Guid/ComponentId.h>
#include <TelemetryCommon.h>

#include "PlatformFirmwareVersion.h"

//
// Arrays for Adapter Information
//
//
// The order of this array must match PLATFORM_VERSION_INDEX
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_6_5_PHAT_VERSION_ELEMENT mPlatformTelemetryVersionData[] = {
  { EDKII_STABLE_VERSION_COMPONENT_ID,     DEFAULT_FVI_TELEMETRY_VERSION, FVI_INTEL_SIGNATURE, },
};

//
// The order of this array must match PLATFORM_VERSION2_INDEX
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_6_5_PHAT_VERSION_ELEMENT mPlatformTelemetryVersion2Data[] = {
  { LAN_PHY_VERSION_COMPONENT_ID,          DEFAULT_FVI_TELEMETRY_VERSION, FVI_INTEL_SIGNATURE, },
  { EC_VERSION_COMPONENT_ID,               DEFAULT_FVI_TELEMETRY_VERSION, FVI_INTEL_SIGNATURE, },
  { GOP_VERSION_COMPONENT_ID,              DEFAULT_FVI_TELEMETRY_VERSION, FVI_INTEL_SIGNATURE, },
  { UNDI_VERSION_COMPONENT_ID,             DEFAULT_FVI_TELEMETRY_VERSION, FVI_INTEL_SIGNATURE, },
};

extern EFI_GUID  *mPlatformFviReadyProtocolGuid; // @todo: remove the extern

/**
  Covert INTEL_FIRMWARE_VERSION to UINT64 Telemetry Version

  @param[in]       A pointer to INTEL_FIRMWARE_VERSION buffer
  @retval          A 64-bit value

**/
UINT64
IntelFwVerToUint64 (
  IN INTEL_FIRMWARE_VERSION *IntelFwVersion
  )
{
  return (((UINT64) LShiftU64 (IntelFwVersion->MajorVersion, VERSION_MAJOR_VERSION_SHIFT)) | \
          ((UINT64) IntelFwVersion->MinorVersion << VERSION_MINOR_VERSION_SHIFT) | \
          ((UINT64) IntelFwVersion->Revision     << VERSION_REVISION_SHIFT) | \
          ((UINT16) IntelFwVersion->BuildNumber));

}

/**
  Get EDKII Version.
**/
VOID
GetEDKIIStableVersion (
  VOID
  )
{
  mPlatformTelemetryVersionData[Edk2Ver].VersionValue = (UINT64)PcdGet8 (PcdEDKIIVersionMajor) << VERSION_MAJOR_VERSION_SHIFT
                                                        | (UINT64)PcdGet8 (PcdEDKIIVersionMinor) << VERSION_MINOR_VERSION_SHIFT
                                                        | (UINT64)PcdGet8 (PcdEDKIIVersionRevision) << VERSION_REVISION_SHIFT
                                                        | (UINT64)PcdGet8 (PcdEDKIIVersionBuild) << VERSION_BUILD_NUM_SHIFT;

}

/**
  Get Platform Firmware Version Information.

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  Pointer to the context data registered to the Event.

**/
VOID
EFIAPI
InitializeFviDataCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                   Status;
  UINT16                       RecordCount;
  UINTN                        Index;
  VOID                         *Interface;
  EFI_SMBIOS_PROTOCOL          *Smbios;
  EFI_SMBIOS_HANDLE            SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER      *Record;
  INTEL_FIRMWARE_VERSION_INFO  *FviItem;
  UINTN                        FviCount;
  CHAR8                        *Buffer;
  CHAR8                        *EcFviString;
  CHAR8                        *LanPhyFviString;
  CHAR8                        *GopFviString;
  CHAR8                        *UndiFviString;

  DEBUG ((DEBUG_INFO, "%a - Start\n", __FUNCTION__));

  Status = gBS->LocateProtocol (mPlatformFviReadyProtocolGuid, NULL, &Interface);

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a - Locate protocol (%g) failed = %r.\n", __FUNCTION__, mPlatformFviReadyProtocolGuid, Status));
    return;
  }

  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (Smbios == NULL || EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Error locating gEfiSmbiosProtocolGuid failure. Status = %r\n", Status));
    return;
  }

  EcFviString     = (CHAR8 *)PcdGetPtr (PcdPlatformFviEcStr);
  LanPhyFviString = (CHAR8 *)PcdGetPtr (PcdPlatformFviLanPhyStr);
  GopFviString    = (CHAR8 *)PcdGetPtr (PcdPlatformFviGop);
  UndiFviString   = (CHAR8 *)PcdGetPtr (PcdPlatformFviUndi);

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  do {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (Record->Type == INTEL_FVI_SMBIOS_TYPE) {

      FviCount = ((SMBIOS_TABLE_TYPE_OEM_INTEL_FVI *) Record)->Count;
      FviItem  = &((SMBIOS_TABLE_TYPE_OEM_INTEL_FVI *) Record)->Fvi[0];
      Buffer = (CHAR8 *) Record + Record->Length;

      for (Index = 0; Index < FviCount; Index++) {
        if (AsciiStrCmp ((CHAR8 *) Buffer, EcFviString) == 0) {
          //
          // EC Fvi
          //
          mPlatformTelemetryVersion2Data[EcVer].VersionValue = IntelFwVerToUint64 (&FviItem[Index].Version);
        }

        if (AsciiStrCmp ((CHAR8 *) Buffer, LanPhyFviString) == 0) {
          //
          // Lan Phy Fvi
          //
          mPlatformTelemetryVersion2Data[LanPhyVer].VersionValue = IntelFwVerToUint64 (&FviItem[Index].Version);
        }

        if (AsciiStrCmp ((CHAR8 *) Buffer, GopFviString) == 0) {
          //
          // Gop Fvi
          //
          mPlatformTelemetryVersion2Data[GopVer].VersionValue = IntelFwVerToUint64 (&FviItem[Index].Version);
        }

        if (AsciiStrCmp ((CHAR8 *) Buffer, UndiFviString) == 0) {
          //
          // Undi Fvi
          //
          mPlatformTelemetryVersion2Data[Undiver].VersionValue = IntelFwVerToUint64 (&FviItem[Index].Version);
        }

        if (FviItem[Index].ComponentName != 0) {
          Buffer += AsciiStrSize (Buffer);
        }
        if (FviItem[Index].VersionString != 0) {
          Buffer += AsciiStrSize (Buffer);
        }
      }
    }
  } while (Status == EFI_SUCCESS);

  RecordCount = sizeof (mPlatformTelemetryVersion2Data) / sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT);
  for (Index = 0; Index < RecordCount; Index++) {
    Status = AppendTelemetryFviBlock (mPlatformTelemetryVersion2Data[Index].ComponentId, mPlatformTelemetryVersion2Data[Index].VersionValue,
                                      mPlatformTelemetryVersion2Data[Index].ProducerId);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ConnectAll Platform Fvi: AppendTelemetryFviBlock returns %r.\n", Status));
      return;
    }
  }

  return;
}


/**
  This is driver entry point to register the notification event.

  @param[in] ImageHandle  A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The initialization finished successfully.
**/
EFI_STATUS
EFIAPI
PlatformFirmwareVersionDxe (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS        Status;
  UINT16            RecordCount;
  UINT16            Index;
  EFI_EVENT         GetPlatformFviEvent;
  VOID              *Registration;

  Index   = 0;
  Status  = EFI_SUCCESS;
  GetPlatformFviEvent = NULL;

  if (PcdGetSize (PcdPlatformFviReadyProtocolGuid) == sizeof (EFI_GUID)) {
    mPlatformFviReadyProtocolGuid = (EFI_GUID *) PcdGetPtr (PcdPlatformFviReadyProtocolGuid);
  }

  // For Version of EC, Lan Phy, please see function InitializeFviDataCallback()
  GetEDKIIStableVersion ();

  RecordCount = sizeof (mPlatformTelemetryVersionData) / sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT);
  for (Index = 0; Index < RecordCount; Index++) {
    Status = AppendTelemetryFviBlock (mPlatformTelemetryVersionData[Index].ComponentId, mPlatformTelemetryVersionData[Index].VersionValue,
                                      mPlatformTelemetryVersionData[Index].ProducerId);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Platform Fvi: AppendTelemetryFviBlock returns %r.\n", Status));
      return Status;
    }
  }

  if (mPlatformFviReadyProtocolGuid != NULL) {

    GetPlatformFviEvent = EfiCreateProtocolNotifyEvent (
                            mPlatformFviReadyProtocolGuid,
                            TPL_CALLBACK,
                            InitializeFviDataCallback,
                            NULL,
                            &Registration
                            );

  }

  if (GetPlatformFviEvent == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Create callback for protocol notify (%g) failed.\n", __FUNCTION__, mPlatformFviReadyProtocolGuid));
  }

  return EFI_SUCCESS;
}
