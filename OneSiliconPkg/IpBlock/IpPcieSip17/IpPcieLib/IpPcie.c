/**@file
  IP PCIE Init library.

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

#include <IpCpcie.h>
#include <IpWrapper.h>
#include <IpWrapper64Bit.h>
#include <IpPcieRegs.h>
#include <IpPcieVersion.h>
#include <IpPcieDefines.h>
#include "IpPcieSipInitLib.h"

#define DISABLE_TIMEOUT      5000

#define ASPM_L1_NO_LIMIT      0xFF
#define ASPM_L0s_NO_LIMIT     0x7
#define ASPM_L1_LATENCY_LIMIT 0x7

#define MAX_T_POWER_ON_SCALE  2

#define PCI_CLASSCODE_OFFSET             0x09
#define PCI_CLASS_SYSTEM_PERIPHERAL      0x08
#define PCI_SUBCLASS_PIC                 0x00
#define PCI_IF_APIC_CONTROLLER           0x10
#define PCI_IF_APIC_CONTROLLER2          0x20

#define LINK_TIMEOUT                     1500

#pragma pack(push, 1)
typedef union {
  struct {
    UINT32  RequesterCapable       : 1;
    UINT32  ResponderCapable       : 1;
    UINT32  RootCapable            : 1;
    UINT32  Reserved               : 5;
    UINT32  LocalClockGranularity  : 8;
    UINT32  Reserved2              : 16;
  } Bits;
  UINT32  Uint32;
} PTM_CAPS;

typedef union {
  struct {
    UINT32 Enable                 : 1;
    UINT32 RootSelect             : 1;
    UINT32 Reserved               : 6;
    UINT32 EffectiveGranularity   : 8;
    UINT32 Reserved2              : 16;
  } Bits;
  UINT32  Uint32;
} PTM_CTRL;

typedef struct {
  UINT32 MaxSnoopLatencyValue         : 10;
  UINT32 MaxSnoopLatencyScale         : 3;
  UINT32 MaxNoSnoopLatencyValue       : 10;
  UINT32 MaxNoSnoopLatencyScale       : 3;
  UINT32 Reserved                     : 6;
} LTR_LIMIT;

typedef union {
  struct {
  UINT32 MaxSnoopLatencyValue         : 10;
  UINT32 MaxSnoopLatencyScale         : 3;
  UINT32 MaxSnoopLatencyRequirement   : 1;
  UINT32 MaxNoSnoopLatencyValue       : 10;
  UINT32 MaxNoSnoopLatencyScale       : 3;
  UINT32 MaxNoSnoopLatencyRequirement : 1;
  UINT32 ForceOverride                : 1;
  UINT32 Reserved                     : 3;
  } Bits;
  UINT32  Uint32;
} LTR_OVERRIDE;

typedef union {
  struct {
    UINT32 PmL12     : 1;
    UINT32 PmL11     : 1;
    UINT32 AspmL12   : 1;
    UINT32 AspmL11   : 1;
    UINT32 L1pss     : 1;
    UINT32 L1sses    : 1;
    UINT32 L1sseis   : 1;
    UINT32 Reserved0 : 1;
    UINT32 Cmrt      : 8;
    UINT32 TpoScale  : 2;
    UINT32 Reserved1 : 1;
    UINT32 TpoValue  : 5;
    UINT32 Reserved2 : 8;
  } Bits;
  UINT32  Uint32;
} L1SS_CAPS;

typedef union {
  struct {
    UINT32  L0sSupported          : 1;
    UINT32  L1Supported           : 1;
    UINT32  L0sAcceptableLatency  : 3; // encoded as in hardware register
    UINT32  L1AcceptableLatencyUs : 8; // decoded to microseconds
    UINT32  LinkL0sExitLatency    : 3; // encoded as in hardware register
    UINT32  LinkL1ExitLatencyUs   : 8; // decoded to microseconds
    UINT32  Reserved0             : 8;
    } Bits;
    UINT32  Uint32;
} ASPM_CAPS;
#pragma pack(pop)

/**
  Init IP

  @param[in]  pInst instance

  @retval         IpCsiStsSuccess on success
  @retval         !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpPcieIpInit (
  IP_PCIE_INST  *pInst
  )
{
  BAR0_PCIE_CFG_STRUCT           Bar0;
  BAR1_PCIE_CFG_STRUCT           Bar1;
  MEMBARCTL_PCIE_MEM_RCRB_STRUCT MemBarCtl;
  CMD_PCIE_CFG_STRUCT            Cmd;
  UINT32                         LowerBaseAddress;
  UINT64                         MemRcrb;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  MemRcrb       = pInst->MemRcrbBase;
  if(MemRcrb != 0)
  {
    PRINT_LEVEL1 ("MemRcrb(%d) = 0x%lx\n", pInst->RpIndex, MemRcrb);

    Bar0.Data     = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, BAR0_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Bar0.Bits.mmt = 0x2;
    Bar0.Bits.p   = 0x1;
    LowerBaseAddress = (UINT32) (MemRcrb & 0xFFFFFFFF);
    Bar0.Bits.lba    = LowerBaseAddress >> 4;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, BAR0_PCIE_CFG_REG, Bar0.Data, IpWrRegFlagSize32Bits);

    Bar1.Data     = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, BAR1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Bar1.Bits.uba = 0x0;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, BAR1_PCIE_CFG_REG, Bar1.Data, IpWrRegFlagSize32Bits);

    MemBarCtl.Data          = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, MEMBARCTL_PCIE_MEM_RCRB_REG + 0x1000, IpWrRegFlagSize32Bits);
    MemBarCtl.Bits.membaren = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, MEMBARCTL_PCIE_MEM_RCRB_REG + 0x1000, MemBarCtl.Data, IpWrRegFlagSize32Bits); // MEMBARCTL.MEMBAREN needs to be programed through SB access.

    Cmd.Data      = (UINT16)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CMD_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    Cmd.Bits.bme  = 0x1;
    Cmd.Bits.mse  = 0x1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, CMD_PCIE_CFG_REG, Cmd.Data, IpWrRegFlagSize16Bits);
  }
  else {
    Bar0.Data     = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, BAR0_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Bar0.Bits.mmt = 0x0;
    Bar0.Bits.p   = 0;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, BAR0_PCIE_CFG_REG, Bar0.Data, IpWrRegFlagSize32Bits);
  }

  IpPcieGetVersion (pInst, IpCsiVerIdCsi);
  IpPcieGetVersion (pInst, IpCsiVerIdIpFw);
  IpPcieGetVersion (pInst, IpCsiVerIdHwIp);
  IpPcieGetVersion (pInst, IpCsiVerIdIpFwIntf);
  IpPcieGetVersion (pInst, IpCsiVerIdHwIpCustom);
  IpPcieGetVersion (pInst, IpCsiVerIdIpFwIntfExt);

  SipEarlyDecodeEnable (pInst);

  //
  // Perform Power down mapping before enabling powermanagement
  //
  SipConfigurePowerDownMapping (pInst);
  //
  // ResetPrep Programming
  //
  SipConfigureResetPrep (pInst);

  //IpPcieTaskDispatch (pInst, PcieCallTable);
  return IpCsiStsSuccess;
}

/**
  Default Configuration for Pinst Members

  @param[in] pInst  The instance, already allocated
**/
void
PcieLoadDefaultConfig (
  IP_PCIE_INST                  *pInst
  )
{
  pInst->PcieRpCommonConfig.RpFunctionSwap = TRUE;

  pInst->PcieRpCommonConfig.Aspm                   = IpPchPcieAspmAutoConfig;
  pInst->PcieRpCommonConfig.PmSci                  = FALSE;
  pInst->PcieRpCommonConfig.AcsEnabled             = TRUE;
  pInst->PcieRpCommonConfig.PtmEnabled             = TRUE;
  pInst->PcieRpCommonConfig.MaxPayload             = IpPchPcieMaxPayload512;
  pInst->PcieRpCommonConfig.SlotImplemented        = TRUE;
  pInst->PcieRpCommonConfig.PhysicalSlotNumber     = (UINT16) pInst->RpIndex;
  pInst->PcieRpCommonConfig.L1Substates            = IpPchPcieL1SubstatesL1_1_2;
  pInst->PcieRpCommonConfig.L1Low                  = TRUE;
  pInst->PcieRpCommonConfig.EnableCpm              = TRUE;
  pInst->PcieRpCommonConfig.RelaxedOrder           = TRUE;
  pInst->PcieRpCommonConfig.NoSnoop                = TRUE;
  pInst->PcieRpCommonConfig.LedManagement          = FALSE;

  pInst->PcieRpCommonConfig.LtrEnable              = TRUE;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency                           = 0x100F;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency                         = 0x100F;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode                     = 2;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMultiplier               = 2;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideValue                    = 60;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode                  = 2;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier            = 2;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideValue                 = 60;
  pInst->PcieRpCommonConfig.PcieCfgDump                                                  = 0;
  pInst->PcieRpCommonConfig.EqPhBypass                                                   = 0;

  pInst->PcieRpCommonConfig.HotPlug                       = FALSE;
  pInst->PcieRpCommonConfig.TransmitterHalfSwing          = FALSE;
  pInst->PcieRpCommonConfig.AdvancedErrorReporting        = TRUE;
  pInst->PcieRpCommonConfig.UnsupportedRequestReport      = TRUE;
  pInst->PcieRpCommonConfig.NoFatalErrorReport            = TRUE;
  pInst->PcieRpCommonConfig.FatalErrorReport              = TRUE;
  pInst->PcieRpCommonConfig.CorrectableErrorReport        = TRUE;
  pInst->PcieRpCommonConfig.SystemErrorOnFatalError       = FALSE;
  pInst->PcieRpCommonConfig.SystemErrorOnNonFatalError    = FALSE;
  pInst->PcieRpCommonConfig.SystemErrorOnCorrectableError = FALSE;
  pInst->PcieRpCommonConfig.ClkReqDetect                  = TRUE;
  pInst->PcieRpCommonConfig.EnablePeerMemoryWrite         = FALSE;
  pInst->PcieRpCommonConfig.ClockGating                   = TRUE;
  pInst->PcieRpCommonConfig.PowerGating                   = TRUE;
  pInst->PcieRpCommonConfig.PcieFomsCp                    = 0;
  pInst->PcieRpCommonConfig.CompletionTimeout             = 0;
  pInst->PcieRpCommonConfig.PcieSpeed                     = IpPcieAuto;
  pInst->PcieRpCommonConfig.DetectTimeoutMs               = 0;
  pInst->PcieRpCommonConfig.LinkDownGpios                 = 0;
  pInst->PcieRpCommonConfig.OverrideEqualizationDefaults  = 0;
  pInst->PcieRpCommonConfig.ComplianceTestMode            = 0;
  pInst->PcieRpCommonConfig.EnablePort8xhDecode           = 0;
  pInst->PcieRpCommonConfig.VisaClockGating               = TRUE;
  pInst->PcieRpCommonConfig.AutoPowerGating               = TRUE;
  pInst->Integration                                      = IpPchPcie;
  //
  // Initialize PrivateConfig members of pInst.
  //
IpWrMemset (pInst->MemCntxt, &(pInst->PrivateConfig), 0, sizeof (pInst->PrivateConfig));

}

