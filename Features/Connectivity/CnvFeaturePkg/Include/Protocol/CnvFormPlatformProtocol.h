/** @file
  CNV Form Platform Protocol

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

#ifndef _CNV_FORM_PLATFORM_PROTOCOL_H_
#define _CNV_FORM_PLATFORM_PROTOCOL_H_

#include <Protocol/HiiConfigAccess.h>
#include <CnvVfrSetupMenuHii.h>

typedef struct _CNV_FORM_PLATFORM_PROTOCOL CNV_FORM_PLATFORM_PROTOCOL;

//
// Extern the GUID for protocol users.
//
extern EFI_GUID  gCnvFormPlatformProtocolGuid;

/**
  Protocol revision number
**/
#define CNV_FORM_PLATFORM_REVISION  1

/**

  This function is called to provide results data to the driver.
  This data consists of a unique key that is used to identify
  which data is either being passed back or being asked for.

  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  QuestionId         A unique value which is sent to the original
  @param[in]  Value              A pointer to the data being sent to the original exporting driver.
  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.
**/
typedef
  EFI_STATUS
(EFIAPI *CNV_FORM_PLATFORM_CALLBACK)(
                                     IN EFI_BROWSER_ACTION                   Action,
                                     IN EFI_QUESTION_ID                      QuestionId,
                                     IN EFI_IFR_TYPE_VALUE                   *Value
                                     );

//
// This protocol provides a callable interface between CNV form and
// Platform code. Platform driver which provides specific callback to
// CNV form is required to publish this protocol.
//
struct _CNV_FORM_PLATFORM_PROTOCOL {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT8                         Revision;
  CNV_VFR_CONFIG_SETUP          CnvFormData;
  CNV_FORM_PLATFORM_CALLBACK    Callback;
};

#endif
