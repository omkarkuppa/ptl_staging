/** @file
  The TCSS PCIe Root Port Init Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <IpCsi.h>
#include <IpWrapper.h>
#include <IpWrapperCntxtInfoClient.h>
#include <IpTcPcieRp.h>
#include <Register/TcPcieRpRegs.h>
#include "IpTcPcieRpPrivate.h"

/**
  Sets the FPBCAPR

  @param[in] CfgCntxt     PCIe Root Port register access context
**/
void
IpTcPcieSetFpbcapr (
  IP_WR_REG_CNTXT   CfgCntxt
  )
{
  UINT32 Data32And;
  UINT32 Data32Or;
  UINT32 Data32;

  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_FPBCAPR_FPBRIDVECSIZES | B_TC_PCIE_RP_CFG_FPBCAPR_FPBMEMLVECSIZES | B_TC_PCIE_RP_CFG_FPBCAPR_FPBMEMHVECSIZES);
  Data32Or = (UINT32) (B_TC_PCIE_RP_CFG_FPBCAPR_FPBRIDS | B_TC_PCIE_RP_CFG_FPBCAPR_FPBMEMLS | B_TC_PCIE_RP_CFG_FPBCAPR_FPBMEMHS     |
                        (V_FPBCAPR_FPBMEMLVECSIZES << N_TC_PCIE_RP_CFG_FPBCAPR_FPBMEMLVECSIZES) |
                        (V_FPBCAPR_FPBMEMHVECSIZES << N_TC_PCIE_RP_CFG_FPBCAPR_FPBMEMHVECSIZES));

  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_FPBCAPR, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_FPBCAPR, Data32, IpWrRegFlagSize32Bits);
}

