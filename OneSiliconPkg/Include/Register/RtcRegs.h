/** @file
  Register names for RTC device

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
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _RTCREGS_H_
#define _RTCREGS_H_

#define R_RTC_IO_INDEX                        0x00000070U      ///< RTC Index Register
                                                               /// Register default value: 0x00000000
                                                               /* This 8-bit register selects which indirect
                                                                * register appears in the target register
                                                                * to be manipulated by software. Software
                                                                * will program this register to select the
                                                                * desired RTC indexed register.
                                                                */
#define R_RTC_IO_TARGET                       0x00000071U      ///< RTC Target Register
                                                               /// Register default value: 0x00000000
                                                               /* This 32-bit register specifies the data
                                                                * to be read or written to the register pointed
                                                                * to by the INDEX register.
                                                                */
#define R_RTC_IO_INDEX_ALT                    0x00000074U      ///< RTC Alternative Index Register
#define R_RTC_IO_TARGET_ALT                   0x00000075U      ///< RTC Alternative Target Register
#define R_RTC_IO_EXT_INDEX_ALT                0x00000076U      ///< RTC External Alternative Index Register
#define R_RTC_IO_EXT_TARGET_ALT               0x00000077U      ///< RTC External Alternative Target Register
#define R_RTC_IO_REGISTER_A                   0x0000000aU      ///< Register A
                                                               /// Register default value: 0x00000070
                                                               /* RTC Index: 0Ah
                                                                * Attribute: Read/Write
                                                                * Default Value: 0UUUUUUU
                                                                * Size: 8-bit
                                                                * Lockable: No Power
                                                                * Well: RTC
                                                                * This register is used for general configuration
                                                                * of the RTC functions. None of the bits
                                                                * are affected by RSMRST# or any other reset
                                                                * signal.
                                                                */
#define R_RTC_IO_REGISTER_B                   0x0000000bU      ///< Register B General Configuration
                                                               /// Register default value: 0x00000080
                                                               /* RTC Index: 0Bh
                                                                * Attribute: Read/Write
                                                                * Default Value: 10000UUU
                                                                * Size: 8-bit
                                                                * Lockable: No
                                                                * Power Well: RTC
                                                                */
#define B_RTC_IO_REGISTER_B_RTC_SET           BIT7             ///< Update Cycle Inhibit
                                                               /* Enables/Inhibits the update cycles.
                                                                * When SET is 0, update cycle occurs
                                                                * normally once each second. If set
                                                                * to one, a current update cycle will
                                                                * abort and subsequent update cycles
                                                                * will not occur until SET is returned
                                                                * to zero.
                                                                * When set is one, the BIOS may initialize
                                                                * time and calendar bytes safely.
                                                                * This bit is not affected by RSMRST#
                                                                * nor any other reset signal.
                                                                * Note: Software must ensure this
                                                                * bit is at least transitioned from
                                                                * '1' to '0' once
                                                                * whenever the RTC coin battery is
                                                                * inserted. This is to ensure that
                                                                * the internal RTC time updates occur
                                                                * properly.
                                                                */
#define R_RTC_IO_REGISTER_C                   0x0000000cU      ///< Register C Flag Register
                                                               /// Register default value: 0x00000000
                                                               /* RTC Index: 0Ch
                                                                * Attribute: Read-Only (Writes have no effect).
                                                                * Default Value: 00000000
                                                                * Size: 8-bit
                                                                * Lockable: No
                                                                * Power Well: RTC
                                                                */
#define R_RTC_IO_REGISTER_D                   0x0000000dU      ///< Register D Flag Register
                                                               /// Register default value: 0x00000080
                                                               /* RTC Index: 0Dh
                                                                * Attribute: Read/Write
                                                                * Default Value: 10UUUUUU
                                                                * Size: 8-bit
                                                                * Lockable: No Power
                                                                * Well: RTC
                                                                */
#define R_RTC_IO_REGISTER_E                   0x0000000eU      ///< 114 Bytes Of Lower User RAM
                                                               /// Register default value: 0x00000000
                                                               /* Remaining 114 Bytes of Lower User RAM. Each
                                                                * byte in this bank share the same description
                                                                * as shown below. RAM default values are undetermined
                                                                * and the last written value will be retained
                                                                * until RTC power is removed.
                                                                */

//
// Private Configuration Register
// RTC PCRs (PID:RTC)
//
#define R_RTC_PCR_RC                          0x00003400U      ///< RTC Configuration
                                                               /// Register default value: 0x00000000
                                                               /* All bits in this register are in the Primary
                                                                * Well and cleared by host_side_rst_b.
                                                                */
#define B_RTC_PCR_RC_UE                       BIT2             ///< Upper 128 Byte Enable
                                                               /* When set, the upper 128 byte bank
                                                                * of RTC RAM can be accessed.
                                                                */
#define B_RTC_PCR_RC_LL                       BIT3             ///< Partial Range Lock in Lower 128 Bytes
                                                               /* When set, bytes 38h-3Fh in the lower
                                                                * 128 byte bank of RTC RAM are locked
                                                                * and cannot be accessed. Writes will
                                                                * be dropped and reads will not return
                                                                * any guaranteed data. Bit reset on
                                                                * system reset.
                                                                */
