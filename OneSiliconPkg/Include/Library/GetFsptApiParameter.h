/** @file
  Library to get FSP-T API parameter.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _GET_FSP_API_PARAMETER_H_
#define _GET_FSP_API_PARAMETER_H_

/**
  This function gets Fspt API parameter

  @return   The value of Fspt API parameter.

**/
UINTN
SecGetFsptApiParameter (
  VOID
  );

#endif //_GET_FSP_API_PARAMETER_H_
