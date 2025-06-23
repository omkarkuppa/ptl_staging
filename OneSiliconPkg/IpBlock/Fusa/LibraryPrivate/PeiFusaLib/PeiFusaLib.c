/** @file
  Initilize FUSA device in PEI

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
#include <Library/DebugLib.h>
#include <Register/SaRegsHostBridge.h>
#include <Ppi/SiPolicy.h>
#include <FusaConfig.h>
#include <Register/FusaRegs.h>
#include <Library/PciSegmentLib.h>
#include <Library/IoLib.h>
#include <Register/CpuGenRegs.h>
#include <Register/CpuPsfRegs.h>
#include <Library/CpuRegbarAccessLib.h>
#include <Register/GenerationMsr.h>
#include <Library/PeiFusaLib.h>
#include <Library/IGpuInfoLib.h>

typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_STRUCT;

/**
  Lock FUSA Registers After BIOS_DONE.
**/
VOID
FusaRegisterLock (
  VOID
  )
{
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  UINT32      Data32;
  UINT64      McD2BaseAddress;
  UINTN       GttMmAdr;

  GttMmAdr = IGpuGetGttMmAdrBase ();

  //lock the FUSA lock bit
  MmioOr32 (GttMmAdr + R_SA_GTMMADR_DE_FUSA_PARITY_CTRL_OFFSET, B_SA_GTMMADR_DE_FUSA_IOSF_PARITY_CTL_LOCK_BIT);
  Data32 = MmioRead32(GttMmAdr + R_SA_GTMMADR_DE_FUSA_PARITY_CTRL_OFFSET);
  DEBUG ((DEBUG_INFO, "Lock FUSA Display Lock Bit 0x%x\n", Data32));

  MmioOr32 (GttMmAdr + R_SA_GTMMADR_SF_CTL_0, B_SA_GTMMADR_DE_FUSA_SF_CTL_LOCK_BIT);
  Data32 = MmioRead32(GttMmAdr + R_SA_GTMMADR_SF_CTL_0);
  DEBUG ((DEBUG_INFO, "Lock FUSA Graphics Lock Bit 0x%x\n", Data32));
#endif
}

/**
  FusaOverrideProgramming: Override the Display FUSA register for enabling/disabling FUSA features

  @param[in] FUSA_CONFIG        The FUSA Policy PPI instance
  @retval     EFI_SUCCESS     - Display workarounds done
**/
EFI_STATUS
DisplayFusaOverrideProgramming (
   IN  FUSA_CONFIG         *FusaConfig,
   IN  UINTN               GttMmAdr
  )
{
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  UINT32                  Data32;

  if (FusaConfig->DisplayFusaConfigEnable == 1){
    DEBUG ((DEBUG_INFO, "Enable Fusa on Display\n"));

    Data32 = ( B_SA_GTMMADR_DE_FUSA_IOSF_PARITY_CMD_BIT | B_SA_GTMMADR_DE_FUSA_IOSF_PARITY_DATA_BIT | B_SA_GTMMADR_DE_FUSA_DIP_PARITY_CMD_BIT | B_SA_GTMMADR_DE_FUSA_DIP_PARITY_DATA_BIT);
    MmioOr32 (GttMmAdr + R_SA_GTMMADR_DE_FUSA_PARITY_CTRL_OFFSET, Data32);

    Data32 = B_SA_GTMMADR_DISPLAY_ERROR_MASK;
    MmioOr32 (GttMmAdr + R_SA_GTMMADR_DISPLAY_ERR_COR_OFFSET, Data32);
    MmioOr32 (GttMmAdr + R_SA_GTMMADR_DISPLAY_ERR_NONFATAL_OFFSET, Data32);
    MmioOr32 (GttMmAdr + R_SA_GTMMADR_DISPLAY_ERR_FATAL_OFFSET, Data32);
  } else {
    DEBUG ((DEBUG_INFO, "Disable Fusa on Display\n"));
    Data32 = (UINT32)~( B_SA_GTMMADR_DE_FUSA_IOSF_PARITY_CMD_BIT | B_SA_GTMMADR_DE_FUSA_IOSF_PARITY_DATA_BIT | B_SA_GTMMADR_DE_FUSA_DIP_PARITY_CMD_BIT | B_SA_GTMMADR_DE_FUSA_DIP_PARITY_DATA_BIT);
    MmioAnd32(GttMmAdr + R_SA_GTMMADR_DE_FUSA_PARITY_CTRL_OFFSET, Data32);

    Data32 = ~(B_SA_GTMMADR_DISPLAY_ERROR_MASK);
    MmioAnd32 (GttMmAdr + R_SA_GTMMADR_DISPLAY_ERR_COR_OFFSET, Data32);
    MmioAnd32 (GttMmAdr + R_SA_GTMMADR_DISPLAY_ERR_NONFATAL_OFFSET, Data32);
    MmioAnd32 (GttMmAdr + R_SA_GTMMADR_DISPLAY_ERR_FATAL_OFFSET, Data32);
  }

#endif
  return EFI_SUCCESS;
}

