/** @file
  IOC_VMD.h

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

@par Specification
**/

/* The following security policy groups are used by registers in this file:     */
/* IOP_OS_W_PG                                                                  */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI | ESE_SAI |             */
/*    PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | IOM_SAI |                     */
/*    DFX_INTEL_PRODUCTION_SAI                                                  */

#ifndef _IOC_VMD_PTL_CDIE_H_
#define _IOC_VMD_PTL_CDIE_H_

#include <Base.h>


// IP GUID: b387c877-2746-4bd7-ab1a-50933e3f944e
#define IOC_VMD_ID 0xb387c87727464bd7

//************************************************ RegisterStructs


/** CFGBAR_IOC_VMD desc:
  * Register default value:        0x0000000C
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/CFGBAR_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * VMD Configuration Base Address
  */

#define CFGBAR_IOC_VMD_REG               (0x00000010U)

typedef union {
  struct {
    UINT64 memory_space_indicator : 1; /**< Memory Space Indicator */

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* BAR resource is memory (as opposed to I/O). */

    UINT64 type_f : 2; /**< Type CFGBAR */

                            /* Bits[2:1], Access Type=RW/L, default=0x00000002*/

                            /*
                               Memory type claimed by this BAR is 64-bit
                               addressable
                            */

    UINT64 prefetchable : 1; /**< Prefetchable CFGBAR */

                            /* Bits[3:3], Access Type=RW/L, default=0x00000001*/

                            /* BAR points to Prefetchable memory. */

    UINT64 reserved : 16; /**< Reserved */

                            /* Bits[19:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */

    UINT64 memory_base_address : 22; /**< Memory Base Address */

                            /* Bits[41:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the CFGBAR in memory
                               space. The size is programmed in CFGBARSZ
                               by BIOS. CFGBARSZ specifies the lowest order
                               address bit that is writeable. The minimum
                               granularity is 1MB.           [br]If CFGBAR.Type
                               = 10b, then bits 63:32 are writeable.
                               [br]If CFGBAR.Type = 01b, then bits 63:32
                               are read-only.
                            */

    UINT64 rsvd_0 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} CFGBAR_IOC_VMD_STRUCT;

#define CFGBAR_IOC_VMD_WIDTH 64

#define CFGBAR_IOC_VMD_MEMORY_SPACE_INDICATOR_LSB 0x0000
#define CFGBAR_IOC_VMD_MEMORY_SPACE_INDICATOR_SIZE 0x0001
#define CFGBAR_IOC_VMD_TYPE_F_LSB 0x0001
#define CFGBAR_IOC_VMD_TYPE_F_SIZE 0x0002
#define CFGBAR_IOC_VMD_PREFETCHABLE_LSB 0x0003
#define CFGBAR_IOC_VMD_PREFETCHABLE_SIZE 0x0001
#define CFGBAR_IOC_VMD_RESERVED_LSB 0x0004
#define CFGBAR_IOC_VMD_RESERVED_SIZE 0x0010
#define CFGBAR_IOC_VMD_MEMORY_BASE_ADDRESS_LSB 0x0014
#define CFGBAR_IOC_VMD_MEMORY_BASE_ADDRESS_SIZE 0x0016


/** MEMBAR1_IOC_VMD desc:
  * Register default value:        0x0000000C
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/MEMBAR1_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * VMD Memory Base Address Range x
  */

#define MEMBAR1_IOC_VMD_REG              (0x00000018U)

typedef union {
  struct {
    UINT64 memory_space_indicator : 1; /**< Memory Space Indicator */

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* BAR resource is memory (as opposed to I/O). */

    UINT64 type_f : 2; /**< Type MEMBAR1 */

                            /* Bits[2:1], Access Type=RW/L, default=0x00000002*/

                            /*
                               Memory type claimed by this BAR is 64-bit
                               addressable
                            */

    UINT64 prefetchable : 1; /**< Prefetchable MEMBAR1 */

                            /* Bits[3:3], Access Type=RW/L, default=0x00000001*/

                            /* BAR points to Prefetchable memory. */

    UINT64 reserved : 8; /**< Reserved */

                            /* Bits[11:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */

    UINT64 memory_base_address : 30; /**< Memory Base Address */

                            /* Bits[41:12], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the MEMBARx in memory
                               space. The size is programmed in MEMBARxSZ
                               by BIOS. MEMBARxSZ specifies the lowest order
                               address bit that is writeable. The minimum
                               granularity is 4kB.           [br]If MEMBARx.Type
                               = 10b, then bits 63:32 are writeable.
                               [br]If MEMBARx.Type = 01b, then bits 63:32
                               are read-only.           [br]If MEMBARxSZ
                               = 0, then all address bits are read-only.
                               [br]If MEMBARxSZ &#60; 12, then the effective
                               value is 12.
                            */

    UINT64 rsvd_0 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MEMBAR1_IOC_VMD_STRUCT;

#define MEMBAR1_IOC_VMD_WIDTH 64

#define MEMBAR1_IOC_VMD_MEMORY_SPACE_INDICATOR_LSB 0x0000
#define MEMBAR1_IOC_VMD_MEMORY_SPACE_INDICATOR_SIZE 0x0001
#define MEMBAR1_IOC_VMD_TYPE_F_LSB 0x0001
#define MEMBAR1_IOC_VMD_TYPE_F_SIZE 0x0002
#define MEMBAR1_IOC_VMD_PREFETCHABLE_LSB 0x0003
#define MEMBAR1_IOC_VMD_PREFETCHABLE_SIZE 0x0001
#define MEMBAR1_IOC_VMD_RESERVED_LSB 0x0004
#define MEMBAR1_IOC_VMD_RESERVED_SIZE 0x0008
#define MEMBAR1_IOC_VMD_MEMORY_BASE_ADDRESS_LSB 0x000c
#define MEMBAR1_IOC_VMD_MEMORY_BASE_ADDRESS_SIZE 0x001e


/** MEMBAR2_IOC_VMD desc:
  * Register default value:        0x0000000C
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/MEMBAR2_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * VMD Memory Base Address Range x
  */

#define MEMBAR2_IOC_VMD_REG              (0x00000020U)

typedef union {
  struct {
    UINT64 memory_space_indicator : 1; /**< Memory Space Indicator */

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* BAR resource is memory (as opposed to I/O). */

    UINT64 type_f : 2; /**< Type MEMBAR2 */

                            /* Bits[2:1], Access Type=RW/L, default=0x00000002*/

                            /*
                               Memory type claimed by this BAR is 64-bit
                               addressable
                            */

    UINT64 prefetchable : 1; /**< Prefetchable MEMBAR2 */

                            /* Bits[3:3], Access Type=RW/L, default=0x00000001*/

                            /* BAR points to Prefetchable memory. */

    UINT64 reserved : 9; /**< Reserved */

                            /* Bits[12:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */

    UINT64 memory_base_address : 29; /**< Memory Base Address */

                            /* Bits[41:13], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the MEMBARx in memory
                               space. The size is programmed in MEMBARxSZ
                               by BIOS. MEMBARxSZ specifies the lowest order
                               address bit that is writeable. The minimum
                               granularity is 4kB.           [br]If MEMBARx.Type
                               = 10b, then bits 63:32 are writeable.
                               [br]If MEMBARx.Type = 01b, then bits 63:32
                               are read-only.           [br]If MEMBARxSZ
                               = 0, then all address bits are read-only.
                               [br]If MEMBARxSZ &#60; 12, then the effective
                               value is 12.
                            */

    UINT64 rsvd_0 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MEMBAR2_IOC_VMD_STRUCT;

#define MEMBAR2_IOC_VMD_WIDTH 64

#define MEMBAR2_IOC_VMD_MEMORY_SPACE_INDICATOR_LSB 0x0000
#define MEMBAR2_IOC_VMD_MEMORY_SPACE_INDICATOR_SIZE 0x0001
#define MEMBAR2_IOC_VMD_TYPE_F_LSB 0x0001
#define MEMBAR2_IOC_VMD_TYPE_F_SIZE 0x0002
#define MEMBAR2_IOC_VMD_PREFETCHABLE_LSB 0x0003
#define MEMBAR2_IOC_VMD_PREFETCHABLE_SIZE 0x0001
#define MEMBAR2_IOC_VMD_RESERVED_LSB 0x0004
#define MEMBAR2_IOC_VMD_RESERVED_SIZE 0x0009
#define MEMBAR2_IOC_VMD_MEMORY_BASE_ADDRESS_LSB 0x000d
#define MEMBAR2_IOC_VMD_MEMORY_BASE_ADDRESS_SIZE 0x001d

/** VMCAP_IOC_VMD desc:
  * Register default value:        0x00000001
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/VMCAP_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * Volume Management Capability
  */

#define VMCAP_IOC_VMD_REG                (0x00000040U)

typedef union {
  struct {
    UINT32 bus_restrict_cap : 1; /**< BUS_RESTRICT_CAP */

                            /* Bits[0:0], Access Type=RW/L, default=0x00000001*/

                            /*
                               0: Indicates that hardware supports a separate
                               set of bus encodings for VMD. Bus restrictrions
                               are not required.           [br]1: Indicates
                               that bus restrictions are required. BIOS must
                               program VMCONFIG.BUS_RESTRICTIONS field. VMD
                               may only use 128 bus numbers unless VMD owns
                               all devices in its ACPI domain. VMD driver
                               reads this field to determine if bus number
                               restrictions are required. When set, VMD driver
                               must also read VMCONFIG.BUS_RESTRICTIONS field
                               to determine what range of bus numbers to
                               use for Secondary and Subbordinate bus ranges.
                            */

    UINT32 reserved : 31; /**< Reserved */

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VMCAP_IOC_VMD_STRUCT;

#define VMCAP_IOC_VMD_WIDTH 32

#define VMCAP_IOC_VMD_BUS_RESTRICT_CAP_LSB 0x0000
#define VMCAP_IOC_VMD_BUS_RESTRICT_CAP_SIZE 0x0001
#define VMCAP_IOC_VMD_RESERVED_LSB 0x0001
#define VMCAP_IOC_VMD_RESERVED_SIZE 0x001f

/** VMCONFIG_IOC_VMD desc:
  * Register default value:        0x0191FE00
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/VMCONFIG_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  */

#define VMCONFIG_IOC_VMD_REG             (0x00000044U)

typedef union {
  struct {
    UINT32 vmdlock : 1; /**< VMD Lock Register */

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Locks registers in the Volume Management Device.
                               This must be set by BIOS after programming
                               VMD registers.
                            */

    UINT32 msi_rmp_dis : 1; /**< MSI_RMP_DIS */

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               MSI Remapping Disable - When set, causes MSI
                               transactions to bypass the remap indexing
                               that triggers a native VMD MSI-X Table Entries.
                               The original MSI will be set up without indexing,
                               however, the Requestor ID will be changed
                               to match the VMD before being translated by
                               VTd. This bit is programmed by the OS or by
                               the VMD Driver.
                            */

    UINT32 reserved1 : 2; /**< RSVD */

                            /* Bits[3:2], Access Type=RO, default=0x00000000*/

                            /* RSVD */

    UINT32 devid_select : 4; /**< DEVID SELECT */

                            /* Bits[7:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               NOTE: This field is not used for client!
                               For Server: Selects the Device ID that appears
                               in the Device ID register. Values are device
                               specific. Only BIOS may program this register
                               field.  Volume Management Device v2.0 for
                               Server uses Device ID values 0x28C0 to 0x28CF
                            */

    UINT32 bus_restrictions : 2; /**< BUS_RESTRICTIONS */

                            /* Bits[9:8], Access Type=RW/L, default=0x00000002*/

                            /*
                               00: VMD-Owned Root Port and PCI Switch Bridges
                               must use the 0-127 of bus numbers (0 is the
                               first bus in CFGBAR at offset 0). [br]01:
                               VMD-Owned Root Port and PCI Switch Bridges
                               must use the 0,129-255 of bus numbers (0 is
                               the first bus in CFGBAR at offset 0). [br]10-11:
                               VMD-Owned Root Port and PCI Switch Bridges
                               must use 0,225-255 bus numbers (0 is the first
                               bus in CFGBAR at offset 0).   This field is
                               read only when VMCAP.BUS_RESTRICT_CAP bit
                               is clear. [br]BIOS programs this field to
                               a value that ensures the OS visible busses
                               in the ACPI domain where this VMD resides
                               do not conflict with bus number encodings
                               assigned to VMD. [br]VMD driver reads this
                               field to select the appropriate range of bus
                               numbers to use in Secondary and Subbordinate
                               Bus registers of PCIe Root Port and Switch
                               Bridges. [br]VMD driver does not use this
                               field to determine the primary bus number
                               for PCIe Root Port and Switch Bridges owned
                               by VMD. The primary bus number must be the
                               same as the Root Bus number that the VMD Root
                               Complext Integrated Endpoint resides on.
                            */

    UINT32 did_assign : 16; /**< DID_ASSIGN */

                            /* Bits[25:10], Access Type=RW/L, default=0x0000647F*/

                            /*
                               This field is used to program the PCIE standard
                               Device ID register of VMD device.  Only BIOS
                               may program this register field.
                            */

    UINT32 reserved0 : 6; /**< RSVD */

                            /* Bits[31:26], Access Type=RO, default=0x00000000*/

                            /* RSVD */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VMCONFIG_IOC_VMD_STRUCT;

#define VMCONFIG_IOC_VMD_WIDTH 32

#define VMCONFIG_IOC_VMD_VMDLOCK_LSB 0x0000
#define VMCONFIG_IOC_VMD_VMDLOCK_SIZE 0x0001
#define VMCONFIG_IOC_VMD_MSI_RMP_DIS_LSB 0x0001
#define VMCONFIG_IOC_VMD_MSI_RMP_DIS_SIZE 0x0001
#define VMCONFIG_IOC_VMD_RESERVED1_LSB 0x0002
#define VMCONFIG_IOC_VMD_RESERVED1_SIZE 0x0002
#define VMCONFIG_IOC_VMD_DEVID_SELECT_LSB 0x0004
#define VMCONFIG_IOC_VMD_DEVID_SELECT_SIZE 0x0004
#define VMCONFIG_IOC_VMD_BUS_RESTRICTIONS_LSB 0x0008
#define VMCONFIG_IOC_VMD_BUS_RESTRICTIONS_SIZE 0x0002
#define VMCONFIG_IOC_VMD_DID_ASSIGN_LSB 0x000a
#define VMCONFIG_IOC_VMD_DID_ASSIGN_SIZE 0x0010
#define VMCONFIG_IOC_VMD_RESERVED0_LSB 0x001a
#define VMCONFIG_IOC_VMD_RESERVED0_SIZE 0x0006

/** VMASSIGN_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/VMASSIGN_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * Volume Management Device Assignment
  */

#define VMASSIGN_IOC_VMD_REG             (0x00000048U)

typedef union {
  struct {
    UINT32 port_assign : 32; /**< Port_Assign */

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on
                               the same bus number as VMD (RootBus0). For
                               each bit set, the associated device will be
                               assigned to the VMD and hidden from the OS.
                               Each bit may be Read-Write for BIOS or Read
                               Only based on the specific VMD implementation.
                               To discover which bits are writeable, BIOS
                               may write 1's to all bits and read the value
                               back.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VMASSIGN_IOC_VMD_STRUCT;

#define VMASSIGN_IOC_VMD_WIDTH 32

#define VMASSIGN_IOC_VMD_PORT_ASSIGN_LSB 0x0000
#define VMASSIGN_IOC_VMD_PORT_ASSIGN_SIZE 0x0020

/** CFGBARSZ_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/CFGBARSZ_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * VMD Configuration BAR Size
  */

#define CFGBARSZ_IOC_VMD_REG             (0x0000004cU)

typedef union {
  struct {
    UINT8 size : 8; /**< Size */

                            /* Bits[7:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Value indicating the size of CFGBAR. This
                               value is written by BIOS prior to enumeration.
                               The value indicates the lowest order address
                               bit that is writeable in CFGBAR. The only
                               legal settings are 20 to 28 (decimal), representing
                               BAR sizes of 1MB through 256MB.
                               [br]If CFGBARSZ = 0, then all address bits
                               are read-only.           [br]If CFGBARSZ &#60;
                               20, then the effective value is 20.
                               [br]If CFGBARSZ &#62; 28, then the effective
                               value is 28.
                            */

  }     Bits;
  UINT8 Data;
} CFGBARSZ_IOC_VMD_STRUCT;

#define CFGBARSZ_IOC_VMD_WIDTH 8

#define CFGBARSZ_IOC_VMD_SIZE_LSB 0x0000
#define CFGBARSZ_IOC_VMD_SIZE_SIZE 0x0008

/** MEMBAR1SZ_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/MEMBAR1SZ_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * VMD Memory BAR x Size
  */

#define MEMBAR1SZ_IOC_VMD_REG            (0x0000004dU)

typedef union {
  struct {
    UINT8 size : 8; /**< Size */

                            /* Bits[7:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Value indicating the size of MEMBARxSZ. This
                               value is written by BIOS prior to enumeration.
                               The value indicates the lowest order address
                               bit that is writeable in MEMBARxSZ. The only
                               legal settings are 12 to 47 (decimal), representing
                               BAR sizes of 4kB through 256TB.
                               [br]If MEMBARxSZ = 0, then all address bits
                               are read-only.           [br]If MEMBARxSZ
                               &#60; 12, then the effective value is 12.
                               [br]If MEMBARxSZ &#62; 47, then the effective
                               value is 47.
                            */

  }     Bits;
  UINT8 Data;
} MEMBAR1SZ_IOC_VMD_STRUCT;

#define MEMBAR1SZ_IOC_VMD_WIDTH 8

#define MEMBAR1SZ_IOC_VMD_SIZE_LSB 0x0000
#define MEMBAR1SZ_IOC_VMD_SIZE_SIZE 0x0008

/** MEMBAR2SZ_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/MEMBAR2SZ_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * VMD Memory BAR x Size
  */

#define MEMBAR2SZ_IOC_VMD_REG            (0x0000004eU)

typedef union {
  struct {
    UINT8 size : 8; /**< Size */

                            /* Bits[7:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Value indicating the size of MEMBARxSZ. This
                               value is written by BIOS prior to enumeration.
                               The value indicates the lowest order address
                               bit that is writeable in MEMBARxSZ. The only
                               legal settings are 12 to 47 (decimal), representing
                               BAR sizes of 4kB through 256TB.
                               [br]If MEMBARxSZ = 0, then all address bits
                               are read-only.           [br]If MEMBARxSZ
                               &#60; 12, then the effective value is 12.
                               [br]If MEMBARxSZ &#62; 47, then the effective
                               value is 47.
                            */

  }     Bits;
  UINT8 Data;
} MEMBAR2SZ_IOC_VMD_STRUCT;

#define MEMBAR2SZ_IOC_VMD_WIDTH 8

#define MEMBAR2SZ_IOC_VMD_SIZE_LSB 0x0000
#define MEMBAR2SZ_IOC_VMD_SIZE_SIZE 0x0008


/** FN0_ASSIGN_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/FN0_ASSIGN_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * Volume Management Function x Assignment
  */

#define FN0_ASSIGN_IOC_VMD_REG           (0x00000050U)

typedef union {
  struct {
    UINT32 device : 32; /**< Device */

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on
                               the same bus number as VMD (Bus0). For each
                               bit set, the associated device function x
                               will be assigned to the VMD and hidden from
                               the OS.           [br]Each bit may be Read-Write
                               for BIOS or Read Only based on the specific
                               VMD implementation. To discover which bits
                               are writeable, BIOS may write 1's to all bits
                               and read the value back.           [br]If
                               a bit in this register is set for a device,
                               then a dummy function will appear at function
                               x in its place in OS visible configuration
                               space.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FN0_ASSIGN_IOC_VMD_STRUCT;

#define FN0_ASSIGN_IOC_VMD_WIDTH 32

#define FN0_ASSIGN_IOC_VMD_DEVICE_LSB 0x0000
#define FN0_ASSIGN_IOC_VMD_DEVICE_SIZE 0x0020

/** FN1_ASSIGN_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/FN1_ASSIGN_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * Volume Management Function x Assignment
  */

#define FN1_ASSIGN_IOC_VMD_REG           (0x00000054U)

typedef union {
  struct {
    UINT32 device : 32; /**< Device */

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on
                               the same bus number as VMD (Bus0). For each
                               bit set, the associated device function x
                               will be assigned to the VMD and hidden from
                               the OS.           [br]Each bit may be Read-Write
                               for BIOS or Read Only based on the specific
                               VMD implementation. To discover which bits
                               are writeable, BIOS may write 1's to all bits
                               and read the value back.           [br]If
                               a bit in this register is set for a device,
                               then a dummy function will appear at function
                               x in its place in OS visible configuration
                               space.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FN1_ASSIGN_IOC_VMD_STRUCT;

#define FN1_ASSIGN_IOC_VMD_WIDTH 32

#define FN1_ASSIGN_IOC_VMD_DEVICE_LSB 0x0000
#define FN1_ASSIGN_IOC_VMD_DEVICE_SIZE 0x0020

/** FN2_ASSIGN_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/FN2_ASSIGN_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * Volume Management Function x Assignment
  */

#define FN2_ASSIGN_IOC_VMD_REG           (0x00000058U)

typedef union {
  struct {
    UINT32 device : 32; /**< Device */

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on
                               the same bus number as VMD (Bus0). For each
                               bit set, the associated device function x
                               will be assigned to the VMD and hidden from
                               the OS.           [br]Each bit may be Read-Write
                               for BIOS or Read Only based on the specific
                               VMD implementation. To discover which bits
                               are writeable, BIOS may write 1's to all bits
                               and read the value back.           [br]If
                               a bit in this register is set for a device,
                               then a dummy function will appear at function
                               x in its place in OS visible configuration
                               space.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FN2_ASSIGN_IOC_VMD_STRUCT;

#define FN2_ASSIGN_IOC_VMD_WIDTH 32

#define FN2_ASSIGN_IOC_VMD_DEVICE_LSB 0x0000
#define FN2_ASSIGN_IOC_VMD_DEVICE_SIZE 0x0020

/** FN3_ASSIGN_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/FN3_ASSIGN_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * Volume Management Function x Assignment
  */

#define FN3_ASSIGN_IOC_VMD_REG           (0x0000005cU)

typedef union {
  struct {
    UINT32 device : 32; /**< Device */

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on
                               the same bus number as VMD (Bus0). For each
                               bit set, the associated device function x
                               will be assigned to the VMD and hidden from
                               the OS.           [br]Each bit may be Read-Write
                               for BIOS or Read Only based on the specific
                               VMD implementation. To discover which bits
                               are writeable, BIOS may write 1's to all bits
                               and read the value back.           [br]If
                               a bit in this register is set for a device,
                               then a dummy function will appear at function
                               x in its place in OS visible configuration
                               space.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FN3_ASSIGN_IOC_VMD_STRUCT;

#define FN3_ASSIGN_IOC_VMD_WIDTH 32

#define FN3_ASSIGN_IOC_VMD_DEVICE_LSB 0x0000
#define FN3_ASSIGN_IOC_VMD_DEVICE_SIZE 0x0020

/** FN4_ASSIGN_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/FN4_ASSIGN_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * Volume Management Function x Assignment
  */

#define FN4_ASSIGN_IOC_VMD_REG           (0x00000060U)

typedef union {
  struct {
    UINT32 device : 32; /**< Device */

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on
                               the same bus number as VMD (Bus0). For each
                               bit set, the associated device function x
                               will be assigned to the VMD and hidden from
                               the OS.           [br]Each bit may be Read-Write
                               for BIOS or Read Only based on the specific
                               VMD implementation. To discover which bits
                               are writeable, BIOS may write 1's to all bits
                               and read the value back.           [br]If
                               a bit in this register is set for a device,
                               then a dummy function will appear at function
                               x in its place in OS visible configuration
                               space.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FN4_ASSIGN_IOC_VMD_STRUCT;

#define FN4_ASSIGN_IOC_VMD_WIDTH 32

#define FN4_ASSIGN_IOC_VMD_DEVICE_LSB 0x0000
#define FN4_ASSIGN_IOC_VMD_DEVICE_SIZE 0x0020

/** FN5_ASSIGN_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/FN5_ASSIGN_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * Volume Management Function x Assignment
  */

#define FN5_ASSIGN_IOC_VMD_REG           (0x00000064U)

typedef union {
  struct {
    UINT32 device : 32; /**< Device */

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on
                               the same bus number as VMD (Bus0). For each
                               bit set, the associated device function x
                               will be assigned to the VMD and hidden from
                               the OS.           [br]Each bit may be Read-Write
                               for BIOS or Read Only based on the specific
                               VMD implementation. To discover which bits
                               are writeable, BIOS may write 1's to all bits
                               and read the value back.           [br]If
                               a bit in this register is set for a device,
                               then a dummy function will appear at function
                               x in its place in OS visible configuration
                               space.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FN5_ASSIGN_IOC_VMD_STRUCT;

#define FN5_ASSIGN_IOC_VMD_WIDTH 32

#define FN5_ASSIGN_IOC_VMD_DEVICE_LSB 0x0000
#define FN5_ASSIGN_IOC_VMD_DEVICE_SIZE 0x0020

/** FN6_ASSIGN_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/FN6_ASSIGN_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * Volume Management Function x Assignment
  */

#define FN6_ASSIGN_IOC_VMD_REG           (0x00000068U)

typedef union {
  struct {
    UINT32 device : 32; /**< Device */

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on
                               the same bus number as VMD (Bus0). For each
                               bit set, the associated device function x
                               will be assigned to the VMD and hidden from
                               the OS.           [br]Each bit may be Read-Write
                               for BIOS or Read Only based on the specific
                               VMD implementation. To discover which bits
                               are writeable, BIOS may write 1's to all bits
                               and read the value back.           [br]If
                               a bit in this register is set for a device,
                               then a dummy function will appear at function
                               x in its place in OS visible configuration
                               space.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FN6_ASSIGN_IOC_VMD_STRUCT;

#define FN6_ASSIGN_IOC_VMD_WIDTH 32

#define FN6_ASSIGN_IOC_VMD_DEVICE_LSB 0x0000
#define FN6_ASSIGN_IOC_VMD_DEVICE_SIZE 0x0020

/** FN7_ASSIGN_IOC_VMD desc:
  * Register default value:        0x00000000
  * Register full path in IP: SAF_SOUTH/saf_south/ioc/IOP_VMD_REGS/IOP_VMD_0_14_0_PCI/FN7_ASSIGN_0_14_0_PCI
  * Security PolicyGroup: IOP_OS_W_PG
  * Volume Management Function x Assignment
  */

#define FN7_ASSIGN_IOC_VMD_REG           (0x0000006cU)

typedef union {
  struct {
    UINT32 device : 32; /**< Device */

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on
                               the same bus number as VMD (Bus0). For each
                               bit set, the associated device function x
                               will be assigned to the VMD and hidden from
                               the OS.           [br]Each bit may be Read-Write
                               for BIOS or Read Only based on the specific
                               VMD implementation. To discover which bits
                               are writeable, BIOS may write 1's to all bits
                               and read the value back.           [br]If
                               a bit in this register is set for a device,
                               then a dummy function will appear at function
                               x in its place in OS visible configuration
                               space.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FN7_ASSIGN_IOC_VMD_STRUCT;

#define FN7_ASSIGN_IOC_VMD_WIDTH 32

#define FN7_ASSIGN_IOC_VMD_DEVICE_LSB 0x0000
#define FN7_ASSIGN_IOC_VMD_DEVICE_SIZE 0x0020

//************************************************ /RegisterStructs




#endif      // _IOC_VMD_h

