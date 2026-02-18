/** @file

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

#include <CnvSetupId.h>
#include <CnvVfrSetupMenu.h>
#include <Protocol/CnvFormPlatformProtocol.h>

extern EFI_HII_HANDLE                   mHiiHandle;
extern EFI_HII_DATABASE_PROTOCOL        *mHiiDatabase;
extern EFI_HII_CONFIG_ROUTING_PROTOCOL  *mConfigRouting;

CHAR16  mCnvVfrSetupName[] = CNV_SETUP_VARIABLE_NAME;

/**
  Construct Request String (L"&OFFSET=%x&WIDTH=%x") base on the input Offset and Width.
  If the input RequestString is not NULL, new request will be cat at the end of it. The full
  request string will be constructed and return. Caller is responsible to free it.

  @param[in] RequestString   Current request string.
  @param[in] Offset          Offset of data in Storage.
  @param[in] Width           Width of data.

  @return String             Request string with input Offset and Width.
**/
EFI_STRING
EFIAPI
HiiConstructRequestString (
  IN EFI_STRING  RequestString,
  OPTIONAL
  IN UINTN       Offset,
  IN UINTN       Width
  )
{
  CHAR16      RequestElement[30];
  UINTN       StringLength;
  EFI_STRING  NewString;

  StringLength = UnicodeSPrint (
                   RequestElement,
                   sizeof (RequestElement),
                   L"&OFFSET=%x&WIDTH=%x",
                   Offset,
                   Width
                   );
  if (RequestString != NULL) {
    StringLength = StringLength + StrLen (RequestString);
  }

  NewString = AllocateZeroPool ((StringLength + 1) * sizeof (CHAR16));
  if (NewString == NULL) {
    return NULL;
  }

  if (RequestString != NULL) {
    StrCatS (NewString, ((StringLength + 1) * sizeof (CHAR16)) / sizeof (CHAR16), RequestString);
    FreePool (RequestString);
  }

  StrCatS (NewString, ((StringLength + 1) * sizeof (CHAR16)) / sizeof (CHAR16), RequestElement);
  return NewString;
}

/**
  Swap bytes in the buffer.

  @param[in, out] Buffer     -  Binary buffer.
  @param[in] BufferSize      -  Size of the buffer in bytes.

**/
STATIC
VOID
SwapBuffer (
  IN OUT UINT8  *Buffer,
  IN UINTN      BufferSize
  )
{
  UINTN  Index;
  UINT8  Temp;
  UINTN  SwapCount;

  SwapCount = BufferSize / 2;
  for (Index = 0; Index < SwapCount; Index++) {
    Temp                           = Buffer[Index];
    Buffer[Index]                  = Buffer[BufferSize - 1 - Index];
    Buffer[BufferSize - 1 - Index] = Temp;
  }
}

/**
  Determines if a Unicode character is a hexadecimal digit.
  The test is case insensitive.

  @param[out] Digit - Pointer to byte that receives the value of the hex character.
  @param[in]  Char  - Unicode character to test.

  @return TRUE  - If the character is a hexadecimal digit.
  @return FALSE - Otherwise.

**/
BOOLEAN
IsHexDigit (
  OUT UINT8   *Digit,
  IN  CHAR16  Char
  )
{
  if ((Char >= L'0') && (Char <= L'9')) {
    *Digit = (UINT8)(Char - L'0');
    return TRUE;
  }

  if ((Char >= L'A') && (Char <= L'F')) {
    *Digit = (UINT8)(Char - L'A' + 0x0A);
    return TRUE;
  }

  if ((Char >= L'a') && (Char <= L'f')) {
    *Digit = (UINT8)(Char - L'a' + 0x0A);
    return TRUE;
  }

  return FALSE;
}

