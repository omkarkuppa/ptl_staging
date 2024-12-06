/** @file
  Library to initialize PeiHostBridgeIpStatusLib

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

#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/CpuPlatformLib.h>
#include <Library/BaseLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/IoLib.h>
#include <Register/HostBridgeRegs.h>
#include <Library/PmcLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/SncuRegs.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <TcssInfo.h>
#include <Library/PcdInfoLib.h>

/**
  This function is to update DEVEN Register for Uncore IPs.

  @param[in] Ip       - Instance of UNCORE_IP
  @param[in] Instance - Instance of UNCORE_IP_INSTANCE
  @param[in] Status   - Instance of UNCORE_IP_STATUS

**/
VOID
EFIAPI
UncoreDevEnWrite (
  IN UNCORE_IP           Ip,
  IN UNCORE_IP_INSTANCE  Instance,
  IN UNCORE_IP_STATUS    Status
  )
{
  DEVEN_HOSTBRIDGE_CFG_STRUCT DevenCfg;
  DevenCfg.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS(SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, DEVEN_HOSTBRIDGE_CFG_REG));
  switch (Ip) {
    case Graphics:
        DevenCfg.Bits.ig_en = Status;
      break;
    case Npu:
        DevenCfg.Bits.npu_en = Status;
      break;
    case Ipu:
        DevenCfg.Bits.imgu_en = Status;
      break;
    case Iax:
        DevenCfg.Bits.iax_en = Status;
      break;
    case Vmd:
        DevenCfg.Bits.vmd_en = Status;
      break;
    case Camarillo:
        DevenCfg.Bits.cd_en = Status;
      break;
    case Telemetry:
        DevenCfg.Bits.crashlog_en = Status;
      break;
    default:
      DEBUG ((DEBUG_INFO, "Invalid IP Name\n"));
      ASSERT(FALSE);
  }
  PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, DEVEN_HOSTBRIDGE_CFG_REG), DevenCfg.Data);
}

/**
  This function is to Read Cap ID Register for Uncore IPs.

  @param[in] Ip       - Instance of UNCORE_IP
  @param[in] Instance - Instance of UNCORE_IP_INSTANCE

  @retval FALSE       - Uncore Ip is fused off
  @retval TRUE        - Uncore Ip is supported by Sillicon

**/
BOOLEAN
EFIAPI
UncoreCapIdRead (
  IN UNCORE_IP           Ip,
  IN UNCORE_IP_INSTANCE  Instance
  )
{
  CAPID0_A_HOSTBRIDGE_CFG_STRUCT        Capid0A;
  CAPID_DEV_CDIE_HOSTBRIDGE_CFG_STRUCT  Capid0DevCdie;
  CAPID0_B_HOSTBRIDGE_CFG_STRUCT        Capid0B;

  Capid0A.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, CAPID0_A_HOSTBRIDGE_CFG_REG));
  Capid0DevCdie.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, CAPID_DEV_CDIE_HOSTBRIDGE_CFG_REG));
  Capid0B.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, CAPID0_B_HOSTBRIDGE_CFG_REG));

  switch (Ip) {
    case Npu:
      if (Capid0DevCdie.Bits.npu_dis) {
        return FALSE;
      }
      break;
    case Iax:
      if (Capid0DevCdie.Bits.iax_dis) {
        return FALSE;
      }
      break;
    case Vmd:
      if (Capid0DevCdie.Bits.vmd_dis) {
        return FALSE;
      }
      break;
    case Ipu:
      if (Capid0DevCdie.Bits.imgu_dis) {
        return FALSE;
      }
      break;
    case Vtd:
      if (Capid0A.Bits.vtdd) {
        return FALSE;
      }
      break;
    case Adm:
      if (Capid0B.Bits.adm_tech == 0) {
        return FALSE;
      }
      break;
    default:
      DEBUG ((DEBUG_INFO, "Invalid IP Name\n"));
      ASSERT(FALSE);
      return FALSE;
  }
  return TRUE;
}

