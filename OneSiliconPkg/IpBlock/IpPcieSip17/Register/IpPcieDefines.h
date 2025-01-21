/** @file
  Register names for PCIe
  <b>Conventions</b>:
  - Prefixes:
    - Definitions beginning with "R_" are registers
    - Definitions beginning with "B_" are bits within registers
    - Definitions beginning with "V_" are meaningful values of bits within the registers
    - Definitions beginning with "S_" are register sizes
    - Definitions beginning with "N_" are the bit position


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

#ifndef __IP_PCIE_DEFINES_H__
#define __IP_PCIE_DEFINES_H__

#define IP_REVISION_MAJOR                     0x3

#ifndef BIT0
#define BIT0                     0x0001
#define BIT1                     0x0002
#define BIT2                     0x0004
#define BIT3                     0x0008
#define BIT4                     0x0010
#define BIT8                     0x0100
#define BIT24                0x01000000
#endif

#define MAX_UINT16  ((UINT16)0xFFFF)
#define ARRAY_SIZE(Array)  (sizeof (Array) / sizeof ((Array)[0]))

//
// Log Phy Control Policy 4 register
//
#define V_LPHYCP4_LGCLKSQEXITDBTIMERS           0x2
#define V_LPHYCP4_RXL0SEG12FTSE                 0x2

//
// PIPE Power Down Control register
//
#define V_PIPEPDCTL_DETNOPGPDCTL              0x3
#define V_PIPEPDCTL_DETPGPDCTL_PS3            0x3
#define V_PIPEPDCTL_DETPGPDCTL_PS13           0xd
#define V_PIPEPDCTL_L23NOPGPDCTL              0x3
#define V_PIPEPDCTL_L23PGPDCTL_PS3            0x3
#define V_PIPEPDCTL_L23PGPDCTL_PS13           0xd
#define V_PIPEPDCTL_DISNOPGPDCTL_PS3          0x3
#define V_PIPEPDCTL_DISNOPGPDCTL_PS13         0xd
#define V_PIPEPDCTL_DISPGPDCTL_PS13           0xd
#define V_PIPEPDCTL_DISPGPDCTL_PS3            0x3
#define V_PIPEPDCTL_L1PGNOPGPDCTL             0x4
#define V_PIPEPDCTL_L1PGUPGPDCTL              0x4

//
// PIPE Power Down Control 2 register
//
#define V_PIPEPDCTL2_L1D2PGPDCTL              0x4
#define V_PIPEPDCTL2_L1D2NOPGPDCTL            0x4
#define V_PIPEPDCTL2_L1D1PGPGPDCTL            0x4
#define V_PIPEPDCTL2_L1D1PGNOPGPDCTL          0x4
#define V_PIPEPDCTL2_DUCFGPGPDCTL             0xd
#define V_PIPEPDCTL2_L1D1UPUPGPDCTL           0x4
#define V_PIPEPDCTL2_L1UPNOPGPDCTL            0x4

//
// PIPE Power Down Control 3 register
//
#define V_PIPEPDCTL3_L1DLOWNOPGPDCTL          0x4
#define V_PIPEPDCTL3_L1DLOWPGPDCTL            0x4

//
// PIPE Power Down Control Extension register
//
#define V_PIPEPDCTLEXT_LSDPMRFM               0xe
#define V_PIPEPDCTLEXT_PDDPMRFM               0x17

//
// Feature Control 2 register
//
#define V_FCTL2_RPTOT                         0x5

//
// Reset Prep Decode 1 register
//
#define V_RPDEC1_RPWRERT                      0x10
#define V_RPDEC1_RPCRERT                      0x11

//
// Reset Prep Decode 2 register
//
#define V_RPDEC2_RPS3ERT                      0x3
#define V_RPDEC2_RPS4ERT                      0x4

//
//Reset Prep Decode 3 register
//
#define V_RPDEC3_RPS5ERT                      0x5
#define V_RPDEC3_RPDH                         0x5

//
// PCI Express Status 1 register
//

#define V_PCIESTS1_LTSMSTATE_DETRDY           0x01
#define V_PCIESTS1_LTSMSTATE_DISWAITPG        0x60

//
// Link Control 2 register
//
#define V_LCTL2_TLS_GEN3                      0x3
#define V_LCTL2_TLS_GEN4                      0x4
#define V_LCTL2_TLS_GEN5                      0x5

//
// Advance Mode Control register
//
#define V_ADVMCTRL_PMREQCWC_LNK_PRIM          0x6
#define V_ADVMCTRL_PMREQBLKPGRSPT_10US        0x2

//
// Hardware Save And Restore register
//
#define V_HWSNR_REPW_2CLK                     0x1
#define V_HWSNR_EEH_16CLK                     0x2
#define V_HWSNR_BEPW                          0x5

//
// Channel Configuration register
//
#define V_CCFG_UPRS_64B                       0x7
#define V_CCFG_UPRS_128B                      0x0
#define V_CCFG_UPRS_256B                      0x1
#define V_CCFG_UNRS_64B                       0x7
#define V_CCFG_UNRS_128B                      0x0
#define V_CCFG_UNRS_256B                      0x1
#define V_CCFG_UPRS_512B                      0x2
#define V_CCFG_UNRS_512B                      0x2

//
// Device Control 2 register
//
#define V_PCIE_DCTL2_CTV_DEFAULT                  0x0
#define V_PCIE_DCTL2_CTV_50MS_100MS               0x1
#define V_PCIE_DCTL2_CTV_1MS_10MS                 0x2
#define V_PCIE_DCTL2_CTV_16MS_55MS                0x5
#define V_PCIE_DCTL2_CTV_65MS_210MS               0x6
#define V_PCIE_DCTL2_CTV_260MS_900MS              0x9
#define V_PCIE_DCTL2_CTV_1S_3P5S                  0xA
#define V_PCIE_DCTL2_CTV_4S_13S                   0xD
#define V_PCIE_DCTL2_CTV_17S_64S                  0xE
#define B_PCIE_DCTL2_RANGE_A                      BIT0
#define B_PCIE_DCTL2_RANGE_B                      BIT1
#define B_PCIE_DCTL2_RANGE_C                      BIT2
#define B_PCIE_DCTL2_RANGE_D                      BIT3

//
// RX Margin Control 1 register
//
#define V_RXMC1_MMNOLS                        0x0
#define V_RXMC1_VMSLOP                        0x1
#define V_RXMC1_TMSLOP                        0x1
#define V_RXMC1_MSRTS_SE32                    0x1f
#define V_RXMC1_MSRTS_SC16                    0x3f
#define V_RXMC1_MSRVS                         0x1f
#define V_RXMC2_MNOTSS_SC16                   0x9
#define V_RXMC2_MNOTSS_SE32                   0x8
#define V_RXMC2_MMTOS                         0x14
#define V_RXMC2_MNOVSS                        0x20
#define V_RXMC2_MMVOS                         0x5

//
//  PCIe PM Extension Control register
//
#define V_PCIEPMECTL_L1LTRTLV                 0x41
#define V_PCIEPMECTL_POFFWT                   0x3

//
//Thermal And Power Throttling register
//
#define V_TNPT_TP_5_MS                        0x4
#define V_TNPT_TP_3_MS                        0x2

//
// PCIE Extension Capability Register
//
#define V_EXCAP_NCO_LISTEND                   0x0

//
// L1 Sub-States Extended Capability Register
//
#define V_EX_L1SCAP_PTPOS_10us                0x1

//
// Program L1 Sub-States Control 1 register
//
#define V_EX_L1SCTL1_CMRT                     0x3c

//
// Program PCI Power Management Control register
//
#define V_PMCS_PS_D3H                         0x3

//
//IOSF Primary Control registers
//
#define V_IPCS_IMRS_64B                       0x7
#define V_IPCS_IMRS_256B                      0x1
#define V_IPCS_IMPS_64B                       0x7

//
// Virtual Channel 1 Resource Control register
//
#define V_VC1CTL_ID_ONE                       0x1

//
// Program PCIe PM Extension Control 3 register
//
#define V_PCIEPMECTL3_OSCCGH_1US              0x1

//
// Vnn Removal register programming
//
#define V_VNNREMCTL_ISPLFVNNRE_16_OSC_CLK     0x1

//
// Coalescing register
//
#define V_COCTL_CT                            0x3

#define V_CLS_GEN2                            0x2

//
// Link Capabilities Register
//
#define V_LCAP_MLS_GEN3                       0x3

//
// Power Gating Control register
//
#define V_PGCTRL_PMREQBLKRSPT_25US            0x5

//
// L1 Sub-States Control 2 register
//
#define N_EX_L1SCTL2_POWT                     0x3

#define N_CDM_MCS                             0X0
#define N_CDM_MSS                             0X4
#define N_MM_MSST                             0X8

#define B_EX_L01EC_TP                         0xf ///< Bits used for each transmitter preset value
#define S_EX_L01EC_TP                         0x8 ///< Each of the transmitter presets configuration fields
#define S_EX_PL16L01EC_TP                     0x4 ///< Each of the transmitter presets configuration fields

#define R_LFFS_OFFSET_FROM_LANE_BASE          0x1c ///< This register offset is taken from base of the lane presets configuration
                                                     ///< starting with R_PCIE_CFG_LANE0_PRESETS register.
#define N_LFFS_FS                             0x8 ///< Offset of LFFS being 0x51C
#define S_SINGLE_LANE_PRESETS                 0x20


//
// Local Transmitter Coefficient Override 1 register
//
#define B_LTCO1_L0TCOE                          BIT24

//
// 16 GT/s Local Transmitter Coefficient Override 1 register
//
#define B_PX16GLTCO1_L0TCOE                     BIT24

//
// 32 GT/s Local Transmitter Coefficient Override 1 register
//
#define B_PX32GLTCO1_L0TCOE                     BIT24


#define B_PCIE_CFG_FOMS_FOMSV                         0x00FFFFFF
#define B_PCIE_CFG_FOMS_FOMSV0                        0x000000FF
#define N_PCIE_CFG_FOMS_FOMSV0                        0
#define B_PCIE_CFG_FOMS_FOMSV1                        0x0000FF00
#define N_PCIE_CFG_FOMS_FOMSV1                        8
#define B_PCIE_CFG_FOMS_FOMSV2                        0x00FF0000
#define N_PCIE_CFG_FOMS_FOMSV2                        16

//
//  "IndustryStandard/Pci30.h" Dependent Registers
//
#define PCI_VENDOR_ID_OFFSET                  0x00
#define PCI_PRIMARY_STATUS_OFFSET             0x06
#define PCI_HEADER_TYPE_OFFSET                0x0E
#define PCI_CAPBILITY_POINTER_OFFSET          0x34
#define PCI_MAXLAT_OFFSET                     0x3F ///< Max Latency Register

//
// Default Vendor ID and Subsystem ID
//
#define V_PCH_INTEL_VENDOR_ID                 0x8086 ///< Default Intel PCH Vendor ID

//
// Following are the PCI status control bit
//
#define EFI_PCI_STATUS_CAPABILITY            BIT4 ///< 0x0010

//
// the definition of Header Type
//
#define HEADER_TYPE_CARDBUS_BRIDGE           0x02

//
// Mask of Header type
//
#define HEADER_LAYOUT_CODE                   0x7f
///
/// defined in PC Card Standard
///
#define EFI_PCI_CARDBUS_BRIDGE_CAPABILITY_PTR 0x14

///
/// PCI Capability List IDs and records
///
#define EFI_PCI_CAPABILITY_ID_PCIEXP          0x10

//
// Latency Tolerance Reporting Extended Capability Registers (CAPID:0018h)
//
#define R_LTRECH_CID                            0x0018

#define R_PCIE_LTRECH_MSLR_OFFSET               0x04
#define B_PCIE_LTRECH_MSLR_VALUE                0x3FF
#define N_PCIE_LTRECH_MSLR_VALUE                0
#define B_PCIE_LTRECH_MSLR_SCALE                0x1C00
#define N_PCIE_LTRECH_MSLR_SCALE                10

#define R_PCIE_LTRECH_MNSLR_OFFSET              0x06
#define B_PCIE_LTRECH_MNSLR_VALUE               0x3FF
#define N_PCIE_LTRECH_MNSLR_VALUE               0
#define B_PCIE_LTRECH_MNSLR_SCALE               0x1C00
#define N_PCIE_LTRECH_MNSLR_SCALE               10

//
// Monitor signals
//
#define R_SIP_MONITOR_CAT2                      0x2
#define R_SIP_MONITOR_CAT2_BEST4COEFF_GEN3      0xf
#define R_SIP_MONITOR_CAT2_BEST4COEFF_GEN4      0x10
#define R_SIP_MONITOR_CAT2_BEST4COEFF_GEN5      0x11


//
// Capability ID
//
#define V_EX_AEC_CID                          0x0001
#define V_EX_VCCH_CID                         0x0002
#define V_EX_ACS_CID                          0x000d
#define V_EX_SPE_CID                          0x0019
#define V_EX_L1S_CID                          0x001e
#define V_EX_DPC_CID                          0x001d
#define V_EX_PTM_CID                          0x001f
#define V_EX_DLFECH_CID                       0x0025
#define V_EX_PL16GECH_CID                     0x0026
#define V_EX_PL16MECH_CID                     0x0027
#define V_EX_G5ECH_CID                        0x002A

#endif // __IP_PCIE_DEFINES_H__
