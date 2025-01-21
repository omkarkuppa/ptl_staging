/** @file
  ADR policy

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

#ifndef _ADR_CONFIG_H_
#define _ADR_CONFIG_H_

#include <ConfigBlock.h>

#define ADR_CONFIG_REVISION 1
extern EFI_GUID gAdrConfigGuid;

#pragma pack (push,1)

typedef enum {
  AdrScale1us,
  AdrScale10us,
  AdrScale100us,
  AdrScale1ms,
  AdrScale10ms,
  AdrScale100ms,
  AdrScale1s,
  AdrScale10s
} ADR_TIMER_SCALE;

/**
  ADR Source Enable
**/
typedef union {
  struct {
    UINT32 Reserved1           : 1;
    UINT32 AdrSrcPbo           : 1;
    UINT32 AdrSrcPmcUncErr     : 1;
    UINT32 AdrSrcPchThrm       : 1;
    UINT32 AdrSrcMePbo         : 1;
    UINT32 AdrSrcCpuThrm       : 1;
    UINT32 AdrSrcMegbl         : 1;
    UINT32 AdrSrcLtReset       : 1;
    UINT32 AdrSrcPmcWdt        : 1;
    UINT32 AdrSrcMeWdt         : 1;
    UINT32 AdrSrcPmcFw         : 1;
    UINT32 AdrSrcPchpwrFlr     : 1;
    UINT32 AdrSrcSyspwrFlr     : 1;
    UINT32 Reserved2           : 1;
    UINT32 AdrSrcMiaUxsErr     : 1;
    UINT32 AdrSrcMiaUxErr      : 1;
    UINT32 AdrSrcCpuThrmWdt    : 1;
    UINT32 AdrSrcMeUncErr      : 1;
    UINT32 AdrSrcAdrGpio       : 1;
    UINT32 AdrSrcOcwdtNoicc    : 1;
    UINT32 AdrSrcOcwdtIcc      : 1;
    UINT32 AdrSrcCseHecUncErr  : 1;
    UINT32 AdrSrcPmcSramUncErr : 1;
    UINT32 AdrSrcPmcIromParity : 1;
    UINT32 AdrSrcPmcRfFusaErr  : 1;
    UINT32 Reserved3           : 4;
    UINT32 AdrSrcPpbrParityErr : 1;
    UINT32 Reserved4           : 2;
  } Field;
  UINT32 Value;
} ADR_GLOBAL_RESET_ENABLE;

/**
  ADR Configuration
  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  UINT32        AdrEn                  : 2; ///< Determine if Adr is enabled - 0: PLATFORM_POR, 1: FORCE_ENABLE, 2: FORCE_DISABLE
  UINT32        AdrTimerEn             : 2; ///< Determine if Adr timer options are enabled - 0: PLATFORM_POR, 1: FORCE_ENABLE, 2: FORCE_DISABLE
  UINT32        AdrTimer1Val           : 8; ///< Determines the Timeout value used for the ADR timer 1. A value of zero bypasses the timer
  UINT32        AdrMultiplier1Val      : 3; ///< Specifies the tick frequency upon which the timer 1 will increment. ADR_TIMER_SCALE should be used to encode values
  UINT32        AdrTimer2Val           : 8; ///< Determines the Timeout value used for the ADR timer 2. A value of zero bypasses the timer
  UINT32        AdrMultiplier2Val      : 3; ///< Specifies the tick frequency upon which the timer 2 will increment. ADR_TIMER_SCALE should be used to encode values
  UINT32        AdrHostPartitionReset  : 2; ///< Determine if Host Partition Reset is enabled - 0: PLATFORM_POR, 1: FORCE_ENABLE, 2: FORCE_DISABLE
  UINT32        AdrPlatAckEn           : 2; ///< Determine if Platform Acknowledge is enabled - 0: PLATFORM_POR, 1: FORCE_ENABLE, 2: FORCE_DISABLE
  UINT32        ReservedBits           : 2;
  ADR_GLOBAL_RESET_ENABLE   AdrSrcOff;      ///< Determine which ADR sources are disabled. The code perform clearing bits in ADR Global Reset Enable register. This is first step.
  ADR_GLOBAL_RESET_ENABLE   AdrSrcOn;       ///< Determine which ADR sources are enabled. The code perform setting bits in ADR Global Reset Enable register. This is second step.
} ADR_CONFIG;

#pragma pack (pop)

#endif // _ADR_CONFIG_H_