/**
  This function is to Read DEVEN Register for Uncore IPs.

  @param[in] Ip       - Instance of UNCORE_IP
  @param[in] Instance - Instance of UNCORE_IP_INSTANCE

  @retval FALSE       - Uncore Ip is Disable by DEVEN
  @retval TRUE        - Uncore Ip is Enable by DEVEN

**/
BOOLEAN
EFIAPI
UncoreDevEnRead (
  IN UNCORE_IP           Ip,
  IN UNCORE_IP_INSTANCE  Instance
  )
{
  DEVEN_HOSTBRIDGE_CFG_STRUCT DevenCfg;
  DevenCfg.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS(SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, DEVEN_HOSTBRIDGE_CFG_REG));
  switch (Ip) {
    case Graphics:
      if (DevenCfg.Bits.ig_en == TRUE) {
        return TRUE;
      }
      break;
    case Npu:
      if (DevenCfg.Bits.npu_en == TRUE) {
        return TRUE;
      }
      break;
    case Ipu:
      if (DevenCfg.Bits.imgu_en == TRUE) {
        return TRUE;
      }
      break;
    case Iax:
      if (DevenCfg.Bits.iax_en == TRUE) {
        return TRUE;
      }
      break;
    case Vmd:
      if (DevenCfg.Bits.vmd_en == TRUE) {
        return TRUE;
      }
      break;
    case Camarillo:
      if (DevenCfg.Bits.cd_en == TRUE) {
        return TRUE;
      }
      break;
    case Telemetry:
      if (DevenCfg.Bits.crashlog_en == TRUE) {
        return TRUE;
      }
      break;
    default:
      DEBUG ((DEBUG_INFO, "Invalid IP Name\n"));
      ASSERT(FALSE);
  }
  return FALSE;
}

/**
  Program PcieFunc register for Function Swapping when PEG10 and PEG60 are not enabled, Here offset is 0x7C

  @param[in] RpIndex     - Root Port Index (0-based)
  @param[in] NewFunction - New function number
**/
VOID
EFIAPI
IocSetPcieFunction (
  IN UINT32  RpIndex,
  IN UINT32  NewFunction
  )
{
  MCHBAR_HOSTBRIDGE_CFG_STRUCT      MchBar;
  UINT32                            BdfAllocRegOffset;
  BDF_ALLOC_B_8_IOC_MCHBAR_STRUCT   BdfAllocCfg;

  DEBUG ((DEBUG_INFO, "ProgramBdfAllocRegister Start!\n"));
      switch (RpIndex) {
        case 8:
          BdfAllocRegOffset = BDF_ALLOC_B_8_IOC_MCHBAR_REG;
          break;
        case 9:
          BdfAllocRegOffset = BDF_ALLOC_B_9_IOC_MCHBAR_REG;
          break;
        case 10:
          BdfAllocRegOffset = BDF_ALLOC_B_12_IOC_MCHBAR_REG;
          break;
        case 11:
          BdfAllocRegOffset = BDF_ALLOC_B_16_IOC_MCHBAR_REG;
          break;
        default:
          DEBUG ((DEBUG_ERROR, "Invalid RpIndex %d\n", RpIndex));
          return;
      }
  MchBar.Data = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  BdfAllocCfg.Data = MmioRead32 ((UINTN)(MchBar.Data + BdfAllocRegOffset));
  BdfAllocCfg.Bits.func = NewFunction;
  MmioWrite32 ((UINTN)(MchBar.Data + BdfAllocRegOffset), BdfAllocCfg.Data);
  DEBUG ((DEBUG_INFO, "BdfAllocRegOffset (%x) = %x\n", BdfAllocRegOffset, MmioRead32 ((UINTN)(MchBar.Data + BdfAllocRegOffset))));
  DEBUG ((DEBUG_INFO, "ProgramBdfAllocRegister End!\n"));
}

