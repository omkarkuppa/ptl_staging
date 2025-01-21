/** @file
  PEIM Private Library to initialize for DMI device

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
#include <IndustryStandard/Pci22.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiDmiInitLib.h>
#include <Library/PeiDmiTaskDispatchLib.h>
#include <Library/IocRegAccessLib.h>
#include <Register/DmiRegs.h>
#include <Register/HostBridgeRegs.h>
#include <Library/DmiInfoLib.h>
#include <Library/TimerLib.h>
#include <DmiHob.h>

  enum DMI_EQ_MODE {
    DmiEqDisabled,
    DmiFixedEq,
    DmiLinearEq,
  };

  static CONST UINT16 DmiGen3DsPortPreset[] = {
    R_DMI_L01EC,
    R_DMI_L23EC,
    R_DMI_L45EC,
    R_DMI_L67EC
  };

  static CONST UINT16 DmiGen4DsPortPreset[] = {
    R_DMI_PL16L01EC,
    R_DMI_PL16L23EC,
    R_DMI_PL16L45EC,
    R_DMI_PL16L67EC
  };

/**
  The Task Table for Dmi PreMem
**/
GLOBAL_REMOVE_IF_UNREFERENCED IP_DMI3_FEAT_ID_DISPATCH_ENTRY  DmiPrememCallTable[] = {
  ///
  /// The functions are executed in the following order, as the policy flag dictates.
  /// DmiTask, DmiBootModeFlag, debug_string
  ///
  { DpDmiPrememInit,                             DMI_ALL,                 "DpDmiPrememInit"},
  { DpDmiBasicInit,                              DMI_ALL,                 "DpDmiBasicInit"},
  { DpDmiElasticBufferProgramming,               DMI_ALL,                 "DpDmiElasticBufferProgramming"},
  { DpDmiConfigureClockGating,                   DMI_ALL,                 "DpDmiConfigureClockGating"},
  { DpDmiConfigureLinkPowerManagement,           DMI_ALL,                 "DpDmiConfigureLinkPowerManagement"},
  { DpDmiConfigureSquelchPowerManagement,        DMI_ALL,                 "DpDmiConfigureSquelchPowerManagement"},
  { DpDmiConfigurePllShutdown,                   DMI_ALL,                 "DpDmiConfigurePllShutdown"},
  { DpDmiConfigurePowerGating,                   DMI_ALL,                 "DpDmiConfigurePowerGating"},
  { DpDmiConfigureRetimerSupport,                DMI_ALL,                 "DpDmiConfigureRetimerSupport"},
  { DpDmiConfigurePtm,                           DMI_ALL,                 "DpDmiConfigurePtm"},
  { DpDmiConfigureCoalescing,                    DMI_ALL,                 "DpDmiConfigureCoalescing"},
  { DpDmiConfigurePowerDownMapping,              DMI_ALL,                 "DpDmiConfigurePowerDownMapping"},
  { DpDmiConfigureAtomics,                       DMI_ALL,                 "DpDmiConfigureAtomics"},
  { DpDmiConfigureSmiSci,                        DMI_ALL,                 "DpDmiConfigureSmiSci"},
  { DpDmiConfigureTlpPrefix,                     DMI_ALL,                 "DpDmiConfigureTlpPrefix"},
  { DpDmiConfigureDesignSpecificConfiguration,   DMI_ALL,                 "DpDmiConfigureDesignSpecificConfiguration"},
  { DpDmiConfigureEnabling,                      DMI_ALL,                 "DpDmiConfigureEnabling"},
  { DpDmiConfigureMultiVC,                       DMI_ALL,                 "DpDmiConfigureMultiVC"},
  { DpDmiConfigure10BitTagAndScaledFlowControl,  DMI_ALL,                 "DpDmiConfigure10BitTagAndScaledFlowControl"},
  { DpDmiConfigureRxPrimaryCycleDecode,          DMI_ALL,                 "DpDmiConfigureRxPrimaryCycleDecode"},
  { DpDmiConfigureDPC,                           DMI_ALL,                 "DpDmiConfigureDPC"},
  { DpDmiConfigureAckLatency,                    DMI_ALL,                 "DpDmiConfigureAckLatency"},
  { DpDmiEnablePcieRelaxedOrder,                 DMI_ALL,                 "DpDmiEnablePcieRelaxedOrder"},
  { DpDmiConfigureRxMarginProgramming,           DMI_ALL,                 "DpDmiConfigureRxMarginProgramming"},
  { DpDmiConfigureMPS,                           DMI_ALL,                 "DpDmiConfigureMPS"},
  { DpDmiConfigureD3VnnRemCtl,                   DMI_ALL,                 "DpDmiConfigureD3VnnRemCtl"},
  { NULL ,                                       DMI_NULL,                "END_OF_TASK"}
};

/**
  The Task Table for Dmi PostMem
**/
GLOBAL_REMOVE_IF_UNREFERENCED IP_DMI3_FEAT_ID_DISPATCH_ENTRY  DmiPostMemCallTable[] = {
  ///
  /// The functions are executed in the following order, as the policy flag dictates.
  /// DmiTask, DmiBootModeFlag, debug_string
  ///
  { DpDmiPostMemInit,             DMI_ALL,                 "DpDmiPostMemInit"},
  { DpDmiSpeedChange,             DMI_ALL,                 "DpDmiSpeedChange"},
  { NULL ,                        DMI_NULL,                "END_OF_TASK"}
};

/**
  DpDmiPrememInit: Initialize the DMI device

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiPrememInit (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;
  PeiDpDmiConfig->DmiBaseAddress = (IocRegAccessRead32 (DMI3BAR_IOC_MCHBAR_REG) & ~(BIT0));
  DEBUG ((DEBUG_INFO, "DpDmiBaseAddress = 0x%x\n", PeiDpDmiConfig->DmiBaseAddress));

  return EFI_SUCCESS;
}

/**
  DpDmiPostMemInit: Initialize the DMI device

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiPostMemInit (
  IN  VOID    *PeiDmiConfig
  )
{
  return EFI_SUCCESS;
}


/*
  Program registers for Elastic Buffer

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
*/
EFI_STATUS
EFIAPI
DpDmiElasticBufferProgramming (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32          Data32Or;
  UINT32          Data32And;
  PEI_DMI_CONFIG *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32And = (UINT32)~(B_DMI_ACGR3S2_G1EBM | B_DMI_ACGR3S2_G2EBM | B_DMI_ACGR3S2_G3EBM | B_DMI_ACGR3S2_G4EBM);
  Data32Or  = (UINT32)(B_DMI_ACGR3S2_G1EBM | B_DMI_ACGR3S2_G2EBM | B_DMI_ACGR3S2_SRT);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_ACGR3S2, Data32And, Data32Or);

  DEBUG ((DEBUG_INFO, "R_DMI_ACGR3S2 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_ACGR3S2)));

  return EFI_SUCCESS;
}

/**
  Configure DMI clock gating.
  This function must be called before ASPM is enabled

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
*/
EFI_STATUS
EFIAPI
DpDmiBasicInit (
  IN  VOID    *PeiDmiConfig
  )
{
  DMI_ASPM      DmiAspm;
  UINT16        Data16;
  UINT32        Data32;
  UINT32        Data32Or;
  UINT32        Data32And;

  PEI_DMI_CONFIG *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;
  DmiAspm        = PeiDpDmiConfig->DmiAspm;

  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_CMD, ~0u, (EFI_PCI_COMMAND_SERR | B_DMI_CMD_SEE));
  MmioAndThenOr8 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_BCTRL, (UINT8)~0, (B_DMI_BCTRL_SE));

  //
  // Program the Capabilities Pointer
  //
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_CAPP, (UINT32)~B_DMI_CAPP_PTR_MASK, V_DMI_CAPP_PTR);

  //
  // Program 0x40[15:8] - 0x80
  // Program 0x40[7:0] - 0x10
  //
  Data16 = (V_DMI_CLIST_NEXT << N_DMI_CLIST_NEXT) | V_DMI_CLIST_CID;
  MmioWrite16 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_CLIST, Data16);

  //
  // Program 0x42[3:0] - 0x2
  //
  MmioOr16 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_XCAP, (UINT16)V_DMI_XCAP_CV);

  //
  // Program 0x80[15:8] - 0x98
  // Program 0x80[7:0] - 0x05
  //
  Data16 = (V_DMI_MID_NEXT << N_DMI_CLIST_NEXT) | V_DMI_MID_CID;
  MmioWrite16 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_MID, Data16);

  //
  // Program 0x98[15:8] - 0xA0
  // Program 0x98[7:0] - 0x0D
  //
  Data16 = (V_DMI_SVCAP_NEXT << N_DMI_CLIST_NEXT) | V_DMI_SVCAP_CID;
  MmioWrite16 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_SVCAP, Data16);

  //
  // Program 0xA0[15:8] - 0x00
  // Program 0xA0[7:0] - 0x01
  //
  Data16 = (V_DMI_PMCAP_NEXT << N_DMI_CLIST_NEXT) | V_DMI_PMCAP_CID;
  MmioWrite16 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PMCAP, Data16);

  //
  // Program 0x100[31:20] - 0x220
  // Program 0x100[19:16] - 0x01
  // Program 0x100[15:0] - 0x0001
  //
  Data32 = (V_DMI_AECH_NEXT << N_DMI_AECH_NEXT) | (V_DMI_AECH_CV << N_DMI_AECH_CV) | V_DMI_AECH_CID;
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_AECH, Data32);

  //
  // Program 0x220[31:20] - 0x200
  // Program 0x220[19:16] - 0x01
  // Program 0x220[15:0] - 0x000D
  //
  Data32 = (V_DMI_ACSECH_NEXT << N_DMI_ACSECH_NEXT) | (V_DMI_ACSECH_CV << N_DMI_ACSECH_CV) | V_DMI_ACSECH_CID;
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_ACSECH, Data32);

  //
  // Program 0x200[31:20] - 0x150
  // Program 0x200[19:16] - 0x01
  // Program 0x200[15:0] - 0x001E
  //
  Data32 = (V_DMI_L1SECH_NEXT << N_DMI_L1SECH_NEXT) | (V_DMI_L1SECH_CV << N_DMI_L1SECH_CV) | V_DMI_L1SECH_CID;
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_L1SECH, Data32);

  //
  // Program 0x150[31:20] - 0x280
  // Program 0x150[19:16] - 0x01
  // Program 0x150[15:0] - 0x0002
  //
  Data32 = (V_DMI_PTMECH_NEXT << N_DMI_PTMECH_NEXT) | (V_DMI_PTMECH_CV << N_DMI_PTMECH_CV) | V_DMI_PTMECH_CID;
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PTMECH, Data32);

  //
  // Program 0x280[31:20] - 0xA30
  // Program 0x280[19:16] - 0x01
  // Program 0x280[15:0] - 0x001F
  //
  Data32 = (UINT32)((V_DMI_VCCH_NEXT << N_DMI_VCCH_NEXT) | (V_DMI_VCCH_CV << N_DMI_VCCH_CV) | V_DMI_VCCH_CID);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_VCCH, Data32);

  //
  // Program 0xA30[31:20] - 0xA90
  // Program 0xA30[19:16] - 0x01
  // Program 0xA30[15:0] - 0x0019
  //
  Data32 = (UINT32)((V_DMI_SPEECH_NEXT << N_DMI_SPEECH_NEXT) | (V_DMI_SPEECH_CV << N_DMI_SPEECH_CV) | V_DMI_SPEECH_CID);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_SPEECH, Data32);

  //
  // Program 0xA90[31:20] - 0xA9C
  // Program 0xA90[19:16] - 0x01
  // Program 0xA90[15:0] - 0x0025
  //
  Data32 = (UINT32)((V_DMI_DLFECH_NEXT << N_DMI_DLFECH_NEXT) | (V_DMI_DLFECH_CV << N_DMI_DLFECH_CV) | V_DMI_DLFECH_CID);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DLFECH, Data32);

  //
  // Program 0xA9C[31:20] - 0xEDC
  // Program 0xA9C[19:16] - 0x01
  // Program 0xA9C[15:0] - 0x0026
  //
  Data32 = (UINT32)((V_DMI_PL16GECH_NEXT << N_DMI_PL16GECH_NEXT) | (V_DMI_PL16GECH_CV << N_DMI_PL16GECH_CV) | V_DMI_PL16GECH_CID);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PL16GECH, Data32);

  //
  // Program 0xEDC[31:20] - 0x00
  // Program 0xEDC[19:16] - 0x01
  // Program 0xEDC[15:0] - 0x0027
  //
  Data32 = (V_DMI_PL16MECH_NEXT << N_DMI_PL16MECH_NEXT) | (V_DMI_PL16MECH_CV << N_DMI_PL16MECH_CV) | V_DMI_PL16MECH_CID;
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PL16MECH, Data32);


  MmioAndThenOr32 ( PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCTL, ~0u,
                   (B_DMI_DCTL_URE | B_DMI_DCTL_FEE | B_DMI_DCTL_NFE | B_DMI_DCTL_CEE));

  Data32Or = 0;
  if (DmiAspm == DmiAspmL0sL1) {
    Data32Or = V_DMI_LCAP_APMS_L0S_L1;
  } else if (DmiAspm == DmiAspmL0s) {
    Data32Or = V_DMI_LCAP_APMS_L0S;
  } else if ((DmiAspm == DmiAspmAutoConfig) || (DmiAspm == DmiAspmL1)) {
    Data32Or = V_DMI_LCAP_APMS_L1;
  } else {
    Data32Or  = V_DMI_LCAP_APMS_DIS;
  }
  Data32Or = (Data32Or << N_DMI_LCAP_APMS);
  Data32Or |= (PeiDpDmiConfig->DmiAspmL1ExitLatency << N_DMI_LCAP_EL1);

  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_LCAP,
                   (UINT32)~(B_DMI_LCAP_APMS | B_DMI_LCAP_EL1),
                   Data32Or);
  DEBUG ((DEBUG_INFO, "R_DMI_LCAP = 0x%x \n",(MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_LCAP))));

  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PMCS, (UINT32)~B_DMI_PMCS_NSR);

  ///
  /// Configure Completion Timeout
  ///
  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCTL2, B_DMI_DCTL2_CTD);

  //
  // Program Common Clock Exit Latency = 100b => 512ns to less than 1us
  //
  Data32And = (UINT32)~(B_DMI_MPC_BMERCE | B_DMI_MPC_IRRCE | B_DMI_MPC_MMBNCE | B_DMI_MPC_CCEL);
  Data32Or = (V_DMI_MPC_CCEL_2US_4US << N_DMI_MPC_CCEL);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_MPC, Data32And, Data32Or);

  Data32And = (UINT32)~(B_DMI_DCGEN1_PXLTULDCGEN | B_DMI_DCGEN1_PXTTULDCGEN );
  Data32Or = (B_DMI_DCGEN1_PXKGULDCGEN | B_DMI_DCGEN1_PXCULDCGEN | B_DMI_DCGEN1_PXLRULDCGEN | B_DMI_DCGEN1_PXLIULDCGEN | B_DMI_DCGEN1_PXLSULDCGEN | B_DMI_DCGEN1_PXTRULDCGEN | B_DMI_DCGEN1_PXTRSULDCGEN | B_DMI_DCGEN1_PXTTSULDCGEN  );
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_DCGEN1, Data32And, Data32Or);

  Data32Or = (B_DMI_DCGEN2_PXPIULDCGEN | B_DMI_DCGEN2_PXPSULDCGEN | B_DMI_DCGEN2_PXPBULDCGEN | B_DMI_DCGEN2_PXFTULDCGEN | B_DMI_DCGEN2_PXFRULDCGEN | B_DMI_DCGEN2_PXSDIULDCGEN | B_DMI_DCGEN2_PXSDTULDCGEN | B_DMI_DCGEN2_PXSDRULDCGEN );
  MmioOr32(PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_DCGEN2, Data32Or);

  Data32And = (UINT32)~(B_DMI_DCGM1_PXTTULDCGM);
  Data32Or = (B_DMI_DCGM1_PXKGULDCGM | B_DMI_DCGM1_PXCULDCGM | B_DMI_DCGM1_PXLRULDCGM | B_DMI_DCGM1_PXLTULDCGM | B_DMI_DCGM1_PXLIULDCGM | B_DMI_DCGM1_PXLSULDCGM | B_DMI_DCGM1_PXTRULDCGM | B_DMI_DCGM1_PXTRSULDCGM | B_DMI_DCGM1_PXTTSULDCGM );
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_DCGM1, Data32And,Data32Or);

  Data32Or = (B_DMI_DCGM2_PXPIULDCGM | B_DMI_DCGM2_PXPSULDCGM | B_DMI_DCGM2_PXPBULDCGM | B_DMI_DCGM2_PXFTULDCGM | B_DMI_DCGM2_PXFRULDCGM | B_DMI_DCGM2_PXSDRULDCGM | B_DMI_DCGM2_PXSDTULDCGM | B_DMI_DCGM2_PXSDIULDCGM );
  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_DCGM2,Data32Or);

  MmioOr8(PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_IPCLKCTR, B_DMI_IPCLKCTR_MDPCCEN);

  Data32And = (UINT32)~(B_DMI_DCGEN3_PXCUPDCGEN |B_DMI_DCGEN3_PXCUPSRCDCGEN | B_DMI_DCGEN3_PXCUPSNRDCGEN | B_DMI_DCGEN3_PXTOUPDCGEN );
  Data32Or = (B_DMI_DCGEN3_PXEUPDCGEN |B_DMI_DCGEN3_PXBUPDCGEN | B_DMI_DCGEN3_PXSRUSSNRDCGEN | B_DMI_DCGEN3_PXLRUPDCGEN | B_DMI_DCGEN3_PXLIUPDCGEN | B_DMI_DCGEN3_PXTRUPDCGEN | B_DMI_DCGEN3_PXTRSUPDCGEN |  B_DMI_DCGEN3_PXTTUPDCGEN | B_DMI_DCGEN3_PXTTSUPDCGEN );
  MmioAndThenOr32(PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_DCGEN3, Data32And,Data32Or);

  Data32And = (UINT32)~(B_DMI_DCGM3_PXCUPDCGM | B_DMI_DCGM3_PXTOUPDCGM );
  Data32Or = (B_DMI_DCGM3_PXEUPDCGM | B_DMI_DCGM3_PXBUPDCGM | B_DMI_DCGM3_PXCUPSRCDCGM | B_DMI_DCGM3_PXCUPSNRDCGM | B_DMI_DCGM3_PXSRUSSNRDCGM | B_DMI_DCGM3_PXLRUPDCGM | B_DMI_DCGM3_PXLTUPDCGM | B_DMI_DCGM3_PXLIUPDCGM | B_DMI_DCGM3_PXTRUPDCGM | B_DMI_DCGM3_PXTRSUPDCGM | B_DMI_DCGM3_PXTTUPDCGM | B_DMI_DCGM3_PXTTSUPDCGM );
  MmioAndThenOr32(PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_DCGM3, Data32And, Data32Or);
  //
  // Configure DP EQTS2 Recovery
  //
  MmioOr8 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_EQCFG1, B_DMI_EQCFG1_EQTS2IRRC);
  MmioOr8 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_EQCFG4, B_DMI_EQCFG4_PX16GEQTS2IRRC);

  Data32And = (UINT32)~(B_DMI_IORCCP1_ORRPGM);
  Data32Or = (B_DMI_IORCCP1_DRCORRP | B_DMI_IORCCP1_DISORCL12REC | B_DMI_IORCCP1_DISORCRODI | 0x1 << N_DMI_IORCCP1_G1ORRRXECC | 0x1 << N_DMI_IORCCP1_G2ORRRXECC | 0x1 << N_DMI_IORCCP1_G3ORRRXECC | 0x1 << N_DMI_IORCCP1_G4ORRRXECC );
  MmioAndThenOr32(PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_IORCCP1, Data32And, Data32Or);
  return EFI_SUCCESS;
}