/**
This function creates Capability and Extended Capability List

@param[in] CfgCntxt              PCIe Root Port register access context
@param[in] PcieRootPortPolicy    PCIe RP configuration

**/
void
IpTcPcieInitCapabilityList (
  IP_WR_REG_CNTXT       CfgCntxt,
  IP_TC_PCIE_RP_CONFIG  *PcieRootPortPolicy
  )
{
  UINT32      Data32;
  UINT32      Data32And;
  UINT32      Data32Or;
  UINT16      Data16;
  UINT8       Data8;
  UINT16      NextCap;

  ///
  /// If not valid root port configuration space then return
  ///
  if (IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_ID, IpWrRegFlagSize16Bits) == 0xFFFF) {
    TC_PCIE_RP_PRINT_ERROR_PINST_NULL ("[TCSS] IpTcPcieInitCapabilityList Invalid/Disabled RpBase\n");
    return;
  }

  ///
  /// I.Build PCI Capability linked list
  /// 0x34 - CAPP
  /// 0x40 - CLIST
  /// 0x80 - MID
  /// 0x90 - SVCAP
  /// 0xD0 - FPBCAP
  /// 0xA0 - PMCAP
  ///
  Data8 = R_TC_PCIE_RP_CFG_CLIST;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_CAPP, Data8, IpWrRegFlagSize8Bits);

  Data16 = (R_TC_PCIE_RP_CFG_MID << N_TC_PCIE_RP_CFG_CLIST_NEXT) | V_TCSS_CLIST_CID;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_CLIST, Data16, IpWrRegFlagSize16Bits);

  Data16 = (R_TC_PCIE_RP_CFG_SVCAP << N_TC_PCIE_RP_CFG_MID_NEXT) | V_TCSS_MID_CID;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_MID, Data16, IpWrRegFlagSize16Bits);
  // Disable FPB
  //
  Data16 = (R_TC_PCIE_RP_CFG_PMCAP << N_TC_PCIE_RP_CFG_SVCAP_NEXT) | V_TCSS_SVCAP_CID;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_SVCAP, Data16, IpWrRegFlagSize16Bits);

  Data16 =  V_TCSS_PMCAP_CID;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PMCAP, Data16, IpWrRegFlagSize16Bits);

  ///
  /// II.Build PCI Extended Capability linked list
  /// 0xA90 - DLFECH (CAPID:0025h)
  /// 0xA30 - SPEECH (CAPID:0019h) Secondary PCI Express Extended Capability Header
  /// 0x220 - ACSECH (CAPID:000Dh) ACS Extended Capability
  /// 0x150 - PTMECH (CAPID:001Fh) PTM Extended Capability Register
  /// 0x100 - AECH   (CAPID:0001h) Advanced Error Reporting Capability
  ///

  /*
  a. NEXT_CAP = 0h
  */
  NextCap = V_TCSS_EXCAP_NCO_LISTEND;

  /*
   Program [0xA90] Data Link Feature Extended Capability Header (DLFECH)
    1. Set Next Capability Offset, 0xA90[31:20] = NEXT_CAP
    2. Set Capability Version, 0xA90[19:16] = 1h
    3. Set Capability ID, 0xA90[15:0] = 0025h
    4.  NEXT_CAP = 0xA90
  */
  Data32 = (V_TCSS_DLFECH_CV << N_TC_PCIE_RP_CFG_DLFECH_CV) | V_TCSS_DLFECH_CID;
  Data32 |= (NextCap << N_TC_PCIE_RP_CFG_DLFECH_NCO);
  NextCap = R_TC_PCIE_RP_CFG_DLFECH;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_DLFECH, Data32, IpWrRegFlagSize32Bits);

  /*
  b. If the RP is GEN3 capable (by fuse and BIOS policy), enable Secondary PCI Express Extended Capability
  1. Set Next Capability Offset,0xA30[31:20] = NEXT_CAP
  2. Set Capability Version, 0xA30[19:16] = 1h
  3. Set Capability ID,  0xA30[15:0] = 0019h
  4. NEXT_CAP = 0xA30
  ELSE, set 0xA30[31:0] = 0
  */
  Data32 = (V_TC_PCIE_RP_CFG_SPEECH_CV << N_TC_PCIE_RP_CFG_SPEECH_CV) | V_TC_PCIE_RP_CFG_SPEECH_CID;
  Data32 |= (NextCap << N_TC_PCIE_RP_CFG_SPEECH_NCO);
  NextCap = R_TC_PCIE_RP_CFG_SPEECH;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_SPEECH, Data32, IpWrRegFlagSize32Bits);

  /*
  e. If support PTM
  1. Set Next Capability Offset,  150h[31:20] = NEXT_CAP
  2. Set Capability Version, 150h[19:16] = 1h
  3. Set Capability ID, 150h[15:0] = 001Fh
  4. Read and write back to  150h
  5. NEXT_CAP = 150h
  */
  Data32 = 0;
  if (PcieRootPortPolicy->PtmEnabled == TRUE) {
    Data32 = (V_TC_PCIE_RP_CFG_PTMECH_CV << N_TC_PCIE_RP_CFG_PTMECH_CV) | V_TC_PCIE_RP_CFG_PTMECH_CID;
    Data32 |= (NextCap << N_TC_PCIE_RP_CFG_PTMECH_NCO);
    NextCap = R_TC_PCIE_RP_CFG_PTMECH;
  }
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PTMECH, Data32, IpWrRegFlagSize32Bits);

  /*
  d. If support ACS
  1. Set Next Capability Offset,  0x220[31:20] = NEXT_CAP
  2. Set Capability Version, 0x220[19:16] = 1h
  3. Set Capability ID, 0x220[15:0] = 000Dh
  4. Read and write back to  0x220
  5. NEXT_CAP = 0x220
  */
  Data32 = 0;
  if (PcieRootPortPolicy->AcsEnabled == TRUE) {
    Data32 = (V_TC_PCIE_RP_CFG_ACSECH_CV << N_TC_PCIE_RP_CFG_ACSECH_CV) | V_TC_PCIE_RP_CFG_ACSECH_CID;
    Data32 |= (NextCap << N_TC_PCIE_RP_CFG_ACSECH_NCO);
    NextCap = R_TC_PCIE_RP_CFG_ACSECH;
  }
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_ACSECH, Data32, IpWrRegFlagSize32Bits);

  /*
  g. If support Advanced Error Reporting
  1. Set Next Capability Offset, 100h[31:20] = NEXT_CAP
  2. Set Capability Version, 100h[19:16] = 1h
  3. Set Capability ID, 100h[15:0] = 0001h
  ELSE
  1. Set Next Capability Offset, 100h[31:20] = NEXT_CAP
  2. Set Capability Version, 100h[19:16]  = 0h
  3. Set Capability ID, 100h[15:10]  = 0000h
  */
  Data32 = 0;
  if (PcieRootPortPolicy->AdvancedErrorReporting) {
    Data32 = (V_TC_PCIE_RP_CFG_AECH_CV << N_TC_PCIE_RP_CFG_AECH_CV) | V_TC_PCIE_RP_CFG_AECH_CID;
  }
  Data32 |= (NextCap << N_TC_PCIE_RP_CFG_AECH_NCO);
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_AECH, Data32, IpWrRegFlagSize32Bits);

  ///
  /// III. Initalize Capability Registers
  ///

  /// FPB configuration
  IpTcPcieSetFpbcapr (CfgCntxt);

  ///
  /// PCI Express capabilities
  /// Slot Implemented R_TC_PCIE_RP_CFG_XCAP 0x42[8]  = 1
  /// Max Payload R_TC_PCIE_RP_CFG_DCAP 0x44[0:7] = 0 (128B)
  ///
  Data16 = (UINT16)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_XCAP, IpWrRegFlagSize16Bits);
  Data16 &= (UINT16) (~B_TC_PCIE_RP_CFG_XCAP_CV);
  Data16 |= B_TC_PCIE_RP_CFG_XCAP_SI | V_TCSS_XCAP_CV;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_XCAP, Data16, IpWrRegFlagSize16Bits);

  Data16 = (UINT16)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_DCAP, IpWrRegFlagSize16Bits);
  Data16 &= (UINT16)~B_TC_PCIE_RP_CFG_DCAP_MPS;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_DCAP, Data16, IpWrRegFlagSize16Bits);

  ///
  /// Aspm Support R_TC_PCIE_RP_CFG_LCAP 0x4C[11:10] = 0x2
  /// R_TC_PCIE_RP_CFG_LCAP.EL1 0x4C [17:15] = 0x4 for GEN3
  ///
  Data32And = (UINT32)~ (B_TC_PCIE_RP_CFG_LCAP_APMS | B_TC_PCIE_RP_CFG_LCAP_EL1);
  Data32Or = (UINT32) (0x2 << N_TC_PCIE_RP_CFG_LCAP_APMS) | (0x4 << N_TC_PCIE_RP_CFG_LCAP_EL1);
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_LCAP, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_LCAP, Data32, IpWrRegFlagSize32Bits);

  ///
  /// Physical Slot Number R_TC_PCIE_RP_CFG_SLCAP 0x54 [31:19] ,SLS/SLV[16:7],HotplugCapable HPC[6] ,HPS[5]
  ///
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_SLCAP, IpWrRegFlagSize32Bits);
  Data32 |= (PcieRootPortPolicy->SlotNumber << 19);
  Data32 |= (B_TC_PCIE_RP_CFG_SLCAP_HPC | B_TC_PCIE_RP_CFG_SLCAP_HPS);
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_SLCAP, Data32, IpWrRegFlagSize32Bits);

  ///
  ///  No Soft Reset R_TC_PCIE_RP_CFG_PMCS 0xA4[3] = 0
  ///
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PMCS, IpWrRegFlagSize32Bits);
  Data32 &= ~ ((UINT32)B_TC_PCIE_RP_CFG_PMCS_NSR);
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PMCS, Data32, IpWrRegFlagSize32Bits);

  ///
  /// Set DPC capabilities
  ///
  Data16 = (UINT16)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_DPCCAPR, IpWrRegFlagSize16Bits);
  Data16 &= (UINT16) ~B_TC_PCIE_RP_CFG_DPCCAPR_RPEFDPC;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_DPCCAPR, Data16, IpWrRegFlagSize16Bits);

  ///
  /// Init ACS capabilities & PTM capabilities
  /// Set ACS Upstream Forwarding (U)
  ///
  Data16 = (UINT16)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_ACSCAPR, IpWrRegFlagSize16Bits);
  Data16 |= B_TC_PCIE_RP_CFG_ACSCAPR_U;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_ACSCAPR, Data16, IpWrRegFlagSize16Bits);

  if (PcieRootPortPolicy->PtmEnabled) {
    Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PTMCAPR, IpWrRegFlagSize32Bits) | B_TC_PCIE_RP_CFG_PTMCAPR_PTMRSPC;
    IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PTMCAPR, Data32, IpWrRegFlagSize32Bits);
  }

  ///
  /// 0xF0  10:8  IOSF_Max_Payload_Size (IMPS) 111b
  ///
  Data32And = (UINT32) ~ B_TC_PCIE_RP_CFG_IPCS_IMPS;
  Data32Or = (0x07 << N_TC_PCIE_RP_CFG_IPCS_IMPS);
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_IPCS, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_IPCS, Data32, IpWrRegFlagSize32Bits);

  ///
  /// 0xD0  24  Upstream Posted Split Disable (UPSD): 0
  /// 0xD0  2:0  Upstream Posted Request Size (UPRS): 111b
  /// 0xD0  6:4  Upstream Non-Posted Request Size (UNRS):111b
  /// 0xD0  13:12 Upstream Non-Posted Request Delay (UNRD):01b
  ///
  Data32And = (UINT32)~ (B_TC_PCIE_RP_CFG_CCFG_UPSD | B_TC_PCIE_RP_CFG_CCFG_UPRS | B_TC_PCIE_RP_CFG_CCFG_UNRS | B_TC_PCIE_RP_CFG_CCFG_UNRD);
  Data32Or = (0x07 | (0x07 << N_TC_PCIE_RP_CFG_CCFG_UNRS) | (1 << N_TC_PCIE_RP_CFG_CCFG_UNRD));
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_CCFG, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_CCFG, Data32, IpWrRegFlagSize32Bits);

  ///
  /// Link Speed Training Policy (LSTP) 0xD4 6 1 - BIOS to program it to 1 only after the PCIe
  ///                                              link trains to Gen3 or higher
  /// EOI Forwarding Disable (EOIFD):  0xD4 1 1
  /// Poisoned TLP Non-Fatal Advisory Error Enable (PTNFAE): 0xD4 12 1
  ///
  Data32Or = B_TC_PCIE_RP_CFG_MPC2_LSTP | B_TC_PCIE_RP_CFG_MPC2_PTNFAE | B_TC_PCIE_RP_CFG_MPC2_EOIFD;
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_MPC2, IpWrRegFlagSize32Bits);
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_MPC2, Data32, IpWrRegFlagSize32Bits);

  ///
  /// MCTP Message Bus Number Check Enable (MMBNCE): 0xD8 27 1b
  /// According to section 5.9.4.3 of PCIe root port BWG Rev2.27, the FCDL1E should be set for TBT root port.
  /// Flow Control During L1 Entry (FCDL1E): 0xD8 21 1b
  /// Flow Control Update Policy (FCP): 0xD8 6:4 100b
  /// MCTP Support Enable (MCTPSE): 0xD8 3 1b
  ///
  Data32And = (UINT32)~ (B_TC_PCIE_RP_CFG_MPC_FCP);
  Data32Or  = B_TC_PCIE_RP_CFG_MPC_MMBNCE | B_TC_PCIE_RP_CFG_MPC_FCDL1E | B_TC_PCIE_RP_CFG_MPC_MCTPSE | B_TC_PCIE_RP_CFG_MPC_IRRCE |
              0x4 << N_TC_PCIE_RP_CFG_MPC_FCP;
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_MPC, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_MPC, Data32, IpWrRegFlagSize32Bits);
}

/**
This function Initalizes the PCI PowerManagemnet Registres
SIP15 PCI Express Root Port BIOS Requirement Rev 0.9

@param[in] CfgCntxt              PCIe Root Port register access context
**/

