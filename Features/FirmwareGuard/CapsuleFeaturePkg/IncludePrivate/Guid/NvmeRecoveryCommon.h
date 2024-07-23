/** @file
  Definition for NvmeRecoveryInfoVar Variable and NVME_BACKUP_INFO used
  for NVMe Recovery support

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef _NVME_RECOVERY_COMMON_H_
#define _NVME_RECOVERY_COMMON_H_

#include <Uefi.h>
#include <Guid/NvmeRecoveryHiiFormValues.h>

#define NVME_BASED_RECOVERY_INFO_VARIABLE  L"NvmeRecoveryInfoVar"

typedef enum {
  BackupNotStarted = 0,
  BackupSuccess,
  BackupUnsupported
} NVME_BACKUP_STATUS;

#define NVME_BACKUP_INFO_REVISION  0x1  // Please update after change NVME_BACKUP_INFO structure

#pragma pack(1)

typedef struct {
  UINT64              Revision;  // Revision must be set to NVME_BACKUP_INFO_REVISION
  NVME_BACKUP_STATUS  BackupStatus;
  UINT32              Reserved;
} NVME_BACKUP_INFO;

#pragma pack()

extern EFI_GUID gNvmeBasedRecoveryVarGuid;

#endif