/**
  Converts Unicode string to binary buffer.
  The conversion may be partial.
  The first character in the string that is not hex digit stops the conversion.
  At a minimum, any blob of data could be represented as a hex string.

  @param[in, out] Buf         - Pointer to buffer that receives the data.
  @param[in, out] Len         - Length in bytes of the buffer to hold converted data.
                                If routine return with EFI_SUCCESS, containing length of converted data.
                                If routine return with EFI_BUFFER_TOO_SMALL, containing length of buffer desired.
  @param[in] Str              - String to be converted from.
  @param[out] ConvertedStrLen - Length of the Hex String consumed.

  @return EFI_SUCCESS: Routine Success.
  @return EFI_BUFFER_TOO_SMALL: The buffer is too small to hold converted data.

**/
EFI_STATUS
HexStringToBuf (
  IN OUT UINT8   *Buf,
  IN OUT UINTN   *Len,
  IN     CHAR16  *Str,
  OUT    UINTN   *ConvertedStrLen  OPTIONAL
  )
{
  UINTN  HexCnt;
  UINTN  Idx;
  UINTN  BufferLength;
  UINT8  Digit;
  UINT8  Byte;

  //
  // Find out how many hex characters the string has.
  //
  for (Idx = 0, HexCnt = 0; IsHexDigit (&Digit, Str[Idx]); Idx++, HexCnt++) {
  }

  if (HexCnt == 0) {
    *Len = 0;
    return EFI_SUCCESS;
  }

  //
  // Two Unicode characters make up 1 buffer byte. Round up.
  //
  BufferLength = (HexCnt + 1) / 2;

  //
  // Test if buffer is passed enough.
  //
  if (BufferLength > (*Len)) {
    *Len = BufferLength;
    return EFI_BUFFER_TOO_SMALL;
  }

  *Len = BufferLength;

  for (Idx = 0; Idx < HexCnt; Idx++) {
    IsHexDigit (&Digit, Str[HexCnt - 1 - Idx]);

    //
    // For odd charaters, write the lower nibble for each buffer byte,
    // and for even characters, the upper nibble.
    //
    if ((Idx & 1) == 0) {
      Byte = Digit;
    } else {
      Byte  = Buf[Idx / 2];
      Byte &= 0x0F;
      Byte |= Digit << 4;
    }

    Buf[Idx / 2] = Byte;
  }

  if (ConvertedStrLen != NULL) {
    *ConvertedStrLen = HexCnt;
  }

  return EFI_SUCCESS;
}

/**
  Converts Hex String to binary buffer in reversed byte order from HexStringToBuf().

  @param[in, out] Buffer     - Pointer to buffer that receives the data.
  @param[in, out] BufferSize - Length in bytes of the buffer to hold converted data.
                               If routine return with EFI_SUCCESS, containing length of converted data.
                               If routine return with EFI_BUFFER_TOO_SMALL, containing length of buffer desired.
  @param[in] Str             - String to be converted from.

  @return EFI_SUCCESS        - The function completed successfully.

**/
EFI_STATUS
HexStringToBuffer (
  IN OUT UINT8  *Buffer,
  IN OUT UINTN  *BufferSize,
  IN CHAR16     *Str
  )
{
  EFI_STATUS  Status;
  UINTN       ConvertedStrLen;

  ConvertedStrLen = 0;
  Status          = HexStringToBuf (Buffer, BufferSize, Str, &ConvertedStrLen);
  if (!EFI_ERROR (Status)) {
    SwapBuffer (Buffer, (ConvertedStrLen + 1) / 2);
  }

  return Status;
}

/**
  Convert binary representation Config string (e.g. "0041004200430044") to the
  original string (e.g. "ABCD"). Config string appears in <ConfigHdr> (i.e.
  "&NAME=<string>"), or Name/Value pair in <ConfigBody> (i.e. "label=<string>").

  @param[in, out] UnicodeString - Original Unicode string.
  @param[in, out] StrBufferLen  - On input: Length in bytes of buffer to hold the Unicode string.
                                  Includes tailing '\0' character.
                                  On output:
                                    If return EFI_SUCCESS, containing length of Unicode string buffer.
                                    If return EFI_BUFFER_TOO_SMALL, containing length of string buffer desired.
  @param[in] ConfigString       - Binary representation of Unicode String, <string> := (<HexCh>4)+

  @return EFI_SUCCESS          - Routine success.
  @return EFI_BUFFER_TOO_SMALL - The string buffer is too small.

**/
EFI_STATUS
ConfigStringToUnicode (
  IN OUT CHAR16  *UnicodeString,
  IN OUT UINTN   *StrBufferLen,
  IN CHAR16      *ConfigString
  )
{
  UINTN   Index;
  UINTN   Len;
  UINTN   BufferSize;
  CHAR16  BackupChar;

  Len        = StrLen (ConfigString) / 4;
  BufferSize = (Len + 1) * sizeof (CHAR16);

  if (*StrBufferLen < BufferSize) {
    *StrBufferLen = BufferSize;
    return EFI_BUFFER_TOO_SMALL;
  }

  *StrBufferLen = BufferSize;

  for (Index = 0; Index < Len; Index++) {
    BackupChar      = ConfigString[4];
    ConfigString[4] = L'\0';

    HexStringToBuf ((UINT8 *)UnicodeString, &BufferSize, ConfigString, NULL);

    ConfigString[4] = BackupChar;

    ConfigString  += 4;
    UnicodeString += 1;
  }

  //
  // Add tailing '\0' character
  //
  *UnicodeString = L'\0';

  return EFI_SUCCESS;
}