/**
  InstInit for this version of IP
  @param[in] pInst  The instance, already allocated

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpPcieInstInit (
  IP_PCIE_INST                *pInst
  )
{

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  if (pInst->Prefix.pName == NULL) {
    // force valid name for debug printing
    pInst->Prefix.pName = "PcieAgent";
  }

  PRINT_LEVEL1 ("%s(%s):\n", __FUNCTION__, pInst->Prefix.pName);

  //
  // Load Config Block Deafults
  //
  PcieLoadDefaultConfig (pInst);

  PRINT_LEVEL1 ("IpPcieInstInit End\n");
  return IpCsiStsSuccess;
}

/**
  Check for device presence with timeout.

  @param[in]     pInst      *pInst
  @param[in]     TimeoutUs  Timeout in microseconds
  @param[in,out] Timer      Timer value, must be initialized to zero
                            before the first call of this function.
**/
BOOLEAN
IpPcieSipIsDevicePresent (
  IP_PCIE_INST        *pInst,
  UINT32              TimeoutUs,
  UINT32              *Timer
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return FALSE;
  }

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  if (Timer == NULL) {
     return FALSE;
  }
  while (TRUE) {
    if (SipIsEndpointConnected (pInst)) {
      return TRUE;
    }
    if (*Timer < TimeoutUs) {
      IpWrDelayUs (pInst->TimeCntxt, 10);
      *Timer += 10;
    } else {
      break;
    }
  }
  return FALSE;
}

