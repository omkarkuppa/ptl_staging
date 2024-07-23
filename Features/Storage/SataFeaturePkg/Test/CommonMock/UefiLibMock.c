/**@file
    UefiLib mock.

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
#include <cmocka.h>

#include <Library/UefiLib.h>
#include <Library/DebugLib.h>

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
  return mock_type (EFI_STATUS);
}

/**
  Tests whether a child handle is a child device of the controller.

  This function tests whether ChildHandle is one of the children of
  ControllerHandle.  This test is performed by checking to see if the protocol
  specified by ProtocolGuid is present on ControllerHandle and opened by
  ChildHandle with an attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.
  If ProtocolGuid is NULL, then ASSERT().

  @param  ControllerHandle     A handle for a (parent) controller to test.
  @param  ChildHandle          A child handle to test.
  @param  ProtocolGuid         Supplies the protocol that the child controller
                               opens on its parent controller.

  @retval EFI_SUCCESS          ChildHandle is a child of the ControllerHandle.
  @retval EFI_UNSUPPORTED      ChildHandle is not a child of the
                               ControllerHandle.

**/
EFI_STATUS
EFIAPI
EfiTestChildHandle (
  IN CONST EFI_HANDLE  ControllerHandle,
  IN CONST EFI_HANDLE  ChildHandle,
  IN CONST EFI_GUID    *ProtocolGuid
  )
{
  return mock_type (EFI_STATUS);
}

/**
  This function looks up a Unicode string in UnicodeStringTable.

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
  *UnicodeString = mock_type (CHAR16 *);
  return mock_type (EFI_STATUS);
}
