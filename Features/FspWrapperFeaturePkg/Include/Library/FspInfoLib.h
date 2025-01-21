/** @file
  Header file for FSP Information Library.

  Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
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

@par Specification
**/

#ifndef _FSP_INFO_LIB_H_
#define _FSP_INFO_LIB_H_

extern EFI_GUID gFspInfoGuid;

#include <SiFvi.h>

#define BUILD_TIME_STAMP_SIZE 12

#pragma pack (push, 1)

typedef struct {
UINT8            VersionMajor;
UINT8            VersionMinor;
UINT16           VersionRevision;
UINT16           VersionBuild;
UINT8            TimeStamp [BUILD_TIME_STAMP_SIZE];
} FSP_INFO_HOB;

#pragma pack (pop)

VOID
EFIAPI
DumpFspInfo (
  IN FSP_INFO_HOB *FspInfo
  );

EFI_STATUS
EFIAPI
BuildFspInfo (
  VOID
  );
#endif // _FSP_INFO_LIB_H_
