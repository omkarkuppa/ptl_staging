/** @file
  Provides Silicon code function for MTRRs programming.

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
#include <Library/SiMtrr.h>
#include <Library/HostBridgeInfoLib.h>
#include <Register/SncuRegs.h>

/**
  Function attempts to get the default memory type

  @param[in]  MtrrSetting    A buffer holding all MTRRs content.
  @return     Memory cache type of the specific address

**/
MTRR_MEMORY_CACHE_TYPE
GetDefaultMemoryType (
  IN CONST MTRR_SETTINGS     *MtrrSetting
  )
{
  MSR_IA32_MTRR_DEF_TYPE_REGISTER  DefType;

  DefType.Uint64 = MtrrSetting->MtrrDefType;

  return (MTRR_MEMORY_CACHE_TYPE)DefType.Bits.Type;
}

/**
  Function attempts to update the memory type of the MTRR

  @param[in]  MtrrDefType - Memory Type of MTRR.
  @param[in]  CacheType   - Cache type.
  @return     DefType     -  Memory Type of MTRR.

**/
UINT64
ChangeMtrrDefaultType (
  IN  UINT64                  MtrrDefType,
  IN  MTRR_MEMORY_CACHE_TYPE  Attribute
  )
{
  MSR_IA32_MTRR_DEF_TYPE_REGISTER  DefType;

  DefType.Uint64    = MtrrDefType;
  DefType.Bits.Type = Attribute;

  return DefType.Uint64;
}

/**
  Function attempts to get the number of variable MTRR used

  @param[in, out]  MtrrSetting - A buffer holding all MTRRs content.
  @retval          UsedVariableMtrrCount - Number of MTRR used.

**/
UINT32
EFIAPI
GetUsedVariableMtrrCount (
  IN OUT MTRR_SETTINGS      *MtrrSetting
  )
{
  UINTN                             VariableMtrrCount;
  UINTN                             Index;
  MSR_IA32_MTRR_PHYSMASK_REGISTER   Mask;
  UINT32                            UsedVariableMtrrCount;

  UsedVariableMtrrCount = 0;
  VariableMtrrCount = GetVariableMtrrCount ();

  if (MtrrSetting == NULL) {
    return MAX_UINT32;
  }

  for (Index = 0; Index < VariableMtrrCount; Index++) {
    Mask.Uint64 = MtrrSetting->Variables.Mtrr[Index].Mask;
    if (Mask.Bits.V != 0) {
      UsedVariableMtrrCount += 1;
    }
  }
  return UsedVariableMtrrCount;
}

