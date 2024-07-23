/** @file
  FSP Information Library.

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

#include <PiPei.h>
#include <FspEas.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FspInfoLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/FspWrapperApiLib.h>
#include <FspProducerDataHeader.h>
#include <Guid/MigratedFvInfo.h>

/**
  Dump FSP Information.

  @param[in]  None

  @retval     None.

**/
VOID
EFIAPI
DumpFspInfo (
  IN FSP_INFO_HOB *FspInfo
  )
{

  DEBUG ((DEBUG_INFO, "=====================================\n"));

  if (FspInfo == NULL) {
    DEBUG ((DEBUG_WARN, "FSP Information pointer is NULL\n"));
    return;
  }
  DEBUG ((DEBUG_INFO,
          "FSP Version : %02x.%02x.%04x.%04x\n",
          FspInfo->VersionMajor,
          FspInfo->VersionMinor,
          FspInfo->VersionRevision,
          FspInfo->VersionBuild
        ));

  DEBUG ((DEBUG_INFO, "Build Date  : %.12a\n", FspInfo->TimeStamp));

  DEBUG ((DEBUG_INFO, "=====================================\n"));
}

/**
  Build FSP Information

  @retval EFI_SUCCESS    Create the FSP information successfully.

**/
EFI_STATUS
EFIAPI
BuildFspInfo (
  VOID
  )
{
  FSP_PRODUCER_DATA_TABLES      *FspmTable;
  FSP_INFO_HOB                  *FspInfo;
  VOID                          *Hob;
  EDKII_MIGRATED_FV_INFO        *MigratedFvInfo;

  //
  // Create FSP Information HOB
  //
  FspInfo = BuildGuidHob (&gFspInfoGuid, sizeof (FSP_INFO_HOB));
  ASSERT (FspInfo != NULL);
  FspmTable = NULL;

  Hob = GetFirstGuidHob (&gEdkiiMigratedFvInfoGuid);
  while (Hob != NULL) {
    MigratedFvInfo = GET_GUID_HOB_DATA (Hob);
    if (MigratedFvInfo->FvOrgBase == PcdGet32 (PcdFspmBaseAddress)) {
      DEBUG ((DEBUG_INFO, "gEdkiiMigratedFvInfoGuid HOB for FSP-M is found!\n"));
      FspmTable = (FSP_PRODUCER_DATA_TABLES *) FspFindFspHeader (MigratedFvInfo->FvNewBase);
      break;
    }
    Hob = GetNextGuidHob (&gEdkiiMigratedFvInfoGuid, GET_NEXT_HOB (Hob));
  }

  if (FspmTable == NULL) {
    FspmTable = (FSP_PRODUCER_DATA_TABLES *) FspFindFspHeader (PcdGet32 (PcdFspmBaseAddress));
  }
  ASSERT (FspmTable != NULL);

  if ((FspInfo != NULL) && (FspmTable != NULL)) {
    ZeroMem (FspInfo, sizeof (FSP_INFO_HOB));
    CopyMem (FspInfo->TimeStamp, FspmTable->FspProduceDataType1.BuildTimeStamp, BUILD_TIME_STAMP_SIZE);
    FspInfo->VersionMajor = ((FspmTable->FspInfoHeader.ImageRevision >> 24) & 0xFF);
    FspInfo->VersionMinor = ((FspmTable->FspInfoHeader.ImageRevision >> 16) & 0xFF);
    FspInfo->VersionRevision = ((FspmTable->FspInfoHeader.ExtendedImageRevision  & 0xFF00) |
                                  ((FspmTable->FspInfoHeader.ImageRevision >> 8) & 0x00FF));
    FspInfo->VersionBuild = (((FspmTable->FspInfoHeader.ExtendedImageRevision  & 0x00FF) << 8) |
                               (FspmTable->FspInfoHeader.ImageRevision & 0xFF));
  }

  DumpFspInfo(FspInfo);
  return EFI_SUCCESS;
}