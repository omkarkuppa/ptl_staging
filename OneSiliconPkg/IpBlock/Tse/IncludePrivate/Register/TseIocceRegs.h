/** @file
    Register names for TSE IOCCE

  Conventions:

  - Register definition format:
    Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  - Prefix:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register size
    Definitions beginning with "N_" are the bit position
  - [GenerationName]:
    Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -H denoted by "_PCH_H_" in component name.
    Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  - SubsystemName:
    This field indicates the subsystem name of the component that the register belongs to
    (e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  - RegisterSpace:
    MEM - MMIO space register of subsystem.
    IO  - IO space register of subsystem.
    PCR - Private configuration register of subsystem.
    CFG - PCI configuration space register of subsystem.
  - RegisterName:
    Full register name.

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

    @par Specification Reference:
**/

#ifndef _TSE_IOCCE_REGS_H_
#define _TSE_IOCCE_REGS_H_

#define R_IOCCE_TSE_BASE                                      0x12400
#define R_IOCCE_TSE_LIMIT                                     R_IOCCE_TSE_BASE + 0x7FF

#define R_IOCCE_TSE_TABLE_OFFSET_MCHBAR                       0x28

#define V_TSE_TABLE_OFFSET_MASK                               0xFFFFFFFFFF

typedef union {
  struct {
    UINT64 rsvd_0 : 6; /** Undefined - auto filled rsvd_[05:00] */
    UINT64 rsvd_1 : 6; /** Undefined - auto filled rsvd_[12:06] */
    UINT64 TseOffset : 40;
    /*
      Pointer to TSE HPA, 4KB aligned.
    */
    UINT64 rsvd_2 : 11; /** Undefined - auto filled rsvd_[62:53] */
    UINT64 Lock : 1;
    /*
      Lock bit for TseOffset
    */
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} TSE_TABLE_OFFSET_MCHBAR_STRUCT;

#define B_IOCCE_TSE_TABLE_INDEX_MASK_MCHBAR                   0x30

typedef union {
  struct {
    UINT64 rsvd_0 : 40; /** Undefined - auto filled rsvd_[39:00] */
    UINT64 Mask : 24;
    /*
      Mask. Default 0xFFF000
    */
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} TSE_TABLE_INDEX_MASK_STRUCT;

#define B_IOCCE_TSE_CACHE_INVALIDATION_MCHBAR                 0x38

typedef union {
  struct {
    UINT32 InvIndex : 12;
    /*
      Invalidation index with the invalidation mask field invalidate the corresponding cache's tags.
      If (TAG & InvIndexMask = InvIndex & InvIndexMask), then invalidate (TAG & InvIndexMask)
     */
    UINT32 InvIndexMask: 12;
    /*
      Invalidation index mask. Enable index group invalidation.
      Bit 0: 1 - invalidate InvIndex[0], 0 - Don't invalidate InvIndex[0].
      Bit 1: 1 - invalidate InvIndex[1], 0 - Don't invalidate InvIndex[1].
      ...
      Bit 11: 1 - invalidate InvIndex[11], 0 - Don't invalidate InvIndex[11].
     */
    UINT32 InvTrig: 1;
    /*
      Trigger cache invalidation based on InvIndex
     */
    UINT32 rsvd_0 : 6; /** Undefined - auto filled rsvd_[30:25] */
    UINT32 FlushAll: 1;
    /*
      Flush entire IOCCE cache
     */
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TSE_IOCCE_TSE_CACHE_INVALIDATION_STRUCT;

#define V_IOCCE_TSE_IOCCE_CAPABILITY_TYPE                       0x0
#define V_IOCCE_TSE_IOCCE_CAPABILITY_FLAGS_PCI_ALL              BIT0

#define V_IOCCE_TSE_IOCCE_CAPABILITY_KEY_SOURCE_CPU_ISA         BIT0
#define V_IOCCE_TSE_IOCCE_CAPABILITY_KEY_SOURCE_SECURITY_CTRL   BIT1
#define V_IOCCE_TSE_IOCCE_CAPABILITY_KEY_SOURCE_TSE_KEY_ENGINE  BIT2

#define V_IOCCE_TSE_IOCCE_CAPABILITY_MAX_BDF_SUPPORTED          0x8

#define V_IOCCE_TSE_IOCCE_CAPABILITY_PAGE_SIZE_SUPPORT_4K       BIT0
#define V_IOCCE_TSE_IOCCE_CAPABILITY_PAGE_SIZE_SUPPORT_2M       BIT1

#define V_IOCCE_TSE_IOCCE_CAPABILITY_LBA_GRANULARITY_512        BIT0
#define V_IOCCE_TSE_IOCCE_CAPABILITY_LBA_GRANULARITY_4K         BIT1

#define V_IOCCE_TSE_IOCCE_CAPABILITY_DATA_ALIGNMENT_16B         BIT0

#define B_IOCCE_TSE_IOCCE_CAPABILITY                          0x40

typedef union {
  struct {
    UINT64 IndexSupported : 16;
    /*
      TSE Engine Index bit supported
      BIT0: 1 index bit is supported
      BIT1: 2 index bits are supported
      ...
      BIT15: 16 index bits are supported
     */
    UINT64 LBA_Granularity: 8;
    /*
      Granularity of LBA supported
      BIT0: 512B
      BIT1: 4KB
      BIT2-7: RSVD
    */
    UINT64 PageSizeSupported: 8;
    /*
      TSE Engine Page Size Supported
      BIT0: 4KB
      BIT1: 2MB
      BIT2-7: RSVD
    */
    UINT64 TSE_MAX_BDF_Supported: 8;
    /*
      Maximum number of devices this engine can support.
      This is the maximum number of NVMe drives that can be configured in the TSE engine to enable encryption.
     */
    UINT64 Data_Transfer_Alignment: 8;
    /*
      Data Transfer Alignment
      BIT0: 4B alignment
      BIT1: 8B alignment
      BIT2: 16B alignment
      BIT3-BIT7: Reserved
     */
    UINT64 Data_Transfer_Granularity: 8;
    /*
      Data Transfer Granularity
      BIT0: 16B Transfer Granularity
      BIT1: 32B Transfer Granularity
      BIT2: 64B Transfer Granularity
      BIT3-BIT7: Reserved
     */
    UINT32 rsvd_0 : 8; /** Undefined - auto filled rsvd_[56:63] */
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} TSE_IOCCE_CAPABILITY_MCHBAR_STRUCT;

#define B_IOCCE_TSE_SELF_TEST_STS         0x104

typedef union {
  struct {
    UINT32 Fuse_SelfTest_EN : 1;
    /*
      Self-test fuse enable status. Reflect TSE_FIPS_FUSE.FIPS_MODE
     */
    UINT32 rsvd_0 : 3; /** Undefined - auto filled rsvd_[3:1] */
    UINT32 XTS_AES_SELFTEST_PASS : 1;
    /*
      Indicates that the IOCCE module passed the XTS-AES Power on self test for FIPS 140-3 L1 compliance.
      Updated only when TSE_FIPS_SELF_TEST_STS. Fuse_SelfTest_EN=1 Sticky bit until next reset
     */
    UINT32 XTS_AES_SELFTEST_FAIL : 1;
    /*
      Indicates that the IOCCE module failed the XTS-AES Power on self test for FIPS 140-3 L1 compliance.
      Updated only when TSE_FIPS_SELF_TEST_STS. Fuse_SelfTest_EN=1 Sticky bit until next reset
     */
    UINT32 rsvd_1 : 26; /** Undefined - auto filled rsvd_[6:31] */
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TSE_IOCCE_TSE_FIPS_SELF_TEST_STS_MCHBAR_STRUCT;

#endif // _TSE_IOCCE_REGS_H_
