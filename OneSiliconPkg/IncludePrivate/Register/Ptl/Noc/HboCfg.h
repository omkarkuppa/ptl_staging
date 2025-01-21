/** @file
  HBO_CFG.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2007 Intel Corporation.

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

  This file contains Silicon register definitions.

  This is a generated file; please do not modify it directly.

**/

/* The following security policy groups are used by registers in this file:     */
/* HBO_BIOS_W_PG_POLICY                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | RESERVED_1_SAI | RESERVED_2_SAI |  */
/*    HOSTIA_SEAM_SAI | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI |        */
/*    VTD_SAI | PM_DIE_TO_DIE_SAI | UNASSIGNED_1C_SAI | HOSTCP_PMA_SAI |        */
/*    CSE_INTEL_SAI | DMU_UNTRUSTED_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |     */
/*    GSC_SAI | PM_IOSS_SAI | UNASSIGNED_2C_SAI | UNASSIGNED_2E_SAI |           */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | ISH_SAI |               */
/*    CLINK_KVM_SAI | NPK_SAI | TBT_SAI | IOM_SAI | HW_PCH_SAI | SPI_SAI |      */
/*    UNASSIGNED_42_SAI | ESE_UNTRUSTED_SAI | GT_PMA_SAI | ESE_SAI |            */
/*    CSE_UNTRUSTED_SAI | UNASSIGNED_4C_SAI | UNASSIGNED_4E_SAI | IPU_SAI |     */
/*    RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI |          */
/*    DISPLAY_SAI | LT_SAI | UNASSIGNED_5C_SAI | DISPLAY_KVM_SAI |              */
/*    UNASSIGNED_60_SAI | ACE_SAI | UNASSIGNED_64_SAI | CSME_SMS_SAI |          */
/*    CORE_EVENT_PROXY_SAI | DEVICE_POSTBOOT_SAI | RCIOMMU_BYPASS_SAI |         */
/*    PCH_IMR_SAI | EC_ESPI_SAI | CNVi_SAI | UNASSIGNED_74_SAI |                */
/*    UNASSIGNED_76_SAI | UNASSIGNED_78_SAI | CRASHLOG_SAI |                    */
/*    UNASSIGNED_7C_SAI | DEVICE_UNTRUSTED_SAI                                  */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    HW_CPU_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI       */

#ifndef _HBO_CFG_PTL_CDIE_H_
#define _HBO_CFG_PTL_CDIE_H_

#include <Base.h>


// IP GUID: 379170b7-e0d5-4175-93b3-15ad681793c8
#define HBO_CFG_ID 0x379170b7e0d54175

//************************************************ RegisterStructs


/** LOCAL_HOME_SLICE_HASH_HBO_CFG desc:
  * Register default value:        0x00082504
  * Register full path in IP: hbo_top/hbo_tractor/CFG_hbo_tractor/local_home_slice_hash
  * Std path: VER=1, UVM=hbo_top.hbo_tractor.local_home_slice_hash, OSX=hbo_top.hbo_tractor.CFG_hbo_tractor.local_home_slice_hash
  * Security PolicyGroup: HBO_BIOS_W_PG_POLICY
  * Defines the hash function between the 2 home agents
  */

#define LOCAL_HOME_SLICE_HASH_HBO_CFG_REG (0x0000008cU)

