/**@file
  Ata storage device library header file to support secure feature

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

#ifndef _ATA_SECURE_H_
#define _ATA_SECURE_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>
#include <Library/AhciCommandLib.h>

#define MAX_PASSWORD_TRY_COUNT   3

typedef struct {
  UINT8     Supported            : 1;
  UINT8     Enabled              : 1;
  UINT8     Locked               : 1;
  UINT8     Frozen               : 1;
  UINT8     CountExpired         : 1;
  UINT8     UserPasswordStatus   : 1;
  UINT8     MasterPasswordStatus : 1;
  UINT8     Reserved             : 1;
} ATA_PASSWORD_SECURITY_STATUS;

#endif /*_ATA_SECURE_H_*/
