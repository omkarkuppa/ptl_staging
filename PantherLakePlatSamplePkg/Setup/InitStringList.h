/** @file
  Internal header of the Setup Component.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#ifndef _INIT_STRING_LIST_H_
#define _INIT_STRING_LIST_H_
#define INIT_OVERCLOCK_STRINGS
#define INIT_LIST \
  InitDebugStrings, \
  InitCPUStrings, \
  InitSaStrings, \
  InitSBStrings, \
  InitAcpiStrings, \
  InitHhmMobileStrings, \
  InitMeInfo, \
  INIT_OVERCLOCK_STRINGS \
  InitPlatformStrings, \
  InitBoardStrings, \
  InitTcgStrings,           \

#endif // _INIT_STRING_LIST_H_
