/** @file
  Header file for VMD Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _VMD_INIT_LIB_H_
#define _VMD_INIT_LIB_H_

#include <VmdPeiConfig.h>

/**
  This function performs basic initialization for VMD in PEI phase (post-mem)
  @param[in]    VMD_PEI_PREMEM_CONFIG - Pointer to config data
  @retval     EFI_SUCCESS     Successfully initialized VMD
  @retval     EFI_UNSUPPORTED         VMD unsupported
  @retval     EFI_INVALID_PARAMETER   If VmdPeiConfig is null, return invalid parameter
**/
EFI_STATUS
EFIAPI
VmdInit (
IN   VMD_PEI_CONFIG  *VmdPeiConfig
);

#endif /* _VMD_INIT_LIB_H_ */
