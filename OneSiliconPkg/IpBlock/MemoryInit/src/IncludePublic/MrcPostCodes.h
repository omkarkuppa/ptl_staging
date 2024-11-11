/** @file
  This file defines all the MRC debug hooks.

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
#ifndef _MRCPOSTCODES_H_
#define _MRCPOSTCODES_H_

#define MRC_FAILURE_INDICATION          (0xDF)    /// This value is or'ed with below high byte of "*_ERROR" codes
#define MRC_TASKEND_INDICATION          (0x0200)  /// OR'ed with a POST code to indicate the end of a task

#define MRC_INITIALIZATION_START        (0xDD00)
#define MRC_INITIALIZATION_END          (MRC_INITIALIZATION_START | 0x7F)

#define MRC_INTERPRETER                 (0xDD01)
#define MRC_MC_DESWIZZLE_REG            (0xDD02)
#define MRC_DCC_PI_SERIALIZER_CAL       (0xDD03)
#define MRC_VDDQ                        (0xDD04)
#define MRC_CMD_EQ                      (0xDD05)
#define MRC_VCC_LVR_AUTO_TRIM           (0xDD06)
#define MRC_CMD_SR                      (0xDD07)
#define MRC_DCC_TLINE_CLK_CAL           (0xDD08)
#define MRC_UC_RESET                    (0xDD09)
#define MRC_UC_LOAD_GREEN_CODE          (0xDD0A)
#define MRC_UC_LOAD_GREEN_DATA          (0xDD0B)
#define MRC_UNMATCHED_RX_CAL_PRE        (0xDD0C)
#define MRC_DCC_LPDDR5_WCK              (0xDD0D)
#define MRC_DATA_PI_LINEARITY           (0xDD0E)
#define MRC_VIEW_PIN_CAL                (0xDD0F)
#define MRC_RESET_SEQUENCE_BLUE         (0xDD10)
#define MRC_FREQ_LOCK                   (0xDD11)
#define MRC_DDRIO_INIT                  (0xDD12)
#define MRC_UC_EXECUTE_GREEN            (0xDD13)
#define MRC_DDRCOMP_CAL_PRE             (0xDD14)
#define MRC_DCC_DATA_DQ                 (0xDD15)
#define MRC_READ_VREF_DECAP             (0xDD16)
#define MRC_DIMM_RX_OFFSET              (0xDD17)
#define MRC_TXDQTCO_COMP_TRAINING       (0xDD18)
#define MRC_MARGIN_LIMIT_CHECK          (0xDD19)
#define MRC_DCC_PI_CODE_LUT             (0xDD1A)
#define MRC_FAST_BOOT_PERMITTED         (0xDD1B)
#define MRC_RESTORE_NON_TRAINING        (0xDD1C)
#define MRC_PRINT_INPUT_PARAMS          (0xDD1D)
#define MRC_DUNIT_CONFIG                (0xDD1E)
#define MRC_QCLK_DCC                    (0xDD1F)
#define MRC_SPD_PROCESSING_STATIC       (0xDD20)
#define MRC_SET_OVERRIDES               (0xDD21)
#define MRC_MC_CAPABILITY               (0xDD22)
#define MRC_MC_CONFIG                   (0xDD23)
#define MRC_PPR_ENABLE                  (0xDD24)
#define MRC_PRINT_DDR_MRS               (0xDD25)
#define MRC_RESET_SEQUENCE_GREEN        (0xDD26)
#define MRC_PRE_TRAINING                (0xDD27)
#define MRC_EARLY_COMMAND               (0xDD28)
#define MRC_SENSE_AMP_OFFSET            (0xDD29)
#define MRC_READ_MPR                    (0xDD2A)
#define MRC_RECEIVE_ENABLE              (0xDD2B)
#define MRC_JEDEC_WRITE_LEVELING        (0xDD2C)
#define MRC_DLL_BW_SEL                  (0xDD2D)
#define MRC_WRITE_TIMING_1D             (0xDD2E)
#define MRC_READ_TIMING_1D              (0xDD2F)
#define MRC_DIMM_ODT                    (0xDD30)
#define MRC_EARLY_WRITE_TIMING_2D       (0xDD31)
#define MRC_WRITE_DS                    (0xDD32)
#define MRC_WRITE_EQ                    (0xDD33)
#define MRC_EARLY_READ_TIMING_2D        (0xDD34)
#define MRC_READ_DQS_ODT                (0xDD35)
#define MRC_READ_EQ                     (0xDD36)
#define MRC_READ_AMP_POWER              (0xDD37)
#define MRC_WRITE_TIMING_2D             (0xDD38)
#define MRC_READ_TIMING_2D              (0xDD39)
#define MRC_CMD_VREF                    (0xDD3A)
#define MRC_WRITE_VREF_2D               (0xDD3B)
#define MRC_READ_VREF_2D                (0xDD3C)
#define MRC_RXDQS_DCC                   (0xDD3D)
#define MRC_LATE_COMMAND                (0xDD3E)
//Place Holder
#define MRC_VCC_CLK_RX_FF_CAL           (0xDD3F)
#define MRC_TURN_AROUND                 (0xDD40)
#define MRC_CMP_OPT                     (0xDD41)
#define MRC_SAVE_MC_VALUES              (0xDD42)
#define MRC_RESTORE_TRAINING            (0xDD43)
#define MRC_RMT_TOOL                    (0xDD44)
//#define MRC_                            (0xDD45)
#define MRC_DIMM_RON                    (0xDD46)
#define MRC_DCC_PHASE_CLK_CAL           (0xDD47)
#define MRC_MR_FILL                     (0xDD48)
#define MRC_PWR_MTR                     (0xDD49)
#define MRC_SPD_PROCESSING_CALC         (0xDD4A)
#define MRC_WRITE_VOLTAGE_1D            (0xDD4B)
#define MRC_EARLY_RDMPR_TIMING_2D       (0xDD4C)
#define MRC_DIMM_DFE                    (0xDD4D)
#define MRC_ROUNDTRIP_MATCH             (0xDD4E)
#define MRC_DBI                         (0xDD4F)
#define MRC_DATA_SUMMARY                (0xDD50)
#define MRC_RH_PREVENTION               (0xDD51)
#define MRC_DDR5_ODT_CONFIG             (0xDD52)
#define MRC_WRITE_DQ_DQS_RETRAINING     (0xDD53)
#define MRC_APPLY_MC_REG_OFFSETS        (0xDD54)


///
/// To have distinct post codes for debugging purposes, do not define a training step
/// value to have the same lower byte value as MRC_MEM_INIT_DONE.  This value is
/// specific to signal the completion of the module.  This holds higher signifiance
/// on systems that do not use the high byte.
///
/// #define MRC_DO_NOT_USE                (0x__55)
///
#define MRC_MEM_INIT_DONE               (0x0055)
#define MRC_MEM_INIT_DONE_WITH_ERRORS   ((MRC_FAILURE_INDICATION << 8) | MRC_MEM_INIT_DONE)

#define MRC_TXTCO_COMP_TRAINING         (0xDD56)
#define MRC_READ_VOLTAGE_1D             (0xDD57)
#define MRC_RETRAIN_CHECK               (0xDD58)
#define MRC_RMT_TOOL_BIT                (0xDD59)
#define MRC_SA_GV_SWITCH                (0xDD5A)
#define MRC_ALIAS_CHECK                 (0xDD5B)
#define MRC_ECC_CLEAN_START             (0xDD5C)
#define MRC_WCK_PAD_DCC                 (0xDD5D)
#define MRC_DDR5_RX_XTALK               (0xDD5E)
#define MRC_DLL_DCC_CAL                 (0xDD5F)
#define MRC_LP5_DVFSQ_VOLTAGE_RAMP      (0xDD60)
#define MRC_WCK_CLK_PRE_DRIVER_DCC      (0xDD61)
#define MRC_CLKTCO_COMP_TRAINING        (0xDD62)
#define MRC_DIMM_ODT_NT                 (0xDD63)
#define MRC_DDR5_CKD_CONFIG             (0xDD64)
#define MRC_WCKTCO_COMP_TRAINING        (0xDD65)
#define MRC_READ_CTLE                   (0xDD66)
#define MRC_DIMM_ODT_CA                 (0xDD67)
#define MRC_DDRIO_FINALIZE_POST_DFI     (0xDD68)
#define MRC_FILL_BDAT_STRUCTURE         (0xDD69)
#define MRC_WCK_LEVELING                (0xDD6A)
#define MRC_MBIST                       (0xDD6B)
#define MRC_UNMATCHED_WRITE_TIMING_1D   (0xDD6C)
#define MRC_SWITCH_DFI_CONTROL          (0xDD6D)
#define MRC_MTG_REG_ACCESS_TEST         (0xDD6E)
#define MRC_DCC_RISE_FALL_ALIGNMENT     (0xDD6F)
#define MRC_SELF_REFRESH_EXIT           (0xDD70)
#define MRC_NORMAL_MODE                 (0xDD71)
#define MRC_MC_FINALIZE                 (0xDD72)
#define MRC_VCCCLK_FFOFFCORR            (0xDD73)
#define MRC_DCC_LP5_READ_DCA            (0xDD74)
#define MRC_DCC_LP5_WCK_DCA             (0xDD75)
#define MRC_DCC_DDR5_CLK                (0xDD76)
#define MRC_IBECC                       (0xDD77)
#define MRC_UPDATE_SAVE_MC_VALUES       (0xDD78)
#define MRC_MAP_DQDQS_SWIZZLE           (0xDD79)
#define MRC_RMT_TOOL_EVENODD            (0xDD7A)
#define MRC_PMA_CONFIG                  (0xDD7B)
#define MRC_DDRIO_FINALIZE              (0xDD7C)
#define MRC_REFPI_CAL                   (0xDD7D)
#define MRC_NO_MEMORY_DETECTED          ((MRC_FAILURE_INDICATION << 8) | 0x7E)
#define MRC_DCC_DDR5_DQS                (0xDD7F)

// Do not use 0xDD80 since this was the old way of indicating MRC failure. It could add confusion
//#define MRC_DO_NOT_USE                (0xDD80)    // Bit [7] is used to indicate task failure - see MRC_FAILURE_INDICATION

#define MRC_READ_DQ_ODT                 (0xDD81)
#define MRC_LP5_DRAM_SUPPORT_DVFSC      (0xDD82)
#define MRC_EMPHASIS                    (0xDD83)
#define MRC_HVM_FINALIZE                (0xDD84)
#define MRC_DQ_LOOPBACK_TEST            (0xDD85)
#define MRC_DQS_PAD_DCC                 (0xDD86)
#endif
