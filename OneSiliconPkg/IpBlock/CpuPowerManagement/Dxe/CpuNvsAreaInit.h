/** @file
  Private data structures and function prototypes.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _CPU_NVS_AREA_INIT_H
#define _CPU_NVS_AREA_INIT_H

#define MWAIT_C6                     0x20
#define MWAIT_C6_1                   0x21
#define MWAIT_C7                     0x30
#define MWAIT_C7_1                   0x31
#define MWAIT_C7_2                   0x32
#define MWAIT_C7_3                   0x33
#define MWAIT_CD                     0x40
#define MWAIT_CD_1                   0x50
#define MWAIT_CD_2                   0x60

/**
  Initialize Cpu Nvs Area Protocol

  @retval EFI_SUCCESS           - thread can be successfully created
**/
EFI_STATUS
InitCpuNvsArea (
  VOID
  );

#endif
