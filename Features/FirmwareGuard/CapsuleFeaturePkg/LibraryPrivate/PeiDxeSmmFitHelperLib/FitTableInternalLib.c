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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
//
// EDK-II Platform Foundation.
//
#include <IndustryStandard/FirmwareInterfaceTable.h>
//
// Module Self Foundation.
//
#include "FitEntryInternalLib.h"
#include "FitTableInternalLib.h"

typedef struct {
  UINT8   FitType;
  UINT16  FitEntryNumType;
} FIT_ENTRY_NUM_INFO;

#define FIT_ENTRY_TYPE_INST_ONE_ONLY      0x00
#define FIT_ENTRY_TYPE_INST_AT_LEAST_ONE  0x01
#define FIT_ENTRY_TYPE_INST_AT_MOST_ONE   0x02
#define FIT_ENTRY_TYPE_INST_NO_LIMITED    0x03

FIT_ENTRY_NUM_INFO  mFitEntryNumInfoTbl[] = {
  { FIT_TYPE_00_HEADER,                 FIT_ENTRY_TYPE_INST_ONE_ONLY     },
  { FIT_TYPE_01_MICROCODE,              FIT_ENTRY_TYPE_INST_AT_LEAST_ONE },
  { FIT_TYPE_02_STARTUP_ACM,            FIT_ENTRY_TYPE_INST_AT_LEAST_ONE },
  { FIT_TYPE_03_DIAGNOSTIC_ACM,         FIT_ENTRY_TYPE_INST_NO_LIMITED   },
  { FIT_TYPE_04_PROT_BOOT_POLICY,       FIT_ENTRY_TYPE_INST_NO_LIMITED   },
  { FIT_TYPE_05_MMC_FIRMWARE_IMAGE,     FIT_ENTRY_TYPE_INST_NO_LIMITED   },
  { FIT_TYPE_06_FIT_RESET_STATE,        FIT_ENTRY_TYPE_INST_NO_LIMITED   },
  { FIT_TYPE_07_BIOS_STARTUP_MODULE,    FIT_ENTRY_TYPE_INST_NO_LIMITED   },
  { FIT_TYPE_08_TPM_POLICY,             FIT_ENTRY_TYPE_INST_AT_MOST_ONE  },
  { FIT_TYPE_09_BIOS_POLICY,            FIT_ENTRY_TYPE_INST_AT_MOST_ONE  },
  { FIT_TYPE_0A_TXT_POLICY,             FIT_ENTRY_TYPE_INST_AT_MOST_ONE  },
  { FIT_TYPE_0B_KEY_MANIFEST,           FIT_ENTRY_TYPE_INST_NO_LIMITED   },
  { FIT_TYPE_0C_BOOT_POLICY_MANIFEST,   FIT_ENTRY_TYPE_INST_NO_LIMITED   },
  { FIT_TYPE_0D_FSP_BOOT_MANIFEST,      FIT_ENTRY_TYPE_INST_NO_LIMITED   },
  { FIT_TYPE_10_CSE_SECURE_BOOT,        FIT_ENTRY_TYPE_INST_NO_LIMITED   },
  { FIT_TYPE_1A_VAB_PROVISIONING_TABLE, FIT_ENTRY_TYPE_INST_AT_MOST_ONE  },
  { FIT_TYPE_1B_VAB_KEY_MANIFEST,       FIT_ENTRY_TYPE_INST_AT_MOST_ONE  },
  { FIT_TYPE_1C_VAB_IMAGE_MANIFEST,     FIT_ENTRY_TYPE_INST_AT_MOST_ONE  },
  { FIT_TYPE_1D_VAB_IMAGE_HASH_DESC,    FIT_ENTRY_TYPE_INST_AT_MOST_ONE  },
  { FIT_TYPE_2C_SACM_DEBUG,             FIT_ENTRY_TYPE_INST_AT_MOST_ONE  },
  { FIT_TYPE_2D_TXTSX_POLICY,           FIT_ENTRY_TYPE_INST_NO_LIMITED   },
  { FIT_TYPE_2E_GRANULAR_SCRTM_ERROR,   FIT_ENTRY_TYPE_INST_AT_MOST_ONE  },
  { FIT_TYPE_2F_JMP_DEBUG_POLICY,       FIT_ENTRY_TYPE_INST_NO_LIMITED   }
};

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
  )
{
  EFI_STATUS                      Status;
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitType00Entry;

  if ((FitPtr == NULL) || (FitTableSize == 0) || (FitEntryNum == 0)) {
    return FALSE;
  }

  if ((FitTableSize % sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY)) != 0) {
    return FALSE;
  }

  Status = GetFitEntryType00 (FitPtr, &FitType00Entry);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if (FitType00Entry->C_V == CHECKSUM_DISABLED) {
    return TRUE;
  }

  if (CalculateCheckSum8 ((UINT8 *)FitPtr, FitTableSize) != 0x00) {
    return FALSE;
  }

  return TRUE;
}

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
  )
{
  UINT32                          Idx;
  UINT8                           PreviousFitEntryType;
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryTable;

  if ((FitPtr == NULL) || (FitTableSize == 0) || (FitEntryNum == 0)) {
    return FALSE;
  }

  if ((FitTableSize % sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY)) != 0) {
    return FALSE;
  }

  FitEntryTable        = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FitPtr;
  PreviousFitEntryType = FIT_TYPE_00_HEADER;

  for (Idx = 0; Idx < FitEntryNum; Idx++) {
    if (FitEntryTable[Idx].Type < PreviousFitEntryType) {
      DEBUG ((DEBUG_ERROR, "Invalid FIT entry found - [%d]\n", Idx));
      return FALSE;
    }

    PreviousFitEntryType = FitEntryTable[Idx].Type;
  }

  return TRUE;
}

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
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitTablePtr;
  UINT32                          Idx;

  if ((FitPtr == NULL) || (FitTableSize == 0) || (FitEntryNum == 0)) {
    return FALSE;
  }

  if ((FitTableSize % sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY)) != 0) {
    return FALSE;
  }

  FitTablePtr = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FitPtr;

  for (Idx = 0; Idx < FitEntryNum; Idx++) {
    if (!IsFitEntryTypeValid (FitTablePtr[Idx].Type)) {
      DEBUG ((DEBUG_ERROR, "Invalid FIT entry found - [%d]\n", Idx));
      return FALSE;
    }
  }

  return TRUE;
}

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
  )
{
  EFI_STATUS  Status;
  UINT32      Idx;
  UINT32      FitEntryTypeNum;
  UINT32      MinNum;
  UINT32      MaxNum;

  if ((FitPtr == NULL) || (FitTableSize == 0) || (FitEntryNum == 0)) {
    return FALSE;
  }

  if ((FitTableSize % sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY)) != 0) {
    return FALSE;
  }

  for (Idx = 0; Idx < sizeof (mFitEntryNumInfoTbl) / sizeof (FIT_ENTRY_NUM_INFO); Idx++) {
    switch (mFitEntryNumInfoTbl[Idx].FitEntryNumType) {
      case FIT_ENTRY_TYPE_INST_ONE_ONLY:
        MinNum = 0x000001;
        MaxNum = 0x000001;
        break;
      case FIT_ENTRY_TYPE_INST_AT_LEAST_ONE:
        MinNum = 0x000001;
        MaxNum = 0xFFFFFF;
        break;
      case FIT_ENTRY_TYPE_INST_AT_MOST_ONE:
        MinNum = 0x000000;
        MaxNum = 0x000001;
        break;
      case FIT_ENTRY_TYPE_INST_NO_LIMITED:
        MinNum = 0x000000;
        MaxNum = 0xFFFFFF;
        break;
      default:
        return FALSE;
    }

    Status = GetEntryNumByType (FitPtr, mFitEntryNumInfoTbl[Idx].FitType, &FitEntryTypeNum);
    if (EFI_ERROR (Status)) {
      return FALSE;
    }

    if ((FitEntryTypeNum < MinNum) || (FitEntryTypeNum > MaxNum)) {
      DEBUG ((DEBUG_ERROR, "FIT entry Type (0x%X)\n", mFitEntryNumInfoTbl[Idx].FitType));
      DEBUG ((DEBUG_ERROR, "Expected Number of instances - Min (%d) - Max (%d)\n", MinNum, MaxNum));
      DEBUG ((DEBUG_ERROR, "Invalid number of FIT entry type instances - (%d)\n", FitEntryTypeNum));
      return FALSE;
    }
  }

  return TRUE;
}

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
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryType00;

  if ((FitPtr == NULL) || (FitEntryPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  FitEntryType00 = &(((FIRMWARE_INTERFACE_TABLE_ENTRY *)FitPtr)[FIT_TYPE_00_ENTRY_NUMBER]);

  if (!IsFitEntryType00Valid (FitEntryType00)) {
    return EFI_NOT_FOUND;
  }

  *FitEntryPtr = FitEntryType00;

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                      Status;
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitType00Entry;

  if ((FitPtr == NULL) || (FitEntryNumPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetFitEntryType00 (FitPtr, &FitType00Entry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the FIT entry type 00 - %r.\n", Status));
    return Status;
  }

  Status = GetFitEntrySizeField (FitType00Entry, FitEntryNumPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the number of FIT entries in FIT - %r.\n", Status));
    return Status;
  }

  if (*FitEntryNumPtr == 0) {
    DEBUG ((DEBUG_ERROR, "Invalid number of FIT entries - 0.\n"));
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                      Status;
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryBufferPtr;
  UINT32                          EntryNum;
  UINT32                          Index;
  UINT32                          FoundEntryNum;

  FoundEntryNum = 0;

  if ((FitPtr == NULL) || !IsFitEntryTypeValid (FitEntryType) || (FitEntryNumPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetEntryNumOfFit (FitPtr, &EntryNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the entry number of FIT table - %r\n", Status));
    return Status;
  }

  FitEntryBufferPtr = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FitPtr;
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntryBufferPtr[Index].Type != FitEntryType) {
      continue;
    }

    FoundEntryNum += 1;
  }

  CopyMem ((VOID *)FitEntryNumPtr, (VOID *)&FoundEntryNum, sizeof (UINT32));

  return EFI_SUCCESS;
}
