/** @file
  Definition for SysFwUpdateProgress Variable and Capsule backup filenames
  used for Seamless Recovery support

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

@par Specification
**/

#ifndef __SYSTEM_FIRMWARE_UPDATE_PROGRESS_H__
#define __SYSTEM_FIRMWARE_UPDATE_PROGRESS_H__

#define SYSFW_UPDATE_PROGRESS_GUID \
  { \
    0xf92b8157, 0xc647, 0x44d7, { 0x8d, 0x94, 0x81, 0x7d, 0x18, 0xa2, 0x76, 0xdc } \
  }

#define SYSFW_UPDATE_PROGRESS_VARIABLE_NAME                 L"SysFwUpdateProgress"
#define SYSFW_UPDATE_CAPSULE_DIGEST_VARIABLE_NAME           L"CapDigest"
#define CAPSULE_LAST_BACKUP_VARIABLE_NAME                   L"CapLastBackup"

#define SYSFW_UPDATE_CURRENT_OBB_BACKUP_FILE_NAME           L"Obb.bin"
#define SYSFW_UPDATE_CURRENT_EXTENDED_BIOS_BACKUP_FILE_NAME L"ExtendedBios.bin"
#define SYSFW_UPDATE_CURRENT_PAYLOAD_BACKUP_FILE_NAME       L"NonFitPayload.bin"
#define SYSFW_UPDATE_NEW_OBB_BACKUP_FILE_NAME               L"ObbN.bin"
#define SYSFW_UPDATE_NEW_EXTENDED_BIOS_BACKUP_FILE_NAME     L"ExtendedBiosN.bin"
#define SYSFW_UPDATE_NEW_PAYLOAD_BACKUP_FILE_NAME           L"NonFitPayloadN.bin"

#define SYSFW_UPDATE_CAPSULE_BACKUP_FILE_NAME               L"Capsule.cap"
#define SYSFW_UPDATE_WINDOWS_UX_CAPSULE_FILE_NAME           L"UxCap.cap"
#define SYSFW_UPDATE_ME_RECOVERY_CAPSULE_NAME               L"MeRecov.Cap"
#define SYSFW_UPDATE_ME_RECOVERY_CAPSULE_NAME_NEW           L"MeRecovN.Cap"

#define SYSFW_UPDATE_RESILIENCY_PENDING_ACTION_EC_RESET  BIT0
//
// SYSFW_UNDER_MONOLITHIC_UPDATE indicates whether is under monolithic capsule update.
// 0: system is not under monolithic update.
// 1: system is under monolithic update.
//
#define SYSFW_UNDER_MONOLITHIC_UPDATE                    BIT0

typedef enum {
  NotStarted              = 0,
  UpdatingMe              = 1,
  UpdatingIshPdt          = 2,
  UpdatingBios            = 3,
  UpdatingEc              = 4,
  UpdatingGbE             = 5,
  UpdatingBtGAcm          = 7,
  UpdatinguCode           = 8,
  UpdatingResiliency      = 9, // Post-BIOS update phase. Indicates BIOS backup requirement.
  UpdatingMeResiliency    = 10,
  UpdatingFsp             = 11,
  UpdatingTypeMax
} SYSTEM_FIRMWARE_COMPONENT;

typedef enum {
  BiosIbbR     = 0,
  BiosIbb,
  BiosObb,
  BiosMax
} BIOS_UPDATE_PROGRESS;

typedef enum {
  EcMainImage        = 0,
  EcRecoveryImage    = 1,
  EcMax
} EC_UPDATE_PROGRESS;

#pragma pack(1)

typedef struct {
  SYSTEM_FIRMWARE_COMPONENT  Component;
  UINT16                     Progress;
  UINT16                     Flag;
} SYSTEM_FIRMWARE_UPDATE_PROGRESS;

#pragma pack()

extern EFI_GUID gSysFwUpdateProgressGuid;

#endif
