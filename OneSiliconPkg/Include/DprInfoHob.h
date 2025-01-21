/** @file
  The GUID definition for DprInfoHob

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _DPR_INFO_HOB_H_
#define _DPR_INFO_HOB_H_

extern EFI_GUID gDprInfoHobGuid;

#pragma pack (push,1)
///
/// DPR Directory Types
///
typedef enum {
  EnumDprDirectoryTxt   = 0,
} DPR_DIRECTORY_ELEMENT;

#define DPR_DIRECTORY_TYPE_TXT          0x01      ///< DPR directory type - TXT
#define DPR_DIRECTORY_TYPE_BIOSGUARD    0x02      ///< DPR directory type - BIOS Guard
#define DPR_DIRECTORY_MAX               1         ///< DPR Maximum Size

///
/// DPR directory entry definition
///
typedef struct {
  UINT8   Type;          ///< DPR Directory Type
  UINT8   Size;          ///< DPR Size in MB
  UINT32  PhysBase;      ///< Must be 4K aligned (bits 11..0 must be clear)
  UINT16  Reserved;      ///< Must be 0
} DPR_DIRECTORY_ENTRY;

///
/// DPR Info Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                           ///< GUID Hob type structure for gDprInfoHobGuid
  DPR_DIRECTORY_ENTRY      DprDirectory[DPR_DIRECTORY_MAX];          ///< DPR directory entry definition
} DPR_INFO_HOB;

/**
  Init and Install DPR Info

  @param[out] DprInfoHob         - DPR_INFO_HOB instance installed by this function
  @retval EFI_SUCCESS
**/
EFI_STATUS
InstallDprInfoHob (
  OUT      DPR_INFO_HOB                **DprInfoHobOut
  );

/**
  Determine the Total DPR memory size needed based on the DPR directory in the SA Data HOB.

  @param[in, out] Size        - The memory size in MB to return.

  @retval Nothing.
**/
VOID
EFIAPI
CalculateTotalDprMemorySize (
  IN OUT   UINT8             *Size
  );

/**
  Calculates the bases for each technology consuming the DPR region
  and updates the SA Data HOB with the appropriate values in the Dpr
  directory

  @param[in] Base           - The memory base to return.
  @param[in] TotalDprSizeMB - The total DPR size in MB

  @retval Nothing.
**/
VOID
EFIAPI
UpdateDprHobInfo (
  IN       EFI_PHYSICAL_ADDRESS  Base,
  IN       UINT8                 TotalDprSizeMB
  );

#pragma pack (pop)
#endif
