/** @file
  Library to provide the generic function to operate on FIT table.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Register/Cpuid.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
//
// EDK-II Platform Foundation.
//
#include <IndustryStandard/FirmwareInterfaceTable.h>
//
// Advanced Feature Package Foundation.
//
#include <Library/FitHelperLib.h>
//
// Module Self Foundation.
//
#include "FitTableInternalLib.h"
#include "FitEntryInternalLib.h"

/**
  Check if FIT entry is resident in firmware address range.

  Note:
    (1) Input of address should be converted into memory range.
    (2) FIT table is in the range FIT_TABLE_LOWER_ADDRESS
        to FIT_TABLE_UPPER_ADDRESS.

  @param[in]  FitEntryAddress  FIT entry address mapping to memory.

  @retval  TRUE   FIT entry is resident in valid range.
  @retval  FALSE  FIT entry is not resident in valid range.

**/
BOOLEAN
EFIAPI
IsFitEntryAddressValid (
  IN EFI_PHYSICAL_ADDRESS  FitEntryAddress
  )
{
  EFI_PHYSICAL_ADDRESS  StartAddress;
  EFI_PHYSICAL_ADDRESS  EndAddress;

  StartAddress = FitEntryAddress;
  EndAddress   = StartAddress + sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY) - 1;

  if (StartAddress < FIT_TABLE_LOWER_ADDRESS) {
    DEBUG ((DEBUG_ERROR, "Start offset of FIT entry is above 4G - 16MB - [0x%X]\n", StartAddress));
    return FALSE;
  }

  if (EndAddress > FIT_TABLE_UPPER_ADDRESS) {
    DEBUG ((DEBUG_ERROR, "End offset of FIT entry is below 4G - 40h - [0x%X]\n", EndAddress));
    return FALSE;
  }

  return TRUE;
}

