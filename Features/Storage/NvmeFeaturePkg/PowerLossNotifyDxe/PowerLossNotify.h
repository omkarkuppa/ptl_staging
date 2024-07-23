/**@file
  Power Loss Notification Dxe driver provides functionlity to enable or disable PLN feature.

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
#ifndef _POWER_LOSS_NOTIFY_H_
#define _POWER_LOSS_NOTIFY_H_

#include <Uefi.h>
#include <Protocol/NvmExpressPassthru.h>
#include <Protocol/PowerLossNotifySetting.h>
#include <IndustryStandard/Pci22.h>
#include <IndustryStandard/Nvme.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Base.h>

//
// define fields of NVME Get/Set Feature command. Refer to NVMe Specification
//
#define NVME_CONTROLLER_ID                       0      //A value of 0 denotes the NVM Express controller
#define NVME_ALL_NAMESPACES                     0xFFFFFFFF
#define PLN_FEATURE_ID                          0xD8
#define FEATURE_SV_OFFSET                       31
#define FEATURE_SEL_OFFSET                      8
#define FEATURE_SEL_MASK                        0xF00
#define FEATURE_SEL_CURRENT                     0x0

typedef struct {
  UINT32                            Reserved1:17;
  UINT32                            StatusCode:8;
  UINT32                            StatusCodeType:3;
  UINT32                            Reserved2:2;
  UINT32                            More:1;
  UINT32                            DoNotRetry:1;
} SAVE_FEATURE_DW3;

typedef struct {
  UINT32                            PlnEnable:1;
  UINT32                            Reserved:31;
} SAVE_FEATURE_DW0;

typedef struct {
  UINT32                            PlnEnable:1;
  UINT32                            Reserved:31;
} GET_FEATURE_ATTR_CUR;

#endif
