/** @file
  The GUID definition for DpDmiHob

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

@par Specification Reference:
**/
#ifndef _DMI_HOB_H_
#define _DMI_HOB_H_

#include <Base.h>

#define DP_DMI_HWEQ_COEFFS_MAX    8

extern EFI_GUID gDmiHobGuid;
#pragma pack (push,1)

/**
  The DP_DMI_HOB_INFO block describes the expected configuration of Dp Dmi
**/
typedef struct {
  ///
  /// These members describe the configuration of each DP Dmi root port.
  ///
  UINT8   CmGen3[DP_DMI_HWEQ_COEFFS_MAX];                 ///< Coefficient C-1 for Gen3
  UINT8   CpGen3[DP_DMI_HWEQ_COEFFS_MAX];                 ///< Coefficient C+1 for Gen3
  UINT8   CmGen4[DP_DMI_HWEQ_COEFFS_MAX];                 ///< Coefficient C-1 for Gen4
  UINT8   CpGen4[DP_DMI_HWEQ_COEFFS_MAX];                 ///< Coefficient C+1 for Gen4
} DP_DMI_INFO_HOB;

/**
  The DMI_HOB block describes the expected configuration of Dmi
**/
typedef struct {
  EFI_HOB_GUID_TYPE           EfiHobGuidType;                           ///< Offset 0 - 23: GUID Hob type structure for gCpuDmieHobGuid
  DP_DMI_INFO_HOB             DpDmiInfoHob;
} DMI_HOB;
#pragma pack (pop)
#endif
