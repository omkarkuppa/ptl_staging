/** @file
  PEIM Private Library to initialize PeiHostBridgeInitLib registers

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <Ppi/SiPolicy.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HobLib.h>
#include <HostBridgeDataHob.h>
#include <CMrcInterface.h>
#include <Register/SncuRegs.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/PeiHostBridgeInitLib.h>
#include <Register/MemSSRegs.h>
#include <Library/IGpuInfoLib.h>

/**
  Determine MMIO Size for Dynamic Tolud

  @param[in] PchPcieMmioLength - Total PCIe MMIO length on all PCH root ports
  @param[in] PegMmioLength     - Total PEG MMIO length on all PEG ports
  @param[in] IGpuMmioLength    - Total IGPU MMIO length
**/
VOID
MmioSizeCalculation (
  IN    UINT32  PchPcieMmioLength,
  IN    UINT32  PegMmioLength,
  IN    UINT32  IGpuMmioLength
  )
{
  HOST_BRIDGE_PREMEM_CONFIG     *HostBridgePreMemConfig;
  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi;
  UINT64                        TotalMmioLength;
  UINT32                        ResMemLimit1;
  UINT32                        AddMem;
  EFI_STATUS                    Status;

  ///
  /// Get policy settings through the SaInitConfigBlock PPI
  ///
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if ((Status != EFI_SUCCESS) || (SiPreMemPolicyPpi == NULL)) {
    DEBUG ((DEBUG_INFO, "Fail to locate SiPreMemPolicyPpi\n"));
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Determine MMIO Size for Dynamic Tolud
  ///
  TotalMmioLength = PchPcieMmioLength + PegMmioLength + IGpuMmioLength;
  DEBUG ((DEBUG_INFO, "TotalMmioLength:   0x%08X bytes\n", TotalMmioLength));
  if (HostBridgePreMemConfig->MmioSize == 0x00) {
    /**
      Dynamic MmioSize will calculate the value basing on MmioLength found from main consumers. (Video cards)
      Since PCIe and some other Chipset MMIO BAR occupied Top MMIO region, we need to enlarge MmioSize.
      By default 0xE0000000 ~ 0xFFFFFFFF are not available as MMIO resource so add 512MB in all cases.
      HostBridgePreMemConfig->MmioSizeAdjustment will be used later if needed to increase or decrease the final MmioSize.
    **/

    ///
    /// Get PCI Express Base address and calculate the memory occupied on Top MMIO region by that.
    ///
    ResMemLimit1 = PcdGet32 (PcdPciReservedMemLimit);
    if (ResMemLimit1 == 0) {
      ResMemLimit1 = (UINT32) PcdGet64 (PcdSiPciExpressBaseAddress);
    }

    AddMem = ((MAX_UINT32 - ResMemLimit1) + 1) >> 20;

    //
    // Total Mmio Size Calculation
    //
    HostBridgePreMemConfig->MmioSize = (UINT32) (RShiftU64 (TotalMmioLength, 20) + AddMem);

    //
    // Increase or Decrease MMIO size basing on platform requirement: MmioSizeAdjustment and it can be negative or positive.
    // If MmioSize not in reasonable range (Minimal should be larger than 0MB and Maximal 3840MB.), reset to safe default 1GB.
    //
    DEBUG ((DEBUG_INFO, "MmioSizeAdjustment = 0x%x\n", HostBridgePreMemConfig->MmioSizeAdjustment));
    HostBridgePreMemConfig->MmioSize = (INT32) (HostBridgePreMemConfig->MmioSize) + HostBridgePreMemConfig->MmioSizeAdjustment;
    DEBUG ((DEBUG_INFO, "MmioSize before alignment = 0x%x\n", HostBridgePreMemConfig->MmioSize));

    //
    // Do the Dynamic Calculation to allocate the required MMIO Size
    //
    HostBridgePreMemConfig->MmioSize = DynamicMmioCalculation (HostBridgePreMemConfig->MmioSize, HostBridgePreMemConfig->MmioSizeAdjustment);

    DEBUG ((DEBUG_INFO, "MmioSize after Dynamic calculation = 0x%x\n", HostBridgePreMemConfig->MmioSize));

    //
    // Align the MMIO Size to 256MB, so that it will consume lesser DRAM. HostBridge requires 256B alignment requirement.
    // if we set the alignment less than 256MB, it may collide with DRAM with some configurations.
    //
    HostBridgePreMemConfig->MmioSize = ALIGN_VALUE (HostBridgePreMemConfig->MmioSize, BASE_256MB >> 20);

    //
    // After Mmio Adjustment if the Size is 0 or higher than the available, allocate 2GB MMIO.
    // The minimum MMIO size should be 2GB so that it will not overlap with PCI Config Space.
    // if we allocate more, it will ended up with lesser DRAM which might cause system hang.
    //
    if ((HostBridgePreMemConfig->MmioSize == 0) || (HostBridgePreMemConfig->MmioSize > 0xF00) || (HostBridgePreMemConfig->MmioSize < 0x800)) {
      DEBUG ((DEBUG_INFO, "MmioSize = 0x%x is not reasonable, reset to default 2048MB!\n", HostBridgePreMemConfig->MmioSize));
      HostBridgePreMemConfig->MmioSize = 0x800;
    }
    DEBUG ((DEBUG_INFO, "MmioSize after alignment = 0x%x\n", HostBridgePreMemConfig->MmioSize));
  }
}

/**
  This function program TOLUD register

  @param[in] TouldBase
**/
VOID
ProgramTolud (
  UINT32  ToludBase
  )
{
  UINT64    McBaseAddress;

  McBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);

  DEBUG ((DEBUG_INFO, "Tolud = %x\n", ToludBase));

  PciSegmentWrite32 (McBaseAddress + TOLUD_HOSTBRIDGE_CFG_REG, (ToludBase << TOLUD_HOSTBRIDGE_CFG_TOLUD_LSB));
}

