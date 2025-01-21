/** @file
  DXE SMM Non-Eviction Mode (NEM) Map Library

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

#include <Base.h>
#include <CpuRegs.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BootMediaLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/NemMapLib.h>
#include <Library/PcdLib.h>

#include <IndustryStandard/FirmwareInterfaceTable.h>

//
// The NEM_MAP_STRUCTURE created by S-ACM on block media boot
//
typedef struct {
  EFI_GUID StructureGuid; // GUID
  UINT16   Version;       // Major:Minor
  UINT16   Reserved;      // Alignment
  UINT32   NemBase;       // NEM Base address
  UINT32   NemSize;       // NEM Size
  UINT32   IbbBase;       // IBB Base in NEM
  UINT32   IbbSize;       // IBB Size in NEM
  UINT32   FitBase;       // FIT Base address
  UINT32   TempRamBase;   // PEI TempRamBase for stack data
  UINT32   TempRamSize;   // PEI TempRamSize
} NEM_MAP_STRUCTURE;

/**
  Returns a pointer to the NEM map structure which must exist on a block media boot.

  @param[out]   NemMapStructureAddress    A pointer to the NEM_MAP_STRUCTURE.

  @retval       EFI_SUCCESS               The NEM map structure was located and returned successfully.
  @retval       EFI_NOT_FOUND             The gNemMapStructureHobGuid was not found.
  @retval       EFI_DEVICE_ERROR          The GUID in the NEM map structure is invalid.
**/
EFI_STATUS
EFIAPI
GetNemMapStructure (
  OUT NEM_MAP_STRUCTURE     **NemMapStructure
  )
{
  EFI_STATUS                Status;
  EFI_HOB_GUID_TYPE         *GuidHobPtr;
  NEM_MAP_STRUCTURE         *NemMapStructurePtr;

  GuidHobPtr  = GetFirstGuidHob (&gNemMapStructureHobGuid);
  if (GuidHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "NemMapLib Error: NEM map structure HOB does not exist!\n"));
    return EFI_NOT_FOUND;
  }

  NemMapStructurePtr = (NEM_MAP_STRUCTURE *) GET_GUID_HOB_DATA (GuidHobPtr);

  if (!CompareGuid (&(NemMapStructurePtr->StructureGuid), &gNemMapStructureGuid)) {
    DEBUG ((DEBUG_ERROR, "NEM Map signature is invalid!\n"));
    return EFI_DEVICE_ERROR;
  }

  *NemMapStructure = NemMapStructurePtr;

  return EFI_SUCCESS;
}

/**
  Determines the Intel Boot Block (IBB) base address and size.

  @param[out] IbbBase         The IBB base address.
  @param[out] IbbSize         The IBB size.

  @retval       EFI_SUCCESS       The operation completed successfully.
  @retval       EFI_NOT_FOUND     The gNemMapStructureHobGuid was not found.
  @retval       EFI_DEVICE_ERROR  The GUID in the NEM map structure is invalid.
**/
EFI_STATUS
EFIAPI
GetIbbBaseAndSize (
  OUT UINTN    *IbbBase, OPTIONAL
  OUT UINT32   *IbbSize  OPTIONAL
  )
{
  EFI_STATUS        Status;
  NEM_MAP_STRUCTURE *NemStructurePtr = NULL;

  if (BootMediaIsSpi ()) {
    if (IbbBase != NULL) {
      *IbbBase = (UINTN) (FixedPcdGet32 (PcdBiosAreaBaseAddress) + FixedPcdGet32 (PcdNemCodeCacheBase));
    }
    if (IbbSize != NULL) {
      *IbbSize = FixedPcdGet32 (PcdFlashFvRecoverySize) +
                   FixedPcdGet32 (PcdFlashFvRecovery2Size);
    }
  } else {
    Status = GetNemMapStructure (&NemStructurePtr);
    if (EFI_ERROR (Status)) {
      return Status;
    } else if (NemStructurePtr == NULL) {
      return EFI_NOT_FOUND;
    }

    if (IbbBase != NULL) {
      *IbbBase = NemStructurePtr->IbbBase;
    }
    if (IbbSize != NULL) {
      *IbbSize = NemStructurePtr->IbbSize;
    }
  }

  return EFI_SUCCESS;
}

