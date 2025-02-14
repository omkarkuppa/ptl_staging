/** @file
  Register names for PCH THC Controllers

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
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _THC_REGS_
#define _THC_REGS_

//
// PCI Config
//
#define R_THC_CFG_BAR                                     0x10      //< Base Address Register
#define R_THC_CFG_BAR_HIGH                                0x14      //< Base Address Register High
#define R_THC_CFG_BAR1                                    0x18      //< Base Address Register1
#define R_THC_CFG_BAR1_HIGH                               0x1C      //< Base Address Register1 High
#define R_THC_CFG_UR_STS_CTL                              0x40 ///< THC Unsupported Request Status
#define B_THC_CFG_UR_STS_CTL_FD                           BIT2 ///< [RW/L] Function Disable - If set to 1 by softweare, THC is disabled
#define R_THC_CFG_PMD_PMCSRBSE_PMCSR                      0x74 ///< PCI Power Management Control and Status
#define V_THC_CFG_PMD_PMCSRBSE_PMCSR_PWRST_D3             (BIT1| BIT0)
#define R_THC_CFG_PCE                                     0xA4 ///< Power Control Enables
#define B_THC_CFG_PCE_D3HE                                BIT2 ///< D3-Hot Enable

//
// Port Control
//
#define R_THC_MEM_PRT_CONTROL                             0x1008        ///< Touch Host Controller Control Register (THC_M_PRT_CONTROL)
#define B_THC_MEM_PRT_CONTROL_SPI_IO_RDY                  BIT29         ///< 0: This SPI port IO is not ready for operation 1: This SPI port IO is ready for operation
                                                                        // This bit is used by driver to decide whether the SPI port is ready to use.
                                                                        // SW needs to wait this bit to be 1 once on D0 entry (boot or D3 exit) before running any PIO/DMA cycles on SPI bus.
#define B_THC_MEM_PRT_CONTROL_PORT_SUPPORTED              BIT28         ///< Whether this port is supported or not
                                                                        // Driver will not operate the other ports where the PORT_SUPPORTED bit is 0.
#define B_THC_MEM_PRT_CONTROL_THC_BIOS_LOCK_EN            BIT27         ///< Locks all THC_M_PRT_GUC_VDM* registers until driver takes
#define B_THC_MEM_PRT_CONTROL_DEVRST                      BIT3          ///< 1: Deassert Device reset/power on through GPIO. 0: Assert Device reset/power off through GPIO
#define B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_HW_STS   BIT2          ///< HW will set this bit once RX sequencer is IDLE, after completing processing of any microframe that started before the THC_DEVINT_QUIESCE_EN
                                                                        // SW cannot clear THC_DEVINT_QUIESCE_EN until it sees this bit set
#define B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_EN       BIT1          ///< When SW writes 1 to this bit, THC is expected to complete processing the current microframe and then set THC_DEVINT_QUIESCE_HW_STS
#define R_THC_MEM_PRT_SPI_CFG                             0x1010        ///< THC SPI Bus Configuration Register (THC_M_PRT_SPI_CFG)
#define B_THC_MEM_PRT_SPI_CFG_SPI_LOW_FREQ_EN             BIT23         // Enable SPI Clock Divide by 8 to support low freq device. (SPI_LOW_FREQ_EN)
                                                                        // 1: All the SPI read/write clock frequency (defined in SPI_TCWF/SPI_TCRF)
                                                                        //    will be divided further by 8 SPI_TCWF
                                                                        // 0: No further divider is supported.
#define N_THC_MEM_PRT_SPI_CFG_SPI_TCWF                    20
#define N_THC_MEM_PRT_SPI_CFG_SPI_TWMODE                  18
#define B_THC_MEM_PRT_SPI_CFG_FREQ                        (BIT0|BIT1|BIT2)   // Mask for both read and write freq
#define V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_40MHZ             (BIT0|BIT1)        // 40 MHz 011b Divide by 3, SPI_LOW_FREQ_EN = 0
#define V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_30MHZ             (BIT2)             // 30 MHz 100b Divide by 4, SPI_LOW_FREQ_EN = 0
#define V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_24MHZ             (BIT0|BIT2)        // 24 MHz 101b Divide by 5, SPI_LOW_FREQ_EN = 0
#define V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_20MHZ             (BIT1|BIT2)        // 20 MHZ 110b Divide by 6, SPI_LOW_FREQ_EN = 0
#define V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_17MHZ             (BIT0|BIT1|BIT2)   // 17 MHZ 111b Divide by 7, SPI_LOW_FREQ_EN = 0
#define V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_15MHZ              (BIT0)             // 15 MHz 001b Divide by 8, SPI_LOW_FREQ_EN = 1
#define V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_7P5MHZ             (BIT1)             // 7.5 MHz 010b Divide by 16, SPI_LOW_FREQ_EN = 1
#define V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_5MHZ               (BIT0|BIT1)        // 5 MHz 011b Divide by 24, SPI_LOW_FREQ_EN = 1
#define V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_3P75MHZ            (BIT2)             // 3.75 MHz 100b Divide by 32, SPI_LOW_FREQ_EN = 1
#define V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_3MHZ               (BIT0|BIT2)        // 3 MHz 101b Divide by 40, SPI_LOW_FREQ_EN = 1
#define V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_2P5MHZ             (BIT1|BIT2)        // 2.5 MHz 110b Divide by 48, SPI_LOW_FREQ_EN = 1
#define V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_2P1MHZ             (BIT0|BIT1|BIT2)   // 2.1 MHz 111b Divide by 56, SPI_LOW_FREQ_EN = 1
#define N_THC_MEM_PRT_SPI_CFG_SPI_TCRF                    4
#define N_THC_MEM_PRT_SPI_CFG_SPI_TRMODE                  2
#define B_THC_MEM_PRT_SPI_CFG_SPI_TRDC                    (BIT1|BIT0)    // Mask for both read and write mode
#define V_THC_MEM_PRT_SPI_CFG_SINLGE_IO                   0
#define R_THC_MEM_PRT_SPI_ICRRD_OPCODE                    0x1014         ///< THC SPI Bus Configuration Register
#define R_THC_MEM_PRT_SPI_DMARD_OPCODE                    0x1018         ///< THC SPI Bus Read Opcode Register
#define R_THC_MEM_PRT_SPI_WR_OPCODE                       0x101C         ///< THC SPI Bus Write Opcode Register
#define R_THC_MEM_PRT_INT_ENABLE                          0x1020         ///< THC Interrupt Enable Register
#define R_THC_MEM_PRT_INT_STATUS                          0x1024         ///< THC Interrupt Status Register
#define R_THC_MEM_PRT_ERR_CAUSE                           0x1028         ///< THC Error Cause Register
#define R_THC_MEM_PRT_SW_SEQ_CNTRL                        0x1040         ///< THC SW sequencing Control
#define B_THC_MEM_PRT_SW_SEQ_CNTRL_TSSGO                  BIT0           ///< THC SW Sequence Cycle Go RW/1S/V
#define R_THC_MEM_PRT_SW_SEQ_STS                          0x1044         ///< THC SW sequencing Status
#define B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE                  BIT0           ///< 1 when the SW Touch Cycle completes after software previously set the TSSGO bit
                                                                         // This bit remains asserted until cleared by software writing a 1 or host partition reset.
#define B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR               BIT1           ///< This bit remains asserted until cleared by software writing a 1 or until a partition reset occurs.
                                                                         // Software must clear this bit before setting the THC Cycle GO bit in this register.n reset.
#define B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_CIP               BIT3           // Hardware sets this bit when software sets the THC Cycle Go (TSSGO) bit.
                                                                         // This bit remains set until the cycle completes on the bus interface.
                                                                         // Hardware automatically sets and clears this bit so that software can determine when read data is valid.
                                                                         // And/Or when it is safe to begin programming the next command. Software must only program the next command when this bit is 0.
#define R_THC_MEM_PRT_SW_SEQ_DATA0_ADDR                   0x1048         ///< THC SW Sequencing Data DW0 or SPI Address Register
#define R_THC_MEM_PRT_SW_SEQ_DATA1                        0x104C         ///< THC SW sequencing Data DW1
#define R_THC_MEM_PRT_WPRD_BA_LOW                         0x1090         ///< THC Write PRD Base Address Register Low
#define R_THC_MEM_PRT_WPRD_BA_HI                          0x1094         ///< THC Write PRD Base Address Register High
#define R_THC_MEM_PRT_WRITE_DMA_CNTRL                     0x1098         ///< THC Write DMA Control
#define R_THC_MEM_PRT_WRITE_INT_STS                       0x109C         ///< THC Write DMA Interrupt Status
#define R_THC_MEM_PRT_WR_BULK_ADDR                        0x10B4         ///< THC device address for the bulk write - Output Report Header Address
#define R_THC_MEM_PRT_DEV_INT_CAUSE_ADDR                  0x10B8         ///< THC Device Interrupt Cause Register Address - Input Report Header Address
#define R_THC_MEM_PRT_DEV_INT_CAUSE_REG_VAL               0x10BC         ///< THC Device Interrupt Cause Register Value - Input Report Header Value
#define R_THC_MEM_PRT_DEVINT_CNT                          0x10E8         ///< Touch Device Interrupt Counter
#define R_THC_MEM_PRT_TX_FRM_CNT                          0x10E0         ///< Touch TX Frame Counter
#define R_THC_MEM_PRT_TXDMA_PKT_CNT                       0x10E4         ///< Touch TX DMA Packet Counter
#define R_THC_MEM_PRT_DEVINT_CFG_1                        0x10EC         ///< Touch Device Interrupt Cause register Format Configuration Register 1
#define R_THC_MEM_PRT_DEVINT_CFG_2                        0x10F0         ///< Touch Device Interrupt Cause register Format Configuration Register 2
#define R_THC_MEM_PRT_READ_DMA_CNTRL_1                    0x110C         ///< THC Read DMA Control for the 1st RXDMA
#define R_THC_MEM_PRT_READ_DMA_INT_STS_1                  0x1110         ///< THC Read Interrupt Status for the 1st RXDMA
#define R_THC_MEM_PRT_TSEQ_CNTRL_1                        0x1128         ///< Touch Sequencer Control for the 1st DMA
#define R_THC_MEM_PRT_RD_BULK_ADDR_1                      0x1170         ///< THC Device Address for the bulk/touch data read for the 1st RXDMA
#define R_THC_MEM_PRT_DB_CNT_1                            0x11A0         ///< Touch Host Controller Doorbell Counter for the 1st RXDMA
#define R_THC_MEM_PRT_FRM_CNT_1                           0x11A4         ///< THC Frame Count from the 1st Stream RXDMA on this port
#define R_THC_MEM_PRT_UFRM_CNT_1                          0x11A8         ///< Touch Microframe Counter for the 1st RXDMA
#define R_THC_MEM_PRT_RXDMA_PKT_CNT_1                     0x11AC         ///< Touch RX DMA Packet Counter for the 1st RXDMA
#define R_THC_MEM_PRT_SWINT_CNT_1                         0x11B0         ///< THC Software Interrupt Count from the 1st Stream RXDMA on this port
#define R_THC_MEM_PRT_FRAME_DROP_CNT_1                    0x11B4         ///< Touch Sequencer Frame Drop Counter for the 1st RXDMA
#define R_THC_MEM_PRT_RPRD_BA_LOW_2                       0x1200         ///< THC Read PRD Base Address Low for the 2nd RXDMA
#define R_THC_MEM_PRT_RPRD_BA_HI_2                        0x1204         ///< THC Read PRD Base Address High for the 2nd RXDMA
#define R_THC_MEM_PRT_RPRD_CNTRL_2                        0x1208         ///< THC Read PRD Control for the 2nd RXDMA
#define R_THC_MEM_PRT_READ_DMA_INT_STS_2                  0x1210         // < THC Read Interrupt Status for the 2nd RXDMA
#define R_THC_MEM_PRT_READ_DMA_CNTRL_2                    0x120C         ///< THC Read DMA Control for the 2nd RXDMA
#define R_THC_MEM_PRT_RD_BULK_ADDR_2                      0x1270         ///< THC Device Address for the bulk/touch data read for the 2nd RXDMA
#define R_THC_MEM_PRT_FRM_CNT_2                           0x12A4         ///< Touch Frame Counter for the 2nd DMA engine
#define R_THC_MEM_PRT_UFRM_CNT_2                          0x12A8         ///< Touch Microframe Counter for the 2nd DMA engine
#define R_THC_MEM_PRT_RXDMA_PKT_CNT_2                     0x12AC         ///< Touch RX DMA Packet Counter for the 2nd DMA engine
#define R_THC_MEM_PRT_SWINT_CNT_2                         0x12B0         ///< Touch Host Controller Doorbell Counter for the 2nd DMA engine
#define R_THC_MEM_PRT_FRAME_DROP_CNT_2                    0x12B4         ///< Touch Sequencer Frame Drop Counter for the 2nd RXDMA
#define R_THC_MEM_PRT_RPRD_BA_LOW_SW                      0x12C0         ///< THC Read PRD Base Address Low for the SW RXDMA
#define R_THC_MEM_PRT_RPRD_BA_HI_SW                       0x12C4         /// < THC Read PRD Base Address High for the SW RXDMA
#define R_THC_MEM_PRT_RPRD_CNTRL_SW                       0x12C8         ///< THC Read PRD Control for the SW RXDMA
#define R_THC_MEM_PRT_READ_DMA_CNTRL_SW                   0x12CC         ///< THC Read DMA Control for the SW RXDMA
#define R_THC_MEM_PRT_READ_DMA_INT_STS_SW                 0x12D0         ///< THC Read Interrupt Status for the SW RXDMA
#define R_THC_MEM_PRT_SW_DMA_PRD_TABLE_LEN                0x12E4         ///< SW DMA PRD Table Length
#define R_THC_MEM_PRT_PRD_EMPTY_CNT_1                     0x12F0         ///< Touch Sequencer PRD Table Empty Counter when dvice asserts interrupt for the 1st RXDMA. One register per RXDMA enginer.
#define R_THC_MEM_PRT_PRD_EMPTY_CNT_2                     0x12F4         ///< Touch Sequencer PRD Table Empty Counter when dvice asserts interrupt for the 2nd RXDMA. One register per RXDMA enginer.
#define R_THC_MEM_PRT_SW_SEQ_I2C_WR_CNTRL                 0x1304         ///< THC Register for SW I2C write sequecing control

//
//  Clocks
//
#define R_THC_PCR_SB_SPI_CTRL                             0x40           ///< THC SPI Interface Control Register
#define B_THC_PCR_SB_SPI_CTRL_SPI_DFX_CLK_EN              BIT0           ///< Enables the SPI DFX clock in HVM mode
                                                                         // 0: Use the functional clock for SPI interface clock
                                                                         // 1: Use the Dfx_spi_clock input for SPI interface clock.
//
// D0I2 Entry Timer
//
#define R_THC_PCR_SB_PM_CTRL                              0x48           ///< THC Power Management Control Register
                                                                         // 4'b0000: 0us
                                                                         // 4'b0001: 100ns
                                                                         // 4'b0010: 1us
#define V_THC_PCR_SB_PM_CTRL_D0I2_ENTRY_TIMER_10US        (BIT0|BIT1)    // 4'b0011: 10us
                                                                         // 4'b0100: 100us
                                                                         // 4'b0101: 1ms
                                                                         // 4'b0110: 10ms
                                                                         // 4'b0111: 100ms
                                                                         // 4'b1000: 1s
                                                                         // 4'b1001: 10s
#define R_THC_PCR_RESOURCE_EN_CTL                         0x98           ///< THC Resource Enable Control Register
#define V_THC_PCR_SB_RESOURCE_EN_CTL_DISABLE_0_9          (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8|BIT9)
#define R_THC_PCR_SLEEP_STATES_CTL                        0xA8           ///< THC Sleep States Control
#define B_THC_PCR_SLEEP_STATES_CTL_DIS_QOS_DMD_RSP        BIT15          ///< When set THC will send QOS_DMD request, which do not require SOC to return QOS_RSP

#endif //_THC_REGS_
