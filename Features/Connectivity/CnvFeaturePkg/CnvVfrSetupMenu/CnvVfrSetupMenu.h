/** @file
  The header file of CNV VFR menu setup Module.

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

#ifndef _CNV_VFR_MENU_SETUP_H_
#define _CNV_VFR_MENU_SETUP_H_

#include <CnvVfrSetupMenuHii.h>
#include "CnvVfrSetupMenuStrTokens.h"

#ifndef VFRCOMPILE

  #include <Uefi.h>
  #include <Protocol/HiiConfigAccess.h>
  #include <Guid/MdeModuleHii.h>
  #include <Library/DebugLib.h>
  #include <Library/BaseLib.h>
  #include <Library/BaseMemoryLib.h>
  #include <Library/UefiRuntimeServicesTableLib.h>
  #include <Library/UefiDriverEntryPoint.h>
  #include <Library/UefiBootServicesTableLib.h>
  #include <Library/MemoryAllocationLib.h>
  #include <Library/UefiHiiServicesLib.h>
  #include <Library/HiiLib.h>
  #include <Library/DevicePathLib.h>
  #include <Library/PrintLib.h>
  #include <Library/UefiLib.h>
  #include <Library/PcdLib.h>
  #include <PiDxe.h>

extern UINT8  CnvVfrSetupMenuVfrBin[];
extern UINT8  CnvVfrSetupMenuStrings[];

  #pragma pack (1)

typedef struct {
  VENDOR_DEVICE_PATH          VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    End;
} HII_VENDOR_DEVICE_PATH;

  #pragma pack()

EFI_STATUS
EFIAPI
CnvVfrSetupMenuFormExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Request,
  OUT EFI_STRING                            *Progress,
  OUT EFI_STRING                            *Results
  );

EFI_STATUS
EFIAPI
CnvVfrSetupMenuFormRouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                           *Progress
  );

EFI_STATUS
EFIAPI
CnvVfrSetupMenuFormCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       QuestionId,
  IN UINT8                                 Type,
  IN EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  );

#endif
#endif
