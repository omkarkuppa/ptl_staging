/** @file
  Mock instance of HiiLib library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <Library/HiiLib.h>

/**
  Mock HiiAddPackages: Registers a list of packages in the HII Database and returns the HII Handle
  associated with that registration.  If an HII Handle has already been registered
  with the same PackageListGuid and DeviceHandle, then NULL is returned.  If there
  are not enough resources to perform the registration, then NULL is returned.
  If an empty list of packages is passed in, then NULL is returned.  If the size of
  the list of package is 0, then NULL is returned.

  The variable arguments are pointers which point to package header that defined
  by UEFI VFR compiler and StringGather tool.

  #pragma pack (push, 1)
  typedef struct {
    UINT32                  BinaryLength;
    EFI_HII_PACKAGE_HEADER  PackageHeader;
  } EDKII_AUTOGEN_PACKAGES_HEADER;
  #pragma pack (pop)

  @param[in]  PackageListGuid  The GUID of the package list.
  @param[in]  DeviceHandle     If not NULL, the Device Handle on which
                               an instance of DEVICE_PATH_PROTOCOL is installed.
                               This Device Handle uniquely defines the device that
                               the added packages are associated with.
  @param[in]  ...              The variable argument list that contains pointers
                               to packages terminated by a NULL.

  @retval NULL   A HII Handle has already been registered in the HII Database with
                 the same PackageListGuid and DeviceHandle.
  @retval NULL   The HII Handle could not be created.
  @retval NULL   An empty list of packages was passed in.
  @retval NULL   All packages are empty.
  @retval Other  The HII Handle associated with the newly registered package list.

**/
EFI_HII_HANDLE
EFIAPI
HiiAddPackages (
  IN CONST EFI_GUID    *PackageListGuid,
  IN       EFI_HANDLE  DeviceHandle  OPTIONAL,
  ...
  )
{
  check_expected_ptr (PackageListGuid);
  check_expected_ptr (DeviceHandle);

  return mock_ptr_type (EFI_HII_HANDLE);
}

/**
  Mock HiiSetString: This function create a new string in String Package or updates an existing
  string in a String Package.  If StringId is 0, then a new string is added to
  a String Package.  If StringId is not zero, then a string in String Package is
  updated.  If SupportedLanguages is NULL, then the string is added or updated
  for all the languages that the String Package supports.  If SupportedLanguages
  is not NULL, then the string is added or updated for the set of languages
  specified by SupportedLanguages.

  If HiiHandle is NULL, then ASSERT().
  If String is NULL, then ASSERT().

  @param[in]  HiiHandle           A handle that was previously registered in the
                                  HII Database.
  @param[in]  StringId            If zero, then a new string is created in the
                                  String Package associated with HiiHandle.  If
                                  non-zero, then the string specified by StringId
                                  is updated in the String Package  associated
                                  with HiiHandle.
  @param[in]  String              A pointer to the Null-terminated Unicode string
                                  to add or update in the String Package associated
                                  with HiiHandle.
  @param[in]  SupportedLanguages  A pointer to a Null-terminated ASCII string of
                                  language codes.  If this parameter is NULL, then
                                  String is added or updated in the String Package
                                  associated with HiiHandle for all the languages
                                  that the String Package supports.  If this
                                  parameter is not NULL, then then String is added
                                  or updated in the String Package associated with
                                  HiiHandle for the set oflanguages specified by
                                  SupportedLanguages.  The format of
                                  SupportedLanguages must follow the language
                                  format assumed the HII Database.

  @retval 0      The string could not be added or updated in the String Package.
  @retval Other  The EFI_STRING_ID of the newly added or updated string.

**/
EFI_STRING_ID
EFIAPI
HiiSetString (
  IN EFI_HII_HANDLE    HiiHandle,
  IN EFI_STRING_ID     StringId             OPTIONAL,
  IN CONST EFI_STRING  String,
  IN CONST CHAR8       *SupportedLanguages  OPTIONAL
  )
{
  check_expected_ptr (HiiHandle);
  check_expected (StringId);
  check_expected_ptr (String);
  check_expected_ptr (SupportedLanguages);

  return mock_type (EFI_STRING_ID);
}

