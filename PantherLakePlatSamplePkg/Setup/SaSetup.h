/** @file
  Interface definition details for North Bridge Setup Routines.

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

#ifndef __SA_SETUP_H__  // To Avoid this header get compiled twice
#define __SA_SETUP_H__

#define MAX_TOLUD_DYNAMIC  0
#define MAX_TOLUD_1G       1
#define MAX_TOLUD_1_25G    2
#define MAX_TOLUD_1_5G     3
#define MAX_TOLUD_1_75G    4
#define MAX_TOLUD_2G       5

#define MEM_4GB   (0x1000)  ///< Define the 4GB size in 1MB units (1024MB = 1GB).
#define MEM_8GB   (0x2000)
#define MEM_12GB  (0x3000)
#define MEM_16GB  (0x4000)

#define MAX_IBECC_REGION_OVERLAP    4

//
// SPD Profile
//
#define DEFAULT_SPD_PROFILE 0
#define CUSTOM_PROFILE      1
#define XMP_PROFILE_1       2
#define XMP_PROFILE_2       3

#define CS_ICL_A0     0

#define DISABLE   0
#define DP_ONLY   1
#define NO_TBT    2
#define NO_PCIE   3
#define FULL_FUNC 7

#endif