/**
  Extract GUID name from ConfigHdr.

  @param[in]   ConfigHdr     Either <ConfigRequest> or <ConfigResp>.
  @param[out]  Guid          Pointer to an EFI_GUID that is the routing information GUID.
  @param[out]  Name          Pointer to a Null-terminated Unicode string that is
                             the routing information NAME.
  @param[out]  Offset        Pointer to the offset of the routing information NAME.

  @retval EFI_SUCCESS             The routing information NAME and GUID are extracted
                                  from ConfigHdr.
  @retval EFI_INVALID_PARAMETER   The format of ConfigHdr is invalid.

**/
EFI_STATUS
ExtractGuidNameFromConfigHdr (
  IN CONST EFI_STRING  ConfigHdr,
  OUT      EFI_GUID    *Guid,
  OUT      CHAR16      **Name,
  OUT      UINTN       *Offset
  )
{
  EFI_STATUS  Status;
  CHAR16      *StrPtr;
  UINTN       Length;
  CHAR16      *StrName;

  *Offset = 0;
  //
  // <ConfigHdr> ::=
  // GUID=<HexCh>32&NAME=<Char>NameStrLen&PATH=<HexChar>DevicePathStrLen <NULL>
  // | 5  |   32   |  6  |  NameStrLen*4 |  6  |    DevicePathStrLen    | 1 |
  //
  if (StrLen (ConfigHdr) <= (5 + 32 + 6)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // check syntax for guid
  //
  StrPtr = ConfigHdr + 5 + 32;
  if (*StrPtr != L'&') {
    return EFI_INVALID_PARAMETER;
  }

  *StrPtr = L'\0';

  //
  // extract guid
  //
  Length = sizeof (EFI_GUID);
  Status = HexStringToBuffer (
             (UINT8 *)Guid,
             &Length,
             ConfigHdr + 5
             );
  *StrPtr = L'&';
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[CNV] HII Config Access: Guid extraction failed\n"));
    *Offset = 5;
    return EFI_INVALID_PARAMETER;
  }

  //
  // check syntax for name
  //
  StrPtr = ConfigHdr + 5 + 32 + 6;
  while (*StrPtr != L'\0' && *StrPtr != L'&') {
    StrPtr++;
  }

  if (*StrPtr != L'&') {
    *Offset = 5 + 32 + 6;
    return EFI_INVALID_PARAMETER;
  }

  //
  // extract name
  //
  *StrPtr = L'\0';
  Length  = StrSize (ConfigHdr + 5 + 32 + 6);
  StrName = AllocatePool (Length);
  ASSERT (StrName != NULL);
  if (StrName == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = ConfigStringToUnicode (
             StrName,
             &Length,
             ConfigHdr + 5 + 32 + 6
             );
  *StrPtr = L'&';
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[CNV] HII Config Access: Name extraction failed\n"));
    *Offset = 5 + 32 + 6;
    return EFI_INVALID_PARAMETER;
  }

  *Name = StrName;

  return EFI_SUCCESS;
}