void
IpTcPcieMiscInit (
  IP_WR_REG_CNTXT   CfgCntxt
  )
{
  UINT32  Data32And;
  UINT32  Data32Or;
  UINT32  Data32;

  ///
  /// Gen 2 x1 (G2X1) 0x300  23:20 0xB
  /// Gen 2 x2 (G2X2) 0x300  19:16 0x7
  /// Gen 2 x4 (G2X4) 0x300  15:12 0x5
  /// Gen 1 x1 (G1X1) 0x300  11:8  0xF
  /// Gen 1 x2 (G1X2) 0x300  7:4   0xA
  ///
  Data32And = (UINT32)~ (B_TC_PCIE_RP_CFG_PCIERTP1_G2X1 | B_TC_PCIE_RP_CFG_PCIERTP1_G2X2 | B_TC_PCIE_RP_CFG_PCIERTP1_G2X4 |
                         B_TC_PCIE_RP_CFG_PCIERTP1_G1X1 | B_TC_PCIE_RP_CFG_PCIERTP1_G1X2);
  Data32Or = (0xB << N_TC_PCIE_RP_CFG_PCIERTP1_G2X1) | (0x7 << N_TC_PCIE_RP_CFG_PCIERTP1_G2X2) | (0x5 << N_TC_PCIE_RP_CFG_PCIERTP1_G2X4) |
             (0xF << N_TC_PCIE_RP_CFG_PCIERTP1_G1X1) | (0xA << N_TC_PCIE_RP_CFG_PCIERTP1_G1X2);
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PCIERTP1, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PCIERTP1, Data32, IpWrRegFlagSize32Bits);

  ///
  /// Gen 3 x1 (G3X1) 0x304  11:8   0xC
  /// Gen 3 x2 (G3X2) 0x304  7:4    0x9
  /// Gen 3 x4 (G3X4) 0x304  3:0    0x7
  Data32And = (UINT32)~ (B_TC_PCIE_RP_CFG_PCIERTP2_G3X1 | B_TC_PCIE_RP_CFG_PCIERTP2_G3X2 | B_TC_PCIE_RP_CFG_PCIERTP2_G3X4);
  Data32Or = (0xC << N_TC_PCIE_RP_CFG_PCIERTP2_G3X1) | (0x9 << N_TC_PCIE_RP_CFG_PCIERTP2_G3X2) | (0x7 << N_TC_PCIE_RP_CFG_PCIERTP2_G3X4);
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PCIERTP2, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PCIERTP2, Data32, IpWrRegFlagSize32Bits);

  ///
  ///  Completion Ordering Mode (COM)0xEC1  13 1
  ///
  Data32Or = B_TC_PCIE_RP_CFG_DC_COM;
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_DC, IpWrRegFlagSize32Bits);
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_DC, Data32, IpWrRegFlagSize32Bits);

  ///
  /// Section 3.23 Coalescing
  ///

  ///  Posted Write Chaining Enable (PWCE):0x594  0 1
  ///  Completion Chaining Enable (DDCE):0x594  1 1
  ///  Chain Timer (CT):0x594  9:02 0x8
  ///  Chain Timer Enable (CTE):0x594  10 1
  ///  Relaxed Order Attribute Override Policy (ROAOP):0x594  11 0x0
  ///  Posted Chain Limit Mode (PCLM):0x594  14:13 0x2
  ///  Non-Posted Chain Limit Mode (NPCLM):0x594  16:15 0x2

  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_COCTL_PCLM | B_TC_PCIE_RP_CFG_COCTL_NPCLM | B_TC_PCIE_RP_CFG_COCTL_CT | B_TC_PCIE_RP_CFG_COCTL_ROAOP);
  Data32Or = B_TC_PCIE_RP_CFG_COCTL_PWCE | B_TC_PCIE_RP_CFG_COCTL_DDCE | 0x8 << N_TC_PCIE_RP_CFG_COCTL_CT |
    B_TC_PCIE_RP_CFG_COCTL_CTE | (0x2 << N_TC_PCIE_RP_CFG_COCTL_PCLM) | (0x2 << N_TC_PCIE_RP_CFG_COCTL_NPCLM);
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_COCTL, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_COCTL, Data32, IpWrRegFlagSize32Bits);


  ///
  /// Section 3.28  LTR Subtraction
  ///

  ///  LTR Non-Snoop Latency Subtraction Enable (LTRNSLSUBEN) : 0xC54  31 1
  ///  LTR Non-Snoop Latency Scale Subtraction Value (LTRNSLSSUBV):0xC54  28:26 0x2
  ///  LTR Non-Snoop Latency Subtraction Value (LTRNSLSUBV) : 0xC54  25 : 16 0x5
  ///  LTR Snoop Latency Subtraction Enable (LTRSLSUBEN) : 0xC54  15 1
  ///  LTR Snoop Latency Scale Subtraction Value (LTRSLSSUBV) : 0xC54  12 : 10 0x2
  ///  LTR Snoop Latency Subtraction Value (LTRSLSUBV) 0xC54  9:00 0x5

  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_CLTRSUB_LTRNSLSSUBV | B_TC_PCIE_RP_CFG_CLTRSUB_LTRNSLSUBV |
                         B_TC_PCIE_RP_CFG_CLTRSUB_LTRSLSSUBV | B_TC_PCIE_RP_CFG_CLTRSUB_LTRSLSUBV);
  Data32Or = (B_TC_PCIE_RP_CFG_CLTRSUB_LTRNSLSUBEN | B_TC_PCIE_RP_CFG_CLTRSUB_LTRSLSUBEN | 0x2 << N_TC_PCIE_RP_CFG_CLTRSUB_LTRNSLSSUBV |
             0x5 << N_TC_PCIE_RP_CFG_CLTRSUB_LTRNSLSUBV | 0x2 << N_TC_PCIE_RP_CFG_CLTRSUB_LTRSLSSUBV | 0x5);

  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_CLTRSUB, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_CLTRSUB, Data32, IpWrRegFlagSize32Bits);

}

/**
This function Initalizes the PCI PowerManagemnet Registers
SIP15 PCI Express Root Port BIOS Requirement Rev 0.9

@param[in] CfgCntxt              PCIe Root Port register access context
**/

void
IpTcPciePtmInit (
IP_WR_REG_CNTXT     CfgCntxt
 )
{
  UINT32  Data32And;
  UINT32  Data32Or;
  UINT32  Data32;

  ///
  /// Periodic Local TSC Link Fetch Frequency (PLTLFF):0x3B0  20:18  0x1h
  /// PTM Request Number Of Periodic ACK DLLP (PTMRNOPAD) : 0x3B0 5:4 0x1h
  /// PTM Request Periodic ACK Enable (PTMRPAE) : 0x3B0 6 0x1
  ///
  Data32And = (UINT32)~ (B_TC_PCIE_RP_CFG_PTMECFG_PLTLFF | B_TC_PCIE_RP_CFG_PTMECFG_PTMRNOPAD | B_TC_PCIE_RP_CFG_PTMECFG_IOSFMADP);
  Data32Or = (0x1 << N_TC_PCIE_RP_CFG_PTMECFG_PLTLFF) | (0x1 << N_TC_PCIE_RP_CFG_PTMECFG_PTMRNOPAD) |
             B_TC_PCIE_RP_CFG_PTMECFG_PTMRPAE | (0x2 << N_TC_PCIE_RP_CFG_PTMECFG_IOSFMADP);
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PTMECFG, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PTMECFG, Data32, IpWrRegFlagSize32Bits);

  ///
  /// PTM Enable (PTME) 0x158 0 1
  /// Root Select (RS) 0x158 1 1
  ///
  Data32Or = B_TC_PCIE_RP_CFG_PTMCTLR_PTME | B_TC_PCIE_RP_CFG_PTMCTLR_RS;
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PTMCTLR, IpWrRegFlagSize32Bits);
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PTMCTLR, Data32, IpWrRegFlagSize32Bits);
  ///
  /// PTM Root Capable (PTMRC) 0x154 2
  /// PTM Responder Capable (PTMRSPC) 0x154 1
  ///
  Data32Or = B_TC_PCIE_RP_CFG_PTMCAPR_PTMRC | B_TC_PCIE_RP_CFG_PTMCAPR_PTMRSPC;
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PTMCAPR, IpWrRegFlagSize32Bits);
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PTMCAPR, Data32, IpWrRegFlagSize32Bits);

  ///
  /// B_TC_PCIE_RP_CFG_ACRG3_PPTMTMUCFE  0x6cc 17:14 0x2
  /// B_TC_PCIE_RP_CFG_ACRG3_PPTMTMUCFE 0x6cc 18    0x1
  ///
  Data32And = (UINT32)~ (B_TC_PCIE_RP_CFG_ACRG3_PPTMTMUCF);
  Data32Or = (0x2 << N_TC_PCIE_RP_CFG_ACRG3_PPTMTMUCF) | B_TC_PCIE_RP_CFG_ACRG3_PPTMTMUCFE;
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_ACRG3, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_ACRG3, Data32, IpWrRegFlagSize32Bits);
}