/**
  This function is to read the hostbrige register.

  @param[in] RegiterName   - Instance of HostBridgeRester
  @param[in] DataRequested - Instance of Mode of the Register

  @retval UINT64           - Register Details based on Mode

**/
UINT64
EFIAPI
GetHostBridgeRegisterData (
  IN HOSTBRIDGE_REGISTER  RegiterName,
  IN UINT16               DataRequested
  )
{
  MCHBAR_HOSTBRIDGE_CFG_STRUCT  MchBar;
  TSEGMB_HOSTBRIDGE_CFG_STRUCT  TsegmbCfg;
  TOLUD_HOSTBRIDGE_CFG_STRUCT   ToludCfg;
  GGC_HOSTBRIDGE_CFG_STRUCT     GgcCfg;
  BDSM_HOSTBRIDGE_CFG_STRUCT    BdsmCfg;
  BGSM_HOSTBRIDGE_CFG_STRUCT    BgsmCfg;
  TOUUD_HOSTBRIDGE_CFG_STRUCT   TouudCfg;
  TOM_HOSTBRIDGE_CFG_STRUCT     TomCfg;
  SAFBAR_HOSTBRIDGE_CFG_STRUCT  SafBar;
  PAVPC_HOSTBRIDGE_CFG_STRUCT   Pavpc;
  DEVEN_HOSTBRIDGE_CFG_STRUCT   Deven;

  switch (RegiterName) {
    case HostBridgeCfgReg:
      if (DataRequested == MchBarCfgBaseLow) {
        MchBar.Data32[0] = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, MCHBAR_HOSTBRIDGE_CFG_REG));
        MchBar.Bits.mchbaren = FALSE;
        return MchBar.Data32[0];
      } else if (DataRequested == MchBarCfgBaseHigh) {
        MchBar.Data32[1] = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, MCHBAR_HOSTBRIDGE_CFG_REG + 4));
        return MchBar.Data32[1];
      } else {
        MchBar.Data32[0] = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, MCHBAR_HOSTBRIDGE_CFG_REG));
        MchBar.Data32[1] = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, MCHBAR_HOSTBRIDGE_CFG_REG + 4));
        MchBar.Bits.mchbaren = FALSE;
        return MchBar.Data;
      }
      break;

    case HostBridgeGgcCfgReg:
      GgcCfg.Data = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS(SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0) + GGC_HOSTBRIDGE_CFG_REG);
      if (DataRequested == HostBridgGgcLock) {
        return (BOOLEAN) GgcCfg.Bits.ggclck;
      } else if (DataRequested == HostBridgeGgcDsmSizeSelector) {
        return GgcCfg.Bits.gms;
      } else if (DataRequested == HostBridgeGgcGgsmSize) {
        return GgcCfg.Bits.ggms;
      } else if (DataRequested == HostBridgeGgcMask) {
        return  (GgcCfg.Data & B_SA_HOSTBRIDGE_GGC_MASK);
      } else {
        return GgcCfg.Data;
      }
      break;

    case HostBridgeTsegMb:
      TsegmbCfg.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, TSEGMB_HOSTBRIDGE_CFG_REG));
      if (DataRequested == HostBridgeTsegMbLock) {
        return (BOOLEAN) (TsegmbCfg.Bits.lock);
      } else if (DataRequested == HostBridgeTsegMbBase) {
        return (UINT32) (TsegmbCfg.Bits.tsegmb << TSEGMB_HOSTBRIDGE_CFG_TSEGMB_LSB);
      } else if (DataRequested == HostBridgeTsegMbFullData) {
        return TsegmbCfg.Data;
      } else {
        TsegmbCfg.Bits.lock = FALSE;
        return TsegmbCfg.Data;
      }

    case HostBridgeTolud:
      ToludCfg.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, TOLUD_HOSTBRIDGE_CFG_REG));
      if (DataRequested == HostBridgeToludLock) {
        return (BOOLEAN) (ToludCfg.Bits.lock);
      } else if (DataRequested == HostBridgeToludBase) {
        return (UINT32) (ToludCfg.Bits.tolud << TOLUD_HOSTBRIDGE_CFG_TOLUD_LSB);
      } else {
        ToludCfg.Bits.lock = FALSE;
        return ToludCfg.Data;
      }

    case HostBridgeDeviceId:
      return (UINT16) PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS(SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, PCI_DEVICE_ID_OFFSET));

    case HostBridgeTouud:
      TouudCfg.Data32[0] = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, TOUUD_HOSTBRIDGE_CFG_REG));
      TouudCfg.Data32[1] = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, TOUUD_HOSTBRIDGE_CFG_REG + 4));
      if (DataRequested == HostBridgeTouudBase) {
        return LShiftU64 (TouudCfg.Bits.touud, TOUUD_HOSTBRIDGE_CFG_TOUUD_LSB);
      } else if (DataRequested == HostBridgeTouudLock) {
        return (BOOLEAN) TouudCfg.Bits.lock;
      } else {
        TouudCfg.Bits.lock = FALSE;
        return TouudCfg.Data;
      }

    case HostBridgeBdsm:
      BdsmCfg.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS(SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, BDSM_HOSTBRIDGE_CFG_REG));
      if (DataRequested == HostBridgeBdsmLock) {
        return (BOOLEAN) BdsmCfg.Bits.lock;
      } else if (DataRequested == HostBridgeBdsmBase) {
        return (UINT32) (BdsmCfg.Bits.bdsm << BDSM_HOSTBRIDGE_CFG_BDSM_LSB);
      } else if (DataRequested == HostBridgeBdsmFullData) {
        return BdsmCfg.Data;
      } else {
        BdsmCfg.Bits.lock = FALSE;
        return BdsmCfg.Data;
      }

    case HostBridgeBgsm:
      BgsmCfg.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS(SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, BGSM_HOSTBRIDGE_CFG_REG));
      if (DataRequested == HostBridgeBgsmLock) {
        return (BOOLEAN) BgsmCfg.Bits.lock;
      } else if (DataRequested == HostBridgeBgsmBase) {
        return (UINT32) (BgsmCfg.Bits.bgsm << BGSM_HOSTBRIDGE_CFG_BGSM_LSB);
      } else if (DataRequested == HostBridgeBgsmFullData) {
        return BgsmCfg.Data;
      } else {
        BgsmCfg.Bits.lock = FALSE;
        return BgsmCfg.Data;
      }

    case HostBridgeTom:
      TomCfg.Data32[0] = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, TOM_HOSTBRIDGE_CFG_REG));
      TomCfg.Data32[1] = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, TOM_HOSTBRIDGE_CFG_REG + 4));
      if (DataRequested == HostBridgeTomInMb) {
        return TomCfg.Bits.tom;
      } else {
        TomCfg.Bits.lock = FALSE;
        return TomCfg.Data;
      }

    case HostBridgeSafBar:
      if (DataRequested == SafBarCfgBaseLow) {
        SafBar.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, SAFBAR_HOSTBRIDGE_CFG_REG));
        SafBar.Bits.safbaren = FALSE;
        return SafBar.Data32[0];
      } else if (DataRequested == SafBarCfgBaseHigh) {
        SafBar.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, SAFBAR_HOSTBRIDGE_CFG_REG + 4));
        return SafBar.Data32[1];
      } else {
        SafBar.Data32[0] = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, SAFBAR_HOSTBRIDGE_CFG_REG));
        SafBar.Data32[1] = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, SAFBAR_HOSTBRIDGE_CFG_REG + 4));
        SafBar.Bits.safbaren = FALSE;
        return SafBar.Data;
      }

    case HostBridgePavpc:
      Pavpc.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, PAVPC_HOSTBRIDGE_CFG_REG));
      return Pavpc.Data;

    case HostBridgeDeven:
      Deven.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, DEVEN_HOSTBRIDGE_CFG_REG));
      return Deven.Data;

    default:
      DEBUG ((DEBUG_INFO, "Invalid HostBridge Register Requested\n"));
      ASSERT(FALSE);
      break;
  }

  DEBUG ((DEBUG_INFO, "Invalid HostBridge Register Requested\n"));
  ASSERT(FALSE);
  return MAX_UINT64;
}

