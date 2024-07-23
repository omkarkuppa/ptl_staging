/** @file
  Header file for PEI IEH Init Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _PEI_IEH_INIT_LIB_H_
#define _PEI_IEH_INIT_LIB_H_

#include <IehHandle.h>

/**
  Initialize IEH device.

  @param[in] IehHandle    IEH Handle instance
**/
VOID
IehInit (
  IN  IEH_HANDLE      *IehHandle
  );

#endif // _PEI_IEH_INIT_LIB_H_