/**
  This function program TOUUD register

  @param[in] TouudBase
**/
VOID
ProgramTouud (
  UINT32  TouudBase
  )
{
  UINT64    Touud;
  UINT64    McBaseAddress;

  McBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);

  DEBUG ((DEBUG_INFO, "Touud = %x\n", TouudBase));

  Touud = LShiftU64 (TouudBase, TOUUD_HOSTBRIDGE_CFG_TOUUD_LSB);
  PciSegmentWrite32 (McBaseAddress +  TOUUD_HOSTBRIDGE_CFG_REG    , (UINT32)Touud);
  PciSegmentWrite32 (McBaseAddress +  TOUUD_HOSTBRIDGE_CFG_REG + 4, (UINT32)RShiftU64 (Touud, 32));
}

/**
  This function program Tom register

  @param[in] TouudBase
**/
VOID
ProgramTom (
  UINT32  TotalMemorySize
  )
{
  UINT64    Tom;
  UINT64    McBaseAddress;

  McBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
  DEBUG ((DEBUG_INFO, "TotalMemorySize = %x\n", TotalMemorySize));

  Tom = LShiftU64 (TotalMemorySize, TOM_HOSTBRIDGE_CFG_TOM_LSB);
  PciSegmentWrite32 (McBaseAddress +  TOM_HOSTBRIDGE_CFG_REG    , (UINT32) Tom);
  PciSegmentWrite32 (McBaseAddress +  TOM_HOSTBRIDGE_CFG_REG + 4, (UINT32) RShiftU64 (Tom, 32));
}

/**
  This function program Tseg register

  @param[in] TsegBase
**/
VOID
ProgramTseg (
  UINT32  TsegBase
  )
{
  UINT64    McBaseAddress;

  McBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);

  DEBUG ((DEBUG_INFO, "TsegBase = %x\n", TsegBase));

  PciSegmentWrite32 (McBaseAddress + TSEGMB_HOSTBRIDGE_CFG_REG, (TsegBase << TSEGMB_HOSTBRIDGE_CFG_TSEGMB_LSB));
}