/**
  This function is to write to hostbrige register.

  @param[in] RegiterName   - Instance of HostBridgeRester
  @param[in] RegisterField - Instance of Mode of the Register
  @param[in] AdditionalData - when API need's to write some custom data, this parameter will be used

**/
VOID
EFIAPI
SetHostBridgeRegisterData (
  IN HOSTBRIDGE_REGISTER  RegiterName,
  IN UINT16               RegisterField,
  IN UINT64               AdditionalData
  )
{
  GGC_HOSTBRIDGE_CFG_STRUCT     GgcCfgStruct;
  MCHBAR_HOSTBRIDGE_CFG_STRUCT  MchBarCfgStruct;
  SAFBAR_HOSTBRIDGE_CFG_STRUCT  SafBar;
  DPR_HOSTBRIDGE_CFG_STRUCT     DprCfgStruct;
  UINT64                        McD0BaseAddress;
  UINT32                        MchBar;

  switch (RegiterName) {
    case HostBridgeGgcCfgReg:
      GgcCfgStruct.Data = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0) + GGC_HOSTBRIDGE_CFG_REG);
      if (RegisterField == HostBridgeGgcCfgSetIvdVamen) {
        GgcCfgStruct.Bits.ivd = TRUE;
        GgcCfgStruct.Bits.vamen = TRUE;
      } else if (RegisterField == HostBridgeGgcCfgClearIvdVamen) {
        GgcCfgStruct.Bits.ivd = FALSE;
        GgcCfgStruct.Bits.vamen = FALSE;
      } else if (RegisterField == HostBridgeGgcGmsWriteData) {
        GgcCfgStruct.Bits.gms = (UINT16)AdditionalData;
      } else if (RegisterField == HostBridgeGgcClearGmsSetIvdVamen) {
        GgcCfgStruct.Bits.gms = 0;
        GgcCfgStruct.Bits.ivd = TRUE;
        GgcCfgStruct.Bits.vamen = TRUE;
      }
      PciSegmentWrite16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0) + GGC_HOSTBRIDGE_CFG_REG, GgcCfgStruct.Data);
      break;

    case HostBridgePam0CfgReg:
      McD0BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
      PciSegmentWrite8 (McD0BaseAddress + PAM0_HOSTBRIDGE_CFG_REG, 0x30);
      break;

    case HostBridgePam0CfgRegLateInit:
      McD0BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
      PciSegmentWrite32 (McD0BaseAddress + PAM0_HOSTBRIDGE_CFG_REG, 0x33333330);
      break;

    case HostBridgePam4CfgReg:
      McD0BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
      PciSegmentWrite8 (McD0BaseAddress + PAM4_HOSTBRIDGE_CFG_REG, 0x33);
      break;

    case HostBridgePam5CfgReg:
      McD0BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
      PciSegmentWrite8 (McD0BaseAddress + PAM5_HOSTBRIDGE_CFG_REG, 0x33);
      break;

    case HostBridgePam6CfgReg:
      McD0BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
      PciSegmentWrite8 (McD0BaseAddress + PAM6_HOSTBRIDGE_CFG_REG, 0x33);
      break;

    case HostBridgePamWpgWac:
      MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
      MmioAnd64 (MchBar + PAM_W_PG_WAC_MCHBAR_CBO_EGRESS_REG, (UINT64) ~(BIT0));
      MmioAnd64 (MchBar + SNCU_PAM_W_PG_WAC_HOSTBRIDGE_MCHBAR_REG, (UINT64) ~(BIT0));
      break;

    case HostBridgePamWpgCp:
      MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
      MmioAnd64 (MchBar + PAM_W_PG_CP_MCHBAR_CBO_EGRESS_REG, (UINT64) ~(BIT0));
      MmioAnd64 (MchBar + SNCU_PAM_W_PG_CP_HOSTBRIDGE_MCHBAR_REG, (UINT64) ~(BIT0));
      break;

    case HostBridgeDprCfg:
      DprCfgStruct.Data = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, DPR_HOSTBRIDGE_CFG_REG));
      if (RegisterField == HostBridgeDprSetlock) {
        DprCfgStruct.Bits.lock = TRUE;
      } else if (RegisterField == HostBridgeDprClearlock) {
        DprCfgStruct.Bits.lock = FALSE;
      }
      PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, DPR_HOSTBRIDGE_CFG_REG), DprCfgStruct.Data);
      break;

    case HostBridgeCfgReg:
      if (RegisterField == MchBarCfgBaseLow) {
        MchBarCfgStruct.Data32[0] = (UINT32) AdditionalData;
        PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, MCHBAR_HOSTBRIDGE_CFG_REG), MchBarCfgStruct.Data32[0]);
      } else if (RegisterField == MchBarCfgBaseHigh) {
        MchBarCfgStruct.Data32[1] = (UINT32) AdditionalData;
        PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, MCHBAR_HOSTBRIDGE_CFG_REG + 4), MchBarCfgStruct.Data32[1]);
      }
      break;

    case HostBridgeSafBar:
      if (RegisterField == SafBarCfgBaseLow) {
        SafBar.Data32[0] = (UINT32) AdditionalData;
        PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, SAFBAR_HOSTBRIDGE_CFG_REG), SafBar.Data32[0]);
      } else if (RegisterField == SafBarCfgBaseHigh) {
        SafBar.Data32[1] = (UINT32) AdditionalData;
        PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, SAFBAR_HOSTBRIDGE_CFG_REG + 4), SafBar.Data32[1]);
     }
      break;

    default:
      DEBUG ((DEBUG_INFO, "Invalid HostBridge Register Write Requested\n"));
      ASSERT(FALSE);
      break;
  }

  return;
}


