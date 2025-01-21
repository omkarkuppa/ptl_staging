/** @file
  Register names for TraceHub device

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
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _TRACEHUBREG_H_
#define _TRACEHUBREG_H_


#define R_TRACE_HUB_CFG_MTB_LBAR              0x00000010U      ///< MSC Trace Buffer Lower BAR
                                                               /* MSC Trace Buffer Lower BAR
                                                                */
#define B_TRACE_HUB_CFG_MTB_LBAR_ADDR         0xfff00000U
                                                               /* CSR and MTB BAR (Lower). This register
                                                                * specifies the lower 32 bits of the
                                                                * configurable base address for CSRs
                                                                * (Configuration and Status Registers)
                                                                * and MTB (Memory Trace Buffer). This
                                                                * BAR is called BAR 0.
                                                                */
#define R_TRACE_HUB_CFG_MTB_UBAR              0x00000014U      ///< MSC Trace Buffer Upper BAR
                                                               /* MSC Trace Buffer Upper BAR
                                                                */
#define B_TRACE_HUB_CFG_MTB_UBAR_ADDR         0xffffffffU
                                                               /* CSR and MTB BAR (Upper). This register
                                                                * specifies the upper 32 bits of the
                                                                * configurable base address for CSRs
                                                                * (Configuration and Status Registers)
                                                                * and MTB (Memory Trace Buffer). This
                                                                * BAR is called BAR 0.
                                                                */
#define R_TRACE_HUB_CFG_SW_LBAR               0x00000018U      ///< Software Lower BAR
                                                               /* Software Lower BAR
                                                                */
#define B_TRACE_HUB_CFG_SW_LBAR_ADDR          0xff800000U
                                                               /* Software Trace BAR (Lower). This
                                                                * register specifies the lower 32
                                                                * bits of the configurable base address
                                                                * for Software trace data . This BAR
                                                                * is called BAR 1.
                                                                */
#define R_TRACE_HUB_CFG_SW_UBAR               0x0000001cU      ///< Software Upper BAR
                                                               /* Software Upper BAR
                                                                */
#define B_TRACE_HUB_CFG_SW_UBAR_ADDR          0xffffffffU
                                                               /* Software Trace BAR (Upper). This
                                                                * register specifies the upper 32
                                                                * bits of the configurable base address
                                                                * for Software trace data . This BAR
                                                                * is called BAR 1.
                                                                */
#define R_TRACE_HUB_CFG_RTIT_LBAR             0x00000020U      ///< RTIT Lower BAR
                                                               /* RTIT Lower BAR
                                                                */
#define B_TRACE_HUB_CFG_RTIT_LBAR_ADDR        0xffff8000U
                                                               /* RTIT Trace BAR (Lower). This register
                                                                * specifies the lower 32 bits of the
                                                                * configurable base address for RTIT
                                                                * trace data . This BAR is called
                                                                * BAR 2.
                                                                */
#define R_TRACE_HUB_CFG_RTIT_UBAR             0x00000024U      ///< RTIT Upper BAR
                                                               /* RTIT Upper BAR
                                                                */
#define B_TRACE_HUB_CFG_RTIT_UBAR_ADDR        0xffffffffU
                                                               /* RTIT Trace BAR (Upper). This register
                                                                * specifies the upper 32 bits of the
                                                                * configurable base address for RTIT
                                                                * trace data . This BAR is called
                                                                * BAR 2.
                                                                */
#define R_TRACE_HUB_CFG_FW_LBAR               0x00000070U      ///< Firmware Lower Bar
                                                               /* Firmware Lower Bar
                                                                */
#define B_TRACE_HUB_CFG_FW_LBAR_ADDR          0xffe00000U
                                                               /* Firmware Trace BAR (Lower). This
                                                                * register specifies the lower 32
                                                                * bits of the configurable base address
                                                                * for Firmware trace data . This BAR
                                                                * is called BAR 3, though it is a
                                                                * non-standard BAR (not a BAR defined
                                                                * by the PCI Specification).
                                                                */
