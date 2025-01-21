/** @file
  The definition for TprInfHob

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
#ifndef _TPR_INFO_HOB_H_
#define _TPR_INFO_HOB_H_

#include <Base.h>

extern EFI_GUID gTprInfoHobGuid;

#pragma pack (push,1)

typedef struct {
  UINT8                TprCount;
  UINT64               Tpr0BaseOffset;
  UINT64               Tpr0LimitOffset;
  UINT64               Tpr1BaseOffset;
  UINT64               Tpr1LimitOffset;
} INSTANCE_ARRAY;

typedef struct {
  UINT64               GtDrainControl;
  UINT64               MediaDrainControl;
  UINT64               DisplayDrainControl;
  UINT64               IaxDrainControl;
  UINT64               IocDrainControl;
  UINT64               IvtuDrainControl;
  UINT64               SvtuDrainControl;
  UINT64               IpuDrainControl;
  UINT64               NpuDrainControl;
  UINT64               PunitDrainControl;
} DRAIN_CTL_REG;

///
/// TPR Info Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE    EfiHobGuidType;                           ///< GUID Hob type structure for gTprInfoHobGuid
  UINT64               Tpr0Base;
  UINT64               Tpr0Limit;
  UINT64               Tpr1Base;
  UINT64               Tpr1Limit;
  UINT8                InstanceCount;
  INSTANCE_ARRAY       InstanceArray[2];
  UINT8                DrainControlCount;
  DRAIN_CTL_REG        DrainControlArray;
} TPR_INFO_HOB;

#pragma pack (pop)
#endif