/**
  This function will update Graphics register.

  @param[in] GraphicsGttSize    - GraphicsGttSize
  @param[in] GraphicsStolenSize - GraphicsStolenSize

  @retval UINT64                - Register Data

**/
UINT16
EFIAPI
UpdateGraphicsController (
   IN  UINT32                       GraphicsGttSize,
   IN  UINT32                       GraphicsStolenSize
  )
{
  GGC_HOSTBRIDGE_CFG_STRUCT    GgcCfg;
  UINT32                       Gms;

  GgcCfg.Data = (UINT16) GetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgeGgcFullData);
  //
  // Graphics Stolen Size below 64MB has a higher granularity and can be set in 4MB increments
  // For Sizes 4MB to 60MB sizes (excluding 32MB), GMS values range from 240-254 (240 + Size/4 - 1)
  // Graphics Stolen Size above 64MB has a granularity of 32MB increments
  // For Sizes 0, 32MB and above 64MB GMS values are Size/32
  //
  if ((GraphicsStolenSize == 32) || (GraphicsStolenSize == 0) || (GraphicsStolenSize >= 64)) {
    Gms = GraphicsStolenSize / 32;
  } else {
    Gms = 240 + (GraphicsStolenSize / 4 - 1);
  }
  GgcCfg.Bits.gms = (UINT16) Gms;

  return GgcCfg.Data;
}

