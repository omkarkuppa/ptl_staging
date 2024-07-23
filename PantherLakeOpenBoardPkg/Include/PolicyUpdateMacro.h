/**@file
  Macros for platform to update different types of policy.

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

@par Specification
**/
#ifndef _POLICY_UPDATE_MACRO_H_
#define _POLICY_UPDATE_MACRO_H_

#ifdef UPDATE_POLICY
#undef UPDATE_POLICY
#endif

#ifdef COPY_POLICY
#undef COPY_POLICY
#endif

#ifdef GET_POLICY
#undef GET_POLICY
#endif

#ifdef AND_POLICY
#undef AND_POLICY
#endif

#ifdef OR_POLICY
#undef OR_POLICY
#endif

#define UPDATE_POLICY(ConfigField, Value)  ConfigField = Value;
#define COPY_POLICY(ConfigField, Value, Size)  CopyMem (ConfigField, Value, Size);
#define GET_POLICY(ConfigField, Value)  Value = ConfigField;
#define AND_POLICY(ConfigField, Value)  ConfigField &= Value;
#define OR_POLICY(ConfigField, Value)  ConfigField |= Value;

#endif //_POLICY_UPDATE_MACRO_H_
