/** @file
  EDP_PGA_REGION.h

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

@par Specification

  This file contains Silicon register definitions.

  This is a generated file; please do not modify it directly.

**/

/* The following security policy groups are used by registers in this file:     */
/* P_U_CODE_POLICY_0                                                            */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | ESE_SAI | PM_IOSS_SAI |            */
/*    DFX_INTEL_MANUFACTURING_SAI | CSE_SAI | RC_MORPHED_SAI |                  */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                             */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI | HOSTIA_SUNPASS_SAI |                  */
/*    HOSTIA_BOOT_SAI | ESE_SAI | PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI |   */
/*    CSE_SAI | DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                   */

#ifndef _EDP_PGA_REGION3_PTL_CDIE_H_
#define _EDP_PGA_REGION3_PTL_CDIE_H_

#include <Base.h>

//************************************************ RegisterStructs


/** PRIVREG4_EDP_PGA_REGION desc:
  * Register default value:        0x00000000
  * Register full path in IP: EDP_PGA/edp_pga_wrapper_top/edp_btrs_reg_top/edp_pga_region1_reg/PRIVREG4
  * Std path: VER=1, UVM=EDP_PGA.edp_pga_wrapper_top.edp_btrs_reg_top.PRIVREG4, OSX=EDP_PGA.edp_pga_wrapper_top.edp_btrs_reg_top.edp_pga_region1_reg.PRIVREG4
  * Security PolicyGroup: P_U_CODE_POLICY_0
  * EDP PGA PRIVREG4
  */

#define PRIVREG4_EDP_PGA_REGION_REG      (0x00003114U)