/**
  Disables the root port using DEV_EN bit.

  @param[in] RpIndex            Root Port Number

**/
VOID
EFIAPI
IocDisablePcieRootPort (
  IN UINT32   RpIndex
  )
{
  MCHBAR_HOSTBRIDGE_CFG_STRUCT              MchBar;
  IOC_PMC_DEVEN_B_IOC_MCHBAR_STRUCT         PmcDevenStruct;

  DEBUG ((DEBUG_INFO, "IocDisablePcieRootPort Start\n"));

  MchBar.Data = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
    PmcDevenStruct.Data = MmioRead32 ((UINTN)(MchBar.Data + IOC_PMC_DEVEN_B_IOC_MCHBAR_REG));
    DEBUG ((DEBUG_INFO, "DevEn = %x\n", PmcDevenStruct.Data));

      switch (RpIndex) {
        case 8:
          PmcDevenStruct.Bits.spc_f0_en = FALSE;
          break;
        case 9:
          PmcDevenStruct.Bits.spc_f1_en = FALSE;
          break;
        case 10:
          PmcDevenStruct.Bits.spd_f0_en = FALSE;
          break;
        case 11:
          PmcDevenStruct.Bits.spe_f0_en = FALSE;
          break;
        default:
          ASSERT(FALSE);
          return;
      }

    MmioWrite32 ((UINTN)(MchBar.Data + IOC_PMC_DEVEN_B_IOC_MCHBAR_REG), PmcDevenStruct.Data);
    DEBUG ((DEBUG_INFO, "DevEn after write = %x\n", MmioRead32 ((UINTN)(MchBar.Data + IOC_PMC_DEVEN_B_IOC_MCHBAR_REG))));
  DEBUG ((DEBUG_INFO, "IocDisablePcieRootPort End\n"));
  return;
}