#define R_TRACE_HUB_CFG_FW_UBAR               0x00000074U      ///< Firmware Upper Bar
                                                               /* Firmware Upper Bar
                                                                */
#define B_TRACE_HUB_CFG_FW_UBAR_UADDR         0xffffffffU
                                                               /* Firmware Trace BAR (Upper). This
                                                                * register specifies the upper 32
                                                                * bits of the configurable base address
                                                                * for Firmware trace data . This BAR
                                                                * is called BAR 3, though it is a
                                                                * non-standard BAR (not a BAR defined
                                                                * by the PCI Specification).
                                                                */
#define R_TRACE_HUB_CFG_NPKDSC                0x00000080U      ///< NPK Device Specific Control
                                                               /* NPK Device Specific Control
                                                                */
#define B_TRACE_HUB_CFG_NPKDSC_FLR            BIT1
                                                               /* Software Reset. IMPORTANT: Despite
                                                                * the name, this is **not** FLR (Function
                                                                * Level Reset) as defined by the PCI
                                                                * Express Specification.(br)This is
                                                                * North Peak's software controlled
                                                                * functional reset (see [Internal
                                                                * Reset Generation] for more details).
                                                                * Writing a 1  to this bit will assert
                                                                * the reset signal.  Reading this
                                                                * bit will always return a zero.
                                                                */
#define B_TRACE_HUB_CFG_NPKDSC_TRACT          BIT4
                                                               /* Tracing Active: Setting this bit
                                                                * will force North Peak to request
                                                                * its North Peak clock (NPCLK) independent
                                                                * of any activity and disable clock
                                                                * gating
                                                                */
#define B_TRACE_HUB_CFG_NPKDSC_TSACT          BIT5
                                                               /* Time Stamping Active: Setting this
                                                                * bit will force North Peak to maintain
                                                                * requesting the XCLK and disable
                                                                * internal clock gating
                                                                */


//
// Offsets from SCR_MTB_BAR
//
#define R_TRACE_HUB_MEM_MTB_80000             0x00080000U
#define R_TRACE_HUB_MEM_SCR                   0x000000c8U      ///< Source Control Register
                                                               /* Source Control Register
                                                                */
#define B_TRACE_HUB_MEM_SCR_STOREENOVRD4      BIT20
                                                               /* Storage Enable Override 4 (STH).
                                                                * See StoreEnOvrd2 for details.
                                                                */
#define B_TRACE_HUB_MEM_SCR_STOREENOVRD5      BIT21
                                                               /* Storage Enable Override 5 (NDB;
                                                                * DTF Input). See StoreEnOvrd2 for
                                                                * details.
                                                                */
#define R_TRACE_HUB_MEM_TSCU_FREQ             0x000000ccU      ///< TSCU Frequency Register
                                                               /* TSCU Frequency Register
                                                                */

#define R_TRACE_HUB_MEM_GTHSTAT               0x000000d4U      ///< GTH Status Register
                                                               /* GTH Status Register
                                                                */
#define B_TRACE_HUB_MEM_GTHSTAT_IBE4          BIT12
                                                               /* Input Buffer Empty, Trace Source
                                                                * 4 (STH). See IBE0 for details.
                                                                */
#define B_TRACE_HUB_MEM_GTHSTAT_IBE5          BIT13
                                                               /* Input Buffer Empty, Trace Source
                                                                * 5 (DTF Input). See IBE0 for details.
                                                                */

#define R_TRACE_HUB_MEM_SCRPD0                0x000000e0U      ///< ScratchPad[0] Register
                                                               /* ScratchPad[0] Register
                                                                */
#define B_TRACE_HUB_MEM_SCRPD0_DEBUGGERINUSE  BIT24
                                                               /* Debugger In Use. Used during initialization
                                                                * flows to indicate to BIOS/IA firmware
                                                                * that an external host debugger is
                                                                * actively using this North Peak instance
                                                                */
