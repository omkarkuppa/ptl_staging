/** @file
  Register names for PCH DCI device

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

#ifndef _DCIREGSB_H_
#define _DCIREGSB_H_

//
// DCI PCR Registers
//

#define R_DCI_PCR_ECTRL                       0x00000004U      ///< ECTRL REG
                                                               /* ExI Control Register
                                                                */

#define B_DCI_PCR_ECTRL_HOST_EXI_EN_LOCK      BIT0             ///< HOST EXI_EN_LOCK
                                                               /* Host EXI Enable Lock
                                                                */
#define B_DCI_PCR_ECTRL_HEEN                  BIT4             ///< HEEN Field
                                                               /* Host-EXIenable(HEEN) whensetto1
                                                                * allow BSSB-EXI
                                                                */
#define B_DCI_PCR_ECTRL_USB2DBCEN             BIT5             ///< USB2DBCEN Field
                                                               /* USB2 DBC enable indication
                                                                */
#define B_DCI_PCR_ECTRL_USB3DBCEN             BIT6             ///< USB3DBCEN Field
                                                               /* USB3 DBC enable indication
                                                                */
#define B_DCI_PCR_ECTRL_DBGENABLE             BIT8             ///< DBGENABLE Field
                                                               /* Debug Enable indication, previously
                                                                * eAvail
                                                                */
#define B_DCI_PCR_ECTRL_EGRANT                BIT9             ///< EGRANT Field
                                                               /* Bit indicating that the EXI bridge
                                                                * is currently owned, i.e., there
                                                                * is logical connection already established
                                                                */
#define B_DCI_PCR_ECTRL_KET                   BIT17            ///< Keep Early Trace
#define B_DCI_PCR_ECTRL_TOOLEN                BIT18            ///< DCI Tool in use

#define R_DCI_PCR_ECKPWRCTL                   0x00000008U      ///< ECKPWRCTL REG
                                                               /* ExI Clock and Power Control Register
                                                                */
#define B_DCI_PCR_ECKPWRCTL_RO_FAST_CLKGATE_DIS BIT18          ///< RO_FAST_CLKGATE_DIS Field
                                                               /* RO Fast Clock Gate Disable
                                                                */
#define B_DCI_PCR_ECKPWRCTL_RO_SLOW_CLKGATE_DIS BIT19          ///< RO_SLOW_CLKGATE_DIS Field
                                                               /* RO Slow Clock Gate Disable
                                                                */
#define R_DCI_PCR_EARBCTRL                    0x0000000cU      ///< EARBCTRL REG
                                                               /* EXI Arbitration control Register
                                                                */
#define B_DCI_PCR_EARBCTRL_ARB_GNT_BSSB       BIT16            ///< ARB_GNT_BSSB Field
                                                               /* Arb grant status for BSSB
                                                                */
#define B_DCI_PCR_EARBCTRL_ARB_GNT_DBC_SOUTH  BIT17            ///< ARB_GNT_DBC_SOUTH Field
                                                               /* Arb grant status for DBC South Hosting
                                                                */
#define R_DCI_PCR_EXIPORTID                   0x0000300cU      ///< EXIPORTID REG
                                                               /* ExI  PORTID Register
                                                                */
#define B_DCI_PCR_EXIPORTID_PORTID            ( BIT4 | BIT3 | BIT2 | BIT1 | BIT0 ) ///< PORTID Field
                                                               /* Hosting Type Port Number Connected
                                                                */
#define V_DCI_PCR_PORT_ID_MISC                15               ///< DCI Port ID number 0~15 could be 4-wire DCI OOB, USB3, USB2
#define V_DCI_PCR_PORT_ID_DCI_OOB_2_WIRE      23               ///< DCI Port ID number 16~23 is 2-wire DCI OOB
#define R_DCI_PCR_ESRCCLK                     0x000030c8U      ///< ESRCCLK REG
                                                               /* ExI SRCCLK Register
                                                                */
#define B_DCI_PCR_ESRCCLK_SRCCLK_RO_FAST_CLK_OVR_S0I3P4 BIT0   ///< SRCCLK RO_FAST_CLK_OVR_S0i3P4
                                                               /* RO FAST CLK Override Disable when
                                                                * system goes to S0i3.4 (Disaabled
                                                                * if 1)
                                                                */
#define B_DCI_PCR_ESRCCLK_SRCCLK_RO_FAST_CLK_OVR BIT1          ///< SRCCLK RO_FAST_CLK_OVR
                                                               /* RO FAST CLK Override Disable if
                                                                * 1
                                                                */
#define R_DCI_DBCDOTDBC_BUSNUM                0x0500           ///< DBC.DBC Bus Number Setup
#define B_DCI_DBCDOTDBC_BUSNUM_REQID          ( BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0 )
                                                               /* DBC.DBC Bus Number.
                                                                * Bits[31:8] remain reserved.
                                                                */
#define R_DCI_PCR_EMB0                        0x0030U          ///< EMB0 REG
                                                               /* ExI MAILBOX 0 Register
                                                                * Register default value: 0x00000000
                                                                */

#endif