typedef union {
  struct {
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[00:00] */

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 hash_bit0_lsb : 3; /**< hash mask lsb */

                            /* Bits[3:1], Access Type=RW, default=0x00000002*/

                            /*
                               LSB used in hashing between the two slices.
                               Encoding: 000 - address bit 6 001 - address
                               bit 7 ... 111 - address bit 13
                            */

    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[05:04] */

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 hash_bit0_mask : 14; /**< hash mask */

                            /* Bits[19:6], Access Type=RW, default=0x00002094*/

                            /*
                               Address bits in range of [19:6] used for hashing.
                               Hash_mask should match Hash_LSB encoding (i.e.
                               Hash_LSB is the lowest bit set in the mask)
                            */

    UINT32 rsvd_2 : 12; /**< Undefined - auto filled rsvd_[31:20] */

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LOCAL_HOME_SLICE_HASH_HBO_CFG_STRUCT;

#define LOCAL_HOME_SLICE_HASH_HBO_CFG_WIDTH 32

#define LOCAL_HOME_SLICE_HASH_HBO_CFG_HASH_BIT0_LSB_LSB 0x0001
#define LOCAL_HOME_SLICE_HASH_HBO_CFG_HASH_BIT0_LSB_SIZE 0x0003
#define LOCAL_HOME_SLICE_HASH_HBO_CFG_HASH_BIT0_MASK_LSB 0x0006
#define LOCAL_HOME_SLICE_HASH_HBO_CFG_HASH_BIT0_MASK_SIZE 0x000e

#ifdef SV_HOOKS

/** TOLUD_0_0_0_PCI_HBO_CFG desc:
  * Register default value:        0x00100000
  * Register full path in IP: hbo_top/hbo_tractor/CFG_hbo_tractor/tolud_0_0_0_pci
  * Std path: VER=1, UVM=hbo_top.hbo_tractor.tolud_0_0_0_pci, OSX=hbo_top.hbo_tractor.CFG_hbo_tractor.tolud_0_0_0_pci
  * Security PolicyGroup: HBO_BIOS_W_PG_POLICY
  * This 32 bit register defines the Top of Low Usable DRAM. TSEG, GTT Graphics memory and Graphics Stolen Memory are within the DRAM space          Programming Example:          C1DRB3 is set to 4GB          TSEG is enabled and TSEG size is set to 1MB          Internal Graphics is enabled, and Graphics Mode Select is set to 32MB          GTT Graphics Stolen Memory Size set to 2MB          BIOS knows the OS requires 1G of PCI space.          BIOS also knows the range from 0_FEC0_0000h to 0_FFFF_FFFFh is not usable by the system. This 20MB range at the very top of addressable memory space is lost          According to the above equation, TOLUD is originally calculated to: 4GB = 1_0000_0000h          The system memory requirements are: 4GB (max addressable space) - 1GB (pci space) - 35MB (lost memory) = 3GB - 35MB (minimum granularity) = 0_ECB0_0000h          Since 0_ECB0_0000h (PCI and other system requirements) is less than 1_0000_0000h, TOLUD should be programmed to ECBh. These bits are Intel TXT lockable.
  */

#define TOLUD_0_0_0_PCI_HBO_CFG_REG      (0x000000bcU)

typedef union {
  struct {
    UINT32 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT32 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[19:01] */

                            /* Bits[19:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 tolud : 12;

                            /* Bits[31:20], Access Type=RW/L, default=0x00000001*/

                            /*
                               This register contains bits 31 to 20 of an
                               address one byte above the maximum DRAM memory
                               below 4G that is usable by the OS. The Top
                               of Low Usable DRAM is the lowest address above
                               both Graphics Stolen memory and Tseg. BIOS
                               determines the base of Graphics Stolen Memory
                               by subtracting the Gr This register must be
                               1MB aligned when reclaim is enabled.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TOLUD_0_0_0_PCI_HBO_CFG_STRUCT;

#define TOLUD_0_0_0_PCI_HBO_CFG_WIDTH 32

#define TOLUD_0_0_0_PCI_HBO_CFG_LOCK_LSB 0x0000
#define TOLUD_0_0_0_PCI_HBO_CFG_LOCK_SIZE 0x0001
#define TOLUD_0_0_0_PCI_HBO_CFG_TOLUD_LSB 0x0014
#define TOLUD_0_0_0_PCI_HBO_CFG_TOLUD_SIZE 0x000c

#endif      // SV_HOOKS

//************************************************ /RegisterStructs

#endif      // _HBO_CFG_h

