/** @file
  The GUID definition for Pch DMI Hob

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#ifndef _PCH_DMI_HOB_H_
#define _PCH_DMI_HOB_H_

#include <Base.h>

#define PCH_DMI_HWEQ_COEFFS_MAX    8

extern EFI_GUID gPchDmiHobGuid;
#pragma pack (push,1)

/**
  The PCH_DMI_HOB block describes the expected configuration of Pch Dmi Equalization
**/
typedef struct {
  EFI_HOB_GUID_TYPE           EfiHobGuidType;                           ///< Offset 0 - 23: GUID Hob type structure for gPchPcieHobGuid
  ///
  /// These members describe the configuration of each PCH Dmi root port.
  ///
  UINT8                       CmGen3[PCH_DMI_HWEQ_COEFFS_MAX];          ///< Coefficient C-1 for Gen3
  UINT8                       CpGen3[PCH_DMI_HWEQ_COEFFS_MAX];          ///< Coefficient C+1 for Gen3
  UINT8                       CmGen4[PCH_DMI_HWEQ_COEFFS_MAX];          ///< Coefficient C-1 for Gen4
  UINT8                       CpGen4[PCH_DMI_HWEQ_COEFFS_MAX];          ///< Coefficient C+1 for Gen4
} PCH_DMI_HOB;
#pragma pack (pop)
#endif