/**
  Pre Link Active Programming

  @param[in] pInst          *pInst

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpPciePreLinkActiveProgramming (
  IP_PCIE_INST        *pInst
  )
{
  AECR1G3_PCIE_CFG_STRUCT            Aecr1g3;
  XCAP_PCIE_CFG_STRUCT               Xcap;
  UINT32                             DetectTimeoutUs;
  UINT32                             DetectTimer;
  MPHYCAPCFG_PCIE_MEM_RCRB_STRUCT    MPhyCapCfg;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("%s Start %d\n", __FUNCTION__, pInst->RpIndex);

  if ((UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, 0, IpWrRegFlagSize16Bits) == 0xFFFF) {
    pInst->PrivateConfig.RootPortDisable = TRUE;
  }

  if (!pInst->PrivateConfig.RootPortDisable) {
    ///
    /// Program PCI Express Capabilities register 0x0042
    /// Set the Slot Implemented Bit.
    /// PCH BIOS Spec section 8.2.3, The System BIOS must
    /// initialize the "Slot Implemented" bit of the PCI Express* Capabilities Register,
    /// XCAP Dxx:Fn:42h[8] of each available and enabled downstream root port.
    /// Ports with hotplug capability must have SI bit set
    /// The register is write-once so must be written even if we're not going to set SI, in order to lock it.
    ///
    /// This must happen before code reads PresenceDetectState, because PDS is invalid unless SI is set
    ///
    if (pInst->PcieRpCommonConfig.SlotImplemented || pInst->PcieRpCommonConfig.HotPlug) {
      Xcap.Data    = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, XCAP_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Xcap.Bits.si = 1;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, XCAP_PCIE_CFG_REG, Xcap.Data, IpWrRegFlagSize16Bits);
    } else {
      Xcap.Data    = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, XCAP_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Xcap.Bits.si = 0;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, XCAP_PCIE_CFG_REG, Xcap.Data, IpWrRegFlagSize16Bits);
    }

    DetectTimer = 0;
    DetectTimeoutUs = pInst->PcieRpCommonConfig.DetectTimeoutMs * 1000;

    if (IpPcieSipIsDevicePresent (pInst, DetectTimeoutUs, &DetectTimer) || (pInst->PcieRpCommonConfig.ComplianceTestMode == TRUE)) {
      PRINT_LEVEL1 ("Port %d has a device attached.\n", pInst->RpIndex);
    } else {
      if (pInst->PcieRpCommonConfig.HotPlug == FALSE) {
        pInst->PrivateConfig.RootPortDisable = TRUE;
      }
    }
  }

  if (SipGetMaxLinkSpeed (pInst) >= 3) {
    if (pInst->PcieRpCommonConfig.OverrideEqualizationDefaults) {
      IpWrMemcpy (pInst->MemCntxt, &pInst->PrivateConfig.Gen3EqSettings.PlatformSettings, &pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings, sizeof (IP_PCIE_LINK_EQ_PLATFORM_SETTINGS));
      IpWrMemcpy (pInst->MemCntxt, &pInst->PrivateConfig.Gen4EqSettings.PlatformSettings, &pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings, sizeof (IP_PCIE_LINK_EQ_PLATFORM_SETTINGS));
      IpWrMemcpy (pInst->MemCntxt, &pInst->PrivateConfig.Gen5EqSettings.PlatformSettings, &pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings, sizeof (IP_PCIE_LINK_EQ_PLATFORM_SETTINGS));
    }
    SipLinkEqualizeInit (pInst, &pInst->PrivateConfig.Gen3EqSettings, &pInst->PrivateConfig.Gen4EqSettings, &pInst->PrivateConfig.Gen5EqSettings);
  }

  //
  // PhyLanePgEnable depends on the port supporting CLKREQ# or being disabled.
  //
  SipConfigurePortBasePowerManagement (
    pInst
    );

  ///
  /// Program Advance Extended Control Register 1 Group 3 register 0x0C80
  /// Set C80h[0] DCDCCTDT to 0x0.
  ///
  Aecr1g3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, AECR1G3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Aecr1g3.Bits.dcdcctdt = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, AECR1G3_PCIE_CFG_REG, Aecr1g3.Data, IpWrRegFlagSize32Bits);

  if (pInst->IsController == TRUE) {
    SipConfigureControllerBasePowerManagement (
      pInst
    );
    //
    // Program MAC PHY Capability Configurations register 0x15a8
    //
    MPhyCapCfg.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, MPHYCAPCFG_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
    MPhyCapCfg.Bits.mlsosgssvcc = 0;
    IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, MPHYCAPCFG_PCIE_MEM_RCRB_REG, MPhyCapCfg.Data, IpWrRegFlagSize32Bits);

    SipConfigurePtm (pInst, &pInst->PrivateConfig.PtmConfig);

    SipConfigureLtrSubstraction (
      pInst,
      &pInst->PrivateConfig.L1StandardConfig,
      &pInst->PrivateConfig.L1p1Config,
      &pInst->PrivateConfig.L1p2Config,
      &pInst->PrivateConfig.LtrSubL11Npg
    );
  }

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
  return IpCsiStsSuccess;
}

/**
  This function disables the root ports that are either fuse disabled, disabled via setup or has No Device Attached

  @param[in] IP_PCIE_INST       *pInst

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
IpPcieDisableRootPort (
  IP_PCIE_INST         *pInst
  )
{
  UINT32                      LoopTime;
  UINT32                      TargetState;
  UINT32                      LinkActive;
  LCTL_PCIE_CFG_STRUCT        Lctl;
  LSTS_PCIE_CFG_STRUCT        Lsts;
  PCIEALC_PCIE_CFG_STRUCT     PcieAlc;
  PHYCTL4_PCIE_CFG_STRUCT     PhyCtl4;
  PCIESTS1_PCIE_CFG_STRUCT    PcieSts;
  PMCS_PCIE_CFG_STRUCT        Pmcs;
  FCTL2_PCIE_MEM_RCRB_STRUCT  Fctl2;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("%s Start (%d) \n", __FUNCTION__, pInst->RpIndex);
  if ((UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, ID_PCIE_CFG_REG, IpWrRegFlagSize16Bits) == 0xFFFF) {
    PRINT_LEVEL1 (("DID read through SideBand returns FF's\n"));
    return IpCsiStsError;
  }
  /// Link Status register 0x0052
  Lsts.Data  = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);//access LSTS using dword-aligned read
  LinkActive = Lsts.Bits.la;

  if (LinkActive) {
    ///
    /// If device is present disable the link.
    ///
    PRINT_LEVEL1 (("Disabling the link.\n"));
    /// PCI Express Additional Link Control register 0x0338
    PcieAlc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEALC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PcieAlc.Bits.blkdqda = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCIEALC_PCIE_CFG_REG, PcieAlc.Data, IpWrRegFlagSize32Bits);

    /// PCI Express Additional Link Control register 0x0050
    Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    Lctl.Bits.ld = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);

    if(pInst->SipVersion >= IpPcieSip17){
      Fctl2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, FCTL2_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
      Fctl2.Bits.blknat = 1;
      IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, FCTL2_PCIE_MEM_RCRB_REG, Fctl2.Data, IpWrRegFlagSize32Bits);
    }

    for (LoopTime = 0; LoopTime < 5000; LoopTime++) {
      /// PCI Express Status 1 register 0x0328
      PcieSts.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIESTS1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      if ((PcieSts.Bits.ltsmstate == V_PCIESTS1_LTSMSTATE_DETRDY) ||
         (PcieSts.Bits.ltsmstate == V_PCIESTS1_LTSMSTATE_DISWAITPG))  {
        break;
      }
      IpWrDelayUs (pInst->TimeCntxt, 10);
    }
    /// PCI Express Additional Link Control register 0x0050
    Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    Lctl.Bits.ld = 0;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);
  } else {
    ///
    /// Otherwise if device is not present perform the following steps using sideband access:
    /// 1.  Set B0:Dxx:Fn:338h[26] = 1b
    /// 2.  Poll B0:Dxx:Fn:328h[31:24] until 0x1 with 50ms timeout
    /// 3.  Set B0:Dxx:Fn +408h[27] =1b
    ///

    PRINT_LEVEL1 (("Stopping the port.\n"));
    /// PCI Express Additional Link Control register 0x0338
    PcieAlc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEALC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PcieAlc.Bits.blkdqda = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCIEALC_PCIE_CFG_REG, PcieAlc.Data, IpWrRegFlagSize32Bits);

    TargetState = V_PCIESTS1_LTSMSTATE_DETRDY;
    for (LoopTime = 0; LoopTime < DISABLE_TIMEOUT; LoopTime++) {
      /// PCI Express Status 1 register 0x0328
      PcieSts.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIESTS1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      if (PcieSts.Bits.ltsmstate == TargetState) {
        break;
      }
      IpWrDelayUs (pInst->TimeCntxt, 10);
    }
    //
    // Show polling status
    //
    if (LoopTime > 0) {
      PRINT_LEVEL1 ("Polling for DETRDY for %dus. PcieSts = 0x%08x\n", LoopTime*10, PcieSts.Data);
    }
  }
  ///
  /// Program Physical Layer And AFE Control 4 register 0x0408
  /// Set offset 408h[27] to 1b to disable squelch.
  ///
  PhyCtl4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PHYCTL4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PhyCtl4.Bits.sqdis = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PHYCTL4_PCIE_CFG_REG, PhyCtl4.Data, IpWrRegFlagSize32Bits);

  /// Program PCI Power Management Control register 0x00A4
  Pmcs.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PMCS_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Pmcs.Bits.ps = V_PMCS_PS_D3H;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PMCS_PCIE_CFG_REG, Pmcs.Data, IpWrRegFlagSize32Bits);
  //
  // Make port disappear from PCI bus
  //
  SipDisableRootPortInController (pInst);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
  return IpCsiStsSuccess;
}

/**
  Performs mandatory Root Port Initialization.
  This function is silicon-specific and configures proprietary registers

  @param[in] IP_PCIE_INST       *pInst

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
IpPcieRpInit (
  IP_PCIE_INST     *pInst
  )
{
  MPC_PCIE_CFG_STRUCT      Mpc;
  PTMCTLR_PCIE_CFG_STRUCT  Ptmctlr;
  PCIEALC_PCIE_CFG_STRUCT  PcieAlc;
  DC_PCIE_CFG_STRUCT       Dc;
  FCUCTL_PCIE_MEM_RCRB_STRUCT Fcuctl;
  RXMC1_PCIE_CFG_STRUCT       Rxmc1;
  RXMC2_PCIE_CFG_STRUCT       Rxmc2;
  PTMECH_PCIE_CFG_STRUCT      PtmEch;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("%s Start (%d) \n", __FUNCTION__, pInst->RpIndex);

  if (pInst->IsController) {
    //
    // For each controller set Initialize Transaction Layer Receiver Control on Link Down
    // and Initialize Link Layer Receiver Control on Link Down.
    // Use sideband access in case 1st port of a controller is disabled
    //
    // This is a survivability setting to recover system from crash on surprise removal
    //
    PcieAlc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEALC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PcieAlc.Bits.rtd3pdsp    = 1;
    PcieAlc.Bits.onpraspml1p = 0;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCIEALC_PCIE_CFG_REG, PcieAlc.Data, IpWrRegFlagSize32Bits);

    Dc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, DC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Dc.Bits.com    = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, DC_PCIE_CFG_REG, Dc.Data, IpWrRegFlagSize32Bits);

    Fcuctl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, FCUCTL_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
    Fcuctl.Bits.fc_cp_fcm = 0;
    IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, FCUCTL_PCIE_MEM_RCRB_REG, Fcuctl.Data, IpWrRegFlagSize32Bits);

    ///
    /// Program R_PCIE_RXMC1[C90]
    /// Set C90[31:26] MSRVS to 0x1F->SNPS E32
    /// Set C90[25:20] MSRTS to 0x1F->SNPS E32, 0x3F->SNPS C16
    /// Set C90[9] MSRM to 0x1
    /// Set C90[3] MVS to 0x1->SNPS E32, 0x0->SNPS C16
    /// Set C90[2] MILRTS to 0x1
    /// Set C90[1] MIUDVMS to 0x1->SNPS E32, 0x0->SNPS C16
    /// Set C90[0] MIESS to 0x0
    ///
    Rxmc1.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, RXMC1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Rxmc1.Bits.msrm   = 0x1;
    Rxmc1.Bits.milrts = 0x1;
    Rxmc1.Bits.miess  = 0x0;

    if (pInst->SipPhyVersion == IP_SE32) {
      Rxmc1.Bits.msrvs   = V_RXMC1_MSRVS;
      Rxmc1.Bits.msrts   = V_RXMC1_MSRTS_SE32;
      Rxmc1.Bits.miudvms = 0x1;
      Rxmc1.Bits.mvs     = 0x1;
    } else {
      Rxmc1.Bits.miudvms = 0x0;
      Rxmc1.Bits.msrts   = V_RXMC1_MSRTS_SC16;
      Rxmc1.Bits.mvs     = 0x0;
    }
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, RXMC1_PCIE_CFG_REG, Rxmc1.Data, IpWrRegFlagSize32Bits);
    PRINT_LEVEL1("RXMC1 : %x\n", Rxmc1.Data);

    ///
    /// Program R_PCIE_RXMC2[C94]
    /// Set C94[19:24] MNOTSS to 0x8->SNPS E32, 0x9->SNPS C16
    /// Set C94[18:13] MMTOS to 0x14
    /// Set C94[12:6] MNOVSS to 0x20 (Gen5)
    /// Set C94[5:0] MMVOS to 0x5 (Gen5)
    ///
    Rxmc2.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, RXMC2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Rxmc2.Bits.mmtos  = V_RXMC2_MMTOS;
    if (pInst->SipPhyVersion == IP_SE32) {
      Rxmc2.Bits.mnotss = V_RXMC2_MNOTSS_SE32;
      Rxmc2.Bits.mnovss = V_RXMC2_MNOVSS;
      Rxmc2.Bits.mmvos  = V_RXMC2_MMVOS;
    } else {
      Rxmc2.Bits.mnotss = V_RXMC2_MNOTSS_SC16;
    }
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, RXMC2_PCIE_CFG_REG, Rxmc2.Data, IpWrRegFlagSize32Bits);
    PRINT_LEVEL1("RXMC2 : %x\n", Rxmc2.Data);
  }

  SipInitRootPort (pInst);

  Ptmctlr.Data      = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PTMCTLR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);

  PtmEch.Data      = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PTMECH_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  if (PtmEch.Data == V_EX_PTM_CID) {
      Ptmctlr.Bits.ptme = 1;
  }
  Ptmctlr.Bits.rs   = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PTMCTLR_PCIE_CFG_REG, Ptmctlr.Data,IpWrRegFlagSize32Bits);

  ///
  /// Program Miscellaneous Port Configuration register 0x00D8
  /// PCH BIOS Spec Section 8.10 PCI Bus Emulation & Port80 Decode Support
  /// The I/O cycles within the 80h-8Fh range can be explicitly claimed
  /// by the PCIe RP by setting MPC.P8XDE, PCI offset D8h[26] = 1 (using byte access)
  /// BIOS must also configure the corresponding DMI registers GCS.RPR and GCS.RPRDID
  /// to enable DMI to forward the Port8x cycles to the corresponding PCIe RP
  /// @TODO:Add condition for PchPciePort8xhDecodePortIndex if required
  if (pInst->PrivateConfig.EnablePort80Decode) {
    Mpc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Mpc.Bits.p8xde = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, Mpc.Data, IpWrRegFlagSize32Bits);
  }

  return IpCsiStsSuccess;
}

/**
  This function checks whether PHY lane power gating is enabled on the port.

  @param[in] pInst  *pInst

  @retval TRUE                      PHY power gating is enabled
  @retval FALSE                     PHY power gating disabled
**/
BOOLEAN
SipIsPhyLanePgEnabled (
  IP_PCIE_INST      *pInst
  )
{
  PCIEPMECTL_PCIE_CFG_STRUCT  PciePmeCtl;

  PciePmeCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  return (PciePmeCtl.Bits.dlsulppge) != 0;
}

/**
  Configures proprietary parts of L1 substates configuration in Root Port

  @param[in] pInst  *pInst
**/
void
SipL1ssProprietaryConfiguration (
  IP_PCIE_INST      *pInst
  )
{
  PCIEPMECTL_PCIE_CFG_STRUCT  PciePmeCtl;

  ///
  /// Disable L1.LOW by setting Dxx:Fn:420[17] = 0b
  ///
  PciePmeCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIEPMECTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PciePmeCtl.Bits.l1le = 0;
  PciePmeCtl.Bits.l1fsoe = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIEPMECTL_PCIE_CFG_REG, PciePmeCtl.Data, IpWrRegFlagSize32Bits);
}

