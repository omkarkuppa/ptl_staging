/** @file
  Header file to get CCE information.

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
#ifndef _CCE_INFO_LIB_H_
#define _CCE_INFO_LIB_H_

// RemapBase and RemapLimit register offset in CCE IP
#define R_SA_MCHBAR_CCE_REMAP_BASE_OFFSET         0x30

#define R_SA_MCHBAR_CCE_REMAP_LIMIT_OFFSET        0x38

// CCE0 register offset in MCHBAR
#define R_SA_MCHBAR_CCE0_OFFSET                   0x13000

// CCE1 register offset in MCHBAR
#define R_SA_MCHBAR_CCE1_OFFSET                   0x13400

///
/// Used to identify which CCE
///
typedef enum {
  CCE0  = 0,
  CCE1  = 1
} CCE_ID;

/**
  This function returns the specified CCE Remap Base value

  @param[in] CceId      - Id of CCE to retrieve value.

  @retval Remap Base
**/
UINT64
EFIAPI
GetCceRemapBase (
  IN CCE_ID CceId
  );

/**
  This function returns the specified CCE Remap Limit value

  @param[in] CceId      - Id of CCE to retrieve value.

  @retval Remap Limit
**/
UINT64
EFIAPI
GetCceRemapLimit (
  IN CCE_ID CceId
  );

#endif /* _CCE_INFO_LIB_H_ */
