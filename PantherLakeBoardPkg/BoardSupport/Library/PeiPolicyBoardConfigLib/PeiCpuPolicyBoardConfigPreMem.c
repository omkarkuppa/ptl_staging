/** @file
 Intel PEI CPU Pre-Memory Policy update by board configuration

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
#include "PeiPolicyBoardConfig.h"
#include <PlatformNvRamHookLib.h>
#include <Register/Cpuid.h>
#include <Library/TxtLib.h>
#include <TxtConfig.h>
#include <PolicyUpdateMacro.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <FspmUpd.h>

#define TXT_SINIT_MEMORY_SIZE  0x50000
#define TXT_HEAP_MEMORY_SIZE   0xF0000

#define GET_OCCUPIED_SIZE(ActualSize, Alignment) \
  ((ActualSize) + (((Alignment) - ((ActualSize) & ((Alignment) - 1))) & ((Alignment) - 1)))

#pragma pack(1)

typedef struct {
  UINT64    Address;
  UINT8     Model         : 4;
  UINT8     Family        : 4;
  UINT8     ProcessorType : 4;
  UINT8     ExtModel      : 4;
  UINT8     ModelMask     : 4;
  UINT8     FamilyMask    : 4;
  UINT8     TypeMask      : 4;
  UINT8     ExtModelMask  : 4;
  UINT16    Version;
  UINT8     FitType       : 7;
  UINT8     C_V           : 1;
  UINT8     ExtFamily     : 4;
  UINT8     ExtFamilyMask : 4;
} FIT_TYPE2_VER_200_ENTRY;

#pragma pack()

/**
  Check FIT content to find ACM base.

  @param[out]  BiosAcBase  A pointer to pointer to variable to hold found address.

  @retval  EFI_SUCCESS    If address has been found.
  @retval  EFI_NOT_FOUND  If address has not been found.
**/
EFI_STATUS
FindBiosAcmFromFit (
  OUT UINT32  **BiosAcmBase
  )
{
  EFI_STATUS                      Status;
  UINT64                          FitPointer;
  UINT64                          FitEnding;
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitTable;
  UINT32                          EntryNum;
  CPUID_VERSION_INFO_EAX          CpuSignature;
  UINT32                          Index;
  FIT_TYPE2_VER_200_ENTRY         *MultiAcmEntry;

  DEBUG ((DEBUG_INFO, "%a - start\n", __func__));

  Status              = EFI_SUCCESS;
  FitPointer          = *(UINT64 *)(UINTN)FIT_POINTER_ADDRESS;
  CpuSignature.Uint32 = 0;

  //
  // The entire FIT table must reside with in the firmware address range
  // of (4GB-16MB) to (4GB-40h).
  //
  if ((FitPointer < (SIZE_4GB - SIZE_16MB)) || (FitPointer >= (SIZE_4GB - 0x40))) {
    //
    // Invalid FIT address, treat it as no FIT table.
    //
    DEBUG ((DEBUG_ERROR, "Error: Invalid FIT pointer 0x%x.\n", FitPointer));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Check FIT header.
  //
  FitTable = (FIRMWARE_INTERFACE_TABLE_ENTRY *)(UINTN)FitPointer;
  if ((FitTable[0].Type != FIT_TYPE_00_HEADER) ||
      (FitTable[0].Address != FIT_TYPE_00_SIGNATURE))
  {
    DEBUG ((DEBUG_ERROR, "Error: Invalid FIT header.\n"));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Check FIT ending address in valid range.
  //
  EntryNum  = (((UINT32)(FitTable[0].Size[0])) + (((UINT32)(FitTable[0].Size[1])) << 8) + (((UINT32)(FitTable[0].Size[2])) << 16)) & 0x00FFFFFF;
  FitEnding = FitPointer + sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY) * EntryNum;
  if (FitEnding  > (SIZE_4GB - 0x40)) {
    DEBUG ((DEBUG_ERROR, "Table exceeds valid range.\n"));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  AsmCpuid (CPUID_VERSION_INFO, &CpuSignature.Uint32, NULL, NULL, NULL);
  DEBUG ((DEBUG_INFO, "CpuSignature:0x%x\n", CpuSignature.Uint32));

  //
  // Check Type 02 entry for ACM base address
  //
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitTable[Index].Type != FIT_TYPE_02_STARTUP_ACM) {
      continue;
    }

    //
    // Type 02 entry found
    //
    if (FitTable[Index].Version == FIT_TYPE_VERSION) {
      //
      // Make this a default base address
      //
      *BiosAcmBase = (UINT32 *)(UINTN)FitTable[Index].Address;
      DEBUG ((DEBUG_INFO, "BiosAcmBase (0x100) for entry %d:0x%x\n", Index, *BiosAcmBase));
    } else if (FitTable[Index].Version == 0x200) {
      //
      // Check for multi ACM FIT entry (Type 2, verion 0x200)
      //
      MultiAcmEntry = (FIT_TYPE2_VER_200_ENTRY *)&FitTable[Index];
      DEBUG ((DEBUG_INFO, "Type2 0x200 entry(%d):\n", Index));
      DEBUG ((DEBUG_INFO, "  Address:0x%x\n", MultiAcmEntry->Address));

      DEBUG ((DEBUG_INFO, "  Model         :0x%x\n", MultiAcmEntry->Model));
      DEBUG ((DEBUG_INFO, "  Family        :0x%x\n", MultiAcmEntry->Family));
      DEBUG ((DEBUG_INFO, "  ProcessorType :0x%x\n", MultiAcmEntry->ProcessorType));
      DEBUG ((DEBUG_INFO, "  ExtModel      :0x%x\n", MultiAcmEntry->ExtModel));
      DEBUG ((DEBUG_INFO, "  ExtFamily     :0x%x\n", MultiAcmEntry->ExtFamily));

      DEBUG ((DEBUG_INFO, "  ModelMask     :0x%x\n", MultiAcmEntry->ModelMask));
      DEBUG ((DEBUG_INFO, "  FamilyMask    :0x%x\n", MultiAcmEntry->FamilyMask));
      DEBUG ((DEBUG_INFO, "  TypeMask      :0x%x\n", MultiAcmEntry->TypeMask));
      DEBUG ((DEBUG_INFO, "  ExtModelMask  :0x%x\n", MultiAcmEntry->ExtModelMask));
      DEBUG ((DEBUG_INFO, "  ExtFamilyMask :0x%x\n", MultiAcmEntry->ExtFamilyMask));

      if (((CpuSignature.Bits.Model            & MultiAcmEntry->ModelMask)     == MultiAcmEntry->Model) && \
          ((CpuSignature.Bits.FamilyId         & MultiAcmEntry->FamilyMask)    == MultiAcmEntry->Family) && \
          ((CpuSignature.Bits.ProcessorType    & MultiAcmEntry->TypeMask)      == MultiAcmEntry->ProcessorType) && \
          ((CpuSignature.Bits.ExtendedModelId  & MultiAcmEntry->ExtModelMask)  == MultiAcmEntry->ExtModel) && \
          ((CpuSignature.Bits.ExtendedFamilyId & MultiAcmEntry->ExtFamilyMask) == MultiAcmEntry->ExtFamily))
      {
        *BiosAcmBase = (UINT32 *)(UINTN)FitTable[Index].Address;
        DEBUG ((DEBUG_INFO, "Found ACM base 0x%x in Type2 Ver 0x200 entry\n", *BiosAcmBase));
        break;
      }
    } else {
      Status = EFI_UNSUPPORTED;
      DEBUG ((DEBUG_ERROR, "Unrecognized Type 2 version\n"));
      ASSERT (FALSE);
    }
  }

Exit:
  ///
  /// If BIOS ACM is not found - don't hang system. Assume that TXT
  /// must be disabled.
  ///
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to find BIOS ACM from FIT\n"));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "ACM Module Location: %x\n", *BiosAcmBase));

  // found Startup ACM; check for 256KB alignment
  // note: no longer strictly enforced on PTL+
  if ((((EFI_PHYSICAL_ADDRESS)(BiosAcmBase)) & (EFI_PHYSICAL_ADDRESS) 0x3FFFF) != 0) {
    DEBUG ((DEBUG_INFO, "ACM not 256KB aligned\n"));
    DEBUG ((DEBUG_INFO, "This is okay on PTL+ though\n"));
  }

  return EFI_SUCCESS;
}