/**
This function Initalizes the PCI PowerManagemnet Registres
SIP15 PCI Express Root Port BIOS Requirement Rev 0.9

@param[in] CfgCntxt              PCIe Root Port register access context
@param[in] PcieRootPortPolicy    PCIe RP configuration
**/
void
IpTcPciePowerManagementInit (
  IP_WR_REG_CNTXT       CfgCntxt,
  IP_TC_PCIE_RP_CONFIG  *PcieRootPortPolicy
  )
{
  UINT32  Data32And;
  UINT32  Data32Or;
  UINT8   Data8And;
  UINT8   Data8Or;
  UINT32  Data32;
  UINT8   Data8;

  ///
  /// Section 3.1.3 Clock Gating
  ///

  ///
  /// Dynamic Clock Gating Enable on ISM Active Enable  0xD0  15 1
  ///
  Data32Or  = B_TC_PCIE_RP_CFG_CCFG_DCGEISMA;
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_CCFG, IpWrRegFlagSize32Bits);
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_CCFG, Data32, IpWrRegFlagSize32Bits);

  ///
  /// Partition/Trunk Oscillator Clock Gate Enable (PTOCGE): 0xE1  6 1
  /// PCIe Link CLKREQ Enable (PCIELCLKREQEN) 0xE1  5 1
  /// PCIe Backbone CLKREQ Enable (PCIEBBCLKREQEN) 0xE1  4 1
  /// Shared Resource Dynamic Backbone Clock Gate Enable (SRDBCGEN) 0xE1  2 1
  /// Root Port Dynamic Link Clock Gate Enable (RPDLCGEN) 0xE1  1 1
  /// Root Port Dynamic Backbone Clock Gate Enable (RPDBCGEN) 0xE1  0 1
  ///
  Data8Or = (UINT8) (B_TC_PCIE_RP_CFG_RPDCGEN_PTOCGE | B_TC_PCIE_RP_CFG_RPDCGEN_LCLKREQEN | B_TC_PCIE_RP_CFG_RPDCGEN_BBCLKREQEN | B_TC_PCIE_RP_CFG_RPDCGEN_SRDBCGEN | B_TC_PCIE_RP_CFG_RPDCGEN_RPDLCGEN | B_TC_PCIE_RP_CFG_RPDCGEN_RPDBCGEN);
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_RPDCGEN, IpWrRegFlagSize8Bits);
  Data32 |= Data8Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_RPDCGEN, Data32, IpWrRegFlagSize8Bits);

  ///
  /// Sideband Endpoint Oscillator / Side Clock Gating Enable (SEOSCGE) 0xE2  4 1 - (PTOTOP) 0xE2 6 1
  /// MDLSWPR= 0
  ///
  Data8And = (UINT8)~ B_TC_PCIE_RP_CFG_RPPGEN_MDLSWPR;
  Data8Or = (B_TC_PCIE_RP_CFG_RPPGEN_SEOSCGE | B_TC_PCIE_RP_CFG_RPPGEN_PTOTOP);
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_RPPGEN, IpWrRegFlagSize8Bits);
  Data32 &= Data8And;
  Data32 |= Data8Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_RPPGEN, Data32, IpWrRegFlagSize8Bits);
  ///
  /// Side Clock Partition/Trunk Clock Gating Enable (SCPTCGE) 0xF7  6 1
  /// IOSF Sideband Interface Idle Counter (SIID):0xF7 3:2 0
  ///
  Data8And = (UINT8) ~ B_TC_PCIE_RP_CFG_IOSFSBCS_SIID;
  Data8Or = B_TC_PCIE_RP_CFG_IOSFSBCS_SCPTCGE;
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_IOSFSBCS, IpWrRegFlagSize8Bits);
  Data32 &= Data8And;
  Data32 |= Data8Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_IOSFSBCS, Data32, IpWrRegFlagSize8Bits);

  ///
  /// B_TC_PCIE_RP_CFG_PCIEDBG_CTONFAE 0x324 14 1
  /// B_TC_PCIE_RP_CFG_PCIEDBG_LDSWQRP 0x324 13 0
  /// B_TC_PCIE_RP_CFG_PCIEDBG_SPCE    0x324 5  1
  /// B_TC_PCIE_RP_CFG_PCIEDBG_LGCLKSQEXITDBTIMERS 0x324 25:24 00
  ///
  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_PCIEDBG_LDSWQRP | B_TC_PCIE_RP_CFG_PCIEDBG_LGCLKSQEXITDBTIMERS);
  Data32Or  = B_TC_PCIE_RP_CFG_PCIEDBG_CTONFAE | B_TC_PCIE_RP_CFG_PCIEDBG_SPCE;
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PCIEDBG, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PCIEDBG, Data32, IpWrRegFlagSize32Bits);

  ///
  /// Reset Link Layer In GEN3 Recovery (RLLG3R) 0x320  27:0
  /// Completion Relaxed Ordering Attribute Override Value (CROAOV) 0x320  24:1
  /// Completion Relaxed Ordering Attribute Override Enable (CROAOE) 0x320  23:1
  /// Completion Retry Status Replay Enable (CRSREN) 0x320 22:1
  /// B_PCIE_PCIECFG2_PMET 0x320  21:20  01
  ///
  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_PCIECFG2_PMET | B_TC_PCIE_RP_CFG_PCIECFG2_RLLG3R);
  Data32Or = (B_TC_PCIE_RP_CFG_PCIECFG2_CROAOV | B_TC_PCIE_RP_CFG_PCIECFG2_CROAOE | B_TC_PCIE_RP_CFG_PCIECFG2_CRSREN | 0x1 << N_TC_PCIE_RP_CFG_PCIECFG2_PMET);
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PCIECFG2, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PCIECFG2, Data32, IpWrRegFlagSize32Bits);

  ///
  /// Section 3.1.5 OBFF and Latency Tolerance Reporting
  ///

  ///
  /// Optimized Buffer Flush/Fill Supported (OBFFS) 0x64  19:18 10b
  /// 10-Bit Tag Completer Supported (PX10BTCS) 0x64  16 1
  /// LTR Mechanism Supported (LTRMS) 0x64  11 1
  ///
  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_DCAP2_OBFFS | B_TC_PCIE_RP_CFG_DCAP2_LTRMS);
  Data32Or  = B_TC_PCIE_RP_CFG_DCAP2_PX10BTCS;
  if (PcieRootPortPolicy->LtrEnable) {
    Data32Or |= B_TC_PCIE_RP_CFG_DCAP2_LTRMS;
  }
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_DCAP2, IpWrRegFlagSize32Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_DCAP2, Data32, IpWrRegFlagSize32Bits);

  if (PcieRootPortPolicy->LtrEnable) {
    Data8Or = 0;
    if (PcieRootPortPolicy->SnoopLatencyOverrideMode) {
      ///
      /// LTR Snoop Override Enable (LTRSOVREN) 404  0 1
      ///
      Data8Or   = B_TC_PCIE_RP_CFG_LTROVR2_LTRSOVREN;
      Data32And = B_TC_PCIE_RP_CFG_LTROVR_LTRSROVR | B_TC_PCIE_RP_CFG_LTROVR_LTRSLSOVRV | B_TC_PCIE_RP_CFG_LTROVR_LTRSLOVRV;
      Data32Or  = ( (PcieRootPortPolicy->SnoopLatencyOverrideMultiplier << N_TC_PCIE_RP_CFG_LTROVR_LTRSLSOVRV) & B_TC_PCIE_RP_CFG_LTROVR_LTRSLSOVRV) |
                  (PcieRootPortPolicy->SnoopLatencyOverrideValue & B_TC_PCIE_RP_CFG_LTROVR_LTRSLOVRV) | B_TC_PCIE_RP_CFG_LTROVR_LTRSROVR;
    }

    if (PcieRootPortPolicy->NonSnoopLatencyOverrideMode) {
      ///
      /// LTR Non-Snoop Override Enable (LTRNSOVREN) x404  1 1
      ///
      Data8Or   |= B_TC_PCIE_RP_CFG_LTROVR2_LTRNSOVREN;
      Data32And |= B_TC_PCIE_RP_CFG_LTROVR_LTRNSROVR | B_TC_PCIE_RP_CFG_LTROVR_LTRNSLSOVRV | B_TC_PCIE_RP_CFG_LTROVR_LTRNSLOVRV;
      Data32Or  |= B_TC_PCIE_RP_CFG_LTROVR_LTRNSROVR | ( (PcieRootPortPolicy->NonSnoopLatencyOverrideMultiplier << N_TC_PCIE_RP_CFG_LTROVR_LTRNSLSOVRV) & B_TC_PCIE_RP_CFG_LTROVR_LTRNSLSOVRV) |
                   ( (PcieRootPortPolicy->NonSnoopLatencyOverrideValue << N_TC_PCIE_RP_CFG_LTROVR_LTRNSLOVRV) & B_TC_PCIE_RP_CFG_LTROVR_LTRNSLOVRV);
    }

    if (PcieRootPortPolicy->ForceLtrOverride) {
      Data8Or |= B_TC_PCIE_RP_CFG_LTROVR2_LTROVRPLCY;
    }

    Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_LTROVR, IpWrRegFlagSize32Bits);
    Data32 &= Data32And;
    Data32 |= Data32Or;
    IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_LTROVR, Data32, IpWrRegFlagSize32Bits);

    ///
    /// LTR Non-Snoop Override Enable (LTRNSOVREN) x404  1 1 404  3 0
    /// LTR Snoop Override Enable (LTRSOVREN) 404  0 1
    ///
    Data8 = (UINT8)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_LTROVR2, IpWrRegFlagSize8Bits);
    Data8 |= Data8Or;
    IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_LTROVR2, Data8, IpWrRegFlagSize8Bits);
  }

  ///
  /// Section 3.1.6 PLL Shutdown
  ///

  ///
  /// PCI Express GEN2 PLL Off Enable (PXPG2PLLOFFEN) 0xF5  0 1
  /// PCI Express GEN3 PLL Off Enable (PXPG3PLLOFFEN) 0xF5  1 1
  /// Transmit Datapath Flush Timer (TDFT) 0xF5 7:6 00
  /// Transmit Configuration Change Wait Time (TXCFGCHGWAIT) 0xF5 5:4 00
  ///
  Data8And = (UINT8)~ (B_TC_PCIE_RP_CFG_PHYCTL2_TXCFGCHGWAIT | B_TC_PCIE_RP_CFG_PHYCTL2_TDFT);
  Data8Or = (B_TC_PCIE_RP_CFG_PHYCTL2_PXPG3PLLOFFEN | B_TC_PCIE_RP_CFG_PHYCTL2_PXPG2PLLOFFEN);
  Data32 = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PHYCTL2, IpWrRegFlagSize8Bits);
  Data32 &= Data8And;
  Data32 |= Data8Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PHYCTL2, Data32, IpWrRegFlagSize8Bits);

  return;
}