/**
  Check if root port is enabled.

  @param[in] RpIndex            Root Port Number
**/
BOOLEAN
EFIAPI
IocIsPcieRootPortEnabled (
  IN UINT32   RpIndex
  )
{
  MCHBAR_HOSTBRIDGE_CFG_STRUCT              MchBar;
  IOC_PMC_DEVEN_B_IOC_MCHBAR_STRUCT         PmcDevenStruct;

  MchBar.Data = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
    PmcDevenStruct.Data = MmioRead32 ((UINTN)(MchBar.Data + IOC_PMC_DEVEN_B_IOC_MCHBAR_REG));
    DEBUG ((DEBUG_INFO, "DevEn = %x\n", PmcDevenStruct.Data));

      switch (RpIndex) {
        case 8:
          return (BOOLEAN)PmcDevenStruct.Bits.spc_f0_en;
          break;
        case 9:
          return (BOOLEAN)PmcDevenStruct.Bits.spc_f1_en;
          break;
        case 10:
          return (BOOLEAN)PmcDevenStruct.Bits.spd_f0_en;
          break;
        case 11:
          return (BOOLEAN)PmcDevenStruct.Bits.spe_f0_en;
          break;
        default:
          ASSERT(FALSE);
          return FALSE;
      }
}

/**
  This function wtites TCSS DEVEN Register.

  @param[in]  TcssDevEn  - Maps to bits in TCSS_DEVEN_0_0_0_MCHBAR_IMPH

**/
VOID
HostBridgeSetTcssDeven (
  IN UINT32 TcssDevEn
  )
{
  UINT32  MchBar;

  ///
  ///   BIOS fills TCSS DEVEN based on BIOS policy
  ///   BIOS can only disables/enables supported TCSS DEV IP by SKU/Fuse
  ///
  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  DEBUG ((DEBUG_INFO, "Set TcssDevEn=0x%x\n", TcssDevEn));
  MmioWrite32 (MchBar + IOC_PMC_DEVEN_C_IOC_MCHBAR_REG, TcssDevEn);
}

/**
  This function is to read the TCSS DEVEN register

  @retval UINT32           - TCSS DEVEN Register

**/
UINT32
HostBridgeGetTcssDeven (
  VOID
  )
{
  UINT32  TcssDevEn;
  UINT32  MchBar;

  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  TcssDevEn = MmioRead32 (MchBar + IOC_PMC_DEVEN_C_IOC_MCHBAR_REG);

  DEBUG ((DEBUG_INFO, "Get TcssDevEn=0x%x\n", TcssDevEn));
  return TcssDevEn;
}

/**
  Sets function mapping for TCSS PCIe root ports
  Writes function mapping into BDF_ALLOC_C registers

  @param[in]  PcieFuncMap - TCSS PCIe root ports mapping

**/
VOID
HostBridgeSetTcssPcieRpFuncMap (
  IN UINT32  PcieFuncMap
  )
{
  UINT64  MchBar;
  UINT32  Index;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    ///
    /// BdfAllocRegOffset = BDF_ALLOC_C_0_IOC_MCHBAR_REG + (PMC_DEVEN_C[i] * 4)
    ///
    MmioAndThenOr32 (
      MchBar + (BDF_ALLOC_C_0_IOC_MCHBAR_REG + (Index * 4)),
      ~((UINT32)0x7),
      (PcieFuncMap >> (Index * 3)) & 0x7
    );
  }
}
