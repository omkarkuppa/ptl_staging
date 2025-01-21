/** @file
  Auto-generated register address map table.

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

#include <Library/RegisterAddressMapLib.h>

///
/// OsXml Release: mtl-m_a0-21ww46.a
///
UINT8 gSocAddressMapTable[] = {
  0x6d, 0x74, 0x6c, 0x73, 0x6f, 0x63, 0x2d, 0x6d, // HW ID: mtlsoc-m
  0x34, 0x36, 0x2e, 0x61, // Release: ww46.a
  0x52, 0x06, 0x00, 0x00, // Length: 1618
  0xc3, 0x1d, 0x8d, 0x5f, // Checksum: 0x5f8d1dc3
  0x01, 0x02,             // Version: 2.1
  0x25, 0x00,             // Endpoint Count: 37

  //
  // BARs[18]
  //
  0x12, 0x00,             // BAR Count: 18
  // BAR_0
    // SCF_BAR
      0xff,                                           //  Type: n/a
      0x0c,                                           // Shift: 12
      0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000000007ff
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // MMIO_BASE
      0xff,                                           //  Type: n/a
      0x17,                                           // Shift: 23
      0xff, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x000000001fffffff
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_1
    // PCU_BAR
      0xff,                                           //  Type: n/a
      0x0c,                                           // Shift: 12
      0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000000007ff
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // MMIO_BASE
      0xff,                                           //  Type: n/a
      0x17,                                           // Shift: 23
      0xff, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x000000001fffffff
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_2
    // MEM_BAR0
      0xff,                                           //  Type: n/a
      0x00,                                           // Shift: 0
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x0000000000000000
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_3
    // MEM_BAR1
      0xff,                                           //  Type: n/a
      0x00,                                           // Shift: 0
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x0000000000000000
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_4
    // MEM_BAR2
      0xff,                                           //  Type: n/a
      0x00,                                           // Shift: 0
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x0000000000000000
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_5
    // MEM_BAR3
      0xff,                                           //  Type: n/a
      0x00,                                           // Shift: 0
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x0000000000000000
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_6
    // MEM_BAR4
      0xff,                                           //  Type: n/a
      0x00,                                           // Shift: 0
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x0000000000000000
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_7
    // MEM_BAR5
      0xff,                                           //  Type: n/a
      0x00,                                           // Shift: 0
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x0000000000000000
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_8
    // MEM_BAR6
      0xff,                                           //  Type: n/a
      0x00,                                           // Shift: 0
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x0000000000000000
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_9
    // MEM_BAR7
      0xff,                                           //  Type: n/a
      0x00,                                           // Shift: 0
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x0000000000000000
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_10
    // SBREG_BAR
      0x02,                                           //  Type: PciCfg
      0x00,                                           // Shift: 0
      0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0xff000000
      0x10, 0x90, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, // B/D/F: 0/31/1: 0x10
      // N/A
      0x02,                                           //  Type: PciCfg
      0x20,                                           // Shift: 32
      0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0xffffffff
      0x14, 0x90, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, // B/D/F: 0/31/1: 0x14
  // BAR_11
    // SBREG_PTE0_N0
      0xff,                                           //  Type: n/a
      0x00,                                           // Shift: 0
      0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000ffffffff
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_12
    // SBREG_PTE0_N1
      0xff,                                           //  Type: n/a
      0x00,                                           // Shift: 0
      0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000ffffffff
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_13
    // socs/NPK/northpeak_top_map/file_ttc_pci_cfg_file_ttc_pci_msg/MTB_LBAR
      0x02,                                           //  Type: PciCfg
      0x00,                                           // Shift: 0
      0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000fff00000
      0x10, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, // B/D/F: 0/31/7: 0x10
    // socs/NPK/northpeak_top_map/file_ttc_pci_cfg_file_ttc_pci_msg/MTB_UBAR
      0x02,                                           //  Type: PciCfg
      0x20,                                           // Shift: 32
      0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000ffffffff
      0x14, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, // B/D/F: 0/31/7: 0x14
  // BAR_14
    // socs/PMC/pmc/pci_cfg_ssram/pci_cfg_ssram_pci_cfg_ssram_sb/BAR
      0x02,                                           //  Type: PciCfg
      0x00,                                           // Shift: 0
      0xf0, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000fffffff0
      0x10, 0x20, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, // B/D/F: 0/20/2: 0x10
    // socs/PMC/pmc/pci_cfg_ssram/pci_cfg_ssram_pci_cfg_ssram_sb/BAR_HIGH
      0x02,                                           //  Type: PciCfg
      0x20,                                           // Shift: 32
      0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000ffffffff
      0x14, 0x20, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, // B/D/F: 0/20/2: 0x14
  // BAR_15
    // socs/PMC/pmc/pci_cfg/pci_cfg_pci_cfg_sb/BAR
      0x02,                                           //  Type: PciCfg
      0x00,                                           // Shift: 0
      0xf0, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000fffffff0
      0x10, 0xa0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, // B/D/F: 0/31/2: 0x10
    // socs/PMC/pmc/pci_cfg/pci_cfg_pci_cfg_sb/BAR_HIGH
      0x02,                                           //  Type: PciCfg
      0x20,                                           // Shift: 32
      0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000ffffffff
      0x14, 0xa0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, // B/D/F: 0/31/2: 0x14
  // BAR_16
    // socs/SATA_CONTR/sata_pci_config_bridge/sata_pci_config_regs_sata_pci_config_regs_sb/ABAR
      0x02,                                           //  Type: PciCfg
      0x00,                                           // Shift: 0
      0x00, 0xf8, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //  Mask: 0x00000000fffff800
      0x24, 0x80, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, // B/D/F: 0/23/0: 0x24
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a
  // BAR_17
    // socs/USB3_HOST/usbx_bridge/usbx_pci_config_registers_usbx_pci_config_registers_msg/MBAR
      0x02,                                           //  Type: PciCfg
      0x00,                                           // Shift: 0
      0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: 0xffffffffffff0000
      0x10, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, // B/D/F: 0/20/0: 0x10
    // N/A
      0xff,                                           //  Type: n/a
      0xff,                                           // Shift: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  Mask: n/a
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // n/a


  //
  // Endpoint ID: 0x58d09a00a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x00, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x02, 0x00, 0x01, 0x00, //   Cfg[1]
  0x06, 0x00, 0x01, 0x00, //   MsgCfg[1]
    //
    // P2SBCFG:IOSF_Primary.P2SBCFG_REG
    //
      // socs/P2SB/P2SBCFG
      0x00, 0x90, 0x0f, 0x00, // 0/31/1:0
    //
    // P2SBCFG:IOSF_SideBand.P2SBMSG_REG
    //
      // socs/P2SB/P2SBCFG
      0xc5, 0x00,    // PortId: 197
        0xf9, 0x00,  //  SbFid: 249 SbBar: n/a

  //
  // Endpoint ID: 0x58d09a02a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x02, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x02, 0x00, 0x01, 0x00, //   Cfg[1]
  0x06, 0x00, 0x01, 0x00, //   MsgCfg[1]
    //
    // EBGSIEHREG:IOSF_Primary.cfg_inst
    //
      // socs/P2SB/EBGSIEHREG
      0x00, 0x50, 0x0a, 0x00, // 0/20/5:0
    //
    // EBGSIEHREG:IOSF_SideBand.cfg_inst_sb
    //
      // socs/P2SB/EBGSIEHREG
      0xc2, 0x00,    // PortId: 194
        0xa5, 0x00,  //  SbFid: 165 SbBar: n/a

  //
  // Endpoint ID: 0x58d09a04a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x04, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x02, 0x00, 0x01, 0x00, //   Cfg[1]
  0x06, 0x00, 0x01, 0x00, //   MsgCfg[1]
    //
    // pci_cfg:IOSF_Primary.pci_cfg
    //
      // socs/PMC/pmc/pci_cfg
      0x00, 0xa0, 0x0f, 0x00, // 0/31/2:0
    //
    // pci_cfg:IOSF_SideBand.pci_cfg_sb
    //
      // socs/PMC/pmc/pci_cfg
      0xcc, 0x00,    // PortId: 204
        0xfa, 0x00,  //  SbFid: 250 SbBar: n/a

  //
  // Endpoint ID: 0x58d09a06a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x06, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // pci_mmr:IOSF_Primary.pci_mmr
    //
      // socs/PMC/pmc/pci_mmr
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x0f, 0x00,             // BAR_15
    //
    // pci_mmr:IOSF_SideBand.pci_mmr_sb
    //
      // socs/PMC/pmc/pci_mmr
      0xcc, 0x00,    // PortId: 204
        0xfa, 0x00,  //  SbFid: 250 SbBar: 0

  //
  // Endpoint ID: 0x58d09a08a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x08, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x02, 0x00, 0x01, 0x00, //   Cfg[1]
  0x06, 0x00, 0x01, 0x00, //   MsgCfg[1]
    //
    // pci_cfg_ssram:IOSF_Primary.pci_cfg_ssram
    //
      // socs/PMC/pmc/pci_cfg_ssram
      0x00, 0x20, 0x0a, 0x00, // 0/20/2:0
    //
    // pci_cfg_ssram:IOSF_SideBand.pci_cfg_ssram_sb
    //
      // socs/PMC/pmc/pci_cfg_ssram
      0x65, 0x00,    // PortId: 101
        0xa2, 0x00,  //  SbFid: 162 SbBar: n/a

  //
  // Endpoint ID: 0x58d09a0aa2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x0a, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // pmc_ssram_ram:IOSF_Primary.pmc_ssram_ram
    //
      // socs/PMC/pmc/pmc_ssram_ram
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x0e, 0x00,             // BAR_14
    //
    // pmc_ssram_ram:IOSF_SideBand.pmc_ssram_ram_sb
    //
      // socs/PMC/pmc/pmc_ssram_ram
      0x65, 0x00,    // PortId: 101
        0xa2, 0x00,  //  SbFid: 162 SbBar: 0

  //
  // Endpoint ID: 0x58d09a0ca2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x0c, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // gpio_mem_0:IOSF_SideBand.SB
    //
      // socs/GPIO_0
      0x6e, 0x00,    // PortId: 110
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a0da2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x0d, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // gpio_mem_1:IOSF_SideBand.SB
    //
      // socs/GPIO_1
      0x6d, 0x00,    // PortId: 109
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a0ea2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x0e, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // gpio_mem_3:IOSF_SideBand.SB
    //
      // socs/GPIO_3
      0x6b, 0x00,    // PortId: 107
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a0fa2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x0f, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // gpio_mem_4:IOSF_SideBand.SB
    //
      // socs/GPIO_4
      0x6a, 0x00,    // PortId: 106
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a10a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x10, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // gpio_mem_5:IOSF_SideBand.SB
    //
      // socs/GPIO_5
      0x69, 0x00,    // PortId: 105
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a11a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x11, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // exi:IOSF_SideBand.map_exi_sb_pvt_cfg_file_i
    //
      // socs/EXI/exi
      0xb8, 0x00,    // PortId: 184
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a12a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x12, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // psf_1_pch:IOSF_SideBand.psf_1_pch
    //
      // socs/PSF/psf_1_pch
      0xba, 0x00,    // PortId: 186
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a13a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x13, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // psf_2_pch:IOSF_SideBand.psf_2_pch
    //
      // socs/PSF/psf_2_pch
      0xbb, 0x00,    // PortId: 187
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a14a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x14, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // psf_3_pch:IOSF_SideBand.psf_3_pch
    //
      // socs/PSF/psf_3_pch
      0xbc, 0x00,    // PortId: 188
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a15a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x15, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // psf_4_pch:IOSF_SideBand.psf_4_pch
    //
      // socs/PSF/psf_4_pch
      0xbd, 0x00,    // PortId: 189
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a16a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x16, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // psf_6_pch:IOSF_SideBand.psf_6_pch
    //
      // socs/PSF/psf_6_pch
      0x7f, 0x00,    // PortId: 127
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a17a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x17, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // psf_7_pch:IOSF_SideBand.psf_7_pch
    //
      // socs/PSF/psf_7_pch
      0x7e, 0x00,    // PortId: 126
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a18a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x18, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x02, 0x00, 0x01, 0x00, //   Cfg[1]
  0x06, 0x00, 0x01, 0x00, //   MsgCfg[1]
    //
    // northpeak_top_map:IOSF_Primary.file_ttc_pci_cfg
    //
      // socs/NPK/northpeak_top_map
      0x00, 0xf0, 0x0f, 0x00, // 0/31/7:0
    //
    // northpeak_top_map:IOSF_SideBand.file_ttc_pci_msg
    //
      // socs/NPK/northpeak_top_map
      0xb6, 0x00,    // PortId: 182
        0xff, 0x00,  //  SbFid: 255 SbBar: n/a

  //
  // Endpoint ID: 0x58d09a1aa2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x1a, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // northpeak_top_map:IOSF_Primary.file_ttc_mmio
    //
      // socs/NPK/northpeak_top_map
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x0d, 0x00,             // BAR_13
    //
    // northpeak_top_map:IOSF_SideBand.file_ttc_mmio_msg
    //
      // socs/NPK/northpeak_top_map
      0xb6, 0x00,    // PortId: 182
        0xff, 0x00,  //  SbFid: 255 SbBar: 0

  //
  // Endpoint ID: 0x58d09a1ca2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x1c, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // northpeak_top_map:IOSF_Primary.file_tph
    //
      // socs/NPK/northpeak_top_map
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x0d, 0x00,             // BAR_13
    //
    // northpeak_top_map:IOSF_SideBand.file_tph_msg
    //
      // socs/NPK/northpeak_top_map
      0xb6, 0x00,    // PortId: 182
        0xff, 0x00,  //  SbFid: 255 SbBar: 0

  //
  // Endpoint ID: 0x58d09a1ea2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x1e, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // file_map_dfxagg_CFG:IOSF_SideBand.map_dfxagg
    //
      // socs/DFX_AGG/file_map_dfxagg_CFG
      0xb7, 0x00,    // PortId: 183
        0x00, 0x00,  //  SbFid: n/a SbBar: n/a

  //
  // Endpoint ID: 0x58d09a1fa2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x1f, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x02, 0x00, 0x01, 0x00, //   Cfg[1]
  0x06, 0x00, 0x01, 0x00, //   MsgCfg[1]
    //
    // sata_pci_config_bridge:IOSF_Primary.sata_pci_config_regs
    //
      // socs/SATA_CONTR/sata_pci_config_bridge
      0x00, 0x80, 0x0b, 0x00, // 0/23/0:0
    //
    // sata_pci_config_bridge:IOSF_SideBand.sata_pci_config_regs_sb
    //
      // socs/SATA_CONTR/sata_pci_config_bridge
      0xd9, 0x00,    // PortId: 217
        0xb8, 0x00,  //  SbFid: 184 SbBar: n/a

  //
  // Endpoint ID: 0x58d09a21a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x21, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // sata_mem_abar_bridge:IOSF_Primary.sata_mem_abar_regs
    //
      // socs/SATA_CONTR/sata_mem_abar_bridge
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x10, 0x00,             // BAR_16
    //
    // sata_mem_abar_bridge:IOSF_SideBand.sata_mem_abar_regs_sb
    //
      // socs/SATA_CONTR/sata_mem_abar_bridge
      0xd9, 0x00,    // PortId: 217
        0xb8, 0x05,  //  SbFid: 1464 SbBar: 0

  //
  // Endpoint ID: 0x58d09a23a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x23, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // sata_mem_port0_bridge:IOSF_Primary.sata_mem_port0_regs
    //
      // socs/SATA_CONTR/sata_mem_port0_bridge
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x10, 0x00,             // BAR_16
    //
    // sata_mem_port0_bridge:IOSF_SideBand.sata_mem_port0_regs_sb
    //
      // socs/SATA_CONTR/sata_mem_port0_bridge
      0xd9, 0x00,    // PortId: 217
        0xb8, 0x05,  //  SbFid: 1464 SbBar: 0

  //
  // Endpoint ID: 0x58d09a25a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x25, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // sata_mem_port1_bridge:IOSF_Primary.sata_mem_port1_regs
    //
      // socs/SATA_CONTR/sata_mem_port1_bridge
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x10, 0x00,             // BAR_16
    //
    // sata_mem_port1_bridge:IOSF_SideBand.sata_mem_port1_regs_sb
    //
      // socs/SATA_CONTR/sata_mem_port1_bridge
      0xd9, 0x00,    // PortId: 217
        0xb8, 0x05,  //  SbFid: 1464 SbBar: 0

  //
  // Endpoint ID: 0x58d09a27a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x27, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // sata_mem_port2_bridge:IOSF_Primary.sata_mem_port2_regs
    //
      // socs/SATA_CONTR/sata_mem_port2_bridge
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x10, 0x00,             // BAR_16
    //
    // sata_mem_port2_bridge:IOSF_SideBand.sata_mem_port2_regs_sb
    //
      // socs/SATA_CONTR/sata_mem_port2_bridge
      0xd9, 0x00,    // PortId: 217
        0xb8, 0x05,  //  SbFid: 1464 SbBar: 0

  //
  // Endpoint ID: 0x58d09a29a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x29, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // sata_mem_port3_bridge:IOSF_Primary.sata_mem_port3_regs
    //
      // socs/SATA_CONTR/sata_mem_port3_bridge
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x10, 0x00,             // BAR_16
    //
    // sata_mem_port3_bridge:IOSF_SideBand.sata_mem_port3_regs_sb
    //
      // socs/SATA_CONTR/sata_mem_port3_bridge
      0xd9, 0x00,    // PortId: 217
        0xb8, 0x05,  //  SbFid: 1464 SbBar: 0

  //
  // Endpoint ID: 0x58d09a2ba2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x2b, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // sata_mem_port4_bridge:IOSF_Primary.sata_mem_port4_regs
    //
      // socs/SATA_CONTR/sata_mem_port4_bridge
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x10, 0x00,             // BAR_16
    //
    // sata_mem_port4_bridge:IOSF_SideBand.sata_mem_port4_regs_sb
    //
      // socs/SATA_CONTR/sata_mem_port4_bridge
      0xd9, 0x00,    // PortId: 217
        0xb8, 0x05,  //  SbFid: 1464 SbBar: 0

  //
  // Endpoint ID: 0x58d09a2da2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x2d, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // sata_mem_port5_bridge:IOSF_Primary.sata_mem_port5_regs
    //
      // socs/SATA_CONTR/sata_mem_port5_bridge
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x10, 0x00,             // BAR_16
    //
    // sata_mem_port5_bridge:IOSF_SideBand.sata_mem_port5_regs_sb
    //
      // socs/SATA_CONTR/sata_mem_port5_bridge
      0xd9, 0x00,    // PortId: 217
        0xb8, 0x05,  //  SbFid: 1464 SbBar: 0

  //
  // Endpoint ID: 0x58d09a2fa2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x2f, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // sata_mem_port6_bridge:IOSF_Primary.sata_mem_port6_regs
    //
      // socs/SATA_CONTR/sata_mem_port6_bridge
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x10, 0x00,             // BAR_16
    //
    // sata_mem_port6_bridge:IOSF_SideBand.sata_mem_port6_regs_sb
    //
      // socs/SATA_CONTR/sata_mem_port6_bridge
      0xd9, 0x00,    // PortId: 217
        0xb8, 0x05,  //  SbFid: 1464 SbBar: 0

  //
  // Endpoint ID: 0x58d09a31a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x31, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // sata_mem_port7_bridge:IOSF_Primary.sata_mem_port7_regs
    //
      // socs/SATA_CONTR/sata_mem_port7_bridge
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x10, 0x00,             // BAR_16
    //
    // sata_mem_port7_bridge:IOSF_SideBand.sata_mem_port7_regs_sb
    //
      // socs/SATA_CONTR/sata_mem_port7_bridge
      0xd9, 0x00,    // PortId: 217
        0xb8, 0x05,  //  SbFid: 1464 SbBar: 0

  //
  // Endpoint ID: 0x58d09a33a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x33, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x02, 0x00, 0x01, 0x00, //   Cfg[1]
  0x06, 0x00, 0x01, 0x00, //   MsgCfg[1]
    //
    // usbx_bridge:IOSF_Primary.usbx_pci_config_registers
    //
      // socs/USB3_HOST/usbx_bridge
      0x00, 0x00, 0x0a, 0x00, // 0/20/0:0
    //
    // usbx_bridge:IOSF_SideBand.usbx_pci_config_registers_msg
    //
      // socs/USB3_HOST/usbx_bridge
      0x70, 0x00,    // PortId: 112
        0xa0, 0x00,  //  SbFid: 160 SbBar: n/a

  //
  // Endpoint ID: 0x58d09a35a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x35, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x03, 0x00, 0x01, 0x00, //   MemBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // usbx_bridge:IOSF_Primary.usbx_mmio_registers
    //
      // socs/USB3_HOST/usbx_bridge
      0x00, 0x00, 0x00, 0x00, // Offset: 0x00000000
      0x11, 0x00,             // BAR_17
    //
    // usbx_bridge:IOSF_SideBand.usbx_mmio_registers_msg
    //
      // socs/USB3_HOST/usbx_bridge
      0x70, 0x00,    // PortId: 112
        0xa0, 0x00,  //  SbFid: 160 SbBar: 0

  //
  // Endpoint ID: 0x58d09a37a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x37, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // usbx_bridge:IOSF_SideBand.usbx_dap_private_config_registers
    //
      // socs/USB3_HOST/usbx_bridge
      0x70, 0x00,    // PortId: 112
        0xa0, 0x00,  //  SbFid: 160 SbBar: n/a

  //
  // Endpoint ID: 0x58d09a38a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x38, 0x9a, 0xd0, 0x58,
  0x01, 0x00, 0x00, 0x00, // Accessor count: 1
  0x09, 0x00, 0x01, 0x00, //   MsgCr[1]
    //
    // usbx_exi_on_dbc_bridge:IOSF_SideBand.usbx_exi_private_config_registers
    //
      // socs/USB3_EXI_DBC/usbx_exi_on_dbc_bridge
      0x70, 0x00,    // PortId: 112
        0xa0, 0x00,  //  SbFid: 160 SbBar: n/a

  //
  // Endpoint ID: 0x58d09a39a2e04dea
  //
  0xea, 0x4d, 0xe0, 0xa2, 0x39, 0x9a, 0xd0, 0x58,
  0x02, 0x00, 0x00, 0x00, // Accessor count: 2
  0x00, 0x00, 0x01, 0x00, //   MemNoBar[1]
  0x07, 0x00, 0x01, 0x00, //   MsgMem[1]
    //
    // usbx_exi_on_dbc_bridge:IOSF_Primary.usbx_exi_on_dbc_registers_primary
    //
      // socs/USB3_EXI_DBC/usbx_exi_on_dbc_bridge
        0x00, 0x00, 0x00, 0x00, // Base: 0x00000000
    //
    // usbx_exi_on_dbc_bridge:IOSF_SideBand.usbx_exi_on_dbc_registers_sb
    //
      // socs/USB3_EXI_DBC/usbx_exi_on_dbc_bridge
      0x70, 0x00,    // PortId: 112
        0xa0, 0x05,  //  SbFid: 1440 SbBar: 0
};

//
// String table for trace purpose.
//
CHAR8 *gSocAddressMapStringTable[] = {
  //
  // Endpoint ID: 0x58d09a00a2e04dea
  //
  // Accessor types: 2
  // P2SBCFG:IOSF_Primary.P2SBCFG_REG
  //   Cfg[1]
      "P2SB/P2SBCFG",
  // P2SBCFG:IOSF_SideBand.P2SBMSG_REG
  //   MsgCfg[1]
      "P2SB/P2SBCFG",
  //
  // Endpoint ID: 0x58d09a02a2e04dea
  //
  // Accessor types: 2
  // EBGSIEHREG:IOSF_Primary.cfg_inst
  //   Cfg[1]
      "P2SB/EBGSIEHREG",
  // EBGSIEHREG:IOSF_SideBand.cfg_inst_sb
  //   MsgCfg[1]
      "P2SB/EBGSIEHREG",
  //
  // Endpoint ID: 0x58d09a04a2e04dea
  //
  // Accessor types: 2
  // pci_cfg:IOSF_Primary.pci_cfg
  //   Cfg[1]
      "PMC/pmc/pci_cfg",
  // pci_cfg:IOSF_SideBand.pci_cfg_sb
  //   MsgCfg[1]
      "PMC/pmc/pci_cfg",
  //
  // Endpoint ID: 0x58d09a06a2e04dea
  //
  // Accessor types: 2
  // pci_mmr:IOSF_Primary.pci_mmr
  //   MemBar[1]
      "PMC/pmc/pci_mmr",
  // pci_mmr:IOSF_SideBand.pci_mmr_sb
  //   MsgMem[1]
      "PMC/pmc/pci_mmr",
  //
  // Endpoint ID: 0x58d09a08a2e04dea
  //
  // Accessor types: 2
  // pci_cfg_ssram:IOSF_Primary.pci_cfg_ssram
  //   Cfg[1]
      "PMC/pmc/pci_cfg_ssram",
  // pci_cfg_ssram:IOSF_SideBand.pci_cfg_ssram_sb
  //   MsgCfg[1]
      "PMC/pmc/pci_cfg_ssram",
  //
  // Endpoint ID: 0x58d09a0aa2e04dea
  //
  // Accessor types: 2
  // pmc_ssram_ram:IOSF_Primary.pmc_ssram_ram
  //   MemBar[1]
      "PMC/pmc/pmc_ssram_ram",
  // pmc_ssram_ram:IOSF_SideBand.pmc_ssram_ram_sb
  //   MsgMem[1]
      "PMC/pmc/pmc_ssram_ram",
  //
  // Endpoint ID: 0x58d09a0ca2e04dea
  //
  // Accessor types: 1
  // gpio_mem_0:IOSF_SideBand.SB
  //   MsgCr[1]
      "GPIO_0",
  //
  // Endpoint ID: 0x58d09a0da2e04dea
  //
  // Accessor types: 1
  // gpio_mem_1:IOSF_SideBand.SB
  //   MsgCr[1]
      "GPIO_1",
  //
  // Endpoint ID: 0x58d09a0ea2e04dea
  //
  // Accessor types: 1
  // gpio_mem_3:IOSF_SideBand.SB
  //   MsgCr[1]
      "GPIO_3",
  //
  // Endpoint ID: 0x58d09a0fa2e04dea
  //
  // Accessor types: 1
  // gpio_mem_4:IOSF_SideBand.SB
  //   MsgCr[1]
      "GPIO_4",
  //
  // Endpoint ID: 0x58d09a10a2e04dea
  //
  // Accessor types: 1
  // gpio_mem_5:IOSF_SideBand.SB
  //   MsgCr[1]
      "GPIO_5",
  //
  // Endpoint ID: 0x58d09a11a2e04dea
  //
  // Accessor types: 1
  // exi:IOSF_SideBand.map_exi_sb_pvt_cfg_file_i
  //   MsgCr[1]
      "EXI/exi",
  //
  // Endpoint ID: 0x58d09a12a2e04dea
  //
  // Accessor types: 1
  // psf_1_pch:IOSF_SideBand.psf_1_pch
  //   MsgCr[1]
      "PSF/psf_1_pch",
  //
  // Endpoint ID: 0x58d09a13a2e04dea
  //
  // Accessor types: 1
  // psf_2_pch:IOSF_SideBand.psf_2_pch
  //   MsgCr[1]
      "PSF/psf_2_pch",
  //
  // Endpoint ID: 0x58d09a14a2e04dea
  //
  // Accessor types: 1
  // psf_3_pch:IOSF_SideBand.psf_3_pch
  //   MsgCr[1]
      "PSF/psf_3_pch",
  //
  // Endpoint ID: 0x58d09a15a2e04dea
  //
  // Accessor types: 1
  // psf_4_pch:IOSF_SideBand.psf_4_pch
  //   MsgCr[1]
      "PSF/psf_4_pch",
  //
  // Endpoint ID: 0x58d09a16a2e04dea
  //
  // Accessor types: 1
  // psf_6_pch:IOSF_SideBand.psf_6_pch
  //   MsgCr[1]
      "PSF/psf_6_pch",
  //
  // Endpoint ID: 0x58d09a17a2e04dea
  //
  // Accessor types: 1
  // psf_7_pch:IOSF_SideBand.psf_7_pch
  //   MsgCr[1]
      "PSF/psf_7_pch",
  //
  // Endpoint ID: 0x58d09a18a2e04dea
  //
  // Accessor types: 2
  // northpeak_top_map:IOSF_Primary.file_ttc_pci_cfg
  //   Cfg[1]
      "NPK/northpeak_top_map",
  // northpeak_top_map:IOSF_SideBand.file_ttc_pci_msg
  //   MsgCfg[1]
      "NPK/northpeak_top_map",
  //
  // Endpoint ID: 0x58d09a1aa2e04dea
  //
  // Accessor types: 2
  // northpeak_top_map:IOSF_Primary.file_ttc_mmio
  //   MemBar[1]
      "NPK/northpeak_top_map",
  // northpeak_top_map:IOSF_SideBand.file_ttc_mmio_msg
  //   MsgMem[1]
      "NPK/northpeak_top_map",
  //
  // Endpoint ID: 0x58d09a1ca2e04dea
  //
  // Accessor types: 2
  // northpeak_top_map:IOSF_Primary.file_tph
  //   MemBar[1]
      "NPK/northpeak_top_map",
  // northpeak_top_map:IOSF_SideBand.file_tph_msg
  //   MsgMem[1]
      "NPK/northpeak_top_map",
  //
  // Endpoint ID: 0x58d09a1ea2e04dea
  //
  // Accessor types: 1
  // file_map_dfxagg_CFG:IOSF_SideBand.map_dfxagg
  //   MsgCr[1]
      "DFX_AGG/file_map_dfxagg_CFG",
  //
  // Endpoint ID: 0x58d09a1fa2e04dea
  //
  // Accessor types: 2
  // sata_pci_config_bridge:IOSF_Primary.sata_pci_config_regs
  //   Cfg[1]
      "SATA_CONTR/sata_pci_config_bridge",
  // sata_pci_config_bridge:IOSF_SideBand.sata_pci_config_regs_sb
  //   MsgCfg[1]
      "SATA_CONTR/sata_pci_config_bridge",
  //
  // Endpoint ID: 0x58d09a21a2e04dea
  //
  // Accessor types: 2
  // sata_mem_abar_bridge:IOSF_Primary.sata_mem_abar_regs
  //   MemBar[1]
      "SATA_CONTR/sata_mem_abar_bridge",
  // sata_mem_abar_bridge:IOSF_SideBand.sata_mem_abar_regs_sb
  //   MsgMem[1]
      "SATA_CONTR/sata_mem_abar_bridge",
  //
  // Endpoint ID: 0x58d09a23a2e04dea
  //
  // Accessor types: 2
  // sata_mem_port0_bridge:IOSF_Primary.sata_mem_port0_regs
  //   MemBar[1]
      "SATA_CONTR/sata_mem_port0_bridge",
  // sata_mem_port0_bridge:IOSF_SideBand.sata_mem_port0_regs_sb
  //   MsgMem[1]
      "SATA_CONTR/sata_mem_port0_bridge",
  //
  // Endpoint ID: 0x58d09a25a2e04dea
  //
  // Accessor types: 2
  // sata_mem_port1_bridge:IOSF_Primary.sata_mem_port1_regs
  //   MemBar[1]
      "SATA_CONTR/sata_mem_port1_bridge",
  // sata_mem_port1_bridge:IOSF_SideBand.sata_mem_port1_regs_sb
  //   MsgMem[1]
      "SATA_CONTR/sata_mem_port1_bridge",
  //
  // Endpoint ID: 0x58d09a27a2e04dea
  //
  // Accessor types: 2
  // sata_mem_port2_bridge:IOSF_Primary.sata_mem_port2_regs
  //   MemBar[1]
      "SATA_CONTR/sata_mem_port2_bridge",
  // sata_mem_port2_bridge:IOSF_SideBand.sata_mem_port2_regs_sb
  //   MsgMem[1]
      "SATA_CONTR/sata_mem_port2_bridge",
  //
  // Endpoint ID: 0x58d09a29a2e04dea
  //
  // Accessor types: 2
  // sata_mem_port3_bridge:IOSF_Primary.sata_mem_port3_regs
  //   MemBar[1]
      "SATA_CONTR/sata_mem_port3_bridge",
  // sata_mem_port3_bridge:IOSF_SideBand.sata_mem_port3_regs_sb
  //   MsgMem[1]
      "SATA_CONTR/sata_mem_port3_bridge",
  //
  // Endpoint ID: 0x58d09a2ba2e04dea
  //
  // Accessor types: 2
  // sata_mem_port4_bridge:IOSF_Primary.sata_mem_port4_regs
  //   MemBar[1]
      "SATA_CONTR/sata_mem_port4_bridge",
  // sata_mem_port4_bridge:IOSF_SideBand.sata_mem_port4_regs_sb
  //   MsgMem[1]
      "SATA_CONTR/sata_mem_port4_bridge",
  //
  // Endpoint ID: 0x58d09a2da2e04dea
  //
  // Accessor types: 2
  // sata_mem_port5_bridge:IOSF_Primary.sata_mem_port5_regs
  //   MemBar[1]
      "SATA_CONTR/sata_mem_port5_bridge",
  // sata_mem_port5_bridge:IOSF_SideBand.sata_mem_port5_regs_sb
  //   MsgMem[1]
      "SATA_CONTR/sata_mem_port5_bridge",
  //
  // Endpoint ID: 0x58d09a2fa2e04dea
  //
  // Accessor types: 2
  // sata_mem_port6_bridge:IOSF_Primary.sata_mem_port6_regs
  //   MemBar[1]
      "SATA_CONTR/sata_mem_port6_bridge",
  // sata_mem_port6_bridge:IOSF_SideBand.sata_mem_port6_regs_sb
  //   MsgMem[1]
      "SATA_CONTR/sata_mem_port6_bridge",
  //
  // Endpoint ID: 0x58d09a31a2e04dea
  //
  // Accessor types: 2
  // sata_mem_port7_bridge:IOSF_Primary.sata_mem_port7_regs
  //   MemBar[1]
      "SATA_CONTR/sata_mem_port7_bridge",
  // sata_mem_port7_bridge:IOSF_SideBand.sata_mem_port7_regs_sb
  //   MsgMem[1]
      "SATA_CONTR/sata_mem_port7_bridge",
  //
  // Endpoint ID: 0x58d09a33a2e04dea
  //
  // Accessor types: 2
  // usbx_bridge:IOSF_Primary.usbx_pci_config_registers
  //   Cfg[1]
      "USB3_HOST/usbx_bridge",
  // usbx_bridge:IOSF_SideBand.usbx_pci_config_registers_msg
  //   MsgCfg[1]
      "USB3_HOST/usbx_bridge",
  //
  // Endpoint ID: 0x58d09a35a2e04dea
  //
  // Accessor types: 2
  // usbx_bridge:IOSF_Primary.usbx_mmio_registers
  //   MemBar[1]
      "USB3_HOST/usbx_bridge",
  // usbx_bridge:IOSF_SideBand.usbx_mmio_registers_msg
  //   MsgMem[1]
      "USB3_HOST/usbx_bridge",
  //
  // Endpoint ID: 0x58d09a37a2e04dea
  //
  // Accessor types: 1
  // usbx_bridge:IOSF_SideBand.usbx_dap_private_config_registers
  //   MsgCr[1]
      "USB3_HOST/usbx_bridge",
  //
  // Endpoint ID: 0x58d09a38a2e04dea
  //
  // Accessor types: 1
  // usbx_exi_on_dbc_bridge:IOSF_SideBand.usbx_exi_private_config_registers
  //   MsgCr[1]
      "USB3_EXI_DBC/usbx_exi_on_dbc_bridge",
  //
  // Endpoint ID: 0x58d09a39a2e04dea
  //
  // Accessor types: 2
  // usbx_exi_on_dbc_bridge:IOSF_Primary.usbx_exi_on_dbc_registers_primary
  //   MemNoBar[1]
      "USB3_EXI_DBC/usbx_exi_on_dbc_bridge",
  // usbx_exi_on_dbc_bridge:IOSF_SideBand.usbx_exi_on_dbc_registers_sb
  //   MsgMem[1]
      "USB3_EXI_DBC/usbx_exi_on_dbc_bridge",
  NULL
};


