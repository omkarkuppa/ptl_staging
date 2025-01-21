/** @file
  The GUID definition for Vtd Data Hob

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _VTD_DATA_HOB_H_
#define _VTD_DATA_HOB_H_

#include <VtdInfo.h>
#include <Base.h>

extern EFI_GUID gVtdDataHobGuid;

#pragma pack (push,1)

///
/// The data elements should be initialized by a Platform Module.
/// The data structure is for VT-d driver initialization
///
typedef struct {
  EFI_HOB_GUID_TYPE     EfiHobGuidType;                    ///< GUID Hob type structure for gVtdDataHobGuid
  BOOLEAN               VtdDisable;                        ///< 1 = Avoids programming Vtd bars, Vtd overrides and DMAR table
  UINT32                BaseAddress[VTD_ENGINE_NUMBER];    ///< This field is used to describe the base addresses for VT-d function
  BOOLEAN               DmaControlGuarantee;               ///< This field is used to enable the DMA_CONTROL_GUARANTEE bit in the DMAR table. <b>1=Enable/Set</b> and 0=Disable/Clear
  BOOLEAN               InterruptRemappingSupport;         ///< This field is used to indicate Interrupt Remapping supported or not
  UINT32                DmaBufferBase;                     ///< Iommu PEI DMA buffer base in low memory region, in Mbytes units
} VTD_DATA_HOB;

#pragma pack (pop)
#endif
