/** @file
  Macros for platform to update different types of policy.

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

#if FixedPcdGet8(PcdFspModeSelection) == 1


#define UPDATE_POLICY_V2(UpdField, ConfigField, Value)  UpdField = Value;
#define COPY_POLICY_V2(UpdField, ConfigField, Value, Size)  CopyMem (UpdField, Value, Size);
#define GET_POLICY_V2(UpdField, ConfigField, Value)  Value = UpdField;
#define AND_POLICY_V2(UpdField, ConfigField, Value)  UpdField &= Value;
#define OR_POLICY_V2(UpdField, ConfigField, Value)  UpdField |= Value;

#define POLICY_DEBUG_WARNING_V2(UpdField, ConfigField, SetupField)  {\
  DEBUG ((DEBUG_INFO, ""#UpdField"= 0x%x mismatch with "#SetupField"= 0x%x\n", UpdField, SetupField));\
}

#define COMPARE_AND_UPDATE_POLICY_V2(UpdField, ConfigField, Value) {\
  UPDATE_POLICY_V2(UpdField, ConfigField, Value);\
}
#define COMPARE_UPDATE_POLICY_ARRAY_V2(UpdField, ConfigField, Value, ArrayIndex) {\
  UPDATE_POLICY_V2(UpdField, ConfigField, Value);\
}

#else

#define UPDATE_POLICY_V2(UpdField, ConfigField, Value)  ConfigField = Value;
#define COPY_POLICY_V2(UpdField, ConfigField, Value, Size)  CopyMem (ConfigField, Value, Size);
#define GET_POLICY_V2(UpdField, ConfigField, Value)  Value = ConfigField;
#define AND_POLICY_V2(UpdField, ConfigField, Value)  ConfigField &= Value;
#define OR_POLICY_V2(UpdField, ConfigField, Value)  ConfigField |= Value;

#define POLICY_DEBUG_WARNING_V2(UpdField, ConfigField, SetupField)  {\
  DEBUG ((DEBUG_INFO, ""#ConfigField"= 0x%x mismatch with "#SetupField"= 0x%x\n", ConfigField, SetupField));\
}

#if ((!defined(MDEPKG_NDEBUG)) && (FixedPcdGetBool(PcdPolicyDefaultChkEnable) == 1))
#define COMPARE_AND_UPDATE_POLICY_V2(UpdField, ConfigField, Value) {\
  if ((ConfigField != Value) && (IsPolicyDefaultCheckRequired())) {\
    POLICY_DEBUG_WARNING_V2(UpdField, ConfigField, Value);\
  }\
  UPDATE_POLICY_V2(UpdField, ConfigField, Value);\
}

#define COMPARE_UPDATE_POLICY_ARRAY_V2(UpdField, ConfigField, Value, ArrayIndex) {\
  if ((ConfigField != Value) && (IsPolicyDefaultCheckRequired())) {\
    POLICY_DEBUG_WARNING_V2(UpdField, ConfigField, Value);\
    DEBUG ((DEBUG_INFO, "Index= 0x%x\n", ArrayIndex));\
  }\
  UPDATE_POLICY_V2(UpdField, ConfigField, Value);\
}

#else
#define COMPARE_AND_UPDATE_POLICY_V2(UpdField, ConfigField, Value) {\
  UPDATE_POLICY_V2(UpdField, ConfigField, Value);\
}
#define COMPARE_UPDATE_POLICY_ARRAY_V2(UpdField, ConfigField, Value, ArrayIndex) {\
  UPDATE_POLICY_V2(UpdField, ConfigField, Value);\
}
#endif

#endif

#define UPDATE_POLICY(ConfigField, Value)  ConfigField = Value;
#define COPY_POLICY(ConfigField, Value, Size)  CopyMem (ConfigField, Value, Size);
#define GET_POLICY(ConfigField, Value)  Value = ConfigField;
#define AND_POLICY(ConfigField, Value)  ConfigField &= Value;
#define OR_POLICY(ConfigField, Value)  ConfigField |= Value;

#define POLICY_DEBUG_WARNING(ConfigField, SetupField)  {\
  DEBUG ((DEBUG_INFO, ""#ConfigField"= 0x%x mismatch with "#SetupField"= 0x%x\n", ConfigField, SetupField));\
}

#if ((!defined(MDEPKG_NDEBUG)) && (FixedPcdGetBool(PcdPolicyDefaultChkEnable) == 1))
#define COMPARE_AND_UPDATE_POLICY(ConfigField, Value) {\
  if ((ConfigField != Value) && (IsPolicyDefaultCheckRequired())) {\
    POLICY_DEBUG_WARNING(ConfigField, Value);\
  }\
  UPDATE_POLICY(ConfigField, Value);\
}

#define COMPARE_UPDATE_POLICY_ARRAY(ConfigField, Value, ArrayIndex) {\
  if ((ConfigField != Value) && (IsPolicyDefaultCheckRequired())) {\
    POLICY_DEBUG_WARNING(ConfigField, Value);\
    DEBUG ((DEBUG_INFO, "Index= 0x%x\n", ArrayIndex));\
  }\
  UPDATE_POLICY(ConfigField, Value);\
}

#else
#define COMPARE_AND_UPDATE_POLICY(ConfigField, Value) {\
  UPDATE_POLICY(ConfigField, Value);\
}
#define COMPARE_UPDATE_POLICY_ARRAY(ConfigField, Value, ArrayIndex) {\
  UPDATE_POLICY(ConfigField, Value);\
}
#endif

#endif //_POLICY_UPDATE_MACRO_H_
