/** @file
  The PEI VMD Init Library in SA init

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

#include <VmdPeiConfig.h>
#include <Library/PeiHostBridgeIpStatusLib.h>

/**
  This function performs basic initialization for VMD in PEI phase (post-mem)
  @param VMD_PEI_PREMEM_CONFIG - Pointer to config data
  @retval     EFI_SUCCESS     Successfully initialized VMD
**/

EFI_STATUS
EFIAPI
VmdInit (
  IN   VMD_PEI_CONFIG  *VmdPeiConfig
  )
{
  BOOLEAN                    VmdDevenStatus;
  /// This library will get called while PCD "PcdVmdEnable" is disabled
  /// Disable the devEnable bit for VMD device (bit 14) if its enabled
  VmdDevenStatus = UncoreDevEnRead (Vmd, 0);
  if (VmdDevenStatus == TRUE) {
    UncoreDevEnWrite(Vmd, 0, Disable);
  }
  return EFI_SUCCESS;
}
