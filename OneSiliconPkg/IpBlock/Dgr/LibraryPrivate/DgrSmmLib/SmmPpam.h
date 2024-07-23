/** @file
  SMM PPAM support

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

#ifndef _SMM_PPAM_H_
#define _SMM_PPAM_H_

#include <Protocol/SmMonitorInit.h>
#include "PpamApi.h"

/**

  Create 4G page table for PPAM.
  2M PAE page table in X64 version.

  @param PageTableBase        The page table base in MSEG

**/
VOID
PpamGen4GPageTable (
  IN UINTN              PageTableBase
  );

/**

Initializes MsegBase and MsegSize variables

**/
VOID
GetMsegInfo(
  VOID
  );

/**

  Get PPAM state.

  @return PPAM state

**/
EFI_SM_MONITOR_STATE
EFIAPI
PpamGetMonitorState (
  VOID
  );

/**

  Load PPAM image to MSEG.

  @param PpamImage      PPAM image
  @param PpamImageSize  PPAM image size

  @retval EFI_SUCCESS            Load PPAM to MSEG successfully
  @retval EFI_BUFFER_TOO_SMALL   MSEG is smaller than minimal requirement of PPAM image

**/
EFI_STATUS
EFIAPI
PpamLoadMonitor (
  IN EFI_PHYSICAL_ADDRESS PpamImage,
  IN UINTN                PpamImageSize
  );

/**

  Add resources in list to database. Allocate new memory areas as needed.

  @param ResourceList  A pointer to resource list to be added
  @param NumEntries    Optional number of entries.
                       If 0, list must be terminated by END_OF_RESOURCES.

  @retval EFI_SUCCESS            If resources are added
  @retval EFI_INVALID_PARAMETER  If nested procedure detected resource failer
  @retval EFI_OUT_OF_RESOURCES   If nested procedure returned it and we cannot allocate more areas.

**/
EFI_STATUS
EFIAPI
PpamAddPiResource (
  IN  STM_RSC  *ResourceList,
  IN  UINT32    NumEntries OPTIONAL
  );

/**

  Delete resources in list to database.

  @param ResourceList  A pointer to resource list to be deleted
                       NULL means delete all resources.
  @param NumEntries    Optional number of entries.
                       If 0, list must be terminated by END_OF_RESOURCES.

  @retval EFI_SUCCESS            If resources are deleted
  @retval EFI_INVALID_PARAMETER  If nested procedure detected resource failer

**/
EFI_STATUS
EFIAPI
PpamDeletePiResource (
  IN  STM_RSC    *ResourceList,
  IN  UINT32      NumEntries OPTIONAL
  );

/**

  Get BIOS resources.

  @param ResourceList  A pointer to resource list to be filled
  @param ResourceSize  On input it means size of resource list input.
                       On output it means size of resource list filled,
                       or the size of resource list to be filled if size of too small.

  @retval EFI_SUCCESS            If resources are returned.
  @retval EFI_BUFFER_TOO_SMALL   If resource list buffer is too small to hold the whole resources.

**/
EFI_STATUS
EFIAPI
PpamGetPiResource (
  OUT    STM_RSC *ResourceList,
  IN OUT UINT32  *ResourceSize
  );

/**
Internal worker function that is called to complete CPU initialization at the
end of SmmCpuFeaturesInitializeProcessor().

**/
VOID
FinishSmmCpuFeaturesInitializeProcessor(
  VOID
  );

/**
  Copies the data into SMM Entry Info Table

  @param[in] SmmEntryPointHeader  Points to the SMM Entry Point Header.
  @param[in] Type                 SMM Entry point Information type.
  @param[in] Data                 Points to Data to be copied.
  @param[in] DataSize             Size of Data.
**/
VOID
EFIAPI
PatchSmmEntryPoint (
  IN VOID      *SmmEntryPointHeader,
  IN UINT8     Type,
  IN VOID      *Data,
  IN UINT8     DataSize
  );

#endif
