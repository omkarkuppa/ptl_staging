/** @file
  This contains code necessary to build the Domain Pcie Library.

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

@par Specification
**/

#include <IpPcieDefines.h>
#include <Library/DomainPcie.h>
#include <IpWrapperCntxtInfoClient.h>
#include <Register/HostDmaRegs.h>

#include <Library/PchPciBdfLib.h>

#include <Library/MemoryAllocationLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Register/PchPcieRpRegs.h>
#include <Library/IoLib.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <PchPcieRpConfig.h>
#include <Library/PciSegmentLib.h>
#include <Library/IocLib.h>
#include <Library/PeiPcieRpInitLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <PchPcieRpInfo.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPcieRpLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/DebugLib.h>
#include <Defines/PchReservedResources.h>
#include <Library/TimerLib.h>
#include <PcieHelpersLib.h>
#include <Library/PcieHelperLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdFiaSocLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <IpPcieRegs.h>
#include <Library/PcdInfoLib.h>
#include <Library/PeiVtdInitFruLib.h>
#include <Library/HobLib.h>
#include <Register/PmcRegs.h>
#define PCH_PWRM_BASE_ADDRESS 0xFE000000 // PMC MBAR MMIO base address

#define TIMEOUT_100_MS  100000000  // 100 ms in Nanoseconds

static UINT64   TimeStampAfterPcieLinkRetrainSet;
extern EFI_GUID gIpPcieInstHobGuid;

/**
  Fia Inst Init
**/
VOID
IpPcieFiaInstInit (
  )
{
  return;
}

/**
  Phy Inst Init
**/
VOID
IpPciePhyInstInit (
  )
{
  return;
}