/**
  Check the FIT table is valid.

  @param[in]  FitPtr        Pointer to FIT table.
  @param[in]  FitTableSize  Size of the FIT table.

  @retval  TRUE   FIT table is valid.
  @retval  FALSE  FIT table is not valid.

**/
BOOLEAN
EFIAPI
IsFitTableValid (
  IN CONST VOID   *FitPtr,
  IN       UINTN  FitTableSize
  )
{
  EFI_STATUS  Status;
  UINT32      FitEntryNum;

  if ((FitPtr == NULL) || (FitTableSize == 0)) {
    return FALSE;
  }

  if ((FitTableSize % sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY)) != 0) {
    return FALSE;
  }

  Status = GetEntryNumOfFit (FitPtr, &FitEntryNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the number of FIT entries.\n"));
    return FALSE;
  }

  if ((FitEntryNum * sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY)) != FitTableSize) {
    DEBUG ((DEBUG_ERROR, "Check FIT table size failed.\n"));
    return FALSE;
  }

  if (!IsCheckSumValid (FitPtr, FitTableSize, FitEntryNum)) {
    DEBUG ((DEBUG_ERROR, "Check FIT table checksum failed.\n"));
    return FALSE;
  }

  if (!IsAllFitEntryTypeValid (FitPtr, FitTableSize, FitEntryNum)) {
    DEBUG ((DEBUG_ERROR, "Check FIT entry type failed.\n"));
    return FALSE;
  }

  if (!IsAllFitEntryOrderValid (FitPtr, FitTableSize, FitEntryNum)) {
    DEBUG ((DEBUG_ERROR, "Check FIT entry order failed.\n"));
    return FALSE;
  }

  if (!IsAllFitEntryTypeNumValid (FitPtr, FitTableSize, FitEntryNum)) {
    DEBUG ((DEBUG_ERROR, "Check number of FIT entry type instances failed.\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Get the FIT pointer address from input BIOS buffer.

  @param[in]   BiosBufferPtr   Pointer to BIOS image buffer in memory.
  @param[in]   BiosBufferSize  Size of BIOS image buffer.
  @param[out]  FitPtr          Double pointer to point to FIT table.

  @retval  EFI_SUCCESS            Succeed to get the FIT pointer from
                                  input BIOS buffer.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetFitPointer (
  IN     CONST UINT8  *BiosBufferPtr,
  IN           UINTN  BiosBufferSize,
     OUT       VOID   **FitPtr
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  FitBaseAddress;
  EFI_PHYSICAL_ADDRESS  FitLastEntryAddress;
  EFI_PHYSICAL_ADDRESS  BiosAreaBaseAddress;
  UINT64                FitTableOffset;
  VOID                  *BiosBufferFitPtr;
  UINT32                FitEntryNum;
  UINTN                 FitTableSize;

  if ((BiosBufferPtr == NULL) || (BiosBufferSize == 0) || (FitPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (BiosBufferSize < FIT_POINTER_OFFSET) {
    return EFI_INVALID_PARAMETER;
  }

  *FitPtr = NULL;

  //
  // Check the FIT table resident in the input BIOS buffer.
  //
  // [BIOS Mapping MMIO]                            [BIOS in Buffer]
  //                  4G +-----------------+ BiosBufferPtr + BiosBufferSize
  //                     |   BIOS Buffer   |
  // FitLastEntryAddress +-----------------+ BiosBufferFitPtr + FitTableSize
  //                     |    FIT TABLE    |
  //      FitBaseAddress +------------------ BiosBufferFitPtr
  //                     |                 |
  //                     |   BIOS Buffer   |
  //                     |                 |
  //                     +-----------------+ BiosBufferPtr

  //
  // Ensure the first entry of FIT table in valid region.
  //
  FitBaseAddress = *((UINTN *)(BiosBufferPtr + (BiosBufferSize - FIT_POINTER_OFFSET)));
  if (!IsFitEntryAddressValid (FitBaseAddress)) {
    DEBUG ((DEBUG_ERROR, "Invalid of FIT pointer address in memory [0x%X]\n", FitBaseAddress));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the FIT table pointer inside the input buffer.
  //
#if FixedPcdGetBool (PcdBiosExtenedRegionEnable) == 0
  BiosAreaBaseAddress = (EFI_PHYSICAL_ADDRESS)PcdGet32 (PcdBiosAreaBaseAddress);
#else
  BiosAreaBaseAddress = (EFI_PHYSICAL_ADDRESS)((UINTN)PcdGet32 (PcdBiosAreaBaseAddress) + (UINTN)PcdGet32 (PcdFlashExtendOffset));
#endif
  FitTableOffset      = FitBaseAddress - BiosAreaBaseAddress;
  BiosBufferFitPtr    = (VOID *)(BiosBufferPtr + FitTableOffset);

  //
  // Get the FIT table size.
  //
  Status = GetEntryNumOfFit (BiosBufferFitPtr, &FitEntryNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the number of FIT entry.\n"));
    return Status;
  }

  FitTableSize = FitEntryNum * sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY);
  if ((EFI_PHYSICAL_ADDRESS)((UINT8 *)BiosBufferFitPtr + FitTableSize) > (EFI_PHYSICAL_ADDRESS)(BiosBufferPtr + BiosBufferSize)) {
    DEBUG ((DEBUG_ERROR, "FIT table is located out of the input BIOS buffer.\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Ensure the last entry of FIT table in valid region.
  //
  FitLastEntryAddress = FitBaseAddress + ((FitEntryNum - 1) * sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY));
  if (!IsFitEntryAddressValid (FitLastEntryAddress)) {
    DEBUG ((DEBUG_ERROR, "Invalid of last entry of FIT table address in memory [0x%X]\n", FitLastEntryAddress));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if FIT table is valid.
  //
  if (!IsFitTableValid (BiosBufferFitPtr, FitTableSize)) {
    DEBUG ((DEBUG_ERROR, "Invalid of FIT table.\n"));
    return EFI_INVALID_PARAMETER;
  }

  *FitPtr = BiosBufferFitPtr;

  return EFI_SUCCESS;
}

/**
  Get the FIT entries buffer by type.

  As per the FIT specification, the order of the records should be in
  ascending order of their type attribute in FIT, i.e., the entries
  with same type are continuous.

  @param[in]       BiosBufferPtr   Pointer to BIOS image buffer in
                                   memory.
  @param[in]       BiosBufferSize  Size of BIOS image buffer.
  @param[in]       FitEntryType    Type of FIT entry.
  @param[in, out]  DataSizePtr     Pointer to size of buffer to place.
  @param[out]      DataBufferPtr   Double pointer to the specific
                                   address inside the BiosBufferPtr.

  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_BUFFER_TOO_SMALL   Caller provided the NULL pointer
                                  to store the address of first entry.
  @retval  EFI_NOT_FOUND          Assigned type of entry is not found.
  @retval  EFI_SUCCESS            Succeed to get the entries buffer by
                                  assigned type of entry.

**/
EFI_STATUS
EFIAPI
GetFitEntryBufferByType (
  IN     CONST UINT8  *BiosBufferPtr,
  IN           UINTN  BiosBufferSize,
  IN           UINT8  FitEntryType,
  IN OUT       UINTN  *DataSizePtr,
     OUT       VOID   **DataBufferPtr  OPTIONAL
  )
{
  EFI_STATUS                      Status;
  VOID                            *FitPtr;
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryPtr;
  UINT32                          Index;
  UINT32                          FitEntryNum;
  UINT32                          FoundEntryNum;
  UINTN                           FoundEntryDataSize;

  FitPtr = NULL;

  //
  // Check the input parameters are valid.
  //
  if ((BiosBufferPtr == NULL) || (BiosBufferSize == 0) || (DataSizePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsFitEntryTypeValid (FitEntryType)) {
    DEBUG ((DEBUG_ERROR, "Invalid type of FIT entry - [0x%X]\n", FitEntryType));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the FIT pointer from input buffer.
  //
  Status = GetFitPointer (BiosBufferPtr, BiosBufferSize, &FitPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the FIT pointer from input buffer.\n"));
    return Status;
  }

  //
  // Get the data size with requested type of FIT entry.
  //
  Status = GetEntryNumByType (FitPtr, FitEntryType, &FoundEntryNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the type %X entry in FIT table - %r.\n", FitEntryType, Status));
    return Status;
  }

  FoundEntryDataSize = FoundEntryNum * sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY);
  *DataSizePtr       = FoundEntryDataSize;
  if (FoundEntryDataSize == 0) {
    DEBUG ((DEBUG_WARN, "Type %X entry is not in FIT table - %r.\n", FitEntryType, Status));
    return EFI_NOT_FOUND;
  }

  //
  // Check if needs to pass out the address of first entry.
  //
  if (DataBufferPtr == NULL) {
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Find the pointer of requesting type of FIT entry.
  //
  Status = GetEntryNumOfFit (FitPtr, &FitEntryNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the total number of FIT entry\n"));
    return EFI_NOT_FOUND;
  }

  FitEntryPtr = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FitPtr;
  for (Index = 0; Index < FitEntryNum; Index++) {
    if (FitEntryPtr[Index].Type != FitEntryType) {
      continue;
    }

    *DataBufferPtr = (VOID *)&FitEntryPtr[Index];
    break;
  }

  return Status;
}

/**
  Get the processor FMS and FMS mask from type 2 Ver. 0x200 FIT entry.

  @param[in]   FitEntryPtr          Pointer for specific FIT entry.
  @param[out]  ProcessorFmsPtr      Pointer for processor FMS value.
  @param[out]  ProcessorFmsMaskPtr  Pointer for processor FMS mask value.

  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_SUCCESS            Succeed to get the both of FMS and
                                  FMS mask value.

**/
EFI_STATUS
EFIAPI
GetFitType2EntryVer200Info (
  IN     CONST FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryPtr,
     OUT       CPUID_VERSION_INFO_EAX          *ProcessorFmsPtr,
     OUT       CPUID_VERSION_INFO_EAX          *ProcessorFmsMaskPtr
  )
{
  FIT_TYPE_02_VERSION_200_ENTRY  *FitType02Ver200EntryPtr;
  CPUID_VERSION_INFO_EAX         FmsVal;
  CPUID_VERSION_INFO_EAX         FmsMaskVal;

  FmsVal.Uint32     = 0;
  FmsMaskVal.Uint32 = 0;

  if ((FitEntryPtr == NULL) || (ProcessorFmsPtr == NULL) || ProcessorFmsMaskPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((FitEntryPtr->Type != FIT_TYPE_02_STARTUP_ACM) ||
      (FitEntryPtr->Version != FIT_TYPE_02_VERSION_200))
  {
    return EFI_INVALID_PARAMETER;
  }

  FitType02Ver200EntryPtr = (FIT_TYPE_02_VERSION_200_ENTRY *)FitEntryPtr;

  //
  // Fill in the FMS information from FIT entry.
  //
  FmsVal.Bits.Model            = FitType02Ver200EntryPtr->Model;
  FmsVal.Bits.FamilyId         = FitType02Ver200EntryPtr->Family;
  FmsVal.Bits.ProcessorType    = FitType02Ver200EntryPtr->ProcessorType;
  FmsVal.Bits.ExtendedModelId  = FitType02Ver200EntryPtr->ExtModel;
  FmsVal.Bits.ExtendedFamilyId = FitType02Ver200EntryPtr->ExtFamily;

  //
  // Fill in the FMS mask information from FIT entry.
  //
  FmsMaskVal.Bits.Model            = FitType02Ver200EntryPtr->ModelMask;
  FmsMaskVal.Bits.FamilyId         = FitType02Ver200EntryPtr->FamilyMask;
  FmsMaskVal.Bits.ProcessorType    = FitType02Ver200EntryPtr->ProcessorTypeMask;
  FmsMaskVal.Bits.ExtendedModelId  = FitType02Ver200EntryPtr->ExtModelMask;
  FmsMaskVal.Bits.ExtendedFamilyId = FitType02Ver200EntryPtr->ExtFamilyMask;

  DEBUG ((DEBUG_INFO, "FMS Value     : 0x%X\n", FmsVal));
  DEBUG ((DEBUG_INFO, "FMS Mask Value: 0x%X\n", FmsMaskVal));

  CopyMem ((VOID *)ProcessorFmsPtr, (VOID *)&FmsVal, sizeof (CPUID_VERSION_INFO_EAX));
  CopyMem ((VOID *)ProcessorFmsMaskPtr, (VOID *)&FmsMaskVal, sizeof (CPUID_VERSION_INFO_EAX));

  return EFI_SUCCESS;
}