/**
  This function program BDSM register

  @param[in] BdsmBase
**/
VOID
ProgramBdsm (
  UINT32  BdsmBase
  )
{
  UINT64    McBaseAddress;

  McBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);

  DEBUG ((DEBUG_INFO, "BdsmBase = %x\n", BdsmBase));

  PciSegmentWrite32 (McBaseAddress + BDSM_HOSTBRIDGE_CFG_REG, (BdsmBase << BDSM_HOSTBRIDGE_CFG_BDSM_LSB));
}

/**
  This function program BGSM register

  @param[in] BgsmBase
**/
VOID
ProgramBgsm (
  UINT32  BgsmBase
  )
{
  UINT64    McBaseAddress;

  McBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);

  DEBUG ((DEBUG_INFO, "BgsmBase = %x\n", BgsmBase));

  PciSegmentWrite32 (McBaseAddress + BGSM_HOSTBRIDGE_CFG_REG, (BgsmBase << BGSM_HOSTBRIDGE_CFG_BGSM_LSB));
}

/**
  This function program Graphic Control register
**/
VOID
ProgramGfxCr (
  VOID
  )
{
  UINT64    McBaseAddress;
  UINT16    IGpuControlRegister;
  UINT32    GraphicsGttSize;
  UINT32    GraphicsStolenSize;

  GraphicsGttSize    = (IGpuGetGsmSizeInBytes () >> 20);
  GraphicsStolenSize = (IGpuGetDsmSizeInBytes () >> 20);

  IGpuControlRegister = UpdateGraphicsController (GraphicsGttSize, GraphicsStolenSize);

  McBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
  DEBUG ((DEBUG_INFO, "IGpuControlRegister = %x\n", IGpuControlRegister));

  PciSegmentWrite16 (McBaseAddress + GGC_HOSTBRIDGE_CFG_REG, IGpuControlRegister);
}

/**
  This function Lock Memory Map registers
**/
VOID
LockHostBridgeMemoryMapRegs (
  VOID
  )
{
  UINT64                       McBaseAddress;
  TSEGMB_HOSTBRIDGE_CFG_STRUCT TsegmbCfg;
  BGSM_HOSTBRIDGE_CFG_STRUCT   BgsmCfg;
  BDSM_HOSTBRIDGE_CFG_STRUCT   BdsmCfg;
  TOUUD_HOSTBRIDGE_CFG_STRUCT  TouudCfg;
  TOLUD_HOSTBRIDGE_CFG_STRUCT  ToludCfg;
  GGC_HOSTBRIDGE_CFG_STRUCT    GgcCfg;
  TOM_HOSTBRIDGE_CFG_STRUCT    TomCfg;

  McBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);

  //
  // Lock TOLUD
  //
  ToludCfg.Data = PciSegmentRead32 (McBaseAddress + TOLUD_HOSTBRIDGE_CFG_REG);
  ToludCfg.Bits.lock = TRUE;
  PciSegmentWrite32 (McBaseAddress + TOLUD_HOSTBRIDGE_CFG_REG, ToludCfg.Data);
  //
  // Lock TOUUD
  //
  TouudCfg.Data = PciSegmentRead32 (McBaseAddress + TOUUD_HOSTBRIDGE_CFG_REG);
  TouudCfg.Bits.lock = TRUE;
  PciSegmentWrite32 (McBaseAddress + TOUUD_HOSTBRIDGE_CFG_REG, (UINT32) TouudCfg.Data);
  //
  // Lock Tom
  //
  TomCfg.Data = PciSegmentRead32 (McBaseAddress + TOM_HOSTBRIDGE_CFG_REG);
  TomCfg.Bits.lock = TRUE;
  PciSegmentWrite32 (McBaseAddress + TOM_HOSTBRIDGE_CFG_REG, (UINT32) TomCfg.Data);
  //
  // Lock Tseg
  //
  TsegmbCfg.Data = PciSegmentRead32 (McBaseAddress + TSEGMB_HOSTBRIDGE_CFG_REG);
  TsegmbCfg.Bits.lock = TRUE;
  PciSegmentWrite32 (McBaseAddress + TSEGMB_HOSTBRIDGE_CFG_REG, TsegmbCfg.Data);
  //
  // Lock Bdsm
  //
  BdsmCfg.Data = PciSegmentRead32 (McBaseAddress + BDSM_HOSTBRIDGE_CFG_REG);
  BdsmCfg.Bits.lock = TRUE;
  PciSegmentWrite32 (McBaseAddress + BDSM_HOSTBRIDGE_CFG_REG, BdsmCfg.Data);
  //
  // Lock Bgsm
  //
  BgsmCfg.Data = PciSegmentRead32 (McBaseAddress + BGSM_HOSTBRIDGE_CFG_REG);
  BgsmCfg.Bits.lock = TRUE;
  PciSegmentOr32 (McBaseAddress + BGSM_HOSTBRIDGE_CFG_REG, BgsmCfg.Data);
  //
  // Lock Graphic Control register
  //
  GgcCfg.Data = PciSegmentRead16 (McBaseAddress + GGC_HOSTBRIDGE_CFG_REG);
  GgcCfg.Bits.ggclck = TRUE;
  PciSegmentOr16 (McBaseAddress + GGC_HOSTBRIDGE_CFG_REG, GgcCfg.Data);
}