typedef union {
  struct {
    UINT32 forceoverallon : 1; /**< Force Overall ON */

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               force_overall_on
                               [br]Overall override to ensure all the following
                               force_* are set to power on:
                               [br]- force_sram_pwr_gate to 0
                               [br]- force_pma_pwr_gate to 0
                               [br]- force_pwr_on to 1
                               [br]- force_phy_reset to 0
                               [br]- force_phy_de_reset to 1
                            */

    UINT32 forcesrampwrgate : 1; /**< Force SRAM Power Gate */

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               force_sram_pwr_gate
                               [br]Override to force power gate SRAM
                            */

    UINT32 forcepmapwrgate : 1; /**< Force PMA Power Gate */

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               force_pma_pwr_gate
                               [br]Override to force off PHY PMA switch
                            */

    UINT32 forcepwron : 1; /**< Force Power On */

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               force_pwr_on
                               [br]Override to force ungate the FET controlling
                               PCS PGD, SRAM and USB3 PHY PMA switch
                            */

    UINT32 rsvd1 : 6; /**< Reserved */

                            /* Bits[9:4], Access Type=RW, default=0x00000000*/

                            /* Reserved */

    UINT32 forcephyrst : 1; /**< Force PHY Reset */

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               force_phy_reset
                               [br]Override to assert the PHY reset at the
                               PCS boundary.
                            */

    UINT32 forcephyderst : 1; /**< Force PHY DE Reset */

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               force_phy_de_reset
                               [br]Override to deassert the PHY reset at
                               the PCS boundary.
                            */

    UINT32 rsvd2 : 9; /**< Reserved */

                            /* Bits[20:12], Access Type=RW, default=0x00000000*/

                            /* Reserved */

    UINT32 mbistsramclksel : 1; /**< MBIST SRAM Clock Select */

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Configuration for MBIST SRAM Clock Select
                               (cfg_scr_dfx_mbist_sram_clk_sel)
                               [br]Refer to Section 19.1.1 for more info.
                               [br]This is 1-bit register shared by all PHY
                               Macros for MP x8.
                               [br]1: SRAM Clock will be sourced from SOC
                               [br]0: SRAM Clock will be sourced from PHY
                            */

    UINT32 rsvd3 : 6; /**< Reserved */

                            /* Bits[27:22], Access Type=RW, default=0x00000000*/

                            /* Reserved */

    UINT32 tsncfgcrif : 1; /**< TSN Configuration for CRI Interface */

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               tsn_config_crif
                               [br]0: PIA owns CR IF and sram_ext_ld_done
                               for TSN enabled macro. pia_pcs_tsn_mux2_sel
                               will be 0
                               [br]1: TSN XPCS owns CR IF and sram_ext_ld_done
                               for TSN enabled macro. pia_pcs_tsn_mux2_sel
                               will be 1 for TSN lanes.
                               [br]
                               [br]Shall only be used for survivability and
                               debug.
                               [br]This register shall only be changed when
                               Chassis Buttress CR Interface is in idle state.
                               [br]
                               [br]PHY Pin through PIA: sel_xmux_phy0
                               [br]This signal is overridden to 0 when soc_pia_tsn_ovr
                               =1.
                            */

    UINT32 tsncfgphyrst : 1; /**< TSN Configuration for PHY Reset */

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               tsn_config_phyrst
                               [br]0: PIA owns PHY Reset for TSN enabled
                               macro. pia_pcs_tsn_mux2cmn_sel will be 0
                               [br]1: TSN XPCS owns PHY Reset for TSN enabled
                               macro. pia_pcs_tsn_mux2cmn_sel will be 1 for
                               TSN lanes.
                               [br]
                               [br]PHY Pin through PIA: sel_xmux_cmn
                               [br]This signal is NOT overridden to 0 when
                               soc_pia_tsn_ovr =1.
                            */

    UINT32 rsvd4 : 2; /**< Reserved */

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PRIVREG4_EDP_PGA_REGION_STRUCT;

#define PRIVREG4_EDP_PGA_REGION_WIDTH 32

#define PRIVREG4_EDP_PGA_REGION_FORCEOVERALLON_LSB 0x0000
#define PRIVREG4_EDP_PGA_REGION_FORCEOVERALLON_SIZE 0x0001
#define PRIVREG4_EDP_PGA_REGION_FORCESRAMPWRGATE_LSB 0x0001
#define PRIVREG4_EDP_PGA_REGION_FORCESRAMPWRGATE_SIZE 0x0001
#define PRIVREG4_EDP_PGA_REGION_FORCEPMAPWRGATE_LSB 0x0002
#define PRIVREG4_EDP_PGA_REGION_FORCEPMAPWRGATE_SIZE 0x0001
#define PRIVREG4_EDP_PGA_REGION_FORCEPWRON_LSB 0x0003
#define PRIVREG4_EDP_PGA_REGION_FORCEPWRON_SIZE 0x0001
#define PRIVREG4_EDP_PGA_REGION_RSVD1_LSB 0x0004
#define PRIVREG4_EDP_PGA_REGION_RSVD1_SIZE 0x0006
#define PRIVREG4_EDP_PGA_REGION_FORCEPHYRST_LSB 0x000a
#define PRIVREG4_EDP_PGA_REGION_FORCEPHYRST_SIZE 0x0001
#define PRIVREG4_EDP_PGA_REGION_FORCEPHYDERST_LSB 0x000b
#define PRIVREG4_EDP_PGA_REGION_FORCEPHYDERST_SIZE 0x0001
#define PRIVREG4_EDP_PGA_REGION_RSVD2_LSB 0x000c
#define PRIVREG4_EDP_PGA_REGION_RSVD2_SIZE 0x0009
#define PRIVREG4_EDP_PGA_REGION_MBISTSRAMCLKSEL_LSB 0x0015
#define PRIVREG4_EDP_PGA_REGION_MBISTSRAMCLKSEL_SIZE 0x0001
#define PRIVREG4_EDP_PGA_REGION_RSVD3_LSB 0x0016
#define PRIVREG4_EDP_PGA_REGION_RSVD3_SIZE 0x0006
#define PRIVREG4_EDP_PGA_REGION_TSNCFGCRIF_LSB 0x001c
#define PRIVREG4_EDP_PGA_REGION_TSNCFGCRIF_SIZE 0x0001
#define PRIVREG4_EDP_PGA_REGION_TSNCFGPHYRST_LSB 0x001d
#define PRIVREG4_EDP_PGA_REGION_TSNCFGPHYRST_SIZE 0x0001
#define PRIVREG4_EDP_PGA_REGION_RSVD4_LSB 0x001e
#define PRIVREG4_EDP_PGA_REGION_RSVD4_SIZE 0x0002


/** PHYCOMMONREG2_EDP_PGA_REGION desc:
  * Register default value:        0x30020000
  * Register full path in IP: EDP_PGA/edp_pga_wrapper_top/edp_btrs_reg_top/edp_pga_region3_reg/PHYCOMMONREG2
  * Std path: VER=1, UVM=EDP_PGA.edp_pga_wrapper_top.edp_btrs_reg_top.PHYCOMMONREG2, OSX=EDP_PGA.edp_pga_wrapper_top.edp_btrs_reg_top.edp_pga_region3_reg.PHYCOMMONREG2
  * Security PolicyGroup: P_U_CODE_POLICY_0
  * EDP PGA PHYCOMMONREG2
  */

#define PHYCOMMONREG2_EDP_PGA_REGION_REG (0x0000360cU)

typedef union {
  struct {
    UINT32 force_phy_reset : 1; /**< Phy Reset Override */

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               PHY_RESET_OVERRIDE: 0 : no change, 1: force
                               phy_reset to the EDP PHY to be asserted
                            */

    UINT32 cfg_tcsspwrack_control : 1; /**< TCSS Powr Ack Control */

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               CFG_TCSSPWRACK_CONTROL: Chicken Bit for Display
                               starting PTL
                            */

    UINT32 rsvd : 14; /**< Reserved */

                            /* Bits[15:2], Access Type=RW, default=0x00000000*/

                            /*
                               DISPLAY_SPARE: Future Chicken Bits for Display
                               when needed
                            */

    UINT32 i2c_mode : 1; /**< desc: i2c_mode */

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* PHY Pin: i2c_mode */

    UINT32 aux_mode : 1; /**< desc: aux_mode */

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /* PHY Pin: aux_mode */

    UINT32 i2c_vph_1v2_en : 1; /**< desc: i2c_vph_1v2_en */

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* PHY Pin: i2c_vph_1v2_en */

    UINT32 i2c_padn_pd : 1; /**< desc: i2c_padn_pd */

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /* PHY Pin: i2c_padn_pd */

    UINT32 i2c_padp_pd : 1; /**< desc: i2c_padp_pd */

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /* PHY Pin: i2c_padp_pd */

    UINT32 nen_rterm : 1; /**< desc: nen_rterm */

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* PHY Pin: nen_rterm */

    UINT32 aux_dp_dn_swap : 1; /**< desc: aux_dp_dn_swap */

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* PHY Pin: aux_dp_dn_swap */

    UINT32 aux_ctrl : 4; /**< desc: aux_ctrl */

                            /* Bits[26:23], Access Type=RW, default=0x00000000*/

                            /* PHY Pin: aux_ctrl */

    UINT32 aux_hys_tune : 2; /**< desc: aux_hys_tune */

                            /* Bits[28:27], Access Type=RW, default=0x00000002*/

                            /* PHY Pin: aux_hys_tune */

    UINT32 aux_vod_tune : 2; /**< desc: aux_vod_tune */

                            /* Bits[30:29], Access Type=RW, default=0x00000001*/

                            /* PHY Pin: aux_vod_tune */

    UINT32 phy_rtune_req : 1; /**< desc: phy_rtune_req */

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* PHY Pin: phy_rtune_req */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PHYCOMMONREG2_EDP_PGA_REGION_STRUCT;

#define PHYCOMMONREG2_EDP_PGA_REGION_WIDTH 32

#define PHYCOMMONREG2_EDP_PGA_REGION_FORCE_PHY_RESET_LSB 0x0000
#define PHYCOMMONREG2_EDP_PGA_REGION_FORCE_PHY_RESET_SIZE 0x0001
#define PHYCOMMONREG2_EDP_PGA_REGION_CFG_TCSSPWRACK_CONTROL_LSB 0x0001
#define PHYCOMMONREG2_EDP_PGA_REGION_CFG_TCSSPWRACK_CONTROL_SIZE 0x0001
#define PHYCOMMONREG2_EDP_PGA_REGION_RSVD_LSB 0x0002
#define PHYCOMMONREG2_EDP_PGA_REGION_RSVD_SIZE 0x000e
#define PHYCOMMONREG2_EDP_PGA_REGION_I2C_MODE_LSB 0x0010
#define PHYCOMMONREG2_EDP_PGA_REGION_I2C_MODE_SIZE 0x0001
#define PHYCOMMONREG2_EDP_PGA_REGION_AUX_MODE_LSB 0x0011
#define PHYCOMMONREG2_EDP_PGA_REGION_AUX_MODE_SIZE 0x0001
#define PHYCOMMONREG2_EDP_PGA_REGION_I2C_VPH_1V2_EN_LSB 0x0012
#define PHYCOMMONREG2_EDP_PGA_REGION_I2C_VPH_1V2_EN_SIZE 0x0001
#define PHYCOMMONREG2_EDP_PGA_REGION_I2C_PADN_PD_LSB 0x0013
#define PHYCOMMONREG2_EDP_PGA_REGION_I2C_PADN_PD_SIZE 0x0001
#define PHYCOMMONREG2_EDP_PGA_REGION_I2C_PADP_PD_LSB 0x0014
#define PHYCOMMONREG2_EDP_PGA_REGION_I2C_PADP_PD_SIZE 0x0001
#define PHYCOMMONREG2_EDP_PGA_REGION_NEN_RTERM_LSB 0x0015
#define PHYCOMMONREG2_EDP_PGA_REGION_NEN_RTERM_SIZE 0x0001
#define PHYCOMMONREG2_EDP_PGA_REGION_AUX_DP_DN_SWAP_LSB 0x0016
#define PHYCOMMONREG2_EDP_PGA_REGION_AUX_DP_DN_SWAP_SIZE 0x0001
#define PHYCOMMONREG2_EDP_PGA_REGION_AUX_CTRL_LSB 0x0017
#define PHYCOMMONREG2_EDP_PGA_REGION_AUX_CTRL_SIZE 0x0004
#define PHYCOMMONREG2_EDP_PGA_REGION_AUX_HYS_TUNE_LSB 0x001b
#define PHYCOMMONREG2_EDP_PGA_REGION_AUX_HYS_TUNE_SIZE 0x0002
#define PHYCOMMONREG2_EDP_PGA_REGION_AUX_VOD_TUNE_LSB 0x001d
#define PHYCOMMONREG2_EDP_PGA_REGION_AUX_VOD_TUNE_SIZE 0x0002
#define PHYCOMMONREG2_EDP_PGA_REGION_PHY_RTUNE_REQ_LSB 0x001f
#define PHYCOMMONREG2_EDP_PGA_REGION_PHY_RTUNE_REQ_SIZE 0x0001

//************************************************ /RegisterStructs




#endif      // _EDP_PGA_REGION_h

