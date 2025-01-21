/** @file
  Register names for CPU registers

  <b>Conventions</b>
  - Definitions beginning with "MSR_" are MSRs
  - Definitions beginning with "R_" are registers
  - Definitions beginning with "B_" are bits within registers
  - Definitions beginning with "V_" are meaningful values of bits within the registers
  - Definitions beginning with "S_" are register sizes
  - Definitions beginning with "N_" are the bit position

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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
#ifndef _CPU_REGS_H_
#define _CPU_REGS_H_

#define V_CSTATE_LIMIT_C1                                             0x01
#define V_CSTATE_LIMIT_C3                                             0x02
#define V_CSTATE_LIMIT_C6                                             0x03
#define V_CSTATE_LIMIT_C7                                             0x04
#define V_CSTATE_LIMIT_C7S                                            0x05
#define V_CSTATE_LIMIT_C8                                             0x06
#define V_CSTATE_LIMIT_C9                                             0x07
#define V_CSTATE_LIMIT_C10                                            0x08

///
/// Other defines
///

#ifndef CPU_FEATURE_DISABLE
#define CPU_FEATURE_DISABLE  0
#endif
#ifndef CPU_FEATURE_ENABLE
#define CPU_FEATURE_ENABLE   1
#endif

#endif