/**
This function Initalizes the PCI PowerManagemnet Registres 3.1.10 Power Gating
SIP15 PCI Express Root Port BIOS Requirement Rev 0.9

@param[in] CfgCntxt              PCIe Root Port register access context

**/
void
IpTcPciePowerGatingRegisterInit (
  IP_WR_REG_CNTXT     CfgCntxt,
  TCSS_VER            TcssVersion
  )
{
  UINT32  Data32And;
  UINT32  Data32Or;
  UINT32  Data32;
  UINT32  RegVal;

  ///
  /// Wake PLL On Detect mod-PHY Power Gating Exit Policy (WPDMPGEP) : 0xE8  17 1
  ///
  RegVal = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PWRCTL, IpWrRegFlagSize32Bits);
  RegVal |= B_TC_PCIE_RP_CFG_PWRCTL_WPDMPGEP;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PWRCTL, RegVal, IpWrRegFlagSize32Bits);

  ///
  /// Function Disable PHY Power Gating Enable (FDPPGE) :0x420  31 1
  /// L1 Full Squelch OFF Enable (L1FSOE): 420 0 0
  /// Detect and L23_Rdy State PHY Lane Power Gating Enable (DLSULPPGE): 420 30 1
  /// Detect, L23_Rdy State, Un - Configured Lane and Down - Configured Lane Squelch Disable (DLSULDLSD) :420 29 0
  ///
  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_PCIEPMECTL_L1FSOE | B_TC_PCIE_RP_CFG_PCIEPMECTL_DLSULDLSD);
  Data32Or =  B_TC_PCIE_RP_CFG_PCIEPMECTL_FDPPGE | B_TC_PCIE_RP_CFG_PCIEPMECTL_DLSULPPGE;
  RegVal = (UINT32)  IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PCIEPMECTL, IpWrRegFlagSize32Bits);
  RegVal &= Data32And;
  RegVal |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PCIEPMECTL, RegVal, IpWrRegFlagSize32Bits);

  RegVal =  (UINT32) IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_DCTL, IpWrRegFlagSize16Bits);
  RegVal |= (UINT32) (B_TC_PCIE_RP_CFG_DCTL_CEE |
                      B_TC_PCIE_RP_CFG_DCTL_NFE |
                      B_TC_PCIE_RP_CFG_DCTL_FEE |
                      B_TC_PCIE_RP_CFG_DCTL_URE
                     );
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_DCTL, (UINT16) RegVal, IpWrRegFlagSize16Bits);

  ///
  /// Chassis PMC Save and Restore Enable (CPMCSRE)  : 0x424   27 1
  /// PHY Common Lane Power Gating Enable (PHYCLPGE) : 0x424   11 1
  /// Function Disable Controller Power Gating Enable (FDCPGE) : 0x424   8 1
  /// Detect State Controller Power Gating Enable (DETSCPGE) : 0x424  7 1
  /// L23_Rdy State Controller Power Gating Enable (L23RDYSCPGE) : 0x424  6 1
  /// Disabled State Controller Power Gating Enable   (DISSCPGE) : 0x424  5 1
  /// L1 State Controller Power Gating Enable (L1SCPGE) : 0x424  4 1
  ///
  Data32Or = (B_TC_PCIE_RP_CFG_PCIEPMECTL2_CPMCSRE | B_TC_PCIE_RP_CFG_PCIEPMECTL2_PHYCLPGE | B_TC_PCIE_RP_CFG_PCIEPMECTL2_FDCPGE | B_TC_PCIE_RP_CFG_PCIEPMECTL2_DETSCPGE |
  B_TC_PCIE_RP_CFG_PCIEPMECTL2_L23RDYSCPGE | B_TC_PCIE_RP_CFG_PCIEPMECTL2_DISSCPGE | B_TC_PCIE_RP_CFG_PCIEPMECTL2_L1SCPGE);
  RegVal = (UINT32) IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PCIEPMECTL2, IpWrRegFlagSize32Bits);
  RegVal |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PCIEPMECTL2, RegVal, IpWrRegFlagSize32Bits);

  ///
  /// Hardware Autonomous Enable (HAE):  0x428  5 0
  /// Sleep Enable (SE): 0x428  1 0
  /// PMC Request Enable (PMCRE):0x428  0 1
  ///
  Data32And = ~ ((UINT32) (B_TC_PCIE_RP_CFG_PCE_HAE | B_TC_PCIE_RP_CFG_PCE_SE));
  Data32Or = B_TC_PCIE_RP_CFG_PCE_PMCRE;
  if (TcssVersion == TcssNewH) {
    Data32Or |= B_TC_PCIE_RP_CFG_PCE_SE;
  }
  Data32 = (UINT32) IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PCE, IpWrRegFlagSize8Bits);
  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PCE, (UINT8)Data32, IpWrRegFlagSize8Bits);

  Data32 = (UINT32) IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_AECR1G3, IpWrRegFlagSize32Bits);
  Data32 |= B_TC_PCIE_RP_CFG_AECR1G3_DTCGCM;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_AECR1G3, Data32, IpWrRegFlagSize32Bits);

  /// Program TVM to 0x7F
  Data32 = (UINT32) IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_V0CTL, IpWrRegFlagSize32Bits);
  Data32 |= B_TC_PCIE_RP_CFG_V0CTL_TVM;  // [7:1] = 0x7F
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_V0CTL, Data32, IpWrRegFlagSize32Bits);

  ///
  /// Bank Enable Pulse Width (BEPW) 0x5F0  3:0 "0101"
  /// Restore Enable Pulse Width (REPW) 0x5F0  7:4 "0001"
  /// Entry and Exit Hysteresis (EEH) 0x5F0  9:8 "10"
  ///
  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_HWSNR_EEH | B_TC_PCIE_RP_CFG_HWSNR_REPW | B_TC_PCIE_RP_CFG_HWSNR_BEPW);
  Data32Or = ( (0x02 << N_TC_PCIE_RP_CFG_HWSNR_EEH) | (0x01 << N_TC_PCIE_RP_CFG_HWSNR_REPW) | 0x05 );
  RegVal = (UINT32) IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_HWSNR, IpWrRegFlagSize32Bits);
  RegVal &= Data32And;
  RegVal |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_HWSNR, RegVal, IpWrRegFlagSize32Bits);

  ///
  /// PM_REQ Block Response Time (PMREQBLKRSPT) 0x5F4  2:0 "101"
  ///
  Data32And = ~ (UINT32) B_TC_PCIE_RP_CFG_PGCTRL_PMREQBLKRSPT;
  Data32Or = 0x05 ;
  RegVal = (UINT32) IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PGCTRL, IpWrRegFlagSize32Bits);
  RegVal &= Data32And;
  RegVal |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PGCTRL, RegVal, IpWrRegFlagSize32Bits);

  ///
  /// PM_REQ Block and Power Gate Response Time (PMREQBLKPGRSPT) 0x5BC  7:5 "101"
  /// PM_REQ_Clock Wake Control (PMREQCWC).0x5BC 18:16  "110"
  ///
  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_ADVMCTRL_PMREQCWC | B_TC_PCIE_RP_CFG_ADVMCTRL_PMREQBLKPGRSPT);
  Data32Or = (0x06 << N_TC_PCIE_RP_CFG_ADVMCTRL_PMREQCWC)      |
             (0x5 << N_TC_PCIE_RP_CFG_ADVMCTRL_PMREQBLKPGRSPT) |
             B_TC_PCIE_RP_CFG_ADVMCTRL_G3STFER                 |
             B_TC_PCIE_RP_CFG_ADVMCTRL_EIOSMASKRX              |
             B_TC_PCIE_RP_CFG_ADVMCTRL_EIOSDISDS               |
             B_TC_PCIE_RP_CFG_ADVMCTRL_INRXL0CTRL;
  RegVal = (UINT32) IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_ADVMCTRL, IpWrRegFlagSize32Bits);
  RegVal &= Data32And;
  RegVal |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_ADVMCTRL, RegVal, IpWrRegFlagSize32Bits);

  RegVal =  (UINT32) IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_ACGR3S2, IpWrRegFlagSize32Bits);
  RegVal &= ~ (UINT32) (B_TC_PCIE_RP_CFG_ACGR3S2_LSTPTLS);
  RegVal |= (1 << N_TC_PCIE_RP_CFG_ACGR3S2_LSTPTLS);
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_ACGR3S2, RegVal, IpWrRegFlagSize32Bits);

  ///
  /// L1 Power Gating LTR Enable (L1PGLTREN) 0x5C0  0 1
  /// L1 Power Gating LTR Threshold Latency Scale Value (L1PGLTRTLSV) 0x5C0 31:29 0x2
  /// L1 Power Gating LTR Threshold Latency Value  (L1PGLTRTLV) 0x5C0 25:16 0x32
  ///
  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_PGTHRES_L1PGLTRTLSV | B_TC_PCIE_RP_CFG_PGTHRES_L1PGLTRTLV);
  Data32Or = (B_TC_PCIE_RP_CFG_PGTHRES_L1PGLTREN | 0x2 << N_TC_PCIE_RP_CFG_PGTHRES_L1PGLTRTLSV | 0x32 << N_TC_PCIE_RP_CFG_PGTHRES_L1PGLTRTLV);
  RegVal = (UINT32)  IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_PGTHRES, IpWrRegFlagSize32Bits);
  RegVal &= Data32And;
  RegVal |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_PGTHRES, RegVal, IpWrRegFlagSize32Bits);

  return;
}

