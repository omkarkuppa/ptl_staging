/** @file
  Library to provide the generic function to operate on ACM image.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef _ACM_HELPER_LIB_H_
#define _ACM_HELPER_LIB_H_

//
// EDK-II Foundation.
//
#include <Uefi.h>


/**
  Check if the input ACM buffer is supported on current CPU.

  @param[in]  Image      Pointer that point to ACM image buffer.
  @param[in]  ImageSize  Size of ACM image buffer.

  @retval  TRUE   Input ACM buffer is supported on current CPU.
  @retval  FALSE  Input ACM buffer is not supported on current CPU.

**/
BOOLEAN
EFIAPI
IsSAcmImageForCurrentCpu (
  IN CONST VOID   *Image,
  IN       UINTN  ImageSize
  );

/**
  Get Min FBM SVN from input ACM buffer

  @param[in]  Image      A pointer to ACM image buffer.
  @param[in]  ImageSize  Size of ACM image buffer.

  @retval     MIN FBM SVN value in ACM. If failed to find MinFbmSvn, return 0xFF

**/
UINT8
EFIAPI
GetFbmMinSvnFromAcm (
  IN CONST VOID   *Image,
  IN       UINTN  ImageSize
  );

#endif
