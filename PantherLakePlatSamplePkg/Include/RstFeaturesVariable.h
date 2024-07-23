/** @file
  RST Features variable definition

#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
**/

#ifndef __RST_FEATURES_VARIABLE_H_
#define __RST_FEATURES_VARIABLE_H_

#define EFI_RST_FEATURE_VARIABLE_REVISION_ID 1
#define EFI_RST_FEATURE_VARIABLE_NAME        L"IntelRstFeatures"

extern EFI_GUID gRstFeaturesVariableGuid;

/**
  This variable is needed for VMD to know what Rst features are enabled on platform.
  Feature enable values are set up in SA SETUP
**/
typedef struct {
  UINT32  RevisionId;
  UINT32  Raid0        :  1;
  UINT32  Raid1        :  1;
  UINT32  Raid5        :  1;
  UINT32  Raid10       :  1;
  UINT32  RsvdBits0    : 28;
} EFI_RST_FEATURE_DATA;

#endif
