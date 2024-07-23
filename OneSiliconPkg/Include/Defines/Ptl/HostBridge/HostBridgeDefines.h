/** @file
  Header file for HostBridge IP specific Registers.

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
#ifndef _HOSTBRIDGE_DEFINES_PTL_CDIE_H_
#define _HOSTBRIDGE_DEFINES_PTL_CDIE_H_

#define SA_SEG_NUM              0x00
#define SA_MC_BUS               0x00
#define SA_MC_DEV               0x00
#define SA_MC_FUN               0x00

#define V_SA_DEVICE_ID_INVALID  0xFFFF

//
// Maximum DMI lanes and bundles supported (x8 and 4 lanes)
//
#define SA_DMI_MAX_LANE                      0x08
#define SA_DMI_MAX_BUNDLE                    0x04

//
// PantherLake CPU mobile SA Device IDs B0:D0:F0
//
#define PTL_U_SA_DEVICE_ID_4C               0xB000   ///< PantherLake U            (4+0+4)      SA DID
#define PTL_H_12XE_SA_DEVICE_ID_4C_8A       0xB001   ///< PantherLake H 12Xe       (4+8+4+12Xe) SA DID
#define PTL_H_4XE_SA_DEVICE_ID_4C_8A        0xB002   ///< PantherLake H 4Xe        (4+8+4+4Xe)  SA DID
#define PTL_U_SA_DEVICE_ID_2C               0xB003   ///< PantherLake U            (2+0+4)      SA DID
#define PTL_H_12XE_SA_DEVICE_ID_4C_4A       0xB004   ///< PantherLake H 12Xe       (4+4+4+12Xe) SA DID
#define PTL_H_4XE_SA_DEVICE_ID_4C_4A        0xB005   ///< PantherLake H 4Xe        (4+4+4+4Xe)  SA DID
#define PTL_U_NEX_SA_DEVICE_ID_4C           0xB007   ///< PantherLake U      (NEX) (4+0+4)      SA DID
#define PTL_H_12XE_NEX_SA_DEVICE_ID_4C_8A   0xB008   ///< PantherLake H 12Xe (NEX) (4+8+4+12Xe) SA DID
#define PTL_H_12XE_SA_DEVICE_ID_2C_8A       0xB009   ///< PantherLake H 12Xe       (2+8+4+12Xe) SA DID
#define PTL_H_12XE_SA_DEVICE_ID_2C_4A       0xB00A   ///< PantherLake H 12Xe       (2+4+4+12Xe) SA DID
#define PTL_H_4XE_SA_DEVICE_ID_2C_8A        0xB00B   ///< PantherLake H 4Xe        (2+8+4+4Xe)  SA DID
#define PTL_H_4XE_SA_DEVICE_ID_2C_4A        0xB00C   ///< PantherLake H 4Xe        (2+4+4+4Xe)  SA DID
#define PTL_U_NEX_SA_DEVICE_ID_2C           0xB010   ///< PantherLake U      (NEX) (2+0+4)      SA DID
#define PTL_H_12XE_NEX_SA_DEVICE_ID_4C_4A   0xB011   ///< PantherLake H 12Xe (NEX) (4+4+4+12Xe) SA DID
#define PTL_H_12XE_NEX_SA_DEVICE_ID_2C_8A   0xB012   ///< PantherLake H 12Xe (NEX) (2+8+4+12Xe) SA DID
#define PTL_H_12XE_NEX_SA_DEVICE_ID_2C_4A   0xB013   ///< PantherLake H 12Xe (NEX) (2+4+4+12Xe) SA DID
#define PTL_H_4XE_NEX_SA_DEVICE_ID_4C_8A    0xB014   ///< PantherLake H 4Xe  (NEX) (4+8+4+4Xe)  SA DID
#define PTL_H_4XE_NEX_SA_DEVICE_ID_4C_4A    0xB015   ///< PantherLake H 4Xe  (NEX) (4+4+4+4Xe)  SA DID
#define PTL_H_4XE_NEX_SA_DEVICE_ID_2C_8A    0xB016   ///< PantherLake H 4Xe  (NEX) (2+8+4+4Xe)  SA DID
#define PTL_H_4XE_NEX_SA_DEVICE_ID_2C_4A    0xB017   ///< PantherLake H 4Xe  (NEX) (2+4+4+4Xe)  SA DID



//
// Description of TMBAR (15:38)
// - This field corresponds to bits 38 to 15 of the base address PCI TMBAR configuration space.
// - SA PCI Device to Access TMBAR register
//
#define SA_PCI_DEV_4              (0x04)
#define SA_PCI_FUN_0              (0x00)
#define SA_PCI_DEV_A              (0x0A)

/**
 Description of GGCLCK (0:0)
 - When set to 1b, this bit will lock all bits in this register.
**/
#define B_SA_HOSTBRIDGE_GGC_MASK (0xffc0)