/**
  Configure DMI clock gating.
  This function must be called before ASPM is enabled

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
*/
EFI_STATUS
EFIAPI
DpDmiConfigureClockGating (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_CCFG,
    ~0u,
    B_DMI_CCFG_DCGEISMA
    );

  MmioAndThenOr8 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_RPDCGEN,
    (UINT8)~0,
    (B_DMI_LCLKREQEN | B_DMI_BBCLKREQEN | B_DMI_SRDBCGEN |
    B_DMI_RPDLCGEN | B_DMI_RPDBCGEN | B_DMI_PTOCGE)
    );

  MmioAndThenOr8 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_IOSFSBCS,
    (UINT8)~B_DMI_IOSFSBCS_SIID,
    B_DMI_IOSFSBCS_SCPTCGE
    );

  MmioAndThenOr16 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_RPPGEN,
    (UINT8)~0,
    B_DMI_RPPGEN_SEOSCGE
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PME_CTL3,
    (UINT8)~B_DMI_PME_CTL3_OSCCGH,
    (UINT32) (V_DMI_PME_CTL3_OSCCGH << N_DMI_PME_CTL3_OSCCGH)
    );
  MmioAnd8(
     PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIERTP1,
    (UINT8)~(B_DMI_PCIERTP1_G1X4_MASK)
  );
  return EFI_SUCCESS;
}

/**
  Configure DMI link power management.
  This function must be called before ASPM is enabled

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureLinkPowerManagement (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEL0SC,
    (UINT32)~(B_DMI_PCIEL0SC_G2ASL0SPL_MASK  | B_DMI_PCIEL0SC_G1ASL0SPL_MASK),
    ((V_DMI_PCIEL0SC_G2ASL0SPL << N_DMI_PCIEL0SC_G2ASL0SPL) |
    (V_DMI_PCIEL0SC_G1ASL0SPL << N_DMI_PCIEL0SC_G1ASL0SPL))
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIENFTS,
    (UINT32)~(B_DMI_PCIENFTS_G2CCNFTS_MASK  | B_DMI_PCIENFTS_G1CCNFTS_MASK  | B_DMI_PCIENFTS_G2UCNFTS_MASK  | B_DMI_PCIENFTS_G1UCNFTS_MASK ),
    ((V_DMI_PCIENFTS_G2CCNFTS << N_DMI_PCIENFTS_G2CCNFTS) |
    (V_DMI_PCIENFTS_G1CCNFTS << N_DMI_PCIENFTS_G1CCNFTS) |
    (V_DMI_PCIENFTS_G2UCNFTS << N_DMI_PCIENFTS_G2UCNFTS) |
    (V_DMI_PCIENFTS_G1UCNFTS << N_DMI_PCIENFTS_G1UCNFTS))
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_G3L0SCTL,
    (UINT32)~(B_DMI_G3L0SCTL_G3UCNFTS_MASK | B_DMI_G3L0SCTL_G3CCNFTS_MASK),
    ((V_DMI_G3L0SCTL_G3UCNFTS << N_DMI_G3L0SCTL_G3UCNFTS) |
    (V_DMI_G3L0SCTL_G3CCNFTS << N_DMI_G3L0SCTL_G3CCNFTS))
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_G4L0SCTL,
    (UINT32)~(B_DMI_G4L0SCTL_G4UCNFTS_MASK | B_DMI_G4L0SCTL_G4CCNFTS_MASK),
    ((V_DMI_G4L0SCTL_G4UCNFTS << B_DMI_G4L0SCTL_G4UCNFTS_OFFSET) |
    (V_DMI_G4L0SCTL_G4CCNFTS << B_DMI_G4L0SCTL_G4CCNFTS_OFFSET))
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_L1SCAP,
    (UINT32)~(B_DMI_L1SCAP_PPL12S | B_DMI_L1SCAP_PPL11S | B_DMI_L1SCAP_AL11S | B_DMI_L1SCAP_PTPOS | B_DMI_L1SCAP_PCMRT_MASK | B_DMI_L1SCAP_PTV),
    (B_DMI_L1SCAP_AL12S | (V_DMI_L1SCAP_PTPOS_10US << B_DMI_L1SCAP_PTPOS_OFFSET) | B_DMI_L1SCAP_L1PSS | (V_DMI_L1SCAP_PTV_50US << B_DMI_L1SCAP_PTV_OFFSET))
    );

  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_LLPC, B_DMI_LLPC_AL1NTP);

  return EFI_SUCCESS;
}

/**
  Configure DMI Squelch Power Management.

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureSquelchPowerManagement (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32          Data32And;
  UINT32          Data32Or;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  MmioAnd32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEDBG,
    (UINT32)~(B_DMI_PCIEDBG_LGCLKSQEXITDBTIMERS | B_DMI_PCIEDBG_SPCE)
    );

  Data32And = (UINT32)~(B_DMI_PWRCTL_RPL1SQPOL | B_DMI_PWRCTL_RPDTSQPOL);
  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PWRCTL,
    Data32And,
    0u
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEDBG,
    (UINT32)~(B_DMI_PCIEDBG_SQOL0),
    0u
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEDBG,
    (UINT32)~B_DMI_PCIEDBG_USSP,
    (V_DMI_PCIEDBG_USSP << N_DMI_PCIEDBG_USSP)
    );

  Data32And = (UINT32)~(B_DMI_PCIEPMECTL_DLSULDLSD | B_DMI_PCIEPMECTL_L1FSOE);
  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEPMECTL,
    Data32And,
    B_DMI_PCIEPMECTL_DLSULPPGE
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCAP2,
    (UINT32) ~B_DMI_DCAP2_OBFFS,
    B_DMI_DCAP2_LTRMS
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_RXL0SESQCP1,
    (UINT32) ~B_DMI_RXL0SESQCP1_SSMFRXL0S,
    V_DMI_RXL0SESQCP1_SSMFRXL0S
    );

  Data32And = (UINT32) ~(B_DMI_RXL0SESQCP4_G1LBWSST | B_DMI_RXL0SESQCP4_G2LBWSST | B_DMI_RXL0SESQCP4_G3LBWSST | B_DMI_RXL0SESQCP4_G4LBWSST);
  Data32Or  = ((V_DMI_RXL0SESQCP4_G1LBWSST << N_DMI_RXL0SESQCP4_G1LBWSST) | (V_DMI_RXL0SESQCP4_G2LBWSST << N_DMI_RXL0SESQCP4_G2LBWSST) |
                (V_DMI_RXL0SESQCP4_G3LBWSST << N_DMI_RXL0SESQCP4_G3LBWSST) | (V_DMI_RXL0SESQCP4_G4LBWSST << N_DMI_RXL0SESQCP4_G4LBWSST));
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_RXL0SESQCP4, Data32And, Data32Or);

  Data32And = (UINT32) ~(B_DMI_LPHYCP4_RXVLDRXL0SSP | B_DMI_LPHYCP4_RXL0SEG12FTSE | B_DMI_LPHYCP4_LGUSSP | B_DMI_LPHYCP4_OSUSSP | B_DMI_LPHYCP4_REEFRXL0SED );
  Data32Or  = ((V_DMI_LPHYCP4_LGCLKSQEXITDBTIMERS << N_DMI_LPHYCP4_LGCLKSQEXITDBTIMERS) | (V_DMI_LPHYCP4_OSCCLKSQEXITDBTIMERS << N_DMI_LPHYCP4_OSCCLKSQEXITDBTIMERS) |
               (V_DMI_LPHYCP4_LGUSSP << N_DMI_LPHYCP4_LGUSSP) | (V_DMI_LPHYCP4_OSUSSP << N_DMI_LPHYCP4_OSUSSP) | (V_DMI_LPHYCP4_RXVLDRXL0SSP << N_DMI_LPHYCP4_RXVLDRXL0SSP) | (V_DMI_LPHYCP4_RXL0SEG12FTSE << N_DMI_LPHYCP4_RXL0SEG12FTSE) |
                B_DMI_LPHYCP4_G1AREDRL0S | B_DMI_LPHYCP4_G2AREDRL0S | B_DMI_LPHYCP4_G3AREDRL0S | B_DMI_LPHYCP4_G4AREDRL0S | B_DMI_LPHYCP4_PLLBUSDRC);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_LPHYCP4, Data32And, Data32Or);

  return EFI_SUCCESS;
}

/**
  Configure DMI PLL Shutdown.

  @param[in] PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigurePllShutdown (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;
  //
  // Enable PCI Express GEN2 and GEN3 PLL Off
  // If enabled and no devices require the use of the PXP PLL clock outputs,
  // the PXP PLL can be shutdown.
  //
  MmioAndThenOr8 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PHYCTL2,
    (UINT8)~0,
    (B_DMI_PHYCTL2_PXPG3PLLOFFEN | B_DMI_PHYCTL2_PXPG2PLLOFFEN)
    );

  DEBUG ((DEBUG_INFO, "R_DMI_PHYCTL2 reg value: 0x%x\n", MmioRead8 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PHYCTL2)));

  return EFI_SUCCESS;
}

/**
  Configure DMI Power Gating.
  This needs to done before enabling ASPM L1

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigurePowerGating (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32           Data32And;
  UINT32           Data32Or;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PWRCTL,
    (UINT32)~(B_DMI_PWRCTL_WPDMPGEP),
    (UINT32) (B_DMI_PWRCTL_DLP)
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEPMECTL,
    (UINT32) ~(B_DMI_PCIEPMECTL_L1LTRTLV_MASK | B_DMI_PCIEPMECTL_L1LE | B_DMI_PCIEPMECTL_L1OCREWD),
    (V_DMI_PCIEPMECTL_L1LTRTLV << N_DMI_PCIEPMECTL_L1LTRTLV) | (V_DMI_PCIEPMECTL_L1LTRTLSV << N_DMI_PCIEPMECTL_L1LTRTLSV)
    );

  Data32And = ~(UINT32)(B_DMI_PCIEPMECTL2_CPGENH | B_DMI_PCIEPMECTL2_CPGEXH);
  Data32Or = (B_DMI_PCIEPMECTL2_PHYCLPGE | B_DMI_PCIEPMECTL2_DISSCPGE | B_DMI_PCIEPMECTL2_L1SCPGE | B_DMI_PCIEPMECTL2_FDCPGE |
              B_DMI_PCIEPMECTL2_L23RDYSCPGE | B_DMI_PCIEPMECTL2_DETSCPGE);
  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEPMECTL2,
    Data32And,
    Data32Or
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCE,
    (UINT32)~(B_DMI_PCE_PMCRE | B_DMI_PCE_HAE),
    (B_DMI_PCE_PMCRE | B_DMI_PCE_SE)
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PME_CTL3,
    (UINT32)~(B_DMI_PME_CTL3_L1PGAUTOPGEN | B_DMI_PME_CTL3_PMREQCPGEXH),
    V_DMI_PME_CTL3_PMREQCPGEXH
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_HWSNR,
    (UINT32)~(B_DMI_HWSNR_BEPW | B_DMI_HWSNR_REPW | B_DMI_HWSNR_EEH),
    V_DMI_HWSNR_BEPW | (V_DMI_HWSNR_REPW << N_DMI_HWSNR_REPW) | (V_DMI_HWSNR_EEH << N_DMI_HWSNR_EEH)
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PGCTRL,
    (UINT32)~(B_DMI_PGCTRL_PMREQBLKRSPT),
    V_DMI_PGCTRL_PMREQBLKRSPT
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_ADVMCTRL,
    (UINT32)~(B_DMI_ADVMCTRL_PMREQBLKPGRSPT | B_DMI_ADVMCTRL_PMREQCWC),
    (V_DMI_ADVMCTRL_PMREQBLKPGRSPT << N_DMI_ADVMCTRL_PMREQBLKPGRSPT) | (V_DMI_ADVMCTRL_PMREQCWC << N_DMI_ADVMCTRL_PMREQCWC)
    );

  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEPMECTL2, (UINT32)B_DMI_PCIEPMECTL2_CPMCSRE);

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PGTHRES,
    (UINT32)~(B_DMI_PGTHRES_L1PGLTREN | B_DMI_PGTHRES_L1PGLTRTLSV | B_DMI_PGTHRES_L1PGLTRTLV_MASK),
    (V_DMI_PGTHRES_L1PGLTRTLSV << N_DMI_PGTHRES_L1PGLTRTLSV) | (V_DMI_PGTHRES_L1PGLTRTLV << N_DMI_PGTHRES_L1PGLTRTLV)
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_MPC2,
    (UINT32)~(B_DMI_MPC2_PTNFAE | B_DMI_MPC2_EOIFD | B_DMI_MPC2_DISPLLEWL1SE),
    B_DMI_MPC2_PTNFAE | B_DMI_MPC2_EOIFD | B_DMI_MPC2_IPF
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_MPC,
    (UINT32)~(B_DMI_MPC_IRRCE | B_DMI_MPC_FCP_MASK),
     B_DMI_MPC_BT | B_DMI_MPC_FCDL1E | (V_DMI_MPC_FCP << N_DMI_MPC_FCP)
    );

  MmioAnd8 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PHYCTL2,
            (UINT8) ~(B_DMI_PHYCTL2_TDFT | B_DMI_PHYCTL2_TXCFGCHGWAIT));

  MmioAnd8 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_IOSFSBCS, (UINT8)~(B_DMI_IOSFSBCS_SIID));

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEDBG,
    ~0u,
    B_DMI_PCIEDBG_CTONFAE
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_LCAP,
    (UINT32)~B_DMI_LCAP_ASPMOC,
    B_DMI_LCAP_ASPMOC
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIECFG2,
    (UINT32)~B_DMI_PCIECFG2_PMET,
    (V_DMI_PCIECFG2_PMET_10MS << N_DMI_PCIECFG2_PMET)
    );

  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_CTRL2, (UINT32)B_DMI_CTRL2_PMETOFD);

  //
  // Dynamic Link Throttling
  //
  Data32And = (UINT32)(B_DMI_TNPT_TT | B_DMI_TNPT_DRXLTE | B_DMI_TNPT_DTXLTE);
  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_TNPT,
    Data32And,
    (V_DMI_TNPT_TP << N_DMI_TNPT_TP)
    );

  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PHYPG,
   (UINT32)(B_DMI_PHYPG_DETPHYPGE | B_DMI_PHYPG_DISPHYPGE | B_DMI_PHYPG_L23PHYPGE | B_DMI_PHYPG_DUCFGPHYPGE));

  return EFI_SUCCESS;
}

/**
  Configure DMI Coalescing

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureCoalescing (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32           Data32Or;
  UINT32           Data32And;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32And = ~(UINT32)(B_DMI_COCTL_CT_MASK | B_DMI_COCTL_PCLM | B_DMI_COCTL_NPCLM);
  Data32Or  = B_DMI_COCTL_PWCE | B_DMI_COCTL_ROAOP | (V_DMI_COCTL_CT << N_DMI_COCTL_CT) | B_DMI_COCTL_DDCE | B_DMI_COCTL_CTE |
              B_DMI_COCTL_CHAINBARBE | (V_DMI_COCTL_PCLM << N_DMI_COCTL_PCLM) | (V_DMI_COCTL_NPCLM << N_DMI_COCTL_NPCLM);
  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_COCTL,
    Data32And,
    Data32Or
    );

  DEBUG ((DEBUG_INFO, "R_DMI_COCTL reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_COCTL)));

  ///
  /// Chain Bit Generation Mode(CBGM)
  ///
  MmioOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_ACRG3,
    B_DMI_ACRG3_CBGM
    );
  DEBUG ((DEBUG_INFO, "R_DMI_ACRG3 DMI= %x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_ACRG3)));

  return EFI_SUCCESS;
}

/**
  Configure DMI Atomics

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureAtomics (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32          Data32And;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32And = (UINT32)~(B_DMI_DCAP2_AC64BS | B_DMI_DCAP2_AC32BS | B_DMI_DCAP2_ARS);
  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCAP2, Data32And);

  Data32And = (UINT32)~(B_DMI_DCTL2_AEB | B_DMI_DCTL2_ARE);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCTL2, Data32And, B_DMI_DCTL2_AFE);

  DEBUG ((DEBUG_INFO, "R_DMI_DCAP2 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCAP2)));
  DEBUG ((DEBUG_INFO, "R_DMI_DCTL2 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCTL2)));

  return EFI_SUCCESS;
}

/**
  Configure DMI SMI/SCI

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureSmiSci (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32     Data32And;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32And = (UINT32)~(B_DMI_EQCFG1_LERSMIE | B_DMI_EQCFG1_LERSCIE);
  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_EQCFG1, Data32And);

  DEBUG ((DEBUG_INFO, "R_DMI_EQCFG1 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_EQCFG1)));

  return EFI_SUCCESS;
}

/**
  Configure DMI Design Specific Configuration

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureDesignSpecificConfiguration (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32           Data32And;
  UINT32           Data32Or;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32And = (UINT32) ~(B_DMI_PCIECFG2_RLLG3R | B_DMI_PCIECFG2_RRCP);
  Data32Or  = (UINT32) B_DMI_PCIECFG2_CRSREN;
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIECFG2, Data32And, Data32Or);

  Data32And = (UINT32)~(B_DMI_DC_DCT1C | B_DMI_DC_COM);
  Data32Or = (UINT32)(B_DMI_DC_RCRBNRCE);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DC, Data32And, Data32Or);

  Data32And = (UINT32)~(B_DMI_PCIEALC_SSRLD | B_DMI_PCIEALC_SSRRS);
  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEALC, Data32And);

  Data32Or = (B_DMI_IPCS_IMPS | B_DMI_IPCS_IMRS);
  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_IPCS, Data32Or);
  DEBUG ((DEBUG_INFO, "R_DMI_IPCS reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_IPCS)));

  Data32And = (UINT32) ~(B_DMI_CCFG_IORE | B_DMI_CCFG_UNRD | B_DMI_CCFG_UPSD);
  Data32Or  = (UINT32) (B_DMI_CCFG_UPRS | B_DMI_CCFG_UNRS);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_CCFG, Data32And, Data32Or);

  Data32And = (UINT32)~(B_DMI_ACRG3_RRXC | B_DMI_ACRG3_RRXDME | B_DMI_ACRG3_ADESKEW_DIS);
  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_ACRG3, Data32And);

  Data32And = (UINT32)~B_DMI_PWRCTL_DARECE;
  Data32Or  = (UINT32) B_DMI_PWRCTL_LIFECF;
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PWRCTL, Data32And, Data32Or);

  Data32And = (UINT32)~B_DMI_PCIEDBG_LDSWQRP;
  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEDBG, Data32And);

  Data32And = (UINT32)~(B_DMI_ADVMCTRL_RRLLCL | B_DMI_ADVMCTRL_RLLG12R);
  Data32Or  = (UINT32)(B_DMI_ADVMCTRL_INRXL0CTRL | B_DMI_ADVMCTRL_EIOSDISDS |
               B_DMI_ADVMCTRL_EIOSMASKRX | B_DMI_ADVMCTRL_RXL0DC | B_DMI_ADVMCTRL_G3STFER);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_ADVMCTRL, Data32And, Data32Or);

  MmioOr16 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_RPPGEN,
    B_DMI_RPPGEN_PTOTOP
    );

  MmioOr8 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_EINJCTL,
    B_DMI_EINJCTL_EINJDIS
    );

  Data32And = (UINT32) ~(B_DMI_LPHYCP4_TLSPDRS);
  Data32Or  = (UINT32) B_DMI_LPHYCP4_DPMDNDBFE;
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_LPHYCP4, Data32And, Data32Or);

  MmioAnd16 (
    PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_FCTL,
    (UINT16)~(B_DMI_FCTL_CRSPSEL)
    );

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_FCTL,
    (UINT32)~(B_DMI_FCTL_CTM),
    (V_DMI_FCTL_CTM << N_DMI_FCTL_CTM)
  );

  MmioAnd16 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_RCTL,
    (UINT16)~(B_DMI_RCTL_CRSSVE)
    );

  MmioAnd16 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_ROOTCAP,
    (UINT16)~(B_DMI_ROOTCAP_CRSSV)
    );

  MmioOr32 (
    PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_FCTL2,
    (B_DMI_FCTL2_RXCPPREALLOCEN | V_DMI_FCTL2_RPTOT << B_DMI_FCTL2_RPTOT_OFFSET)
    );

  return EFI_SUCCESS;
}

/**
  Configure DMI Enabling

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureEnabling (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32           Data32And;
  UINT32           Data32Or;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32And = (UINT32)~(B_DMI_PCIEALC_ITLRCLD | B_DMI_PCIEALC_ILLRCLD | B_DMI_PCIEALC_BLKDQDASD | B_DMI_PCIEALC_ONPRASPML1P);
  Data32Or  = (UINT32)(B_DMI_PCIEALC_RTD3PDSP);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEALC, Data32And, Data32Or);

  DEBUG ((DEBUG_INFO, "R_DMI_PCIEALC reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEALC)));

  return EFI_SUCCESS;
}

/**
  Configure DMI 10 Bit Tag and Scaled Flow Control

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigure10BitTagAndScaledFlowControl (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCAP2,
    (UINT32) ~(B_DMI_DCAP2_PX10BTRS | B_DMI_DCAP2_PX10BTCS),
    0
    );

  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCTL2, (UINT32) ~B_DMI_DCTL2_PX10BTRE);

  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_ADVMCTRL, (UINT32) ~(B_DMI_ADVMCTRL_F10BTSE | B_DMI_ADVMCTRL_CCBE));

  return EFI_SUCCESS;
}

/**
  Configure DMI TLP Prefix

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureTlpPrefix (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCAP2, B_DMI_DCAP2_EFFS);

  DEBUG ((DEBUG_INFO, "R_DMI_DCAP2 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCAP2)));

  return EFI_SUCCESS;
}

/**
  Configure DMI Ack Latency

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureAckLatency (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32           Data32And;
  UINT32           Data32Or;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32And = (UINT32)~(B_DMI_PCIEATL_G1X1);
  Data32Or  = (V_DMI_PCIEATL_G1X1 << N_DMI_PCIEATL_G1X1);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEATL, Data32And, Data32Or);

  DEBUG ((DEBUG_INFO, "R_DMI_PCIEATL reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PCIEATL)));

  return EFI_SUCCESS;
}

/**

  Poll DP DMI negotiation completion

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig

  @retval EFI_SUCCESS             -  Succeed.
  @retval EFI_INVALID_PARAMETER   -  Wrong phase parameter passed in.
**/

