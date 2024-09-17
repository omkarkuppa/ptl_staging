/** @file
  Register defines for USB Host controller

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
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _USB_HOST_REGS_H_
#define _USB_HOST_REGS_H_

//
// XHCI PCI Config Space registers
//
#define R_XHCI_CFG_VID                      0x00    ///< Vendor ID
#define R_XHCI_CFG_CMD                      0x04    ///> Command Reg
#define B_XHCI_CFG_CMD_MSE                  BIT1    ///< Memory Space Enable
#define R_XHCI_CFG_MBAR_N0                  0x10    ///> MMIO registers base address
#define R_XHCI_CFG_MBAR_N1                  0x14    ///< Memory Base Address
#define R_XHCI_CFG_IPIN                     0x3D   ///< Interrupt Pin Register

#define R_XHCI_CFG_XHCC1                    0x40    ///< XHC System Bus Configuration 1
#define B_XHCI_CFG_XHCC1_ACCTRL             BIT31   ///< Access Control
#define B_XHCI_CFG_XHCC1_URD                BIT23   ///< Unsupported Request Detected
#define B_XHCI_CFG_XHCC1_URRE               BIT22   ///< Unsupported Request Report Enable
#define R_XHCI_CFG_XHCC2                    0x44    ///< XHC System Bus Configuration 2
#define B_XHCI_CFG_XHCC2_DREQBCC            BIT25   ///< I/O DMA Request Boundary Crossing Control
#define N_XHCI_CFG_XHCC2_DREQBCC            25      ///< I/O DMA Request Boundary Crossing Control
#define B_XHCI_CFG_XHCC2_WRREQSZCTRL        (BIT24 | BIT23 | BIT22)  ///< IDMA Write Request Size Control
#define N_XHCI_CFG_XHCC2_WRREQSZCTRL        22                       ///< IDMA Write Request Size Control bit position
#define B_XHCI_CFG_XHCC2_RAWDD              BIT11   ///< MMIO Read After MMIO Write Delay Disable
#define B_XHCI_CFG_XHCC2_RDREQSZCTRL        (BIT2 | BIT1 | BIT0)     ///< Read Request Size Control
#define B_XHCI_CFG_XHCC2_OCCFGDONE          BIT31            ///< OC Configuration Done
#define B_XHCI_CFG_XHCC2_OCCFGDONE          BIT31
#define R_XHCI_CFG_XHCLKGTEN                0x50    ///< Clock Gating
#define B_XHCI_CFG_XHCLKGTEN_SSTCGE         (BIT19 | BIT18 | BIT17 | BIT16)
#define N_XHCI_CFG_XHCLKGTEN_SSTCGE         16
#define R_XHCI_CFG_PM_CS                    0x74
#define B_XHCI_CFG_PM_CS_POWERSTATE         (BIT1 | BIT0)
#define V_XHCI_CFG_PM_CS_POWERSTATE_D3      (BIT1 | BIT0)
#define R_XHCI_CFG_PCE_REG                  0xA2
#define B_XHCI_CFG_PCE_REG_D3_HOT_EN        BIT2
#define R_XHCI_CFG_HSCFG2                   0xA4    ///< High Speed Configuration 2
#define R_XHCI_CFG_SSCFG1                   0xA8    ///< Super Speed Configuration 1
#define R_XHCI_CFG_HSCFG1                   0xAC    ///< High Speed Configuration 1
#define B_XHCI_CFG_HSCFG1_HSAAPE            BIT9
#define R_XHCI_CFG_XHCC3                    0xFC    ///< XHC System Bus Configuration 3
#define B_XHCI_CFG_XHCC3_FXN_DISABLE        BIT0    ///< Function Disable

//
// xHCI MMIO registers
//

//
// 0x00 - 0x1F - Capability Registers
//
#define R_XHCI_MEM_CAPLENGTH                0x00    ///< Capability Registers Length
#define B_XHCI_MEM_HCIVERSION_HCIVERSION    0xFFFF
#define V_XHCI_MEM_HCIVERSION_0120          0x0120
#define R_XHCI_MEM_HCSPARAMS3               0x0C    ///< Structural Parameters 3
#define B_XHCI_MEM_HCSPARAMS3_U2DEL         0xFFFF0000
#define N_XHCI_MEM_HCSPARAMS3_U2DEL         16
#define R_XHCI_MEM_HCCPARAMS2               0x1C    ///< Capability Parameters
#define B_XHCI_MEM_HCCPARAMS2_GSPC          BIT8    ///< Get/Set Extended Property Capability

//
// 0x80 - 0xBF - Operational Registers
//
#define R_XHCI_MEM_PORTSC1      0x480  ///< Port Status and Control Registers base offset
#define S_XHCI_MEM_PORTSC_PORT_SPACING      0x10   ///< Size of space between PortSC register for each port

#define B_XHCI_MEM_PORTSC1_PED              BIT1   ///< Port Enable/Disabled
#define B_XHCI_MEM_PORTSC1_CCS              BIT0   ///< Current Connect Status
//
// 0x2000 - 0x21FF - Runtime Registers
// 0x3000 - 0x307F - Doorbell Registers
//
#define R_XHCI_MEM_XECP_SUPP_USB2_2           0x8008  ///< XECP SUPP USB2_2
#define B_XHCI_MEM_XECP_SUPP_USB2_2_CPC       (BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8) ///< Compatible Port Count
#define N_XHCI_MEM_XECP_SUPP_USB2_2_CPC       8       ///< Shift for Compatible Port Count
#define R_XHCI_MEM_XECP_SUPP_USB3_2           0x8028  ///< XECP SUPP USB3_2
#define B_XHCI_MEM_XECP_SUPP_USB3_2_CPC       (BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8) ///< Compatible Port Count
#define N_XHCI_MEM_XECP_SUPP_USB3_2_CPC       8
#define R_XHCI_MEM_HOST_CTRL_SCH_REG          0x8094  ///< Host Control Scheduler
#define R_XHCI_MEM_HOST_CTRL_ODMA_REG         0x8098  ///< Host Control ODMA
#define R_XHCI_MEM_HOST_CTRL_PORT_CTRL        0x80A0  ///< Global Port Control
#define B_XHCI_MEM_HOST_CTRL_PORT_CTRL_HBUF_WATER_MARK 0x7ffff000  ///< HBUF_WATER_MARK
#define N_XHCI_MEM_HOST_CTRL_PORT_CTRL_HBUF_WATER_MARK 12
#define R_XHCI_MEM_PMCTRL_REG                 0x80A4  ///< Power Management Control
#define R_XHCI_MEM_HOST_CTRL_MISC_REG         0x80B0  ///< Host Controller Misc Reg
#define R_XHCI_MEM_HOST_CTRL_MISC_REG2        0x80B4  ///< Host Controller Misc Reg 2
#define B_XHCI_MEM_HOST_CTRL_MISC_REG2_HCRST_CTRL_ISOL_DISABLE BIT15 ///< HC Reset Controller Isolation Disable
#define R_XHCI_MEM_SSPE_REG                   0x80B8  ///< Super Speed Port Enables
#define R_XHCI_MEM_AUX_CTRL_REG               0x80C0  ///< AUX Reset Control
#define R_XHCI_MEM_HOST_CTRL_PORT_LINK_REG    0x80EC  ///< SuperSpeed Port Link Control
#define B_XHCI_MEM_HOST_CTRL_PORT_LINK_REG_LR_MIN_TM    (BIT14 | BIT13 | BIT12)  ///< Link Recovery Minimum Time
#define N_XHCI_MEM_HOST_CTRL_PORT_LINK_REG_LR_MIN_TM    12  ///< Link Recovery Minimum Time
#define B_XHCI_MEM_HOST_CTRL_PORT_LINK_REG_LP_MIN_TM    (BIT11 | BIT10 | BIT9)  ///< Link Polling Minimum Time
#define N_XHCI_MEM_HOST_CTRL_PORT_LINK_REG_LP_MIN_TM    9  ///< Link Polling Minimum Time

#define R_XHCI_MEM_HOST_IF_CTRL_REG           0x8108  ///< Host Interface Control
#define R_XHCI_MEM_HOST_CTRL_TRM_REG2         0x8110  ///< Host Control Transfer Manager TRM
#define B_XHCI_MEM_HOST_CTRL_TRM_REG2_CLR_CPCR BIT2   ///< port Credits management
#define R_XHCI_MEM_USB2_PROTOCOL_GAP_TIMER_REG_HI     0x8138  ///< USB2_PROTOCOL_GAP_TIMER_HIGH_REG - USB2 Protocol Gap Timer HIGH
#define B_XHCI_MEM_USB2_PROTOCOL_GAP_TIMER_REG_HI_LSTX_GAP_TIME  (BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_XHCI_MEM_USB2_PROTOCOL_GAP_TIMER_REG_HI_LSTX_GAP_TIME  16
#define R_XHCI_MEM_PWR_SCHED_CTRL0              0x8140  ///< HOST_IF_PWR_CTRL_REG0 - Power Scheduler Control 0
#define B_XHCI_MEM_PWR_SCHED_CTRL0_BPSAW        0xFFF000 ///< FPSAW - Fabric PLL Shutdown Advance Wake
#define N_XHCI_MEM_PWR_SCHED_CTRL0_BPSAW        12      ///< FPSAW - Fabric PLL Shutdown Advance Wake
#define R_XHCI_MEM_PWR_SCHED_CTRL2              0x8144  ///< HOST_IF_PWR_CTRL_REG1 - Power Scheduler Control 1
#define B_XHCI_MEM_PWR_SCHED_CTRL2_NDE_SBMSG_EN BIT24   ///< Enable NDE Sideband Messaging
#define B_XHCI_MEM_PWR_SCHED_CTRL2_HS_INT_IN_ALRM         BIT8    ///< HS Interrupt-IN Alarm
#define R_XHCI_MEM_AUX_CTRL_REG2                0x8154  ///< AUX Power Management Control
#define B_XHCI_MEM_AUX_CTRL_REG2_EN_U2_P3       BIT13   ///< Enable U2 P3 Mode
#define R_XHCI_MEM_USB_LPM_PARAM                0x8170  ///< USB LPM Parameters
#define R_XHCI_MEM_XLTP_LTV1                    0x8174  ///< xHC Latency Tolerance Parameters - LTV Control
#define R_XHCI_MEM_XLTP_LTV2                    0x8178  ///< xHC Latency Tolerance Control 2 - LTV Control 2
#define B_XHCI_MEM_XLTP_LTV2_LTV_LMT            0x1FFF  ///< LTV Limit field bits
#define R_XHCI_MEM_XLTP_HITC                    0x817C  ///< xHC Latency Tolerance Parameters - High Idle Time Control
#define R_XHCI_MEM_XLTP_MITC                    0x8180  ///< xHC Latency Tolerance Parameters - Medium Idle Time Control
#define R_XHCI_MEM_XLTP_LITC                    0x8184  ///< xHC Latency Tolerance Parameters - Low Idle Time Control
#define R_XHCI_MEM_CFG_USB2_LTV_U2_NOREQ_REG    0x8188  ///< ADO USB2 LTR Register
#define R_XHCI_MEM_PDDIS_REG                    0x8198  ///< xHC Pull Down Disable Control
#define R_XHCI_MEM_LFPS_PM_CTRL_REG             0x81A0  ///< LFPS PM Control
#define R_XHCI_MEM_D0I2_CTRL_REG                0x81BC  ///< D0I2 Control Register
#define B_XHCI_MEM_D0I2_CTRL_REG_B2B_BTO_EN     BIT31   ///< USB2 Back to Back BTO Handling Enable
#define N_XHCI_MEM_D0I2_CTRL_REG_MSID0I2PWT     16      ///< Bitshift for MSI D0i2 Pre Wake Time
#define R_XHCI_MEM_D0I2_SCH_ALARM_CTRL_REG      0x81C0  ///< D0i2 Scheduler Alarm Control Register
#define B_XHCI_MEM_D0I2_SCH_ALARM_CTRL_REG_D0I2WL   0x1FFF      ///< D0i2 Wake Latency
#define B_XHCI_MEM_D0I2_SCH_ALARM_CTRL_REG_D0I2IT   0x1FFF0000  ///< D0i2 Idle Time
#define N_XHCI_MEM_D0I2_SCH_ALARM_CTRL_REG_D0I2IT   16          ///< Bitshift for D0i2 Idle Time
#define R_XHCI_MEM_USB2PMCTRL_REG               0x81C4  ///< USB2 Power Management Control
#define R_XHCI_MEM_PM_CTRL_REG1                 0x81C8  ///< AUX Reset Control
#define B_XHCI_MEM_PM_CTRL_REG1_IP_INACCESSIBLE_HYSTERESIS_TIMER          (BIT9 | BIT8 | BIT7)  ///< Inaccessible Hysteresis Timer
#define N_XHCI_MEM_PM_CTRL_REG1_IP_INACCESSIBLE_HYSTERESIS_TIMER          7  ///< Inaccessible Hysteresis Timer
#define V_XHCI_MEM_PM_CTRL_REG1_IP_INACCESSIBLE_HYSTERESIS_TIMER_150US    BIT8                  ///< Force PG timeout 150us
#define B_XHCI_MEM_PM_CTRL_REG1_OVERRIDE_DISABLE    BIT6                  ///< Override_Disable

#define R_XHCI_MEM_DBGDEV_CTRL_REG1                     0x8754  ///< Debug Device Control Register 1

#define R_XHCI_MEM_PMCTRL2                      0x8468    ///< Power Management Control 2
#define B_XHCI_MEM_PMCTRL2_PORT_RF_PG_USB2_DIS_DISC_EN    BIT4 ///< Per Port RF Power Gating for USB2
#define R_XHCI_MEM_HOST_CTRL_SUS_LINK_PORT_REG  0x81F8    ///< Host Controller SUS Link Port Reg
#define R_XHCI_MEM_HOST_CTRL_EARLY_DBG_REG      0x81FC    ///< HOST CTRL EARLY DBG REG
#define R_XHCI_MEM_MULT_IN_SCH_POLICY           0x82A0    ///< Multiple_in Scheduler Policy Register
#define R_XHCI_MEM_TRB_PRF_CTRL_REG4            0x82F0
#define B_XHCI_MEM_TRB_PRF_CTRL_REG4_LTRNDEISOINEN_ESS    BIT1
#define B_XHCI_MEM_TRB_PRF_CTRL_REG4_TDWTMRK              (BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_XHCI_MEM_TRB_PRF_CTRL_REG4_TDWTMRK              16
#define V_XHCI_MEM_TRB_PRF_CTRL_REG4_TDWTMRK              5
#define R_XHCI_MEM_HOST_CTRL_ENG_MISC_REG3      0x82FC
#define R_XHCI_MEM_GRP0_SAI_WAC_POLICY_LO       0x8308    ///< Group0 SAI Write Access Control Policy Register Low
#define R_XHCI_MEM_GRP0_SAI_RAC_POLICY_LO       0x8310    ///< Group0 SAI Read Access Control Policy Low
#define R_XHCI_MEM_PMREQ_CTRL_REG               0x83D0    ///< PMREQ Control Register
#define B_XHCI_MEM_PMREQ_CTRL_REG_PWRSCH_IT_IN_LTR BIT12  ///< Enable feeding Periodic Scheduler Idle time into LTR
#define R_XHCI_MEM_U2UB_SURVIVABILITY_REG1      0x83E8    ///< U2U Bridge Survivability Register 1
#define R_XHCI_MEM_U2UB_SURVIVABILITY_REG2      0x83EC    ///< U2U Bridge Survivability Register 2
#define B_XHCI_MEM_U2UB_SURVIVABILITY_REG2_DRIVE_DEV_BYP_CLK_READY BIT14
#define R_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_REG_0_MMIO                                    0x89C4   ///< DAP USB2 Port Control 0 Register
#define B_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_REG_0_MMIO_CONNECTOR_EVENT                    (BIT7 | BIT6 | BIT5)   ///< Connector Event (CE) in DAP USB2 Port<N> Control 0 Register
#define B_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_REG_0_MMIO_SW_VBUS                            BIT8   ///< VBUS presence
#define R_XHCI_MEM_DAP_USB2_PORT_CONTROL_1_REG_0_MMIO                                    0x89C8   ///< DAP USB2 Port Control 1 Register
#define B_XHCI_MEM_DAP_USB2_PORT_CONTROL_1_REG_0_MMIO_CONNECTOR_TYPE                     (BIT10 | BIT9 | BIT8)   ///< Connector Type (CT)
#define V_XHCI_MEM_DAP_USB2_PORT_CONTROL_1_REG_0_MMIO_CONNECTOR_TYPE_AB                  BIT8
#define V_XHCI_MEM_DAP_USB2_PORT_CONTROL_1_REG_0_MMIO_CONNECTOR_TYPE_C                   0x0
#define R_XHCI_MEM_DAP_USB2_PORT_STATUS_REG_0_MMIO                                       0x89CC   ///< DAP USB2 Port Status Register
#define V_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_REG_0_MMIO_CONNECTOR_EVENT_DEV_SUBSCRIPTION   0x60    ///< Device subscription
#define V_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_REG_0_MMIO_CONNECTOR_EVENT_TYPEABANDC         0x20     ///< Value loaded for Connector Type AB and Type C
#define S_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_SPACING                                       0x10     ///< Size of space between DAP USB2 Port <N> Control 0 Register for each port
#define R_XHCI_MEM_STRAP13_REG                  0x8830    ///< Miscellaneous Strap Register
#define B_XHCI_MEM_STRAP13_REG_FORCE_P3         BIT2      ///< FORCE_P3
#define R_XHCI_MEM_HOST_CTRL_SSP_LINK_PORT_REG1      0x8E5C    ///< Dublin HOST_CTRL_SSP_LINK_REG1
#define R_XHCI_MEM_HOST_CTRL_SSP_LINK_REG2      0x8E68    ///< Dublin HOST_CTRL_SSP_LINK_REG2
#define R_XHCI_MEM_HOST_CTRL_SSP_LFPS_REG4      0x8E7C    ///< Dublin LFPS Register 4
#define B_XHCI_MEM_HOST_CTRL_SSP_LFPS_REG4_CFG_SCD_LFPS_TIMEOUT   (BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17)
#define N_XHCI_MEM_HOST_CTRL_SSP_LFPS_REG4_CFG_SCD_LFPS_TIMEOUT   17
#define B_XHCI_MEM_HOST_CTRL_SSP_LFPS_REG4_CFG_TX_SCD_COUNT       (BIT16 | BIT15 | BIT14)
#define N_XHCI_MEM_HOST_CTRL_SSP_LFPS_REG4_CFG_TX_SCD_COUNT       14
#define R_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG1    0x8e80    ///< Host Ctrl SSP Config Register 1
#define B_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG1_CFG_DIS_HHH_FLUSH_FIX    BIT27    ///< DIS HHH_FLUSH_FIX
#define R_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG2    0x8E9C    ///< Host Ctrl SSP Config Register 2
#define R_XHCI_MEM_HOST_CTRL_USB3_RECAL_REG     0x8E84    ///< Host Ctrl USB3 Recalibration Register
#define R_XHCI_MEM_HOST_CTRL_SSP_TUNNELING_REG  0x8EA0    ///< Host Ctrl SSP Tunneling Register
#define R_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE 0x8ED4  ///< REG HOST_CTRL_USB3_PORT_CONFIG2_REG
#define B_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE_USB3_PROG_DONE   BIT0
#define B_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE_CHK_BIT_EN_B2B_RETRY_FIX BIT24 ///< CHK_BIT_EN_B2B_RETRY_FIX

#define R_XHCI_MEM_HOST_CTRL_USB3_CP13_DEEMPH_REG   0x8E8C
#define R_XHCI_MEM_HOST_CTRL_USB3_CP14_DEEMPH_REG   0x8E90
#define R_XHCI_MEM_HOST_CTRL_USB3_CP15_DEEMPH_REG   0x8E94
#define R_XHCI_MEM_HOST_CTRL_USB3_CP16_DEEMPH_REG   0x8E98

//
// TCSS CPU XHCI DAP ESS PORT Registers in MMIO Space
//
#define R_XHCI_MEM_DAP_ESS_PORT_CONTROL_1_REG_0_MMIO  0x8AC8 ///< DAP eSS Port <N> Control1 Register for TCSS CPU XHCI PORT.
#define S_CPU_XHCI_MEM_ESS_PORT_CONTROL1_SPACING      0x10   ///< Size of space between DAP eSS Port <N> Control1 Register for each port. Max 4 CPU XHCI Port
#define B_XHCI_PCR_DAP_ESS_PORT_CONTROL_1_REG_0_RTCO  BIT3   ///< DAP eSS Port <N> Control1 Register for TCSS CPU XHCI PORT.

#define R_XHCI_MEM_USB2PDO    0x84F8    ///< USB2 Port Disable Override register
#define R_XHCI_MEM_USB3PDO    0x84FC    ///< USB3 Port Disable Override register
#define R_XHCI_MEM_ADO_CONFIG_REG1  0x91F4    ///< Audio Offload Control
#define B_XHCI_MEM_ADO_CONFIG_REG1_ADO_MAX_PYLD_SIZE  (BIT5 | BIT4 | BIT3)    ///< ADO Max Payload Size
#define N_XHCI_MEM_ADO_CONFIG_REG1_ADO_MAX_PYLD_SIZE  3    ///< ADO Max Payload Size
#define R_XHCI_MEM_U2PRM_U2PRDE     0x92F4    ///< USB2 Port Reset Messaging Enable

//
// Debug Capability Descriptor Parameters
//
#define R_XHCI_MEM_DBCCTL_REG                   0x8760    ///< DBCCTL - DbC Control
#define N_XHCI_MEM_DBCCTL_REG_DISC_RXD_CNT      2         ///< Soft Disconnect RX Detect Count bitshift

#define R_XHCI_MEM_SOCHWSTSAVE_REG1             0x8504    ///< SOC HW State Save 1 Register
#define B_XHCI_MEM_SOCHWSTSAVE_REG1_CMD_SSV     BIT31     ///< cmd_ssv flag which indicates that context is saved or not

//
// Over Current Mapping registers
//
#define R_XHCI_MEM_U2OCM1   0x90A4    ///< XHCI USB2 Overcurrent Pin N Mapping
#define R_XHCI_MEM_U3OCM1   0x9124    ///< XHCI USB3 Overcurrent Pin N Mapping



//
// xHCI Private registers
//
#define R_XHCI_PCR_DAP_COMMON_CONTROL_REG         0x440   ///< DAP Common Control Register
#define R_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0  0x500   ///< DAP USB2 Port0 Control 0 Register
#define B_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_CONNECTOR_EVENT                  (BIT7 | BIT6 | BIT5)   ///< Connector Event (CE) in DAP USB2 Port<N> Control 0 Register
#define V_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_CONNECTOR_EVENT_HOST             0        ///< Host Operation State
#define V_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_CONNECTOR_EVENT_DISCONNECT       0x20     ///< Un-subscription Operation State
#define V_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_CONNECTOR_EVENT_DBC              0x80    ///< DBC Operation State
#define B_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_SW_VBUS  BIT8   ///< VBUS presence
#define R_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0     0x508   ///< DAP USB2 Port0 Status 0 Register
#define B_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0_OPERATION_STATE       (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)    ///< Operation State (OS) in DAP USB2 Port<N> Status 0 Register
#define V_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0_OPERATION_STATE_HOST     0x01    ///< Host Operation State
#define V_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0_OPERATION_STATE_DISCON   0x02    ///< Disconnected Operation State
#define V_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0_OPERATION_STATE_DBC      0x40    ///< DBC Operation State
#define R_XHCI_PCR_DAP_ESS_PORT_CONTROL_1_REG_0   0x604   ///< DAP eSS Port <N> Control 1 Register
#define S_XHCI_PCR_DAP_ESS_PORT_CONTROL1_SPACING  0x10    ///< Size of space between DAP eSS Port <N> Control 1 Register for each port

//
// xHCI DCI MMIO Registers
//
#define R_XHCI_PCR_DBC_TRACE_IN_PAYLOAD_BP_LOW            0x50
#define R_XHCI_PCR_DBC_TRACE_IN_PAYLOAD_BP_HI             0x54
#define R_XHCI_PCR_DBC_TRACE_IN_PAYLOAD_QUALIFIERS        0x58
#define R_XHCI_PCR_DBC_TRACE_IN_STATUS_BP_LOW             0x60
#define R_XHCI_PCR_DBC_TRACE_IN_STATUS_BP_HI              0x64
#define R_XHCI_PCR_DBC_TRACE_IN_STATUS_QUALIFIERS         0x68
#define V_XHCI_PCR_DCI_DBC_TRACE_QUALIFIERS               0x22B800
#define R_XHCI_PCR_DBC_ECO_POLICY_REG1                    0x78
#define B_XHCI_PCR_DBC_ECO_POLICY_REG1_EN_DBCHHH_USB3LL   BIT28
#define R_XHCI_PCR_PMREQDBC                               0xC0
#define B_XHCI_PCR_PMREQDBC_DBCTRACEEPWAKE                BIT8

#endif
