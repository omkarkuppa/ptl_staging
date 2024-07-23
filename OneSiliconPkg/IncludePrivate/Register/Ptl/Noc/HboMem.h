/** @file
  HBO_MEM.h

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

#ifndef _HBO_MEM_PTL_CDIE_H_
#define _HBO_MEM_PTL_CDIE_H_

#include <Base.h>


// IP GUID: 379170b7-e0d5-4175-93b3-15ad681793c8
#define HBO_MEM_ID 0x379170b7e0d54175

//************************************************ RegisterStructs


/** MEMORY_SLICE_HASH_HBO_MEM desc:
  * Register default value:        0x3FF82505
  * Register full path in IP: hbo_top/hbo_tractor/MEM_hbo_tractor/memory_slice_hash
  * Std path: VER=1, UVM=hbo_top.hbo_tractor.memory_slice_hash, OSX=hbo_top.hbo_tractor.MEM_hbo_tractor.memory_slice_hash
  * Security PolicyGroup: HBO_BIOS_W_PG_POLICY
  * MC hash config defines MC channel hashing
  */

#define MEMORY_SLICE_HASH_HBO_MEM_REG    (0x00000200U)

typedef union {
  struct {
    UINT32 hash_enabled : 1; /**< hash enabled */

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Hashing in Zone0 between the two slices is
                               enabled (a value of zero means that only one
                               slice Slice_L_ID has memory attached to it)
                            */

    UINT32 hash_lsb : 3; /**< hash LSB */

                            /* Bits[3:1], Access Type=RW, default=0x00000002*/

                            /*
                               LSB used in hashing between the two slices.
                               Encoding: 000 - address bit 6 001 - address
                               bit 7 ... 111 - address bit 13
                            */

    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[05:04] */

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 hash_mask : 14; /**< hash mask */

                            /* Bits[19:6], Access Type=RW, default=0x00002094*/

                            /*
                               Address bits in range of [19:6] used for hashing.
                               Hash_mask should match Hash_LSB encoding (i.e.
                               Hash_LSB is the lowest bit set in the mask)
                            */

    UINT32 zone1_start : 10; /**< zone1 start */

                            /* Bits[29:20], Access Type=RW, default=0x000003FF*/

                            /*
                               Address in GB of the non-interleaved portion
                               of asymmetric memory.  i.e. smaller slice
                               size*2 default value is 0x3FF
                            */

    UINT32 slice_l_id : 1; /**< Large Slice ID */

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Identify the slice with higher memory capacity
                               in case of asymmetric memory or the slice
                               used in case of single slice.
                            */

    UINT32 stacked_mode : 1; /**< stacked mode */

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Debug mode to override and disable hashing
                               in Zone 0. Only supported in Symmetric memory
                               configurations
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMORY_SLICE_HASH_HBO_MEM_STRUCT;

#define MEMORY_SLICE_HASH_HBO_MEM_WIDTH 32

#define MEMORY_SLICE_HASH_HBO_MEM_HASH_ENABLED_LSB 0x0000
#define MEMORY_SLICE_HASH_HBO_MEM_HASH_ENABLED_SIZE 0x0001
#define MEMORY_SLICE_HASH_HBO_MEM_HASH_LSB_LSB 0x0001
#define MEMORY_SLICE_HASH_HBO_MEM_HASH_LSB_SIZE 0x0003
#define MEMORY_SLICE_HASH_HBO_MEM_HASH_MASK_LSB 0x0006
#define MEMORY_SLICE_HASH_HBO_MEM_HASH_MASK_SIZE 0x000e
#define MEMORY_SLICE_HASH_HBO_MEM_ZONE1_START_LSB 0x0014
#define MEMORY_SLICE_HASH_HBO_MEM_ZONE1_START_SIZE 0x000a
#define MEMORY_SLICE_HASH_HBO_MEM_SLICE_L_ID_LSB 0x001e
#define MEMORY_SLICE_HASH_HBO_MEM_SLICE_L_ID_SIZE 0x0001
#define MEMORY_SLICE_HASH_HBO_MEM_STACKED_MODE_LSB 0x001f
#define MEMORY_SLICE_HASH_HBO_MEM_STACKED_MODE_SIZE 0x0001


/** REMAP_BASE_HBO_MEM desc:
  * Register default value:        0x3FFFFF00000
  * Register full path in IP: hbo_top/hbo_tractor/MEM_hbo_tractor/remap_base
  * Std path: VER=1, UVM=hbo_top.hbo_tractor.remap_base, OSX=hbo_top.hbo_tractor.MEM_hbo_tractor.remap_base
  * Security PolicyGroup: HBO_BIOS_W_PG_POLICY
  * Remap Base Register
  */

#define REMAP_BASE_HBO_MEM_REG           (0x00000208U)

typedef union {
  struct {
    UINT64 reserved_0 : 20; /**< reserved for future0 */

                            /* Bits[19:0], Access Type=RO, default=0x00000000*/

                            /* reserved */

    UINT64 remapbase : 22; /**< remap base */

                            /* Bits[41:20], Access Type=RW, default=0x003FFFFF*/

                            /*
                               Remap Base The value in this register defines
                               the lower boundary of the remap window. The
                               remap window is inclusive of this address.
                               These bits are compared with bits 41:20 of
                               the address and only they will be changed
                               in case of remapping. When the value in this
                               register is greater than the value programmed
                               into the Remap Limit register, the remap window
                               is disabled. These bits are Intel TXT lockable.
                            */

    UINT64 reserved_1 : 22; /**< reserved for future1 */

                            /* Bits[63:42], Access Type=RO, default=0x00000000*/

                            /* Reserved bits for future usage */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} REMAP_BASE_HBO_MEM_STRUCT;

#define REMAP_BASE_HBO_MEM_WIDTH 64

#define REMAP_BASE_HBO_MEM_RESERVED_0_LSB 0x0000
#define REMAP_BASE_HBO_MEM_RESERVED_0_SIZE 0x0014
#define REMAP_BASE_HBO_MEM_REMAPBASE_LSB 0x0014
#define REMAP_BASE_HBO_MEM_REMAPBASE_SIZE 0x0016
#define REMAP_BASE_HBO_MEM_RESERVED_1_LSB 0x002a
#define REMAP_BASE_HBO_MEM_RESERVED_1_SIZE 0x0016


/** REMAP_LIMIT_HBO_MEM desc:
  * Register default value:        0x000FFFFF
  * Register full path in IP: hbo_top/hbo_tractor/MEM_hbo_tractor/remap_limit
  * Std path: VER=1, UVM=hbo_top.hbo_tractor.remap_limit, OSX=hbo_top.hbo_tractor.MEM_hbo_tractor.remap_limit
  * Security PolicyGroup: HBO_BIOS_W_PG_POLICY
  * Remap Limit register. Defines the upper boundary of the remap window.
  */

#define REMAP_LIMIT_HBO_MEM_REG          (0x00000210U)

typedef union {
  struct {
    UINT64 reserved_0 : 20; /**< reserved for future0 */

                            /* Bits[19:0], Access Type=RO, default=0x000FFFFF*/

                            /* Reserved bits for future usage */

    UINT64 remaplmt : 22; /**< remap limit */

                            /* Bits[41:20], Access Type=RW, default=0x00000000*/

                            /*
                               Remap limit The value in this regsiter defines
                               the upper boundary of the remap window. The
                               remap window is inclusive of this address.
                               These bits are compared with bits 41:20 of
                               the address and only they will be changed
                               in case of remapping. When the value in this
                               register is less than the value programmed
                               into the Remap Base register, the remap window
                               is disabled. These bits are Intel TXT lockable.
                            */

    UINT64 reserved_1 : 22; /**< reserved for future1 */

                            /* Bits[63:42], Access Type=RO, default=0x00000000*/

                            /* Reserved bits for future usage */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} REMAP_LIMIT_HBO_MEM_STRUCT;

#define REMAP_LIMIT_HBO_MEM_WIDTH 64

#define REMAP_LIMIT_HBO_MEM_RESERVED_0_LSB 0x0000
#define REMAP_LIMIT_HBO_MEM_RESERVED_0_SIZE 0x0014
#define REMAP_LIMIT_HBO_MEM_REMAPLMT_LSB 0x0014
#define REMAP_LIMIT_HBO_MEM_REMAPLMT_SIZE 0x0016
#define REMAP_LIMIT_HBO_MEM_RESERVED_1_LSB 0x002a
#define REMAP_LIMIT_HBO_MEM_RESERVED_1_SIZE 0x0016

//************************************************ /RegisterStructs

#endif      // _HBO_MEM_h

