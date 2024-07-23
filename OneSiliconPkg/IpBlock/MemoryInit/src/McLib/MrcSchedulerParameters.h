/** @file
  This module includes the memory controller scheduler parameters.

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
#ifndef _MrcSchedulerParameters_h_
#define _MrcSchedulerParameters_h_

#include "CMrcTypes.h"
#include "CMcAddress.h"
#include "MrcCommon.h"

#define SCHED_CBIT_DEFAULT    (0x00100030)
#define SCHED_CBIT_DEFAULT_B0 (0x00100000)

#define SC_WDBWM_DEFAULT      (0x553C3038)

#define VC0_LATENCY_GUARD_TIMER_x16_PS     (500 * 1000) // 500ns
#define VC1_LATENCY_GUARD_TIMER_x8_PS      (500 * 1000) // 500ns
#define VC1_ISOC_LATENCY_GUARD_TIMER_x8_PS (200 * 1000) // 200ns
#define LATENCY_SENSITIVE_TIMER_x16_PS     (100 * 1000) // 100ns

#define CMI_SOURCE_ID0 (0x4)
#define CMI_SOURCE_ID1 (0x5)
#define CMI_SOURCE_ID2 (0x8)
#define CMI_SOURCE_ID3 (0x9)

/**
  This function configures the memory controller scheduler.

  @param[in] MrcData - Include all MRC global data.

  @retval Nothing.
**/
void
MrcSchedulerParametersConfig (
  IN MrcParameters *const MrcData
  );

/**
  Set ODT Logic behavior.

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]      Profile - behavior Type. Currently Profile 1 is supported

  @retval none
**/
void
MrcSetOdtMatrix (
  IN OUT MrcParameters *const MrcData,
  IN UINT8                    Profile
  );

/**
  Configure DDR5_1DPC performance feature.

  @param[in] MrcData - Include all MRC global data.

  @retval Nothing.
**/
void
MrcMcOneDpcConfig (
  IN MrcParameters *const MrcData
  );

/**
  This function enable/disables the Memory Controller low power management features.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] IsEnabled  - TRUE enables all PM features, FALSE disables them.

  @retval Nothing.
**/
void
MrcMcLowPowerMode (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsEnabled
  );

/**
  This function configures the MC to drive the WCK signals to DDRIO allowing WCK to park differentially low when in IDLE.

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] IsCbtEnter - Defines if the call is for a CBT enter (TRUE) or CBT exit (FALSE)
**/
VOID
MrcMcLpddrCbtWckMode (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              IsCbtEnter
  );
#endif // _MrcSchedulerParameters_h_