/**
  This function attempts to set the attributes into MTRR setting buffer for a memory range.

  @retval RETURN_SUCCESS

**/
RETURN_STATUS
EFIAPI
SetMemoryAttributeInMtrrSettings (
  IN PHYSICAL_ADDRESS        BaseAddress,
  IN UINT64                  Length,
  IN MTRR_MEMORY_CACHE_TYPE  Attribute
  )
{
  MTRR_SETTINGS      LocalMtrrs;
  MTRR_SETTINGS      LocalMtrrsUC;
  MTRR_SETTINGS      LocalMtrrsWB;
  UINTN              RangeCountUC;
  UINTN              RangeCountWB;
  MTRR_MEMORY_RANGE  RangesUC[MTRR_NUMBER_OF_FIXED_MTRR * sizeof (UINT64) + 2 * MTRR_NUMBER_OF_VARIABLE_MTRR + 1];
  MTRR_MEMORY_RANGE  RangesWB[MTRR_NUMBER_OF_FIXED_MTRR * sizeof (UINT64) + 2 * MTRR_NUMBER_OF_VARIABLE_MTRR + 1];
  EFI_STATUS         Status;
  EFI_STATUS         StatusUC;
  EFI_STATUS         StatusWB;
  UINT32             UsedMtrrUC;
  UINT32             UsedMtrrWB;
  UINT8              ScratchUC[4 * SIZE_4KB];
  UINTN              ScratchSizeUC;
  UINTN              ScratchSizeWB;

  DEBUG ((DEBUG_INFO, "%a BaseAddress = 0x%lx Length = 0x%lx Attribute = 0x%x\n", __FUNCTION__, BaseAddress, Length, Attribute));

  Status = EFI_OUT_OF_RESOURCES;

  MtrrGetAllMtrrs (&LocalMtrrs);
  ZeroMem (&LocalMtrrsUC, sizeof (LocalMtrrsUC));
  ZeroMem (&LocalMtrrsWB, sizeof (LocalMtrrsWB));

  RangeCountUC = ARRAY_SIZE (RangesUC);
  RangeCountWB = ARRAY_SIZE (RangesWB);

  Status = MtrrGetMemoryAttributesInMtrrSettings (NULL, RangesUC, &RangeCountUC);
  Status = MtrrGetMemoryAttributesInMtrrSettings (NULL, RangesWB, &RangeCountWB);

  LocalMtrrsUC.MtrrDefType = ChangeMtrrDefaultType (LocalMtrrs.MtrrDefType, CacheUncacheable);
  LocalMtrrsWB.MtrrDefType = ChangeMtrrDefaultType (LocalMtrrs.MtrrDefType, CacheWriteBack);

  DEBUG ((DEBUG_INFO, "MtrrsUC->MtrrDefType = 0x%x UsedMtrrWB = 0x%x\n", LocalMtrrsUC.MtrrDefType, LocalMtrrsWB.MtrrDefType));
  ScratchSizeUC = sizeof (ScratchUC);
  ScratchSizeWB = sizeof (ScratchUC);

  StatusUC = MtrrSetMemoryAttributesInMtrrSettings (&LocalMtrrsUC, ScratchUC, &ScratchSizeUC, RangesUC, RangeCountUC);
  StatusWB = MtrrSetMemoryAttributesInMtrrSettings (&LocalMtrrsWB, ScratchUC, &ScratchSizeWB, RangesWB, RangeCountWB);
  DEBUG ((DEBUG_INFO, "StatusUC = %r StatusWB = %r\n", StatusUC, StatusWB));

  if (StatusUC == EFI_SUCCESS) {
    StatusUC = MtrrSetMemoryAttributeInMtrrSettings (
                &LocalMtrrsUC,
                BaseAddress,
                Length,
                Attribute
                );
  }

  if (StatusWB == EFI_SUCCESS) {
    StatusWB = MtrrSetMemoryAttributeInMtrrSettings (
                &LocalMtrrsWB,
                BaseAddress,
                Length,
                Attribute
                );
  }
  DEBUG ((DEBUG_INFO, "StatusUC = %r StatusWB = %r\n", StatusUC, StatusWB));
  if ((StatusUC == EFI_SUCCESS) && (StatusWB == EFI_SUCCESS)) {
    Status  = MtrrGetMemoryAttributesInMtrrSettings (&LocalMtrrsUC, RangesUC, &RangeCountUC);
    Status  = MtrrGetMemoryAttributesInMtrrSettings (&LocalMtrrsWB, RangesWB, &RangeCountWB);

    UsedMtrrUC = GetUsedVariableMtrrCount (&LocalMtrrsUC);
    UsedMtrrWB = GetUsedVariableMtrrCount (&LocalMtrrsWB);

    DEBUG ((DEBUG_INFO, "UsedMtrrUC = %d UsedMtrrWB = %d\n", UsedMtrrUC, UsedMtrrWB));

    if (UsedMtrrWB >= UsedMtrrUC){
      MtrrSetAllMtrrs (&LocalMtrrsUC);
    } else {
      MtrrSetAllMtrrs (&LocalMtrrsWB);
    }
  } else if (StatusUC == EFI_SUCCESS) {
    MtrrSetAllMtrrs (&LocalMtrrsUC);
  } else if (StatusWB == EFI_SUCCESS) {
    MtrrSetAllMtrrs (&LocalMtrrsWB);
  } else {
    ASSERT_RETURN_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a dumping MTRRs", __FUNCTION__));
  MtrrDebugPrintAllMtrrs ();

  return EFI_SUCCESS;
}

/**
  Function attempts to update MTRRs for flash region.

**/
VOID
EFIAPI
MtrrUpdateForFlashRegion (
  VOID
  )
{
  EFI_STATUS             Status;
  VOID                   *SpiInfoHob;
  SPI_INFO_HOB           *SpiInfoHobData;
  BOOLEAN                ExtendedBiosSupport = FALSE;

  SpiInfoHob = GetFirstGuidHob (&gSpiInfoHobGuid);

  if (SpiInfoHob != NULL) {
    SpiInfoHobData = (SPI_INFO_HOB *) GET_GUID_HOB_DATA (SpiInfoHob);
    ExtendedBiosSupport = SpiInfoHobData->ExtendedBiosDecodeRangeEnable;
  } else {
    DEBUG ((DEBUG_INFO, "SPI Info Hob not found\n"));
  }

  DEBUG ((DEBUG_INFO, "Silicon MTRR Update MTRRs Setting Start\n"));

  Status = MtrrSetMemoryAttributeInMtrrSettings (
              NULL,
              0xff000000,
              0x01000000,
              CacheUncacheable
            );
  ASSERT_EFI_ERROR (Status);

  if (ExtendedBiosSupport) {
    DEBUG ((DEBUG_INFO, "Extended BIOS Region Support Enabled, Update the MtrrPatchTable\n"));
    Status = MtrrSetMemoryAttributeInMtrrSettings (
                NULL,
                FixedPcdGet32(PcdSiExtendedBiosRegionBaseAddress),
                FixedPcdGet32(PcdSiExtendedBiosRegionSize),
                CacheUncacheable
              );
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "Silicon MTRR Update MTRRs Setting End\n"));
}

