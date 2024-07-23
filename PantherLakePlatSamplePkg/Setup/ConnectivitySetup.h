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
#ifndef _CONNECTIVITY_SETUP_H_
#define _CONNECTIVITY_SETUP_H_

#include <Protocol/HiiConfigAccess.h>

/**
  Platform CNV setup callback executed when user selects a knob from the CNV browser UI.
  Changes visibility and settings of other options.

  @param[in] Action          Specifies the type of action taken by the browser.
  @param[in] KeyValue        A unique value which is sent to the original exporting driver
  @param[in] Value           A pointer to the data being sent to the original exporting driver.

  @retval EFI_SUCCESS        The callback successfully handled the action
  @retval EFI_NOT_FOUND      Failed to retrive protocol/data required for execution
  @retval EFI_UNSUPPORTED    The specified action is not supported by the callback
**/
EFI_STATUS
EFIAPI
CnvFormPlatformCallback (
  IN EFI_BROWSER_ACTION      Action,
  IN EFI_QUESTION_ID         QuestionId,
  IN EFI_IFR_TYPE_VALUE      *Value
  );

/**
  Callback executed when user enters the DSP Features submenu of HD-Configuration form.
  Changes visibility and settings of other options according to the changes in CNV setup.

  @param[in] Action             Specifies the type of action taken by the browser.

  @retval EFI_SUCCESS           The callback successfully handled the action
  @retval EFI_UNSUPPORTED       The specified action is not supported by the callback
  @retval EFI_NOT_FOUND         Failed to retrive protocol/data required for execution
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate resources.
**/
EFI_STATUS
DspFeaturesCnvSetupCallback (
  IN EFI_BROWSER_ACTION         Action
  );

#endif
