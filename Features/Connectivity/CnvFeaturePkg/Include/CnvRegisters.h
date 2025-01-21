/**@file
  Definition for Guid Lock Status Indicator Protocol API.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _CNV_REGISTERS_H_
#define _CNV_REGISTERS_H_

#define CNVD_REG_NUM                                    4
//
//  CNVd GaP Registers Offset
//
#define CNVD_MAX_NO_SNOOP_LATENCY_REG_OFFSET            0x18C   // Max No-Snoop Latency, Max Snoop Latency
#define CNVD_L1_PM_SUB_STATES_CONTROL1_REG_OFFSET       0x198   // L1 PM Substates Control 1 Register
#define CNVD_L1_PM_SUB_STATES_CONTROL2_REG_OFFSET       0x19C   // L1 PM Substates Control 2 Register
#define CNVD_PTM_REG_OFFSET                             0x2B4   // PTM

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mCnvdRegOffestTable[] = {
  CNVD_MAX_NO_SNOOP_LATENCY_REG_OFFSET,       // Max No-Snoop Latency, Max Snoop Latency
  CNVD_L1_PM_SUB_STATES_CONTROL1_REG_OFFSET,  // L1 PM Substates Control 1 Register
  CNVD_L1_PM_SUB_STATES_CONTROL2_REG_OFFSET,  // L1 PM Substates Control 2 Register
  CNVD_PTM_REG_OFFSET                         // PTM
};

#endif //_CNV_REGISTERS_H_