EFI_STATUS
EFIAPI
DpDmiPollVcStatus (
  IN  PEI_DMI_CONFIG  *PeiDpDmiConfig
  )
{

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  while ((MmioRead16 ((UINTN) (PeiDpDmiConfig->DmiBaseAddress + R_DMI_V0STS)) & B_DMI_V0STS_NP) != 0);

  DEBUG ((DEBUG_INFO, "%x \n", PeiDpDmiConfig->DmiBaseAddress + R_DMI_V0STS));

  while ((MmioRead16 ((UINTN) (PeiDpDmiConfig->DmiBaseAddress + R_DMI_V1STS)) & B_DMI_V1STS_NP) != 0);

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Configure DMI MultiVC support

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureMultiVC (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32           Data32Or;
  UINT32           Data32And;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PVCCR1,
   (UINT32) ~B_DMI_PVCCR1_EVCC,
    V_DMI_PVCCR1_EVCC
    );

  Data32And = (UINT32)~B_DMI_V0VCRC_MTS_MASK;
  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_V0VCRC, Data32And);

  Data32And = (UINT32)~B_DMI_V1VCRC_MTS_MASK;
  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_V1VCRC, Data32And);

  Data32And = (UINT32)~(B_DMI_VC0CTL_ETVM_MASK | B_DMI_VC0CTL_TVM_MASK);
  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_VC0CTL, Data32And);

  Data32And = (UINT32)~(B_DMI_VC1CTL_ID_MASK | B_DMI_VC1CTL_ETVM_MASK);
  Data32Or = (UINT32)(B_DMI_VC1CTL_EN | (V_DMI_VC1CTL_ID << N_DMI_VC1CTL_ID) | B_DMI_VC1CTL_TVM_MASK );
  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_VC1CTL,
    Data32And,
    Data32Or
    );

  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_RSPVCC, B_DMI_RSPVCC_RS3VCCP);

  MmioAnd32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_RS3V0CTL, (UINT32) ~B_DMI_RS3V0CTL_TVM);

  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_RS3V1CTL, (B_DMI_RS3V1CTL_EN | V_DMI_RS3V1CTL_TVM << N_DMI_RS3V1CTL_TVM));

  DEBUG ((DEBUG_INFO, "R_DMI_V0VCRC reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_V0VCRC)));
  DEBUG ((DEBUG_INFO, "R_DMI_V1VCRC reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_V1VCRC)));
  DEBUG ((DEBUG_INFO, "R_DMI_PVCCR1 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PVCCR1)));
  DEBUG ((DEBUG_INFO, "R_DMI_VC0CTL reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_VC0CTL)));
  DEBUG ((DEBUG_INFO, "R_DMI_VC1CTL reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_VC1CTL)));
  DEBUG ((DEBUG_INFO, "R_DMI_RSPVCC reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_RSPVCC)));
  DEBUG ((DEBUG_INFO, "R_DMI_RS3V0CTL reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_RS3V0CTL)));
  DEBUG ((DEBUG_INFO, "R_DMI_RS3V1CTL reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_RS3V1CTL)));

  DpDmiPollVcStatus (PeiDpDmiConfig);

  return EFI_SUCCESS;
}

/**
  Configure DMI Ptm Init

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigurePtm (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32           Data32Or;
  UINT32           Data32And;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PTMPSDC1, 0xF44BFF53);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PTMPSDC2, 0xA01ED247);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PTMPSDC3, 0x781A851C);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PTMPSDC4, 0x3E114B0D);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PTMPSDC5, 0x3A14);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PTMPSDC6, 0xC845);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PTMPSDC7, 0x761D);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PTMPSDC8, 0x3317);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PTMPSDC9, 0x2C0D330F);
  MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PTMPSDC10, 0x270C2C0C);

  Data32And = (UINT32)~(B_DMI_PTMCAPR_PTMREQC);
  Data32Or  = (B_DMI_PTMCAPR_PTMRSPC | B_DMI_PTMCAPR_PTMRC);
  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PTMCAPR,
    Data32And,
    Data32Or
    );

  MmioOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PTMCTLR,
    (B_DMI_PTMCTLR_PTME | B_DMI_PTMCTLR_RS)
    );

  Data32And = (UINT32)~(B_DMI_PTMECFG_PTMRNOPAD | B_DMI_PTMECFG_PLTLFF | B_DMI_PTMECFG_PGTSCFE | B_DMI_PTMECFG_PGTSCFF);
  Data32Or  = ((V_DMI_PTMECFG_PTMRNOPAD << N_DMI_PTMECFG_PTMRNOPAD) | B_DMI_PTMECFG_PTMRPAE | (V_DMI_PTMECFG_PLTLFF << N_DMI_PTMECFG_PLTLFF) | (V_DMI_PTMECFG_PGTSCFF << N_DMI_PTMECFG_PGTSCFF) | V_DMI_PTMECFG_IOSFMADP);
  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_PTMECFG,
    Data32And,
    Data32Or
  );

  DEBUG ((DEBUG_INFO, "R_DMI_PTMECFG reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_PTMECFG)));

  return EFI_SUCCESS;
}

/**
  Configure DMI Retimer support

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureRetimerSupport (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  MmioOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_LCAP2,
    B_DMI_LCAP2_RPDS
  );

  DEBUG ((DEBUG_INFO, "R_DMI_LCAP2 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_LCAP2)));

  return EFI_SUCCESS;
}

/**
  Configure DMI Rx Primary Cycle Decode support

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureRxPrimaryCycleDecode (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32           Data32Or;
  UINT32           Data32And;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32And = (UINT32)~(B_DMI_DECCTL_RXIMDECEN | B_DMI_DECCTL_URRXURIDVDM | B_DMI_DECCTL_RXSBEMCAPDECEN | B_DMI_DECCTL_URRXUVDMINTELVID | B_DMI_DECCTL_URRXUVDMUVID | B_DMI_DECCTL_RXVDMDECE | B_DMI_DECCTL_RXLTRMDECH);
  Data32Or  = (B_DMI_DECCTL_ATSCE | B_DMI_DECCTL_BUSNUMZCHK | B_DMI_DECCTL_MTRXLTC | B_DMI_DECCTL_OUTBEXPCPLCHKEN | B_DMI_DECCTL_DROPCPLATNZCE |
               B_DMI_DECCTL_LCRXINT | B_DMI_DECCTL_RXGPEDECEN | B_DMI_DECCTL_RXADLEDDECEN | B_DMI_DECCTL_LCRXERRMSG | B_DMI_DECCTL_VDMATAC | B_DMI_DECCTL_URVDME16DW |
               B_DMI_DECCTL_URRXURTRCVDM | B_DMI_DECCTL_URRXULTVDM | B_DMI_DECCTL_URRXURAVDM |  B_DMI_DECCTL_URRXUORVDM | B_DMI_DECCTL_URRXUVDMRBFRC |
               B_DMI_DECCTL_URRXUVDMRGRTRC | B_DMI_DECCTL_URRXVMCTPBFRC | B_DMI_DECCTL_RXMCTPDECEN | B_DMI_DECCTL_RXMCTPBRCDECEN | B_DMI_DECCTL_URRXMCTPNTCO |
               B_DMI_DECCTL_ICHKINVREQRMSGRBID | B_DMI_DECCTL_LCRXPTMREQ);

  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_DECCTL,
    Data32And,
    Data32Or
  );

  DEBUG ((DEBUG_INFO, "R_DMI_DECCTL reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_DECCTL)));

  MmioAnd32 (
    PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PIDECCTL,
    (UINT32)~(B_DMI_PIDECCTL_CPLBNCHK)
  );

  MmioOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_MPC,
    B_DMI_MPC_MCTPSE
  );

  MmioOr32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_MPC2,
    B_DMI_MPC2_EOIFD
  );

  MmioAnd32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_CCFG,
    (UINT32)~(B_DMI_CCFG_CRE | B_DMI_CCFG_IORE)
  );

  return EFI_SUCCESS;
}

/**
  Set Max Payload Size Control.

  @param[in] UINT32    DmiMaxPayloadSize
**/
VOID
DpDmiSetMpsCtrl (
  IN  VOID    *PeiDmiConfig,
  IN  UINT32  DmiMaxPayloadSize
  )
{
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  if (PeiDmiConfig != NULL) {
    PeiDpDmiConfig = PeiDmiConfig;
    MmioAndThenOr32 (
      PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCTL,
      (UINT32)~(B_DMI_DCTL_MPS),
      (DmiMaxPayloadSize << N_DMI_DCTL_MPS_256)
    );
  }
}

/**
  Configure DPDMI Max Payload Size.

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureMPS (
  IN  VOID    *PeiDmiConfig
  )
{
  UINT32           Data32;
  UINT32           DmiMaxPayloadSizeCap;
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32 = MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCAP);
  DEBUG ((DEBUG_INFO, "R_DMI_DCAP reg value: 0x%x\n", Data32));
  DmiMaxPayloadSizeCap = Data32 & B_DMI_DCAP_MPS;

  //
  // Checking if B_DMI_DCAP_MPS[2:0] is set to 1(256B).
  // If yes, then DMI can be configured for MPS of 256B or 128B
  //
  if (DmiMaxPayloadSizeCap == V_DMI_DCAP_MPS_256) {
    DpDmiSetMpsCtrl (PeiDpDmiConfig, DmiMaxPayloadSizeCap);
  }
  DEBUG ((DEBUG_INFO, "R_DMI_DCTL reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCTL)));

  return EFI_SUCCESS;
}

/**
  DPDMI Enable Relaxed Order
  @param[in] PEI_DMI_CONFIG     *PeiDmiConfig

**/
EFI_STATUS
EFIAPI
DpDmiEnablePcieRelaxedOrder (
    IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;
  MmioAndThenOr32 (
    PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_IPSCE,
    (UINT32)~(B_DMI_IPSCE_TXROM | B_DMI_IPSCE_RXROM),
    (V_DMI_IPSCE_TXROM << N_DMI_IPSCE_TXROM)
  );
  DEBUG ((DEBUG_INFO, "R_DMI_IPSCE reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_IPSCE)));

  return EFI_SUCCESS;
}

/**
  Configure Rx Margin Programming
  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig

**/
EFI_STATUS
EFIAPI
DpDmiConfigureRxMarginProgramming (
    IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG  *PeiDpDmiConfig;
  UINT32          Data32And;
  UINT32          Data32Or;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32And = (UINT32)~(B_DMI_RXMC1_MSRTS_MASK | B_DMI_RXMC1_TMSLOP_MASK | B_DMI_RXMC1_MVS | B_DMI_RXMC1_MIUDVMS
                       | B_DMI_RXMC1_VMSLOP_MASK | B_DMI_RXMC1_MMNOLS_MASK | B_DMI_RXMC1_MIESS);
  Data32Or  = (UINT32) (V_DMI_RXMC1_MSRTS << B_DMI_RXMC1_MSRTS_OFFSET) | (V_DMI_RXMC1_TMSLOP << B_DMI_RXMC1_TMSLOP_OFFSET)
              | (V_DMI_RXMC1_VMSLOP << B_DMI_RXMC1_VMSLOP_OFFSET) | B_DMI_RXMC1_MILRTS | B_DMI_RXMC1_MSRM;
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_RXMC1, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "R_DMI_RXMC1 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_RXMC1)));

  Data32And = (UINT32)~(B_DMI_RXMC2_MMTOS_MASK | B_DMI_RXMC2_MNOTSS_MASK);
  Data32Or  = (V_DMI_RXMC2_MMTOS << B_DMI_RXMC2_MMTOS_OFFSET) | (V_DMI_RXMC2_MNOTSS << B_DMI_RXMC2_MNOTSS_OFFSET);

  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_RXMC2, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "R_DMI_RXMC2 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_RXMC2)));

  MmioAnd32 ( PeiDpDmiConfig->DmiBaseAddress + R_DMI_PL16MPCPS, (UINT32) ~(B_DMI_PL16MPCPS_MARGINDRISW));

  return EFI_SUCCESS;
}

/**
  Configure DMI DPC support

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureDPC (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;

  MmioAnd32 (
    PeiDpDmiConfig->DmiBaseAddress + R_DMI_DPCCAPR,
    (UINT32)~(B_DMI_DPCCAPR_RPEFDPC)
  );

  DEBUG ((DEBUG_INFO, "R_DMI_DPCCAPR reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DPCCAPR)));

  return EFI_SUCCESS;
}


/**
    This function performs configuration of DMI D3 Vnn removal

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureD3VnnRemCtl (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG  *PeiDpDmiConfig;

  PeiDpDmiConfig = PeiDmiConfig;
  UINT32   Data32And;

  Data32And = (UINT32)~(B_DMI_CFG_VNNREMCTL_ISPLFVNNRE | B_DMI_CFG_VNNREMCTL_LRSLFVNNRE);
  MmioAnd32(PeiDpDmiConfig->DmiBaseAddress + R_DMI_CFG_VNNREMCTL, Data32And);
  DEBUG ((DEBUG_INFO, "R_DMI_CFG_VNNREMCTL reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_CFG_VNNREMCTL)));
  return EFI_SUCCESS;
}


/**
  This function configure PCIe power down mapping

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigurePowerDownMapping (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG  *PeiDpDmiConfig;
  UINT32   Data32And;
  UINT32   Data32Or;

  PeiDpDmiConfig = PeiDmiConfig;

  Data32And = (UINT32)~(B_DMI_PIPEPDCTL_DETPGPDCTL | B_DMI_PIPEPDCTL_L23NOPGPDCTL | B_DMI_PIPEPDCTL_L23PGPDCTL | B_DMI_PIPEPDCTL_DISPGPDCTL | B_DMI_PIPEPDCTL_DISNOPGPDCTL |B_DMI_PIPEPDCTL_L1PGNOPGPDCTL);
  Data32Or  = (UINT32) (V_DMI_PIPEPDCTL_DETPGPDCTL << N_DMI_PIPEPDCTL_DETPGPDCTL) | (V_DMI_PIPEPDCTL_L23NOPGPDCTL << N_DMI_PIPEPDCTL_L23NOPGPDCTL) |
              (V_DMI_PIPEPDCTL_L23PGPDCTL << N_DMI_PIPEPDCTL_L23PGPDCTL) | (V_DMI_PIPEPDCTL_DISNOPGPDCTL << N_DMI_PIPEPDCTL_DISNOPGPDCTL) |
              (V_DMI_PIPEPDCTL_DISPGPDCTL << N_DMI_PIPEPDCTL_DISPGPDCTL) | (V_DMI_PIPEPDCTL_L1PGNOPGPDCTL << N_DMI_PIPEPDCTL_L1PGNOPGPDCTL) |
              (V_DMI_PIPEPDCTL_L1PGUPGPDCTL << N_DMI_PIPEPDCTL_L1PGUPGPDCTL);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress  + SIZE_8KB + R_DMI_PIPEPDCTL, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "R_DMI_PIPEPDCTL reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress  + SIZE_8KB + R_DMI_PIPEPDCTL)));

  Data32And = (UINT32)~(B_DMI_PIPEPDCTL2_DUCFGPGPDCTL | B_DMI_PIPEPDCTL2_L1UPNOPGPDCTL | B_DMI_PIPEPDCTL2_L1D1UPUPGPDCTL | B_DMI_PIPEPDCTL2_L1D1PGNOPGPDCTL | B_DMI_PIPEPDCTL2_L1D1PGPGPDCTL | B_DMI_PIPEPDCTL2_L1D2NOPGPDCTL | B_DMI_PIPEPDCTL2_L1D2PGPDCTL);
  Data32Or  = (UINT32) (V_DMI_PIPEPDCTL2_DUCFGPGPDCTL << N_DMI_PIPEPDCTL2_DUCFGPGPDCTL) | (V_DMI_PIPEPDCTL2_L1UPNOPGPDCTL << N_DMI_PIPEPDCTL2_L1UPNOPGPDCTL) | (V_DMI_PIPEPDCTL2_L1D1UPUPGPDCTL << N_DMI_PIPEPDCTL2_L1D1UPUPGPDCTL) |
              (V_DMI_PIPEPDCTL2_L1D1PGNOPGPDCTL << N_DMI_PIPEPDCTL2_L1D1PGNOPGPDCTL) | (V_DMI_PIPEPDCTL2_L1D1PGPGPDCTL << N_DMI_PIPEPDCTL2_L1D1PGPGPDCTL) | (V_DMI_PIPEPDCTL2_L1D2NOPGPDCTL << N_DMI_PIPEPDCTL2_L1D2NOPGPDCTL) |
              (V_DMI_PIPEPDCTL2_L1D2PGPDCTL << N_DMI_PIPEPDCTL2_L1D2PGPDCTL);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PIPEPDCTL2, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "R_DMI_PIPEPDCTL2 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PIPEPDCTL2)));

  Data32And = (UINT32)~(B_DMI_PIPEPDCTL3_L1DLOWNOPGPDCTL | B_DMI_PIPEPDCTL3_L1DLOWPGPDCTL);
  Data32Or  = (UINT32) (V_DMI_PIPEPDCTL3_L1DLOWNOPGPDCTL << N_DMI_PIPEPDCTL3_L1DLOWNOPGPDCTL) | (V_DMI_PIPEPDCTL3_L1DLOWPGPDCTL << N_DMI_PIPEPDCTL3_L1DLOWPGPDCTL);
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PIPEPDCTL3, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "R_DMI_PIPEPDCTL3 reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PIPEPDCTL3)));

  Data32And = (UINT32)~(B_DMI_PIPEPDCTLEXT_P2TP2TCD | B_DMI_PIPEPDCTLEXT_P2TP2TP | B_DMI_PIPEPDCTLEXT_P2UGTPGSM | B_DMI_PIPEPDCTLEXT_LSDPMRFM | B_DMI_PIPEPDCTLEXT_PDDPMRFM);
  Data32Or  = (UINT32) ((V_DMI_PIPEPDCTLEXT_LSDPMRFM << N_DMI_PIPEPDCTLEXT_LSDPMRFM) | (V_DMI_PIPEPDCTLEXT_PDDPMRFM << N_DMI_PIPEPDCTLEXT_PDDPMRFM));
  MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PIPEPDCTLEXT, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "R_DMI_PIPEPDCTLEXT reg value: 0x%x\n", MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + SIZE_8KB + R_DMI_PIPEPDCTLEXT)));

  return EFI_SUCCESS;
}

/**
  Lock SRL bits
  @param[in] DmiBar                 DmiBar
**/
VOID
DmiSetSecuredRegisterLock (
  IN  UINT32                       DmiBar
  )
{
  UINT32                                Data32Or;

  DEBUG ((DEBUG_INFO, "DmiSetSecuredRegisterLock () Start!\n"));

  ///Secure Register Lock (SRL) 0xC8C[0]: 1
  ///Secure Equalization Register Lock (SERL) 0XC8C[8]: 1
  ///Device ID Override Lock (DIDOVR_LOCK) 0xC8C[24]: 1
  Data32Or = (B_DMI_LPCR_SRL | B_DMI_LPCR_SERL | B_DMI_LPCR_DIDOVR_LOCK);
  MmioOr32 (DmiBar + R_DMI_LPCR, Data32Or);
  DEBUG ((DEBUG_INFO, "R_DMI_LPCR after setting SRL = %x\n", MmioRead32 (DmiBar + R_DMI_LPCR)));

  DEBUG ((DEBUG_INFO, "DmiSetSecuredRegisterLock () End!\n"));
}

/**
  Create DPDMI Hob
**/

VOID
CreateDmiHob (
  VOID
)
{
  EFI_STATUS            Status;
  DMI_HOB               *DmiHob;

  Status = PeiServicesCreateHob (
            EFI_HOB_TYPE_GUID_EXTENSION,
            sizeof (DMI_HOB),
            (VOID **) &DmiHob
            );
  DEBUG ((DEBUG_INFO, "DmiHob Created \n"));
  ASSERT_EFI_ERROR (Status);

  if (DmiHob != NULL) {
    //
    // Initialize the DMI HOB.
    //
    DmiHob->EfiHobGuidType.Name = gDmiHobGuid;
    ZeroMem ((VOID *)&DmiHob->DpDmiInfoHob, sizeof(DP_DMI_INFO_HOB));

    DEBUG ((DEBUG_INFO, "DmiHob->EfiHobGuidType.Name: %g\n", &DmiHob->EfiHobGuidType.Name));
    DEBUG ((DEBUG_INFO, "DmiHob @ %X\n", DmiHob));
    DEBUG ((DEBUG_INFO, "DmiHob - HobHeader: %X\n", sizeof (DMI_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  }
}

/**
  Get max Dmi link speed supported

  @param[in] DmiBar          DmiBar
  @return    Max link speed
**/
UINT32
DmiGetMaxLinkSpeed (
  IN  UINT32                       DmiBar
  )
{
  return MmioRead32 (DmiBar + R_DMI_LCAP) & B_DMI_LCAP_MLS;
}

/**
  Configure Gen3 PresetToCoef

  @param[in] UINT32          DmiBar
**/
VOID
DmiGen3PresetToCoeff (
  IN  UINT32       DmiBar
  )
{
  //
  // Before training to GEN3 Configure Lane0 P0-P10 Preset Coefficient mapping based on recommendation for CPU DMI
  //
  DEBUG ((DEBUG_INFO, "DmiGen3PresetToCoeff!\n"));
  MmioWrite32 (DmiBar + R_DMI_L0P0P1PCM,   0x00a0c024);
  MmioWrite32 (DmiBar + R_DMI_L0P1P2P3PCM, 0x2a280988);
  MmioWrite32 (DmiBar + R_DMI_L0P3P4PCM,   0x00030180);
  MmioWrite32 (DmiBar + R_DMI_L0P5P6PCM,   0x06a8012C);
  MmioWrite32 (DmiBar + R_DMI_L0P6P7P8PCM, 0x24206880);
  MmioWrite32 (DmiBar + R_DMI_L0P8P9PCM,   0x00228186);
  MmioWrite32 (DmiBar + R_DMI_L0P10PCM,    0x00010020);
  MmioWrite32 (DmiBar + R_DMI_L0LFFS,      0x00003010);
}

/**
  Configure Gen4 PresetToCoef

  @param[in] UINT32          DmiBar
**/
VOID
DmiGen4PresetToCoeff (
  IN  UINT32       DmiBar
  )
{
  //
  // Before training to GEN4 Configure 16.0 GT/s P0-P10 Preset Coefficient mapping based on recommendation for CPU DMI
  //
  DEBUG ((DEBUG_INFO, "DmiGen4PresetToCoeff!\n"));

  MmioWrite32 (DmiBar + R_DMI_PX16GP0P1PCM,   0x00A0C024);
  MmioWrite32 (DmiBar + R_DMI_PX16GP1P2P3PCM, 0x2A280988);
  MmioWrite32 (DmiBar + R_DMI_PX16GP3P4PCM,   0x00030180);
  MmioWrite32 (DmiBar + R_DMI_PX16GP5P6PCM,   0x06A8012C);
  MmioWrite32 (DmiBar + R_DMI_PX16GP6P7P8PCM, 0x24206880);
  MmioWrite32 (DmiBar + R_DMI_PX16GP8P9PCM,   0x00228186);
  MmioWrite32 (DmiBar + R_DMI_PX16GP10PCM,    0x00010020);
  MmioWrite32 (DmiBar + R_DMI_PX16GLFFS,      0x00003010);
}

/**
  Set Gen3 coefficient list entry.

  @param[in] DmiBar      DmiBar
  @param[in] ListEntry   ListEntry (0-9)
  @param[in] Cm          C-1
  @param[in] Cp          C+1
**/
VOID
DmiSetGen3Presets (
  IN  UINT32 DmiBar,
  IN  UINT32 ListEntry,
  IN  UINT32 Cm,
  IN  UINT32 Cp
  )
{
  UINT32  PreReg;
  UINT32  PostReg;
  UINT32  PreField;
  UINT32  PostField;
  UINT32  Data32And;
  UINT32  Data32Or;

  DEBUG ((DEBUG_VERBOSE, "DmiSetGen3Presets Start!\n"));

  ASSERT (ListEntry < 14);
  ASSERT ((Cm & ~0x3F) == 0);
  ASSERT ((Cp & ~0x3F) == 0);
  ///
  /// CPU DMI has 7 pairs of coefficients Cm (Co-efficient minus) and Cp (Co-efficient Plus) which is stored in RTPCL registers.
  /// Each of this is stored in 5 bitfields in RTPCL and is covered in 4 registers.
  ///
  PreReg    = ((ListEntry * 2)) / 5;
  PreField  = ((ListEntry * 2)) % 5;
  PostReg   = ((ListEntry * 2) + 1) / 5;
  PostField = ((ListEntry * 2) + 1) % 5;

  ASSERT (PreReg  < 4);
  ASSERT (PostReg < 4);

  Data32And = (UINT32) ~(0x3F << (6 * PreField));
  Data32Or  = (Cm << (6 * PreField));
  ASSERT ((Data32And & Data32Or) == 0);
  MmioAndThenOr32 (DmiBar + R_DMI_RTPCL1 + (PreReg  * 4), Data32And, Data32Or);

  DEBUG ((DEBUG_INFO, "ListEntry = %x offset = %x R_DMI_RTPCL1 = %x\n",ListEntry,
  (DmiBar + R_DMI_RTPCL1 + (PreReg * 4)), MmioRead32(DmiBar + R_DMI_RTPCL1 + (PreReg * 4))));

  Data32And = (UINT32) ~(0x3F << (6 * PostField));
  Data32Or  = (Cp << (6 * PostField));
  ASSERT ((Data32And & Data32Or) == 0);
  MmioAndThenOr32 (DmiBar + R_DMI_RTPCL1 + (PostReg * 4), Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "ListEntry = %x offset = %x R_DMI_RTPCL1 = %x\n",ListEntry,
  (DmiBar + R_DMI_RTPCL1 + (PostReg * 4)), MmioRead32(DmiBar + R_DMI_RTPCL1 + (PostReg * 4))));

  DEBUG ((DEBUG_INFO, "R_DMI_RTPCL1 %x \n",  MmioRead32(DmiBar + R_DMI_RTPCL1)));
  DEBUG ((DEBUG_INFO, "DmiSetGen3Presets End!\n"));
}

/**
  Set Gen4 coefficient list entry.

  @param[in] DmiBar      DmiBar
  @param[in] ListEntry   ListEntry (0-9)
  @param[in] Cm          C-1
  @param[in] Cp          C+1
**/
VOID
DmiSetGen4Presets (
  IN  UINT32 DmiBar,
  IN  UINT32 ListEntry,
  IN  UINT32 Cm,
  IN  UINT32 Cp
  )
{
  UINT32  PreReg;
  UINT32  PostReg;
  UINT32  PreField;
  UINT32  PostField;
  UINT32  Data32And;
  UINT32  Data32Or;

  DEBUG ((DEBUG_INFO, "DmiSetGen4Presets Start!\n"));

  ASSERT (ListEntry < 20);
  ASSERT ((Cm & ~0x3F) == 0);
  ASSERT ((Cp & ~0x3F) == 0);
  ///
  /// CPU DMI has 10 pairs of coefficients Cm (Co-efficient minus) and Cp (Co-efficient Plus) which is stored in PX16GRTPCL registers.
  /// Each of this is stored in 5 bitfields in PX16GRTPCL and is covered in 7 registers.
  ///
  PreReg    = ((ListEntry * 2)) / 5;
  PreField  = ((ListEntry * 2)) % 5;
  PostReg   = ((ListEntry * 2) + 1) / 5;
  PostField = ((ListEntry * 2) + 1) % 5;

  ASSERT (PreReg  < 7);
  ASSERT (PostReg < 7);

  Data32And = (UINT32) ~(0x3F << (6 * PreField));
  Data32Or  = (Cm << (6 * PreField));
  ASSERT ((Data32And & Data32Or) == 0);
  MmioAndThenOr32 (DmiBar + R_DMI_PX16GRTPCL1 + (PreReg  * 4), Data32And, Data32Or);

  DEBUG ((DEBUG_INFO, "ListEntry = %x offset = %x R_DMI_PX16GRTPCL1 = %x\n",ListEntry,
  (DmiBar + R_DMI_PX16GRTPCL1 + (PreReg * 4)), MmioRead32(DmiBar + R_DMI_PX16GRTPCL1 + (PreReg * 4))));

  Data32And = (UINT32) ~(0x3F << (6 * PostField));
  Data32Or  = (Cp << (6 * PostField));
  ASSERT ((Data32And & Data32Or) == 0);
  MmioAndThenOr32 (DmiBar + R_DMI_PX16GRTPCL1 + (PostReg * 4), Data32And, Data32Or);

  DEBUG ((DEBUG_INFO, "ListEntry = %x offset = %x R_DMI_PX16GRTPCL1 = %x\n",ListEntry,
  (DmiBar + R_DMI_PX16GRTPCL1 + (PostReg * 4)), MmioRead32(DmiBar + R_DMI_PX16GRTPCL1 + (PostReg * 4))));

  DEBUG ((DEBUG_INFO, "DmiSetGen4Presets End!\n"));
}

/**
  Configure DMI Hweq
  @param[in] DmiBar                 DmiBar
  @param[in] PEI_DMI_CONFIG         Instance of PEI_DMI_CONFIG
**/
VOID
DpDmiConfigureGen3HwEq (
  IN  UINT32                       DmiBar,
  IN  PEI_DMI_CONFIG               *PeiDpDmiConfig,
  IN  UINT8                        DmiEqMode
  )
{
  UINT32           Data32And;
  UINT32           Data32Or;
  UINT32           LaneIndex;
  UINT32           LaneRegisterIndex;
  UINT32           LaneIndexInsideRegister;
  UINT8            Gen3NumberOfPresetCoeffList;
  UINT8            DpDmiGen3PresetCoeffSelection;
  UINT8            Gen3PcetTimer;
  UINT8            Gen3TsLockTimer;
  UINT32           PreCursorReg;
  UINT32           PreCursorField;
  UINT32           PostCursorReg;
  UINT32           PostCursorField;
  UINT32           PreCursor;
  UINT32           PostCursor;

  LaneIndex                      = 0;
  LaneRegisterIndex              = 0;
  LaneIndexInsideRegister        = 0;
  DpDmiGen3PresetCoeffSelection  = 0;
  Gen3PcetTimer                  = 0;
  Gen3TsLockTimer                = 0;
  PreCursorReg                   = 0;
  PreCursorField                 = 0;
  PostCursorReg                  = 0;
  PostCursorField                = 0;
  PreCursor                      = 0;
  PostCursor                     = 0;

  DEBUG ((DEBUG_INFO, "DpDmiConfigureGen3HwEq Start!\n"));
  //
  // Step1:Program all the preset-coeff mapping registers
  //
  DmiGen3PresetToCoeff(DmiBar);

  //
  // Step3:Program RTPCL1.PCM = 0 to enable Preset mode - Gen3 Preset/Coefficient Mode Selection
  //
  DpDmiGen3PresetCoeffSelection = PeiDpDmiConfig->Gen3RtcoRtpoEnable;
  DEBUG ((DEBUG_INFO, "Gen3 Preset/Coefficient Mode Selection %x\n", DpDmiGen3PresetCoeffSelection));
  MmioAndThenOr32 (DmiBar + R_DMI_RTPCL1, (UINT32)~(B_DMI_RTPCL1_PCM), (DpDmiGen3PresetCoeffSelection << B_DMI_RTPCL1_PCM_OFFSET));
  // LTCO enable
  DEBUG ((DEBUG_INFO, "Gen3 Gen3LtcoEnable %x\n", PeiDpDmiConfig->Gen3LtcoEnable));
  if (PeiDpDmiConfig->Gen3LtcoEnable == 1) {
    for (LaneIndex = 0; LaneIndex < 4 ; LaneIndex ++) {
      PreCursor  = PeiDpDmiConfig->DmiGen3Ltcpre[LaneIndex];
      PostCursor = PeiDpDmiConfig->DmiGen3Ltcpo[LaneIndex];

      ASSERT ((PreCursor & ~0x3F) == 0);
      ASSERT ((PostCursor & ~0x3F) == 0);
      //
      // Each of the local transmitter override registers
      // holds configuration fields for 2 lanes.
      // The layout of the register is as follows
      // lower lane pre-cursor value
      // lower lane post-cursor value
      // upper lane pre-cursor value
      // upper lane post-cursor value
      // lower lane local transmitter override enable
      // upper lane local transmitter override enable
      // Cursor values fields are 6 bits wide.
      //
      LaneRegisterIndex = LaneIndex / 2;
      LaneIndexInsideRegister = LaneIndex % 2;
      MmioOr32 (
        DmiBar + R_DMI_LTCO1 + (4 * LaneRegisterIndex),
        B_DMI_L0246TCOE << LaneIndexInsideRegister
        );
      //
      // We program the LTCXX register in 2 writes since according to PCIe BWG
      // the pre and post cursor values must be valid at the time of setting the
      // override enable bits.
      //
      PreCursorReg    = (LaneIndex * 2) / 4;
      PreCursorField  = (LaneIndex * 2) % 4;
      PostCursorReg   = ((LaneIndex * 2) + 1) / 4;
      PostCursorField = ((LaneIndex * 2) + 1) % 4;

      DEBUG ((DEBUG_INFO, "Gen3 Offset = %x LaneIndex = %d PreCursor = %d\n",(DmiBar + R_DMI_LTCO1), LaneIndex, PreCursor));
      Data32And = (UINT32) ~(0x3F << (6 * PreCursorField));
      Data32Or  = (PreCursor << (6 * PreCursorField));
      ASSERT ((Data32And & Data32Or) == 0);
      MmioAndThenOr32 (DmiBar + R_DMI_LTCO1 + (PreCursorReg * 4), Data32And, Data32Or);

      DEBUG ((DEBUG_INFO, "Gen3 Offset = %x LaneIndex = %d PostCursor = %d\n",(DmiBar + R_DMI_LTCO1), LaneIndex, PostCursor));
      Data32And = (UINT32) ~(0x3F << (6 * PostCursorField));
      Data32Or  = (PostCursor << (6 * PostCursorField));
      ASSERT ((Data32And & Data32Or) == 0);
      MmioAndThenOr32 (DmiBar + R_DMI_LTCO1 + (PostCursorReg * 4), Data32And, Data32Or);
      DEBUG ((DEBUG_INFO, "LTCO1: %x\n", MmioRead32(DmiBar + R_DMI_LTCO1)));
    }
    //
    // for ltco34
    //
    for (LaneIndex = 0; LaneIndex < 4 ; LaneIndex ++) {
      PreCursor = PeiDpDmiConfig->DmiGen3Ltcpre[LaneIndex];
      PostCursor = PeiDpDmiConfig->DmiGen3Ltcpo[LaneIndex];
      ASSERT ((PreCursor & ~0x3F) == 0);
      ASSERT ((PostCursor & ~0x3F) == 0);
      //
      // Each of the local transmitter override registers
      // holds configuration fields for 2 lanes.
      // The layout of the register is as follows
      // lower lane pre-cursor value
      // lower lane post-cursor value
      // upper lane pre-cursor value
      // upper lane post-cursor value
      // lower lane local transmitter override enable
      // upper lane local transmitter override enable
      // Cursor values fields are 6 bits wide.
      //
      LaneRegisterIndex = LaneIndex / 2;
      LaneIndexInsideRegister = LaneIndex % 2;
      MmioOr32 (
        DmiBar + R_DMI_LTCO3 + (4 * LaneRegisterIndex),
        B_DMI_L0246TCOE << LaneIndexInsideRegister
        );
      //
      // We program the LTCXX register in 2 writes since according to PCIe BWG
      // the pre and post cursor values must be valid at the time of setting the
      // override enable bits.
      //
      PreCursorReg    = (LaneIndex * 2) / 4;
      PreCursorField  = (LaneIndex * 2) % 4;
      PostCursorReg   = ((LaneIndex * 2) + 1) / 4;
      PostCursorField = ((LaneIndex * 2) + 1) % 4;

      DEBUG ((DEBUG_INFO, "Gen3 Offset = %x LaneIndex = %d PreCursor = %d\n",(DmiBar + R_DMI_LTCO3), LaneIndex, PreCursor));
      Data32And = (UINT32) ~(0x3F << (6 * PreCursorField));
      Data32Or  = (PreCursor << (6 * PreCursorField));
      ASSERT ((Data32And & Data32Or) == 0);
      MmioAndThenOr32 (DmiBar + R_DMI_LTCO3 + (PreCursorReg * 4), Data32And, Data32Or);

      DEBUG ((DEBUG_INFO, "Gen3 Offset = %x LaneIndex = %d PostCursor = %d\n",(DmiBar + R_DMI_LTCO3), LaneIndex, PostCursor));
      Data32And = (UINT32) ~(0x3F << (6 * PostCursorField));
      Data32Or  = (PostCursor << (6 * PostCursorField));
      ASSERT ((Data32And & Data32Or) == 0);
      MmioAndThenOr32 (DmiBar + R_DMI_LTCO3 + (PostCursorReg * 4), Data32And, Data32Or);
      DEBUG ((DEBUG_INFO, "LTCO3: %x\n", MmioRead32(DmiBar + R_DMI_LTCO3)));
    }
  }
  //
  //Step4:Program the Presets
  //
  for (LaneIndex = 0; LaneIndex < GetMaxDmiLanes(); LaneIndex ++) {
    DmiSetGen3Presets (
      DmiBar,
      LaneIndex,
      PeiDpDmiConfig->DmiHwEqGen3CoeffList[LaneIndex].Cm,
      PeiDpDmiConfig->DmiHwEqGen3CoeffList[LaneIndex].Cp
      );
  }
  //
  // Step5:Program EQCFG1.LEB to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG1, (UINT32)~(B_DMI_EQCFG1_LEB));
  //
  // Step6:Program EQCFG1.LEP23B to 0 so that Phase 2 and Phase 3 EQ is not bypass
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG1,  (UINT32)~(B_DMI_EQCFG1_LEP23B));

  if (PeiDpDmiConfig->Gen3EqPhase23Bypass == 0) {
    MmioAnd32 (DmiBar + R_DMI_EQCFG1,  (UINT32)~(B_DMI_EQCFG1_LEP23B));
  } else {
    MmioOr32 (DmiBar + R_DMI_EQCFG1,  (UINT32)(B_DMI_EQCFG1_LEP23B));
  }

  //
  // Step7:Program EQCFG1.LEP3B to 0 so that Phase 3 EQ is not bypass
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG1,  (UINT32)~(B_DMI_EQCFG1_LEP3B));

  if (PeiDpDmiConfig->Gen3EqPhase3Bypass == 0) {
    MmioAnd32 (DmiBar + R_DMI_EQCFG1,  (UINT32)~(B_DMI_EQCFG1_LEP3B));
  } else {
    MmioOr32 (DmiBar + R_DMI_EQCFG1,  (UINT32)(B_DMI_EQCFG1_LEP3B));
  }
  if (DmiEqMode == DmiLinearEq) {
    //
    // Step8:Program EQCFG1 RTLEPCEB to 0 enable Hardware Search Algorithm
    //
    MmioAnd32 (DmiBar + R_DMI_EQCFG1, (UINT32)~B_DMI_EQCFG1_RTLEPCEB);
    //
    // Step9:Program EQCFG1.RTPCOE to 0 enable Hardware Search Algorithm
    //
    MmioAnd32 (DmiBar + R_DMI_EQCFG1, (UINT32)~B_DMI_EQCFG1_RTPCOE);
  } else {
    //
    // Step8:Program EQCFG1 RTLEPCEB to 1
    //
    MmioOr32 (DmiBar + R_DMI_EQCFG1, (UINT32)B_DMI_EQCFG1_RTLEPCEB);
    //
    // Step9:Program EQCFG1.RTPCOE to 1
    //
    MmioOr32 (DmiBar + R_DMI_EQCFG1, (UINT32)B_DMI_EQCFG1_RTPCOE);
  }
  //
  // Step10:Clear EQCFG1.HPCMQE to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG1, (UINT32)~B_DMI_EQCFG1_HPCMQE);
  //
  // Step11:Rx wait time for each new eq should be configured to 1us
  //
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG1, (UINT32)~B_DMI_EQCFG1_RWTNEVE_MASK, (UINT32)V_DMI_EQCFG1_RWTNEVE_1US << B_DMI_EQCFG1_RWTNEVE_OFFSET);
  //
  // Step12: For Linear Mode, BIOS needs to program the EQCFG1.MFLNTL to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG1, (UINT32)~B_DMI_EQCFG1_MFLNTL);
  //
  // Step13: Program EQCFG4.PX8GTSWLPCE to 3 as per post silicon charecterization - Gen3 TS Lock Timer
  //
  Gen3TsLockTimer = 3;
  DEBUG ((DEBUG_INFO, "Gen3 TS Lock Timer = %d\n", Gen3TsLockTimer));
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_PX8GTSWLPCE_MASK , (Gen3TsLockTimer << B_DMI_EQCFG4_PX8GTSWLPCE_OFFSET));
  //
  // Step14: Program EQCFG1.TUPP to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG1, (UINT32)~B_DMI_EQCFG1_TUPP);
  //
  // Step15: Program EQCFG1.RUPP to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG1, (UINT32)~B_DMI_EQCFG1_RUPP);
  //
  // Step16: BIOS would program the Receiver Eye Margin Error Threshold and Multiplier to 4 errors; in
  // which the EQCFG2.REWMETM would be set to 00b and EQCFG2.REWMET would be set
  //  to 10h
  //
  Data32And = (UINT32)~(B_DMI_EQCFG2_REWMETM |
                         B_DMI_EQCFG2_REWMET);
  Data32Or = (0 << B_DMI_EQCFG2_REWMETM_OFFSET) | 0x10; // REWMET = 2 (4 errors)
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG2, Data32And, Data32Or);
  //
  // Step17,18: Program EQCFG2.NTIC,EQCFG2.EMD  to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG2, (UINT32)~(B_DMI_EQCFG2_NTIC | B_DMI_EQCFG2_EMD));
  //
  // Step19: Program EQCFG2.PECT as below - Gen3 PCET Timer
  //
  Gen3PcetTimer = 4;
  DEBUG ((DEBUG_INFO, "Gen3 PCET timeout = %d ms\n",Gen3PcetTimer));
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG2, (UINT32)~(B_DMI_EQCFG2_PCET), (Gen3PcetTimer << B_DMI_EQCFG2_PCET_OFFSET));
  //
  // Step20: Program EQCFG2.HAPCSB as below and clear HAPCCPIE - Gen3 Number of Preset/Coefficient List
  //
  Gen3NumberOfPresetCoeffList = 4;
  DEBUG ((DEBUG_INFO, "Number Of Preset/Coefficient List = %d\n", Gen3NumberOfPresetCoeffList));
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG2, (UINT32)~B_DMI_EQCFG2_HAPCSB, Gen3NumberOfPresetCoeffList << B_DMI_EQCFG2_HAPCSB_OFFSET);
  MmioAnd32 (DmiBar + R_DMI_EQCFG1, (UINT32)~(B_DMI_EQCFG1_HAPCCPIE));
  //
  // Step21,22: Program EQCFG2.NTEME and EQCFG2.MPEME to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG2, (UINT32)~(B_DMI_EQCFG2_NTEME | B_DMI_EQCFG2_MPEME));
  //
  // Step23,24: Program EQCFG1.MEQSMMFLNTL and EQCFG1.MFLNTL to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG1, (UINT32)~(B_DMI_EQCFG1_MEQSMMFLNTL | B_DMI_EQCFG1_MFLNTL));
  //
  // Step25: Program EQCFG1.REIC - value TBD in BWG - @TODO follow up with Arch to get the value and program it here
  //

  //
  // Step26: Program the HAEQ.HAPCCPI 0 to allow 1 iteration of Recovery.Equalization entry
  //
  MmioAndThenOr32 (DmiBar + R_DMI_HAEQ, (UINT32)~B_DMI_HAEQ_HAPCCPI, 0 << B_DMI_HAEQ_HAPCCPI_OFFSET);
  //
  // Step27-31: Program HAEQ.FOMEM to 0 | HAEQ.MACFOMC to 0 | HAEQ.SL to 0 | HAEQ.DL to 0xE | HAEQ.SFOMFM to 0
  //
  Data32And = (UINT32)~(B_DMI_HAEQ_FOMEM |
                        B_DMI_HAEQ_MACFOMC |
                        B_DMI_HAEQ_SL |
                        B_DMI_HAEQ_DL |
                        B_DMI_HAEQ_SFOMFM);

  Data32Or = (0xE << B_DMI_HAEQ_DL_OFFSET);
  MmioAndThenOr32 (DmiBar + R_DMI_HAEQ, Data32And, Data32Or);
  //
  // Step32: Program the following register for the Downstream Port Transmitter Preset value to P7
  //
  DEBUG ((DEBUG_INFO, "Preset Programming\n"));
  for (LaneIndex = 0; LaneIndex < GetMaxDmiLanes(); LaneIndex +=2) {
  Data32And = (UINT32) ~(B_DMI_L01EC_DPL0246TP_MASK | B_DMI_L01EC_DPL0246RPH_MASK | B_DMI_L01EC_DPL1357TP_MASK | B_DMI_L01EC_DPL1357RPH_MASK);
  Data32Or = ((PeiDpDmiConfig->DmiGen3DsPortTxPreset[LaneIndex] << B_DMI_L01EC_DPL0246TP_OFFSET) |
              (PeiDpDmiConfig->DmiGen3DsPortRxPreset[LaneIndex] << B_DMI_L01EC_DPL0246RPH_OFFSET) |
              (PeiDpDmiConfig->DmiGen3DsPortTxPreset[LaneIndex + 1] << B_DMI_L01EC_DPL1357TP_OFFSET) |
              (PeiDpDmiConfig->DmiGen3DsPortRxPreset[LaneIndex + 1] << B_DMI_L01EC_DPL1357RPH_OFFSET));
  MmioAndThenOr32 (DmiBar + DmiGen3DsPortPreset[LaneIndex/2], Data32And, Data32Or);

  DEBUG ((DEBUG_INFO, "LaneIndex = %x offset = %x value = %x\n",LaneIndex, (DmiBar + DmiGen3DsPortPreset[LaneIndex/2]), MmioRead32 (DmiBar + DmiGen3DsPortPreset[LaneIndex/2])));
  }
  for (LaneIndex = 0; LaneIndex < GetMaxDmiLanes(); LaneIndex +=2) {
    Data32And = (UINT32) ~(B_DMI_L01EC_UPL0246TP_MASK | B_DMI_L01EC_UPL0246RPH_MASK | B_DMI_L01EC_UPL1357TP_MASK | B_DMI_L01EC_UPL1357RPH_MASK);
    Data32Or = ((PeiDpDmiConfig->DmiGen3UsPortTxPreset[LaneIndex] << B_DMI_L01EC_UPL0246TP_OFFSET) |
                (PeiDpDmiConfig->DmiGen3UsPortRxPreset[LaneIndex] << B_DMI_L01EC_UPL0246RPH_OFFSET) |
                (PeiDpDmiConfig->DmiGen3UsPortTxPreset[LaneIndex + 1] << B_DMI_L01EC_UPL1357TP_OFFSET) |
                (PeiDpDmiConfig->DmiGen3UsPortRxPreset[LaneIndex + 1] << B_DMI_L01EC_UPL1357RPH_OFFSET));
    MmioAndThenOr32 (DmiBar + DmiGen3DsPortPreset[LaneIndex/2], Data32And, Data32Or);

  DEBUG ((DEBUG_INFO, "LaneIndex = %x offset = %x value = %x\n",LaneIndex, (DmiBar + DmiGen3DsPortPreset[LaneIndex/2]), MmioRead32 (DmiBar + DmiGen3DsPortPreset[LaneIndex/2])));
  }

  DEBUG ((DEBUG_INFO, "DpDmiConfigureGen3HwEq End!\n"));
}

/**
  Configure DMI Hweq
  @param[in] DmiBar                     Dmi Base Address
  @param[in] PEI_DMI_CONFIG             Dmi config block
**/
VOID
DpDmiConfigureGen4HwEq (
  IN  UINT32                       DmiBar,
  IN  PEI_DMI_CONFIG               *PeiDpDmiConfig,
  IN  UINT8                        DmiEqMode
  )
{
  UINT16           Data16And;
  UINT16           Data16Or;
  UINT32           LaneIndex;
  UINT32           LaneRegisterIndex;
  UINT32           LaneIndexInsideRegister;
  UINT8            Gen4NumberOfPresetCoeffList;
  UINT8            DmiGen4PresetCoeffSelection;
  UINT8            Gen4PcetTimer;
  UINT8            Gen4TsLockTimer;

  DEBUG ((DEBUG_INFO, "DpDmiConfigureGen4HwEq Start!\n"));

  LaneIndex                      = 0;
  LaneRegisterIndex              = 0;
  LaneIndexInsideRegister        = 0;
  DmiGen4PresetCoeffSelection    = 0;
  Gen4PcetTimer                  = 0;
  Gen4TsLockTimer                = 0;
  //
  //
  // Step1:Program all the preset-coeff mapping registers
  //
  DmiGen4PresetToCoeff (DmiBar);

  //
  // Step3:Program RTPCL1.PCM = 0 to enable Preset mode - Gen4 Preset/Coefficient Mode Selection
  //
  DmiGen4PresetCoeffSelection = PeiDpDmiConfig->Gen4RtcoRtpoEnable;
  DEBUG ((DEBUG_INFO, "Gen4 Preset/Coefficient Mode Selection %x\n", DmiGen4PresetCoeffSelection));
  MmioAndThenOr32 (DmiBar + R_DMI_PX16GRTPCL1, (UINT32)~(B_DMI_PX16GRTPCL1_PCM), (DmiGen4PresetCoeffSelection << B_DMI_PX16GRTPCL1_PCM_OFFSET));
  // LTCO enable
  if (PeiDpDmiConfig->Gen4LtcoEnable == 1) {
    for (LaneIndex = 0; LaneIndex < GetMaxDmiLanes(); LaneIndex ++) {
      ASSERT ((PeiDpDmiConfig->DmiGen4Ltcpre[LaneIndex] & ~0x3F) == 0);
      ASSERT ((PeiDpDmiConfig->DmiGen4Ltcpo[LaneIndex] & ~0x3F) == 0);
      //
      // Each of the local transmitter override registers
      // holds configuration fields for 2 lanes.
      // The layout of the register is as follows
      // lower lane pre-cursor value
      // lower lane post-cursor value
      // upper lane pre-cursor value
      // upper lane post-cursor value
      // lower lane local transmitter override enable
      // upper lane local transmitter override enable
      // Cursor values fields are 6 bits wide.
      //
      LaneRegisterIndex = LaneIndex / 2;
      LaneIndexInsideRegister = LaneIndex % 2;
      //
      // We program the LTCXX register in 2 writes since according to PCIe BWG
      // the pre and post cursor values must be valid at the time of setting the
      // override enable bits.
      //
      MmioAndThenOr32 (
        DmiBar + R_DMI_PX16GLTCO1 + (4 * LaneRegisterIndex),
        (UINT32) ~((0x3F << (6 * LaneIndexInsideRegister)) | (0x3F << (12 * LaneIndexInsideRegister))),
        (PeiDpDmiConfig->DmiGen4Ltcpre[LaneIndex] << (6 * LaneIndexInsideRegister) | (PeiDpDmiConfig->DmiGen4Ltcpo[LaneIndex] << (12 * LaneIndexInsideRegister)))
        );

      MmioOr32 (
        DmiBar + R_DMI_PX16GLTCO1 + (4 * LaneRegisterIndex),
        B_DMI_PX16GLTCO1_L0246TCOE << LaneIndexInsideRegister
        );
      }
    }

  //
  // Step4:Program the Presets
  //
  for (LaneIndex = 0; LaneIndex < GetMaxDmiLanes(); LaneIndex ++) {
    DmiSetGen4Presets (
      DmiBar,
      LaneIndex,
      PeiDpDmiConfig->DmiHwEqGen4CoeffList[LaneIndex].Cm,
      PeiDpDmiConfig->DmiHwEqGen4CoeffList[LaneIndex].Cp
      );
  }
  //
  // Step5:Program EQCFG4.PX16GLEP23B to 0 so that Phase 2 and Phase 3 EQ is not bypass
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG4,  (UINT32)~(B_DMI_EQCFG4_PX16GLEP23B));

  if (PeiDpDmiConfig->Gen4EqPhase23Bypass == 0) {
    MmioAnd32 (DmiBar + R_DMI_EQCFG4,  (UINT32)~(B_DMI_EQCFG4_PX16GLEP23B));
  } else {
    MmioOr32 (DmiBar + R_DMI_EQCFG4,  (UINT32)(B_DMI_EQCFG4_PX16GLEP23B));
  }

  //
  // Step6:Program EQCFG4.PX16GLEP3B to 0 so that Phase 3 EQ is not bypass
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG4,  (UINT32)~(B_DMI_EQCFG4_PX16GLEP3B));

  if (PeiDpDmiConfig->Gen4EqPhase3Bypass == 0) {
    MmioAnd32 (DmiBar + R_DMI_EQCFG4,  (UINT32)~(B_DMI_EQCFG4_PX16GLEP3B));
  } else {
    MmioOr32 (DmiBar + R_DMI_EQCFG4,  (UINT32)(B_DMI_EQCFG4_PX16GLEP3B));
  }
  if (DmiEqMode == DmiLinearEq) {
    //
    // Step8:Program EQCFG1 RTLEPCEB to 0 enable Hardware Search Algorithm
    //
    MmioAnd32 (DmiBar + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_PX16GLEPCEB);
    //
    // Step9:Program EQCFG1.RTPCOE to 0 enable Hardware Search Algorithm
    //
    MmioAnd32 (DmiBar + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_PX16GRTPCOE);
  } else {
    //
    // Step7:Program EQCFG4.PX16GRTLEPCEB to 1
    //
    MmioOr32 (DmiBar + R_DMI_EQCFG4, (UINT32)B_DMI_EQCFG4_PX16GLEPCEB);
    //
    // Step8:Program EQCFG4.PX16GRTPCOE to 1
    //
    MmioOr32 (DmiBar + R_DMI_EQCFG4, (UINT32)B_DMI_EQCFG4_PX16GRTPCOE);
  }
  //
  // Step9,10:Rx wait time for each new eq should be configured to 1us
  //
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_PX16GRWTNEVE_MASK, (UINT32)V_DMI_EQCFG4_PX16GRWTNEVE_1US << B_DMI_EQCFG4_PX16GRWTNEVE_OFFSET);
  //
  // Step11: For Linear Mode, BIOS needs to program the EQCFG4.PX16GMFLNTL to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_PX16GMFLNTL);
  //
  // Step12: Program EQCFG4.PX16GTSWLPCE to 0 // Program it to 4 (5us) as per post silicon recommendation - Gen4 TS Lock Timer
  //
  Gen4TsLockTimer = 3;
  DEBUG((DEBUG_INFO, "Gen4 TS Lock Timer = %d \n", Gen4TsLockTimer));
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_PX16GTSWLPCE_MASK, (Gen4TsLockTimer << B_DMI_EQCFG4_PX16GTSWLPCE_OFFSET));
  // Step13:Rx wait time for each new eq should be configured to 3us
  //
  //
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_PX16GRWTNEVE_MASK, ((UINT32)V_DMI_EQCFG4_PX16GRWTNEVE_3US << B_DMI_EQCFG4_PX16GRWTNEVE_OFFSET));
  //
  // Step14: Program the HAEQ.HAPCCPI 0 to allow 1 iteration of Recovery.Equalization entry
  // @ TODO : Follow up with IP team on the value
  //
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_PX16GHAPCCPI_MASK, 2 << B_DMI_EQCFG4_PX16GHAPCCPI_OFFSET);
  //
  // Step15: Program EQCFG5.HAPCSB as below and clear HAPCCPIE
  //
  Gen4NumberOfPresetCoeffList = 3;
  DEBUG((DEBUG_INFO, "Gen4 Number of Preset/Coefficient = %d\n", Gen4NumberOfPresetCoeffList));
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG5, (UINT32)~B_DMI_EQCFG5_HAPCSB_MASK, Gen4NumberOfPresetCoeffList << B_DMI_EQCFG5_HAPCSB_OFFSET);
  MmioAnd32 (DmiBar + R_DMI_EQCFG4, (UINT32)~(B_DMI_EQCFG4_PX16GHAPCCPIE));
  //
  // Step16,17: Program EQCFG4.PX16GMEQSMMFLNTL and EQCFG4.PX16GMFLNTL to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG4, (UINT32)~(B_DMI_EQCFG4_PX16GMEQSMMFLNTL | B_DMI_EQCFG4_PX16GMFLNTL));
  //
  // Step18: Program EQCFG4.PX16GREIC to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG4, (UINT32)~(B_DMI_EQCFG4_PX16GREIC));
  //
  // Step19,20: Program EQCFG5.NTIC,EQCFG5.EMD to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG5, (UINT32)~(B_DMI_EQCFG5_NTIC_MASK | B_DMI_EQCFG5_EMD));
  //
  // Step21: Program EQCFG2.PECT as below - Gen4 PCET Timer
  //
  Gen4PcetTimer = 5;
  DEBUG ((DEBUG_INFO, "Gen4 PCET timeout = %d ms\n",Gen4PcetTimer));
  MmioAndThenOr32 (DmiBar + R_DMI_EQCFG5, (UINT32)~(B_DMI_EQCFG5_PCET_MASK), (Gen4PcetTimer << B_DMI_EQCFG5_PCET_OFFSET));
  //
  // Step22,23: Program EQCFG5.NTEME and EQCFG5.MPEME to 0
  //
  MmioAnd32 (DmiBar + R_DMI_EQCFG5, (UINT32)~(B_DMI_EQCFG5_NTEME | B_DMI_EQCFG5_MPEME));
  //
  // Step24: Program the following register for the Downstream Port Transmitter Preset value to P7
  //
  DEBUG ((DEBUG_INFO, "Preset Programming\n"));
  for (LaneIndex = 0; LaneIndex < GetMaxDmiLanes(); LaneIndex +=2) {
  Data16And = (UINT16) ~(B_DMI_PL16L01EC_DP16L0246TP_MASK | B_DMI_PL16L01EC_DP16L1357TP_MASK);
  Data16Or = ((PeiDpDmiConfig->DmiGen4DsPortTxPreset[LaneIndex] << B_DMI_PL16L01EC_DP16L0246TP_OFFSET) |
              (PeiDpDmiConfig->DmiGen4DsPortTxPreset[LaneIndex + 1] << B_DMI_PL16L01EC_DP16L1357TP_OFFSET));
  MmioAndThenOr16 (DmiBar + DmiGen4DsPortPreset[LaneIndex/2], Data16And, Data16Or);
  DEBUG ((DEBUG_INFO, "LaneIndex = %x offset = %x value = %x\n",LaneIndex, (DmiBar + DmiGen4DsPortPreset[LaneIndex/2]), MmioRead16 (DmiBar + DmiGen4DsPortPreset[LaneIndex/2])));
  }
  for (LaneIndex = 0; LaneIndex < GetMaxDmiLanes(); LaneIndex +=2) {
    Data16And = (UINT16) ~(B_DMI_PL16L01EC_UP16L0246TP_MASK | B_DMI_PL16L01EC_UP16L1357TP_MASK);
    Data16Or = ((PeiDpDmiConfig->DmiGen4UsPortTxPreset[LaneIndex] << B_DMI_PL16L01EC_UP16L0246TP_OFFSET) |
                (PeiDpDmiConfig->DmiGen4UsPortTxPreset[LaneIndex + 1] << B_DMI_PL16L01EC_UP16L1357TP_OFFSET));
    MmioAndThenOr16 (DmiBar + DmiGen4DsPortPreset[LaneIndex/2], Data16And, Data16Or);
    DEBUG ((DEBUG_INFO, "LaneIndex = %x offset = %x value = %x\n",LaneIndex, (DmiBar + DmiGen4DsPortPreset[LaneIndex/2]), MmioRead16 (DmiBar + DmiGen4DsPortPreset[LaneIndex/2])));
  }
  DEBUG ((DEBUG_INFO, "DpDmiConfigureGen4HwEq End!\n"));
}

/**
  Reads out internal signal state from the SIP IP hardware.
  This is accomplished with monitor mux register in RP.

  @param[in] DmiBar           Dmi base address
  @param[in] MonitorCategory  Monitor Category
  @param[in] MonitorSegment   Monitor Segment (Lane number)
  @param[in] MonitorCategory  Monitor Signal (To Indentify For Which Gen Speed Signal State Is Required)

  @return Value of the signal state.
**/
UINT32
DmiSipGetInternalSignalState (
  UINT32                   DmiBar,
  IN UINT32                MonitorCategory,
  IN UINT32                MonitorSegment,
  IN UINT32                MonitorSignal
  )
{
  UINT32                   SignalState;

  if (MonitorCategory > 0xF ||
      MonitorSegment >= 0xF ||
      MonitorSignal > 0xFF) {
    DEBUG ((DEBUG_ERROR, "Failed to read monitor signal, index out of range C:%X S:%X S:%X\n",
                           MonitorCategory, MonitorSegment, MonitorSignal));
    return 0xFFFFFFFF;
  }

  MmioWrite32 (DmiBar + R_DMI_CFG_MM, MonitorSignal);
  MmioWrite32 (DmiBar + R_DMI_CFG_CDM, (MonitorCategory << N_DMI_CFG_CDM_MCS | MonitorSegment << N_DMI_CFG_CDM_MSS));
  SignalState = MmioRead32 (DmiBar + R_DMI_CFG_MM) >> N_DMI_CFG_MM_MSST;
  SignalState = (SignalState >> 12) & 0xF;

  return SignalState;
}

/**
  Dumps best EQ coefficients values that were selected during EQ.

  @param[in] DmiBar      Dmi base address
  @param[in] DpDmiSpeed  Current Dmi Link Speed
**/

VOID
DpDmiDumpBestEqCoefficients (
  UINT32                   DmiBar,
  IN UINT8                 DpDmiSpeed
  )
{
  UINT32                   SignalState;
  DMI_HOB                  *DmiHob;
  UINT16                   Index;

  DmiHob = NULL;
  DmiHob = (DMI_HOB *) GetFirstGuidHob (&gDmiHobGuid);

  if (DmiHob == NULL) {
    DEBUG ((DEBUG_INFO, "DmiHob is NULL !\n"));
    ASSERT (FALSE);
    return;
  }
  for (Index = 0; Index < 8; Index++) {

    DEBUG ((DEBUG_INFO, "Lane %d: ", Index));

    if (DpDmiSpeed == 3) {
    SignalState = DmiSipGetInternalSignalState (
                    DmiBar,
                    R_DMI_SIP_MONITOR_CAT2,
                    Index,
                    R_DMI_SIP_MONITOR_CAT2_BEST4COEFF_GEN3);
      DEBUG ((DEBUG_INFO, "GEN3: %x \n", SignalState));
      if (Index % 2 == 0) {
        DmiHob->DpDmiInfoHob.CmGen3[Index/2] = (UINT8) SignalState;
      } else {
        DmiHob->DpDmiInfoHob.CpGen3[Index/2] = (UINT8) SignalState;
      }
    }

    if (DpDmiSpeed == 4) {
      SignalState = DmiSipGetInternalSignalState (
                      DmiBar,
                      R_DMI_SIP_MONITOR_CAT2,
                      Index,
                      R_DMI_SIP_MONITOR_CAT2_BEST4COEFF_GEN4);
      DEBUG ((DEBUG_INFO, "GEN4: %x\n", SignalState));
      if (Index % 2 == 0) {
        DmiHob->DpDmiInfoHob.CmGen4[Index/2] = (UINT8) SignalState;
      } else {
        DmiHob->DpDmiInfoHob.CpGen4[Index/2] = (UINT8) SignalState;
      }
    }
  }
}

/**
  This function configures ASPM on DMI

  @param[in] DmiBar                 DmiBar
  @param[in] PeiDpDmiConfig         Instance of PEI_DMI_CONFIG
**/
STATIC
VOID
DpDmiConfigureAspm (
  IN  UINT32                       DmiBar,
  IN  PEI_DMI_CONFIG               *PeiDpDmiConfig
  )
{
  UINT16                        Data16And;
  UINT16                        Data16Or;
  UINT32                        Data32And;
  UINT32                        Data32Or;
  DMI_ASPM                      DmiAspmCtrl;

  //
  // Enable DMI ASPM
  //
  DmiAspmCtrl = PeiDpDmiConfig->DmiAspmCtrl;

  Data32And = (UINT32)~(B_DMI_G4L0SCTL_G4ASL0SPL_MASK | B_DMI_G4L0SCTL_G4L0SICL_MASK);
  Data32Or  = (UINT32)(V_DMI_G4L0SCTL_G4ASL0SPL << B_DMI_G4L0SCTL_G4ASL0SPL_OFFSET | V_DMI_G4L0SCTL_G4L0SIC << B_DMI_G4L0SCTL_G4L0SICL_OFFSET);
  //
  // Program Gen4 Active State L0s Preparation Latency
  //
  MmioAndThenOr32 (
    DmiBar + R_DMI_G4L0SCTL,
    Data32And,
    Data32Or
    );

  Data32And = (UINT32)~B_DMI_G3L0SCTL_G3ASL0SPL_MASK;
  Data32Or  = (UINT32)(0x28 << N_DMI_G3L0SCTL_G3ASL0SPL);
  //
  // Program Gen3 Active State L0s Preparation Latency
  //
  MmioAndThenOr32 (
    DmiBar + R_DMI_G3L0SCTL,
    Data32And,
    Data32Or
    );

  Data32And = (UINT32)~B_DMI_PCIEL0SC_G2ASL0SPL_MASK;
  Data32Or  = (UINT32)(0x14 << N_DMI_PCIEL0SC_G2ASL0SPL);
  //
  // Program Gen2 Active State L0s Preparation Latency
  //
  MmioAndThenOr32 (
    DmiBar + R_DMI_PCIEL0SC,
    Data32And,
    Data32Or
    );

  Data32And = (UINT32)~B_DMI_PCIEL0SC_G1ASL0SPL_MASK;
  Data32Or  = (UINT32)(0x14 << N_DMI_PCIEL0SC_G1ASL0SPL);
  //
  // Program Gen1 Active State L0s Preparation Latency
  //
  MmioAndThenOr32 (
    DmiBar + R_DMI_PCIEL0SC,
    Data32And,
    Data32Or
    );

  Data16And = (UINT16)~B_DMI_LCTL_ASPM;

  if (DmiAspmCtrl == DmiAspmL0sL1) {
    //
    // Enable L0s/L1 on DMI
    //
    Data16Or = V_DMI_LCTL_ASPM_L0SL1;
  } else if (DmiAspmCtrl == DmiAspmL0s) {
    //
    // Enable L0s Entry only
    //
    Data16Or = V_DMI_LCTL_ASPM_L0S;

  } else if ((DmiAspmCtrl == DmiAspmAutoConfig) || (DmiAspmCtrl == DmiAspmL1)) {
    //
    // Enable L1 Entry only
    //
    Data16Or = V_DMI_LCTL_ASPM_L1;
  } else {
    //
    // ASPM Disabled
    //
    Data16Or  = V_DMI_LCTL_ASPM_DIS;
  }
  //
  //Configure Common Clock Configuration
  //
  Data16Or  |= B_DMI_LCTL_CCC;
  //
  // Configue DMI ASPM
  //
  MmioAndThenOr16 (
    DmiBar + R_DMI_LCTL,
    Data16And,
    Data16Or
    );

}

/**
  Configure DPDMI Speed Change

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiSpeedChange (
  IN  VOID    *PeiDmiConfig
  )
{
  PEI_DMI_CONFIG                *PeiDpDmiConfig;
  UINT32                        DpDmiBaseAddress;
  UINT32                        Data32And;
  UINT32                        Data32Or;
  UINT32                        Data32;
  UINT16                        Index;
  UINT8                         DmiEqMode;
  UINT8                         Tls;
  BOOLEAN                       PchPciePeerMemoryWriteStatus;

  Index            = 0;
  DmiEqMode        = DmiLinearEq;
  Tls              = 1;

  PeiDpDmiConfig   = PeiDmiConfig;
  DpDmiBaseAddress = PeiDpDmiConfig->DmiBaseAddress;
  DmiEqMode        = PeiDpDmiConfig->DmiHweq;

  DEBUG ((DEBUG_INFO, "DmiBar = %x \n", DpDmiBaseAddress));
  DEBUG ((DEBUG_INFO, "DmiEqMode = %x \n", DmiEqMode));

  //
  // Create DMI Hob
  //
  CreateDmiHob ();

  //
  // Configure Phy.
  // This programming has to be performed before equalization.
  //
  MmioAnd32 (
    DpDmiBaseAddress + SIZE_8KB + R_DMI_MEM_LPHYCP1,
    (UINT32) ~(B_DMI_MEM_LPHYCP1_RXEQFNEVC | B_DMI_MEM_LPHYCP1_RXADPHM)
  );
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_MEM_LPHYCP1 = %x \n",(MmioRead32 (DpDmiBaseAddress + SIZE_8KB + R_DMI_MEM_LPHYCP1))));

  if (PeiDpDmiConfig->DmiMaxLinkSpeed == V_DMI_LCAP_MLS_AUTO) {
    PeiDpDmiConfig->DmiMaxLinkSpeed = (UINT8) DmiGetMaxLinkSpeed (DpDmiBaseAddress);
  } else {
    MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_MPC,
    (UINT32) ~B_DMI_CFG_MPC_PCIESD,
    PeiDpDmiConfig->DmiMaxLinkSpeed << N_DMI_CFG_MPC_PCIESD
    );
  }

  if (PeiDpDmiConfig->DmiMaxLinkSpeed == V_DMI_LCAP_MLS_GEN2) {
    //
    // Program PCH TLS to 0x2
    //
    Tls = 0x2;
    //
    // Configure UP DMI Link Speed to Gen2
    //
    UpDmiConfigureTls (Tls, DmiEqMode);

    MmioAndThenOr16 (DpDmiBaseAddress + R_DMI_LCTL2, (UINT16)~B_DMI_LCTL2_TLS, (UINT16) Tls);
    DEBUG ((DEBUG_INFO,"TLS = %x\n",(MmioRead16 (DpDmiBaseAddress + R_DMI_LCTL2))));

    MmioAndThenOr32 (
      DpDmiBaseAddress + R_DMI_MPC2,
      (UINT32)~(B_DMI_MPC2_LSTP),
      B_DMI_MPC2_LSTP
    );
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_MPC2 = %x \n",(MmioRead32 (DpDmiBaseAddress + R_DMI_MPC2))));

    MmioAndThenOr32 (
      DpDmiBaseAddress + R_DMI_ACGR3S2,
      (UINT32)~(0xF),
      (V_DMI_ACGR3S2_LSTPTLS << B_DMI_ACGR3S2_LSTPTLS_OFFSET)
    );
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_ACGR3S2 = %x \n",(MmioRead32 (DpDmiBaseAddress + R_DMI_ACGR3S2))));

    MmioAndThenOr32 (
      DpDmiBaseAddress + R_DMI_AECR1G3,
      (UINT32)~(B_DMI_AECR1G3_TPSE | B_DMI_AECR1G3_DCDCCTDT),
      (UINT32)(B_DMI_AECR1G3_DTCGCM | B_DMI_AECR1G3_REQTMBOF)
    );
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_AECR1G3 = %x \n",(MmioRead32 (DpDmiBaseAddress + R_DMI_AECR1G3))));

    DEBUG ((DEBUG_INFO, "Programming link retrain"));
    MmioAndThenOr16 (
      DpDmiBaseAddress + R_DMI_LCTL,
      (UINT16)~(B_DMI_LCTL_RL),
      B_DMI_LCTL_RL
    );
    while(((MmioRead16 (DpDmiBaseAddress+ R_DMI_LSTS)) & ((UINT16) B_DMI_LSTS_LT_OFFSET)) != 0);
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_LSTS = %x \n",(MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS))));

    //
    // Loop for 5ms to check for current link speed
    //
    for (Index = 0; Index < 50 ; Index++) {
      if (((MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS)) & 0x0F) == ((UINT16) PeiDpDmiConfig->DmiMaxLinkSpeed)) {
        break;
      }
      MicroSecondDelay (100);
    }
    DEBUG ((DEBUG_INFO, "Dmi CLS = %x \n",(MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS)) & B_DMI_LSTS_CLS_MASK));
  }

  //
  // Program EQ registers
  //
  if ((PeiDpDmiConfig->DmiHweq >= 1) && (PeiDpDmiConfig->DmiMaxLinkSpeed >= V_DMI_LCAP_MLS_GEN3)) {
    if (PeiDpDmiConfig->DmiHweq == 2) {
      DmiEqMode = DmiLinearEq;
    } else {
      DmiEqMode = DmiFixedEq;
    }
    Tls = 0x3;
    //
    // Configure UP DMI Link Speed to Gen3
    //
    UpDmiConfigureTls (Tls, DmiEqMode);

    MmioAndThenOr32  (DpDmiBaseAddress + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_FOMSCP_MASK , (0 << B_DMI_EQCFG4_FOMSCP_OFFSET));
    DEBUG ((DEBUG_INFO, "DmiEqMode : %x\n", DmiEqMode));
    DpDmiConfigureGen3HwEq (DpDmiBaseAddress, PeiDpDmiConfig, DmiEqMode);
    MmioAndThenOr16 (DpDmiBaseAddress + R_DMI_LCTL2, (UINT16)~B_DMI_LCTL2_TLS, (UINT16) Tls);
    DEBUG((DEBUG_INFO,"TLS = %x\n",(MmioRead16 (DpDmiBaseAddress + R_DMI_LCTL2)) & B_DMI_LCTL2_TLS));

    MmioAndThenOr32 (
      DpDmiBaseAddress + R_DMI_MPC2,
      (UINT32)~(B_DMI_MPC2_LSTP),
      B_DMI_MPC2_LSTP
    );
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_MPC2 = %x \n",(MmioRead32 (DpDmiBaseAddress + R_DMI_MPC2))));

    MmioAndThenOr32 (
      DpDmiBaseAddress + R_DMI_ACGR3S2,
      (UINT32)~(0xF),
      (V_DMI_ACGR3S2_LSTPTLS << B_DMI_ACGR3S2_LSTPTLS_OFFSET)
    );
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_ACGR3S2 = %x \n",(MmioRead32 (DpDmiBaseAddress + R_DMI_ACGR3S2))));

    MmioAndThenOr32 (
      DpDmiBaseAddress + R_DMI_AECR1G3,
      (UINT32)~(B_DMI_AECR1G3_TPSE),
      (UINT32)(B_DMI_AECR1G3_DTCGCM)
    );
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_AECR1G3 = %x \n",(MmioRead32 (DpDmiBaseAddress + R_DMI_AECR1G3))));
    //
    // Programming LCTL3_PE
    //
    MmioOr32 (
      DpDmiBaseAddress + R_DMI_LCTL3,
      (UINT32) B_DMI_LCTL3_PE
    );

    DEBUG ((DEBUG_INFO, "Programming link retrain"));
    MmioAndThenOr16 (
      DpDmiBaseAddress + R_DMI_LCTL,
      (UINT16)~(B_DMI_LCTL_RL),
      B_DMI_LCTL_RL
    );
    while(((MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS)) & ((UINT16) B_DMI_LSTS_LT_OFFSET)) != 0);
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_LSTS = %x \n",(MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS))));
    //
    // Loop for 5ms to check for current link speed
    //
    for (Index = 0; Index < 50 ; Index++) {
      if (((MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS)) & 0x0F) == ((UINT16) PeiDpDmiConfig->DmiMaxLinkSpeed)) {
        break;
      }
      MicroSecondDelay (100);
    }
    DEBUG ((DEBUG_INFO, "Dmi CLS = %x \n",(MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS)) & B_DMI_LSTS_CLS_MASK));
    DEBUG ((DEBUG_INFO, "Dmi DmiEqMode = %x \n",DmiEqMode));

    MmioAndThenOr32 (
      DpDmiBaseAddress + R_DMI_AECR1G3,
      (UINT32)~(B_DMI_AECR1G3_TPSE),
      (UINT32)(B_DMI_AECR1G3_DTCGCM)
    );
  if (DmiEqMode == DmiLinearEq) {
    MmioAndThenOr32  (DpDmiBaseAddress + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_FOMSCP_MASK , (0 << B_DMI_EQCFG4_FOMSCP_OFFSET));
  }

    if(PeiDpDmiConfig->DmiMaxLinkSpeed == V_DMI_LCAP_MLS_GEN4) {
      Tls = 0x4;
      //
      // Configure UP DMI Link Speed to Gen4
      //
    UpDmiConfigureTls (Tls, DmiEqMode);

      MmioAndThenOr32  (DpDmiBaseAddress + R_DMI_EQCFG4, (UINT32)~B_DMI_EQCFG4_FOMSCP_MASK , (1 << B_DMI_EQCFG4_FOMSCP_OFFSET));
      DpDmiConfigureGen4HwEq (DpDmiBaseAddress, PeiDpDmiConfig, DmiEqMode);
      MmioAndThenOr16 (DpDmiBaseAddress + R_DMI_LCTL2, (UINT16)~B_DMI_LCTL2_TLS, (UINT16) Tls);
      DEBUG((DEBUG_INFO,"TLS = %x\n",(MmioRead16 (DpDmiBaseAddress + R_DMI_LCTL2))));
    }
  }

  DEBUG ((DEBUG_INFO, "DmiGetMaxLinkSpeed = %x \n",DmiGetMaxLinkSpeed (DpDmiBaseAddress)));
  if (DmiGetMaxLinkSpeed (DpDmiBaseAddress) >= V_DMI_LCAP_MLS_GEN2) {
    if(DmiGetMaxLinkSpeed (DpDmiBaseAddress) == V_DMI_LCAP_MLS_GEN4) {
      //
      // Programming LCTL3_PE
      //
      MmioOr32 (
        DpDmiBaseAddress + R_DMI_LCTL3,
        (UINT32) B_DMI_LCTL3_PE
      );

      DEBUG ((DEBUG_INFO, "Programming link retrain"));
      MmioAndThenOr16 (
      DpDmiBaseAddress + R_DMI_LCTL,
      (UINT16)~(B_DMI_LCTL_RL),
      B_DMI_LCTL_RL
      );
      //
      // Polling till link training is complete
      //
      while(((MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS)) & ((UINT16) B_DMI_LSTS_LT_OFFSET)) != 0);
      DEBUG ((DEBUG_INFO, "Dmi R_DMI_LSTS = %x \n",(MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS))));
    }
    //
    // Loop for 10ms to check for current link speed
    //
    for (Index = 0; Index < 100 ; Index++) {
      if (((MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS)) & 0x0F) == ((UINT16) PeiDpDmiConfig->DmiMaxLinkSpeed)) {
        break;
      }
      MicroSecondDelay (100);
    }
    DEBUG ((DEBUG_INFO, "Dmi CLS = %x \n",(MmioRead16 (DpDmiBaseAddress + R_DMI_LSTS)) & B_DMI_LSTS_CLS_MASK));
  }
    DEBUG ((DEBUG_INFO, "R_DMI_PX16GRTPCL1 = %x \n",(MmioRead32 (DpDmiBaseAddress + R_DMI_PX16GRTPCL1))));
  if (DmiEqMode == DmiLinearEq) {
    if (PeiDpDmiConfig->DmiMaxLinkSpeed == V_DMI_LCAP_MLS_GEN3) {
      UpDmiDumpFoms (V_DMI_LCAP_MLS_GEN3);
      DpDmiDumpBestEqCoefficients (DpDmiBaseAddress, V_DMI_LCAP_MLS_GEN3);
    } else {
      UpDmiDumpFoms (V_DMI_LCAP_MLS_GEN3);
      DpDmiDumpBestEqCoefficients (DpDmiBaseAddress, V_DMI_LCAP_MLS_GEN3);
      UpDmiDumpFoms (Tls);
      DpDmiDumpBestEqCoefficients (DpDmiBaseAddress, Tls);
    }
  }

  //
  // Configure UP DMI ASPM
  //
  UpDmiConfigureAspm ();

  //
  // Configure DP DMI ASPM
  //
  DEBUG ((DEBUG_INFO, "PeiDpDmiConfig->DmiAspm = %x \n",PeiDpDmiConfig->DmiAspm));
  DEBUG ((DEBUG_INFO, "PeiDpDmiConfig->DmiAspmCtrl = %x \n",PeiDpDmiConfig->DmiAspmCtrl));
  DpDmiConfigureAspm (DpDmiBaseAddress, PeiDpDmiConfig);

  DEBUG ((DEBUG_INFO, "Dmi R_DMI_LCTL3 = %x \n",(MmioRead32 (DpDmiBaseAddress + R_DMI_LCTL3))));

  //
  // ReConfigure Max Payload Size
  //
  UpDmiGetPchPciePeerMemoryWriteStatus (&PchPciePeerMemoryWriteStatus);
  if (PchPciePeerMemoryWriteStatus) {
    DEBUG ((DEBUG_INFO, "%a Reconfigure MPS due to P2P \n", __FUNCTION__));
    DpDmiSetMpsCtrl (PeiDpDmiConfig, V_DMI_DCAP_MPS_128);
  }

  DEBUG ((DEBUG_INFO, "PeiDpDmiConfig->DmiL1ssEnable = %x \n",PeiDpDmiConfig->DmiL1ssEnable));

  if (PeiDpDmiConfig->DmiL1ssEnable == TRUE) {
    //
    // Configure UP DMI L1SS
    //
    UpDmiConfigureL1ss ();

    Data32And = (UINT32)~(B_DMI_L1SCTL1_L12LTRTLSV | B_DMI_L1SCTL1_L12LTRTLV_MASK | B_DMI_L1SCTL1_CMRT | B_DMI_L1SCTL1_AL11E |
                          B_DMI_L1SCTL1_PPL11E | B_DMI_L1SCTL1_AL12E | B_DMI_L1SCTL1_PPL12E | B_DMI_L1SCTL1_L1SSEIE | B_DMI_L1SCAP_L1SSES);
    Data32Or  = (V_DMI_L1SCTL1_L12LTRTLSV << B_DMI_L1SCTL1_L12LTRTLSV_OFFSET) | (V_DMI_L1SCTL1_L12LTRTLV << B_DMI_L1SCTL1_L12LTRTLV_OFFSET) |
                (0x3C << N_DMI_L1SCTL1_CMRT);

    Data32 = MmioRead32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_L1SCAP);
    if (Data32 & B_DMI_L1SCAP_AL12S){
      Data32Or |= (B_DMI_L1SCTL1_AL12E | B_DMI_L1SCTL1_PPL12E);
    }
    MmioAndThenOr32 (
      PeiDpDmiConfig->DmiBaseAddress + R_DMI_L1SCTL1,
      Data32And,
      Data32Or
    );
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_L1SCTL1 = %x \n", MmioRead32 (DpDmiBaseAddress + R_DMI_L1SCTL1)));

    MmioAndThenOr32 (
      PeiDpDmiConfig->DmiBaseAddress + R_DMI_L1SCTL2,
      (UINT32)~(B_DMI_L1SCTL2_TPOS | B_DMI_L1SCTL2_POWT),
      (V_DMI_L1SCTL2_TPOS | (V_DMI_L1SCTL2_POWT << N_DMI_L1SCTL2_POWT))
    );
    DEBUG ((DEBUG_INFO, "Dmi R_DMI_L1SCTL2 = %x \n", MmioRead32 (DpDmiBaseAddress + R_DMI_L1SCTL2)));

    MmioWrite32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_LTROVR2, 0x0);
    MmioAndThenOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_LTROVR, ~0u, 0x880F880F);
    MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_DCTL2, B_DMI_DCTL2_LTREN);
  }
  //
  // Lock SRL bits after function remapping
  //
    DEBUG((DEBUG_INFO, "Lock the registers\n"));
    DmiSetSecuredRegisterLock (DpDmiBaseAddress);

  return EFI_SUCCESS;
}

/**
  Enable Port8x decode for DP DMI.

  @param[in] PeiDpDmiConfig  DP DMI configuration
**/
VOID
DpDmiEnablePort8xDecode (
  IN PEI_DMI_CONFIG  *PeiDpDmiConfig
  )
{
  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));
  MmioOr32 (PeiDpDmiConfig->DmiBaseAddress + R_DMI_MPC, B_DMI_MPC_P8XDE);
}
