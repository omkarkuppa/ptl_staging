/** @file
  Memory Sub System Fru library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Pi/PiPeiCis.h>
#include <Library/PeiServicesLib.h>
#include <MemorySubSystemHandle.h>
#include <Library/PeiHostBridgeInitLib.h>
#include <Library/TmeLib.h>
#include <Library/CceLib.h>
#include <Library/PeiTdxLib.h>
#include <Library/TdxInfoLib.h>
#include <Library/PeiNguInitLib.h>
#include <CpuInitPreMemConfig.h>
#include <Library/PeiImrInitLib.h>
#include <Library/PsmiInitLib.h>
#include <Library/HobLib.h>
#include <Library/TxtPeiLib.h>
#include <Library/CpuCommonLib.h>
#include <IGpuDataHob.h>
#include <Library/TseInfoLib.h>
#include <Library/TseDataHob.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/PeiCpuInitFruLib.h>
#include <Library/DomainIGpuInit.h>
#include <Library/CpuMailboxLib.h>
#include <Library/IGpuInfoLib.h>
#include <Library/MemorySubSystemInfoLib.h>
#include <Library/TdxFruLib.h>
#include <Library/TdxDataHob.h>

/**
  Memory Sub System Handler Initialization

  @param[in] MemorySubSystemHandle    Hanlder for Memory Sub System
  @param[in] MemorySubSystemCallback  CallBack for Memory Sub System
**/
EFI_STATUS
EFIAPI
SetMemoryMap (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcInput                     *Inputs;
  MrcOutput                    *Outputs;
  MrcMemoryMap                 *MemoryMap;
  UINT32                       GraphicsStolenSize;
  UINT32                       TsegBaseOrg;
  UINT32                       PrmrrBaseOrg;
  UINT32                       PrmrrAlign;
  INT32                        TsegAlignment;
  UINT32                       MmioSize;
  UINT32                       LowestBase;
  UINT32                       ImrBaseOrg;
  UINT32                       PsmiHandlerSize;
  UINT32                       GraphicsGttSize;
  UINT32                       TseImrSize;
  TSE_DATA_HOB                 *TseDataHob;
  TDX_DATA_HOB                 *TdxDataHobPtr;
  PSMI_DATA_HOB_VER1           *PsmiDataHob;
  MRC_EXT_INPUTS_TYPE          *ExtInputs;
  UINT32                       ToludBaseInBytes;
  UINT64                       RemapBaseInBytes;
  UINT64                       TouudBaseInBytes;
  UINT32                       PsmiHandlerBase;
  UINT32                       SeamrrSize;
  EFI_STATUS                   Status;
  UINT32                       Remainder;

  Inputs    = &MrcData->Inputs;
  Outputs   = &MrcData->Outputs;
  MemoryMap = &Outputs->MemoryMapData;
  ExtInputs = Inputs->ExtInputs.Ptr;
  PsmiHandlerSize = 0;
  GraphicsStolenSize = 0;
  GraphicsGttSize = 0;
  PsmiHandlerBase = 0;
  SeamrrSize = 0;
  Remainder = 0;
  PsmiDataHob = NULL;
  Status = EFI_SUCCESS;
  PsmiDataHob = (PSMI_DATA_HOB_VER1 *) GetFirstGuidHob (&gPsmiDataHobGuid);
  if (PsmiDataHob != NULL) {
    PsmiHandlerSize = PsmiDataHob->PsmiConfig.HandlerInfo.HandlerSize;
    // If handler if more than 0 we will allocate Max 1MB later based on the Size we will convert the memory to write back
    if (PsmiHandlerSize > 0) {
      PsmiHandlerSize = 1;
    }
  }

  //
  // Set TOM register
  //
  DEBUG ((DEBUG_INFO, "Total physical memory size = %u MB\n", MemoryMap->TotalPhysicalMemorySize));

  //
  // Find the TOM minus ME size only for internal calculations
  //
  MemoryMap->TomMinusMe = MemoryMap->TotalPhysicalMemorySize;

  MmioSize = Inputs->MmioSize;

  //
  // Find and set TOLUD.
  // TOLUD which is the minimum value by comparing between "4GB minus MMIO size" and "TOM minus ME size"
  //
  MemoryMap->ToludBase = MIN (MemoryMap->TomMinusMe, MEM_4GB - MmioSize);

  if (IGpuIsSupported ()) {
    //
    // Graphics Stolen size in Mb
    //
    GraphicsGttSize  = (IGpuGetGsmSizeInBytes () >> 20);

    //
    // Find and set BDSM Graphics Stolen Base.
    // Graphics Stolen Base, BDSM by subtracting the graphics data stolen memory size and PSMI region size from TOLUD.
    //
    GraphicsStolenSize = (IGpuGetDsmSizeInBytes () >> 20);
  }

  DEBUG ((DEBUG_INFO, "GraphicsStolenSize = 0x%x\n",GraphicsStolenSize));
  DEBUG ((DEBUG_INFO, "GraphicsGttSize = 0x%x\n",GraphicsGttSize));

  MemoryMap->BdsmBase = MemoryMap->ToludBase - GraphicsStolenSize;

  //
  // Graphics GTT Stolen Base
  // Graphics GTT Stolen Base, BGSM by subtracting the GTT graphics stolen memory size from BDSM.
  //
  MemoryMap->GttBase = MemoryMap->BdsmBase - GraphicsGttSize;

  //
  // TSEG Base
  //
  MemoryMap->TsegBase = MemoryMap->GttBase - Inputs->TsegSize;
  LowestBase = MemoryMap->TsegBase;

  //
  // DPR is after TSEG.
  //
  LowestBase         -= Inputs->DprSize;

  DEBUG ((DEBUG_INFO, "Unaligned TSEG base = %Xh\n", MemoryMap->TsegBase));
  DEBUG ((DEBUG_INFO, "Unaligned GTT base = %Xh\n", MemoryMap->GttBase));
  DEBUG ((DEBUG_INFO, "Unaligned BDSM base = %Xh\n", MemoryMap->BdsmBase));
  DEBUG ((DEBUG_INFO, "Unaligned TOLUD = %Xh\n", MemoryMap->ToludBase));

  //
  // Reserve PRMRR region after the DPR region or 4GB for PrmrrSize < 512 MB.
  // If PrmrrSize >= 512 MB, place at 4 GB. This code is much later, after this memory is determined.
  //
  if (Inputs->PrmrrSize > 0 && Inputs->PrmrrSize < 512) {
    PrmrrBaseOrg  = LowestBase - Inputs->PrmrrSize;

    //
    // Size aligned
    //
    MemoryMap->PrmrrBase = PrmrrBaseOrg & ~(Inputs->PrmrrSize - 1);

    //
    // Used to align other regions
    //
    PrmrrAlign = PrmrrBaseOrg - MemoryMap->PrmrrBase;

    LowestBase = MemoryMap->PrmrrBase;

    DEBUG ((DEBUG_INFO, "PRMRR base = %Xh\n", MemoryMap->PrmrrBase));
  } else {
    //
    // No shift for no PRMRR
    //
    PrmrrAlign = 0;
  }

  MemoryMap->MemoryHoles[0].BaseAddress = MemoryMap->PrmrrBase + Inputs->PrmrrSize;
  //
  //  Align TSEG to PRMRR.
  //
  MemoryMap->TsegBase = MemoryMap->TsegBase - PrmrrAlign;

  //
  //  Align TSEG to natural alignment.
  //
  if (MemoryMap->TsegBase != (MemoryMap->TsegBase & ~(Inputs->TsegSize - 1))) {
    TsegBaseOrg = MemoryMap->TsegBase;
    if (Inputs->PrmrrSize > 0 && Inputs->PrmrrSize < 512) {

      //
      // If PRMRR is present, naturally align TSEG up.
      //
      MemoryMap->TsegBase = (MemoryMap->TsegBase & ~(Inputs->TsegSize - 1)) + Inputs->TsegSize;
    } else {

      //
      // No PRMRR so naturally align TSEG down.
      //
      MemoryMap->TsegBase = (MemoryMap->TsegBase & ~(Inputs->TsegSize - 1));
    }

    //
    // Calculate the delta to shift BDSM, GTT, and TOLUD with TSEG.
    // DPR is always right below TSEG so it's shift is implied.
    //
    TsegAlignment = ((INT32) MemoryMap->TsegBase) - ((INT32) TsegBaseOrg);
  } else {

    //
    // TSEG is already aligned
    //
    TsegAlignment = 0;
  }

  //
  // Shift everything up through TOLUD.
  //
  MemoryMap->MemoryHoles[0].Length = MemoryMap->TsegBase - Inputs->DprSize - MemoryMap->MemoryHoles[0].BaseAddress;
  MemoryMap->GttBase   = ((UINT32) (((INT32) (MemoryMap->GttBase - PrmrrAlign)) + TsegAlignment));
  MemoryMap->BdsmBase  = ((UINT32) (((INT32) (MemoryMap->BdsmBase - PrmrrAlign)) + TsegAlignment));
  MemoryMap->ToludBase = ((UINT32) (((INT32) (MemoryMap->ToludBase - PrmrrAlign)) + TsegAlignment));
  //
  // TOLUD has 2MB base Alignment requirement
  //
  ToludBaseInBytes = (UINT32) LShiftU64 (MemoryMap->ToludBase, 20);
  ToludBaseInBytes = (UINT32) NaturalAlignment (ToludBaseInBytes, SIZE_2MB);
  MemoryMap->ToludBase = (UINT32) RShiftU64 (ToludBaseInBytes, 20);

  DEBUG ((DEBUG_INFO, "TSEG base = %Xh\n", MemoryMap->TsegBase));
  DEBUG ((DEBUG_INFO, "GTT base = %Xh\n", MemoryMap->GttBase));
  DEBUG ((DEBUG_INFO, "BDSM base = %Xh\n", MemoryMap->BdsmBase));
  DEBUG ((DEBUG_INFO, "TOLUD = %Xh\n", MemoryMap->ToludBase));

  //
  // In order to avoid memory hole, assign LowestBase to new aligned DPRBASE when PRMRR is absent.
  //

  if (Inputs->PrmrrSize == 0 || Inputs->PrmrrSize >= 512) {
    LowestBase = MemoryMap->TsegBase - Inputs->DprSize;
  }

  //
  // test if Reclaim is available
  // If "TOM minus ME Stolem Memory Size" is greater than the value of TOLUD, then memory reclaim is available to enable
  //
  if (ExtInputs->RemapEnable && (MemoryMap->TomMinusMe > MemoryMap->ToludBase)) {
    DEBUG ((DEBUG_INFO, "Reclaim Enable\n"));
    MemoryMap->ReclaimEnable = TRUE;
    //
    // Remap Base
    // This is the maximum value by comparing between 4GB and "TOM minus ME size".
    // Remap Base has 2MB base Alignment requirement
    //
    MemoryMap->RemapBase = MAX (MEM_4GB, MemoryMap->TomMinusMe);
    RemapBaseInBytes = LShiftU64 (MemoryMap->RemapBase, 20);
    RemapBaseInBytes     = NaturalAlignment (RemapBaseInBytes, SIZE_2MB);
    MemoryMap->RemapBase = (UINT32) RShiftU64 (RemapBaseInBytes, 20);
    DEBUG ((DEBUG_INFO, "Remap Base %Xh\n", MemoryMap->RemapBase));
    //
    // Remap Limit
    // This is the value of REMAPBASE plus "the difference between the value in TOLUD
    // register and the lower of either 4GB or 'TOM minus ME Stolen memory size", and
    // then minus 1MB boundary.
    // TOUUD has 2MB base Alignment requirement
    //
    MemoryMap->RemapLimit = MemoryMap->RemapBase + (MIN (MEM_4GB, MemoryMap->TomMinusMe) - MemoryMap->ToludBase);

    MemoryMap->TouudBase = MemoryMap->RemapLimit;
    TouudBaseInBytes     = LShiftU64 (MemoryMap->TouudBase, 20);
    TouudBaseInBytes     = NaturalAlignment (TouudBaseInBytes, SIZE_2MB);
    MemoryMap->TouudBase = (UINT32) RShiftU64 (TouudBaseInBytes, 20);

    MemoryMap->RemapLimit -= 0x1;

    DEBUG ((DEBUG_INFO, "Remap Limit %Xh\n", MemoryMap->RemapLimit));
    DEBUG ((DEBUG_INFO, "TOUUD %Xh\n", MemoryMap->TouudBase));
  } else {
    MemoryMap->ReclaimEnable = FALSE;
    DEBUG ((DEBUG_INFO, "Reclaim disable \n"));
    //
    // TOUUD
    // If relcaim is disabled, this value is calculated by "TOM minus ME stolen size".
    // TOUUD has 2MB base Alignment requirement
    //
    MemoryMap->TouudBase = MemoryMap->TomMinusMe;
    TouudBaseInBytes     = LShiftU64 (MemoryMap->TouudBase, 20);
    TouudBaseInBytes     = NaturalAlignment (TouudBaseInBytes, SIZE_2MB);
    MemoryMap->TouudBase = (UINT32) RShiftU64 (TouudBaseInBytes, 20);
    DEBUG ((DEBUG_INFO, "TOUUD %Xh\n", MemoryMap->TouudBase));
  }

  //
  // Reserve IMR region.
  // Must be aligned to largest IMR Size
  //
  if (Inputs->MeStolenSize > 0) {
    ImrBaseOrg  = LowestBase - Inputs->MeStolenSize;
    //
    // Largest IMR size alignment
    //
    MemoryMap->MeStolenBase = ImrBaseOrg & ~(Inputs->ImrAlignment - 1);
    LowestBase = MemoryMap->MeStolenBase;
    MemoryMap->MemoryHoles[1].BaseAddress = MemoryMap->MeStolenBase + Inputs->MeStolenSize;
    MemoryMap->MemoryHoles[1].Length = ImrBaseOrg - MemoryMap->MeStolenBase;
  }
  MemoryMap->MeStolenSize = Inputs->MeStolenSize;
  DEBUG ((DEBUG_INFO, "ME stolen base %Xh\nME stolen size %Xh\n", MemoryMap->MeStolenBase, MemoryMap->MeStolenSize));

  if (Inputs->PrmrrSize >= 512) {
    //MRC_DEBUG_ASSERT ((MEM_4GB + Inputs->PrmrrSize) < MemoryMap->TouudBase, Debug, "PRMRR Size is >= 512 MB, but no memory available above 4 GB."); @Ashraf
    if (MEM_4GB + Inputs->PrmrrSize < MemoryMap->TouudBase) {
      MemoryMap->PrmrrBase = MEM_4GB;
      DEBUG ((DEBUG_INFO, "PRMRR base = %Xh\n", MemoryMap->PrmrrBase));
    }
  }

  //
  // TSE IMR allocation
  //
  TseDataHob = (TSE_DATA_HOB *) GetFirstGuidHob (&gTseDataHobGuid);

  if ((IsTseEnabled () == TRUE) &&
      (TseDataHob != NULL)) {
    TseImrSize = SIZE_256KB;
    TseImrSize = MAX (SIZE_1MB, TseImrSize); // Ensure allocations are at minumum 1MB.
    TseImrSize = TseImrSize >> 20;           // Convert to MB units

    MemoryMap->TseImrSize = TseImrSize;
    ImrBaseOrg = LowestBase - TseImrSize;

    //
    // Check for natural alignment
    //
    MemoryMap->TseImrBase = (ImrBaseOrg & ~(TseImrSize - 1));
    LowestBase = MemoryMap->TseImrBase;

    MemoryMap->MemoryHoles[2].BaseAddress = MemoryMap->TseImrBase + MemoryMap->TseImrSize;
    MemoryMap->MemoryHoles[2].Length = ImrBaseOrg - MemoryMap->TseImrBase;

    DEBUG ((DEBUG_INFO, "TSE IMR base %Xh\nTSE IMR size %Xh\n", MemoryMap->TseImrBase, MemoryMap->TseImrSize));

    TseDataHob->TseDramMemSize = MemoryMap->TseImrSize;
    TseDataHob->TseDramMemBase = MemoryMap->TseImrBase;
  } else {
    MemoryMap->TseImrSize = 0;
    MemoryMap->TseImrBase = 0;
  }

  //
  // Seamrr assignment below 4GB
  //
  if (IsTdxSupported () && MemoryMap->TotalPhysicalMemorySize <= 0x1000) {
    TdxDataHobPtr = (TDX_DATA_HOB *) GetFirstGuidHob (&gTdxDataHobGuid);
    if (TdxDataHobPtr == NULL) {
      return EFI_UNSUPPORTED;
    }
    SeamrrSize = CalculateSeamrrSize (Outputs);
    DEBUG ((DEBUG_INFO, "SeamrrSize = %Xh \n", SeamrrSize));

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Cannot execute GetSeamrrSize() properly %r\n", Status));
    }
    if (SeamrrSize > 0) {
      ImrBaseOrg = LowestBase - SeamrrSize;
      MemoryMap->SeamrrBase = ImrBaseOrg;
      DEBUG ((DEBUG_INFO, "ImrBaseOrg= %Xh \n", ImrBaseOrg));
      DEBUG ((DEBUG_INFO, "PMemoryMap->SeamrrBase= %Xh \n", MemoryMap->SeamrrBase));
      //
      // Check for 32MB alignment
      //
      DivU64x32Remainder ((UINT32) TdxDataHobPtr->SeamRrBaseAddress, SIZE_32MB, &Remainder);
      if (Remainder != 0) {
        MemoryMap->MemoryHoles[3].BaseAddress = (UINT32) MemoryMap->SeamrrBase;
        MemoryMap->MemoryHoles[3].Length = (UINT32) Remainder;
        MemoryMap->SeamrrBase -= (UINT32) Remainder;
    }
      TdxDataHobPtr->SeamRrBaseAddress = MemoryMap->SeamrrBase;
      TdxDataHobPtr->SeamrrSize = SeamrrSize;
      LowestBase = (UINT32) MemoryMap->SeamrrBase;
      DEBUG ((DEBUG_INFO, "TdxDataHobPtr->SeamRrBaseAddress= %Xh \n", TdxDataHobPtr->SeamRrBaseAddress));
      DEBUG ((DEBUG_INFO, "TdxDataHobPtr->SeamrrSize= %Xh \n", TdxDataHobPtr->SeamrrSize));
    }
  }
  //
  // PSMI Handelr to allocate the memory for the Handler
  //
  DEBUG ((DEBUG_INFO, "PsmiHandlerSize = 0x%x\n", PsmiHandlerSize));
  //
  // Allocate the PSMI Handelr Base Below MeStolenBase
  //
  if (PsmiHandlerSize == 0) {
    PsmiHandlerBase = 0;
  } else {
    PsmiHandlerBase = LowestBase - PsmiHandlerSize;
    LowestBase = PsmiHandlerBase;
  }

  //
  // Update PSMI Handler Base
  //
  UpdatePsmiHandlerBaseAddress (PsmiHandlerBase);

  DEBUG ((DEBUG_INFO, "LowestBase = %Xh\n", LowestBase));
  DEBUG ((DEBUG_INFO, "PsmiHanlder base = %Xh\n", PsmiHandlerBase << 20));

  //
  // Shift from MB's to actual address.
  //
  LowestBase = LowestBase << 20;

  MemoryMap->LowestBase = LowestBase;

  return EFI_SUCCESS;
}

