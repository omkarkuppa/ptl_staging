/** @file
  Functional Safety policy.

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

#ifndef _FUSA_CONFIG_H_
#define _FUSA_CONFIG_H_

#define FUSA_CONFIG_REVISION 2
extern EFI_GUID gFusaConfigGuid;

#pragma pack (push,1)

/**
  The FUSA_CONFIG block describes the expected configuration of Functional Safety
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Config Block Header
  /**
    Enable/Disable Fusa feature on Display
    Fusa configuration on Display <b>0: Disable</b>; 1: Enable.
    Enable/Disable Fusa feature on IOP MCA Check
    Fusa configuration on IOP MCA Check <b>0: Disable</b>; 1: Enable.
  **/
  UINT32    DisplayFusaConfigEnable                :  1;    ///< Enable/Disable FuSa feature on DE: 0=Disable, 1=Enable
  UINT32    GraphicFusaConfigEnable                :  1;    ///< Enable/Disable FuSa feature on GT: 0=Disable, 1=Enable
  UINT32    OpioFusaConfigEnable                   :  1;    ///< Enable/Disable FuSa feature on OPIO: 0=Disable, 1=Enable
  UINT32    PsfFusaConfigEnable                    :  1;    ///< Enable/Disable FuSa feature on PSF: 0=Disable, 1=Enable
  UINT32    IopFusaConfigEnable                    :  1;    ///< Enable/Disable FuSa feature on IOP: 0=Disable, 1=Enable
  UINT32    IopFusaMcaCheckEnable                  :  1;    ///< Enable/Disable FuSa feature on IOP MCA Check Enable: 0=Disable, 1=Enable
  UINT32    RsvdBits0                              :  26;   ///< Reserved bits
} FUSA_CONFIG;

#pragma pack (pop)

#endif // _FUSA_CONFIG_H_
