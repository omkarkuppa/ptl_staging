#ifndef __MrcRegisterPtl13xxx_h__
#define __MrcRegisterPtl13xxx_h__
/** @file
  This file was automatically generated. Modify at your own risk.
  Note that no error checking is done in these functions so ensure that the correct values are passed.

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

#pragma pack(push, 1)


#define MEMSS_PMA_CR_CAPABILITIES_MISC_REG                             (0x00013C00)
#define MEMSS_PMA_CR_CAPABILITIES_MISC_DEF                             (0x00000000)
#define MEMSS_PMA_CR_CAPABILITIES_MISC_VOLATILE_BITFIELDS_MSK          (0xFFFFFFFF)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDPCD_OFF                     ( 0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDPCD_WID                     ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDPCD_MSK                     (0x00000001)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDPCD_MIN                     (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDPCD_MAX                     (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDPCD_DEF                     (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDPCD_HSH                     (0x01813C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDRSZ_OFF                     ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDRSZ_WID                     ( 2)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDRSZ_MSK                     (0x00000006)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDRSZ_MIN                     (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDRSZ_MAX                     (3) // 0x00000003
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDRSZ_DEF                     (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDRSZ_HSH                     (0x02833C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_FDEE_OFF                      ( 3)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_FDEE_WID                      ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_FDEE_MSK                      (0x00000008)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_FDEE_MIN                      (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_FDEE_MAX                      (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_FDEE_DEF                      (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_FDEE_HSH                      (0x01873C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECCDIS_OFF                    ( 4)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECCDIS_WID                    ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECCDIS_MSK                    (0x00000010)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECCDIS_MIN                    (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECCDIS_MAX                    (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECCDIS_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECCDIS_HSH                    (0x01893C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECC_EXIST_OFF                 ( 5)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECC_EXIST_WID                 ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECC_EXIST_MSK                 (0x00000020)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECC_EXIST_MIN                 (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECC_EXIST_MAX                 (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECC_EXIST_DEF                 (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_ECC_EXIST_HSH                 (0x018B3C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_IBECCDIS_OFF                  ( 6)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_IBECCDIS_WID                  ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_IBECCDIS_MSK                  (0x00000040)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_IBECCDIS_MIN                  (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_IBECCDIS_MAX                  (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_IBECCDIS_DEF                  (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_IBECCDIS_HSH                  (0x018D3C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_QCLK_GV_DISABLE_OFF           ( 7)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_QCLK_GV_DISABLE_WID           ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_QCLK_GV_DISABLE_MSK           (0x00000080)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_QCLK_GV_DISABLE_MIN           (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_QCLK_GV_DISABLE_MAX           (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_QCLK_GV_DISABLE_DEF           (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_QCLK_GV_DISABLE_HSH           (0x018F3C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_0_OFF                ( 8)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_0_WID                ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_0_MSK                (0x00000100)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_0_MIN                (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_0_MAX                (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_0_DEF                (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_0_HSH                (0x01913C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_OC_EN_OFF                 ( 9)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_OC_EN_WID                 ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_OC_EN_MSK                 (0x00000200)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_OC_EN_MIN                 (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_OC_EN_MAX                 (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_OC_EN_DEF                 (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_OC_EN_HSH                 (0x01933C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_SLICE_DIS_OFF             (10)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_SLICE_DIS_WID             ( 2)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_SLICE_DIS_MSK             (0x00000C00)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_SLICE_DIS_MIN             (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_SLICE_DIS_MAX             (3) // 0x00000003
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_SLICE_DIS_DEF             (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_DDR_SLICE_DIS_HSH             (0x02953C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_MAX_F_QCLK_OFF                (12)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_MAX_F_QCLK_WID                ( 9)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_MAX_F_QCLK_MSK                (0x001FF000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_MAX_F_QCLK_MIN                (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_MAX_F_QCLK_MAX                (511) // 0x000001FF
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_MAX_F_QCLK_DEF                (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_MAX_F_QCLK_HSH                (0x09993C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_SMX_DIS_OFF                   (21)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_SMX_DIS_WID                   ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_SMX_DIS_MSK                   (0x00200000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_SMX_DIS_MIN                   (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_SMX_DIS_MAX                   (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_SMX_DIS_DEF                   (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_SMX_DIS_HSH                   (0x01AB3C00)

  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_1_OFF                (22)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_1_WID                (10)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_1_MSK                (0xFFC00000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_1_MIN                (0)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_1_MAX                (1023) // 0x000003FF
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_1_DEF                (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_MISC_Reserved_1_HSH                (0x0AAD3C00)

#define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_REG                    (0x00013C04)
#define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_DEF                    (0x00100401)
#define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_VOLATILE_BITFIELDS_MSK (0xFFFFFC00)

  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED0_OFF        ( 0)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED0_WID        ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED0_MSK        (0x00000001)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED0_MIN        (0)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED0_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED0_DEF        (0x00000001)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED0_HSH        (0x01013C04)

  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED1_OFF        ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED1_WID        ( 9)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED1_MSK        (0x000003FE)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED1_MIN        (0)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED1_MAX        (511) // 0x000001FF
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED1_DEF        (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_RESERVED1_HSH        (0x09033C04)

  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_LPDDR5_EN_OFF        (10)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_LPDDR5_EN_WID        ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_LPDDR5_EN_MSK        (0x00000400)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_LPDDR5_EN_MIN        (0)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_LPDDR5_EN_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_LPDDR5_EN_DEF        (0x00000001)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_LPDDR5_EN_HSH        (0x01953C04)

  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_LPDDR5_OFF     (11)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_LPDDR5_WID     ( 9)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_LPDDR5_MSK     (0x000FF800)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_LPDDR5_MIN     (0)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_LPDDR5_MAX     (511) // 0x000001FF
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_LPDDR5_DEF     (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_LPDDR5_HSH     (0x09973C04)

  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_DDR5_EN_OFF          (20)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_DDR5_EN_WID          ( 1)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_DDR5_EN_MSK          (0x00100000)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_DDR5_EN_MIN          (0)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_DDR5_EN_MAX          (1) // 0x00000001
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_DDR5_EN_DEF          (0x00000001)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_DDR5_EN_HSH          (0x01A93C04)

  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_DDR5_OFF       (21)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_DDR5_WID       ( 9)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_DDR5_MSK       (0x3FE00000)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_DDR5_MIN       (0)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_DDR5_MAX       (511) // 0x000001FF
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_DDR5_DEF       (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_DDR5_HSH       (0x09AB3C04)

  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_Reserved_OFF         (30)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_Reserved_WID         ( 2)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_Reserved_MSK         (0xC0000000)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_Reserved_MIN         (0)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_Reserved_MAX         (3) // 0x00000003
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_Reserved_DEF         (0x00000000)
  #define MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_Reserved_HSH         (0x02BD3C04)

#define MEMSS_PMA_CR_MOP_FUSES_REG                                     (0x00013CE4)
#define MEMSS_PMA_CR_MOP_FUSES_DEF                                     (0x00000000)
#define MEMSS_PMA_CR_MOP_FUSES_VOLATILE_BITFIELDS_MSK                  (0x00000FFF)

  #define MEMSS_PMA_CR_MOP_FUSES_MOP_DIE_DENSITY_OFF                   ( 0)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_DIE_DENSITY_WID                   ( 3)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_DIE_DENSITY_MSK                   (0x00000007)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_DIE_DENSITY_MIN                   (0)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_DIE_DENSITY_MAX                   (7) // 0x00000007
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_DIE_DENSITY_DEF                   (0x00000000)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_DIE_DENSITY_HSH                   (0x03813CE4)

  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RANKS_OFF                         ( 3)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RANKS_WID                         ( 1)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RANKS_MSK                         (0x00000008)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RANKS_MIN                         (0)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RANKS_MAX                         (1) // 0x00000001
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RANKS_DEF                         (0x00000000)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RANKS_HSH                         (0x01873CE4)

  #define MEMSS_PMA_CR_MOP_FUSES_MOP_PACKAGES_OFF                      ( 4)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_PACKAGES_WID                      ( 2)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_PACKAGES_MSK                      (0x00000030)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_PACKAGES_MIN                      (0)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_PACKAGES_MAX                      (3) // 0x00000003
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_PACKAGES_DEF                      (0x00000000)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_PACKAGES_HSH                      (0x02893CE4)

  #define MEMSS_PMA_CR_MOP_FUSES_MOP_VENDOR_OFF                        ( 6)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_VENDOR_WID                        ( 3)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_VENDOR_MSK                        (0x000001C0)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_VENDOR_MIN                        (0)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_VENDOR_MAX                        (7) // 0x00000007
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_VENDOR_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_VENDOR_HSH                        (0x038D3CE4)

  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RESERVED_OFF                      ( 9)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RESERVED_WID                      ( 3)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RESERVED_MSK                      (0x00000E00)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RESERVED_MIN                      (0)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RESERVED_MAX                      (7) // 0x00000007
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RESERVED_DEF                      (0x00000000)
  #define MEMSS_PMA_CR_MOP_FUSES_MOP_RESERVED_HSH                      (0x03933CE4)

#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_REG                               (0x00013D00)
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_DEF                               (0x00000000)
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC0_EN_OFF                      ( 0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC0_EN_WID                      ( 1)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC0_EN_MSK                      (0x00000001)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC0_EN_MIN                      (0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC0_EN_MAX                      (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC0_EN_DEF                      (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC0_EN_HSH                      (0x01013D00)

  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC1_EN_OFF                      ( 1)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC1_EN_WID                      ( 1)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC1_EN_MSK                      (0x00000002)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC1_EN_MIN                      (0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC1_EN_MAX                      (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC1_EN_DEF                      (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MC1_EN_HSH                      (0x01033D00)

  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC0_EN_OFF                   ( 2)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC0_EN_WID                   ( 1)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC0_EN_MSK                   (0x00000004)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC0_EN_MIN                   (0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC0_EN_MAX                   (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC0_EN_DEF                   (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC0_EN_HSH                   (0x01053D00)

  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC1_EN_OFF                   ( 3)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC1_EN_WID                   ( 1)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC1_EN_MSK                   (0x00000008)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC1_EN_MIN                   (0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC1_EN_MAX                   (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC1_EN_DEF                   (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_IBECC1_EN_HSH                   (0x01073D00)

  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE0_EN_OFF                     ( 4)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE0_EN_WID                     ( 1)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE0_EN_MSK                     (0x00000010)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE0_EN_MIN                     (0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE0_EN_MAX                     (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE0_EN_DEF                     (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE0_EN_HSH                     (0x01093D00)

  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE1_EN_OFF                     ( 5)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE1_EN_WID                     ( 1)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE1_EN_MSK                     (0x00000020)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE1_EN_MIN                     (0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE1_EN_MAX                     (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE1_EN_DEF                     (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_CCE1_EN_HSH                     (0x010B3D00)

  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_DDR_TYPE_OFF                    ( 6)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_DDR_TYPE_WID                    ( 3)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_DDR_TYPE_MSK                    (0x000001C0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_DDR_TYPE_MIN                    (0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_DDR_TYPE_MAX                    (7) // 0x00000007
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_DDR_TYPE_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_DDR_TYPE_HSH                    (0x030D3D00)

  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_TME_EN_OFF                      ( 9)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_TME_EN_WID                      ( 1)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_TME_EN_MSK                      (0x00000200)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_TME_EN_MIN                      (0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_TME_EN_MAX                      (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_TME_EN_DEF                      (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_TME_EN_HSH                      (0x01133D00)

  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_Reserved_OFF                    (10)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_Reserved_WID                    (22)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_Reserved_MSK                    (0xFFFFFC00)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_Reserved_MIN                    (0)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_Reserved_MAX                    (4194303) // 0x003FFFFF
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_Reserved_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MEM_CONFIG_Reserved_HSH                    (0x16153D00)

#define MEMSS_PMA_CR_MEM_CONFIG_REG                                    (0x00013D04)
#define MEMSS_PMA_CR_MEM_CONFIG_DEF                                    (0x00000000)
#define MEMSS_PMA_CR_MEM_CONFIG_VOLATILE_BITFIELDS_MSK                 (0x000003FF)

  #define MEMSS_PMA_CR_MEM_CONFIG_MC0_EN_OFF                           ( 0)
  #define MEMSS_PMA_CR_MEM_CONFIG_MC0_EN_WID                           ( 1)
  #define MEMSS_PMA_CR_MEM_CONFIG_MC0_EN_MSK                           (0x00000001)
  #define MEMSS_PMA_CR_MEM_CONFIG_MC0_EN_MIN                           (0)
  #define MEMSS_PMA_CR_MEM_CONFIG_MC0_EN_MAX                           (1) // 0x00000001
  #define MEMSS_PMA_CR_MEM_CONFIG_MC0_EN_DEF                           (0x00000000)
  #define MEMSS_PMA_CR_MEM_CONFIG_MC0_EN_HSH                           (0x01813D04)

  #define MEMSS_PMA_CR_MEM_CONFIG_MC1_EN_OFF                           ( 1)
  #define MEMSS_PMA_CR_MEM_CONFIG_MC1_EN_WID                           ( 1)
  #define MEMSS_PMA_CR_MEM_CONFIG_MC1_EN_MSK                           (0x00000002)
  #define MEMSS_PMA_CR_MEM_CONFIG_MC1_EN_MIN                           (0)
  #define MEMSS_PMA_CR_MEM_CONFIG_MC1_EN_MAX                           (1) // 0x00000001
  #define MEMSS_PMA_CR_MEM_CONFIG_MC1_EN_DEF                           (0x00000000)
  #define MEMSS_PMA_CR_MEM_CONFIG_MC1_EN_HSH                           (0x01833D04)

  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC0_EN_OFF                        ( 2)
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC0_EN_WID                        ( 1)
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC0_EN_MSK                        (0x00000004)
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC0_EN_MIN                        (0)
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC0_EN_MAX                        (1) // 0x00000001
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC0_EN_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC0_EN_HSH                        (0x01853D04)

  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC1_EN_OFF                        ( 3)
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC1_EN_WID                        ( 1)
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC1_EN_MSK                        (0x00000008)
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC1_EN_MIN                        (0)
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC1_EN_MAX                        (1) // 0x00000001
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC1_EN_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_MEM_CONFIG_IBECC1_EN_HSH                        (0x01873D04)

  #define MEMSS_PMA_CR_MEM_CONFIG_CCE0_EN_OFF                          ( 4)
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE0_EN_WID                          ( 1)
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE0_EN_MSK                          (0x00000010)
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE0_EN_MIN                          (0)
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE0_EN_MAX                          (1) // 0x00000001
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE0_EN_DEF                          (0x00000000)
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE0_EN_HSH                          (0x01893D04)

  #define MEMSS_PMA_CR_MEM_CONFIG_CCE1_EN_OFF                          ( 5)
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE1_EN_WID                          ( 1)
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE1_EN_MSK                          (0x00000020)
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE1_EN_MIN                          (0)
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE1_EN_MAX                          (1) // 0x00000001
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE1_EN_DEF                          (0x00000000)
  #define MEMSS_PMA_CR_MEM_CONFIG_CCE1_EN_HSH                          (0x018B3D04)

  #define MEMSS_PMA_CR_MEM_CONFIG_DDR_TYPE_OFF                         ( 6)
  #define MEMSS_PMA_CR_MEM_CONFIG_DDR_TYPE_WID                         ( 3)
  #define MEMSS_PMA_CR_MEM_CONFIG_DDR_TYPE_MSK                         (0x000001C0)
  #define MEMSS_PMA_CR_MEM_CONFIG_DDR_TYPE_MIN                         (0)
  #define MEMSS_PMA_CR_MEM_CONFIG_DDR_TYPE_MAX                         (7) // 0x00000007
  #define MEMSS_PMA_CR_MEM_CONFIG_DDR_TYPE_DEF                         (0x00000000)
  #define MEMSS_PMA_CR_MEM_CONFIG_DDR_TYPE_HSH                         (0x038D3D04)

  #define MEMSS_PMA_CR_MEM_CONFIG_TME_EN_OFF                           ( 9)
  #define MEMSS_PMA_CR_MEM_CONFIG_TME_EN_WID                           ( 1)
  #define MEMSS_PMA_CR_MEM_CONFIG_TME_EN_MSK                           (0x00000200)
  #define MEMSS_PMA_CR_MEM_CONFIG_TME_EN_MIN                           (0)
  #define MEMSS_PMA_CR_MEM_CONFIG_TME_EN_MAX                           (1) // 0x00000001
  #define MEMSS_PMA_CR_MEM_CONFIG_TME_EN_DEF                           (0x00000000)
  #define MEMSS_PMA_CR_MEM_CONFIG_TME_EN_HSH                           (0x01933D04)

#define MEMSS_PMA_CR_BIOS_REQ_REG                                      (0x00013D08)
#define MEMSS_PMA_CR_BIOS_REQ_DEF                                      (0x00000000)
#define MEMSS_PMA_CR_BIOS_REQ_VOLATILE_BITFIELDS_MSK                   (0x80000000)

  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_RATIO_OFF                         ( 0)
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_RATIO_WID                         ( 8)
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_RATIO_MSK                         (0x000000FF)
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_RATIO_MIN                         (0)
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_RATIO_MAX                         (255) // 0x000000FF
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_RATIO_DEF                         (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_RATIO_HSH                         (0x08013D08)

  #define MEMSS_PMA_CR_BIOS_REQ_GEAR_TYPE_OFF                          ( 8)
  #define MEMSS_PMA_CR_BIOS_REQ_GEAR_TYPE_WID                          ( 1)
  #define MEMSS_PMA_CR_BIOS_REQ_GEAR_TYPE_MSK                          (0x00000100)
  #define MEMSS_PMA_CR_BIOS_REQ_GEAR_TYPE_MIN                          (0)
  #define MEMSS_PMA_CR_BIOS_REQ_GEAR_TYPE_MAX                          (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_REQ_GEAR_TYPE_DEF                          (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_GEAR_TYPE_HSH                          (0x01113D08)

  #define MEMSS_PMA_CR_BIOS_REQ_DVFS_VDDQ_OFF                          ( 9)
  #define MEMSS_PMA_CR_BIOS_REQ_DVFS_VDDQ_WID                          ( 5)
  #define MEMSS_PMA_CR_BIOS_REQ_DVFS_VDDQ_MSK                          (0x00003E00)
  #define MEMSS_PMA_CR_BIOS_REQ_DVFS_VDDQ_MIN                          (0)
  #define MEMSS_PMA_CR_BIOS_REQ_DVFS_VDDQ_MAX                          (31) // 0x0000001F
  #define MEMSS_PMA_CR_BIOS_REQ_DVFS_VDDQ_DEF                          (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_DVFS_VDDQ_HSH                          (0x05133D08)

  #define MEMSS_PMA_CR_BIOS_REQ_VDDQ_CHANGE_ONLY_OFF                   (14)
  #define MEMSS_PMA_CR_BIOS_REQ_VDDQ_CHANGE_ONLY_WID                   ( 1)
  #define MEMSS_PMA_CR_BIOS_REQ_VDDQ_CHANGE_ONLY_MSK                   (0x00004000)
  #define MEMSS_PMA_CR_BIOS_REQ_VDDQ_CHANGE_ONLY_MIN                   (0)
  #define MEMSS_PMA_CR_BIOS_REQ_VDDQ_CHANGE_ONLY_MAX                   (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_REQ_VDDQ_CHANGE_ONLY_DEF                   (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_VDDQ_CHANGE_ONLY_HSH                   (0x011D3D08)

  #define MEMSS_PMA_CR_BIOS_REQ_MAX_BW_GBPS_OFF                        (18)
  #define MEMSS_PMA_CR_BIOS_REQ_MAX_BW_GBPS_WID                        (11)
  #define MEMSS_PMA_CR_BIOS_REQ_MAX_BW_GBPS_MSK                        (0x1FFC0000)
  #define MEMSS_PMA_CR_BIOS_REQ_MAX_BW_GBPS_MIN                        (0)
  #define MEMSS_PMA_CR_BIOS_REQ_MAX_BW_GBPS_MAX                        (2047) // 0x000007FF
  #define MEMSS_PMA_CR_BIOS_REQ_MAX_BW_GBPS_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_MAX_BW_GBPS_HSH                        (0x0B253D08)

  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_WP_IDX_OFF                        (29)
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_WP_IDX_WID                        ( 2)
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_WP_IDX_MSK                        (0x60000000)
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_WP_IDX_MIN                        (0)
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_WP_IDX_MAX                        (3) // 0x00000003
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_WP_IDX_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_QCLK_WP_IDX_HSH                        (0x023B3D08)

  #define MEMSS_PMA_CR_BIOS_REQ_RUN_BUSY_OFF                           (31)
  #define MEMSS_PMA_CR_BIOS_REQ_RUN_BUSY_WID                           ( 1)
  #define MEMSS_PMA_CR_BIOS_REQ_RUN_BUSY_MSK                           (0x80000000)
  #define MEMSS_PMA_CR_BIOS_REQ_RUN_BUSY_MIN                           (0)
  #define MEMSS_PMA_CR_BIOS_REQ_RUN_BUSY_MAX                           (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_REQ_RUN_BUSY_DEF                           (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_RUN_BUSY_HSH                           (0x01BF3D08)

#define MEMSS_PMA_CR_INIT_STATE_REG                                    (0x00013D0C)
#define MEMSS_PMA_CR_INIT_STATE_DEF                                    (0x00000000)
#define MEMSS_PMA_CR_INIT_STATE_VOLATILE_BITFIELDS_MSK                 (0xFFFFFFEF)

  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE0_OFF                        ( 0)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE0_WID                        ( 1)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE0_MSK                        (0x00000001)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE0_MIN                        (0)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE0_MAX                        (1) // 0x00000001
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE0_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE0_HSH                        (0x01813D0C)

  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE1_OFF                        ( 1)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE1_WID                        ( 1)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE1_MSK                        (0x00000002)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE1_MIN                        (0)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE1_MAX                        (1) // 0x00000001
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE1_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE1_HSH                        (0x01833D0C)

  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE2_OFF                        ( 2)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE2_WID                        ( 1)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE2_MSK                        (0x00000004)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE2_MIN                        (0)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE2_MAX                        (1) // 0x00000001
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE2_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE2_HSH                        (0x01853D0C)

  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE3_OFF                        ( 3)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE3_WID                        ( 1)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE3_MSK                        (0x00000008)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE3_MIN                        (0)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE3_MAX                        (1) // 0x00000001
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE3_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_INIT_STATE_MRC_SAVE3_HSH                        (0x01873D0C)

  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_OFF                  ( 4)
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_WID                  ( 1)
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_MSK                  (0x00000010)
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_MIN                  (0)
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_MAX                  (1) // 0x00000001
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_DEF                  (0x00000000)
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_HSH                  (0x01093D0C)

  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_ACK_OFF              ( 5)
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_ACK_WID              ( 1)
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_ACK_MSK              (0x00000020)
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_ACK_MIN              (0)
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_ACK_MAX              (1) // 0x00000001
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_ACK_DEF              (0x00000000)
  #define MEMSS_PMA_CR_INIT_STATE_MEM_CONFIG_DONE_ACK_HSH              (0x018B3D0C)

  #define MEMSS_PMA_CR_INIT_STATE_Reserved_OFF                         ( 6)
  #define MEMSS_PMA_CR_INIT_STATE_Reserved_WID                         (26)
  #define MEMSS_PMA_CR_INIT_STATE_Reserved_MSK                         (0xFFFFFFC0)
  #define MEMSS_PMA_CR_INIT_STATE_Reserved_MIN                         (0)
  #define MEMSS_PMA_CR_INIT_STATE_Reserved_MAX                         (67108863) // 0x03FFFFFF
  #define MEMSS_PMA_CR_INIT_STATE_Reserved_DEF                         (0x00000000)
  #define MEMSS_PMA_CR_INIT_STATE_Reserved_HSH                         (0x1A8D3D0C)

#define MEMSS_PMA_CR_BIOS_DATA_REG                                     (0x00013D10)
#define MEMSS_PMA_CR_BIOS_DATA_DEF                                     (0x00000000)
#define MEMSS_PMA_CR_BIOS_DATA_VOLATILE_BITFIELDS_MSK                  (0xFFFF01FF)

  #define MEMSS_PMA_CR_BIOS_DATA_QCLK_RATIO_OFF                        ( 0)
  #define MEMSS_PMA_CR_BIOS_DATA_QCLK_RATIO_WID                        ( 8)
  #define MEMSS_PMA_CR_BIOS_DATA_QCLK_RATIO_MSK                        (0x000000FF)
  #define MEMSS_PMA_CR_BIOS_DATA_QCLK_RATIO_MIN                        (0)
  #define MEMSS_PMA_CR_BIOS_DATA_QCLK_RATIO_MAX                        (255) // 0x000000FF
  #define MEMSS_PMA_CR_BIOS_DATA_QCLK_RATIO_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_BIOS_DATA_QCLK_RATIO_HSH                        (0x08813D10)

  #define MEMSS_PMA_CR_BIOS_DATA_GEAR_TYPE_OFF                         ( 8)
  #define MEMSS_PMA_CR_BIOS_DATA_GEAR_TYPE_WID                         ( 1)
  #define MEMSS_PMA_CR_BIOS_DATA_GEAR_TYPE_MSK                         (0x00000100)
  #define MEMSS_PMA_CR_BIOS_DATA_GEAR_TYPE_MIN                         (0)
  #define MEMSS_PMA_CR_BIOS_DATA_GEAR_TYPE_MAX                         (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_DATA_GEAR_TYPE_DEF                         (0x00000000)
  #define MEMSS_PMA_CR_BIOS_DATA_GEAR_TYPE_HSH                         (0x01913D10)

  #define MEMSS_PMA_CR_BIOS_DATA_VCCMEM_VOLTAGE_OFF                    (16)
  #define MEMSS_PMA_CR_BIOS_DATA_VCCMEM_VOLTAGE_WID                    (16)
  #define MEMSS_PMA_CR_BIOS_DATA_VCCMEM_VOLTAGE_MSK                    (0xFFFF0000)
  #define MEMSS_PMA_CR_BIOS_DATA_VCCMEM_VOLTAGE_MIN                    (0)
  #define MEMSS_PMA_CR_BIOS_DATA_VCCMEM_VOLTAGE_MAX                    (65535) // 0x0000FFFF
  #define MEMSS_PMA_CR_BIOS_DATA_VCCMEM_VOLTAGE_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_BIOS_DATA_VCCMEM_VOLTAGE_HSH                    (0x10A13D10)

#define MEMSS_PMA_CR_BIOS_MAILBOX_REG                                  (0x00013D14)
#define MEMSS_PMA_CR_BIOS_MAILBOX_DEF                                  (0x00000000)
#define MEMSS_PMA_CR_BIOS_MAILBOX_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_RAMP_VCCSA_EN_OFF                  ( 0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_RAMP_VCCSA_EN_WID                  ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_RAMP_VCCSA_EN_MSK                  (0x00000001)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_RAMP_VCCSA_EN_MIN                  (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_RAMP_VCCSA_EN_MAX                  (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MAILBOX_RAMP_VCCSA_EN_DEF                  (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_RAMP_VCCSA_EN_HSH                  (0x01013D14)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_0_OFF               ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_0_WID               ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_0_MSK               (0x00000002)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_0_MIN               (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_0_MAX               (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_0_DEF               (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_0_HSH               (0x01033D14)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_1_OFF               ( 2)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_1_WID               ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_1_MSK               (0x00000004)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_1_MIN               (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_1_MAX               (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_1_DEF               (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_1_HSH               (0x01053D14)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_2_OFF               ( 3)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_2_WID               ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_2_MSK               (0x00000008)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_2_MIN               (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_2_MAX               (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_2_DEF               (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_2_HSH               (0x01073D14)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_3_OFF               ( 4)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_3_WID               ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_3_MSK               (0x00000010)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_3_MIN               (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_3_MAX               (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_3_DEF               (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_ZQ_IS_DISABLED_3_HSH               (0x01093D14)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_L_SHAPE_CONFIG_OFF                 ( 5)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_L_SHAPE_CONFIG_WID                 ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_L_SHAPE_CONFIG_MSK                 (0x00000020)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_L_SHAPE_CONFIG_MIN                 (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_L_SHAPE_CONFIG_MAX                 (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MAILBOX_L_SHAPE_CONFIG_DEF                 (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_L_SHAPE_CONFIG_HSH                 (0x010B3D14)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_0_OFF            ( 6)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_0_WID            ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_0_MSK            (0x00000040)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_0_MIN            (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_0_MAX            (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_0_DEF            (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_0_HSH            (0x010D3D14)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_1_OFF            ( 7)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_1_WID            ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_1_MSK            (0x00000080)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_1_MIN            (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_1_MAX            (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_1_DEF            (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_1_HSH            (0x010F3D14)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_2_OFF            ( 8)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_2_WID            ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_2_MSK            (0x00000100)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_2_MIN            (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_2_MAX            (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_2_DEF            (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_2_HSH            (0x01113D14)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_3_OFF            ( 9)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_3_WID            ( 1)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_3_MSK            (0x00000200)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_3_MIN            (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_3_MAX            (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_3_DEF            (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_EDVFSC_IS_ENABLED_3_HSH            (0x01133D14)

  #define MEMSS_PMA_CR_BIOS_MAILBOX_Reserved_OFF                       (10)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_Reserved_WID                       (22)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_Reserved_MSK                       (0xFFFFFC00)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_Reserved_MIN                       (0)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_Reserved_MAX                       (4194303) // 0x003FFFFF
  #define MEMSS_PMA_CR_BIOS_MAILBOX_Reserved_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAILBOX_Reserved_HSH                       (0x16153D14)

#define MEMSS_PMA_CR_BIOS_ERROR_STATUS_REG                             (0x00013D18)
#define MEMSS_PMA_CR_BIOS_ERROR_STATUS_DEF                             (0x000000FF)
#define MEMSS_PMA_CR_BIOS_ERROR_STATUS_VOLATILE_BITFIELDS_MSK          (0x000000FF)

  #define MEMSS_PMA_CR_BIOS_ERROR_STATUS_ERROR_CODE_OFF                ( 0)
  #define MEMSS_PMA_CR_BIOS_ERROR_STATUS_ERROR_CODE_WID                ( 8)
  #define MEMSS_PMA_CR_BIOS_ERROR_STATUS_ERROR_CODE_MSK                (0x000000FF)
  #define MEMSS_PMA_CR_BIOS_ERROR_STATUS_ERROR_CODE_MIN                (0)
  #define MEMSS_PMA_CR_BIOS_ERROR_STATUS_ERROR_CODE_MAX                (255) // 0x000000FF
  #define MEMSS_PMA_CR_BIOS_ERROR_STATUS_ERROR_CODE_DEF                (0x000000FF)
  #define MEMSS_PMA_CR_BIOS_ERROR_STATUS_ERROR_CODE_HSH                (0x08813D18)

#define MEMSS_PMA_CR_MISC_ENABLES_REG                                  (0x00013D1C)
#define MEMSS_PMA_CR_MISC_ENABLES_DEF                                  (0x0030C084)
#define MEMSS_PMA_CR_MISC_ENABLES_VOLATILE_BITFIELDS_MSK               (0x00001800)

  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED0_OFF                      ( 0)
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED0_WID                      ( 1)
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED0_MSK                      (0x00000001)
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED0_MIN                      (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED0_MAX                      (1) // 0x00000001
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED0_DEF                      (0x00000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED0_HSH                      (0x01013D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEDEF_OFF           ( 1)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEDEF_WID           ( 1)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEDEF_MSK           (0x00000002)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEDEF_MIN           (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEDEF_MAX           (1) // 0x00000001
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEDEF_DEF           (0x00000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEDEF_HSH           (0x01033D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEN_OFF             ( 2)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEN_WID             ( 1)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEN_MSK             (0x00000004)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEN_MIN             (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEN_MAX             (1) // 0x00000001
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEN_DEF             (0x00000001)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_CLKGATEN_HSH             (0x01053D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_IDLECNT_OFF              ( 3)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_IDLECNT_WID              ( 8)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_IDLECNT_MSK              (0x000007F8)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_IDLECNT_MIN              (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_IDLECNT_MAX              (255) // 0x000000FF
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_IDLECNT_DEF              (0x00000010)
  #define MEMSS_PMA_CR_MISC_ENABLES_SB_CGCTRL_IDLECNT_HSH              (0x08073D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_OFF                 (11)
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_WID                 ( 1)
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_MSK                 (0x00000800)
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_MIN                 (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_MAX                 (1) // 0x00000001
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_DEF                 (0x00000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_HSH                 (0x01973D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_VALID_OFF           (12)
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_VALID_WID           ( 1)
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_VALID_MSK           (0x00001000)
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_VALID_MIN           (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_VALID_MAX           (1) // 0x00000001
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_VALID_DEF           (0x00000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_MEM_UNLOCK_STS_VALID_HSH           (0x01993D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_DFX_DISABLE_OFF                    (13)
  #define MEMSS_PMA_CR_MISC_ENABLES_DFX_DISABLE_WID                    ( 1)
  #define MEMSS_PMA_CR_MISC_ENABLES_DFX_DISABLE_MSK                    (0x00002000)
  #define MEMSS_PMA_CR_MISC_ENABLES_DFX_DISABLE_MIN                    (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_DFX_DISABLE_MAX                    (1) // 0x00000001
  #define MEMSS_PMA_CR_MISC_ENABLES_DFX_DISABLE_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_DFX_DISABLE_HSH                    (0x011B3D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_DeepSR_Exit_Latency_OFF            (14)
  #define MEMSS_PMA_CR_MISC_ENABLES_DeepSR_Exit_Latency_WID            (10)
  #define MEMSS_PMA_CR_MISC_ENABLES_DeepSR_Exit_Latency_MSK            (0x00FFC000)
  #define MEMSS_PMA_CR_MISC_ENABLES_DeepSR_Exit_Latency_MIN            (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_DeepSR_Exit_Latency_MAX            (1023) // 0x000003FF
  #define MEMSS_PMA_CR_MISC_ENABLES_DeepSR_Exit_Latency_DEF            (0x000000C3)
  #define MEMSS_PMA_CR_MISC_ENABLES_DeepSR_Exit_Latency_HSH            (0x0A1D3D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_MRC_START_ACK_OFF                  (24)
  #define MEMSS_PMA_CR_MISC_ENABLES_MRC_START_ACK_WID                  ( 1)
  #define MEMSS_PMA_CR_MISC_ENABLES_MRC_START_ACK_MSK                  (0x01000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_MRC_START_ACK_MIN                  (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_MRC_START_ACK_MAX                  (1) // 0x00000001
  #define MEMSS_PMA_CR_MISC_ENABLES_MRC_START_ACK_DEF                  (0x00000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_MRC_START_ACK_HSH                  (0x01313D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_ALWAYS_CNT_CKE_LOW_OFF             (25)
  #define MEMSS_PMA_CR_MISC_ENABLES_ALWAYS_CNT_CKE_LOW_WID             ( 1)
  #define MEMSS_PMA_CR_MISC_ENABLES_ALWAYS_CNT_CKE_LOW_MSK             (0x02000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_ALWAYS_CNT_CKE_LOW_MIN             (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_ALWAYS_CNT_CKE_LOW_MAX             (1) // 0x00000001
  #define MEMSS_PMA_CR_MISC_ENABLES_ALWAYS_CNT_CKE_LOW_DEF             (0x00000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_ALWAYS_CNT_CKE_LOW_HSH             (0x01333D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED1_OFF                      (26)
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED1_WID                      ( 2)
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED1_MSK                      (0x0C000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED1_MIN                      (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED1_MAX                      (3) // 0x00000003
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED1_DEF                      (0x00000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_RESERVED1_HSH                      (0x02353D1C)

  #define MEMSS_PMA_CR_MISC_ENABLES_Reserved_OFF                       (28)
  #define MEMSS_PMA_CR_MISC_ENABLES_Reserved_WID                       ( 4)
  #define MEMSS_PMA_CR_MISC_ENABLES_Reserved_MSK                       (0xF0000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_Reserved_MIN                       (0)
  #define MEMSS_PMA_CR_MISC_ENABLES_Reserved_MAX                       (15) // 0x0000000F
  #define MEMSS_PMA_CR_MISC_ENABLES_Reserved_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_MISC_ENABLES_Reserved_HSH                       (0x04393D1C)

#define MEMSS_PMA_CR_QCLK_POINT0_REG                                   (0x00013D20)
#define MEMSS_PMA_CR_QCLK_POINT0_DEF                                   (0x00000000)
#define MEMSS_PMA_CR_QCLK_POINT0_VOLATILE_BITFIELDS_MSK                (0x00003FFF)

  #define MEMSS_PMA_CR_QCLK_POINT0_QCLK_RATIO_0_OFF                    ( 0)
  #define MEMSS_PMA_CR_QCLK_POINT0_QCLK_RATIO_0_WID                    ( 8)
  #define MEMSS_PMA_CR_QCLK_POINT0_QCLK_RATIO_0_MSK                    (0x000000FF)
  #define MEMSS_PMA_CR_QCLK_POINT0_QCLK_RATIO_0_MIN                    (0)
  #define MEMSS_PMA_CR_QCLK_POINT0_QCLK_RATIO_0_MAX                    (255) // 0x000000FF
  #define MEMSS_PMA_CR_QCLK_POINT0_QCLK_RATIO_0_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT0_QCLK_RATIO_0_HSH                    (0x08813D20)

  #define MEMSS_PMA_CR_QCLK_POINT0_GEAR_TYPE_OFF                       ( 8)
  #define MEMSS_PMA_CR_QCLK_POINT0_GEAR_TYPE_WID                       ( 1)
  #define MEMSS_PMA_CR_QCLK_POINT0_GEAR_TYPE_MSK                       (0x00000100)
  #define MEMSS_PMA_CR_QCLK_POINT0_GEAR_TYPE_MIN                       (0)
  #define MEMSS_PMA_CR_QCLK_POINT0_GEAR_TYPE_MAX                       (1) // 0x00000001
  #define MEMSS_PMA_CR_QCLK_POINT0_GEAR_TYPE_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT0_GEAR_TYPE_HSH                       (0x01913D20)

  #define MEMSS_PMA_CR_QCLK_POINT0_DVFS_VDDQ_OFF                       ( 9)
  #define MEMSS_PMA_CR_QCLK_POINT0_DVFS_VDDQ_WID                       ( 5)
  #define MEMSS_PMA_CR_QCLK_POINT0_DVFS_VDDQ_MSK                       (0x00003E00)
  #define MEMSS_PMA_CR_QCLK_POINT0_DVFS_VDDQ_MIN                       (0)
  #define MEMSS_PMA_CR_QCLK_POINT0_DVFS_VDDQ_MAX                       (31) // 0x0000001F
  #define MEMSS_PMA_CR_QCLK_POINT0_DVFS_VDDQ_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT0_DVFS_VDDQ_HSH                       (0x05933D20)

#define MEMSS_PMA_CR_QCLK_POINT1_REG                                   (0x00013D24)
#define MEMSS_PMA_CR_QCLK_POINT1_DEF                                   (0x00000000)
#define MEMSS_PMA_CR_QCLK_POINT1_VOLATILE_BITFIELDS_MSK                (0x00003FFF)

  #define MEMSS_PMA_CR_QCLK_POINT1_QCLK_RATIO_1_OFF                    ( 0)
  #define MEMSS_PMA_CR_QCLK_POINT1_QCLK_RATIO_1_WID                    ( 8)
  #define MEMSS_PMA_CR_QCLK_POINT1_QCLK_RATIO_1_MSK                    (0x000000FF)
  #define MEMSS_PMA_CR_QCLK_POINT1_QCLK_RATIO_1_MIN                    (0)
  #define MEMSS_PMA_CR_QCLK_POINT1_QCLK_RATIO_1_MAX                    (255) // 0x000000FF
  #define MEMSS_PMA_CR_QCLK_POINT1_QCLK_RATIO_1_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT1_QCLK_RATIO_1_HSH                    (0x08813D24)

  #define MEMSS_PMA_CR_QCLK_POINT1_GEAR_TYPE_OFF                       ( 8)
  #define MEMSS_PMA_CR_QCLK_POINT1_GEAR_TYPE_WID                       ( 1)
  #define MEMSS_PMA_CR_QCLK_POINT1_GEAR_TYPE_MSK                       (0x00000100)
  #define MEMSS_PMA_CR_QCLK_POINT1_GEAR_TYPE_MIN                       (0)
  #define MEMSS_PMA_CR_QCLK_POINT1_GEAR_TYPE_MAX                       (1) // 0x00000001
  #define MEMSS_PMA_CR_QCLK_POINT1_GEAR_TYPE_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT1_GEAR_TYPE_HSH                       (0x01913D24)

  #define MEMSS_PMA_CR_QCLK_POINT1_DVFS_VDDQ_OFF                       ( 9)
  #define MEMSS_PMA_CR_QCLK_POINT1_DVFS_VDDQ_WID                       ( 5)
  #define MEMSS_PMA_CR_QCLK_POINT1_DVFS_VDDQ_MSK                       (0x00003E00)
  #define MEMSS_PMA_CR_QCLK_POINT1_DVFS_VDDQ_MIN                       (0)
  #define MEMSS_PMA_CR_QCLK_POINT1_DVFS_VDDQ_MAX                       (31) // 0x0000001F
  #define MEMSS_PMA_CR_QCLK_POINT1_DVFS_VDDQ_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT1_DVFS_VDDQ_HSH                       (0x05933D24)

#define MEMSS_PMA_CR_QCLK_POINT2_REG                                   (0x00013D28)
#define MEMSS_PMA_CR_QCLK_POINT2_DEF                                   (0x00000000)
#define MEMSS_PMA_CR_QCLK_POINT2_VOLATILE_BITFIELDS_MSK                (0x00003FFF)

  #define MEMSS_PMA_CR_QCLK_POINT2_QCLK_RATIO_2_OFF                    ( 0)
  #define MEMSS_PMA_CR_QCLK_POINT2_QCLK_RATIO_2_WID                    ( 8)
  #define MEMSS_PMA_CR_QCLK_POINT2_QCLK_RATIO_2_MSK                    (0x000000FF)
  #define MEMSS_PMA_CR_QCLK_POINT2_QCLK_RATIO_2_MIN                    (0)
  #define MEMSS_PMA_CR_QCLK_POINT2_QCLK_RATIO_2_MAX                    (255) // 0x000000FF
  #define MEMSS_PMA_CR_QCLK_POINT2_QCLK_RATIO_2_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT2_QCLK_RATIO_2_HSH                    (0x08813D28)

  #define MEMSS_PMA_CR_QCLK_POINT2_GEAR_TYPE_OFF                       ( 8)
  #define MEMSS_PMA_CR_QCLK_POINT2_GEAR_TYPE_WID                       ( 1)
  #define MEMSS_PMA_CR_QCLK_POINT2_GEAR_TYPE_MSK                       (0x00000100)
  #define MEMSS_PMA_CR_QCLK_POINT2_GEAR_TYPE_MIN                       (0)
  #define MEMSS_PMA_CR_QCLK_POINT2_GEAR_TYPE_MAX                       (1) // 0x00000001
  #define MEMSS_PMA_CR_QCLK_POINT2_GEAR_TYPE_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT2_GEAR_TYPE_HSH                       (0x01913D28)

  #define MEMSS_PMA_CR_QCLK_POINT2_DVFS_VDDQ_OFF                       ( 9)
  #define MEMSS_PMA_CR_QCLK_POINT2_DVFS_VDDQ_WID                       ( 5)
  #define MEMSS_PMA_CR_QCLK_POINT2_DVFS_VDDQ_MSK                       (0x00003E00)
  #define MEMSS_PMA_CR_QCLK_POINT2_DVFS_VDDQ_MIN                       (0)
  #define MEMSS_PMA_CR_QCLK_POINT2_DVFS_VDDQ_MAX                       (31) // 0x0000001F
  #define MEMSS_PMA_CR_QCLK_POINT2_DVFS_VDDQ_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT2_DVFS_VDDQ_HSH                       (0x05933D28)

#define MEMSS_PMA_CR_QCLK_POINT3_REG                                   (0x00013D2C)
#define MEMSS_PMA_CR_QCLK_POINT3_DEF                                   (0x00000000)
#define MEMSS_PMA_CR_QCLK_POINT3_VOLATILE_BITFIELDS_MSK                (0x00003FFF)

  #define MEMSS_PMA_CR_QCLK_POINT3_QCLK_RATIO_3_OFF                    ( 0)
  #define MEMSS_PMA_CR_QCLK_POINT3_QCLK_RATIO_3_WID                    ( 8)
  #define MEMSS_PMA_CR_QCLK_POINT3_QCLK_RATIO_3_MSK                    (0x000000FF)
  #define MEMSS_PMA_CR_QCLK_POINT3_QCLK_RATIO_3_MIN                    (0)
  #define MEMSS_PMA_CR_QCLK_POINT3_QCLK_RATIO_3_MAX                    (255) // 0x000000FF
  #define MEMSS_PMA_CR_QCLK_POINT3_QCLK_RATIO_3_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT3_QCLK_RATIO_3_HSH                    (0x08813D2C)

  #define MEMSS_PMA_CR_QCLK_POINT3_GEAR_TYPE_OFF                       ( 8)
  #define MEMSS_PMA_CR_QCLK_POINT3_GEAR_TYPE_WID                       ( 1)
  #define MEMSS_PMA_CR_QCLK_POINT3_GEAR_TYPE_MSK                       (0x00000100)
  #define MEMSS_PMA_CR_QCLK_POINT3_GEAR_TYPE_MIN                       (0)
  #define MEMSS_PMA_CR_QCLK_POINT3_GEAR_TYPE_MAX                       (1) // 0x00000001
  #define MEMSS_PMA_CR_QCLK_POINT3_GEAR_TYPE_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT3_GEAR_TYPE_HSH                       (0x01913D2C)

  #define MEMSS_PMA_CR_QCLK_POINT3_DVFS_VDDQ_OFF                       ( 9)
  #define MEMSS_PMA_CR_QCLK_POINT3_DVFS_VDDQ_WID                       ( 5)
  #define MEMSS_PMA_CR_QCLK_POINT3_DVFS_VDDQ_MSK                       (0x00003E00)
  #define MEMSS_PMA_CR_QCLK_POINT3_DVFS_VDDQ_MIN                       (0)
  #define MEMSS_PMA_CR_QCLK_POINT3_DVFS_VDDQ_MAX                       (31) // 0x0000001F
  #define MEMSS_PMA_CR_QCLK_POINT3_DVFS_VDDQ_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_QCLK_POINT3_DVFS_VDDQ_HSH                       (0x05933D2C)

#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_REG                          (0x00013D30)
#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_DEF                          (0x00000000)
#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_VOLATILE_BITFIELDS_MSK       (0x000007FF)

  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_MAX_BW_0_GBPS_OFF          ( 0)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_MAX_BW_0_GBPS_WID          (11)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_MAX_BW_0_GBPS_MSK          (0x000007FF)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_MAX_BW_0_GBPS_MIN          (0)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_MAX_BW_0_GBPS_MAX          (2047) // 0x000007FF
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_MAX_BW_0_GBPS_DEF          (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_MAX_BW_0_GBPS_HSH          (0x0B813D30)

#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_REG                          (0x00013D34)
#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_DEF                          (0x00000000)
#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_VOLATILE_BITFIELDS_MSK       (0x000007FF)

  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_MAX_BW_1_GBPS_OFF          ( 0)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_MAX_BW_1_GBPS_WID          (11)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_MAX_BW_1_GBPS_MSK          (0x000007FF)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_MAX_BW_1_GBPS_MIN          (0)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_MAX_BW_1_GBPS_MAX          (2047) // 0x000007FF
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_MAX_BW_1_GBPS_DEF          (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_MAX_BW_1_GBPS_HSH          (0x0B813D34)

#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_REG                          (0x00013D38)
#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_DEF                          (0x00000000)
#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_VOLATILE_BITFIELDS_MSK       (0x000007FF)

  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_MAX_BW_2_GBPS_OFF          ( 0)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_MAX_BW_2_GBPS_WID          (11)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_MAX_BW_2_GBPS_MSK          (0x000007FF)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_MAX_BW_2_GBPS_MIN          (0)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_MAX_BW_2_GBPS_MAX          (2047) // 0x000007FF
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_MAX_BW_2_GBPS_DEF          (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_MAX_BW_2_GBPS_HSH          (0x0B813D38)

#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_REG                          (0x00013D3C)
#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_DEF                          (0x00000000)
#define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_VOLATILE_BITFIELDS_MSK       (0x000007FF)

  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_MAX_BW_3_GBPS_OFF          ( 0)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_MAX_BW_3_GBPS_WID          (11)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_MAX_BW_3_GBPS_MSK          (0x000007FF)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_MAX_BW_3_GBPS_MIN          (0)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_MAX_BW_3_GBPS_MAX          (2047) // 0x000007FF
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_MAX_BW_3_GBPS_DEF          (0x00000000)
  #define MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_MAX_BW_3_GBPS_HSH          (0x0B813D3C)

#define MEMSS_PMA_CR_MRC_VERSION_REG                                   (0x00013D40)
#define MEMSS_PMA_CR_MRC_VERSION_DEF                                   (0x00000000)
#define MEMSS_PMA_CR_MRC_VERSION_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MEMSS_PMA_CR_MRC_VERSION_MRC_REVISION_OFF                    ( 0)
  #define MEMSS_PMA_CR_MRC_VERSION_MRC_REVISION_WID                    (32)
  #define MEMSS_PMA_CR_MRC_VERSION_MRC_REVISION_MSK                    (0xFFFFFFFF)
  #define MEMSS_PMA_CR_MRC_VERSION_MRC_REVISION_MIN                    (0)
  #define MEMSS_PMA_CR_MRC_VERSION_MRC_REVISION_MAX                    (4294967295) // 0xFFFFFFFF
  #define MEMSS_PMA_CR_MRC_VERSION_MRC_REVISION_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_MRC_VERSION_MRC_REVISION_HSH                    (0x20013D40)

#define MEMSS_PMA_CR_COUNTDOWN_TIMER_REG                               (0x00013D44)
#define MEMSS_PMA_CR_COUNTDOWN_TIMER_DEF                               (0x00000000)
#define MEMSS_PMA_CR_COUNTDOWN_TIMER_VOLATILE_BITFIELDS_MSK            (0x80000000)

  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_COUNTDOWN_TIMER_CLOCKS_OFF      ( 0)
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_COUNTDOWN_TIMER_CLOCKS_WID      (31)
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_COUNTDOWN_TIMER_CLOCKS_MSK      (0x7FFFFFFF)
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_COUNTDOWN_TIMER_CLOCKS_MIN      (0)
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_COUNTDOWN_TIMER_CLOCKS_MAX      (2147483647) // 0x7FFFFFFF
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_COUNTDOWN_TIMER_CLOCKS_DEF      (0x00000000)
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_COUNTDOWN_TIMER_CLOCKS_HSH      (0x1F013D44)

  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_RUN_BUSY_OFF                    (31)
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_RUN_BUSY_WID                    ( 1)
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_RUN_BUSY_MSK                    (0x80000000)
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_RUN_BUSY_MIN                    (0)
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_RUN_BUSY_MAX                    (1) // 0x00000001
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_RUN_BUSY_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_COUNTDOWN_TIMER_RUN_BUSY_HSH                    (0x01BF3D44)

#define MEMSS_PMA_CR_DFX_MISC_ENABLES_REG                              (0x00013D48)
#define MEMSS_PMA_CR_DFX_MISC_ENABLES_DEF                              (0x00000008)
#define MEMSS_PMA_CR_DFX_MISC_ENABLES_VOLATILE_BITFIELDS_MSK           (0x187FF806)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAGTRAIN_ENABLE_OFF            ( 0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAGTRAIN_ENABLE_WID            ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAGTRAIN_ENABLE_MSK            (0x00000001)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAGTRAIN_ENABLE_MIN            (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAGTRAIN_ENABLE_MAX            (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAGTRAIN_ENABLE_DEF            (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAGTRAIN_ENABLE_HSH            (0x01013D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAVE_RUN_BUSY_OFF              ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAVE_RUN_BUSY_WID              ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAVE_RUN_BUSY_MSK              (0x00000002)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAVE_RUN_BUSY_MIN              (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAVE_RUN_BUSY_MAX              (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAVE_RUN_BUSY_DEF              (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_SAVE_RUN_BUSY_HSH              (0x01833D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_RESTORE_RUN_BUSY_OFF           ( 2)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_RESTORE_RUN_BUSY_WID           ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_RESTORE_RUN_BUSY_MSK           (0x00000004)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_RESTORE_RUN_BUSY_MIN           (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_RESTORE_RUN_BUSY_MAX           (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_RESTORE_RUN_BUSY_DEF           (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_RESTORE_RUN_BUSY_HSH           (0x01853D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_DLY_COUNTER_STEP_OFF           ( 3)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_DLY_COUNTER_STEP_WID           ( 8)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_DLY_COUNTER_STEP_MSK           (0x000007F8)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_DLY_COUNTER_STEP_MIN           (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_DLY_COUNTER_STEP_MAX           (255) // 0x000000FF
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_DLY_COUNTER_STEP_DEF           (0x00000001)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_DLY_COUNTER_STEP_HSH           (0x08073D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_BLOCK_RUN_BUSY_OFF         (11)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_BLOCK_RUN_BUSY_WID         ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_BLOCK_RUN_BUSY_MSK         (0x00000800)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_BLOCK_RUN_BUSY_MIN         (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_BLOCK_RUN_BUSY_MAX         (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_BLOCK_RUN_BUSY_DEF         (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_BLOCK_RUN_BUSY_HSH         (0x01973D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_BLOCK_RUN_BUSY_OFF         (12)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_BLOCK_RUN_BUSY_WID         ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_BLOCK_RUN_BUSY_MSK         (0x00001000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_BLOCK_RUN_BUSY_MIN         (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_BLOCK_RUN_BUSY_MAX         (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_BLOCK_RUN_BUSY_DEF         (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_BLOCK_RUN_BUSY_HSH         (0x01993D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_BLOCK_RUN_BUSY_OFF      (13)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_BLOCK_RUN_BUSY_WID      ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_BLOCK_RUN_BUSY_MSK      (0x00002000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_BLOCK_RUN_BUSY_MIN      (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_BLOCK_RUN_BUSY_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_BLOCK_RUN_BUSY_DEF      (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_BLOCK_RUN_BUSY_HSH      (0x019B3D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_BLOCK_RUN_BUSY_OFF      (14)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_BLOCK_RUN_BUSY_WID      ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_BLOCK_RUN_BUSY_MSK      (0x00004000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_BLOCK_RUN_BUSY_MIN      (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_BLOCK_RUN_BUSY_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_BLOCK_RUN_BUSY_DEF      (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_BLOCK_RUN_BUSY_HSH      (0x019D3D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_BLOCK_RUN_BUSY_OFF        (15)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_BLOCK_RUN_BUSY_WID        ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_BLOCK_RUN_BUSY_MSK        (0x00008000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_BLOCK_RUN_BUSY_MIN        (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_BLOCK_RUN_BUSY_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_BLOCK_RUN_BUSY_DEF        (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_BLOCK_RUN_BUSY_HSH        (0x019F3D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_BLOCK_RUN_BUSY_OFF        (16)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_BLOCK_RUN_BUSY_WID        ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_BLOCK_RUN_BUSY_MSK        (0x00010000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_BLOCK_RUN_BUSY_MIN        (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_BLOCK_RUN_BUSY_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_BLOCK_RUN_BUSY_DEF        (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_BLOCK_RUN_BUSY_HSH        (0x01A13D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_UNBLOCK_RUN_BUSY_OFF       (17)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_UNBLOCK_RUN_BUSY_WID       ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_UNBLOCK_RUN_BUSY_MSK       (0x00020000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_UNBLOCK_RUN_BUSY_MIN       (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_UNBLOCK_RUN_BUSY_MAX       (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_UNBLOCK_RUN_BUSY_DEF       (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC0_UNBLOCK_RUN_BUSY_HSH       (0x01A33D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_UNBLOCK_RUN_BUSY_OFF       (18)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_UNBLOCK_RUN_BUSY_WID       ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_UNBLOCK_RUN_BUSY_MSK       (0x00040000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_UNBLOCK_RUN_BUSY_MIN       (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_UNBLOCK_RUN_BUSY_MAX       (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_UNBLOCK_RUN_BUSY_DEF       (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_MC1_UNBLOCK_RUN_BUSY_HSH       (0x01A53D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_UNBLOCK_RUN_BUSY_OFF    (19)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_UNBLOCK_RUN_BUSY_WID    ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_UNBLOCK_RUN_BUSY_MSK    (0x00080000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_UNBLOCK_RUN_BUSY_MIN    (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_UNBLOCK_RUN_BUSY_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_UNBLOCK_RUN_BUSY_DEF    (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC0_UNBLOCK_RUN_BUSY_HSH    (0x01A73D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_UNBLOCK_RUN_BUSY_OFF    (20)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_UNBLOCK_RUN_BUSY_WID    ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_UNBLOCK_RUN_BUSY_MSK    (0x00100000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_UNBLOCK_RUN_BUSY_MIN    (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_UNBLOCK_RUN_BUSY_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_UNBLOCK_RUN_BUSY_DEF    (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_IBECC1_UNBLOCK_RUN_BUSY_HSH    (0x01A93D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_UNBLOCK_RUN_BUSY_OFF      (21)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_UNBLOCK_RUN_BUSY_WID      ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_UNBLOCK_RUN_BUSY_MSK      (0x00200000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_UNBLOCK_RUN_BUSY_MIN      (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_UNBLOCK_RUN_BUSY_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_UNBLOCK_RUN_BUSY_DEF      (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE0_UNBLOCK_RUN_BUSY_HSH      (0x01AB3D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_UNBLOCK_RUN_BUSY_OFF      (22)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_UNBLOCK_RUN_BUSY_WID      ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_UNBLOCK_RUN_BUSY_MSK      (0x00400000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_UNBLOCK_RUN_BUSY_MIN      (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_UNBLOCK_RUN_BUSY_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_UNBLOCK_RUN_BUSY_DEF      (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CCE1_UNBLOCK_RUN_BUSY_HSH      (0x01AD3D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_BLOCK_FORMAT_SEL_OFF           (23)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_BLOCK_FORMAT_SEL_WID           ( 2)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_BLOCK_FORMAT_SEL_MSK           (0x01800000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_BLOCK_FORMAT_SEL_MIN           (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_BLOCK_FORMAT_SEL_MAX           (3) // 0x00000003
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_BLOCK_FORMAT_SEL_DEF           (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_BLOCK_FORMAT_SEL_HSH           (0x022F3D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_PG_SEQUENCE_ENABLE_OFF         (25)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_PG_SEQUENCE_ENABLE_WID         ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_PG_SEQUENCE_ENABLE_MSK         (0x02000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_PG_SEQUENCE_ENABLE_MIN         (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_PG_SEQUENCE_ENABLE_MAX         (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_PG_SEQUENCE_ENABLE_DEF         (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_PG_SEQUENCE_ENABLE_HSH         (0x01333D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CLK_SPINE_GATING_DISABLE_OFF   (26)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CLK_SPINE_GATING_DISABLE_WID   ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CLK_SPINE_GATING_DISABLE_MSK   (0x04000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CLK_SPINE_GATING_DISABLE_MIN   (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CLK_SPINE_GATING_DISABLE_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CLK_SPINE_GATING_DISABLE_DEF   (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_CLK_SPINE_GATING_DISABLE_HSH   (0x01353D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_BLOCK_RUN_BUSY_OFF        (27)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_BLOCK_RUN_BUSY_WID        ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_BLOCK_RUN_BUSY_MSK        (0x08000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_BLOCK_RUN_BUSY_MIN        (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_BLOCK_RUN_BUSY_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_BLOCK_RUN_BUSY_DEF        (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_BLOCK_RUN_BUSY_HSH        (0x01B73D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_UNBLOCK_RUN_BUSY_OFF      (28)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_UNBLOCK_RUN_BUSY_WID      ( 1)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_UNBLOCK_RUN_BUSY_MSK      (0x10000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_UNBLOCK_RUN_BUSY_MIN      (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_UNBLOCK_RUN_BUSY_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_UNBLOCK_RUN_BUSY_DEF      (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_ONDD_UNBLOCK_RUN_BUSY_HSH      (0x01B93D48)

  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_Reserved_OFF                   (29)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_Reserved_WID                   ( 3)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_Reserved_MSK                   (0xE0000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_Reserved_MIN                   (0)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_Reserved_MAX                   (7) // 0x00000007
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_Reserved_DEF                   (0x00000000)
  #define MEMSS_PMA_CR_DFX_MISC_ENABLES_Reserved_HSH                   (0x033B3D48)

#define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_REG                     (0x00013D4C)
#define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_DEF                     (0x00000000)
#define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VOLATILE_BITFIELDS_MSK  (0xFFFFFFFF)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_STS_OFF  ( 0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_STS_WID  ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_STS_MSK  (0x00000001)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_STS_MIN  (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_STS_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_STS_DEF  (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_STS_HSH  (0x01813D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_STS_OFF ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_STS_MSK (0x00000002)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_STS_HSH (0x01833D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_STS_OFF  ( 2)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_STS_WID  ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_STS_MSK  (0x00000004)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_STS_MIN  (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_STS_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_STS_DEF  (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_STS_HSH  (0x01853D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_STS_OFF ( 3)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_STS_MSK (0x00000008)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_STS_HSH (0x01873D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_STS_OFF ( 4)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_STS_MSK (0x00000010)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_STS_HSH (0x01893D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_STS_OFF ( 5)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_STS_MSK (0x00000020)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_STS_HSH (0x018B3D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_STS_OFF ( 6)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_STS_MSK (0x00000040)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_STS_HSH (0x018D3D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_STS_OFF ( 7)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_STS_MSK (0x00000080)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_STS_HSH (0x018F3D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_RST_STS_OFF ( 8)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_RST_STS_MSK (0x00000100)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC0_RST_STS_HSH (0x01913D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_RST_STS_OFF ( 9)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_RST_STS_MSK (0x00000200)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC0_RST_STS_HSH (0x01933D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_RST_STS_OFF (10)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_RST_STS_MSK (0x00000400)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMMC1_RST_STS_HSH (0x01953D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_RST_STS_OFF (11)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_RST_STS_MSK (0x00000800)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGTrainMC1_RST_STS_HSH (0x01973D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_RST_STS_OFF (12)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_RST_STS_MSK (0x00001000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC0_RST_STS_HSH (0x01993D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_RST_STS_OFF (13)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_RST_STS_MSK (0x00002000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMIBECC1_RST_STS_HSH (0x019B3D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_RST_STS_OFF (14)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_RST_STS_MSK (0x00004000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE0_RST_STS_HSH (0x019D3D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_RST_STS_OFF (15)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_RST_STS_MSK (0x00008000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCCE1_RST_STS_HSH (0x019F3D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CMI_PLL_LOCK_STS_OFF  (16)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CMI_PLL_LOCK_STS_WID  ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CMI_PLL_LOCK_STS_MSK  (0x00010000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CMI_PLL_LOCK_STS_MIN  (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CMI_PLL_LOCK_STS_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CMI_PLL_LOCK_STS_DEF  (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CMI_PLL_LOCK_STS_HSH  (0x01A13D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC0_BLOCK_STS_OFF     (17)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC0_BLOCK_STS_WID     ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC0_BLOCK_STS_MSK     (0x00020000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC0_BLOCK_STS_MIN     (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC0_BLOCK_STS_MAX     (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC0_BLOCK_STS_DEF     (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC0_BLOCK_STS_HSH     (0x01A33D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC1_BLOCK_STS_OFF     (18)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC1_BLOCK_STS_WID     ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC1_BLOCK_STS_MSK     (0x00040000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC1_BLOCK_STS_MIN     (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC1_BLOCK_STS_MAX     (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC1_BLOCK_STS_DEF     (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_MC1_BLOCK_STS_HSH     (0x01A53D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC0_BLOCK_STS_OFF  (19)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC0_BLOCK_STS_WID  ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC0_BLOCK_STS_MSK  (0x00080000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC0_BLOCK_STS_MIN  (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC0_BLOCK_STS_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC0_BLOCK_STS_DEF  (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC0_BLOCK_STS_HSH  (0x01A73D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC1_BLOCK_STS_OFF  (20)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC1_BLOCK_STS_WID  ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC1_BLOCK_STS_MSK  (0x00100000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC1_BLOCK_STS_MIN  (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC1_BLOCK_STS_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC1_BLOCK_STS_DEF  (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_IBECC1_BLOCK_STS_HSH  (0x01A93D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE0_BLOCK_STS_OFF    (21)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE0_BLOCK_STS_WID    ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE0_BLOCK_STS_MSK    (0x00200000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE0_BLOCK_STS_MIN    (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE0_BLOCK_STS_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE0_BLOCK_STS_DEF    (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE0_BLOCK_STS_HSH    (0x01AB3D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE1_BLOCK_STS_OFF    (22)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE1_BLOCK_STS_WID    ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE1_BLOCK_STS_MSK    (0x00400000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE1_BLOCK_STS_MIN    (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE1_BLOCK_STS_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE1_BLOCK_STS_DEF    (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_CCE1_BLOCK_STS_HSH    (0x01AD3D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_RST_STS_OFF (23)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_RST_STS_MSK (0x00800000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_RST_STS_HSH (0x01AF3D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_RST_STS_OFF (24)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_RST_STS_MSK (0x01000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_RST_STS_HSH (0x01B13D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_STS_OFF (25)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_STS_MSK (0x02000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMCMIPLL_STS_HSH (0x01B33D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_STS_OFF (26)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_STS_MSK (0x04000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMDDRPHYPMA_STS_HSH (0x01B53D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_RST_STS_OFF (27)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_RST_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_RST_STS_MSK (0x08000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_RST_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_RST_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_RST_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_RST_STS_HSH (0x01B73D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_ONDD_BLOCK_STS_OFF    (28)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_ONDD_BLOCK_STS_WID    ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_ONDD_BLOCK_STS_MSK    (0x10000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_ONDD_BLOCK_STS_MIN    (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_ONDD_BLOCK_STS_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_ONDD_BLOCK_STS_DEF    (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_ONDD_BLOCK_STS_HSH    (0x01B93D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_STS_OFF (29)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_STS_WID ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_STS_MSK (0x20000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_STS_MIN (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_VccSAGMEMONDD_STS_HSH (0x01BB3D4C)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_Reserved_OFF          (30)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_Reserved_WID          ( 2)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_Reserved_MSK          (0xC0000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_Reserved_MIN          (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_Reserved_MAX          (3) // 0x00000003
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_Reserved_DEF          (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_0_Reserved_HSH          (0x02BD3D4C)

#define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_REG                   (0x00013D50)
#define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_DEF                   (0x0000003F)
#define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC0_OFF             ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC0_WID             ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC0_MSK             (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC0_MIN             (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC0_MAX             (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC0_DEF             (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC0_HSH             (0x01013D50)

  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC1_OFF             ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC1_WID             ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC1_MSK             (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC1_MIN             (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC1_MAX             (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC1_DEF             (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_MC1_HSH             (0x01033D50)

  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC0_OFF          ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC0_WID          ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC0_MSK          (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC0_MIN          (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC0_MAX          (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC0_DEF          (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC0_HSH          (0x01053D50)

  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC1_OFF          ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC1_WID          ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC1_MSK          (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC1_MIN          (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC1_MAX          (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC1_DEF          (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_IBECC1_HSH          (0x01073D50)

  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE0_OFF            ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE0_WID            ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE0_MSK            (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE0_MIN            (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE0_MAX            (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE0_DEF            (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE0_HSH            (0x01093D50)

  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE1_OFF            ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE1_WID            ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE1_MSK            (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE1_MIN            (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE1_MAX            (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE1_DEF            (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_CCE1_HSH            (0x010B3D50)

  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_Reserved_OFF        ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_Reserved_WID        (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_Reserved_MSK        (0xFFFFFFC0)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_Reserved_MIN        (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_Reserved_MAX        (67108863) // 0x03FFFFFF
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_Reserved_DEF        (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_IP_PWRUP_MASK_Reserved_HSH        (0x1A0D3D50)

#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_REG                  (0x00013D54)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_DEF                  (0x0F0300B4)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_OFF        ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_WID        (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_MSK        (0x00000FFF)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_MIN        (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_MAX        (4095) // 0x00000FFF
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_DEF        (0x000000B4)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_HSH        (0x0C013D54)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_to_IPPwrGood_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_to_IPPwrGood_WID (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_to_IPPwrGood_MSK (0x00FFF000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_to_IPPwrGood_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_to_IPPwrGood_MAX (4095) // 0x00000FFF
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_to_IPPwrGood_DEF (0x00000030)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_PG_Ramp_to_IPPwrGood_HSH (0x0C193D54)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_DLY_AFTR_TGL_GLBDRV_EN_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_DLY_AFTR_TGL_GLBDRV_EN_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_DLY_AFTR_TGL_GLBDRV_EN_MSK (0x0F000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_DLY_AFTR_TGL_GLBDRV_EN_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_DLY_AFTR_TGL_GLBDRV_EN_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_DLY_AFTR_TGL_GLBDRV_EN_DEF (0x0000000F)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_DLY_AFTR_TGL_GLBDRV_EN_HSH (0x04313D54)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_Reserved_OFF       (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_Reserved_WID       ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_Reserved_MSK       (0xF0000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_Reserved_MIN       (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_Reserved_MAX       (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_Reserved_DEF       (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_0_Reserved_HSH       (0x04393D54)

#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_REG                  (0x00013D58)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_DEF                  (0x001FFC00)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_IPResetAssertion_to_PLLUnlock_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_IPResetAssertion_to_PLLUnlock_WID ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_IPResetAssertion_to_PLLUnlock_MSK (0x0001FC00)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_IPResetAssertion_to_PLLUnlock_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_IPResetAssertion_to_PLLUnlock_MAX (127) // 0x0000007F
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_IPResetAssertion_to_PLLUnlock_DEF (0x0000007F)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_IPResetAssertion_to_PLLUnlock_HSH (0x07153D58)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_PLLLOCK_to_GLBDRV_EN_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_PLLLOCK_to_GLBDRV_EN_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_PLLLOCK_to_GLBDRV_EN_MSK (0x001E0000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_PLLLOCK_to_GLBDRV_EN_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_PLLLOCK_to_GLBDRV_EN_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_PLLLOCK_to_GLBDRV_EN_DEF (0x0000000F)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_PLLLOCK_to_GLBDRV_EN_HSH (0x04233D58)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_SAGV_PM12_ACK_to_PM9_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_SAGV_PM12_ACK_to_PM9_WID (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_SAGV_PM12_ACK_to_PM9_MSK (0xFFE00000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_SAGV_PM12_ACK_to_PM9_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_SAGV_PM12_ACK_to_PM9_MAX (2047) // 0x000007FF
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_SAGV_PM12_ACK_to_PM9_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_1_SAGV_PM12_ACK_to_PM9_HSH (0x0B2B3D58)

#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_REG                  (0x00013D5C)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_DEF                  (0x00000014)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_ConductingDelay_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_ConductingDelay_WID (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_ConductingDelay_MSK (0x00000FFF)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_ConductingDelay_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_ConductingDelay_MAX (4095) // 0x00000FFF
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_ConductingDelay_DEF (0x00000014)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_ConductingDelay_HSH (0x0C013D5C)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_DFI_Timers_OFF (30)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_DFI_Timers_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_DFI_Timers_MSK (0x40000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_DFI_Timers_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_DFI_Timers_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_DFI_Timers_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_DFI_Timers_HSH (0x013D3D5C)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_PCHANNEL_Timers_OFF (31)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_PCHANNEL_Timers_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_PCHANNEL_Timers_MSK (0x80000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_PCHANNEL_Timers_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_PCHANNEL_Timers_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_PCHANNEL_Timers_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_2_Skip_PCHANNEL_Timers_HSH (0x013F3D5C)

#define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_REG                   (0x00013D60)
#define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEF                   (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_VOLATILE_BITFIELDS_MSK (0x00000006)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_MODE_EN_OFF   ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_MODE_EN_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_MODE_EN_MSK   (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_MODE_EN_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_MODE_EN_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_MODE_EN_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_MODE_EN_HSH   (0x01013D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEFAULT_NS_STEP_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEFAULT_NS_STEP_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEFAULT_NS_STEP_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEFAULT_NS_STEP_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEFAULT_NS_STEP_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEFAULT_NS_STEP_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEFAULT_NS_STEP_HSH (0x01833D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_STEP_OFF    ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_STEP_WID    ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_STEP_MSK    (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_STEP_MIN    (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_STEP_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_STEP_DEF    (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_STEP_HSH    (0x01853D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_FM_SEL_OFF    ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_FM_SEL_WID    ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_FM_SEL_MSK    (0x00000038)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_FM_SEL_MIN    (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_FM_SEL_MAX    (7) // 0x00000007
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_FM_SEL_DEF    (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_DEBUG_FM_SEL_HSH    (0x03073D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_SEL_OFF     ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_SEL_WID     ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_SEL_MSK     (0x00000FC0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_SEL_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_SEL_MAX     (63) // 0x0000003F
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_SEL_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_OVRD_NS_SEL_HSH     (0x060D3D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC0_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC0_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC0_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC0_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC0_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC0_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC0_HSH (0x01193D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC1_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC1_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC1_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC1_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC1_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC1_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_MC1_HSH (0x011B3D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC0_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC0_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC0_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC0_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC0_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC0_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC0_HSH (0x011D3D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC1_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC1_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC1_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC1_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC1_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC1_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_IBECC1_HSH (0x011F3D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE0_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE0_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE0_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE0_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE0_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE0_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE0_HSH (0x01213D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE1_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE1_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE1_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE1_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE1_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE1_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_REQ_CCE1_HSH (0x01233D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_MSG_DDRPHY_PMA_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_MSG_DDRPHY_PMA_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_MSG_DDRPHY_PMA_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_MSG_DDRPHY_PMA_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_MSG_DDRPHY_PMA_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_MSG_DDRPHY_PMA_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_MSG_DDRPHY_PMA_HSH (0x01253D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC0_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC0_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC0_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC0_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC0_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC0_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC0_HSH (0x01273D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC1_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC1_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC1_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC1_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC1_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC1_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_MC1_HSH (0x01293D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC0_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC0_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC0_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC0_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC0_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC0_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC0_HSH (0x012B3D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC1_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC1_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC1_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC1_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC1_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC1_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_IBECC1_HSH (0x012D3D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE0_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE0_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE0_MSK (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE0_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE0_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE0_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE0_HSH (0x012F3D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE1_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE1_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE1_MSK (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE1_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE1_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE1_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_RSP_CCE1_HSH (0x01313D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_ACK_DDRPHY_PMA_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_ACK_DDRPHY_PMA_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_ACK_DDRPHY_PMA_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_ACK_DDRPHY_PMA_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_ACK_DDRPHY_PMA_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_ACK_DDRPHY_PMA_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_PM_ACK_DDRPHY_PMA_HSH (0x01333D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC0_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC0_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC0_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC0_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC0_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC0_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC0_HSH (0x01353D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC1_OFF (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC1_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC1_MSK (0x08000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC1_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC1_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC1_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_REQ_MC1_HSH (0x01373D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC0_OFF (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC0_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC0_MSK (0x10000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC0_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC0_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC0_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC0_HSH (0x01393D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC1_OFF (29)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC1_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC1_MSK (0x20000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC1_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC1_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC1_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_Skip_oppSR_PM_RSP_MC1_HSH (0x013B3D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_DFI_INIT_COMPLETE_OFF (30)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_DFI_INIT_COMPLETE_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_DFI_INIT_COMPLETE_MSK (0x40000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_DFI_INIT_COMPLETE_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_DFI_INIT_COMPLETE_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_DFI_INIT_COMPLETE_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_DFI_INIT_COMPLETE_HSH (0x013D3D60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_ONDD_ACK_OFF   (31)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_ONDD_ACK_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_ONDD_ACK_MSK   (0x80000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_ONDD_ACK_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_ONDD_ACK_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_ONDD_ACK_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_SKIP_ONDD_ACK_HSH   (0x013F3D60)

#define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_REG                   (0x00013D64)
#define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_DEF                   (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_VOLATILE_BITFIELDS_MSK (0x80000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FM_SEL_OFF          ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FM_SEL_WID          ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FM_SEL_MSK          (0x00000007)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FM_SEL_MIN          (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FM_SEL_MAX          (7) // 0x00000007
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FM_SEL_DEF          (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FM_SEL_HSH          (0x03013D64)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_EN_OFF   ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_EN_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_EN_MSK   (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_EN_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_EN_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_EN_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_EN_HSH   (0x01073D64)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_SEL_OFF  ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_SEL_WID  ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_SEL_MSK  (0x000003F0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_SEL_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_SEL_MAX  (63) // 0x0000003F
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_SEL_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_SEL_HSH  (0x06093D64)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_DLY_OFF  (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_DLY_WID  ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_DLY_MSK  (0x0001FC00)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_DLY_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_DLY_MAX  (127) // 0x0000007F
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_DLY_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE0_DLY_HSH  (0x07153D64)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_EN_OFF   (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_EN_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_EN_MSK   (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_EN_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_EN_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_EN_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_EN_HSH   (0x01233D64)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_SEL_OFF  (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_SEL_WID  ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_SEL_MSK  (0x00FC0000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_SEL_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_SEL_MAX  (63) // 0x0000003F
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_SEL_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_SEL_HSH  (0x06253D64)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_DLY_OFF  (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_DLY_WID  ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_DLY_MSK  (0x7F000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_DLY_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_DLY_MAX  (127) // 0x0000007F
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_DLY_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_STATE1_DLY_HSH  (0x07313D64)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_RESUME_BP_OFF   (31)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_RESUME_BP_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_RESUME_BP_MSK   (0x80000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_RESUME_BP_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_RESUME_BP_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_RESUME_BP_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_FSM_RESUME_BP_HSH   (0x01BF3D64)

#define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_1_REG                   (0x00013D68)
//Duplicate of MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_REG

#define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_2_REG                   (0x00013D6C)
//Duplicate of MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_REG

#define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_3_REG                   (0x00013D70)
//Duplicate of MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_REG

#define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_4_REG                   (0x00013D74)
//Duplicate of MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_REG

#define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_5_REG                   (0x00013D78)
//Duplicate of MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_REG

#define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_6_REG                   (0x00013D7C)
//Duplicate of MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_REG

#define MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_7_REG                   (0x00013D80)
//Duplicate of MEMSS_PMA_CR_SURVIVABILITY_FM_PRGM_DLY_0_REG

#define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_REG                     (0x00013D84)
#define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_DEF                     (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_VOLATILE_BITFIELDS_MSK  (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_0_OFF     ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_0_WID     ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_0_MSK     (0x0000000F)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_0_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_0_MAX     (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_0_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_0_HSH     (0x04013D84)

  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_1_OFF     ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_1_WID     ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_1_MSK     (0x000000F0)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_1_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_1_MAX     (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_1_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_1_HSH     (0x04093D84)

  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_2_OFF     ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_2_WID     ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_2_MSK     (0x00000F00)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_2_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_2_MAX     (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_2_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_2_HSH     (0x04113D84)

  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_3_OFF     (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_3_WID     ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_3_MSK     (0x0000F000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_3_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_3_MAX     (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_3_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_3_HSH     (0x04193D84)

  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_4_OFF     (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_4_WID     ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_4_MSK     (0x000F0000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_4_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_4_MAX     (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_4_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_4_HSH     (0x04213D84)

  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_5_OFF     (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_5_WID     ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_5_MSK     (0x00F00000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_5_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_5_MAX     (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_5_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_5_HSH     (0x04293D84)

  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_6_OFF     (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_6_WID     ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_6_MSK     (0x0F000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_6_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_6_MAX     (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_6_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_6_HSH     (0x04313D84)

  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_7_OFF     (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_7_WID     ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_7_MSK     (0xF0000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_7_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_7_MAX     (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_7_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_SR_GROUPS_0_SR_GROUP_ID_7_HSH     (0x04393D84)

#define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_REG          (0x00013D88)
#define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_DEF          (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_en_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_en_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_en_HSH (0x01013D88)

  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_val_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_val_WID ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_val_MSK (0x000001FE)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_val_MAX (255) // 0x000000FF
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_pllratio_override_val_HSH (0x08033D88)

  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_en_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_en_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_en_HSH (0x01133D88)

  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_val_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_val_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_val_MSK (0x00003C00)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_val_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_refclk_div_cfg_override_val_HSH (0x04153D88)

  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_Reserved_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_Reserved_WID (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_Reserved_MSK (0xFFFFC000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_Reserved_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_Reserved_MAX (262143) // 0x0003FFFF
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_Reserved_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PLL_RATIO_DIV_OVERRIDE_Reserved_HSH (0x121D3D88)

#define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_REG                     (0x00013D90)
#define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_DEF                     (0x00000000)
#define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_VOLATILE_BITFIELDS_MSK  (0x0000003F)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC0_ACK_STS_OFF       ( 0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC0_ACK_STS_WID       ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC0_ACK_STS_MSK       (0x00000001)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC0_ACK_STS_MIN       (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC0_ACK_STS_MAX       (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC0_ACK_STS_DEF       (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC0_ACK_STS_HSH       (0x01813D90)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC1_ACK_STS_OFF       ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC1_ACK_STS_WID       ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC1_ACK_STS_MSK       (0x00000002)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC1_ACK_STS_MIN       (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC1_ACK_STS_MAX       (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC1_ACK_STS_DEF       (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_MC1_ACK_STS_HSH       (0x01833D90)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC0_ACK_STS_OFF    ( 2)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC0_ACK_STS_WID    ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC0_ACK_STS_MSK    (0x00000004)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC0_ACK_STS_MIN    (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC0_ACK_STS_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC0_ACK_STS_DEF    (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC0_ACK_STS_HSH    (0x01853D90)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC1_ACK_STS_OFF    ( 3)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC1_ACK_STS_WID    ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC1_ACK_STS_MSK    (0x00000008)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC1_ACK_STS_MIN    (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC1_ACK_STS_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC1_ACK_STS_DEF    (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_IBECC1_ACK_STS_HSH    (0x01873D90)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE0_ACK_STS_OFF      ( 4)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE0_ACK_STS_WID      ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE0_ACK_STS_MSK      (0x00000010)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE0_ACK_STS_MIN      (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE0_ACK_STS_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE0_ACK_STS_DEF      (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE0_ACK_STS_HSH      (0x01893D90)

  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE1_ACK_STS_OFF      ( 5)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE1_ACK_STS_WID      ( 1)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE1_ACK_STS_MSK      (0x00000020)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE1_ACK_STS_MIN      (0)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE1_ACK_STS_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE1_ACK_STS_DEF      (0x00000000)
  #define MEMSS_PMA_CR_DFX_PWR_CLK_RST_BLK_STS_1_CCE1_ACK_STS_HSH      (0x018B3D90)

#define MEMSS_PMA_CR_CCF_NCEVENTS_CR_LTCTRLSTS_REG                     (0x00013D94)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define MEMSS_PMA_CR_FLOW_STATUS_REG                                   (0x00013D98)
#define MEMSS_PMA_CR_FLOW_STATUS_DEF                                   (0x00000000)
#define MEMSS_PMA_CR_FLOW_STATUS_VOLATILE_BITFIELDS_MSK                (0x7FFFFFFF)

  #define MEMSS_PMA_CR_FLOW_STATUS_MRC_FLOW_STS_OFF                    ( 0)
  #define MEMSS_PMA_CR_FLOW_STATUS_MRC_FLOW_STS_WID                    ( 3)
  #define MEMSS_PMA_CR_FLOW_STATUS_MRC_FLOW_STS_MSK                    (0x00000007)
  #define MEMSS_PMA_CR_FLOW_STATUS_MRC_FLOW_STS_MIN                    (0)
  #define MEMSS_PMA_CR_FLOW_STATUS_MRC_FLOW_STS_MAX                    (7) // 0x00000007
  #define MEMSS_PMA_CR_FLOW_STATUS_MRC_FLOW_STS_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_MRC_FLOW_STS_HSH                    (0x03813D98)

  #define MEMSS_PMA_CR_FLOW_STATUS_GLOBAL_FLOW_STS_OFF                 ( 3)
  #define MEMSS_PMA_CR_FLOW_STATUS_GLOBAL_FLOW_STS_WID                 ( 3)
  #define MEMSS_PMA_CR_FLOW_STATUS_GLOBAL_FLOW_STS_MSK                 (0x00000038)
  #define MEMSS_PMA_CR_FLOW_STATUS_GLOBAL_FLOW_STS_MIN                 (0)
  #define MEMSS_PMA_CR_FLOW_STATUS_GLOBAL_FLOW_STS_MAX                 (7) // 0x00000007
  #define MEMSS_PMA_CR_FLOW_STATUS_GLOBAL_FLOW_STS_DEF                 (0x00000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_GLOBAL_FLOW_STS_HSH                 (0x03873D98)

  #define MEMSS_PMA_CR_FLOW_STATUS_CLOCK_PWR_STS_OFF                   ( 6)
  #define MEMSS_PMA_CR_FLOW_STATUS_CLOCK_PWR_STS_WID                   ( 3)
  #define MEMSS_PMA_CR_FLOW_STATUS_CLOCK_PWR_STS_MSK                   (0x000001C0)
  #define MEMSS_PMA_CR_FLOW_STATUS_CLOCK_PWR_STS_MIN                   (0)
  #define MEMSS_PMA_CR_FLOW_STATUS_CLOCK_PWR_STS_MAX                   (7) // 0x00000007
  #define MEMSS_PMA_CR_FLOW_STATUS_CLOCK_PWR_STS_DEF                   (0x00000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_CLOCK_PWR_STS_HSH                   (0x038D3D98)

  #define MEMSS_PMA_CR_FLOW_STATUS_QCLK_CURRENT_IDX_OFF                ( 9)
  #define MEMSS_PMA_CR_FLOW_STATUS_QCLK_CURRENT_IDX_WID                ( 2)
  #define MEMSS_PMA_CR_FLOW_STATUS_QCLK_CURRENT_IDX_MSK                (0x00000600)
  #define MEMSS_PMA_CR_FLOW_STATUS_QCLK_CURRENT_IDX_MIN                (0)
  #define MEMSS_PMA_CR_FLOW_STATUS_QCLK_CURRENT_IDX_MAX                (3) // 0x00000003
  #define MEMSS_PMA_CR_FLOW_STATUS_QCLK_CURRENT_IDX_DEF                (0x00000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_QCLK_CURRENT_IDX_HSH                (0x02933D98)

  #define MEMSS_PMA_CR_FLOW_STATUS_FUSE_PULL_DONE_OFF                  (11)
  #define MEMSS_PMA_CR_FLOW_STATUS_FUSE_PULL_DONE_WID                  ( 1)
  #define MEMSS_PMA_CR_FLOW_STATUS_FUSE_PULL_DONE_MSK                  (0x00000800)
  #define MEMSS_PMA_CR_FLOW_STATUS_FUSE_PULL_DONE_MIN                  (0)
  #define MEMSS_PMA_CR_FLOW_STATUS_FUSE_PULL_DONE_MAX                  (1) // 0x00000001
  #define MEMSS_PMA_CR_FLOW_STATUS_FUSE_PULL_DONE_DEF                  (0x00000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_FUSE_PULL_DONE_HSH                  (0x01973D98)

  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_PLLRATIO_OFF                (12)
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_PLLRATIO_WID                ( 8)
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_PLLRATIO_MSK                (0x000FF000)
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_PLLRATIO_MIN                (0)
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_PLLRATIO_MAX                (255) // 0x000000FF
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_PLLRATIO_DEF                (0x00000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_PLLRATIO_HSH                (0x08993D98)

  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_REF_CLK_DIV_OFF             (20)
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_REF_CLK_DIV_WID             ( 4)
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_REF_CLK_DIV_MSK             (0x00F00000)
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_REF_CLK_DIV_MIN             (0)
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_REF_CLK_DIV_MAX             (15) // 0x0000000F
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_REF_CLK_DIV_DEF             (0x00000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_CMI_PLL_REF_CLK_DIV_HSH             (0x04A93D98)

  #define MEMSS_PMA_CR_FLOW_STATUS_GEAR_TYPE_OFF                       (24)
  #define MEMSS_PMA_CR_FLOW_STATUS_GEAR_TYPE_WID                       ( 1)
  #define MEMSS_PMA_CR_FLOW_STATUS_GEAR_TYPE_MSK                       (0x01000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_GEAR_TYPE_MIN                       (0)
  #define MEMSS_PMA_CR_FLOW_STATUS_GEAR_TYPE_MAX                       (1) // 0x00000001
  #define MEMSS_PMA_CR_FLOW_STATUS_GEAR_TYPE_DEF                       (0x00000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_GEAR_TYPE_HSH                       (0x01B13D98)

  #define MEMSS_PMA_CR_FLOW_STATUS_FSM_CURR_STATE_OFF                  (25)
  #define MEMSS_PMA_CR_FLOW_STATUS_FSM_CURR_STATE_WID                  ( 6)
  #define MEMSS_PMA_CR_FLOW_STATUS_FSM_CURR_STATE_MSK                  (0x7E000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_FSM_CURR_STATE_MIN                  (0)
  #define MEMSS_PMA_CR_FLOW_STATUS_FSM_CURR_STATE_MAX                  (63) // 0x0000003F
  #define MEMSS_PMA_CR_FLOW_STATUS_FSM_CURR_STATE_DEF                  (0x00000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_FSM_CURR_STATE_HSH                  (0x06B33D98)

  #define MEMSS_PMA_CR_FLOW_STATUS_Reserved_OFF                        (31)
  #define MEMSS_PMA_CR_FLOW_STATUS_Reserved_WID                        ( 1)
  #define MEMSS_PMA_CR_FLOW_STATUS_Reserved_MSK                        (0x80000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_Reserved_MIN                        (0)
  #define MEMSS_PMA_CR_FLOW_STATUS_Reserved_MAX                        (1) // 0x00000001
  #define MEMSS_PMA_CR_FLOW_STATUS_Reserved_DEF                        (0x00000000)
  #define MEMSS_PMA_CR_FLOW_STATUS_Reserved_HSH                        (0x013F3D98)

#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_REG               (0x00013D9C)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_DEF               (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC0PowerGood_en_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC0PowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC0PowerGood_en_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC0PowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC0PowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC0PowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC0PowerGood_en_HSH (0x01013D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC0PowerGood_en_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC0PowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC0PowerGood_en_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC0PowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC0PowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC0PowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC0PowerGood_en_HSH (0x01033D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_prim_reset_b_en_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_prim_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_prim_reset_b_en_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_prim_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_prim_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_prim_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_prim_reset_b_en_HSH (0x01053D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_side_reset_b_en_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_side_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_side_reset_b_en_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_side_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_side_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_side_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_side_reset_b_en_HSH (0x01073D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_train_reset_b_en_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_train_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_train_reset_b_en_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_train_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_train_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_train_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_train_reset_b_en_HSH (0x01093D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_credit_config_done_en_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_credit_config_done_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_credit_config_done_en_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_credit_config_done_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_credit_config_done_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_credit_config_done_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_credit_config_done_en_HSH (0x010B3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_safe_mode_en_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_safe_mode_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_safe_mode_en_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_safe_mode_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_safe_mode_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_safe_mode_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_safe_mode_en_HSH (0x010D3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_q_restore_b_en_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_q_restore_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_q_restore_b_en_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_q_restore_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_q_restore_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_q_restore_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc0_q_restore_b_en_HSH (0x010F3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC1PowerGood_en_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC1PowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC1PowerGood_en_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC1PowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC1PowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC1PowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemMC1PowerGood_en_HSH (0x01113D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC1PowerGood_en_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC1PowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC1PowerGood_en_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC1PowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC1PowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC1PowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGTrainMC1PowerGood_en_HSH (0x01133D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_prim_reset_b_en_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_prim_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_prim_reset_b_en_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_prim_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_prim_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_prim_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_prim_reset_b_en_HSH (0x01153D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_side_reset_b_en_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_side_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_side_reset_b_en_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_side_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_side_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_side_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_side_reset_b_en_HSH (0x01173D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_train_reset_b_en_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_train_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_train_reset_b_en_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_train_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_train_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_train_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_train_reset_b_en_HSH (0x01193D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_credit_config_done_en_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_credit_config_done_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_credit_config_done_en_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_credit_config_done_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_credit_config_done_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_credit_config_done_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_credit_config_done_en_HSH (0x011B3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_safe_mode_en_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_safe_mode_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_safe_mode_en_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_safe_mode_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_safe_mode_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_safe_mode_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_safe_mode_en_HSH (0x011D3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_q_restore_b_en_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_q_restore_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_q_restore_b_en_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_q_restore_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_q_restore_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_q_restore_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_mc1_q_restore_b_en_HSH (0x011F3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC0PowerGood_en_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC0PowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC0PowerGood_en_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC0PowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC0PowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC0PowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC0PowerGood_en_HSH (0x01213D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_prim_reset_b_en_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_prim_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_prim_reset_b_en_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_prim_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_prim_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_prim_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_prim_reset_b_en_HSH (0x01233D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_side_reset_b_en_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_side_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_side_reset_b_en_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_side_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_side_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_side_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_side_reset_b_en_HSH (0x01253D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_credit_config_done_en_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_credit_config_done_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_credit_config_done_en_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_credit_config_done_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_credit_config_done_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_credit_config_done_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_credit_config_done_en_HSH (0x01273D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_safe_mode_en_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_safe_mode_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_safe_mode_en_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_safe_mode_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_safe_mode_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_safe_mode_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_safe_mode_en_HSH (0x01293D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_q_restore_b_en_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_q_restore_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_q_restore_b_en_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_q_restore_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_q_restore_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_q_restore_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc0_q_restore_b_en_HSH (0x012B3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC1PowerGood_en_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC1PowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC1PowerGood_en_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC1PowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC1PowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC1PowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_SAGMemIBECC1PowerGood_en_HSH (0x012D3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_prim_reset_b_en_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_prim_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_prim_reset_b_en_MSK (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_prim_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_prim_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_prim_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_prim_reset_b_en_HSH (0x012F3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_side_reset_b_en_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_side_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_side_reset_b_en_MSK (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_side_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_side_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_side_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_side_reset_b_en_HSH (0x01313D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_credit_config_done_en_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_credit_config_done_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_credit_config_done_en_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_credit_config_done_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_credit_config_done_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_credit_config_done_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_credit_config_done_en_HSH (0x01333D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_safe_mode_en_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_safe_mode_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_safe_mode_en_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_safe_mode_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_safe_mode_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_safe_mode_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_safe_mode_en_HSH (0x01353D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_q_restore_b_en_OFF (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_q_restore_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_q_restore_b_en_MSK (0x08000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_q_restore_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_q_restore_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_q_restore_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_ibecc1_q_restore_b_en_HSH (0x01373D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_side_reset_b_en_OFF (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_side_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_side_reset_b_en_MSK (0x10000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_side_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_side_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_side_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_side_reset_b_en_HSH (0x01393D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_vccdistpwrgood_en_OFF (29)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_vccdistpwrgood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_vccdistpwrgood_en_MSK (0x20000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_vccdistpwrgood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_vccdistpwrgood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_vccdistpwrgood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_vccdistpwrgood_en_HSH (0x013B3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_lock_en_en_OFF (30)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_lock_en_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_lock_en_en_MSK (0x40000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_lock_en_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_lock_en_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_lock_en_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_lock_en_en_HSH (0x013D3D9C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_clken_en_OFF (31)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_clken_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_clken_en_MSK (0x80000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_clken_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_clken_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_clken_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_0_cmi_pll_clken_en_HSH (0x013F3D9C)

#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_REG               (0x00013DA0)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_DEF               (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE0PowerGood_en_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE0PowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE0PowerGood_en_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE0PowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE0PowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE0PowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE0PowerGood_en_HSH (0x01013DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_prim_reset_b_en_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_prim_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_prim_reset_b_en_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_prim_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_prim_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_prim_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_prim_reset_b_en_HSH (0x01033DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_side_reset_b_en_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_side_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_side_reset_b_en_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_side_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_side_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_side_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_side_reset_b_en_HSH (0x01053DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_credit_config_done_en_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_credit_config_done_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_credit_config_done_en_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_credit_config_done_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_credit_config_done_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_credit_config_done_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_credit_config_done_en_HSH (0x01073DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_safe_mode_en_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_safe_mode_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_safe_mode_en_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_safe_mode_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_safe_mode_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_safe_mode_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_safe_mode_en_HSH (0x01093DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_q_restore_b_en_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_q_restore_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_q_restore_b_en_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_q_restore_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_q_restore_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_q_restore_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce0_q_restore_b_en_HSH (0x010B3DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE1PowerGood_en_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE1PowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE1PowerGood_en_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE1PowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE1PowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE1PowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCCE1PowerGood_en_HSH (0x010D3DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_prim_reset_b_en_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_prim_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_prim_reset_b_en_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_prim_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_prim_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_prim_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_prim_reset_b_en_HSH (0x010F3DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_side_reset_b_en_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_side_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_side_reset_b_en_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_side_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_side_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_side_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_side_reset_b_en_HSH (0x01113DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_credit_config_done_en_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_credit_config_done_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_credit_config_done_en_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_credit_config_done_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_credit_config_done_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_credit_config_done_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_credit_config_done_en_HSH (0x01133DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_safe_mode_en_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_safe_mode_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_safe_mode_en_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_safe_mode_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_safe_mode_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_safe_mode_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_safe_mode_en_HSH (0x01153DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_q_restore_b_en_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_q_restore_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_q_restore_b_en_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_q_restore_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_q_restore_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_q_restore_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_cce1_q_restore_b_en_HSH (0x01173DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ddrphy_pma_side_reset_b_en_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ddrphy_pma_side_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ddrphy_pma_side_reset_b_en_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ddrphy_pma_side_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ddrphy_pma_side_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ddrphy_pma_side_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ddrphy_pma_side_reset_b_en_HSH (0x01193DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_side_rst_b_en_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_side_rst_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_side_rst_b_en_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_side_rst_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_side_rst_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_side_rst_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_side_rst_b_en_HSH (0x011B3DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_save_restore_trigger_en_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_save_restore_trigger_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_save_restore_trigger_en_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_save_restore_trigger_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_save_restore_trigger_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_save_restore_trigger_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem0_save_restore_trigger_en_HSH (0x011D3DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_side_rst_b_en_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_side_rst_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_side_rst_b_en_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_side_rst_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_side_rst_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_side_rst_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_side_rst_b_en_HSH (0x011F3DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_save_restore_trigger_en_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_save_restore_trigger_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_save_restore_trigger_en_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_save_restore_trigger_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_save_restore_trigger_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_save_restore_trigger_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_svrs_mem1_save_restore_trigger_en_HSH (0x01213DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_req_en_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_req_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_req_en_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_req_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_req_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_req_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_req_en_HSH (0x01233DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_ack_en_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_ack_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_ack_en_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_ack_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_ack_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_ack_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_prep_unprep_ack_en_HSH (0x01253DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_req_en_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_req_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_req_en_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_req_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_req_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_req_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_req_en_HSH (0x01273DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_ack_en_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_ack_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_ack_en_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_ack_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_ack_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_ack_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_go_incgb_decgb_ack_en_HSH (0x01293DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_pok_en_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_pok_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_pok_en_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_pok_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_pok_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_pok_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_pok_en_HSH (0x012B3DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCMIPLLPowerGood_en_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCMIPLLPowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCMIPLLPowerGood_en_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCMIPLLPowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCMIPLLPowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCMIPLLPowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemCMIPLLPowerGood_en_HSH (0x012D3DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemDDRPHYPMAPowerGood_en_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemDDRPHYPMAPowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemDDRPHYPMAPowerGood_en_MSK (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemDDRPHYPMAPowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemDDRPHYPMAPowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemDDRPHYPMAPowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_SAGMemDDRPHYPMAPowerGood_en_HSH (0x012F3DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_RESERVED_OFF    (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_RESERVED_WID    ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_RESERVED_MSK    (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_RESERVED_MIN    (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_RESERVED_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_RESERVED_DEF    (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_RESERVED_HSH    (0x01313DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc0_en_OFF   (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc0_en_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc0_en_MSK   (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc0_en_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc0_en_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc0_en_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc0_en_HSH   (0x01333DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc1_en_OFF   (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc1_en_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc1_en_MSK   (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc1_en_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc1_en_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc1_en_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_ibecc1_en_HSH   (0x01353DA0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_cmi_pll_keep_regulator_on_en_OFF (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_cmi_pll_keep_regulator_on_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_cmi_pll_keep_regulator_on_en_MSK (0x08000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_cmi_pll_keep_regulator_on_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_cmi_pll_keep_regulator_on_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_cmi_pll_keep_regulator_on_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_1_memsspma_cmi_pll_keep_regulator_on_en_HSH (0x01373DA0)

#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_REG               (0x00013DA4)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_DEF               (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_tme_en_OFF      ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_tme_en_WID      ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_tme_en_MSK      (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_tme_en_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_tme_en_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_tme_en_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_tme_en_HSH      (0x01013DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_RESERVED1_OFF   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_RESERVED1_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_RESERVED1_MSK   (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_RESERVED1_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_RESERVED1_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_RESERVED1_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_RESERVED1_HSH   (0x01033DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_memsspma_resource_own_req_vccmemss_electrical_en_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_memsspma_resource_own_req_vccmemss_electrical_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_memsspma_resource_own_req_vccmemss_electrical_en_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_memsspma_resource_own_req_vccmemss_electrical_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_memsspma_resource_own_req_vccmemss_electrical_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_memsspma_resource_own_req_vccmemss_electrical_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_memsspma_resource_own_req_vccmemss_electrical_en_HSH (0x01053DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_block_req_en_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_block_req_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_block_req_en_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_block_req_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_block_req_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_block_req_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_block_req_en_HSH (0x01093DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_q_restore_b_en_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_q_restore_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_q_restore_b_en_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_q_restore_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_q_restore_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_q_restore_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_q_restore_b_en_HSH (0x010B3DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_safe_mode_en_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_safe_mode_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_safe_mode_en_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_safe_mode_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_safe_mode_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_safe_mode_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_safe_mode_en_HSH (0x010D3DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_side_reset_b_en_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_side_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_side_reset_b_en_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_side_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_side_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_side_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_side_reset_b_en_HSH (0x010F3DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_prim_reset_b_en_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_prim_reset_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_prim_reset_b_en_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_prim_reset_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_prim_reset_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_prim_reset_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ondd_prim_reset_b_en_HSH (0x01113DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_VCCMemssONDDPowerGood_en_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_VCCMemssONDDPowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_VCCMemssONDDPowerGood_en_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_VCCMemssONDDPowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_VCCMemssONDDPowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_VCCMemssONDDPowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_VCCMemssONDDPowerGood_en_HSH (0x01133DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_SAGMemONDDPowerGood_en_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_SAGMemONDDPowerGood_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_SAGMemONDDPowerGood_en_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_SAGMemONDDPowerGood_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_SAGMemONDDPowerGood_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_SAGMemONDDPowerGood_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_SAGMemONDDPowerGood_en_HSH (0x01153DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_svrs_mem_switch2vccret_en_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_svrs_mem_switch2vccret_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_svrs_mem_switch2vccret_en_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_svrs_mem_switch2vccret_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_svrs_mem_switch2vccret_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_svrs_mem_switch2vccret_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_svrs_mem_switch2vccret_en_HSH (0x01173DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_FDEE_en_OFF     (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_FDEE_en_WID     ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_FDEE_en_MSK     (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_FDEE_en_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_FDEE_en_MAX     (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_FDEE_en_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_FDEE_en_HSH     (0x01193DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ECCDIS_en_OFF   (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ECCDIS_en_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ECCDIS_en_MSK   (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ECCDIS_en_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ECCDIS_en_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ECCDIS_en_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_ECCDIS_en_HSH   (0x011B3DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_SLEEP_en_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_SLEEP_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_SLEEP_en_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_SLEEP_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_SLEEP_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_SLEEP_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_SLEEP_en_HSH (0x011D3DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_SLEEP_en_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_SLEEP_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_SLEEP_en_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_SLEEP_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_SLEEP_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_SLEEP_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_SLEEP_en_HSH (0x011F3DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_SLEEP_en_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_SLEEP_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_SLEEP_en_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_SLEEP_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_SLEEP_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_SLEEP_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_SLEEP_en_HSH (0x01213DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_SLEEP_en_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_SLEEP_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_SLEEP_en_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_SLEEP_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_SLEEP_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_SLEEP_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_SLEEP_en_HSH (0x01233DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_SLEEP_en_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_SLEEP_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_SLEEP_en_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_SLEEP_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_SLEEP_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_SLEEP_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_SLEEP_en_HSH (0x01253DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_SLEEP_en_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_SLEEP_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_SLEEP_en_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_SLEEP_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_SLEEP_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_SLEEP_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_SLEEP_en_HSH (0x01273DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_FORCE_CLKS_OFF_en_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_FORCE_CLKS_OFF_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_FORCE_CLKS_OFF_en_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_FORCE_CLKS_OFF_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_FORCE_CLKS_OFF_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_FORCE_CLKS_OFF_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE1_FORCE_CLKS_OFF_en_HSH (0x01293DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_FORCE_CLKS_OFF_en_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_FORCE_CLKS_OFF_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_FORCE_CLKS_OFF_en_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_FORCE_CLKS_OFF_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_FORCE_CLKS_OFF_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_FORCE_CLKS_OFF_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_CCE0_FORCE_CLKS_OFF_en_HSH (0x012B3DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_FORCE_CLKS_OFF_en_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_FORCE_CLKS_OFF_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_FORCE_CLKS_OFF_en_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_FORCE_CLKS_OFF_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_FORCE_CLKS_OFF_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_FORCE_CLKS_OFF_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC1_FORCE_CLKS_OFF_en_HSH (0x012D3DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_FORCE_CLKS_OFF_en_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_FORCE_CLKS_OFF_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_FORCE_CLKS_OFF_en_MSK (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_FORCE_CLKS_OFF_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_FORCE_CLKS_OFF_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_FORCE_CLKS_OFF_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_IBECC0_FORCE_CLKS_OFF_en_HSH (0x012F3DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_FORCE_CLKS_OFF_en_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_FORCE_CLKS_OFF_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_FORCE_CLKS_OFF_en_MSK (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_FORCE_CLKS_OFF_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_FORCE_CLKS_OFF_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_FORCE_CLKS_OFF_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC1_FORCE_CLKS_OFF_en_HSH (0x01313DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_FORCE_CLKS_OFF_en_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_FORCE_CLKS_OFF_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_FORCE_CLKS_OFF_en_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_FORCE_CLKS_OFF_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_FORCE_CLKS_OFF_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_FORCE_CLKS_OFF_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_MC0_FORCE_CLKS_OFF_en_HSH (0x01333DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_PMA_FORCE_CLKS_OFF_en_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_PMA_FORCE_CLKS_OFF_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_PMA_FORCE_CLKS_OFF_en_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_PMA_FORCE_CLKS_OFF_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_PMA_FORCE_CLKS_OFF_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_PMA_FORCE_CLKS_OFF_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_PMA_FORCE_CLKS_OFF_en_HSH (0x01353DA4)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_Reserved_OFF    (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_Reserved_WID    ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_Reserved_MSK    (0xF8000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_Reserved_MIN    (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_Reserved_MAX    (31) // 0x0000001F
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_Reserved_DEF    (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_2_Reserved_HSH    (0x05373DA4)

#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_REG                (0x00013DA8)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_DEF                (0x19A69C9C)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC0PowerGood_val_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC0PowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC0PowerGood_val_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC0PowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC0PowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC0PowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC0PowerGood_val_HSH (0x01013DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC0PowerGood_val_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC0PowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC0PowerGood_val_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC0PowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC0PowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC0PowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC0PowerGood_val_HSH (0x01033DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_prim_reset_b_val_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_prim_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_prim_reset_b_val_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_prim_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_prim_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_prim_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_prim_reset_b_val_HSH (0x01053DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_side_reset_b_val_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_side_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_side_reset_b_val_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_side_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_side_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_side_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_side_reset_b_val_HSH (0x01073DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_train_reset_b_val_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_train_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_train_reset_b_val_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_train_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_train_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_train_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_train_reset_b_val_HSH (0x01093DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_credit_config_done_val_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_credit_config_done_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_credit_config_done_val_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_credit_config_done_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_credit_config_done_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_credit_config_done_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_credit_config_done_val_HSH (0x010B3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_safe_mode_val_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_safe_mode_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_safe_mode_val_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_safe_mode_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_safe_mode_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_safe_mode_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_safe_mode_val_HSH (0x010D3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_q_restore_b_val_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_q_restore_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_q_restore_b_val_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_q_restore_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_q_restore_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_q_restore_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc0_q_restore_b_val_HSH (0x010F3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC1PowerGood_val_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC1PowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC1PowerGood_val_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC1PowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC1PowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC1PowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemMC1PowerGood_val_HSH (0x01113DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC1PowerGood_val_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC1PowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC1PowerGood_val_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC1PowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC1PowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC1PowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGTrainMC1PowerGood_val_HSH (0x01133DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_prim_reset_b_val_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_prim_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_prim_reset_b_val_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_prim_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_prim_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_prim_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_prim_reset_b_val_HSH (0x01153DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_side_reset_b_val_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_side_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_side_reset_b_val_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_side_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_side_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_side_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_side_reset_b_val_HSH (0x01173DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_train_reset_b_val_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_train_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_train_reset_b_val_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_train_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_train_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_train_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_train_reset_b_val_HSH (0x01193DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_credit_config_done_val_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_credit_config_done_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_credit_config_done_val_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_credit_config_done_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_credit_config_done_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_credit_config_done_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_credit_config_done_val_HSH (0x011B3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_safe_mode_val_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_safe_mode_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_safe_mode_val_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_safe_mode_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_safe_mode_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_safe_mode_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_safe_mode_val_HSH (0x011D3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_q_restore_b_val_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_q_restore_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_q_restore_b_val_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_q_restore_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_q_restore_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_q_restore_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_mc1_q_restore_b_val_HSH (0x011F3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC0PowerGood_val_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC0PowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC0PowerGood_val_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC0PowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC0PowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC0PowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC0PowerGood_val_HSH (0x01213DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_prim_reset_b_val_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_prim_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_prim_reset_b_val_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_prim_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_prim_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_prim_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_prim_reset_b_val_HSH (0x01233DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_side_reset_b_val_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_side_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_side_reset_b_val_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_side_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_side_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_side_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_side_reset_b_val_HSH (0x01253DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_credit_config_done_val_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_credit_config_done_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_credit_config_done_val_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_credit_config_done_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_credit_config_done_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_credit_config_done_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_credit_config_done_val_HSH (0x01273DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_safe_mode_val_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_safe_mode_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_safe_mode_val_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_safe_mode_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_safe_mode_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_safe_mode_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_safe_mode_val_HSH (0x01293DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_q_restore_b_val_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_q_restore_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_q_restore_b_val_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_q_restore_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_q_restore_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_q_restore_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc0_q_restore_b_val_HSH (0x012B3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC1PowerGood_val_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC1PowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC1PowerGood_val_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC1PowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC1PowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC1PowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_SAGMemIBECC1PowerGood_val_HSH (0x012D3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_prim_reset_b_val_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_prim_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_prim_reset_b_val_MSK (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_prim_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_prim_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_prim_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_prim_reset_b_val_HSH (0x012F3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_side_reset_b_val_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_side_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_side_reset_b_val_MSK (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_side_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_side_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_side_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_side_reset_b_val_HSH (0x01313DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_credit_config_done_val_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_credit_config_done_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_credit_config_done_val_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_credit_config_done_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_credit_config_done_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_credit_config_done_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_credit_config_done_val_HSH (0x01333DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_safe_mode_val_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_safe_mode_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_safe_mode_val_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_safe_mode_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_safe_mode_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_safe_mode_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_safe_mode_val_HSH (0x01353DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_q_restore_b_val_OFF (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_q_restore_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_q_restore_b_val_MSK (0x08000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_q_restore_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_q_restore_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_q_restore_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_ibecc1_q_restore_b_val_HSH (0x01373DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_side_reset_b_val_OFF (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_side_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_side_reset_b_val_MSK (0x10000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_side_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_side_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_side_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_side_reset_b_val_HSH (0x01393DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_vccdistpwrgood_val_OFF (29)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_vccdistpwrgood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_vccdistpwrgood_val_MSK (0x20000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_vccdistpwrgood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_vccdistpwrgood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_vccdistpwrgood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_vccdistpwrgood_val_HSH (0x013B3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_lock_en_val_OFF (30)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_lock_en_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_lock_en_val_MSK (0x40000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_lock_en_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_lock_en_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_lock_en_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_lock_en_val_HSH (0x013D3DA8)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_clken_val_OFF (31)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_clken_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_clken_val_MSK (0x80000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_clken_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_clken_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_clken_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_0_cmi_pll_clken_val_HSH (0x013F3DA8)

#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_REG                (0x00013DAC)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_DEF                (0x0000B9A6)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE0PowerGood_val_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE0PowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE0PowerGood_val_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE0PowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE0PowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE0PowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE0PowerGood_val_HSH (0x01013DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_prim_reset_b_val_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_prim_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_prim_reset_b_val_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_prim_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_prim_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_prim_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_prim_reset_b_val_HSH (0x01033DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_side_reset_b_val_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_side_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_side_reset_b_val_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_side_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_side_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_side_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_side_reset_b_val_HSH (0x01053DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_credit_config_done_val_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_credit_config_done_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_credit_config_done_val_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_credit_config_done_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_credit_config_done_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_credit_config_done_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_credit_config_done_val_HSH (0x01073DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_safe_mode_val_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_safe_mode_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_safe_mode_val_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_safe_mode_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_safe_mode_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_safe_mode_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_safe_mode_val_HSH (0x01093DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_q_restore_b_val_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_q_restore_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_q_restore_b_val_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_q_restore_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_q_restore_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_q_restore_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce0_q_restore_b_val_HSH (0x010B3DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE1PowerGood_val_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE1PowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE1PowerGood_val_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE1PowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE1PowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE1PowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCCE1PowerGood_val_HSH (0x010D3DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_prim_reset_b_val_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_prim_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_prim_reset_b_val_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_prim_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_prim_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_prim_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_prim_reset_b_val_HSH (0x010F3DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_side_reset_b_val_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_side_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_side_reset_b_val_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_side_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_side_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_side_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_side_reset_b_val_HSH (0x01113DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_credit_config_done_val_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_credit_config_done_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_credit_config_done_val_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_credit_config_done_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_credit_config_done_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_credit_config_done_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_credit_config_done_val_HSH (0x01133DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_safe_mode_val_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_safe_mode_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_safe_mode_val_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_safe_mode_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_safe_mode_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_safe_mode_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_safe_mode_val_HSH (0x01153DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_q_restore_b_val_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_q_restore_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_q_restore_b_val_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_q_restore_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_q_restore_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_q_restore_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_cce1_q_restore_b_val_HSH (0x01173DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ddrphy_pma_side_reset_b_val_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ddrphy_pma_side_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ddrphy_pma_side_reset_b_val_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ddrphy_pma_side_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ddrphy_pma_side_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ddrphy_pma_side_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ddrphy_pma_side_reset_b_val_HSH (0x01193DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_side_rst_b_val_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_side_rst_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_side_rst_b_val_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_side_rst_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_side_rst_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_side_rst_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_side_rst_b_val_HSH (0x011B3DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_save_restore_trigger_val_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_save_restore_trigger_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_save_restore_trigger_val_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_save_restore_trigger_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_save_restore_trigger_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_save_restore_trigger_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem0_save_restore_trigger_val_HSH (0x011D3DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_side_rst_b_val_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_side_rst_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_side_rst_b_val_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_side_rst_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_side_rst_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_side_rst_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_side_rst_b_val_HSH (0x011F3DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_save_restore_trigger_val_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_save_restore_trigger_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_save_restore_trigger_val_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_save_restore_trigger_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_save_restore_trigger_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_save_restore_trigger_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_svrs_mem1_save_restore_trigger_val_HSH (0x01213DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_req_val_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_req_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_req_val_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_req_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_req_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_req_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_req_val_HSH (0x01233DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_ack_val_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_ack_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_ack_val_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_ack_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_ack_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_ack_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_prep_unprep_ack_val_HSH (0x01253DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_req_val_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_req_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_req_val_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_req_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_req_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_req_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_req_val_HSH (0x01273DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_ack_val_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_ack_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_ack_val_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_ack_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_ack_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_ack_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_go_incgb_decgb_ack_val_HSH (0x01293DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_pok_val_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_pok_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_pok_val_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_pok_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_pok_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_pok_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_pok_val_HSH (0x012B3DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCMIPLLPowerGood_val_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCMIPLLPowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCMIPLLPowerGood_val_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCMIPLLPowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCMIPLLPowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCMIPLLPowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemCMIPLLPowerGood_val_HSH (0x012D3DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemDDRPHYPMAPowerGood_val_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemDDRPHYPMAPowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemDDRPHYPMAPowerGood_val_MSK (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemDDRPHYPMAPowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemDDRPHYPMAPowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemDDRPHYPMAPowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_SAGMemDDRPHYPMAPowerGood_val_HSH (0x012F3DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_RESERVED_OFF     (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_RESERVED_WID     ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_RESERVED_MSK     (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_RESERVED_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_RESERVED_MAX     (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_RESERVED_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_RESERVED_HSH     (0x01313DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc0_en_val_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc0_en_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc0_en_val_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc0_en_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc0_en_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc0_en_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc0_en_val_HSH (0x01333DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc1_en_val_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc1_en_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc1_en_val_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc1_en_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc1_en_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc1_en_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_ibecc1_en_val_HSH (0x01353DAC)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_cmi_pll_keep_regulator_on_val_OFF (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_cmi_pll_keep_regulator_on_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_cmi_pll_keep_regulator_on_val_MSK (0x08000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_cmi_pll_keep_regulator_on_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_cmi_pll_keep_regulator_on_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_cmi_pll_keep_regulator_on_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_1_memsspma_cmi_pll_keep_regulator_on_val_HSH (0x01373DAC)

#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_REG                (0x00013DB0)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_DEF                (0x000005D0)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_tme_en_val_OFF   ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_tme_en_val_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_tme_en_val_MSK   (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_tme_en_val_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_tme_en_val_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_tme_en_val_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_tme_en_val_HSH   (0x01013DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_RESERVED1_OFF    ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_RESERVED1_WID    ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_RESERVED1_MSK    (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_RESERVED1_MIN    (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_RESERVED1_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_RESERVED1_DEF    (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_RESERVED1_HSH    (0x01033DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_memsspma_resource_own_req_vccmemss_electrical_val_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_memsspma_resource_own_req_vccmemss_electrical_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_memsspma_resource_own_req_vccmemss_electrical_val_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_memsspma_resource_own_req_vccmemss_electrical_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_memsspma_resource_own_req_vccmemss_electrical_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_memsspma_resource_own_req_vccmemss_electrical_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_memsspma_resource_own_req_vccmemss_electrical_val_HSH (0x01053DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_block_req_val_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_block_req_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_block_req_val_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_block_req_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_block_req_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_block_req_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_block_req_val_HSH (0x01093DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_q_restore_b_val_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_q_restore_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_q_restore_b_val_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_q_restore_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_q_restore_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_q_restore_b_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_q_restore_b_val_HSH (0x010B3DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_safe_mode_val_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_safe_mode_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_safe_mode_val_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_safe_mode_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_safe_mode_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_safe_mode_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_safe_mode_val_HSH (0x010D3DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_side_reset_b_val_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_side_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_side_reset_b_val_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_side_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_side_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_side_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_side_reset_b_val_HSH (0x010F3DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_prim_reset_b_val_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_prim_reset_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_prim_reset_b_val_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_prim_reset_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_prim_reset_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_prim_reset_b_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ondd_prim_reset_b_val_HSH (0x01113DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_VCCMemssONDDPowerGood_val_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_VCCMemssONDDPowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_VCCMemssONDDPowerGood_val_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_VCCMemssONDDPowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_VCCMemssONDDPowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_VCCMemssONDDPowerGood_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_VCCMemssONDDPowerGood_val_HSH (0x01133DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_SAGMemONDDPowerGood_val_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_SAGMemONDDPowerGood_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_SAGMemONDDPowerGood_val_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_SAGMemONDDPowerGood_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_SAGMemONDDPowerGood_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_SAGMemONDDPowerGood_val_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_SAGMemONDDPowerGood_val_HSH (0x01153DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_svrs_mem_switch2vccret_val_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_svrs_mem_switch2vccret_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_svrs_mem_switch2vccret_val_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_svrs_mem_switch2vccret_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_svrs_mem_switch2vccret_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_svrs_mem_switch2vccret_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_svrs_mem_switch2vccret_val_HSH (0x01173DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_FDEE_val_OFF     (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_FDEE_val_WID     ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_FDEE_val_MSK     (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_FDEE_val_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_FDEE_val_MAX     (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_FDEE_val_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_FDEE_val_HSH     (0x01193DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ECCDIS_val_OFF   (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ECCDIS_val_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ECCDIS_val_MSK   (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ECCDIS_val_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ECCDIS_val_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ECCDIS_val_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_ECCDIS_val_HSH   (0x011B3DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_SLEEP_val_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_SLEEP_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_SLEEP_val_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_SLEEP_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_SLEEP_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_SLEEP_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_SLEEP_val_HSH (0x011D3DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_SLEEP_val_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_SLEEP_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_SLEEP_val_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_SLEEP_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_SLEEP_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_SLEEP_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_SLEEP_val_HSH (0x011F3DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_SLEEP_val_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_SLEEP_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_SLEEP_val_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_SLEEP_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_SLEEP_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_SLEEP_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_SLEEP_val_HSH (0x01213DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_SLEEP_val_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_SLEEP_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_SLEEP_val_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_SLEEP_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_SLEEP_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_SLEEP_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_SLEEP_val_HSH (0x01233DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_SLEEP_val_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_SLEEP_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_SLEEP_val_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_SLEEP_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_SLEEP_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_SLEEP_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_SLEEP_val_HSH (0x01253DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_SLEEP_val_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_SLEEP_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_SLEEP_val_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_SLEEP_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_SLEEP_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_SLEEP_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_SLEEP_val_HSH (0x01273DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_FORCE_CLKS_OFF_val_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_FORCE_CLKS_OFF_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_FORCE_CLKS_OFF_val_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_FORCE_CLKS_OFF_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_FORCE_CLKS_OFF_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_FORCE_CLKS_OFF_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE1_FORCE_CLKS_OFF_val_HSH (0x01293DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_FORCE_CLKS_OFF_val_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_FORCE_CLKS_OFF_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_FORCE_CLKS_OFF_val_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_FORCE_CLKS_OFF_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_FORCE_CLKS_OFF_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_FORCE_CLKS_OFF_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_CCE0_FORCE_CLKS_OFF_val_HSH (0x012B3DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_FORCE_CLKS_OFF_val_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_FORCE_CLKS_OFF_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_FORCE_CLKS_OFF_val_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_FORCE_CLKS_OFF_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_FORCE_CLKS_OFF_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_FORCE_CLKS_OFF_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC1_FORCE_CLKS_OFF_val_HSH (0x012D3DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_FORCE_CLKS_OFF_val_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_FORCE_CLKS_OFF_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_FORCE_CLKS_OFF_val_MSK (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_FORCE_CLKS_OFF_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_FORCE_CLKS_OFF_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_FORCE_CLKS_OFF_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_IBECC0_FORCE_CLKS_OFF_val_HSH (0x012F3DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_FORCE_CLKS_OFF_val_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_FORCE_CLKS_OFF_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_FORCE_CLKS_OFF_val_MSK (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_FORCE_CLKS_OFF_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_FORCE_CLKS_OFF_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_FORCE_CLKS_OFF_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC1_FORCE_CLKS_OFF_val_HSH (0x01313DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_FORCE_CLKS_OFF_val_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_FORCE_CLKS_OFF_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_FORCE_CLKS_OFF_val_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_FORCE_CLKS_OFF_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_FORCE_CLKS_OFF_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_FORCE_CLKS_OFF_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_MC0_FORCE_CLKS_OFF_val_HSH (0x01333DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_PMA_FORCE_CLKS_OFF_val_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_PMA_FORCE_CLKS_OFF_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_PMA_FORCE_CLKS_OFF_val_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_PMA_FORCE_CLKS_OFF_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_PMA_FORCE_CLKS_OFF_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_PMA_FORCE_CLKS_OFF_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_PMA_FORCE_CLKS_OFF_val_HSH (0x01353DB0)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_Reserved_OFF     (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_Reserved_WID     ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_Reserved_MSK     (0xF8000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_Reserved_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_Reserved_MAX     (31) // 0x0000001F
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_Reserved_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_2_Reserved_HSH     (0x05373DB0)

#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_REG               (0x00013DB4)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_DEF               (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideEn_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideEn_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideEn_HSH (0x01013DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideVal_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideVal_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PowerUpParOverrideVal_HSH (0x01033DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideEn_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideEn_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideEn_HSH (0x01053DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideVal_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideVal_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ForceOnParOverrideVal_HSH (0x01073DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PG_Enable_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PG_Enable_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PG_Enable_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PG_Enable_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PG_Enable_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PG_Enable_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PG_Enable_HSH (0x01093DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Master_mode_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Master_mode_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Master_mode_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Master_mode_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Master_mode_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Master_mode_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Master_mode_HSH (0x010B3DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_SingleStep_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_SingleStep_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_SingleStep_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_SingleStep_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_SingleStep_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_SingleStep_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_SingleStep_HSH (0x010D3DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideEn_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideEn_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideEn_HSH (0x010F3DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideVal_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideVal_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_PwrGoodOverrideVal_HSH (0x01113DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideEn_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideEn_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideEn_HSH (0x01133DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideVal_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideVal_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_EarlyForceOnOverrideVal_HSH (0x01153DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Skip_voltage_only_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Skip_voltage_only_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Skip_voltage_only_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Skip_voltage_only_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Skip_voltage_only_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Skip_voltage_only_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_Skip_voltage_only_HSH (0x01173DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ovr_gated_fet_ack_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ovr_gated_fet_ack_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ovr_gated_fet_ack_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ovr_gated_fet_ack_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ovr_gated_fet_ack_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ovr_gated_fet_ack_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC0_DFT_ovr_gated_fet_ack_HSH (0x01193DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_0_OFF  (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_0_WID  ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_0_MSK  (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_0_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_0_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_0_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_0_HSH  (0x011B3DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideEn_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideEn_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideEn_HSH (0x011D3DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideVal_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideVal_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PowerUpParOverrideVal_HSH (0x011F3DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideEn_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideEn_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideEn_HSH (0x01213DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideVal_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideVal_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ForceOnParOverrideVal_HSH (0x01233DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PG_Enable_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PG_Enable_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PG_Enable_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PG_Enable_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PG_Enable_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PG_Enable_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PG_Enable_HSH (0x01253DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Master_mode_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Master_mode_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Master_mode_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Master_mode_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Master_mode_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Master_mode_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Master_mode_HSH (0x01273DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_SingleStep_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_SingleStep_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_SingleStep_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_SingleStep_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_SingleStep_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_SingleStep_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_SingleStep_HSH (0x01293DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideEn_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideEn_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideEn_HSH (0x012B3DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideVal_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideVal_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_PwrGoodOverrideVal_HSH (0x012D3DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideEn_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideEn_MSK (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideEn_HSH (0x012F3DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideVal_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideVal_MSK (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_EarlyForceOnOverrideVal_HSH (0x01313DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Skip_voltage_only_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Skip_voltage_only_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Skip_voltage_only_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Skip_voltage_only_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Skip_voltage_only_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Skip_voltage_only_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_Skip_voltage_only_HSH (0x01333DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ovr_gated_fet_ack_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ovr_gated_fet_ack_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ovr_gated_fet_ack_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ovr_gated_fet_ack_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ovr_gated_fet_ack_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ovr_gated_fet_ack_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_SAGMEMMC1_DFT_ovr_gated_fet_ack_HSH (0x01353DB4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_1_OFF  (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_1_WID  ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_1_MSK  (0xF8000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_1_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_1_MAX  (31) // 0x0000001F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_1_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_0_Reserved_1_HSH  (0x05373DB4)

#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_REG               (0x00013DB8)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_DEF               (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideEn_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideEn_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideEn_HSH (0x01013DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideVal_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideVal_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PowerUpParOverrideVal_HSH (0x01033DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideEn_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideEn_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideEn_HSH (0x01053DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideVal_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideVal_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ForceOnParOverrideVal_HSH (0x01073DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_PG_Enable_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_PG_Enable_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_PG_Enable_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_PG_Enable_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_PG_Enable_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_PG_Enable_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_PG_Enable_HSH (0x01093DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_Master_mode_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_Master_mode_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_Master_mode_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_Master_mode_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_Master_mode_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_Master_mode_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_Master_mode_HSH (0x010B3DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_SingleStep_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_SingleStep_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_SingleStep_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_SingleStep_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_SingleStep_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_SingleStep_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC0_DFT_SingleStep_HSH (0x010D3DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideEn_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideEn_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideEn_HSH (0x010F3DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideVal_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideVal_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_PwrGoodOverrideVal_HSH (0x01113DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideEn_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideEn_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideEn_HSH (0x01133DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideVal_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideVal_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_EarlyForceOnOverrideVal_HSH (0x01153DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_Skip_voltage_only_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_Skip_voltage_only_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_Skip_voltage_only_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_Skip_voltage_only_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_Skip_voltage_only_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_Skip_voltage_only_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_Skip_voltage_only_HSH (0x01173DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ovr_gated_fet_ack_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ovr_gated_fet_ack_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ovr_gated_fet_ack_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ovr_gated_fet_ack_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ovr_gated_fet_ack_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ovr_gated_fet_ack_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC0_DFT_ovr_gated_fet_ack_HSH (0x01193DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_0_OFF  (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_0_WID  ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_0_MSK  (0x00006000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_0_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_0_MAX  (3) // 0x00000003
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_0_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_0_HSH  (0x021B3DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideEn_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideEn_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideEn_HSH (0x011F3DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideVal_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideVal_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PowerUpParOverrideVal_HSH (0x01213DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideEn_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideEn_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideEn_HSH (0x01233DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideVal_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideVal_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ForceOnParOverrideVal_HSH (0x01253DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_PG_Enable_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_PG_Enable_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_PG_Enable_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_PG_Enable_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_PG_Enable_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_PG_Enable_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_PG_Enable_HSH (0x01273DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_Master_mode_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_Master_mode_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_Master_mode_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_Master_mode_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_Master_mode_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_Master_mode_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_Master_mode_HSH (0x01293DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_SingleStep_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_SingleStep_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_SingleStep_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_SingleStep_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_SingleStep_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_SingleStep_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTRAINMC1_DFT_SingleStep_HSH (0x012B3DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideEn_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideEn_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideEn_HSH (0x012D3DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideVal_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideVal_MSK (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_PwrGoodOverrideVal_HSH (0x012F3DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideEn_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideEn_MSK (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideEn_HSH (0x01313DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideVal_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideVal_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_EarlyForceOnOverrideVal_HSH (0x01333DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_Skip_voltage_only_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_Skip_voltage_only_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_Skip_voltage_only_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_Skip_voltage_only_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_Skip_voltage_only_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_Skip_voltage_only_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_Skip_voltage_only_HSH (0x01353DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ovr_gated_fet_ack_OFF (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ovr_gated_fet_ack_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ovr_gated_fet_ack_MSK (0x08000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ovr_gated_fet_ack_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ovr_gated_fet_ack_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ovr_gated_fet_ack_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_SAGTrainMC1_DFT_ovr_gated_fet_ack_HSH (0x01373DB8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_1_OFF  (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_1_WID  ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_1_MSK  (0xF0000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_1_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_1_MAX  (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_1_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_1_Reserved_1_HSH  (0x04393DB8)

#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_REG               (0x00013DBC)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_DEF               (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideEn_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideEn_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideEn_HSH (0x01013DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideVal_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideVal_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PowerUpParOverrideVal_HSH (0x01033DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideEn_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideEn_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideEn_HSH (0x01053DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideVal_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideVal_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ForceOnParOverrideVal_HSH (0x01073DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PG_Enable_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PG_Enable_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PG_Enable_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PG_Enable_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PG_Enable_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PG_Enable_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PG_Enable_HSH (0x01093DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Master_mode_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Master_mode_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Master_mode_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Master_mode_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Master_mode_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Master_mode_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Master_mode_HSH (0x010B3DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_SingleStep_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_SingleStep_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_SingleStep_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_SingleStep_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_SingleStep_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_SingleStep_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_SingleStep_HSH (0x010D3DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_0_OFF  ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_0_WID  ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_0_MSK  (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_0_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_0_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_0_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_0_HSH  (0x010F3DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideEn_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideEn_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideEn_HSH (0x01113DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideVal_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideVal_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_PwrGoodOverrideVal_HSH (0x01133DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideEn_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideEn_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideEn_HSH (0x01153DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideVal_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideVal_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_EarlyForceOnOverrideVal_HSH (0x01173DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Skip_voltage_only_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Skip_voltage_only_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Skip_voltage_only_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Skip_voltage_only_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Skip_voltage_only_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Skip_voltage_only_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_Skip_voltage_only_HSH (0x01193DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ovr_gated_fet_ack_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ovr_gated_fet_ack_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ovr_gated_fet_ack_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ovr_gated_fet_ack_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ovr_gated_fet_ack_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ovr_gated_fet_ack_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC0_DFT_ovr_gated_fet_ack_HSH (0x011B3DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_1_OFF  (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_1_WID  ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_1_MSK  (0x0000C000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_1_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_1_MAX  (3) // 0x00000003
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_1_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_1_HSH  (0x021D3DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideEn_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideEn_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideEn_HSH (0x01213DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideVal_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideVal_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PowerUpParOverrideVal_HSH (0x01233DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideEn_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideEn_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideEn_HSH (0x01253DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideVal_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideVal_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ForceOnParOverrideVal_HSH (0x01273DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PG_Enable_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PG_Enable_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PG_Enable_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PG_Enable_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PG_Enable_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PG_Enable_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PG_Enable_HSH (0x01293DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Master_mode_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Master_mode_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Master_mode_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Master_mode_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Master_mode_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Master_mode_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Master_mode_HSH (0x012B3DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_SingleStep_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_SingleStep_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_SingleStep_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_SingleStep_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_SingleStep_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_SingleStep_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_SingleStep_HSH (0x012D3DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_2_OFF  (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_2_WID  ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_2_MSK  (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_2_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_2_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_2_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_2_HSH  (0x012F3DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideEn_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideEn_MSK (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideEn_HSH (0x01313DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideVal_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideVal_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_PwrGoodOverrideVal_HSH (0x01333DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideEn_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideEn_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideEn_HSH (0x01353DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideVal_OFF (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideVal_MSK (0x08000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_EarlyForceOnOverrideVal_HSH (0x01373DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Skip_voltage_only_OFF (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Skip_voltage_only_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Skip_voltage_only_MSK (0x10000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Skip_voltage_only_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Skip_voltage_only_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Skip_voltage_only_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_Skip_voltage_only_HSH (0x01393DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ovr_gated_fet_ack_OFF (29)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ovr_gated_fet_ack_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ovr_gated_fet_ack_MSK (0x20000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ovr_gated_fet_ack_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ovr_gated_fet_ack_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ovr_gated_fet_ack_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_SAGMEMIBECC1_DFT_ovr_gated_fet_ack_HSH (0x013B3DBC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_3_OFF  (30)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_3_WID  ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_3_MSK  (0xC0000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_3_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_3_MAX  (3) // 0x00000003
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_3_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_2_Reserved_3_HSH  (0x023D3DBC)

#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_REG               (0x00013DC0)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_DEF               (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideEn_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideEn_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideEn_HSH (0x01013DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideVal_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideVal_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PowerUpParOverrideVal_HSH (0x01033DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideEn_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideEn_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideEn_HSH (0x01053DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideVal_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideVal_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ForceOnParOverrideVal_HSH (0x01073DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PG_Enable_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PG_Enable_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PG_Enable_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PG_Enable_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PG_Enable_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PG_Enable_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PG_Enable_HSH (0x01093DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Master_mode_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Master_mode_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Master_mode_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Master_mode_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Master_mode_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Master_mode_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Master_mode_HSH (0x010B3DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_SingleStep_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_SingleStep_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_SingleStep_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_SingleStep_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_SingleStep_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_SingleStep_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_SingleStep_HSH (0x010D3DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_0_OFF  ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_0_WID  ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_0_MSK  (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_0_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_0_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_0_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_0_HSH  (0x010F3DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideEn_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideEn_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideEn_HSH (0x01113DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideVal_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideVal_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_PwrGoodOverrideVal_HSH (0x01133DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideEn_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideEn_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideEn_HSH (0x01153DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideVal_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideVal_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_EarlyForceOnOverrideVal_HSH (0x01173DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Skip_voltage_only_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Skip_voltage_only_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Skip_voltage_only_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Skip_voltage_only_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Skip_voltage_only_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Skip_voltage_only_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_Skip_voltage_only_HSH (0x01193DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ovr_gated_fet_ack_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ovr_gated_fet_ack_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ovr_gated_fet_ack_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ovr_gated_fet_ack_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ovr_gated_fet_ack_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ovr_gated_fet_ack_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE0_DFT_ovr_gated_fet_ack_HSH (0x011B3DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_1_OFF  (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_1_WID  ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_1_MSK  (0x0000C000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_1_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_1_MAX  (3) // 0x00000003
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_1_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_1_HSH  (0x021D3DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideEn_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideEn_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideEn_HSH (0x01213DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideVal_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideVal_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PowerUpParOverrideVal_HSH (0x01233DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideEn_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideEn_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideEn_HSH (0x01253DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideVal_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideVal_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ForceOnParOverrideVal_HSH (0x01273DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PG_Enable_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PG_Enable_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PG_Enable_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PG_Enable_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PG_Enable_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PG_Enable_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PG_Enable_HSH (0x01293DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Master_mode_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Master_mode_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Master_mode_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Master_mode_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Master_mode_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Master_mode_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Master_mode_HSH (0x012B3DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_SingleStep_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_SingleStep_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_SingleStep_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_SingleStep_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_SingleStep_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_SingleStep_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_SingleStep_HSH (0x012D3DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_2_OFF  (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_2_WID  ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_2_MSK  (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_2_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_2_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_2_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_2_HSH  (0x012F3DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideEn_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideEn_MSK (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideEn_HSH (0x01313DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideVal_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideVal_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_PwrGoodOverrideVal_HSH (0x01333DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideEn_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideEn_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideEn_HSH (0x01353DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideVal_OFF (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideVal_MSK (0x08000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_EarlyForceOnOverrideVal_HSH (0x01373DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Skip_voltage_only_OFF (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Skip_voltage_only_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Skip_voltage_only_MSK (0x10000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Skip_voltage_only_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Skip_voltage_only_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Skip_voltage_only_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_Skip_voltage_only_HSH (0x01393DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ovr_gated_fet_ack_OFF (29)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ovr_gated_fet_ack_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ovr_gated_fet_ack_MSK (0x20000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ovr_gated_fet_ack_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ovr_gated_fet_ack_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ovr_gated_fet_ack_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_SAGMEMCCE1_DFT_ovr_gated_fet_ack_HSH (0x013B3DC0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_3_OFF  (30)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_3_WID  ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_3_MSK  (0xC0000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_3_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_3_MAX  (3) // 0x00000003
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_3_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_3_Reserved_3_HSH  (0x023D3DC0)

#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_REG               (0x00013DC4)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_DEF               (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideEn_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideEn_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideEn_HSH (0x01013DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideVal_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideVal_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PowerUpParOverrideVal_HSH (0x01033DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideEn_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideEn_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideEn_HSH (0x01053DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideVal_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideVal_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ForceOnParOverrideVal_HSH (0x01073DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PG_Enable_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PG_Enable_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PG_Enable_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PG_Enable_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PG_Enable_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PG_Enable_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PG_Enable_HSH (0x01093DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Master_mode_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Master_mode_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Master_mode_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Master_mode_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Master_mode_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Master_mode_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Master_mode_HSH (0x010B3DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_SingleStep_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_SingleStep_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_SingleStep_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_SingleStep_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_SingleStep_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_SingleStep_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_SingleStep_HSH (0x010D3DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideEn_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideEn_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideEn_HSH (0x010F3DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideVal_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideVal_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_PwrGoodOverrideVal_HSH (0x01113DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideEn_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideEn_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideEn_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideEn_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideEn_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideEn_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideEn_HSH (0x01133DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideVal_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideVal_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideVal_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideVal_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideVal_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideVal_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_EarlyForceOnOverrideVal_HSH (0x01153DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Skip_voltage_only_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Skip_voltage_only_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Skip_voltage_only_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Skip_voltage_only_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Skip_voltage_only_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Skip_voltage_only_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_Skip_voltage_only_HSH (0x01173DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ovr_gated_fet_ack_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ovr_gated_fet_ack_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ovr_gated_fet_ack_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ovr_gated_fet_ack_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ovr_gated_fet_ack_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ovr_gated_fet_ack_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_SAGMEMONDD_DFT_ovr_gated_fet_ack_HSH (0x01193DC4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_Reserved_1_OFF  (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_Reserved_1_WID  (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_Reserved_1_MSK  (0xFFFFE000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_Reserved_1_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_Reserved_1_MAX  (524287) // 0x0007FFFF
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_Reserved_1_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_OVERRIDES_4_Reserved_1_HSH  (0x131B3DC4)

#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_REG           (0x00013DC8)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_DEF           (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_VOLATILE_BITFIELDS_MSK (0xFFFFFFFF)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC0_DFT_CurrentState_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC0_DFT_CurrentState_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC0_DFT_CurrentState_MSK (0x0000000F)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC0_DFT_CurrentState_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC0_DFT_CurrentState_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC0_DFT_CurrentState_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC0_DFT_CurrentState_HSH (0x04813DC8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC1_DFT_CurrentState_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC1_DFT_CurrentState_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC1_DFT_CurrentState_MSK (0x000000F0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC1_DFT_CurrentState_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC1_DFT_CurrentState_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC1_DFT_CurrentState_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMMC1_DFT_CurrentState_HSH (0x04893DC8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC0_DFT_CurrentState_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC0_DFT_CurrentState_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC0_DFT_CurrentState_MSK (0x00000F00)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC0_DFT_CurrentState_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC0_DFT_CurrentState_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC0_DFT_CurrentState_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC0_DFT_CurrentState_HSH (0x04913DC8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC1_DFT_CurrentState_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC1_DFT_CurrentState_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC1_DFT_CurrentState_MSK (0x0000F000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC1_DFT_CurrentState_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC1_DFT_CurrentState_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC1_DFT_CurrentState_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGTrainMC1_DFT_CurrentState_HSH (0x04993DC8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC0_DFT_CurrentState_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC0_DFT_CurrentState_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC0_DFT_CurrentState_MSK (0x000F0000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC0_DFT_CurrentState_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC0_DFT_CurrentState_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC0_DFT_CurrentState_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC0_DFT_CurrentState_HSH (0x04A13DC8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC1_DFT_CurrentState_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC1_DFT_CurrentState_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC1_DFT_CurrentState_MSK (0x00F00000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC1_DFT_CurrentState_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC1_DFT_CurrentState_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC1_DFT_CurrentState_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMIBECC1_DFT_CurrentState_HSH (0x04A93DC8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE0_DFT_CurrentState_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE0_DFT_CurrentState_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE0_DFT_CurrentState_MSK (0x0F000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE0_DFT_CurrentState_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE0_DFT_CurrentState_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE0_DFT_CurrentState_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE0_DFT_CurrentState_HSH (0x04B13DC8)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE1_DFT_CurrentState_OFF (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE1_DFT_CurrentState_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE1_DFT_CurrentState_MSK (0xF0000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE1_DFT_CurrentState_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE1_DFT_CurrentState_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE1_DFT_CurrentState_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_0_SAGMEMCCE1_DFT_CurrentState_HSH (0x04B93DC8)

#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_REG           (0x00013DCC)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_DEF           (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_VOLATILE_BITFIELDS_MSK (0x00001FFF)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC0_DFT_PG_in_Trans_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC0_DFT_PG_in_Trans_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC0_DFT_PG_in_Trans_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC0_DFT_PG_in_Trans_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC0_DFT_PG_in_Trans_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC0_DFT_PG_in_Trans_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC0_DFT_PG_in_Trans_HSH (0x01813DCC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC1_DFT_PG_in_Trans_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC1_DFT_PG_in_Trans_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC1_DFT_PG_in_Trans_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC1_DFT_PG_in_Trans_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC1_DFT_PG_in_Trans_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC1_DFT_PG_in_Trans_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMMC1_DFT_PG_in_Trans_HSH (0x01833DCC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC0_DFT_PG_in_Trans_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC0_DFT_PG_in_Trans_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC0_DFT_PG_in_Trans_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC0_DFT_PG_in_Trans_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC0_DFT_PG_in_Trans_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC0_DFT_PG_in_Trans_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC0_DFT_PG_in_Trans_HSH (0x01853DCC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC1_DFT_PG_in_Trans_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC1_DFT_PG_in_Trans_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC1_DFT_PG_in_Trans_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC1_DFT_PG_in_Trans_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC1_DFT_PG_in_Trans_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC1_DFT_PG_in_Trans_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGTrainMC1_DFT_PG_in_Trans_HSH (0x01873DCC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC0_DFT_PG_in_Trans_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC0_DFT_PG_in_Trans_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC0_DFT_PG_in_Trans_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC0_DFT_PG_in_Trans_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC0_DFT_PG_in_Trans_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC0_DFT_PG_in_Trans_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC0_DFT_PG_in_Trans_HSH (0x01893DCC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC1_DFT_PG_in_Trans_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC1_DFT_PG_in_Trans_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC1_DFT_PG_in_Trans_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC1_DFT_PG_in_Trans_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC1_DFT_PG_in_Trans_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC1_DFT_PG_in_Trans_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMIBECC1_DFT_PG_in_Trans_HSH (0x018B3DCC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE0_DFT_PG_in_Trans_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE0_DFT_PG_in_Trans_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE0_DFT_PG_in_Trans_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE0_DFT_PG_in_Trans_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE0_DFT_PG_in_Trans_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE0_DFT_PG_in_Trans_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE0_DFT_PG_in_Trans_HSH (0x018D3DCC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE1_DFT_PG_in_Trans_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE1_DFT_PG_in_Trans_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE1_DFT_PG_in_Trans_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE1_DFT_PG_in_Trans_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE1_DFT_PG_in_Trans_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE1_DFT_PG_in_Trans_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMCCE1_DFT_PG_in_Trans_HSH (0x018F3DCC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_PG_in_Trans_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_PG_in_Trans_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_PG_in_Trans_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_PG_in_Trans_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_PG_in_Trans_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_PG_in_Trans_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_PG_in_Trans_HSH (0x01913DCC)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_CurrentState_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_CurrentState_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_CurrentState_MSK (0x00001E00)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_CurrentState_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_CurrentState_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_CurrentState_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_OUTPUTS_1_SAGMEMONDD_DFT_CurrentState_HSH (0x04933DCC)

#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_REG                (0x00013DD0)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_DEF                (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_scan_iso_en_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_scan_iso_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_scan_iso_en_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_scan_iso_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_scan_iso_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_scan_iso_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_scan_iso_en_HSH (0x01013DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_byprst_b_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_byprst_b_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_byprst_b_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_byprst_b_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_byprst_b_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_byprst_b_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_byprst_b_HSH (0x01033DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_rstbypen_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_rstbypen_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_rstbypen_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_rstbypen_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_rstbypen_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_rstbypen_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC0_fscan_rstbypen_HSH (0x01053DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_scan_iso_en_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_scan_iso_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_scan_iso_en_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_scan_iso_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_scan_iso_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_scan_iso_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_scan_iso_en_HSH (0x01073DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_byprst_b_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_byprst_b_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_byprst_b_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_byprst_b_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_byprst_b_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_byprst_b_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_byprst_b_HSH (0x01093DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_rstbypen_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_rstbypen_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_rstbypen_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_rstbypen_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_rstbypen_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_rstbypen_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMMC1_fscan_rstbypen_HSH (0x010B3DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_scan_iso_en_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_scan_iso_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_scan_iso_en_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_scan_iso_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_scan_iso_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_scan_iso_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_scan_iso_en_HSH (0x010D3DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_byprst_b_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_byprst_b_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_byprst_b_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_byprst_b_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_byprst_b_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_byprst_b_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_byprst_b_HSH (0x010F3DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_rstbypen_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_rstbypen_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_rstbypen_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_rstbypen_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_rstbypen_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_rstbypen_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC0_fscan_rstbypen_HSH (0x01113DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_scan_iso_en_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_scan_iso_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_scan_iso_en_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_scan_iso_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_scan_iso_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_scan_iso_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_scan_iso_en_HSH (0x01133DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_byprst_b_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_byprst_b_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_byprst_b_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_byprst_b_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_byprst_b_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_byprst_b_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_byprst_b_HSH (0x01153DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_rstbypen_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_rstbypen_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_rstbypen_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_rstbypen_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_rstbypen_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_rstbypen_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGTrainMC1_fscan_rstbypen_HSH (0x01173DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_scan_iso_en_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_scan_iso_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_scan_iso_en_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_scan_iso_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_scan_iso_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_scan_iso_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_scan_iso_en_HSH (0x01193DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_byprst_b_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_byprst_b_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_byprst_b_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_byprst_b_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_byprst_b_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_byprst_b_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_byprst_b_HSH (0x011B3DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_rstbypen_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_rstbypen_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_rstbypen_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_rstbypen_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_rstbypen_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_rstbypen_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC0_fscan_rstbypen_HSH (0x011D3DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_scan_iso_en_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_scan_iso_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_scan_iso_en_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_scan_iso_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_scan_iso_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_scan_iso_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_scan_iso_en_HSH (0x011F3DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_byprst_b_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_byprst_b_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_byprst_b_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_byprst_b_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_byprst_b_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_byprst_b_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_byprst_b_HSH (0x01213DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_rstbypen_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_rstbypen_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_rstbypen_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_rstbypen_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_rstbypen_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_rstbypen_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMIBECC1_fscan_rstbypen_HSH (0x01233DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_scan_iso_en_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_scan_iso_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_scan_iso_en_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_scan_iso_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_scan_iso_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_scan_iso_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_scan_iso_en_HSH (0x01253DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_byprst_b_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_byprst_b_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_byprst_b_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_byprst_b_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_byprst_b_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_byprst_b_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_byprst_b_HSH (0x01273DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_rstbypen_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_rstbypen_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_rstbypen_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_rstbypen_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_rstbypen_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_rstbypen_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE0_fscan_rstbypen_HSH (0x01293DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_scan_iso_en_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_scan_iso_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_scan_iso_en_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_scan_iso_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_scan_iso_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_scan_iso_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_scan_iso_en_HSH (0x012B3DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_byprst_b_OFF (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_byprst_b_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_byprst_b_MSK (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_byprst_b_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_byprst_b_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_byprst_b_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_byprst_b_HSH (0x012D3DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_rstbypen_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_rstbypen_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_rstbypen_MSK (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_rstbypen_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_rstbypen_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_rstbypen_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMCCE1_fscan_rstbypen_HSH (0x012F3DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_scan_iso_en_OFF (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_scan_iso_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_scan_iso_en_MSK (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_scan_iso_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_scan_iso_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_scan_iso_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_scan_iso_en_HSH (0x01313DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_byprst_b_OFF (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_byprst_b_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_byprst_b_MSK (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_byprst_b_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_byprst_b_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_byprst_b_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_byprst_b_HSH (0x01333DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_rstbypen_OFF (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_rstbypen_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_rstbypen_MSK (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_rstbypen_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_rstbypen_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_rstbypen_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO0_fscan_rstbypen_HSH (0x01353DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_scan_iso_en_OFF (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_scan_iso_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_scan_iso_en_MSK (0x08000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_scan_iso_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_scan_iso_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_scan_iso_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_scan_iso_en_HSH (0x01373DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_byprst_b_OFF (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_byprst_b_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_byprst_b_MSK (0x10000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_byprst_b_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_byprst_b_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_byprst_b_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_byprst_b_HSH (0x01393DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_rstbypen_OFF (29)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_rstbypen_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_rstbypen_MSK (0x20000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_rstbypen_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_rstbypen_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_rstbypen_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_SAGMEMHBO1_fscan_rstbypen_HSH (0x013B3DD0)

  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_Reserved_OFF     (30)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_Reserved_WID     ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_Reserved_MSK     (0xC0000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_Reserved_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_Reserved_MAX     (3) // 0x00000003
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_Reserved_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PGFSM_DEBUG_SCAN_Reserved_HSH     (0x023D3DD0)

#define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_REG                           (0x00013DD4)
#define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_DEF                           (0x00000000)
#define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_VOLATILE_BITFIELDS_MSK        (0x0000001F)

  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_FORCE_IND_OFF               ( 0)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_FORCE_IND_WID               ( 1)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_FORCE_IND_MSK               (0x00000001)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_FORCE_IND_MIN               (0)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_FORCE_IND_MAX               (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_FORCE_IND_DEF               (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_FORCE_IND_HSH               (0x01813DD4)

  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_VCCMEMSS_ELECTRICAL_OFF     ( 1)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_VCCMEMSS_ELECTRICAL_WID     ( 1)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_VCCMEMSS_ELECTRICAL_MSK     (0x00000002)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_VCCMEMSS_ELECTRICAL_MIN     (0)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_VCCMEMSS_ELECTRICAL_MAX     (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_VCCMEMSS_ELECTRICAL_DEF     (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_VCCMEMSS_ELECTRICAL_HSH     (0x01833DD4)

  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMUP_SERVICE_OFF           ( 2)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMUP_SERVICE_WID           ( 1)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMUP_SERVICE_MSK           (0x00000004)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMUP_SERVICE_MIN           (0)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMUP_SERVICE_MAX           (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMUP_SERVICE_DEF           (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMUP_SERVICE_HSH           (0x01853DD4)

  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMCLK_SERVICE_OFF          ( 3)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMCLK_SERVICE_WID          ( 1)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMCLK_SERVICE_MSK          (0x00000008)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMCLK_SERVICE_MIN          (0)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMCLK_SERVICE_MAX          (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMCLK_SERVICE_DEF          (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_MEMCLK_SERVICE_HSH          (0x01873DD4)

  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_WARM_RESET_SERVICE_OFF      ( 4)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_WARM_RESET_SERVICE_WID      ( 1)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_WARM_RESET_SERVICE_MSK      (0x00000010)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_WARM_RESET_SERVICE_MIN      (0)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_WARM_RESET_SERVICE_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_WARM_RESET_SERVICE_DEF      (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_WARM_RESET_SERVICE_HSH      (0x01893DD4)

  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_Reserved_OFF                ( 5)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_Reserved_WID                (27)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_Reserved_MSK                (0xFFFFFFE0)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_Reserved_MIN                (0)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_Reserved_MAX                (134217727) // 0x07FFFFFF
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_Reserved_DEF                (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_SL_REQ_CONFIG_Reserved_HSH                (0x1B0B3DD4)

#define MEMSS_PMA_CR_PUNIT_REQ_DATA_REG                                (0x00013DD8)
#define MEMSS_PMA_CR_PUNIT_REQ_DATA_DEF                                (0x00000000)
#define MEMSS_PMA_CR_PUNIT_REQ_DATA_VOLATILE_BITFIELDS_MSK             (0x001FFFFF)

  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_QCLK_CURRENT_IDX_OFF             ( 0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_QCLK_CURRENT_IDX_WID             ( 2)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_QCLK_CURRENT_IDX_MSK             (0x00000003)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_QCLK_CURRENT_IDX_MIN             (0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_QCLK_CURRENT_IDX_MAX             (3) // 0x00000003
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_QCLK_CURRENT_IDX_DEF             (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_QCLK_CURRENT_IDX_HSH             (0x02813DD8)

  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_CMI_PLL_PLLRATIO_OFF             ( 2)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_CMI_PLL_PLLRATIO_WID             ( 8)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_CMI_PLL_PLLRATIO_MSK             (0x000003FC)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_CMI_PLL_PLLRATIO_MIN             (0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_CMI_PLL_PLLRATIO_MAX             (255) // 0x000000FF
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_CMI_PLL_PLLRATIO_DEF             (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_CMI_PLL_PLLRATIO_HSH             (0x08853DD8)

  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_GEAR_TYPE_OFF                    (10)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_GEAR_TYPE_WID                    ( 1)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_GEAR_TYPE_MSK                    (0x00000400)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_GEAR_TYPE_MIN                    (0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_GEAR_TYPE_MAX                    (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_GEAR_TYPE_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_GEAR_TYPE_HSH                    (0x01953DD8)

  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_DVFS_VDDQ_OFF                    (11)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_DVFS_VDDQ_WID                    ( 5)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_DVFS_VDDQ_MSK                    (0x0000F800)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_DVFS_VDDQ_MIN                    (0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_DVFS_VDDQ_MAX                    (31) // 0x0000001F
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_DVFS_VDDQ_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_DVFS_VDDQ_HSH                    (0x05973DD8)

  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_FORCE_IND_OFF                    (16)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_FORCE_IND_WID                    ( 1)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_FORCE_IND_MSK                    (0x00010000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_FORCE_IND_MIN                    (0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_FORCE_IND_MAX                    (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_FORCE_IND_DEF                    (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_FORCE_IND_HSH                    (0x01A13DD8)

  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_VCCMEMSS_ELECTRICAL_OFF          (17)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_VCCMEMSS_ELECTRICAL_WID          ( 1)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_VCCMEMSS_ELECTRICAL_MSK          (0x00020000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_VCCMEMSS_ELECTRICAL_MIN          (0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_VCCMEMSS_ELECTRICAL_MAX          (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_VCCMEMSS_ELECTRICAL_DEF          (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_VCCMEMSS_ELECTRICAL_HSH          (0x01A33DD8)

  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMUP_SERVICE_OFF                (18)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMUP_SERVICE_WID                ( 1)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMUP_SERVICE_MSK                (0x00040000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMUP_SERVICE_MIN                (0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMUP_SERVICE_MAX                (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMUP_SERVICE_DEF                (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMUP_SERVICE_HSH                (0x01A53DD8)

  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMCLK_SERVICE_OFF               (19)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMCLK_SERVICE_WID               ( 1)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMCLK_SERVICE_MSK               (0x00080000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMCLK_SERVICE_MIN               (0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMCLK_SERVICE_MAX               (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMCLK_SERVICE_DEF               (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_MEMCLK_SERVICE_HSH               (0x01A73DD8)

  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_WARM_RESET_SERVICE_OFF           (20)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_WARM_RESET_SERVICE_WID           ( 1)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_WARM_RESET_SERVICE_MSK           (0x00100000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_WARM_RESET_SERVICE_MIN           (0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_WARM_RESET_SERVICE_MAX           (1) // 0x00000001
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_WARM_RESET_SERVICE_DEF           (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_WARM_RESET_SERVICE_HSH           (0x01A93DD8)

  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_Reserved_OFF                     (21)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_Reserved_WID                     (11)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_Reserved_MSK                     (0xFFE00000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_Reserved_MIN                     (0)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_Reserved_MAX                     (2047) // 0x000007FF
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_Reserved_DEF                     (0x00000000)
  #define MEMSS_PMA_CR_PUNIT_REQ_DATA_Reserved_HSH                     (0x0B2B3DD8)

#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_REG                  (0x00013DDC)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_DEF                  (0x07FF7FFF)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_Start_to_nComplete_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_Start_to_nComplete_WID (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_Start_to_nComplete_MSK (0x0000FFFF)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_Start_to_nComplete_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_Start_to_nComplete_MAX (65535) // 0x0000FFFF
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_Start_to_nComplete_DEF (0x00007FFF)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_Start_to_nComplete_HSH (0x10013DDC)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_nStart_to_Complete_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_nStart_to_Complete_WID (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_nStart_to_Complete_MSK (0xFFFF0000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_nStart_to_Complete_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_nStart_to_Complete_MAX (65535) // 0x0000FFFF
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_nStart_to_Complete_DEF (0x000007FF)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_3_ddrphy_PM12_Message_nStart_to_Complete_HSH (0x10213DDC)

#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_REG                  (0x00013DE0)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_DEF                  (0x7FFF7FFF)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Message_Preq_to_Paccept_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Message_Preq_to_Paccept_WID (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Message_Preq_to_Paccept_MSK (0x0000FFFF)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Message_Preq_to_Paccept_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Message_Preq_to_Paccept_MAX (65535) // 0x0000FFFF
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Message_Preq_to_Paccept_DEF (0x00007FFF)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Message_Preq_to_Paccept_HSH (0x10013DE0)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Messages_nPreq_to_nPaccept_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Messages_nPreq_to_nPaccept_WID (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Messages_nPreq_to_nPaccept_MSK (0xFFFF0000)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Messages_nPreq_to_nPaccept_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Messages_nPreq_to_nPaccept_MAX (65535) // 0x0000FFFF
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Messages_nPreq_to_nPaccept_DEF (0x00007FFF)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_4_ddrphy_PM_Messages_nPreq_to_nPaccept_HSH (0x10213DE0)

#define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_REG             (0x00013DE4)
#define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DEF             (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_VOLATILE_BITFIELDS_MSK (0x000FFF37)

  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_FSM_STATUS_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_FSM_STATUS_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_FSM_STATUS_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_FSM_STATUS_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_FSM_STATUS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_FSM_STATUS_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_FSM_STATUS_HSH (0x01813DE4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_REQ_TO_ACC_TIMEOUT_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_REQ_TO_ACC_TIMEOUT_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_REQ_TO_ACC_TIMEOUT_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_REQ_TO_ACC_TIMEOUT_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_REQ_TO_ACC_TIMEOUT_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_REQ_TO_ACC_TIMEOUT_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_REQ_TO_ACC_TIMEOUT_HSH (0x01833DE4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_nREQ_TO_nACC_TIMEOUT_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_nREQ_TO_nACC_TIMEOUT_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_nREQ_TO_nACC_TIMEOUT_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_nREQ_TO_nACC_TIMEOUT_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_nREQ_TO_nACC_TIMEOUT_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_nREQ_TO_nACC_TIMEOUT_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_nREQ_TO_nACC_TIMEOUT_HSH (0x01853DE4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCH_DENY_STS_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCH_DENY_STS_WID ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCH_DENY_STS_MSK (0x00000030)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCH_DENY_STS_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCH_DENY_STS_MAX (3) // 0x00000003
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCH_DENY_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCH_DENY_STS_HSH (0x02893DE4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_PACTIVE_VAL_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_PACTIVE_VAL_WID ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_PACTIVE_VAL_MSK (0x0000FF00)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_PACTIVE_VAL_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_PACTIVE_VAL_MAX (255) // 0x000000FF
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_PACTIVE_VAL_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_PACTIVE_VAL_HSH (0x08913DE4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_FSM_STATUS_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_FSM_STATUS_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_FSM_STATUS_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_FSM_STATUS_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_FSM_STATUS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_FSM_STATUS_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_FSM_STATUS_HSH (0x01A13DE4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_START_TO_nCOMPLETE_TIMEOUT_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_START_TO_nCOMPLETE_TIMEOUT_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_START_TO_nCOMPLETE_TIMEOUT_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_START_TO_nCOMPLETE_TIMEOUT_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_START_TO_nCOMPLETE_TIMEOUT_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_START_TO_nCOMPLETE_TIMEOUT_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_START_TO_nCOMPLETE_TIMEOUT_HSH (0x01A33DE4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_nSTART_TO_COMPLETE_TIMEOUT_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_nSTART_TO_COMPLETE_TIMEOUT_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_nSTART_TO_COMPLETE_TIMEOUT_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_nSTART_TO_COMPLETE_TIMEOUT_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_nSTART_TO_COMPLETE_TIMEOUT_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_nSTART_TO_COMPLETE_TIMEOUT_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_DFI_nSTART_TO_COMPLETE_TIMEOUT_HSH (0x01A53DE4)

  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_BAD_PSTATE_REQ_STS_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_BAD_PSTATE_REQ_STS_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_BAD_PSTATE_REQ_STS_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_BAD_PSTATE_REQ_STS_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_BAD_PSTATE_REQ_STS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_BAD_PSTATE_REQ_STS_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_PCHANNEL_DFI_STATUS_PCHANNEL_BAD_PSTATE_REQ_STS_HSH (0x01A73DE4)

  #define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_REG           (0x00013DE8)
  #define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_DEF           (0x00000000)
  #define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_VOLATILE_BITFIELDS_MSK (0xFFFFFFFF)

  #define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_Count_OFF   ( 0)
  #define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_Count_WID   (32)
  #define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_Count_MSK   (0xFFFFFFFF)
  #define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_Count_MIN   (0)
  #define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_Count_MAX   (4294967295) // 0xFFFFFFFF
  #define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_Count_DEF   (0x00000000)
  #define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_Count_HSH   (0x20813DE8)

#define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP1_REG           (0x00013DEC)
//Duplicate of MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_REG

#define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP2_REG           (0x00013DF0)
//Duplicate of MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_REG

#define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP3_REG           (0x00013DF4)
//Duplicate of MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_REG

#define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC1_GVP0_REG           (0x00013DF8)
//Duplicate of MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_REG

#define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC1_GVP1_REG           (0x00013DFC)
//Duplicate of MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_REG

#define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC1_GVP2_REG           (0x00013E00)
//Duplicate of MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_REG

#define MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC1_GVP3_REG           (0x00013E04)
//Duplicate of MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_REG

#define MEMSS_PMA_CR_SURVIVABILITY_MISC_REG                            (0x00013E08)
#define MEMSS_PMA_CR_SURVIVABILITY_MISC_DEF                            (0x0D005000)
#define MEMSS_PMA_CR_SURVIVABILITY_MISC_VOLATILE_BITFIELDS_MSK         (0x00008040)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_OPP_SR_DISABLE_OFF      ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_OPP_SR_DISABLE_WID      ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_OPP_SR_DISABLE_MSK      (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_OPP_SR_DISABLE_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_OPP_SR_DISABLE_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_OPP_SR_DISABLE_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_OPP_SR_DISABLE_HSH      (0x01013E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_VRCG_DISABLE_OFF        ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_VRCG_DISABLE_WID        ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_VRCG_DISABLE_MSK        (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_VRCG_DISABLE_MIN        (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_VRCG_DISABLE_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_VRCG_DISABLE_DEF        (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_VRCG_DISABLE_HSH        (0x01033E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_FORCE_FAKE_GV_EN_OFF    ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_FORCE_FAKE_GV_EN_WID    ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_FORCE_FAKE_GV_EN_MSK    (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_FORCE_FAKE_GV_EN_MIN    (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_FORCE_FAKE_GV_EN_MAX    (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_FORCE_FAKE_GV_EN_DEF    (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SAGV_FORCE_FAKE_GV_EN_HSH    (0x01053E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_AON_SBEP_CLKREQ_OFF          ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_AON_SBEP_CLKREQ_WID          ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_AON_SBEP_CLKREQ_MSK          (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_AON_SBEP_CLKREQ_MIN          (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_AON_SBEP_CLKREQ_MAX          (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_AON_SBEP_CLKREQ_DEF          (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_AON_SBEP_CLKREQ_HSH          (0x01073E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC3_PM3_NOT_PM6_OFF        ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC3_PM3_NOT_PM6_WID        ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC3_PM3_NOT_PM6_MSK        (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC3_PM3_NOT_PM6_MIN        (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC3_PM3_NOT_PM6_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC3_PM3_NOT_PM6_DEF        (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC3_PM3_NOT_PM6_HSH        (0x01093E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC8_PM3_NOT_PM6_OFF        ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC8_PM3_NOT_PM6_WID        ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC8_PM3_NOT_PM6_MSK        (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC8_PM3_NOT_PM6_MIN        (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC8_PM3_NOT_PM6_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC8_PM3_NOT_PM6_DEF        (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_PKGC8_PM3_NOT_PM6_HSH        (0x010B3E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_RUN_BUSY_OFF   ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_RUN_BUSY_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_RUN_BUSY_MSK   (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_RUN_BUSY_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_RUN_BUSY_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_RUN_BUSY_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_RUN_BUSY_HSH   (0x018D3E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_SEL_OFF        ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_SEL_WID        ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_SEL_MSK        (0x00000780)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_SEL_MIN        (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_SEL_MAX        (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_SEL_DEF        (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_DDRPHY_PM_MSG_SEL_HSH        (0x040F3E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_WAIT_CYCLES_OFF    (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_WAIT_CYCLES_WID    ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_WAIT_CYCLES_MSK    (0x00007800)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_WAIT_CYCLES_MIN    (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_WAIT_CYCLES_MAX    (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_WAIT_CYCLES_DEF    (0x0000000A)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_WAIT_CYCLES_HSH    (0x04173E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_RUN_BUSY_OFF       (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_RUN_BUSY_WID       ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_RUN_BUSY_MSK       (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_RUN_BUSY_MIN       (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_RUN_BUSY_MAX       (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_RUN_BUSY_DEF       (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_RUN_BUSY_HSH       (0x019F3E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_IP_SEL_OFF         (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_IP_SEL_WID         ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_IP_SEL_MSK         (0x003F0000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_IP_SEL_MIN         (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_IP_SEL_MAX         (63) // 0x0000003F
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_IP_SEL_DEF         (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_IP_SEL_HSH         (0x06213E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_DIRECTION_OFF      (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_DIRECTION_WID      ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_DIRECTION_MSK      (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_DIRECTION_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_DIRECTION_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_DIRECTION_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_DIRECTION_HSH      (0x012D3E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_SLEEP_2_POWERGOOD_DLY_OFF (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_SLEEP_2_POWERGOOD_DLY_WID ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_SLEEP_2_POWERGOOD_DLY_MSK (0x07800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_SLEEP_2_POWERGOOD_DLY_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_SLEEP_2_POWERGOOD_DLY_MAX (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_SLEEP_2_POWERGOOD_DLY_DEF (0x0000000A)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_RETENTION_SLEEP_2_POWERGOOD_DLY_HSH (0x042F3E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_TELEMETRY_CNTRS_EN_OFF       (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_TELEMETRY_CNTRS_EN_WID       ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_TELEMETRY_CNTRS_EN_MSK       (0x08000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_TELEMETRY_CNTRS_EN_MIN       (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_TELEMETRY_CNTRS_EN_MAX       (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_TELEMETRY_CNTRS_EN_DEF       (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_TELEMETRY_CNTRS_EN_HSH       (0x01373E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MEMPLL_LOCK_UNLOCK_OFF  (28)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MEMPLL_LOCK_UNLOCK_WID  ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MEMPLL_LOCK_UNLOCK_MSK  (0x10000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MEMPLL_LOCK_UNLOCK_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MEMPLL_LOCK_UNLOCK_MAX  (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MEMPLL_LOCK_UNLOCK_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MEMPLL_LOCK_UNLOCK_HSH  (0x01393E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_ISO_OFF              (29)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_ISO_WID              ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_ISO_MSK              (0x20000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_ISO_MIN              (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_ISO_MAX              (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_ISO_DEF              (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_ISO_HSH              (0x013B3E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_GV_RESTORE_OFF       (30)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_GV_RESTORE_WID       ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_GV_RESTORE_MSK       (0x40000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_GV_RESTORE_MIN       (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_GV_RESTORE_MAX       (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_GV_RESTORE_DEF       (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_SKIP_MC_GV_RESTORE_HSH       (0x013D3E08)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_CBIT_IGNORE_ONDD_UNBLOCK_ACK_OFF (31)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_CBIT_IGNORE_ONDD_UNBLOCK_ACK_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_CBIT_IGNORE_ONDD_UNBLOCK_ACK_MSK (0x80000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_CBIT_IGNORE_ONDD_UNBLOCK_ACK_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_CBIT_IGNORE_ONDD_UNBLOCK_ACK_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_CBIT_IGNORE_ONDD_UNBLOCK_ACK_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_CBIT_IGNORE_ONDD_UNBLOCK_ACK_HSH (0x013F3E08)

#define PKG_MC_C0_LO_REG                                               (0x00013E0C)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define PKG_MC_C0_HI_REG                                               (0x00013E10)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define PKG_MC_C0_RATIO_LO_REG                                         (0x00013E14)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define PKG_MC_C0_RATIO_HI_REG                                         (0x00013E18)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define SAGV_PERFCOUNTER_0_LO_REG                                      (0x00013E1C)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define SAGV_PERFCOUNTER_0_HI_REG                                      (0x00013E20)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define SAGV_PERFCOUNTER_1_LO_REG                                      (0x00013E24)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define SAGV_PERFCOUNTER_1_HI_REG                                      (0x00013E28)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define SAGV_PERFCOUNTER_2_LO_REG                                      (0x00013E2C)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define SAGV_PERFCOUNTER_2_HI_REG                                      (0x00013E30)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define SAGV_PERFCOUNTER_3_LO_REG                                      (0x00013E34)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define SAGV_PERFCOUNTER_3_HI_REG                                      (0x00013E38)
//Duplicate of MC0_REQ0_CR_CPGC_SEQ_ROW_ADDR_DQ_MAP0_REG

#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_REG               (0x00013E3C)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_DEF               (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_gv_in_progress_b_en_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_gv_in_progress_b_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_gv_in_progress_b_en_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_gv_in_progress_b_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_gv_in_progress_b_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_gv_in_progress_b_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_gv_in_progress_b_en_HSH (0x01013E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_preq_en_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_preq_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_preq_en_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_preq_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_preq_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_preq_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_preq_en_HSH (0x01033E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_pstate_en_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_pstate_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_pstate_en_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_pstate_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_pstate_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_pstate_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_pma_phy_pstate_en_HSH (0x01053E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_init_start_en_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_init_start_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_init_start_en_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_init_start_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_init_start_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_init_start_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_init_start_en_HSH (0x010B3E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_frequency_en_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_frequency_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_frequency_en_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_frequency_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_frequency_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_frequency_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_dfi_frequency_en_HSH (0x010D3E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_NOC_BYPASS_EN_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_NOC_BYPASS_EN_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_NOC_BYPASS_EN_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_NOC_BYPASS_EN_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_NOC_BYPASS_EN_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_NOC_BYPASS_EN_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_NOC_BYPASS_EN_HSH (0x01173E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc0_immediate_sleep_level_rsp_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc0_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc0_immediate_sleep_level_rsp_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc0_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc0_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc0_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc0_immediate_sleep_level_rsp_HSH (0x01193E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc1_immediate_sleep_level_rsp_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc1_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc1_immediate_sleep_level_rsp_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc1_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc1_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc1_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mc1_immediate_sleep_level_rsp_HSH (0x011B3E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_immediate_sleep_level_rsp_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_immediate_sleep_level_rsp_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_immediate_sleep_level_rsp_HSH (0x011D3E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_immediate_sleep_level_rsp_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_immediate_sleep_level_rsp_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_immediate_sleep_level_rsp_HSH (0x011F3E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc0_immediate_sleep_level_rsp_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc0_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc0_immediate_sleep_level_rsp_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc0_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc0_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc0_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc0_immediate_sleep_level_rsp_HSH (0x01213E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc1_immediate_sleep_level_rsp_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc1_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc1_immediate_sleep_level_rsp_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc1_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc1_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc1_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_ibecc1_immediate_sleep_level_rsp_HSH (0x01233E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mctrain_clkgate_ovrd_en_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mctrain_clkgate_ovrd_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mctrain_clkgate_ovrd_en_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mctrain_clkgate_ovrd_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mctrain_clkgate_ovrd_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mctrain_clkgate_ovrd_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_mctrain_clkgate_ovrd_en_HSH (0x01253E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_shutoff_ovrd_en_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_shutoff_ovrd_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_shutoff_ovrd_en_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_shutoff_ovrd_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_shutoff_ovrd_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_shutoff_ovrd_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce0_shutoff_ovrd_en_HSH (0x01273E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_shutoff_ovrd_en_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_shutoff_ovrd_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_shutoff_ovrd_en_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_shutoff_ovrd_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_shutoff_ovrd_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_shutoff_ovrd_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_cce1_shutoff_ovrd_en_HSH (0x01293E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_global_clken_ovrd_ovrd_en_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_global_clken_ovrd_ovrd_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_global_clken_ovrd_ovrd_en_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_global_clken_ovrd_ovrd_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_global_clken_ovrd_ovrd_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_global_clken_ovrd_ovrd_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_global_clken_ovrd_ovrd_en_HSH (0x012B3E3C)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_Reserved_OFF    (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_Reserved_WID    (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_Reserved_MSK    (0xFFC00000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_Reserved_MIN    (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_Reserved_MAX    (1023) // 0x000003FF
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_Reserved_DEF    (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_ENABLE_3_Reserved_HSH    (0x0A2D3E3C)

#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_REG                (0x00013E40)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_DEF                (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_gv_in_progress_b_val_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_gv_in_progress_b_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_gv_in_progress_b_val_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_gv_in_progress_b_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_gv_in_progress_b_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_gv_in_progress_b_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_gv_in_progress_b_val_HSH (0x01013E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_preq_val_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_preq_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_preq_val_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_preq_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_preq_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_preq_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_preq_val_HSH (0x01033E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_pstate_val_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_pstate_val_WID ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_pstate_val_MSK (0x0000001C)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_pstate_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_pstate_val_MAX (7) // 0x00000007
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_pstate_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_pma_phy_pstate_val_HSH (0x03053E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_init_start_val_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_init_start_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_init_start_val_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_init_start_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_init_start_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_init_start_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_init_start_val_HSH (0x010B3E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_frequency_val_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_frequency_val_WID ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_frequency_val_MSK (0x000007C0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_frequency_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_frequency_val_MAX (31) // 0x0000001F
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_frequency_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_dfi_frequency_val_HSH (0x050D3E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_NOC_BYPASS_VAL_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_NOC_BYPASS_VAL_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_NOC_BYPASS_VAL_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_NOC_BYPASS_VAL_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_NOC_BYPASS_VAL_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_NOC_BYPASS_VAL_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_NOC_BYPASS_VAL_HSH (0x01173E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc0_immediate_sleep_level_rsp_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc0_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc0_immediate_sleep_level_rsp_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc0_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc0_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc0_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc0_immediate_sleep_level_rsp_HSH (0x01193E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc1_immediate_sleep_level_rsp_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc1_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc1_immediate_sleep_level_rsp_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc1_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc1_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc1_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mc1_immediate_sleep_level_rsp_HSH (0x011B3E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_immediate_sleep_level_rsp_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_immediate_sleep_level_rsp_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_immediate_sleep_level_rsp_HSH (0x011D3E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_immediate_sleep_level_rsp_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_immediate_sleep_level_rsp_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_immediate_sleep_level_rsp_HSH (0x011F3E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc0_immediate_sleep_level_rsp_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc0_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc0_immediate_sleep_level_rsp_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc0_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc0_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc0_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc0_immediate_sleep_level_rsp_HSH (0x01213E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc1_immediate_sleep_level_rsp_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc1_immediate_sleep_level_rsp_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc1_immediate_sleep_level_rsp_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc1_immediate_sleep_level_rsp_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc1_immediate_sleep_level_rsp_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc1_immediate_sleep_level_rsp_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_ibecc1_immediate_sleep_level_rsp_HSH (0x01233E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mctrain_clkgate_ovrd_val_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mctrain_clkgate_ovrd_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mctrain_clkgate_ovrd_val_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mctrain_clkgate_ovrd_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mctrain_clkgate_ovrd_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mctrain_clkgate_ovrd_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_mctrain_clkgate_ovrd_val_HSH (0x01253E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_shutoff_ovrd_val_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_shutoff_ovrd_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_shutoff_ovrd_val_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_shutoff_ovrd_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_shutoff_ovrd_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_shutoff_ovrd_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce0_shutoff_ovrd_val_HSH (0x01273E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_shutoff_ovrd_val_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_shutoff_ovrd_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_shutoff_ovrd_val_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_shutoff_ovrd_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_shutoff_ovrd_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_shutoff_ovrd_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_cce1_shutoff_ovrd_val_HSH (0x01293E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_global_clken_ovrd_ovrd_val_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_global_clken_ovrd_ovrd_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_global_clken_ovrd_ovrd_val_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_global_clken_ovrd_ovrd_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_global_clken_ovrd_ovrd_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_global_clken_ovrd_ovrd_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_global_clken_ovrd_ovrd_val_HSH (0x012B3E40)

  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_Reserved_OFF     (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_Reserved_WID     (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_Reserved_MSK     (0xFFC00000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_Reserved_MIN     (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_Reserved_MAX     (1023) // 0x000003FF
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_Reserved_DEF     (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_OVERRIDE_VALUE_3_Reserved_HSH     (0x0A2D3E40)

#define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_REG         (0x00013E44)
#define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_DEF         (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_paccept_en_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_paccept_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_paccept_en_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_paccept_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_paccept_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_paccept_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_paccept_en_HSH (0x01013E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_pdeny_en_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_pdeny_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_pdeny_en_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_pdeny_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_pdeny_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_pdeny_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_pdeny_en_HSH (0x01033E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_dfi_init_complete_en_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_dfi_init_complete_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_dfi_init_complete_en_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_dfi_init_complete_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_dfi_init_complete_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_dfi_init_complete_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_phy_pma_dfi_init_complete_en_HSH (0x01053E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_dfi_init_complete_initialization_en_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_dfi_init_complete_initialization_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_dfi_init_complete_initialization_en_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_dfi_init_complete_initialization_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_dfi_init_complete_initialization_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_dfi_init_complete_initialization_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_dfi_init_complete_initialization_en_HSH (0x01073E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_lock_ack_en_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_lock_ack_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_lock_ack_en_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_lock_ack_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_lock_ack_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_lock_ack_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_lock_ack_en_HSH (0x01093E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_early_lock_ack_en_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_early_lock_ack_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_early_lock_ack_en_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_early_lock_ack_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_early_lock_ack_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_early_lock_ack_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_early_lock_ack_en_HSH (0x010B3E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc0_side_POK_en_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc0_side_POK_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc0_side_POK_en_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc0_side_POK_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc0_side_POK_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc0_side_POK_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc0_side_POK_en_HSH (0x010D3E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc1_side_POK_en_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc1_side_POK_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc1_side_POK_en_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc1_side_POK_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc1_side_POK_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc1_side_POK_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_mc1_side_POK_en_HSH (0x010F3E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce0_side_POK_en_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce0_side_POK_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce0_side_POK_en_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce0_side_POK_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce0_side_POK_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce0_side_POK_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce0_side_POK_en_HSH (0x01113E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce1_side_POK_en_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce1_side_POK_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce1_side_POK_en_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce1_side_POK_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce1_side_POK_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce1_side_POK_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_cce1_side_POK_en_HSH (0x01133E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc0_side_POK_en_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc0_side_POK_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc0_side_POK_en_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc0_side_POK_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc0_side_POK_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc0_side_POK_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc0_side_POK_en_HSH (0x01153E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc1_side_POK_en_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc1_side_POK_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc1_side_POK_en_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc1_side_POK_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc1_side_POK_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc1_side_POK_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ibecc1_side_POK_en_HSH (0x01173E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_side_POK_en_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_side_POK_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_side_POK_en_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_side_POK_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_side_POK_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_side_POK_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_side_POK_en_HSH (0x01193E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_IP_READY_en_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_IP_READY_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_IP_READY_en_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_IP_READY_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_IP_READY_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_IP_READY_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_pll_IP_READY_en_HSH (0x011B3E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem0_IP_READY_en_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem0_IP_READY_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem0_IP_READY_en_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem0_IP_READY_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem0_IP_READY_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem0_IP_READY_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem0_IP_READY_en_HSH (0x011D3E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem1_IP_READY_en_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem1_IP_READY_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem1_IP_READY_en_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem1_IP_READY_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem1_IP_READY_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem1_IP_READY_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_svrs_mem1_IP_READY_en_HSH (0x011F3E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_IP_READY_en_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_IP_READY_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_IP_READY_en_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_IP_READY_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_IP_READY_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_IP_READY_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_IP_READY_en_HSH (0x01213E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ddrio_fuse_pull_done_en_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ddrio_fuse_pull_done_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ddrio_fuse_pull_done_en_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ddrio_fuse_pull_done_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ddrio_fuse_pull_done_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ddrio_fuse_pull_done_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ddrio_fuse_pull_done_en_HSH (0x01233E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_block_ack_en_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_block_ack_en_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_block_ack_en_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_block_ack_en_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_block_ack_en_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_block_ack_en_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_ondd_block_ack_en_HSH (0x01253E44)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_Reserved_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_Reserved_WID (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_Reserved_MSK (0xFFF80000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_Reserved_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_Reserved_MAX (8191) // 0x00001FFF
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_Reserved_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_ENABLE_1_Reserved_HSH (0x0D273E44)

#define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_REG          (0x00013E48)
#define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_DEF          (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_paccept_val_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_paccept_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_paccept_val_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_paccept_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_paccept_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_paccept_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_paccept_val_HSH (0x01013E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_pdeny_val_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_pdeny_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_pdeny_val_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_pdeny_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_pdeny_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_pdeny_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_pdeny_val_HSH (0x01033E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_dfi_init_complete_val_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_dfi_init_complete_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_dfi_init_complete_val_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_dfi_init_complete_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_dfi_init_complete_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_dfi_init_complete_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_phy_pma_dfi_init_complete_val_HSH (0x01053E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_lock_ack_val_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_lock_ack_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_lock_ack_val_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_lock_ack_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_lock_ack_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_lock_ack_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_lock_ack_val_HSH (0x01093E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_early_lock_ack_val_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_early_lock_ack_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_early_lock_ack_val_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_early_lock_ack_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_early_lock_ack_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_early_lock_ack_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_early_lock_ack_val_HSH (0x010B3E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc0_side_POK_val_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc0_side_POK_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc0_side_POK_val_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc0_side_POK_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc0_side_POK_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc0_side_POK_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc0_side_POK_val_HSH (0x010D3E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc1_side_POK_val_OFF ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc1_side_POK_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc1_side_POK_val_MSK (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc1_side_POK_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc1_side_POK_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc1_side_POK_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_mc1_side_POK_val_HSH (0x010F3E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce0_side_POK_val_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce0_side_POK_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce0_side_POK_val_MSK (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce0_side_POK_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce0_side_POK_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce0_side_POK_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce0_side_POK_val_HSH (0x01113E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce1_side_POK_val_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce1_side_POK_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce1_side_POK_val_MSK (0x00000200)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce1_side_POK_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce1_side_POK_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce1_side_POK_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_cce1_side_POK_val_HSH (0x01133E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc0_side_POK_val_OFF (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc0_side_POK_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc0_side_POK_val_MSK (0x00000400)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc0_side_POK_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc0_side_POK_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc0_side_POK_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc0_side_POK_val_HSH (0x01153E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc1_side_POK_val_OFF (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc1_side_POK_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc1_side_POK_val_MSK (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc1_side_POK_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc1_side_POK_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc1_side_POK_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ibecc1_side_POK_val_HSH (0x01173E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_side_POK_val_OFF (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_side_POK_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_side_POK_val_MSK (0x00001000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_side_POK_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_side_POK_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_side_POK_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_side_POK_val_HSH (0x01193E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_IP_READY_val_OFF (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_IP_READY_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_IP_READY_val_MSK (0x00002000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_IP_READY_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_IP_READY_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_IP_READY_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_pll_IP_READY_val_HSH (0x011B3E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem0_IP_READY_val_OFF (14)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem0_IP_READY_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem0_IP_READY_val_MSK (0x00004000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem0_IP_READY_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem0_IP_READY_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem0_IP_READY_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem0_IP_READY_val_HSH (0x011D3E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem1_IP_READY_val_OFF (15)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem1_IP_READY_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem1_IP_READY_val_MSK (0x00008000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem1_IP_READY_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem1_IP_READY_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem1_IP_READY_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_svrs_mem1_IP_READY_val_HSH (0x011F3E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_IP_READY_val_OFF (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_IP_READY_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_IP_READY_val_MSK (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_IP_READY_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_IP_READY_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_IP_READY_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_IP_READY_val_HSH (0x01213E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ddrio_fuse_pull_done_val_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ddrio_fuse_pull_done_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ddrio_fuse_pull_done_val_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ddrio_fuse_pull_done_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ddrio_fuse_pull_done_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ddrio_fuse_pull_done_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ddrio_fuse_pull_done_val_HSH (0x01233E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_block_ack_val_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_block_ack_val_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_block_ack_val_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_block_ack_val_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_block_ack_val_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_block_ack_val_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_ondd_block_ack_val_HSH (0x01253E48)

  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_Reserved_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_Reserved_WID (13)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_Reserved_MSK (0xFFF80000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_Reserved_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_Reserved_MAX (8191) // 0x00001FFF
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_Reserved_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_INPUT_OVERRIDE_VALUE_1_Reserved_HSH (0x0D273E48)

#define GLOBAL_B2B_FLOW_DELAY_REG                                      (0x00013E4C)
#define GLOBAL_B2B_FLOW_DELAY_DEF                                      (0x00001800)
#define GLOBAL_B2B_FLOW_DELAY_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define GLOBAL_B2B_FLOW_DELAY_DATA_OFF                               ( 0)
  #define GLOBAL_B2B_FLOW_DELAY_DATA_WID                               (26)
  #define GLOBAL_B2B_FLOW_DELAY_DATA_MSK                               (0x03FFFFFF)
  #define GLOBAL_B2B_FLOW_DELAY_DATA_MIN                               (0)
  #define GLOBAL_B2B_FLOW_DELAY_DATA_MAX                               (67108863) // 0x03FFFFFF
  #define GLOBAL_B2B_FLOW_DELAY_DATA_DEF                               (0x00001800)
  #define GLOBAL_B2B_FLOW_DELAY_DATA_HSH                               (0x1A013E4C)

  #define GLOBAL_B2B_FLOW_DELAY_RESERVED0_OFF                          (26)
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED0_WID                          ( 1)
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED0_MSK                          (0x04000000)
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED0_MIN                          (0)
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED0_MAX                          (1) // 0x00000001
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED0_DEF                          (0x00000000)
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED0_HSH                          (0x01353E4C)

  #define GLOBAL_B2B_FLOW_DELAY_PKGC_EXIT_Counter_En_OFF               (27)
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_EXIT_Counter_En_WID               ( 1)
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_EXIT_Counter_En_MSK               (0x08000000)
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_EXIT_Counter_En_MIN               (0)
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_EXIT_Counter_En_MAX               (1) // 0x00000001
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_EXIT_Counter_En_DEF               (0x00000000)
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_EXIT_Counter_En_HSH               (0x01373E4C)

  #define GLOBAL_B2B_FLOW_DELAY_PKGC_ENTRY_Counter_En_OFF              (28)
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_ENTRY_Counter_En_WID              ( 1)
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_ENTRY_Counter_En_MSK              (0x10000000)
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_ENTRY_Counter_En_MIN              (0)
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_ENTRY_Counter_En_MAX              (1) // 0x00000001
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_ENTRY_Counter_En_DEF              (0x00000000)
  #define GLOBAL_B2B_FLOW_DELAY_PKGC_ENTRY_Counter_En_HSH              (0x01393E4C)

  #define GLOBAL_B2B_FLOW_DELAY_SAGV_Counter_En_OFF                    (29)
  #define GLOBAL_B2B_FLOW_DELAY_SAGV_Counter_En_WID                    ( 1)
  #define GLOBAL_B2B_FLOW_DELAY_SAGV_Counter_En_MSK                    (0x20000000)
  #define GLOBAL_B2B_FLOW_DELAY_SAGV_Counter_En_MIN                    (0)
  #define GLOBAL_B2B_FLOW_DELAY_SAGV_Counter_En_MAX                    (1) // 0x00000001
  #define GLOBAL_B2B_FLOW_DELAY_SAGV_Counter_En_DEF                    (0x00000000)
  #define GLOBAL_B2B_FLOW_DELAY_SAGV_Counter_En_HSH                    (0x013B3E4C)

  #define GLOBAL_B2B_FLOW_DELAY_RESERVED1_OFF                          (30)
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED1_WID                          ( 1)
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED1_MSK                          (0x40000000)
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED1_MIN                          (0)
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED1_MAX                          (1) // 0x00000001
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED1_DEF                          (0x00000000)
  #define GLOBAL_B2B_FLOW_DELAY_RESERVED1_HSH                          (0x013D3E4C)

  #define GLOBAL_B2B_FLOW_DELAY_OPP_SR_DISABLE_Counter_En_OFF          (31)
  #define GLOBAL_B2B_FLOW_DELAY_OPP_SR_DISABLE_Counter_En_WID          ( 1)
  #define GLOBAL_B2B_FLOW_DELAY_OPP_SR_DISABLE_Counter_En_MSK          (0x80000000)
  #define GLOBAL_B2B_FLOW_DELAY_OPP_SR_DISABLE_Counter_En_MIN          (0)
  #define GLOBAL_B2B_FLOW_DELAY_OPP_SR_DISABLE_Counter_En_MAX          (1) // 0x00000001
  #define GLOBAL_B2B_FLOW_DELAY_OPP_SR_DISABLE_Counter_En_DEF          (0x00000000)
  #define GLOBAL_B2B_FLOW_DELAY_OPP_SR_DISABLE_Counter_En_HSH          (0x013F3E4C)

#define MEMSS_PMA_CR_SURVIVABILITY_BISR_REG                            (0x00013E50)
#define MEMSS_PMA_CR_SURVIVABILITY_BISR_DEF                            (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_BISR_VOLATILE_BITFIELDS_MSK         (0x07FF0000)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_SKIP_BISR_OFF            ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_SKIP_BISR_WID            ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_SKIP_BISR_MSK            (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_SKIP_BISR_MIN            (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_SKIP_BISR_MAX            (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_SKIP_BISR_DEF            (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_SKIP_BISR_HSH            (0x01013E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_SKIP_BISR_OFF            ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_SKIP_BISR_WID            ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_SKIP_BISR_MSK            (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_SKIP_BISR_MIN            (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_SKIP_BISR_MAX            (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_SKIP_BISR_DEF            (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_SKIP_BISR_HSH            (0x01033E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_SKIP_BISR_OFF           ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_SKIP_BISR_WID           ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_SKIP_BISR_MSK           (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_SKIP_BISR_MIN           (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_SKIP_BISR_MAX           (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_SKIP_BISR_DEF           (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_SKIP_BISR_HSH           (0x01053E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_SKIP_BISR_OFF           ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_SKIP_BISR_WID           ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_SKIP_BISR_MSK           (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_SKIP_BISR_MIN           (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_SKIP_BISR_MAX           (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_SKIP_BISR_DEF           (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_SKIP_BISR_HSH           (0x01073E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_SKIP_BISR_OFF         ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_SKIP_BISR_WID         ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_SKIP_BISR_MSK         (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_SKIP_BISR_MIN         (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_SKIP_BISR_MAX         (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_SKIP_BISR_DEF         (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_SKIP_BISR_HSH         (0x01093E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_SKIP_BISR_OFF         ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_SKIP_BISR_WID         ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_SKIP_BISR_MSK         (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_SKIP_BISR_MIN         (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_SKIP_BISR_MAX         (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_SKIP_BISR_DEF         (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_SKIP_BISR_HSH         (0x010B3E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_SKIP_BISR_OFF           ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_SKIP_BISR_WID           ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_SKIP_BISR_MSK           (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_SKIP_BISR_MIN           (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_SKIP_BISR_MAX           (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_SKIP_BISR_DEF           (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_SKIP_BISR_HSH           (0x010D3E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_SKIP_BISR_OFF         ( 7)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_SKIP_BISR_WID         ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_SKIP_BISR_MSK         (0x00000080)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_SKIP_BISR_MIN         (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_SKIP_BISR_MAX         (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_SKIP_BISR_DEF         (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_SKIP_BISR_HSH         (0x010F3E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_SKIP_BISR_OFF         ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_SKIP_BISR_WID         ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_SKIP_BISR_MSK         (0x00000100)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_SKIP_BISR_MIN         (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_SKIP_BISR_MAX         (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_SKIP_BISR_DEF         (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_SKIP_BISR_HSH         (0x01113E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_BISR_ACK_STS_OFF         (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_BISR_ACK_STS_WID         ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_BISR_ACK_STS_MSK         (0x00010000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_BISR_ACK_STS_MIN         (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_BISR_ACK_STS_MAX         (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_BISR_ACK_STS_DEF         (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC0_BISR_ACK_STS_HSH         (0x01A13E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_BISR_ACK_STS_OFF         (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_BISR_ACK_STS_WID         ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_BISR_ACK_STS_MSK         (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_BISR_ACK_STS_MIN         (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_BISR_ACK_STS_MAX         (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_BISR_ACK_STS_DEF         (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_MC1_BISR_ACK_STS_HSH         (0x01A33E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_BISR_ACK_STS_OFF        (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_BISR_ACK_STS_WID        ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_BISR_ACK_STS_MSK        (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_BISR_ACK_STS_MIN        (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_BISR_ACK_STS_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_BISR_ACK_STS_DEF        (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE0_BISR_ACK_STS_HSH        (0x01A53E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_BISR_ACK_STS_OFF        (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_BISR_ACK_STS_WID        ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_BISR_ACK_STS_MSK        (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_BISR_ACK_STS_MIN        (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_BISR_ACK_STS_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_BISR_ACK_STS_DEF        (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_CCE1_BISR_ACK_STS_HSH        (0x01A73E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_BISR_ACK_STS_OFF      (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_BISR_ACK_STS_WID      ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_BISR_ACK_STS_MSK      (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_BISR_ACK_STS_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_BISR_ACK_STS_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_BISR_ACK_STS_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC0_BISR_ACK_STS_HSH      (0x01A93E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_BISR_ACK_STS_OFF      (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_BISR_ACK_STS_WID      ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_BISR_ACK_STS_MSK      (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_BISR_ACK_STS_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_BISR_ACK_STS_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_BISR_ACK_STS_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_IBECC1_BISR_ACK_STS_HSH      (0x01AB3E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_BISR_ACK_STS_OFF        (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_BISR_ACK_STS_WID        ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_BISR_ACK_STS_MSK        (0x00400000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_BISR_ACK_STS_MIN        (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_BISR_ACK_STS_MAX        (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_BISR_ACK_STS_DEF        (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_ONDD_BISR_ACK_STS_HSH        (0x01AD3E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_BISR_ACK_STS_OFF      (23)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_BISR_ACK_STS_WID      ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_BISR_ACK_STS_MSK      (0x00800000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_BISR_ACK_STS_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_BISR_ACK_STS_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_BISR_ACK_STS_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM0_BISR_ACK_STS_HSH      (0x01AF3E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_BISR_ACK_STS_OFF      (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_BISR_ACK_STS_WID      ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_BISR_ACK_STS_MSK      (0x01000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_BISR_ACK_STS_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_BISR_ACK_STS_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_BISR_ACK_STS_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_SRFSM1_BISR_ACK_STS_HSH      (0x01B13E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_BISR_ACK_STS_OFF      (25)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_BISR_ACK_STS_WID      ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_BISR_ACK_STS_MSK      (0x02000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_BISR_ACK_STS_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_BISR_ACK_STS_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_BISR_ACK_STS_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_BISR_ACK_STS_HSH      (0x01B33E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_SR_BISR_ACK_STS_OFF   (26)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_SR_BISR_ACK_STS_WID   ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_SR_BISR_ACK_STS_MSK   (0x04000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_SR_BISR_ACK_STS_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_SR_BISR_ACK_STS_MAX   (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_SR_BISR_ACK_STS_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_DDRPHY_SR_BISR_ACK_STS_HSH   (0x01B53E50)

  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_Reserved_OFF                 (27)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_Reserved_WID                 ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_Reserved_MSK                 (0xF8000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_Reserved_MIN                 (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_Reserved_MAX                 (31) // 0x0000001F
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_Reserved_DEF                 (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_BISR_Reserved_HSH                 (0x05373E50)

#define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_REG                          (0x00013E54)
#define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_DEF                          (0x000E28C8)
#define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_VOLATILE_BITFIELDS_MSK       (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CLKS_FORCE_ON_CFG_OFF      ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CLKS_FORCE_ON_CFG_WID      ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CLKS_FORCE_ON_CFG_MSK      (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CLKS_FORCE_ON_CFG_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CLKS_FORCE_ON_CFG_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CLKS_FORCE_ON_CFG_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CLKS_FORCE_ON_CFG_HSH      (0x01013E54)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_RST_CNTR_DLY_OFF  ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_RST_CNTR_DLY_WID  ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_RST_CNTR_DLY_MSK  (0x000001FE)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_RST_CNTR_DLY_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_RST_CNTR_DLY_MAX  (255) // 0x000000FF
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_RST_CNTR_DLY_DEF  (0x00000064)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_RST_CNTR_DLY_HSH  (0x08033E54)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_PWRG_CNTR_DLY_OFF ( 9)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_PWRG_CNTR_DLY_WID ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_PWRG_CNTR_DLY_MSK (0x0001FE00)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_PWRG_CNTR_DLY_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_PWRG_CNTR_DLY_MAX (255) // 0x000000FF
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_PWRG_CNTR_DLY_DEF (0x00000014)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_FORCE_ON_PWRG_CNTR_DLY_HSH (0x08133E54)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_ENTRY_PM8_PARALEL_TO_BLOCK_IPS_OFF (17)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_ENTRY_PM8_PARALEL_TO_BLOCK_IPS_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_ENTRY_PM8_PARALEL_TO_BLOCK_IPS_MSK (0x00020000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_ENTRY_PM8_PARALEL_TO_BLOCK_IPS_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_ENTRY_PM8_PARALEL_TO_BLOCK_IPS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_ENTRY_PM8_PARALEL_TO_BLOCK_IPS_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_ENTRY_PM8_PARALEL_TO_BLOCK_IPS_HSH (0x01233E54)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_RST_ENTRY_SEND_PM6_AFTER_ASSRT_RST_TO_IPS_OFF (18)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_RST_ENTRY_SEND_PM6_AFTER_ASSRT_RST_TO_IPS_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_RST_ENTRY_SEND_PM6_AFTER_ASSRT_RST_TO_IPS_MSK (0x00040000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_RST_ENTRY_SEND_PM6_AFTER_ASSRT_RST_TO_IPS_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_RST_ENTRY_SEND_PM6_AFTER_ASSRT_RST_TO_IPS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_RST_ENTRY_SEND_PM6_AFTER_ASSRT_RST_TO_IPS_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_RST_ENTRY_SEND_PM6_AFTER_ASSRT_RST_TO_IPS_HSH (0x01253E54)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_EXIT_GATHER_PM0_ACK_BEFORE_DASSRT_RST_TO_IPS_OFF (19)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_EXIT_GATHER_PM0_ACK_BEFORE_DASSRT_RST_TO_IPS_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_EXIT_GATHER_PM0_ACK_BEFORE_DASSRT_RST_TO_IPS_MSK (0x00080000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_EXIT_GATHER_PM0_ACK_BEFORE_DASSRT_RST_TO_IPS_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_EXIT_GATHER_PM0_ACK_BEFORE_DASSRT_RST_TO_IPS_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_EXIT_GATHER_PM0_ACK_BEFORE_DASSRT_RST_TO_IPS_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PKGC_EXIT_GATHER_PM0_ACK_BEFORE_DASSRT_RST_TO_IPS_HSH (0x01273E54)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PWRGATE_TM_CBIT_USE_OLD_RTL_FOR_GATE_ACKS_ARC_OFF (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PWRGATE_TM_CBIT_USE_OLD_RTL_FOR_GATE_ACKS_ARC_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PWRGATE_TM_CBIT_USE_OLD_RTL_FOR_GATE_ACKS_ARC_MSK (0x00100000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PWRGATE_TM_CBIT_USE_OLD_RTL_FOR_GATE_ACKS_ARC_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PWRGATE_TM_CBIT_USE_OLD_RTL_FOR_GATE_ACKS_ARC_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PWRGATE_TM_CBIT_USE_OLD_RTL_FOR_GATE_ACKS_ARC_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_PWRGATE_TM_CBIT_USE_OLD_RTL_FOR_GATE_ACKS_ARC_HSH (0x01293E54)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CBIT_CLKGATE_MCTRAIN_EN_OFF (21)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CBIT_CLKGATE_MCTRAIN_EN_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CBIT_CLKGATE_MCTRAIN_EN_MSK (0x00200000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CBIT_CLKGATE_MCTRAIN_EN_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CBIT_CLKGATE_MCTRAIN_EN_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CBIT_CLKGATE_MCTRAIN_EN_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_CBIT_CLKGATE_MCTRAIN_EN_HSH (0x012B3E54)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_Reserved_OFF               (22)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_Reserved_WID               (10)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_Reserved_MSK               (0xFFC00000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_Reserved_MIN               (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_Reserved_MAX               (1023) // 0x000003FF
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_Reserved_DEF               (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_1_Reserved_HSH               (0x0A2D3E54)

#define MEMSS_PMA_CR_MC_IN_SR_COUNT_MC0_REG                            (0x00013E58)
//Duplicate of MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_REG

#define MEMSS_PMA_CR_MC_IN_SR_COUNT_MC1_REG                            (0x00013E5C)
//Duplicate of MEMSS_PMA_CR_PM_ALL_RANKS_CKE_LOW_COUNT_MC0_GVP0_REG

#define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_REG                 (0x00013E60)
#define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_DEF                 (0x00000002)
#define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM_DFI_MSG_DDRPHY_PMA_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM_DFI_MSG_DDRPHY_PMA_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM_DFI_MSG_DDRPHY_PMA_MSK (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM_DFI_MSG_DDRPHY_PMA_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM_DFI_MSG_DDRPHY_PMA_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM_DFI_MSG_DDRPHY_PMA_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM_DFI_MSG_DDRPHY_PMA_HSH (0x01013E60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM8_RST_ENTRY_OFF ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM8_RST_ENTRY_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM8_RST_ENTRY_MSK (0x00000002)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM8_RST_ENTRY_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM8_RST_ENTRY_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM8_RST_ENTRY_DEF (0x00000001)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SKIP_PM8_RST_ENTRY_HSH (0x01033E60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_CB_VIRTUAL_INTERRUPT_EN_OFF ( 2)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_CB_VIRTUAL_INTERRUPT_EN_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_CB_VIRTUAL_INTERRUPT_EN_MSK (0x00000004)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_CB_VIRTUAL_INTERRUPT_EN_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_CB_VIRTUAL_INTERRUPT_EN_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_CB_VIRTUAL_INTERRUPT_EN_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_CB_VIRTUAL_INTERRUPT_EN_HSH (0x01053E60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_RE_VIRTUAL_INTERRUPT_EN_OFF ( 3)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_RE_VIRTUAL_INTERRUPT_EN_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_RE_VIRTUAL_INTERRUPT_EN_MSK (0x00000008)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_RE_VIRTUAL_INTERRUPT_EN_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_RE_VIRTUAL_INTERRUPT_EN_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_RE_VIRTUAL_INTERRUPT_EN_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_RE_VIRTUAL_INTERRUPT_EN_HSH (0x01073E60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCE_VIRTUAL_INTERRUPT_EN_OFF ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCE_VIRTUAL_INTERRUPT_EN_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCE_VIRTUAL_INTERRUPT_EN_MSK (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCE_VIRTUAL_INTERRUPT_EN_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCE_VIRTUAL_INTERRUPT_EN_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCE_VIRTUAL_INTERRUPT_EN_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCE_VIRTUAL_INTERRUPT_EN_HSH (0x01093E60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCX_VIRTUAL_INTERRUPT_EN_OFF ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCX_VIRTUAL_INTERRUPT_EN_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCX_VIRTUAL_INTERRUPT_EN_MSK (0x00000020)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCX_VIRTUAL_INTERRUPT_EN_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCX_VIRTUAL_INTERRUPT_EN_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCX_VIRTUAL_INTERRUPT_EN_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_PCX_VIRTUAL_INTERRUPT_EN_HSH (0x010B3E60)

  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SAGV_VIRTUAL_INTERRUPT_EN_OFF ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SAGV_VIRTUAL_INTERRUPT_EN_WID ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SAGV_VIRTUAL_INTERRUPT_EN_MSK (0x00000040)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SAGV_VIRTUAL_INTERRUPT_EN_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SAGV_VIRTUAL_INTERRUPT_EN_MAX (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SAGV_VIRTUAL_INTERRUPT_EN_DEF (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_FM_DEBUG_MODE_1_SAGV_VIRTUAL_INTERRUPT_EN_HSH (0x010D3E60)

#define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_REG                         (0x00013E64)
#define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_DEF                         (0x00000000)
#define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_DOWN_DLY_OFF    ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_DOWN_DLY_WID    ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_DOWN_DLY_MSK    (0x000000FF)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_DOWN_DLY_MIN    (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_DOWN_DLY_MAX    (255) // 0x000000FF
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_DOWN_DLY_DEF    (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_DOWN_DLY_HSH    (0x08013E64)

  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_UP_DLY_OFF      ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_UP_DLY_WID      ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_UP_DLY_MSK      (0x0000FF00)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_UP_DLY_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_UP_DLY_MAX      (255) // 0x000000FF
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_UP_DLY_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_PG_TM_PWR_UP_DLY_HSH      (0x08113E64)

  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_ASRT_RST_DLY_OFF   (16)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_ASRT_RST_DLY_WID   ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_ASRT_RST_DLY_MSK   (0x00FF0000)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_ASRT_RST_DLY_MIN   (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_ASRT_RST_DLY_MAX   (255) // 0x000000FF
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_ASRT_RST_DLY_DEF   (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_ASRT_RST_DLY_HSH   (0x08213E64)

  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_DASRT_RST_DLY_OFF  (24)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_DASRT_RST_DLY_WID  ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_DASRT_RST_DLY_MSK  (0xFF000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_DASRT_RST_DLY_MIN  (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_DASRT_RST_DLY_MAX  (255) // 0x000000FF
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_DASRT_RST_DLY_DEF  (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_TM_MISC_RST_TM_DASRT_RST_DLY_HSH  (0x08313E64)

#define MEMSS_PMA_CR_HVM_OVERRIDE_REG                                  (0x00013E68)
#define MEMSS_PMA_CR_HVM_OVERRIDE_DEF                                  (0x00000001)
#define MEMSS_PMA_CR_HVM_OVERRIDE_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define MEMSS_PMA_CR_HVM_OVERRIDE_MRC_START_AUTO_ACK_OFF             ( 0)
  #define MEMSS_PMA_CR_HVM_OVERRIDE_MRC_START_AUTO_ACK_WID             ( 1)
  #define MEMSS_PMA_CR_HVM_OVERRIDE_MRC_START_AUTO_ACK_MSK             (0x00000001)
  #define MEMSS_PMA_CR_HVM_OVERRIDE_MRC_START_AUTO_ACK_MIN             (0)
  #define MEMSS_PMA_CR_HVM_OVERRIDE_MRC_START_AUTO_ACK_MAX             (1) // 0x00000001
  #define MEMSS_PMA_CR_HVM_OVERRIDE_MRC_START_AUTO_ACK_DEF             (0x00000001)
  #define MEMSS_PMA_CR_HVM_OVERRIDE_MRC_START_AUTO_ACK_HSH             (0x01013E68)

#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_REG                  (0x00013E6C)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DEF                  (0x00000F0F)
#define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_PWRGOOD_TO_SLEEP_DLY_OFF ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_PWRGOOD_TO_SLEEP_DLY_WID ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_PWRGOOD_TO_SLEEP_DLY_MSK (0x000000FF)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_PWRGOOD_TO_SLEEP_DLY_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_PWRGOOD_TO_SLEEP_DLY_MAX (255) // 0x000000FF
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_PWRGOOD_TO_SLEEP_DLY_DEF (0x0000000F)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_PWRGOOD_TO_SLEEP_DLY_HSH (0x08013E6C)

  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_SLEEP_TO_PWRDOWN_DLY_OFF ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_SLEEP_TO_PWRDOWN_DLY_WID ( 8)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_SLEEP_TO_PWRDOWN_DLY_MSK (0x0000FF00)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_SLEEP_TO_PWRDOWN_DLY_MIN (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_SLEEP_TO_PWRDOWN_DLY_MAX (255) // 0x000000FF
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_SLEEP_TO_PWRDOWN_DLY_DEF (0x0000000F)
  #define MEMSS_PMA_CR_SURVIVABILITY_DELAY_TIMERS_5_DIS_IPS_SLEEP_TO_PWRDOWN_DLY_HSH (0x08113E6C)

#define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_REG                          (0x00013E70)
#define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_DEF                          (0x00000005)
#define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_VOLATILE_BITFIELDS_MSK       (0x00000010)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_WAIT_CYCLES_OFF    ( 0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_WAIT_CYCLES_WID    ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_WAIT_CYCLES_MSK    (0x0000000F)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_WAIT_CYCLES_MIN    (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_WAIT_CYCLES_MAX    (15) // 0x0000000F
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_WAIT_CYCLES_DEF    (0x00000005)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_WAIT_CYCLES_HSH    (0x04013E70)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_RUN_BUSY_OFF       ( 4)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_RUN_BUSY_WID       ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_RUN_BUSY_MSK       (0x00000010)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_RUN_BUSY_MIN       (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_RUN_BUSY_MAX       (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_RUN_BUSY_DEF       (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_RUN_BUSY_HSH       (0x01893E70)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_IP_SEL_OFF         ( 5)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_IP_SEL_WID         ( 6)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_IP_SEL_MSK         (0x000007E0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_IP_SEL_MIN         (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_IP_SEL_MAX         (63) // 0x0000003F
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_IP_SEL_DEF         (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_IP_SEL_HSH         (0x060B3E70)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_DIRECTION_OFF      (11)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_DIRECTION_WID      ( 1)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_DIRECTION_MSK      (0x00000800)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_DIRECTION_MIN      (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_DIRECTION_MAX      (1) // 0x00000001
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_DIRECTION_DEF      (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_SHUTOFF_DIRECTION_HSH      (0x01173E70)

  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_Reserved_OFF               (12)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_Reserved_WID               (20)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_Reserved_MSK               (0xFFFFF000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_Reserved_MIN               (0)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_Reserved_MAX               (1048575) // 0x000FFFFF
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_Reserved_DEF               (0x00000000)
  #define MEMSS_PMA_CR_SURVIVABILITY_MISC_2_Reserved_HSH               (0x14193E70)

#define MEMSS_PMA_CR_SPARE_CBITS_REG                                   (0x00013E74)
#define MEMSS_PMA_CR_SPARE_CBITS_DEF                                   (0x00000000)
#define MEMSS_PMA_CR_SPARE_CBITS_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MEMSS_PMA_CR_SPARE_CBITS_SPARE_CBITS_OFF                     ( 0)
  #define MEMSS_PMA_CR_SPARE_CBITS_SPARE_CBITS_WID                     (32)
  #define MEMSS_PMA_CR_SPARE_CBITS_SPARE_CBITS_MSK                     (0xFFFFFFFF)
  #define MEMSS_PMA_CR_SPARE_CBITS_SPARE_CBITS_MIN                     (0)
  #define MEMSS_PMA_CR_SPARE_CBITS_SPARE_CBITS_MAX                     (4294967295) // 0xFFFFFFFF
  #define MEMSS_PMA_CR_SPARE_CBITS_SPARE_CBITS_DEF                     (0x00000000)
  #define MEMSS_PMA_CR_SPARE_CBITS_SPARE_CBITS_HSH                     (0x20013E74)

#define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_REG                           (0x00013E78)
#define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_DEF                           (0x00000000)
#define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_VOLATILE_BITFIELDS_MSK        (0x0000FFFF)

  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_RATIO_OFF              ( 0)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_RATIO_WID              ( 8)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_RATIO_MSK              (0x000000FF)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_RATIO_MIN              (0)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_RATIO_MAX              (255) // 0x000000FF
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_RATIO_DEF              (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_RATIO_HSH              (0x08813E78)

  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_GEAR_TYPE_OFF               ( 8)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_GEAR_TYPE_WID               ( 1)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_GEAR_TYPE_MSK               (0x00000100)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_GEAR_TYPE_MIN               (0)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_GEAR_TYPE_MAX               (1) // 0x00000001
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_GEAR_TYPE_DEF               (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_GEAR_TYPE_HSH               (0x01913E78)

  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_VDDQ_V_OFF                  ( 9)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_VDDQ_V_WID                  ( 5)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_VDDQ_V_MSK                  (0x00003E00)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_VDDQ_V_MIN                  (0)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_VDDQ_V_MAX                  (31) // 0x0000001F
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_VDDQ_V_DEF                  (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_VDDQ_V_HSH                  (0x05933E78)

  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_WP_IDX_OFF             (14)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_WP_IDX_WID             ( 2)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_WP_IDX_MSK             (0x0000C000)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_WP_IDX_MIN             (0)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_WP_IDX_MAX             (3) // 0x00000003
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_WP_IDX_DEF             (0x00000000)
  #define MEMSS_PMA_CR_BIOS_REQ_PMA_STATUS_QCLK_WP_IDX_HSH             (0x029D3E78)
#pragma pack(pop)
#endif
