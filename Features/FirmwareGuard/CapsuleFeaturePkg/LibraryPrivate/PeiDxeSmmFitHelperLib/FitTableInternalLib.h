/** @file
  Internal library to provide a series of APIs for FIT table.

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

#ifndef _FIT_INTERNAL_LIB_H_
#define _FIT_INTERNAL_LIB_H_

//
// The processor locates the FIT by following the FIT Pointer,
// which is located at the fixed address 4 GB - 40h in memory.
//
#define FIT_POINTER_OFFSET  0x40

//
// Based on the FIT specification, the first entry number must be a type 0.
//
#define FIT_TYPE_00_ENTRY_NUMBER  0

/**
  Check the FIT table value of checksum is valid.

  @param[in]  FitPtr        Pointer to FIT table.
  @param[in]  FitTableSize  Size of the FIT table.
  @param[in]  FitEntryNum   Number of entries within FIT table.

  @retval  TRUE   FIT table checksum is valid.
  @retval  FALSE  FIT table checksum is not valid.

**/
BOOLEAN
IsCheckSumValid (
  IN CONST VOID    *FitPtr,
  IN       UINTN   FitTableSize,
  IN       UINT32  FitEntryNum
  );

/**
  Check the order of all the FIT entries is valid.

  @param[in]  FitPtr        Pointer to FIT table.
  @param[in]  FitTableSize  Size of the FIT table.
  @param[in]  FitEntryNum   Number of entries within FIT table.

  @retval  TRUE   Order of FIT entries is valid.
  @retval  FALSE  Order of FIT entries is not valid.

**/
BOOLEAN
IsAllFitEntryOrderValid (
  IN CONST VOID    *FitPtr,
  IN       UINTN   FitTableSize,
  IN       UINT32  FitEntryNum
  );

/**
  Check the type of all the FIT entries is valid.

  @param[in]  FitPtr        Pointer to FIT table.
  @param[in]  FitTableSize  Size of the FIT table.
  @param[in]  FitEntryNum   Number of entries within FIT table.

  @retval  TRUE   Type of FIT entries is valid.
  @retval  FALSE  Type of FIT entries is not valid.

**/
BOOLEAN
IsAllFitEntryTypeValid (
  IN CONST VOID    *FitPtr,
  IN       UINTN   FitTableSize,
  IN       UINT32  FitEntryNum
  );

/**
  Check the number of instances for each type within FIT table is valid.

  @param[in]  FitPtr        Pointer to FIT table.
  @param[in]  FitTableSize  Size of the FIT table.
  @param[in]  FitEntryNum   Number of entries within FIT table.

  @retval  TRUE   Number of instances for each type is valid.
  @retval  FALSE  Number of instances for each type is not valid.

**/
BOOLEAN
IsAllFitEntryTypeNumValid (
  IN CONST VOID    *FitPtr,
  IN       UINTN   FitTableSize,
  IN       UINT32  FitEntryNum
  );

/**
  Get the FIT entry type 00.

  @param[in]   FitPtr       Pointer to FIT table.
  @param[out]  FitEntryPtr  Double pointer to FIT entry type 00.

  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_NOT_FOUND          Not found the FIT entry type 00.
  @retval  EFI_SUCCESS            Succeed to get the FIT entry type 00.

**/
EFI_STATUS
GetFitEntryType00 (
  IN     CONST VOID                            *FitPtr,
     OUT       FIRMWARE_INTERFACE_TABLE_ENTRY  **FitEntryPtr
  );

/**
  Get the total number of entries in FIT table.

  @param[in]   FitPtr          Pointer to FIT table.
  @param[out]  FitEntryNumPtr  Pointer to number of entries in FIT table.

  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_SUCCESS            Succeed to get the number of entries.

**/
EFI_STATUS
GetEntryNumOfFit (
  IN     CONST VOID    *FitPtr,
     OUT       UINT32  *FitEntryNumPtr
  );

/**
  Get the number of entries by type in FIT table.

  @param[in]   FitPtr          Pointer to FIT table.
  @param[in]   FitEntryType    Type of FIT entry.
  @param[out]  FitEntryNumPtr  Pointer to number of entries in FIT table.

  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_SUCCESS            Succeed to get the number of entries
                                  by type.

**/
EFI_STATUS
GetEntryNumByType (
  IN     CONST VOID    *FitPtr,
  IN           UINT8   FitEntryType,
     OUT       UINT32  *FitEntryNumPtr
  );

#endif