/**
  Mock HiiGetSupportedLanguages: Retrieves a pointer to the a Null-terminated ASCII string containing the list
  of languages that an HII handle in the HII Database supports.  The returned
  string is allocated using AllocatePool().  The caller is responsible for freeing
  the returned string using FreePool().  The format of the returned string follows
  the language format assumed the HII Database.

  If HiiHandle is NULL, then ASSERT().

  @param[in]  HiiHandle  A handle that was previously registered in the HII Database.

  @retval NULL   HiiHandle is not registered in the HII database
  @retval NULL   There are not enough resources available to retrieve the supported
                 languages.
  @retval NULL   The list of supported languages could not be retrieved.
  @retval Other  A pointer to the Null-terminated ASCII string of supported languages.

**/
CHAR8 *
EFIAPI
HiiGetSupportedLanguages (
  IN EFI_HII_HANDLE  HiiHandle
  )
{
  check_expected_ptr (HiiHandle);

  return mock_ptr_type (CHAR8 *);
}

/**
  Mock HiiAllocateOpCodeHandle: Allocates and returns a new OpCode Handle.  OpCode Handles must be freed with
  HiiFreeOpCodeHandle().

  @retval NULL   There are not enough resources to allocate a new OpCode Handle.
  @retval Other  A new OpCode handle.

**/
VOID *
EFIAPI
HiiAllocateOpCodeHandle (
  VOID
  )
{
  return mock_ptr_type (VOID *);
}

/**
  Mock HiiFreeOpCodeHandle: Frees an OpCode Handle that was previously allocated with HiiAllocateOpCodeHandle().
  When an OpCode Handle is freed, all of the opcodes associated with the OpCode
  Handle are also freed.

  If OpCodeHandle is NULL, then ASSERT().

  @param[in]  OpCodeHandle   Handle to the buffer of opcodes.

**/
VOID
EFIAPI
HiiFreeOpCodeHandle (
  VOID  *OpCodeHandle
  )
{
  check_expected_ptr (OpCodeHandle);
}

/**
  Mock HiiCreateGuidOpCode: Create EFI_IFR_GUID opcode.

  If OpCodeHandle is NULL, then ASSERT().
  If Guid is NULL, then ASSERT().
  If OpCodeSize < sizeof (EFI_IFR_GUID), then ASSERT().

  @param[in]  OpCodeHandle  Handle to the buffer of opcodes.
  @param[in]  Guid          Pointer to EFI_GUID of this guided opcode.
  @param[in]  GuidOpCode    Pointer to an EFI_IFR_GUID opcode.  This is an
                            optional parameter that may be NULL.  If this
                            parameter is NULL, then the GUID extension
                            region of the created opcode is filled with zeros.
                            If this parameter is not NULL, then the GUID
                            extension region of GuidData will be copied to
                            the GUID extension region of the created opcode.
  @param[in]  OpCodeSize    The size, in bytes, of created opcode.  This value
                            must be >= sizeof(EFI_IFR_GUID).

  @retval NULL   There is not enough space left in Buffer to add the opcode.
  @retval Other  A pointer to the created opcode.

**/
UINT8 *
EFIAPI
HiiCreateGuidOpCode (
  IN VOID            *OpCodeHandle,
  IN CONST EFI_GUID  *Guid,
  IN CONST VOID      *GuidOpCode     OPTIONAL,
  IN UINTN           OpCodeSize
  )
{
  check_expected_ptr (OpCodeHandle);
  check_expected_ptr (Guid);
  check_expected_ptr (GuidOpCode);
  check_expected (OpCodeSize);

  return mock_ptr_type (UINT8 *);
}

/**
  Mock HiiCreateGotoOpCode: Create EFI_IFR_REF_OP opcode.

  If OpCodeHandle is NULL, then ASSERT().
  If any reserved bits are set in QuestionFlags, then ASSERT().

  @param[in]  OpCodeHandle   Handle to the buffer of opcodes.
  @param[in]  FormId         Destination Form ID
  @param[in]  Prompt         String ID for Prompt
  @param[in]  Help           String ID for Help
  @param[in]  QuestionFlags  Flags in Question Header
  @param[in]  QuestionId     Question ID

  @retval NULL   There is not enough space left in Buffer to add the opcode.
  @retval Other  A pointer to the created opcode.

**/
UINT8 *
EFIAPI
HiiCreateGotoOpCode (
  IN VOID             *OpCodeHandle,
  IN EFI_FORM_ID      FormId,
  IN EFI_STRING_ID    Prompt,
  IN EFI_STRING_ID    Help,
  IN UINT8            QuestionFlags,
  IN EFI_QUESTION_ID  QuestionId
  )
{
  check_expected_ptr (OpCodeHandle);
  check_expected (FormId);
  check_expected (Prompt);
  check_expected (Help);
  check_expected (QuestionFlags);
  check_expected (QuestionId);

  return mock_ptr_type (UINT8 *);
}

