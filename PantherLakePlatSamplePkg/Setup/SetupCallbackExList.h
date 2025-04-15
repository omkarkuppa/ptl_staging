/** @file
  Internal header of the Setup Component.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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

#define EVAL(a) a

#define OC_SETUP_ITEM_CALLBACK_LIST
#define SETUP_ITEM_CALLBACK_LIST_EX                                                                                                      \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitRatio0, CpuFormCallBackFunction),                                           \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitRatio1, CpuFormCallBackFunction),                                           \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitRatio2, CpuFormCallBackFunction),                                           \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitRatio3, CpuFormCallBackFunction),                                           \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitRatio4, CpuFormCallBackFunction),                                           \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitRatio5, CpuFormCallBackFunction),                                           \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitRatio6, CpuFormCallBackFunction),                                           \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitRatio7, CpuFormCallBackFunction),                                           \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitNumCore0, CpuFormCallBackFunction),                                         \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitNumCore1, CpuFormCallBackFunction),                                         \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitNumCore2, CpuFormCallBackFunction),                                         \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitNumCore3, CpuFormCallBackFunction),                                         \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitNumCore4, CpuFormCallBackFunction),                                         \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitNumCore5, CpuFormCallBackFunction),                                         \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitNumCore6, CpuFormCallBackFunction),                                         \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_RatioLimitNumCore7, CpuFormCallBackFunction),                                         \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitRatio0, CpuFormCallBackFunction),                                       \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitRatio1, CpuFormCallBackFunction),                                       \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitRatio2, CpuFormCallBackFunction),                                       \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitRatio3, CpuFormCallBackFunction),                                       \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitRatio4, CpuFormCallBackFunction),                                       \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitRatio5, CpuFormCallBackFunction),                                       \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitRatio6, CpuFormCallBackFunction),                                       \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitRatio7, CpuFormCallBackFunction),                                       \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitNumCore0, CpuFormCallBackFunction),                                     \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitNumCore1, CpuFormCallBackFunction),                                     \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitNumCore2, CpuFormCallBackFunction),                                     \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitNumCore3, CpuFormCallBackFunction),                                     \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitNumCore4, CpuFormCallBackFunction),                                     \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitNumCore5, CpuFormCallBackFunction),                                     \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitNumCore6, CpuFormCallBackFunction),                                     \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_AtomRatioLimitNumCore7, CpuFormCallBackFunction),                                     \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_BootFrequency, CpuFormCallBackFunction),                                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_TurboMode, CpuFormCallBackFunction),                                                  \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_CpuRatioLimit, CpuFormCallBackFunction),                                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_DTBT_SUPPORT,DTbtFormCallBackFunction),                                               \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_DTBT_CONTROLLER0_HOSTROUTER,DTbtFormCallBackFunction),                                \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_DTBT_CONTROLLER1_HOSTROUTER,DTbtFormCallBackFunction),                                \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_ITBT_SUPPORT, ITbtFormCallBackFunction),                                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_LOW_POWER_S0_IDLE, LowPowerS0IdleEnableCallback),                                     \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, MNG_STATE_KEY, MeFormCallBackFunction),                                                   \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, DAM_STATE_KEY, MeFormCallBackFunction),                                                   \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, ME_FORM_ACTION_KEY, MeFormCallBackFunction),                                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, AMT_FORM_ACTION_KEY, MeFormCallBackFunction),                                             \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, PLATFORM_DEBUG_OPTION_KEY, DebugFormCallBackFunction),                                    \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_SA_TCSS_XHCI_EN, TcssXhciCallback),                                                   \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_ENABLE_THERMAL_FUN, ThermalFunctionCallback),                                         \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_TXT_POLICY_FIT, TxtPolicyCallBackFunction),                                           \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_PCH_STATE_AFTER_G3, StateAfterG3CallBackFunction),                                    \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_CoreVrEnableFastVmode, CpuVrConfigCallBackFunction),                                  \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_GtVrEnableFastVmode, CpuVrConfigCallBackFunction),                                    \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_SaVrEnableFastVmode, CpuVrConfigCallBackFunction),                                    \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IccLimit0, CpuVrConfigCallBackFunction),                                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IccLimit1, CpuVrConfigCallBackFunction),                                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IccLimit2, CpuVrConfigCallBackFunction),                                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_USB4_CM_MODE, Usb4CmModeCallBack),                                                    \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_DEEPEST_USB_SLEEP_WAKE_CAPABILITY, DeepestUsbSleepWakeCapabilityCallBack),            \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, SA_VTD_QUESTION_ID,VtdCallBackFunction),                                                  \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, MKTME_STATUS_QUESTION_ID, MktmeCallBackFunction),                                         \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, VT_QUESTION_ID, VTCallBackFunction),                                                      \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, RTD3_SUPPORT_QUESTION_ID, Rtd3SupportCallBackFunction),                                   \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_BASE0_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_BASE1_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_BASE2_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_BASE3_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_BASE4_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_BASE5_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_BASE6_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_BASE7_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_MASK0_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_MASK1_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_MASK2_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_MASK3_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_MASK4_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_MASK5_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_MASK6_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_IBECC_MASK7_CHECK, IbeccRangeCheckCallBack),                                          \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, RTD3_SUPPORT_QUESTION_ID, Rtd3SupportCallBackFunction),                                   \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, TRACEHUB_BUFFER_SIZE_KEY,  TraceHubMemRegSizeCheckCallBack),                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, TRACEHUB_BUFFER_SIZE_KEY1, TraceHubMemRegSizeCheckCallBack),                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, TRACEHUB_BUFFER_SIZE_KEY2, TraceHubMemRegSizeCheckCallBack),                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, TRACEHUB_BUFFER_SIZE_KEY3, TraceHubMemRegSizeCheckCallBack),                              \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_DLVR_RFIM, CpuFormCallBackFunction),                                                  \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, PROCESSOR_TRACE_MEM_SIZE_KEY, ProcessorTraceMemSizeCheckCallBack),                        \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, KEY_SA_VMD_GLOBAL_MAPPING, VmdCallback),                                                  \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, PCH_HDAUDIO_DSP_FEATURES_FORM_ACTION_KEY, HdaDspCallBack),                                \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, PCH_HDAUDIO_DBT_OFFLOAD_ACTION_KEY, HdaDspCallBack),                                      \
  ITEM_CALLBACK_EX(ADVANCED_FORM_SET_CLASS, 0, PCH_HDAUDIO_BT_SIDEBAND_ACTION_KEY, HdaDspCallBack),                                      \
  EVAL(OC_SETUP_ITEM_CALLBACK_LIST)