/**
  FusaOverrideProgramming: Override the Graphics FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - Graphics workarounds done
**/
EFI_STATUS
GraphicsFusaOverrideProgramming (
   IN  FUSA_CONFIG         *FusaConfig,
   IN  UINTN               GttMmAdr
  )
{
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  UINT32                  Data32;

  if (FusaConfig->GraphicFusaConfigEnable == 1){
    DEBUG ((DEBUG_INFO, "Enable Fusa on Graphics\n"));
    Data32 = B_SA_GTMMADR_GT_ERROR_MASK;
    MmioOr32 (GttMmAdr + R_SA_GTMMADR_GT_ERR_COR_OFFSET, Data32);
    MmioOr32 (GttMmAdr + R_SA_GTMMADR_GT_ERR_NONFATAL_OFFSET, Data32);
    MmioOr32 (GttMmAdr + R_SA_GTMMADR_GT_ERR_FATAL_OFFSET, Data32);
  }
  else{
    DEBUG ((DEBUG_INFO, "Disable Fusa on Graphics\n"));
    Data32 = ~(B_SA_GTMMADR_GT_ERROR_MASK);
    MmioAnd32 (GttMmAdr + R_SA_GTMMADR_GT_ERR_COR_OFFSET, Data32);
    MmioAnd32 (GttMmAdr + R_SA_GTMMADR_GT_ERR_NONFATAL_OFFSET, Data32);
    MmioAnd32 (GttMmAdr + R_SA_GTMMADR_GT_ERR_FATAL_OFFSET, Data32);
  }

#endif
  return EFI_SUCCESS;
}

/**
  FusaOverrideProgramming: Override the IGD FUSA registers for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - IGD workarounds done
**/
EFI_STATUS
IgdFusaOverrideProgramming (
   IN  SI_POLICY_PPI    *SiPolicy
  )
{
  #if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
    UINT64                  McD2BaseAddress;
    UINTN                   GttMmAdr;
    EFI_STATUS              Status;
    FUSA_CONFIG             *FusaConfig;

    Status = GetConfigBlock ((VOID *) SiPolicy, &gFusaConfigGuid, (VOID *) &FusaConfig);
    ASSERT_EFI_ERROR (Status);

    GttMmAdr = IGpuGetGttMmAdrBase ();

    // Enable error reporting for Fusa Graphic or Display
    if (FusaConfig->GraphicFusaConfigEnable == 1 || FusaConfig->DisplayFusaConfigEnable == 1) {
      PciSegmentOr32 (McD2BaseAddress + R_SA_PCI_DEVICECTL_OFFSET, ( BIT2 | BIT1 | BIT0));
    } else {
      PciSegmentAnd32 (McD2BaseAddress + R_SA_PCI_DEVICECTL_OFFSET, (UINT32)~( BIT2 | BIT1 | BIT0));
    }

    GraphicsFusaOverrideProgramming (FusaConfig, GttMmAdr);
    DisplayFusaOverrideProgramming (FusaConfig, GttMmAdr);

  #endif
  return EFI_SUCCESS;
}