#define B_RTC_PCR_RC_UL                       BIT4             ///< Partial Range Lock in Upper 128 Bytes
                                                               /* When set, bytes 38h-3Fh in the upper
                                                                * 128 byte bank of RTC RAM are locked
                                                                * and cannot be accessed. Writes will
                                                                * be dropped and reads will not return
                                                                * any guaranteed data. Bit reset on
                                                                * system reset.
                                                                */
#define B_RTC_PCR_RC_HPM_HW_DIS               BIT6             ///< RTC High Power Mode HW Disable
                                                               /* When set to 1 the internal VRM that
                                                                * generates the rtc well supply voltage
                                                                * in SUS mode is disabled when SLP_S0#
                                                                * is asserted to '0'. (via irtcdswen
                                                                * pin to RTC EBB). When 0, HW control
                                                                * of the RTC internal VRM is disabled.
                                                                */
#define B_RTC_PCR_RC_BILD                     BIT31            ///< Bios Interface Lock-Down
                                                               /* When set, prevents RTC version of
                                                                * TS (BUC.TS) from being changed.
                                                                * This bit can only be written from
                                                                * 0 to 1 once. This BILD bit has different
                                                                * function compared to LPC, SPI and
                                                                * eSPI version but BIOS should set
                                                                * all the corresponding bits after
                                                                * reset in order to lock down the
                                                                * BIOS interface correctly.
                                                                */
#define R_RTC_PCR_BUC                         0x00003414U      ///< Backed Up Control
                                                               /// Register default value: 0x00000000
                                                               /* All bits in this register are in the RTC
                                                                * well and only cleared by RTEST.
                                                                */
#define B_RTC_PCR_BUC_TS                      BIT0             ///< Top Swap
                                                               /* This should be set by BIOS when
                                                                * the corresponding TS bit in the
                                                                * LPC or eSPI controller is set in
                                                                * order to properly restore the state
                                                                * of that field after reset since
                                                                * they are not preserved in an RTC
                                                                * well bit in those devices.
                                                                * *If PCH is strapped for Top-Swap
                                                                * (GNT(3)# is low at rising edge of
                                                                * PWROK), then this bit cannot be
                                                                * cleared by software. The strap jumper
                                                                * should be removed and the system
                                                                * rebooted.
                                                                */
#define B_RTC_PCR_BUC_SDO                     BIT4             ///< Daylight Savings Override
                                                               /* When this bit is a '1', the DSE
                                                                * bit in the RTC Register B bit(0)
                                                                * is a RW bit but has no effect where
                                                                * daylight savings is hard-disabled
                                                                * internally. When this bit is a '0',
                                                                * the DSE bit in the RTC register
                                                                * B bit(0) is a RW bit that is configurable
                                                                * by software to enable the daylight
                                                                * savings.
                                                                * System BIOS shall configure this
                                                                * bit accordingly during the boot
                                                                * process before RTC time is initialized.
                                                                */
#define R_RTC_PCR_RTCDCG                      0x00003418U      ///< RTC Dynamic Clock Gating Control
                                                               /// Register default value: 0x00000000
                                                               /* All bits in this register are in the Primary
                                                                * Well and cleared by host_side_rst_b.
                                                                */
#define B_RTC_PCR_RTCDCG_RTCROSIDEDCGEN       BIT0             ///< rosc_side_clk (120MHz) Dynamic Clock Gate Enable
                                                               /* 0 - Disable dynamic clock gate on
                                                                * rosc_side_clk (Default) 1 - Enable
                                                                * dynamic clock gate on rosc_side_clk
                                                                */
#define B_RTC_PCR_RTCDCG_RTCPCICLKDCGEN       BIT1             ///< ipciclk_clk (24MHz) Dynamic Clock Gate Enable
                                                               /* 0 - Disable dynamic clock gate on
                                                                * ipci_clk (Default) 1 - Enable dynamic
                                                                * clock gate on ipci_clk
                                                                */
#define B_RTC_PCR_RTCDCG_RTCPGCBDCGEN         BIT2             ///< pgcb_clk (12MHz) Dynamic Clock Gate Enable
                                                               /* 0 - Disable dynamic clock gate on
                                                                * pgcb_clk (Default) 1 - Enable dynamic
                                                                * clock gate on pgcb_clk
                                                                */
#define B_RTC_PCR_RTCDCG_RTCAME               BIT3             ///< Alternate Access Mode Enable (AME)
                                                               /* 0 - RTC Port 70h bit [7:0] returns
                                                                * value 0 1 - The regular IO Read
                                                                * to Port 70h always return the shadowed
                                                                * NMI_EN# in RTC
                                                                */
#define R_RTC_PCR_RTCTM                       0x00003f00U      ///< RTC Test Modes
                                                               /// Register default value: 0x001f000f
                                                               /* This register exists in the RTC well. The
                                                                * description of this register should not
                                                                * be put in EDS
                                                                */
#define R_RTC_PCR_SAIPG1_CP_LO                0x00003428U      ///< SAI Policy Group 1 Control Policy
                                                               /// Register default value: 0x00000000
                                                               /* SAI Policy Group 1 Control Policy bit[31:0]
                                                                */

#endif      // _RTCREGS_H_