/**
This function Initalizes the Chapter 5  TBT Specific Configuration
SIP15 PCI Express Root Port BIOS Requirement Rev 0.9

@param[in] CfgCntxt              PCIe Root Port register access context

**/
void
IpTcPcieITbtInit (
  IP_WR_REG_CNTXT CfgCntxt
  )
{
  UINT32  Data32And;
  UINT32  Data32Or;
  UINT32  RegVal;

  ///
  /// PTM Block Enable (PTMBE) 0xFC  0 1
  /// Periodic Mode Enable (PME) 0xFC  1 0
  /// Filter Increment Enable (FIE) 0xFC  2 1
  /// Filter Strength (FS) 0xFC  7:3 0xC
  /// Calculation Period (CP) 0xFC  15:8 0x0
  /// TMU Sampling Correction (TMUSC) 0xFC  20:16 0x2
  /// Threshold Limit (TL) 0xFC  25:21 0xF
  ///

  Data32And = ~ (UINT32) (B_TC_PCIE_RP_CFG_TBTPTMBCTRL_PME | B_TC_PCIE_RP_CFG_TBTPTMBCTRL_FS | B_TC_PCIE_RP_CFG_TBTPTMBCTRL_CP |
                        B_TC_PCIE_RP_CFG_TBTPTMBCTRL_TMUSC | B_TC_PCIE_RP_CFG_TBTPTMBCTRL_TL);
  Data32Or = (B_TC_PCIE_RP_CFG_TBTPTMBCTRL_PTMBE | B_TC_PCIE_RP_CFG_TBTPTMBCTRL_FIE) | (0x0C << N_TC_PCIE_RP_CFG_TBTPTMBCTRL_FS) |
             (0x02 << N_TC_PCIE_RP_CFG_TBTPTMBCTRL_TMUSC) | (0x0F << N_TC_PCIE_RP_CFG_TBTPTMBCTRL_TL);

  RegVal = (UINT32)IpWrRegRead (CfgCntxt, R_TC_PCIE_RP_CFG_TBTPTMBCTRL, IpWrRegFlagSize32Bits);
  RegVal &= Data32And;
  RegVal |= Data32Or;
  IpWrRegWrite (CfgCntxt, R_TC_PCIE_RP_CFG_TBTPTMBCTRL, RegVal, IpWrRegFlagSize32Bits);

  return;
}


