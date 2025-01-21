/** @file
  String support

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#include "BdsPlatform.h"

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HII_HANDLE gStringPackHandle;

/**
  Initialize HII global accessor for string support.

**/
VOID
InitializeStringSupport (
  VOID
  )
{
  gStringPackHandle = HiiAddPackages (
                        &gEfiCallerIdGuid,
                        gImageHandle,
                        DxePlatformBootManagerLibStrings,
                        NULL
                        );
  ASSERT (gStringPackHandle != NULL);
}

/**
  Get string by string id from HII Interface
  FeaturePcd cannot defeat Macro in this case

  @param Id              String ID.

  @retval  CHAR16 *  String from ID.
  @retval  NULL      If error occurs.

**/
CHAR16 *
GetStringById (
  UINT16   Id
  )
{
  return HiiGetString (gStringPackHandle, Id, NULL);
}
