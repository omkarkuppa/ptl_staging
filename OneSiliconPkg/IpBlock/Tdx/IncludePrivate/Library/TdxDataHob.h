/** @file
    This file contains definitions required for creation of TDX Data HOB.

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

#ifndef _TDX_DATA_HOB_H_
#define _TDX_DATA_HOB_H_

#include <Base.h>
#include <Library/PeiTdxLib.h>

extern EFI_GUID gTdxDataHobGuid;

#define TDX_DATA_HOB_REVISION  1

#pragma pack(push, 1)
///
/// TDX Hob data.
///
typedef struct _TDX_DATA {
  EFI_HOB_GUID_TYPE            EfiHobGuidType;    /// EFI Hob Guid Type.
  /**
     This member specifies the revision of the TDX Data HOB. This field is used
     to indicate back wards compatible changes to the protocol. Platform code that
     consumes this protocol must read the correct revision value to correctly interpret
     the content of the protocol fields.
  **/
  UINT8                       Revision;
  UINT32                      TdxEnabled;        /// 0: Not Enabled, 1: Enabled
  ACTM_DIMM_MANIFEST          ActmDimmManifest;
  MCHECK_PARAM_INFO_SEAM_DATA CmrList;
  UINT32                      SeamrrSize;
  UINT64                      SeamRrBaseAddress;
} TDX_DATA_HOB;

#pragma pack(pop)

/**
   Create TDX DATA HOB

   @param[out] TdxDataHobPtr  - Pointer to the TDX Data HOB structure

   @retval  EFI_SUCCESS  TdxDataHobPtr has been initialized successfully.
   @retval  Others       HOB creation failed.
**/
EFI_STATUS
TdxHobInit (
  OUT TDX_DATA_HOB **TdxDataHobPtr
  );

#endif // _TDX_DATA_HOB_H_
