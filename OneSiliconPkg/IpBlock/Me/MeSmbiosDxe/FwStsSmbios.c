/** @file
  Firmware Status Smbios Interface implementation.

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
#include <PiDxe.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeShowBufferLib.h>
#include <Library/MeFwStsLib.h>
#include <Library/MeInfoLib.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/Pci22.h>
#include <MeFwHob.h>
#include <FwStsSmbiosTable.h>

/**
  Create Group Association (Type14) SMBIOS tabale

  @param[in] Smbios               Pointer to SMBIOS protocol

  @retval EFI_SUCCESS             Group Association Table installed successfully
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate required resources
  @retval Others                  Encountered an error
**/
EFI_STATUS
CreateSmbiosTableType14 (
  IN EFI_SMBIOS_PROTOCOL *Smbios
  )
{
  EFI_STATUS          Status;
  SMBIOS_TABLE_TYPE14 *SmbiosTableType14;
  UINTN               Size;
  CHAR8               *StringPtr;
  static const CHAR8  T14String[sizeof (T14_FWSTS_STRING)] = T14_FWSTS_STRING;

  SmbiosTableType14 = NULL;

  ///
  /// Install SMBIOS Type14 - Group Associations Indicator structure
  /// Computed as 5 + (3 bytes for each item in the group).
  /// The user of this structure determines the number of items as (Length -5)/3.
  /// Add 1 bytes as table terminator.
  ///
  Size = sizeof (SMBIOS_TABLE_TYPE14) + AsciiStrSize (T14_FWSTS_STRING) + 1;

  SmbiosTableType14 = AllocateZeroPool (Size);
  if (SmbiosTableType14 == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosTableType14->Hdr.Type        = EFI_SMBIOS_TYPE_GROUP_ASSOCIATIONS;
  SmbiosTableType14->Hdr.Length      = (UINT8) sizeof (SMBIOS_TABLE_TYPE14);
  SmbiosTableType14->Hdr.Handle      = SMBIOS_HANDLE_PI_RESERVED;                // Assign an unused handle.
  SmbiosTableType14->GroupName       = 1;
  SmbiosTableType14->Group->ItemType = PcdGet8 (PcdFwStsSmbiosType);
  StringPtr = ((CHAR8 *) SmbiosTableType14) + SmbiosTableType14->Hdr.Length;
  AsciiStrCpyS (StringPtr, AsciiStrSize (T14_FWSTS_STRING), T14String);

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "SmbiosTableType14 data printed for FWSTS SMBIOS support -\n"));
  ShowBuffer ((UINT8 *) SmbiosTableType14, (UINT32) Size);
  DEBUG_CODE_END ();

  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &SmbiosTableType14->Hdr.Handle,
                     (EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType14
                     );
  ASSERT_EFI_ERROR (Status);

  FreePool (SmbiosTableType14);

  return Status;
}

