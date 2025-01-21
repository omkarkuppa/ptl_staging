/**@file
  MeSmbios driver

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

@par Specification Reference:
**/

#include "MeSmbiosInternal.h"
#include <Guid/EventGroup.h>
#include <Protocol/MeSmbiosUpdateProtocol.h>
#include <Library/MeUtilsLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  Add an SMBIOS record.

  @param[in]  SmbiosTable       The pointer to the SMBIOS record.
  @param[in]  SmbiosTableSize   The size of the SMBIOS record.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.

**/
EFI_STATUS
AddSmbiosRecord (
  IN VOID   *SmbiosTable,
  IN UINT32 SmbiosTableSize
  )
{
  EFI_STATUS              Status;
  EFI_SMBIOS_PROTOCOL     *Smbios;
  EFI_SMBIOS_TABLE_HEADER *SmbiosRecord;
  EFI_SMBIOS_HANDLE       SmbiosHandle;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  //
  // Add two zeros at the tail. SmbiosAdd will check the last two zeros as terminator.
  //
  SmbiosRecord = AllocateZeroPool (SmbiosTableSize + 2);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (SmbiosRecord, SmbiosTable, SmbiosTableSize);

  Status = Smbios->Add (Smbios, NULL, &SmbiosHandle, SmbiosRecord);

  FreePool (SmbiosRecord);

  return Status;
}

/**
  Add and update tables 130 (if AMT is enabled) and 131 to SMBIOS.

  @param[in]  Event         The Event this notify function registered to.
  @param[in]  Context       Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
UpdateMeSmbiosTable (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                       Status;
  ME_SMBIOS_TABLES_UPDATE_PROTOCOL *MeSmbiosTablesUpdateProtocol;

#if FixedPcdGetBool(PcdAmtEnable) == 1
  CreateSmbiosTableType130 ();
#endif
  CreateSmbiosTableType131 ();

  MeSmbiosTablesUpdateProtocol = NULL;
  Status = gBS->LocateProtocol (&gMeSmbiosTablesUpdateProtocolGuid, NULL, (VOID**)&MeSmbiosTablesUpdateProtocol);
  if (EFI_ERROR (Status)) {
    gBS->CloseEvent (Event);
    return;
  }

#if FixedPcdGetBool(PcdAmtEnable) == 1
  MeSmbiosTablesUpdateProtocol->UpdateSmbios130 ();
#endif
  MeSmbiosTablesUpdateProtocol->UpdateSmbios131 ();

  gBS->CloseEvent (Event);
}

/**
  Initialize ME SMBIOS driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The driver initializes correctly.
  @retval EFI_UNSUPPORTED   The ME doesn't run the client FW.
**/
EFI_STATUS
EFIAPI
MeSmbiosDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  if (!MeSkuIsClient ()) {
    return EFI_UNSUPPORTED;
  }

  //
  // Register End of DXE event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UpdateMeSmbiosTable,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Register Ready to Boot event. SMBIOS Table update event is created with Notify TPL level in order to
  /// ensure that SMBIOS tables are updated before they are measured by TPM (see Extended Measurement
  /// boot for more details).
  ///
  Status = EfiCreateEventReadyToBootEx (
             TPL_NOTIFY,
             AddFwStsSmbiosEntry,
             NULL,
             &Event
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
