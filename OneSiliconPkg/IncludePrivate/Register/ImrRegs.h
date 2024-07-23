/** @file
  Register names for IMR registers

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _IMR_REGS_H_
#define _IMR_REGS_H_

#include <Register/HostBridgeRegs.h>

#define IMR_BASE_2_TYPE_BASE    IMR0BASE_IMPH_IOC_MCHBAR_REG
#define IMR_BASE_1M_TYPE_BASE   IMR1M0BASE_IMPH_IOC_MCHBAR_REG
#define S_IMR_BASE_TYPE         0x0030

typedef enum {
  Csme0,
  Ish,
  Audio,
  IpuCamera,
  IpuScratchpad,
  KvmImr,
  TcssIom,
  TcssTbt,
  Cnvi,
  Csme1,
  TcssIomSR,
  Sse0,
  Sse1,
  Csme2,
  Csme3,
  TraceHubImr,
  FusaImr,
  Prmrr,
  RemoteTrace,
  PseImr,
  AvbImr,
  TseImr,
  NpuSpare,
  IpuSpare,
  DisplayMediaSpare,
  PunitSpare
} IMR_Base_2_Type;

typedef enum {
  Pmrh,
  Pmrl,
  Gsm,
  Dsm,
  Imr1M4,
  Imr1M5,
  Ccs,
  Tpr0,
  Tpr1,
  Dpr,
  Tseg,
  Imr1M13,
  Imr1M14,
  Imr1M15
} IMR_1M_Type;

#define IMR_BASE_2_TYPE_BASE_OFFSET(x)  IMR_BASE_2_TYPE_BASE + (x) * S_IMR_BASE_TYPE
#define IMR_BASE_2_TYPE_MASK_OFFSET(x)  IMR_BASE_2_TYPE_BASE_OFFSET (x) + 0x8
#define IMR_1M_TYPE_BASE_OFFSET(x)      IMR_1M_TYPE_BASE + (x) * S_IMR_BASE_TYPE
#define IMR_1M_TYPE_MASK_OFFSET(x)      IMR_1M_TYPE_BASE_OFFSET (x) + 0x8

#endif  // _IMR_REGS_H_