/**
  Create FWSTS SMBIOS SMBIOS table. Type of this table is determined by Group Association (Type14)
  SMBIOS table.

  @param[in] Smbios               Pointer to SMBIOS protocol

  @retval EFI_SUCCESS             Group Association Table installed successfully
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate required resources
  @retval Others                  Encountered an error
**/
EFI_STATUS
CreateSmbiosTableFwsts (
  IN EFI_SMBIOS_PROTOCOL *Smbios
  )
{
  EFI_STATUS          Status;
  FWSTS_SMBIOS_RECORD *Record;
  UINTN               Size;
  UINT8               Count;
  UINT8               Index;
  ME_FW_HOB           *MeFwHob;
  UINT64              DevicePciCfgBase;
  CHAR8               *StringPtr;

  Record  = NULL;
  MeFwHob = NULL;

  //
  // Get ME FW HOB
  //
  MeFwHob = GetFirstGuidHob (&gMeFwHobGuid);
  if (MeFwHob == NULL) {
    DEBUG ((DEBUG_WARN, "MeFwHob is not available\n"));
  }

  ///
  /// Install SMBIOS Type (PcdFwStsSmbiosType)
  /// Create full FWSTS SMBIOS record.
  /// Adjust size to include variable length strings,
  /// and set string numbers to point to the correct string.
  /// Add 1 bytes as table terminator.
  ///
  Size = sizeof (FWSTS_SMBIOS_RECORD) + (AsciiStrSize (MEI1_FWSTS_STRING) * MAX_HECI_FUNC) + 1;

  Record = AllocateZeroPool (Size);
  if (Record == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Record->SmbiosHeader.Type   = PcdGet8 (PcdFwStsSmbiosType);
  Record->SmbiosHeader.Length = sizeof (FWSTS_SMBIOS_RECORD);
  Record->SmbiosHeader.Handle = SMBIOS_HANDLE_PI_RESERVED; // Assign an unused handle.
  Record->Version             = 1;
  Record->Count               = MAX_HECI_FUNC;

  ///
  /// Copy the data from MeFwHob and update current FWSTS if the device is not disabled.
  /// If the device is disabled, report the FWSTS saved by MeFwHob directly if it exists
  ///
  StringPtr = ((CHAR8 *) Record) + Record->SmbiosHeader.Length;

  for (Count = 0; Count < MAX_HECI_FUNC; Count++) {
    Record->Group[Count].ComponentNumber = (Count + 1);
    DevicePciCfgBase = MeHeciPciCfgBase (gFwStsDeviceList[Count].HeciDev);

    if (PciSegmentRead16 (DevicePciCfgBase + PCI_DEVICE_ID_OFFSET) != 0xFFFF) {
      //
      // Read current FWSTS and report to SMBIOS directly
      //
      for (Index = 0; Index < MAX_FWSTS_REG; Index++) {
        Record->Group[Count].Reg[Index] = PciSegmentRead32 (DevicePciCfgBase + gFwStsOffsetTable[Index]);
      }
    } else if (MeFwHob != NULL) {
      //
      // The device is disabled, report the FWSTS saved by MeFwHob directly
      //
      CopyMem (&Record->Group[Count].Reg, &MeFwHob->Group[Count].Reg, (sizeof (Record->Group[Count].Reg)));
    }

    DEBUG_CODE (
      ShowBuffer ((UINT8 *) &Record->Group[Count], sizeof (FWSTS_SET_RECORD));
      );

    ///
    /// Fill in corresponding string context for current component
    ///
    AsciiStrCpyS (StringPtr, AsciiStrSize (gFwStsDeviceList[Count].String), gFwStsDeviceList[Count].String);
    StringPtr += AsciiStrSize (gFwStsDeviceList[Count].String);
  }

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "FWSTS SMBIOS table OEM type 0x%x data printed - \n", Record->SmbiosHeader.Type));
  ShowBuffer ((UINT8 *) Record, (UINT32) Size);
  DEBUG_CODE_END ();
  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &Record->SmbiosHeader.Handle,
                     (EFI_SMBIOS_TABLE_HEADER *) Record
                     );
  ASSERT_EFI_ERROR (Status);

  FreePool (Record);

  return Status;
}

/**
  Create the FWSTS info to the SMBIOS table using the SMBIOS protocol
  Invoke this routine to add the table entry when all the FWSTS data is finalized.

  @param[in]  Event               - A pointer to the Event that triggered the callback.
  @param[in]  Context             - A pointer to private data registered with the callback

  @retval EFI_SUCCESS             - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES    - if not able to get resources.
  @retval EFI_UNSUPPORTED         - if required DataHub or SMBIOS protocol is not available.
**/
VOID
EFIAPI
AddFwStsSmbiosEntry (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  EFI_STATUS                      Status;
  EFI_SMBIOS_PROTOCOL             *Smbios;

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  gBS->CloseEvent (Event);

  ///
  /// gSiPkgTokenSpaceGuid.PcdFwStsSmbiosType determines the SMBIOS OEM type (0x80 to 0xFF) defined
  /// in SMBIOS, values 0-0x7F will be treated as disable FWSTS SMBIOS reporting. OEM structure type
  /// is dynamic type extracted from the "Type 14 - Group Associations Structure" to avoid conflicts
  /// introduced by hardcode OEM type.
  /// FWSTS structure uses it as SMBIOS OEM type to provide FWSTS information.
  ///
  if (PcdGet8 (PcdFwStsSmbiosType) <= 0x7F) {
    DEBUG ((DEBUG_WARN, "FwStsSmbiosType value is 0x%x, it isn't defined as SMBIOS OEM type range(0x80 to 0xFF).\n", PcdGet8 (PcdFwStsSmbiosType)));
    DEBUG ((DEBUG_WARN, "FWSTS SMBIOS support is disabled now.\n"));
    return;
  }

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (Smbios == NULL) {
    return;
  }

  Status = CreateSmbiosTableType14 (Smbios);
  if (EFI_ERROR (Status)) {
    return;
  }

  CreateSmbiosTableFwsts (Smbios);
}