/**
  This function allows the caller to request the current
  configuration for one or more named elements. The resulting
  string is in <ConfigAltResp> format. Any and all alternative
  configuration strings shall also be appended to the end of the
  current configuration string. If they are, they must appear
  after the current configuration. They must contain the same
  routing (GUID, NAME, PATH) as the current configuration string.
  They must have an additional description indicating the type of
  alternative configuration the string represents,
  "ALTCFG=<StringToken>". That <StringToken> (when
  converted from Hex UNICODE to binary) is a reference to a
  string in the associated string pack.

  @param[in] This       Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Request    A null-terminated Unicode string in
                        <ConfigRequest> format. Note that this
                        includes the routing information as well as
                        the configurable name / value pairs. It is
                        invalid for this string to be in
                        <MultiConfigRequest> format.
  @param[out] Progress  On return, points to a character in the
                        Request string. Points to the string's null
                        terminator if request was successful. Points
                        to the most recent "&" before the first
                        failing name / value pair (or the beginning
                        of the string if the failure is in the first
                        name / value pair) if the request was not
                        successful.
  @param[out] Results   A null-terminated Unicode string in
                        <ConfigAltResp> format which has all values
                        filled in for the names in the Request string.
                        String to be allocated by the called function.

  @retval EFI_SUCCESS             The Results string is filled with the
                                  values corresponding to all requested
                                  names.
  @retval EFI_OUT_OF_RESOURCES    Not enough memory to store the
                                  parts of the results that must be
                                  stored awaiting possible future
                                  protocols.
  @retval EFI_INVALID_PARAMETER   For example, passing in a NULL
                                  for the Request parameter
                                  would result in this type of
                                  error. In this case, the
                                  Progress parameter would be
                                  set to NULL.
  @retval EFI_NOT_FOUND           Routing data doesn't match any
                                  known driver. Progress set to the
                                  first character in the routing header.
                                  Note: There is no requirement that the
                                  driver validate the routing data. It
                                  must skip the <ConfigHdr> in order to
                                  process the names.
  @retval EFI_INVALID_PARAMETER   Illegal syntax. Progress set
                                  to most recent & before the
                                  error or the beginning of the
                                  string.
  @retval EFI_INVALID_PARAMETER   Unknown name. Progress points
                                  to the & before the name in
                                  question.Currently not implemented.
**/
EFI_STATUS
EFIAPI
CnvVfrSetupMenuFormExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Request,
  OUT EFI_STRING                            *Progress,
  OUT EFI_STRING                            *Results
  )
{
  EFI_STATUS  Status;
  UINTN       Length;
  EFI_GUID    Guid;
  CHAR16      *Name;
  UINT8       *Var;
  UINTN       Offset;
  EFI_HANDLE  DriverHandle;
  EFI_STRING  ConfigRequest;
  EFI_STRING  ConfigRequestHdr;
  BOOLEAN     AllocatedRequest;
  UINTN       BufferSize;

  if ((Progress == NULL) || (Results == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  AllocatedRequest = FALSE;
  ConfigRequestHdr = NULL;
  ConfigRequest    = NULL;
  Length           = 0;

  *Progress = Request;
  if ((Request != NULL) &&
      !HiiIsConfigHdrMatch (Request, &gCnvFeatureSetupGuid, mCnvVfrSetupName))
  {
    return EFI_NOT_FOUND;
  }

  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  BufferSize    = sizeof (CNV_VFR_CONFIG_SETUP);
  ConfigRequest = Request;
  if ((Request == NULL) || (StrStr (Request, L"OFFSET") == NULL)) {
    Status = mHiiDatabase->GetPackageListHandle (mHiiDatabase, mHiiHandle, &DriverHandle);
    if (EFI_ERROR (Status)) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // Request has no request element, construct full request string.
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&gCnvFeatureSetupGuid, mCnvVfrSetupName, DriverHandle);
    if (ConfigRequestHdr == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    Length        = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Length);
    if (ConfigRequest == NULL) {
      ASSERT (ConfigRequest != NULL);
      return EFI_OUT_OF_RESOURCES;
    }

    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Length, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
    ConfigRequestHdr = NULL;
  }

  Status = ExtractGuidNameFromConfigHdr (ConfigRequest, &Guid, &Name, &Offset);
  if (EFI_ERROR (Status)) {
    *Progress = ConfigRequest + Offset;
    return Status;
  }

  //
  // Get variable by name and guid from NVRAM
  //
  Length = 0;
  Var    = NULL;
  Status = gRT->GetVariable (Name, &Guid, NULL, &Length, Var);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    Var = AllocateZeroPool (Length);
    ASSERT (Var != NULL);
    if (Var == NULL) {
      if (AllocatedRequest) {
        FreePool (ConfigRequest);
      }

      *Progress = Request;
      return EFI_OUT_OF_RESOURCES;
    }

    Status = gRT->GetVariable (Name, &Guid, NULL, &Length, Var);
  }

  if (EFI_ERROR (Status)) {
    if (AllocatedRequest) {
      FreePool (ConfigRequest);
    }

    if (Var != NULL) {
      FreePool (Var);
    }

    *Progress = Request;
    return Status;
  }

  Status = mConfigRouting->BlockToConfig (mConfigRouting, ConfigRequest, Var, Length, Results, Progress);
  FreePool (Var);

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
  }

  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

  return Status;
}

