/** @file
  VTD_IOMMU.h

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
**/

/* The following security policy groups are used by registers in this file:     */
/* IOMMU_STANDARD                                                               */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI | ESE_SAI |             */
/*    PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |    */
/*    DFX_THIRDPARTY_SAI                                                        */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI | ESE_SAI |             */
/*    PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |    */
/*    DFX_THIRDPARTY_SAI                                                        */

#ifndef _VTD_IOMMU_PTL_CDIE_H_
#define _VTD_IOMMU_PTL_CDIE_H_

#include <Base.h>


// IP GUID: 53ead291-e2ed-4052-b540-0b79643067c6
#define VTD_IOMMU_ID 0x53ead291e2ed4052

//************************************************ RegisterStructs


/** CAP_VTD_IOMMU desc:
  * Register default value:        0xE9DE008CEE690402
  * Register full path in IP: SAF_SOUTH/saf_south/vtu/ioc_iommu_instance/iommu_regs_iommu_regs_sb/CAP_REG_0_0_0_VTDBAR
  * Std path: VER=1, UVM=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.CAP_REG_0_0_0_VTDBAR, OSX=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.iommu_regs_iommu_regs_sb.CAP_REG_0_0_0_VTDBAR
  * Security PolicyGroup: IOMMU_STANDARD
  * Register to report general remapping hardware capabilities. [IntelRsvd][p]Note: These values can change based on defeature bits.[/p][/IntelRsvd]
  */

#define CAP_VTD_IOMMU_REG                (0x00000008U)

