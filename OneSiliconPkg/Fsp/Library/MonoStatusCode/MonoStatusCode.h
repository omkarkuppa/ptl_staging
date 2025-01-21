/**@file

  Module Name:

    MonoStatusCode.h

  Abstract:

    Contains Platform specific implementations required to use status codes.
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

@par Specification
**/

#ifndef _MONO_STATUS_CODE_H_
#define _MONO_STATUS_CODE_H_

#include <PiPei.h>
#include <Ppi/StatusCode.h>

//
// Status code reporting function
//
EFI_STATUS
EFIAPI
SerialReportStatusCode (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN CONST EFI_GUID                 * CallerId,
  IN CONST EFI_STATUS_CODE_DATA     * Data OPTIONAL
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices - GC_TODO: add argument description
  CodeType    - GC_TODO: add argument description
  Value       - GC_TODO: add argument description
  Instance    - GC_TODO: add argument description
  CallerId    - GC_TODO: add argument description
  Data        - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

//
// This is the platform function that calls all of the listeners desired by the
// platform.
//
EFI_STATUS
EFIAPI
PlatformReportStatusCode (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN CONST EFI_GUID                 * CallerId,
  IN CONST EFI_STATUS_CODE_DATA     * Data OPTIONAL
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices - GC_TODO: add argument description
  CodeType    - GC_TODO: add argument description
  Value       - GC_TODO: add argument description
  Instance    - GC_TODO: add argument description
  CallerId    - GC_TODO: add argument description
  Data        - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

//
// Initialize the status code listeners and publish the status code PPI.
//

VOID
EFIAPI
InitializeMonoStatusCode (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN const EFI_PEI_SERVICES          **PeiServices
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  FfsHeader   - GC_TODO: add argument description
  PeiServices - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

extern EFI_PEI_PROGRESS_CODE_PPI    mStatusCodePpi;
extern EFI_PEI_PPI_DESCRIPTOR       mPpiListStatusCode;

#endif
