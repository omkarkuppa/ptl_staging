/** @file
  Implementation of managing the UEFI Variable default data HOB in PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include "UefiVariableDefaultHob.h"

#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/UefiVariableDefaultHobLib.h>
#include <Library/UefiVariableHelperLib.h>
#include <Ppi/ReadOnlyVariable2.h>

/**
  Find variable from default variable HOB.

  @param[in]  VariableName      A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in]  VendorGuid        A unique identifier for the vendor.
  @param[out] AuthFlag          Pointer to output Authenticated variable flag.

  @return Pointer to variable header, NULL if not found.

**/
AUTHENTICATED_VARIABLE_HEADER *
EFIAPI
FindVariableFromHob (
  IN CHAR16                     *VariableName,
  IN EFI_GUID                   *VendorGuid,
  OUT BOOLEAN                   *AuthFlag
  )
{
  EFI_HOB_GUID_TYPE             *GuidHob;
  VARIABLE_STORE_HEADER         *VariableStoreHeader;
  AUTHENTICATED_VARIABLE_HEADER *StartPtr;
  AUTHENTICATED_VARIABLE_HEADER *EndPtr;
  AUTHENTICATED_VARIABLE_HEADER *CurrPtr;
  VOID                          *Point;

  VariableStoreHeader = NULL;

  GuidHob = GetFirstGuidHob (&gEfiAuthenticatedVariableGuid);
  if (GuidHob != NULL) {
    VariableStoreHeader = (VARIABLE_STORE_HEADER *) GET_GUID_HOB_DATA (GuidHob);
    *AuthFlag = TRUE;
  } else {
    GuidHob = GetFirstGuidHob (&gEfiVariableGuid);
    if (GuidHob != NULL) {
      VariableStoreHeader = (VARIABLE_STORE_HEADER *) GET_GUID_HOB_DATA (GuidHob);
      *AuthFlag = FALSE;
    }
  }
  ASSERT (VariableStoreHeader != NULL);
  if (VariableStoreHeader == NULL) {
    return NULL;
  }

  StartPtr = GetStartPointer (VariableStoreHeader);
  EndPtr   = GetEndPointer (VariableStoreHeader);
  for ( CurrPtr = StartPtr
      ; (CurrPtr < EndPtr) && IsValidVariableHeader (CurrPtr)
      ; CurrPtr = GetNextVariablePtr (CurrPtr, *AuthFlag)
      ) {
    if (CurrPtr->State == VAR_ADDED) {
      if (CompareGuid (VendorGuid, GetVendorGuidPtr (CurrPtr, *AuthFlag))) {
        Point = (VOID *) GetVariableNamePtr (CurrPtr, *AuthFlag);

        ASSERT (NameSizeOfVariable (CurrPtr, *AuthFlag) != 0);
        if (CompareMem (VariableName, Point, NameSizeOfVariable (CurrPtr, *AuthFlag)) == 0) {
          return CurrPtr;
        }
      }
    }
  }

  return NULL;
}