/**
  CNV VFR SetupMenuForm Route Config

  @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Configuration    A null-terminated Unicode string in <ConfigRequest> format.
  @param[out] Progress       On return, points to end of Configuration string.

  @return EFI_SUCCESS        - The function completed successfully.
  @retval  EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this driver.
**/
EFI_STATUS
EFIAPI
CnvVfrSetupMenuFormRouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                           *Progress
  )
{
  
  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration + StrLen (Configuration);
  if (!HiiIsConfigHdrMatch (Configuration, &gCnvFeatureSetupGuid, CNV_SETUP_VARIABLE_NAME)) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  CNV VFR SetupMenuForm Callback function.

  @param[in]  This               Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  QuestionId         A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[out] ActionRequest      On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.

**/
EFI_STATUS
EFIAPI
CnvVfrSetupMenuFormCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       QuestionId,
  IN UINT8                                 Type,
  IN EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  CNV_VFR_CONFIG_SETUP        *CnvSetupDataPtr;
  UINTN                       VarSize;
  EFI_STATUS                  Status;
  CNV_FORM_PLATFORM_PROTOCOL  *CnvFormPlatformProtocol;

  Status                  = EFI_SUCCESS;
  CnvFormPlatformProtocol = NULL;

  DEBUG ((DEBUG_INFO, " %a Entry - Action: %d, QuestionId: %d\n", __FUNCTION__, Action, QuestionId));

  // Validate Action
  if ((Action != EFI_BROWSER_ACTION_CHANGED)   &&
      (Action != EFI_BROWSER_ACTION_SUBMITTED) &&
      (Action != EFI_BROWSER_ACTION_FORM_OPEN) &&
      (Action != EFI_BROWSER_ACTION_FORM_CLOSE) &&
      (Action != EFI_BROWSER_ACTION_DEFAULT_STANDARD) &&
      (Action != EFI_BROWSER_ACTION_DEFAULT_MANUFACTURING) &&
      (Action != EFI_BROWSER_ACTION_DEFAULT_SAFE))
  {
    DEBUG ((DEBUG_INFO, "Unsupported action: %d\n", Action));
    return EFI_UNSUPPORTED;
  }

  if ((Value == NULL) || (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "CNV callback action = %d begin with key value = %d\n", Action, QuestionId));

  VarSize         = sizeof (CNV_VFR_CONFIG_SETUP);
  CnvSetupDataPtr = AllocatePool (VarSize);
  ASSERT (CnvSetupDataPtr != NULL);
  if (CnvSetupDataPtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Handle F3 (Load Setup Defaults) action
  if ((Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) || (Action == EFI_BROWSER_ACTION_DEFAULT_SAFE)) {
    DEBUG ((DEBUG_ERROR, "F3 detected for QuestionId %d - calling platform callback with Action %d\n", QuestionId, Action));

    Status = gBS->LocateProtocol (&gCnvFormPlatformProtocolGuid, NULL, (VOID **)&CnvFormPlatformProtocol);
    if ((Status == EFI_SUCCESS) && (CnvFormPlatformProtocol != NULL)) {
      DEBUG ((DEBUG_ERROR, "Calling CnvFormPlatformProtocol->Callback with Action=%d, QuestionId=%d\n", Action, QuestionId));
      CnvFormPlatformProtocol->Callback (Action, QuestionId, Value);
    } else {
      DEBUG ((DEBUG_ERROR, "Failed to locate CnvFormPlatformProtocol, Status=%r\n", Status));
    }

    FreePool (CnvSetupDataPtr);
    return EFI_SUCCESS;
  }

  // For non-F3 actions, get browser data as usual
  if (!HiiGetBrowserData (&gCnvFeatureSetupGuid, CNV_SETUP_VARIABLE_NAME, VarSize, (UINT8 *)CnvSetupDataPtr)) {
    Status = EFI_NOT_FOUND;
  }

  ASSERT_EFI_ERROR (Status);

  //
  // Locate gCnvFormPlatformProtocolGuid and update the latest CNV browser data to the protocol
  // (For non-F3 actions)
  //
  Status = gBS->LocateProtocol (&gCnvFormPlatformProtocolGuid, NULL, (VOID **)&CnvFormPlatformProtocol);
  if ((Status == EFI_SUCCESS) && (CnvFormPlatformProtocol != NULL)) {
    if (Action != EFI_BROWSER_ACTION_FORM_OPEN) {
      CopyMem (&CnvFormPlatformProtocol->CnvFormData, CnvSetupDataPtr, sizeof (CNV_VFR_CONFIG_SETUP));
      CnvFormPlatformProtocol->Callback (Action, QuestionId, Value);
    }
  }

  FreePool (CnvSetupDataPtr);

  DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));
  return EFI_SUCCESS;
}
