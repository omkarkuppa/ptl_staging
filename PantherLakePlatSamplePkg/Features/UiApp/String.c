/** @file
  String support

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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

#include "Ui.h"
#include "Language.h"
#include "FrontPage.h"

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HII_HANDLE gStringPackHandle;

GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID mUiStringPackGuid = {
  0x136a3048, 0x752a, 0x4bf6, { 0xa7, 0x57, 0x9, 0x36, 0x11, 0x95, 0x38, 0xed }
};

/**
  Initialize HII global accessor for string support.

**/
VOID
InitializeStringSupport (
  VOID
  )
{
  gStringPackHandle = HiiAddPackages (
                         &mUiStringPackGuid,
                         gImageHandle,
                         UiAppStrings,
                         NULL
                         );
  ASSERT (gStringPackHandle != NULL);
}

VOID
UninitializeStringSupport (
  VOID
  )
{
  HiiRemovePackages (gStringPackHandle);
}

/**
  Get string by string id from HII Interface


  @param Id              String ID.

  @retval  CHAR16 *  String from ID.
  @retval  NULL      If error occurs.

**/
CHAR16 *
GetStringById (
  IN  EFI_STRING_ID   Id
  )
{
  return HiiGetString (gStringPackHandle, Id, NULL);
}
