/** @file
    Register names for TSE IOC

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

#ifndef _TSE_IOC_REGS_H_
#define _TSE_IOC_REGS_H_

#define R_IOC_TSE_BASE                                        0x73D0
#define R_IOC_TSE_LIMIT                                       R_IOC_TSE_BASE + 0xFA

#define R_IOC_TSE_FUSE_BASE                                   0x73D0

typedef union {
  struct {
    UINT32 En : 1;
    /*
      0 - TSE Disabled
      1 - TSE Enabled
     */
    UINT32 rsvd_0 : 31; /** Undefined - auto filled rsvd_[31:01] */
  } Bits;
  UINT32 Data32;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TSE_IOC_TSE_FUSE_STRUCT;

#define R_IOC_TSE_BDF_EN_BASE                                 0x73D4

typedef union {
  struct {
    UINT32 Enable0 : 1;
    /*
      1 - TSE_BDF_CMP0.BDF0 is enabled
     */
    UINT32 Enable1 : 1;
    /*
      1 - TSE_BDF_CMP0.BDF1 is enabled
     */
    UINT32 Enable2 : 1;
    /*
      1 - TSE_BDF_CMP0.BDF2 is enabled
     */
    UINT32 Enable3 : 1;
    /*
      1 - TSE_BDF_CMP0.BDF3 is enabled
     */
    UINT32 Enable4 : 1;
    /*
      1 - TSE_BDF_CMP1.BDF4 is enabled
     */
    UINT32 Enable5 : 1;
    /*
      1 - TSE_BDF_CMP1.BDF5 is enabled
     */
    UINT32 Enable6 : 1;
    /*
      1 - TSE_BDF_CMP1.BDF6 is enabled
     */
    UINT32 Enable7 : 1;
    /*
      1 - TSE_BDF_CMP1.BDF7 is enabled
     */
    UINT32 rsvd_0 : 24; /** Undefined - auto filled rsvd_[31:08] */
  } Bits;
  UINT32 Data32;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TSE_IOC_TSE_BDF_EN_STRUCT;

#define R_IOC_TSE_MSI_ADDR_BASE                               0x73D8

typedef union {
  struct {
    UINT64 MsiAddr : 64;
    /*
      Used by system software to assign an MSI address to the TSE device.
      The TSE device handles an MSI by writing the padded contents of the TSE_MSI_DATA register to this address.
    */
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} TSE_IOC_TSE_MSI_ADDR_STRUCT;

#define R_IOC_TSE_BDF_CMP0_BASE                               0x73E0

typedef union {
  struct {
    UINT64 BDF0 : 16;
    /*
      TSE BDF qualifier
    */
    UINT64 BDF1 : 16;
    /*
      TSE BDF qualifier
    */
    UINT64 BDF2 : 16;
    /*
      TSE BDF qualifier
    */
    UINT64 BDF3 : 16;
    /*
      TSE BDF qualifier
    */
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} TSE_IOC_TSE_BDF_CMP0_STRUCT;

#define R_IOC_TSE_BDF_CMP1_BASE                               0x73E8

typedef union {
  struct {
    UINT64 BDF4 : 16;
    /*
      TSE BDF qualifier
    */
    UINT64 BDF5 : 16;
    /*
      TSE BDF qualifier
    */
    UINT64 BDF6 : 16;
    /*
      TSE BDF qualifier
    */
    UINT64 BDF7 : 16;
    /*
      TSE BDF qualifier
    */
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} TSE_IOC_TSE_BDF_CMP1_STRUCT;

#define R_IOC_TSE_MSI_DATA_BASE                               0x73F0

typedef union {
  struct {
    UINT32 MsiData : 16;
    /*
      Base message data pattern assigned by system software and used to handle an MSI from the TSE device.
      When the device must generate an interrupt request, it writes a 32-bit value to the memory address specified
      in the TSE_MSI_ADDR register.

      The upper 16 bits are always set to 0. The lower 16 bits are supplied by this register.
     */
    UINT32 rsvd_0 : 16; /** Undefined - auto filled rsvd_[31:16] */
  } Bits;
  UINT32 Data32;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TSE_IOC_TSE_MSI_DATA_STRUCT;

#define R_IOC_TSE_MSI_CTRL_BASE                               0x73F4

typedef union {
  struct {
    UINT32 MsiEn : 1;
    /*
      TSE MSI enable. 0 - MSI disabled. 1 - MSI Enabled.
     */
    UINT32 MsiPend : 1;
    /*
      MSI is pending. Set once TSE error occur. Deassert once MSI completion was accepted.
     */
    UINT32 rsvd_0 : 16; /** Undefined - auto filled rsvd_[31:2] */
  } Bits;
  UINT32 Data32;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TSE_IOC_TSE_MSI_CTRL_STRUCT;

#define R_IOC_TSE_ERR_LOG_BASE                               0x73F8

typedef union {
  struct {
    UINT32 BdfErr : 16;
    /*
      Log the BDF which caused the TSE Error. HW capture the BDF when MSI_pend = 0.
     */
    UINT32 rsvd_0 : 15; /** Undefined - auto filled rsvd_[30:16] */
    UINT32 ErrOccr : 1;
    /*
      Error occurred. Set by HW, cleared by SW.
    */
  } Bits;
  UINT32 Data32;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TSE_IOC_TSE_ERR_STRUCT;

#endif // _TSE_IOC_REGS_H_