/**
  Update Reg Context for PCIE

  @param[in] IpPcieInst   Pointer to IP inst
  @param[in] IpTarget     IP defined value for target within the PCIE IP

  @return EFI_STATUS
  @return EFI_INVALID_PARAMETER When pInst is incorrect
**/
EFI_STATUS
PcieUpdateRegCntxt (
  IP_PCIE_INST *pInst,
  UINT32       IpTarget
  )
{
  UINTN               Dev;
  UINTN               Fun;
  IP_WR_REG_INFO      *RegInfo;
  EFI_STATUS          Status;
  P2SB_CONTROLLER     P2SbController;

  if (pInst == NULL) {
    DEBUG ((DEBUG_INFO,"pInst == NULL\n"));
    return IpCsiStsErrorNullPtr;
  }

  pInst->RegCntxt_Cfg_Pri = (IP_WR_REG_CNTXT)(UINTN)AllocateZeroPool (sizeof (IP_WR_REG_INFO));
  if (pInst->RegCntxt_Cfg_Pri == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Get Sideband bridge data
  //
  Status = PtlPcdGetFirstP2SbController (&P2SbController);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Update PCIe Config access
  //
  RegInfo = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Cfg_Pri;


  RegInfo->AccessType      = IpWrRegAccessCfg;
  RegInfo->AccessSubType   = IpWrRegSubTypeNone;

  GetPchPcieRpDevFun (IpTarget, &Dev, &Fun);
  RegInfo->RegType.Pci.Seg = 0x0;
  RegInfo->RegType.Pci.Bus = 0x0;
  RegInfo->RegType.Pci.Dev = (UINT8) Dev;
  RegInfo->RegType.Pci.Fun = (UINT8) Fun;

  //
  // Update RCRB Register access
  //
  pInst->RegCntxt_Mem_Rcrb = (IP_WR_REG_CNTXT)(UINTN)AllocateZeroPool (sizeof (IP_WR_REG_INFO));
  if (pInst->RegCntxt_Mem_Rcrb == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  RegInfo                  = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Mem_Rcrb;

  RegInfo->AccessType           = IpWrRegAccessSb;
  RegInfo->AccessSubType        = IpWrRegAccessP2Sb;
  RegInfo->RegType.P2Sb.P2SbPid = GetRootPortSbiPid (IpTarget);

  RegInfo->RegType.P2Sb.P2SbCtrl = (IP_WR_P2SB_CONTROLLER *)AllocateZeroPool (sizeof (IP_WR_P2SB_CONTROLLER));
  if (RegInfo->RegType.P2Sb.P2SbCtrl == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  RegInfo->RegType.P2Sb.P2SbCtrl->Mmio                = P2SbController.Mmio;
  RegInfo->RegType.P2Sb.P2SbCtrl->PciCfgBaseAddr      = P2SbController.PciCfgBaseAddr;
  RegInfo->RegType.P2Sb.P2SbCtrl->P2sb20bPcrSupported = P2SbController.P2sb20bPcrSupported;
  RegInfo->RegType.P2Sb.Offset                        = 0x1000;

  //
  // Update AccessMethod
  // In PEI IOE root ports need to be accessed with
  // MSG access due to MMIO not being available in 32-bit
  // environment.
  //
  RegInfo->RegType.MmioBase                      = PCH_PCR_BASE_ADDRESS;
  RegInfo->RegType.P2Sb.AccessMethod             = IpWrP2SbMsgAccess;

  RegInfo->RegType.P2Sb.RegisterSpace = P2SbMemory;
  RegInfo->RegType.P2Sb.PostedWrites  = TRUE;
  RegInfo->RegType.P2Sb.Fid           = GetRpSbiFid ((UINT32) IpTarget);


  //
  // Private Register Access
  //
  pInst->RegCntxt_Prvt_Cfg = (IP_WR_REG_CNTXT)(UINTN)AllocateZeroPool (sizeof (IP_WR_REG_INFO));
  if (pInst->RegCntxt_Prvt_Cfg == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  RegInfo                  = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Prvt_Cfg;

  RegInfo->AccessType    = IpWrRegAccessSb;
  RegInfo->AccessSubType = IpWrRegAccessP2Sb;
  //
  // Update P2SB Config
  //
  RegInfo->RegType.P2Sb.P2SbPid       = GetRootPortSbiPid (IpTarget);
  RegInfo->RegType.P2Sb.Offset        = 0;
  RegInfo->RegType.P2Sb.RegisterSpace = P2SbPrivateConfig;
  RegInfo->RegType.P2Sb.Fid           = GetRpSbiFid ((UINT32) IpTarget);
  RegInfo->RegType.P2Sb.PostedWrites  = FALSE;

  RegInfo->RegType.P2Sb.P2SbCtrl = (IP_WR_P2SB_CONTROLLER *)AllocateZeroPool (sizeof (IP_WR_P2SB_CONTROLLER));
  if (RegInfo->RegType.P2Sb.P2SbCtrl == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  RegInfo->RegType.P2Sb.P2SbCtrl->Mmio                = P2SbController.Mmio;
  RegInfo->RegType.P2Sb.P2SbCtrl->PciCfgBaseAddr      = P2SbController.PciCfgBaseAddr;
  RegInfo->RegType.P2Sb.P2SbCtrl->P2sb20bPcrSupported = P2SbController.P2sb20bPcrSupported;

  //
  // Update AccessMethod
  // In PEI IOE root ports need to be accessed with
  // MSG access due to MMIO not being available in 32-bit
  // environment.
  //
  RegInfo->RegType.MmioBase                      = PCH_PCR_BASE_ADDRESS;
  RegInfo->RegType.P2Sb.AccessMethod             = IpWrP2SbMsgAccess;
  RegInfo->RegType.P2Sb.PostedWrites             = FALSE;
  //
  // SideBand Access
  //
  pInst->RegCntxt_Cfg_Sb = (IP_WR_REG_CNTXT)(UINTN)AllocateZeroPool (sizeof (IP_WR_REG_INFO));
  if (pInst->RegCntxt_Cfg_Sb  == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  RegInfo                = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Cfg_Sb;

  RegInfo->AccessType         = IpWrRegAccessSb;
  RegInfo->AccessSubType      = IpWrRegAccessP2Sb;
  //
  // Update P2SB Config
  //
  RegInfo->RegType.P2Sb.P2SbPid       = GetRootPortSbiPid (IpTarget);
  RegInfo->RegType.P2Sb.Offset        = 0;
  RegInfo->RegType.P2Sb.RegisterSpace = P2SbPciConfig;
  RegInfo->RegType.P2Sb.Fid           = GetRpSbiFid ((UINT32) IpTarget);
  RegInfo->RegType.P2Sb.PostedWrites  = FALSE;

  RegInfo->RegType.P2Sb.P2SbCtrl = (IP_WR_P2SB_CONTROLLER *)AllocateZeroPool (sizeof (IP_WR_P2SB_CONTROLLER));
  if (RegInfo->RegType.P2Sb.P2SbCtrl == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  RegInfo->RegType.P2Sb.P2SbCtrl->Mmio                = P2SbController.Mmio;
  RegInfo->RegType.P2Sb.P2SbCtrl->PciCfgBaseAddr      = P2SbController.PciCfgBaseAddr;
  RegInfo->RegType.P2Sb.P2SbCtrl->P2sb20bPcrSupported = P2SbController.P2sb20bPcrSupported;

  //
  // Update AccessMethod
  // In PEI IOE root ports need to be accessed with
  // MSG access due to MMIO not being available in 32-bit
  // environment.
  //
  RegInfo->RegType.MmioBase                      = PCH_PCR_BASE_ADDRESS;
  RegInfo->RegType.P2Sb.AccessMethod             = IpWrP2SbMsgAccess;

  //Update MemRcrbBase to 0 as MemBar is disabled
  pInst->MemRcrbBase = 0;

  return IpCsiStsSuccess;
}

/**
  Update mem Context for IP Instance

  @param[in] pInst        Pointer to IP inst
  @param[in] IpTarget     IP defined value for target within the PCIE IP

  @retval IpCsiStsSuccess         Operation completed successfully
  @retval IpCsiStsErrorNullPtr    pInst was NULL
**/
IP_CSI_STATUS
PcieUpdateMemCntxt (
  IP_PCIE_INST *pInst,
  UINT32       IpTarget
  )
{
  IP_WR_MEM_INFO      *MemInfo;

  if (pInst == NULL) {
    DEBUG ((DEBUG_ERROR, "pInst == NULL\n"));
    return IpCsiStsErrorNullPtr;
  }

  pInst->MemCntxt = (IP_WR_REG_CNTXT)(UINTN)AllocateZeroPool (sizeof (IP_WR_MEM_INFO));
  if (pInst->MemCntxt == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return IpCsiStsErrorInsufficientResources;
  }

  MemInfo = (IP_WR_MEM_INFO*) pInst->MemCntxt;
  MemInfo->MemoryType      = EfiBootServicesData;

  return IpCsiStsSuccess;
}

/**
  Update mem Context for IP Instance

  @param[in] pInst        Pointer to IP inst
  @param[in] Index        Index of Rootport

**/

VOID
GetPcieTopology (
  IP_PCIE_INST    *pInst,
  UINT32           Index
  )
{
  if (pInst == NULL) {
    DEBUG ((DEBUG_ERROR,"pInst == NULL\n"));
    return;
  }

  pInst->RpIndex      = Index;
  pInst->IsController = IsController (Index);
  pInst->IocDecoded   = IsRpIocDecoded (Index);
}

/**
  This function assigns bus number to PCI bus .

  @param[in]  RpBase    Root Port PCI base address
  @param[in]  UINT8     TempPciBus
**/
VOID
PcieAssignTemporaryBus (
  UINT64    RpBase,
  UINT8     TempPciBus
  )
{
  BNUM_SLT_PCIE_CFG_STRUCT BnumCfg;

  BnumCfg.Data = PciSegmentRead32 (RpBase + BNUM_SLT_PCIE_CFG_REG);
  BnumCfg.Bits.scbn = TempPciBus;
  BnumCfg.Bits.sbbn = TempPciBus;
  PciSegmentWrite32 (RpBase + BNUM_SLT_PCIE_CFG_REG, BnumCfg.Data);
}

/**
  Clear temporary bus number.

  @param[in]  RpBase    Root Port PCI base address
**/
VOID
PcieClearTemporaryBus (
  UINT64    RpBase
  )
{
  BNUM_SLT_PCIE_CFG_STRUCT BnumCfg;

  BnumCfg.Data = PciSegmentRead32 (RpBase + BNUM_SLT_PCIE_CFG_REG);
  BnumCfg.Bits.scbn = 0x0;
  BnumCfg.Bits.sbbn = 0x0;
  PciSegmentWrite32 (RpBase + BNUM_SLT_PCIE_CFG_REG, BnumCfg.Data);
}

/**
  Get information about the endpoint

  @param[in]  IP_PCIE_INST    *pInst
  @param[in]  TempPciBus       Temporary bus number
  @param[out] DeviceInfo       Device information structure

  @raturn TRUE if device was found, FALSE otherwise
**/
BOOLEAN
PcieGetDeviceInfo (
  IP_PCIE_INST     *pInst,
  UINT8             TempPciBus,
  PCIE_DEV_INFO    *DeviceInfo
  )
{
  UINT32                  Data32;
  UINT64                  RpBase;
  UINT64                  EpBase;
  UINT8                   EpPcieCapPtr;
  UINT8                   EpLinkSpeed;
  IP_WR_REG_INFO          *RegInfo;
  LCAP_PCIE_CFG_STRUCT    LcapCfg;
  SLSTS_PCIE_CFG_STRUCT   SlstsCfg;

  DeviceInfo->Vid          = 0xFFFF;
  DeviceInfo->Did          = 0xFFFF;
  DeviceInfo->MaxLinkSpeed = 0;

  RegInfo = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Cfg_Pri;
  RpBase  = PCI_SEGMENT_LIB_ADDRESS (RegInfo->RegType.Pci.Seg, RegInfo->RegType.Pci.Bus, RegInfo->RegType.Pci.Dev, RegInfo->RegType.Pci.Fun, 0);

  //
  // Check for device presence
  //
  SlstsCfg.Data = PciSegmentRead16 (RpBase + SLSTS_PCIE_CFG_REG);
  if (!!(SlstsCfg.Bits.pds) == 0) {
    return FALSE;
  }

  PcieAssignTemporaryBus (RpBase, TempPciBus);

  //
  // A config write is required in order for the device to re-capture the Bus number,
  // according to PCI Express Base Specification, 2.2.6.2
  // Write to a read-only register VendorID to not cause any side effects.
  //
  EpBase = PCI_SEGMENT_LIB_ADDRESS (RegInfo->RegType.Pci.Seg, TempPciBus, 0, 0, 0);
  PciSegmentWrite16 (EpBase + PCI_VENDOR_ID_OFFSET, 0);

  Data32          = PciSegmentRead32 (EpBase + PCI_VENDOR_ID_OFFSET);
  DeviceInfo->Vid = (UINT16)(Data32 & 0xFFFF);
  DeviceInfo->Did = (UINT16)(Data32 >> 16);

  EpLinkSpeed = 0;
  if (Data32 != 0xFFFFFFFF) {
    EpPcieCapPtr = PcieBaseFindCapId (EpBase, EFI_PCI_CAPABILITY_ID_PCIEXP);
    if (EpPcieCapPtr != 0) {
      LcapCfg.Data = PciSegmentRead32 (EpBase + EpPcieCapPtr + R_PCIE_LCAP_OFFSET);
      EpLinkSpeed = (UINT8)LcapCfg.Bits.mls;
    }
    DeviceInfo->MaxLinkSpeed = EpLinkSpeed;
  }

  PcieClearTemporaryBus (RpBase);

  DEBUG ((DEBUG_INFO, "VID: %04X DID: %04X  MLS: %d\n", DeviceInfo->Vid, DeviceInfo->Did, DeviceInfo->MaxLinkSpeed));
  return (Data32 != 0xFFFFFFFF);
}
/**
  This function checks if de-emphasis needs to be changed from default for a given rootport

  @param[in] IP_PCIE_INST    *pInst
  @param[in] DevInfo           Information on device that is connected to rootport
**/
VOID
PcieDecreasedDeEmphasis (
  IP_PCIE_INST   *pInst,
  PCIE_DEV_INFO  DevInfo
  )
{
  UINT64                RpBase;
  IP_WR_REG_INFO        *RegInfo;
  LCTL2_PCIE_CFG_STRUCT Lctl2Cfg;

  RegInfo = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Cfg_Pri;
  RpBase  = PCI_SEGMENT_LIB_ADDRESS (RegInfo->RegType.Pci.Seg, RegInfo->RegType.Pci.Bus, RegInfo->RegType.Pci.Dev, RegInfo->RegType.Pci.Fun, 0);

  //
  // Intel WiGig devices
  //
  if (DevInfo.Vid == V_PCH_INTEL_VENDOR_ID && DevInfo.Did == 0x093C) {
    Lctl2Cfg.Data = PciSegmentRead16 (RpBase + LCTL2_PCIE_CFG_REG);
    Lctl2Cfg.Bits.sd = TRUE;
    PciSegmentWrite16 (RpBase + LCTL2_PCIE_CFG_REG, Lctl2Cfg.Data);
  }
}

/**
  This function provides BIOS workaround for WiFi device cannot enter S0i3 state due to LCTL.ECPM bit is cleared.
  This workaround is applied for Intel Wireless-AC 9260(Thunder Peak).

  This function does speed change earlier, so that Endpoint will be in correct state by the time
  root port and its downstream devices are initialized.

  @param[in] pInst      Pointer to IP inst
  @param[in] DevInfo    Information on device that is connected to rootport
**/
VOID
PcieWifiLinkSpeedSyncWorkaround (
  IP_PCIE_INST     *pInst,
  PCIE_DEV_INFO    DevInfo
  )
{
  UINTN                 TimeoutCount;
  UINT64                RpBase;
  IP_WR_REG_INFO        *RegInfo;
  LSTS_PCIE_CFG_STRUCT  LstsCfg;
  LCTL_PCIE_CFG_STRUCT  LctlCfg;
  LCTL2_PCIE_CFG_STRUCT Lctl2Cfg;

  RegInfo = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Cfg_Pri;
  RpBase  = PCI_SEGMENT_LIB_ADDRESS (RegInfo->RegType.Pci.Seg, RegInfo->RegType.Pci.Bus, RegInfo->RegType.Pci.Dev, RegInfo->RegType.Pci.Fun, 0);

  if ((DevInfo.Vid == V_PCH_INTEL_VENDOR_ID) && (DevInfo.Did == 0x2526) && (pInst->PcieRpCommonConfig.PcieSpeed != IpPcieGen1)) {
    Lctl2Cfg.Data = PciSegmentRead16 (RpBase + LCTL2_PCIE_CFG_REG);
    Lctl2Cfg.Bits.tls = (UINT16)DevInfo.MaxLinkSpeed;
    PciSegmentWrite16(RpBase + LCTL2_PCIE_CFG_REG, Lctl2Cfg.Data);

    // Retrain the Link
    LctlCfg.Data = PciSegmentRead16 (RpBase + LCTL_PCIE_CFG_REG);
    LctlCfg.Bits.rl = TRUE;
    PciSegmentWrite16(RpBase + LCTL_PCIE_CFG_REG, LctlCfg.Data);

    // 100 ms timeout while checking for link training is completed.
    for (TimeoutCount = 0; TimeoutCount < 1000; TimeoutCount++) {
      // Delay 100 us
      MicroSecondDelay (100);
      LstsCfg.Data = PciSegmentRead16 (RpBase + LSTS_PCIE_CFG_REG);
      if (LstsCfg.Bits.lt == 0) {
        break;
      }
    }

    // 100 ms timeout while checking for link active on retrained link
    for (TimeoutCount = 0; TimeoutCount < 1000; TimeoutCount++) {
      // Delay 100 us
      MicroSecondDelay (100);
      LstsCfg.Data = PciSegmentRead16 (RpBase + LSTS_PCIE_CFG_REG);
      if (!!(LstsCfg.Bits.la)) {
        break;
      }
    }
  }
}
/**
  Called by SIP library to check if root port's lane
  is connected to PHY.

  @param[in] pInst        Point of pInst
  @param[in] RpLaneIndex  Index of the root port's lane which to check.

  @retval TRUE   PCIe lane connected to PHY.
  @retval FALSE  PCIe lane not connected to PHY.
**/
BOOLEAN
IpInstIsPcieLaneConnected (
  IN IP_PCIE_INST       *pInst,
  IN UINT32              RpLaneIndex
  )
{
  UINT32   PcieLane;
  UINT32   ControllerIndex;
  UINT8    LaneNum;

  if (pInst->RpIndex < 4) {
    ControllerIndex = RpIndexToControllerIndex (pInst->RpIndex);
    PcieLane = (ControllerIndex * PCH_PCIE_CONTROLLER_PORTS) + RpLaneIndex;

    return PtlPcdFiaGetPcieLaneNum (PcieLane, &LaneNum);
  } else {
      return TRUE;
  }
}

/**
  Checks if lane reversal is enabled on a given Root Port

  @param[in] pInst  *pInst

  @retval TRUE if lane reversal is enbabled, FALSE otherwise
**/
BOOLEAN
PcieIsLaneReversalEnabled (
  IP_PCIE_INST  *pInst
  )
{
  STRPFUSECFG_PCIE_CFG_STRUCT Strpfusecfg;

  //
  // From SIP17, LR bit is moved from PCIEDBG to STRPFUSECFG
  //
  Strpfusecfg.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, STRPFUSECFG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  return !!(Strpfusecfg.Bits.lr);
}

/**
  Verify whether the PCIe port does own all lanes according to the port width.

  @param[in] pInst   *pInst

  @retval TRUE   All lanes are owned by PCIe controller.
  @retval FALSE  Not all lanes are owned by PCIe root port.
**/
BOOLEAN
PcieIsRpOwningLanes (
  IN IP_PCIE_INST    *pInst
  )
{
  UINT32  MaxLinkWidth;
  UINT32  RpLaneIndex;
  UINT32  LaneIndexInController;
  UINT32  FirstRpLaneInController;
  UINT32  TotalLanesInController;

  MaxLinkWidth = pInst->PrivateConfig.LanesPerRootPort;
  for (RpLaneIndex = 0; RpLaneIndex < MaxLinkWidth; ++RpLaneIndex) {
    if (PcieIsLaneReversalEnabled (pInst)) {
      TotalLanesInController = pInst->PrivateConfig.RootPortsInController * pInst->PrivateConfig.LanesPerRootPort;
      FirstRpLaneInController = TotalLanesInController - (pInst->PrivateConfig.RootPortIndexInController * pInst->PrivateConfig.LanesPerRootPort);
      // 1 is subtracted to make the PhysicalLaneIndex 0-based.
      LaneIndexInController = FirstRpLaneInController - RpLaneIndex - 1;
    } else {
      FirstRpLaneInController = (pInst->PrivateConfig.RootPortIndexInController * pInst->PrivateConfig.LanesPerRootPort);
      LaneIndexInController = FirstRpLaneInController + RpLaneIndex;
    }
    if (!IpInstIsPcieLaneConnected (pInst, LaneIndexInController)) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
  Detect min temp and determine link speed when DTR is enabled

  @param[in] pInst   Point of pInst
  @param[in] UINT8   Speed per root port and end point capability
  @retval UINT8  Speed per DTR
**/
UINT8 PcieDtrSpeedDetermine(
  IN IP_PCIE_INST       *pInst,
  IN UINT8              CapSpeed
  )
{
  UINT32                            MinTemp;
  UINT32                            DtrHiVal;
  BOOLEAN                           TempHigerThanDtrHiVal;
  BOOLEAN                           TempValid;
  UINT32                            TssCount;
  UINTN                             TSSAddr;
  UINT32                            PwrmBase;

  PwrmBase = PCH_PWRM_BASE_ADDRESS;

  PRINT_LEVEL1 ("%s start at RootPort %d\n", __FUNCTION__, pInst->RpIndex);

  if (CapSpeed < IpPcieGen5) {
    PRINT_LEVEL1 ("DTR Not Need: Port %d either RP or EP has no Gen5 cap\n", pInst->RpIndex);
    IpPcieSetDtrStat(pInst, IpPcieDtrNotNeed);
    return CapSpeed;
  }

  // Following is handle Gen5 case

  PRINT_LEVEL1 ("DTR: Port %d both RP and EP have Gen5 cap\n", pInst->RpIndex);

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("DTR: Port %d Invalid pInst, so target Gen4\n", pInst->RpIndex);
    return IpPcieGen4;
  }

  if (IpPcieGetDtrStat(pInst) != IpPcieDtrReady) {
    PRINT_LEVEL1 ("DTR: Port %d state machine not ready, so target Gen4\n", pInst->RpIndex);
    return IpPcieGen4;
  }

  // Check Min temp valid or not
  TempValid = FALSE;
  for(TssCount = 0; TssCount < 8; TssCount++) {
    TSSAddr = PwrmBase + R_PMC_PWRM_TSS0 + TssCount * 4;
    if (MmioRead32(TSSAddr) & B_PMC_PWRM_TSS0_TSRV) {
      TempValid = TRUE;
      break;
    }
  }

  if (!TempValid){
    PRINT_LEVEL1 ("DTR: Port %d Temperature is not valid, so target Gen4\n", pInst->RpIndex);
    return IpPcieGen4;
  }

  // Read MinTemp;
  MinTemp = MmioRead32 (PwrmBase + R_PMC_PWRM_MIN_TEMP) & B_PMC_PWRM_MIN_TEMP_MIN_TEMP;

  // Read DTRHIVAL
  DtrHiVal = MmioRead32 (PwrmBase + R_PMC_PWRM_TSDTR_THRESH) & B_PMC_PWRM_TSDTR_THRESH_DTRHIVAL;

  PRINT_LEVEL1 ("DTR: MinTemp = 0x%x, DtrHiVal = 0x%x\n", MinTemp, DtrHiVal);

  TempHigerThanDtrHiVal = FALSE;

  // MinTemp is negative and DtrHiVal is negative
  if ((MinTemp & 0x100) && (DtrHiVal & 0x100)) {
    if (MinTemp > DtrHiVal) {
      TempHigerThanDtrHiVal = TRUE;
    }
  }
  // MinTemp is positive and DtrHiVal is positive
  if (!(MinTemp & 0x100) && !(DtrHiVal & 0x100)) {
    if (MinTemp > DtrHiVal) {
      TempHigerThanDtrHiVal = TRUE;
    }
  }

  // MinTemp is positive and DtrHiVal is negative
  if (!(MinTemp & 0x100) && (DtrHiVal & 0x100)) {
    TempHigerThanDtrHiVal = TRUE;
  }

  PRINT_LEVEL1 ("DTR: TempHigerThanDtrHiVal = %d\n", TempHigerThanDtrHiVal);

  if (!TempHigerThanDtrHiVal) {
    PRINT_LEVEL1 ("DTR: Temperature too low in BIOS phase for Gen5 train at RootPort %d, so target Gen4\n", pInst->RpIndex);
    return IpPcieGen4;
  } else {
    PRINT_LEVEL1 ("DTR: Temperature is fine in BIOS phase for Gen5 train at RootPort %d\n", pInst->RpIndex);
    return IpPcieGen5;
  }
}

/**
  Called by SIP library to check if CLKREQ signal is being pulled down.

  @param[in] pInst   Point of pInst
**/
BOOLEAN
PcieIsClkReqPulledDown (
  IN IP_PCIE_INST       *pInst
  )
{
  UINT32                      RpIndex;

  RpIndex = pInst->RpIndex;

  if (!IsClkReqAssigned (PchClockUsagePchPcie0 + RpIndex)) {
    return FALSE;
  }
  if (pInst->PcieRpCommonConfig.ClkReqDetect &&
      EFI_ERROR (CheckClkReq (PchClockUsagePchPcie0 + RpIndex))) {
    DEBUG ((DEBUG_INFO, "CLKREQ is not Low, disabling power management for RP %d.\n", RpIndex));

    return FALSE;
  }
  return TRUE;
}

/**
  Checks if root port is disabled in controller.
  @param[in] ControllerDev        Pointer to controller device.
  @param[in] RpIndexInController  Which root port in controller to disable.
  @retval TRUE   Root port is enabled.
  @retval FALSE  Root port is disabled.
**/
BOOLEAN
PcieSipIsRpEnabledInController (
  IN IP_PCIE_INST        *pInst,
  IN UINT32              RpIndexInController
  )
{
  UINT8  RpDisabled;

  if (RpIndexInController >= 4) {
    return FALSE;
  }

  RpDisabled = (UINT8) IpWrRegRead (pInst->RegCntxt_Prvt_Cfg, PCD_PCIE_PRVT_REG + 2, IpWrRegFlagSize8Bits);
  return !((RpDisabled >> RpIndexInController) & 0x1);
}

/**
  The init function of Pcie IPWR

  @param[in] Index                Pcie RootPort Index
  @param[in] PcieType             PCIe Integration Type
  @param[in] IP_PCIE_INST         *pInst

  @retval     Pointer to Ip Inst
**/

IP_PCIE_INST*
DomainPcieInstSingleInit (
  IN  UINT32       Index,
  IN  BOOLEAN      PcieType,
  IN  IP_PCIE_INST *pInst
  )
{
  UINT32                DetectTimeoutUs;
  UINT32                DetectTimer;
  PCIE_DEV_INFO         DevInfo;
  UINT8                 MaxLinkSpeed;
  UINT64                RpBase;
  IP_WR_REG_INFO       *RegInfo;
  LCAP_PCIE_CFG_STRUCT  LcapCfg;
  EFI_STATUS            Status;
  PCH_PCIE_CONFIG      *PcieConfig;
  SI_POLICY_PPI        *SiPolicy;
  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi;
  VTD_CONFIG           *VtdConfig;
  IOC_ROOT_PORT_DEST_ID IocRpDestinationId;
  PSF_PORT_DEST_ID      PsfRpDestinationId;
  TC_VC_MAP             TcVcMap;
  STATIC IP_PCIE_INST      *pInstController = NULL;

  if (pInst == NULL) {
    DEBUG ((DEBUG_WARN, "%a: Invalid pInst\n", __FUNCTION__));
    ASSERT (FALSE);
    return NULL;
  }

  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID *) &PcieConfig);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );

  if (EFI_ERROR (Status)) {
    return NULL;
  }

  if (pInst->Prefix.pName == NULL) {
    // force valid name for debug printing
    pInst->Prefix.pName = "PcieAgent";
  }
  DEBUG ((DEBUG_INFO, "DomainPcieInstSingleInit(%d) start \n", Index));

  //
  // Init print Context
  //
  pInst->PrintError   = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoError;
  pInst->PrintWarning = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoWarning;
  pInst->PrintLevel1  = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoLevel1;

  pInst->Integration   = PcieType;
  pInst->SipPhyVersion = PcieRpGetSipPhyVer (Index);

  pInst->PrivateConfig.ClkReqEnable = 0; // will update after PcieSipPreLinkActiveProgramming ()

  GetPcieTopology (pInst, Index);

  if (pInst->IsController == TRUE) {
     pInstController = pInst;
  }
  pInst->ControllerInst = (void*)pInstController;

  //
  // Update RegInfo
  //
  PcieUpdateRegCntxt (pInst, Index);

  RegInfo = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Cfg_Pri;
  RpBase  = PCI_SEGMENT_LIB_ADDRESS (RegInfo->RegType.Pci.Seg, RegInfo->RegType.Pci.Bus, RegInfo->RegType.Pci.Dev, RegInfo->RegType.Pci.Fun, 0);

  pInst->SipVersion    = (IP_PCIE_SIP_VERSION) IpPcieGetVersion (pInst, IpCsiVerIdHwIp);

  //
  // Update MemInfo
  //
  PcieUpdateMemCntxt (pInst, Index);

  IpPcieInstInit (pInst);

  VtdConfig = NULL;
#if FixedPcdGetBool (PcdVtdEnable) == 1
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *) &VtdConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Status = %r\n", Status));
  }
#endif

  if (VtdConfig == NULL || IsVtdDisabled(VtdConfig)) {
    pInst->PrivateConfig.VtdEnabled = FALSE;
  } else {
    pInst->PrivateConfig.VtdEnabled = TRUE;
  }
  pInst->PrivateConfig.InterruptPin   = ItssGetDevIntPin (SiPolicy, (UINT8) RegInfo->RegType.Pci.Dev, (UINT8) RegInfo->RegType.Pci.Fun);//use itss api directly as vtd enable
  pInst->PrivateConfig.ClkReqAssigned = IsClkReqAssigned (PchClockUsagePchPcie0 + (pInst->RpIndex));

  //
  // Update Config Block Deafults from setup option values
  //
  PcieUpdateConfig (pInst, PcieConfig);

  //
  // Update Private Config
  //
  PcieUpdatePrivateConfig (pInst, PcieConfig);

  PcieUpdateControllerConfig (pInst);

  IpPcieIpInit (pInst);

  pInst->PrivateConfig.RootPortDisable = FALSE;

  if (PcieIsRpOwningLanes (pInst) == FALSE ||
      !PcieSipIsRpEnabledInController (pInst->ControllerInst, pInst->PrivateConfig.RootPortIndexInController)) {
    pInst->PrivateConfig.RootPortDisable = TRUE;
  }

  IpPciePreLinkActiveProgramming (pInst);

  //
  //Init DTR status
  //
  if (pInst->PcieRpCommonConfig.EnableDtr){
    PRINT_LEVEL1 ("DTR Ready: assume on Port %d\n", pInst->RpIndex);
    IpPcieSetDtrStat(pInst, IpPcieDtrReady);
  }

  pInst->PrivateConfig.ClkReqEnable = pInst->PrivateConfig.ClkReqAssigned;
  if (!pInst->PrivateConfig.RootPortDisable) {
    DetectTimer = 0;
    DetectTimeoutUs = pInst->PcieRpCommonConfig.DetectTimeoutMs * 1000;
    if (IpPcieSipIsDevicePresent (pInst, DetectTimeoutUs, &DetectTimer) || (pInst->PcieRpCommonConfig.ComplianceTestMode == TRUE)) {
      //
      // At this point in boot, CLKREQ pad is still configured as GP input and doesnt' block clock generation
      // regardless of input state. Before switching it to native mode when it will start gating clock, we
      // verify if CLKREQ is really connected. If not, pad will not switch and power management
      // will be disabled in rootport.
      // By the time this code runs device can't have CPM or L1 substates enabled, so it is guaranteed to pull ClkReq down.
      // If ClkReq is detected to be high anyway, it means ClkReq is not connected correctly.
      // Checking pad's input value is primarily a measure to prevent problems with long cards inserted into short
      // open-ended PCIe slots on motherboards which route PRSNT signal to CLKREQ. Such config causes CLKREQ signal to float.
      //
      if (pInst->PcieRpCommonConfig.ClkReqDetect &&
        pInst->PrivateConfig.ClkReqAssigned &&
        !PcieIsClkReqPulledDown (pInst)) {
          pInst->PrivateConfig.ClkReqEnable = FALSE;
      }
    }
  }

  ///
  /// Configure power management applicable to all port including disabled ports.
  ///
  if (pInst->PrivateConfig.ClkReqEnable) {
    EnableClkReq (PchClockUsagePchPcie0 + pInst->RpIndex);
  } else {
    ChangePadRstCfg (PchClockUsagePchPcie0 + pInst->RpIndex);
  }

  if (pInst->PrivateConfig.RootPortDisable) {
    IpPcieDisableRootPort (pInst);
    if (pInst->IocDecoded) {
      IocDisablePcieRootPort (pInst->RpIndex);
    } else {
      PtlPcdPsfDisablePcieRootPort (pInst->RpIndex);
    }
    DisableClock (PchClockUsagePchPcie0 + pInst->RpIndex);

    //DTR not need since RP disabled
    if (pInst->PcieRpCommonConfig.EnableDtr) {
      PRINT_LEVEL1 ("DTR Not Need: Port %d disabled\n", pInst->RpIndex);
      IpPcieSetDtrStat(pInst, IpPcieDtrNotNeed);
    }
    return pInst;
  }

  PcieGetDeviceInfo (pInst, pInst->PrivateConfig.BusMin, &DevInfo);

  PcieDecreasedDeEmphasis (pInst, DevInfo);

  PcieWifiLinkSpeedSyncWorkaround (pInst, DevInfo);

  PcieRpDownStreamCcConfiguration (pInst);

  IpPcieRpInit (pInst);

  if (pInst->PrivateConfig.EnablePort80Decode) {
    PsfRpDestinationId = PtlPcdPsfPcieDestinationId (pInst->RpIndex);
    IocRpDestinationId.PsfId       = (UINT8)PsfRpDestinationId.Fields.PsfId;
    IocRpDestinationId.PortGroupId = (UINT8)PsfRpDestinationId.Fields.PortGroupId;
    IocRpDestinationId.PortId      = (UINT8)PsfRpDestinationId.Fields.PortId;
    IocRpDestinationId.ChannelId   = (UINT8)PsfRpDestinationId.Fields.ChannelId;
    IocSetIoPort80Decode (IocRpDestinationId);
  }

  PcieRpDownstreamConfiguration (pInst);

  if (pInst->PrivateConfig.MultiVcEnabled) {
    ZeroMem (&TcVcMap, sizeof (TC_VC_MAP));
    TcVcMap.TcVcMap[0] = 0x01;
    TcVcMap.TcVcMap[1] = 0xFE;
    PcieRpMultiVcConfiguration (pInst, &TcVcMap);
  }

  LcapCfg.Data = PciSegmentRead32 (RpBase + LCAP_PCIE_CFG_REG);
  MaxLinkSpeed = MIN ((UINT8)LcapCfg.Bits.mls, DevInfo.MaxLinkSpeed);

  if (pInst->PcieRpCommonConfig.EnableDtr) {
    MaxLinkSpeed = PcieDtrSpeedDetermine (pInst, MaxLinkSpeed);
  }
  IpPcieRpSpeedChangeStart (pInst, MaxLinkSpeed);
  TimeStampAfterPcieLinkRetrainSet = GetTimeInNanoSecond (GetPerformanceCounter ());

  return pInst;
}

/**
  DomainInit for Initilizing IP Instances

  @param[in] MaxRootPortNum       Pcie Max RootPort number
  @param[in] PcieType             PCIe Integration Type

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    pInst configuration has bad size
**/
EFI_STATUS
DomainPcieInit (
  IN  UINT32  MaxRootPortNum,
  IN BOOLEAN  PcieType
  )
{
  UINT32            Index;
  IP_PCIE_INST      **pInstArray;
  IP_PCIE_INST      *pInst;
  EFI_HOB_GUID_TYPE *GuidHob;

  Index = 0;

  DEBUG ((DEBUG_INFO, "DomainPcieInit start, MaxRootPortNum %d\n", MaxRootPortNum));
  //
  // Calling PHY and FIA Inst Init before calling Pcie IP Inst Init
  //
  IpPciePhyInstInit ();
  IpPcieFiaInstInit ();

  pInstArray = (IP_PCIE_INST **) AllocateZeroPool (sizeof (IP_PCIE_INST *) * MaxRootPortNum);
  if (pInstArray == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  GuidHob = GetFirstGuidHob (&gIpPcieInstHobGuid);
  while (GuidHob != NULL) {
    pInst = (IP_PCIE_INST *) GET_GUID_HOB_DATA (GuidHob);

    pInstArray [Index] = DomainPcieInstSingleInit (Index, PcieType, pInst);
    Index++;
    GuidHob = GetNextGuidHob (&gIpPcieInstHobGuid, GET_NEXT_HOB(GuidHob));
  }

  DEBUG ((DEBUG_INFO, "DomainPcieInit end\n"));

  return EFI_SUCCESS;
}

/**
  Calculate the required timeout value after setting RL in SpeedChangeStart API.
  Required Timeout value is difference between current CPU timestamp value and CPU timestamp when RL is set.

  @retval  UINT32  Required Timeout value in milli seconds
**/
UINT32
PcieGetTimeoutValue (
 )
{
  UINT64  TimeElapsed;
  UINT64  RequiredTimeout;

  TimeElapsed  = GetTimeInNanoSecond (GetPerformanceCounter ()) - TimeStampAfterPcieLinkRetrainSet;
  if (TimeElapsed < TIMEOUT_100_MS) {
    RequiredTimeout = TIMEOUT_100_MS - TimeElapsed;
    return ((UINT32)DivU64x32 (RequiredTimeout, 1000u));
  }
  return 0;
}