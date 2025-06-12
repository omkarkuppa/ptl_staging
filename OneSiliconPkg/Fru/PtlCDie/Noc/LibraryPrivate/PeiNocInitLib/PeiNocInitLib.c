/** @file
  PEIM Private Library to initialize NOC.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <Uefi.h>
#include <Register/CpuGenRegs.h>
#include <TprInfoHob.h>
#include <Library/PeiNguInitLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuRegbarAccessLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/IpuInitLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/PeiImrInitLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/NpuInfoLib.h>
#include <Library/IaxInfoLib.h>
#include <Library/VtdInfoLib.h>
#include <Register/HostBridgeRegs.h>
#include <CMrcInterface.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/NocRegs.h>
#include <Register/MemSSRegs.h>
#include <Library/IGpuInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/NguInfoLib.h>
#include <Ppi/SiPolicy.h>
#include <MemoryConfig.h>

#define DRAIN_CTL_OFFSET  0x3E0
#define DRAIN_CTL_REGISTER_ADDRESS(Sid, Pid, Offset) (PcdGet64(PcdSafBarBaseAddress) | ((UINT8)(Sid & 0x1F) << 20) | ((UINT8)(Pid) << 12) | (UINT16)(Offset))
#define MEM_1G                                 0x400

/**
  Init and Install TPR INFO Hob

  @retval EFI_SUCCESS - TPR HOB installed successfully.
**/
EFI_STATUS
EFIAPI
InstallTprInfoHob (
  VOID
  )
{
  TPR_INFO_HOB         *TprInfoHob;
  EFI_STATUS           Status;
  UINT32               MchBar;
  UINT8                TprDrainCtrlCount;

  TprDrainCtrlCount = 0;

  //
  // Get the MCHBAR value.
  //
  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  //
  // Create HOB for TPR INFO
  //
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (TPR_INFO_HOB),
             (VOID **) &TprInfoHob
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize default HOB data
  //
  TprInfoHob->EfiHobGuidType.Name = gTprInfoHobGuid;
  DEBUG ((DEBUG_INFO, "TprInfoHob->EfiHobGuidType.Name: %g\n", &TprInfoHob->EfiHobGuidType.Name));
  ZeroMem (&(TprInfoHob->Tpr0Base), sizeof (TPR_INFO_HOB) - sizeof (EFI_HOB_GUID_TYPE));

  DEBUG ((DEBUG_INFO, "TprInfoHob @ %X\n", TprInfoHob));
  DEBUG ((DEBUG_INFO, "TprInfoHob Size - HobHeaderSize: %X\n", sizeof (TPR_INFO_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "TprInfoHobSize: %X\n", sizeof (TPR_INFO_HOB)));

  TprInfoHob->Tpr0Base      = 0x0;
  TprInfoHob->Tpr0Limit     = 0x0;
  TprInfoHob->Tpr1Base      = 0x0;
  TprInfoHob->Tpr1Limit     = 0x0;
  TprInfoHob->InstanceCount = 0x1;
  TprInfoHob->InstanceArray[0].TprCount        = 0x2;
  TprInfoHob->InstanceArray[0].Tpr0BaseOffset  = MchBar + GET_NOC_EFFECTIVE_ADDRESS(IMR1M7BASE_IMPH_IOC_MCHBAR_REG);
  TprInfoHob->InstanceArray[0].Tpr0LimitOffset = MchBar + GET_NOC_EFFECTIVE_ADDRESS(IMR1M7BASE_IMPH_IOC_MCHBAR_REG) + RELATIVE_MASK_OFFSET;
  TprInfoHob->InstanceArray[0].Tpr1BaseOffset  = MchBar + GET_NOC_EFFECTIVE_ADDRESS(IMR1M8BASE_IMPH_IOC_MCHBAR_REG);
  TprInfoHob->InstanceArray[0].Tpr1LimitOffset = MchBar + GET_NOC_EFFECTIVE_ADDRESS(IMR1M8BASE_IMPH_IOC_MCHBAR_REG) + RELATIVE_MASK_OFFSET;

  if (IGpuIsSupported ()) {
    // Program GCD Drain Control Data into Hob.
    if (IGpuIsGtPresent ()) {
      TprDrainCtrlCount++;
      TprInfoHob->DrainControlArray.GtDrainControl = DRAIN_CTL_REGISTER_ADDRESS (CPU_SB_SID_GT_PB, CPU_SB_PID_GT_PB, DRAIN_CTL_OFFSET);
    }

    // Program Media Drain Control Data into Hob.
    if (IGpuIsMediaPresent ()) {
      TprDrainCtrlCount++;
      TprInfoHob->DrainControlArray.MediaDrainControl = DRAIN_CTL_REGISTER_ADDRESS (CPU_SB_SID_COMPUTE_MAIN, CPU_SB_PID_MEDIA_PB, DRAIN_CTL_OFFSET);
    }

    // Program Media Drain Control Data into Hob.
    if (IGpuIsDisplayPresent ()) {
      TprDrainCtrlCount++;
      TprInfoHob->DrainControlArray.DisplayDrainControl = DRAIN_CTL_REGISTER_ADDRESS (CPU_SB_SID_COMPUTE_MAIN, CPU_SB_PID_DNI2CFI, DRAIN_CTL_OFFSET);
    }
  }

  if (IsIaxEnabled ()) {
    TprDrainCtrlCount++;
    TprInfoHob->DrainControlArray.IaxDrainControl = DRAIN_CTL_REGISTER_ADDRESS (CPU_SB_SID_COMPUTE_MAIN, CPU_SB_PID_IAX_PB, DRAIN_CTL_OFFSET);
  }

  // Save IOC Drain Control Data into Hob.
  TprDrainCtrlCount++;
  TprInfoHob->DrainControlArray.IocDrainControl = DRAIN_CTL_REGISTER_ADDRESS (CPU_SB_SID_IOC_IVTU, CPU_SB_PID_IOC, DRAIN_CTL_OFFSET);

  if (IsVtdSupported ()) {
    TprDrainCtrlCount += 2;
    TprInfoHob->DrainControlArray.IvtuDrainControl = DRAIN_CTL_REGISTER_ADDRESS (CPU_SB_SID_IOC_IVTU, CPU_SB_PID_IVTU, DRAIN_CTL_OFFSET);
    TprInfoHob->DrainControlArray.SvtuDrainControl = DRAIN_CTL_REGISTER_ADDRESS (CPU_SB_SID_COMPUTE_MAIN, CPU_SB_PID_SVTU, DRAIN_CTL_OFFSET);
  }
  // Program IPU Drain Control Data into Hob.
  if (IsIpuEnabled ()) {
    TprDrainCtrlCount++;
    TprInfoHob->DrainControlArray.IpuDrainControl = DRAIN_CTL_REGISTER_ADDRESS (CPU_SB_SID_COMPUTE_MAIN, CPU_SB_PID_IPU_PB, DRAIN_CTL_OFFSET);
  }

  // Program NPU Drain Control Data into Hob.
  if (IsNpuEnabled ()) {
    TprDrainCtrlCount++;
    TprInfoHob->DrainControlArray.NpuDrainControl = DRAIN_CTL_REGISTER_ADDRESS (CPU_SB_SID_COMPUTE_MAIN, CPU_SB_PID_NPU_PB, DRAIN_CTL_OFFSET);
  }

  TprDrainCtrlCount++;
  TprInfoHob->DrainControlArray.PunitDrainControl = DRAIN_CTL_REGISTER_ADDRESS (CPU_SB_SID_COMPUTE_MAIN, CPU_SB_PID_PUNIT, DRAIN_CTL_OFFSET);;

  TprInfoHob->DrainControlCount = TprDrainCtrlCount;
  DEBUG ((DEBUG_INFO, "TPR INFO HOB installed\n"));

  return Status;
}

/**
  This function programs the MC Hash.

  @param[in] Mc0Size                  - Size of MC0
  @param[in] Mc1Size                  - Size of MC1
  @param[in] IsDdr5                   - Memory Type
  **/
VOID
EFIAPI
ProgramMcHash (
  IN UINT32     Mc0Size,
  IN UINT32     Mc1Size,
  IN BOOLEAN    IsDdr5
  )
{
  UINT32                               MchBar;
  UINT32                               McD0BaseAddress;
  MEMORY_SLICE_HASH_HBO_MEM_STRUCT     MemorySliceHash;
  LOCAL_HOME_SLICE_HASH_HBO_CFG_STRUCT LocalHomeSlice;
  SI_PREMEM_POLICY_PPI                 *SiPreMemPolicy;
  MEMORY_CONFIGURATION                 *MemConfig;
  MRC_EXT_INPUTS_TYPE                  *ExtInputs;
  EFI_STATUS                           Status;

  SiPreMemPolicy  = NULL;
  MemConfig       = NULL;

  DEBUG ((DEBUG_INFO,"Programming MC Hash\n"));
  DEBUG ((DEBUG_INFO,"Mc0Size %d\nMc1Size %d\n", Mc0Size, Mc1Size));


  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
  McD0BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
  LocalHomeSlice.Data = PciSegmentRead32 (McD0BaseAddress + LOCAL_HOME_SLICE_HASH_HBO_CFG_REG);
  MemorySliceHash.Data= MmioRead32 (MchBar + GET_HBO_EFFECTIVE_ADDRESS(MEMORY_SLICE_HASH_HBO_MEM_REG));

  MemorySliceHash.Bits.hash_enabled = FALSE;
  if (Mc0Size && Mc1Size) {
    MemorySliceHash.Bits.hash_enabled = TRUE;
    if (IsDdr5) {
      MemorySliceHash.Bits.hash_lsb = 0x3;
      MemorySliceHash.Bits.hash_mask = 0x2098;
    } else {
      MemorySliceHash.Bits.hash_lsb = 0x2;
      MemorySliceHash.Bits.hash_mask = 0x2094;
    }

    Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicy);
    if (!EFI_ERROR (Status) && (SiPreMemPolicy != NULL)) {
      Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gMemoryConfigGuid, (VOID *) &MemConfig);
      if (!EFI_ERROR (Status) && (MemConfig != NULL)) {
        ExtInputs = &MemConfig->ExternalInputs;
        if (ExtInputs->MsHashOverride) {
          MemorySliceHash.Bits.hash_lsb  = ExtInputs->MsHashInterleaveBit & ((1 << MEMORY_SLICE_HASH_HBO_MEM_HASH_LSB_SIZE) - 1);
          MemorySliceHash.Bits.hash_mask = ExtInputs->MsHashMask & ((1 << MEMORY_SLICE_HASH_HBO_MEM_HASH_MASK_SIZE) - 1);
          DEBUG ((DEBUG_INFO, "Override MemorySlice Hash LSB and MASK to %u and 0x%X\n", MemorySliceHash.Bits.hash_lsb, MemorySliceHash.Bits.hash_mask));
        }
      }
    }
    LocalHomeSlice.Bits.hash_bit0_lsb  = MemorySliceHash.Bits.hash_lsb;
    LocalHomeSlice.Bits.hash_bit0_mask = MemorySliceHash.Bits.hash_mask;

    if (Mc1Size > Mc0Size) {
      MemorySliceHash.Bits.slice_l_id = TRUE;
      MemorySliceHash.Bits.zone1_start = (Mc0Size * 2) >> 10; //Convert size into GB
    } else {
      // This configuration is for Symmetric Config Or (Mc1Size < Mc0Size)
      MemorySliceHash.Bits.zone1_start = (Mc1Size * 2) >> 10; //Convert size into GB
    }
  } else {
    if (Mc0Size) {
      MemorySliceHash.Bits.zone1_start = Mc0Size >> 10; //Convert size into GB
    }
    if (Mc1Size) {
      // slice_l_id is required only when single MC
      MemorySliceHash.Bits.slice_l_id = TRUE;
      MemorySliceHash.Bits.zone1_start = Mc1Size >> 10; //Convert size into GB
    }
  }

  MmioWrite32 (MchBar + GET_HBO_EFFECTIVE_ADDRESS(MEMORY_SLICE_HASH_HBO_MEM_REG), MemorySliceHash.Data);
  DEBUG ((DEBUG_INFO, "Memory Slice Hash after write= 0x%x\n", MmioRead32 (MchBar + GET_HBO_EFFECTIVE_ADDRESS(MEMORY_SLICE_HASH_HBO_MEM_REG))));
  PciSegmentWrite32 (McD0BaseAddress + LOCAL_HOME_SLICE_HASH_HBO_CFG_REG, LocalHomeSlice.Data);
  DEBUG ((DEBUG_INFO, "Local Home Slice Hash after write = 0x%x\n", PciSegmentRead32 (McD0BaseAddress + LOCAL_HOME_SLICE_HASH_HBO_CFG_REG)));
}

