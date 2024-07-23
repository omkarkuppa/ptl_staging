/** @file
  Header file for PchInfoDefs.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _PCH_INFO_DEFS_H_
#define _PCH_INFO_DEFS_H_


#define PCH_A0                0x00
#define PCH_A1                0x01
#define PCH_B0                0x10
#define PCH_B1                0x11
#define PCH_C0                0x20
#define PCH_C1                0x21
#define PCH_D0                0x30
#define PCH_D1                0x31
#define PCH_Z0                0xF0
#define PCH_Z1                0xF1
#define PCH_STEPPING_MAX      0xFF

#define PCH_H                   1
#define PCH_LP                  2
#define PCH_N                   3
#define PCH_S                   4
#define PCH_P                   5
#define PCH_M                   6
#define PCH_SERVER              0x80
#define PCH_UNKNOWN_SERIES      0xFF

#define TGL_PCH                 5
#define ADL_PCH                 7
#define MTL_SOC                 8
#define LNL_SOC                 0xA
#define PTL_SOC                 0xB
#define CDF_PCH                 0x80
#define EBG_PCH                 0x81
#define S3M2_IBL                0x90
#define PCH_UNKNOWN_GENERATION  0xFF

#endif