#define R_SA_MC_SIM_RECOGNITION_OFFSET              0xFC
#define V_SA_MC_SIM_RECOGNITION_PLATFORM_SIMICS     0x04
#define V_SA_MC_SIM_RECOGNITION_PLATFORM_HFPGA      0x05
#define V_SA_MC_SIM_RECOGNITION_PLATFORM_HSLE       0x03
#define V_SA_MC_SIM_RECOGNITION_PLATFORM_HSLE_UPF   0x02

//
// MCHBAR IO Register Offset Equates
//
#define V_SA_MCHBAR_REGBAR_SIZE                    SIZE_128MB
#define B_SA_MCHBAR_REGBAR_MASK_64B               (0x0000007FFFFFF000ULL)
#define V_SA_SAFBAR_REGBAR_SIZE                    SIZE_32MB    ///< Safbar Size 32MB

#define R_SA_MCHBAR_IMRIAEXCBASE_CBO_INGRESS_REG   0x6A40
#define R_SA_MCHBAR_IMRIAEXCLIMIT_CBO_INGRESS_REG  0x6A48
#define R_SA_MCHBAR_IMRIAEXCBASE1_CBO_INGRESS_REG  0x6A50
#define R_SA_MCHBAR_IMRIAEXCLIMIT1_CBO_INGRESS_REG 0x6A58


///
/// This register is used as interface between BIOS and MemSS PMA
///
typedef union {
  struct {
    UINT32 mc0_en : 1; /**< MC0 Enable */
                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/
                            /* BIOS Enable for MC0 */
    UINT32 mc1_en : 1; /**< MC1 Enable */
                            /* Bits[1:1], Access Type=RW/V, default=0x00000000*/
                            /* BIOS Enable for MC1 */
    UINT32 ibecc0_en : 1; /**< IBECC0 Enable */
                            /* Bits[2:2], Access Type=RW/V, default=0x00000000*/
                            /* BIOS Enable for IBECC0 */
    UINT32 ibecc1_en : 1; /**< IBECC1 Enable */
                            /* Bits[3:3], Access Type=RW/V, default=0x00000000*/
                            /* BIOS Enable for IBECC1 */
    UINT32 cce0_en : 1; /**< CCE0 Enable */
                            /* Bits[4:4], Access Type=RW/V, default=0x00000000*/
                            /* BIOS Enable for CCE0 */
    UINT32 cce1_en : 1; /**< CCE1 Enable */
                            /* Bits[5:5], Access Type=RW/V, default=0x00000000*/
                            /* BIOS Enable for CCE1 */
    UINT32 ddr_type : 3; /**< DDR Type */
                            /* Bits[8:6], Access Type=RW/V, default=0x00000000*/
                            /*
                               DDR Type  000 - Reserved,  001 - DDR5,  010
                               - LPDDR5,  011 - LPDDR4,  1xx - reserved
                            */
    UINT32 reserved : 23; /**< Reserved for BitField */
                            /* Bits[31:9], Access Type=RO, default=0x00000000*/
                            /* Reserved */
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} R_SA_MCHBAR_MEMSS_PMA_CR_MEM_CONFIG_STRUCT;

#define R_SA_MCHBAR_MEMSS_PMA_CR_MEM_CONFIG_REG    0x13D04  ///< MEMSS_PMA_CR_MEM_CONFIG

#endif  /* _HOSTBRIDGE_DEFINES_H_ */
