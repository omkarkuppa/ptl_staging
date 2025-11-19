/**@file
    UefiLib mock of the I2c Platform Dxe driver.
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

@par Specification
**/
#include <Uefi.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  Mock of UefiDriverModel to Install Driver Binding Protocol.
  @param  ImageHandle          The image handle of the driver.
  @param  SystemTable          The EFI System Table that was passed to the driver's entry point.
  @param  DriverBinding        A Driver Binding Protocol instance that this driver is producing.
  @param  DriverBindingHandle  The handle that DriverBinding is to be installed onto.  If this
                               parameter is NULL, then a new handle is created.
  @param  ComponentName        A Component Name Protocol instance that this driver is producing.
  @param  ComponentName2       A Component Name 2 Protocol instance that this driver is producing.
  @retval EFI_SUCCESS           The protocol installation successfully completed.
  @retval EFI_OUT_OF_RESOURCES  There was not enough memory in pool to install all the protocols.
**/

EFI_STATUS
EFIAPI
EfiLibInstallDriverBindingComponentName2 (
  IN CONST EFI_HANDLE                    ImageHandle,
  IN CONST EFI_SYSTEM_TABLE              *SystemTable,
  IN EFI_DRIVER_BINDING_PROTOCOL         *DriverBinding,
  IN EFI_HANDLE                          DriverBindingHandle,
  IN CONST EFI_COMPONENT_NAME_PROTOCOL   *ComponentName        OPTIONAL,
  IN CONST EFI_COMPONENT_NAME2_PROTOCOL  *ComponentName2       OPTIONAL
  )
{
  *mock_ptr_type (EFI_DRIVER_BINDING_PROTOCOL **) = DriverBinding;
  return EFI_SUCCESS;
}
/**
  Mock of AddUnicodeString2.

  If Language is a member of SupportedLanguages then UnicodeString is added to
  UnicodeStringTable.  New buffers are allocated for both Language and UnicodeString.
  The contents of Language and UnicodeString are copied into these new buffers.
  These buffers are automatically freed when EfiLibFreeUnicodeStringTable() is called.

  @param  Language            A pointer to an ASCII string containing the ISO 639-2 or
                              the RFC 4646 language code for the Unicode string to add.
                              If Iso639Language is TRUE, then this ASCII string is not
                              assumed to be Null-terminated, and only the first three
                              chacters are used. If Iso639Language is FALSE, then this
                              ASCII string must be Null-terminated.
  @param  SupportedLanguages  A pointer to a Null-terminated ASCII string that contains
                              a set of ISO 639-2 or RFC 4646 language codes that the Unicode
                              string table supports.  Language must be a member of this set.
                              If Iso639Language is TRUE, then this string contains one or more
                              ISO 639-2 language codes with no separator characters.
                              If Iso639Language is FALSE, then is string contains one or more
                              RFC 4646 language codes separated by ';'.
  @param  UnicodeStringTable  A pointer to the table of Unicode strings. Type EFI_UNICODE_STRING_TABLE
                              is defined in "Related Definitions".
  @param  UnicodeString       A pointer to the Unicode string to add.
  @param  Iso639Language      Specifies the supported language code format. If it is TRUE,
                              then Language and SupportedLanguages follow ISO 639-2 language code format.
                              Otherwise, they follow RFC 4646 language code format.

  @retval EFI_SUCCESS            The Unicode string that matches the language specified by
                                 Language was found in the table of Unicode strings UnicodeStringTable,
                                 and it was returned in UnicodeString.
  @retval EFI_INVALID_PARAMETER  Language is NULL.
  @retval EFI_INVALID_PARAMETER  UnicodeString is NULL.
  @retval EFI_INVALID_PARAMETER  UnicodeString is an empty string.
  @retval EFI_UNSUPPORTED        SupportedLanguages is NULL.
  @retval EFI_ALREADY_STARTED    A Unicode string with language Language is already present in
                                 UnicodeStringTable.
  @retval EFI_OUT_OF_RESOURCES   There is not enough memory to add another Unicode string UnicodeStringTable.
  @retval EFI_UNSUPPORTED        The language specified by Language is not a member of SupportedLanguages.

**/
EFI_STATUS
EFIAPI
AddUnicodeString2 (
  IN     CONST CHAR8               *Language,
  IN     CONST CHAR8               *SupportedLanguages,
  IN OUT EFI_UNICODE_STRING_TABLE  **UnicodeStringTable,
  IN     CONST CHAR16              *UnicodeString,
  IN     BOOLEAN                   Iso639Language
  )
{
  CHAR16              **CheckString;

  CheckString = mock_ptr_type (CHAR16 **);
  *CheckString = AllocateCopyPool (StrSize (UnicodeString), UnicodeString);
  return mock_type (EFI_STATUS);
}
/**
  Mock of LookupUnicodeString2.

  If Language is a member of SupportedLanguages and a Unicode string is found in
  UnicodeStringTable that matches the language code specified by Language, then
  it is returned in UnicodeString.

  @param  Language             A pointer to an ASCII string containing the ISO 639-2 or the
                               RFC 4646 language code for the Unicode string to look up and
                               return. If Iso639Language is TRUE, then this ASCII string is
                               not assumed to be Null-terminated, and only the first three
                               characters are used. If Iso639Language is FALSE, then this ASCII
                               string must be Null-terminated.
  @param  SupportedLanguages   A pointer to a Null-terminated ASCII string that contains a
                               set of ISO 639-2 or RFC 4646 language codes that the Unicode
                               string table supports.  Language must be a member of this set.
                               If Iso639Language is TRUE, then this string contains one or more
                               ISO 639-2 language codes with no separator characters. If Iso639Language
                               is FALSE, then is string contains one or more RFC 4646 language
                               codes separated by ';'.
  @param  UnicodeStringTable   A pointer to the table of Unicode strings. Type EFI_UNICODE_STRING_TABLE
                               is defined in "Related Definitions".
  @param  UnicodeString        A pointer to the Null-terminated Unicode string from UnicodeStringTable
                               that matches the language specified by Language.
  @param  Iso639Language       Specifies the supported language code format. If it is TRUE, then
                               Language and SupportedLanguages follow ISO 639-2 language code format.
                               Otherwise, they follow RFC 4646 language code format.


  @retval  EFI_SUCCESS            The Unicode string that matches the language specified by Language
                                  was found in the table of Unicode strings UnicodeStringTable, and
                                  it was returned in UnicodeString.
  @retval  EFI_INVALID_PARAMETER  Language is NULL.
  @retval  EFI_INVALID_PARAMETER  UnicodeString is NULL.
  @retval  EFI_UNSUPPORTED        SupportedLanguages is NULL.
  @retval  EFI_UNSUPPORTED        UnicodeStringTable is NULL.
  @retval  EFI_UNSUPPORTED        The language specified by Language is not a member of SupportedLanguages.
  @retval  EFI_UNSUPPORTED        The language specified by Language is not supported by UnicodeStringTable.

**/
EFI_STATUS
EFIAPI
LookupUnicodeString2 (
  IN CONST CHAR8                     *Language,
  IN CONST CHAR8                     *SupportedLanguages,
  IN CONST EFI_UNICODE_STRING_TABLE  *UnicodeStringTable,
  OUT CHAR16                         **UnicodeString,
  IN BOOLEAN                         Iso639Language
  )
{

  *UnicodeString = UnicodeStringTable->UnicodeString;
  return mock_type (EFI_STATUS);
}
/**
  Mock of HidI2cInitialize.

  @param[in]  Handle               Handle of the device

  @retval TRUE      interrupt is active
  @retval FALSE     no interrupt

**/
EFI_STATUS
EFIAPI
HidI2cInitialize (
  IN EFI_HANDLE Handle
  )
{
  //
  // This is the place to add power management or reset sequence if needed
  //
  return EFI_SUCCESS;
}
/**
  Mock of HidI2cCheckInterrupt.

  @param[in]  Handle               Handle of the device whose interrupt is being checked

  @retval TRUE      interrupt is active
  @retval FALSE     no interrupt

**/
BOOLEAN
EFIAPI
HidI2cCheckInterrupt (
  IN EFI_HANDLE Handle
  )
{
  return TRUE;
}
/**
  Mock of HidI2cGetHidRegisterAddress

  @param[in]  Handle               Handle of the device whose HID address is needed.

  @return                          HID descriptor register's address

**/
UINT16
EFIAPI
HidI2cGetHidRegisterAddress (
  IN EFI_HANDLE Handle
  )
{
    return 0xFFFF;
}

EFI_EVENT
EFIAPI
EfiCreateProtocolNotifyEvent (
  IN  EFI_GUID          *ProtocolGuid,
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction,
  IN  VOID              *NotifyContext   OPTIONAL,
  OUT VOID              **Registration
  )
{
  EFI_EVENT Event;
  VOID      *Record;

  Event = NULL;
  Record = AllocatePool (sizeof (UINTN) * 6);
  *Registration = Record;
  Event = AllocatePool (sizeof (EFI_EVENT));

  return Event;
}