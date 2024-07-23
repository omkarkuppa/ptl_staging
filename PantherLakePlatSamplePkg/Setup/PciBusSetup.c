/**@file

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

@par Specification Reference:
**/

#include <SetupPrivate.h>

// Function Definitions


//
//----------------------------------------------------------------------------
// Procedure:  InitBoardStrings
//
// Description:  This function initializes the board related setup option values
//
// Input:    HiiHandle  Handle to HII database
//        Class    Indicates the setup class
//
// Output:    None
//
//----------------------------------------------------------------------------
//

VOID
InitPciBusStrings(
  EFI_HII_HANDLE    HiiHandle,
  UINT16        Class
)
{
     if(Class == ADVANCED_FORM_SET_CLASS){
        InitString(HiiHandle, STRING_TOKEN(STR_PCI_DRIVER_VER), L"V %d.%02d.%02d",
        0xA, 0, 0);
    }
  return;
}