/**
  Perform write operation on RWO fileds to ensure locking of these registers

  @param[in] pInst               *pInst
**/
void
SipLockCapRegisters(
  IP_PCIE_INST    *pInst
)
{
  UINT32  Data32;
  UINT16  Data16;
  UINT8   Data8;
  UINT32  RwoFieldIndex;
  UINT32  RwoFieldSize;

  struct RegOffSetSzInfo{
    UINT32 Offset;    ///Register Addr Offset
    UINT8  width;     ///Register width in bits
  };

  struct RegOffSetSzInfo RwoCfgRegisters[] = {
    { V0VCRC_PCIE_CFG_REG, V0VCRC_PCIE_CFG_WIDTH }, { V1VCRC_PCIE_CFG_REG, V1VCRC_PCIE_CFG_WIDTH }, 
    { DPCECH_PCIE_CFG_REG, DPCECH_PCIE_CFG_WIDTH }, { DLFCAP_PCIE_CFG_REG, DLFCAP_PCIE_CFG_WIDTH }, 
    { LPCR_PCIE_CFG_REG, LPCR_PCIE_CFG_WIDTH }
  };

  struct RegOffSetSzInfo  RwoPvtRegisters[] = {
    { IMRLE_PCIE_PRVT_REG, IMRLE_PCIE_PRVT_WIDTH }, { SRL_PCIE_PRVT_REG, SRL_PCIE_PRVT_WIDTH }, 
    { DRPC_PCIE_PRVT_REG, DRPC_PCIE_PRVT_WIDTH }
  };

  RwoFieldSize = sizeof (RwoCfgRegisters) / sizeof (struct RegOffSetSzInfo);
  for (RwoFieldIndex = 0; RwoFieldIndex < RwoFieldSize; ++RwoFieldIndex) {
    if ((RwoCfgRegisters[RwoFieldIndex].width == 32)) {
      Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, RwoCfgRegisters[RwoFieldIndex].Offset, IpWrRegFlagSize32Bits);
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, RwoCfgRegisters[RwoFieldIndex].Offset, Data32, IpWrRegFlagSize32Bits);
    } else if ((RwoCfgRegisters[RwoFieldIndex].width == 16)) {
      Data16 = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, RwoCfgRegisters[RwoFieldIndex].Offset, IpWrRegFlagSize16Bits);
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, RwoCfgRegisters[RwoFieldIndex].Offset, Data16, IpWrRegFlagSize16Bits);
    } else {
      Data8 = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, RwoCfgRegisters[RwoFieldIndex].Offset, IpWrRegFlagSize8Bits);
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, RwoCfgRegisters[RwoFieldIndex].Offset, Data8, IpWrRegFlagSize8Bits);
    }
  }

  RwoFieldSize = sizeof (RwoPvtRegisters) / sizeof (struct RegOffSetSzInfo);
  for (RwoFieldIndex = 0; RwoFieldIndex < RwoFieldSize; ++RwoFieldIndex) {
    if ((RwoPvtRegisters[RwoFieldIndex].width == 32)) {
      Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Prvt_Cfg, RwoPvtRegisters[RwoFieldIndex].Offset, IpWrRegFlagSize32Bits);
      IpWrRegWrite (pInst->RegCntxt_Prvt_Cfg, RwoPvtRegisters[RwoFieldIndex].Offset, Data32, IpWrRegFlagSize32Bits);
    } else if ((RwoPvtRegisters[RwoFieldIndex].width == 16)) {
      Data16 = (UINT16) IpWrRegRead (pInst->RegCntxt_Prvt_Cfg, RwoPvtRegisters[RwoFieldIndex].Offset, IpWrRegFlagSize16Bits);
      IpWrRegWrite (pInst->RegCntxt_Prvt_Cfg, RwoPvtRegisters[RwoFieldIndex].Offset, Data16, IpWrRegFlagSize16Bits);
    } else {
      Data8 = (UINT8) IpWrRegRead (pInst->RegCntxt_Prvt_Cfg, RwoPvtRegisters[RwoFieldIndex].Offset, IpWrRegFlagSize8Bits);
      IpWrRegWrite (pInst->RegCntxt_Prvt_Cfg, RwoPvtRegisters[RwoFieldIndex].Offset, Data8, IpWrRegFlagSize8Bits);
    }
  }
}

