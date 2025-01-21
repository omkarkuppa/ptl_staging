/** @file
  The GUID definition for VrDataHob

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
#ifndef _VR_DATA_HOB_H_
#define _VR_DATA_HOB_H_

#include <CpuPowerMgmtVrConfig.h>

extern EFI_GUID gVrDataHobGuid;


///
/// Vr Data Hob
///
typedef struct {
  UINT8                    SvidEnabled;  ///< Svid Enabled\Disabled according to various VR domains. Refer to the definition in Fru.
  UINT8                    MaxNumVrs;
  UINT8                    VrAddress[MAX_NUM_VRS]; ///< VR address according to various VR domains.
  UINT16                   IccMaxItrip[MAX_NUM_VRS]; /// < VR default ICCMAX value when FVM enabled.
  UINT8                    FastVmodeSupported[MAX_NUM_VRS]; ///< Indicate platform supports FVM or not.
} VR_DATA_HOB;

#endif // _VR_DATA_HOB_H_
