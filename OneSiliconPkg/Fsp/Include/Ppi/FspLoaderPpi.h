/** @file
  This file contains FSP loader PPI definition. FSP loader PPI defines
  APIs for FSP-M/S verificaion. BSP can consume these APIs to verify FSP-M/S
  once they are loaded.

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

#ifndef __FSP_LOADER_PPI_H__
#define __FSP_LOADER_PPI_H__

//
// Global ID for the FSP_LOADER_PPI
//
#define FSP_LOADER_PPI_GUID \
  { \
    0x7d0ab2f9, 0x8c16, 0x464b, { 0x84, 0x94, 0xe5, 0xbf, 0x72, 0x1a, 0x78, 0x1c } \
  }

typedef struct _FSP_LOADER_PPI FSP_LOADER_PPI;

/**
  A service provided by FSP to verify FSP-S and published for BSP use in Dispatch mode.
  In API mode, it's not supported.

  @param[in] FspsImageBase        FSP-S image base.

  @retval EFI_UNSUPPORTED         FBM is not found or valid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
typedef
EFI_STATUS
(EFIAPI *FSP_VERIFY_FSPS) (
  IN UINTN    FspsImageBase
  );

struct _FSP_LOADER_PPI {
  FSP_VERIFY_FSPS                FspVerifyFsps;
};

extern EFI_GUID gFspLoaderPpiGuid;

#endif

