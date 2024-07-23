/** @file
  The GUID definition for HOBs for SCS

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

#ifndef _SCS_INFO_HOB_H_
#define _SCS_INFO_HOB_H_

#include <Library/PchLimits.h>

extern EFI_GUID  gScsInfoHobGuid;

typedef struct {
  UINT32  Supported :  1;  ///< TRUE if UFS is supported by current hardware configuration
  UINT32  Reserved  : 31;
} SCS_UFS_INFO;

typedef struct {
  UINT32  Supported :  1; ///< TRUE if SdCard is supported by current hardware configuration
  UINT32  Reserved  : 31;
} SCS_SDCARD_INFO;

typedef struct {
  UINT32  Supported :  1; ///< TRUE if eMMC is supported by current hardware configuration
  UINT32  Reserved  : 31;
} SCS_EMMC_INFO;

typedef struct {
  SCS_EMMC_INFO    EmmcInfo;
  SCS_SDCARD_INFO  SdCardInfo;
  SCS_UFS_INFO     UfsInfo[PCH_MAX_UFS_DEV_NUM];
} SCS_INFO_HOB;

#endif
