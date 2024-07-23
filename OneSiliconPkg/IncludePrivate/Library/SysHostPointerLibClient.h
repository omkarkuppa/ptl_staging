/** @file
  Interface header file for accessing pointers in a highly optimized manner

  This library provides a way of accessing pointers that is highly optimized for
  performance.  This is necessary because some silicon code has significant
  performance challenges using PPI, protocol, and other standard services.

  Please note that this is not intended for general purpose use.  This should only
  be used when data flow design and standard services are shown to be insufficient.
  And shown with measured performance data.

  The API is based on specific Get/Set pointer functions.  This allows for flatter
  implementations.  This is consistent with EDK II library service API like
  PeiServicesLib.  Reducing parameters and other such instruction count reduction
  steps are necessary for maximum performance.

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

#ifndef _SYS_HOST_POINTER_LIB_CLIENT_H_
#define _SYS_HOST_POINTER_LIB_CLIENT_H_

#include <Uefi/UefiBaseType.h>

//
// If this struct is the first member of a SysHostPointerLibClient pool and the size is non-zero,
// this StaticPointerTableLib Pool will be copied into SMM for use by SMM drivers.  If this
// struct does not exist in a given SPT Pool, then the PEI/DXE pointer is used.  Keep in
// mind that the PEI/DXE memory cannot be accessed from SMM after SMM Lock.
//
// Note: The Guid member must be equal to gStaticPointerSmmSizeGuid to enable SMM copy
//
typedef struct _STATIC_POINTER_SMM_SIZE_STRUCT {
  EFI_GUID        Guid;
  UINT16          Size;
} STATIC_POINTER_SMM_SIZE_STRUCT;

/**
  Gets the SysInfoVar pointer.

  This function abstracts the retrieval of the SysInfoVar pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @return   Pointer to SysInfoVar
**/
VOID *
EFIAPI
GetSysInfoVarPointer (
  VOID
  );

/**
  Sets the SysInfoVar pointer.

  This function abstracts the setting of the SysInfoVar pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  SysInfoVar              Pointer to be stored

**/
VOID
EFIAPI
SetSysInfoVarPointer (
  IN VOID *SysInfoVar
  );

/**
  Migrate Static Table Pointer

  Fix table pointer according to the HOB address which have been placed in permanent memory
  Fix the Entries address according to the new addrees which have been placed in permanent memory

  It is caller's responsibility to ensure that HOB already exists.
**/
VOID
MigrateMemoryStaticPointers (
  VOID
  );
#endif  // _SYS_HOST_POINTER_LIB_CLIENT_H_