/**
  Init and Install Host Bridge Data Hob

  @param[in] HOST_BRIDGE_PREMEM_CONFIG   - Instance of HOST_BRIDGE_PREMEM_CONFIG
  @param[out] HOST_BRIDGE_DATA_HOB       - HOST_BRIDGE_DATA_HOB instance installed by this function

  @retval EFI_SUCCESS
**/
EFI_STATUS
InstallHostBridgeDataHob (
  IN    HOST_BRIDGE_PREMEM_CONFIG   *HostBridgePreMemConfig,
  OUT   HOST_BRIDGE_DATA_HOB        **HostBridgeDataHobOut
  )
{
  HOST_BRIDGE_DATA_HOB      *HostBridgeDataHob;
  EFI_STATUS                 Status;

  //
  // Create HOB for Host Bridge INFO
  //
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (HOST_BRIDGE_DATA_HOB),
             (VOID **) &HostBridgeDataHob
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize default HOB data
  //
  CopyGuid (&HostBridgeDataHob->EfiHobGuidType.Name, &gHostBridgeDataHobGuid);
  DEBUG ((DEBUG_INFO, "HostBridgeDataHob->EfiHobGuidType.Name: %g\n", &HostBridgeDataHob->EfiHobGuidType.Name));
  ZeroMem (&(HostBridgeDataHob->EnableAbove4GBMmio), sizeof (HOST_BRIDGE_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE));

  DEBUG ((DEBUG_INFO, "HostBridgeDataHob @ %X\n", HostBridgeDataHob));
  DEBUG ((DEBUG_INFO, "HostBridgeDataHob Size - HobHeaderSize: %X\n", sizeof (HOST_BRIDGE_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "HostBridgeDataHobSize: %X\n", sizeof (HOST_BRIDGE_DATA_HOB)));

  HostBridgeDataHob->EnableAbove4GBMmio = (UINT8) HostBridgePreMemConfig->EnableAbove4GBMmio;
  DEBUG ((DEBUG_INFO, "HostBridgeDataHob->EnableAbove4GBMmio: %X\n", HostBridgeDataHob->EnableAbove4GBMmio));

  HostBridgeDataHob->CridEnable = (BOOLEAN) (UINTN) HostBridgePreMemConfig->CridEnable;

  (*HostBridgeDataHobOut) = HostBridgeDataHob;

  DEBUG ((DEBUG_INFO, "Host Bridge Data HOB installed\n"));

  return EFI_SUCCESS;
}

/**
  Update HostBridge Hob in PostMem

  @param[in] HostBridgePeiConfig    Instance of HOST_BRIDGE_PEI_CONFIG

  @retval EFI_SUCCESS
**/
EFI_STATUS
UpdateHostBridgeHobPostMem (
  IN    HOST_BRIDGE_PEI_CONFIG      *HostBridgePeiConfig
)
{
  HOST_BRIDGE_DATA_HOB        *HostBridgeDataHob;

  ///
  /// Locate and update Host Bridge Hob Data
  ///
  HostBridgeDataHob = (HOST_BRIDGE_DATA_HOB *) GetFirstGuidHob (&gHostBridgeDataHobGuid);
  if (HostBridgeDataHob != NULL) {
     HostBridgeDataHob->SkipPamLock = (BOOLEAN) (UINTN) HostBridgePeiConfig->SkipPamLock;
  }
  return EFI_SUCCESS;
}

/**
  This function program Mem Subsystem Config register

  @param[in] Mc0Size - Size of MC0
  @param[in] Mc1Size - Size of MC1
**/
VOID
ProgramMemSsConfig (
  IN UINT32     Mc0Size,
  IN UINT32     Mc1Size
  )
{
  MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_STRUCT    MemSsPmaCrMemConfig;
  UINT64                                    MchBar;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  MemSsPmaCrMemConfig.Data = MmioRead32 ((UINTN) (MchBar + MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_REG));
  if (((Mc0Size && Mc1Size) == 0) || (Mc0Size == Mc1Size)) {
    MemSsPmaCrMemConfig.Bits.l_shape_config = 0;
  } else {
    MemSsPmaCrMemConfig.Bits.l_shape_config = 1;
  }
  MmioWrite32 (MchBar + MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_REG, MemSsPmaCrMemConfig.Data);
  DEBUG ((DEBUG_INFO, "l_shape_config Mc0Size = %X Mc1Size = %X MemSsPmaCrMemConfig =%x\n", Mc0Size,Mc1Size,(MmioRead32 ((UINTN) (MchBar + MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_REG)))));
}
/**
  This function calls into MemoryMap APIs to program memory Map.

  @param[in] TotalPhysicalMemorySize  - DRAM Size
  @param[in] ToludBase                - Tolud Base Address
  @param[in] TouudBase                - Touud Base Address
  @param[in] TsegBase                 - Tseg Base Address
  @param[in] BdsmBase                 - BDSM Base Address
  @param[in] GttBase                  - GTT Base Address
  @param[in] Mc0Size                  - Size of MC0
  @param[in] Mc1Size                  - Size of MC1

**/
VOID
HostBridgeMemoryMapInit (
  IN UINT32     TotalPhysicalMemorySize,
  IN UINT32     ToludBase,
  IN UINT32     TouudBase,
  IN UINT32     TsegBase,
  IN UINT32     BdsmBase,
  IN UINT32     GttBase,
  IN UINT32     Mc0Size,
  IN UINT32     Mc1Size
  )
{
  DEBUG ((DEBUG_INFO,"%a Start\n", __FUNCTION__));

  ProgramTom (TotalPhysicalMemorySize);
  //
  // Write TOLUD register
  //
  ProgramTolud (ToludBase);
  //
  // Write TOUUD register
  //
  ProgramTouud (TouudBase);
  //
  // Write TSEG register
  //
  ProgramTseg (TsegBase);
  //
  // Write BDSM register
  //
  ProgramBdsm (BdsmBase);
  //
  // Write BGSM register
  //
  ProgramBgsm (GttBase);
  //
  // Write graphics control register
  //
  ProgramGfxCr ();
  //
  //  Write MemSs config register
  //
  ProgramMemSsConfig (Mc0Size, Mc1Size);
  DEBUG ((DEBUG_INFO,"%a End\n", __FUNCTION__));
}

/**
  This function will return MmioSize.

  @retval UINT32 returns MmioSize
**/
UINT32
GetPreMemMmioSize (
  VOID
  )
{
  HOST_BRIDGE_PREMEM_CONFIG     *HostBridgePreMemConfig;
  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi;
  EFI_STATUS                    Status;

  ///
  /// Get policy settings through the SaInitConfigBlock PPI
  ///
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if ((Status != EFI_SUCCESS) || (SiPreMemPolicyPpi == NULL)) {
    DEBUG ((DEBUG_INFO, "Fail to locate SiPreMemPolicyPpi\n"));
    return 0;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  if (HostBridgePreMemConfig == NULL) {
    DEBUG ((DEBUG_INFO, "Fail to locate HostBridgePreMemConfig\n"));
    return 0;   
  }

  return HostBridgePreMemConfig->MmioSize;
}