typedef union {
  struct {
    UINT64 nd : 3; /**< Number of Domains Supported */

                            /* Bits[2:0], Access Type=RO/V, default=0x00000002*/

                            /*
                               [list][*]000b = Hardware supports 4-bit domain-ids
                               with support for up to 16 domains.[*]001b
                               = Hardware supports 6-bit domain-ids with
                               support for up to 64 domains.[*]010b = Hardware
                               supports 8-bit domain-ids with support for
                               up to 256 domains.[*]011b = Hardware supports
                               10-bit domain-ids with support for up to 1024
                               domains.[*]100b = Hardware supports 12-bit
                               domain-ids with support for up to 4K domains.[*]100b
                               = Hardware supports 14-bit domain-ids with
                               support for up to 16K domains.[*]110b = Hardware
                               supports 16-bit domain-ids with support for
                               up to 64K domains.[*]111b = Reserved.[/list]
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 afl : 1; /**< Advanced Fault Logging */

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Indicates advanced fault logging
                               is not supported. Only primary fault logging
                               is supported.[*]1 = Indicates advanced fault
                               logging is supported.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 rwbf : 1; /**< Required Write-Buffer Flushing */

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Indicates no write-buffer flushing
                               is needed to ensure changes to memory-resident
                               structures are visible to hardware.[*]1 =
                               Indicates software must explicitly flush the
                               write buffers to ensure updates made to memory-resident
                               remapping structures are visible to hardware.[/list]
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 plmr : 1; /**< Protected Low-Memory Region */

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Indicates protected low-memory
                               region is not supported.[*]1 = Indicates protected
                               low-memory region is supported.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 phmr : 1; /**< Protected High-Memory Region */

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Indicates protected high-memory
                               region is not supported.[*]1 = Indicates protected
                               high-memory region is  supported.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 cm : 1; /**< Caching Mode */

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Not-present and erroneous entries
                               are not cached in any of the renmapping caches.
                               Invalidations are not required for modifications
                               to individual not present or invalid entries.
                               However, any modifications that result in
                               decreasing the effective permissions or partial
                               permission increases require invalidations
                               for them to be effective.[*]1 = Not-present
                               and erroneous mappings may be cached in the
                               remapping caches. Any software updates to
                               the remapping structures (including updates
                               to not-present or erroneous entries) require
                               explicit invalidation.[/list][p]Hardware implementations
                               of this architecture must support a value
                               of 0 in this field.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 sagaw : 5; /**< Supported Adjusted Guest Address Widths */

                            /* Bits[12:8], Access Type=RO/V, default=0x00000004*/

                            /*
                               [p]This 5-bit field indicates the supported
                               adjusted guest address widths (which in turn
                               represents the levels of page-table walks
                               for the 4KB base page size) supported by the
                               hardware implementation.[/p][p]A value of
                               1 in any of these bits indicates the corresponding
                               adjusted guest address width is supported.
                               The adjusted guest address widths corresponding
                               to various bit positions within this field
                               are:[/p][list][*]0 = 30-bit AGAW (2-level
                               page table)[*]1 = 39-bit AGAW (3-level page
                               table)[*]2 = 48-bit AGAW (4-level page table)[*]3
                               = 57-bit AGAW (5-level page table)[*]4 = Reserved
                               [/list][p]Software must ensure that the adjusted
                               guest address width used to setup the page
                               tables is one of the supported guest address
                               widths reported in this field.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[15:13] */

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 mgaw : 6; /**< Maximum Guest Address Width */

                            /* Bits[21:16], Access Type=RO/V, default=0x00000029*/

                            /*
                               [p]This field indicates the maximum DMA virtual
                               addressability supported by remapping  hardware.
                               The Maximum Guest Address Width (MGAW) is
                               computed as (N+1), where N is the value reported
                               in this field. For example, a hardware implementation
                               supporting 48-bit MGAW reports a value of
                               47 (101111b) in this field.[/p][p]If the value
                               in this field is X, untranslated and translated
                               DMA requests to addresses above 2(x+1)-1 are
                               always blocked by hardware. Translations requests
                               to address above 2(x+1)-1 from allowed devices
                               return a null Translation Completion Data
                               Entry with R=W=0.[/p][p]Guest addressability
                               for a given DMA request is limited to the
                               minimum of the value reported through this
                               field and the adjusted guest address width
                               of the corresponding page-table structure.
                               (Adjusted guest address widths supported by
                               hardware are reported through the SAGAW field).[/p][p]Implementations
                               are recommended to support MGAW at least equal
                               to the physical addressability (host address
                               width) of the platform.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 zlr : 1; /**< Zero Length Read */

                            /* Bits[22:22], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Indicates the remapping hardware
                               unit blocks (and treats as fault) zero length
                               DMA read requests to write-only pages.[*]1
                               = Indicates the remapping hardware unit supports
                               zero length DMA read requests to write-only
                               pages.[/list][p]DMA remapping hardware implementations
                               are recommended to report ZLR field as Set.[/p]
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[23:23] */

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 fro : 10; /**< Fault-Recording Register Offset */

                            /* Bits[33:24], Access Type=RO/V, default=0x000000EE*/

                            /*
                               [p]This field specifies the location to the
                               first fault recording register relative to
                               the register base address of this remapping
                               hardware unit.[/p][p]If the register base
                               address is X, and the value reported in this
                               field is Y, the address for the first fault
                               recording register is calculated as X+(16*Y).[/p]
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 sllps : 4; /**< Second Level Large Page Support */

                            /* Bits[37:34], Access Type=RO/V, default=0x00000003*/

                            /*
                               [p]This field indicates the super page sizes
                               supported by hardware.[/p][p]A value of 1
                               in any of these bits indicates the corresponding
                               super-page size is supported. The super-page
                               sizes corresponding to various bit positions
                               within this field are:[/p][list][*]0 = 21-bit
                               offset to page frame (2MB)[*]1 = 30-bit offset
                               to page frame (1GB)[*]2 = 39-bit offset to
                               page frame (512GB)[*]3 = 48-bit offset to
                               page frame (1TB)[/list][p]Hardware implementations
                               supporting a specific super-page size must
                               support all smaller super-page sizes, i.e.
                               only valid values for this field are 0000b,
                               0001b, 0011b, 0111b, 1111b.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[38:38] */

                            /* Bits[38:38], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 psi : 1; /**< Page Selective Invalidation */

                            /* Bits[39:39], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware supports only domain
                               and global invalidates for IOTLB.[*]1 = Hardware
                               supports page selective, domain and global
                               invalidates for IOTLB.[/list][p]Hardware implementations
                               reporting this field as set are recommended
                               to support a Maximum Address Mask Value (MAMV)
                               value of at least 9 (or 18 if supporting 1GB
                               pages with second level translation).[/p]
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 nfr : 8; /**< Number of Fault-Recording Registers */

                            /* Bits[47:40], Access Type=RO/V, default=0x00000000*/

                            /*
                               [p]Number of fault recording registers is
                               computed as N+1, where N is the value reported
                               in this field.[/p][p]Implementations must
                               support at least one fault recording register
                               (NFR = 0) for each remapping hardware unit
                               in the platform.[/p][p]The maximum number
                               of fault recording registers per remapping
                               hardware unit is 256.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 mamv : 6; /**< Maximum Address Mask Value */

                            /* Bits[53:48], Access Type=RO/V, default=0x0000001E*/

                            /*
                               [p]The value in this field indicates the maximum
                               supported value for the Address Mask (AM)
                               field in the Invalidation Address register
                               (IVA_REG) and IOTLB Invalidation Descriptor
                               (iotlb_inv_dsc) used for invalidations of
                               second-level translation.[/p][p]This field
                               is valid only when the PSI field in Capability
                               register is reported as Set.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 dwd : 1; /**< Write Draining */

                            /* Bits[54:54], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support draining
                               of DMA write requests.[*]1 = Hardware supports
                               draining of DMA write requests.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 drd : 1; /**< Read Draining */

                            /* Bits[55:55], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support draining
                               of DMA read requests.[*]1 = Hardware supports
                               draining of DMA read requests.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 fl1gp : 1; /**< First Level 1-GByte Page Support */

                            /* Bits[56:56], Access Type=RO/V, default=0x00000001*/

                            /*
                               A value of 1 in this field indicates 1-GByte
                               page size is supported for first-level translation.
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 rsvd_3 : 2; /**< Undefined - auto filled rsvd_[58:57] */

                            /* Bits[58:57], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 pi : 1; /**< Posted Interrupt Support */

                            /* Bits[59:59], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support Posting
                               of Interrupts.[*]1 = Hardware supports Posting
                               of Interrupts.[/list][p]Hardware implementations
                               reporting this field as Set must also report
                               Interrupt Remapping support (IR field in Extended
                               Capability Register)[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 fl5lp : 1; /**< First Level 5-level Paging */

                            /* Bits[60:60], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0: Hardware does not support 5-level
                               paging for requests-with-PASID subject to
                               first-level translation.[*]1: Hardware supports
                               5-level paging for requests-with-PASID subject
                               to first-level translation.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 ecmds : 1; /**< Enhanced Command Support */

                            /* Bits[61:61], Access Type=RO, default=0x00000001*/

                            /* Refer to VT-d specification */

    UINT64 esirtps : 1; /**< Enhanced Set Interrupt Root Table Pointer Support */

                            /* Bits[62:62], Access Type=RO, default=0x00000001*/

                            /*
                               [list][*]0: Hardware does not invalidate all
                               Interrupt remapping hardware translation caches
                               as part of SIRTP flow.[*]1: Hardware invalidates
                               all Interrupt remapping hardware translation
                               caches as part of SIRTP flow.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 esrtps : 1; /**< Enhanced Set Root Table Pointer Support */

                            /* Bits[63:63], Access Type=RO, default=0x00000001*/

                            /*
                               [list][*]0: Hardware does not invalidate all
                               DMA remapping hardware translation caches
                               as part of SRTP flow.[*]1: Hardware invalidates
                               all DMA remapping hardware translation caches
                               as part of SRTP flow.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} CAP_VTD_IOMMU_STRUCT;

#define CAP_VTD_IOMMU_WIDTH 64

#define CAP_VTD_IOMMU_ND_LSB 0x0000
#define CAP_VTD_IOMMU_ND_SIZE 0x0003
#define CAP_VTD_IOMMU_AFL_LSB 0x0003
#define CAP_VTD_IOMMU_AFL_SIZE 0x0001
#define CAP_VTD_IOMMU_RWBF_LSB 0x0004
#define CAP_VTD_IOMMU_RWBF_SIZE 0x0001
#define CAP_VTD_IOMMU_PLMR_LSB 0x0005
#define CAP_VTD_IOMMU_PLMR_SIZE 0x0001
#define CAP_VTD_IOMMU_PHMR_LSB 0x0006
#define CAP_VTD_IOMMU_PHMR_SIZE 0x0001
#define CAP_VTD_IOMMU_CM_LSB 0x0007
#define CAP_VTD_IOMMU_CM_SIZE 0x0001
#define CAP_VTD_IOMMU_SAGAW_LSB 0x0008
#define CAP_VTD_IOMMU_SAGAW_SIZE 0x0005
#define CAP_VTD_IOMMU_MGAW_LSB 0x0010
#define CAP_VTD_IOMMU_MGAW_SIZE 0x0006
#define CAP_VTD_IOMMU_ZLR_LSB 0x0016
#define CAP_VTD_IOMMU_ZLR_SIZE 0x0001
#define CAP_VTD_IOMMU_FRO_LSB 0x0018
#define CAP_VTD_IOMMU_FRO_SIZE 0x000a
#define CAP_VTD_IOMMU_SLLPS_LSB 0x0022
#define CAP_VTD_IOMMU_SLLPS_SIZE 0x0004
#define CAP_VTD_IOMMU_PSI_LSB 0x0027
#define CAP_VTD_IOMMU_PSI_SIZE 0x0001
#define CAP_VTD_IOMMU_NFR_LSB 0x0028
#define CAP_VTD_IOMMU_NFR_SIZE 0x0008
#define CAP_VTD_IOMMU_MAMV_LSB 0x0030
#define CAP_VTD_IOMMU_MAMV_SIZE 0x0006
#define CAP_VTD_IOMMU_DWD_LSB 0x0036
#define CAP_VTD_IOMMU_DWD_SIZE 0x0001
#define CAP_VTD_IOMMU_DRD_LSB 0x0037
#define CAP_VTD_IOMMU_DRD_SIZE 0x0001
#define CAP_VTD_IOMMU_FL1GP_LSB 0x0038
#define CAP_VTD_IOMMU_FL1GP_SIZE 0x0001
#define CAP_VTD_IOMMU_PI_LSB 0x003b
#define CAP_VTD_IOMMU_PI_SIZE 0x0001
#define CAP_VTD_IOMMU_FL5LP_LSB 0x003c
#define CAP_VTD_IOMMU_FL5LP_SIZE 0x0001
#define CAP_VTD_IOMMU_ECMDS_LSB 0x003d
#define CAP_VTD_IOMMU_ECMDS_SIZE 0x0001
#define CAP_VTD_IOMMU_ESIRTPS_LSB 0x003e
#define CAP_VTD_IOMMU_ESIRTPS_SIZE 0x0001
#define CAP_VTD_IOMMU_ESRTPS_LSB 0x003f
#define CAP_VTD_IOMMU_ESRTPS_SIZE 0x0001


/** ECAP_VTD_IOMMU desc:
  * Register default value:        0x3AC89884F0EFDA
  * Register full path in IP: SAF_SOUTH/saf_south/vtu/ioc_iommu_instance/iommu_regs_iommu_regs_sb/ECAP_REG_0_0_0_VTDBAR
  * Std path: VER=1, UVM=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.ECAP_REG_0_0_0_VTDBAR, OSX=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.iommu_regs_iommu_regs_sb.ECAP_REG_0_0_0_VTDBAR
  * Security PolicyGroup: IOMMU_STANDARD
  * Register to report remapping hardware extended capabilities. [IntelRsvd][p]Note: These values can change based on defeature bits.[/p][/IntelRsvd]
  */

#define ECAP_VTD_IOMMU_REG               (0x00000010U)

typedef union {
  struct {
    UINT64 c : 1; /**< Page-Walk Coherency */

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               [p]This field indicates if hardware access
                               to the root, context, extended-context and
                               interrupt-remap tables, and second-level paging
                               structures for requests-without-PASID, are
                               coherent (snooped) or not.[/p][list][*]0 =
                               Indicates hardware accesses to remapping structures
                               are non-coherent.[*]1 = Indicates hardware
                               accesses to remapping structures are coherent.[/list][p]Hardware
                               access to advanced fault log, invalidation
                               queue, invalidation semaphore, page-request
                               queue, PASID-table, PASID-state table, and
                               first-level page-tables are always coherent.[/p]
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 qi : 1; /**< Queued Invalidation Support */

                            /* Bits[1:1], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support queued
                               invalidations.[*]1 = Hardware supports queued
                               invalidations.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 dt : 1; /**< Device-TLB Support */

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support device-IOTLBs.[*]1
                               = Hardware supports Device-IOTLBs.[/list][p]Implementations
                               reporting this field as Set must also support
                               Queued Invalidation (QI).[/p][p]Hardware implementations
                               supporting I/O Page Requests (PRS field Set
                               in Extended Capability register) must report
                               a value of 1b in this field.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 ir : 1; /**< Interrupt Remapping Suport */

                            /* Bits[3:3], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support interrupt
                               remapping.[*]1 = Hardware supports interrupt
                               remapping.[/list][p]Implementations reporting
                               this field as Set must also support Queued
                               Invalidation (QI).[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 eim : 1; /**< Extended Interrupt Mode */

                            /* Bits[4:4], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = On Intel64 platforms, hardware
                               supports only 8-bit APIC-IDs (xAPIC mode).[*]1
                               = On Intel64 platforms, hardware supports
                               32-bit APIC-IDs (x2APIC mode).[/list][p]This
                               field is valid only on Intel64 platforms reporting
                               Interrupt Remapping support (IR field Set).[/p]
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[05:05] */

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 pt : 1; /**< Pass Through */

                            /* Bits[6:6], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support pass-through
                               translation type in context entries and extended-context-entries.[*]1
                               = Hardware supports pass-through translation
                               type in context entries and extended-context-entries.[/list][p]Pass-through
                               translation is specified through Translation-Type
                               (T) field value of 10b in context-entries,
                               or T field value of 010b in extended-context-entries.[/p][p]Hardware
                               implementations supporting PASID must report
                               a value of 1b in this field.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 sc : 1; /**< Snoop Control */

                            /* Bits[7:7], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support 1-setting
                               of the SNP field in the page-table entries.[*]1
                               = Hardware supports the 1-setting of the SNP
                               field in the page-table entries.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 iro : 10; /**< IOTLB Register Offset */

                            /* Bits[17:8], Access Type=RO/V, default=0x000000EF*/

                            /*
                               [p]This field specifies the offset to the
                               IOTLB registers relative to the register base
                               address of this remapping hardware unit.[/p][p]If
                               the register base address is X, and the value
                               reported in this field is Y, the address for
                               the first IOTLB invalidation register is calculated
                               as X+(16*Y).[/p] [IntelRsvd][p]Note: These
                               values can change based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[19:18] */

                            /* Bits[19:18], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 mhmv : 4; /**< Maximum Handle Mask Value */

                            /* Bits[23:20], Access Type=RO/V, default=0x0000000F*/

                            /*
                               [p]The value in this field indicates the maximum
                               supported value for the Handle Mask (HM) field
                               in the interrupt entry cache invalidation
                               descriptor (iec_inv_dsc).[/p][p]This field
                               is valid only when the IR field in Extended
                               Capability register is reported as Set.[/p]
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[24:24] */

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 mts : 1; /**< Memory Type Support */

                            /* Bits[25:25], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support Memory
                               Type in first-level translation and Extended
                               Memory type in second-level translation.[*]1
                               = Hardware supports Memory Type in first-level
                               translation and Extended Memory type in second-level
                               translation.[/list][p]This field is valid
                               only when PASID and ECS fields are reported
                               as Set.[/p][p]Remapping hardware units with,
                               one or more devices that operate in processor
                               coherency domain, under its scope must report
                               this field as Set.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 nest : 1; /**< Nested Translation Support */

                            /* Bits[26:26], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support nested
                               translations.[*]1 = Hardware supports nested
                               translations.[/list][p]This field is valid
                               only when PASID field is reported as Set.[/p]
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 rsvd_3 : 2; /**< Undefined - auto filled rsvd_[28:27] */

                            /* Bits[28:27], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 prs : 1; /**< Page Request Support */

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support Page
                               Requests.[*]1 = Hardware supports Page Requests[/list][p]This
                               field is valid only when Device-TLB (DT) field
                               is reported as Set.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 ers : 1; /**< Execute Request Support */

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = H/W does not support requests-with-PASID
                               seeking execute permission.[*]1 = H/W supports
                               requests-with-PASID seeking execute permission.[/list][p]This
                               field is valid only when PASID field is reported
                               as Set.[/p] [IntelRsvd][p]Note: These values
                               can change based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 srs : 1; /**< Supervisor Request Support */

                            /* Bits[31:31], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = H/W does not support requests
                               (with or without PASID) seeking supervisor
                               privilege.[*]1 = H/W supports requests (with
                               or without PASID) seeking supervisor privilege.[/list][p]Hardware
                               implementations reporting Scalable Mode Translation
                               Support (SMTS) field as Clear also report
                               this field as Clear.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 rsvd_4 : 1; /**< Undefined - auto filled rsvd_[32:32] */

                            /* Bits[32:32], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 nwfs : 1; /**< No Write Flag Support */

                            /* Bits[33:33], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware ignores the No Write
                               (NW) flag in Device-TLB translationrequests,
                               and behaves as if NW is always 0.[*]1 = Hardware
                               supports the No Write (NW) flag in Device-TLB
                               translationrequests.[/list][p]This field is
                               valid only when Device-TLB support (DT) field
                               is reported as Set.[/p] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 eafs : 1; /**< Extended Accessed Flag Support */

                            /* Bits[34:34], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support the
                               extended-accessed (EA) bit in first-level
                               paging-structure entries.[*]1 = Hardware supports
                               the extended accessed (EA) bit in first-level
                               paging-structure entries.[/list][p]This field
                               is valid only when PASID field is reported
                               as Set.[/p] [IntelRsvd][p]Note: These values
                               can change based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 pss : 5; /**< PASID Size Supported */

                            /* Bits[39:35], Access Type=RO/V, default=0x00000013*/

                            /*
                               [p]This field reports the PASID size supported
                               by the remapping hardware for requests-with-PASID.
                               A value of N in this field indicates hardware
                               supports PASID field of N+1 bits (For example,
                               value of 7 in this field, indicates 8-bit
                               PASIDs are supported).[/p][p]Requests-with-PASID
                               with PASID value beyond the limit specified
                               by this field are treated as error by the
                               remapping hardware.[/p][p]This field is valid
                               only when PASID field is reported as Set.[/p]
                               [IntelRsvd][p]Note: These values can change
                               based on defeature bits.[/p][/IntelRsvd]
                            */

    UINT64 pasid : 1; /**< Process Address Space ID Support */

                            /* Bits[40:40], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support requests
                               tagged with Process Address Space IDs.[*]1
                               = Hardware supports requests tagged with Process
                               Address Space IDs.[/list] [IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 dit : 1; /**< Device-TLB Invalidation Throttle */

                            /* Bits[41:41], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support Device-TLB
                               Invalidation Throttling.[*]1 = Hardware supports
                               Device-TLB Invalidation Throttling.[/list][p]This
                               field is valid only when Page Request Support
                               (PRS) field is reported as Set.[/p][IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 pds : 1; /**< Page Request Draining Support */

                            /* Bits[42:42], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support Page-Request
                               Drain (PD) flag in Inv_wait_dsc.[*]1 = Hardware
                               supports Page-Request Drain (PD) flag in Inv_wait_dsc.[/list][p]This
                               field is valid only when Device-TLB support
                               field is reported as Set.[/p][IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[/p][/IntelRsvd]
                            */

    UINT64 smts : 1; /**< Scalable Mode Translation Support */

                            /* Bits[43:43], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support Scalable
                               Mode DMA Remapping.[*]1 = Hardware supports
                               Scalable Mode DMA Remapping through scalable-mode
                               context-table and PASID-table structures.[/list][p]Hardware
                               implementations reporting Queued Invalidation
                               (QI) field as Clear also report this field
                               as Clear.[/p][IntelRsvd][p]Note: These values
                               can change based on defeature bits.[[/p][/IntelRsvd]
                            */

    UINT64 vcs : 1; /**< Virtual Command Support */

                            /* Bits[44:44], Access Type=RO, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support command
                               submission to virtual-DMA Remapping hardware.[*]1
                               = Hardware does support command submission
                               to virtual-DMA Remapping hardware.[/list][p]Hardware
                               implementations of this architecture report
                               a value of 0 in this field. Software implementations
                               (emulation) of this architecture may report
                               VCS=1. Software managing remapping hardware
                               should be written to handle both values of
                               VCS.[/p]
                            */

    UINT64 slads : 1; /**< Second-Level Accessed/Dirty Support */

                            /* Bits[45:45], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support Accessed/Dirty
                               bits in Second-Level translation.[*]1 = Hardware
                               supports Accessed/Dirty bits in Second-Level
                               translation.[/list][p][/p][IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[[/p][/IntelRsvd]
                            */

    UINT64 slts : 1; /**< Second-Level Translation Support */

                            /* Bits[46:46], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support PASID-Granular
                               Translation Type of second-level (PGTT=010b)
                               in Scalable-Mode PASID Table Entry.[*]1 =
                               Hardware supports PASID-Granular Translation
                               Type of second-level (PGTT=010b) in Scalable-Mode
                               PASID Table Entry.[/list][p]Hardware implementations
                               reporting Scalable Mode Translation Support
                               (SMTS) as Clear also report this field as
                               Clear.[/p][IntelRsvd][p]Note: These values
                               can change based on defeature bits.[[/p][/IntelRsvd]
                            */

    UINT64 flts : 1; /**< First-Level Translation Support */

                            /* Bits[47:47], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = Hardware does not support PASID-Granular
                               Translation Type of first-level (PGTT=001b)
                               in Scalable-Mode PASID Table Entry.[*]1 =
                               Hardware supports PASID-Granular Translation
                               Type of first-level (PGTT=001b) in Scalable-Mode
                               PASID Table Entry.[/list][p]Hardware implementations
                               reporting Scalable Mode Translation Support
                               (SMTS) as Clear also report this field as
                               Clear.[/p][IntelRsvd][p]Note: These values
                               can change based on defeature bits.[[/p][/IntelRsvd]
                            */

    UINT64 smpwcs : 1; /**< Scalable Mode Page-walk Coherency */

                            /* Bits[48:48], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware access to paging structures
                               accessed through the PASID Table Entry are
                               not snooped.[*]1 = Hardware access to paging
                               structures accessed through the PASID Table
                               Entry are snooped if PWSNP field in the PASID
                               Table Entry is Set. Paging structures accessed
                               through the PASID Table Entry are not snooped
                               if PWSNP field in the PASID Table Entry is
                               Clear.[/list][p]Hardware implementations reporting
                               scalable Mode Translation Support (SMTS) as
                               Clear, also report this field as Clear.[/p][IntelRsvd][p]Note:
                               These values can change based on defeature
                               bits.[[/p][/IntelRsvd]
                            */

    UINT64 rps : 1; /**< RID_PASID Support */

                            /* Bits[49:49], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0: Hardware does not support RID_PASID
                               field in Scalablemode context-entry. It uses
                               the value of 0 for RID_PASID.[*]1: Hardware
                               supports the RID_PASID field in Scalable-mode
                               context-entry.[/list][p]Hardware implementations
                               reporting Scalable Mode Translation Support
                               (SMTS) as Clear also report this field as
                               Clear.[/p]
                            */

#ifdef SV_HOOKS
    UINT64 tdxio : 1; /**< TDX_IO Support */

                            /* Bits[50:50], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = TDX IO not supported.[*]1 = TDX
                               IO supported[/list]
                            */


#else
    UINT64 rsvd_5 : 1;

                            /* Bits[50:50], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

    UINT64 pms : 1; /**< Performance Monitoring Support */

                            /* Bits[51:51], Access Type=RO/V, default=0x00000001*/

                            /* Refer to VT-d specification */

    UINT64 adms : 1; /**< Abort DMA Mode Support */

                            /* Bits[52:52], Access Type=RO, default=0x00000001*/

                            /*
                               [list][*]0 = The hardware does not support
                               Abort DMA Mode.[*]1 = The hardware supports
                               Abort DMA Mode.[/list]
                            */

    UINT64 rprivs : 1; /**< RID-PRIV Supported */

                            /* Bits[53:53], Access Type=RO/V, default=0x00000001*/

                            /*
                               [list][*]0 = The hardware does not support
                               RID-PRIV in scalable-mode context-entry.[*]1
                               = The hardware supports RID-PRIV in scalable-mode
                               context-entry.[/list]
                            */

    UINT64 rsvd_6 : 1; /**< Undefined - auto filled rsvd_[54:54] */

                            /* Bits[54:54], Access Type=RO, default=None*/

                            /* Reserved */

#ifdef SV_HOOKS
    UINT64 hpts : 1; /**< HPT Support */

                            /* Bits[55:55], Access Type=RO/V, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support host
                               permission tables.1 = Hardware supports host
                               permission tables.[/list]
                            */


#else
    UINT64 rsvd_7 : 1;

                            /* Bits[55:55], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

#ifdef SV_HOOKS
    UINT64 ptrs : 1; /**< PASID in Translated Requests Support */

                            /* Bits[56:56], Access Type=RO, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support PASID
                               in Translated Requests.1 = Hardware does support
                               PASID in Translated Requests. Support may
                               be enabled separately for each device by setting
                               the Enable PASID in Translated Requests field
                               in the Scalable-mode Context Entry.[/list]
                            */


#else
    UINT64 rsvd_8 : 1;

                            /* Bits[56:56], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

#ifdef SV_HOOKS
    UINT64 pbds : 1; /**< Page Specific DMA Support */

                            /* Bits[57:57], Access Type=RO, default=0x00000000*/

                            /*
                               [list][*]0 = Hardware does not support Page-Specific
                               Block DMA.[*]1 = Hardware supports Page-Specific
                               Block DMA. Support may be enabled for all
                               devices by setting the Page-Specific Block
                               DMA Enable field in the Root Table Address
                               Register.[/list]
                            */


#else
    UINT64 rsvd_9 : 1;

                            /* Bits[57:57], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

    UINT64 rsvd_10 : 6; /**< Undefined - auto filled rsvd_[63:58] */

                            /* Bits[63:58], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} ECAP_VTD_IOMMU_STRUCT;

#define ECAP_VTD_IOMMU_WIDTH 64

#define ECAP_VTD_IOMMU_C_LSB 0x0000
#define ECAP_VTD_IOMMU_C_SIZE 0x0001
#define ECAP_VTD_IOMMU_QI_LSB 0x0001
#define ECAP_VTD_IOMMU_QI_SIZE 0x0001
#define ECAP_VTD_IOMMU_DT_LSB 0x0002
#define ECAP_VTD_IOMMU_DT_SIZE 0x0001
#define ECAP_VTD_IOMMU_IR_LSB 0x0003
#define ECAP_VTD_IOMMU_IR_SIZE 0x0001
#define ECAP_VTD_IOMMU_EIM_LSB 0x0004
#define ECAP_VTD_IOMMU_EIM_SIZE 0x0001
#define ECAP_VTD_IOMMU_PT_LSB 0x0006
#define ECAP_VTD_IOMMU_PT_SIZE 0x0001
#define ECAP_VTD_IOMMU_SC_LSB 0x0007
#define ECAP_VTD_IOMMU_SC_SIZE 0x0001
#define ECAP_VTD_IOMMU_IRO_LSB 0x0008
#define ECAP_VTD_IOMMU_IRO_SIZE 0x000a
#define ECAP_VTD_IOMMU_MHMV_LSB 0x0014
#define ECAP_VTD_IOMMU_MHMV_SIZE 0x0004
#define ECAP_VTD_IOMMU_MTS_LSB 0x0019
#define ECAP_VTD_IOMMU_MTS_SIZE 0x0001
#define ECAP_VTD_IOMMU_NEST_LSB 0x001a
#define ECAP_VTD_IOMMU_NEST_SIZE 0x0001
#define ECAP_VTD_IOMMU_PRS_LSB 0x001d
#define ECAP_VTD_IOMMU_PRS_SIZE 0x0001
#define ECAP_VTD_IOMMU_ERS_LSB 0x001e
#define ECAP_VTD_IOMMU_ERS_SIZE 0x0001
#define ECAP_VTD_IOMMU_SRS_LSB 0x001f
#define ECAP_VTD_IOMMU_SRS_SIZE 0x0001
#define ECAP_VTD_IOMMU_NWFS_LSB 0x0021
#define ECAP_VTD_IOMMU_NWFS_SIZE 0x0001
#define ECAP_VTD_IOMMU_EAFS_LSB 0x0022
#define ECAP_VTD_IOMMU_EAFS_SIZE 0x0001
#define ECAP_VTD_IOMMU_PSS_LSB 0x0023
#define ECAP_VTD_IOMMU_PSS_SIZE 0x0005
#define ECAP_VTD_IOMMU_PASID_LSB 0x0028
#define ECAP_VTD_IOMMU_PASID_SIZE 0x0001
#define ECAP_VTD_IOMMU_DIT_LSB 0x0029
#define ECAP_VTD_IOMMU_DIT_SIZE 0x0001
#define ECAP_VTD_IOMMU_PDS_LSB 0x002a
#define ECAP_VTD_IOMMU_PDS_SIZE 0x0001
#define ECAP_VTD_IOMMU_SMTS_LSB 0x002b
#define ECAP_VTD_IOMMU_SMTS_SIZE 0x0001
#define ECAP_VTD_IOMMU_VCS_LSB 0x002c
#define ECAP_VTD_IOMMU_VCS_SIZE 0x0001
#define ECAP_VTD_IOMMU_SLADS_LSB 0x002d
#define ECAP_VTD_IOMMU_SLADS_SIZE 0x0001
#define ECAP_VTD_IOMMU_SLTS_LSB 0x002e
#define ECAP_VTD_IOMMU_SLTS_SIZE 0x0001
#define ECAP_VTD_IOMMU_FLTS_LSB 0x002f
#define ECAP_VTD_IOMMU_FLTS_SIZE 0x0001
#define ECAP_VTD_IOMMU_SMPWCS_LSB 0x0030
#define ECAP_VTD_IOMMU_SMPWCS_SIZE 0x0001
#define ECAP_VTD_IOMMU_RPS_LSB 0x0031
#define ECAP_VTD_IOMMU_RPS_SIZE 0x0001
#ifdef SV_HOOKS
#define ECAP_VTD_IOMMU_TDXIO_LSB 0x0032
#define ECAP_VTD_IOMMU_TDXIO_SIZE 0x0001
#endif      // SV_HOOKS
#define ECAP_VTD_IOMMU_PMS_LSB 0x0033
#define ECAP_VTD_IOMMU_PMS_SIZE 0x0001
#define ECAP_VTD_IOMMU_ADMS_LSB 0x0034
#define ECAP_VTD_IOMMU_ADMS_SIZE 0x0001
#define ECAP_VTD_IOMMU_RPRIVS_LSB 0x0035
#define ECAP_VTD_IOMMU_RPRIVS_SIZE 0x0001
#ifdef SV_HOOKS
#define ECAP_VTD_IOMMU_HPTS_LSB 0x0037
#define ECAP_VTD_IOMMU_HPTS_SIZE 0x0001
#endif      // SV_HOOKS
#ifdef SV_HOOKS
#define ECAP_VTD_IOMMU_PTRS_LSB 0x0038
#define ECAP_VTD_IOMMU_PTRS_SIZE 0x0001
#endif      // SV_HOOKS
#ifdef SV_HOOKS
#define ECAP_VTD_IOMMU_PBDS_LSB 0x0039
#define ECAP_VTD_IOMMU_PBDS_SIZE 0x0001
#endif      // SV_HOOKS


/** GSTS_VTD_IOMMU desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/vtu/ioc_iommu_instance/iommu_regs_iommu_regs_sb/GSTS_REG_0_0_0_VTDBAR
  * Std path: VER=1, UVM=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.GSTS_REG_0_0_0_VTDBAR, OSX=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.iommu_regs_iommu_regs_sb.GSTS_REG_0_0_0_VTDBAR
  * Security PolicyGroup: IOMMU_STANDARD
  * Register to report general remapping hardware status.
  */

#define GSTS_VTD_IOMMU_REG               (0x0000001cU)

typedef union {
  struct {
    UINT32 rsvd_0 : 23; /**< Undefined - auto filled rsvd_[22:00] */

                            /* Bits[22:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 cfis : 1; /**< Compatibility Format Interrupt Status */

                            /* Bits[23:23], Access Type=RO/V, default=0x00000000*/

                            /*
                               [p]This field indicates the status of Compatibility
                               format interrupts on Intel64 implementations
                               supporting interrupt-remapping.  The value
                               reported in this field is applicable only
                               when interrupt-remapping is enabled and Extended
                               Interrupt Mode (x2APIC mode) is not enabled.[/p][list][*]0
                               =  Compatibility format interrupts are blocked.[*]1
                               =  Compatibility format interrupts are processed
                               as pass-through (bypassing interrupt remapping).[/list]
                            */

    UINT32 irtps : 1; /**< Interrupt Remapping Pointer Status */

                            /* Bits[24:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               [p]This field indicates the status of the
                               interrupt remapping table pointer in hardware.[/p][p]This
                               field is cleared by hardware when software
                               sets the SIRTP field in the Global Command
                               register.  This field is Set by hardware when
                               hardware completes the set interrupt remap
                               table pointer operation using the value provided
                               in the Interrupt Remapping Table Address register.[/p]
                            */

    UINT32 ires : 1; /**< Interrupt Remapping Enable Status */

                            /* Bits[25:25], Access Type=RO/V, default=0x00000000*/

                            /*
                               [p]This field indicates the status of Interrupt-remapping
                               hardware.[/p][list][*]0 =  Interrupt-remapping
                               hardware is not enabled.[*]1 =  Interrupt-remapping
                               hardware is enabled[/list]
                            */

    UINT32 qies : 1; /**< Queued Invalidation Enable Status */

                            /* Bits[26:26], Access Type=RO/V, default=0x00000000*/

                            /*
                               [p]This field indicates queued invalidation
                               enable status.[/p][list][*]0 =  queued invalidation
                               is not enabled.[*]1 =  queued invalidation
                               is enabled[/list]
                            */

    UINT32 wbfs : 1; /**< Write Buffer Flush Status */

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               [p]This field is valid only for implementations
                               requiring write buffer flushing.  This field
                               indicates the status of the write buffer flush
                               command.  It is:[/p][list][*]Set by hardware
                               when software sets the WBF field in the Global
                               Command register.[*]Cleared by hardware when
                               hardware completes the write buffer flushing
                               operation.[/list]
                            */

    UINT32 afls : 1; /**< Adavanced Fault Logging Status */

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               [p]This field is valid only for implementations
                               supporting advanced fault logging.  It indicates
                               the advanced fault logging status:[/p][list][*]0
                               = Advanced Fault Logging is not enabled.[*]1
                               = Advanced Fault Logging is enabled.[/list]
                            */

    UINT32 fls : 1; /**< Fault Log Status */

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               [p]This field:[/p][list][*]Is cleared by hardware
                               when software Sets the SFL field in the Global
                               Command register.[*]Is Set by hardware whn
                               hardware completes the Set Fault Log Pointer
                               operation using the value provided in the
                               Advanced Fault Log register.[/list]
                            */

    UINT32 rtps : 1; /**< Root Table Pointer Status */

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /*
                               [p]This field indicates the status of the
                               root- table pointer in hardware.[/p][p]This
                               field is cleared by hardware when software
                               sets the SRTP field in the Global Command
                               register.  This field is set by hardware when
                               hardware completes the Set Root Table Pointer
                               operation using the value provided in the
                               Root-Entry Table Address register.[/p]
                            */

    UINT32 tes : 1; /**< Translation Enable Status */

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /*
                               [p]This field indicates the status of DMA-remapping
                               hardware.[/p][list][*]0 =  DMA-remapping hardware
                               is not enabled.[*]1 =  DMA-remapping hardware
                               is enabled[/list]
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSTS_VTD_IOMMU_STRUCT;

#define GSTS_VTD_IOMMU_WIDTH 32

#define GSTS_VTD_IOMMU_CFIS_LSB 0x0017
#define GSTS_VTD_IOMMU_CFIS_SIZE 0x0001
#define GSTS_VTD_IOMMU_IRTPS_LSB 0x0018
#define GSTS_VTD_IOMMU_IRTPS_SIZE 0x0001
#define GSTS_VTD_IOMMU_IRES_LSB 0x0019
#define GSTS_VTD_IOMMU_IRES_SIZE 0x0001
#define GSTS_VTD_IOMMU_QIES_LSB 0x001a
#define GSTS_VTD_IOMMU_QIES_SIZE 0x0001
#define GSTS_VTD_IOMMU_WBFS_LSB 0x001b
#define GSTS_VTD_IOMMU_WBFS_SIZE 0x0001
#define GSTS_VTD_IOMMU_AFLS_LSB 0x001c
#define GSTS_VTD_IOMMU_AFLS_SIZE 0x0001
#define GSTS_VTD_IOMMU_FLS_LSB 0x001d
#define GSTS_VTD_IOMMU_FLS_SIZE 0x0001
#define GSTS_VTD_IOMMU_RTPS_LSB 0x001e
#define GSTS_VTD_IOMMU_RTPS_SIZE 0x0001
#define GSTS_VTD_IOMMU_TES_LSB 0x001f
#define GSTS_VTD_IOMMU_TES_SIZE 0x0001


/** PMEN_VTD_IOMMU desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/vtu/ioc_iommu_instance/iommu_regs_iommu_regs_sb/PMEN_REG_0_0_0_VTDBAR
  * Std path: VER=1, UVM=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.PMEN_REG_0_0_0_VTDBAR, OSX=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.iommu_regs_iommu_regs_sb.PMEN_REG_0_0_0_VTDBAR
  * Security PolicyGroup: IOMMU_STANDARD
  * [p]Register to enable the DMA-protected memory regions setup through the PLMBASE,..PLMLIMT, PHMBASE, PHMLIMIT registers.  This register is always treated as RO for implementations not supporting protected memory regions (PLMR and PHMR fields reported as Clear in the Capability register).[/p][p]Protected memory regions may be used by software to securely initialize remapping structures in memory.  To avoid impact to legacy BIOS usage of memory, software is recommended to not overlap protected memory regions with any reserved memory regions of the platform reported through the Reserved Memory Region Reporting (RMRR) structures.[/p]
  */

#define PMEN_VTD_IOMMU_REG               (0x00000064U)

typedef union {
  struct {
    UINT32 prs : 1; /**< Protected Region Status */

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               [p]This field indicates the status of protected
                               memory region(s):[/p][list][*]0 = Protected
                               memory region(s) disabled.[*]1 = Protected
                               memory region(s) enabled.[/list]
                            */

    UINT32 rsvd_0 : 30; /**< Undefined - auto filled rsvd_[30:01] */

                            /* Bits[30:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 epm : 1; /**< Enable Protected Memory */

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               [p]This field controls DMA accesses to the
                               protected low-memory and protected high-memory
                               regions.[/p][list][*]0 = Protected memory
                               regions are disabled. [*]1 = Protected memory
                               regions are enabled.DMA requests accessing
                               protected memory regions are handled as follows:[list][*]When
                               DMA remapping is not enabled, all DMA requests
                               accessing protected memory regions are blocked.[*]When
                               DMA remapping is enabled:[list][*]DMA requests
                               processed as pass-through (Translation Type
                               value of 10b in Context-Entry) and accessing
                               the protected memory regions are blocked.[*]DMA
                               requests with translated address (AT=10b)
                               and accessing the protected memory regions
                               are blocked.[*]DMA requests that are subject
                               to address remapping, and accessing the protected
                               memory regions may or may not be blocked by
                               hardware.  For such requests, software must
                               not depend on hardware protection of the protected
                               memory regions, and instead program the DMA-remapping
                               page-tables to not allow DMA to protected
                               memory regions.[/list][/list][/list][p]Remapping
                               hardware access to the remapping structures
                               are not subject to protected memory region
                               checks.[/p][p]DMA requests blocked due to
                               protected memory region violation are not
                               recorded or reported as remapping faults.[/p][p]Hardware
                               reports the status of the protected memory
                               enable/disable operation through the PRS field
                               in this register.Hardware implementations
                               supporting DMA draining must drain any in-flight
                               translated DMA requests queued within the
                               Root-Complex before indicating the protected
                               memory region as enabled through the PRS field.[/p][p]After
                               writing to this field software must wait for
                               the operation to be completed and reflected
                               in the PRS status field (bit 0) before changing
                               the value of this field again.[/p]
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PMEN_VTD_IOMMU_STRUCT;

#define PMEN_VTD_IOMMU_WIDTH 32

#define PMEN_VTD_IOMMU_PRS_LSB 0x0000
#define PMEN_VTD_IOMMU_PRS_SIZE 0x0001
#define PMEN_VTD_IOMMU_EPM_LSB 0x001f
#define PMEN_VTD_IOMMU_EPM_SIZE 0x0001


/** PLMBASE_VTD_IOMMU desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/vtu/ioc_iommu_instance/iommu_regs_iommu_regs_sb/PLMBASE_REG_0_0_0_VTDBAR
  * Std path: VER=1, UVM=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.PLMBASE_REG_0_0_0_VTDBAR, OSX=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.iommu_regs_iommu_regs_sb.PLMBASE_REG_0_0_0_VTDBAR
  * Security PolicyGroup: IOMMU_STANDARD
  * [p]Register to set up the base address of DMA-protected low-memory region below 4GB.  This register must be set up before enabling protected memory through PMEN_REG, and must not be updated when protected memory regions are enabled.[/p][p]This register is always treated as RO for implementations not supporting protected low memory region (PLMR field reported as Clear in the Capability register).[/p][p]The alignment of the protected low memory region base depends on the number of  reserved bits (N:0) of this register. Software may determine N by writing all 1s to this register, and finding the most significant zero bit position with 0 in the value read back from the register.  Bits N:0 of this register is decoded by hardware as all 0s...Software must setup the protected low memory region below 4GB.[/p][p]Software must not modify this register when protected memory regions are enabled (PRS field Set in PMEN_REG).[/p]
  */

#define PLMBASE_VTD_IOMMU_REG            (0x00000068U)

typedef union {
  struct {
    UINT32 rsvd_0 : 20; /**< Undefined - auto filled rsvd_[19:00] */

                            /* Bits[19:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 plmb : 12; /**< Protected Low-Memory Base */

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /*
                               [p]This register specifies the base of protected
                               low-memory region in system memory.[/p]
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PLMBASE_VTD_IOMMU_STRUCT;

#define PLMBASE_VTD_IOMMU_WIDTH 32

#define PLMBASE_VTD_IOMMU_PLMB_LSB 0x0014
#define PLMBASE_VTD_IOMMU_PLMB_SIZE 0x000c


/** PLMLIMIT_VTD_IOMMU desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/vtu/ioc_iommu_instance/iommu_regs_iommu_regs_sb/PLMLIMIT_REG_0_0_0_VTDBAR
  * Std path: VER=1, UVM=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.PLMLIMIT_REG_0_0_0_VTDBAR, OSX=SAF_SOUTH.saf_south.vtu.ioc_iommu_instance.iommu_regs_iommu_regs_sb.PLMLIMIT_REG_0_0_0_VTDBAR
  * Security PolicyGroup: IOMMU_STANDARD
  * [p]Register to set up the limit address of DMA-protected low-memory region below 4GB.  This register must be set up before enabling protected memory through PMEN_REG, and must not be updated when protected memory regions are enabled[/p][p]This register is always treated as RO for implementations not supporting protected low memory region (PLMR field reported as Clear in the Capability register)[/p][p]The alignment of the protected low memory region limit depends on the number of reserved bits (N:0) of this register.  Software may determine N by writing all 1s to this register, and finding most significant zero bit position with 0 in the value read back from the register.  Bits N:0 of the limit register is decoded by hardware as all 1s[/p][p]The Protected low-memory base and limit registers functions as follows:[/p][list][*]Programming the protected low-memory base and limit registers with the same value in bits 31: (N+1) specifies a protected low-memory region of size 2(N+1) bytes[*]Programming the protected low-memory limit register with a value less than the protected low-memory base register disables the protected low-memory region[/list][p]Software must not modify this register when protected memory regions are enabled (PRS field Set in PMEN_REG).[/p]
  */

#define PLMLIMIT_VTD_IOMMU_REG           (0x0000006cU)

typedef union {
  struct {
    UINT32 rsvd_0 : 20; /**< Undefined - auto filled rsvd_[19:00] */

                            /* Bits[19:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 plml : 12; /**< Protected Low-Memory Limit */

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /*
                               [p]This register specifies the last host physical
                               address of the DMA-protected low-memory region
                               in system memory.[/p]
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PLMLIMIT_VTD_IOMMU_STRUCT;

#define PLMLIMIT_VTD_IOMMU_WIDTH 32

#define PLMLIMIT_VTD_IOMMU_PLML_LSB 0x0014
#define PLMLIMIT_VTD_IOMMU_PLML_SIZE 0x000c

//************************************************ /RegisterStructs




#endif      // _VTD_IOMMU_h

