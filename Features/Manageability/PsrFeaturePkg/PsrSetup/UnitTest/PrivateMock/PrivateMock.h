/** @file
  Private mock for the Psr Setup driver.

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

#ifndef _PRIVATE_MOCK_H_
#define _PRIVATE_MOCK_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <PiDxe.h>
#include <PsrSetup/PsrSetup.h>
#include <PsrSetup/PsrSettings.h>
#include <Protocol/PsrProtocol.h>
#include <Protocol/HiiString.h>

#define MOCK_HII_HANDLE_SIGNATURE  SIGNATURE_32 ('h','i','h','l')

typedef struct {
  UINTN         Signature;
  LIST_ENTRY    Handle;
  UINTN         Key;
} MOCK_HII_HANDLE;

extern  EFI_BOOT_SERVICES         *gBS;
extern  EFI_GUID                  mPsrFormSetGuid;
extern  PSR_CONFIGURATION         mPsrConfiguration;

VOID                              *gReturnResult;
EFI_HANDLE                        *gMockHandle;
EFI_HII_HANDLE                    *gMockHiiHandle;
PLATFORM_SERVICE_RECORD_PROTOCOL  *gPlatformServiceRecord;
EFI_HII_STRING_PROTOCOL           *gHiiStringProtocol;
UINT8                             *gRandomPsrData;

/**
  Frees a buffer and set the pointer to NULL.

  @param  Buffer  Pointer to the buffer.
**/
VOID
FreeSetNull (
  IN OUT VOID  **Buffer
  );

/**
  Generate random data.

  @param  Buffer      The buffer to return data.
  @param  BufferSize  The number of bytes in Buffer to fill with random data.
  @param  IsWord      If random data with [0-9], [A-Z], [a-z] only.

  @retval TRUE        Generate random data success.
  @retval FALSE       Generate random data fail.
**/
BOOLEAN
RandomData (
  IN OUT UINT8    *Buffer,
  IN     UINTN    BufferSize,
  IN     BOOLEAN  IsWord
  );

/**
  Generate random PSR Data for unit test.

  @param  Output  The buffer to return data.

  @retval TRUE    Generate random data success.
  @retval FALSE   Generate random data fail.
**/
BOOLEAN
GenerateRandomPsrData (
  IN OUT UINT8  **Output
  );

/**
  Mock BootService LocateProtocol: Return the first Protocol Interface that matches the Protocol GUID. If
  Registration is passed in, return a Protocol Instance that was just add
  to the system. If Registration is NULL return the first Protocol Interface
  you find.

  @param  Protocol               The protocol to search for
  @param  Registration           Optional Registration Key returned from
                                 RegisterProtocolNotify()
  @param  Interface              Return the Protocol interface (instance).

  @retval EFI_SUCCESS            If a valid Interface is returned
  @retval EFI_INVALID_PARAMETER  Invalid parameter
  @retval EFI_NOT_FOUND          Protocol interface not found
**/
EFI_STATUS
EFIAPI
MockBootServiceLocateProtocol (
  IN EFI_GUID  *Protocol,
  IN VOID      *Registration  OPTIONAL,
  OUT VOID     **Interface
  );

/**
  Mock BootService InstallMultipleProtocolInterfaces.
  @param[in, out]  Handle       The pointer to a handle to install the new protocol interfaces on,
                                or a pointer to NULL if a new handle is to be allocated.
  @param  ...                   A variable argument list containing pairs of protocol GUIDs and protocol
                                interfaces.
  @retval EFI_SUCCESS           All the protocol interface was installed.
  @retval EFI_OUT_OF_RESOURCES  There was not enough memory in pool to install all the protocols.
  @retval EFI_ALREADY_STARTED   A Device Path Protocol instance was passed in that is already present in
                                the handle database.
  @retval EFI_INVALID_PARAMETER Handle is NULL.
  @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle.
**/
EFI_STATUS
EFIAPI
MockBootServiceInstallMultipleProtocolInterfaces (
  IN OUT EFI_HANDLE  *Handle,
  ...
  );

/**
  Mock PlatformServiceRecordProtocol PsrGetData: Get PSR blob from CSME via HECI cmds.

  @param[in]      *This                  Pointer of PLATFORM_SERVICE_RECORD_PROTOCOL.
  @param[in]      PsrDataBlobSize        PSR Data Buffer Size.
  @param[in, out] *PsrDataBlob           Pointer of the PSR Data Blob.

  @retval         EFI_SUCCESS            Retrieved PSR from CSME successfully.
  @retval         Others                 Failed to retrieve the record.
**/
EFI_STATUS
EFIAPI
MockPlatformServiceRecordPsrGetData (
  IN     PLATFORM_SERVICE_RECORD_PROTOCOL *This,
  IN     UINTN                            PsrDataBlobSize,
  IN OUT UINT8                            *PsrDataBlob
  );

/**
  Mock HiiSetString of EFI_HII_STRING_PROTOCOL.
  This function updates the string specified by StringId in the specified PackageList to the text
  specified by String and, optionally, the font information specified by StringFontInfo.

  @param  This                    A pointer to the EFI_HII_STRING_PROTOCOL
                                  instance.
  @param  PackageList             The package list containing the strings.
  @param  StringId                The string's id, which is unique within
                                  PackageList.
  @param  Language                Points to the language for the updated string.
  @param  String                  Points to the new null-terminated string.
  @param  StringFontInfo          Points to the string's font information or NULL
                                  if the string font information is not changed.

  @retval EFI_SUCCESS             The string was updated successfully.
  @retval EFI_NOT_FOUND           The string specified by StringId is not in the
                                  database.
  @retval EFI_INVALID_PARAMETER   The String or Language was NULL.
  @retval EFI_OUT_OF_RESOURCES    The system is out of resources to accomplish the
                                  task.

**/
EFI_STATUS
EFIAPI
MockHiiSetString (
  IN CONST EFI_HII_STRING_PROTOCOL  *This,
  IN EFI_HII_HANDLE                 PackageList,
  IN EFI_STRING_ID                  StringId,
  IN CONST CHAR8                    *Language,
  IN CONST EFI_STRING               String,
  IN CONST EFI_FONT_INFO            *StringFontInfo OPTIONAL
  );

#endif
