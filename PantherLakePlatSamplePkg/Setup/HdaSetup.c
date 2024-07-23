/**@file

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

@par Specification Reference:
**/

#include "SetupPrivate.h"
#include "ConnectivitySetup.h"

/**
  Setup callback executed when user enters DSP features submenu.

  @param[in] This               Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action             Specifies the type of action taken by the browser.
  @param[in] KeyValue           A unique value which is sent to the original exporting driver
                                so that it can identify the type of data to expect.
  @param[in] Type               The type of value for the question.
  @param[in] Value              A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest     On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS           The callback successfully handled the action
  @retval EFI_UNSUPPORTED       The specified action is not supported by the callback
  @retval EFI_NOT_FOUND         Failed to retrive protocol/data required for execution
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate resources.
**/
EFI_STATUS
EFIAPI
HdaDspCallBack(
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL      *This,
  IN EFI_BROWSER_ACTION                        Action,
  IN EFI_QUESTION_ID                           KeyValue,
  IN UINT8                                     Type,
  IN EFI_IFR_TYPE_VALUE                        *Value,
  OUT EFI_BROWSER_ACTION_REQUEST               *ActionRequest
  )
{
  EFI_STATUS        Status;

  if (Action == EFI_BROWSER_ACTION_SUBMITTED) {
    Status = CnvFormPlatformCallback (Action, KeyValue, Value);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  if (Action == EFI_BROWSER_ACTION_FORM_OPEN || Action == EFI_BROWSER_ACTION_CHANGED) {
    Status = DspFeaturesCnvSetupCallback (Action);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