/**
  Initiate Speed change

  @param[in] pInst               *pInst
  @param[in] MaxLinkSpeed         Max Link Speed

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
IpPcieRpSpeedChange (
  IP_PCIE_INST    *pInst,
  UINT8            MaxLinkSpeed
  )
{
  UINT32                            TimeoutCount;
  UINT32                            NumOfRootPortsToTrain;
  MPC2_PCIE_CFG_STRUCT              Mpc2;
  ACGR3S2_PCIE_CFG_STRUCT           Acgr3s2;
  LCTL_PCIE_CFG_STRUCT              Lctl;
  LCTL2_PCIE_CFG_STRUCT             Lctl2;
  LCTL3_PCIE_CFG_STRUCT             Lctl3;
  LSTS_PCIE_CFG_STRUCT              Lsts;
  LSTS2_PCIE_CFG_STRUCT             Lsts2;
  PCIESTS1_PCIE_CFG_STRUCT          Pciests1;
  PX32EQCFG1_PCIE_MEM_RCRB_STRUCT   Px32EqCfg1;
  UINT32                            LtssmState;
  UINT16                            FomsCpValue;
  PL16S_PCIE_CFG_STRUCT             Pl16s;
  G5STS_PCIE_CFG_STRUCT             G5sts;
  EQCFG1_PCIE_CFG_STRUCT            EqCfg1;
  EQCFG4_PCIE_CFG_STRUCT            EqCfg4;
  UINT8                             EqReAttemptCount;
  UINT16                            Tls;
  UINT16                            Cls;

  LtssmState = 0x33; //  LTSSMSTATE L0

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  if (MaxLinkSpeed > IpPcieGen5) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid MaxLinkSpeed\n", __FUNCTION__);
    return IpCsiStsErrorBadParam;
  }

  PRINT_LEVEL1 ("%s Start (%d) \n", __FUNCTION__, pInst->RpIndex);
  PRINT_LEVEL1 ("MaxLinkSpeed = %d \n", MaxLinkSpeed);
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// Set "Link Speed Training Policy", Dxx:Fn + D4h[6] to 1.
  /// Make sure this is after mod-PHY related programming is completed.
  Mpc2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MPC2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Mpc2.Bits.lstp = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MPC2_PCIE_CFG_REG, Mpc2.Data, IpWrRegFlagSize32Bits);

  Acgr3s2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, ACGR3S2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Acgr3s2.Bits.lstptls = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, ACGR3S2_PCIE_CFG_REG, Acgr3s2.Data, IpWrRegFlagSize32Bits);

  SipL1ssProprietaryConfiguration (pInst);

  ///
  /// PCH BIOS Spec Section 8.14 Additional PCI Express* Programming Steps
  /// NOTE: Detection of Non-Complaint PCI Express Devices
  ///
  NumOfRootPortsToTrain = 0;

  if ((UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCI_VENDOR_ID_OFFSET, IpWrRegFlagSize16Bits) == 0xFFFF) {
    PRINT_LEVEL1 ("Device unknown at RootPort %d\n", pInst->RpIndex);
    return IpCsiStsSuccess;
  }

  PRINT_LEVEL1 ("Device present at RootPort %d\n", pInst->RpIndex);
  //Section 6.15 C Step 2
  Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Cls = Lsts.Bits.cls;
  if(Cls == MaxLinkSpeed){
    PRINT_LEVEL1 ("CLS = MLS\n");
    return IpCsiStsSuccess;
  }

  if (MaxLinkSpeed > 1) {
    // Program Link Control 2 register 0x0070
    PRINT_LEVEL1 ("Program TLS to %d\n", MaxLinkSpeed);
    Lctl2.Data     = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    Lctl2.Bits.tls = MaxLinkSpeed;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, Lctl2.Data, IpWrRegFlagSize16Bits);

    // Section 6.15 c step 4
    Lctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    if (Lctl2.Bits.tls == IpPcieGen2) {
      PRINT_LEVEL1 ("TLS is Gen2 so perform RL\n");
      Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Lctl.Bits.rl = 1;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);
      return IpCsiStsSuccess;
    }

    if (SipIsLinkActive (pInst)) {
      //
      // Program Link Control 3 register 0x0A34
      // Retrain the link if device is present
      //
      if (pInst->PcieRpCommonConfig.EqPhBypass == 0) {
        PRINT_LEVEL1 ("Set PE\n");
        Lctl3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
        Lctl3.Bits.pe = 1;
        IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL3_PCIE_CFG_REG, Lctl3.Data, IpWrRegFlagSize32Bits);
      }

      // Program PCI Express Additional Link Control register 0x0050
      Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Lctl.Bits.rl = 1;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);

      pInst->PrivateConfig.LinkRetrainInProgress = TRUE;
      NumOfRootPortsToTrain++;
    }
  }

  FomsCpValue = 0;
  switch (pInst->PcieRpCommonConfig.PcieFomsCp)
  {
    case 0: // Auto
      /// Program Link Control 2 register 0x0070
      Lctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      if (Lctl2.Bits.tls == V_LCTL2_TLS_GEN3) {
        FomsCpValue = 0;
      } else if (Lctl2.Bits.tls == V_LCTL2_TLS_GEN4) {
        FomsCpValue = 1;
      } else if (Lctl2.Bits.tls == V_LCTL2_TLS_GEN5) {
        FomsCpValue = 3;
      }
      break;
    case 1: // Gen3 FOMS
      FomsCpValue = 0;
      break;
    case 2: // Gen4 FOMS
      FomsCpValue = 1;
      break;
    case 3: // Gen3 and Gen4 FOMS
      FomsCpValue = 2;
      break;
    case 4: // Gen5 FOMS
      FomsCpValue = 3;
      break;
    default:
      PRINT_ERROR ( "Invalid Fomsp value\n");
      break;
  }
  PRINT_LEVEL1 ("FomsCpValue : %x\n", FomsCpValue);
  /// Program Equalization Configuration 4 register 0x048C
  EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 ( "FomsCp : %x\n", EqCfg4.Bits.fomscp);
  EqCfg4.Bits.fomscp = FomsCpValue;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, EQCFG4_PCIE_CFG_REG, EqCfg4.Data, IpWrRegFlagSize32Bits);
  EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);

  //
  // 150 ms timeout while checking for link active on retrained link
  //
  for (TimeoutCount = 0; ((NumOfRootPortsToTrain != 0) && (TimeoutCount < LINK_TIMEOUT)); TimeoutCount++) {
    //
    // Delay 100 us
    //
    IpWrDelayUs (pInst->TimeCntxt, 100);
    //
    // Check for remaining root port which was link retrained
    //
    if (pInst->PrivateConfig.LinkRetrainInProgress) {
      Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Cls = Lsts.Bits.cls;
      //
      // Program Link Status register 0x0052
      // If the link is active, clear the bitmap
      //
      Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Pciests1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIESTS1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      if ((Lsts.Bits.la) &&
      (Cls == MaxLinkSpeed) &&
      (Pciests1.Bits.ltsmstate == LtssmState)) {
        switch(MaxLinkSpeed) {
          case IpPcieGen3:
            Lsts2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
            EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
            if(Lsts2.Bits.eqp3s && EqCfg1.Bits.haed) {
              pInst->PrivateConfig.LinkRetrainInProgress = FALSE;
              NumOfRootPortsToTrain--;
              PRINT_LEVEL1 ("Rootport %d At Gen3\n", pInst->RpIndex);
            }
            break;
          case IpPcieGen4:
            Pl16s.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PL16S_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
            EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
            if(Pl16s.Bits.eqp3sg4 && EqCfg4.Bits.px16ghaed){
              pInst->PrivateConfig.LinkRetrainInProgress = FALSE;
              NumOfRootPortsToTrain--;
              PRINT_LEVEL1 ("Rootport %d At Gen4\n", pInst->RpIndex);
            }
            break;
          case IpPcieGen5:
            G5sts.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, G5STS_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
            Px32EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
            if(G5sts.Bits.eq32ph3succ && Px32EqCfg1.Bits.px32ghaed){
              pInst->PrivateConfig.LinkRetrainInProgress = FALSE;
              NumOfRootPortsToTrain--;
              PRINT_LEVEL1 ("Rootport %d At Gen5\n", pInst->RpIndex);
            }
            break;
          default:
            break;
        }
      }
    }
  }

  //
  // Re-attempt 'EqReAttemptCount' times
  // to train ports not trained to Gen speed requested
  // Section 6.15 c step 5 re-attempt part.
  //
  if(NumOfRootPortsToTrain != 0){
    PRINT_LEVEL1 ("Reattempt NumOfRootPortsToTrain %d\n", NumOfRootPortsToTrain);
    for (EqReAttemptCount = 2; EqReAttemptCount > 0; EqReAttemptCount--) {
      Lctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Tls = Lctl2.Bits.tls;
      Cls = Lsts.Bits.cls;
      PRINT_LEVEL1 ("ReAttempting EQ at - EqReAttemptCount %d TargetLinkSpeed %d CurrentLinkSpeed %d\n", EqReAttemptCount, Tls, Cls);

      if(Tls < MaxLinkSpeed){
        if((EqReAttemptCount) && (Cls != Tls) && (Cls >= IpPcieGen3)){
          PRINT_LEVEL1 ("ReAttempt at more than Gen3\n");
          //
          // Set TLS to CLS + 1
          //
          Lctl2.Bits.tls = Cls + 1;
          IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, Lctl2.Data, IpWrRegFlagSize16Bits);
          //
          // Retrain link
          //
          Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
          Lctl.Bits.rl = 1;
          IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);
          NumOfRootPortsToTrain++;
        }
      }

      //
      // 150 ms timeout while checking for link active on retrained link
      //
      for (TimeoutCount = 0; ((NumOfRootPortsToTrain != 0) && (TimeoutCount < LINK_TIMEOUT)); TimeoutCount++) {
        //
        // Delay 100 us
        //
        IpWrDelayUs (pInst->TimeCntxt, 100);
        //
        // Check for remaining root port which was link retrained
        //
        if (pInst->PrivateConfig.LinkRetrainInProgress) {

          Lctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
          Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
          Tls = Lctl2.Bits.tls;
          Cls = Lsts.Bits.cls;

          //
          // Program Link Status register 0x0052
          // If the link is active, clear the bitmap
          //
          Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
          Pciests1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIESTS1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
          if ((Lsts.Bits.la) &&
          (Cls == MaxLinkSpeed) &&
          (Pciests1.Bits.ltsmstate == LtssmState)) {
            switch(Tls) {
              case IpPcieGen3:
                Lsts2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
                EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
                if(Lsts2.Bits.eqp3s && EqCfg1.Bits.haed) {
                  pInst->PrivateConfig.LinkRetrainInProgress = FALSE;
                  NumOfRootPortsToTrain--;
                  PRINT_LEVEL1 ("Rootport %d At Gen3\n", pInst->RpIndex);
                }
                break;
              case IpPcieGen4:
                Pl16s.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PL16S_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
                EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
                if(Pl16s.Bits.eqp3sg4 && EqCfg4.Bits.px16ghaed){
                  pInst->PrivateConfig.LinkRetrainInProgress = FALSE;
                  NumOfRootPortsToTrain--;
                  PRINT_LEVEL1 ("Rootport %d At Gen4\n", pInst->RpIndex);
                }
                break;
              case IpPcieGen5:
                G5sts.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, G5STS_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
                Px32EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
                if(G5sts.Bits.eq32ph3succ && Px32EqCfg1.Bits.px32ghaed){
                  pInst->PrivateConfig.LinkRetrainInProgress = FALSE;
                  NumOfRootPortsToTrain--;
                  PRINT_LEVEL1 ("Rootport %d At Gen5\n", pInst->RpIndex);
                }
                break;
              default:
                break;
            }
          }
        }
      }
    }
  }

  //
  // If 150 ms has timeout, and some link are not active,
  // or CLS still doesn't match MLS, revert back to CLS
  // Section 6.15 c step 6
  //
  if (NumOfRootPortsToTrain != 0) {
    PRINT_LEVEL1 ("Still not trained\n");

    if (pInst->PrivateConfig.LinkRetrainInProgress) {
      //
      // Set TLS to CLS
      //
      Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Cls = Lsts.Bits.cls;
      PRINT_LEVEL1 ("[Rootport %d]Device not trained - reverting to CLS at %d\n", pInst->RpIndex, Cls);
      Lctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Lctl2.Bits.tls = Cls;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, Lctl2.Data, IpWrRegFlagSize16Bits);
      //
      // Retrain link
      //
      Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Lctl.Bits.rl = 1;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);
    }

    //
    // Wait for retrain completion or timeout in 22.5ms. Do not expect failure as
    // port was detected and trained at CLS earlier
    //
    for (TimeoutCount = 0; ((NumOfRootPortsToTrain != 0) && (TimeoutCount < 150)); TimeoutCount++) {
      //
      // Delay 150 us
      //
      IpWrDelayUs (pInst->TimeCntxt, 150);
      //
      // Check for remaining root port which was link retrained
      //
      if (pInst->PrivateConfig.LinkRetrainInProgress) {
        PRINT_LEVEL1 ("Still not trained in the loop - retrain in progress\n");
        //
        // If the link is active, clear the bitmap
        //
        Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
        PRINT_LEVEL1 ("LA status at: %d\n", Lsts.Bits.la);
        Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
        PRINT_LEVEL1 ("CLS status at: %d\n", Lsts.Bits.cls);
        Pciests1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIESTS1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
        PRINT_LEVEL1 ("LTSSM status at: %d\n", Pciests1.Bits.ltsmstate);
        if((Lsts.Bits.la != 0) &&
           (Lsts.Bits.cls == (UINT16)MaxLinkSpeed) &&
           (Pciests1.Bits.ltsmstate == LtssmState)) {
            pInst->PrivateConfig.LinkRetrainInProgress = FALSE;
        }
      }
    }
  }

  if (((pInst->PrivateConfig.Gen3EqSettings.PlatformSettings.PcieLinkEqMethod == IpPcieLinkHardwareEq) ||
      (pInst->PrivateConfig.Gen4EqSettings.PlatformSettings.PcieLinkEqMethod == IpPcieLinkHardwareEq)) && MaxLinkSpeed >= IpPcieGen3) {
    SipDumpHwEqResults (pInst);
  }

  SipResetErrorCounts (pInst);
  SipLockCapRegisters (pInst);
  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);

  return IpCsiStsSuccess;
}

/**
  Assign root port function number in PCIe controller.

  @param[in] pInst                      pInst of Controller Rootport.
  @param[in] RpIndexInController        Which root port in controller to swap function for.
  @param[in] NewFun                     New function number.
  @param[in] SwapWithinController       If TRUE function swap is done within a single controller
  @param[in] SecondRpIndexInController  Second root port for which we want to change function number
  @param[in] SecondRpNewFun             Second root port function number
**/
void
IpPcieAssignRootPortFunctionNumber (
  IP_PCIE_INST    *pInst,
  UINT32          RpIndexInController,
  UINT8           NewFun,
  BOOLEAN         SwapWithinController,
  UINT32          SecondRpIndexInController,
  UINT8           SecondRpNewFun
  )
{
  PCD_PCIE_PRVT_STRUCT PcdPcie;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return;
  }

  PcdPcie.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Prvt_Cfg, PCD_PCIE_PRVT_REG, IpWrRegFlagSize32Bits);

  switch (RpIndexInController)
  {
  case 0:
    PcdPcie.Bits.rp0fn = NewFun;
    break;
  case 1:
    PcdPcie.Bits.rp1fn = NewFun;
    break;
  case 2:
    PcdPcie.Bits.rp2fn = NewFun;
    break;
  case 3:
    PcdPcie.Bits.rp3fn = NewFun;
    break;
  default:
    PRINT_WARNING ("Index of Rootport in Controller Invalid: %d\n", RpIndexInController);
    break;
  }

  if (SwapWithinController) {
    switch (SecondRpIndexInController)
    {
    case 0:
      PcdPcie.Bits.rp0fn = SecondRpNewFun;
      break;
    case 1:
      PcdPcie.Bits.rp1fn = SecondRpNewFun;
      break;
    case 2:
      PcdPcie.Bits.rp2fn = SecondRpNewFun;
      break;
    case 3:
      PcdPcie.Bits.rp3fn = SecondRpNewFun;
      break;
    default:
      PRINT_WARNING ("Index of Rootport in Controller Invalid: %d\n", SecondRpIndexInController);
      break;
    }
  }

  IpWrRegWrite(pInst->RegCntxt_Prvt_Cfg, PCD_PCIE_PRVT_REG, PcdPcie.Data, IpWrRegFlagSize32Bits);

  return;
}

/**
  Enable PCIe root port in controller.

  @param[in] pInst                      Pointer to controller pInst.
  @param[in] RpIndexInController        Root port in controller to be enable.
**/
void
IpPcieEnableRootPortInController (
  IP_PCIE_INST    *pInst,
  UINT32          RpIndexInController
  )
{
  PCD_PCIE_PRVT_STRUCT PcdPcie;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return;
  }

  PcdPcie.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Prvt_Cfg, PCD_PCIE_PRVT_REG, IpWrRegFlagSize32Bits);

  switch(RpIndexInController) {
    case 0:
      PcdPcie.Bits.p0d = 0;
      break;
    case 1:
      PcdPcie.Bits.p1d = 0;
      break;
    case 2:
      PcdPcie.Bits.p2d = 0;
      break;
    case 3:
      PcdPcie.Bits.p3d = 0;
      break;
    default:
      PRINT_WARNING ("Invalid RpIndexInController: 0x%x\n", RpIndexInController);
      break;
  }
  IpWrRegWrite(pInst->RegCntxt_Prvt_Cfg, PCD_PCIE_PRVT_REG, PcdPcie.Data, IpWrRegFlagSize32Bits);

  return;
}