VOID
EFIAPI
MemorySubSystemHandleInit (
  IN OUT  MEMORY_SUB_SYSTEM_HANDLE    *MemorySubSystemHandle,
  IN OUT  MEMORY_SUB_SYSTEM_CALLBACK  *MemorySubSystemCallback
  )
{
  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  ZeroMem (MemorySubSystemCallback, sizeof (MEMORY_SUB_SYSTEM_CALLBACK));
  MemorySubSystemHandle->Callback = MemorySubSystemCallback;

  MemorySubSystemCallback->TmeInit                 = TmeInit;
  MemorySubSystemCallback->SetMemoryMap            = SetMemoryMap;
  MemorySubSystemCallback->SetIaImrExclusionBelow4Gb = SetIaImrExclusionBelow4Gb;
  MemorySubSystemCallback->SetIaImrExclusionAbove4Gb = SetIaImrExclusionAbove4Gb;
  MemorySubSystemCallback->SetNocImrExclusion      = SetNocImrExclusion;
  MemorySubSystemCallback->SetVgaImrExclusion      = SetVgaImrExclusion;
  MemorySubSystemCallback->HostBridgeMemoryMapInit = HostBridgeMemoryMapInit;
  MemorySubSystemCallback->SetIGpuMemMap           = IGpuSetMemMap;
  MemorySubSystemCallback->ImrInit                 = ImrInit;
  MemorySubSystemCallback->NocInit                 = NocInit;
  MemorySubSystemCallback->CceInit                 = CceInit;
  MemorySubSystemCallback->AliasCheck              = AliasCheck;
  MemorySubSystemCallback->ClearSecretsBit         = ClearSecretsBit;
  MemorySubSystemCallback->SetPowerDownRequest     = SetPowerDownRequest;
  MemorySubSystemCallback->CpuSetPrmrrRegion       = PeiCpuSetPrmrrRegion;
  MemorySubSystemCallback->LockDpr                 = LockDpr;
  MemorySubSystemCallback->SetIaImrExclusionLock   = SetIaImrExclusionLock;
  MemorySubSystemCallback->LockHostBridgeMemoryMapRegs = LockHostBridgeMemoryMapRegs;
  MemorySubSystemCallback->SetMemConfigDone        = SetMemConfigDone;
  MemorySubSystemCallback->TdxInit                 = TdxInit;

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}