/** @file
  This file provides IOSF Sideband EndPoint Port IDs for PTL PCD.

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

#ifndef _PTL_PCD_SB_PORT_IDS_H_
#define _PTL_PCD_SB_PORT_IDS_H_

#define PTL_SID_F3_P2SB_1                   0xF3
#define PTL_SID_F2_P2SB_2                   0xF2

///
/// IOSF Sideband Target Port IDs definition common for all.
///

//
// IP: Audio & Context Engine
//
#define PTL_SID_F2_PID_ACE                      0xF250  // Audio Context Engine

//
// IP: Connectivity Integrated
//
#define PTL_SID_F2_PID_CNVI                     0xF251  // CNVI HIP
#define PTL_SID_F2_PID_WIFI                     0xF252  // WIFI controller
#define PTL_SID_F2_PID_BTCORE                   0xF253  // Bluetooth controller

//
// IP: CSME
//
#define PTL_SID_F2_PID_CSME0                    0xF240  // CSE
#define PTL_SID_F2_PID_CSME1                    0xF238  // KVMcc
#define PTL_SID_F2_PID_CSME2                    0xF239  // USB-R
#define PTL_SID_F2_PID_CSME4                    0xF241  // SMBus Message Transport (SMT) 1
#define PTL_SID_F2_PID_CSME5                    0xF242  // SMT2
#define PTL_SID_F2_PID_CSME6                    0xF243  // SMT3
#define PTL_SID_F2_PID_CSME10                   0xF23A  // CLink
#define PTL_SID_F2_PID_CSME12                   0xF23B  // PTIO
#define PTL_SID_F2_PID_CSME13                   0xF23C  // PMT
#define PTL_SID_F2_PID_CSME14                   0xF23D  // SMS1
#define PTL_SID_F2_PID_CSME15                   0xF23E  // SMS2
#define PTL_SID_F2_PID_CSME17                   0xF244  // CSME CSE PBA (Post BIOS Access)
#define PTL_SID_F2_PID_CSMERTC                  0xF245  // CSME RTC

//
// IP: DFT
//
#define PTL_SID_F2_PID_DFXAGG                   0xF288  // DFx Aggregator
#define PTL_SID_F2_PID_TAM                      0xF289  // Test Access Method
#define PTL_SID_F2_PID_TAP2SB                   0xF28A  // TAP to IOSF-SB
#define PTL_SID_F2_PID_ENDEBUG                  0xF28B  // Encrypted Debug
#define PTL_SID_F2_PID_NPK                      0xF28C  // Northpeak
#define PTL_SID_F2_PID_RTB                      0xF296  // Region Trigger Block
#define PTL_SID_F2_PID_EXI                      0xF28E  // EXI
#define PTL_SID_F2_PID_BPK                      0xF28F  // Baltic Peak
#define PTL_SID_F2_PID_DBG_I3C                  0xF297  // Debug I3C
#define PTL_SID_F2_PID_DFX_GPEP                 0xF28D  // DFX General Purpose Endpoint
#define PTL_SID_F2_PID_PSF_OBS                  0xF295  // IOSF Primary Observer
#define PTL_SID_F2_PID_SB_OBS0                  0xF290  // IOSF-SB Observer - probe pmc
#define PTL_SID_F3_PID_SB_OBS2                  0xF348  // IOSF-SB Observer - probe d2d, sbb
#define PTL_SID_F3_PID_SB_OBS_TCSS              0xF349  // IOSF-SB Observer - tcss

//
// IP: Display
//
#define PTL_SID_F2_PID_DISPIO0PGA               0xF223  // Display IO PGA
#define PTL_SID_F2_PID_DISPIO0PHY               0xF224  // Display IO PHY
#define PTL_SID_F2_PID_PICA                     0xF298  // Display SHIM
#define PTL_SID_F2_PID_P2D                      0xF299  // 2D Display

//
// IP: Digital Thermal Sensor
//
#define PTL_SID_F2_PID_DTS0                     0xF266

//
// IP: ESE
//
#define PTL_SID_F2_PID_ESE_CSE                  0xF258  // ESE CSE
#define PTL_SID_F2_PID_ESE_PBA                  0xF249  // ESE CSE PBA (Post BIOS Access)
#define PTL_SID_F2_PID_ESE_LLEP                 0xF248  // ESE Low Latency Endpoint

//
// IP: FIA
//
#define PTL_SID_F2_PID_FIA_U                    0xF230  // FIA - USB3
#define PTL_SID_F2_PID_FIACPCB_U                0xF231  // FIA CPCB - USB3
#define PTL_SID_F2_PID_FIA_PG                   0xF228  // FIA - PCIe Gen4/GbE
#define PTL_SID_F2_PID_FIACPCB_PG               0xF229  // FIA CPCB - PCIe Gen4/GbE
#define PTL_SID_F2_PID_FIA_X                    0xF22A  // FIA - PCIe Gen4
#define PTL_SID_F2_PID_FIACPCB_X                0xF22B  // FIA CPCB - PCIe Gen4

//
// IP: PHY Gasket USB3
//
#define PTL_SID_F2_PID_U3FPW1A                  0xF218  // USB3 Foundry PHY Wrapper - AIC
#define PTL_SID_F2_PID_U3FPW1P0                 0xF219  // USB3 Foundry PHY Wrapper - PHY

//
// IP: PHY Global Adapter
//
#define PTL_SID_F2_PID_MPFPW1A                  0xF210  // PGA1 AIC
#define PTL_SID_F2_PID_MPFPW1P0                 0xF211  // PGA1 PHY
#define PTL_SID_F2_PID_MPFPW2A                  0xF212  // PGA2 AIC
#define PTL_SID_F2_PID_MPFPW2P0                 0xF213  // PGA2 PHY

//
// IP: Fuse Controller
//
#define PTL_SID_F2_PID_FUSE0                    0xF260  // Fuse Controller 16b port ID domain IOSFSB interface for segment 242
#define PTL_SID_F3_PID_FUSE1                    0xF360  // Fuse Controller 16b port ID domain IOSFSB interface for segment 243
#define PTL_SID_F2_PID_DRNG                     0xF261  // Digital Random Number Generator
#define PTL_SID_F3_PID_FUSETCSS                 0xF38C  // Fuse Controller 8b port ID interface for TCSS
#define PTL_SID_F2_PID_FUSEOSSE                 0xF262  // Dedicated Fuse Controller for OSSE
#define PTL_SID_F2_PID_DRNGOSSE                 0xF263  // DRNG in OSSE dedicated fuse controller

//
// IP: Fuse Proxy
//
#define PTL_SID_F2_PID_FSPROX0                  0xF264

//
// IP: GbE
//
#define PTL_SID_F2_PID_GBE                      0xF20D  // Gigabit Ethernet

//
// IP: GPIO
//
#define PTL_SID_F2_PID_GPCOM0                   0xF259  // GPIO Community Endpoint
#define PTL_SID_F2_PID_GPCOM1                   0xF25A  // GPIO Community Endpoint
#define PTL_SID_F2_PID_GPCOM3                   0xF25B  // GPIO Community Endpoint
#define PTL_SID_F2_PID_GPCOM4                   0xF25C  // GPIO Community Endpoint
#define PTL_SID_F2_PID_GPCOM5                   0xF25D  // GPIO Community Endpoint

//
// IP: ICC
//
#define PTL_SID_F2_PID_ICC_A                    0xF273  // Main ICC
#define PTL_SID_F2_PID_ICC_B                    0xF274  // Debug ICC

//
// IP: iSCLK
//
#define PTL_SID_F2_PID_ICLK                     0xF272  // Integrated System Clock

//
// IP: ISH
//
#define PTL_SID_F2_PID_ISH                      0xF25E  // Integrated Sensor Hub
#define PTL_SID_F2_PID_ISHBR                    0xF25F

//
// IP: LPSS
//
#define PTL_SID_F2_PID_LPSS                     0xF265  // Low Power Sub-System (I2C, I3C, UART, SPI)

//
// IP: LSX
//
#define PTL_SID_F2_PID_TBTLSX                   0xF267

//
// IP: OSSE
//
#define PTL_SID_F2_PID_OSSE_CSE                 0xF24A  // OSSE CSE
#define PTL_SID_F2_PID_OSSE_PBA                 0xF24B  // OSSE CSE PBA (Post BIOS Access)
#define PTL_SID_F2_PID_OSSE_SMT1                0xF24C  // OSSE SMT
#define PTL_SID_F2_PID_OSSE_SMT2                0xF24D  // OSSE SMT
#define PTL_SID_F2_PID_OSSE_SMT3                0xF24E  // OSSE SMT

//
// IP: PCIe
//
#define PTL_SID_F2_PID_PXPA                     0xF201
#define PTL_SID_F2_PID_PXPB                     0xF202

//
// IP: PES
//
#define PTL_SID_F2_PID_ITSS                     0xF269
#define PTL_SID_F2_PID_LPC                      0xF268
#define PTL_SID_F2_PID_P2SB0                    0xF2C7  // Primary to Sideband Bridge 16b port ID domain - segment 242
#define PTL_SID_F2_PID_IEH0                     0xF2C8  // Integrated Error Handler 16b port ID domain - segment 242
#define PTL_SID_F3_PID_P2SB1                    0xF3C7  // Primary to Sideband Bridge 16b port ID domain - segment 243
#define PTL_SID_F3_PID_IEH1                     0xF3C8  // Integrated Error Handle 16b port ID domain - segment 243
#define PTL_SID_F2_PID_PSTH                     0xF26A  // IO trapping and LT registers
#define PTL_SID_F2_PID_SMB                      0xF26B
#define PTL_SID_F2_PID_RTC_HOST                 0xF26C  // Real Time Clock #0 (Host)

//
// IP: PMC
//
#define PTL_SID_F2_PID_PMC                      0xF270
#define PTL_SID_F2_PID_PMCBR                    0xF271

//
// IP: Primary Scalable Fabric
//
#define PTL_SID_F3_PID_PSF0                     0xF3B5
#define PTL_SID_F2_PID_PSF4                     0xF2B0
#define PTL_SID_F2_PID_PSF6                     0xF2B1
#define PTL_SID_F2_PID_PSF8                     0xF2B2
#define PTL_SID_F2_PID_PSF14                    0xF2B3
#define PTL_SID_F2_PID_PSF15                    0xF2B4

//
// IP: SAF
//
#define PTL_SID_F3_PID_IOC                      0xF358
#define PTL_SID_F3_PID_VTU                      0xF359
#define PTL_SID_F3_PID_SCA                      0xF35B
#define PTL_SID_F3_PID_SAF_PMA                  0xF35C
#define PTL_SID_F3_PID_IOCCE                    0xF35D  // Total Storage Encryption
#define PTL_SID_F3_PID_SAF_SRM                  0xF35A

//
// IP: Save/Restore Machine
//
#define PTL_SID_F2_PID_SRM0                     0xF2C0  // dtb0, tc_remap, psf6, psf15, tcss, fia_u, fia_pg. xhci, p2sb0
#define PTL_SID_F2_PID_SRM1                     0xF2C1  // psf8, lpss, thc0, thc1
#define PTL_SID_F2_PID_SRM2                     0xF2C2  // dtb1, psf4, psf5, fia_x
#define PTL_SID_F3_PID_SRM3                     0xF3C0  // dtb2, psf0, d2d_noc, d2d_ipu, d2d_disp, p2sb1
#define PTL_SID_F2_PID_SRM4                     0xF2C3  // csme1/2/12/13, xdci, pxpa
#define PTL_SID_F2_PID_SRM5                     0xF2C4  // pxpb, ufs
#define PTL_SID_F2_PID_SRM6                     0xF2C5  // pxpc, fia_p5x4

//
// IP: SPBC
//
#define PTL_SID_F2_PID_SPBC                     0xF26D  // Secure Platform Boot Controller (SPI/eSPI controller)

//
// IP: Touch Host Controller
//
#define PTL_SID_F2_PID_THC0                     0xF26E  // Touch Host Controller
#define PTL_SID_F2_PID_THC1                     0xF26F  // Touch Host Controller

//
// IP: Type-C SubSystem
//
#define PTL_SID_F3_PID_TC_IOM                   0xF380  // IOMe
#define PTL_SID_F3_PID_TC_XHCI                  0xF381  // XHCI
#define PTL_SID_F3_PID_TC_XDCI                  0xF382  // XDCI
#define PTL_SID_F3_PID_TC_HTI                   0xF383  // HTI
#define PTL_SID_F3_PID_TC_SNF                   0xF384  // TYPC_SNF
#define PTL_SID_F3_PID_TC_SRMROOT               0xF385  // SRFSM_TYPEC_ROOT
#define PTL_SID_F3_PID_TC_PSFTOP                0xF386  // PSF_TCTOP4
#define PTL_SID_F3_PID_TC_PSF                   0xF387  // PSF_TC4
#define PTL_SID_F3_PID_TC_FABPLL                0xF38D  // TCFAB_PLL. Not required. Reserve for IP.
#define PTL_SID_F3_PID_TC_MCAST0                0xF38E
#define PTL_SID_F3_PID_TC_MCAST1                0xF38F
#define PTL_SID_F3_PID_TC_SRM0                  0xF388  // Slice 0
#define PTL_SID_F3_PID_TC_FIA0                  0xF390  // Slice 0
#define PTL_SID_F3_PID_TC_TBT0                  0xF391  // Slice 0
#define PTL_SID_F3_PID_TC_PCIE0                 0xF392  // Slice 0
#define PTL_SID_F3_PID_TC_PCIE1                 0xF393  // Slice 0
#define PTL_SID_F3_PID_TC_PGA0A                 0xF394  // Slice 0
#define PTL_SID_F3_PID_TC_PGA0P                 0xF395  // Slice 0
#define PTL_SID_F3_PID_TC_PGA1A                 0xF396  // Slice 0
#define PTL_SID_F3_PID_TC_PGA1P                 0xF397  // Slice 0
#define PTL_SID_F3_PID_TC_SRM1                  0xF389  // Slice 1
#define PTL_SID_F3_PID_TC_FIA1                  0xF398  // Slice 1
#define PTL_SID_F3_PID_TC_TBT1                  0xF399  // Slice 1
#define PTL_SID_F3_PID_TC_PCIE2                 0xF39A  // Slice 1
#define PTL_SID_F3_PID_TC_PCIE3                 0xF39B  // Slice 1
#define PTL_SID_F3_PID_TC_PGA2A                 0xF39C  // Slice 1
#define PTL_SID_F3_PID_TC_PGA2P                 0xF39D  // Slice 1
#define PTL_SID_F3_PID_TC_PGA3A                 0xF39E  // Slice 1
#define PTL_SID_F3_PID_TC_PGA3P                 0xF39F  // Slice 1

//
// IP: UFS
//
#define PTL_SID_F2_PID_UFS                      0xF20E  // UFS Gen3 2-lane Host Controller
#define PTL_SID_F2_PID_UFSPGA                   0xF220  // UFS PHY Gasket Adaptor
#define PTL_SID_F2_PID_UFSPHY                   0xF221  // UFS PHY SRAM alternate access

//
// IP: USB
//
#define PTL_SID_F2_PID_XHCI                     0xF209  // USB Host Controller
#define PTL_SID_F2_PID_XDCI                     0xF20A  // USB Device Controller
#define PTL_SID_F2_PID_DBC                      0xF20B  // USB Debug Controller
#define PTL_SID_F2_PID_SPR                      0xF20C  // USB Subsystem Port Routing

//
// IP: USB2 Foundry PHY Gasket
//
#define PTL_SID_F2_PID_USB2PHY                  0xF222

//
// IP: Die-to-Die
//
#define PTL_SID_F3_PID_D2D_NOC                  0xF350  // Die-to-Die SIP common block
#define PTL_SID_F3_PID_D2D_DISP                 0xF351  // Die-to-Die SIP common block
#define PTL_SID_F3_PID_D2D_IPU                  0xF352  // Die-to-Die SIP common block

//
// IP: Domain Translation Bridge
//
#define PTL_SID_F2_PID_DTB8B0                   0xF2A0
#define PTL_SID_F2_PID_DTB16B0                  0xF2A0
#define PTL_SID_F2_PID_DTB8B1                   0xF2A1
#define PTL_SID_F2_PID_DTB16B1                  0xF2A1
#define PTL_SID_F3_PID_DTB8B2                   0xF3A0
#define PTL_SID_F3_PID_DTB16B2                  0xF3A0

//
// IP: TCSS Remapper
//
#define PTL_SID_F2_PID_TC_REMAP                 0xF27F  // TCSS remapper DTB register access
#define PTL_SID_F2_PID_TC_IOM_REMAP             0xF280
#define PTL_SID_F2_PID_TC_XHCI_REMAP            0xF281
#define PTL_SID_F2_PID_TC_HTI_REMAP             0xF283
#define PTL_SID_F2_PID_TC_SNF_REMAP             0xF284
#define PTL_SID_F2_PID_TC_TBT0_REMAP            0xF291
#define PTL_SID_F2_PID_TC_PCIE0_REMAP           0xF292
#define PTL_SID_F2_PID_TC_PCIE1_REMAP           0xF293
#define PTL_SID_F2_PID_TC_PCIE2_REMAP           0xF29A
#define PTL_SID_F2_PID_TC_PCIE3_REMAP           0xF29B
#define PTL_SID_F2_PID_TC_REMAP_SPARE0          0xF282
#define PTL_SID_F3_PID_SOC_REMAP                0xF37F
#define PTL_SID_F3_PID_SOC_PMC_REMAP            0xF378
#define PTL_SID_F3_PID_SOC_SRM_REMAP            0xF379

///
/// IOSF Sideband Target Port IDs definition common for PTL_P and PTL_H
///

//
// IP: FIA
//
#define PTL_P_H_SID_F2_PID_FIA_P5X4             0xF22C  // FIA - PCIe Gen5
#define PTL_P_H_SID_F2_PID_FIACPCB_P5X4         0xF22D  // FIA CPCB - PCIe Gen5

//
// IP: PHY Global Adapter
//
#define PTL_P_H_SID_F2_PID_G5FPW1A              0xF214  // G5PGA1 AIC
#define PTL_P_H_SID_F2_PID_G5FPW1P0             0xF215  // G5PGA1 PHY

//
// IP: PCIe
//
#define PTL_P_H_SID_F2_PID_PXPC                 0xF203

//
// IP: PG5_PMA
//
#define PTL_P_H_SID_F2_PID_PG5_PMA0             0xF204

///
/// IOSF Sideband Target Port IDs definition specific for PTL_P
///

///
/// IOSF Sideband Target Port IDs definition specific for PTL_H
///

//
// IP: FIA
//
#define PTL_H_SID_F2_PID_FIA_P5X8               0xF22E  // FIA - PCIe Gen5 (8 lanes)
#define PTL_H_SID_F2_PID_FIACPCB_P5X8           0xF22F  // FIA CPCB - PCIe Gen5 (8 lanes)

//
// IP: PHY Global Adapter
//
#define PTL_H_SID_F2_PID_G5FPW2A                0xF21C  // G5PGA2 AIC
#define PTL_H_SID_F2_PID_G5FPW2P0               0xF21D  // G5PGA2 PHY (4-lane)
#define PTL_H_SID_F2_PID_G5FPW2P1               0xF21E  // G5PGA2 PHY (4-lane)
#define PTL_H_SID_F2_PID_G5FPW2P01              0xF21F  // G5PGA2 PHY (local broadcast)

//
// IP: PCIe
//
#define PTL_H_SID_F2_PID_PXPD                   0xF205
#define PTL_H_SID_F2_PID_PXPE                   0xF206

//
// IP: PG5_PMA
//
#define PTL_H_SID_F2_PID_PG5_PMA1               0xF207

//
// IP: Primary Scalable Fabric
//
#define PTL_H_SID_F2_PID_PSF5                   0xF2B6

//
// IP: Save/Restore Machine
//
#define PTL_H_SID_F2_PID_SRM7                   0xF2C6  // pxpd, pxpe, fia_p5x8

///
/// IOSF Sideband Target Port IDs definition specific for PTL_A
///

//
// IP: FIA
//
#define PTL_A_SID_F2_PID_FIA_P5X8               0xF22E  // FIA - PCIe Gen5 (8 lanes)
#define PTL_A_SID_F2_PID_FIACPCB_P5X8           0xF22F  // FIA CPCB - PCIe Gen5 (8 lanes)

//
// IP: PHY Global Adapter
//
#define PTL_A_SID_F2_PID_G5FPW2A                0xF21C  // G5PGA2 AIC
#define PTL_A_SID_F2_PID_G5FPW2P0               0xF21D  // G5PGA2 PHY (4-lane)
#define PTL_A_SID_F2_PID_G5FPW2P1               0xF21E  // G5PGA2 PHY (4-lane)
#define PTL_A_SID_F2_PID_G5FPW2P01              0xF21F  // G5PGA2 PHY (local broadcast)
#define PTL_A_SID_F2_PID_TSNFPW1A               0xF214
#define PTL_A_SID_F2_PID_TSNFPW1P0              0xF215

//
// IP: TSN
//
#define PTL_A_SID_F2_PID_TSN0                   0xF203
#define PTL_A_SID_F2_PID_TSN1                   0xF204
#define PTL_A_SID_F2_PID_TSN2                   0xF22C
#define PTL_A_SID_F2_PID_TSN3                   0xF22D

//
// IP: GPIO
//
#define PTL_A_SID_F2_PID_GPCOM6                 0xF278  // GPIO Community Endpoint

//
// IP: PCIe
//
#define PTL_A_SID_F2_PID_PXPD                   0xF205
#define PTL_A_SID_F2_PID_PXPE                   0xF206

//
// IP: PG5_PMA
//
#define PTL_A_SID_F2_PID_PG5_PMA1               0xF207

//
// IP: Primary Scalable Fabric
//
#define PTL_A_SID_F2_PID_PSF5                   0xF2B6

//
// IP: Save/Restore Machine
//
#define PTL_A_SID_F2_PID_SRM7                   0xF2C6  // pxpd, pxpe, fia_p5x8

//
// IP: CAN FD
//
#define PTL_A_SID_F2_PID_CANFD                  0xF279

#endif //_PTL_PCD_SB_PORT_IDS_H_