/**
  Checks if root port is enabled in controller.

  @param[in] ControllerInst       Pointer to controller root port.
  @param[in] RpIndexInController  Root port index in controller.

  @retval TRUE   Root port is enabled.
  @retval FALSE  Root port is disabled.
**/
BOOLEAN
IpPcieIsRootPortEnabledInController (
  IP_PCIE_INST        *pInst,
  UINT32              RpIndexInController
  )
{
  BOOLEAN              RpEnabled;
  PCD_PCIE_PRVT_STRUCT PcdPcie;

  PRINT_LEVEL1 ("%s Start\n", __FUNCTION__);

  PcdPcie.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Prvt_Cfg, PCD_PCIE_PRVT_REG, IpWrRegFlagSize32Bits);

  switch (RpIndexInController)
  {
  case 0:
    RpEnabled = !PcdPcie.Bits.p0d;
    break;
  case 1:
    RpEnabled = !PcdPcie.Bits.p1d;
    break;
  case 2:
    RpEnabled = !PcdPcie.Bits.p2d;
    break;
  case 3:
    RpEnabled = !PcdPcie.Bits.p3d;
    break;  
  default:
    PRINT_WARNING ("Invalid RpIndexInController: %x\n", RpIndexInController);
    return FALSE;
  }
  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);
  return RpEnabled;
}

/**
  Returns the PCIe controller configuration (4x1, 1x2-2x1, 2x2, 1x4)

  @param[in] ControllerInst       Pointer to controller root port.

  @retval PCIe controller configuration
**/
IP_PCIE_CONTROLLER_CONFIG
IpPcieGetControllerConfig (
  IP_PCIE_INST  *pInst
  )
{
  STRPFUSECFG_PCIE_CFG_STRUCT StrpFuseCfg;
  IP_PCIE_CONTROLLER_CONFIG   Config;

  PRINT_LEVEL1 ("%s Start\n", __FUNCTION__);
  StrpFuseCfg.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, STRPFUSECFG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (StrpFuseCfg.Data == 0xFFFFFFFF) {
    PRINT_ERROR ("Failed to get controller config, defaulting to Pcie4x1\n");
    return IpPcie4x1;
  }

  Config = StrpFuseCfg.Bits.rpc;
  
  if (Config == 0x4) {
    return IpPcie1x4;
  }

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);
  return Config;
}

/**
  Get version information

  @param[in]  pInst  The instance
  @param[in]  VerId  Version ID

  @retval            0 is returned upon any error
  @retval            return value define by VerId
**/
UINT32
IpPcieGetVersion (
  IP_PCIE_INST  *pInst,
  IP_CSI_VER_ID  VerId
  )
{
  UINT32 Ret = 0;
  UINT32 MajorVersion;
  UINT32 MinorVersion;
  IPRV_PCIE_MEM_RCRB_STRUCT Iprv;

  MajorVersion = 0;
  MinorVersion = 0;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return 0;
  }

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  switch (VerId) {
    case IpCsiVerIdCsi:
      Ret = VERSION_CSI;
      PRINT_LEVEL1 ("IpPcieGetVersion(IpCsiVerIdCsi) = %x.%x (0x%x)\n",
                    IpWrRShiftU64 (Ret, 16),
                    Ret & 0xFFFF,
                    Ret);
      break;
    case IpCsiVerIdHwIp:
      /// Program IP Revision Register 0x1080
      Iprv.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, IPRV_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
      if ((Iprv.Bits.ipgenrv) == IP_REVISION_MAJOR) {
         MajorVersion = 17;
         switch (Iprv.Bits.ipdvrv) {
           case 0:
             Ret = IpPcieSip17;
             MinorVersion = 0;
             break;
           case 2:
             Ret = IpPcieSip17_2;
             MinorVersion = 2;
             break;
           case 4:
             Ret = IpPcieSip17_4;
             MinorVersion = 4;
             break;
           case 5:
             Ret = IpPcieSip17_5;
             MinorVersion = 5;
             break;
           case 6:
             Ret = IpPcieSip17_6;
             MinorVersion = 6;
             break;
           default:
             Ret = IpPcieSipUnknown;
         }
      } else {
        Ret = IpPcieSipUnknown;
      }
      PRINT_LEVEL1 ("IpPcieGetVersion(IpCsiVerIdHwIp) = Major=%d Minor=%d\n",
                    MajorVersion,
                    MinorVersion
                    );
      break;
    case IpCsiVerIdIpFwIntf:
      Ret = VERSION_IPFWINTF_CPCIE;
      PRINT_LEVEL1 ("IpPcieGetVersion(IpCsiVerIdIpFwIntf) = Gen=%x Year=%x Qtr=%x Rev=%x (0x%x)\n",
                    (UINT8) IpWrRShiftU64 (Ret, 24),
                    (UINT8) IpWrRShiftU64 (Ret, 16),
                    (UINT8) IpWrRShiftU64 (Ret, 8),
                    (UINT8) IpWrRShiftU64 (Ret, 0),
                    Ret);
      break;
    case IpCsiVerIdIpFwIntfExt:
      Ret = VERSION_IPFWINTFEXT_CPCIE;
      PRINT_LEVEL1 ("IpPcieGetVersion(IpCsiVerIdIpFwIntfExt) = Major=%x (0x%x)\n",
                    (UINT8) IpWrRShiftU64 (Ret, 0),
                    Ret);
      break;
    case IpCsiVerIdIpFw:
      Ret = VERSION_IPFW_SIP17PCIE;
      PRINT_LEVEL1 ("IpPcieGetVersion(IpCsiVerIdIpFw) = Year=%x WW=%x Day=%x Oops=%x (0x%x)\n",
                    (UINT8) IpWrRShiftU64 (Ret, 24),
                    (UINT8) IpWrRShiftU64 (Ret, 16),
                    (UINT8) IpWrRShiftU64 (Ret, 8),
                    (UINT8) IpWrRShiftU64 (Ret, 0),
                    Ret);
      break;
    case IpCsiVerIdHwIpCustom:
      Ret = 0;
      PRINT_LEVEL1 ("IpPcieGetVersion(IpCsiVerIdHwIpCustom) = 0x%x\n", Ret);
      break;
    default:
      PRINT_WARNING ("GetVersion(%d) is not supported\n", VerId);
      Ret = 0;
      break;
  }

  return Ret;
}

/**
  Finds the Offset to a given Capabilities ID
  Each capability has an ID and a pointer to next Capability, so they form a linked list.
  This function walks the list of Capabilities present in device's pci cfg. If requested capability
  can be found, its offset is returned.
  If the capability can't be found or if device doesn't exist, function returns 0
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] pInst  the instance
  @param[in] CapId  CAPID to search for

  @retval 0         CAPID not found (this includes situation where device doesn't exit)
  @retval Other     CAPID found, Offset of desired CAPID
**/
UINT8
IpPcieFindCapId (
  IP_PCIE_INST  *pInst,
  UINT8          CapId
  )
{
  UINT8  CapHeaderOffset;
  UINT8  CapHeaderId;
  UINT16 Data16;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return 0;
  }

  //
  // We do not explicitly check if device exists to save time and avoid unnecessary PCI access
  // If the device doesn't exist, check for CapHeaderId != 0xFF will fail and function will return offset 0
  //
  if (((UINT8)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCI_PRIMARY_STATUS_OFFSET, IpWrRegFlagSize8Bits) & EFI_PCI_STATUS_CAPABILITY) == 0x00) {
    ///
    /// Function has no capability pointer
    ///
    return 0;
  } else {
    ///
    /// Check the header layout to determine the Offset of Capabilities Pointer Register
    ///
    if (((UINT8)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCI_HEADER_TYPE_OFFSET, IpWrRegFlagSize8Bits) & HEADER_LAYOUT_CODE) == (HEADER_TYPE_CARDBUS_BRIDGE)) {
      ///
      /// If CardBus bridge, start at Offset 0x14
      ///
      CapHeaderOffset = EFI_PCI_CARDBUS_BRIDGE_CAPABILITY_PTR;
    } else {
      ///
      /// Otherwise, start at Offset 0x34
      ///
      CapHeaderOffset = PCI_CAPBILITY_POINTER_OFFSET;
    }
    ///
    /// Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
    ///
    CapHeaderId     = 0;
    CapHeaderOffset = (UINT8)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CapHeaderOffset, IpWrRegFlagSize8Bits) & ((UINT8) ~(BIT0 | BIT1));
    while (CapHeaderOffset != 0 && CapHeaderId != 0xFF) {
      Data16 = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CapHeaderOffset, IpWrRegFlagSize16Bits);
      CapHeaderId = (UINT8)(Data16 & 0xFF);
      if (CapHeaderId == CapId) {
        if (CapHeaderOffset > PCI_MAXLAT_OFFSET) {
          ///
          /// Return valid capability offset
          ///
          return CapHeaderOffset;
        } else {
          return 0;
        }
      }
      ///
      /// Each capability must be DWORD aligned.
      /// The bottom two bits of all pointers (including the initial pointer at 34h) are reserved
      /// and must be implemented as 00b although software must mask them to allow for future uses of these bits.
      ///
      CapHeaderOffset = (UINT8)(Data16 >> 8);
    }
    return 0;
  }
}

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Reporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] pInst  the instance
  @param[in] CapId  Extended CAPID to search for

  @retval 0         CAPID not found, this includes situation where device doesn't exist
  @retval Other     CAPID found, Offset of desired CAPID
**/
UINT16
SipFindExtendedCapId (
  IP_PCIE_INST  *pInst,
  UINT16        CapId
  )
{
  AECH_PCIE_CFG_STRUCT   Aech;
  UINT16                 CapHeaderId;
  UINT16                 CapHeaderOffset;
  ///
  /// Start to search at Offset 0x100
  /// Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
  ///
  CapHeaderId     = 0;
  /// Program Advanced Error Extended register 0x0100
  CapHeaderOffset = AECH_PCIE_CFG_REG;
  while (CapHeaderOffset != 0 && CapHeaderId != MAX_UINT16) {
    Aech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CapHeaderOffset, IpWrRegFlagSize32Bits);
    CapHeaderId = (UINT16) Aech.Bits.cid;
    if (CapHeaderId == CapId) {
      return CapHeaderOffset;
    }
    ///
    /// Each capability must be DWORD aligned.
    /// The bottom two bits of all pointers are reserved and must be implemented as 00b
    /// although software must mask them to allow for future uses of these bits.
    ///
    CapHeaderOffset = (UINT16) ((Aech.Bits.nco) & ~(BIT0 | BIT1));
  }
  return 0;
}

