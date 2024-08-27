/** @file
  Register names for PCH Smbus Device.

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
#ifndef _PCH_REGS_SMBUS_H_
#define _PCH_REGS_SMBUS_H_

//
// SMBus Controller Registers
//
#define R_SMBUS_CFG_SBA                       0x20             ///< SMB Base Address
#define V_SMBUS_CFG_BASE_SIZE                 (1 << 5)
#define B_SMBUS_CFG_SBA_BA                    0x0000FFE0       ///< Base Address
#define R_SMBUS_CFG_HCFG                      0x40             ///< Host Configuration
#define B_SMBUS_CFG_HCFG_SPDWD                BIT4             ///< SPD Write Disable
#define B_SMBUS_CFG_HCFG_SSRESET              BIT3             ///< SSRESET
#define B_SMBUS_CFG_HCFG_I2CEN                BIT2             ///< I2C_EN
#define B_SMBUS_CFG_HCFG_SSEN                 BIT1             ///< SMB_SMI_EN
#define B_SMBUS_CFG_HCFG_HSTEN                BIT0             ///< HST_EN
#define R_SMBUS_CFG_TCOBASE                   0x50             ///< TCO Base Address
#define B_SMBUS_CFG_TCOBASE_TCOBA             0x0000FFE0
#define R_SMBUS_CFG_TCOCTL                    0x54             ///< TCO Control
#define B_SMBUS_CFG_TCOCTL_TCO_BASE_EN        BIT8             ///< TCO Base Enable
#define B_SMBUS_CFG_TCOCTL_TCO_BASE_LOCK      BIT0             ///< TCO Base Lock
#define R_SMBUS_CFG_HTIM                      0x64             ///< Host_Timing Register
#define R_SMBUS_CFG_SMBSM                     0x80             ///< SMB_Safemode Register

//
// SMBus I/O Registers
//
#define R_SMBUS_IO_HSTS                  0x00                  ///< Host Status Register Address
#define B_SMBUS_IO_HSTS_HBSY             BIT0                  ///< Host Busy
#define B_SMBUS_IO_HSTS_INTR             BIT1                  ///< Interrupt
#define B_SMBUS_IO_HSTS_DERR             BIT2                  ///< Device Error
#define B_SMBUS_IO_HSTS_BERR             BIT3                  ///< Bus Error
#define B_SMBUS_IO_HSTS_FAIL             BIT4                  ///< Failed
#define B_SMBUS_IO_HSTS_SMSTS            BIT5                  ///< SMBALERT_STS
#define B_SMBUS_IO_HSTS_IUS              BIT6                  ///< In Use Status
#define B_SMBUS_IO_HSTS_BDS              BIT7                  ///< BYTE_DONE_STS
#define R_SMBUS_IO_HCTL                  0x02                   ///< Host Control Register
#define B_SMBUS_IO_HCTL_INTREN           BIT0                   ///< INTREN
#define B_SMBUS_IO_HCTL_KILL             BIT1                   ///< KILL
#define B_SMBUS_IO_HCTL_SMB_CMD          ( BIT4 | BIT3 | BIT2 ) ///< SMB_CMD
#define V_SMBUS_IO_SMB_CMD_QUICK         0x00
#define V_SMBUS_IO_SMB_CMD_BYTE          0x04
#define V_SMBUS_IO_SMB_CMD_BYTE_DATA     0x08
#define V_SMBUS_IO_SMB_CMD_WORD_DATA     0x0C
#define V_SMBUS_IO_SMB_CMD_PROCESS_CALL  0x10
#define V_SMBUS_IO_SMB_CMD_BLOCK         0x14
#define V_SMBUS_IO_SMB_CMD_IIC_READ      0x18
#define V_SMBUS_IO_SMB_CMD_BLOCK_PROCESS 0x1C
#define V_SMBUS_IO_HSTS_ALL              0xFF
#define B_SMBUS_IO_HCTL_LAST_BYTE        BIT5                   ///< LAST_BYTE
#define B_SMBUS_IO_HCTL_START            BIT6                   ///< START
#define B_SMBUS_IO_HCTL_PEC_EN           BIT7                   ///< PEC_EN
#define R_SMBUS_IO_HCMD                  0x03                   ///< Host Command Register
#define R_SMBUS_IO_TSA                   0x04                   ///< Transmit Target Device Address Register
#define B_SMBUS_IO_TSA_RW                BIT0                   ///< RW
#define V_SMBUS_IO_READ                  0x01                   // RW
#define V_SMBUS_IO_WRITE                 0x00                   // RW
#define B_SMBUS_IO_TSA_ADDR              ( BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 ) ///< ADDRESS
#define R_SMBUS_IO_HD0                   0x05                   ///< Data 0 Register
#define R_SMBUS_IO_HD1                   0x06                   ///< Data 1 Register
#define R_SMBUS_IO_HBD                   0x07                   ///< Host Block Data
#define R_SMBUS_IO_PEC                   0x08                   ///< Packet Error Check Data Register
#define R_SMBUS_IO_RSA                   0x09                   ///< Receive Device Address Register
#define B_SMBUS_IO_RSA_SA_6_0            ( BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0 ) ///< DEVICE_ADDR[6:0]
#define R_SMBUS_IO_SD                    0x0A                   ///< Device Data Register
#define R_SMBUS_IO_AUXS                  0x0C                   ///< Auxiliary Status
#define B_SMBUS_IO_AUXS_CRCE             BIT0                   ///< CRC Error
#define R_SMBUS_IO_AUXC                  0x0D                   ///< Auxiliary Control
#define B_SMBUS_IO_AUXC_AAC              BIT0                   ///< Automatically Append CRC
#define B_SMBUS_IO_AUXC_E32B             BIT1                   ///< Enable 32-byte Buffer
#define R_SMBUS_IO_SMLC                  0x0E                   ///< SMLINK_PIN_CTL Register
#define B_SMBUS_IO_SMLC_SMLINK0_CUR_STS  BIT0                   ///< SMLINK[0]_CUR_STS
#define B_SMBUS_IO_SMLC_SMLINK1_CUR_STS  BIT1                   ///< SMLINK[1]_CUR_STS
#define B_SMBUS_IO_SMLC_SMLINK_CLK_CTL   BIT2                   ///< SMLINK_CLK_CTL
#define R_SMBUS_IO_SMBC                  0x0F                   ///< SMBUS_PIN_CTL Register
#define B_SMBUS_IO_SMBC_SMBCLK_CUR_STS   BIT0                   ///< SMBCLK_CUR_STS
#define B_SMBUS_IO_SMBC_SMBDATA_CUR_STS  BIT1                   ///< SMBDATA_CUR_STS
#define B_SMBUS_IO_SMBC_SMBCLK_CTL       BIT2                   ///< SMBCLK_CTL
#define R_SMBUS_IO_SSTS                  0x10                   ///< Device Status Register
#define B_SMBUS_IO_SSTS_HNS              BIT0                   ///< HOST_NOTIFY_STS
#define R_SMBUS_IO_SCMD                  0x11                   ///< Device Command Register
#define B_SMBUS_IO_SCMD_HNW              BIT1                   ///< HOST_NOTIFY_WKEN
#define B_SMBUS_IO_SCMD_HNI              BIT0                   ///< HOST_NOTIFY_INTREN
#define B_SMBUS_IO_SCMD_HNW              BIT1                   ///< HOST_NOTIFY_WKEN
#define B_SMBUS_IO_SCMD_SMB_D            BIT2                   ///< SMBALERT_DIS
#define R_SMBUS_IO_NDA                   0x14                   ///< Notify Device Address Register
#define B_SMBUS_IO_NDA_DEV_ADDR          ( BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 ) ///< DEVICE_ADDRESS
#define R_SMBUS_IO_NDLB                  0x16                   ///< Notify Data Low Byte Register
#define R_SMBUS_IO_NDHB                  0x17                   ///< Notify Data High Byte Register

//
// SMBus Private Config Registers
// (PID:SMB)
//
#define R_SMBUS_PCR_TCOCFG                0x00                        ///< TCO Configuration
#define B_SMBUS_PCR_TCOCFG_IE             BIT7                        ///< TCO IRQ Enable
#define B_SMBUS_PCR_TCOCFG_IRQSEL         ( BIT2 | BIT1 | BIT0 )      ///< TCO IRQ Select
#define V_SMBUS_PCR_TCOCFG_IRQ_9          0x00
#define V_SMBUS_PCR_TCOCFG_IRQ_10         0x01
#define V_SMBUS_PCR_TCOCFG_IRQ_11         0x02
#define V_SMBUS_PCR_TCOCFG_IRQ_20         0x04                        ///< only if APIC enabled
#define V_SMBUS_PCR_TCOCFG_IRQ_21         0x05                        ///< only if APIC enabled
#define V_SMBUS_PCR_TCOCFG_IRQ_22         0x06                        ///< only if APIC enabled
#define V_SMBUS_PCR_TCOCFG_IRQ_23         0x07                        ///< only if APIC enabled
#define R_SMBUS_PCR_SMBTM                 0x04                        ///< SMBus Test Mode
#define B_SMBUS_PCR_SMBTM_SMBCT           BIT1                        ///< SM Bus Counter
#define B_SMBUS_PCR_SMBTM_SMBDG           BIT0                        ///< SMBus Deglitch
#define R_SMBUS_PCR_SCTM                  0x08                        ///< Short Counter Test Mode
#define B_SMBUS_PCR_SCTM_SSU              BIT31                       ///< Simulation Speed-Up
#define R_SMBUS_PCR_GC                    0x0C                        ///< General Control
#define B_SMBUS_PCR_GC_FD                 BIT0                        ///< Function Disable
#define B_SMBUS_PCR_GC_SMBSCGE            BIT2                        ///< SMB Static Clock Gating Enable
#define R_SMBUS_PCR_PCE                   0x10                        ///< Power Control Enable
#define B_SMBUS_PCR_PCE_PMCRE             BIT0                        ///< PMCRE bit
#define B_SMBUS_PCR_PCE_HAE               BIT5                        ///< Hardware Autonomous Enable
#define B_SMBUS_PCR_GC_SMBSLV             BIT8                        ///< SMB Target Device Support
                                                                      /*
                                                                      * 0  : SMB controller as Target Device is supported
                                                                      * 1  : SMB controller as Target Device is not supported
                                                                      */
#endif
