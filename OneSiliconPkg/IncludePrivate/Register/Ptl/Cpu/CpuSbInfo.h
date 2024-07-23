/** @file
  Register names for CPU Side Band Information

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
#ifndef _CPU_SB_INFO_PTL_CDIE_H_
#define _CPU_SB_INFO_PTL_CDIE_H_

//
// CPU SB Segment ID
//

#define CPU_SB_SID_COMPUTE_MAIN   0x0E
#define CPU_SB_SID_GT_PB          0x10
#define CPU_SB_SID_IOC_IVTU       0x13
#define CPU_SB_SID_CHIPSET16      0xF2

//
// CPU SB Device
//

#define CPU_SB_PID_PSF0           0x67
#define CPU_SB_PID_PSF1           0x64
#define CPU_SB_PID_PSF2           0x54
#define CPU_SB_PID_IOM            0xC1

#define CPU_SB_PID_NOC_MAIN_4     0x1
#define CPU_SB_PID_CMI_NOC_1      0x3
#define CPU_SB_PID_CMI_NOC        0x4
#define CPU_SB_PID_NOC_MAIN_0     0x5
#define CPU_SB_PID_NOC_MAIN_1     0x6
#define CPU_SB_PID_NOC_MAIN_2     0x7
#define CPU_SB_PID_NOC_MAIN_3     0x8

#define CPU_SB_PID_PUNIT          0x46
#define CPU_SB_PID_SVTU           0x50
#define CPU_SB_PID_MEDIA_PB       0x69
#define CPU_SB_PID_IPU_PB         0x88
#define CPU_SB_PID_IAX_PB         0x8E
#define CPU_SB_PID_NPU_PB         0x90
#define CPU_SB_PID_DNI2CFI        0x9A
#define CPU_SB_PID_GT_PB          0x7B
#define CPU_SB_PID_IOC            0x58
#define CPU_SB_PID_IVTU           0x59

#define CPU_SB_PID_NPU_BUTTREESS  0xE0

//
// PMON (Performance Monitoring) Port Id
//
#define PMON_PORTID_MC0           0x5A
#define PMON_PORTID_MC1           0x5C
#define PMON_PORTID_HBO0          0x54
#define PMON_PORTID_HBO1          0x53
#define PMON_PORTID_NCU           0x4B
#endif  // _CPU_SB_INFO_H_