/**
  This function performs PEI CPU Pre-Memory Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyBoardConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  UINT32                          *BiosAcmBase;
  CPUID_VERSION_INFO_ECX          Ecx;
  UINT32                          TxtCapableChipset;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  TXT_PREMEM_CONFIG               *TxtPreMemConfig;
  CPU_SECURITY_PREMEM_CONFIG      *CpuSecurityPreMemConfig;
#endif

  DEBUG ((DEBUG_INFO, "Updating CPU Policy by board config in Pre-Mem\n"));
  BiosAcmBase = NULL;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  TxtPreMemConfig      = NULL;
  CpuSecurityPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTxtPreMemConfigGuid, (VOID *) &TxtPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  CpuSecurityPreMemConfig->TxtAcheckRequest = GetTxtAliasCheckAndReset () & 1;
  CpuSecurityPreMemConfig->TxtPowerdownRequest = GetTxtPowerdownRequest () & 1;
#endif // FspMode check

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);
  TxtCapableChipset = CheckSmxCapabilities ();

  Status = FindBiosAcmFromFit (&BiosAcmBase);
  if (!EFI_ERROR (Status)) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.BiosAcmBase, TxtPreMemConfig->BiosAcmBase, (UINTN) BiosAcmBase);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.BiosSize, CpuSecurityPreMemConfig->BiosSize, (UINT16) RShiftU64 (FixedPcdGet32 (PcdBiosSize), 10));
  }
  if ((Ecx.Bits.SMX == 1) && (TxtCapableChipset & BIT0)) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SinitMemorySize, TxtPreMemConfig->SinitMemorySize, TXT_SINIT_MEMORY_SIZE);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TxtHeapMemorySize, TxtPreMemConfig->TxtHeapMemorySize, TXT_HEAP_MEMORY_SIZE);
  }
  return EFI_SUCCESS;
}