/**
  Mock HiiCreateTextOpCode: Create EFI_IFR_TEXT_OP opcode.

  If OpCodeHandle is NULL, then ASSERT().

  @param[in]  OpCodeHandle  Handle to the buffer of opcodes.
  @param[in]  Prompt        String ID for Prompt.
  @param[in]  Help          String ID for Help.
  @param[in]  TextTwo       String ID for TextTwo.

  @retval NULL   There is not enough space left in Buffer to add the opcode.
  @retval Other  A pointer to the created opcode.

**/
UINT8 *
EFIAPI
HiiCreateTextOpCode (
  IN VOID           *OpCodeHandle,
  IN EFI_STRING_ID  Prompt,
  IN EFI_STRING_ID  Help,
  IN EFI_STRING_ID  TextTwo
  )
{
  check_expected_ptr (OpCodeHandle);
  check_expected (Prompt);
  check_expected (Help);
  check_expected (TextTwo);

  return mock_ptr_type (UINT8 *);
}

/**
  Mock HiiUpdateForm: This function updates a form that has previously been registered with the HII
  Database.  This function will perform at most one update operation.

  The form to update is specified by Handle, FormSetGuid, and FormId.  Binary
  comparisons of IFR opcodes are performed from the beginning of the form being
  updated until an IFR opcode is found that exactly matches the first IFR opcode
  specified by StartOpCodeHandle.  The following rules are used to determine if
  an insert, replace, or delete operation is performed.

  1) If no matches are found, then NULL is returned.
  2) If a match is found, and EndOpCodeHandle is NULL, then all of the IFR opcodes
     from StartOpCodeHandle except the first opcode are inserted immediately after
     the matching IFR opcode in the form to be updated.
  3) If a match is found, and EndOpCodeHandle is not NULL, then a search is made
     from the matching IFR opcode until an IFR opcode exactly matches the first
     IFR opcode specified by EndOpCodeHandle.  If no match is found for the first
     IFR opcode specified by EndOpCodeHandle, then NULL is returned.  If a match
     is found, then all of the IFR opcodes between the start match and the end
     match are deleted from the form being updated and all of the IFR opcodes
     from StartOpCodeHandle except the first opcode are inserted immediately after
     the matching start IFR opcode.  If StartOpCcodeHandle only contains one
     IFR instruction, then the result of this operation will delete all of the IFR
     opcodes between the start end matches.

  If HiiHandle is NULL, then ASSERT().
  If StartOpCodeHandle is NULL, then ASSERT().

  @param[in]  HiiHandle          The HII Handle of the form to update.
  @param[in]  FormSetGuid        The Formset GUID of the form to update.  This
                                 is an optional parameter that may be NULL.
                                 If it is NULL, all FormSet will be updated.
  @param[in]  FormId             The ID of the form to update.
  @param[in]  StartOpCodeHandle  An OpCode Handle that contains the set of IFR
                                 opcodes to be inserted or replaced in the form.
                                 The first IFR instruction in StartOpCodeHandle
                                 is used to find matching IFR opcode in the
                                 form.
  @param[in]  EndOpCodeHandle    An OpCcode Handle that contains the IFR opcode
                                 that marks the end of a replace operation in
                                 the form.  This is an optional parameter that
                                 may be NULL.  If it is NULL, then an the IFR
                                 opcodes specified by StartOpCodeHandle are
                                 inserted into the form.

  @retval EFI_OUT_OF_RESOURCES   No enough memory resource is allocated.
  @retval EFI_NOT_FOUND          The following cases will return EFI_NOT_FOUND.
                                 1) The form specified by HiiHandle, FormSetGuid,
                                 and FormId could not be found in the HII Database.
                                 2) No IFR opcodes in the target form match the first
                                 IFR opcode in StartOpCodeHandle.
                                 3) EndOpCOde is not NULL, and no IFR opcodes in the
                                 target form following a matching start opcode match
                                 the first IFR opcode in EndOpCodeHandle.
  @retval EFI_SUCCESS            The matched form is updated by StartOpcode.

**/
EFI_STATUS
EFIAPI
HiiUpdateForm (
  IN EFI_HII_HANDLE  HiiHandle,
  IN EFI_GUID        *FormSetGuid         OPTIONAL,
  IN EFI_FORM_ID     FormId,
  IN VOID            *StartOpCodeHandle,
  IN VOID            *EndOpCodeHandle     OPTIONAL
  )
{
  check_expected_ptr (HiiHandle);
  check_expected_ptr (FormSetGuid);
  check_expected (FormId);
  check_expected_ptr (StartOpCodeHandle);
  check_expected_ptr (EndOpCodeHandle);

  return (EFI_STATUS)mock ();
}
