/** @file
  DGR driver, which initializes ACPI and SMBIOS

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DgrInfoLib.h>
#include <MeSmbios.h>

/**
  Returns the SMBIOS table with the specified type.

  @param[in]        Type            The type of the SMBIOS table to be searched.
  @param[out]       Table           On Success, the Table pointer contains the pointer of SMBIOS table of given SMBIOS Type.

  @retval EFI_SUCCESS               Record was found.
  @retval EFI_INVALID_PARAMETER     Table pointer is NULL.
  @retval EFI_NOT_FOUND             Record cannot be found.
**/
EFI_STATUS
GetSmbiosTable (
  IN  SMBIOS_TYPE              Type,
  OUT EFI_SMBIOS_TABLE_HEADER  **Table
  )
{
  EFI_STATUS                 Status;
  EFI_SMBIOS_HANDLE          SmbiosHandle;
  EFI_SMBIOS_PROTOCOL        *Smbios;
  EFI_SMBIOS_TABLE_HEADER    *Record;

  if (Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Table = NULL;
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID**) &Smbios
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  do {
    Status = Smbios->GetNext (
                       Smbios,
                       &SmbiosHandle,
                       NULL,
                       &Record,
                       NULL
                       );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
  } while (Record->Type != Type);

  *Table = Record;

  return EFI_SUCCESS;
}

/**
  Call Back function to Update SMBIOS table 131.
  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.
**/
VOID
EFIAPI
UpdateSmbiosType131
(
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  EFI_STATUS                   Status;
  EFI_SMBIOS_TABLE_HEADER      *Record;
  SMBIOS_TABLE_TYPE131         *SmbiosTableType131Data;
  UINT8                        NiftyRockSupport;

  Status = GetSmbiosTable (MESMBIOS_TYPE_131, &Record);
  DEBUG ((DEBUG_INFO, "UpdateSmbiosType131 GetSmbiosTable %r\n",Status));
  if (!EFI_ERROR (Status)) {
    SmbiosTableType131Data = (SMBIOS_TABLE_TYPE131*) Record;
    NiftyRockSupport = NiftyRockSupportLevel();
    if (NiftyRockSupport == NR_PPAM_11_SUPPORT) {
      SmbiosTableType131Data->BiosCapabilities.IsrdIssrCapability = 1;
    }
  }
}

/**
  Entry point for the DGR Acpi Init driver.

  @param[in] ImageHandle         Image handle of this driver.
  @param[in] SystemTable         Global system service table.

  @retval EFI_SUCCESS     Initialization complete.
  @retval EFI_UNSUPPORTED Feature is disabled
  @retval EFI_ERROR       Driver exits abnormally
**/
EFI_STATUS
EFIAPI
DgrDxeInitEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   EndOfDxeEvent;

  DEBUG ((DEBUG_INFO, "DgrDxeInit Entry\n"));
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  UpdateSmbiosType131,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  DEBUG ((DEBUG_INFO, "UpdateSmbiosType131 CreateEventEx %r\n",Status));
  ASSERT_EFI_ERROR (Status);

  return Status;
}