/**
  Get variable from default variable HOB.

  @param[in]      VariableName  A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in]      VendorGuid    A unique identifier for the vendor.
  @param[out]     Attributes    If not NULL, a pointer to the memory location to return the
                                attributes bitmask for the variable.
  @param[in, out] DataSize      On input, the size in bytes of the return Data buffer.
                                On output the size of data returned in Data.
  @param[out]     Data          The buffer to return the contents of the variable.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_NOT_FOUND         The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL  The DataSize is too small for the result.
  @retval EFI_INVALID_PARAMETER VariableName is NULL.
  @retval EFI_INVALID_PARAMETER VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER DataSize is NULL.
  @retval EFI_INVALID_PARAMETER The DataSize is not too small and Data is NULL.

**/
EFI_STATUS
EFIAPI
GetVariableFromHob (
  IN     CHAR16                 *VariableName,
  IN     EFI_GUID               *VendorGuid,
  OUT    UINT32                 *Attributes, OPTIONAL
  IN OUT UINTN                  *DataSize,
  OUT    VOID                   *Data
  )
{
  BOOLEAN                       AuthFlag;
  AUTHENTICATED_VARIABLE_HEADER *Variable;
  UINTN                         VarDataSize;

  if ((VariableName == NULL) || (VendorGuid == NULL) || (DataSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Variable = FindVariableFromHob (VariableName, VendorGuid, &AuthFlag);
  if (Variable == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Get data size
  //
  VarDataSize = DataSizeOfVariable (Variable, AuthFlag);
  if (*DataSize >= VarDataSize) {
    if (Data == NULL) {
      return EFI_INVALID_PARAMETER;
    }

    CopyMem (Data, GetVariableDataPtr (Variable, AuthFlag), VarDataSize);

    if (Attributes != NULL) {
      *Attributes = Variable->Attributes;
    }

    *DataSize = VarDataSize;
    return EFI_SUCCESS;
  } else {
    *DataSize = VarDataSize;
    return EFI_BUFFER_TOO_SMALL;
  }
}

/**
  Set variable to default variable HOB.

  @param[in] VariableName       A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in] VendorGuid         A unique identifier for the vendor.
  @param[in] Attributes         If not NULL, a pointer to the memory location to set the
                                attributes bitmask for the variable.
  @param[in] DataSize           On input, the size in bytes of the return Data buffer.
                                On output the size of data returned in Data.
  @param[in] Data               The buffer to return the contents of the variable.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_NOT_FOUND         The variable was not found.
  @retval EFI_INVALID_PARAMETER VariableName is NULL.
  @retval EFI_INVALID_PARAMETER VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER Attributes is not NULL, but attributes value is 0.
  @retval EFI_INVALID_PARAMETER DataSize is not equal to the variable data size.
  @retval EFI_INVALID_PARAMETER The DataSize is equal to the variable data size, but Data is NULL.

**/
EFI_STATUS
EFIAPI
SetVariableToHob (
  IN CHAR16                     *VariableName,
  IN EFI_GUID                   *VendorGuid,
  IN UINT32                     *Attributes, OPTIONAL
  IN UINTN                      DataSize,
  IN VOID                       *Data
  )
{
  BOOLEAN                       AuthFlag;
  AUTHENTICATED_VARIABLE_HEADER *Variable;
  UINTN                         VarDataSize;

  if (VariableName == NULL || VariableName[0] == 0 || VendorGuid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Variable = FindVariableFromHob (VariableName, VendorGuid, &AuthFlag);
  if (Variable == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Get data size
  //
  VarDataSize = DataSizeOfVariable (Variable, AuthFlag);
  if (DataSize == VarDataSize) {
    if (Data == NULL) {
      return EFI_INVALID_PARAMETER;
    }

    if (Attributes != NULL) {
      if (*Attributes == 0) {
        return EFI_INVALID_PARAMETER;
      }
      Variable->Attributes = *Attributes;
    }

    CopyMem (GetVariableDataPtr (Variable, AuthFlag), Data, VarDataSize);

    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
}

/**
  This function finds the matched default data and create GUID hob for it.

  @param[in] DefaultId         - Specifies the type of defaults to retrieve.

  @return EFI_SUCCESS          - The matched default data is found.
  @return EFI_NOT_FOUND        - The matched default data is not found.
  @return EFI_OUT_OF_RESOURCES - No enough resource to create HOB.

**/
EFI_STATUS
EFIAPI
CreateDefaultVariableHob (
  IN UINT16  DefaultId
  )
{
  UINTN                      FvInstance;
  EFI_FIRMWARE_VOLUME_HEADER *FvHeader;
  EFI_FFS_FILE_HEADER        *FfsHeader;
  UINT32                     FileSize;
  EFI_COMMON_SECTION_HEADER  *Section;
  UINT32                     SectionLength;
  BOOLEAN                    DefaultSettingIsFound;
  DEFAULT_DATA               *DefaultData;
  DEFAULT_INFO               *DefaultInfo;
  VARIABLE_STORE_HEADER      *VarStoreHeader;
  VARIABLE_STORE_HEADER      *VarStoreHeaderHob;
  UINT8                      *VarHobPtr;
  UINT8                      *VarPtr;
  UINT32                     VarDataOffset;
  UINT32                     VarHobDataOffset;
  BOOLEAN                    IsFirstSection;
  DATA_DELTA                 *DataDelta;
  UINTN                      DataDeltaSize;
  UINTN                      Index;
  CONST EFI_PEI_SERVICES     **PeiServices;

  //
  // Get PeiService pointer
  //
  PeiServices = GetPeiServicesTablePointer ();

  //
  // Find the FFS file that stores all default data.
  //
  DefaultSettingIsFound = FALSE;
  FvInstance            = 0;
  FfsHeader             = NULL;
  while (((*PeiServices)->FfsFindNextVolume (PeiServices, FvInstance, (VOID **) &FvHeader) == EFI_SUCCESS) &&
         (!DefaultSettingIsFound)) {
    FfsHeader = NULL;
    while ((*PeiServices)->FfsFindNextFile (PeiServices, EFI_FV_FILETYPE_FREEFORM, FvHeader, (VOID **) &FfsHeader) == EFI_SUCCESS) {
      if (CompareGuid ((EFI_GUID *) FfsHeader, &gDefaultDataOptSizeFileGuid)) {
        DefaultSettingIsFound = TRUE;
        break;
      }
    }
    FvInstance++;
  }

  //
  // FFS file is not found.
  //
  if (!DefaultSettingIsFound) {
    return EFI_NOT_FOUND;
  }

  //
  // Find the matched default data for the input default ID and plat ID.
  //
  DefaultSettingIsFound = FALSE;
  VarStoreHeaderHob     = NULL;
  VarHobPtr             = NULL;
  DataDelta             = NULL;
  DataDeltaSize         = 0;
  IsFirstSection        = TRUE;
  VarStoreHeader        = NULL;
  Section               = (EFI_COMMON_SECTION_HEADER *)(FfsHeader + 1);
  FileSize              = FFS_FILE_SIZE (FfsHeader);

  while (((UINTN) Section < (UINTN) FfsHeader + FileSize) && !DefaultSettingIsFound) {
    DefaultData   = (DEFAULT_DATA *) (Section + 1);
    DefaultInfo   = &(DefaultData->DefaultInfo[0]);
    SectionLength = SECTION_SIZE (Section);

    if (IsFirstSection) {
      //
      // Create HOB to store default data so that Variable driver can use it.
      // Allocate more data for header alignment.
      //
      VarStoreHeader = (VARIABLE_STORE_HEADER *) ((UINT8 *) DefaultData + DefaultData->HeaderSize);
      VarStoreHeaderHob = (VARIABLE_STORE_HEADER *) BuildGuidHob (&VarStoreHeader->Signature, VarStoreHeader->Size + HEADER_ALIGNMENT - 1);
      if (VarStoreHeaderHob == NULL) {
        //
        // No enough hob resource.
        //
        return EFI_OUT_OF_RESOURCES;
      }

      //
      // Copy variable storage header.
      //
      CopyMem (VarStoreHeaderHob, VarStoreHeader, sizeof (VARIABLE_STORE_HEADER));
      //
      // Copy variable data.
      //
      VarPtr           = (UINT8 *) HEADER_ALIGN ((UINTN) (VarStoreHeader + 1));
      VarDataOffset    = (UINT32) ((UINTN) VarPtr - (UINTN) VarStoreHeader);
      VarHobPtr        = (UINT8 *) HEADER_ALIGN ((UINTN) (VarStoreHeaderHob + 1));
      VarHobDataOffset = (UINT32) ((UINTN) VarHobPtr - (UINTN) VarStoreHeaderHob);
      CopyMem (VarHobPtr, VarPtr, VarStoreHeader->Size - VarDataOffset);
      //
      // Update variable size.
      //
      VarStoreHeaderHob->Size = VarStoreHeader->Size - VarDataOffset + VarHobDataOffset;

      //
      // Update Delta Data
      //
      VarHobPtr = (UINT8 *) VarStoreHeaderHob - VarDataOffset + VarHobDataOffset;
    } else {
      //
      // Apply delta setting
      //
      DataDelta     = (DATA_DELTA *) ((UINT8 *) DefaultData + DefaultData->HeaderSize);
      DataDeltaSize = SectionLength - sizeof (EFI_COMMON_SECTION_HEADER) - DefaultData->HeaderSize;
      for (Index = 0; Index < DataDeltaSize / sizeof (DATA_DELTA); Index++) {
        *((UINT8 *) VarHobPtr + DataDelta[Index].Offset) = DataDelta[Index].Value;
      }
    }

    //
    // Find the matched DefaultId
    //
    while ((UINTN) DefaultInfo < (UINTN) DefaultData + DefaultData->HeaderSize) {
      if (DefaultInfo->DefaultId == DefaultId) {
        DefaultSettingIsFound = TRUE;
        break;
      }
      DefaultInfo++;
    }
    //
    // Size is 24 bits wide so mask upper 8 bits.
    // SectionLength is adjusted it is 4 byte aligned.
    // Go to the next section
    //
    SectionLength = (SectionLength + 3) & (~3);
    ASSERT (SectionLength != 0);
    Section = (EFI_COMMON_SECTION_HEADER *)((UINT8 *)Section + SectionLength);
    IsFirstSection = FALSE;
  }
  //
  // Matched default data is not found.
  //
  if (!DefaultSettingIsFound) {
    //
    // Change created HOB type to be unused.
    //
    if (VarStoreHeaderHob != NULL) {
      ((EFI_HOB_GUID_TYPE *)((UINT8 *) VarStoreHeaderHob - sizeof (EFI_HOB_GUID_TYPE)))->Header.HobType = EFI_HOB_TYPE_UNUSED;
    }
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}