/**
  Get capability

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP.
  @param[in]      FeatureId  Cap ID
  @param[in, out] pCsiSts    Optional and may be NULL.  If non-NULL return API status

  @retval                    defined by FeatureId
**/
UINT32
IpPcieGetCapability (
  IP_PCIE_INST     *pInst,
  UINT32           IpTarget,
  IP_PCIE_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  )
{
  UINT16                  CapHeaderOffset;
  UINT32                  FeatureVal;
  ASPM_CAPS               Aspm;
  L1SCAP_PCIE_CFG_STRUCT  L1Scap;
  LSTS_PCIE_CFG_STRUCT    Lsts;
  RID_CC_PCIE_CFG_STRUCT  Rid;
  DCAP_PCIE_CFG_STRUCT    Dcap;
  DCAP2_PCIE_CFG_STRUCT   Dcap2;
  LCAP_PCIE_CFG_STRUCT    Lcap;

  FeatureVal      = IpPcieFeatValDis;
  CapHeaderOffset = 0;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    if (pCsiSts != NULL) {
      *pCsiSts = IpCsiStsErrorNullPtr;
    }
    return 0;
  }

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  switch (FeatureId) {
    case IpPcieFeatIdMps:  // Max_Payload_Size Supported
      CapHeaderOffset = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);
      if (CapHeaderOffset != 0) {
      /// Program Device Capabilities register 0x0044
      Dcap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      FeatureVal = Dcap.Bits.mps;
      }
      break;

    case IpPcieFeatIdIoApic:
      /// Program Revision ID register 0x0008
      Rid.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, RID_CC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      if ((Rid.Bits.bcc == PCI_CLASS_SYSTEM_PERIPHERAL) && (Rid.Bits.scc == PCI_SUBCLASS_PIC) &&
         ((Rid.Bits.pi == PCI_IF_APIC_CONTROLLER) || (Rid.Bits.pi == PCI_IF_APIC_CONTROLLER2))) {
        FeatureVal = 1;  // IoApicPresent
      }
      break;

    case IpPcieFeatIdCcc:  // Slot Clock Configuration
      CapHeaderOffset = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);
      if (CapHeaderOffset != 0) {
        /// Program Link Status register 0x0052
        Lsts.Data  = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
        FeatureVal = (UINT32) Lsts.Bits.scc;
      }
      break;

    case IpPcieFeatIdPtm:  // PTM Capability Register
      CapHeaderOffset = SipFindExtendedCapId (pInst, V_EX_PTM_CID);
      if (CapHeaderOffset != 0) {
        /// Program PTM Capability Register register 0x0154
        FeatureVal = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PTMCAPR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      }
      break;

    case IpPcieFeatIdLtr:  // LTR Mechanism Supported
      CapHeaderOffset = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);
      if (CapHeaderOffset != 0) {
        /// Program Device Capabilities 2 register 0x0064
        Dcap2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCAP2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
        FeatureVal = Dcap2.Bits.ltrms;
      }
      break;

    case IpPcieFeatIdL1ss:
      CapHeaderOffset = SipFindExtendedCapId (pInst, V_EX_L1S_CID);
      if (CapHeaderOffset != 0) {
        /// Program L1 Sub-States Capabilities register 0x0204
        L1Scap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, L1SCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
        if (L1Scap.Bits.l1pss) { // if L1 PM Substates Supported
          FeatureVal = L1Scap.Data;
        }
      }
      break;

    case IpPcieFeatIdAspm:
      CapHeaderOffset = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);
      Aspm.Uint32     = 0;

      if (CapHeaderOffset != 0) {
        /// Program Link Capabilities register 0x004C
        /// Program Device Capabilities register 0x0044
        Lcap.Data  = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
        Dcap.Data  = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
        ///
        /// Check endpoint for pre-1.1 devices based on the Role based Error Reporting Capability bit. Don't report L0s support for old devices
        ///
        if (Dcap.Bits.rber) {
          Aspm.Bits.L0sSupported = !!(Lcap.Bits.apms & 0x1);
        }
        if (Lcap.Bits.el1 < ASPM_L1_LATENCY_LIMIT) {
          Aspm.Bits.LinkL1ExitLatencyUs = 1 * (BIT0 << Lcap.Bits.el1);
        } else {
          Aspm.Bits.LinkL1ExitLatencyUs =  ASPM_L1_NO_LIMIT;
        }
        Aspm.Bits.L1Supported           = !!(Lcap.Bits.apms & 0x2);
        Aspm.Bits.LinkL0sExitLatency    = Lcap.Bits.el0;
        Aspm.Bits.L0sAcceptableLatency  = ASPM_L0s_NO_LIMIT;
        Aspm.Bits.L1AcceptableLatencyUs = ASPM_L1_NO_LIMIT;
        FeatureVal                      = Aspm.Uint32;
      }
      break;

    case IpPcieFeatIdMls:
      CapHeaderOffset = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);

      if (CapHeaderOffset != 0) {
        Lcap.Data  = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
        FeatureVal = (UINT8)Lcap.Bits.mls;
        }
      break;

    default:
      PRINT_WARNING ("WARNING:%s() unknown/unexpected FeatureId=%d\n", __FUNCTION__, FeatureId);
      FeatureVal      = IpPcieFeatValDis;
      if (pCsiSts != NULL) {
      *pCsiSts = IpCsiStsErrorUnknownId;
      }
      return FeatureVal;
      break;

  }

  if (pCsiSts != NULL) {
    *pCsiSts = IpCsiStsSuccess;
  }
  PRINT_LEVEL1 ("[%s] RpIndex: %d FeatureId: %x FeatureVal: %x\n", __FUNCTION__, pInst->RpIndex, FeatureId, FeatureVal);
  return FeatureVal;
}

/**
  Get status

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP.
  @param[in]      FeatureId  Status ID
  @param[in, out] pApiSts    Optional and may be NULL.  If non-NULL return API status

  @retval         defined by FeatureId
**/
UINT32
IpPcieGetStatus (
  IP_PCIE_INST    *pInst,
  UINT32           IpTarget,
  IP_PCIE_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  )
{
  SLSTS_PCIE_CFG_STRUCT       SlstsCfg;
  UINT32                      FeatureVal;
  STRPFUSECFG_PCIE_CFG_STRUCT Strpfusecfg;

  FeatureVal      = IpPcieFeatValDis;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    if (pCsiSts != NULL) {
      *pCsiSts = IpCsiStsErrorNullPtr;
    }
    return 0;
  }

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  switch(FeatureId){

    case IpPcieFeatIdPds:
      SlstsCfg.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, SLSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      FeatureVal = SlstsCfg.Bits.pds;
      break;

    case IpPcieFeatIdLr:
      Strpfusecfg.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, STRPFUSECFG_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      FeatureVal =  Strpfusecfg.Bits.lr;
      break;  

    default:
      PRINT_WARNING ("WARNING:%s() unknown/unexpected FeatureId=%d\n", __FUNCTION__, FeatureId);
      FeatureVal      = IpPcieFeatValDis;
      if (pCsiSts != NULL) {
      *pCsiSts = IpCsiStsErrorUnknownId;
      }
      return FeatureVal;
      break;

  }

  if (pCsiSts != NULL) {
    *pCsiSts = IpCsiStsSuccess;
  }
  PRINT_LEVEL1 ("[%s] RpIndex: %d FeatureId: %x FeatureVal: %x\n", __FUNCTION__, pInst->RpIndex, FeatureId, FeatureVal);
  return FeatureVal;
}

/**
  Get control

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP.
  @param[in]      FeatureId  Control ID
  @param[in, out] pApiSts    Optional and may be NULL.  If non-NULL return API status

  @retval         defined by FeatureId
**/
UINT32
IpPcieGetControl (
  IP_PCIE_INST     *pInst,
  UINT32           IpTarget,
  IP_PCIE_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  )
{
  if (pCsiSts != NULL) {
    *pCsiSts = IpCsiStsSuccess;
  }
  return IpPcieFeatValDis;
}

