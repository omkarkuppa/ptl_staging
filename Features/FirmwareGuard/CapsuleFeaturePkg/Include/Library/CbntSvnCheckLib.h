/**@file
  Compare S-ACM/BPM/KM from image with current SVN in fuse.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef __CBNT_SVN_CHECK_LIB__
#define __CBNT_SVN_CHECK_LIB__

/**
  Check if S-ACM SVN from input S-ACM image buffer is greater than or equal to ARBH SVN

  @param[in] SAcmImage                Pointer to S-ACM image buffer.
  @param[in] SAcmImageSize            The size of SAcmImageSize

  @retval EFI_SUCCESS                 S-ACM SVN from SAcmImage is greater than or equal to ARBH SVN
  @retval EFI_INCOMPATIBLE_VERSION    S-ACM SVN from SAcmImage is smaller than ARBH SVN
  @retval Others                      Fail to check S-ACM SVN from SAcmImage

**/
EFI_STATUS
EFIAPI
CheckSvnFromSAcmImage (
  IN  UINT8         *SAcmImage,
  IN  UINTN         SAcmImageSize
  );

/**
  Check if S-ACM/BPM/KM SVN from input BIOS image buffer is greater than or equal to ARBH SVN

  @param[in] BiosImage                Pointer to BIOS image buffer.
  @param[in] BiosImageSize            The size of BiosImage

  @retval EFI_SUCCESS                 SVN from BiosImage is greater than or equal to ARBH SVN
  @retval EFI_INCOMPATIBLE_VERSION    SVN from BiosImage is smaller than ARBH SVN
  @retval Others                      Fail to check SVN from BiosImage

**/
EFI_STATUS
EFIAPI
CheckSvnFromBiosImage (
  IN  UINT8         *BiosImage,
  IN  UINTN         BiosImageSize
  );

#endif