/**
This function Implements Early PCI Port initialization

SIP15 PCI Express Root Port BIOS Requirement Rev 0.9

@param[in] CfgCntxt              PCIe Root Port register access context
@param[in] PcieRootPortPolicy    PCIe RP configuration

**/
void
IpTcPciePortEarlyinit (
  IP_WR_REG_CNTXT       CfgCntxt,
  IP_TC_PCIE_RP_CONFIG  *PcieRootPortPolicy,
  TCSS_VER              TcssVersion
  )
{
  IpTcPciePowerManagementInit (CfgCntxt, PcieRootPortPolicy);
  IpTcPciePowerGatingRegisterInit (CfgCntxt, TcssVersion);
  if (PcieRootPortPolicy->PtmEnabled) {
    IpTcPciePtmInit (CfgCntxt);
  }
  IpTcPcieMiscInit (CfgCntxt);
  IpTcPcieITbtInit (CfgCntxt);
}

/**
  Instance structure initialization for this version of IP
  For current version of TC PCIe RP there is no requirements for this function.
  All data in Instance Structure is initialized by the caller.
  The function just returns success status.

  @param[in] pInst  TC PCIe RP Instance Structure pointer

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpTcPcieRpInstInit (
  IP_TC_PCIE_RP_INST    *pInst
  )
{
  TC_PCIE_RP_API_ENTRY ();

  return IpCsiStsSuccess;
}

/**
  TypeC PCIe Root Ports Initialization procedure.
  The function shall be the first API function called after IpTcPcieRpInstInit.
  This is a good place to fix bad register defaults or work around bugs.

  @param[in]  pInst TC PCIe RP Instance Structure pointer

  @retval         IpCsiStsSuccess on success
  @retval         !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpTcPcieRpIpInit (
  IP_TC_PCIE_RP_INST    *pInst
  )
{
  UINT8                   Index;
  IP_PCIE_ROOT_PORT       *PciRootPort;
  UINT32                  RegVal;
  UINT8                   InterruptPin;

  TC_PCIE_RP_API_ENTRY ();

  ///
  /// Initialize TBT PCI Root Ports
  ///
  TC_PCIE_RP_PRINT_INFO ("PcieRootPorts PcieFuncMap = 0x%x\n", pInst->PcieFuncMap);

  PciRootPort = pInst->PciRootPortList;
  for (Index = 0; PciRootPort != NULL; Index++) {
    if ( (pInst->DevEn.DevEn >> Index) & BIT0) {
      TC_PCIE_RP_PRINT_INFO ("[TCSS] IpTcPcieRpIpInit RootPortFn[%d] = 0x%x\n", Index, ((pInst->PcieFuncMap >> (Index * 3)) & 7));

      ///
      /// Main PCIe Root Port initialization
      ///
      IpTcPciePortEarlyinit (PciRootPort->PrimCfgAccess, &pInst->PciRootPortList->PcieRpConfig, pInst->TcssVersion);

      RegVal = (UINT32)IpWrRegRead (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_STRPFUSECFG, IpWrRegFlagSize32Bits);
      RegVal &= (UINT32) ~B_TC_PCIE_RP_CFG_STRPFUSECFG_PXIP;
      InterruptPin = Index + 1;
      RegVal |= (UINT32) (InterruptPin << N_TC_PCIE_RP_CFG_STRPFUSECFG_PXIP);
      IpWrRegWrite (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_STRPFUSECFG, RegVal, IpWrRegFlagSize32Bits);

      ///
      /// PCIe capability list initialization
      ///
      IpTcPcieInitCapabilityList (PciRootPort->PrimCfgAccess, &pInst->PciRootPortList->PcieRpConfig);

      RegVal =  (UINT32) IpWrRegRead (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_CMD, IpWrRegFlagSize16Bits);
      RegVal |= (UINT32) (B_TC_PCIE_RP_CFG_CMD_SEE);
      IpWrRegWrite (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_CMD, (UINT16) RegVal, IpWrRegFlagSize16Bits);

      RegVal =  (UINT32) IpWrRegRead (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_BCTRL, IpWrRegFlagSize16Bits);
      RegVal |= (UINT32) (B_TC_PCIE_RP_CFG_BCTRL_SE);
      IpWrRegWrite (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_BCTRL, (UINT16) RegVal, IpWrRegFlagSize16Bits);

      //
      // Program BLKDQDA and BLKDQDASA to 0 as per PCIE BWG r0.9
      //
      RegVal = (UINT32)IpWrRegRead (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_PCIEALC, IpWrRegFlagSize32Bits);
      RegVal &= (UINT32) ~ (B_TC_PCIE_RP_CFG_PCIEALC_BLKDQDA | B_TC_PCIE_RP_CFG_PCIEALC_BLKDQDASD);
      RegVal |= (UINT32)(B_TC_PCIE_RP_CFG_PCIEALC_RTD3PDSP | B_TC_PCIE_RP_CFG_PCIEALC_PDSP);
      IpWrRegWrite (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_PCIEALC, RegVal, IpWrRegFlagSize32Bits);

      RegVal =  (UINT32) IpWrRegRead (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_LCTL, IpWrRegFlagSize16Bits);
      RegVal |= (UINT32) (B_TC_PCIE_RP_CFG_LCTL_CCC);
      IpWrRegWrite (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_LCTL, RegVal, IpWrRegFlagSize16Bits);

      //
      //  PCIe RP Downstream port configuration
      //
      if (pInst->Callbacks->PcieRpDownStreamConfig != NULL) {
        pInst->Callbacks->PcieRpDownStreamConfig (Index);
      }
    }
    PciRootPort = PciRootPort->Next;
  }

  return IpCsiStsSuccess;
}

/**
  Locks PCIe Root Port configuration.

  @param[in]    PcieRpInst        PCIE Root Port instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         PCIe RP programming error
**/
IP_CSI_STATUS
IpTcPcieRpLockConfiguration (
  IP_TC_PCIE_RP_INST *pInst
  )
{
  UINT8               Index;
  IP_PCIE_ROOT_PORT   *PciRootPort;
  UINT32              RegVal;

  TC_PCIE_RP_API_ENTRY ();

  PciRootPort = pInst->PciRootPortList;
  for (Index = 0; PciRootPort != NULL; Index++) {
    if ( (pInst->DevEn.DevEn >> Index) & BIT0) {
      ///
      /// Program SRL bit to lock the PCIe RP at end of PCIe initialization.
      ///
      IpWrRegWrite (PciRootPort->PcrAccess, R_TC_PCIE_RP_PCR_SRL, B_TC_PCIE_RP_PCR_SRL_SRL, IpWrRegFlagSize32Bits);
      RegVal = (UINT32)IpWrRegRead (PciRootPort->PcrAccess, R_TC_PCIE_RP_PCR_IMRLE, IpWrRegFlagSize32Bits);
      RegVal |= B_TC_PCIE_RP_PCR_IMRLE_SRL;
      IpWrRegWrite (PciRootPort->PcrAccess, R_TC_PCIE_RP_PCR_IMRLE, RegVal, IpWrRegFlagSize32Bits);
    }
    PciRootPort = PciRootPort->Next;
  }
  return IpCsiStsSuccess;
}

/**
  This interface will return a version value that corresponds to the VerId provided and
  is formatted per the CSI specification.
  - CSI Defined Interface

  @param[in]      pInst      A pointer to the IP instance to be used.
  @param[in]      VerId      An enum value defined in the CSI specification of type IP_CSI_VER_ID.
  @retval         0          The VerId value is not recognized and a version could not be determined.
  @retval         Non-Zero   A version value corresponding to the VerId provided, and formatted per the CSI.
**/
UINT32
IpTcPcieRpGetVersion (
  IP_TC_PCIE_RP_INST    *pInst,
  IP_CSI_VER_ID         VerId
  )
{
  IP_CSI_VER_ID RetValue;

  TC_PCIE_RP_API_ENTRY ();

  RetValue = VERSION_UNKNOWN_TC_PCIE_RP;

  switch (VerId) {
    case (IpCsiVerIdHwIp):
      RetValue = VERSION_HW_TC_PCIE_RP;
      break;
    case (IpCsiVerIdHwIpCustom):
      RetValue = VERSION_HW_CUSTOM_TC_PCIE_RP;
      break;
    case (IpCsiVerIdIpFwIntf):
      RetValue = VERSION_IPFWINTF_TC_PCIE_RP;
      break;
    case (IpCsiVerIdIpFwIntfExt):
      RetValue = VERSION_IPFWINTFEXT_TC_PCIE_RP;
      break;
    case (IpCsiVerIdIpFw):
      RetValue = VERSION_FW_TC_PCIE_RP;
      break;
    case (IpCsiVerIdCsi):
      RetValue = VERSION_CSI;
      break;
    default:
      RetValue = VERSION_UNKNOWN_TC_PCIE_RP;
      break;
  }

  return RetValue;
}

