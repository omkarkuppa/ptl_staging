/** @file
  Nmve setup

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

@par Specification Reference:
**/

#ifndef _NVME_SETUP_H_
#define _NVME_SETUP_H_

#pragma pack(1)
#define NVME_SETUP_REVISION 1
typedef struct {
  UINT8   PlnEnable;
  UINT8   NvmeRwRecoveryEnable;
  UINT8   NvmeReMaxAttempt;
} NVME_SETUP;

#pragma pack()

#define NVME_SETUP_GUID \
 { 0xB0F5c22A, 0xE7E0, 0x4658, { 0xB9, 0xFF, 0x22, 0x7E, 0x14, 0xB1, 0xAB, 0x3D } }

#define NVME_SETUP_DATA_VARSTORE\
    efivarstore NVME_SETUP, \
        attribute = 0x7, name = NvmeSetup, guid = NVME_SETUP_GUID;

#endif /*_NVME_SETUP_H_*/