/**
  This function programs the Remap Base and Limit registers.

  @param[in] RemapBase  - Remap Base Address
  @param[in] RemapLimit - Remap Limit
**/
VOID
EFIAPI
ProgramRemapBaseLimit (
  IN UINT32   RemapBase,
  IN UINT32   RemapLimit
  )
{
  UINTN                        MchBar;
  REMAP_BASE_HBO_MEM_STRUCT    RemapBaseHbo;
  REMAP_LIMIT_HBO_MEM_STRUCT   RemapLimitHbo;

  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
  DEBUG ((DEBUG_INFO, "MchBar 0x%lx\n", MchBar));

  RemapBaseHbo.Data            = MmioRead64 (MchBar + GET_HBO_EFFECTIVE_ADDRESS (REMAP_BASE_HBO_MEM_REG));
  RemapBaseHbo.Bits.remapbase  = (UINT64) RemapBase;
  MmioWrite64 (MchBar + GET_HBO_EFFECTIVE_ADDRESS (REMAP_BASE_HBO_MEM_REG), RemapBaseHbo.Data);
  DEBUG ((DEBUG_INFO, "RemapBase 0x%lx\n", MmioRead64 (MchBar + GET_HBO_EFFECTIVE_ADDRESS (REMAP_BASE_HBO_MEM_REG))));

  RemapLimitHbo.Data           = MmioRead64 (MchBar + GET_HBO_EFFECTIVE_ADDRESS (REMAP_LIMIT_HBO_MEM_REG));
  RemapLimitHbo.Bits.remaplmt  = (UINT64) RemapLimit;
  MmioWrite64 (MchBar + GET_HBO_EFFECTIVE_ADDRESS (REMAP_LIMIT_HBO_MEM_REG), RemapLimitHbo.Data);
  DEBUG ((DEBUG_INFO, "RemapLimit 0x%lx\n", MmioRead64 (MchBar + GET_HBO_EFFECTIVE_ADDRESS (REMAP_LIMIT_HBO_MEM_REG))));
}

/**
  This function initialize NOC .

  @param[in] Mc0Size                  - Size of MC0
  @param[in] Mc1Size                  - Size of MC1
  @param[in] RemapBase                - Remap Base Address
  @param[in] RemapLimit               - Remap Limit
  @param[in] IsDdr5                   - Memory Type
**/
VOID
EFIAPI
NocInit (
  IN UINT32   Mc0Size,
  IN UINT32   Mc1Size,
  IN UINT32   RemapBase,
  IN UINT32   RemapLimit,
  IN BOOLEAN  IsDdr5
  )
{
  DEBUG ((DEBUG_INFO, "%a: Entry\n",__FUNCTION__));

  //
  // Program MC Hash
  //
  ProgramMcHash (Mc0Size, Mc1Size, IsDdr5);

  //
  // Program NOC Memory Remap Base and Limit registers.
  //
  ProgramRemapBaseLimit (RemapBase, RemapLimit);

  DEBUG((DEBUG_INFO, "%a: Exit\n", __FUNCTION__));
}