/**
  Programs PCIe Root Port function mapping.
  Returns programmed mapping to be used by SOC related programming.

  @param[in]    PcieRpInst        PCIE Root Port instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         PCIe RP programming error
**/
IP_CSI_STATUS
IpTcPcieRpSetFunctionMap (
  IP_TC_PCIE_RP_INST *pInst
  )
{
  UINT32                  DevEn;
  UINT8                   Index;
  INTN                    FirstEnabledItbtPortNumber;
  UINT8                   ItbtPcieFunctionNumber[MAX_ITBT_PCIE_PORT];
  UINT32                  PcieFuncMap;
  IP_PCIE_ROOT_PORT       *PciRootPort;
  IP_WR_REG_INFO          *RegInfo;

  TC_PCIE_RP_API_ENTRY ();

  DevEn = pInst->DevEn.DevEn & 0xFF;

  //
  // Get the first enabled iTBT PCIe port number
  //
  FirstEnabledItbtPortNumber = 0;
  while (0 == (DevEn & 1)) {
    DevEn >>= 1;
    FirstEnabledItbtPortNumber++;
    if (FirstEnabledItbtPortNumber >= MAX_ITBT_PCIE_PORT) {
      // Error case if all PCIe ports are disabled.
      pInst->PcieFuncMap = ~(UINT32)0;
      return IpCsiStsError;
    }
  }

  ///
  /// According to each iTBT PCIe RP enable status to update Function Map.
  ///
  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    ItbtPcieFunctionNumber[Index] = Index;
    //
    // Swap the function number if the first enabled iTBT PCIe RP is not port0.
    //
    if ( (Index != 0) && (FirstEnabledItbtPortNumber == Index)) {
      ItbtPcieFunctionNumber[Index] = 0;
      ItbtPcieFunctionNumber[0] = Index;
    }
  }

  PciRootPort = pInst->PciRootPortList;
  for (Index = 0; Index < MAX_ITBT_PCIE_PORT && PciRootPort != NULL; Index++) {
    ///
    /// Program RP0FN for each iTBT PCIE Root Port if it is enabled then lock the register.
    /// Since currently we only defined PortId for PCIe0 ~ PCIe3, so we only program it for these 4 ports.
    ///
    IpWrRegWrite (PciRootPort->PcrAccess, R_TC_PCIE_RP_PCR_PCD, ItbtPcieFunctionNumber[Index], IpWrRegFlagSize32Bits);
    RegInfo = (IP_WR_REG_INFO*)(UINTN)PciRootPort->PrimCfgAccess;
    RegInfo->RegType.Pci.Fun = ItbtPcieFunctionNumber[Index];
    PciRootPort = PciRootPort->Next;
  }

  //
  // iTBT PCIE RP Function Number Assignment
  //
  PcieFuncMap = (ItbtPcieFunctionNumber[0] |
                 ItbtPcieFunctionNumber[1] << 3 |
                 ItbtPcieFunctionNumber[2] << 6 |
                 ItbtPcieFunctionNumber[3] << 9);

  pInst->PcieFuncMap = PcieFuncMap;

  return IpCsiStsSuccess;
}

/**
  Returns PCIe Root Port Function mapping

  @param[in]    pInst       PCIE RP instance structure
  @param[out]   Mapping     Pointer to mapping variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfuly
    - IpCsiStsError:         No PCIe Root Port are enabled
**/
IP_CSI_STATUS
IpTcPcieRpGetFunctionMap (
  IP_TC_PCIE_RP_INST  *pInst,
  UINT32              *Mapping
  )
{
  UINT32    DevEn;
  UINT8     Index;
  INTN      FirstEnabledItbtPortNumber;
  UINT8     ItbtPcieFunctionNumber[MAX_ITBT_PCIE_PORT];
  UINT32    PcieFuncMap;

  TC_PCIE_RP_API_ENTRY ();

  if (Mapping == NULL) {
    TC_PCIE_RP_PRINT_ERROR ("%a: Error: Mapping pointer not set!\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  DevEn = pInst->DevEn.DevEn & 0xFF;

  //
  // Get the first enabled iTBT PCIe port number
  //
  FirstEnabledItbtPortNumber = 0;
  while (0 == (DevEn & 1)) {
    DevEn >>= 1;
    FirstEnabledItbtPortNumber++;
    if (FirstEnabledItbtPortNumber >= MAX_ITBT_PCIE_PORT) {
      // Error case if all PCIe ports are disabled.
      pInst->PcieFuncMap = ~(UINT32)0;
      return IpCsiStsError;
    }
  }

  ///
  /// According to each iTBT PCIe RP enable status to update Function Map.
  ///
  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    ItbtPcieFunctionNumber[Index] = Index;
    //
    // Swap the function number if the first enabled iTBT PCIe RP is not port0.
    //
    if ( (Index != 0) && (FirstEnabledItbtPortNumber == Index)) {
      ItbtPcieFunctionNumber[Index] = 0;
      ItbtPcieFunctionNumber[0] = Index;
    }
  }

  //
  // iTBT PCIE RP Function Number Assignment
  //
  PcieFuncMap = (ItbtPcieFunctionNumber[0] |
                 ItbtPcieFunctionNumber[1] << 3 |
                 ItbtPcieFunctionNumber[2] << 6 |
                 ItbtPcieFunctionNumber[3] << 9);

  *Mapping = PcieFuncMap;

  return IpCsiStsSuccess;
}

/**
  Clears VGA and VGA-16-bit decodes of Bridge control register
  for all TC PCIe Root Ports

  @param[in]    pInst       PCIE RP instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfuly
    - IpCsiStsError:         PCIe Root Port disabled
**/
IP_CSI_STATUS
IpTcPcieRpClearVgaRegisters (
  IP_TC_PCIE_RP_INST  *pInst
  )
{
  UINT8               Index;
  IP_PCIE_ROOT_PORT   *PciRootPort;
  UINT16              RegVal;

  TC_PCIE_RP_API_ENTRY ();

  PciRootPort = pInst->PciRootPortList;
  for (Index = 0; PciRootPort != NULL; Index++) {
    if ( (pInst->DevEn.DevEn >> Index) & BIT0) {
      TC_PCIE_RP_PRINT_INFO ("IpTcPcieRpClearVgaRegisters: RootPort[%d]\n", Index);
      if ((UINT16)IpWrRegRead (PciRootPort->PrimCfgAccess, 0, IpWrRegFlagSize16Bits) == 0xFFFF) {
        TC_PCIE_RP_PRINT_ERROR ("IpTcPcieRpClearVgaRegisters: ERROR: RootPort[%d] not present but enabled!\n", Index);
        return IpCsiStsError;
      }
      RegVal = (UINT16)IpWrRegRead (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_BCTRL, IpWrRegFlagSize16Bits);
      RegVal &= (UINT16) ~(B_TC_PCIE_RP_CFG_BCTRL_VE | B_TC_PCIE_RP_CFG_BCTRL_V16);
      IpWrRegWrite (PciRootPort->PrimCfgAccess, R_TC_PCIE_RP_CFG_BCTRL, RegVal, IpWrRegFlagSize16Bits);
    }
    PciRootPort = PciRootPort->Next;
  }

  return IpCsiStsSuccess;
}