/**
  FusaOverrideProgramming: Override the Opio FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - Display workarounds done
**/
EFI_STATUS
OpioFusaOverrideProgramming (
   IN  SI_POLICY_PPI    *SiPolicy
  )
{
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  UINT32                      Data32;
  UINT64_STRUCT               DmiBar;
  UINT64                      McD0BaseAddress;
  FUSA_CONFIG                 *FusaConfig;
  EFI_STATUS                  Status;
  UINTN                       DmiBarAddress;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFusaConfigGuid, (VOID *) &FusaConfig);
  ASSERT_EFI_ERROR (Status);

  McD0BaseAddress    = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, 0);
  DmiBar.Data32.High = PciSegmentRead32 (McD0BaseAddress + R_SA_DMIBAR + 4);
  DmiBar.Data32.Low  =  PciSegmentRead32 (McD0BaseAddress + R_SA_DMIBAR);
  DmiBar.Data       &= (UINT64) ~BIT0;

  DmiBarAddress = (UINTN)DmiBar.Data;

  DEBUG ((DEBUG_INFO, "OPIO FUSA override Fuction \n"));
  if (FusaConfig-> OpioFusaConfigEnable == 1){
    DEBUG ((DEBUG_INFO, "Enable Fusa on Opio\n"));
    Data32 = ( B_SA_DMIBAR_PARITY_GEN_EN | B_SA_DMIBAR_DATA_PARITY_FATAL_CFG | B_SA_DMIBAR_DATA_PARITY_FATAL_OPI_CFG);
    MmioOr32 (DmiBarAddress+ R_SA_DMIBAR_FUSA_E2EPARITY_CTL_OFFSET, Data32);
  }
  else{
    DEBUG ((DEBUG_INFO, "Disable Fusa on Opio\n"));
    Data32 = ~( B_SA_DMIBAR_PARITY_GEN_EN | B_SA_DMIBAR_DATA_PARITY_FATAL_CFG | B_SA_DMIBAR_DATA_PARITY_FATAL_OPI_CFG);
    MmioAnd32 ((DmiBarAddress + R_SA_DMIBAR_FUSA_E2EPARITY_CTL_OFFSET), Data32);
  }
#endif
  return EFI_SUCCESS;
}

/**
  FusaOverrideProgramming: Override the Psf FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - Psf workarounds done
**/
EFI_STATUS
PsfFusaOverrideProgramming (
  IN  SI_POLICY_PPI    *SiPolicy
  )
{
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  FUSA_CONFIG                 *FusaConfig;
  EFI_STATUS                  Status;
  UINT16                      OrData;
  UINT16                      AndData;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFusaConfigGuid, (VOID *) &FusaConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "PSF CPU FUSA override Function \n"));
  DEBUG ((DEBUG_INFO, "Read Default PSF Config = %04x\n", CpuRegbarRead16 (SOC_DIE, CPU_SB_PID_PSF0, 0, R_SA_PSF_GLOBAL_CONFIG)));
  DEBUG ((DEBUG_INFO, "Read Default PSF Config = %04x\n", CpuRegbarRead16 (SOC_DIE, CPU_SB_PID_PSF1, 0, R_SA_PSF_GLOBAL_CONFIG)));

  if (FusaConfig-> PsfFusaConfigEnable == 1){
    DEBUG ((DEBUG_INFO, "Enable Fusa on PSF \n"));
    OrData = (UINT16)(B_SA_PSF_GLOBAL_CONFIG_ENCMDPARDETEGRESS | B_SA_PSF_GLOBAL_CONFIG_ENCMDPARDETINGRESS);
    CpuRegbarOr16 (SOC_DIE, CPU_SB_PID_PSF0, 0, R_SA_PSF_GLOBAL_CONFIG , OrData);
    CpuRegbarOr16 (SOC_DIE, CPU_SB_PID_PSF1, 0, R_SA_PSF_GLOBAL_CONFIG , OrData);
    DEBUG ((DEBUG_INFO, "Read After PSF Config = %04x\n", CpuRegbarRead16 (SOC_DIE, CPU_SB_PID_PSF0, 0, R_SA_PSF_GLOBAL_CONFIG)));
    DEBUG ((DEBUG_INFO, "Read After PSF Config = %04x\n", CpuRegbarRead16 (SOC_DIE, CPU_SB_PID_PSF1, 0, R_SA_PSF_GLOBAL_CONFIG)));
  } else {
    AndData = (UINT16)~(B_SA_PSF_GLOBAL_CONFIG_ENCMDPARDETEGRESS | B_SA_PSF_GLOBAL_CONFIG_ENCMDPARDETINGRESS);
    DEBUG ((DEBUG_INFO, "Disable Fusa on PSF \n"));
    CpuRegbarAnd16 (SOC_DIE, CPU_SB_PID_PSF0, 0, R_SA_PSF_GLOBAL_CONFIG  , AndData);
    CpuRegbarAnd16 (SOC_DIE, CPU_SB_PID_PSF1, 0, R_SA_PSF_GLOBAL_CONFIG  , AndData);
  }
