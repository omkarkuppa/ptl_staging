/** @file
  This file contains definitions required for One-Touch function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#ifndef _TXT_ONE_TOUCH_H_
#define _TXT_ONE_TOUCH_H_

//
// Operation
//
#define ENABLE_VT                             128
#define DISABLE_VT_TXT                        129
#define ENABLE_VTD                            130
#define DISABLE_VTD_TXT                       131
#define ENABLE_ACTTPM_VT_VTD_TXT_DISABLE_STM  132
#define ENABLE_ACTTPM_VT_VTD_TXT_STM          133
#define DISABLE_STM                           134
#define DISABLE_TXT_STM                       135
#define DISABLE_SENTER_VMX                    136
#define ENABLE_VMX_SMX_ONLY                   137
#define ENABLE_VMX_OUTSIDE_SMX                138
#define ENABLE_VMX                            139
#define ENABLE_SENTER_ONLY                    140
#define ENABLE_SENTER_VMX_IN_SMX              141
#define ENABLE_SENTER_VMX_OUTSIDE_SMX         142
#define ENABLE_SENTER_VMX                     143
#define SET_NO_TXT_MAINTENANCE_FALSE          159
#define SET_NO_TXT_MAINTENANCE_TRUE           160

#endif
