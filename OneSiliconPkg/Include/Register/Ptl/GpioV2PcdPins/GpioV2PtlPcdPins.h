/** @file
  GPIO pins definition for PTL_PCD PCH.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _GPIOV2_PINS_PTL_PCD_H_
#define _GPIOV2_PINS_PTL_PCD_H_

#include <Register/GpioV2ChipsetId.h>
#include <Register/GpioAcpiDefines.h>

//
// Unique IDs used in GpioPad defines
//

//
// Community 0
//
#define PTL_PCD_COM_IDX_GPP_V                   0
#define PTL_PCD_GRP_IDX_GPP_V                   0
#define PTL_PCD_COM_IDX_GPP_C                   0
#define PTL_PCD_GRP_IDX_GPP_C                   1

//
// Community 1
//
#define PTL_PCD_COM_IDX_GPP_F                   1
#define PTL_PCD_GRP_IDX_GPP_F                   0
#define PTL_PCD_COM_IDX_GPP_E                   1
#define PTL_PCD_GRP_IDX_GPP_E                   1

//
// Community 3
//
#define PTL_PCD_COM_IDX_CPUJTAG                 2
#define PTL_PCD_GRP_IDX_CPUJTAG                 0
#define PTL_PCD_COM_IDX_GPP_H                   2
#define PTL_PCD_GRP_IDX_GPP_H                   1
#define PTL_PCD_COM_IDX_GPPASPI0                2
#define PTL_PCD_GRP_IDX_GPPASPI0                2
#define PTL_PCD_COM_IDX_VGPIO_3                 2
#define PTL_PCD_GRP_IDX_VGPIO_3                 3

//
// Community 4
//
#define PTL_PCD_COM_IDX_GPP_S                   3
#define PTL_PCD_GRP_IDX_GPP_S                   0

//
// Community 5
//
#define PTL_PCD_COM_IDX_GPP_B                   4
#define PTL_PCD_GRP_IDX_GPP_B                   0
#define PTL_PCD_COM_IDX_GPP_D                   4
#define PTL_PCD_GRP_IDX_GPP_D                   1
#define PTL_PCD_COM_IDX_VGPIO                   4
#define PTL_PCD_GRP_IDX_VGPIO                   2


//
// Group definitions
//

// Com 0
#define PTL_PCD_GROUP_GPP_V                     GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0xFF)
#define PTL_PCD_GROUP_GPP_C                     GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0xFF)

// Com 1
#define PTL_PCD_GROUP_GPP_F                     GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0xFF)
#define PTL_PCD_GROUP_GPP_E                     GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0xFF)
#define PTL_PCD_GROUP_CPUJTAG                   GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0xFF)

// Com 3
#define PTL_PCD_GROUP_GPP_H                     GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0xFF)
#define PTL_PCD_GROUP_GPPASPI0                  GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0xFF)
#define PTL_PCD_GROUP_VGPIO_3                   GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0xFF)

// Com 4
#define PTL_PCD_GROUP_GPP_S                     GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_S, PTL_PCD_GRP_IDX_GPP_S, 0xFF)
#define PTL_PCD_GROUP_GPP_B                     GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0xFF)
#define PTL_PCD_GROUP_GPP_D                     GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0xFF)
#define PTL_PCD_GROUP_VGPIO                     GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0xFF)


//
// Group name: GPP_V
// Community number: 0
// Group index: 0
// Number of pads: 24
//


// Pin:                          xxgpp_v_0 ( Value: 0x001A0000 )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            batlow_b
#define GPIOV2_PTL_PCD_XXGPP_V_0                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x00)

// Pin:                          xxgpp_v_1 ( Value: 0x001A0001 )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            ac_present
#define GPIOV2_PTL_PCD_XXGPP_V_1                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x01)

// Pin:                          xxgpp_v_2 ( Value: 0x001A0002 )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            soc_wake_b
#define GPIOV2_PTL_PCD_XXGPP_V_2                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x02)

// Pin:                          xxgpp_v_3 ( Value: 0x001A0003 )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            pwrbtn_b
#define GPIOV2_PTL_PCD_XXGPP_V_3                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x03)

// Pin:                          xxgpp_v_4 ( Value: 0x001A0004 )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            slp_s3_b
#define GPIOV2_PTL_PCD_XXGPP_V_4                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x04)

// Pin:                          xxgpp_v_5 ( Value: 0x001A0005 )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            slp_s4_b
#define GPIOV2_PTL_PCD_XXGPP_V_5                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x05)

// Pin:                          xxgpp_v_6 ( Value: 0x001A0006 )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            slp_a_b
#define GPIOV2_PTL_PCD_XXGPP_V_6                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x06)

// Pin:                          xxgpp_v_7 ( Value: 0x001A0007 )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            susclk
#define GPIOV2_PTL_PCD_XXGPP_V_7                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x07)

// Pin:                          xxgpp_v_8 ( Value: 0x001A0008 )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            slp_wlan_b
#define GPIOV2_PTL_PCD_XXGPP_V_8                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x08)

// Pin:                          xxgpp_v_9 ( Value: 0x001A0009 )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            slp_s5_b
#define GPIOV2_PTL_PCD_XXGPP_V_9                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x09)

// Pin:                          xxgpp_v_10 ( Value: 0x001A000A )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            lanphypc
#define GPIOV2_PTL_PCD_XXGPP_V_10               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x0A)

// Pin:                          xxgpp_v_11 ( Value: 0x001A000B )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            slp_lan_b
#define GPIOV2_PTL_PCD_XXGPP_V_11               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x0B)

// Pin:                          xxgpp_v_12 ( Value: 0x001A000C )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            wake_b
#define GPIOV2_PTL_PCD_XXGPP_V_12               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x0C)

// Pin:                          xxgpp_v_13 ( Value: 0x001A000D )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            caterr_b
#define GPIOV2_PTL_PCD_XXGPP_V_13               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x0D)

// Pin:                          xxgpp_v_14 ( Value: 0x001A000E )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            forcepr_b
#define GPIOV2_PTL_PCD_XXGPP_V_14               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x0E)

// Pin:                          xxgpp_v_15 ( Value: 0x001A000F )
// Family:                       MiscV
// Default function:             Function 1
// Native function 1:            thermtrip_b
#define GPIOV2_PTL_PCD_XXGPP_V_15               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x0F)

// Pin:                          xxgpp_v_16 ( Value: 0x001A0010 )
// Family:                       MiscV2
// Default function:             Function 1
// Native function 1:            vccst_en
#define GPIOV2_PTL_PCD_XXGPP_V_16               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x10)

// Pin:                          xxgpp_v_17 ( Value: 0x001A0011 )
// Family:                       MiscV2
// Default function:             GP-Out
#define GPIOV2_PTL_PCD_XXGPP_V_17               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x11)

// Pin:                          xxsys_pwrok ( Value: 0x001A0012 )
// Family:                       MiscV2
// Default function:             Function 1
// Native function 1:            xxsys_pwrok
#define GPIOV2_PTL_PCD_XXSYS_PWROK              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x12)

// Pin:                          xxsys_reset_b ( Value: 0x001A0013 )
// Family:                       MiscV2
// Default function:             Function 1
// Native function 1:            xxsys_reset_b
#define GPIOV2_PTL_PCD_XXSYS_RESET_B            GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x13)

// Pin:                          xxl_bklten ( Value: 0x001A0014 )
// Family:                       MiscV2
// Default function:             Function 1
// Native function 1:            xxl_bklten
#define GPIOV2_PTL_PCD_XXL_BKLTEN               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x14)

// Pin:                          xxl_bkltctl ( Value: 0x001A0015 )
// Family:                       MiscV2
// Default function:             Function 1
// Native function 1:            xxl_bkltctl
#define GPIOV2_PTL_PCD_XXL_BKLTCTL              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x15)

// Pin:                          xxl_vdden ( Value: 0x001A0016 )
// Family:                       MiscV2
// Default function:             Function 1
// Native function 1:            xxl_vdden
#define GPIOV2_PTL_PCD_XXL_VDDEN                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x16)

// Pin:                          xxmlk_rst_b ( Value: 0x001A0017 )
// Family:                       MiscV2
// Default function:             Function 1
// Native function 1:            xxmlk_rst_b
#define GPIOV2_PTL_PCD_XXMLK_RST_B              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_V, PTL_PCD_GRP_IDX_GPP_V, 0x17)
//
// Group name: GPP_C
// Community number: 0
// Group index: 1
// Number of pads: 24
//
// Pin:                          xxgpp_c_0 ( Value: 0x001A0080 )
// Family:                       MiscC
// Default function:             Function 1
// Native function 1:            smbclk
// Native function 6:            IOM_GPPC_0
#define GPIOV2_PTL_PCD_XXGPP_C_0                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x00)

// Pin:                          xxgpp_c_1 ( Value: 0x001A0081 )
// Family:                       MiscC
// Default function:             Function 1
// Native function 1:            smbdata
// Native function 6:            IOM_GPPC_1
#define GPIOV2_PTL_PCD_XXGPP_C_1                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x01)

// Pin:                          xxgpp_c_2 ( Value: 0x001A0082 )
// Family:                       MiscC
// Default function:             RW-4'b0000[gpio_sstrap_smbalertb ? 4'b0001 : 4'b0000]
// Native function 1:            smbalert_b
// Native function 6:            IOM_GPPC_2
#define GPIOV2_PTL_PCD_XXGPP_C_2                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x02)

// Pin:                          xxgpp_c_3 ( Value: 0x001A0083 )
// Family:                       MiscC
// Default function:             Function 1
// Native function 1:            sml0clk
// Native function 6:            IOM_GPPC_3
#define GPIOV2_PTL_PCD_XXGPP_C_3                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x03)

// Pin:                          xxgpp_c_4 ( Value: 0x001A0084 )
// Family:                       MiscC
// Default function:             Function 1
// Native function 1:            sml0data
// Native function 6:            IOM_GPPC_4
#define GPIOV2_PTL_PCD_XXGPP_C_4                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x04)

// Pin:                          xxgpp_c_5 ( Value: 0x001A0085 )
// Family:                       MiscC
// Default function:             GP-Out
// Native function 1:            sml0alert_b
// Native function 6:            IOM_GPPC_5
#define GPIOV2_PTL_PCD_XXGPP_C_5                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x05)

// Pin:                          xxgpp_c_6 ( Value: 0x001A0086 )
// Family:                       MiscC
// Default function:             GP-In
// Native function 1:            sml1clk
// Native function 6:            IOM_GPPC_6
#define GPIOV2_PTL_PCD_XXGPP_C_6                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x06)

// Pin:                          xxgpp_c_7 ( Value: 0x001A0087 )
// Family:                       MiscC
// Default function:             GP-In
// Native function 1:            sml1data
// Native function 6:            IOM_GPPC_7
#define GPIOV2_PTL_PCD_XXGPP_C_7                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x07)

// Pin:                          xxgpp_c_8 ( Value: 0x001A0088 )
// Family:                       MiscC
// Default function:             GP-Out
// Native function 1:            sml1alert_b
// Native function 2:            pchhot_b
// Native function 6:            IOM_GPPC_8
#define GPIOV2_PTL_PCD_XXGPP_C_8                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x08)

// Pin:                          xxgpp_c_9 ( Value: 0x001A0089 )
// Family:                       MiscC
// Default function:             GP-In
// Native function 1:            srcclkreq0_b
// Native function 6:            IOM_GPPC_9
#define GPIOV2_PTL_PCD_XXGPP_C_9                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x09)

// Pin:                          xxgpp_c_10 ( Value: 0x001A008A )
// Family:                       MiscC
// Default function:             GP-In
// Native function 1:            srcclkreq1_b
// Native function 6:            IOM_GPPC_10
#define GPIOV2_PTL_PCD_XXGPP_C_10               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x0A)

// Pin:                          xxgpp_c_11 ( Value: 0x001A008B )
// Family:                       MiscC
// Default function:             GP-In
// Native function 1:            srcclkreq2_b
// Native function 6:            IOM_GPPC_11
#define GPIOV2_PTL_PCD_XXGPP_C_11               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x0B)

// Pin:                          xxgpp_c_12 ( Value: 0x001A008C )
// Family:                       MiscC
// Default function:             GP-In
// Native function 1:            srcclkreq3_b
// Native function 6:            IOM_GPPC_12
#define GPIOV2_PTL_PCD_XXGPP_C_12               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x0C)

// Pin:                          xxgpp_c_13 ( Value: 0x001A008D )
// Family:                       MiscC
// Default function:             GP-In
// Native function 1:            srcclkreq4_b
// Native function 6:            IOM_GPPC_13
#define GPIOV2_PTL_PCD_XXGPP_C_13               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x0D)

// Pin:                          xxgpp_c_14 ( Value: 0x001A008E )
// Family:                       MiscC
// Default function:             GP-In
// Native function 1:            srcclkreq5_b
// Native function 6:            IOM_GPPC_14
#define GPIOV2_PTL_PCD_XXGPP_C_14               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x0E)

// Pin:                          xxgpp_c_15 ( Value: 0x001A008F )
// Family:                       MiscC
// Default function:             GP-Out
// Native function 6:            IOM_GPPC_15
#define GPIOV2_PTL_PCD_XXGPP_C_15               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x0F)

// Pin:                          xxgpp_c_16 ( Value: 0x001A0090 )
// Family:                       DDP1
// Default function:             GP-In
// Native function 1:            tbt_lsx0_a
// Native function 2:            ddp1_ctrlclk
// Native function 6:            IOM_GPPC_16
#define GPIOV2_PTL_PCD_XXGPP_C_16               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x10)

// Pin:                          xxgpp_c_17 ( Value: 0x001A0091 )
// Family:                       DDP1
// Default function:             GP-In
// Native function 1:            tbt_lsx0_b
// Native function 2:            ddp1_ctrldata
// Native function 6:            IOM_GPPC_17
#define GPIOV2_PTL_PCD_XXGPP_C_17               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x11)

// Pin:                          xxgpp_c_18 ( Value: 0x001A0092 )
// Family:                       DDP2
// Default function:             GP-In
// Native function 1:            tbt_lsx1_a
// Native function 2:            ddp2_ctrlclk
// Native function 6:            IOM_GPPC_18
#define GPIOV2_PTL_PCD_XXGPP_C_18               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x12)

// Pin:                          xxgpp_c_19 ( Value: 0x001A0093 )
// Family:                       DDP2
// Default function:             GP-In
// Native function 1:            tbt_lsx1_b
// Native function 2:            ddp2_ctrldata
// Native function 6:            IOM_GPPC_19
#define GPIOV2_PTL_PCD_XXGPP_C_19               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x13)

// Pin:                          xxgpp_c_20 ( Value: 0x001A0094 )
// Family:                       DDP3
// Default function:             GP-In
// Native function 1:            tbt_lsx2_a
// Native function 2:            ddp3_ctrlclk
// Native function 6:            IOM_GPPC_20
#define GPIOV2_PTL_PCD_XXGPP_C_20               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x14)

// Pin:                          xxgpp_c_21 ( Value: 0x001A0095 )
// Family:                       DDP3
// Default function:             GP-In
// Native function 1:            tbt_lsx2_b
// Native function 2:            ddp3_ctrldata
// Native function 6:            IOM_GPPC_21
#define GPIOV2_PTL_PCD_XXGPP_C_21               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x15)

// Pin:                          xxgpp_c_22 ( Value: 0x001A0096 )
// Family:                       DDP4
// Default function:             GP-In
// Native function 1:            tbt_lsx3_a
// Native function 2:            ddp4_ctrlclk
// Native function 6:            IOM_GPPC_22
#define GPIOV2_PTL_PCD_XXGPP_C_22               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x16)

// Pin:                          xxgpp_c_23 ( Value: 0x001A0097 )
// Family:                       DDP4
// Default function:             GP-In
// Native function 1:            tbt_lsx3_b
// Native function 2:            ddp4_ctrldata
// Native function 6:            IOM_GPPC_23
#define GPIOV2_PTL_PCD_XXGPP_C_23               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_C, PTL_PCD_GRP_IDX_GPP_C, 0x17)
//
// Group name: GPP_F
// Community number: 1
// Group index: 0
// Number of pads: 26
//
// Pin:                          xxgpp_f_0 ( Value: 0x001A0400 )
// Family:                       CNV_OTHER
// Default function:             Function 1
// Native function 1:            cnv_bri_dt
// Native function 2:            uart2_rts_b
// Native function 6:            IOM_GPPF_0
#define GPIOV2_PTL_PCD_XXGPP_F_0                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x00)

// Pin:                          xxgpp_f_1 ( Value: 0x001A0401 )
// Family:                       CNV_OTHER
// Default function:             Function 1
// Native function 1:            cnv_bri_rsp
// Native function 2:            uart2_rxd
// Native function 6:            IOM_GPPF_1
#define GPIOV2_PTL_PCD_XXGPP_F_1                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x01)

// Pin:                          xxgpp_f_2 ( Value: 0x001A0402 )
// Family:                       CNV_OTHER
// Default function:             Function 1
// Native function 1:            cnv_rgi_dt
// Native function 2:            uart2_txd
// Native function 6:            IOM_GPPF_2
#define GPIOV2_PTL_PCD_XXGPP_F_2                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x02)

// Pin:                          xxgpp_f_3 ( Value: 0x001A0403 )
// Family:                       CNV_OTHER
// Default function:             Function 1
// Native function 1:            cnv_rgi_rsp
// Native function 2:            uart2_cts_b
// Native function 6:            IOM_GPPF_3
#define GPIOV2_PTL_PCD_XXGPP_F_3                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x03)

// Pin:                          xxgpp_f_4 ( Value: 0x001A0404 )
// Family:                       CNV_OTHER
// Default function:             Function 1
// Native function 1:            cnv_rf_reset_b
// Native function 6:            IOM_GPPF_4
#define GPIOV2_PTL_PCD_XXGPP_F_4                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x04)

// Pin:                          xxgpp_f_5 ( Value: 0x001A0405 )
// Family:                       CNV_OTHER
// Default function:             Function 3
// Native function 3:            crf_clkreq
// Native function 6:            IOM_GPPF_5
#define GPIOV2_PTL_PCD_XXGPP_F_5                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x05)

// Pin:                          xxgpp_f_6 ( Value: 0x001A0406 )
// Family:                       CNV_OTHER
// Default function:             GP-In
// Native function 1:            cnv_pa_blanking
// Native function 6:            IOM_GPPF_6
#define GPIOV2_PTL_PCD_XXGPP_F_6                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x06)

// Pin:                          xxgpp_f_7 ( Value: 0x001A0407 )
// Family:                       MiscF
// Default function:             Function 1
// Native function 1:            fusa_diagtest_en
// Native function 2:            imgclkout_2
// Native function 6:            IOM_GPPF_7
#define GPIOV2_PTL_PCD_XXGPP_F_7                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x07)

// Pin:                          xxgpp_f_8 ( Value: 0x001A0408 )
// Family:                       MiscF
// Default function:             Function 1
// Native function 1:            fusa_diagtest_mode
// Native function 6:            IOM_GPPF_8
#define GPIOV2_PTL_PCD_XXGPP_F_8                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x08)

// Pin:                          xxgpp_f_9 ( Value: 0x001A0409 )
// Family:                       MiscF
// Default function:             GP-In
// Native function 2:            sx_exit_holdoff_b
// Native function 4:            ish_gp_11
// Native function 5:            ieh_fatal_err2_b
// Native function 6:            IOM_GPPF_9
#define GPIOV2_PTL_PCD_XXGPP_F_9                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x09)

// Pin:                          xxgpp_f_10 ( Value: 0x001A040A )
// Family:                       MiscF
// Default function:             GP-In
// Native function 6:            IOM_GPPF_10
// Native function 8:            a_ish_gp_6
#define GPIOV2_PTL_PCD_XXGPP_F_10               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x0A)

// Pin:                          xxgpp_f_11 ( Value: 0x001A040B )
// Family:                       THC1_GSPI1
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc1_spi2_clk
// Native function 4:            a_ish_spi_clk
// Native function 5:            gspi1_clk
// Native function 6:            IOM_GPPF_11
// Native function 7:            visa2ch2_clk
// Native function 9:            pti_trace_clk
// Native function 11:           visafusech1_clk
#define GPIOV2_PTL_PCD_XXGPP_F_11               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x0B)

// Pin:                          xxgpp_f_12 ( Value: 0x001A040C )
// Family:                       THC1_GSPI1
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 1:            thc_i2c1_scl
// Native function 2:            i3c2_scl
// Native function 3:            thc1_spi2_io_0
// Native function 4:            a_ish_spi_miso
// Native function 5:            gspi1_mosi
// Native function 6:            IOM_GPPF_12
// Native function 7:            visa2ch2_d0
// Native function 8:            i2c5_scl
// Native function 9:            pti_trace_d00
// Native function 11:           visafusech1_d0
#define GPIOV2_PTL_PCD_XXGPP_F_12               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x0C)

// Pin:                          xxgpp_f_13 ( Value: 0x001A040D )
// Family:                       THC1_GSPI1
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 1:            thc_i2c1_sda
// Native function 2:            i3c2_sda
// Native function 3:            thc1_spi2_io_1
// Native function 4:            a_ish_spi_mosi
// Native function 5:            gspi1_miso
// Native function 6:            IOM_GPPF_13
// Native function 7:            visa2ch2_d1
// Native function 8:            i2c5_sda
// Native function 9:            pti_trace_d01
// Native function 11:           visafusech1_d1
#define GPIOV2_PTL_PCD_XXGPP_F_13               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x0D)

// Pin:                          xxgpp_f_14 ( Value: 0x001A040E )
// Family:                       THC1_A_GSPI0
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc1_spi2_io_2
// Native function 6:            IOM_GPPF_14
// Native function 7:            visa2ch2_d2
// Native function 8:            a_gspi0_mosi
// Native function 9:            pti_trace_d02
// Native function 11:           visafusech1_d2
#define GPIOV2_PTL_PCD_XXGPP_F_14               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x0E)

// Pin:                          xxgpp_f_15 ( Value: 0x001A040F )
// Family:                       THC1_A_GSPI0
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc1_spi2_io_3
// Native function 6:            IOM_GPPF_15
// Native function 7:            visa2ch2_d3
// Native function 8:            a_gspi0_miso
// Native function 9:            pti_trace_d03
// Native function 11:           visafusech1_d3
#define GPIOV2_PTL_PCD_XXGPP_F_15               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x0F)

// Pin:                          xxgpp_f_16 ( Value: 0x001A0410 )
// Family:                       THC1_A_GSPI0
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc1_spi2_rst_b
// Native function 6:            IOM_GPPF_16
// Native function 7:            visa2ch2_d4
// Native function 8:            a_gspi0_clk
// Native function 9:            pti_trace_d04
// Native function 11:           visafusech1_d4
#define GPIOV2_PTL_PCD_XXGPP_F_16               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x10)

// Pin:                          xxgpp_f_17 ( Value: 0x001A0411 )
// Family:                       THC1_GSPI1
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc1_spi2_cs_b
// Native function 4:            a_ish_spi_cs_b
// Native function 5:            gspi1_cs0_b
// Native function 6:            IOM_GPPF_17
// Native function 7:            visa2ch2_d5
// Native function 9:            pti_trace_d05
// Native function 11:           visafusech1_d5
#define GPIOV2_PTL_PCD_XXGPP_F_17               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x11)

// Pin:                          xxgpp_f_18 ( Value: 0x001A0412 )
// Family:                       THC1_A_GSPI0
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc1_spi2_int_b
// Native function 6:            IOM_GPPF_18
// Native function 7:            visa2ch2_d6
// Native function 8:            a_gspi0_cs0_b
// Native function 9:            pti_trace_d06
// Native function 11:           visafusech1_d6
#define GPIOV2_PTL_PCD_XXGPP_F_18               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x12)

// Pin:                          xxgpp_f_19 ( Value: 0x001A0413 )
// Family:                       MiscF
// Default function:             GP-Out
// Native function 6:            IOM_GPPF_19
#define GPIOV2_PTL_PCD_XXGPP_F_19               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x13)

// Pin:                          xxgpp_f_20 ( Value: 0x001A0414 )
// Family:                       MiscF
// Default function:             GP-Out
// Native function 6:            IOM_GPPF_20
#define GPIOV2_PTL_PCD_XXGPP_F_20               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x14)

// Pin:                          xxgpp_f_21 ( Value: 0x001A0415 )
// Family:                       MiscF
// Default function:             GP-Out
// Native function 6:            IOM_GPPF_21
#define GPIOV2_PTL_PCD_XXGPP_F_21               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x15)

// Pin:                          xxgpp_f_22 ( Value: 0x001A0416 )
// Family:                       MiscF
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc1_spi2_dsync
// Native function 5:            ieh_corr_err0_b
// Native function 6:            IOM_GPPF_22
// Native function 7:            visa2ch2_d7
// Native function 8:            a_ish_gp_8
// Native function 9:            pti_trace_d07
// Native function 11:           visafusech1_d7
#define GPIOV2_PTL_PCD_XXGPP_F_22               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x16)

// Pin:                          xxgpp_f_23 ( Value: 0x001A0417 )
// Family:                       MiscF
// Default function:             GP-In
// Native function 5:            ieh_nonfatal_err1_b
// Native function 6:            IOM_GPPF_23
// Native function 8:            a_ish_gp_9
#define GPIOV2_PTL_PCD_XXGPP_F_23               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x17)

// Pin:                          thc1_gspi1_i3c2_clk_loopbk ( Value: 0x001A0418 )
// Family:                       THC1_GSPI1
// Default function:             RW - 4'b0000 [(padmode_value[11] == 4'b0011) ? 4'b0011 : (padmode_value[11] == 4'b0101) ? 4'b0101 : (padmode_value[12] == 4'b0010) ? 4'b0010 : 4'b0000]
// Native function 2:            i3c2_clk_loopbk
// Native function 3:            thc1_spi2_clk_loopbk
// Native function 5:            gspi1_clk_loopbk
#define GPIOV2_PTL_PCD_THC1_GSPI1_I3C2_CLK_LOOPBK GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x18)

// Pin:                          a_gspi0_clk_loopbk ( Value: 0x001A0419 )
// Family:                       THC1_A_GSPI0
// Default function:             Function 8
// Native function 8:            a_gspi0_clk_loopbk
#define GPIOV2_PTL_PCD_A_GSPI0_CLK_LOOPBK       GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_F, PTL_PCD_GRP_IDX_GPP_F, 0x19)


//
// Group name: GPP_E
// Community number: 1
// Group index: 1
// Number of pads: 25
//


// Pin:                          xxgpp_e_0 ( Value: 0x001A0480 )
// Family:                       MiscE
// Default function:             GP-In
// Native function 6:            IOM_GPPE_0
#define GPIOV2_PTL_PCD_XXGPP_E_0                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x00)

// Pin:                          xxgpp_e_1 ( Value: 0x001A0481 )
// Family:                       MiscE
// Default function:             Function 2
// Native function 1:            cpu_gp_2
// Native function 2:            slp_dram_b
// Native function 6:            IOM_GPPE_1
// Native function 8:            a_ish_gp_5
#define GPIOV2_PTL_PCD_XXGPP_E_1                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x01)

// Pin:                          xxgpp_e_2 ( Value: 0x001A0482 )
// Family:                       MiscE
// Default function:             GP-In
// Native function 1:            cpu_gp_3
// Native function 2:            vralert_b
// Native function 4:            ish_gp_10
// Native function 6:            IOM_GPPE_2
#define GPIOV2_PTL_PCD_XXGPP_E_2                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x02)

// Pin:                          xxgpp_e_3 ( Value: 0x001A0483 )
// Family:                       MiscE
// Default function:             GP-In
// Native function 1:            cpu_gp_0
// Native function 6:            IOM_GPPE_3
#define GPIOV2_PTL_PCD_XXGPP_E_3                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x03)

// Pin:                          xxgpp_e_4 ( Value: 0x001A0484 )
// Family:                       MiscE
// Default function:             GP-In
// Native function 6:            IOM_GPPE_4
#define GPIOV2_PTL_PCD_XXGPP_E_4                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x04)

// Pin:                          xxgpp_e_5 ( Value: 0x001A0485 )
// Family:                       MiscE
// Default function:             GP-In
// Native function 4:            ish_gp_7
// Native function 6:            IOM_GPPE_5
#define GPIOV2_PTL_PCD_XXGPP_E_5                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x05)

// Pin:                          xxgpp_e_6 ( Value: 0x001A0486 )
// Family:                       MiscE
// Default function:             GP-Out
// Native function 6:            IOM_GPPE_6
#define GPIOV2_PTL_PCD_XXGPP_E_6                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x06)

// Pin:                          xxgpp_e_7 ( Value: 0x001A0487 )
// Family:                       DDPA
// Default function:             GP-In
// Native function 1:            ddpa_ctrlclk
// Native function 6:            IOM_GPPE_7
#define GPIOV2_PTL_PCD_XXGPP_E_7                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x07)

// Pin:                          xxgpp_e_8 ( Value: 0x001A0488 )
// Family:                       DDPA
// Default function:             GP-In
// Native function 1:            ddpa_ctrldata
// Native function 6:            IOM_GPPE_8
#define GPIOV2_PTL_PCD_XXGPP_E_8                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x08)

// Pin:                          xxgpp_e_9 ( Value: 0x001A0489 )
// Family:                       MiscE
// Default function:             GP-In
// Native function 1:            usb2_oc0_b
// Native function 6:            IOM_GPPE_9
#define GPIOV2_PTL_PCD_XXGPP_E_9                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x09)

// Pin:                          xxgpp_e_10 ( Value: 0x001A048A )
// Family:                       MiscE
// Default function:             GP-In
// Native function 6:            IOM_GPPE_10
#define GPIOV2_PTL_PCD_XXGPP_E_10               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x0A)

// Pin:                          xxgpp_e_11 ( Value: 0x001A048B )
// Family:                       THC0_GSPI0
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc0_spi1_clk
// Native function 5:            gspi0_clk
// Native function 6:            IOM_GPPE_11
// Native function 7:            visa2ch1_clk
// Native function 9:            pti_trace_vld
// Native function 11:           visaossefusech1_clk
#define GPIOV2_PTL_PCD_XXGPP_E_11               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x0B)

// Pin:                          xxgpp_e_12 ( Value: 0x001A048C )
// Family:                       THC0_GSPI0
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 1:            thc_i2c0_scl
// Native function 3:            thc0_spi1_io_0
// Native function 5:            gspi0_mosi
// Native function 6:            IOM_GPPE_12
// Native function 7:            visa2ch1_d0
// Native function 8:            i2c4_scl
// Native function 9:            pti_trace_d08
// Native function 10:           a_cnv_debug_00
// Native function 11:           visaossefusech1_d0
#define GPIOV2_PTL_PCD_XXGPP_E_12               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x0C)

// Pin:                          xxgpp_e_13 ( Value: 0x001A048D )
// Family:                       THC0_GSPI0
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 1:            thc_i2c0_sda
// Native function 3:            thc0_spi1_io_1
// Native function 5:            gspi0_miso
// Native function 6:            IOM_GPPE_13
// Native function 7:            visa2ch1_d1
// Native function 8:            i2c4_sda
// Native function 9:            pti_trace_d09
// Native function 10:           a_cnv_debug_01
// Native function 11:           visaossefusech1_d1
#define GPIOV2_PTL_PCD_XXGPP_E_13               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x0D)

// Pin:                          xxgpp_e_14 ( Value: 0x001A048E )
// Family:                       THC0_MISC
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc0_spi1_io_2
// Native function 6:            IOM_GPPE_14
// Native function 7:            visa2ch1_d2
// Native function 9:            pti_trace_d10
// Native function 10:           a_cnv_debug_02
// Native function 11:           visaossefusech1_d2
#define GPIOV2_PTL_PCD_XXGPP_E_14               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x0E)

// Pin:                          xxgpp_e_15 ( Value: 0x001A048F )
// Family:                       THC0_MISC
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc0_spi1_io_3
// Native function 6:            IOM_GPPE_15
// Native function 7:            visa2ch1_d3
// Native function 9:            pti_trace_d11
// Native function 10:           a_cnv_debug_03
// Native function 11:           visaossefusech1_d3
#define GPIOV2_PTL_PCD_XXGPP_E_15               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x0F)

// Pin:                          xxgpp_e_16 ( Value: 0x001A0490 )
// Family:                       THC0_MISC
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc0_spi1_rst_b
// Native function 6:            IOM_GPPE_16
// Native function 7:            visa2ch1_d4
// Native function 9:            pti_trace_d12
// Native function 10:           a_cnv_debug_04
// Native function 11:           visaossefusech1_d4
#define GPIOV2_PTL_PCD_XXGPP_E_16               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x10)

// Pin:                          xxgpp_e_17 ( Value: 0x001A0491 )
// Family:                       THC0_GSPI0
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc0_spi1_cs_b
// Native function 5:            gspi0_cs0_b
// Native function 6:            IOM_GPPE_17
// Native function 7:            visa2ch1_d5
// Native function 9:            pti_trace_d13
// Native function 10:           a_cnv_debug_05
// Native function 11:           visaossefusech1_d5
#define GPIOV2_PTL_PCD_XXGPP_E_17               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x11)

// Pin:                          xxgpp_e_18 ( Value: 0x001A0492 )
// Family:                       THC0_MISC
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc0_spi1_int_b
// Native function 6:            IOM_GPPE_18
// Native function 7:            visa2ch1_d6
// Native function 9:            pti_trace_d14
// Native function 10:           a_cnv_debug_06
// Native function 11:           visaossefusech1_d6
#define GPIOV2_PTL_PCD_XXGPP_E_18               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x12)

// Pin:                          xxgpp_e_19 ( Value: 0x001A0493 )
// Family:                       MiscE
// Default function:             Function 1
// Native function 1:            pmc_i2c_sda
// Native function 6:            IOM_GPPE_19
#define GPIOV2_PTL_PCD_XXGPP_E_19               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x13)

// Pin:                          xxgpp_e_20 ( Value: 0x001A0494 )
// Family:                       MiscE
// Default function:             Function 1
// Native function 1:            pmc_i2c_scl
// Native function 6:            IOM_GPPE_20
#define GPIOV2_PTL_PCD_XXGPP_E_20               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x14)

// Pin:                          xxgpp_e_21 ( Value: 0x001A0495 )
// Family:                       MiscE
// Default function:             GP-In
// Native function 1:            pmcalert_b
// Native function 6:            IOM_GPPE_21
#define GPIOV2_PTL_PCD_XXGPP_E_21               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x15)

// Pin:                          xxgpp_e_22 ( Value: 0x001A0496 )
// Family:                       MiscE
// Default function:             RW - 4'b0000 [gpcom_strap_dfxtestmode_active_b ? 4'b0000 : 4'b1011]
// Native function 3:            thc0_spi1_dsync
// Native function 6:            IOM_GPPE_22
// Native function 7:            visa2ch1_d7
// Native function 9:            pti_trace_d15
// Native function 10:           a_cnv_debug_07
// Native function 11:           visaossefusech1_d7
#define GPIOV2_PTL_PCD_XXGPP_E_22               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x16)

// Pin:                          xxboothalt_b ( Value: 0x001A0497 )
// Family:                       MiscE
// Default function:             Function 1
// Native function 1:            xxboothalt_b
#define GPIOV2_PTL_PCD_XXBOOTHALT_B             GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x17)

// Pin:                          thc0_gspi0_clk_loopbk ( Value: 0x001A0498 )
// Family:                       THC0_GSPI0
// Default function:             RW - 4'b0000 [(padmode_value[37] == 4'b0011) ? 4'b0011 : (padmode_value[37] == 4'b0101) ? 4'b0101 : 4'b0000]
// Native function 3:            thc0_spi1_clk_loopbk
// Native function 5:            gspi0_clk_loopbk
#define GPIOV2_PTL_PCD_THC0_GSPI0_CLK_LOOPBK    GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_E, PTL_PCD_GRP_IDX_GPP_E, 0x18)


//
// Group name: CPUJTAG
// Community number: 3
// Group index: 0
// Number of pads: 15
//


// Pin:                          xxepd_on ( Value: 0x001A0800 )
// Family:                       PM
// Default function:             Function 1
// Native function 1:            xxepd_on
#define GPIOV2_PTL_PCD_XXEPD_ON                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x00)

// Pin:                          xxvdd2_pwrgd ( Value: 0x001A0801 )
// Family:                       PM
// Default function:             Function 1
// Native function 1:            xxvdd2_pwrgd
#define GPIOV2_PTL_PCD_XXVDD2_PWRGD             GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x01)

// Pin:                          xxjtag_mbpb0 ( Value: 0x001A0802 )
// Family:                       JTAG4
// Default function:             Function 1
// Native function 1:            xxjtag_mbpb0
#define GPIOV2_PTL_PCD_XXJTAG_MBPB0             GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x02)

// Pin:                          xxjtag_mbpb1 ( Value: 0x001A0803 )
// Family:                       JTAG4
// Default function:             Function 1
// Native function 1:            xxjtag_mbpb1
#define GPIOV2_PTL_PCD_XXJTAG_MBPB1             GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x03)

// Pin:                          xxjtag_mbpb2 ( Value: 0x001A0804 )
// Family:                       JTAG4
// Default function:             Function 1
// Native function 1:            xxjtag_mbpb2
#define GPIOV2_PTL_PCD_XXJTAG_MBPB2             GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x04)

// Pin:                          xxjtag_mbpb3 ( Value: 0x001A0805 )
// Family:                       JTAG4
// Default function:             Function 1
// Native function 1:            xxjtag_mbpb3
#define GPIOV2_PTL_PCD_XXJTAG_MBPB3             GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x05)

// Pin:                          xxjtag_tdo ( Value: 0x001A0806 )
// Family:                       JTAG0
// Default function:             Function 1
// Native function 1:            xxjtag_tdo
#define GPIOV2_PTL_PCD_XXJTAG_TDO               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x06)

// Pin:                          xxprdy_b ( Value: 0x001A0807 )
// Family:                       JTAG2
// Default function:             Function 1
// Native function 1:            xxprdy_b
#define GPIOV2_PTL_PCD_XXPRDY_B                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x07)

// Pin:                          xxpreq_b ( Value: 0x001A0808 )
// Family:                       JTAG2
// Default function:             Function 1
// Native function 1:            xxpreq_b
#define GPIOV2_PTL_PCD_XXPREQ_B                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x08)

// Pin:                          xxjtag_tdi ( Value: 0x001A0809 )
// Family:                       JTAG0
// Default function:             Function 1
// Native function 1:            xxjtag_tdi
#define GPIOV2_PTL_PCD_XXJTAG_TDI               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x09)

// Pin:                          xxjtag_tms ( Value: 0x001A080A )
// Family:                       JTAG0
// Default function:             Function 1
// Native function 1:            xxjtag_tms
#define GPIOV2_PTL_PCD_XXJTAG_TMS               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x0A)

// Pin:                          xxjtag_tck ( Value: 0x001A080B )
// Family:                       JTAG1
// Default function:             Function 1
// Native function 1:            xxjtag_tck
#define GPIOV2_PTL_PCD_XXJTAG_TCK               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x0B)

// Pin:                          xxdbg_pmode ( Value: 0x001A080C )
// Family:                       JTAG3
// Default function:             Function 1
// Native function 1:            xxdbg_pmode
#define GPIOV2_PTL_PCD_XXDBG_PMODE              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x0C)

// Pin:                          xxjtag_trst_b ( Value: 0x001A080D )
// Family:                       JTAG1
// Default function:             Function 1
// Native function 1:            xxjtag_trst_b
#define GPIOV2_PTL_PCD_XXJTAG_TRST_B            GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x0D)

// Pin:                          xxddsp_hpdalv ( Value: 0x001A080E )
// Family:                       JTAG4
// Default function:             Function 1
// Native function 1:            xxddsp_hpdalv
// Native function 2:            disp_misca
#define GPIOV2_PTL_PCD_XXDDSP_HPDALV            GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_CPUJTAG, PTL_PCD_GRP_IDX_CPUJTAG, 0x0E)


//
// Group name: GPP_H
// Community number: 3
// Group index: 1
// Number of pads: 28
//


// Pin:                          xxgpp_h_0 ( Value: 0x001A0880 )
// Family:                       MiscH
// Default function:             GP-Out
// Native function 6:            IOM_GPPH_0
#define GPIOV2_PTL_PCD_XXGPP_H_0                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x00)

// Pin:                          xxgpp_h_1 ( Value: 0x001A0881 )
// Family:                       MiscH
// Default function:             GP-Out
// Native function 6:            IOM_GPPH_1
#define GPIOV2_PTL_PCD_XXGPP_H_1                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x01)

// Pin:                          xxgpp_h_2 ( Value: 0x001A0882 )
// Family:                       MiscH
// Default function:             GP-Out
// Native function 6:            IOM_GPPH_2
#define GPIOV2_PTL_PCD_XXGPP_H_2                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x02)

// Pin:                          xxgpp_h_3 ( Value: 0x001A0883 )
// Family:                       MiscH
// Default function:             GP-In
// Native function 1:            mic_mute
// Native function 6:            IOM_GPPH_3
#define GPIOV2_PTL_PCD_XXGPP_H_3                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x03)

// Pin:                          xxgpp_h_4 ( Value: 0x001A0884 )
// Family:                       I2C23
// Default function:             GP-In
// Native function 1:            i2c2_sda
// Native function 2:            cnv_mfuart2_rxd
// Native function 6:            IOM_GPPH_4
#define GPIOV2_PTL_PCD_XXGPP_H_4                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x04)

// Pin:                          xxgpp_h_5 ( Value: 0x001A0885 )
// Family:                       I2C23
// Default function:             GP-In
// Native function 1:            i2c2_scl
// Native function 2:            cnv_mfuart2_txd
// Native function 6:            IOM_GPPH_5
#define GPIOV2_PTL_PCD_XXGPP_H_5                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x05)

// Pin:                          xxgpp_h_6 ( Value: 0x001A0886 )
// Family:                       I2C23
// Default function:             GP-In
// Native function 1:            i2c3_sda
// Native function 2:            uart1_rxd
// Native function 3:            a_ish_uart1_rxd
// Native function 6:            IOM_GPPH_6
#define GPIOV2_PTL_PCD_XXGPP_H_6                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x06)

// Pin:                          xxgpp_h_7 ( Value: 0x001A0887 )
// Family:                       I2C23
// Default function:             GP-In
// Native function 1:            i2c3_scl
// Native function 2:            uart1_txd
// Native function 3:            a_ish_uart1_txd
// Native function 6:            IOM_GPPH_7
#define GPIOV2_PTL_PCD_XXGPP_H_7                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x07)

// Pin:                          xxgpp_h_8 ( Value: 0x001A0888 )
// Family:                       MiscH
// Default function:             GP-In
// Native function 1:            uart0_rxd
// Native function 6:            IOM_GPPH_8
#define GPIOV2_PTL_PCD_XXGPP_H_8                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x08)

// Pin:                          xxgpp_h_9 ( Value: 0x001A0889 )
// Family:                       MiscH
// Default function:             GP-In
// Native function 1:            uart0_txd
// Native function 6:            IOM_GPPH_9
#define GPIOV2_PTL_PCD_XXGPP_H_9                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x09)

// Pin:                          xxgpp_h_10 ( Value: 0x001A088A )
// Family:                       A_I3C1
// Default function:             GP-In
// Native function 1:            uart0_rts_b
// Native function 2:            a_i3c1_sda
// Native function 6:            IOM_GPPH_10
// Native function 8:            a_ish_gp_10
#define GPIOV2_PTL_PCD_XXGPP_H_10               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x0A)

// Pin:                          xxgpp_h_11 ( Value: 0x001A088B )
// Family:                       A_I3C1
// Default function:             GP-In
// Native function 1:            uart0_cts_b
// Native function 2:            a_i3c1_scl
// Native function 6:            IOM_GPPH_11
// Native function 8:            a_ish_gp_11
#define GPIOV2_PTL_PCD_XXGPP_H_11               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x0B)

// Pin:                          xxgpp_h_12 ( Value: 0x001A088C )
// Family:                       MiscH
// Default function:             GP-In
// Native function 6:            IOM_GPPH_12
#define GPIOV2_PTL_PCD_XXGPP_H_12               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x0C)

// Pin:                          xxgpp_h_13 ( Value: 0x001A088D )
// Family:                       MiscH
// Default function:             Function 1
// Native function 1:            cpu_c10_gate_b
// Native function 6:            IOM_GPPH_13
#define GPIOV2_PTL_PCD_XXGPP_H_13               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x0D)

// Pin:                          xxgpp_h_14 ( Value: 0x001A088E )
// Family:                       ISH_I3C1
// Default function:             GP-In
// Native function 1:            ish_uart1_rxd
// Native function 2:            a_uart1_rxd
// Native function 3:            ish_i2c1_sda
// Native function 4:            ish_i3c1_sda
// Native function 6:            IOM_GPPH_14
#define GPIOV2_PTL_PCD_XXGPP_H_14               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x0E)

// Pin:                          xxgpp_h_15 ( Value: 0x001A088F )
// Family:                       ISH_I3C1
// Default function:             GP-In
// Native function 1:            ish_uart1_txd
// Native function 2:            a_uart1_txd
// Native function 3:            ish_i2c1_scl
// Native function 4:            ish_i3c1_scl
// Native function 6:            IOM_GPPH_15
#define GPIOV2_PTL_PCD_XXGPP_H_15               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x0F)

// Pin:                          xxgpp_h_16 ( Value: 0x001A0890 )
// Family:                       MiscH
// Default function:             GP-In
// Native function 1:            tbt_lsx2_oe
// Native function 3:            pcie_lnk_down
// Native function 6:            IOM_GPPH_16
#define GPIOV2_PTL_PCD_XXGPP_H_16               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x10)

// Pin:                          xxgpp_h_17 ( Value: 0x001A0891 )
// Family:                       MiscH
// Default function:             GP-Out
// Native function 1:            mic_mute_led
// Native function 6:            IOM_GPPH_17
#define GPIOV2_PTL_PCD_XXGPP_H_17               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x11)

// Pin:                          xxgpp_h_18 ( Value: 0x001A0892 )
// Family:                       MiscH
// Default function:             GP-In
// Native function 6:            IOM_GPPH_18
#define GPIOV2_PTL_PCD_XXGPP_H_18               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x12)

// Pin:                          xxgpp_h_19 ( Value: 0x001A0893 )
// Family:                       I3C0
// Default function:             GP-In
// Native function 1:            i2c0_sda
// Native function 2:            i3c0_sda
// Native function 6:            IOM_GPPH_19
#define GPIOV2_PTL_PCD_XXGPP_H_19               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x13)

// Pin:                          xxgpp_h_20 ( Value: 0x001A0894 )
// Family:                       I3C0
// Default function:             GP-In
// Native function 1:            i2c0_scl
// Native function 2:            i3c0_scl
// Native function 6:            IOM_GPPH_20
#define GPIOV2_PTL_PCD_XXGPP_H_20               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x14)

// Pin:                          xxgpp_h_21 ( Value: 0x001A0895 )
// Family:                       I3C1
// Default function:             GP-In
// Native function 1:            i2c1_sda
// Native function 2:            i3c1_sda
// Native function 6:            IOM_GPPH_21
#define GPIOV2_PTL_PCD_XXGPP_H_21               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x15)

// Pin:                          xxgpp_h_22 ( Value: 0x001A0896 )
// Family:                       I3C1
// Default function:             GP-Out
// Native function 1:            i2c1_scl
// Native function 2:            i3c1_scl
// Native function 6:            IOM_GPPH_22
#define GPIOV2_PTL_PCD_XXGPP_H_22               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x16)

// Pin:                          xxgpp_h_23 ( Value: 0x001A0897 )
// Family:                       MiscH
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_cs4_b
// Native function 6:            IOM_GPPH_23
#define GPIOV2_PTL_PCD_XXGPP_H_23               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x17)

// Pin:                          xxgpp_h_24 ( Value: 0x001A0898 )
// Family:                       MiscH
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_alert4_b
// Native function 6:            IOM_GPPH_24
#define GPIOV2_PTL_PCD_XXGPP_H_24               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x18)

// Pin:                          lpi3c1_clk_loopbk ( Value: 0x001A0899 )
// Family:                       I3C1
// Default function:             RW - 4'b0000 [(padmode_value[37] == 4'b0010) ? 4'b0010 : (padmode_value[26] == 4'b0010) ? 4'b0011 : 4'b0000]
// Native function 2:            i3c1_clk_loopbk
// Native function 3:            a_i3c1_clk_loopbk
#define GPIOV2_PTL_PCD_LPI3C1_CLK_LOOPBK        GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x19)

// Pin:                          lpi3c0_clk_loopbk ( Value: 0x001A089A )
// Family:                       I3C0
// Default function:             Function 2
// Native function 2:            i3c0_clk_loopbk
#define GPIOV2_PTL_PCD_LPI3C0_CLK_LOOPBK        GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x1A)

// Pin:                          ishi3c1_clk_loopbk ( Value: 0x001A089B )
// Family:                       ISH_I3C1
// Default function:             Function 4
// Native function 4:            ish_i3c1_clk_loopbk
#define GPIOV2_PTL_PCD_ISHI3C1_CLK_LOOPBK       GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_H, PTL_PCD_GRP_IDX_GPP_H, 0x1B)


//
// Group name: GPPASPI0
// Community number: 3
// Group index: 2
// Number of pads: 28
//


// Pin:                          xxgpp_a_0 ( Value: 0x001A0900 )
// Family:                       eSPI
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_io_0
// Native function 6:            IOM_GPPA_0
#define GPIOV2_PTL_PCD_XXGPP_A_0                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x00)

// Pin:                          xxgpp_a_1 ( Value: 0x001A0901 )
// Family:                       eSPI
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_io_1
// Native function 6:            IOM_GPPA_1
#define GPIOV2_PTL_PCD_XXGPP_A_1                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x01)

// Pin:                          xxgpp_a_2 ( Value: 0x001A0902 )
// Family:                       eSPI
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0010 : 4'b0001]
// Native function 1:            espi_io_2
// Native function 2:            pripwrdnack
// Native function 6:            IOM_GPPA_2
#define GPIOV2_PTL_PCD_XXGPP_A_2                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x02)

// Pin:                          xxgpp_a_3 ( Value: 0x001A0903 )
// Family:                       eSPI
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0010 : 4'b0001]
// Native function 1:            espi_io_3
// Native function 2:            priack_b
// Native function 6:            IOM_GPPA_3
#define GPIOV2_PTL_PCD_XXGPP_A_3                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x03)

// Pin:                          xxgpp_a_4 ( Value: 0x001A0904 )
// Family:                       eSPI
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_cs0_b
// Native function 6:            IOM_GPPA_4
#define GPIOV2_PTL_PCD_XXGPP_A_4                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x04)

// Pin:                          xxgpp_a_5 ( Value: 0x001A0905 )
// Family:                       eSPI
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_clk
// Native function 6:            IOM_GPPA_5
#define GPIOV2_PTL_PCD_XXGPP_A_5                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x05)

// Pin:                          xxgpp_a_6 ( Value: 0x001A0906 )
// Family:                       eSPI
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_reset_b
// Native function 6:            IOM_GPPA_6
#define GPIOV2_PTL_PCD_XXGPP_A_6                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x06)

// Pin:                          xxgpp_a_7 ( Value: 0x001A0907 )
// Family:                       MiscA
// Default function:             GP-In
// Native function 6:            IOM_GPPA_7
#define GPIOV2_PTL_PCD_XXGPP_A_7                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x07)

// Pin:                          xxgpp_a_8 ( Value: 0x001A0908 )
// Family:                       MiscA
// Default function:             GP-In
// Native function 1:            osse_smlclk
// Native function 6:            IOM_GPPA_8
#define GPIOV2_PTL_PCD_XXGPP_A_8                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x08)

// Pin:                          xxgpp_a_9 ( Value: 0x001A0909 )
// Family:                       MiscA
// Default function:             GP-In
// Native function 1:            osse_smldata
// Native function 6:            IOM_GPPA_9
#define GPIOV2_PTL_PCD_XXGPP_A_9                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x09)

// Pin:                          xxgpp_a_10 ( Value: 0x001A090A )
// Family:                       MiscA
// Default function:             GP-In
// Native function 1:            osse_smlalert_b
// Native function 6:            IOM_GPPA_10
#define GPIOV2_PTL_PCD_XXGPP_A_10               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x0A)

// Pin:                          xxgpp_a_11 ( Value: 0x001A090B )
// Family:                       MiscA
// Default function:             GP-In
// Native function 6:            IOM_GPPA_11
#define GPIOV2_PTL_PCD_XXGPP_A_11               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x0B)

// Pin:                          xxgpp_a_12 ( Value: 0x001A090C )
// Family:                       MiscA
// Default function:             GP-In
// Native function 6:            IOM_GPPA_12
#define GPIOV2_PTL_PCD_XXGPP_A_12               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x0C)

// Pin:                          xxgpp_a_13 ( Value: 0x001A090D )
// Family:                       eSPI
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_cs1_b
// Native function 6:            IOM_GPPA_13
#define GPIOV2_PTL_PCD_XXGPP_A_13               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x0D)

// Pin:                          xxgpp_a_14 ( Value: 0x001A090E )
// Family:                       MiscA
// Default function:             GP-In
// Native function 1:            adr_complete
// Native function 6:            IOM_GPPA_14
#define GPIOV2_PTL_PCD_XXGPP_A_14               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x0E)

// Pin:                          xxgpp_a_15 ( Value: 0x001A090F )
// Family:                       MiscA
// Default function:             Function 1
// Native function 1:            dnx_force_reload
// Native function 6:            IOM_GPPA_15
#define GPIOV2_PTL_PCD_XXGPP_A_15               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x0F)

// Pin:                          xxgpp_a_16 ( Value: 0x001A0910 )
// Family:                       eSPI
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_cs2_b
// Native function 6:            IOM_GPPA_16
#define GPIOV2_PTL_PCD_XXGPP_A_16               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x10)

// Pin:                          xxgpp_a_17 ( Value: 0x001A0911 )
// Family:                       eSPI
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_cs3_b
// Native function 6:            IOM_GPPA_17
#define GPIOV2_PTL_PCD_XXGPP_A_17               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x11)

// Pin:                          xxspi0_io_2 ( Value: 0x001A0912 )
// Family:                       SPI
// Default function:             Function 1
// Native function 1:            xxspi0_io_2
#define GPIOV2_PTL_PCD_XXSPI0_IO_2              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x12)

// Pin:                          xxspi0_io_3 ( Value: 0x001A0913 )
// Family:                       SPI
// Default function:             Function 1
// Native function 1:            xxspi0_io_3
#define GPIOV2_PTL_PCD_XXSPI0_IO_3              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x13)

// Pin:                          xxspi0_mosi_io_0 ( Value: 0x001A0914 )
// Family:                       SPI
// Default function:             Function 1
// Native function 1:            xxspi0_mosi_io_0
#define GPIOV2_PTL_PCD_XXSPI0_MOSI_IO_0         GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x14)

// Pin:                          xxspi0_miso_io_1 ( Value: 0x001A0915 )
// Family:                       SPI
// Default function:             Function 1
// Native function 1:            xxspi0_miso_io_1
#define GPIOV2_PTL_PCD_XXSPI0_MISO_IO_1         GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x15)

// Pin:                          xxspi0_tpm_cs_b ( Value: 0x001A0916 )
// Family:                       SPI
// Default function:             Function 1
// Native function 1:            xxspi0_tpm_cs_b
#define GPIOV2_PTL_PCD_XXSPI0_TPM_CS_B          GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x16)

// Pin:                          xxspi0_flash_0_cs_b ( Value: 0x001A0917 )
// Family:                       SPI
// Default function:             Function 1
// Native function 1:            xxspi0_flash_0_cs_b
#define GPIOV2_PTL_PCD_XXSPI0_FLASH_0_CS_B      GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x17)

// Pin:                          xxspi0_flash_1_cs_b ( Value: 0x001A0918 )
// Family:                       SPI
// Default function:             Function 1
// Native function 1:            xxspi0_flash_1_cs_b
#define GPIOV2_PTL_PCD_XXSPI0_FLASH_1_CS_B      GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x18)

// Pin:                          xxspi0_clk ( Value: 0x001A0919 )
// Family:                       SPI
// Default function:             Function 1
// Native function 1:            xxspi0_clk
#define GPIOV2_PTL_PCD_XXSPI0_CLK               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x19)

// Pin:                          espi_clk_loopbk ( Value: 0x001A091A )
// Family:                       eSPI
// Default function:             Function 1
// Native function 1:            espi_clk_loopbk
#define GPIOV2_PTL_PCD_ESPI_CLK_LOOPBK          GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x1A)

// Pin:                          spi0_clk_loopbk ( Value: 0x001A091B )
// Family:                       SPI
// Default function:             Function 1
// Native function 1:            spi0_clk_loopbk
#define GPIOV2_PTL_PCD_SPI0_CLK_LOOPBK          GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPPASPI0, PTL_PCD_GRP_IDX_GPPASPI0, 0x1B)


//
// Group name: vGPIO_3
// Community number: 3
// Group index: 3
// Number of pads: 14
//


// Pin:                          vGPIO_USB_0 ( Value: 0x001A0980 )
// Family:                       v_PCIE
// Default function:             Function 1
// Native function 1:            ESPI_USB_OCB_0
#define GPIOV2_PTL_PCD_VGPIO_USB_0              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x00)

// Pin:                          vGPIO_USB_1 ( Value: 0x001A0981 )
// Family:                       v_PCIE
// Default function:             Function 1
// Native function 1:            ESPI_USB_OCB_1
#define GPIOV2_PTL_PCD_VGPIO_USB_1              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x01)

// Pin:                          vGPIO_USB_2 ( Value: 0x001A0982 )
// Family:                       v_PCIE
// Default function:             Function 1
// Native function 1:            ESPI_USB_OCB_2
#define GPIOV2_PTL_PCD_VGPIO_USB_2              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x02)

// Pin:                          vGPIO_USB_3 ( Value: 0x001A0983 )
// Family:                       v_PCIE
// Default function:             Function 1
// Native function 1:            ESPI_USB_OCB_3
#define GPIOV2_PTL_PCD_VGPIO_USB_3              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x03)

// Pin:                          vGPIO_USB_4 ( Value: 0x001A0984 )
// Family:                       v_PCIE
// Default function:             Function 1
// Native function 1:            USB_CPU_OCB_0
#define GPIOV2_PTL_PCD_VGPIO_USB_4              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x04)

// Pin:                          vGPIO_USB_5 ( Value: 0x001A0985 )
// Family:                       v_PCIE
// Default function:             Function 1
// Native function 1:            USB_CPU_OCB_1
#define GPIOV2_PTL_PCD_VGPIO_USB_5              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x05)

// Pin:                          vGPIO_USB_6 ( Value: 0x001A0986 )
// Family:                       v_PCIE
// Default function:             Function 1
// Native function 1:            USB_CPU_OCB_2
#define GPIOV2_PTL_PCD_VGPIO_USB_6              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x06)

// Pin:                          vGPIO_USB_7 ( Value: 0x001A0987 )
// Family:                       v_PCIE
// Default function:             Function 1
// Native function 1:            USB_CPU_OCB_3
#define GPIOV2_PTL_PCD_VGPIO_USB_7              GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x07)

// Pin:                          vGPIO_TS0 ( Value: 0x001A0988 )
// Family:                       v_PCIE
// Default function:             GP-In
#define GPIOV2_PTL_PCD_VGPIO_TS0                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x08)

// Pin:                          vGPIO_TS1 ( Value: 0x001A0989 )
// Family:                       v_PCIE
// Default function:             GP-In
#define GPIOV2_PTL_PCD_VGPIO_TS1                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x09)

// Pin:                          vGPIO_THC0 ( Value: 0x001A098A )
// Family:                       v_PCIE
// Default function:             GP-In
#define GPIOV2_PTL_PCD_VGPIO_THC0               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x0A)

// Pin:                          vGPIO_THC1 ( Value: 0x001A098B )
// Family:                       v_PCIE
// Default function:             GP-In
#define GPIOV2_PTL_PCD_VGPIO_THC1               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x0B)

// Pin:                          vGPIO_THC2 ( Value: 0x001A098C )
// Family:                       v_PCIE
// Default function:             GP-In
#define GPIOV2_PTL_PCD_VGPIO_THC2               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x0C)

// Pin:                          vGPIO_THC3 ( Value: 0x001A098D )
// Family:                       v_PCIE
// Default function:             GP-In
#define GPIOV2_PTL_PCD_VGPIO_THC3               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO_3, PTL_PCD_GRP_IDX_VGPIO_3, 0x0D)


//
// Group name: GPP_S
// Community number: 4
// Group index: 0
// Number of pads: 8
//


// Pin:                          xxgpp_s_0 ( Value: 0x001A0C00 )
// Family:                       SWND
// Default function:             GP-In
// Native function 1:            sndw3_clk
// Native function 6:            i2s1_txd
#define GPIOV2_PTL_PCD_XXGPP_S_0                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_S, PTL_PCD_GRP_IDX_GPP_S, 0x00)

// Pin:                          xxgpp_s_1 ( Value: 0x001A0C01 )
// Family:                       SWND
// Default function:             GP-In
// Native function 1:            sndw3_data0
// Native function 6:            i2s1_rxd
#define GPIOV2_PTL_PCD_XXGPP_S_1                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_S, PTL_PCD_GRP_IDX_GPP_S, 0x01)

// Pin:                          xxgpp_s_2 ( Value: 0x001A0C02 )
// Family:                       SWND
// Default function:             GP-In
// Native function 1:            sndw3_data1
// Native function 3:            sndw0_clk
// Native function 5:            dmic_clk_a_0
// Native function 6:            i2s1_sclk
#define GPIOV2_PTL_PCD_XXGPP_S_2                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_S, PTL_PCD_GRP_IDX_GPP_S, 0x02)

// Pin:                          xxgpp_s_3 ( Value: 0x001A0C03 )
// Family:                       SWND
// Default function:             GP-In
// Native function 1:            sndw3_data2
// Native function 2:            sndw2_data1
// Native function 3:            sndw0_data0
// Native function 5:            dmic_data_0
// Native function 6:            i2s1_sfrm
#define GPIOV2_PTL_PCD_XXGPP_S_3                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_S, PTL_PCD_GRP_IDX_GPP_S, 0x03)

// Pin:                          xxgpp_s_4 ( Value: 0x001A0C04 )
// Family:                       SWND
// Default function:             GP-In
// Native function 2:            sndw2_clk
// Native function 5:            dmic_clk_a_0
// Native function 6:            i2s2_sclk
#define GPIOV2_PTL_PCD_XXGPP_S_4                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_S, PTL_PCD_GRP_IDX_GPP_S, 0x04)

// Pin:                          xxgpp_s_5 ( Value: 0x001A0C05 )
// Family:                       SWND
// Default function:             GP-In
// Native function 2:            sndw2_data0
// Native function 5:            dmic_data_0
// Native function 6:            i2s2_sfrm
#define GPIOV2_PTL_PCD_XXGPP_S_5                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_S, PTL_PCD_GRP_IDX_GPP_S, 0x05)

// Pin:                          xxgpp_s_6 ( Value: 0x001A0C06 )
// Family:                       SWND
// Default function:             GP-In
// Native function 2:            sndw2_data1
// Native function 3:            sndw1_clk
// Native function 5:            dmic_clk_a_1
// Native function 6:            i2s2_txd
#define GPIOV2_PTL_PCD_XXGPP_S_6                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_S, PTL_PCD_GRP_IDX_GPP_S, 0x06)

// Pin:                          xxgpp_s_7 ( Value: 0x001A0C07 )
// Family:                       SWND
// Default function:             GP-In
// Native function 1:            sndw3_data3
// Native function 2:            sndw2_data2
// Native function 3:            sndw1_data0
// Native function 5:            dmic_data_1
// Native function 6:            i2s2_rxd
#define GPIOV2_PTL_PCD_XXGPP_S_7                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_S, PTL_PCD_GRP_IDX_GPP_S, 0x07)


//
// Group name: GPP_B
// Community number: 5
// Group index: 0
// Number of pads: 27
//


// Pin:                          xxgpp_b_0 ( Value: 0x001A1000 )
// Family:                       MiscB
// Default function:             GP-In
// Native function 1:            usbc_smlclk
// Native function 6:            IOM_GPPB_0
#define GPIOV2_PTL_PCD_XXGPP_B_0                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x00)

// Pin:                          xxgpp_b_1 ( Value: 0x001A1001 )
// Family:                       MiscB
// Default function:             GP-In
// Native function 1:            usbc_smldata
// Native function 6:            IOM_GPPB_1
#define GPIOV2_PTL_PCD_XXGPP_B_1                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x01)

// Pin:                          xxgpp_b_2 ( Value: 0x001A1002 )
// Family:                       ISH_I3C0
// Default function:             GP-In
// Native function 3:            ish_i2c0_sda
// Native function 4:            ish_i3c0_sda
// Native function 6:            IOM_GPPB_2
// Native function 7:            visa2ch3_d0
// Native function 8:            a_i2c2_sda
// Native function 9:            pti_trace_d00
// Native function 10:           cnv_debug_00
#define GPIOV2_PTL_PCD_XXGPP_B_2                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x02)

// Pin:                          xxgpp_b_3 ( Value: 0x001A1003 )
// Family:                       ISH_I3C0
// Default function:             GP-In
// Native function 3:            ish_i2c0_scl
// Native function 4:            ish_i3c0_scl
// Native function 6:            IOM_GPPB_3
// Native function 7:            visa2ch3_d1
// Native function 8:            a_i2c2_scl
// Native function 9:            pti_trace_d01
// Native function 10:           cnv_debug_01
#define GPIOV2_PTL_PCD_XXGPP_B_3                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x03)

// Pin:                          xxgpp_b_4 ( Value: 0x001A1004 )
// Family:                       MiscB
// Default function:             GP-Out
// Native function 2:            Blink
// Native function 3:            Serial_Blink
// Native function 4:            ish_gp_0
// Native function 6:            IOM_GPPB_4
// Native function 7:            visa2ch3_d2
// Native function 9:            pti_trace_d02
// Native function 10:           cnv_debug_02
#define GPIOV2_PTL_PCD_XXGPP_B_4                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x04)

// Pin:                          xxgpp_b_5 ( Value: 0x001A1005 )
// Family:                       MiscB
// Default function:             GP-In
// Native function 2:            Blink
// Native function 3:            Serial_Blink
// Native function 4:            ish_gp_1
// Native function 6:            IOM_GPPB_5
// Native function 7:            visa2ch3_d3
// Native function 9:            pti_trace_d03
// Native function 10:           cnv_debug_03
#define GPIOV2_PTL_PCD_XXGPP_B_5                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x05)

// Pin:                          xxgpp_b_6 ( Value: 0x001A1006 )
// Family:                       MiscB
// Default function:             GP-In
// Native function 2:            Blink
// Native function 3:            Serial_Blink
// Native function 4:            ish_gp_2
// Native function 6:            IOM_GPPB_6
// Native function 7:            visa2ch3_d4
// Native function 9:            pti_trace_d04
// Native function 10:           cnv_debug_04
#define GPIOV2_PTL_PCD_XXGPP_B_6                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x06)

// Pin:                          xxgpp_b_7 ( Value: 0x001A1007 )
// Family:                       MiscB
// Default function:             GP-In
// Native function 2:            Blink
// Native function 3:            Serial_Blink
// Native function 4:            ish_gp_3
// Native function 6:            IOM_GPPB_7
// Native function 7:            visa2ch3_d5
// Native function 9:            pti_trace_d05
// Native function 10:           cnv_debug_05
#define GPIOV2_PTL_PCD_XXGPP_B_7                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x07)

// Pin:                          xxgpp_b_8 ( Value: 0x001A1008 )
// Family:                       MiscB
// Default function:             GP-In
// Native function 2:            Blink
// Native function 3:            Serial_Blink
// Native function 4:            ish_gp_4
// Native function 6:            IOM_GPPB_8
// Native function 7:            visa2ch3_d6
// Native function 9:            pti_trace_d06
// Native function 10:           cnv_debug_06
#define GPIOV2_PTL_PCD_XXGPP_B_8                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x08)

// Pin:                          xxgpp_b_9 ( Value: 0x001A1009 )
// Family:                       MiscB
// Default function:             GP-In
// Native function 2:            ddsp_hpd1
// Native function 3:            disp_misc1
// Native function 6:            IOM_GPPB_9
#define GPIOV2_PTL_PCD_XXGPP_B_9                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x09)

// Pin:                          xxgpp_b_10 ( Value: 0x001A100A )
// Family:                       MiscB
// Default function:             GP-In
// Native function 2:            ddsp_hpd2
// Native function 3:            disp_misc2
// Native function 6:            IOM_GPPB_10
#define GPIOV2_PTL_PCD_XXGPP_B_10               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x0A)

// Pin:                          xxgpp_b_11 ( Value: 0x001A100B )
// Family:                       MiscB
// Default function:             GP-In
// Native function 1:            usb2_oc1_b
// Native function 2:            ddsp_hpd3
// Native function 3:            disp_misc3
// Native function 6:            IOM_GPPB_11
#define GPIOV2_PTL_PCD_XXGPP_B_11               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x0B)

// Pin:                          xxgpp_b_12 ( Value: 0x001A100C )
// Family:                       MiscB
// Default function:             Function 1
// Native function 1:            slp_s0_b
// Native function 6:            IOM_GPPB_12
#define GPIOV2_PTL_PCD_XXGPP_B_12               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x0C)

// Pin:                          xxgpp_b_13 ( Value: 0x001A100D )
// Family:                       MiscB
// Default function:             Function 1
// Native function 1:            pltrst_b
// Native function 6:            IOM_GPPB_13
#define GPIOV2_PTL_PCD_XXGPP_B_13               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x0D)

// Pin:                          xxgpp_b_14 ( Value: 0x001A100E )
// Family:                       MiscB
// Default function:             GP-Out
// Native function 1:            usb2_oc2_b
// Native function 2:            ddsp_hpd4
// Native function 3:            disp_misc4
// Native function 6:            IOM_GPPB_14
#define GPIOV2_PTL_PCD_XXGPP_B_14               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x0E)

// Pin:                          xxgpp_b_15 ( Value: 0x001A100F )
// Family:                       MiscB
// Default function:             GP-In
// Native function 1:            usb2_oc3_b
// Native function 6:            IOM_GPPB_15
#define GPIOV2_PTL_PCD_XXGPP_B_15               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x0F)

// Pin:                          xxgpp_b_16 ( Value: 0x001A1010 )
// Family:                       MiscB
// Default function:             GP-In
// Native function 1:            tbt_lsx1_oe
// Native function 6:            IOM_GPPB_16
#define GPIOV2_PTL_PCD_XXGPP_B_16               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x10)

// Pin:                          xxgpp_b_17 ( Value: 0x001A1011 )
// Family:                       MiscB
// Default function:             GP-In
// Native function 1:            tbt_lsx0_oe
// Native function 2:            l_vdden2
// Native function 6:            IOM_GPPB_17
// Native function 7:            visa2ch4_d0
// Native function 9:            pti_trace_d08
#define GPIOV2_PTL_PCD_XXGPP_B_17               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x11)

// Pin:                          xxgpp_b_18 ( Value: 0x001A1012 )
// Family:                       A_I2C4
// Default function:             GP-In
// Native function 1:            ish_i2c2_sda
// Native function 2:            a_i2c4_sda
// Native function 3:            cnv_mfuart0_rxd
// Native function 6:            IOM_GPPB_18
// Native function 7:            visa2ch3_d7
// Native function 9:            pti_trace_d07
// Native function 10:           cnv_debug_07
#define GPIOV2_PTL_PCD_XXGPP_B_18               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x12)

// Pin:                          xxgpp_b_19 ( Value: 0x001A1013 )
// Family:                       A_I2C4
// Default function:             GP-In
// Native function 1:            ish_i2c2_scl
// Native function 2:            a_i2c4_scl
// Native function 3:            cnv_mfuart0_txd
// Native function 6:            IOM_GPPB_19
// Native function 7:            visa2ch3_clk
// Native function 9:            pti_trace_clk
#define GPIOV2_PTL_PCD_XXGPP_B_19               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x13)

// Pin:                          xxgpp_b_20 ( Value: 0x001A1014 )
// Family:                       A_I2C5
// Default function:             GP-In
// Native function 2:            a_i2c5_sda
// Native function 3:            cnv_mfuart0_rts_b
// Native function 4:            ish_gp_8
// Native function 6:            IOM_GPPB_20
// Native function 7:            visa2ch4_d1
// Native function 9:            pti_trace_d09
#define GPIOV2_PTL_PCD_XXGPP_B_20               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x14)

// Pin:                          xxgpp_b_21 ( Value: 0x001A1015 )
// Family:                       A_I2C5
// Default function:             GP-In
// Native function 2:            a_i2c5_scl
// Native function 3:            cnv_mfuart0_cts_b
// Native function 4:            ish_gp_9
// Native function 6:            IOM_GPPB_21
// Native function 7:            visa2ch4_d2
// Native function 9:            pti_trace_d10
#define GPIOV2_PTL_PCD_XXGPP_B_21               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x15)

// Pin:                          xxgpp_b_22 ( Value: 0x001A1016 )
// Family:                       MiscB
// Default function:             GP-Out
// Native function 1:            time_sync_0
// Native function 4:            ish_gp_5
// Native function 6:            IOM_GPPB_22
#define GPIOV2_PTL_PCD_XXGPP_B_22               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x16)

// Pin:                          xxgpp_b_23 ( Value: 0x001A1017 )
// Family:                       MiscB
// Default function:             GP-Out
// Native function 1:            time_sync_1
// Native function 4:            ish_gp_6
// Native function 6:            IOM_GPPB_23
// Native function 7:            visa2ch4_d3
// Native function 9:            pti_trace_d11
#define GPIOV2_PTL_PCD_XXGPP_B_23               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x17)

// Pin:                          xxgpp_b_24 ( Value: 0x001A1018 )
// Family:                       MiscB
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_alert0_b
// Native function 6:            IOM_GPPB_24
#define GPIOV2_PTL_PCD_XXGPP_B_24               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x18)

// Pin:                          xxgpp_b_25 ( Value: 0x001A1019 )
// Family:                       MiscB
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_alert1_b
// Native function 6:            IOM_GPPB_25
#define GPIOV2_PTL_PCD_XXGPP_B_25               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x19)

// Pin:                          ishi3c0_clk_loopbk ( Value: 0x001A101A )
// Family:                       ISH_I3C0
// Default function:             Function 4
// Native function 4:            ish_i3c0_clk_loopbk
#define GPIOV2_PTL_PCD_ISHI3C0_CLK_LOOPBK       GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_B, PTL_PCD_GRP_IDX_GPP_B, 0x1A)


//
// Group name: GPP_D
// Community number: 5
// Group index: 1
// Number of pads: 27
//


// Pin:                          xxgpp_d_0 ( Value: 0x001A1080 )
// Family:                       MiscD
// Default function:             GP-In
// Native function 1:            imgclkout_1
// Native function 6:            IOM_GPPD_0
#define GPIOV2_PTL_PCD_XXGPP_D_0                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x00)

// Pin:                          xxgpp_d_1 ( Value: 0x001A1081 )
// Family:                       MiscD
// Default function:             GP-In
// Native function 1:            a_i2c3_sda
// Native function 2:            l_bklten2
// Native function 3:            a_ish_i2c2_sda
// Native function 6:            IOM_GPPD_1
#define GPIOV2_PTL_PCD_XXGPP_D_1                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x01)

// Pin:                          xxgpp_d_2 ( Value: 0x001A1082 )
// Family:                       MiscD
// Default function:             GP-In
// Native function 1:            a_i2c3_scl
// Native function 2:            l_bkltctl2
// Native function 3:            a_ish_i2c2_scl
// Native function 6:            IOM_GPPD_2
#define GPIOV2_PTL_PCD_XXGPP_D_2                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x02)

// Pin:                          xxgpp_d_3 ( Value: 0x001A1083 )
// Family:                       MiscD
// Default function:             GP-In
// Native function 1:            cpu_gp_1
// Native function 6:            IOM_GPPD_3
// Native function 7:            visa2ch4_d4
// Native function 9:            pti_trace_d12
#define GPIOV2_PTL_PCD_XXGPP_D_3                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x03)

// Pin:                          xxgpp_d_4 ( Value: 0x001A1084 )
// Family:                       MiscD
// Default function:             GP-In
// Native function 1:            imgclkout_0
// Native function 6:            IOM_GPPD_4
#define GPIOV2_PTL_PCD_XXGPP_D_4                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x04)

// Pin:                          xxgpp_d_5 ( Value: 0x001A1085 )
// Family:                       ISH_SPI
// Default function:             GP-In
// Native function 2:            ish_uart0_rxd
// Native function 3:            ish_spi_cs_b
// Native function 5:            sml0bdata
// Native function 6:            IOM_GPPD_5
// Native function 7:            visa2ch4_d5
// Native function 9:            pti_trace_d13
#define GPIOV2_PTL_PCD_XXGPP_D_5                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x05)

// Pin:                          xxgpp_d_6 ( Value: 0x001A1086 )
// Family:                       ISH_SPI
// Default function:             GP-In
// Native function 2:            ish_uart0_txd
// Native function 3:            ish_spi_clk
// Native function 5:            sml0bclk
// Native function 6:            IOM_GPPD_6
// Native function 7:            visa2ch4_d6
// Native function 9:            pti_trace_d14
#define GPIOV2_PTL_PCD_XXGPP_D_6                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x06)

// Pin:                          xxgpp_d_7 ( Value: 0x001A1087 )
// Family:                       ISH_SPI
// Default function:             GP-In
// Native function 2:            ish_uart0_rts_b
// Native function 3:            ish_spi_miso
// Native function 6:            IOM_GPPD_7
// Native function 7:            visa2ch4_d7
// Native function 9:            pti_trace_d15
#define GPIOV2_PTL_PCD_XXGPP_D_7                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x07)

// Pin:                          xxgpp_d_8 ( Value: 0x001A1088 )
// Family:                       ISH_SPI
// Default function:             GP-In
// Native function 2:            ish_uart0_cts_b
// Native function 3:            ish_spi_mosi
// Native function 5:            sml0balert_b
// Native function 6:            IOM_GPPD_8
// Native function 7:            visa2ch4_clk
// Native function 9:            pti_trace_vld
#define GPIOV2_PTL_PCD_XXGPP_D_8                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x08)

// Pin:                          xxgpp_d_9 ( Value: 0x001A1089 )
// Family:                       MiscD
// Default function:             GP-In
// Native function 2:            i2s_mclk1_out
// Native function 6:            IOM_GPPD_9
#define GPIOV2_PTL_PCD_XXGPP_D_9                GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x09)

// Pin:                          xxgpp_d_10 ( Value: 0x001A108A )
// Family:                       AZA_I2S0
// Default function:             Function 1
// Native function 1:            hda_bclk
// Native function 2:            i2s0_sclk
// Native function 4:            hdacpu_bclk
// Native function 6:            IOM_GPPD_10
#define GPIOV2_PTL_PCD_XXGPP_D_10               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x0A)

// Pin:                          xxgpp_d_11 ( Value: 0x001A108B )
// Family:                       AZA_I2S0
// Default function:             Function 1
// Native function 1:            hda_sync
// Native function 2:            i2s0_sfrm
// Native function 6:            IOM_GPPD_11
#define GPIOV2_PTL_PCD_XXGPP_D_11               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x0B)

// Pin:                          xxgpp_d_12 ( Value: 0x001A108C )
// Family:                       AZA_I2S0
// Default function:             Function 1
// Native function 1:            hda_sdo
// Native function 2:            i2s0_txd
// Native function 4:            hdacpu_sdo
// Native function 6:            IOM_GPPD_12
#define GPIOV2_PTL_PCD_XXGPP_D_12               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x0C)

// Pin:                          xxgpp_d_13 ( Value: 0x001A108D )
// Family:                       AZA_I2S0
// Default function:             Function 1
// Native function 1:            hda_sdi_0
// Native function 2:            i2s0_rxd
// Native function 4:            hdacpu_sdi
// Native function 6:            IOM_GPPD_13
#define GPIOV2_PTL_PCD_XXGPP_D_13               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x0D)

// Pin:                          xxgpp_d_14 ( Value: 0x001A108E )
// Family:                       MiscD
// Default function:             GP-In
// Native function 2:            tbt_lsx3_oe
// Native function 6:            IOM_GPPD_14
#define GPIOV2_PTL_PCD_XXGPP_D_14               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x0E)

// Pin:                          xxgpp_d_15 ( Value: 0x001A108F )
// Family:                       MiscD
// Default function:             GP-In
// Native function 6:            IOM_GPPD_15
#define GPIOV2_PTL_PCD_XXGPP_D_15               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x0F)

// Pin:                          xxgpp_d_16 ( Value: 0x001A1090 )
// Family:                       AZA_I2S0
// Default function:             GP-In
// Native function 1:            hda_rst_b
// Native function 3:            dmic_clk_a_1
// Native function 6:            IOM_GPPD_16
#define GPIOV2_PTL_PCD_XXGPP_D_16               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x10)

// Pin:                          xxgpp_d_17 ( Value: 0x001A1091 )
// Family:                       AZA_I2S0
// Default function:             Function 1
// Native function 1:            hda_sdi_1
// Native function 3:            dmic_data_1
// Native function 6:            IOM_GPPD_17
#define GPIOV2_PTL_PCD_XXGPP_D_17               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x11)

// Pin:                          xxgpp_d_18 ( Value: 0x001A1092 )
// Family:                       MiscD
// Default function:             GP-In
// Native function 1:            srcclkreq6_b
// Native function 6:            IOM_GPPD_18
#define GPIOV2_PTL_PCD_XXGPP_D_18               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x12)

// Pin:                          xxgpp_d_19 ( Value: 0x001A1093 )
// Family:                       MiscD
// Default function:             GP-In
// Native function 2:            tbt_lsx0_oe
// Native function 6:            IOM_GPPD_19
#define GPIOV2_PTL_PCD_XXGPP_D_19               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x13)

// Pin:                          xxgpp_d_20 ( Value: 0x001A1094 )
// Family:                       MiscD
// Default function:             GP-In
// Native function 1:            srcclkreq7_b
// Native function 6:            IOM_GPPD_20
#define GPIOV2_PTL_PCD_XXGPP_D_20               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x14)

// Pin:                          xxgpp_d_21 ( Value: 0x001A1095 )
// Family:                       MiscD
// Default function:             Function 1
// Native function 1:            ufs_refclk
// Native function 2:            srcclkreq8_b
// Native function 6:            IOM_GPPD_21
#define GPIOV2_PTL_PCD_XXGPP_D_21               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x15)

// Pin:                          xxgpp_d_22 ( Value: 0x001A1096 )
// Family:                       CCDIF
// Default function:             Function 1
// Native function 1:            bpki3c_sda
// Native function 6:            IOM_GPPD_22
#define GPIOV2_PTL_PCD_XXGPP_D_22               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x16)

// Pin:                          xxgpp_d_23 ( Value: 0x001A1097 )
// Family:                       CCDIF
// Default function:             Function 1
// Native function 1:            bpki3c_scl
// Native function 6:            IOM_GPPD_23
#define GPIOV2_PTL_PCD_XXGPP_D_23               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x17)

// Pin:                          xxgpp_d_24 ( Value: 0x001A1098 )
// Family:                       MiscD
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_alert2_b
// Native function 6:            IOM_GPPD_24
#define GPIOV2_PTL_PCD_XXGPP_D_24               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x18)

// Pin:                          xxgpp_d_25 ( Value: 0x001A1099 )
// Family:                       MiscD
// Default function:             RW - 4'b0001 [gpcom_strap_espi_disable ? 4'b0000 : 4'b0001]
// Native function 1:            espi_alert3_b
// Native function 6:            IOM_GPPD_25
#define GPIOV2_PTL_PCD_XXGPP_D_25               GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x19)

// Pin:                          shd_i3c_clk_loopbk ( Value: 0x001A109A )
// Family:                       CCDIF
// Default function:             Function 4
// Native function 4:            shd_i3c_clk_loopbk
#define GPIOV2_PTL_PCD_SHD_I3C_CLK_LOOPBK       GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_GPP_D, PTL_PCD_GRP_IDX_GPP_D, 0x1A)


//
// Group name: vGPIO
// Community number: 5
// Group index: 2
// Number of pads: 18
//


// Pin:                          vGPIO_0 ( Value: 0x001A1100 )
// Family:                       v_CNV
// Default function:             GP-Out
#define GPIOV2_PTL_PCD_VGPIO_0                  GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x00)

// Pin:                          vGPIO_5 ( Value: 0x001A1101 )
// Family:                       v_CNV
// Default function:             GP-Out
#define GPIOV2_PTL_PCD_VGPIO_5                  GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x01)

// Pin:                          vGPIO_30 ( Value: 0x001A1102 )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            i2s0_sclk
// Native function 2:            i2s1_sclk
// Native function 3:            i2s2_sclk
#define GPIOV2_PTL_PCD_VGPIO_30                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x02)

// Pin:                          vGPIO_31 ( Value: 0x001A1103 )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            i2s0_sfrm
// Native function 2:            i2s1_sfrm
// Native function 3:            i2s2_sfrm
#define GPIOV2_PTL_PCD_VGPIO_31                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x03)

// Pin:                          vGPIO_32 ( Value: 0x001A1104 )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            i2s0_rxd
// Native function 2:            i2s1_rxd
// Native function 3:            i2s2_rxd
#define GPIOV2_PTL_PCD_VGPIO_32                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x04)

// Pin:                          vGPIO_33 ( Value: 0x001A1105 )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            i2s0_txd
// Native function 2:            i2s1_txd
// Native function 3:            i2s2_txd
#define GPIOV2_PTL_PCD_VGPIO_33                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x05)

// Pin:                          vGPIO_34 ( Value: 0x001A1106 )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            cnv_bt_i2s_bclk
#define GPIOV2_PTL_PCD_VGPIO_34                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x06)

// Pin:                          vGPIO_35 ( Value: 0x001A1107 )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            cnv_bt_i2s_ws_sync
#define GPIOV2_PTL_PCD_VGPIO_35                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x07)

// Pin:                          vGPIO_36 ( Value: 0x001A1108 )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            cnv_bt_i2s_sdi
#define GPIOV2_PTL_PCD_VGPIO_36                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x08)

// Pin:                          vGPIO_37 ( Value: 0x001A1109 )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            cnv_bt_i2s_sdo
#define GPIOV2_PTL_PCD_VGPIO_37                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x09)

// Pin:                          vGPIO_40 ( Value: 0x001A110A )
// Family:                       v_CNV
// Default function:             Function 2
// Native function 1:            i2s0_sclk
// Native function 2:            i2s1_sclk
// Native function 3:            i2s2_sclk
#define GPIOV2_PTL_PCD_VGPIO_40                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x0A)

// Pin:                          vGPIO_41 ( Value: 0x001A110B )
// Family:                       v_CNV
// Default function:             Function 2
// Native function 1:            i2s0_sfrm
// Native function 2:            i2s1_sfrm
// Native function 3:            i2s2_sfrm
#define GPIOV2_PTL_PCD_VGPIO_41                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x0B)

// Pin:                          vGPIO_42 ( Value: 0x001A110C )
// Family:                       v_CNV
// Default function:             Function 2
// Native function 1:            i2s0_rxd
// Native function 2:            i2s1_rxd
// Native function 3:            i2s2_rxd
#define GPIOV2_PTL_PCD_VGPIO_42                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x0C)

// Pin:                          vGPIO_43 ( Value: 0x001A110D )
// Family:                       v_CNV
// Default function:             Function 2
// Native function 1:            i2s0_txd
// Native function 2:            i2s1_txd
// Native function 3:            i2s2_txd
#define GPIOV2_PTL_PCD_VGPIO_43                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x0D)

// Pin:                          vGPIO_44 ( Value: 0x001A110E )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            cnv_bt_i2s_bclk_2
#define GPIOV2_PTL_PCD_VGPIO_44                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x0E)

// Pin:                          vGPIO_45 ( Value: 0x001A110F )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            cnv_bt_i2s_ws_sync_2
#define GPIOV2_PTL_PCD_VGPIO_45                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x0F)

// Pin:                          vGPIO_46 ( Value: 0x001A1110 )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            cnv_bt_i2s_sdi_2
#define GPIOV2_PTL_PCD_VGPIO_46                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x10)

// Pin:                          vGPIO_47 ( Value: 0x001A1111 )
// Family:                       v_CNV
// Default function:             Function 1
// Native function 1:            cnv_bt_i2s_sdo_2
#define GPIOV2_PTL_PCD_VGPIO_47                 GPIOV2_PAD_ID (0, GPIOV2_PTL_PCD_CHIPSET_ID, 0, PTL_PCD_COM_IDX_VGPIO, PTL_PCD_GRP_IDX_VGPIO, 0x11)


//
// Pin muxing definitions for Group: GPP_V.
//

// Group: GPP_V, Pad: xxgpp_v_0, Function: 1 - batlow_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_0__BATLOW_B                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_0, 1)
// Group: GPP_V, Pad: xxgpp_v_1, Function: 1 - ac_present
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_1__AC_PRESENT                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_1, 1)
// Group: GPP_V, Pad: xxgpp_v_2, Function: 1 - soc_wake_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_2__SOC_WAKE_B                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_2, 1)
// Group: GPP_V, Pad: xxgpp_v_3, Function: 1 - pwrbtn_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_3__PWRBTN_B                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_3, 1)
// Group: GPP_V, Pad: xxgpp_v_4, Function: 1 - slp_s3_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_4__SLP_S3_B                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_4, 1)
// Group: GPP_V, Pad: xxgpp_v_5, Function: 1 - slp_s4_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_5__SLP_S4_B                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_5, 1)
// Group: GPP_V, Pad: xxgpp_v_6, Function: 1 - slp_a_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_6__SLP_A_B                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_6, 1)
// Group: GPP_V, Pad: xxgpp_v_7, Function: 1 - susclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_7__SUSCLK                     GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_7, 1)
// Group: GPP_V, Pad: xxgpp_v_8, Function: 1 - slp_wlan_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_8__SLP_WLAN_B                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_8, 1)
// Group: GPP_V, Pad: xxgpp_v_9, Function: 1 - slp_s5_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_9__SLP_S5_B                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_9, 1)
// Group: GPP_V, Pad: xxgpp_v_10, Function: 1 - lanphypc
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_10__LANPHYPC                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_10, 1)
// Group: GPP_V, Pad: xxgpp_v_11, Function: 1 - slp_lan_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_11__SLP_LAN_B                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_11, 1)
// Group: GPP_V, Pad: xxgpp_v_12, Function: 1 - wake_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_12__WAKE_B                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_12, 1)
// Group: GPP_V, Pad: xxgpp_v_13, Function: 1 - caterr_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_13__CATERR_B                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_13, 1)
// Group: GPP_V, Pad: xxgpp_v_14, Function: 1 - forcepr_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_14__FORCEPR_B                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_14, 1)
// Group: GPP_V, Pad: xxgpp_v_15, Function: 1 - thermtrip_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_15__THERMTRIP_B               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_15, 1)
// Group: GPP_V, Pad: xxgpp_v_16, Function: 1 - vccst_en
#define GPIOV2_PTL_PCD_MUXING__XXGPP_V_16__VCCST_EN                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_V_16, 1)
// Group: GPP_V, Pad: xxsys_pwrok, Function: 1 - xxsys_pwrok
#define GPIOV2_PTL_PCD_MUXING__XXSYS_PWROK__XXSYS_PWROK              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXSYS_PWROK, 1)
// Group: GPP_V, Pad: xxsys_reset_b, Function: 1 - xxsys_reset_b
#define GPIOV2_PTL_PCD_MUXING__XXSYS_RESET_B__XXSYS_RESET_B          GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXSYS_RESET_B, 1)
// Group: GPP_V, Pad: xxl_bklten, Function: 1 - xxl_bklten
#define GPIOV2_PTL_PCD_MUXING__XXL_BKLTEN__XXL_BKLTEN                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXL_BKLTEN, 1)
// Group: GPP_V, Pad: xxl_bkltctl, Function: 1 - xxl_bkltctl
#define GPIOV2_PTL_PCD_MUXING__XXL_BKLTCTL__XXL_BKLTCTL              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXL_BKLTCTL, 1)
// Group: GPP_V, Pad: xxl_vdden, Function: 1 - xxl_vdden
#define GPIOV2_PTL_PCD_MUXING__XXL_VDDEN__XXL_VDDEN                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXL_VDDEN, 1)
// Group: GPP_V, Pad: xxmlk_rst_b, Function: 1 - xxmlk_rst_b
#define GPIOV2_PTL_PCD_MUXING__XXMLK_RST_B__XXMLK_RST_B              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXMLK_RST_B, 1)

//
// Pin muxing definitions for Group: GPP_C.
//

// Group: GPP_C, Pad: xxgpp_c_0, Function: 1 - smbclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_0__SMBCLK                     GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_0, 1)
// Group: GPP_C, Pad: xxgpp_c_0, Function: 6 - IOM_GPPC_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_0__IOM_GPPC_0                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_0, 6)
// Group: GPP_C, Pad: xxgpp_c_1, Function: 1 - smbdata
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_1__SMBDATA                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_1, 1)
// Group: GPP_C, Pad: xxgpp_c_1, Function: 6 - IOM_GPPC_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_1__IOM_GPPC_1                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_1, 6)
// Group: GPP_C, Pad: xxgpp_c_2, Function: 1 - smbalert_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_2__SMBALERT_B                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_2, 1)
// Group: GPP_C, Pad: xxgpp_c_2, Function: 6 - IOM_GPPC_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_2__IOM_GPPC_2                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_2, 6)
// Group: GPP_C, Pad: xxgpp_c_3, Function: 1 - sml0clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_3__SML0CLK                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_3, 1)
// Group: GPP_C, Pad: xxgpp_c_3, Function: 6 - IOM_GPPC_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_3__IOM_GPPC_3                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_3, 6)
// Group: GPP_C, Pad: xxgpp_c_4, Function: 1 - sml0data
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_4__SML0DATA                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_4, 1)
// Group: GPP_C, Pad: xxgpp_c_4, Function: 6 - IOM_GPPC_4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_4__IOM_GPPC_4                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_4, 6)
// Group: GPP_C, Pad: xxgpp_c_5, Function: 1 - sml0alert_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_5__SML0ALERT_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_5, 1)
// Group: GPP_C, Pad: xxgpp_c_5, Function: 6 - IOM_GPPC_5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_5__IOM_GPPC_5                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_5, 6)
// Group: GPP_C, Pad: xxgpp_c_6, Function: 1 - sml1clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_6__SML1CLK                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_6, 1)
// Group: GPP_C, Pad: xxgpp_c_6, Function: 6 - IOM_GPPC_6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_6__IOM_GPPC_6                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_6, 6)
// Group: GPP_C, Pad: xxgpp_c_7, Function: 1 - sml1data
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_7__SML1DATA                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_7, 1)
// Group: GPP_C, Pad: xxgpp_c_7, Function: 6 - IOM_GPPC_7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_7__IOM_GPPC_7                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_7, 6)
// Group: GPP_C, Pad: xxgpp_c_8, Function: 1 - sml1alert_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_8__SML1ALERT_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_8, 1)
// Group: GPP_C, Pad: xxgpp_c_8, Function: 2 - pchhot_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_8__PCHHOT_B                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_8, 2)
// Group: GPP_C, Pad: xxgpp_c_8, Function: 6 - IOM_GPPC_8
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_8__IOM_GPPC_8                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_8, 6)
// Group: GPP_C, Pad: xxgpp_c_9, Function: 1 - srcclkreq0_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_9__SRCCLKREQ0_B               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_9, 1)
// Group: GPP_C, Pad: xxgpp_c_9, Function: 6 - IOM_GPPC_9
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_9__IOM_GPPC_9                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_9, 6)
// Group: GPP_C, Pad: xxgpp_c_10, Function: 1 - srcclkreq1_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_10__SRCCLKREQ1_B              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_10, 1)
// Group: GPP_C, Pad: xxgpp_c_10, Function: 6 - IOM_GPPC_10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_10__IOM_GPPC_10               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_10, 6)
// Group: GPP_C, Pad: xxgpp_c_11, Function: 1 - srcclkreq2_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_11__SRCCLKREQ2_B              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_11, 1)
// Group: GPP_C, Pad: xxgpp_c_11, Function: 6 - IOM_GPPC_11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_11__IOM_GPPC_11               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_11, 6)
// Group: GPP_C, Pad: xxgpp_c_12, Function: 1 - srcclkreq3_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_12__SRCCLKREQ3_B              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_12, 1)
// Group: GPP_C, Pad: xxgpp_c_12, Function: 6 - IOM_GPPC_12
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_12__IOM_GPPC_12               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_12, 6)
// Group: GPP_C, Pad: xxgpp_c_13, Function: 1 - srcclkreq4_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_13__SRCCLKREQ4_B              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_13, 1)
// Group: GPP_C, Pad: xxgpp_c_13, Function: 6 - IOM_GPPC_13
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_13__IOM_GPPC_13               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_13, 6)
// Group: GPP_C, Pad: xxgpp_c_14, Function: 1 - srcclkreq5_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_14__SRCCLKREQ5_B              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_14, 1)
// Group: GPP_C, Pad: xxgpp_c_14, Function: 6 - IOM_GPPC_14
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_14__IOM_GPPC_14               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_14, 6)
// Group: GPP_C, Pad: xxgpp_c_15, Function: 6 - IOM_GPPC_15
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_15__IOM_GPPC_15               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_15, 6)
// Group: GPP_C, Pad: xxgpp_c_16, Function: 1 - tbt_lsx0_a
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_16__TBT_LSX0_A                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_16, 1)
// Group: GPP_C, Pad: xxgpp_c_16, Function: 2 - ddp1_ctrlclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_16__DDP1_CTRLCLK              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_16, 2)
// Group: GPP_C, Pad: xxgpp_c_16, Function: 6 - IOM_GPPC_16
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_16__IOM_GPPC_16               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_16, 6)
// Group: GPP_C, Pad: xxgpp_c_17, Function: 1 - tbt_lsx0_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_17__TBT_LSX0_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_17, 1)
// Group: GPP_C, Pad: xxgpp_c_17, Function: 2 - ddp1_ctrldata
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_17__DDP1_CTRLDATA             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_17, 2)
// Group: GPP_C, Pad: xxgpp_c_17, Function: 6 - IOM_GPPC_17
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_17__IOM_GPPC_17               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_17, 6)
// Group: GPP_C, Pad: xxgpp_c_18, Function: 1 - tbt_lsx1_a
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_18__TBT_LSX1_A                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_18, 1)
// Group: GPP_C, Pad: xxgpp_c_18, Function: 2 - ddp2_ctrlclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_18__DDP2_CTRLCLK              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_18, 2)
// Group: GPP_C, Pad: xxgpp_c_18, Function: 6 - IOM_GPPC_18
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_18__IOM_GPPC_18               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_18, 6)
// Group: GPP_C, Pad: xxgpp_c_19, Function: 1 - tbt_lsx1_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_19__TBT_LSX1_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_19, 1)
// Group: GPP_C, Pad: xxgpp_c_19, Function: 2 - ddp2_ctrldata
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_19__DDP2_CTRLDATA             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_19, 2)
// Group: GPP_C, Pad: xxgpp_c_19, Function: 6 - IOM_GPPC_19
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_19__IOM_GPPC_19               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_19, 6)
// Group: GPP_C, Pad: xxgpp_c_20, Function: 1 - tbt_lsx2_a
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_20__TBT_LSX2_A                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_20, 1)
// Group: GPP_C, Pad: xxgpp_c_20, Function: 2 - ddp3_ctrlclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_20__DDP3_CTRLCLK              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_20, 2)
// Group: GPP_C, Pad: xxgpp_c_20, Function: 6 - IOM_GPPC_20
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_20__IOM_GPPC_20               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_20, 6)
// Group: GPP_C, Pad: xxgpp_c_21, Function: 1 - tbt_lsx2_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_21__TBT_LSX2_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_21, 1)
// Group: GPP_C, Pad: xxgpp_c_21, Function: 2 - ddp3_ctrldata
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_21__DDP3_CTRLDATA             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_21, 2)
// Group: GPP_C, Pad: xxgpp_c_21, Function: 6 - IOM_GPPC_21
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_21__IOM_GPPC_21               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_21, 6)
// Group: GPP_C, Pad: xxgpp_c_22, Function: 1 - tbt_lsx3_a
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_22__TBT_LSX3_A                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_22, 1)
// Group: GPP_C, Pad: xxgpp_c_22, Function: 2 - ddp4_ctrlclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_22__DDP4_CTRLCLK              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_22, 2)
// Group: GPP_C, Pad: xxgpp_c_22, Function: 6 - IOM_GPPC_22
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_22__IOM_GPPC_22               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_22, 6)
// Group: GPP_C, Pad: xxgpp_c_23, Function: 1 - tbt_lsx3_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_23__TBT_LSX3_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_23, 1)
// Group: GPP_C, Pad: xxgpp_c_23, Function: 2 - ddp4_ctrldata
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_23__DDP4_CTRLDATA             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_23, 2)
// Group: GPP_C, Pad: xxgpp_c_23, Function: 6 - IOM_GPPC_23
#define GPIOV2_PTL_PCD_MUXING__XXGPP_C_23__IOM_GPPC_23               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_C_23, 6)

//
// Pin muxing definitions for Group: GPP_F.
//

// Group: GPP_F, Pad: xxgpp_f_0, Function: 1 - cnv_bri_dt
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_0__CNV_BRI_DT                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_0, 1)
// Group: GPP_F, Pad: xxgpp_f_0, Function: 2 - uart2_rts_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_0__UART2_RTS_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_0, 2)
// Group: GPP_F, Pad: xxgpp_f_0, Function: 6 - IOM_GPPF_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_0__IOM_GPPF_0                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_0, 6)
// Group: GPP_F, Pad: xxgpp_f_1, Function: 1 - cnv_bri_rsp
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_1__CNV_BRI_RSP                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_1, 1)
// Group: GPP_F, Pad: xxgpp_f_1, Function: 2 - uart2_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_1__UART2_RXD                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_1, 2)
// Group: GPP_F, Pad: xxgpp_f_1, Function: 6 - IOM_GPPF_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_1__IOM_GPPF_1                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_1, 6)
// Group: GPP_F, Pad: xxgpp_f_2, Function: 1 - cnv_rgi_dt
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_2__CNV_RGI_DT                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_2, 1)
// Group: GPP_F, Pad: xxgpp_f_2, Function: 2 - uart2_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_2__UART2_TXD                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_2, 2)
// Group: GPP_F, Pad: xxgpp_f_2, Function: 6 - IOM_GPPF_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_2__IOM_GPPF_2                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_2, 6)
// Group: GPP_F, Pad: xxgpp_f_3, Function: 1 - cnv_rgi_rsp
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_3__CNV_RGI_RSP                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_3, 1)
// Group: GPP_F, Pad: xxgpp_f_3, Function: 2 - uart2_cts_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_3__UART2_CTS_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_3, 2)
// Group: GPP_F, Pad: xxgpp_f_3, Function: 6 - IOM_GPPF_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_3__IOM_GPPF_3                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_3, 6)
// Group: GPP_F, Pad: xxgpp_f_4, Function: 1 - cnv_rf_reset_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_4__CNV_RF_RESET_B             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_4, 1)
// Group: GPP_F, Pad: xxgpp_f_4, Function: 6 - IOM_GPPF_4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_4__IOM_GPPF_4                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_4, 6)
// Group: GPP_F, Pad: xxgpp_f_5, Function: 3 - crf_clkreq
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_5__CRF_CLKREQ                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_5, 3)
// Group: GPP_F, Pad: xxgpp_f_5, Function: 6 - IOM_GPPF_5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_5__IOM_GPPF_5                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_5, 6)
// Group: GPP_F, Pad: xxgpp_f_6, Function: 1 - cnv_pa_blanking
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_6__CNV_PA_BLANKING            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_6, 1)
// Group: GPP_F, Pad: xxgpp_f_6, Function: 6 - IOM_GPPF_6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_6__IOM_GPPF_6                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_6, 6)
// Group: GPP_F, Pad: xxgpp_f_7, Function: 1 - fusa_diagtest_en
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_7__FUSA_DIAGTEST_EN           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_7, 1)
// Group: GPP_F, Pad: xxgpp_f_7, Function: 2 - imgclkout_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_7__IMGCLKOUT_2                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_7, 2)
// Group: GPP_F, Pad: xxgpp_f_7, Function: 6 - IOM_GPPF_7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_7__IOM_GPPF_7                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_7, 6)
// Group: GPP_F, Pad: xxgpp_f_8, Function: 1 - fusa_diagtest_mode
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_8__FUSA_DIAGTEST_MODE         GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_8, 1)
// Group: GPP_F, Pad: xxgpp_f_8, Function: 6 - IOM_GPPF_8
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_8__IOM_GPPF_8                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_8, 6)
// Group: GPP_F, Pad: xxgpp_f_9, Function: 2 - sx_exit_holdoff_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_9__SX_EXIT_HOLDOFF_B          GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_9, 2)
// Group: GPP_F, Pad: xxgpp_f_9, Function: 4 - ish_gp_11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_9__ISH_GP_11                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_9, 4)
// Group: GPP_F, Pad: xxgpp_f_9, Function: 5 - ieh_fatal_err2_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_9__IEH_FATAL_ERR2_B           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_9, 5)
// Group: GPP_F, Pad: xxgpp_f_9, Function: 6 - IOM_GPPF_9
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_9__IOM_GPPF_9                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_9, 6)
// Group: GPP_F, Pad: xxgpp_f_10, Function: 6 - IOM_GPPF_10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_10__IOM_GPPF_10               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_10, 6)
// Group: GPP_F, Pad: xxgpp_f_10, Function: 8 - a_ish_gp_6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_10__A_ISH_GP_6                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_10, 8)
// Group: GPP_F, Pad: xxgpp_f_11, Function: 3 - thc1_spi2_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_11__THC1_SPI2_CLK             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_11, 3)
// Group: GPP_F, Pad: xxgpp_f_11, Function: 4 - a_ish_spi_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_11__A_ISH_SPI_CLK             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_11, 4)
// Group: GPP_F, Pad: xxgpp_f_11, Function: 5 - gspi1_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_11__GSPI1_CLK                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_11, 5)
// Group: GPP_F, Pad: xxgpp_f_11, Function: 6 - IOM_GPPF_11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_11__IOM_GPPF_11               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_11, 6)
// Group: GPP_F, Pad: xxgpp_f_11, Function: 7 - visa2ch2_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_11__VISA2CH2_CLK              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_11, 7)
// Group: GPP_F, Pad: xxgpp_f_11, Function: 9 - pti_trace_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_11__PTI_TRACE_CLK             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_11, 9)
// Group: GPP_F, Pad: xxgpp_f_11, Function: 11 - visafusech1_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_11__VISAFUSECH1_CLK           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_11, 11)
// Group: GPP_F, Pad: xxgpp_f_12, Function: 1 - thc_i2c1_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__THC_I2C1_SCL              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_12, 1)
// Group: GPP_F, Pad: xxgpp_f_12, Function: 2 - i3c2_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__I3C2_SCL                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_12, 2)
// Group: GPP_F, Pad: xxgpp_f_12, Function: 3 - thc1_spi2_io_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__THC1_SPI2_IO_0            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_12, 3)
// Group: GPP_F, Pad: xxgpp_f_12, Function: 4 - a_ish_spi_miso
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__A_ISH_SPI_MISO            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_12, 4)
// Group: GPP_F, Pad: xxgpp_f_12, Function: 5 - gspi1_mosi
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__GSPI1_MOSI                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_12, 5)
// Group: GPP_F, Pad: xxgpp_f_12, Function: 6 - IOM_GPPF_12
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__IOM_GPPF_12               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_12, 6)
// Group: GPP_F, Pad: xxgpp_f_12, Function: 7 - visa2ch2_d0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__VISA2CH2_D0               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_12, 7)
// Group: GPP_F, Pad: xxgpp_f_12, Function: 8 - i2c5_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__I2C5_SCL                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_12, 8)
// Group: GPP_F, Pad: xxgpp_f_12, Function: 9 - pti_trace_d00
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__PTI_TRACE_D00             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_12, 9)
// Group: GPP_F, Pad: xxgpp_f_12, Function: 11 - visafusech1_d0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_12__VISAFUSECH1_D0            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_12, 11)
// Group: GPP_F, Pad: xxgpp_f_13, Function: 1 - thc_i2c1_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__THC_I2C1_SDA              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_13, 1)
// Group: GPP_F, Pad: xxgpp_f_13, Function: 2 - i3c2_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__I3C2_SDA                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_13, 2)
// Group: GPP_F, Pad: xxgpp_f_13, Function: 3 - thc1_spi2_io_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__THC1_SPI2_IO_1            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_13, 3)
// Group: GPP_F, Pad: xxgpp_f_13, Function: 4 - a_ish_spi_mosi
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__A_ISH_SPI_MOSI            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_13, 4)
// Group: GPP_F, Pad: xxgpp_f_13, Function: 5 - gspi1_miso
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__GSPI1_MISO                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_13, 5)
// Group: GPP_F, Pad: xxgpp_f_13, Function: 6 - IOM_GPPF_13
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__IOM_GPPF_13               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_13, 6)
// Group: GPP_F, Pad: xxgpp_f_13, Function: 7 - visa2ch2_d1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__VISA2CH2_D1               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_13, 7)
// Group: GPP_F, Pad: xxgpp_f_13, Function: 8 - i2c5_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__I2C5_SDA                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_13, 8)
// Group: GPP_F, Pad: xxgpp_f_13, Function: 9 - pti_trace_d01
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__PTI_TRACE_D01             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_13, 9)
// Group: GPP_F, Pad: xxgpp_f_13, Function: 11 - visafusech1_d1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_13__VISAFUSECH1_D1            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_13, 11)
// Group: GPP_F, Pad: xxgpp_f_14, Function: 3 - thc1_spi2_io_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_14__THC1_SPI2_IO_2            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_14, 3)
// Group: GPP_F, Pad: xxgpp_f_14, Function: 6 - IOM_GPPF_14
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_14__IOM_GPPF_14               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_14, 6)
// Group: GPP_F, Pad: xxgpp_f_14, Function: 7 - visa2ch2_d2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_14__VISA2CH2_D2               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_14, 7)
// Group: GPP_F, Pad: xxgpp_f_14, Function: 8 - a_gspi0_mosi
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_14__A_GSPI0_MOSI              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_14, 8)
// Group: GPP_F, Pad: xxgpp_f_14, Function: 9 - pti_trace_d02
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_14__PTI_TRACE_D02             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_14, 9)
// Group: GPP_F, Pad: xxgpp_f_14, Function: 11 - visafusech1_d2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_14__VISAFUSECH1_D2            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_14, 11)
// Group: GPP_F, Pad: xxgpp_f_15, Function: 3 - thc1_spi2_io_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_15__THC1_SPI2_IO_3            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_15, 3)
// Group: GPP_F, Pad: xxgpp_f_15, Function: 6 - IOM_GPPF_15
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_15__IOM_GPPF_15               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_15, 6)
// Group: GPP_F, Pad: xxgpp_f_15, Function: 7 - visa2ch2_d3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_15__VISA2CH2_D3               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_15, 7)
// Group: GPP_F, Pad: xxgpp_f_15, Function: 8 - a_gspi0_miso
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_15__A_GSPI0_MISO              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_15, 8)
// Group: GPP_F, Pad: xxgpp_f_15, Function: 9 - pti_trace_d03
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_15__PTI_TRACE_D03             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_15, 9)
// Group: GPP_F, Pad: xxgpp_f_15, Function: 11 - visafusech1_d3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_15__VISAFUSECH1_D3            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_15, 11)
// Group: GPP_F, Pad: xxgpp_f_16, Function: 3 - thc1_spi2_rst_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_16__THC1_SPI2_RST_B           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_16, 3)
// Group: GPP_F, Pad: xxgpp_f_16, Function: 6 - IOM_GPPF_16
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_16__IOM_GPPF_16               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_16, 6)
// Group: GPP_F, Pad: xxgpp_f_16, Function: 7 - visa2ch2_d4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_16__VISA2CH2_D4               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_16, 7)
// Group: GPP_F, Pad: xxgpp_f_16, Function: 8 - a_gspi0_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_16__A_GSPI0_CLK               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_16, 8)
// Group: GPP_F, Pad: xxgpp_f_16, Function: 9 - pti_trace_d04
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_16__PTI_TRACE_D04             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_16, 9)
// Group: GPP_F, Pad: xxgpp_f_16, Function: 11 - visafusech1_d4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_16__VISAFUSECH1_D4            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_16, 11)
// Group: GPP_F, Pad: xxgpp_f_17, Function: 3 - thc1_spi2_cs_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_17__THC1_SPI2_CS_B            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_17, 3)
// Group: GPP_F, Pad: xxgpp_f_17, Function: 4 - a_ish_spi_cs_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_17__A_ISH_SPI_CS_B            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_17, 4)
// Group: GPP_F, Pad: xxgpp_f_17, Function: 5 - gspi1_cs0_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_17__GSPI1_CS0_B               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_17, 5)
// Group: GPP_F, Pad: xxgpp_f_17, Function: 6 - IOM_GPPF_17
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_17__IOM_GPPF_17               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_17, 6)
// Group: GPP_F, Pad: xxgpp_f_17, Function: 7 - visa2ch2_d5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_17__VISA2CH2_D5               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_17, 7)
// Group: GPP_F, Pad: xxgpp_f_17, Function: 9 - pti_trace_d05
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_17__PTI_TRACE_D05             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_17, 9)
// Group: GPP_F, Pad: xxgpp_f_17, Function: 11 - visafusech1_d5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_17__VISAFUSECH1_D5            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_17, 11)
// Group: GPP_F, Pad: xxgpp_f_18, Function: 3 - thc1_spi2_int_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_18__THC1_SPI2_INT_B           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_18, 3)
// Group: GPP_F, Pad: xxgpp_f_18, Function: 6 - IOM_GPPF_18
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_18__IOM_GPPF_18               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_18, 6)
// Group: GPP_F, Pad: xxgpp_f_18, Function: 7 - visa2ch2_d6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_18__VISA2CH2_D6               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_18, 7)
// Group: GPP_F, Pad: xxgpp_f_18, Function: 8 - a_gspi0_cs0_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_18__A_GSPI0_CS0_B             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_18, 8)
// Group: GPP_F, Pad: xxgpp_f_18, Function: 9 - pti_trace_d06
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_18__PTI_TRACE_D06             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_18, 9)
// Group: GPP_F, Pad: xxgpp_f_18, Function: 11 - visafusech1_d6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_18__VISAFUSECH1_D6            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_18, 11)
// Group: GPP_F, Pad: xxgpp_f_19, Function: 6 - IOM_GPPF_19
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_19__IOM_GPPF_19               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_19, 6)
// Group: GPP_F, Pad: xxgpp_f_20, Function: 6 - IOM_GPPF_20
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_20__IOM_GPPF_20               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_20, 6)
// Group: GPP_F, Pad: xxgpp_f_21, Function: 6 - IOM_GPPF_21
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_21__IOM_GPPF_21               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_21, 6)
// Group: GPP_F, Pad: xxgpp_f_22, Function: 3 - thc1_spi2_dsync
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_22__THC1_SPI2_DSYNC           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_22, 3)
// Group: GPP_F, Pad: xxgpp_f_22, Function: 5 - ieh_corr_err0_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_22__IEH_CORR_ERR0_B           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_22, 5)
// Group: GPP_F, Pad: xxgpp_f_22, Function: 6 - IOM_GPPF_22
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_22__IOM_GPPF_22               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_22, 6)
// Group: GPP_F, Pad: xxgpp_f_22, Function: 7 - visa2ch2_d7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_22__VISA2CH2_D7               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_22, 7)
// Group: GPP_F, Pad: xxgpp_f_22, Function: 8 - a_ish_gp_8
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_22__A_ISH_GP_8                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_22, 8)
// Group: GPP_F, Pad: xxgpp_f_22, Function: 9 - pti_trace_d07
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_22__PTI_TRACE_D07             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_22, 9)
// Group: GPP_F, Pad: xxgpp_f_22, Function: 11 - visafusech1_d7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_22__VISAFUSECH1_D7            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_22, 11)
// Group: GPP_F, Pad: xxgpp_f_23, Function: 5 - ieh_nonfatal_err1_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_23__IEH_NONFATAL_ERR1_B       GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_23, 5)
// Group: GPP_F, Pad: xxgpp_f_23, Function: 6 - IOM_GPPF_23
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_23__IOM_GPPF_23               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_23, 6)
// Group: GPP_F, Pad: xxgpp_f_23, Function: 8 - a_ish_gp_9
#define GPIOV2_PTL_PCD_MUXING__XXGPP_F_23__A_ISH_GP_9                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_F_23, 8)
// Group: GPP_F, Pad: thc1_gspi1_i3c2_clk_loopbk, Function: 2 - i3c2_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__THC1_GSPI1_I3C2_CLK_LOOPBK__I3C2_CLK_LOOPBK GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_THC1_GSPI1_I3C2_CLK_LOOPBK, 2)
// Group: GPP_F, Pad: thc1_gspi1_i3c2_clk_loopbk, Function: 3 - thc1_spi2_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__THC1_GSPI1_I3C2_CLK_LOOPBK__THC1_SPI2_CLK_LOOPBK GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_THC1_GSPI1_I3C2_CLK_LOOPBK, 3)
// Group: GPP_F, Pad: thc1_gspi1_i3c2_clk_loopbk, Function: 5 - gspi1_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__THC1_GSPI1_I3C2_CLK_LOOPBK__GSPI1_CLK_LOOPBK GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_THC1_GSPI1_I3C2_CLK_LOOPBK, 5)
// Group: GPP_F, Pad: a_gspi0_clk_loopbk, Function: 8 - a_gspi0_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__A_GSPI0_CLK_LOOPBK__A_GSPI0_CLK_LOOPBK GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_A_GSPI0_CLK_LOOPBK, 8)

//
// Pin muxing definitions for Group: GPP_E.
//

// Group: GPP_E, Pad: xxgpp_e_0, Function: 6 - IOM_GPPE_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_0__IOM_GPPE_0                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_0, 6)
// Group: GPP_E, Pad: xxgpp_e_1, Function: 1 - cpu_gp_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_1__CPU_GP_2                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_1, 1)
// Group: GPP_E, Pad: xxgpp_e_1, Function: 2 - slp_dram_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_1__SLP_DRAM_B                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_1, 2)
// Group: GPP_E, Pad: xxgpp_e_1, Function: 6 - IOM_GPPE_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_1__IOM_GPPE_1                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_1, 6)
// Group: GPP_E, Pad: xxgpp_e_1, Function: 8 - a_ish_gp_5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_1__A_ISH_GP_5                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_1, 8)
// Group: GPP_E, Pad: xxgpp_e_2, Function: 1 - cpu_gp_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_2__CPU_GP_3                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_2, 1)
// Group: GPP_E, Pad: xxgpp_e_2, Function: 2 - vralert_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_2__VRALERT_B                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_2, 2)
// Group: GPP_E, Pad: xxgpp_e_2, Function: 4 - ish_gp_10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_2__ISH_GP_10                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_2, 4)
// Group: GPP_E, Pad: xxgpp_e_2, Function: 6 - IOM_GPPE_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_2__IOM_GPPE_2                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_2, 6)
// Group: GPP_E, Pad: xxgpp_e_3, Function: 1 - cpu_gp_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_3__CPU_GP_0                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_3, 1)
// Group: GPP_E, Pad: xxgpp_e_3, Function: 6 - IOM_GPPE_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_3__IOM_GPPE_3                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_3, 6)
// Group: GPP_E, Pad: xxgpp_e_4, Function: 6 - IOM_GPPE_4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_4__IOM_GPPE_4                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_4, 6)
// Group: GPP_E, Pad: xxgpp_e_5, Function: 4 - ish_gp_7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_5__ISH_GP_7                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_5, 4)
// Group: GPP_E, Pad: xxgpp_e_5, Function: 6 - IOM_GPPE_5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_5__IOM_GPPE_5                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_5, 6)
// Group: GPP_E, Pad: xxgpp_e_6, Function: 6 - IOM_GPPE_6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_6__IOM_GPPE_6                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_6, 6)
// Group: GPP_E, Pad: xxgpp_e_7, Function: 1 - ddpa_ctrlclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_7__DDPA_CTRLCLK               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_7, 1)
// Group: GPP_E, Pad: xxgpp_e_7, Function: 6 - IOM_GPPE_7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_7__IOM_GPPE_7                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_7, 6)
// Group: GPP_E, Pad: xxgpp_e_8, Function: 1 - ddpa_ctrldata
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_8__DDPA_CTRLDATA              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_8, 1)
// Group: GPP_E, Pad: xxgpp_e_8, Function: 6 - IOM_GPPE_8
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_8__IOM_GPPE_8                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_8, 6)
// Group: GPP_E, Pad: xxgpp_e_9, Function: 1 - usb2_oc0_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_9__USB2_OC0_B                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_9, 1)
// Group: GPP_E, Pad: xxgpp_e_9, Function: 6 - IOM_GPPE_9
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_9__IOM_GPPE_9                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_9, 6)
// Group: GPP_E, Pad: xxgpp_e_10, Function: 6 - IOM_GPPE_10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_10__IOM_GPPE_10               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_10, 6)
// Group: GPP_E, Pad: xxgpp_e_11, Function: 3 - thc0_spi1_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_11__THC0_SPI1_CLK             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_11, 3)
// Group: GPP_E, Pad: xxgpp_e_11, Function: 5 - gspi0_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_11__GSPI0_CLK                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_11, 5)
// Group: GPP_E, Pad: xxgpp_e_11, Function: 6 - IOM_GPPE_11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_11__IOM_GPPE_11               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_11, 6)
// Group: GPP_E, Pad: xxgpp_e_11, Function: 7 - visa2ch1_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_11__VISA2CH1_CLK              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_11, 7)
// Group: GPP_E, Pad: xxgpp_e_11, Function: 9 - pti_trace_vld
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_11__PTI_TRACE_VLD             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_11, 9)
// Group: GPP_E, Pad: xxgpp_e_11, Function: 11 - visaossefusech1_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_11__VISAOSSEFUSECH1_CLK       GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_11, 11)
// Group: GPP_E, Pad: xxgpp_e_12, Function: 1 - thc_i2c0_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_12__THC_I2C0_SCL              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_12, 1)
// Group: GPP_E, Pad: xxgpp_e_12, Function: 3 - thc0_spi1_io_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_12__THC0_SPI1_IO_0            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_12, 3)
// Group: GPP_E, Pad: xxgpp_e_12, Function: 5 - gspi0_mosi
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_12__GSPI0_MOSI                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_12, 5)
// Group: GPP_E, Pad: xxgpp_e_12, Function: 6 - IOM_GPPE_12
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_12__IOM_GPPE_12               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_12, 6)
// Group: GPP_E, Pad: xxgpp_e_12, Function: 7 - visa2ch1_d0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_12__VISA2CH1_D0               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_12, 7)
// Group: GPP_E, Pad: xxgpp_e_12, Function: 8 - i2c4_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_12__I2C4_SCL                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_12, 8)
// Group: GPP_E, Pad: xxgpp_e_12, Function: 9 - pti_trace_d08
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_12__PTI_TRACE_D08             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_12, 9)
// Group: GPP_E, Pad: xxgpp_e_12, Function: 10 - a_cnv_debug_00
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_12__A_CNV_DEBUG_00            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_12, 10)
// Group: GPP_E, Pad: xxgpp_e_12, Function: 11 - visaossefusech1_d0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_12__VISAOSSEFUSECH1_D0        GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_12, 11)
// Group: GPP_E, Pad: xxgpp_e_13, Function: 1 - thc_i2c0_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_13__THC_I2C0_SDA              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_13, 1)
// Group: GPP_E, Pad: xxgpp_e_13, Function: 3 - thc0_spi1_io_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_13__THC0_SPI1_IO_1            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_13, 3)
// Group: GPP_E, Pad: xxgpp_e_13, Function: 5 - gspi0_miso
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_13__GSPI0_MISO                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_13, 5)
// Group: GPP_E, Pad: xxgpp_e_13, Function: 6 - IOM_GPPE_13
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_13__IOM_GPPE_13               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_13, 6)
// Group: GPP_E, Pad: xxgpp_e_13, Function: 7 - visa2ch1_d1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_13__VISA2CH1_D1               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_13, 7)
// Group: GPP_E, Pad: xxgpp_e_13, Function: 8 - i2c4_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_13__I2C4_SDA                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_13, 8)
// Group: GPP_E, Pad: xxgpp_e_13, Function: 9 - pti_trace_d09
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_13__PTI_TRACE_D09             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_13, 9)
// Group: GPP_E, Pad: xxgpp_e_13, Function: 10 - a_cnv_debug_01
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_13__A_CNV_DEBUG_01            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_13, 10)
// Group: GPP_E, Pad: xxgpp_e_13, Function: 11 - visaossefusech1_d1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_13__VISAOSSEFUSECH1_D1        GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_13, 11)
// Group: GPP_E, Pad: xxgpp_e_14, Function: 3 - thc0_spi1_io_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_14__THC0_SPI1_IO_2            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_14, 3)
// Group: GPP_E, Pad: xxgpp_e_14, Function: 6 - IOM_GPPE_14
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_14__IOM_GPPE_14               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_14, 6)
// Group: GPP_E, Pad: xxgpp_e_14, Function: 7 - visa2ch1_d2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_14__VISA2CH1_D2               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_14, 7)
// Group: GPP_E, Pad: xxgpp_e_14, Function: 9 - pti_trace_d10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_14__PTI_TRACE_D10             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_14, 9)
// Group: GPP_E, Pad: xxgpp_e_14, Function: 10 - a_cnv_debug_02
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_14__A_CNV_DEBUG_02            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_14, 10)
// Group: GPP_E, Pad: xxgpp_e_14, Function: 11 - visaossefusech1_d2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_14__VISAOSSEFUSECH1_D2        GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_14, 11)
// Group: GPP_E, Pad: xxgpp_e_15, Function: 3 - thc0_spi1_io_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_15__THC0_SPI1_IO_3            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_15, 3)
// Group: GPP_E, Pad: xxgpp_e_15, Function: 6 - IOM_GPPE_15
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_15__IOM_GPPE_15               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_15, 6)
// Group: GPP_E, Pad: xxgpp_e_15, Function: 7 - visa2ch1_d3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_15__VISA2CH1_D3               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_15, 7)
// Group: GPP_E, Pad: xxgpp_e_15, Function: 9 - pti_trace_d11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_15__PTI_TRACE_D11             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_15, 9)
// Group: GPP_E, Pad: xxgpp_e_15, Function: 10 - a_cnv_debug_03
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_15__A_CNV_DEBUG_03            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_15, 10)
// Group: GPP_E, Pad: xxgpp_e_15, Function: 11 - visaossefusech1_d3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_15__VISAOSSEFUSECH1_D3        GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_15, 11)
// Group: GPP_E, Pad: xxgpp_e_16, Function: 3 - thc0_spi1_rst_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_16__THC0_SPI1_RST_B           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_16, 3)
// Group: GPP_E, Pad: xxgpp_e_16, Function: 6 - IOM_GPPE_16
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_16__IOM_GPPE_16               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_16, 6)
// Group: GPP_E, Pad: xxgpp_e_16, Function: 7 - visa2ch1_d4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_16__VISA2CH1_D4               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_16, 7)
// Group: GPP_E, Pad: xxgpp_e_16, Function: 9 - pti_trace_d12
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_16__PTI_TRACE_D12             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_16, 9)
// Group: GPP_E, Pad: xxgpp_e_16, Function: 10 - a_cnv_debug_04
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_16__A_CNV_DEBUG_04            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_16, 10)
// Group: GPP_E, Pad: xxgpp_e_16, Function: 11 - visaossefusech1_d4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_16__VISAOSSEFUSECH1_D4        GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_16, 11)
// Group: GPP_E, Pad: xxgpp_e_17, Function: 3 - thc0_spi1_cs_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_17__THC0_SPI1_CS_B            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_17, 3)
// Group: GPP_E, Pad: xxgpp_e_17, Function: 5 - gspi0_cs0_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_17__GSPI0_CS0_B               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_17, 5)
// Group: GPP_E, Pad: xxgpp_e_17, Function: 6 - IOM_GPPE_17
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_17__IOM_GPPE_17               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_17, 6)
// Group: GPP_E, Pad: xxgpp_e_17, Function: 7 - visa2ch1_d5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_17__VISA2CH1_D5               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_17, 7)
// Group: GPP_E, Pad: xxgpp_e_17, Function: 9 - pti_trace_d13
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_17__PTI_TRACE_D13             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_17, 9)
// Group: GPP_E, Pad: xxgpp_e_17, Function: 10 - a_cnv_debug_05
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_17__A_CNV_DEBUG_05            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_17, 10)
// Group: GPP_E, Pad: xxgpp_e_17, Function: 11 - visaossefusech1_d5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_17__VISAOSSEFUSECH1_D5        GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_17, 11)
// Group: GPP_E, Pad: xxgpp_e_18, Function: 3 - thc0_spi1_int_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_18__THC0_SPI1_INT_B           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_18, 3)
// Group: GPP_E, Pad: xxgpp_e_18, Function: 6 - IOM_GPPE_18
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_18__IOM_GPPE_18               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_18, 6)
// Group: GPP_E, Pad: xxgpp_e_18, Function: 7 - visa2ch1_d6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_18__VISA2CH1_D6               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_18, 7)
// Group: GPP_E, Pad: xxgpp_e_18, Function: 9 - pti_trace_d14
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_18__PTI_TRACE_D14             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_18, 9)
// Group: GPP_E, Pad: xxgpp_e_18, Function: 10 - a_cnv_debug_06
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_18__A_CNV_DEBUG_06            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_18, 10)
// Group: GPP_E, Pad: xxgpp_e_18, Function: 11 - visaossefusech1_d6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_18__VISAOSSEFUSECH1_D6        GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_18, 11)
// Group: GPP_E, Pad: xxgpp_e_19, Function: 1 - pmc_i2c_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_19__PMC_I2C_SDA               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_19, 1)
// Group: GPP_E, Pad: xxgpp_e_19, Function: 6 - IOM_GPPE_19
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_19__IOM_GPPE_19               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_19, 6)
// Group: GPP_E, Pad: xxgpp_e_20, Function: 1 - pmc_i2c_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_20__PMC_I2C_SCL               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_20, 1)
// Group: GPP_E, Pad: xxgpp_e_20, Function: 6 - IOM_GPPE_20
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_20__IOM_GPPE_20               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_20, 6)
// Group: GPP_E, Pad: xxgpp_e_21, Function: 1 - pmcalert_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_21__PMCALERT_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_21, 1)
// Group: GPP_E, Pad: xxgpp_e_21, Function: 6 - IOM_GPPE_21
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_21__IOM_GPPE_21               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_21, 6)
// Group: GPP_E, Pad: xxgpp_e_22, Function: 3 - thc0_spi1_dsync
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_22__THC0_SPI1_DSYNC           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_22, 3)
// Group: GPP_E, Pad: xxgpp_e_22, Function: 6 - IOM_GPPE_22
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_22__IOM_GPPE_22               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_22, 6)
// Group: GPP_E, Pad: xxgpp_e_22, Function: 7 - visa2ch1_d7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_22__VISA2CH1_D7               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_22, 7)
// Group: GPP_E, Pad: xxgpp_e_22, Function: 9 - pti_trace_d15
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_22__PTI_TRACE_D15             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_22, 9)
// Group: GPP_E, Pad: xxgpp_e_22, Function: 10 - a_cnv_debug_07
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_22__A_CNV_DEBUG_07            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_22, 10)
// Group: GPP_E, Pad: xxgpp_e_22, Function: 11 - visaossefusech1_d7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_E_22__VISAOSSEFUSECH1_D7        GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_E_22, 11)
// Group: GPP_E, Pad: xxboothalt_b, Function: 1 - xxboothalt_b
#define GPIOV2_PTL_PCD_MUXING__XXBOOTHALT_B__XXBOOTHALT_B            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXBOOTHALT_B, 1)
// Group: GPP_E, Pad: thc0_gspi0_clk_loopbk, Function: 3 - thc0_spi1_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__THC0_GSPI0_CLK_LOOPBK__THC0_SPI1_CLK_LOOPBK GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_THC0_GSPI0_CLK_LOOPBK, 3)
// Group: GPP_E, Pad: thc0_gspi0_clk_loopbk, Function: 5 - gspi0_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__THC0_GSPI0_CLK_LOOPBK__GSPI0_CLK_LOOPBK GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_THC0_GSPI0_CLK_LOOPBK, 5)

//
// Pin muxing definitions for Group: CPUJTAG.
//

// Group: CPUJTAG, Pad: xxepd_on, Function: 1 - xxepd_on
#define GPIOV2_PTL_PCD_MUXING__XXEPD_ON__XXEPD_ON                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXEPD_ON, 1)
// Group: CPUJTAG, Pad: xxvdd2_pwrgd, Function: 1 - xxvdd2_pwrgd
#define GPIOV2_PTL_PCD_MUXING__XXVDD2_PWRGD__XXVDD2_PWRGD            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXVDD2_PWRGD, 1)
// Group: CPUJTAG, Pad: xxjtag_mbpb0, Function: 1 - xxjtag_mbpb0
#define GPIOV2_PTL_PCD_MUXING__XXJTAG_MBPB0__XXJTAG_MBPB0            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXJTAG_MBPB0, 1)
// Group: CPUJTAG, Pad: xxjtag_mbpb1, Function: 1 - xxjtag_mbpb1
#define GPIOV2_PTL_PCD_MUXING__XXJTAG_MBPB1__XXJTAG_MBPB1            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXJTAG_MBPB1, 1)
// Group: CPUJTAG, Pad: xxjtag_mbpb2, Function: 1 - xxjtag_mbpb2
#define GPIOV2_PTL_PCD_MUXING__XXJTAG_MBPB2__XXJTAG_MBPB2            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXJTAG_MBPB2, 1)
// Group: CPUJTAG, Pad: xxjtag_mbpb3, Function: 1 - xxjtag_mbpb3
#define GPIOV2_PTL_PCD_MUXING__XXJTAG_MBPB3__XXJTAG_MBPB3            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXJTAG_MBPB3, 1)
// Group: CPUJTAG, Pad: xxjtag_tdo, Function: 1 - xxjtag_tdo
#define GPIOV2_PTL_PCD_MUXING__XXJTAG_TDO__XXJTAG_TDO                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXJTAG_TDO, 1)
// Group: CPUJTAG, Pad: xxprdy_b, Function: 1 - xxprdy_b
#define GPIOV2_PTL_PCD_MUXING__XXPRDY_B__XXPRDY_B                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXPRDY_B, 1)
// Group: CPUJTAG, Pad: xxpreq_b, Function: 1 - xxpreq_b
#define GPIOV2_PTL_PCD_MUXING__XXPREQ_B__XXPREQ_B                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXPREQ_B, 1)
// Group: CPUJTAG, Pad: xxjtag_tdi, Function: 1 - xxjtag_tdi
#define GPIOV2_PTL_PCD_MUXING__XXJTAG_TDI__XXJTAG_TDI                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXJTAG_TDI, 1)
// Group: CPUJTAG, Pad: xxjtag_tms, Function: 1 - xxjtag_tms
#define GPIOV2_PTL_PCD_MUXING__XXJTAG_TMS__XXJTAG_TMS                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXJTAG_TMS, 1)
// Group: CPUJTAG, Pad: xxjtag_tck, Function: 1 - xxjtag_tck
#define GPIOV2_PTL_PCD_MUXING__XXJTAG_TCK__XXJTAG_TCK                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXJTAG_TCK, 1)
// Group: CPUJTAG, Pad: xxdbg_pmode, Function: 1 - xxdbg_pmode
#define GPIOV2_PTL_PCD_MUXING__XXDBG_PMODE__XXDBG_PMODE              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXDBG_PMODE, 1)
// Group: CPUJTAG, Pad: xxjtag_trst_b, Function: 1 - xxjtag_trst_b
#define GPIOV2_PTL_PCD_MUXING__XXJTAG_TRST_B__XXJTAG_TRST_B          GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXJTAG_TRST_B, 1)
// Group: CPUJTAG, Pad: xxddsp_hpdalv, Function: 1 - xxddsp_hpdalv
#define GPIOV2_PTL_PCD_MUXING__XXDDSP_HPDALV__XXDDSP_HPDALV          GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXDDSP_HPDALV, 1)
// Group: CPUJTAG, Pad: xxddsp_hpdalv, Function: 2 - disp_misca
#define GPIOV2_PTL_PCD_MUXING__XXDDSP_HPDALV__DISP_MISCA             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXDDSP_HPDALV, 2)

//
// Pin muxing definitions for Group: GPP_H.
//

// Group: GPP_H, Pad: xxgpp_h_0, Function: 6 - IOM_GPPH_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_0__IOM_GPPH_0                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_0, 6)
// Group: GPP_H, Pad: xxgpp_h_1, Function: 6 - IOM_GPPH_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_1__IOM_GPPH_1                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_1, 6)
// Group: GPP_H, Pad: xxgpp_h_2, Function: 6 - IOM_GPPH_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_2__IOM_GPPH_2                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_2, 6)
// Group: GPP_H, Pad: xxgpp_h_3, Function: 1 - mic_mute
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_3__MIC_MUTE                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_3, 1)
// Group: GPP_H, Pad: xxgpp_h_3, Function: 6 - IOM_GPPH_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_3__IOM_GPPH_3                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_3, 6)
// Group: GPP_H, Pad: xxgpp_h_4, Function: 1 - i2c2_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_4__I2C2_SDA                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_4, 1)
// Group: GPP_H, Pad: xxgpp_h_4, Function: 2 - cnv_mfuart2_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_4__CNV_MFUART2_RXD            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_4, 2)
// Group: GPP_H, Pad: xxgpp_h_4, Function: 6 - IOM_GPPH_4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_4__IOM_GPPH_4                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_4, 6)
// Group: GPP_H, Pad: xxgpp_h_5, Function: 1 - i2c2_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_5__I2C2_SCL                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_5, 1)
// Group: GPP_H, Pad: xxgpp_h_5, Function: 2 - cnv_mfuart2_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_5__CNV_MFUART2_TXD            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_5, 2)
// Group: GPP_H, Pad: xxgpp_h_5, Function: 6 - IOM_GPPH_5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_5__IOM_GPPH_5                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_5, 6)
// Group: GPP_H, Pad: xxgpp_h_6, Function: 1 - i2c3_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_6__I2C3_SDA                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_6, 1)
// Group: GPP_H, Pad: xxgpp_h_6, Function: 2 - uart1_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_6__UART1_RXD                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_6, 2)
// Group: GPP_H, Pad: xxgpp_h_6, Function: 3 - a_ish_uart1_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_6__A_ISH_UART1_RXD            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_6, 3)
// Group: GPP_H, Pad: xxgpp_h_6, Function: 6 - IOM_GPPH_6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_6__IOM_GPPH_6                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_6, 6)
// Group: GPP_H, Pad: xxgpp_h_7, Function: 1 - i2c3_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_7__I2C3_SCL                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_7, 1)
// Group: GPP_H, Pad: xxgpp_h_7, Function: 2 - uart1_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_7__UART1_TXD                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_7, 2)
// Group: GPP_H, Pad: xxgpp_h_7, Function: 3 - a_ish_uart1_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_7__A_ISH_UART1_TXD            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_7, 3)
// Group: GPP_H, Pad: xxgpp_h_7, Function: 6 - IOM_GPPH_7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_7__IOM_GPPH_7                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_7, 6)
// Group: GPP_H, Pad: xxgpp_h_8, Function: 1 - uart0_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_8__UART0_RXD                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_8, 1)
// Group: GPP_H, Pad: xxgpp_h_8, Function: 6 - IOM_GPPH_8
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_8__IOM_GPPH_8                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_8, 6)
// Group: GPP_H, Pad: xxgpp_h_9, Function: 1 - uart0_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_9__UART0_TXD                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_9, 1)
// Group: GPP_H, Pad: xxgpp_h_9, Function: 6 - IOM_GPPH_9
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_9__IOM_GPPH_9                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_9, 6)
// Group: GPP_H, Pad: xxgpp_h_10, Function: 1 - uart0_rts_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_10__UART0_RTS_B               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_10, 1)
// Group: GPP_H, Pad: xxgpp_h_10, Function: 2 - a_i3c1_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_10__A_I3C1_SDA                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_10, 2)
// Group: GPP_H, Pad: xxgpp_h_10, Function: 6 - IOM_GPPH_10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_10__IOM_GPPH_10               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_10, 6)
// Group: GPP_H, Pad: xxgpp_h_10, Function: 8 - a_ish_gp_10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_10__A_ISH_GP_10               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_10, 8)
// Group: GPP_H, Pad: xxgpp_h_11, Function: 1 - uart0_cts_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_11__UART0_CTS_B               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_11, 1)
// Group: GPP_H, Pad: xxgpp_h_11, Function: 2 - a_i3c1_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_11__A_I3C1_SCL                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_11, 2)
// Group: GPP_H, Pad: xxgpp_h_11, Function: 6 - IOM_GPPH_11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_11__IOM_GPPH_11               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_11, 6)
// Group: GPP_H, Pad: xxgpp_h_11, Function: 8 - a_ish_gp_11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_11__A_ISH_GP_11               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_11, 8)
// Group: GPP_H, Pad: xxgpp_h_12, Function: 6 - IOM_GPPH_12
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_12__IOM_GPPH_12               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_12, 6)
// Group: GPP_H, Pad: xxgpp_h_13, Function: 1 - cpu_c10_gate_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_13__CPU_C10_GATE_B            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_13, 1)
// Group: GPP_H, Pad: xxgpp_h_13, Function: 6 - IOM_GPPH_13
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_13__IOM_GPPH_13               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_13, 6)
// Group: GPP_H, Pad: xxgpp_h_14, Function: 1 - ish_uart1_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_14__ISH_UART1_RXD             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_14, 1)
// Group: GPP_H, Pad: xxgpp_h_14, Function: 2 - a_uart1_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_14__A_UART1_RXD               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_14, 2)
// Group: GPP_H, Pad: xxgpp_h_14, Function: 3 - ish_i2c1_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_14__ISH_I2C1_SDA              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_14, 3)
// Group: GPP_H, Pad: xxgpp_h_14, Function: 4 - ish_i3c1_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_14__ISH_I3C1_SDA              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_14, 4)
// Group: GPP_H, Pad: xxgpp_h_14, Function: 6 - IOM_GPPH_14
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_14__IOM_GPPH_14               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_14, 6)
// Group: GPP_H, Pad: xxgpp_h_15, Function: 1 - ish_uart1_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_15__ISH_UART1_TXD             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_15, 1)
// Group: GPP_H, Pad: xxgpp_h_15, Function: 2 - a_uart1_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_15__A_UART1_TXD               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_15, 2)
// Group: GPP_H, Pad: xxgpp_h_15, Function: 3 - ish_i2c1_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_15__ISH_I2C1_SCL              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_15, 3)
// Group: GPP_H, Pad: xxgpp_h_15, Function: 4 - ish_i3c1_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_15__ISH_I3C1_SCL              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_15, 4)
// Group: GPP_H, Pad: xxgpp_h_15, Function: 6 - IOM_GPPH_15
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_15__IOM_GPPH_15               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_15, 6)
// Group: GPP_H, Pad: xxgpp_h_16, Function: 1 - tbt_lsx2_oe
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_16__TBT_LSX2_OE               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_16, 1)
// Group: GPP_H, Pad: xxgpp_h_16, Function: 3 - pcie_lnk_down
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_16__PCIE_LNK_DOWN             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_16, 3)
// Group: GPP_H, Pad: xxgpp_h_16, Function: 6 - IOM_GPPH_16
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_16__IOM_GPPH_16               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_16, 6)
// Group: GPP_H, Pad: xxgpp_h_17, Function: 1 - mic_mute_led
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_17__MIC_MUTE_LED              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_17, 1)
// Group: GPP_H, Pad: xxgpp_h_17, Function: 6 - IOM_GPPH_17
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_17__IOM_GPPH_17               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_17, 6)
// Group: GPP_H, Pad: xxgpp_h_18, Function: 6 - IOM_GPPH_18
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_18__IOM_GPPH_18               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_18, 6)
// Group: GPP_H, Pad: xxgpp_h_19, Function: 1 - i2c0_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_19__I2C0_SDA                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_19, 1)
// Group: GPP_H, Pad: xxgpp_h_19, Function: 2 - i3c0_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_19__I3C0_SDA                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_19, 2)
// Group: GPP_H, Pad: xxgpp_h_19, Function: 6 - IOM_GPPH_19
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_19__IOM_GPPH_19               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_19, 6)
// Group: GPP_H, Pad: xxgpp_h_20, Function: 1 - i2c0_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_20__I2C0_SCL                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_20, 1)
// Group: GPP_H, Pad: xxgpp_h_20, Function: 2 - i3c0_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_20__I3C0_SCL                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_20, 2)
// Group: GPP_H, Pad: xxgpp_h_20, Function: 6 - IOM_GPPH_20
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_20__IOM_GPPH_20               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_20, 6)
// Group: GPP_H, Pad: xxgpp_h_21, Function: 1 - i2c1_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_21__I2C1_SDA                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_21, 1)
// Group: GPP_H, Pad: xxgpp_h_21, Function: 2 - i3c1_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_21__I3C1_SDA                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_21, 2)
// Group: GPP_H, Pad: xxgpp_h_21, Function: 6 - IOM_GPPH_21
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_21__IOM_GPPH_21               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_21, 6)
// Group: GPP_H, Pad: xxgpp_h_22, Function: 1 - i2c1_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_22__I2C1_SCL                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_22, 1)
// Group: GPP_H, Pad: xxgpp_h_22, Function: 2 - i3c1_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_22__I3C1_SCL                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_22, 2)
// Group: GPP_H, Pad: xxgpp_h_22, Function: 6 - IOM_GPPH_22
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_22__IOM_GPPH_22               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_22, 6)
// Group: GPP_H, Pad: xxgpp_h_23, Function: 1 - espi_cs4_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_23__ESPI_CS4_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_23, 1)
// Group: GPP_H, Pad: xxgpp_h_23, Function: 6 - IOM_GPPH_23
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_23__IOM_GPPH_23               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_23, 6)
// Group: GPP_H, Pad: xxgpp_h_24, Function: 1 - espi_alert4_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_24__ESPI_ALERT4_B             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_24, 1)
// Group: GPP_H, Pad: xxgpp_h_24, Function: 6 - IOM_GPPH_24
#define GPIOV2_PTL_PCD_MUXING__XXGPP_H_24__IOM_GPPH_24               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_H_24, 6)
// Group: GPP_H, Pad: lpi3c1_clk_loopbk, Function: 2 - i3c1_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__LPI3C1_CLK_LOOPBK__I3C1_CLK_LOOPBK    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_LPI3C1_CLK_LOOPBK, 2)
// Group: GPP_H, Pad: lpi3c1_clk_loopbk, Function: 3 - a_i3c1_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__LPI3C1_CLK_LOOPBK__A_I3C1_CLK_LOOPBK  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_LPI3C1_CLK_LOOPBK, 3)
// Group: GPP_H, Pad: lpi3c0_clk_loopbk, Function: 2 - i3c0_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__LPI3C0_CLK_LOOPBK__I3C0_CLK_LOOPBK    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_LPI3C0_CLK_LOOPBK, 2)
// Group: GPP_H, Pad: ishi3c1_clk_loopbk, Function: 4 - ish_i3c1_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__ISHI3C1_CLK_LOOPBK__ISH_I3C1_CLK_LOOPBK GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_ISHI3C1_CLK_LOOPBK, 4)

//
// Pin muxing definitions for Group: GPPASPI0.
//

// Group: GPPASPI0, Pad: xxgpp_a_0, Function: 1 - espi_io_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_0__ESPI_IO_0                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_0, 1)
// Group: GPPASPI0, Pad: xxgpp_a_0, Function: 6 - IOM_GPPA_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_0__IOM_GPPA_0                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_0, 6)
// Group: GPPASPI0, Pad: xxgpp_a_1, Function: 1 - espi_io_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_1__ESPI_IO_1                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_1, 1)
// Group: GPPASPI0, Pad: xxgpp_a_1, Function: 6 - IOM_GPPA_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_1__IOM_GPPA_1                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_1, 6)
// Group: GPPASPI0, Pad: xxgpp_a_2, Function: 1 - espi_io_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_2__ESPI_IO_2                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_2, 1)
// Group: GPPASPI0, Pad: xxgpp_a_2, Function: 2 - pripwrdnack
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_2__PRIPWRDNACK                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_2, 2)
// Group: GPPASPI0, Pad: xxgpp_a_2, Function: 6 - IOM_GPPA_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_2__IOM_GPPA_2                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_2, 6)
// Group: GPPASPI0, Pad: xxgpp_a_3, Function: 1 - espi_io_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_3__ESPI_IO_3                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_3, 1)
// Group: GPPASPI0, Pad: xxgpp_a_3, Function: 2 - priack_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_3__PRIACK_B                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_3, 2)
// Group: GPPASPI0, Pad: xxgpp_a_3, Function: 6 - IOM_GPPA_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_3__IOM_GPPA_3                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_3, 6)
// Group: GPPASPI0, Pad: xxgpp_a_4, Function: 1 - espi_cs0_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_4__ESPI_CS0_B                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_4, 1)
// Group: GPPASPI0, Pad: xxgpp_a_4, Function: 6 - IOM_GPPA_4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_4__IOM_GPPA_4                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_4, 6)
// Group: GPPASPI0, Pad: xxgpp_a_5, Function: 1 - espi_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_5__ESPI_CLK                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_5, 1)
// Group: GPPASPI0, Pad: xxgpp_a_5, Function: 6 - IOM_GPPA_5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_5__IOM_GPPA_5                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_5, 6)
// Group: GPPASPI0, Pad: xxgpp_a_6, Function: 1 - espi_reset_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_6__ESPI_RESET_B               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_6, 1)
// Group: GPPASPI0, Pad: xxgpp_a_6, Function: 6 - IOM_GPPA_6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_6__IOM_GPPA_6                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_6, 6)
// Group: GPPASPI0, Pad: xxgpp_a_7, Function: 6 - IOM_GPPA_7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_7__IOM_GPPA_7                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_7, 6)
// Group: GPPASPI0, Pad: xxgpp_a_8, Function: 1 - osse_smlclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_8__OSSE_SMLCLK                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_8, 1)
// Group: GPPASPI0, Pad: xxgpp_a_8, Function: 6 - IOM_GPPA_8
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_8__IOM_GPPA_8                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_8, 6)
// Group: GPPASPI0, Pad: xxgpp_a_9, Function: 1 - osse_smldata
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_9__OSSE_SMLDATA               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_9, 1)
// Group: GPPASPI0, Pad: xxgpp_a_9, Function: 6 - IOM_GPPA_9
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_9__IOM_GPPA_9                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_9, 6)
// Group: GPPASPI0, Pad: xxgpp_a_10, Function: 1 - osse_smlalert_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_10__OSSE_SMLALERT_B           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_10, 1)
// Group: GPPASPI0, Pad: xxgpp_a_10, Function: 6 - IOM_GPPA_10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_10__IOM_GPPA_10               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_10, 6)
// Group: GPPASPI0, Pad: xxgpp_a_11, Function: 6 - IOM_GPPA_11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_11__IOM_GPPA_11               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_11, 6)
// Group: GPPASPI0, Pad: xxgpp_a_12, Function: 6 - IOM_GPPA_12
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_12__IOM_GPPA_12               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_12, 6)
// Group: GPPASPI0, Pad: xxgpp_a_13, Function: 1 - espi_cs1_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_13__ESPI_CS1_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_13, 1)
// Group: GPPASPI0, Pad: xxgpp_a_13, Function: 6 - IOM_GPPA_13
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_13__IOM_GPPA_13               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_13, 6)
// Group: GPPASPI0, Pad: xxgpp_a_14, Function: 1 - adr_complete
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_14__ADR_COMPLETE              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_14, 1)
// Group: GPPASPI0, Pad: xxgpp_a_14, Function: 6 - IOM_GPPA_14
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_14__IOM_GPPA_14               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_14, 6)
// Group: GPPASPI0, Pad: xxgpp_a_15, Function: 1 - dnx_force_reload
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_15__DNX_FORCE_RELOAD          GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_15, 1)
// Group: GPPASPI0, Pad: xxgpp_a_15, Function: 6 - IOM_GPPA_15
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_15__IOM_GPPA_15               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_15, 6)
// Group: GPPASPI0, Pad: xxgpp_a_16, Function: 1 - espi_cs2_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_16__ESPI_CS2_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_16, 1)
// Group: GPPASPI0, Pad: xxgpp_a_16, Function: 6 - IOM_GPPA_16
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_16__IOM_GPPA_16               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_16, 6)
// Group: GPPASPI0, Pad: xxgpp_a_17, Function: 1 - espi_cs3_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_17__ESPI_CS3_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_17, 1)
// Group: GPPASPI0, Pad: xxgpp_a_17, Function: 6 - IOM_GPPA_17
#define GPIOV2_PTL_PCD_MUXING__XXGPP_A_17__IOM_GPPA_17               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_A_17, 6)
// Group: GPPASPI0, Pad: xxspi0_io_2, Function: 1 - xxspi0_io_2
#define GPIOV2_PTL_PCD_MUXING__XXSPI0_IO_2__XXSPI0_IO_2              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXSPI0_IO_2, 1)
// Group: GPPASPI0, Pad: xxspi0_io_3, Function: 1 - xxspi0_io_3
#define GPIOV2_PTL_PCD_MUXING__XXSPI0_IO_3__XXSPI0_IO_3              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXSPI0_IO_3, 1)
// Group: GPPASPI0, Pad: xxspi0_mosi_io_0, Function: 1 - xxspi0_mosi_io_0
#define GPIOV2_PTL_PCD_MUXING__XXSPI0_MOSI_IO_0__XXSPI0_MOSI_IO_0    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXSPI0_MOSI_IO_0, 1)
// Group: GPPASPI0, Pad: xxspi0_miso_io_1, Function: 1 - xxspi0_miso_io_1
#define GPIOV2_PTL_PCD_MUXING__XXSPI0_MISO_IO_1__XXSPI0_MISO_IO_1    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXSPI0_MISO_IO_1, 1)
// Group: GPPASPI0, Pad: xxspi0_tpm_cs_b, Function: 1 - xxspi0_tpm_cs_b
#define GPIOV2_PTL_PCD_MUXING__XXSPI0_TPM_CS_B__XXSPI0_TPM_CS_B      GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXSPI0_TPM_CS_B, 1)
// Group: GPPASPI0, Pad: xxspi0_flash_0_cs_b, Function: 1 - xxspi0_flash_0_cs_b
#define GPIOV2_PTL_PCD_MUXING__XXSPI0_FLASH_0_CS_B__XXSPI0_FLASH_0_CS_B GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXSPI0_FLASH_0_CS_B, 1)
// Group: GPPASPI0, Pad: xxspi0_flash_1_cs_b, Function: 1 - xxspi0_flash_1_cs_b
#define GPIOV2_PTL_PCD_MUXING__XXSPI0_FLASH_1_CS_B__XXSPI0_FLASH_1_CS_B GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXSPI0_FLASH_1_CS_B, 1)
// Group: GPPASPI0, Pad: xxspi0_clk, Function: 1 - xxspi0_clk
#define GPIOV2_PTL_PCD_MUXING__XXSPI0_CLK__XXSPI0_CLK                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXSPI0_CLK, 1)
// Group: GPPASPI0, Pad: espi_clk_loopbk, Function: 1 - espi_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__ESPI_CLK_LOOPBK__ESPI_CLK_LOOPBK      GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_ESPI_CLK_LOOPBK, 1)
// Group: GPPASPI0, Pad: spi0_clk_loopbk, Function: 1 - spi0_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__SPI0_CLK_LOOPBK__SPI0_CLK_LOOPBK      GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_SPI0_CLK_LOOPBK, 1)

//
// Pin muxing definitions for Group: vGPIO_3.
//

// Group: vGPIO_3, Pad: vGPIO_USB_0, Function: 1 - ESPI_USB_OCB_0
#define GPIOV2_PTL_PCD_MUXING__VGPIO_USB_0__ESPI_USB_OCB_0           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_USB_0, 1)
// Group: vGPIO_3, Pad: vGPIO_USB_1, Function: 1 - ESPI_USB_OCB_1
#define GPIOV2_PTL_PCD_MUXING__VGPIO_USB_1__ESPI_USB_OCB_1           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_USB_1, 1)
// Group: vGPIO_3, Pad: vGPIO_USB_2, Function: 1 - ESPI_USB_OCB_2
#define GPIOV2_PTL_PCD_MUXING__VGPIO_USB_2__ESPI_USB_OCB_2           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_USB_2, 1)
// Group: vGPIO_3, Pad: vGPIO_USB_3, Function: 1 - ESPI_USB_OCB_3
#define GPIOV2_PTL_PCD_MUXING__VGPIO_USB_3__ESPI_USB_OCB_3           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_USB_3, 1)
// Group: vGPIO_3, Pad: vGPIO_USB_4, Function: 1 - USB_CPU_OCB_0
#define GPIOV2_PTL_PCD_MUXING__VGPIO_USB_4__USB_CPU_OCB_0            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_USB_4, 1)
// Group: vGPIO_3, Pad: vGPIO_USB_5, Function: 1 - USB_CPU_OCB_1
#define GPIOV2_PTL_PCD_MUXING__VGPIO_USB_5__USB_CPU_OCB_1            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_USB_5, 1)
// Group: vGPIO_3, Pad: vGPIO_USB_6, Function: 1 - USB_CPU_OCB_2
#define GPIOV2_PTL_PCD_MUXING__VGPIO_USB_6__USB_CPU_OCB_2            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_USB_6, 1)
// Group: vGPIO_3, Pad: vGPIO_USB_7, Function: 1 - USB_CPU_OCB_3
#define GPIOV2_PTL_PCD_MUXING__VGPIO_USB_7__USB_CPU_OCB_3            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_USB_7, 1)

//
// Pin muxing definitions for Group: GPP_S.
//

// Group: GPP_S, Pad: xxgpp_s_0, Function: 1 - sndw3_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_0__SNDW3_CLK                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_0, 1)
// Group: GPP_S, Pad: xxgpp_s_0, Function: 6 - i2s1_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_0__I2S1_TXD                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_0, 6)
// Group: GPP_S, Pad: xxgpp_s_1, Function: 1 - sndw3_data0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_1__SNDW3_DATA0                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_1, 1)
// Group: GPP_S, Pad: xxgpp_s_1, Function: 6 - i2s1_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_1__I2S1_RXD                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_1, 6)
// Group: GPP_S, Pad: xxgpp_s_2, Function: 1 - sndw3_data1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_2__SNDW3_DATA1                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_2, 1)
// Group: GPP_S, Pad: xxgpp_s_2, Function: 3 - sndw0_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_2__SNDW0_CLK                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_2, 3)
// Group: GPP_S, Pad: xxgpp_s_2, Function: 5 - dmic_clk_a_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_2__DMIC_CLK_A_0               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_2, 5)
// Group: GPP_S, Pad: xxgpp_s_2, Function: 6 - i2s1_sclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_2__I2S1_SCLK                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_2, 6)
// Group: GPP_S, Pad: xxgpp_s_3, Function: 1 - sndw3_data2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_3__SNDW3_DATA2                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_3, 1)
// Group: GPP_S, Pad: xxgpp_s_3, Function: 2 - sndw2_data1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_3__SNDW2_DATA1                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_3, 2)
// Group: GPP_S, Pad: xxgpp_s_3, Function: 3 - sndw0_data0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_3__SNDW0_DATA0                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_3, 3)
// Group: GPP_S, Pad: xxgpp_s_3, Function: 5 - dmic_data_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_3__DMIC_DATA_0                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_3, 5)
// Group: GPP_S, Pad: xxgpp_s_3, Function: 6 - i2s1_sfrm
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_3__I2S1_SFRM                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_3, 6)
// Group: GPP_S, Pad: xxgpp_s_4, Function: 2 - sndw2_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_4__SNDW2_CLK                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_4, 2)
// Group: GPP_S, Pad: xxgpp_s_4, Function: 5 - dmic_clk_a_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_4__DMIC_CLK_A_0               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_4, 5)
// Group: GPP_S, Pad: xxgpp_s_4, Function: 6 - i2s2_sclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_4__I2S2_SCLK                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_4, 6)
// Group: GPP_S, Pad: xxgpp_s_5, Function: 2 - sndw2_data0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_5__SNDW2_DATA0                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_5, 2)
// Group: GPP_S, Pad: xxgpp_s_5, Function: 5 - dmic_data_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_5__DMIC_DATA_0                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_5, 5)
// Group: GPP_S, Pad: xxgpp_s_5, Function: 6 - i2s2_sfrm
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_5__I2S2_SFRM                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_5, 6)
// Group: GPP_S, Pad: xxgpp_s_6, Function: 2 - sndw2_data1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_6__SNDW2_DATA1                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_6, 2)
// Group: GPP_S, Pad: xxgpp_s_6, Function: 3 - sndw1_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_6__SNDW1_CLK                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_6, 3)
// Group: GPP_S, Pad: xxgpp_s_6, Function: 5 - dmic_clk_a_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_6__DMIC_CLK_A_1               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_6, 5)
// Group: GPP_S, Pad: xxgpp_s_6, Function: 6 - i2s2_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_6__I2S2_TXD                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_6, 6)
// Group: GPP_S, Pad: xxgpp_s_7, Function: 1 - sndw3_data3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_7__SNDW3_DATA3                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_7, 1)
// Group: GPP_S, Pad: xxgpp_s_7, Function: 2 - sndw2_data2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_7__SNDW2_DATA2                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_7, 2)
// Group: GPP_S, Pad: xxgpp_s_7, Function: 3 - sndw1_data0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_7__SNDW1_DATA0                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_7, 3)
// Group: GPP_S, Pad: xxgpp_s_7, Function: 5 - dmic_data_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_7__DMIC_DATA_1                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_7, 5)
// Group: GPP_S, Pad: xxgpp_s_7, Function: 6 - i2s2_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_S_7__I2S2_RXD                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_S_7, 6)

//
// Pin muxing definitions for Group: GPP_B.
//

// Group: GPP_B, Pad: xxgpp_b_0, Function: 1 - usbc_smlclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_0__USBC_SMLCLK                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_0, 1)
// Group: GPP_B, Pad: xxgpp_b_0, Function: 6 - IOM_GPPB_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_0__IOM_GPPB_0                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_0, 6)
// Group: GPP_B, Pad: xxgpp_b_1, Function: 1 - usbc_smldata
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_1__USBC_SMLDATA               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_1, 1)
// Group: GPP_B, Pad: xxgpp_b_1, Function: 6 - IOM_GPPB_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_1__IOM_GPPB_1                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_1, 6)
// Group: GPP_B, Pad: xxgpp_b_2, Function: 3 - ish_i2c0_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_2__ISH_I2C0_SDA               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_2, 3)
// Group: GPP_B, Pad: xxgpp_b_2, Function: 4 - ish_i3c0_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_2__ISH_I3C0_SDA               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_2, 4)
// Group: GPP_B, Pad: xxgpp_b_2, Function: 6 - IOM_GPPB_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_2__IOM_GPPB_2                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_2, 6)
// Group: GPP_B, Pad: xxgpp_b_2, Function: 7 - visa2ch3_d0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_2__VISA2CH3_D0                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_2, 7)
// Group: GPP_B, Pad: xxgpp_b_2, Function: 8 - a_i2c2_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_2__A_I2C2_SDA                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_2, 8)
// Group: GPP_B, Pad: xxgpp_b_2, Function: 9 - pti_trace_d00
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_2__PTI_TRACE_D00              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_2, 9)
// Group: GPP_B, Pad: xxgpp_b_2, Function: 10 - cnv_debug_00
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_2__CNV_DEBUG_00               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_2, 10)
// Group: GPP_B, Pad: xxgpp_b_3, Function: 3 - ish_i2c0_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_3__ISH_I2C0_SCL               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_3, 3)
// Group: GPP_B, Pad: xxgpp_b_3, Function: 4 - ish_i3c0_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_3__ISH_I3C0_SCL               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_3, 4)
// Group: GPP_B, Pad: xxgpp_b_3, Function: 6 - IOM_GPPB_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_3__IOM_GPPB_3                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_3, 6)
// Group: GPP_B, Pad: xxgpp_b_3, Function: 7 - visa2ch3_d1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_3__VISA2CH3_D1                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_3, 7)
// Group: GPP_B, Pad: xxgpp_b_3, Function: 8 - a_i2c2_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_3__A_I2C2_SCL                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_3, 8)
// Group: GPP_B, Pad: xxgpp_b_3, Function: 9 - pti_trace_d01
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_3__PTI_TRACE_D01              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_3, 9)
// Group: GPP_B, Pad: xxgpp_b_3, Function: 10 - cnv_debug_01
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_3__CNV_DEBUG_01               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_3, 10)
// Group: GPP_B, Pad: xxgpp_b_4, Function: 2 - Blink
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_4__BLINK                      GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_4, 2)
// Group: GPP_B, Pad: xxgpp_b_4, Function: 3 - Serial_Blink
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_4__SERIAL_BLINK               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_4, 3)
// Group: GPP_B, Pad: xxgpp_b_4, Function: 4 - ish_gp_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_4__ISH_GP_0                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_4, 4)
// Group: GPP_B, Pad: xxgpp_b_4, Function: 6 - IOM_GPPB_4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_4__IOM_GPPB_4                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_4, 6)
// Group: GPP_B, Pad: xxgpp_b_4, Function: 7 - visa2ch3_d2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_4__VISA2CH3_D2                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_4, 7)
// Group: GPP_B, Pad: xxgpp_b_4, Function: 9 - pti_trace_d02
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_4__PTI_TRACE_D02              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_4, 9)
// Group: GPP_B, Pad: xxgpp_b_4, Function: 10 - cnv_debug_02
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_4__CNV_DEBUG_02               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_4, 10)
// Group: GPP_B, Pad: xxgpp_b_5, Function: 2 - Blink
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_5__BLINK                      GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_5, 2)
// Group: GPP_B, Pad: xxgpp_b_5, Function: 3 - Serial_Blink
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_5__SERIAL_BLINK               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_5, 3)
// Group: GPP_B, Pad: xxgpp_b_5, Function: 4 - ish_gp_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_5__ISH_GP_1                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_5, 4)
// Group: GPP_B, Pad: xxgpp_b_5, Function: 6 - IOM_GPPB_5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_5__IOM_GPPB_5                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_5, 6)
// Group: GPP_B, Pad: xxgpp_b_5, Function: 7 - visa2ch3_d3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_5__VISA2CH3_D3                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_5, 7)
// Group: GPP_B, Pad: xxgpp_b_5, Function: 9 - pti_trace_d03
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_5__PTI_TRACE_D03              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_5, 9)
// Group: GPP_B, Pad: xxgpp_b_5, Function: 10 - cnv_debug_03
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_5__CNV_DEBUG_03               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_5, 10)
// Group: GPP_B, Pad: xxgpp_b_6, Function: 2 - Blink
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_6__BLINK                      GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_6, 2)
// Group: GPP_B, Pad: xxgpp_b_6, Function: 3 - Serial_Blink
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_6__SERIAL_BLINK               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_6, 3)
// Group: GPP_B, Pad: xxgpp_b_6, Function: 4 - ish_gp_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_6__ISH_GP_2                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_6, 4)
// Group: GPP_B, Pad: xxgpp_b_6, Function: 6 - IOM_GPPB_6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_6__IOM_GPPB_6                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_6, 6)
// Group: GPP_B, Pad: xxgpp_b_6, Function: 7 - visa2ch3_d4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_6__VISA2CH3_D4                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_6, 7)
// Group: GPP_B, Pad: xxgpp_b_6, Function: 9 - pti_trace_d04
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_6__PTI_TRACE_D04              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_6, 9)
// Group: GPP_B, Pad: xxgpp_b_6, Function: 10 - cnv_debug_04
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_6__CNV_DEBUG_04               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_6, 10)
// Group: GPP_B, Pad: xxgpp_b_7, Function: 2 - Blink
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_7__BLINK                      GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_7, 2)
// Group: GPP_B, Pad: xxgpp_b_7, Function: 3 - Serial_Blink
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_7__SERIAL_BLINK               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_7, 3)
// Group: GPP_B, Pad: xxgpp_b_7, Function: 4 - ish_gp_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_7__ISH_GP_3                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_7, 4)
// Group: GPP_B, Pad: xxgpp_b_7, Function: 6 - IOM_GPPB_7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_7__IOM_GPPB_7                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_7, 6)
// Group: GPP_B, Pad: xxgpp_b_7, Function: 7 - visa2ch3_d5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_7__VISA2CH3_D5                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_7, 7)
// Group: GPP_B, Pad: xxgpp_b_7, Function: 9 - pti_trace_d05
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_7__PTI_TRACE_D05              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_7, 9)
// Group: GPP_B, Pad: xxgpp_b_7, Function: 10 - cnv_debug_05
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_7__CNV_DEBUG_05               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_7, 10)
// Group: GPP_B, Pad: xxgpp_b_8, Function: 2 - Blink
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_8__BLINK                      GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_8, 2)
// Group: GPP_B, Pad: xxgpp_b_8, Function: 3 - Serial_Blink
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_8__SERIAL_BLINK               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_8, 3)
// Group: GPP_B, Pad: xxgpp_b_8, Function: 4 - ish_gp_4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_8__ISH_GP_4                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_8, 4)
// Group: GPP_B, Pad: xxgpp_b_8, Function: 6 - IOM_GPPB_8
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_8__IOM_GPPB_8                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_8, 6)
// Group: GPP_B, Pad: xxgpp_b_8, Function: 7 - visa2ch3_d6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_8__VISA2CH3_D6                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_8, 7)
// Group: GPP_B, Pad: xxgpp_b_8, Function: 9 - pti_trace_d06
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_8__PTI_TRACE_D06              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_8, 9)
// Group: GPP_B, Pad: xxgpp_b_8, Function: 10 - cnv_debug_06
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_8__CNV_DEBUG_06               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_8, 10)
// Group: GPP_B, Pad: xxgpp_b_9, Function: 2 - ddsp_hpd1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_9__DDSP_HPD1                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_9, 2)
// Group: GPP_B, Pad: xxgpp_b_9, Function: 3 - disp_misc1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_9__DISP_MISC1                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_9, 3)
// Group: GPP_B, Pad: xxgpp_b_9, Function: 6 - IOM_GPPB_9
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_9__IOM_GPPB_9                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_9, 6)
// Group: GPP_B, Pad: xxgpp_b_10, Function: 2 - ddsp_hpd2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_10__DDSP_HPD2                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_10, 2)
// Group: GPP_B, Pad: xxgpp_b_10, Function: 3 - disp_misc2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_10__DISP_MISC2                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_10, 3)
// Group: GPP_B, Pad: xxgpp_b_10, Function: 6 - IOM_GPPB_10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_10__IOM_GPPB_10               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_10, 6)
// Group: GPP_B, Pad: xxgpp_b_11, Function: 1 - usb2_oc1_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_11__USB2_OC1_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_11, 1)
// Group: GPP_B, Pad: xxgpp_b_11, Function: 2 - ddsp_hpd3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_11__DDSP_HPD3                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_11, 2)
// Group: GPP_B, Pad: xxgpp_b_11, Function: 3 - disp_misc3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_11__DISP_MISC3                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_11, 3)
// Group: GPP_B, Pad: xxgpp_b_11, Function: 6 - IOM_GPPB_11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_11__IOM_GPPB_11               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_11, 6)
// Group: GPP_B, Pad: xxgpp_b_12, Function: 1 - slp_s0_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_12__SLP_S0_B                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_12, 1)
// Group: GPP_B, Pad: xxgpp_b_12, Function: 6 - IOM_GPPB_12
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_12__IOM_GPPB_12               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_12, 6)
// Group: GPP_B, Pad: xxgpp_b_13, Function: 1 - pltrst_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_13__PLTRST_B                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_13, 1)
// Group: GPP_B, Pad: xxgpp_b_13, Function: 6 - IOM_GPPB_13
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_13__IOM_GPPB_13               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_13, 6)
// Group: GPP_B, Pad: xxgpp_b_14, Function: 1 - usb2_oc2_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_14__USB2_OC2_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_14, 1)
// Group: GPP_B, Pad: xxgpp_b_14, Function: 2 - ddsp_hpd4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_14__DDSP_HPD4                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_14, 2)
// Group: GPP_B, Pad: xxgpp_b_14, Function: 3 - disp_misc4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_14__DISP_MISC4                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_14, 3)
// Group: GPP_B, Pad: xxgpp_b_14, Function: 6 - IOM_GPPB_14
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_14__IOM_GPPB_14               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_14, 6)
// Group: GPP_B, Pad: xxgpp_b_15, Function: 1 - usb2_oc3_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_15__USB2_OC3_B                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_15, 1)
// Group: GPP_B, Pad: xxgpp_b_15, Function: 6 - IOM_GPPB_15
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_15__IOM_GPPB_15               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_15, 6)
// Group: GPP_B, Pad: xxgpp_b_16, Function: 1 - tbt_lsx1_oe
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_16__TBT_LSX1_OE               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_16, 1)
// Group: GPP_B, Pad: xxgpp_b_16, Function: 6 - IOM_GPPB_16
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_16__IOM_GPPB_16               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_16, 6)
// Group: GPP_B, Pad: xxgpp_b_17, Function: 1 - tbt_lsx0_oe
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_17__TBT_LSX0_OE               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_17, 1)
// Group: GPP_B, Pad: xxgpp_b_17, Function: 2 - l_vdden2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_17__L_VDDEN2                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_17, 2)
// Group: GPP_B, Pad: xxgpp_b_17, Function: 6 - IOM_GPPB_17
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_17__IOM_GPPB_17               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_17, 6)
// Group: GPP_B, Pad: xxgpp_b_17, Function: 7 - visa2ch4_d0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_17__VISA2CH4_D0               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_17, 7)
// Group: GPP_B, Pad: xxgpp_b_17, Function: 9 - pti_trace_d08
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_17__PTI_TRACE_D08             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_17, 9)
// Group: GPP_B, Pad: xxgpp_b_18, Function: 1 - ish_i2c2_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_18__ISH_I2C2_SDA              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_18, 1)
// Group: GPP_B, Pad: xxgpp_b_18, Function: 2 - a_i2c4_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_18__A_I2C4_SDA                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_18, 2)
// Group: GPP_B, Pad: xxgpp_b_18, Function: 3 - cnv_mfuart0_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_18__CNV_MFUART0_RXD           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_18, 3)
// Group: GPP_B, Pad: xxgpp_b_18, Function: 6 - IOM_GPPB_18
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_18__IOM_GPPB_18               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_18, 6)
// Group: GPP_B, Pad: xxgpp_b_18, Function: 7 - visa2ch3_d7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_18__VISA2CH3_D7               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_18, 7)
// Group: GPP_B, Pad: xxgpp_b_18, Function: 9 - pti_trace_d07
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_18__PTI_TRACE_D07             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_18, 9)
// Group: GPP_B, Pad: xxgpp_b_18, Function: 10 - cnv_debug_07
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_18__CNV_DEBUG_07              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_18, 10)
// Group: GPP_B, Pad: xxgpp_b_19, Function: 1 - ish_i2c2_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_19__ISH_I2C2_SCL              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_19, 1)
// Group: GPP_B, Pad: xxgpp_b_19, Function: 2 - a_i2c4_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_19__A_I2C4_SCL                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_19, 2)
// Group: GPP_B, Pad: xxgpp_b_19, Function: 3 - cnv_mfuart0_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_19__CNV_MFUART0_TXD           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_19, 3)
// Group: GPP_B, Pad: xxgpp_b_19, Function: 6 - IOM_GPPB_19
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_19__IOM_GPPB_19               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_19, 6)
// Group: GPP_B, Pad: xxgpp_b_19, Function: 7 - visa2ch3_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_19__VISA2CH3_CLK              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_19, 7)
// Group: GPP_B, Pad: xxgpp_b_19, Function: 9 - pti_trace_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_19__PTI_TRACE_CLK             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_19, 9)
// Group: GPP_B, Pad: xxgpp_b_20, Function: 2 - a_i2c5_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_20__A_I2C5_SDA                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_20, 2)
// Group: GPP_B, Pad: xxgpp_b_20, Function: 3 - cnv_mfuart0_rts_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_20__CNV_MFUART0_RTS_B         GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_20, 3)
// Group: GPP_B, Pad: xxgpp_b_20, Function: 4 - ish_gp_8
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_20__ISH_GP_8                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_20, 4)
// Group: GPP_B, Pad: xxgpp_b_20, Function: 6 - IOM_GPPB_20
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_20__IOM_GPPB_20               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_20, 6)
// Group: GPP_B, Pad: xxgpp_b_20, Function: 7 - visa2ch4_d1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_20__VISA2CH4_D1               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_20, 7)
// Group: GPP_B, Pad: xxgpp_b_20, Function: 9 - pti_trace_d09
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_20__PTI_TRACE_D09             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_20, 9)
// Group: GPP_B, Pad: xxgpp_b_21, Function: 2 - a_i2c5_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_21__A_I2C5_SCL                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_21, 2)
// Group: GPP_B, Pad: xxgpp_b_21, Function: 3 - cnv_mfuart0_cts_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_21__CNV_MFUART0_CTS_B         GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_21, 3)
// Group: GPP_B, Pad: xxgpp_b_21, Function: 4 - ish_gp_9
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_21__ISH_GP_9                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_21, 4)
// Group: GPP_B, Pad: xxgpp_b_21, Function: 6 - IOM_GPPB_21
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_21__IOM_GPPB_21               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_21, 6)
// Group: GPP_B, Pad: xxgpp_b_21, Function: 7 - visa2ch4_d2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_21__VISA2CH4_D2               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_21, 7)
// Group: GPP_B, Pad: xxgpp_b_21, Function: 9 - pti_trace_d10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_21__PTI_TRACE_D10             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_21, 9)
// Group: GPP_B, Pad: xxgpp_b_22, Function: 1 - time_sync_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_22__TIME_SYNC_0               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_22, 1)
// Group: GPP_B, Pad: xxgpp_b_22, Function: 4 - ish_gp_5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_22__ISH_GP_5                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_22, 4)
// Group: GPP_B, Pad: xxgpp_b_22, Function: 6 - IOM_GPPB_22
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_22__IOM_GPPB_22               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_22, 6)
// Group: GPP_B, Pad: xxgpp_b_23, Function: 1 - time_sync_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_23__TIME_SYNC_1               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_23, 1)
// Group: GPP_B, Pad: xxgpp_b_23, Function: 4 - ish_gp_6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_23__ISH_GP_6                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_23, 4)
// Group: GPP_B, Pad: xxgpp_b_23, Function: 6 - IOM_GPPB_23
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_23__IOM_GPPB_23               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_23, 6)
// Group: GPP_B, Pad: xxgpp_b_23, Function: 7 - visa2ch4_d3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_23__VISA2CH4_D3               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_23, 7)
// Group: GPP_B, Pad: xxgpp_b_23, Function: 9 - pti_trace_d11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_23__PTI_TRACE_D11             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_23, 9)
// Group: GPP_B, Pad: xxgpp_b_24, Function: 1 - espi_alert0_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_24__ESPI_ALERT0_B             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_24, 1)
// Group: GPP_B, Pad: xxgpp_b_24, Function: 6 - IOM_GPPB_24
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_24__IOM_GPPB_24               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_24, 6)
// Group: GPP_B, Pad: xxgpp_b_25, Function: 1 - espi_alert1_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_25__ESPI_ALERT1_B             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_25, 1)
// Group: GPP_B, Pad: xxgpp_b_25, Function: 6 - IOM_GPPB_25
#define GPIOV2_PTL_PCD_MUXING__XXGPP_B_25__IOM_GPPB_25               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_B_25, 6)
// Group: GPP_B, Pad: ishi3c0_clk_loopbk, Function: 4 - ish_i3c0_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__ISHI3C0_CLK_LOOPBK__ISH_I3C0_CLK_LOOPBK GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_ISHI3C0_CLK_LOOPBK, 4)

//
// Pin muxing definitions for Group: GPP_D.
//

// Group: GPP_D, Pad: xxgpp_d_0, Function: 1 - imgclkout_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_0__IMGCLKOUT_1                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_0, 1)
// Group: GPP_D, Pad: xxgpp_d_0, Function: 6 - IOM_GPPD_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_0__IOM_GPPD_0                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_0, 6)
// Group: GPP_D, Pad: xxgpp_d_1, Function: 1 - a_i2c3_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_1__A_I2C3_SDA                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_1, 1)
// Group: GPP_D, Pad: xxgpp_d_1, Function: 2 - l_bklten2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_1__L_BKLTEN2                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_1, 2)
// Group: GPP_D, Pad: xxgpp_d_1, Function: 3 - a_ish_i2c2_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_1__A_ISH_I2C2_SDA             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_1, 3)
// Group: GPP_D, Pad: xxgpp_d_1, Function: 6 - IOM_GPPD_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_1__IOM_GPPD_1                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_1, 6)
// Group: GPP_D, Pad: xxgpp_d_2, Function: 1 - a_i2c3_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_2__A_I2C3_SCL                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_2, 1)
// Group: GPP_D, Pad: xxgpp_d_2, Function: 2 - l_bkltctl2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_2__L_BKLTCTL2                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_2, 2)
// Group: GPP_D, Pad: xxgpp_d_2, Function: 3 - a_ish_i2c2_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_2__A_ISH_I2C2_SCL             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_2, 3)
// Group: GPP_D, Pad: xxgpp_d_2, Function: 6 - IOM_GPPD_2
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_2__IOM_GPPD_2                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_2, 6)
// Group: GPP_D, Pad: xxgpp_d_3, Function: 1 - cpu_gp_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_3__CPU_GP_1                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_3, 1)
// Group: GPP_D, Pad: xxgpp_d_3, Function: 6 - IOM_GPPD_3
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_3__IOM_GPPD_3                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_3, 6)
// Group: GPP_D, Pad: xxgpp_d_3, Function: 7 - visa2ch4_d4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_3__VISA2CH4_D4                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_3, 7)
// Group: GPP_D, Pad: xxgpp_d_3, Function: 9 - pti_trace_d12
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_3__PTI_TRACE_D12              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_3, 9)
// Group: GPP_D, Pad: xxgpp_d_4, Function: 1 - imgclkout_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_4__IMGCLKOUT_0                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_4, 1)
// Group: GPP_D, Pad: xxgpp_d_4, Function: 6 - IOM_GPPD_4
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_4__IOM_GPPD_4                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_4, 6)
// Group: GPP_D, Pad: xxgpp_d_5, Function: 2 - ish_uart0_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_5__ISH_UART0_RXD              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_5, 2)
// Group: GPP_D, Pad: xxgpp_d_5, Function: 3 - ish_spi_cs_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_5__ISH_SPI_CS_B               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_5, 3)
// Group: GPP_D, Pad: xxgpp_d_5, Function: 5 - sml0bdata
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_5__SML0BDATA                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_5, 5)
// Group: GPP_D, Pad: xxgpp_d_5, Function: 6 - IOM_GPPD_5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_5__IOM_GPPD_5                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_5, 6)
// Group: GPP_D, Pad: xxgpp_d_5, Function: 7 - visa2ch4_d5
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_5__VISA2CH4_D5                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_5, 7)
// Group: GPP_D, Pad: xxgpp_d_5, Function: 9 - pti_trace_d13
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_5__PTI_TRACE_D13              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_5, 9)
// Group: GPP_D, Pad: xxgpp_d_6, Function: 2 - ish_uart0_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_6__ISH_UART0_TXD              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_6, 2)
// Group: GPP_D, Pad: xxgpp_d_6, Function: 3 - ish_spi_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_6__ISH_SPI_CLK                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_6, 3)
// Group: GPP_D, Pad: xxgpp_d_6, Function: 5 - sml0bclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_6__SML0BCLK                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_6, 5)
// Group: GPP_D, Pad: xxgpp_d_6, Function: 6 - IOM_GPPD_6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_6__IOM_GPPD_6                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_6, 6)
// Group: GPP_D, Pad: xxgpp_d_6, Function: 7 - visa2ch4_d6
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_6__VISA2CH4_D6                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_6, 7)
// Group: GPP_D, Pad: xxgpp_d_6, Function: 9 - pti_trace_d14
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_6__PTI_TRACE_D14              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_6, 9)
// Group: GPP_D, Pad: xxgpp_d_7, Function: 2 - ish_uart0_rts_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_7__ISH_UART0_RTS_B            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_7, 2)
// Group: GPP_D, Pad: xxgpp_d_7, Function: 3 - ish_spi_miso
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_7__ISH_SPI_MISO               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_7, 3)
// Group: GPP_D, Pad: xxgpp_d_7, Function: 6 - IOM_GPPD_7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_7__IOM_GPPD_7                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_7, 6)
// Group: GPP_D, Pad: xxgpp_d_7, Function: 7 - visa2ch4_d7
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_7__VISA2CH4_D7                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_7, 7)
// Group: GPP_D, Pad: xxgpp_d_7, Function: 9 - pti_trace_d15
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_7__PTI_TRACE_D15              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_7, 9)
// Group: GPP_D, Pad: xxgpp_d_8, Function: 2 - ish_uart0_cts_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_8__ISH_UART0_CTS_B            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_8, 2)
// Group: GPP_D, Pad: xxgpp_d_8, Function: 3 - ish_spi_mosi
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_8__ISH_SPI_MOSI               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_8, 3)
// Group: GPP_D, Pad: xxgpp_d_8, Function: 5 - sml0balert_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_8__SML0BALERT_B               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_8, 5)
// Group: GPP_D, Pad: xxgpp_d_8, Function: 6 - IOM_GPPD_8
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_8__IOM_GPPD_8                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_8, 6)
// Group: GPP_D, Pad: xxgpp_d_8, Function: 7 - visa2ch4_clk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_8__VISA2CH4_CLK               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_8, 7)
// Group: GPP_D, Pad: xxgpp_d_8, Function: 9 - pti_trace_vld
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_8__PTI_TRACE_VLD              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_8, 9)
// Group: GPP_D, Pad: xxgpp_d_9, Function: 2 - i2s_mclk1_out
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_9__I2S_MCLK1_OUT              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_9, 2)
// Group: GPP_D, Pad: xxgpp_d_9, Function: 6 - IOM_GPPD_9
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_9__IOM_GPPD_9                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_9, 6)
// Group: GPP_D, Pad: xxgpp_d_10, Function: 1 - hda_bclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_10__HDA_BCLK                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_10, 1)
// Group: GPP_D, Pad: xxgpp_d_10, Function: 2 - i2s0_sclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_10__I2S0_SCLK                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_10, 2)
// Group: GPP_D, Pad: xxgpp_d_10, Function: 4 - hdacpu_bclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_10__HDACPU_BCLK               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_10, 4)
// Group: GPP_D, Pad: xxgpp_d_10, Function: 6 - IOM_GPPD_10
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_10__IOM_GPPD_10               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_10, 6)
// Group: GPP_D, Pad: xxgpp_d_11, Function: 1 - hda_sync
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_11__HDA_SYNC                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_11, 1)
// Group: GPP_D, Pad: xxgpp_d_11, Function: 2 - i2s0_sfrm
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_11__I2S0_SFRM                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_11, 2)
// Group: GPP_D, Pad: xxgpp_d_11, Function: 6 - IOM_GPPD_11
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_11__IOM_GPPD_11               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_11, 6)
// Group: GPP_D, Pad: xxgpp_d_12, Function: 1 - hda_sdo
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_12__HDA_SDO                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_12, 1)
// Group: GPP_D, Pad: xxgpp_d_12, Function: 2 - i2s0_txd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_12__I2S0_TXD                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_12, 2)
// Group: GPP_D, Pad: xxgpp_d_12, Function: 4 - hdacpu_sdo
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_12__HDACPU_SDO                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_12, 4)
// Group: GPP_D, Pad: xxgpp_d_12, Function: 6 - IOM_GPPD_12
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_12__IOM_GPPD_12               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_12, 6)
// Group: GPP_D, Pad: xxgpp_d_13, Function: 1 - hda_sdi_0
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_13__HDA_SDI_0                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_13, 1)
// Group: GPP_D, Pad: xxgpp_d_13, Function: 2 - i2s0_rxd
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_13__I2S0_RXD                  GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_13, 2)
// Group: GPP_D, Pad: xxgpp_d_13, Function: 4 - hdacpu_sdi
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_13__HDACPU_SDI                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_13, 4)
// Group: GPP_D, Pad: xxgpp_d_13, Function: 6 - IOM_GPPD_13
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_13__IOM_GPPD_13               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_13, 6)
// Group: GPP_D, Pad: xxgpp_d_14, Function: 2 - tbt_lsx3_oe
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_14__TBT_LSX3_OE               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_14, 2)
// Group: GPP_D, Pad: xxgpp_d_14, Function: 6 - IOM_GPPD_14
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_14__IOM_GPPD_14               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_14, 6)
// Group: GPP_D, Pad: xxgpp_d_15, Function: 6 - IOM_GPPD_15
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_15__IOM_GPPD_15               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_15, 6)
// Group: GPP_D, Pad: xxgpp_d_16, Function: 1 - hda_rst_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_16__HDA_RST_B                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_16, 1)
// Group: GPP_D, Pad: xxgpp_d_16, Function: 3 - dmic_clk_a_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_16__DMIC_CLK_A_1              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_16, 3)
// Group: GPP_D, Pad: xxgpp_d_16, Function: 6 - IOM_GPPD_16
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_16__IOM_GPPD_16               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_16, 6)
// Group: GPP_D, Pad: xxgpp_d_17, Function: 1 - hda_sdi_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_17__HDA_SDI_1                 GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_17, 1)
// Group: GPP_D, Pad: xxgpp_d_17, Function: 3 - dmic_data_1
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_17__DMIC_DATA_1               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_17, 3)
// Group: GPP_D, Pad: xxgpp_d_17, Function: 6 - IOM_GPPD_17
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_17__IOM_GPPD_17               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_17, 6)
// Group: GPP_D, Pad: xxgpp_d_18, Function: 1 - srcclkreq6_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_18__SRCCLKREQ6_B              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_18, 1)
// Group: GPP_D, Pad: xxgpp_d_18, Function: 6 - IOM_GPPD_18
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_18__IOM_GPPD_18               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_18, 6)
// Group: GPP_D, Pad: xxgpp_d_19, Function: 2 - tbt_lsx0_oe
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_19__TBT_LSX0_OE               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_19, 2)
// Group: GPP_D, Pad: xxgpp_d_19, Function: 6 - IOM_GPPD_19
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_19__IOM_GPPD_19               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_19, 6)
// Group: GPP_D, Pad: xxgpp_d_20, Function: 1 - srcclkreq7_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_20__SRCCLKREQ7_B              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_20, 1)
// Group: GPP_D, Pad: xxgpp_d_20, Function: 6 - IOM_GPPD_20
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_20__IOM_GPPD_20               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_20, 6)
// Group: GPP_D, Pad: xxgpp_d_21, Function: 1 - ufs_refclk
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_21__UFS_REFCLK                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_21, 1)
// Group: GPP_D, Pad: xxgpp_d_21, Function: 2 - srcclkreq8_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_21__SRCCLKREQ8_B              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_21, 2)
// Group: GPP_D, Pad: xxgpp_d_21, Function: 6 - IOM_GPPD_21
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_21__IOM_GPPD_21               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_21, 6)
// Group: GPP_D, Pad: xxgpp_d_22, Function: 1 - bpki3c_sda
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_22__BPKI3C_SDA                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_22, 1)
// Group: GPP_D, Pad: xxgpp_d_22, Function: 6 - IOM_GPPD_22
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_22__IOM_GPPD_22               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_22, 6)
// Group: GPP_D, Pad: xxgpp_d_23, Function: 1 - bpki3c_scl
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_23__BPKI3C_SCL                GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_23, 1)
// Group: GPP_D, Pad: xxgpp_d_23, Function: 6 - IOM_GPPD_23
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_23__IOM_GPPD_23               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_23, 6)
// Group: GPP_D, Pad: xxgpp_d_24, Function: 1 - espi_alert2_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_24__ESPI_ALERT2_B             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_24, 1)
// Group: GPP_D, Pad: xxgpp_d_24, Function: 6 - IOM_GPPD_24
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_24__IOM_GPPD_24               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_24, 6)
// Group: GPP_D, Pad: xxgpp_d_25, Function: 1 - espi_alert3_b
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_25__ESPI_ALERT3_B             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_25, 1)
// Group: GPP_D, Pad: xxgpp_d_25, Function: 6 - IOM_GPPD_25
#define GPIOV2_PTL_PCD_MUXING__XXGPP_D_25__IOM_GPPD_25               GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_XXGPP_D_25, 6)
// Group: GPP_D, Pad: shd_i3c_clk_loopbk, Function: 4 - shd_i3c_clk_loopbk
#define GPIOV2_PTL_PCD_MUXING__SHD_I3C_CLK_LOOPBK__SHD_I3C_CLK_LOOPBK GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_SHD_I3C_CLK_LOOPBK, 4)

//
// Pin muxing definitions for Group: vGPIO.
//

// Group: vGPIO, Pad: vGPIO_30, Function: 1 - i2s0_sclk
#define GPIOV2_PTL_PCD_MUXING__VGPIO_30__I2S0_SCLK                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_30, 1)
// Group: vGPIO, Pad: vGPIO_30, Function: 2 - i2s1_sclk
#define GPIOV2_PTL_PCD_MUXING__VGPIO_30__I2S1_SCLK                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_30, 2)
// Group: vGPIO, Pad: vGPIO_30, Function: 3 - i2s2_sclk
#define GPIOV2_PTL_PCD_MUXING__VGPIO_30__I2S2_SCLK                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_30, 3)
// Group: vGPIO, Pad: vGPIO_31, Function: 1 - i2s0_sfrm
#define GPIOV2_PTL_PCD_MUXING__VGPIO_31__I2S0_SFRM                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_31, 1)
// Group: vGPIO, Pad: vGPIO_31, Function: 2 - i2s1_sfrm
#define GPIOV2_PTL_PCD_MUXING__VGPIO_31__I2S1_SFRM                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_31, 2)
// Group: vGPIO, Pad: vGPIO_31, Function: 3 - i2s2_sfrm
#define GPIOV2_PTL_PCD_MUXING__VGPIO_31__I2S2_SFRM                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_31, 3)
// Group: vGPIO, Pad: vGPIO_32, Function: 1 - i2s0_rxd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_32__I2S0_RXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_32, 1)
// Group: vGPIO, Pad: vGPIO_32, Function: 2 - i2s1_rxd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_32__I2S1_RXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_32, 2)
// Group: vGPIO, Pad: vGPIO_32, Function: 3 - i2s2_rxd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_32__I2S2_RXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_32, 3)
// Group: vGPIO, Pad: vGPIO_33, Function: 1 - i2s0_txd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_33__I2S0_TXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_33, 1)
// Group: vGPIO, Pad: vGPIO_33, Function: 2 - i2s1_txd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_33__I2S1_TXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_33, 2)
// Group: vGPIO, Pad: vGPIO_33, Function: 3 - i2s2_txd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_33__I2S2_TXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_33, 3)
// Group: vGPIO, Pad: vGPIO_34, Function: 1 - cnv_bt_i2s_bclk
#define GPIOV2_PTL_PCD_MUXING__VGPIO_34__CNV_BT_I2S_BCLK             GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_34, 1)
// Group: vGPIO, Pad: vGPIO_35, Function: 1 - cnv_bt_i2s_ws_sync
#define GPIOV2_PTL_PCD_MUXING__VGPIO_35__CNV_BT_I2S_WS_SYNC          GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_35, 1)
// Group: vGPIO, Pad: vGPIO_36, Function: 1 - cnv_bt_i2s_sdi
#define GPIOV2_PTL_PCD_MUXING__VGPIO_36__CNV_BT_I2S_SDI              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_36, 1)
// Group: vGPIO, Pad: vGPIO_37, Function: 1 - cnv_bt_i2s_sdo
#define GPIOV2_PTL_PCD_MUXING__VGPIO_37__CNV_BT_I2S_SDO              GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_37, 1)
// Group: vGPIO, Pad: vGPIO_40, Function: 1 - i2s0_sclk
#define GPIOV2_PTL_PCD_MUXING__VGPIO_40__I2S0_SCLK                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_40, 1)
// Group: vGPIO, Pad: vGPIO_40, Function: 2 - i2s1_sclk
#define GPIOV2_PTL_PCD_MUXING__VGPIO_40__I2S1_SCLK                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_40, 2)
// Group: vGPIO, Pad: vGPIO_40, Function: 3 - i2s2_sclk
#define GPIOV2_PTL_PCD_MUXING__VGPIO_40__I2S2_SCLK                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_40, 3)
// Group: vGPIO, Pad: vGPIO_41, Function: 1 - i2s0_sfrm
#define GPIOV2_PTL_PCD_MUXING__VGPIO_41__I2S0_SFRM                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_41, 1)
// Group: vGPIO, Pad: vGPIO_41, Function: 2 - i2s1_sfrm
#define GPIOV2_PTL_PCD_MUXING__VGPIO_41__I2S1_SFRM                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_41, 2)
// Group: vGPIO, Pad: vGPIO_41, Function: 3 - i2s2_sfrm
#define GPIOV2_PTL_PCD_MUXING__VGPIO_41__I2S2_SFRM                   GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_41, 3)
// Group: vGPIO, Pad: vGPIO_42, Function: 1 - i2s0_rxd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_42__I2S0_RXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_42, 1)
// Group: vGPIO, Pad: vGPIO_42, Function: 2 - i2s1_rxd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_42__I2S1_RXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_42, 2)
// Group: vGPIO, Pad: vGPIO_42, Function: 3 - i2s2_rxd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_42__I2S2_RXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_42, 3)
// Group: vGPIO, Pad: vGPIO_43, Function: 1 - i2s0_txd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_43__I2S0_TXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_43, 1)
// Group: vGPIO, Pad: vGPIO_43, Function: 2 - i2s1_txd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_43__I2S1_TXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_43, 2)
// Group: vGPIO, Pad: vGPIO_43, Function: 3 - i2s2_txd
#define GPIOV2_PTL_PCD_MUXING__VGPIO_43__I2S2_TXD                    GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_43, 3)
// Group: vGPIO, Pad: vGPIO_44, Function: 1 - cnv_bt_i2s_bclk_2
#define GPIOV2_PTL_PCD_MUXING__VGPIO_44__CNV_BT_I2S_BCLK_2           GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_44, 1)
// Group: vGPIO, Pad: vGPIO_45, Function: 1 - cnv_bt_i2s_ws_sync_2
#define GPIOV2_PTL_PCD_MUXING__VGPIO_45__CNV_BT_I2S_WS_SYNC_2        GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_45, 1)
// Group: vGPIO, Pad: vGPIO_46, Function: 1 - cnv_bt_i2s_sdi_2
#define GPIOV2_PTL_PCD_MUXING__VGPIO_46__CNV_BT_I2S_SDI_2            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_46, 1)
// Group: vGPIO, Pad: vGPIO_47, Function: 1 - cnv_bt_i2s_sdo_2
#define GPIOV2_PTL_PCD_MUXING__VGPIO_47__CNV_BT_I2S_SDO_2            GPIOV2_PAD_SET_NATIVE_FUNCTION (GPIOV2_PTL_PCD_VGPIO_47, 1)


#endif // _GPIOV2_PINS_PTL_PCD_H_