#endif
  return EFI_SUCCESS;
}

/**
  FusaOverrideProgramming: Override the IOP FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - IOP workarounds done
**/
EFI_STATUS
IopFusaOverrideProgramming (
  IN SI_POLICY_PPI *SiPolicy
  )
{
  FUSA_CONFIG                 *FusaConfig;
  EFI_STATUS                  Status;
  UINT64                      McD0BaseAddress;
  UINT64_STRUCT               MchBar;
  UINT32                      Data32;
  UINTN                       MchBarAddress;

  McD0BaseAddress    = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
  MchBar.Data32.High = PciSegmentRead32 (McD0BaseAddress + R_SA_MCHBAR + 4);
  MchBar.Data32.Low  = PciSegmentRead32 (McD0BaseAddress + R_SA_MCHBAR);
  MchBar.Data       &= (UINT64) ~BIT0;
  MchBarAddress = (UINTN) MchBar.Data;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gFusaConfigGuid, (VOID *) &FusaConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "IOP FUSA override Function \n"));

  DEBUG ((DEBUG_INFO, "PsfFusaConfigEnable %x DisplayFusaConfigEnable %x GraphicFusaConfigEnable %x OpioFusaConfigEnable %x IopFusaConfigEnable %x IopFusaMcaReporting %x\n", FusaConfig->PsfFusaConfigEnable, FusaConfig->DisplayFusaConfigEnable,
  FusaConfig->GraphicFusaConfigEnable, FusaConfig->OpioFusaConfigEnable, FusaConfig->IopFusaConfigEnable, FusaConfig->IopFusaMcaCheckEnable));

  if ((FusaConfig->PsfFusaConfigEnable == 1) || (FusaConfig->DisplayFusaConfigEnable == 1)
        || (FusaConfig->OpioFusaConfigEnable == 1) || (FusaConfig->IopFusaConfigEnable == 1) || (FusaConfig->IopFusaMcaCheckEnable == 1)) {
    Data32 = B_SA_MCHBAR_FUSA_MCA_REPORTING_EN_0_0_0_MCHBAR_IMPH_MCA_REPORTING_EN;
    MmioOr32 (MchBarAddress + R_MCH_IMPH_FUSA_MCA_REPORTING, Data32);
  } else {
    Data32 = (UINT16)~(B_MCH_IMPH_FUSA_MCA_REPORTING_EN);
    MmioAnd32 (MchBarAddress + R_MCH_IMPH_FUSA_MCA_REPORTING, Data32);
  }
  return EFI_SUCCESS;
}

/**
  Check is the silicon is supporting fusa.

  @retval BOOLEAN TRUE/FALSE
**/
BOOLEAN
IsFusaSupported (
  VOID
  )
{
  MSR_CORE_CAPABILITIES_REGISTER                Msr;

  Msr.Uint64 = AsmReadMsr64 (MSR_CORE_CAPABILITIES);
  if (Msr.Bits.FusaSupported) {
    DEBUG ((DEBUG_INFO, "FuSA supported sku detected\n"));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, "non-FuSA supported sku detected\n"));
    return FALSE;
  }
}