/**
  Set control

  @param[in]  pInst       The instance
  @param[in]  IpTarget    IP defined value for target within the IP.
  @param[in]  FeatureId   Feature ID
  @param[in]  FeatureVal  Value based on FeatureId

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpPcieSetControl (
  IP_PCIE_INST     *pInst,
  UINT32           IpTarget,
  IP_PCIE_FEAT_ID  FeatureId,
  UINT32           FeatureVal
  )
{
  UINT16         Data16;
  UINT16         CapHeaderOffset;
  L1SS_CAPS      L1ss;
  LTR_LIMIT      Ltr;
  ASPM_CAPS      Aspm;
  UINT32         OvrEn;
  LTR_OVERRIDE   TreeLtr;
  UINT8          TpoScaleMultiplier[] = {2, 10, 100};
  MPC2_PCIE_CFG_STRUCT       Mpc2;
  LTROVR_PCIE_CFG_STRUCT     Ltrovr;
  LTROVR2_PCIE_CFG_STRUCT    Ltrovr2;
  LPCR_PCIE_CFG_STRUCT       Lpcr;
  DCTL_PCIE_CFG_STRUCT       Dctl;
  DCTL2_PCIE_CFG_STRUCT      Dctl2;
  LCTL_PCIE_CFG_STRUCT       Lctl;
  LCTL2_PCIE_CFG_STRUCT      Lctl2;
  PTMCTLR_PCIE_CFG_STRUCT    PtmCtlr;
  DCAP2_PCIE_CFG_STRUCT      Dcap2;
  L1SCTL1_PCIE_CFG_STRUCT    L1Sctl1;
  L1SCTL2_PCIE_CFG_STRUCT    L1Sctl2;
  BOOLEAN                    L0sSupported;
  BOOLEAN                    L1Supported;

  CapHeaderOffset = 0;

  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: Invalid pInst\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  switch (FeatureId) {
    case IpPcieFeatIdMps:
      if (FeatureVal >= IpPcieMpsUnknown) {
        return IpCsiStsErrorBadParam;
      }
      CapHeaderOffset = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);
      if (CapHeaderOffset != 0) {
        /// Program Device Control register 0x0048
        Dctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
        Dctl.Bits.mps =(UINT16) FeatureVal;
        IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DCTL_PCIE_CFG_REG, Dctl.Data, IpWrRegFlagSize16Bits);
      }
      break;

    case IpPcieFeatIdIoApic:
      /// Program Miscellaneous Port Configuration 2 register 0xx00D4
      Mpc2.Data  = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MPC2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      if (FeatureVal) {
        Mpc2.Bits.eoifd = 0;
      } else {
        Mpc2.Bits.eoifd = 1;
      }
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MPC2_PCIE_CFG_REG, Mpc2.Data, IpWrRegFlagSize32Bits);
      break;

    case IpPcieFeatIdCcc:
      CapHeaderOffset = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);
      if (FeatureVal && CapHeaderOffset != 0) {
        /// Program PCI Express Additional Link Control register 0x0050
        Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
        Lctl.Bits.ccc = 1;
        IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);
      }
      break;

    case IpPcieFeatIdPtm:
      CapHeaderOffset = SipFindExtendedCapId (pInst, V_EX_PTM_CID);
      if (CapHeaderOffset != 0) {
       /// Program PTM Control Register register 0x0158
       PtmCtlr.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PTMCTLR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
       if ( PtmCtlr.Data != FeatureVal) {
         IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PTMCTLR_PCIE_CFG_REG, FeatureVal, IpWrRegFlagSize32Bits);
       }
      }
      break;

    case IpPcieFeatIdLtr:  // @todo: If required add IpPcieFeatIdLtrOvr
      /// Program Device Capabilities 2 register 0x0064
      Dcap2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCAP2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      if (FeatureVal == Dcap2.Bits.ltrms) {
        Ltr.MaxNoSnoopLatencyScale = (pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency & 0x1c00) >> 10;
        Ltr.MaxNoSnoopLatencyValue = (pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency & 0x3FF);
        Ltr.MaxSnoopLatencyScale   = (pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency & 0x1c00) >> 10;
        Ltr.MaxSnoopLatencyValue   = (pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency & 0x3FF);

        //
        // Enable LTR Capability
        //
        CapHeaderOffset = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);
        if (CapHeaderOffset != 0) {
          /// Program Device Control 2 register 0x0068
          Dctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
          Dctl2.Bits.ltren = 1;
          IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DCTL2_PCIE_CFG_REG, Dctl2.Data, IpWrRegFlagSize16Bits);
        }
        //
        // Set LTR limit
        //
        CapHeaderOffset = SipFindExtendedCapId (pInst, R_LTRECH_CID);
        if (CapHeaderOffset != 0) {
          Data16 = (UINT16) ((Ltr.MaxSnoopLatencyValue << N_PCIE_LTRECH_MSLR_VALUE) | (Ltr.MaxSnoopLatencyScale << N_PCIE_LTRECH_MSLR_SCALE));
          IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, CapHeaderOffset + R_PCIE_LTRECH_MSLR_OFFSET, Data16, IpWrRegFlagSize16Bits);
          Data16 = (UINT16) ((Ltr.MaxNoSnoopLatencyValue << N_PCIE_LTRECH_MNSLR_VALUE) | (Ltr.MaxNoSnoopLatencyScale << N_PCIE_LTRECH_MNSLR_SCALE));
          IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, CapHeaderOffset + R_PCIE_LTRECH_MNSLR_OFFSET, Data16, IpWrRegFlagSize16Bits);
        }
      } else {
        TreeLtr.Uint32 = FeatureVal;
        OvrEn          = 0;

        /// Program Latency Tolerance Reporting Override register 0x0400
        Ltrovr.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LTROVR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
        Ltrovr2.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LTROVR2_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
        /// Program Lock Policy Control Register register 0x0C8C
        Lpcr.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LPCR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
        if (Ltrovr2.Data != 0) {
          IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LTROVR2_PCIE_CFG_REG, 0, IpWrRegFlagSize8Bits);
        }
        //
        // (*)LatencyOverrideMode = 0 -> no override
        //                          1 -> override with RP policy values
        //                          2 -> override with endpoint's override values
        //

        if (pInst->PcieRpCommonConfig.PcieRpLtrConfig.ForceLtrOverride || TreeLtr.Bits.ForceOverride) {
          OvrEn |= (UINT32) Ltrovr2.Bits.ltrovrplcy;
        }
        if (pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrConfigLock == TRUE) {
          OvrEn |= (UINT32) Lpcr.Bits.ltrcfglock;
        }

        if (pInst->PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode == 1) {
          OvrEn  |= (UINT32) Ltrovr2.Bits.ltrsovren;
          Ltrovr.Bits.ltrslovrv = (UINT32) pInst->PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideValue;
          Ltrovr.Bits.ltrslsovrv = ((UINT32) pInst->PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMultiplier);
        } else if (pInst->PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode == 2) {
          if (TreeLtr.Bits.MaxSnoopLatencyRequirement) {
            OvrEn  |= (UINT32) Ltrovr2.Bits.ltrsovren;
            Ltrovr.Bits.ltrslovrv = TreeLtr.Bits.MaxSnoopLatencyValue;
            Ltrovr.Bits.ltrslsovrv = TreeLtr.Bits.MaxSnoopLatencyScale;
          }
        }
        if (pInst->PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode == 1) {
          OvrEn  |= (UINT32) Ltrovr2.Bits.ltrnsovren;
          Ltrovr.Bits.ltrnslovrv = ((UINT32)pInst->PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideValue);
          Ltrovr.Bits.ltrnslsovrv = ((UINT32)pInst->PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier);
        } else if (pInst->PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode == 2) {
          if (TreeLtr.Bits.MaxNoSnoopLatencyRequirement) {
            OvrEn  |= (UINT32) Ltrovr2.Bits.ltrnsovren;
            Ltrovr.Bits.ltrnslovrv = TreeLtr.Bits.MaxNoSnoopLatencyValue;
            Ltrovr.Bits.ltrnslsovrv = TreeLtr.Bits.MaxNoSnoopLatencyScale;
          }
        }
        if (!!Ltrovr.Data) {
          IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LTROVR_PCIE_CFG_REG, Ltrovr.Data, IpWrRegFlagSize32Bits);
        }
        IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LTROVR2_PCIE_CFG_REG, OvrEn, IpWrRegFlagSize8Bits);
        IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LPCR_PCIE_CFG_REG, OvrEn, IpWrRegFlagSize32Bits);
      }
      break;

    case IpPcieFeatIdL1ss:
      CapHeaderOffset = SipFindExtendedCapId (pInst, V_EX_L1S_CID);
      if (CapHeaderOffset == 0) {
        return IpCsiStsErrorNotAllowed;
      }

      L1ss.Uint32   = FeatureVal;
      //
      // Program L1 Sub-States Control 1 register 0x0208
      // Program L1 PM Substates Control 1 Register
      //
      L1Sctl1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, L1SCTL1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      if (L1ss.Bits.PmL12){
        L1Sctl1.Bits.ppl12e = 1;
      }
      if (L1ss.Bits.PmL11){
        L1Sctl1.Bits.ppl11e = 1;
      }
      if (L1ss.Bits.AspmL12){
        L1Sctl1.Bits.al12e = 1;
      }
      if (L1ss.Bits.AspmL11){
        L1Sctl1.Bits.al11e = 1;
      }
      L1Sctl1.Bits.cmrt = L1ss.Bits.Cmrt;
      ///
      /// BWG 1.3 Section 5.5.7.6 LTR Threshold Latency
      /// Set L1.2 LTR threshold using formula (TpoToUs (L1ss.TpoScale, L1ss.TpoValue) + L1ss.Cmrt + 10)
      ///
      Data16 = 0;
      //
      // Converts Tpower_on from value:scale notation to microseconds
      //
      if (L1ss.Bits.TpoScale <= MAX_T_POWER_ON_SCALE) {
        Data16 = (UINT16) (TpoScaleMultiplier[L1ss.Bits.TpoScale] * L1ss.Bits.TpoValue);
      }
      L1Sctl1.Bits.l12ltrtlv  = (Data16 + L1ss.Bits.Cmrt + 10);
      L1Sctl1.Bits.l12ltrtlsv = 0x2;
      //
      // Set CLKREQ Acceleration Interrupt Enable
      //
      L1Sctl1.Bits.l1sseie = 1;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, L1SCTL1_PCIE_CFG_REG, L1Sctl1.Data, IpWrRegFlagSize32Bits);

      //
      // Program L1 Sub-States Control 2 register 0x0020
      // Program L1 PM Substates Control 2 Register
      //
      L1Sctl2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, L1SCTL2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      L1Sctl2.Data |= (L1ss.Bits.TpoScale);
      L1Sctl2.Bits.powt = N_EX_L1SCTL2_POWT;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, L1SCTL2_PCIE_CFG_REG, L1Sctl2.Data, IpWrRegFlagSize32Bits);
      break;

    case IpPcieFeatIdAspm:
      CapHeaderOffset = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);
      Aspm.Uint32   = FeatureVal;
      /// Program PCI Express Additional Link Control register 0x0050
      Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      if (CapHeaderOffset != 0) {
        L0sSupported = Aspm.Bits.L0sSupported && (Aspm.Bits.L0sAcceptableLatency >= Aspm.Bits.LinkL0sExitLatency);
        L1Supported  = Aspm.Bits.L1Supported && (Aspm.Bits.L1AcceptableLatencyUs >= Aspm.Bits.LinkL1ExitLatencyUs);
        if (L0sSupported && L1Supported) {
          Lctl.Bits.aspm = 0x3;
        } else {
          if (L0sSupported) {
            Lctl.Bits.aspm = 0x1;
          }
          else if (L1Supported) {
            Lctl.Bits.aspm = 0x2;
          }
          else {
            Lctl.Bits.aspm = 0x0;
          }
        }
        IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);
      }
      break;

    case IpPcieFeatIdSd:
      Lctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
      Lctl2.Bits.sd = (UINT16)FeatureVal;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, Lctl2.Data, IpWrRegFlagSize32Bits);
      break;

    default:
      PRINT_WARNING ("WARNING:%s() unknown/unexpected FeatureId=%d\n", __FUNCTION__, FeatureId);
      return IpCsiStsErrorUnknownId;
      break;
  }
  return IpCsiStsSuccess;
}
