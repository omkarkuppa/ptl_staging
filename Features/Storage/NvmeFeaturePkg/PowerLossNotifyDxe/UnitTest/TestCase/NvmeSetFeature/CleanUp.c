/**@file
    Unit test case CleanUp of NvmeSetFeature.

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

#include "NvmeSetFeatureMock.h"

/**
  Unit-Test NvmeSetFeature CleanUp Function. Clean the parameter for mock

  @param[in]  Context    Input context for mock.

**/
VOID
CleanUpNvmeSetFeature (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PASSTHRU_SET_FEATURE_MOCK *PassThruMock = (PASSTHRU_SET_FEATURE_MOCK*)Context;

  if (PassThruMock->Completion != NULL) {
    FreePool (PassThruMock->Completion);
  }
}