/**
  Function attempts to and update MTRRs Setting.

  @param[out]  MtrrSetting      - A buffer holding all MTRRs content.

  @retval EFI_SUCCESS           - The function completed successfully.
  @retval EFI_UNSUPPORTED       - Mtrr is not supported.
  @retval EFI_INVALID_PARAMETER - MtrrSetting is NULL.
**/
EFI_STATUS
EFIAPI
MtrrTransfer2DefaultWB (
  OUT MTRR_SETTINGS  *MtrrSetting
  )
{
  EFI_STATUS                    Status;
  UINT64                        McD0BaseAddress;
  UINT64                        Touud;
  TOUUD_HOSTBRIDGE_CFG_STRUCT   TouudCfg;
  UINT64                        Granularity;
  UINT64                        NextPwrOf2;
  UINT64                        TouudLength;
  UINT64                        GtApertureAdr;
  UINT64                        LMemBarSize;

  NextPwrOf2    = 1;
  GtApertureAdr = 0;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  if (!IsMtrrSupported ()) {
    return EFI_UNSUPPORTED;
  }

  if (MtrrSetting == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MtrrUpdateForFlashRegion ();

  //
  // If device 0:2:0 (Internal Graphics Device, or GT) is not present, skip it.
  // Convert the LMem region to UC, later when changing the default MTRR type, LMem will be programmed to WC.
  //
  if (IGpuIsSupported () == TRUE) {
    GtApertureAdr = IGpuGetLMemBar ();
    LMemBarSize   = IGpuGetLMemBarSize ();

    Status = MtrrSetMemoryAttributeInMtrrSettings (
                NULL,
                GtApertureAdr,
                LMemBarSize,
                CacheUncacheable
              );
    ASSERT_EFI_ERROR (Status);
  }

  if (IsAbove4GBMmioEnabled ()) {
    //
    // Set above 4GB Mmio space to Uncacheable
    //
    Status = SetMemoryAttributeInMtrrSettings (
               PcdGet64 (PcdMmio64Base),
               PcdGet64 (PcdMmio64Length),
               CacheUncacheable
               );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  McD0BaseAddress   = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, 0);
  TouudCfg.Data32[0] = PciSegmentRead32 (McD0BaseAddress + TOUUD_HOSTBRIDGE_CFG_REG );
  TouudCfg.Data32[1] = PciSegmentRead32 (McD0BaseAddress + TOUUD_HOSTBRIDGE_CFG_REG + 4);
  Touud = LShiftU64 ((UINT64) TouudCfg.Bits.touud, TOUUD_HOSTBRIDGE_CFG_TOUUD_LSB);

  Granularity = (UINT64)((Touud & 0xFFFFFF) / SIZE_1MB);
  if (!(Granularity && !(Granularity & (Granularity - 1)))) {
    while (NextPwrOf2 < Granularity) {
      NextPwrOf2 <<= 1;
    }
    TouudLength = (NextPwrOf2 - Granularity) * SIZE_1MB;
  } else {
    TouudLength = Granularity * SIZE_1MB;
  }

  //
  // Set above 4GB MMIO space to Uncacheable
  //
  Status = SetMemoryAttributeInMtrrSettings (
              Touud,
              TouudLength,
              CacheUncacheable
              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IGpuIsSupported () == TRUE) {
    Status = SetMemoryAttributeInMtrrSettings (
                GtApertureAdr,
                LMemBarSize,
                CacheWriteCombining
              );
    ASSERT_EFI_ERROR (Status);
  }

  MtrrGetAllMtrrs (MtrrSetting);
  MtrrDebugPrintAllMtrrs ();

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return EFI_SUCCESS;
}
