/** @file
  SNCU_CFG.h

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
**/

/* The following security policy groups are used by registers in this file:     */
/* SNCU_BIOS_LT_W_PG                                                            */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/* SNCU_LT_SA_W_PG                                                              */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/* SNCU_OS_W_PG                                                                 */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI | HOSTCP_PMA_SAI |      */
/*    CSE_INTEL_SAI | DFX_INTEL_MANUFACTURING_SAI | GT_PMA_SAI |                */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI | DISPLAY_SAI               */
/* SNCU_PAM_W_PG                                                                */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI |             */
/*    HOSTIA_BOOT_SAI | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI |              */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                             */

#ifndef _SNCU_CFG_PTL_CDIE_H_
#define _SNCU_CFG_PTL_CDIE_H_

#include <Base.h>


// IP GUID: a4da1120-d43c-4e62-8994-b7670b82fc02
#define HOSTBRIDGE_CFG_ID 0xa4da1120d43c4e62

//************************************************ RegisterStructs


/** VID_SNCU_CFG desc:
  * Register default value:        0x00008086
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/VID_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.VID_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.VID_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register combined with the Device Identification register uniquely identifies any PCI device.
  */

#define VID_HOSTBRIDGE_CFG_REG                 (0x00000000U)

typedef union {
  struct {
    UINT16 vid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /*
                               Vendor Identification Number: PCI standard
                               identification for Intel.
                            */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} VID_HOSTBRIDGE_CFG_STRUCT;

#define VID_HOSTBRIDGE_CFG_WIDTH 16

#define VID_HOSTBRIDGE_CFG_VID_LSB 0x0000
#define VID_HOSTBRIDGE_CFG_VID_SIZE 0x0010


/** DID_SNCU_CFG desc:
  * Register default value:        0x0000B001
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/DID_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.DID_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.DID_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register combined with the Vendor Identification register uniquely identifies any PCI device.
  */

#define DID_HOSTBRIDGE_CFG_REG                 (0x00000002U)

typedef union {
  struct {
    UINT16 did_sku : 8; /**< DID SKU */

                            /* Bits[7:0], Access Type=RW/V/L, default=0x00000001*/

                            /*
                               Device Identification Number SKU: This is
                               the lower part of device identification.
                            */

    UINT16 did_msb : 8; /**< DID MSB */

                            /* Bits[15:8], Access Type=RW/L, default=0x000000B0*/

                            /*
                               Device Identification Number MSB: This is
                               the upper part of device identification.[IntelRsvd]
                               The value of this field can be changed for
                               soft SKU IDs. Reset value is written to 0x160
                               by pcode fuse distribution[/IntelRsvd]
                            */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} DID_HOSTBRIDGE_CFG_STRUCT;

#define DID_HOSTBRIDGE_CFG_WIDTH 16

#define DID_HOSTBRIDGE_CFG_DID_SKU_LSB 0x0000
#define DID_HOSTBRIDGE_CFG_DID_SKU_SIZE 0x0008
#define DID_HOSTBRIDGE_CFG_DID_MSB_LSB 0x0008
#define DID_HOSTBRIDGE_CFG_DID_MSB_SIZE 0x0008


/** PCICMD_SNCU_CFG desc:
  * Register default value:        0x00000006
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/PCICMD_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.PCICMD_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.PCICMD_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Since Device #0 does not physically reside on PCI_A many of the bits are not implemented.
  */

#define PCICMD_HOSTBRIDGE_CFG_REG              (0x00000004U)

typedef union {
  struct {
    UINT16 ioae : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               I/O Access Enable: This bit is not implemented
                               in the CPU and is hardwired to a 0. Writes
                               to this bit position have no effect.
                            */

    UINT16 mae : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Memory Access Enable: The CPU always allows
                               access to main memory, except when such access
                               would violate security principles. Such exceptions
                               are outside the scope of PCI control. This
                               bit is not implemented and is hardwired to
                               1. Writes to this bit position have no effect.
                            */

    UINT16 bme : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               Bus Mstr Enable: The CPU is always enabled
                               as a mstr on the backbone. This bit is hardwired
                               to a 1. Writes to this bit position have no
                               effect.
                            */

    UINT16 sce : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Special Cycle Enable: The CPU does not implement
                               this bit and it is hardwired to a 0. Writes
                               to this bit position have no effect.
                            */

    UINT16 mwie : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Write and Invalidate Enable: The CPU
                               will never issue memory write and invalidate
                               commands. This bit is therefore hardwired
                               to 0. Writes to this bit position will have
                               no effect.
                            */

    UINT16 vgasnoop : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               VGA Palette Snoop Enable: The CPU does not
                               implement this bit and it is hardwired to
                               a 0. Writes to this bit position have no effect.
                            */

    UINT16 perre : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               OPI - N/A Parity Error Enable: Controls whether
                               or not the Mstr Data Parity Error bit in the
                               PCI Status register can bet set. 0: Mstr Data
                               Parity Error bit in PCI Status register can
                               NOT be set. 1:  Mstr Data Parity Error bit
                               in PCI Status register CAN be set.
                            */

    UINT16 adstep : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Address/Data Stepping Enable: Address/data
                               stepping is not implemented in the CPU, and
                               this bit is hardwired to 0. Writes to this
                               bit position have no effect.
                            */

    UINT16 serre : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               SERR Enable: This bit is a global enable bit
                               for Device 0 SERR messaging. The CPU communicates
                               the SERR condition by sending an SERR message
                               over DMI to the PCH.   1: The CPU is enabled
                               to generate SERR messages over DMI for specific
                               Device 0 error conditions that are individually
                               enabled in the ERRCMD and DMIUEMSK registers.
                               The error status is reported in the ERRSTS,
                               PCISTS, and DMIUEST registers.   0: The SERR
                               message is not generated by the Host for Device
                               0.  This bit only controls SERR messaging
                               for Device 0. Other integrated devices have
                               their own SERRE bits to control error reporting
                               for error conditions occurring in each device.
                               The control bits are used in a logical OR
                               manner to enable the SERR DMI message mechanism.
                               OPI N/A
                            */

    UINT16 fb2b : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Fast Back-to-Back Enable: This bit controls
                               whether or not the mstr can do fast back-to-back
                               write. Since device 0 is strictly a target
                               this bit is not implemented and is hardwired
                               to 0. Writes to this bit position have no
                               effect.
                            */

    UINT16 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[15:10] */

                            /* Bits[15:10], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} PCICMD_HOSTBRIDGE_CFG_STRUCT;

#define PCICMD_HOSTBRIDGE_CFG_WIDTH 16

#define PCICMD_HOSTBRIDGE_CFG_IOAE_LSB 0x0000
#define PCICMD_HOSTBRIDGE_CFG_IOAE_SIZE 0x0001
#define PCICMD_HOSTBRIDGE_CFG_MAE_LSB 0x0001
#define PCICMD_HOSTBRIDGE_CFG_MAE_SIZE 0x0001
#define PCICMD_HOSTBRIDGE_CFG_BME_LSB 0x0002
#define PCICMD_HOSTBRIDGE_CFG_BME_SIZE 0x0001
#define PCICMD_HOSTBRIDGE_CFG_SCE_LSB 0x0003
#define PCICMD_HOSTBRIDGE_CFG_SCE_SIZE 0x0001
#define PCICMD_HOSTBRIDGE_CFG_MWIE_LSB 0x0004
#define PCICMD_HOSTBRIDGE_CFG_MWIE_SIZE 0x0001
#define PCICMD_HOSTBRIDGE_CFG_VGASNOOP_LSB 0x0005
#define PCICMD_HOSTBRIDGE_CFG_VGASNOOP_SIZE 0x0001
#define PCICMD_HOSTBRIDGE_CFG_PERRE_LSB 0x0006
#define PCICMD_HOSTBRIDGE_CFG_PERRE_SIZE 0x0001
#define PCICMD_HOSTBRIDGE_CFG_ADSTEP_LSB 0x0007
#define PCICMD_HOSTBRIDGE_CFG_ADSTEP_SIZE 0x0001
#define PCICMD_HOSTBRIDGE_CFG_SERRE_LSB 0x0008
#define PCICMD_HOSTBRIDGE_CFG_SERRE_SIZE 0x0001
#define PCICMD_HOSTBRIDGE_CFG_FB2B_LSB 0x0009
#define PCICMD_HOSTBRIDGE_CFG_FB2B_SIZE 0x0001


/** PCISTS_SNCU_CFG desc:
  * Register default value:        0x00000090
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/PCISTS_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.PCISTS_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.PCISTS_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This status register reports the occurrence of error events on Device 0s PCI interface. Since Device 0 does not physically reside on PCI_A many of the bits are not implemented.
  */

#define PCISTS_HOSTBRIDGE_CFG_REG              (0x00000006U)

typedef union {
  struct {
    UINT16 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[03:00] */

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT16 clist : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Capability List: This bit is hardwired to
                               1 to indicate to the configuration software
                               that this device/function implements a list
                               of new capabilities. A list of new capabilities
                               is accessed via register CAPPTR at configuration
                               address offset 34h. Register CAPPTR contains
                               an offset pointing to the start address within
                               configuration space of this device where the
                               Capability Identification register resides.
                            */

    UINT16 mc66 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               66 MHz Capable: Does not apply to PCI Express.
                               Must be hardwired to 0.
                            */

    UINT16 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[06:06] */

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */

    UINT16 fb2b : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               Fast Back-to-Back: This bit is hardwired to
                               1. Writes to these bit positions have no effect.
                               Device 0 does not physically connect to PCI_A.
                               This bit is set to 1 (indicating fast back-to-back
                               capability) so that the optimum setting for
                               PCI_A is not limited by the Host.
                            */

    UINT16 dpd : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Mstr Data Parity Error Detected: This bit
                               is set when DMI received a Poisoned completion
                               from PCH. This bit can only be set when the
                               Parity Error Enable bit in the PCI Command
                               register is set.
                            */

    UINT16 devt : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /*
                               DEVSEL Timing: These bits are hardwired to
                               00. Writes to these bit positions have no
                               affect. Device 0 does not physically connect
                               to PCI_A. These bits are set to 00 (fast decode)
                               so that optimum DEVSEL timing for PCI_A is
                               not limited by the Host.
                            */

    UINT16 stas : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Signaled Target Abort Status: The CPU will
                               not generate a Target Abort DMI completion
                               packet or Special Cycle. This bit is not implemented
                               and is hardwired to a 0. Writes to this bit
                               position have no effect.
                            */

    UINT16 rtas : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Received Target Abort Status: This bit is
                               set when the CPU generates a DMI request that
                               receives a Completer Abort completion packet.
                               Software clears this bit by writing a 1 to
                               it.
                            */

    UINT16 rmas : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Received Mstr Abort Status: This bit is set
                               when the CPU generates a DMI request that
                               receives an Unsupported Request completion
                               packet. Software clears this bit by writing
                               a 1 to it.
                            */

    UINT16 sse : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Signaled System Error: This bit is set to
                               1 when Device 0 generates an SERR message
                               over DMI for any enabled Device 0 error condition.
                               Device 0 error conditions are enabled in the
                               PCICMD, ERRCMD, and DMIUEMSK registers. Device
                               0 error flags are read/reset from the PCISTS,
                               ERRSTS, or DMIUEST registers. Software clears
                               this bit by writing a 1 to it.
                            */

    UINT16 dpe : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Detected Parity Error: This bit is set when
                               this Device receives a Poisoned TLP.
                            */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} PCISTS_HOSTBRIDGE_CFG_STRUCT;

#define PCISTS_HOSTBRIDGE_CFG_WIDTH 16

#define PCISTS_HOSTBRIDGE_CFG_CLIST_LSB 0x0004
#define PCISTS_HOSTBRIDGE_CFG_CLIST_SIZE 0x0001
#define PCISTS_HOSTBRIDGE_CFG_MC66_LSB 0x0005
#define PCISTS_HOSTBRIDGE_CFG_MC66_SIZE 0x0001
#define PCISTS_HOSTBRIDGE_CFG_FB2B_LSB 0x0007
#define PCISTS_HOSTBRIDGE_CFG_FB2B_SIZE 0x0001
#define PCISTS_HOSTBRIDGE_CFG_DPD_LSB 0x0008
#define PCISTS_HOSTBRIDGE_CFG_DPD_SIZE 0x0001
#define PCISTS_HOSTBRIDGE_CFG_DEVT_LSB 0x0009
#define PCISTS_HOSTBRIDGE_CFG_DEVT_SIZE 0x0002
#define PCISTS_HOSTBRIDGE_CFG_STAS_LSB 0x000b
#define PCISTS_HOSTBRIDGE_CFG_STAS_SIZE 0x0001
#define PCISTS_HOSTBRIDGE_CFG_RTAS_LSB 0x000c
#define PCISTS_HOSTBRIDGE_CFG_RTAS_SIZE 0x0001
#define PCISTS_HOSTBRIDGE_CFG_RMAS_LSB 0x000d
#define PCISTS_HOSTBRIDGE_CFG_RMAS_SIZE 0x0001
#define PCISTS_HOSTBRIDGE_CFG_SSE_LSB 0x000e
#define PCISTS_HOSTBRIDGE_CFG_SSE_SIZE 0x0001
#define PCISTS_HOSTBRIDGE_CFG_DPE_LSB 0x000f
#define PCISTS_HOSTBRIDGE_CFG_DPE_SIZE 0x0001


/** RID_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/RID_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.RID_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.RID_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register contains the revision number of Device #0.  These bits are read only and writes to this register have no effect.
  */

#define RID_HOSTBRIDGE_CFG_REG                 (0x00000008U)

typedef union {
  struct {
    UINT8 rid : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Revision Identification Number: Four LSB of
                               RID
                            */

    UINT8 rid_msb : 4; /**< RID MSB */

                            /* Bits[7:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Revision Identification Number MSB: Four MSB
                               of RID
                            */

  }     Bits;
  UINT8 Data;
} RID_HOSTBRIDGE_CFG_STRUCT;

#define RID_HOSTBRIDGE_CFG_WIDTH 8

#define RID_HOSTBRIDGE_CFG_RID_LSB 0x0000
#define RID_HOSTBRIDGE_CFG_RID_SIZE 0x0004
#define RID_HOSTBRIDGE_CFG_RID_MSB_LSB 0x0004
#define RID_HOSTBRIDGE_CFG_RID_MSB_SIZE 0x0004


/** CC_PI_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/CC_PI_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.CC_PI_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.CC_PI_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register (split from original CC) identifies a register-specific programming interface.
  */

#define CC_PI_HOSTBRIDGE_CFG_REG               (0x00000009U)

typedef union {
  struct {
    UINT8 pi : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Programming Interface: This is an 8-bit value
                               that indicates the programming interface of
                               this device. This value does not specify a
                               particular register set layout and provides
                               no practical use for this device.
                            */

  }     Bits;
  UINT8 Data;
} CC_PI_HOSTBRIDGE_CFG_STRUCT;

#define CC_PI_HOSTBRIDGE_CFG_WIDTH 8

#define CC_PI_HOSTBRIDGE_CFG_PI_LSB 0x0000
#define CC_PI_HOSTBRIDGE_CFG_PI_SIZE 0x0008


/** CC_BCC_SNCU_CFG desc:
  * Register default value:        0x00000600
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/CC_BCC_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.CC_BCC_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.CC_BCC_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register (split from original CC) identifies the basic function of the device and a more specific sub-class.
  */

#define CC_BCC_HOSTBRIDGE_CFG_REG              (0x0000000aU)

typedef union {
  struct {
    UINT16 subcc : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Sub-Class Code: This is an 8-bit value that
                               indicates the category of Bridge into which
                               the Host Bridge device falls. The code is
                               00h indicating a Host Bridge.
                            */

    UINT16 bcc : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000006*/

                            /*
                               Base Class Code: This is an 8-bit value that
                               indicates the base class code for the Host
                               Bridge device.  This code has the value 06h,
                               indicating a Bridge device.
                            */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} CC_BCC_HOSTBRIDGE_CFG_STRUCT;

#define CC_BCC_HOSTBRIDGE_CFG_WIDTH 16

#define CC_BCC_HOSTBRIDGE_CFG_SUBCC_LSB 0x0000
#define CC_BCC_HOSTBRIDGE_CFG_SUBCC_SIZE 0x0008
#define CC_BCC_HOSTBRIDGE_CFG_BCC_LSB 0x0008
#define CC_BCC_HOSTBRIDGE_CFG_BCC_SIZE 0x0008


/** HDR_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/HDR_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.HDR_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.HDR_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register identifies the header layout of the configuration space. No physical register exists at this location.
  */

#define HDR_HOSTBRIDGE_CFG_REG                 (0x0000000eU)

typedef union {
  struct {
    UINT8 hdr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               PCI Header: This field always returns 0 to
                               indicate that the Host Bridge is a single
                               function device with standard header layout.
                               Reads and writes to this location have no
                               effect.
                            */

  }     Bits;
  UINT8 Data;
} HDR_HOSTBRIDGE_CFG_STRUCT;

#define HDR_HOSTBRIDGE_CFG_WIDTH 8

#define HDR_HOSTBRIDGE_CFG_HDR_LSB 0x0000
#define HDR_HOSTBRIDGE_CFG_HDR_SIZE 0x0008


/** SVID_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/SVID_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.SVID_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.SVID_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This value is used to identify the vendor of the subsystem.
  */

#define SVID_HOSTBRIDGE_CFG_REG                (0x0000002cU)

typedef union {
  struct {
    UINT16 subvid : 16;

                            /* Bits[15:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Subsystem Vendor ID: This field should be
                               programmed during boot-up to indicate the
                               vendor of the system board. After it has been
                               written once, it becomes read only.
                            */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SVID_HOSTBRIDGE_CFG_STRUCT;

#define SVID_HOSTBRIDGE_CFG_WIDTH 16

#define SVID_HOSTBRIDGE_CFG_SUBVID_LSB 0x0000
#define SVID_HOSTBRIDGE_CFG_SUBVID_SIZE 0x0010


/** SID_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/SID_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.SID_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.SID_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This value is used to identify a particular subsystem.
  */

#define SID_HOSTBRIDGE_CFG_REG                 (0x0000002eU)

typedef union {
  struct {
    UINT16 subid : 16;

                            /* Bits[15:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Subsystem ID: This field should be programmed
                               during BIOS initialization. After it has been
                               written once, it becomes read only.
                            */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SID_HOSTBRIDGE_CFG_STRUCT;

#define SID_HOSTBRIDGE_CFG_WIDTH 16

#define SID_HOSTBRIDGE_CFG_SUBID_LSB 0x0000
#define SID_HOSTBRIDGE_CFG_SUBID_SIZE 0x0010


/** CAPPTR_SNCU_CFG desc:
  * Register default value:        0x000000E0
  * Register full path in IP: sncu_sncu_top/sncu_dev0_regs_bank/CFG_PCI_D0_F0_MSG_PCI_D0_F0/CAPPTR_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncu_dev0_regs_bank.CAPPTR_0_0_0_PCI, OSX=sncu_sncu_top.sncu_dev0_regs_bank.CFG_PCI_D0_F0_MSG_PCI_D0_F0_54_9.CAPPTR_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * The CAPPTR provides the offset that is the pointer to the location of the first device capability in the capability list.
  */

#define CAPPTR_HOSTBRIDGE_CFG_REG              (0x00000034U)

typedef union {
  struct {
    UINT8 capptr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x000000E0*/

                            /*
                               Capabilities Pointer: Pointer to the offset
                               of the first capability ID register block.
                               In this case the first capability is the product-specific
                               Capability Identifier (CAPID0).
                            */

  }     Bits;
  UINT8 Data;
} CAPPTR_HOSTBRIDGE_CFG_STRUCT;

#define CAPPTR_HOSTBRIDGE_CFG_WIDTH 8

#define CAPPTR_HOSTBRIDGE_CFG_CAPPTR_LSB 0x0000
#define CAPPTR_HOSTBRIDGE_CFG_CAPPTR_SIZE 0x0008


/** PXPEPBAR_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PXPEPBAR_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PXPEPBAR_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PXPEPBAR_0_0_0_PCI
  * Security PolicyGroup: SNCU_LT_SA_W_PG
  * This is the base address for the PCI Express Egress Port MMIO Configuration space.  There is no physical memory within this 4KB window that can be addressed.  The 4KB reserved by this register does not alias to any PCI 2.3 compliant memory mapped space.  On reset, the EGRESS port MMIO configuration space is disabled and must be enabled by writing a 1 to PXPEPBAREN [Dev 0, offset 40h, bit 0]. All the bits in this register are locked in Intel TXT mode.
  */

#define PXPEPBAR_HOSTBRIDGE_CFG_REG            (0x00000040U)

typedef union {
  struct {
    UINT64 pxpepbaren : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               0: PXPEPBAR is disabled and does not claim
                               any memory 1: PXPEPBAR memory mapped accesses
                               are claimed and decoded appropriately This
                               register is locked by Intel TXT.
                            */

    UINT64 rsvd_0 : 11; /**< Undefined - auto filled rsvd_[11:01] */

                            /* Bits[11:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 pxpepbar : 30;

                            /* Bits[41:12], Access Type=RW, default=0x00000000*/

                            /*
                               This field corresponds to bits 41 to 12 of
                               the base address PCI Express Egress Port MMIO
                               configuration space.  BIOS will program this
                               register resulting in a base address for a
                               4KB block of contiguous memory address space.
                               This register ensures that a naturally aligned
                               4KB space is allocated within the first 512GB
                               of addressable memory space.  System Software
                               uses this base address to program the PCI
                               Express Egress Port MMIO register set.  All
                               the bits in this register are locked in Intel
                               TXT mode.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PXPEPBAR_HOSTBRIDGE_CFG_STRUCT;

#define PXPEPBAR_HOSTBRIDGE_CFG_WIDTH 64

#define PXPEPBAR_HOSTBRIDGE_CFG_PXPEPBAREN_LSB 0x0000
#define PXPEPBAR_HOSTBRIDGE_CFG_PXPEPBAREN_SIZE 0x0001
#define PXPEPBAR_HOSTBRIDGE_CFG_PXPEPBAR_LSB 0x000c
#define PXPEPBAR_HOSTBRIDGE_CFG_PXPEPBAR_SIZE 0x001e


/** MCHBAR_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/MCHBAR_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.MCHBAR_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.MCHBAR_0_0_0_PCI
  * Security PolicyGroup: SNCU_LT_SA_W_PG
  * This is the base address for the Host Memory Mapped Configuration space.  There is no physical memory within this 32KB window that can be addressed.  The 32KB reserved by this register does not alias to any PCI 2.3 compliant memory mapped space.  On reset, the Host MMIO Memory Mapped Configuation space is disabled and must be enabled by writing a 1 to MCHBAREN [Dev 0, offset48h, bit 0]. All the bits in this register are locked in intel TXT mode. The register space contains memory control, initialization, timing, and buffer strength registers; clocking registers; and power and thermal management registers.
  */

#define MCHBAR_HOSTBRIDGE_CFG_REG              (0x00000048U)

typedef union {
  struct {
    UINT64 mchbaren : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               0: MCHBAR is disabled and does not claim any
                               memory 1: MCHBAR memory mapped accesses are
                               claimed and decoded appropriately This register
                               is locked in Intel TXT mode.
                            */

    UINT64 rsvd_0 : 16; /**< Undefined - auto filled rsvd_[16:01] */

                            /* Bits[16:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 mchbar : 25;

                            /* Bits[41:17], Access Type=RW, default=0x00000000*/

                            /*
                               This field corresponds to bits 41 to 16 of
                               the base address Host Memory Mapped configuration
                               space.  BIOS will program this register resulting
                               in a base address for a 64KB block of contiguous
                               memory address space.  This register ensures
                               that a naturally aligned 64KB space is allocated
                               within the first 512GB of addressable memory
                               space. System Software uses this base address
                               to program the Host Memory Mapped register
                               set. All the bits in this register are locked
                               in Intel TXT mode.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MCHBAR_HOSTBRIDGE_CFG_STRUCT;

#define MCHBAR_HOSTBRIDGE_CFG_WIDTH 64

#define MCHBAR_HOSTBRIDGE_CFG_MCHBAREN_LSB 0x0000
#define MCHBAR_HOSTBRIDGE_CFG_MCHBAREN_SIZE 0x0001
#define MCHBAR_HOSTBRIDGE_CFG_MCHBAR_LSB 0x0011
#define MCHBAR_HOSTBRIDGE_CFG_MCHBAR_SIZE 0x0019


/** GGC_SNCU_CFG desc:
  * Register default value:        0x000005C0
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/GGC_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.GGC_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.GGC_0_0_0_PCI
  * Security PolicyGroup: SNCU_BIOS_LT_W_PG
  * All the bits in this register are Intel TXT lockable.
  */

#define GGC_HOSTBRIDGE_CFG_REG                 (0x00000050U)

typedef union {
  struct {
    UINT16 ggclck : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               When set to 1b, this bit will lock all bits
                               in this register.
                            */

    UINT16 ivd : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               0: Enable.  Device 2 (IGD) claims VGA memory
                               and IO cycles, the Sub-Class Code within Device
                               2 Class Code register is 00. 1: Disable.
                               Device 2 (IGD) does not claim VGA cycles (Mem
                               and IO), and the Sub- Class Code field within
                               Device 2 function 0 Class Code register is
                               80. BIOS Requirement:  BIOS must not set this
                               bit to 0 if the GMS field (bits 7:3 of this
                               register) pre-allocates no memory. This bit
                               MUST be set to 1 if Device 2 is disabled either
                               via a fuse or fuse override (CAPID0_A[IGD]
                               = 1) or via a register (DEVEN[3] = 0). This
                               register is locked by Intel TXT lock.
                            */

    UINT16 vamen : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               Enables the use of the iGFX enbines for Versatile
                               Acceleration. 1 - iGFX engines are in Versatile
                               Acceleration Mode.  Device 2 Class Code is
                               048000h. 0 - iGFX engines are in iGFX Mode.
                               Device 2 Class Code is 030000h.
                            */

    UINT16 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[05:03] */

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */

    UINT16 ggms : 2;

                            /* Bits[7:6], Access Type=RO, default=0x00000003*/

                            /*
                               This field is used to select the amount of
                               Main Memory that is pre-allocated to support
                               the Internal Graphics Translation Table.
                               The BIOS ensures that memory is pre-allocated
                               only when Internal graphics is enabled. GSM
                               is assumed to be a contiguous physical DRAM
                               space with DSM, and BIOS needs to allocate
                               a contiguous memory chunk.  Hardware will
                               derive the base of GSM from DSM only using
                               the GSM size programmed in the register. Hardware
                               functionality in case of programming this
                               value to Reserved is not guaranteed.
                            */

    UINT16 gms : 8;

                            /* Bits[15:8], Access Type=RW/L, default=0x00000005*/

                            /*
                               This field is used to select the amount of
                               Main Memory that is pre-allocated to support
                               the Internal Graphics device in VGA (non-linear)
                               and Native (linear) modes.  The BIOS ensures
                               that memory is pre-allocated only when Internal
                               graphics is enabled. This register is also
                               Intel TXT lockable. Hardware does not clear
                               or set any of these bits automatically based
                               on IGD being disabled/enabled. BIOS Requirement:
                               BIOS must not set this field to 0h if IVD
                               (bit 1 of this register) is 0.
                            */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} GGC_HOSTBRIDGE_CFG_STRUCT;

#define GGC_HOSTBRIDGE_CFG_WIDTH 16

#define GGC_HOSTBRIDGE_CFG_GGCLCK_LSB 0x0000
#define GGC_HOSTBRIDGE_CFG_GGCLCK_SIZE 0x0001
#define GGC_HOSTBRIDGE_CFG_IVD_LSB 0x0001
#define GGC_HOSTBRIDGE_CFG_IVD_SIZE 0x0001
#define GGC_HOSTBRIDGE_CFG_VAMEN_LSB 0x0002
#define GGC_HOSTBRIDGE_CFG_VAMEN_SIZE 0x0001
#define GGC_HOSTBRIDGE_CFG_GGMS_LSB 0x0006
#define GGC_HOSTBRIDGE_CFG_GGMS_SIZE 0x0002
#define GGC_HOSTBRIDGE_CFG_GMS_LSB 0x0008
#define GGC_HOSTBRIDGE_CFG_GMS_SIZE 0x0008


/** DEVEN_SNCU_CFG desc:
  * Register default value:        0x00005C35
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/DEVEN_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.DEVEN_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.DEVEN_0_0_0_PCI
  * Security PolicyGroup: SNCU_LT_SA_W_PG
  * Allows for enabling/disabling of PCI devices and functions that are within the CPU package. The table below the bit definitions describes the behavior of all combinations of transactions to devices controlled by this register. All the bits in this register are Intel TXT Lockable.
  */

#define DEVEN_HOSTBRIDGE_CFG_REG               (0x00000054U)

typedef union {
  struct {
    UINT32 root_complex_en : 1; /**< Root Complex Enable */

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               Bus 0 Device 0 Function 0 may not be disabled
                               and is therefore hardwired to 1.
                            */

    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 ig_en : 1; /**< Internal Graphics Enable */

                            /* Bits[2:2], Access Type=RW/L, default=0x00000001*/

                            /*
                               Internal Graphic - enable, Managed by BIOS.
                               0: Internal Graphic is disabled and hidden
                               1: Internal Graphic is enabled and visible
                               This bit will be set to 0b and remain 0b if
                               Internal Graphic capability is disabled.
                            */

    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[03:03] */

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 cd_en : 1; /**< Camarillo Enable */

                            /* Bits[4:4], Access Type=RW/L, default=0x00000001*/

                            /*
                               Camarillo Device enable 0: Camarillo is disabled
                               and not visible. 1: Camarillo is enabled and
                               visible. This bit will be set to 0b and remain
                               0b if Camarillo capability is disabled
                            */

    UINT32 imgu_en : 1; /**< IMGU Enable */

                            /* Bits[5:5], Access Type=RW/L, default=0x00000001*/

                            /*
                               IMGU enable 0: IMGU is disabled and not visible.
                               1: IMGU is enabled and visible. This bit will
                               be set to 0b and remain 0b if IMPGU capability
                               is disabled.
                            */

    UINT32 rsvd_2 : 4; /**< Undefined - auto filled rsvd_[09:06] */

                            /* Bits[9:6], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 crashlog_en : 1; /**< Crashlog Enable */

                            /* Bits[10:10], Access Type=RW/L, default=0x00000001*/

                            /*
                               Crashlog Device enable 0: Crashlog is disabled
                               and not visible. 1: Crashlog is enabled and
                               visible.
                            */

    UINT32 npu_en : 1; /**< NPU Enable */

                            /* Bits[11:11], Access Type=RW/L, default=0x00000001*/

                            /*
                               NPU - Device enable 0: Device disabled and
                               hidden 1: Device enabled and visible
                            */

    UINT32 iax_en : 1; /**< IAX Enable */

                            /* Bits[12:12], Access Type=RW/L, default=0x00000001*/

                            /*
                               IAX - Device enable  0: Device disabled and
                               hidden 1: Device enabled and visible
                            */

    UINT32 rsvd_3 : 1; /**< Undefined - auto filled rsvd_[13:13] */

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 vmd_en : 1; /**< VMD Enable */

                            /* Bits[14:14], Access Type=RW/L, default=0x00000001*/

                            /*
                               VMD Enable -  0: VMD Device disabled and hidden
                               1: VMD Device enabled and visible
                            */

    UINT32 rsvd_4 : 17; /**< Undefined - auto filled rsvd_[31:15] */

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEVEN_HOSTBRIDGE_CFG_STRUCT;

#define DEVEN_HOSTBRIDGE_CFG_WIDTH 32

#define DEVEN_HOSTBRIDGE_CFG_ROOT_COMPLEX_EN_LSB 0x0000
#define DEVEN_HOSTBRIDGE_CFG_ROOT_COMPLEX_EN_SIZE 0x0001
#define DEVEN_HOSTBRIDGE_CFG_IG_EN_LSB 0x0002
#define DEVEN_HOSTBRIDGE_CFG_IG_EN_SIZE 0x0001
#define DEVEN_HOSTBRIDGE_CFG_CD_EN_LSB 0x0004
#define DEVEN_HOSTBRIDGE_CFG_CD_EN_SIZE 0x0001
#define DEVEN_HOSTBRIDGE_CFG_IMGU_EN_LSB 0x0005
#define DEVEN_HOSTBRIDGE_CFG_IMGU_EN_SIZE 0x0001
#define DEVEN_HOSTBRIDGE_CFG_CRASHLOG_EN_LSB 0x000a
#define DEVEN_HOSTBRIDGE_CFG_CRASHLOG_EN_SIZE 0x0001
#define DEVEN_HOSTBRIDGE_CFG_NPU_EN_LSB 0x000b
#define DEVEN_HOSTBRIDGE_CFG_NPU_EN_SIZE 0x0001
#define DEVEN_HOSTBRIDGE_CFG_IAX_EN_LSB 0x000c
#define DEVEN_HOSTBRIDGE_CFG_IAX_EN_SIZE 0x0001
#define DEVEN_HOSTBRIDGE_CFG_VMD_EN_LSB 0x000e
#define DEVEN_HOSTBRIDGE_CFG_VMD_EN_SIZE 0x0001


/** PAVPC_SNCU_CFG desc:
  * Register default value:        0x00000001
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PAVPC_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PAVPC_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PAVPC_0_0_0_PCI
  * Security PolicyGroup: SNCU_BIOS_LT_W_PG
  * All the bits in this register are locked by Intel TXT.  When locked the R/W bits are RO.
  */

#define PAVPC_HOSTBRIDGE_CFG_REG               (0x00000058U)

typedef union {
  struct {
    UINT32 pcme : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000001*/

                            /*
                               This field enables Protected Content Memory
                               within Graphics Stolen Memory. This memory
                               is the same           as the WOPCM area, whose
                               size is defined by bit 5 of this register.
                               This register is locked when PAVPLOCK is
                               set. A value of 0 in this field indicates
                               that Protected Content Memory is disabled,
                               and cannot be programmed           in this
                               manner  when PAVP is enabled. A value of 1
                               in this field indicates that Protected Content
                               Memory is           enabled, and is the only
                               programming option available when PAVP is
                               enabled. (Note that the legacy Lite
                               mode programming of PCME bit = 0 is not supported.
                               For non-PAVP3 Mode, even for Lite mode configuration,
                               this           bit should be programmed to
                               1 and HVYMODESEL = 0). This bit should always
                               be programmed to 1 if bits 1 and 2
                               (PAVPE and PAVP lock bits) are both set. With
                               per-App Memory configuration support, the
                               range check for the           WOPCM memory
                               area should always happen when this bit is
                               set, regardless of Lite or Serpent mode, or
                               PAVP2 or           PAVP3 mode programming.
                            */

    UINT32 pavpe : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               0: PAVP functionality is disabled.
                               1: PAVP functionality is enabled.
                               This register is locked when PAVPLCK is set.
                            */

    UINT32 pavplck : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit locks all writeable contents in this
                               register when set (including itself). Only
                               a hardware           reset can unlock the
                               register again. This lock bit needs to be
                               set only if PAVP is enabled (bit 1 of this
                               register is asserted).
                            */

    UINT32 hvymodsel : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit is applicable only for PAVP2 operation
                               mode with a chicken bit also set, or for PAVP3
                               mode           only if the per-App memory
                               config is disabled due to the clearing of
                               an additional chicken bit 9 in the Crypto
                               Function Control_1 register (address 0x320F0).
                               0: Lite Mode (Non-Serpent mode)
                               1: Serpent Mode           For chicken-bit
                               enabled PAVP3 mode, this one type boot time
                               programming has been replaced by per-App
                               programming (through the Media Crypto Copy
                               command). Note that PAVP2 or PAVP3 mode selection
                               is done by           programming bit 8 of
                               the MFX_MODE - Video Mode register.
                            */

    UINT32 ovtattack : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Override of Unsolicited Connection State Attack
                               and Terminate.           0: Disable Override.
                               Attack Terminate allowed.           1: Enable
                               Override. Attack Terminate disallowed.
                               This register bit is locked when PAVPE is
                               set.
                            */

    UINT32 rsvd1 : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /* These bits are reserved for future use. */

    UINT32 asmfen : 1;

                            /* Bits[6:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               ASMF method enabled 0b Disabled (default).
                               1b Enabled. This register is locked when PAVPLCK
                               is set.
                            */

    UINT32 rsvd2 : 13;

                            /* Bits[19:7], Access Type=RW/L, default=0x00000000*/

                            /* These bits are reserved for future use. */

    UINT32 pcmbase : 12;

                            /* Bits[31:20], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sizes supported: 1M, 2M, 4M and 8M. Base value
                               programmed (from Top of Stolen
                               Memory) itself defines the size of the WOPCM.
                               Separate WOPCM size programming is
                               redundant information and not required. Default
                               1M size programming. 4M recommended.
                               This register is locked (becomes read-only)
                               when PAVPE = 1b.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PAVPC_HOSTBRIDGE_CFG_STRUCT;

#define PAVPC_HOSTBRIDGE_CFG_WIDTH 32

#define PAVPC_HOSTBRIDGE_CFG_PCME_LSB 0x0000
#define PAVPC_HOSTBRIDGE_CFG_PCME_SIZE 0x0001
#define PAVPC_HOSTBRIDGE_CFG_PAVPE_LSB 0x0001
#define PAVPC_HOSTBRIDGE_CFG_PAVPE_SIZE 0x0001
#define PAVPC_HOSTBRIDGE_CFG_PAVPLCK_LSB 0x0002
#define PAVPC_HOSTBRIDGE_CFG_PAVPLCK_SIZE 0x0001
#define PAVPC_HOSTBRIDGE_CFG_HVYMODSEL_LSB 0x0003
#define PAVPC_HOSTBRIDGE_CFG_HVYMODSEL_SIZE 0x0001
#define PAVPC_HOSTBRIDGE_CFG_OVTATTACK_LSB 0x0004
#define PAVPC_HOSTBRIDGE_CFG_OVTATTACK_SIZE 0x0001
#define PAVPC_HOSTBRIDGE_CFG_RSVD1_LSB 0x0005
#define PAVPC_HOSTBRIDGE_CFG_RSVD1_SIZE 0x0001
#define PAVPC_HOSTBRIDGE_CFG_ASMFEN_LSB 0x0006
#define PAVPC_HOSTBRIDGE_CFG_ASMFEN_SIZE 0x0001
#define PAVPC_HOSTBRIDGE_CFG_RSVD2_LSB 0x0007
#define PAVPC_HOSTBRIDGE_CFG_RSVD2_SIZE 0x000d
#define PAVPC_HOSTBRIDGE_CFG_PCMBASE_LSB 0x0014
#define PAVPC_HOSTBRIDGE_CFG_PCMBASE_SIZE 0x000c


/** DPR_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/DPR_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.DPR_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.DPR_0_0_0_PCI
  * Security PolicyGroup: SNCU_LT_SA_W_PG
  * DMA protected range register.
  */

#define DPR_HOSTBRIDGE_CFG_REG                 (0x0000005cU)

typedef union {
  struct {
    UINT32 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               All bits which may be updated by SW in this
                               register are locked down when this bit is
                               set.
                            */

    UINT32 prs : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field indicates the status of DPR. 0:
                               DPR protection disabled 1: DPR protection
                               enabled
                            */

    UINT32 epm : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls DMA accesses to the DMA
                               Protected Range (DPR) region. 0: DPR is disabled
                               1: DPR is enabled.  All DMA requests accessing
                               DPR region are blocked. HW reports the status
                               of DPR enable/disable through the PRS field
                               in this register. When this bit change, one
                               must have to wait till status (prs) has updated
                               before changing it again.
                            */

    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

    UINT32 dprsize : 8;

                            /* Bits[11:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               This is the size of memory, in MB, that will
                               be protected from DMA accesses.  A value of
                               0x00 in this field means no additional memory
                               is protected. The maximum amount of memory
                               that will be protected is 255 MB. The amount
                               of memory reported in this field will be protected
                               from all DMA accesses, including translated
                               CPU accesses and graphics. The top of the
                               protected range is the BASE of TSEG -1. Note:
                               If TSEG is not enabled, then the top of this
                               range becomes the base of stolen graphics,
                               or ME stolen space or TOLUD, whichever would
                               have been the location of TSEG, assuming it
                               had been enabled. The DPR range works independently
                               of any other range, including the NoDMA.TABLE
                               protection or the PMRC checks in VTd, and
                               is done post any VTd translation or Intel
                               TXT NoDMA lookup.  Therefore incoming cycles
                               are checked against this range after the VTd
                               translation and faulted if they hit this protected
                               range, even if they passed the VTd translation
                               or were clean in the NoDMA lookup. All the
                               memory checks are ORed with respect to NOT
                               being allowed to go to memory.  So if either
                               PMRC, DPR, NoDMA table lookup, NoDMA.TABLE.PROTECT
                               OR a VTd translation disallows the cycle,
                               then the cycle is not allowed to go to memory.
                               Or in other words, all the above checks must
                               pass before a cycle is allowed to DRAM.
                            */

    UINT32 rsvd_0 : 8; /**< Undefined - auto filled rsvd_[19:12] */

                            /* Bits[19:12], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 topofdpr : 12;

                            /* Bits[31:20], Access Type=RW/L, default=0x00000000*/

                            /*
                               Top address + 1 of DPR.  This is the base
                               of TSEG.  Bits 19:0 of the BASE reported here
                               are 0x0_0000.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DPR_HOSTBRIDGE_CFG_STRUCT;

#define DPR_HOSTBRIDGE_CFG_WIDTH 32

#define DPR_HOSTBRIDGE_CFG_LOCK_LSB 0x0000
#define DPR_HOSTBRIDGE_CFG_LOCK_SIZE 0x0001
#define DPR_HOSTBRIDGE_CFG_PRS_LSB 0x0001
#define DPR_HOSTBRIDGE_CFG_PRS_SIZE 0x0001
#define DPR_HOSTBRIDGE_CFG_EPM_LSB 0x0002
#define DPR_HOSTBRIDGE_CFG_EPM_SIZE 0x0001
#define DPR_HOSTBRIDGE_CFG_RSVD_LSB 0x0003
#define DPR_HOSTBRIDGE_CFG_RSVD_SIZE 0x0001
#define DPR_HOSTBRIDGE_CFG_DPRSIZE_LSB 0x0004
#define DPR_HOSTBRIDGE_CFG_DPRSIZE_SIZE 0x0008
#define DPR_HOSTBRIDGE_CFG_TOPOFDPR_LSB 0x0014
#define DPR_HOSTBRIDGE_CFG_TOPOFDPR_SIZE 0x000c


/** PCIEXBAR_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PCIEXBAR_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PCIEXBAR_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PCIEXBAR_0_0_0_PCI
  * Security PolicyGroup: SNCU_LT_SA_W_PG
  * This register define the PCIEXBAR.
  */

#define PCIEXBAR_HOSTBRIDGE_CFG_REG            (0x00000060U)

typedef union {
  struct {
    UINT64 pciexbaren : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* PCIEX BAR Enable */

    UINT64 length : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000000*/

                            /*
                               This field describes the length of this region.
                               [br]000: 256MB (buses 0-255).  Bits 38:28
                               are decoded in the PCI Express Base Address
                               Field.           [br]001: 128MB (buses 0-127).
                               Bits 38:27 are decoded in the PCI Express
                               Base Address Field.           [br]010: 64MB
                               (buses 0-63).  Bits 38:26 are decoded in the
                               PCI Express Base Address Field.
                               [br]011: 512MB (buses 0-512). Bits 38:29 are
                               decoded in the PCI Express Base Address Field.
                               [br]100: 1024MB (buses 0-1024). Bits 38:30
                               are decoded in the PCI Express Base Address
                               Field.           [br]101: 2048MB (buses 0-2048).
                               Bits 38:31 are decoded in the PCI Express
                               Base Address Field.           [br]110: 4096MB
                               (buses 0-4096). Bits 38:32 are decoded in
                               the PCI Express Base Address Field.
                               [br]111:Rreserved.
                            */

    UINT64 rsvd_0 : 22; /**< Undefined - auto filled rsvd_[25:04] */

                            /* Bits[25:4], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 admsk64 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               This bit is either part of the PCI Express
                               Base Address (R/W) or part of the Address
                               Mask (RO, read 0b), depending on the value
                               of bits [3:1] in this register.
                            */

    UINT64 admsk128 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               This bit is either part of the PCI Express
                               Base Address (R/W) or part of the Address
                               Mask (RO, read 0b), depending on the value
                               of bits [3:1] in this register.
                            */

    UINT64 admsk256 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               This bit is either part of the PCI Express
                               Base Address (R/W) or part of the Address
                               Mask (RO, read 0b), depending on the value
                               of bits [3:1] in this register.
                            */

    UINT64 admsk512 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               This bit is either part of the PCI Express
                               Base Address (R/W) or part of the Address
                               Mask (RO, read 0b), depending on the value
                               of bits [3:1] in this register.
                            */

    UINT32 admsk1024 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               This bit is either part of the PCI Express
                               Base Address (R/W) or part of the Address
                               Mask (RO, read 0b), depending on the value
                               of bits [3:1] in this register.
                            */

    UINT64 pciexbar : 11;

                            /* Bits[41:31], Access Type=RW, default=0x00000000*/

                            /*
                               This field corresponds to bits 41 to 32 of
                               the base address for PCI Express enhanced
                               configuration space including bus segments.
                               BIOS will program this register resulting
                               in a base address for a contiguous memory
                               address space.  The size of the range is defined
                               by bits [3:1] of this register. This Base
                               address shall be assigned on a boundary consistent
                               with the number of buses (defined by the Length
                               field in this register) above TOLUD and still
                               within the 39-bit addressable memory space.
                               The address bits decoded depend on the length
                               of the region defined by this register.  The
                               address used to access the PCI Express configuration
                               space for a specific device can be determined
                               as follows: [br]PCI Express Base Address +Segment
                               Number*256MB+ Bus Number * 1MB + Device Number
                               * 32KB + Function Number * 4KB [br]This address
                               is the beginning of the 4KB space that contains
                               both the PCI compatible configuration space
                               and the PCI Express extended configuration
                               space.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PCIEXBAR_HOSTBRIDGE_CFG_STRUCT;

#define PCIEXBAR_HOSTBRIDGE_CFG_WIDTH 64

#define PCIEXBAR_HOSTBRIDGE_CFG_PCIEXBAREN_LSB 0x0000
#define PCIEXBAR_HOSTBRIDGE_CFG_PCIEXBAREN_SIZE 0x0001
#define PCIEXBAR_HOSTBRIDGE_CFG_LENGTH_LSB 0x0001
#define PCIEXBAR_HOSTBRIDGE_CFG_LENGTH_SIZE 0x0003
#define PCIEXBAR_HOSTBRIDGE_CFG_ADMSK64_LSB 0x001a
#define PCIEXBAR_HOSTBRIDGE_CFG_ADMSK64_SIZE 0x0001
#define PCIEXBAR_HOSTBRIDGE_CFG_ADMSK128_LSB 0x001b
#define PCIEXBAR_HOSTBRIDGE_CFG_ADMSK128_SIZE 0x0001
#define PCIEXBAR_HOSTBRIDGE_CFG_ADMSK256_LSB 0x001c
#define PCIEXBAR_HOSTBRIDGE_CFG_ADMSK256_SIZE 0x0001
#define PCIEXBAR_HOSTBRIDGE_CFG_ADMSK512_LSB 0x001d
#define PCIEXBAR_HOSTBRIDGE_CFG_ADMSK512_SIZE 0x0001
#define PCIEXBAR_HOSTBRIDGE_CFG_ADMSK1024_LSB 0x001e
#define PCIEXBAR_HOSTBRIDGE_CFG_ADMSK1024_SIZE 0x0001
#define PCIEXBAR_HOSTBRIDGE_CFG_PCIEXBAR_LSB 0x001f
#define PCIEXBAR_HOSTBRIDGE_CFG_PCIEXBAR_SIZE 0x000b


/** SAFBAR_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/SAFBAR_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.SAFBAR_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.SAFBAR_0_0_0_PCI
  * Security PolicyGroup: SNCU_LT_SA_W_PG
  * This is the base address for the SAFBAR space to generate SB messages to the CFI hosts.
  */

#define SAFBAR_HOSTBRIDGE_CFG_REG              (0x00000068U)

typedef union {
  struct {
    UINT64 safbaren : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               0: SAFBAR is disabled and does not claim any
                               memory 1: SAFBAR memory mapped accesses are
                               claimed and decoded appropriately
                            */

    UINT64 rsvd_0 : 24; /**< Undefined - auto filled rsvd_[24:01] */

                            /* Bits[24:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 safbar : 17;

                            /* Bits[41:25], Access Type=RW, default=0x00000000*/

                            /*
                               This field corresponds to bits x to 25 of
                               the base address SAFBAR configuration space.
                               BIOS will program this register resulting
                               in a base address for a 32MB block of contiguous
                               memory address space. This register ensures
                               that a naturally aligned 32MB space is allocated
                               within the first 4TB of addressable memory
                               space. System Software uses this base address
                               to program the SAFBAR register set.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} SAFBAR_HOSTBRIDGE_CFG_STRUCT;

#define SAFBAR_HOSTBRIDGE_CFG_WIDTH 64

#define SAFBAR_HOSTBRIDGE_CFG_SAFBAREN_LSB 0x0000
#define SAFBAR_HOSTBRIDGE_CFG_SAFBAREN_SIZE 0x0001
#define SAFBAR_HOSTBRIDGE_CFG_SAFBAR_LSB 0x0019
#define SAFBAR_HOSTBRIDGE_CFG_SAFBAR_SIZE 0x0011


/** CAPID_DEV_CDIE_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/CAPID_DEV_CDIE_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.CAPID_DEV_CDIE_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.CAPID_DEV_CDIE_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Control of bits in this register are only required for customer visible SKU differentiation.
  */

#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_REG      (0x00000070U)

typedef union {
  struct {
    UINT32 root_complex_dis : 1; /**< ROOT Complex Disable */

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* This field is constant 0 */

    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 igd : 1; /**< Internal Graphics Disable */

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               Internal Graphics Disable (IGD): 0: There
                               is a graphics engine within this CPU. 1: There
                               is no graphics engine within this CPU.
                            */

    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[03:03] */

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 cdd : 1; /**< Camarillo Device Disable */

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Camarillo Device Disable (CDD): 0: Enable
                               Camerillo device 1: Disable Camerillo device
                            */

    UINT32 imgu_dis : 1; /**< IMGU Disable */

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates if IMGU (Dev5) is disabled: 0: IMGU
                               is Enabled 1: IMGU is Disabled
                            */

    UINT32 rsvd_2 : 4; /**< Undefined - auto filled rsvd_[09:06] */

                            /* Bits[9:6], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 crashlog_dis : 1; /**< CRASHLOG Disable */

                            /* Bits[10:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               0: Crashlog IP accessible. Device associated
                               memory spaces for Crashlog IP are accessible.
                               1: Crashlog IP inaccessible. Device associated
                               memory and IO spaces for Crashlog IP are disabled.
                            */

    UINT32 npu_dis : 1; /**< NPU Disable */

                            /* Bits[11:11], Access Type=RW/L, default=0x00000000*/

                            /* Disable NPU Disable =1 / Enable =0 */

    UINT32 iax_dis : 1; /**< IAX Disable */

                            /* Bits[12:12], Access Type=RW/L, default=0x00000000*/

                            /* Disable IAX Disable =1 / Enable =0 */

    UINT32 rsvd_3 : 1; /**< Undefined - auto filled rsvd_[13:13] */

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 vmd_dis : 1; /**< VMD Disable */

                            /* Bits[14:14], Access Type=RW/L, default=0x00000000*/

                            /*
                               VMD Enable - 0: VMD is enabled and visible.
                               1:VMD is disabled and hidden.
                            */

    UINT32 rsvd_4 : 17; /**< Undefined - auto filled rsvd_[31:15] */

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CAPID_DEV_CDIE_HOSTBRIDGE_CFG_STRUCT;

#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_WIDTH 32

#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_ROOT_COMPLEX_DIS_LSB 0x0000
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_ROOT_COMPLEX_DIS_SIZE 0x0001
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_IGD_LSB 0x0002
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_IGD_SIZE 0x0001
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_CDD_LSB 0x0004
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_CDD_SIZE 0x0001
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_IMGU_DIS_LSB 0x0005
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_IMGU_DIS_SIZE 0x0001
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_CRASHLOG_DIS_LSB 0x000a
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_CRASHLOG_DIS_SIZE 0x0001
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_NPU_DIS_LSB 0x000b
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_NPU_DIS_SIZE 0x0001
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_IAX_DIS_LSB 0x000c
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_IAX_DIS_SIZE 0x0001
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_VMD_DIS_LSB 0x000e
#define CAPID_DEV_CDIE_HOSTBRIDGE_CFG_VMD_DIS_SIZE 0x0001

#ifdef SV_HOOKS

/** PCIESEGMAP_SNCU_CFG desc:
  * Register default value:        0x00654321
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PCIESEGMAP_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PCIESEGMAP_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PCIESEGMAP_0_0_0_PCI
  * Security PolicyGroup: SNCU_LT_SA_W_PG
  * Associating each PCIe typeC root port with a segment number.
  */

#define PCIESEGMAP_HOSTBRIDGE_CFG_REG          (0x00000078U)

typedef union {
  struct {
    UINT32 pcie0seg : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000001*/

                            /*
                               PCIE0 SEGMENT - This field is programmed by
                               BIOS and indicates the segment# the PCIE TypeC
                               port is associated with.
                            */

    UINT32 pcie1seg : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000002*/

                            /*
                               PCIE1 SEGMENT - This field is programmed by
                               BIOS and indicates the segment# the PCIE TypeC
                               port is associated with.
                            */

    UINT32 pcie2seg : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000003*/

                            /*
                               PCIE2 SEGMENT - This field is programmed by
                               BIOS and indicates the segment# the PCIE TypeC
                               port is associated with.
                            */

    UINT32 pcie3seg : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000004*/

                            /*
                               PCIE3 SEGMENT - This field is programmed by
                               BIOS and indicates the segment# the PCIE TypeC
                               port is associated with.
                            */

    UINT32 pcie4seg : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000005*/

                            /*
                               PCIE4 SEGMENT - This field is programmed by
                               BIOS and indicates the segment# the PCIE TypeC
                               port is associated with.
                            */

    UINT32 pcie5seg : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000006*/

                            /*
                               PCIE5 SEGMENT - This field is programmed by
                               BIOS and indicates the segment# the PCIE TypeC
                               port is associated with.
                            */

    UINT32 pcie6seg : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               PCIE6 SEGMENT - This field is programmed by
                               BIOS and indicates the segment# the PCIE TypeC
                               port is associated with.
                            */

    UINT32 pcie7seg : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               PCIE7 SEGMENT - This field is programmed by
                               BIOS and indicates the segment# the PCIE TypeC
                               port is associated with.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIESEGMAP_HOSTBRIDGE_CFG_STRUCT;

#define PCIESEGMAP_HOSTBRIDGE_CFG_WIDTH 32

#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE0SEG_LSB 0x0000
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE0SEG_SIZE 0x0004
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE1SEG_LSB 0x0004
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE1SEG_SIZE 0x0004
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE2SEG_LSB 0x0008
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE2SEG_SIZE 0x0004
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE3SEG_LSB 0x000c
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE3SEG_SIZE 0x0004
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE4SEG_LSB 0x0010
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE4SEG_SIZE 0x0004
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE5SEG_LSB 0x0014
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE5SEG_SIZE 0x0004
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE6SEG_LSB 0x0018
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE6SEG_SIZE 0x0004
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE7SEG_LSB 0x001c
#define PCIESEGMAP_HOSTBRIDGE_CFG_PCIE7SEG_SIZE 0x0004

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PCIEFUNCMAP_SNCU_CFG desc:
  * Register default value:        0x02208688
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PCIEFUNCMAP_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PCIEFUNCMAP_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PCIEFUNCMAP_0_0_0_PCI
  * Security PolicyGroup: SNCU_LT_SA_W_PG
  * Associating each PEG port and each PCIe typeC root port with a function number.
  */

#define PCIEFUNCMAP_HOSTBRIDGE_CFG_REG         (0x0000007cU)

typedef union {
  struct {
    UINT32 pcie0func : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               PCIE0 FUNCTION - This field is programmed
                               by BIOS and indicates the function# the PCIE
                               TypeC port is associated with.
                            */

    UINT32 pcie1func : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000001*/

                            /*
                               PCIE1 FUNCTION - This field is programmed
                               by BIOS and indicates the function# the PCIE
                               TypeC port is associated with.
                            */

    UINT32 pcie2func : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000002*/

                            /*
                               PCIE2 FUNCTION - This field is programmed
                               by BIOS and indicates the function# the PCIE
                               TypeC port is associated with.
                            */

    UINT32 pcie3func : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000003*/

                            /*
                               PCIE3 FUNCTION - This field is programmed
                               by BIOS and indicates the function# the PCIE
                               TypeC port is associated with.
                            */

    UINT32 peg10func : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               PEG10 FUNCTION - This field is programmed
                               by BIOS and indicates the function# the PCIE
                               PEG10 port is associated with.
                            */

    UINT32 peg11func : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000001*/

                            /*
                               PEG11 FUNCTION - This field is programmed
                               by BIOS and indicates the function# the PCIE
                               PEG11 port is associated with.
                            */

    UINT32 peg60func : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /*
                               PEG60 FUNCTION - This field is programmed
                               by BIOS and indicates the function# the PCIE
                               PEG60 port is associated with.
                            */

    UINT32 peg61func : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000001*/

                            /*
                               PEG61 FUNCTION - This field is programmed
                               by BIOS and indicates the function# the PCIE
                               PEG61 port is associated with.
                            */

    UINT32 peg62func : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000002*/

                            /*
                               PEG62 FUNCTION - This field is programmed
                               by BIOS and indicates the function# the PCIE
                               PEG62 port is associated with.
                            */

    UINT32 rsvd_0 : 5; /**< Undefined - auto filled rsvd_[31:27] */

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIEFUNCMAP_HOSTBRIDGE_CFG_STRUCT;

#define PCIEFUNCMAP_HOSTBRIDGE_CFG_WIDTH 32

#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PCIE0FUNC_LSB 0x0000
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PCIE0FUNC_SIZE 0x0003
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PCIE1FUNC_LSB 0x0003
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PCIE1FUNC_SIZE 0x0003
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PCIE2FUNC_LSB 0x0006
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PCIE2FUNC_SIZE 0x0003
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PCIE3FUNC_LSB 0x0009
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PCIE3FUNC_SIZE 0x0003
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PEG10FUNC_LSB 0x000c
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PEG10FUNC_SIZE 0x0003
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PEG11FUNC_LSB 0x000f
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PEG11FUNC_SIZE 0x0003
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PEG60FUNC_LSB 0x0012
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PEG60FUNC_SIZE 0x0003
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PEG61FUNC_LSB 0x0015
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PEG61FUNC_SIZE 0x0003
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PEG62FUNC_LSB 0x0018
#define PCIEFUNCMAP_HOSTBRIDGE_CFG_PEG62FUNC_SIZE 0x0003

#endif      // SV_HOOKS


/** PAM0_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PAM0_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PAM0_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PAM0_0_0_0_PCI
  * Security PolicyGroup: SNCU_PAM_W_PG
  * This register controls the read, write and shadowing attributes of the BIOS range from F_0000h to F_FFFFh.  The Uncore allows programmable memory attributes on 13 legacy memory segments of various sizes in the 768KB to 1MB address range.  Seven Programmable Attribute Map (PAM) registers are used to support these features.  Cacheability of these areas is controlled via the MTRR register in the core. Two bits are used to specify memory attributes for each memory segment.  These bits apply to host accesses to the PAM areas.  These attributes are: RE - Read Enable.  When RE=1, the host read accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when RE=0, the host read accesses are directed to DMI. WE - Write Enable.  When WE=1, the host write accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when WE=0, the host read accesses are directed to DMI. The RE and WE attributes permit a memory segment to be Read Only, Write Only, Read/Write or Disabled.  For example, if a memory segment has RE=1 and WE=0, the segment is Read Only.
  */

#define PAM0_HOSTBRIDGE_CFG_REG                (0x00000080U)

typedef union {
  struct {
    UINT8 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               If this bit is set, all of the PAM* registers
                               are locked (cannot be written)
                            */

    UINT8 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[03:01] */

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT8 hienable : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0F_0000h to 0F_FFFFh. 00: DRAM Disabled.
                               All accesses are directed to DMI. 01: Read
                               Only.  All reads are sent to DRAM, all writes
                               are forwarded to DMI. 10: Write Only.  All
                               writes are sent to DRAM, all reads are serviced
                               by DMI. 11: Normal DRAM Operation.  All reads
                               and writes are serviced by DRAM.
                            */

    UINT8 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[07:06] */

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT8 Data;
} PAM0_HOSTBRIDGE_CFG_STRUCT;

#define PAM0_HOSTBRIDGE_CFG_WIDTH 8

#define PAM0_HOSTBRIDGE_CFG_LOCK_LSB 0x0000
#define PAM0_HOSTBRIDGE_CFG_LOCK_SIZE 0x0001
#define PAM0_HOSTBRIDGE_CFG_HIENABLE_LSB 0x0004
#define PAM0_HOSTBRIDGE_CFG_HIENABLE_SIZE 0x0002


/** PAM1_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PAM1_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PAM1_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PAM1_0_0_0_PCI
  * Security PolicyGroup: SNCU_PAM_W_PG
  * This register controls the read, write and shadowing attributes of the BIOS range from C_0000h to C_7FFFh.  The Uncore allows programmable memory attributes on 13 legacy memory segments of various sizes in the 768KB to 1MB address range.  Seven Programmable Attribute Map (PAM) registers are used to support these features.  Cacheability of these areas is controlled via the MTRR register in the core. Two bits are used to specify memory attributes for each memory segment.  These bits apply to host accesses to the PAM areas.  These attributes are: RE - Read Enable.  When RE=1, the host read accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when RE=0, the host read accesses are directed to DMI. WE - Write Enable.  When WE=1, the host write accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when WE=0, the host read accesses are directed to DMI. The RE and WE attributes permit a memory segment to be Read Only, Write Only, Read/Write or Disabled.  For example, if a memory segment has RE=1 and WE=0, the segment is Read Only.
  */

#define PAM1_HOSTBRIDGE_CFG_REG                (0x00000081U)

typedef union {
  struct {
    UINT8 loenable : 2;

                            /* Bits[1:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0C0000h to 0C3FFFh. 00: DRAM Disabled.  All
                               reads are sent to DRAM.  All writes are forwarded
                               to DMI. 01: Read Only.  All reads are sent
                               to DRAM.  All writes are forwarded to DMI.
                               10: Write Only.  All writes are sent to DRAM.
                               All reads are serviced by DMI. 11: Normal
                               DRAM Operation.  All reads and writes are
                               serviced by DRAM.
                            */

    UINT8 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[03:02] */

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */

    UINT8 hienable : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0C_4000h to 0C_7FFFh. 00: DRAM Disabled.
                               All accesses are directed to DMI. 01: Read
                               Only.  All reads are sent to DRAM, all writes
                               are forwarded to DMI. 10: Write Only.  All
                               writes are sent to DRAM, all reads are serviced
                               by DMI. 11: Normal DRAM Operation.  All reads
                               and writes are serviced by DRAM.
                            */

    UINT8 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[07:06] */

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT8 Data;
} PAM1_HOSTBRIDGE_CFG_STRUCT;

#define PAM1_HOSTBRIDGE_CFG_WIDTH 8

#define PAM1_HOSTBRIDGE_CFG_LOENABLE_LSB 0x0000
#define PAM1_HOSTBRIDGE_CFG_LOENABLE_SIZE 0x0002
#define PAM1_HOSTBRIDGE_CFG_HIENABLE_LSB 0x0004
#define PAM1_HOSTBRIDGE_CFG_HIENABLE_SIZE 0x0002


/** PAM2_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PAM2_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PAM2_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PAM2_0_0_0_PCI
  * Security PolicyGroup: SNCU_PAM_W_PG
  * This register controls the read, write and shadowing attributes of the BIOS range from C_8000h to C_FFFFh.  The Uncore allows programmable memory attributes on 13 legacy memory segments of various sizes in the 768KB to 1MB address range.  Seven Programmable Attribute Map (PAM) registers are used to support these features.  Cacheability of these areas is controlled via the MTRR register in the core. Two bits are used to specify memory attributes for each memory segment.  These bits apply to host accesses to the PAM areas.  These attributes are: RE - Read Enable.  When RE=1, the host read accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when RE=0, the host read accesses are directed to DMI. WE - Write Enable.  When WE=1, the host write accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when WE=0, the host read accesses are directed to DMI. The RE and WE attributes permit a memory segment to be Read Only, Write Only, Read/Write or Disabled.  For example, if a memory segment has RE=1 and WE=0, the segment is Read Only.
  */

#define PAM2_HOSTBRIDGE_CFG_REG                (0x00000082U)

typedef union {
  struct {
    UINT8 loenable : 2;

                            /* Bits[1:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0C8000h to 0CBFFFh. 00: DRAM Disabled.  All
                               reads are sent to DRAM.  All writes are forwarded
                               to DMI. 01: Read Only.  All reads are sent
                               to DRAM.  All writes are forwarded to DMI.
                               10: Write Only.  All writes are sent to DRAM.
                               All reads are serviced by DMI. 11: Normal
                               DRAM Operation.  All reads and writes are
                               serviced by DRAM.
                            */

    UINT8 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[03:02] */

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */

    UINT8 hienable : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0CC000h to 0CFFFFh. 00: DRAM Disabled.  All
                               accesses are directed to DMI. 01: Read Only.
                               All reads are sent to DRAM, all writes are
                               forwarded to DMI. 10: Write Only.  All writes
                               are sent to DRAM, all reads are serviced by
                               DMI. 11: Normal DRAM Operation.  All reads
                               and writes are serviced by DRAM.
                            */

    UINT8 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[07:06] */

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT8 Data;
} PAM2_HOSTBRIDGE_CFG_STRUCT;

#define PAM2_HOSTBRIDGE_CFG_WIDTH 8

#define PAM2_HOSTBRIDGE_CFG_LOENABLE_LSB 0x0000
#define PAM2_HOSTBRIDGE_CFG_LOENABLE_SIZE 0x0002
#define PAM2_HOSTBRIDGE_CFG_HIENABLE_LSB 0x0004
#define PAM2_HOSTBRIDGE_CFG_HIENABLE_SIZE 0x0002


/** PAM3_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PAM3_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PAM3_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PAM3_0_0_0_PCI
  * Security PolicyGroup: SNCU_PAM_W_PG
  * This register controls the read, write and shadowing attributes of the BIOS range from D0000h to D7FFFh.  The Uncore allows programmable memory attributes on 13 legacy memory segments of various sizes in the 768KB to 1MB address range.  Seven Programmable Attribute Map (PAM) registers are used to support these features.  Cacheability of these areas is controlled via the MTRR register in the core. Two bits are used to specify memory attributes for each memory segment.  These bits apply to host accesses to the PAM areas.  These attributes are: RE - Read Enable.  When RE=1, the host read accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when RE=0, the host read accesses are directed to DMI. WE - Write Enable.  When WE=1, the host write accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when WE=0, the host read accesses are directed to DMI. The RE and WE attributes permit a memory segment to be Read Only, Write Only, Read/Write or Disabled.  For example, if a memory segment has RE=1 and WE=0, the segment is Read Only.
  */

#define PAM3_HOSTBRIDGE_CFG_REG                (0x00000083U)

typedef union {
  struct {
    UINT8 loenable : 2;

                            /* Bits[1:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0D0000h to 0D3FFFh. 00: DRAM Disabled.  All
                               reads are sent to DRAM.  All writes are forwarded
                               to DMI. 01: Read Only.  All reads are sent
                               to DRAM.  All writes are forwarded to DMI.
                               10: Write Only.  All writes are sent to DRAM.
                               All reads are serviced by DMI. 11: Normal
                               DRAM Operation.  All reads and writes are
                               serviced by DRAM.
                            */

    UINT8 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[03:02] */

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */

    UINT8 hienable : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0D4000h to 0D7FFFh. 00: DRAM Disabled.  All
                               accesses are directed to DMI. 01: Read Only.
                               All reads are sent to DRAM, all writes are
                               forwarded to DMI. 10: Write Only.  All writes
                               are sent to DRAM, all reads are serviced by
                               DMI. 11: Normal DRAM Operation.  All reads
                               and writes are serviced by DRAM.
                            */

    UINT8 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[07:06] */

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT8 Data;
} PAM3_HOSTBRIDGE_CFG_STRUCT;

#define PAM3_HOSTBRIDGE_CFG_WIDTH 8

#define PAM3_HOSTBRIDGE_CFG_LOENABLE_LSB 0x0000
#define PAM3_HOSTBRIDGE_CFG_LOENABLE_SIZE 0x0002
#define PAM3_HOSTBRIDGE_CFG_HIENABLE_LSB 0x0004
#define PAM3_HOSTBRIDGE_CFG_HIENABLE_SIZE 0x0002


/** PAM4_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PAM4_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PAM4_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PAM4_0_0_0_PCI
  * Security PolicyGroup: SNCU_PAM_W_PG
  * This register controls the read, write and shadowing attributes of the BIOS range from D8000h to DFFFFh.  The Uncore allows programmable memory attributes on 13 legacy memory segments of various sizes in the 768KB to 1MB address range.  Seven Programmable Attribute Map (PAM) registers are used to support these features.  Cacheability of these areas is controlled via the MTRR register in the core. Two bits are used to specify memory attributes for each memory segment.  These bits apply to host accesses to the PAM areas.  These attributes are: RE - Read Enable.  When RE=1, the host read accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when RE=0, the host read accesses are directed to DMI. WE - Write Enable.  When WE=1, the host write accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when WE=0, the host read accesses are directed to DMI. The RE and WE attributes permit a memory segment to be Read Only, Write Only, Read/Write or Disabled.  For example, if a memory segment has RE=1 and WE=0, the segment is Read Only.
  */

#define PAM4_HOSTBRIDGE_CFG_REG                (0x00000084U)

typedef union {
  struct {
    UINT8 loenable : 2;

                            /* Bits[1:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0D8000h to 0DBFFFh. 00: DRAM Disabled.  All
                               accesses are directed to DMI. 01: Read Only.
                               All reads are sent to DRAM.  All writes are
                               forwarded to DMI. 10: Write Only.  All writes
                               are sent to DRAM.  All reads are serviced
                               by DMI. 11: Normal DRAM Operation.  All reads
                               and writes are serviced by DRAM.
                            */

    UINT8 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[03:02] */

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */

    UINT8 hienable : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0DC000h to 0DFFFFh. 00: DRAM Disabled.  All
                               accesses are directed to DMI. 01: Read Only.
                               All reads are sent to DRAM, all writes are
                               forwarded to DMI. 10: Write Only.  All writes
                               are sent to DRAM, all reads are serviced by
                               DMI. 11: Normal DRAM Operation.  All reads
                               and writes are serviced by DRAM.
                            */

    UINT8 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[07:06] */

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT8 Data;
} PAM4_HOSTBRIDGE_CFG_STRUCT;

#define PAM4_HOSTBRIDGE_CFG_WIDTH 8

#define PAM4_HOSTBRIDGE_CFG_LOENABLE_LSB 0x0000
#define PAM4_HOSTBRIDGE_CFG_LOENABLE_SIZE 0x0002
#define PAM4_HOSTBRIDGE_CFG_HIENABLE_LSB 0x0004
#define PAM4_HOSTBRIDGE_CFG_HIENABLE_SIZE 0x0002


/** PAM5_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PAM5_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PAM5_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PAM5_0_0_0_PCI
  * Security PolicyGroup: SNCU_PAM_W_PG
  * This register controls the read, write and shadowing attributes of the BIOS range from E_0000h to E_7FFFh.  The Uncore allows programmable memory attributes on 13 legacy memory segments of various sizes in the 768KB to 1MB address range.  Seven Programmable Attribute Map (PAM) registers are used to support these features.  Cacheability of these areas is controlled via the MTRR register in the core. Two bits are used to specify memory attributes for each memory segment.  These bits apply to host accesses to the PAM areas.  These attributes are: RE - Read Enable.  When RE=1, the host read accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when RE=0, the host read accesses are directed to DMI. WE - Write Enable.  When WE=1, the host write accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when WE=0, the host read accesses are directed to DMI. The RE and WE attributes permit a memory segment to be Read Only, Write Only, Read/Write or Disabled.  For example, if a memory segment has RE=1 and WE=0, the segment is Read Only.
  */

#define PAM5_HOSTBRIDGE_CFG_REG                (0x00000085U)

typedef union {
  struct {
    UINT8 loenable : 2;

                            /* Bits[1:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0E0000h to 0E3FFFh. 00: DRAM Disabled.  All
                               reads are sent to DRAM.  All writes are forwarded
                               to DMI. 01: Read Only.  All reads are sent
                               to DRAM.  All writes are forwarded to DMI.
                               10: Write Only.  All writes are sent to DRAM.
                               All reads are serviced by DMI. 11: Normal
                               DRAM Operation.  All reads and writes are
                               serviced by DRAM.
                            */

    UINT8 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[03:02] */

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */

    UINT8 hienable : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0E4000h to 0E7FFFh. 00: DRAM Disabled.  All
                               accesses are directed to DMI. 01: Read Only.
                               All reads are sent to DRAM, all writes are
                               forwarded to DMI. 10: Write Only.  All writes
                               are sent to DRAM, all reads are serviced by
                               DMI. 11: Normal DRAM Operation.  All reads
                               and writes are serviced by DRAM.
                            */

    UINT8 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[07:06] */

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT8 Data;
} PAM5_HOSTBRIDGE_CFG_STRUCT;

#define PAM5_HOSTBRIDGE_CFG_WIDTH 8

#define PAM5_HOSTBRIDGE_CFG_LOENABLE_LSB 0x0000
#define PAM5_HOSTBRIDGE_CFG_LOENABLE_SIZE 0x0002
#define PAM5_HOSTBRIDGE_CFG_HIENABLE_LSB 0x0004
#define PAM5_HOSTBRIDGE_CFG_HIENABLE_SIZE 0x0002


/** PAM6_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/PAM6_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.PAM6_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.PAM6_0_0_0_PCI
  * Security PolicyGroup: SNCU_PAM_W_PG
  * This register controls the read, write and shadowing attributes of the BIOS range from E_8000h to E_FFFFh.  The Uncore allows programmable memory attributes on 13 legacy memory segments of various sizes in the 768KB to 1MB address range.  Seven Programmable Attribute Map (PAM) registers are used to support these features.  Cacheability of these areas is controlled via the MTRR register in the core. Two bits are used to specify memory attributes for each memory segment.  These bits apply to host accesses to the PAM areas.  These attributes are: RE - Read Enable.  When RE=1, the host read accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when RE=0, the host read accesses are directed to DMI. WE - Write Enable.  When WE=1, the host write accesses to the corresponding memory segment are claimed by the Uncore and directed to main memory.  Conversely, when WE=0, the host read accesses are directed to DMI. The RE and WE attributes permit a memory segment to be Read Only, Write Only, Read/Write or Disabled.  For example, if a memory segment has RE=1 and WE=0, the segment is Read Only.
  */

#define PAM6_HOSTBRIDGE_CFG_REG                (0x00000086U)

typedef union {
  struct {
    UINT8 loenable : 2;

                            /* Bits[1:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0E8000h to 0EBFFFh. 00: DRAM Disabled.  All
                               reads are sent to DRAM.  All writes are forwarded
                               to DMI. 01: Read Only.  All reads are sent
                               to DRAM.  All writes are forwarded to DMI.
                               10: Write Only.  All writes are sent to DRAM.
                               All reads are serviced by DMI. 11: Normal
                               DRAM Operation.  All reads and writes are
                               serviced by DRAM.
                            */

    UINT8 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[03:02] */

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */

    UINT8 hienable : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the steering of read and
                               write cycles that address the BIOS area from
                               0EC000h to 0EFFFFh. 00: DRAM Disabled.  All
                               accesses are directed to DMI. 01: Read Only.
                               All reads are sent to DRAM, all writes are
                               forwarded to DMI. 10: Write Only.  All writes
                               are sent to DRAM, all reads are serviced by
                               DMI. 11: Normal DRAM Operation.  All reads
                               and writes are serviced by DRAM.
                            */

    UINT8 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[07:06] */

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT8 Data;
} PAM6_HOSTBRIDGE_CFG_STRUCT;

#define PAM6_HOSTBRIDGE_CFG_WIDTH 8

#define PAM6_HOSTBRIDGE_CFG_LOENABLE_LSB 0x0000
#define PAM6_HOSTBRIDGE_CFG_LOENABLE_SIZE 0x0002
#define PAM6_HOSTBRIDGE_CFG_HIENABLE_LSB 0x0004
#define PAM6_HOSTBRIDGE_CFG_HIENABLE_SIZE 0x0002

#ifdef SV_HOOKS

/** LAC_SNCU_CFG desc:
  * Register default value:        0x00000010
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/LAC_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.LAC_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.LAC_0_0_0_PCI
  * Security PolicyGroup: SNCU_LT_SA_W_PG
  * This 8-bit register controls steering of MDA cycles and a fixed DRAM hole from 15-16MB. There can only be at most one MDA device in the system.
  */

#define LAC_HOSTBRIDGE_CFG_REG                 (0x00000087U)

typedef union {
  struct {
    UINT8 mdap10 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               This bit works with the VGA Enable bits in
                               the BCTRL register of Device 1 Function 0
                               to control the routing of CPU initiated transactions
                               targeting MDA compatible I/O and memory address
                               ranges. This bit should not be set if device
                               1 function 0 VGA Enable bit is not set. If
                               device 1 function 0 VGA enable bit is not
                               set, then accesses to IO address range x3BCh-x3BFh
                               remain on the backbone. If the VGA enable
                               bit is set and MDA is not present, then accesses
                               to IO address range x3BCh-x3BFh are forwarded
                               to PCI Express through device 1 function 0
                               if the address is within the corresponding
                               IOBASE and IOLIMIT, otherwise they remain
                               on the backbone. MDA resources are defined
                               as the following:         Memory: 0B0000h
                               - 0B7FFFh         I/O:    3B4h, 3B5h, 3B8h,
                               3B9h, 3BAh, 3BFh,                 (including
                               ISA address aliases, A[15:10] are not used
                               in decode) Any I/O reference that includes
                               the I/O locations listed above, or their aliases,
                               will remain on the backbone even if the reference
                               also includes I/O locations not listed above.
                               The following table shows the behavior for
                               all combinations of MDA and VGA:        VGAEN
                               MDAP     Description         0       0
                               All References to MDA and VGA space are not
                               claimed by Device 1 Function 0.         0
                               1       Illegal combination         1
                               0       All VGA and MDA references are routed
                               to PCI Express Graphics Attach device 1 function
                               0.         1       1       All VGA references
                               are routed to PCI Express Graphics Attach
                               device 1 function 0. MDA references are not
                               claimed by device 1 function 0. VGA and MDA
                               memory cycles can only be routed across PEG10
                               when MAE (PCICMD10[1]) is set. VGA and MDA
                               I/O cycles can only be routed across PEG10
                               if IOAE (PCICMD10[0]) is set.
                            */

    UINT8 mdap11 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit works with the VGA Enable bits in
                               the BCTRL register of Device 1 Function 1
                               to control the routing of CPU initiated transactions
                               targeting MDA compatible I/O and memory address
                               ranges. This bit should not be set if device
                               1 function 1 VGA Enable bit is not set. If
                               device 1 function 1 VGA enable bit is not
                               set, then accesses to IO address range x3BCh-x3BFh
                               remain on the backbone. If the VGA enable
                               bit is set and MDA is not present, then accesses
                               to IO address range x3BCh-x3BFh are forwarded
                               to PCI Express through device 1 function 1
                               if the address is within the corresponding
                               IOBASE and IOLIMIT, otherwise they remain
                               on the backbone. MDA resources are defined
                               as the following:         Memory: 0B0000h
                               - 0B7FFFh         I/O:    3B4h, 3B5h, 3B8h,
                               3B9h, 3BAh, 3BFh,                (including
                               ISA address aliases, A[15:10] are not used
                               in decode) Any I/O reference that includes
                               the I/O locations listed above, or their aliases,
                               will remain on the backbone even if the reference
                               also includes I/O locations not listed above.
                               The following table shows the behavior for
                               all combinations of MDA and VGA:        VGAEN
                               MDAP     Description         0       0
                               All References to MDA and VGA space are not
                               claimed by Device 1 Function 1.         0
                               1       Illegal combination         1
                               0       All VGA and MDA references are routed
                               to PCI Express Graphics Attach device 1 function
                               1.         1       1       All VGA references
                               are routed to PCI Express Graphics Attach
                               device 1 function 1. MDA references are not
                               claimed by device 1 function 1. VGA and MDA
                               memory cycles can only be routed across PEG11
                               when MAE (PCICMD11[1]) is set. VGA and MDA
                               I/O cycles can only be routed across PEG11
                               if IOAE (PCICMD11[0]) is set.
                            */

    UINT8 mdap12 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               This bit works with the VGA Enable bits in
                               the BCTRL register of Device 1 Function 2
                               to control the routing of CPU initiated transactions
                               targeting MDA compatible I/O and memory address
                               ranges. This bit should not be set if device
                               1 function 2 VGA Enable bit is not set. If
                               device 1 function 2 VGA enable bit is not
                               set, then accesses to IO address range x3BCh-x3BFh
                               remain on the backbone. If the VGA enable
                               bit is set and MDA is not present, then accesses
                               to IO address range x3BCh-x3BFh are forwarded
                               to PCI Express through device 1 function 2
                               if the address is within the corresponding
                               IOBASE and IOLIMIT, otherwise they remain
                               on the backbone. MDA resources are defined
                               as the following:         Memory: 0B0000h
                               - 0B7FFFh         I/O:    3B4h, 3B5h, 3B8h,
                               3B9h, 3BAh, 3BFh,                 (including
                               ISA address aliases, A[15:10] are not used
                               in decode) Any I/O reference that includes
                               the I/O locations listed above, or their aliases,
                               will remain on the backbone even if the reference
                               also includes I/O locations not listed above.
                               The following table shows the behavior for
                               all combinations of MDA and VGA:        VGAEN
                               MDAP     Description         0       0
                               All References to MDA and VGA space are not
                               claimed by Device 1 Function 2.         0
                               1       Illegal combination         1
                               0       All VGA and MDA references are routed
                               to PCI Express Graphics Attach device 1 function
                               2.         1       1       All VGA references
                               are routed to PCI Express Graphics Attach
                               device 1 function 2. MDA references are not
                               claimed by device 1 function 2. VGA and MDA
                               memory cycles can only be routed across PEG12
                               when MAE (PCICMD12[1]) is set. VGA and MDA
                               I/O cycles can only be routed across PEG12
                               if IOAE (PCICMD12[0]) is set.
                            */

    UINT8 mdap60 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               This bit works with the VGA Enable bits in
                               the BCTRL register of Device 1 Function 2
                               to control the routing of CPU initiated transactions
                               targeting MDA compatible I/O and memory address
                               ranges. This bit should not be set if device
                               1 function 2 VGA Enable bit is not set. If
                               device 1 function 2 VGA enable bit is not
                               set, then accesses to IO address range x3BCh-x3BFh
                               remain on the backbone. If the VGA enable
                               bit is set and MDA is not present, then accesses
                               to IO address range x3BCh-x3BFh are forwarded
                               to PCI Express through device 1 function 2
                               if the address is within the corresponding
                               IOBASE and IOLIMIT, otherwise they remain
                               on the backbone. MDA resources are defined
                               as the following:         Memory: 0B0000h
                               - 0B7FFFh         I/O:    3B4h, 3B5h, 3B8h,
                               3B9h, 3BAh, 3BFh,                 (including
                               ISA address aliases, A[15:10] are not used
                               in decode) Any I/O reference that includes
                               the I/O locations listed above, or their aliases,
                               will remain on the backbone even if the reference
                               also includes I/O locations not listed above.
                               The following table shows the behavior for
                               all combinations of MDA and VGA:        VGAEN
                               MDAP     Description         0       0
                               All References to MDA and VGA space are not
                               claimed by Device 1 Function 2.         0
                               1       Illegal combination         1
                               0       All VGA and MDA references are routed
                               to PCI Express Graphics Attach device 1 function
                               2.         1       1       All VGA references
                               are routed to PCI Express Graphics Attach
                               device 1 function 2. MDA references are not
                               claimed by device 1 function 2. VGA and MDA
                               memory cycles can only be routed across PEG12
                               when MAE (PCICMD12[1]) is set. VGA and MDA
                               I/O cycles can only be routed across PEG12
                               if IOAE (PCICMD12[0]) is set.
                            */

    UINT8 mdapcie : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               This bit works with the VGA Enable bits in
                               the BCTRL register of Non PEG devices to control
                               the routing of CPU initiated transactions
                               targeting MDA compatible I/O and memory address
                               ranges. This bit should be set to 1 by default.
                               It is assumed that these devices will not
                               need to support legacy MDA graphics. However
                               this single bit is added just to support this
                               rare case of using MDA over these devices.
                               The behavior of this bit field is identical
                               to bits [3:0]
                            */

    UINT8 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[06:05] */

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */

    UINT8 hen : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               This field enables a memory hole in DRAM space.
                               The DRAM that lies behind this space is not
                               remapped. 0: No memory hole. 1: Memory hole
                               from 15MB to 16MB. This bit is Intel TXT lockable.
                            */

  }     Bits;
  UINT8 Data;
} LAC_HOSTBRIDGE_CFG_STRUCT;

#define LAC_HOSTBRIDGE_CFG_WIDTH 8

#define LAC_HOSTBRIDGE_CFG_MDAP10_LSB 0x0000
#define LAC_HOSTBRIDGE_CFG_MDAP10_SIZE 0x0001
#define LAC_HOSTBRIDGE_CFG_MDAP11_LSB 0x0001
#define LAC_HOSTBRIDGE_CFG_MDAP11_SIZE 0x0001
#define LAC_HOSTBRIDGE_CFG_MDAP12_LSB 0x0002
#define LAC_HOSTBRIDGE_CFG_MDAP12_SIZE 0x0001
#define LAC_HOSTBRIDGE_CFG_MDAP60_LSB 0x0003
#define LAC_HOSTBRIDGE_CFG_MDAP60_SIZE 0x0001
#define LAC_HOSTBRIDGE_CFG_MDAPCIE_LSB 0x0004
#define LAC_HOSTBRIDGE_CFG_MDAPCIE_SIZE 0x0001
#define LAC_HOSTBRIDGE_CFG_HEN_LSB 0x0007
#define LAC_HOSTBRIDGE_CFG_HEN_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** LOCAL_HOME_SLICE_HASH_SNCU_CFG desc:
  * Register default value:        0x00082504
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/LOCAL_HOME_SLICE_HASH_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.LOCAL_HOME_SLICE_HASH_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.LOCAL_HOME_SLICE_HASH_0_0_0_PCI
  * Security PolicyGroup: SNCU_LT_SA_W_PG
  * Local Home Hash is done in order to split the memory space that is handled by the system, inorder to get more BW, more Snoop Filter capacity and more System Cache capacity.
  */

#define LOCAL_HOME_SLICE_HASH_HOSTBRIDGE_CFG_REG (0x0000008cU)

typedef union {
  struct {
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[00:00] */

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 hash_bit0_lsb : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000002*/

                            /*
                               LSB used in hashing between the two slices.
                               Encoding: 000 -address bit 6 001 -address
                               bit 7 ... 111 -address bit 13
                            */

    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[05:04] */

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 hash_bit0_mask : 14;

                            /* Bits[19:6], Access Type=RW, default=0x00002094*/

                            /*
                               Address bits in range of [19:6] used for hashing.
                               Hash_mask should match Hash_LSB encoding (i.e.
                               Hash_LSBis the lowest bit set in the mask)
                            */

    UINT32 rsvd_2 : 12; /**< Undefined - auto filled rsvd_[31:20] */

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LOCAL_HOME_SLICE_HASH_HOSTBRIDGE_CFG_STRUCT;

#define LOCAL_HOME_SLICE_HASH_HOSTBRIDGE_CFG_WIDTH 32

#define LOCAL_HOME_SLICE_HASH_HOSTBRIDGE_CFG_HASH_BIT0_LSB_LSB 0x0001
#define LOCAL_HOME_SLICE_HASH_HOSTBRIDGE_CFG_HASH_BIT0_LSB_SIZE 0x0003
#define LOCAL_HOME_SLICE_HASH_HOSTBRIDGE_CFG_HASH_BIT0_MASK_LSB 0x0006
#define LOCAL_HOME_SLICE_HASH_HOSTBRIDGE_CFG_HASH_BIT0_MASK_SIZE 0x000e

#endif      // SV_HOOKS


/** TOM_SNCU_CFG desc:
  * Register default value:        0x7FFFF00000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/TOM_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.TOM_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.TOM_0_0_0_PCI
  * Security PolicyGroup: SNCU_BIOS_LT_W_PG
  * This Register contains the size of physical memory.  BIOS determines the memory size reported to the OS using this Register.
  */

#define TOM_HOSTBRIDGE_CFG_REG                 (0x000000a0U)

typedef union {
  struct {
    UINT64 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT64 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[19:01] */

                            /* Bits[19:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 tom : 22;

                            /* Bits[41:20], Access Type=RW/L, default=0x0007FFFF*/

                            /*
                               This register reflects the total amount of
                               populated physical memory. This is NOT necessarily
                               the highest main memory address (holes may
                               exist in main memory address map due to addresses
                               allocated for memory mapped IO). These bits
                               correspond to address bits 41:20 (1MB granularity).
                               Bits 19:0 are assumed to be 0. All the bits
                               in this register are locked in Intel TXT mode.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} TOM_HOSTBRIDGE_CFG_STRUCT;

#define TOM_HOSTBRIDGE_CFG_WIDTH 64

#define TOM_HOSTBRIDGE_CFG_LOCK_LSB 0x0000
#define TOM_HOSTBRIDGE_CFG_LOCK_SIZE 0x0001
#define TOM_HOSTBRIDGE_CFG_TOM_LSB 0x0014
#define TOM_HOSTBRIDGE_CFG_TOM_SIZE 0x0016


/** TOUUD_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/TOUUD_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.TOUUD_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.TOUUD_0_0_0_PCI
  * Security PolicyGroup: SNCU_BIOS_LT_W_PG
  * This 64 bit register defines the Top of Upper Usable DRAM. Configuration software must set this value to TOM minus all ME stolen memory if reclaim is disabled.  If reclaim is enabled, this value must be set to reclaim limit + 1byte, 1MB aligned, since reclaim limit is 1MB aligned. Address bits 19:0 are assumed to be 000_0000h for the purposes of address comparison. The Host interface positively decodes an address towards DRAM if the incoming address is less than the value programmed in this register and greater than or equal to 4GB. BIOS Restriction: Minimum value for TOUUD is 4GB. These bits are Intel TXT lockable.
  */

#define TOUUD_HOSTBRIDGE_CFG_REG               (0x000000a8U)

typedef union {
  struct {
    UINT64 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT64 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[19:01] */

                            /* Bits[19:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 touud : 22;

                            /* Bits[41:20], Access Type=RW/L, default=0x00000000*/

                            /*
                               This register contains bits 41  to 20 of an
                               address one byte above the maximum DRAM memory
                               above 4G that is usable by the operating system.
                               Configuration software must set this value
                               to TOM minus all ME stolen memory if reclaim
                               is disabled. If reclaim is enabled, this value
                               must be set to reclaim limit 1MB aligned since
                               reclaim limit + 1byte is 1MB aligned. Address
                               bits 19:0 are assumed to be 000_0000h for
                               the purposes of address comparison. The Host
                               interface positively decodes an address towards
                               DRAM if the incoming address is less than
                               the value programmed in this register and
                               greater than 4GB. All the bits in this register
                               are locked in Intel TXT mode.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} TOUUD_HOSTBRIDGE_CFG_STRUCT;

#define TOUUD_HOSTBRIDGE_CFG_WIDTH 64

#define TOUUD_HOSTBRIDGE_CFG_LOCK_LSB 0x0000
#define TOUUD_HOSTBRIDGE_CFG_LOCK_SIZE 0x0001
#define TOUUD_HOSTBRIDGE_CFG_TOUUD_LSB 0x0014
#define TOUUD_HOSTBRIDGE_CFG_TOUUD_SIZE 0x0016


/** BDSM_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/BDSM_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.BDSM_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.BDSM_0_0_0_PCI
  * Security PolicyGroup: SNCU_BIOS_LT_W_PG
  * This register contains the base address of graphics data stolen DRAM memory. BIOS determines the base of graphics data stolen memory by subtracting the graphics data stolen memory size (PCI Device 0 offset 52 bits 7:4) from TOLUD (PCI Device 0 offset BC bits 31:20).
  */

#define BDSM_HOSTBRIDGE_CFG_REG                (0x000000b0U)

typedef union {
  struct {
    UINT32 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT32 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[19:01] */

                            /* Bits[19:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 bdsm : 12;

                            /* Bits[31:20], Access Type=RW/L, default=0x00000000*/

                            /*
                               This register contains bits 31 to 20 of the
                               base address of stolen DRAM memory. BIOS determines
                               the base of graphics stolen memory by subtracting
                               the graphics stolen memory size (PCI Device
                               0 offset 50 bits 15:8) from TOLUD (PCI Device
                               0 offset BC bits 31:20).
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} BDSM_HOSTBRIDGE_CFG_STRUCT;

#define BDSM_HOSTBRIDGE_CFG_WIDTH 32

#define BDSM_HOSTBRIDGE_CFG_LOCK_LSB 0x0000
#define BDSM_HOSTBRIDGE_CFG_LOCK_SIZE 0x0001
#define BDSM_HOSTBRIDGE_CFG_BDSM_LSB 0x0014
#define BDSM_HOSTBRIDGE_CFG_BDSM_SIZE 0x000c


/** BGSM_SNCU_CFG desc:
  * Register default value:        0x00100000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/BGSM_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.BGSM_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.BGSM_0_0_0_PCI
  * Security PolicyGroup: SNCU_BIOS_LT_W_PG
  * This register contains the base address of stolen DRAM memory for the GTT. BIOS determines the base of GTT stolen memory by subtracting the GTT graphics stolen memory size (PCI Device 0 offset 52 bits 9:8) from the Graphics Base of  Data Stolen Memory (PCI Device 0 offset B0 bits 31:20).
  */

#define BGSM_HOSTBRIDGE_CFG_REG                (0x000000b4U)

typedef union {
  struct {
    UINT32 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT32 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[19:01] */

                            /* Bits[19:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 bgsm : 12;

                            /* Bits[31:20], Access Type=RW/L, default=0x00000001*/

                            /*
                               This register contains the base address of
                               stolen DRAM memory for the GTT. BIOS determines
                               the base of GTT stolen memory by subtracting
                               the GTT graphics stolen memory size (PCI Device
                               0 offset 50 bits 7:6) from the Graphics Base
                               of  Data Stolen Memory (PCI Device 0 offset
                               B0 bits 31:20).
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} BGSM_HOSTBRIDGE_CFG_STRUCT;

#define BGSM_HOSTBRIDGE_CFG_WIDTH 32

#define BGSM_HOSTBRIDGE_CFG_LOCK_LSB 0x0000
#define BGSM_HOSTBRIDGE_CFG_LOCK_SIZE 0x0001
#define BGSM_HOSTBRIDGE_CFG_BGSM_LSB 0x0014
#define BGSM_HOSTBRIDGE_CFG_BGSM_SIZE 0x000c


/** TSEGMB_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/TSEGMB_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.TSEGMB_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.TSEGMB_0_0_0_PCI
  * Security PolicyGroup: SNCU_BIOS_LT_W_PG
  * This register contains the base address of TSEG DRAM memory. BIOS determines the base of TSEG memory which must be at or below Graphics Base of GTT Stolen Memory (PCI Device 0 Offset B4 bits 31:20). NOTE: BIOS must program TSEGMB to a 8MB naturally aligned boundary.
  */

#define TSEGMB_HOSTBRIDGE_CFG_REG              (0x000000b8U)

typedef union {
  struct {
    UINT32 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT32 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[19:01] */

                            /* Bits[19:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 tsegmb : 12;

                            /* Bits[31:20], Access Type=RW/L, default=0x00000000*/

                            /*
                               This register contains the base address of
                               TSEG DRAM memory.  BIOS determines the base
                               of TSEG memory which must be at or below Graphics
                               Base of GTT Stolen Memory (PCI Device 0 Offset
                               B4 bits 31:20).  BIOS must program the value
                               of TSEGMB to be the same as BGSM when TSEG
                               is disabled.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TSEGMB_HOSTBRIDGE_CFG_STRUCT;

#define TSEGMB_HOSTBRIDGE_CFG_WIDTH 32

#define TSEGMB_HOSTBRIDGE_CFG_LOCK_LSB 0x0000
#define TSEGMB_HOSTBRIDGE_CFG_LOCK_SIZE 0x0001
#define TSEGMB_HOSTBRIDGE_CFG_TSEGMB_LSB 0x0014
#define TSEGMB_HOSTBRIDGE_CFG_TSEGMB_SIZE 0x000c


/** TOLUD_SNCU_CFG desc:
  * Register default value:        0x00100000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/TOLUD_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.TOLUD_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.TOLUD_0_0_0_PCI
  * Security PolicyGroup: SNCU_BIOS_LT_W_PG
  * This 32 bit register defines the Top of Low Usable DRAM. TSEG, GTT Graphics memory and Graphics Stolen Memory are within the DRAM space defined. From the top, the Host optionally claims 1 to 64MBs of DRAM for internal graphics if enabled, 1or 2MB of DRAM for GTT Graphics Stolen Memory (if enabled) and 1, 2, or 8 MB of DRAM for TSEG if enabled.          Programming Example:          C1DRB3 is set to 4GB          TSEG is enabled and TSEG size is set to 1MB          Internal Graphics is enabled, and Graphics Mode Select is set to 32MB          GTT Graphics Stolen Memory Size set to 2MB          BIOS knows the OS requires 1G of PCI space.          BIOS also knows the range from 0_FEC0_0000h to 0_FFFF_FFFFh is not usable by the system. This 20MB range at the very top of addressable memory space is lost to APIC and Intel TXT.          According to the above equation, TOLUD is originally calculated to: 4GB = 1_0000_0000h          The system memory requirements are: 4GB (max addressable space) - 1GB (pci space) - 35MB (lost memory) = 3GB - 35MB (minimum granularity) = 0_ECB0_0000h          Since 0_ECB0_0000h (PCI and other system requirements) is less than 1_0000_0000h, TOLUD should be programmed to ECBh. These bits are Intel TXT lockable.
  */

#define TOLUD_HOSTBRIDGE_CFG_REG               (0x000000bcU)

typedef union {
  struct {
    UINT32 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT32 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[19:01] */

                            /* Bits[19:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 tolud : 12;

                            /* Bits[31:20], Access Type=RW/L, default=0x00000001*/

                            /*
                               This register contains bits 31 to 20 of an
                               address one byte above the maximum DRAM memory
                               below 4G that is usable by the operating system.
                               Address bits 31 down to 20 programmed to 01h
                               implies a minimum memory size of 1MB. Configuration
                               software must set this value to the smaller
                               of the following 2 choices: maximum amount
                               memory in the system minus ME stolen memory
                               plus one byte or the minimum address allocated
                               for PCI memory. Address bits 19:0 are assumed
                               to be 0_0000h for the purposes of address
                               comparison. The Host interface positively
                               decodes an address towards DRAM if the incoming
                               address is less than the value programmed
                               in this register. The Top of Low Usable DRAM
                               is the lowest address above both Graphics
                               Stolen memory and Tseg. BIOS determines the
                               base of Graphics Stolen Memory by subtracting
                               the Graphics Stolen Memory Size from TOLUD
                               and further decrements by Tseg size to determine
                               base of Tseg. All the Bits in this register
                               are locked in Intel TXT mode. This register
                               must be 1MB aligned when reclaim is enabled.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TOLUD_HOSTBRIDGE_CFG_STRUCT;

#define TOLUD_HOSTBRIDGE_CFG_WIDTH 32

#define TOLUD_HOSTBRIDGE_CFG_LOCK_LSB 0x0000
#define TOLUD_HOSTBRIDGE_CFG_LOCK_SIZE 0x0001
#define TOLUD_HOSTBRIDGE_CFG_TOLUD_LSB 0x0014
#define TOLUD_HOSTBRIDGE_CFG_TOLUD_SIZE 0x000c

#ifdef SV_HOOKS

/** DIDOVR_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/DIDOVR_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.DIDOVR_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.DIDOVR_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This is an INTEL RESERVED register and should NOT be disclosed to customers. It is for test and debug purposes only and will not be included in external documentation.
  */

#define DIDOVR_HOSTBRIDGE_CFG_REG              (0x000000c0U)

typedef union {
  struct {
    UINT32 rsvd_0 : 16; /**< Undefined - auto filled rsvd_[15:00] */

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 did0ovr : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               Device Identifier Override (Device 0): These
                               bits will override the current DID once the
                               DID Override Enable capability (CAPID_A.DID0OE)
                               is set.
                            */

    UINT32 oe0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Device Identifier Override Enable (Device
                               0)
                            */

    UINT32 rsvd_1 : 7; /**< Undefined - auto filled rsvd_[31:25] */

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DIDOVR_HOSTBRIDGE_CFG_STRUCT;

#define DIDOVR_HOSTBRIDGE_CFG_WIDTH 32

#define DIDOVR_HOSTBRIDGE_CFG_DID0OVR_LSB 0x0010
#define DIDOVR_HOSTBRIDGE_CFG_DID0OVR_SIZE 0x0008
#define DIDOVR_HOSTBRIDGE_CFG_OE0_LSB 0x0018
#define DIDOVR_HOSTBRIDGE_CFG_OE0_SIZE 0x0001

#endif      // SV_HOOKS


/** ERRSTS_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/ERRSTS_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.ERRSTS_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.ERRSTS_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register is used to report various error conditions via the SERR DMI messaging mechanism. An SERR DMI message is generated on a zero to one transition of any of these flags (if enabled by the ERRCMD and PCICMD registers). These bits are set regardless of whether or not the SERR is enabled and generated. After the error processing is complete, the error logging mechanism can be unlocked by clearing the appropriate status bit by software writing a 1 to it.
  */

#define ERRSTS_HOSTBRIDGE_CFG_REG              (0x000000c8U)

typedef union {
  struct {
    UINT16 mc0_dserr : 1; /**< MC0 DSERR */

                            /* Bits[0:0], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               If this bit is set to 1, a memory read data
                               transfer had a single-bit correctable error
                               and the           corrected data was returned
                               to the requesting agent. When this bit is
                               set the column, row, bank, and rank
                               where the error occurred and the syndrome
                               of the error are logged in the ECC Error Log
                               register in the channel           where the
                               error occurred. Once this bit is set the ECCERRLOGx
                               fields are locked to further single-bit error
                               updates until the CPU clears this bit by writing
                               a 1. A multiple bit error that occurs after
                               this bit is set           will overwrite the
                               ECCERRLOGx fields with the multiple-bit error
                               signature and the DMERR bit will also be set.
                               A single bit error that occurs after a multibit
                               error will set this bit but will not overwrite
                               the other           fields.
                            */

    UINT16 mc0_dmerr : 1; /**< MC0 DMERR */

                            /* Bits[1:1], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               If this bit is set to 1, a memory read data
                               transfer had an uncorrectable multiple-bit
                               error. When           this bit is set, the
                               column, row, bank, and rank that caused the
                               error, and the error syndrome, are logged
                               in           the ECC Error Log register in
                               the channel where the error occurred. Once
                               this bit is set, the ECCERRLOGx
                               fields are locked until the CPU clears this
                               bit by writing a 1. Software uses bits [1:0]
                               to detect whether the           logged error
                               address is for a Single-bit or a Multiple-bit
                               error.
                            */

    UINT16 fmithermerr : 1;

                            /* Bits[2:2], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               When this bit is set to 1 it indicates a thermal
                               event occurred in FMHC.
                            */

    UINT16 fmian : 1;

                            /* Bits[3:3], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               When this bit is set to 1 FMI Asynchronous
                               Notification error event with Media dead or
                               Health log critical notification has occurred
                               in FMHC.
                            */

    UINT16 fmca : 1;

                            /* Bits[4:4], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               When this bit is set to 1 it indicates a completer
                               abort occurred in FMHC (received from PCIE
                               to SXP).
                            */

    UINT16 fmur : 1;

                            /* Bits[5:5], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               When this bit is set to 1 it indicates a un-supported
                               request event occurred in FMHC (received from
                               PCIE to SXP).
                            */

    UINT16 ibecc_cor : 1; /**< IBECC COR */

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is reserved. Now IOP does not support
                               error messages from IBECC on legacy path.
                               IBECC err messages are logged in the MCA bank.
                               Previously, When this bit is set to 1 it indicates
                               a correctable error occurred in IBECC.
                            */

    UINT16 ibecc_uc : 1; /**< IBECC UC */

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is reserved. Now IOP does not support
                               error messages from IBECC on legacy path.
                               IBECC err messages are logged in the MCA bank.
                               Previously, When this bit is set to 1 it indicates
                               a uncorrectable error occurred in IBECC.
                            */

    UINT16 mc1_dserr : 1; /**< MC1 DSERR */

                            /* Bits[8:8], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               If this bit is set to 1, a memory read data
                               transfer had a single-bit correctable error
                               and the           corrected data was returned
                               to the requesting agent. When this bit is
                               set the column, row, bank, and rank
                               where the error occurred and the syndrome
                               of the error are logged in the ECC Error Log
                               register in the channel           where the
                               error occurred. Once this bit is set the ECCERRLOGx
                               fields are locked to further single-bit error
                               updates until the CPU clears this bit by writing
                               a 1. A multiple bit error that occurs after
                               this bit is set           will overwrite the
                               ECCERRLOGx fields with the multiple-bit error
                               signature and the DMERR bit will also be set.
                               A single bit error that occurs after a multibit
                               error will set this bit but will not overwrite
                               the other           fields.
                            */

    UINT16 mc1_dmerr : 1; /**< MC1 DMERR */

                            /* Bits[9:9], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               If this bit is set to 1, a memory read data
                               transfer had an uncorrectable multiple-bit
                               error. When           this bit is set, the
                               column, row, bank, and rank that caused the
                               error, and the error syndrome, are logged
                               in           the ECC Error Log register in
                               the channel where the error occurred. Once
                               this bit is set, the ECCERRLOGx
                               fields are locked until the CPU clears this
                               bit by writing a 1. Software uses bits [1:0]
                               to detect whether the           logged error
                               address is for a Single-bit or a Multiple-bit
                               error.
                            */

    UINT16 mc0_ddr5_crc : 1; /**< MC0 DDR5 CRC */

                            /* Bits[10:10], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               If this bit is set to 1, a memory read data
                               or write transfer had an uncorrectable DDR5
                               CRC error. The purpose of this feature is
                               just for debug and error isolation and NOT
                               a RAS feature that would be enabled in normal
                               operation. If a CRC error occurs, the system
                               should machine check and BSOD.
                            */

    UINT16 mc1_ddr5_crc : 1; /**< MC1 DDR5 CRC */

                            /* Bits[11:11], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               If this bit is set to 1, a memory read data
                               or write transfer had an uncorrectable DDR5
                               CRC error. The purpose of this feature is
                               just for debug and error isolation and NOT
                               a RAS feature that would be enabled in normal
                               operation. If a CRC error occurs, the system
                               should machine check and BSOD.
                            */

    UINT16 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[15:12] */

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} ERRSTS_HOSTBRIDGE_CFG_STRUCT;

#define ERRSTS_HOSTBRIDGE_CFG_WIDTH 16

#define ERRSTS_HOSTBRIDGE_CFG_MC0_DSERR_LSB 0x0000
#define ERRSTS_HOSTBRIDGE_CFG_MC0_DSERR_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_MC0_DMERR_LSB 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_MC0_DMERR_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_FMITHERMERR_LSB 0x0002
#define ERRSTS_HOSTBRIDGE_CFG_FMITHERMERR_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_FMIAN_LSB 0x0003
#define ERRSTS_HOSTBRIDGE_CFG_FMIAN_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_FMCA_LSB 0x0004
#define ERRSTS_HOSTBRIDGE_CFG_FMCA_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_FMUR_LSB 0x0005
#define ERRSTS_HOSTBRIDGE_CFG_FMUR_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_IBECC_COR_LSB 0x0006
#define ERRSTS_HOSTBRIDGE_CFG_IBECC_COR_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_IBECC_UC_LSB 0x0007
#define ERRSTS_HOSTBRIDGE_CFG_IBECC_UC_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_MC1_DSERR_LSB 0x0008
#define ERRSTS_HOSTBRIDGE_CFG_MC1_DSERR_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_MC1_DMERR_LSB 0x0009
#define ERRSTS_HOSTBRIDGE_CFG_MC1_DMERR_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_MC0_DDR5_CRC_LSB 0x000a
#define ERRSTS_HOSTBRIDGE_CFG_MC0_DDR5_CRC_SIZE 0x0001
#define ERRSTS_HOSTBRIDGE_CFG_MC1_DDR5_CRC_LSB 0x000b
#define ERRSTS_HOSTBRIDGE_CFG_MC1_DDR5_CRC_SIZE 0x0001


/** ERRCMD_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/ERRCMD_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.ERRCMD_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.ERRCMD_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register controls the Host Bridge responses to various system errors. Since the Host Bridge does not have an SERRB signal, SERR messages are passed from the CPU to the PCH over DMI. When a bit in this register is set, a SERR message will be generated on DMI whenever the corresponding flag is set in the ERRSTS register. The actual generation of the SERR message is globally enabled for Device #0 via the PCI Command register.
  */

#define ERRCMD_HOSTBRIDGE_CFG_REG              (0x000000caU)

typedef union {
  struct {
    UINT16 mc0_dserr : 1; /**< MC0 DSERR */

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host Bridge generates an SERR special
                               cycle over DMI when the DRAM controller detects
                               a single bit error. 0: Reporting of this condition
                               via SERR messaging is disabled. For systems
                               that do not support ECC this bit must be disabled.
                            */

    UINT16 mc0_dmerr : 1; /**< MC0 DMERR */

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host Bridge generates an SERR message
                               over DMI when it detects a multiple-bit error
                               reported by the DRAM controller. 0: Reporting
                               of this condition via SERR messaging is disabled.
                               For systems not supporting ECC this bit must
                               be disabled.
                            */

    UINT16 fmithermerr : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               SERR on FMHC thermal event 1: The Host Bridge
                               generates an SERR special cycle over DMI when
                               FMHC reports a thermal event. 0: Reporting
                               of this condition via SERR messaging is disabled.
                            */

    UINT16 fmian : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               SERR on FMI Asynchronous Notification error
                               event 1: The Host Bridge generates an SERR
                               special cycle over DMI when FMHC reports a
                               Asynchronous Notification error event with
                               Media dead or Health log critical notification.
                               0: Reporting of this condition via SERR messaging
                               is disabled.
                            */

    UINT16 fmca : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               SERR on FMHC CA event 1: The Host Bridge generates
                               an SERR special cycle over DMI when FMHC reports
                               a CA event. 0: Reporting of this condition
                               via SERR messaging is disabled.
                            */

    UINT16 fmur : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               SERR on FMHC un-supported request event 1:
                               The Host Bridge generates an SERR special
                               cycle over DMI when FMHC reports an un-supported
                               request event. 0: Reporting of this condition
                               via SERR messaging is disabled.
                            */

    UINT16 ibecc_cor : 1; /**< IBECC COR */

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is reserved. Now IOP does not support
                               error messages from IBECC on legacy path.
                               IBECC err messages are logged in the MCA bank.
                               Previously,  SERR on IBECC correctable error
                               event      1: The Host Bridge generates an
                               SERR special cycle over DMI when IBECC reports
                               correctable error.      0: Reporting of this
                               condition via SERR messaging is disabled.
                            */

    UINT16 ibecc_uc : 1; /**< IBECC UC */

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is reserved. Now IOP does not support
                               error messages from IBECC on legacy path.
                               IBECC err messages are logged in the MCA bank.
                               Previously,  SERR on IBECC uncorrectable error
                               event      1: The Host Bridge generates an
                               SERR special cycle over DMI when IBECC reports
                               uncorrectable error.      0: Reporting of
                               this condition via SERR messaging is disabled.
                            */

    UINT16 mc1_dserr : 1; /**< MC1 DSERR */

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host Bridge generates an SERR special
                               cycle over DMI when the DRAM controller detects
                               a single bit error. 0: Reporting of this condition
                               via SERR messaging is disabled. For systems
                               that do not support ECC this bit must be disabled.
                            */

    UINT16 mc1_dmerr : 1; /**< MC1 DMERR */

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host Bridge generates an SERR message
                               over DMI when it detects a multiple-bit error
                               reported by the DRAM controller. 0: Reporting
                               of this condition via SERR messaging is disabled.
                               For systems not supporting ECC this bit must
                               be disabled.
                            */

    UINT16 mc0_ddr5_crc : 1; /**< MC0 DDR5 CRC */

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host Bridge generates an SERR message
                               over DMI when it detects a CRC error reported
                               by the DRAM controller. 0: Reporting of this
                               condition via SERR messaging is disabled.
                               For systems not supporting DDR5 CRC this bit
                               must be disabled.
                            */

    UINT16 mc1_ddr5_crc : 1; /**< MC1 DDR5 CRC */

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host Bridge generates an SERR message
                               over DMI when it detects a CRC error reported
                               by the DRAM controller. 0: Reporting of this
                               condition via SERR messaging is disabled.
                               For systems not supporting DDR5 CRC this bit
                               must be disabled.
                            */

    UINT16 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[15:12] */

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} ERRCMD_HOSTBRIDGE_CFG_STRUCT;

#define ERRCMD_HOSTBRIDGE_CFG_WIDTH 16

#define ERRCMD_HOSTBRIDGE_CFG_MC0_DSERR_LSB 0x0000
#define ERRCMD_HOSTBRIDGE_CFG_MC0_DSERR_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_MC0_DMERR_LSB 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_MC0_DMERR_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_FMITHERMERR_LSB 0x0002
#define ERRCMD_HOSTBRIDGE_CFG_FMITHERMERR_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_FMIAN_LSB 0x0003
#define ERRCMD_HOSTBRIDGE_CFG_FMIAN_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_FMCA_LSB 0x0004
#define ERRCMD_HOSTBRIDGE_CFG_FMCA_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_FMUR_LSB 0x0005
#define ERRCMD_HOSTBRIDGE_CFG_FMUR_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_IBECC_COR_LSB 0x0006
#define ERRCMD_HOSTBRIDGE_CFG_IBECC_COR_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_IBECC_UC_LSB 0x0007
#define ERRCMD_HOSTBRIDGE_CFG_IBECC_UC_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_MC1_DSERR_LSB 0x0008
#define ERRCMD_HOSTBRIDGE_CFG_MC1_DSERR_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_MC1_DMERR_LSB 0x0009
#define ERRCMD_HOSTBRIDGE_CFG_MC1_DMERR_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_MC0_DDR5_CRC_LSB 0x000a
#define ERRCMD_HOSTBRIDGE_CFG_MC0_DDR5_CRC_SIZE 0x0001
#define ERRCMD_HOSTBRIDGE_CFG_MC1_DDR5_CRC_LSB 0x000b
#define ERRCMD_HOSTBRIDGE_CFG_MC1_DDR5_CRC_SIZE 0x0001


/** SMICMD_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/SMICMD_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.SMICMD_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.SMICMD_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register enables various errors to generate an SMI DMI special cycle. When an error flag is set in the ERRSTS register, it can generate an SERR, SMI, or SCI DMI special cycle when enabled in the ERRCMD, SMICMD, or SCICMD registers, respectively. Note that one and only one message type can be enabled.
  */

#define SMICMD_HOSTBRIDGE_CFG_REG              (0x000000ccU)

typedef union {
  struct {
    UINT16 mc0_dsesmi : 1; /**< MC0 DSESMI */

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SMI DMI special cycle
                               when the DRAM controller detects a single
                               bit error.           0: Reporting of this
                               condition via SMI messaging is disabled. For
                               systems that do not support ECC this bit must
                               be disabled.
                            */

    UINT16 mc0_dmesmi : 1; /**< MC0 DMESMI */

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SMI DMI message when
                               it detects a multiple-bit error reported by
                               the DRAM controller.           0: Reporting
                               of this condition via SMI messaging is disabled.
                               For systems not supporting ECC this bit must
                               be disabled.
                            */

    UINT16 fmithermerr : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               SMI on FMHC thermal event 1: The Host Bridge
                               generates an SMI special cycle over DMI when
                               FMHC reports a thermal event. 0: Reporting
                               of this condition via SMI messaging is disabled.
                            */

    UINT16 fmian : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               SMI on FMI Asynchronous Notification error
                               event 1: The Host Bridge generates an SMI
                               special cycle over DMI when FMHC reports a
                               Asynchronous Notification error event with
                               Media dead or Health log critical notification.
                               0: Reporting of this condition via SMI messaging
                               is disabled.
                            */

    UINT16 fmca : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               SMI on FMHC CA event 1: The Host Bridge generates
                               an SMI special cycle over DMI when FMHC reports
                               a CA event. 0: Reporting of this condition
                               via SMI messaging is disabled.
                            */

    UINT16 fmur : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               SMI on FMHC un-supported request event 1:
                               The Host Bridge generates an SMI special cycle
                               over DMI when FMHC reports an un-supported
                               request event. 0: Reporting of this condition
                               via SMI messaging is disabled.
                            */

    UINT16 ibecc_cor : 1; /**< IBECC COR */

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is reserved. Now IOP does not support
                               error messages from IBECC on legacy path.
                               IBECC err messages are logged in the MCA bank.
                               Previously,  SMI on IBECC correctable error
                               event      1: The Host Bridge generates an
                               SMI special cycle over DMI when IBECC reports
                               correctable error.      0: Reporting of this
                               condition via SMI messaging is disabled.
                            */

    UINT16 ibecc_uc : 1; /**< IBECC UC */

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is reserved. Now IOP does not support
                               error messages from IBECC on legacy path.
                               IBECC err messages are logged in the MCA bank.
                               Previously,  SMI on IBECC uncorrectable error
                               event      1: The Host Bridge generates an
                               SMI special cycle over DMI when IBECC reports
                               uncorrectable error.      0: Reporting of
                               this condition via SMI messaging is disabled.
                            */

    UINT16 mc1_dsesmi : 1; /**< MC1 DSESMI */

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SMI DMI special cycle
                               when the DRAM controller detects a single
                               bit error.           0: Reporting of this
                               condition via SMI messaging is disabled. For
                               systems that do not support ECC this bit must
                               be disabled.
                            */

    UINT16 mc1_dmesmi : 1; /**< MC1 DMEDMI */

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SMI DMI message when
                               it detects a multiple-bit error reported by
                               the DRAM controller.           0: Reporting
                               of this condition via SMI messaging is disabled.
                               For systems not supporting ECC this bit must
                               be disabled.
                            */

    UINT16 mc0_ddr5_crc : 1; /**< MC0 DDR5 CRC */

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SMI DMI message when
                               it detects a CRC error reported by the DRAM
                               controller. 0: Reporting of this condition
                               via SMI messaging is disabled. For systems
                               not supporting DDR5 CRC this bit must be disabled.
                            */

    UINT16 mc1_ddr5_crc : 1; /**< MC1 DDR5 CRC */

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SMI DMI message when
                               it detects a CRC error reported by the DRAM
                               controller. 0: Reporting of this condition
                               via SMI messaging is disabled. For systems
                               not supporting DDR5 CRC this bit must be disabled.
                            */

    UINT16 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[15:12] */

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SMICMD_HOSTBRIDGE_CFG_STRUCT;

#define SMICMD_HOSTBRIDGE_CFG_WIDTH 16

#define SMICMD_HOSTBRIDGE_CFG_MC0_DSESMI_LSB 0x0000
#define SMICMD_HOSTBRIDGE_CFG_MC0_DSESMI_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_MC0_DMESMI_LSB 0x0001
#define SMICMD_HOSTBRIDGE_CFG_MC0_DMESMI_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_FMITHERMERR_LSB 0x0002
#define SMICMD_HOSTBRIDGE_CFG_FMITHERMERR_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_FMIAN_LSB 0x0003
#define SMICMD_HOSTBRIDGE_CFG_FMIAN_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_FMCA_LSB 0x0004
#define SMICMD_HOSTBRIDGE_CFG_FMCA_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_FMUR_LSB 0x0005
#define SMICMD_HOSTBRIDGE_CFG_FMUR_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_IBECC_COR_LSB 0x0006
#define SMICMD_HOSTBRIDGE_CFG_IBECC_COR_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_IBECC_UC_LSB 0x0007
#define SMICMD_HOSTBRIDGE_CFG_IBECC_UC_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_MC1_DSESMI_LSB 0x0008
#define SMICMD_HOSTBRIDGE_CFG_MC1_DSESMI_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_MC1_DMESMI_LSB 0x0009
#define SMICMD_HOSTBRIDGE_CFG_MC1_DMESMI_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_MC0_DDR5_CRC_LSB 0x000a
#define SMICMD_HOSTBRIDGE_CFG_MC0_DDR5_CRC_SIZE 0x0001
#define SMICMD_HOSTBRIDGE_CFG_MC1_DDR5_CRC_LSB 0x000b
#define SMICMD_HOSTBRIDGE_CFG_MC1_DDR5_CRC_SIZE 0x0001


/** SCICMD_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/SCICMD_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.SCICMD_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.SCICMD_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register enables various errors to generate an SMI DMI special cycle. When an error flag is set in the ERRSTS register, it can generate an SERR, SMI, or SCI DMI special cycle when enabled in the ERRCMD, SMICMD, or SCICMD registers, respectively. Note that one and only one message type can be enabled.
  */

#define SCICMD_HOSTBRIDGE_CFG_REG              (0x000000ceU)

typedef union {
  struct {
    UINT16 mc0_dsesci : 1; /**< MC0 DSESCI */

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SCI DMI special cycle
                               when the DRAM controller detects a single
                               bit error.           0: Reporting of this
                               condition via SCI messaging is disabled. For
                               systems that do not support ECC this bit must
                               be disabled.
                            */

    UINT16 mc0_dmesci : 1; /**< MC0 DMESCI */

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SCI DMI message when
                               it detects a multiple-bit error reported by
                               the DRAM controller.           0: Reporting
                               of this condition via SCI messaging is disabled.
                               For systems not supporting ECC this bit must
                               be disabled.
                            */

    UINT16 fmithermerr : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               SCI on FMHC thermal event 1: The Host Bridge
                               generates an SCI special cycle over DMI when
                               FMHC reports a thermal event. 0: Reporting
                               of this condition via SCI messaging is disabled.
                            */

    UINT16 fmian : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               SCI on FMI Asynchronous Notification error
                               event 1: The Host Bridge generates an SCI
                               special cycle over DMI when FMHC reports a
                               Asynchronous Notification error event with
                               Media dead or Health log critical notification.
                               0: Reporting of this condition via SCI messaging
                               is disabled.
                            */

    UINT16 fmca : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               SCI on FMHC CA event 1: The Host Bridge generates
                               an SCI special cycle over DMI when FMHC reports
                               a CA event. 0: Reporting of this condition
                               via SCI messaging is disabled.
                            */

    UINT16 fmur : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               SCI on FMHC un-supported request event 1:
                               The Host Bridge generates an SCI special cycle
                               over DMI when FMHC reports an un-supported
                               request event. 0: Reporting of this condition
                               via SCI messaging is disabled.
                            */

    UINT16 ibecc_cor : 1; /**< IBECC COR */

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is reserved. Now IOP does not support
                               error messages from IBECC on legacy path.
                               IBECC err messages are logged in the MCA bank.
                               Previously,  SCI on IBECC correctable error
                               event      1: The Host Bridge generates an
                               SCI special cycle over DMI when IBECC reports
                               correctable error.      0: Reporting of this
                               condition via SCI messaging is disabled.
                            */

    UINT16 ibecc_uc : 1; /**< IBECC UC */

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is reserved. Now IOP does not support
                               error messages from IBECC on legacy path.
                               IBECC err messages are logged in the MCA bank.
                               Previously,  SCI on IBECC uncorrectable error
                               event      1: The Host Bridge generates an
                               SCI special cycle over DMI when IBECC reports
                               uncorrectable error.      0: Reporting of
                               this condition via SCI messaging is disabled.
                            */

    UINT16 mc1_dsesci : 1; /**< MC1 DSESCI */

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SCI DMI special cycle
                               when the DRAM controller detects a single
                               bit error.           0: Reporting of this
                               condition via SCI messaging is disabled. For
                               systems that do not support ECC this bit must
                               be disabled.
                            */

    UINT16 mc1_dmesci : 1; /**< MC1 DMESCI */

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SCI DMI message when
                               it detects a multiple-bit error reported by
                               the DRAM controller.           0: Reporting
                               of this condition via SCI messaging is disabled.
                               For systems not supporting ECC this bit must
                               be disabled.
                            */

    UINT16 mc0_ddr5_crc : 1; /**< MC0 DDR5 CRC */

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SCI DMI message when
                               it detects a CRC error reported by the DRAM
                               controller. 0: Reporting of this condition
                               via SCI messaging is disabled. For systems
                               not supporting DDR5 CRC this bit must be disabled.
                            */

    UINT16 mc1_ddr5_crc : 1; /**< MC1 DDR5 CRC */

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               1: The Host generates an SCI DMI message when
                               it detects a CRC error reported by the DRAM
                               controller. 0: Reporting of this condition
                               via SCI messaging is disabled. For systems
                               not supporting DDR5 CRC this bit must be disabled.
                            */

    UINT16 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[15:12] */

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SCICMD_HOSTBRIDGE_CFG_STRUCT;

#define SCICMD_HOSTBRIDGE_CFG_WIDTH 16

#define SCICMD_HOSTBRIDGE_CFG_MC0_DSESCI_LSB 0x0000
#define SCICMD_HOSTBRIDGE_CFG_MC0_DSESCI_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_MC0_DMESCI_LSB 0x0001
#define SCICMD_HOSTBRIDGE_CFG_MC0_DMESCI_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_FMITHERMERR_LSB 0x0002
#define SCICMD_HOSTBRIDGE_CFG_FMITHERMERR_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_FMIAN_LSB 0x0003
#define SCICMD_HOSTBRIDGE_CFG_FMIAN_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_FMCA_LSB 0x0004
#define SCICMD_HOSTBRIDGE_CFG_FMCA_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_FMUR_LSB 0x0005
#define SCICMD_HOSTBRIDGE_CFG_FMUR_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_IBECC_COR_LSB 0x0006
#define SCICMD_HOSTBRIDGE_CFG_IBECC_COR_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_IBECC_UC_LSB 0x0007
#define SCICMD_HOSTBRIDGE_CFG_IBECC_UC_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_MC1_DSESCI_LSB 0x0008
#define SCICMD_HOSTBRIDGE_CFG_MC1_DSESCI_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_MC1_DMESCI_LSB 0x0009
#define SCICMD_HOSTBRIDGE_CFG_MC1_DMESCI_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_MC0_DDR5_CRC_LSB 0x000a
#define SCICMD_HOSTBRIDGE_CFG_MC0_DDR5_CRC_SIZE 0x0001
#define SCICMD_HOSTBRIDGE_CFG_MC1_DDR5_CRC_LSB 0x000b
#define SCICMD_HOSTBRIDGE_CFG_MC1_DDR5_CRC_SIZE 0x0001


/** SKPD_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/SKPD_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.SKPD_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.SKPD_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This register holds 32 writable bits with no functionality behind them. It is for the convenience of BIOS and graphics drivers.
  */

#define SKPD_HOSTBRIDGE_CFG_REG                (0x000000dcU)

typedef union {
  struct {
    UINT32 skpd : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* 1 DWORD of data storage. */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SKPD_HOSTBRIDGE_CFG_STRUCT;

#define SKPD_HOSTBRIDGE_CFG_WIDTH 32

#define SKPD_HOSTBRIDGE_CFG_SKPD_LSB 0x0000
#define SKPD_HOSTBRIDGE_CFG_SKPD_SIZE 0x0020

#ifdef SV_HOOKS

/** CAPID0_SNCU_CFG desc:
  * Register default value:        0x00000009
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/CAPID0_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.CAPID0_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.CAPID0_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Control of bits in this register are only required for customer visible SKU differentiation.
  */

#define CAPID0_HOSTBRIDGE_CFG_REG              (0x000000e0U)

typedef union {
  struct {
    UINT16 cap_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000009*/

                            /*
                               This field has the value 1001b to identify
                               the CAP_ID assigned by the PCI SIG for vendor
                               dependent capability pointers.
                            */

    UINT16 next_cap : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               This field is hardwired to 00h indicating
                               the end of the capabilities linked list.
                            */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} CAPID0_HOSTBRIDGE_CFG_STRUCT;

#define CAPID0_HOSTBRIDGE_CFG_WIDTH 16

#define CAPID0_HOSTBRIDGE_CFG_CAP_ID_LSB 0x0000
#define CAPID0_HOSTBRIDGE_CFG_CAP_ID_SIZE 0x0008
#define CAPID0_HOSTBRIDGE_CFG_NEXT_CAP_LSB 0x0008
#define CAPID0_HOSTBRIDGE_CFG_NEXT_CAP_SIZE 0x0008

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** CAPCTRL0_SNCU_CFG desc:
  * Register default value:        0x00000114
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/CAPCTRL0_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.CAPCTRL0_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.CAPCTRL0_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Control of bits in this register are only required for customer visible SKU differentiation.
  */

#define CAPCTRL0_HOSTBRIDGE_CFG_REG            (0x000000e2U)

typedef union {
  struct {
    UINT16 capidlen : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000014*/

                            /*
                               This field has the value 0Ch to indicate the
                               structure length (12 bytes).
                            */

    UINT16 capid_ver : 4;

                            /* Bits[11:8], Access Type=RO, default=0x00000001*/

                            /*
                               This field has the value 0001b to identify
                               the first revision of the CAPID register definition.
                            */

    UINT16 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[15:12] */

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} CAPCTRL0_HOSTBRIDGE_CFG_STRUCT;

#define CAPCTRL0_HOSTBRIDGE_CFG_WIDTH 16

#define CAPCTRL0_HOSTBRIDGE_CFG_CAPIDLEN_LSB 0x0000
#define CAPCTRL0_HOSTBRIDGE_CFG_CAPIDLEN_SIZE 0x0008
#define CAPCTRL0_HOSTBRIDGE_CFG_CAPID_VER_LSB 0x0008
#define CAPCTRL0_HOSTBRIDGE_CFG_CAPID_VER_SIZE 0x0004

#endif      // SV_HOOKS


/** CAPID0_A_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/CAPID0_A_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.CAPID0_A_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.CAPID0_A_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Control of bits in this register are only required for customer visible SKU differentiation.
  */

#define CAPID0_A_HOSTBRIDGE_CFG_REG            (0x000000e4U)

typedef union {
  struct {
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[02:00] */

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 ddr_overclock : 1; /**< DDR OVERCLOCK */

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               DDR overclocking. PCODE will update this field
                               with the value of FUSE_DDR_OC_EN.
                            */

    UINT32 crid : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Compatibility Rev ID: PCODE will update this
                               field with the value of FUSE_CRID.
                            */

    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[09:08] */

                            /* Bits[9:8], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 did0oe : 1;

                            /* Bits[10:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               0b     Disable ability to override DID0 -
                               For production 1b     Enable ability to override
                               DID - For debug and samples only
                            */

    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[11:11] */

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 pdcd : 1;

                            /* Bits[12:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               0: Capable of Dual Channels 1: Not Capable
                               of Dual Channel - only single channel capable.
                            */

    UINT32 x2apic_en : 1; /**< X2APIC ENABLE */

                            /* Bits[13:13], Access Type=RW/L, default=0x00000000*/

                            /*
                               Extended Interrupt Mode. 0b: Hardware does
                               not support Extended APIC mode. 1b: Hardware
                               supports Extended APIC mode.
                            */

    UINT32 ddpcd : 1;

                            /* Bits[14:14], Access Type=RW/L, default=0x00000000*/

                            /*
                               Allows Dual Channel operation but only supports
                               1 DIMM per channel.           0: 2 DIMMs per
                               channel enabled           1: 2 DIMMs per channel
                               disabled. This setting hardwires bits 2 and
                               3 of the rank population field for each channel
                               to zero. (MCHBAR offset 260h, bits 22-23 for
                               channel 0 and MCHBAR offset 660h, bits 22-23
                               for channel 1)
                            */

    UINT32 rsvd_3 : 1; /**< Undefined - auto filled rsvd_[15:15] */

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 fufrd : 1;

                            /* Bits[16:16], Access Type=RW/L, default=0x00000000*/

                            /*
                               Controls how much ULT information is available
                               to debugging software via configuration transactions.
                               0: Full ULT information is available, including
                               Wafer, X, and Y location. 1: ULT information
                               is hidden.  All ULT information will read
                               out as zeros.
                            */

    UINT32 rsvd_4 : 2; /**< Undefined - auto filled rsvd_[18:17] */

                            /* Bits[18:17], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 ddrsz : 2;

                            /* Bits[20:19], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field defines the maximum allowed memory
                               size per channel. 00b     Unlimited (64GB
                               per channel) 01b     Maximum 8GB per channel
                               10b     Maximum 4GB per channel 11b     Maximum
                               2GB per channel
                            */

    UINT32 reserved : 1;

                            /* Bits[21:21], Access Type=RW/L, default=0x00000000*/

                            /* Reserved */

    UINT32 rsvd_5 : 1; /**< Undefined - auto filled rsvd_[22:22] */

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 vtdd : 1;

                            /* Bits[23:23], Access Type=RW/L, default=0x00000000*/

                            /* 0: Enable VTd 1: Disable VTd */

    UINT32 fdee : 1;

                            /* Bits[24:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               0: DRAM ECC optional via software.
                               1: DRAM ECC enabled. MCHBAR C0MISCCTL bit
                               [0] and C1MISCCTL bit [0] are forced to 1
                               and Read-Only.           Note that FDEE and
                               ECCDIS must not both be set to 1.
                            */

    UINT32 eccdis : 1;

                            /* Bits[25:25], Access Type=RW/L, default=0x00000000*/

                            /* 0b   ECC capable 1b   Not ECC capable */

    UINT32 rsvd_6 : 6; /**< Undefined - auto filled rsvd_[31:26] */

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CAPID0_A_HOSTBRIDGE_CFG_STRUCT;

#define CAPID0_A_HOSTBRIDGE_CFG_WIDTH 32

#define CAPID0_A_HOSTBRIDGE_CFG_DDR_OVERCLOCK_LSB 0x0003
#define CAPID0_A_HOSTBRIDGE_CFG_DDR_OVERCLOCK_SIZE 0x0001
#define CAPID0_A_HOSTBRIDGE_CFG_CRID_LSB 0x0004
#define CAPID0_A_HOSTBRIDGE_CFG_CRID_SIZE 0x0004
#define CAPID0_A_HOSTBRIDGE_CFG_DID0OE_LSB 0x000a
#define CAPID0_A_HOSTBRIDGE_CFG_DID0OE_SIZE 0x0001
#define CAPID0_A_HOSTBRIDGE_CFG_PDCD_LSB 0x000c
#define CAPID0_A_HOSTBRIDGE_CFG_PDCD_SIZE 0x0001
#define CAPID0_A_HOSTBRIDGE_CFG_X2APIC_EN_LSB 0x000d
#define CAPID0_A_HOSTBRIDGE_CFG_X2APIC_EN_SIZE 0x0001
#define CAPID0_A_HOSTBRIDGE_CFG_DDPCD_LSB 0x000e
#define CAPID0_A_HOSTBRIDGE_CFG_DDPCD_SIZE 0x0001
#define CAPID0_A_HOSTBRIDGE_CFG_FUFRD_LSB 0x0010
#define CAPID0_A_HOSTBRIDGE_CFG_FUFRD_SIZE 0x0001
#define CAPID0_A_HOSTBRIDGE_CFG_DDRSZ_LSB 0x0013
#define CAPID0_A_HOSTBRIDGE_CFG_DDRSZ_SIZE 0x0002
#define CAPID0_A_HOSTBRIDGE_CFG_RESERVED_LSB 0x0015
#define CAPID0_A_HOSTBRIDGE_CFG_RESERVED_SIZE 0x0001
#define CAPID0_A_HOSTBRIDGE_CFG_VTDD_LSB 0x0017
#define CAPID0_A_HOSTBRIDGE_CFG_VTDD_SIZE 0x0001
#define CAPID0_A_HOSTBRIDGE_CFG_FDEE_LSB 0x0018
#define CAPID0_A_HOSTBRIDGE_CFG_FDEE_SIZE 0x0001
#define CAPID0_A_HOSTBRIDGE_CFG_ECCDIS_LSB 0x0019
#define CAPID0_A_HOSTBRIDGE_CFG_ECCDIS_SIZE 0x0001


/** CAPID0_B_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/CAPID0_B_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.CAPID0_B_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.CAPID0_B_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Control of bits in this register are only required for customer visible SKU differentiation.
  */

#define CAPID0_B_HOSTBRIDGE_CFG_REG            (0x000000e8U)

typedef union {
  struct {
    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[06:00] */

                            /* Bits[6:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 ddd : 1;

                            /* Bits[7:7], Access Type=RW/L, default=0x00000000*/

                            /*
                               0 - Debug mode           [br]1 - Production
                               mode
                            */

    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[10:08] */

                            /* Bits[10:8], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 hdcpd : 1; /**< HDMI and Diaplay Copy Protection */

                            /* Bits[11:11], Access Type=RW/L, default=0x00000000*/

                            /* 0 - Capable of HDCP 1 - HDCP Disabled */

    UINT32 adm_tech : 3; /**< ADM Technology */

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               ADM Technology. 3'b000 - no L4 cache capability
                               (not present or fused off); 3'b001 - the SKU
                               supports ADM config 1 (good ADM config); 3'b011
                               - the SKU supports ADM config 2 (great ADM
                               config)
                            */

    UINT32 rsvd_2 : 3; /**< Undefined - auto filled rsvd_[17:15] */

                            /* Bits[17:15], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 reserved : 1; /**< Reserved Bit */

                            /* Bits[18:18], Access Type=RW/L, default=0x00000000*/

                            /* this is a reserved bit */

    UINT32 pkgtyp : 1; /**< Package Type */

                            /* Bits[19:19], Access Type=RW/L, default=0x00000000*/

                            /* This setting indicates the CPU Package Type. */

    UINT32 rsvd_3 : 1; /**< Undefined - auto filled rsvd_[20:20] */

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 pll_ref100_cfg : 3; /**< PLL REF100 CFG */

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               DDR3 Maximum Frequency Capability with 100
                               Memory.  PCODE will update this field with
                               the value of FUSE_PLL_REF100_CFG and then
                               apply SSKU overrides. Maximum allowed memory
                               frequency with 100 MHz ref clk.  Also serves
                               as defeature. Unlike 133 MHz ref fuses, these
                               are normal 3 bit field    0 - 100 MHz ref
                               disabled    1 - upto DDR-1400 (7 x 200)
                               2 - upto DDR-1600 (8 x 200)    3 - upto DDR-1800
                               (8 x 200)    4 - upto DDR-2000 (10 x 200)
                               5 - upto DDR-2200 (11 x 200)    6 - upto DDR-2400
                               (12 x 200)    7 - no limit (but still limited
                               by _DDR_FREQ200 to 2600)
                            */

    UINT32 svm_disable : 1; /**< SVM DISABLE */

                            /* Bits[24:24], Access Type=RW/L, default=0x00000000*/

                            /* Consumed by Display. */

    UINT32 rsvd_4 : 2; /**< Undefined - auto filled rsvd_[26:25] */

                            /* Bits[26:25], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 smt4 : 1; /**< SMT4 Supported */

                            /* Bits[27:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               SMT4 supported. Resolving table together with
                               CAPID0_B[SMT]: CAPID0_B[SMT] CAPID0_B[SMT4]
                               supported #threads 1    1    4 1    0    2
                               0    1    1 0    0    1
                            */

    UINT32 smt : 1; /**< SMT2 Supported */

                            /* Bits[28:28], Access Type=RW/L, default=0x00000000*/

                            /*
                               SMT2 supported. See CAPID0_B[SMT4] description
                               for details about resolution between this
                               field and SMT4 field.
                            */

    UINT32 oc_enabled : 1; /**< OC ENABLED */

                            /* Bits[29:29], Access Type=RW/L, default=0x00000000*/

                            /*
                               PCODE will update this field with the value
                               of FUSE_OC_ENABLED. 0b   Over-clocking is
                               Disabled 1b   Over-clocking is Enabled  If
                               over-clocking is enabled, FUSE_OC_BINS contains
                               how many bits of over-clocking are supported.
                               The encoding is as follows: 0h   Overclocking
                               is Disabled 1h   Max 1 bin of overclocking
                               is supported 2h   Max 2 bin of overclocking
                               is supported 3h   Max 3 bin of overclocking
                               is supported 4h   Max 4 bin of overclocking
                               is supported 5h   Max 5 bin of overclocking
                               is supported 6h   Max 6 bin of overclocking
                               is supported 7h   Unlimited If overclocking
                               is not enabled, FUSE_OC_BINS is meaningless,
                               and should be 0.
                            */

    UINT32 rsvd_5 : 2; /**< Undefined - auto filled rsvd_[31:30] */

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CAPID0_B_HOSTBRIDGE_CFG_STRUCT;

#define CAPID0_B_HOSTBRIDGE_CFG_WIDTH 32

#define CAPID0_B_HOSTBRIDGE_CFG_DDD_LSB 0x0007
#define CAPID0_B_HOSTBRIDGE_CFG_DDD_SIZE 0x0001
#define CAPID0_B_HOSTBRIDGE_CFG_HDCPD_LSB 0x000b
#define CAPID0_B_HOSTBRIDGE_CFG_HDCPD_SIZE 0x0001
#define CAPID0_B_HOSTBRIDGE_CFG_ADM_TECH_LSB 0x000c
#define CAPID0_B_HOSTBRIDGE_CFG_ADM_TECH_SIZE 0x0003
#define CAPID0_B_HOSTBRIDGE_CFG_RESERVED_LSB 0x0012
#define CAPID0_B_HOSTBRIDGE_CFG_RESERVED_SIZE 0x0001
#define CAPID0_B_HOSTBRIDGE_CFG_PKGTYP_LSB 0x0013
#define CAPID0_B_HOSTBRIDGE_CFG_PKGTYP_SIZE 0x0001
#define CAPID0_B_HOSTBRIDGE_CFG_PLL_REF100_CFG_LSB 0x0015
#define CAPID0_B_HOSTBRIDGE_CFG_PLL_REF100_CFG_SIZE 0x0003
#define CAPID0_B_HOSTBRIDGE_CFG_SVM_DISABLE_LSB 0x0018
#define CAPID0_B_HOSTBRIDGE_CFG_SVM_DISABLE_SIZE 0x0001
#define CAPID0_B_HOSTBRIDGE_CFG_SMT4_LSB 0x001b
#define CAPID0_B_HOSTBRIDGE_CFG_SMT4_SIZE 0x0001
#define CAPID0_B_HOSTBRIDGE_CFG_SMT_LSB 0x001c
#define CAPID0_B_HOSTBRIDGE_CFG_SMT_SIZE 0x0001
#define CAPID0_B_HOSTBRIDGE_CFG_OC_ENABLED_LSB 0x001d
#define CAPID0_B_HOSTBRIDGE_CFG_OC_ENABLED_SIZE 0x0001


/** CAPID0_C_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/CAPID0_C_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.CAPID0_C_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.CAPID0_C_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Control of bits in this register are only required for customer visible SKU differentiation.
  */

#define CAPID0_C_HOSTBRIDGE_CFG_REG            (0x000000ecU)

typedef union {
  struct {
    UINT32 rsvd_0 : 5; /**< Undefined - auto filled rsvd_[04:00] */

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 display_pipec_dis : 1; /**< DISPLAY PIPEC DIS */

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Disable PIPE C 0 - Enable PIPE C 1 - Disable
                               PIPE C
                            */

    UINT32 display_piped_dis : 1; /**< DISPLAY PIPED DIS */

                            /* Bits[6:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Disable PIPE D 0 - Enable PIPE D 1 - Disable
                               PIPE D
                            */

    UINT32 bclkocrange : 2; /**< BCLK OC RANGE */

                            /* Bits[8:7], Access Type=RW/L, default=0x00000000*/

                            /*
                               Bclk OC range - Combination of DSKU and FUSE_BCLKOCRANGE
                               * 00 Bclk can't exceed 100MHz * 01 Bclk can't
                               exceed [Threshold 1] MHz * 10 Bclk can't exceed
                               [Threshold 2] MHz * 11 unlimited Bclk range
                               * Threshold 1 = 115MHz - Hard coded in pCode
                               (298D) * Threshold 2 = 130MHz - Hard coded
                               in pCode (338D).
                            */

    UINT32 se_dis : 1; /**< SE DISABLE */

                            /* Bits[9:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Secure Enclave Disable: PCODE will update
                               this field with the value of FUSE_SE_DIS.
                            */

    UINT32 fdskufp : 4; /**< Future DSKU */

                            /* Bits[13:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               Placeholders for future DSKUs, combination
                               of DSKU and FUSE_FDSKUFP.
                            */

    UINT32 qclk_gv_dis : 1; /**< QCLK GV DISABLE */

                            /* Bits[14:14], Access Type=RW/L, default=0x00000000*/

                            /*
                               0: Qclk GV Enable 1: Qclk GV Disable [IntelRsvd]Used
                               by MRC. Value 1 comes from fuse.[/IntelRsvd]
                            */

    UINT32 rsvd_1 : 17; /**< Undefined - auto filled rsvd_[31:15] */

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CAPID0_C_HOSTBRIDGE_CFG_STRUCT;

#define CAPID0_C_HOSTBRIDGE_CFG_WIDTH 32

#define CAPID0_C_HOSTBRIDGE_CFG_DISPLAY_PIPEC_DIS_LSB 0x0005
#define CAPID0_C_HOSTBRIDGE_CFG_DISPLAY_PIPEC_DIS_SIZE 0x0001
#define CAPID0_C_HOSTBRIDGE_CFG_DISPLAY_PIPED_DIS_LSB 0x0006
#define CAPID0_C_HOSTBRIDGE_CFG_DISPLAY_PIPED_DIS_SIZE 0x0001
#define CAPID0_C_HOSTBRIDGE_CFG_BCLKOCRANGE_LSB 0x0007
#define CAPID0_C_HOSTBRIDGE_CFG_BCLKOCRANGE_SIZE 0x0002
#define CAPID0_C_HOSTBRIDGE_CFG_SE_DIS_LSB 0x0009
#define CAPID0_C_HOSTBRIDGE_CFG_SE_DIS_SIZE 0x0001
#define CAPID0_C_HOSTBRIDGE_CFG_FDSKUFP_LSB 0x000a
#define CAPID0_C_HOSTBRIDGE_CFG_FDSKUFP_SIZE 0x0004
#define CAPID0_C_HOSTBRIDGE_CFG_QCLK_GV_DIS_LSB 0x000e
#define CAPID0_C_HOSTBRIDGE_CFG_QCLK_GV_DIS_SIZE 0x0001


/** CAPID0_E_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/CAPID0_E_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.CAPID0_E_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.CAPID0_E_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Control of bits in this register are only required for customer visible SKU differentiation.
  */

#define CAPID0_E_HOSTBRIDGE_CFG_REG            (0x000000f0U)

typedef union {
  struct {
    UINT32 rsvd_0 : 18; /**< Undefined - auto filled rsvd_[17:00] */

                            /* Bits[17:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 ibecc_dis : 1; /**< IBECC DISABLE */

                            /* Bits[18:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Disable IBECC operation. PCODE will update
                               this field with the value of FUSE_IBECC_DIS.
                            */

    UINT32 vddq_vid_max : 11; /**< VDDQ VOLTAGE MAX */

                            /* Bits[29:19], Access Type=RW/L, default=0x00000000*/

                            /*
                               VDDQ_TX MAX VID value.  PCODE will update
                               this field with the value from 11b fuse
                            */

    UINT32 crashlog_dis : 1; /**< CRASHLOG DISABLE */

                            /* Bits[30:30], Access Type=RW/L, default=0x00000000*/

                            /*
                               0: Device 10 associated memory spaces are
                               accessible. 1: Device 10 associated memory
                               and IO spaces are disabled by hardwiring the
                               D10EN field, bit 17 of the Device Enable register,
                               (DEVEN Dev 0 Offset 54h) to 0.
                            */

    UINT32 spare : 1;

                            /* Bits[31:31], Access Type=RW/L, default=0x00000000*/

                            /* Spare. */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CAPID0_E_HOSTBRIDGE_CFG_STRUCT;

#define CAPID0_E_HOSTBRIDGE_CFG_WIDTH 32

#define CAPID0_E_HOSTBRIDGE_CFG_IBECC_DIS_LSB 0x0012
#define CAPID0_E_HOSTBRIDGE_CFG_IBECC_DIS_SIZE 0x0001
#define CAPID0_E_HOSTBRIDGE_CFG_VDDQ_VID_MAX_LSB 0x0013
#define CAPID0_E_HOSTBRIDGE_CFG_VDDQ_VID_MAX_SIZE 0x000b
#define CAPID0_E_HOSTBRIDGE_CFG_CRASHLOG_DIS_LSB 0x001e
#define CAPID0_E_HOSTBRIDGE_CFG_CRASHLOG_DIS_SIZE 0x0001
#define CAPID0_E_HOSTBRIDGE_CFG_SPARE_LSB 0x001f
#define CAPID0_E_HOSTBRIDGE_CFG_SPARE_SIZE 0x0001

#ifdef SV_HOOKS

/** MID_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/MID_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.MID_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.MID_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * This is an INTEL RESERVED register and should NOT be disclosed to customers. It is for test and debug purposes only and will not be included in external documentation. For A0 the Manufacturing ID will be 0000 0FB8h.
  */

#define MID_HOSTBRIDGE_CFG_REG                 (0x000000f4U)

typedef union {
  struct {
    UINT32 proc : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               These Bits Identify the Fab Process.
                               [br]15h:    Fab code for P1266           [br]17h:
                               Fab code for P1268           [br]19h:    Fab
                               code for P1270           [br]1Dh:    Fab code
                               for P1274           [br]others: Reserved
                            */

    UINT32 found : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               These bits identify the Foundary.
                               [br]0Fh:  Foundary code for Intel
                               [br]Else: Reserved
                            */

    UINT32 mstep : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /*
                               On the A-0 stepping, bits 23:16 will be 00h,
                               and will increment for each stepping.
                            */

    UINT32 dot : 4;

                            /* Bits[27:24], Access Type=RO, default=0x00000000*/

                            /*
                               This identifies the DOT process.
                               [br]000:    Code for .0           [br]001:
                               Code for .1           [br]others: Reserved
                            */

    UINT32 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[31:28] */

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MID_HOSTBRIDGE_CFG_STRUCT;

#define MID_HOSTBRIDGE_CFG_WIDTH 32

#define MID_HOSTBRIDGE_CFG_PROC_LSB 0x0000
#define MID_HOSTBRIDGE_CFG_PROC_SIZE 0x0008
#define MID_HOSTBRIDGE_CFG_FOUND_LSB 0x0008
#define MID_HOSTBRIDGE_CFG_FOUND_SIZE 0x0008
#define MID_HOSTBRIDGE_CFG_MSTEP_LSB 0x0010
#define MID_HOSTBRIDGE_CFG_MSTEP_SIZE 0x0008
#define MID_HOSTBRIDGE_CFG_DOT_LSB 0x0018
#define MID_HOSTBRIDGE_CFG_DOT_SIZE 0x0004

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DEBUP0_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/DEBUP0_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.DEBUP0_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.DEBUP0_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Reserved For Future Use.
  */

#define DEBUP0_HOSTBRIDGE_CFG_REG              (0x000000f8U)

typedef union {
  struct {
    UINT32 rsvd_s0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[03:03] */

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 rsvd_s1 : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[07:07] */

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 rsvd_s2 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[11:11] */

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 rsvd_s3 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd_3 : 1; /**< Undefined - auto filled rsvd_[15:15] */

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 rsvd_s4 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd_4 : 1; /**< Undefined - auto filled rsvd_[19:19] */

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 rsvd_rw : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEBUP0_HOSTBRIDGE_CFG_STRUCT;

#define DEBUP0_HOSTBRIDGE_CFG_WIDTH 32

#define DEBUP0_HOSTBRIDGE_CFG_RSVD_S0_LSB 0x0000
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_S0_SIZE 0x0003
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_S1_LSB 0x0004
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_S1_SIZE 0x0003
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_S2_LSB 0x0008
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_S2_SIZE 0x0003
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_S3_LSB 0x000c
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_S3_SIZE 0x0003
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_S4_LSB 0x0010
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_S4_SIZE 0x0003
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_RW_LSB 0x0014
#define DEBUP0_HOSTBRIDGE_CFG_RSVD_RW_SIZE 0x000c

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DEBUP1_SNCU_CFG desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/SNCU_0_0_0_PCI_PRI/DEBUP1_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.DEBUP1_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.SNCU_0_0_0_PCI_PRI_56_71.DEBUP1_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Reserved For Future Use.
  */

#define DEBUP1_HOSTBRIDGE_CFG_REG              (0x000000fcU)

typedef union {
  struct {
    UINT32 rsvd0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd1 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd2 : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd3 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd4 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd5 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd6 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd7 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd8 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd9 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

    UINT32 rsvd10 : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /* Reserved for future usage. */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEBUP1_HOSTBRIDGE_CFG_STRUCT;

#define DEBUP1_HOSTBRIDGE_CFG_WIDTH 32

#define DEBUP1_HOSTBRIDGE_CFG_RSVD0_LSB 0x0000
#define DEBUP1_HOSTBRIDGE_CFG_RSVD0_SIZE 0x0003
#define DEBUP1_HOSTBRIDGE_CFG_RSVD1_LSB 0x0003
#define DEBUP1_HOSTBRIDGE_CFG_RSVD1_SIZE 0x0001
#define DEBUP1_HOSTBRIDGE_CFG_RSVD2_LSB 0x0004
#define DEBUP1_HOSTBRIDGE_CFG_RSVD2_SIZE 0x0003
#define DEBUP1_HOSTBRIDGE_CFG_RSVD3_LSB 0x0007
#define DEBUP1_HOSTBRIDGE_CFG_RSVD3_SIZE 0x0001
#define DEBUP1_HOSTBRIDGE_CFG_RSVD4_LSB 0x0008
#define DEBUP1_HOSTBRIDGE_CFG_RSVD4_SIZE 0x0003
#define DEBUP1_HOSTBRIDGE_CFG_RSVD5_LSB 0x000b
#define DEBUP1_HOSTBRIDGE_CFG_RSVD5_SIZE 0x0001
#define DEBUP1_HOSTBRIDGE_CFG_RSVD6_LSB 0x000c
#define DEBUP1_HOSTBRIDGE_CFG_RSVD6_SIZE 0x0003
#define DEBUP1_HOSTBRIDGE_CFG_RSVD7_LSB 0x000f
#define DEBUP1_HOSTBRIDGE_CFG_RSVD7_SIZE 0x0001
#define DEBUP1_HOSTBRIDGE_CFG_RSVD8_LSB 0x0010
#define DEBUP1_HOSTBRIDGE_CFG_RSVD8_SIZE 0x0003
#define DEBUP1_HOSTBRIDGE_CFG_RSVD9_LSB 0x0013
#define DEBUP1_HOSTBRIDGE_CFG_RSVD9_SIZE 0x0001
#define DEBUP1_HOSTBRIDGE_CFG_RSVD10_LSB 0x0014
#define DEBUP1_HOSTBRIDGE_CFG_RSVD10_SIZE 0x000c

#endif      // SV_HOOKS

//************************************************ /RegisterStructs


#endif      // _HOSTBRIDGE_CFG_h

