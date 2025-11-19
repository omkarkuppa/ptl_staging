/**@file
    Unit test case CleanUp of GetParentProtocol.

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

#include "GetParentProtocolMock.h"

/**
  Unit-Test GetParentProtocol CleanUp Function. Clean the parameter for mock

  @param[in]  Context    Input context for mock.

**/
VOID
CleanUpGetParentProtocol (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  GET_PARENT_PROTOCOL_MOCK *Parameter = (GET_PARENT_PROTOCOL_MOCK*) Context;

  if (Parameter->PassThru != NULL) {
    FreePool (Parameter->PassThru);
  }

  BootServiceDestructorMock ();
}