#define B_TRACE_HUB_MEM_SCRPD0_MEMISPRIMDEST  BIT0
                                                               /* Used during save/restore flows to
                                                                * indicate to PMU that North Peak
                                                                * s primary tracing destination is
                                                                * system memory
                                                                */
#define R_TRACE_HUB_MEM_SCRPD1                0x000000e4U      ///< ScratchPad[1] Register
                                                               /* ScratchPad[1] Register
                                                                */
#define R_TRACE_HUB_MEM_SCRPD2                0x000000e8U      ///< ScratchPad[2] Register
                                                               /* ScratchPad[2] Register
                                                                */
#define B_TRACE_HUB_MEM_SCRPD2_RS3_BUSNUM     ( BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8 )
                                                               /* Root Space 3 Bus Number. Specifies
                                                                * the PCI Bus Number assigned to North
                                                                * Peak for root space 3 (RS3) transactions.
                                                                */
#define B_TRACE_HUB_MEM_SCRPD2_IAFW_CTRL      ( BIT3 | BIT2 | BIT1 | BIT0 )
                                                               /* IA Firmware (BIOS) tracing verbosity
                                                                * and enabling. These bits are used
                                                                * to communicate to IA Firmware (BIOS)
                                                                * the desired verbosity level, and
                                                                * whether or not tracing to North
                                                                * Peak, is enabled.  Refer to the
                                                                * BIOS Writer's Guide for details
                                                                * on bit encodings.
                                                                */
#define N_TRACE_HUB_MEM_SCRPD2_IAFW_CTRL      0

#define B_TRACE_HUB_MEM_SCRPD2_BIOS_TRACE_EN  BIT0

#define N_TRACE_HUB_MEM_SCRPD2_TRACE_VERBO    1
#define V_TRACE_HUB_MEM_SCRPD2_TRACE_ERR      0                // Errors only
#define V_TRACE_HUB_MEM_SCRPD2_TRACE_WARN     1                // Errors and Warnings only
#define V_TRACE_HUB_MEM_SCRPD2_TRACE_INFO     2                // Errors, Warnings and Info only
#define V_TRACE_HUB_MEM_SCRPD2_TRACE_ALL      3                // Errors, Warnings, Info and Verbose (all debug messages)

#define R_TRACE_HUB_MEM_NDB_CTRL              0x000000f0U      ///< NPK DTF Control Register
                                                               /* NPK DTF Control Register
                                                                */
#define B_TRACE_HUB_MEM_NDB_CTRL_NDB_WAITEMPTY 0x0000ffffU
                                                               /* NDB Wait for Empty. Specifies the
                                                                * number of npclk periods the NDB
                                                                * will wait after a storeEn deassertion
                                                                * before it considers the DTF to be
                                                                * empty, and forcibly deassert its
                                                                * valid and sourceActive outputs.
                                                                */
#define V_TRACE_HUB_MEM_STHCAP0_STHMSTR       0x00000100U
#define V_TRACE_HUB_MEM_STHCAP1_FW_MSTR       0x00000010U
#define V_TRACE_HUB_MEM_STHCAP1_CHLCNT        0x00000080U

#define R_TRACE_HUB_MEM_TSCU_FREQ_HI          0x00000168U      ///< TSCU Frequency Register High
                                                               /* TSCU Frequency Register High
                                                                */

#define R_TRACE_HUB_MEM_LPP_CTL               0x00001c00U      ///< LPP Control Register
                                                               /* LPP Control Register
                                                                */
#define B_TRACE_HUB_MEM_LPP_CTL_LPMEN         BIT24
                                                               /* Low Power Mode Enable:
                                                                * Setting this bit will start the
                                                                * sequence to switch to low power
                                                                * mode (kicks off the LPP Switch FSM).
                                                                * = 0 | Full power mode / S0/ VNN
                                                                * on,
                                                                * = 1 | Low power mode
                                                                */