/**
  Determines the Non-Eviction Mode (NEM) region base address and size.

  @param[out] NemBase         The NEM region base address.
  @param[out] NemSize         The NEM region size.

  @retval       EFI_SUCCESS       The operation completed successfully.
  @retval       EFI_NOT_FOUND     The gNemMapStructureHobGuid was not found.
  @retval       EFI_DEVICE_ERROR  The GUID in the NEM map structure is invalid.
**/
EFI_STATUS
EFIAPI
GetNemBaseAndSize (
  OUT UINTN     *NemBase, OPTIONAL
  OUT UINT32    *NemSize  OPTIONAL
  )
{
  EFI_STATUS        Status;
  UINT32            IbbCodeSize;
  NEM_MAP_STRUCTURE *NemStructurePtr = NULL;

  // For SPI what is the NEM base? T-RAM base or BIOS image base?
  if (BootMediaIsSpi ()) {
    if (NemBase != NULL) {
      *NemBase = (UINTN) FixedPcdGet32 (PcdTemporaryRamBase);
    }

    if (NemSize != NULL) {
      Status = GetIbbBaseAndSize (NULL, &IbbCodeSize);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
      }

      *NemSize = IbbCodeSize + FixedPcdGet32 (PcdTemporaryRamSize);
    }
  } else {
    Status = GetNemMapStructure (&NemStructurePtr);
    if (EFI_ERROR (Status)) {
      return Status;
    } else if (NemStructurePtr == NULL) {
      return EFI_NOT_FOUND;
    }

    if (NemBase != NULL) {
      *NemBase = NemStructurePtr->NemBase;
    }
    if (NemSize != NULL) {
      *NemSize = NemStructurePtr->NemSize;
    }
  }

  return EFI_SUCCESS;
}

/**
  Determines the Temporary RAM (T-RAM) base address and size.

  @param[out] TempRamBase     The T-RAM base address.
  @param[out] TempRamSize     The T-RAM size.

  @retval       EFI_SUCCESS       The operation completed successfully.
  @retval       EFI_NOT_FOUND     The gNemMapStructureHobGuid was not found.
  @retval       EFI_DEVICE_ERROR  The GUID in the NEM map structure is invalid.
**/
EFI_STATUS
EFIAPI
GetTempRamBaseAndSize (
  OUT UINTN     *TempRamBase, OPTIONAL
  OUT UINT32    *TempRamSize  OPTIONAL
  )
{
  EFI_STATUS  Status;
  NEM_MAP_STRUCTURE *NemStructurePtr = NULL;

  if (BootMediaIsSpi ()) {
    if (TempRamBase != NULL) {
      *TempRamBase = (UINTN) FixedPcdGet32 (PcdTemporaryRamBase);
    }

    if (TempRamSize != NULL) {
      *TempRamSize = FixedPcdGet32 (PcdTemporaryRamSize);
    }
  } else {
    Status = GetNemMapStructure (&NemStructurePtr);
    if (EFI_ERROR (Status)) {
      return Status;
    } else if (NemStructurePtr == NULL) {
      return EFI_NOT_FOUND;
    }

    if (TempRamBase != NULL) {
      *TempRamBase = NemStructurePtr->TempRamBase;
    }
    if (TempRamSize != NULL) {
      *TempRamSize = NemStructurePtr->TempRamSize;
    }
  }

  return EFI_SUCCESS;
}

/**
  Determines the Firmware Interface Table (FIT) base address.

  @param[out] FitBase         The FIT base address.

  @retval       EFI_SUCCESS       The operation completed successfully.
  @retval       EFI_NOT_FOUND     The gNemMapStructureHobGuid was not found
  @retval       EFI_NOT_FOUND     The FIT table could not be found (or is no longer available)
  @retval       EFI_DEVICE_ERROR  The GUID in the NEM map structure is invalid.
**/
EFI_STATUS
EFIAPI
GetFitBase (
  OUT UINTN    *FitBase
  )
{
  EFI_STATUS                      Status;
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  NEM_MAP_STRUCTURE               *NemStructurePtr;
  UINT64                          FitTableOffset;

  if (BootMediaIsSpi ()) {
    FitTableOffset = *(UINT64 *) (UINTN) (BASE_4GB - 0x40);
  } else {
    Status = GetNemMapStructure (&NemStructurePtr);
    if (EFI_ERROR (Status)) {
      return Status;
    } else if (NemStructurePtr == NULL) {
      return EFI_NOT_FOUND;
    }

    FitTableOffset = (UINT64) NemStructurePtr->FitBase;
  }

  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *) (UINTN) FitTableOffset;
  if (FitEntry[0].Address != *(UINT64 *) "_FIT_   ") {
    return EFI_NOT_FOUND;
  } else {
    *FitBase = (UINTN) ((VOID *) FitEntry);
  }

  return EFI_SUCCESS;
}