#define R_TRACE_HUB_MEM_TSCUSTAT              0x00002004U      ///< TSCU Status Register
                                                               /* TSCU Status Register
                                                                */

#define R_TRACE_HUB_MEM_CTS_STATUS            0x000032a0U      ///< CTS Status Register
                                                               /* CTS Status Register
                                                                */
#define R_TRACE_HUB_MEM_CTS_CONTROL           0x000032a4U      ///< CTS Control Register
                                                               /* CTS Control Register
                                                                */

#define R_TRACE_HUB_MEM_SDC                   0x00005000U      ///< SoCHAP Device Capabilities
                                                               /* SoCHAP Device Capabilities
                                                                */
/**
 Description of CE(CHAP Enable) (15:15)
**/
#define B_TRACE_HUB_MEM_SDC_CE                BIT15
                                                               /* CHAP Enable: This register enables
                                                                * the SoCHAP block. When this bit
                                                                * is cleared, the SoCHAP block is
                                                                * disabled except for this register.
                                                                */
#define R_TRACE_HUB_MEM_MSUGCTL               0x000a0018U      ///< MSU Global Control Register
                                                               /* MSU Global Control Register
                                                                */
#define B_TRACE_HUB_MEM_MSUGCTL_MSURS         ( BIT1 | BIT0 )
                                                               /* MSU ROOT SPACE: Specifies the root
                                                                * space to which NPK's MSU will be
                                                                * assigned.(br)
                                                                * = 00 | root space 0
                                                                * = 11 | root space 3
                                                                * = others | Reserved. If a reserved
                                                                * value is written, the MSU shall
                                                                * not issue any MemRd or MemWr transactions.
                                                                */
#define R_TRACE_HUB_MEM_MSC0BAR               0x000a0108U      ///< MSC0 Base Address Register
                                                               /* MSC0 Base Address Register
                                                                */
#define R_TRACE_HUB_MEM_MSC0DESTSZ            0x000a010cU      ///< MSC0 Memory Buffer Size
                                                               /* MSC0 Memory Buffer Size
                                                                */
#define R_TRACE_HUB_MEM_MSC0UBAR              0x000a0134U      ///< MSC0 Upper Base Address Register
                                                               /* MSC0 Upper Base Address Register
                                                                */
#define R_TRACE_HUB_MEM_MSC1BAR               0x000a0208U      ///< MSC1 Base Address Register
                                                               /* MSC1 Base Address Register
                                                                */
#define R_TRACE_HUB_MEM_MSC1DESTSZ            0x000a020cU      ///< MSC1 Memory Buffer Size
                                                               /* MSC1 Memory Buffer Size
                                                                */
#define R_TRACE_HUB_MEM_MSC1UBAR              0x000a0234U      ///< MSC1 Upper Base Address Register
                                                               /* MSC1 Upper Base Address Register
                                                                */
#define R_TRACE_HUB_MEM_STREAMCFG2            0x000a1004U      ///< Streaming Configuration 2
                                                               /* Streaming Configuration 2
                                                                */
#define V_TRACE_HUB_MEM_STREAMCFG2_DBCOFFSET  0x0000005CU
#define N_TRACE_HUB_MEM_STREAMCFG2_DBCOFFSET  12
#define R_TRACE_HUB_MEM_DBCSTSCMD             0x000a1008U      ///< DBC.Trace Status Command
                                                               /* DBC.Trace Status Command
                                                                */
#define R_TRACE_HUB_MEM_DBC_BASE_ADDR_LO      0x000a1028U      ///< DBC Base Address Lo register
                                                               /* DBC Base Address Lo register
                                                                */
#define V_PCH_TRACE_HUB_MEM_MTB_DBC_BASE_LO   0xFED60000

#endif
