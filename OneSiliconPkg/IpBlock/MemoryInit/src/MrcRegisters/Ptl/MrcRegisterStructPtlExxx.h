#ifndef __MrcRegisterStructPtlExxx_h__
#define __MrcRegisterStructPtlExxx_h__
/** @file
  This file was automatically generated. Modify at your own risk.
  Note that no error checking is done in these functions so ensure that the correct values are passed.

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

#pragma pack(push, 1)
typedef union {
  struct {
    UINT32 trppb                                   :  8;  // Bits 7:0
    UINT32                                         :  2;  // Bits 9:8
    UINT32 trpab                                   :  8;  // Bits 17:10
    UINT32                                         :  2;  // Bits 19:18
    UINT32 trdpre                                  :  7;  // Bits 26:20
    UINT32                                         :  1;  // Bits 27:27
    UINT32 tppd                                    :  4;  // Bits 31:28
    UINT32                                         :  1;  // Bits 32:32
    UINT32 twrpre                                  :  10;  // Bits 42:33
    UINT32                                         :  2;  // Bits 44:43
    UINT32 tras                                    :  9;  // Bits 53:45
    UINT32 nWR_diff                                :  5;  // Bits 58:54
    UINT32 derating_ext                            :  4;  // Bits 62:59
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_TC_PRE_STRUCT;
typedef union {
  struct {
    UINT32 tRDRD_sg                                :  7;  // Bits 6:0
    UINT32                                         :  1;  // Bits 7:7
    UINT32 tRDRD_dg                                :  7;  // Bits 14:8
    UINT32                                         :  1;  // Bits 15:15
    UINT32 tRDRD_dr                                :  8;  // Bits 23:16
    UINT32 tRDRD_dd                                :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TC_RDRD_STRUCT;
typedef union {
  struct {
    UINT32 tRDWR_sg                                :  8;  // Bits 7:0
    UINT32 tRDWR_dg                                :  8;  // Bits 15:8
    UINT32 tRDWR_dr                                :  8;  // Bits 23:16
    UINT32 tRDWR_dd                                :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TC_RDWR_STRUCT;
typedef union {
  struct {
    UINT32 tWRRD_sg                                :  9;  // Bits 8:0
    UINT32 tWRRD_dg                                :  9;  // Bits 17:9
    UINT32 tWRRD_dr                                :  7;  // Bits 24:18
    UINT32 tWRRD_dd                                :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TC_WRRD_STRUCT;
typedef union {
  struct {
    UINT32 tWRWR_sg                                :  7;  // Bits 6:0
    UINT32                                         :  1;  // Bits 7:7
    UINT32 tWRWR_dg                                :  7;  // Bits 14:8
    UINT32                                         :  1;  // Bits 15:15
    UINT32 tWRWR_dr                                :  7;  // Bits 22:16
    UINT32                                         :  1;  // Bits 23:23
    UINT32 tWRWR_dd                                :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TC_WRWR_STRUCT;
typedef union {
  struct {
    UINT32 MIN_PDWN_IDLE_COUNTER                   :  12;  // Bits 11:0
    UINT32 MAX_PDWN_IDLE_COUNTER                   :  12;  // Bits 23:12
    UINT32                                         :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_PM_ADAPTIVE_CKE_STRUCT;
typedef union {
  struct {
    UINT32 Rank_0_latency                          :  8;  // Bits 7:0
    UINT32 Rank_1_latency                          :  8;  // Bits 15:8
    UINT32 Rank_2_latency                          :  8;  // Bits 23:16
    UINT32 Rank_3_latency                          :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_STRUCT;
typedef union {
  struct {
    UINT32 TT_idle_counter                         :  8;  // Bits 7:0
    UINT32 Global_PD                               :  1;  // Bits 8:8
    UINT32 dis_cke_tt                              :  1;  // Bits 9:9
    UINT32 allow_opp_ref_below_write_threhold      :  1;  // Bits 10:10
    UINT32                                         :  21;  // Bits 31:11
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_STRUCT;
typedef union {
  struct {
    UINT32 dis_opp_cas                             :  1;  // Bits 0:0
    UINT32 dis_opp_is_cas                          :  1;  // Bits 1:1
    UINT32 dis_opp_ras                             :  1;  // Bits 2:2
    UINT32 dis_opp_is_ras                          :  1;  // Bits 3:3
    UINT32 dis_2c_byp                              :  1;  // Bits 4:4
    UINT32                                         :  1;  // Bits 5:5
    UINT32 dis_pt_it                               :  1;  // Bits 6:6
    UINT32 dis_prcnt_normal                        :  1;  // Bits 7:7
    UINT32 dis_prcnt_latency_tolerant              :  1;  // Bits 8:8
    UINT32 dis_blkr_ph                             :  1;  // Bits 9:9
    UINT32 dis_blkr_pe                             :  1;  // Bits 10:10
    UINT32 dis_blkr_pm                             :  1;  // Bits 11:11
    UINT32                                         :  1;  // Bits 12:12
    UINT32 OE_alw_off                              :  1;  // Bits 13:13
    UINT32 block_rpq                               :  1;  // Bits 14:14
    UINT32 block_ipq                               :  1;  // Bits 15:15
    UINT32 block_wpq                               :  1;  // Bits 16:16
    UINT32                                         :  1;  // Bits 17:17
    UINT32 dis_tt                                  :  1;  // Bits 18:18
    UINT32 dis_opp_ref                             :  1;  // Bits 19:19
    UINT32                                         :  4;  // Bits 23:20
    UINT32 dis_starved_read_rank_block             :  1;  // Bits 24:24
    UINT32                                         :  1;  // Bits 25:25
    UINT32 find_first_allocation                   :  1;  // Bits 26:26
    UINT32                                         :  4;  // Bits 30:27
    UINT32 dis_clk_gate                            :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SCHED_CBIT_STRUCT;
typedef union {
  struct {
    UINT32 lpddr_sagv_issue_pdx                    :  1;  // Bits 0:0
    UINT32 dis_ck_tristate                         :  1;  // Bits 1:1
    UINT32 revert_to_pre_gear4_cancel              :  1;  // Bits 2:2
    UINT32 dis_bus_retain                          :  1;  // Bits 3:3
    UINT32 dis_async_odt                           :  1;  // Bits 4:4
    UINT32 dis_trace_valid_on_lpmode               :  1;  // Bits 5:5
    UINT32 disable_starved_prio_on_new_req         :  1;  // Bits 6:6
    UINT32 lpddr_sagv_issue_pde                    :  1;  // Bits 7:7
    UINT32 dis_opp_ref_idle_delay                  :  1;  // Bits 8:8
    UINT32 dis_ignore_1st_trefi                    :  1;  // Bits 9:9
    UINT32                                         :  10;  // Bits 19:10
    UINT32 dis_new_wr_to_full                      :  1;  // Bits 20:20
    UINT32 cmd_bgf_always_on                       :  1;  // Bits 21:21
    UINT32                                         :  1;  // Bits 22:22
    UINT32 do_temp_reading_after_drainless_SAGV    :  1;  // Bits 23:23
    UINT32 ignore_refresh_between_SR               :  1;  // Bits 24:24
    UINT32 DisWrActThrottleOnAnyRead               :  1;  // Bits 25:25
    UINT32                                         :  3;  // Bits 28:26
    UINT32 disable_cas_clubbing                    :  1;  // Bits 29:29
    UINT32                                         :  1;  // Bits 30:30
    UINT32 disable_starv2med_prio_on_new_req       :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SCHED_SECOND_CBIT_STRUCT;
typedef union {
  struct {
    UINT32                                         :  1;  // Bits 0:0
    UINT32 WDB_Block_En                            :  1;  // Bits 1:1
    UINT32                                         :  2;  // Bits 3:2
    UINT32 isolate_data_return                     :  1;  // Bits 4:4
    UINT32                                         :  12;  // Bits 16:5
    UINT32 dft_block_enable                        :  1;  // Bits 17:17
    UINT32 SAM_overloading                         :  3;  // Bits 20:18
    UINT32 ipq_wcdp_act_resources                  :  4;  // Bits 24:21
    UINT32                                         :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_DFT_MISC_STRUCT;
typedef union {
  struct {
    UINT32 PCIT_SUBCH0                             :  10;  // Bits 9:0
    UINT32 PCIT_SUBCH1                             :  10;  // Bits 19:10
    UINT32 PCIT_GT                                 :  10;  // Bits 29:20
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_PCIT_STRUCT;
typedef union {
  struct {
    UINT32 ECC_Inject                              :  3;  // Bits 2:0
    UINT32                                         :  1;  // Bits 3:3
    UINT32 ECC_correction_disable                  :  1;  // Bits 4:4
    UINT32                                         :  27;  // Bits 31:5
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_ECC_DFT_STRUCT;
typedef union {
  struct {
    UINT32 DIS_MCA_LOG                             :  1;  // Bits 0:0
    UINT32 DIS_PCIE_ERR                            :  1;  // Bits 1:1
    UINT32 DIS_RCH_POISON                          :  1;  // Bits 2:2
    UINT32 DIS_RCH_ERROR                           :  1;  // Bits 3:3
    UINT32 ECC_trigger                             :  2;  // Bits 5:4
    UINT32                                         :  26;  // Bits 31:6
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_ECC_DFT_UARCH_STRUCT;
typedef union {
  struct {
    UINT32 PDWN_idle_counter_subch0                :  12;  // Bits 11:0
    UINT32 PDWN_idle_counter_subch1                :  12;  // Bits 23:12
    UINT32 APD                                     :  1;  // Bits 24:24
    UINT32 PPD                                     :  1;  // Bits 25:25
    UINT32                                         :  17;  // Bits 42:26
    UINT32 write_threshold                         :  7;  // Bits 49:43
    UINT32 lpmode_enable                           :  1;  // Bits 50:50
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_PM_PDWN_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 CERRSTS                                 :  1;  // Bits 0:0
    UINT32 CERR_OVERFLOW                           :  1;  // Bits 1:1
    UINT32 MERRSTS                                 :  1;  // Bits 2:2
    UINT32 MERR_OVERFLOW                           :  1;  // Bits 3:3
    UINT32                                         :  12;  // Bits 15:4
    UINT32 ERRSYND                                 :  8;  // Bits 23:16
    UINT32 ERRCHUNK                                :  3;  // Bits 26:24
    UINT32 ERRRANK                                 :  2;  // Bits 28:27
    UINT32 ERRBANK                                 :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_ECCERRLOG0_STRUCT;
typedef union {
  struct {
    UINT32 ERRROW                                  :  18;  // Bits 17:0
    UINT32 ERRCOL                                  :  8;  // Bits 25:18
    UINT32 ERRBANKGROUP                            :  3;  // Bits 28:26
    UINT32                                         :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_ECCERRLOG1_STRUCT;
typedef union {
  struct {
    UINT32 tcke                                    :  7;  // Bits 6:0
    UINT32 txp                                     :  7;  // Bits 13:7
    UINT32 tcpded                                  :  5;  // Bits 18:14
    UINT32 trdpden                                 :  8;  // Bits 26:19
    UINT32 twrpdenLow                              :  5;  // Bits 31:27
    UINT32 twrpdenHigh                             :  5;  // Bits 36:32
    UINT32 tckckeh                                 :  5;  // Bits 41:37
    UINT32 tcsh                                    :  6;  // Bits 47:42
    UINT32 tcsl                                    :  6;  // Bits 53:48
    UINT32 tcacsh                                  :  5;  // Bits 58:54
    UINT32 tprpden                                 :  5;  // Bits 63:59
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_TC_PWRDN_STRUCT;
typedef union {
  struct {
    UINT32 WPQ_disable                             :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_STRUCT;
typedef union {
  struct {
    UINT32 IPQ_disable                             :  16;  // Bits 15:0
    UINT32                                         :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_STRUCT;
typedef union {
  struct {
    UINT32 WPQ_disableLow                          :  32;  // Bits 31:0
    UINT32 WPQ_disableHigh                         :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_STRUCT;
typedef union {
  struct {
    UINT32 WMM_entry_wm                            :  7;  // Bits 6:0
    UINT32                                         :  1;  // Bits 7:7
    UINT32 WMM_exit_wm                             :  7;  // Bits 14:8
    UINT32                                         :  1;  // Bits 15:15
    UINT32 WIM_wm                                  :  7;  // Bits 22:16
    UINT32                                         :  1;  // Bits 23:23
    UINT32 write_isoc_cas_count                    :  6;  // Bits 29:24
    UINT32                                         :  2;  // Bits 31:30
    UINT32 read_cas_count                          :  6;  // Bits 37:32
    UINT32                                         :  2;  // Bits 39:38
    UINT32 write_cas_count                         :  6;  // Bits 45:40
    UINT32                                         :  2;  // Bits 47:46
    UINT32 write_cas_count_for_vc1                 :  6;  // Bits 53:48
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_SC_WDBWM_STRUCT;
typedef union {
  struct {
    UINT32 tccd_32_byte_cas_delta                  :  6;  // Bits 5:0
    UINT32                                         :  10;  // Bits 15:6
    UINT32 tCL                                     :  7;  // Bits 22:16
    UINT32                                         :  1;  // Bits 23:23
    UINT32 tCWL                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TC_CAS_STRUCT;
typedef union {
  struct {
    UINT32 Spare_RW                                :  16;  // Bits 15:0
    UINT32 dis_lpddr5_rdwr_interleaving            :  1;  // Bits 16:16
    UINT32 cbit_sagv_lpmode_masking_behavior       :  1;  // Bits 17:17
    UINT32                                         :  6;  // Bits 23:18
    UINT32 Spare_RW_V                              :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MCSCHEDS_SPARE_STRUCT;
typedef union {
  struct {
    UINT32 tmpc_setup                              :  5;  // Bits 4:0
    UINT32 tmpc_hold                               :  5;  // Bits 9:5
    UINT32 tmpc_cs                                 :  5;  // Bits 14:10
    UINT32                                         :  1;  // Bits 15:15
    UINT32 tSTAB                                   :  14;  // Bits 29:16
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TC_MPC_STRUCT;
typedef union {
  struct {
    UINT32 Read_Rank_0                             :  4;  // Bits 3:0
    UINT32 Read_Rank_1                             :  4;  // Bits 7:4
    UINT32 Read_Rank_2                             :  4;  // Bits 11:8
    UINT32 Read_Rank_3                             :  4;  // Bits 15:12
    UINT32 Write_Rank_0                            :  4;  // Bits 19:16
    UINT32 Write_Rank_1                            :  4;  // Bits 23:20
    UINT32 Write_Rank_2                            :  4;  // Bits 27:24
    UINT32 Write_Rank_3                            :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_ODT_MATRIX_STRUCT;
typedef union {
  struct {
    UINT32 dft_block_cycles                        :  16;  // Bits 15:0
    UINT32 dft_nonblock_cycles                     :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_DFT_BLOCK_STRUCT;
typedef union {
  struct {
    UINT32 dram_technology                         :  2;  // Bits 1:0
    UINT32 cmd_stretch                             :  1;  // Bits 2:2
    UINT32 address_mirror                          :  4;  // Bits 6:3
    UINT32 frequency_point                         :  2;  // Bits 8:7
    UINT32 enable_odt_matrix                       :  1;  // Bits 9:9
    UINT32 gear_mode                               :  1;  // Bits 10:10
    UINT32 ddr_1dpc_split_ranks_on_subch           :  2;  // Bits 12:11
    UINT32 lpddr5_bg_mode                          :  2;  // Bits 14:13
    UINT32 multicyccmd                             :  1;  // Bits 15:15
    UINT32 wckdifflowinidle                        :  1;  // Bits 16:16
    UINT32 cpgc_in_order                           :  1;  // Bits 17:17
    UINT32 cs_geardown_enable                      :  1;  // Bits 18:18
    UINT32 DDR5_CKD_enable                         :  1;  // Bits 19:19
    UINT32                                         :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_GS_CFG_STRUCT;
typedef union {
  struct {
    UINT32                                         :  2;  // Bits 1:0
    UINT32 disable_cs_tristate                     :  1;  // Bits 2:2
    UINT32 disable_tristate                        :  1;  // Bits 3:3
    UINT32 no_gear4_param_divide                   :  1;  // Bits 4:4
    UINT32 no_gear2_param_divide                   :  1;  // Bits 5:5
    UINT32 pl_trace_only_cs_cmd                    :  1;  // Bits 6:6
    UINT32 two_cycle_cke_idle                      :  1;  // Bits 7:7
    UINT32 disable_ca_tristate                     :  1;  // Bits 8:8
    UINT32 disable_ck_tristate                     :  1;  // Bits 9:9
    UINT32 pde_ca11                                :  1;  // Bits 10:10
    UINT32                                         :  1;  // Bits 11:11
    UINT32 force_pre_after_cas                     :  1;  // Bits 12:12
    UINT32 block_cas_when_synccounter_finishes     :  1;  // Bits 13:13
    UINT32 WCK_MRR_SHORTER_BL_DIS                  :  1;  // Bits 14:14
    UINT32 ALLOW_CAS_AFTER_WCKSTOP                 :  1;  // Bits 15:15
    UINT32 ddr5_ckd_ckcke_enforce_tstab            :  1;  // Bits 16:16
    UINT32 roundtrip_spec_stall_free_entries       :  4;  // Bits 20:17
    UINT32                                         :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_GS_CFG_UARCH_STRUCT;
typedef union {
  struct {
    UINT32 loaded_same_rank                        :  6;  // Bits 5:0
    UINT32                                         :  2;  // Bits 7:6
    UINT32 unloaded_same_rank                      :  6;  // Bits 13:8
    UINT32                                         :  2;  // Bits 15:14
    UINT32 loaded_different_rank                   :  6;  // Bits 21:16
    UINT32                                         :  2;  // Bits 23:22
    UINT32 unloaded_different_rank                 :  6;  // Bits 29:24
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_PH_THROTTLING_0_STRUCT;
typedef union {
  struct {
    UINT32 loaded_same_rank                        :  6;  // Bits 5:0
    UINT32                                         :  2;  // Bits 7:6
    UINT32 unloaded_same_rank                      :  6;  // Bits 13:8
    UINT32                                         :  2;  // Bits 15:14
    UINT32 loaded_different_rank                   :  6;  // Bits 21:16
    UINT32                                         :  2;  // Bits 23:22
    UINT32 unloaded_different_rank                 :  6;  // Bits 29:24
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_PH_THROTTLING_1_STRUCT;
typedef union {
  struct {
    UINT32 loaded_same_rank                        :  6;  // Bits 5:0
    UINT32                                         :  2;  // Bits 7:6
    UINT32 unloaded_same_rank                      :  6;  // Bits 13:8
    UINT32                                         :  2;  // Bits 15:14
    UINT32 loaded_different_rank                   :  6;  // Bits 21:16
    UINT32                                         :  2;  // Bits 23:22
    UINT32 unloaded_different_rank                 :  6;  // Bits 29:24
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_PH_THROTTLING_2_STRUCT;
typedef union {
  struct {
    UINT32 loaded_same_rank                        :  6;  // Bits 5:0
    UINT32                                         :  2;  // Bits 7:6
    UINT32 unloaded_same_rank                      :  6;  // Bits 13:8
    UINT32                                         :  2;  // Bits 15:14
    UINT32 loaded_different_rank                   :  6;  // Bits 21:16
    UINT32                                         :  2;  // Bits 23:22
    UINT32 unloaded_different_rank                 :  6;  // Bits 29:24
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_PH_THROTTLING_3_STRUCT;
typedef union {
  struct {
    UINT32 low_wm                                  :  6;  // Bits 5:0
    UINT32 med_wm                                  :  6;  // Bits 11:6
    UINT32 high_wm                                 :  6;  // Bits 17:12
    UINT32 phs_allowed_under_low_wm                :  4;  // Bits 21:18
    UINT32 phs_allowed_under_med_wm                :  4;  // Bits 25:22
    UINT32 phs_allowed_under_high_wm               :  4;  // Bits 29:26
    UINT32 read_ph_weight                          :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_WPQ_THRESHOLD_STRUCT;
typedef union {
  struct {
    UINT32 Normal_Timer                            :  10;  // Bits 9:0
    UINT32 Latency_Tolerant_Timer                  :  10;  // Bits 19:10
    UINT32 VC0_latency_guard_timer_x16             :  9;  // Bits 28:20
    UINT32                                         :  3;  // Bits 31:29
    UINT32 VC1_latency_guard_timer_x8              :  11;  // Bits 42:32
    UINT32 VC1_Isoc_latency_guard_timer_x8         :  11;  // Bits 53:43
    UINT32 Latency_Sensitive_Timer_x16             :  9;  // Bits 62:54
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_SC_PR_CNT_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 CKE_Override                            :  4;  // Bits 3:0
    UINT32 CS_Override_0                           :  4;  // Bits 7:4
    UINT32 CS_Override_1                           :  4;  // Bits 11:8
    UINT32                                         :  4;  // Bits 15:12
    UINT32 CKE_On                                  :  4;  // Bits 19:16
    UINT32 CS_Override_Val_0                       :  4;  // Bits 23:20
    UINT32 CS_Override_Val_1                       :  4;  // Bits 27:24
    UINT32                                         :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_STRUCT;
typedef union {
  struct {
    UINT32                                         :  8;  // Bits 7:0
    UINT32 ignore_cke                              :  1;  // Bits 8:8
    UINT32 mask_cs                                 :  1;  // Bits 9:9
    UINT32                                         :  1;  // Bits 10:10
    UINT32 disable_tristate                        :  1;  // Bits 11:11
    UINT32                                         :  5;  // Bits 16:12
    UINT32 Block_xarb                              :  1;  // Bits 17:17
    UINT32 Block_cke                               :  1;  // Bits 18:18
    UINT32                                         :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_GS_CFG_TRAINING_STRUCT;
typedef union {
  struct {
    UINT32 raise_cke_after_exit_latency            :  1;  // Bits 0:0
    UINT32 idle_length                             :  4;  // Bits 4:1
    UINT32 powerdown_latency                       :  5;  // Bits 9:5
    UINT32 powerdown_length                        :  4;  // Bits 13:10
    UINT32 selfrefresh_latency                     :  6;  // Bits 19:14
    UINT32 selfrefresh_length                      :  4;  // Bits 23:20
    UINT32                                         :  5;  // Bits 28:24
    UINT32 idle_enable                             :  1;  // Bits 29:29
    UINT32 powerdown_enable                        :  1;  // Bits 30:30
    UINT32 selfrefresh_enable                      :  1;  // Bits 31:31
    UINT32 active_length                           :  9;  // Bits 40:32
    UINT32 idle_latency                            :  4;  // Bits 44:41
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_SPID_LOW_POWER_CTL_STRUCT;
typedef union {
  struct {
    UINT32 dis_isoc_block_relaxation               :  1;  // Bits 0:0
    UINT32 no_global_block_with_bgf_invalid        :  1;  // Bits 1:1
    UINT32                                         :  1;  // Bits 2:2
    UINT32 write_ph_blocks_read_pre                :  1;  // Bits 3:3
    UINT32 row_press_dis_pcit_pre                  :  1;  // Bits 4:4
    UINT32 row_press_dis_autopre_new_req_miss      :  1;  // Bits 5:5
    UINT32 row_press_dis_vc1_autopre               :  1;  // Bits 6:6
    UINT32                                         :  4;  // Bits 10:7
    UINT32 preempt_vc1_during_demote               :  1;  // Bits 11:11
    UINT32 dis_idle_ref_start                      :  1;  // Bits 12:12
    UINT32                                         :  1;  // Bits 13:13
    UINT32 dis_oldest_isoch_pre_over_ph            :  1;  // Bits 14:14
    UINT32                                         :  1;  // Bits 15:15
    UINT32 dis_cke_off_in_refresh                  :  1;  // Bits 16:16
    UINT32 dis_1st_cas_split_act                   :  1;  // Bits 17:17
    UINT32 dis_wdb_clk_gate                        :  1;  // Bits 18:18
    UINT32 dis_mnt_clk_gate                        :  1;  // Bits 19:19
    UINT32 dis_scheds_clk_gate                     :  1;  // Bits 20:20
    UINT32 dis_lpmode_on_sagv                      :  1;  // Bits 21:21
    UINT32 dis_lpmode_on_r1blockdrain              :  1;  // Bits 22:22
    UINT32 dis_ignore_1st_zqcs                     :  1;  // Bits 23:23
    UINT32 ipq_ignore_write_ph                     :  1;  // Bits 24:24
    UINT32 cksynccounteroverride                   :  1;  // Bits 25:25
    UINT32 cksynccounteroverridevalue              :  2;  // Bits 27:26
    UINT32 asdynamicmodeinsertbubble               :  1;  // Bits 28:28
    UINT32 oppsr_pending_wr_clkgate_dis_mcchannel  :  1;  // Bits 29:29
    UINT32 bgfwakeupsoon_dis                       :  1;  // Bits 30:30
    UINT32 disable_split_act                       :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SCHED_THIRD_CBIT_STRUCT;
typedef union {
  struct {
    UINT32 No_CAS_threshold                        :  16;  // Bits 15:0
    UINT32 No_CAS_threshold_en                     :  1;  // Bits 16:16
    UINT32 Preemption_threshold_en                 :  1;  // Bits 17:17
    UINT32 Stop_count_during_tt                    :  1;  // Bits 18:18
    UINT32 Attempt_refresh_on_deadlock             :  1;  // Bits 19:19
    UINT32 MajorMode_force_duration                :  8;  // Bits 27:20
    UINT32 MajorMode_force_en                      :  1;  // Bits 28:28
    UINT32 Disable_blocking_rules                  :  1;  // Bits 29:29
    UINT32 Rank_join                               :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_DEADLOCK_BREAKER_STRUCT;

typedef MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_STRUCT MC0_CH0_CR_MCSCHEDS_GLOBAL_DRIVER_GATE_CFG_STRUCT;
typedef union {
  struct {
    UINT32 low_wm_allowed_preempt_priorities       :  3;  // Bits 2:0
    UINT32 med_wm_allowed_preempt_priorities       :  3;  // Bits 5:3
    UINT32 high_wm_allowed_preempt_priorities      :  3;  // Bits 8:6
    UINT32 ph_block_pe                             :  1;  // Bits 9:9
    UINT32 blkr_effect_major_mode                  :  1;  // Bits 10:10
    UINT32 RIM_BW_optimization                     :  1;  // Bits 11:11
    UINT32 RIM_BW_prefer_VC1                       :  1;  // Bits 12:12
    UINT32 loaded_read_threshold                   :  6;  // Bits 18:13
    UINT32 VC1_deadlock_by_stalled_VC0_handler_EN  :  1;  // Bits 19:19
    UINT32 VC1_deadlock_by_stalled_VC0_handler_COUNTER_RST:  9;  // Bits 28:20
    UINT32 VC1_deadlock_by_stalled_VC0_handler_BLOCK_ACT:  1;  // Bits 29:29
    UINT32 VC1_deadlock_by_stalled_VC0_handler_BLOCK_PRE:  1;  // Bits 30:30
    UINT32 VC1_deadlock_by_stalled_VC0_handler_BLOCK_PREEMPT:  1;  // Bits 31:31
    UINT32 VC0_starvation_by_stalled_VC1_handler_EN:  1;  // Bits 32:32
    UINT32                                         :  7;  // Bits 39:33
    UINT32 VC0_starvation_by_stalled_VC1_handler_CYCLES:  8;  // Bits 47:40
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_SC_BLOCKING_RULES_CFG_STRUCT;
typedef union {
  struct {
    UINT32 REQ_OCCUPANCY_COUNTERLow                :  32;  // Bits 31:0
    UINT32 REQ_OCCUPANCY_COUNTERHigh               :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_STRUCT;

typedef MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_STRUCT MC0_CH0_CR_PWM_DDR_SUBCH1_REQ_OCCUPANCY_COUNTER_STRUCT;
typedef union {
  struct {
    UINT32 Rank_0_trigger                          :  1;  // Bits 0:0
    UINT32 Rank_0_wr_en                            :  1;  // Bits 1:1
    UINT32 Rank_0_overflow                         :  1;  // Bits 2:2
    UINT32 Rank_1_trigger                          :  1;  // Bits 3:3
    UINT32 Rank_1_wr_en                            :  1;  // Bits 4:4
    UINT32 Rank_1_overflow                         :  1;  // Bits 5:5
    UINT32 Rank_2_trigger                          :  1;  // Bits 6:6
    UINT32 Rank_2_wr_en                            :  1;  // Bits 7:7
    UINT32 Rank_2_overflow                         :  1;  // Bits 8:8
    UINT32 Rank_3_trigger                          :  1;  // Bits 9:9
    UINT32 Rank_3_wr_en                            :  1;  // Bits 10:10
    UINT32 Rank_3_overflow                         :  1;  // Bits 11:11
    UINT32                                         :  20;  // Bits 31:12
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TR_RRDVALID_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 rank_0_sign                             :  1;  // Bits 0:0
    UINT32 rank_0_value                            :  7;  // Bits 7:1
    UINT32 rank_1_sign                             :  1;  // Bits 8:8
    UINT32 rank_1_value                            :  7;  // Bits 15:9
    UINT32 rank_2_sign                             :  1;  // Bits 16:16
    UINT32 rank_2_value                            :  7;  // Bits 23:17
    UINT32 rank_3_sign                             :  1;  // Bits 24:24
    UINT32 rank_3_value                            :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TR_RRDVALID_DATA_STRUCT;
typedef union {
  struct {
    UINT32 rd_wck_async_gap                        :  10;  // Bits 9:0
    UINT32 wr_wck_async_gap                        :  10;  // Bits 19:10
    UINT32 tWCKSTOP                                :  6;  // Bits 25:20
    UINT32                                         :  6;  // Bits 31:26
    UINT32 twckoff                                 :  9;  // Bits 40:32
    UINT32 casstop_additional_gap                  :  6;  // Bits 46:41
    UINT32                                         :  5;  // Bits 51:47
    UINT32 tCSLCK                                  :  5;  // Bits 56:52
    UINT32                                         :  1;  // Bits 57:57
    UINT32 tckfspx                                 :  6;  // Bits 63:58
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_TC_WCK_STRUCT;
typedef union {
  struct {
    UINT32 Dis_Opp_rd                              :  1;  // Bits 0:0
    UINT32 ACT_Enable                              :  1;  // Bits 1:1
    UINT32 PRE_Enable                              :  1;  // Bits 2:2
    UINT32 MAX_RPQ_CAS                             :  4;  // Bits 6:3
    UINT32                                         :  25;  // Bits 31:7
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_WMM_READ_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 PtrSep_ispid                            :  2;  // Bits 1:0
    UINT32 BgfRun_Ovrd_En                          :  1;  // Bits 2:2
    UINT32 BgfRun_Ovrd_Val                         :  1;  // Bits 3:3
    UINT32 BgfRun_Delay                            :  5;  // Bits 8:4
    UINT32 clkStop_override_en_on_BGF_Off          :  1;  // Bits 9:9
    UINT32 clkStop_override_val_on_BGF_Off         :  1;  // Bits 10:10
    UINT32 Enable_osc_override                     :  1;  // Bits 11:11
    UINT32 Enable_osc_check                        :  1;  // Bits 12:12
    UINT32 MC_2_PHY_mismatch                       :  1;  // Bits 13:13
    UINT32 PHY_2_MC_mismatch                       :  1;  // Bits 14:14
    UINT32 force_off_cmd_bgf_valid                 :  1;  // Bits 15:15
    UINT32                                         :  1;  // Bits 16:16
    UINT32 PtrSep_ospid                            :  2;  // Bits 18:17
    UINT32 cstristate_override_en_on_BGF_Off       :  1;  // Bits 19:19
    UINT32 cstristate_override_val_on_BGF_Off      :  1;  // Bits 20:20
    UINT32 BgfRst_Delay                            :  6;  // Bits 26:21
    UINT32 cs_override_en_on_BGF_Off               :  1;  // Bits 27:27
    UINT32 cs_override_val_on_BGF_Off              :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MC2PHY_BGF_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 ADAPTIVE_PCIT_WINDOW_MULT               :  3;  // Bits 2:0
    UINT32                                         :  7;  // Bits 9:3
    UINT32 MIN_PCIT                                :  10;  // Bits 19:10
    UINT32 MAX_PCIT                                :  10;  // Bits 29:20
    UINT32 CHANGE_RANGE                            :  2;  // Bits 31:30
    UINT32 EARLY_WEIGHT                            :  3;  // Bits 34:32
    UINT32 LATE_WEIGHT                             :  3;  // Bits 37:35
    UINT32 PH_WEIGHT                               :  3;  // Bits 40:38
    UINT32 GOOD_PAGE_CLOSE_WEIGHT                  :  3;  // Bits 43:41
    UINT32 TRANSACTION_FACTOR                      :  3;  // Bits 46:44
    UINT32 IGNORE_VC1                              :  1;  // Bits 47:47
    UINT32 IGNORE_GT                               :  1;  // Bits 48:48
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_SC_ADAPTIVE_PCIT_STRUCT;
typedef union {
  struct {
    UINT32 RH_ENABLED_DIMM0                        :  1;  // Bits 0:0
    UINT32 RH_ENABLED_DIMM1                        :  1;  // Bits 1:1
    UINT32 RH_HIGH_WM                              :  12;  // Bits 13:2
    UINT32 RH_LOW_WM                               :  11;  // Bits 24:14
    UINT32                                         :  7;  // Bits 31:25
    UINT32 NORMAL_REF_SUB                          :  12;  // Bits 43:32
    UINT32 REFM_SUB                                :  12;  // Bits 55:44
    UINT32                                         :  3;  // Bits 58:56
    UINT32 REFm_EN                                 :  1;  // Bits 59:59
    UINT32 RIR_DEC                                 :  1;  // Bits 60:60
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_ROWHAMMER_CTL_STRUCT;
typedef union {
  struct {
    UINT32                                         :  5;  // Bits 4:0
    UINT32 enable_ipq_req_merge                    :  1;  // Bits 5:5
    UINT32 enable_merge_req_vc1_low_prio           :  1;  // Bits 6:6
    UINT32 enable_rpq_req_merge                    :  1;  // Bits 7:7
    UINT32 enable_rpq_req_merge_reuse              :  1;  // Bits 8:8
    UINT32 enable_ipq_req_merge_reuse              :  1;  // Bits 9:9
    UINT32 disable_dyn_rpq_req_merge               :  1;  // Bits 10:10
    UINT32 enable_merge_ipq_when_old_vc1_req_exist :  1;  // Bits 11:11
    UINT32                                         :  20;  // Bits 31:12
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MERGE_REQ_READS_PQ_STRUCT;
typedef union {
  struct {
    UINT32 lp_ctrl_ack_enable                      :  1;  // Bits 0:0
    UINT32                                         :  31;  // Bits 31:1
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_DFI_CTL_STRUCT;
typedef union {
  struct {
    UINT32 AUTO_PRE_EN                             :  1;  // Bits 0:0
    UINT32 RPQ_AP_PM                               :  1;  // Bits 1:1
    UINT32 RPQ_AP_THRESHOLD                        :  6;  // Bits 7:2
    UINT32 WPQ_AP_THRESHOLD                        :  7;  // Bits 14:8
    UINT32 LPDDR5_AUTO_PRE_EN                      :  1;  // Bits 15:15
    UINT32                                         :  16;  // Bits 31:16
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_AUTO_PRE_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 tFAW                                    :  9;  // Bits 8:0
    UINT32 tRRD_sg                                 :  6;  // Bits 14:9
    UINT32 tRRD_dg                                 :  7;  // Bits 21:15
    UINT32 tRCD                                    :  8;  // Bits 29:22
    UINT32                                         :  2;  // Bits 31:30
    UINT32 tRCDW                                   :  8;  // Bits 39:32
    UINT32 RDA2ACT                                 :  9;  // Bits 48:40
    UINT32 WRA2ACT                                 :  10;  // Bits 58:49
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_TC_ACT_STRUCT;
typedef union {
  struct {
    UINT32 RPQ_disableLow                          :  32;  // Bits 31:0
    UINT32 RPQ_disableHigh                         :  16;  // Bits 47:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_STRUCT;
typedef union {
  struct {
    UINT32 twck_en_rd                              :  6;  // Bits 5:0
    UINT32 twck_toggle_rd                          :  7;  // Bits 12:6
    UINT32 twck_en_wr                              :  6;  // Bits 18:13
    UINT32 twck_toggle_wr                          :  7;  // Bits 25:19
    UINT32 twck_en_fs                              :  5;  // Bits 30:26
    UINT32                                         :  1;  // Bits 31:31
    UINT32 twck_toggle_fs                          :  5;  // Bits 36:32
    UINT32 twck_dfi_offset                         :  3;  // Bits 39:37
    UINT32 twck_dfi_stop_offset                    :  3;  // Bits 42:40
    UINT32 LP5_WCK_MODE                            :  2;  // Bits 44:43
    UINT32 WCK_FAST_MODE                           :  1;  // Bits 45:45
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_WCK_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 En_Wr_in_WIM_during_TT                  :  1;  // Bits 0:0
    UINT32                                         :  31;  // Bits 31:1
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_WDBWM_UARCH_STRUCT;
typedef union {
  struct {
    UINT32 REFm_REF_wm_block_sel                   :  1;  // Bits 0:0
    UINT32 REFab_before_REFm_on_IDLE               :  1;  // Bits 1:1
    UINT32 IGNORE_HIGH_WM_BLOCK                    :  1;  // Bits 2:2
    UINT32 REFm_Disable_on_hot                     :  1;  // Bits 3:3
    UINT32                                         :  28;  // Bits 31:4
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_ROWHAMMER_CTL_UARCH_STRUCT;
typedef union {
  struct {
    UINT32 TRR_DRFM_Dimm_Enabled                   :  2;  // Bits 1:0
    UINT32 DRFM_Enabled                            :  2;  // Bits 3:2
    UINT32                                         :  2;  // Bits 5:4
    UINT32 LFSR_0_MASK                             :  4;  // Bits 9:6
    UINT32 LFSR_1_MASK                             :  4;  // Bits 13:10
    UINT32 MA1_Swizzling                           :  2;  // Bits 15:14
    UINT32 MA2_Swizzling                           :  2;  // Bits 17:16
    UINT32 Micron_R_Swizzling                      :  2;  // Bits 19:18
    UINT32 Micron_F_Swizzling                      :  2;  // Bits 21:20
    UINT32                                         :  2;  // Bits 23:22
    UINT32 BRC                                     :  2;  // Bits 25:24
    UINT32 Blast_Radius_LFSR_MASK                  :  3;  // Bits 28:26
    UINT32 DDR5_DRFM_LFSR_MASK                     :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_RH_TRR_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 Refresh_Rank_Mask                       :  4;  // Bits 3:0
    UINT32                                         :  4;  // Bits 7:4
    UINT32 RefZQ_En_Start_Test_Sync                :  1;  // Bits 8:8
    UINT32                                         :  22;  // Bits 30:9
    UINT32 Panic_Refresh_Only                      :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 ZQ_Rank_Mask                            :  4;  // Bits 3:0
    UINT32                                         :  27;  // Bits 30:4
    UINT32 Always_Do_ZQ                            :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_STRUCT;
typedef union {
  struct {
    UINT32 tRFM                                    :  11;  // Bits 10:0
    UINT32 tDRFM                                   :  13;  // Bits 23:11
    UINT32                                         :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TC_REFM_STRUCT;
typedef union {
  struct {
    UINT32 BlockState_Delay                        :  4;  // Bits 3:0
    UINT32 Final_FSM_Delay                         :  3;  // Bits 6:4
    UINT32                                         :  25;  // Bits 31:7
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_RH_TRR_CONTROL_UARCH_STRUCT;
typedef union {
  struct {
    UINT32 Address                                 :  8;  // Bits 7:0
    UINT32 Data                                    :  8;  // Bits 15:8
    UINT32 Rank                                    :  4;  // Bits 19:16
    UINT32 Command                                 :  3;  // Bits 22:20
    UINT32 SPID_cmd_type                           :  3;  // Bits 25:23
    UINT32                                         :  4;  // Bits 29:26
    UINT32 Assume_idle                             :  1;  // Bits 30:30
    UINT32 Busy                                    :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_DDR_MR_COMMAND_STRUCT;
typedef union {
  struct {
    UINT32 R0_D0                                   :  8;  // Bits 7:0
    UINT32 R0_D1                                   :  8;  // Bits 15:8
    UINT32 R1_D0                                   :  8;  // Bits 23:16
    UINT32 R1_D1                                   :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_STRUCT;
typedef union {
  struct {
    UINT32 R0_D2                                   :  8;  // Bits 7:0
    UINT32 R0_D3                                   :  8;  // Bits 15:8
    UINT32 R1_D2                                   :  8;  // Bits 23:16
    UINT32 R1_D3                                   :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_STRUCT;
typedef union {
  struct {
    UINT32 R0_ECC                                  :  8;  // Bits 7:0
    UINT32 R1_ECC                                  :  8;  // Bits 15:8
    UINT32                                         :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_STRUCT;
typedef union {
  struct {
    UINT32 SCH0_RANK0_TEMP                         :  8;  // Bits 7:0
    UINT32 SCH0_RANK1_TEMP                         :  8;  // Bits 15:8
    UINT32 SCH1_RANK0_TEMP                         :  8;  // Bits 23:16
    UINT32 SCH1_RANK1_TEMP                         :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_RANK_TEMPERATURE_STRUCT;
typedef union {
  struct {
    UINT32                                         :  7;  // Bits 6:0
    UINT32 DISABLE_DRAM_TS                         :  1;  // Bits 7:7
    UINT32 MIN_REF_RATE                            :  3;  // Bits 10:8
    UINT32 MAX_REF_RATE                            :  3;  // Bits 13:11
    UINT32 MR4_PERIOD                              :  16;  // Bits 29:14
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_PM_CONFIG_THERM_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 Total_reads_entries                     :  6;  // Bits 5:0
    UINT32                                         :  2;  // Bits 7:6
    UINT32 VC0_reads_entries                       :  6;  // Bits 13:8
    UINT32                                         :  2;  // Bits 15:14
    UINT32 VC1_reads_entries                       :  6;  // Bits 21:16
    UINT32                                         :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MC_RDB_CREDITS_STRUCT;
typedef union {
  struct {
    UINT32 Valid                                   :  1;  // Bits 0:0
    UINT32 Rank                                    :  2;  // Bits 2:1
    UINT32 Group                                   :  2;  // Bits 4:3
    UINT32 Bank                                    :  3;  // Bits 7:5
    UINT32 Row                                     :  18;  // Bits 25:8
    UINT32 Do_Blast_Radius                         :  1;  // Bits 26:26
    UINT32 Do_DDR5_DRFM                            :  1;  // Bits 27:27
    UINT32                                         :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_RH_TRR_ADDRESS_STRUCT;
typedef union {
  struct {
    UINT32                                         :  11;  // Bits 10:0
    UINT32 Reset_refresh_debt                      :  1;  // Bits 11:11
    UINT32                                         :  1;  // Bits 12:12
    UINT32 VRCG_DVFSQ_enabled                      :  1;  // Bits 13:13
    UINT32                                         :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MC_INIT_STATE_UARCH_STRUCT;
typedef union {
  struct {
    UINT32 Refresh_HP_WM                           :  4;  // Bits 3:0
    UINT32 Refresh_panic_wm                        :  4;  // Bits 7:4
    UINT32 Wakeup_on_HP_WM                         :  1;  // Bits 8:8
    UINT32 PBR_Disable                             :  1;  // Bits 9:9
    UINT32 PBR_Exit_on_Idle_Cnt                    :  6;  // Bits 15:10
    UINT32 Refresh_ABR_release                     :  4;  // Bits 19:16
    UINT32 DDR5_RIR                                :  1;  // Bits 20:20
    UINT32                                         :  11;  // Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_RFP_STRUCT;
typedef union {
  struct {
    UINT32 OREF_RI                                 :  8;  // Bits 7:0
    UINT32 CounttREFIWhileRefEnOff                 :  1;  // Bits 8:8
    UINT32 SRX_Ref_Debits                          :  2;  // Bits 10:9
    UINT32 RAISE_BLK_WAIT                          :  4;  // Bits 14:11
    UINT32                                         :  17;  // Bits 31:15
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_RFP_UARCH_STRUCT;
typedef union {
  struct {
    UINT32 Ref_Interval                            :  13;  // Bits 12:0
    UINT32 Disable_Stolen_Refresh                  :  1;  // Bits 13:13
    UINT32 En_Ref_Type_Display                     :  1;  // Bits 14:14
    UINT32 Ref_Stagger_En                          :  1;  // Bits 15:15
    UINT32 Ref_Stagger_Mode                        :  1;  // Bits 16:16
    UINT32 Trefipulse_Stagger_Disable              :  1;  // Bits 17:17
    UINT32                                         :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MC_REFRESH_STAGGER_STRUCT;
typedef union {
  struct {
    UINT32 tZQLATCH                                :  11;  // Bits 10:0
    UINT32                                         :  5;  // Bits 15:11
    UINT32 tZQCAL                                  :  13;  // Bits 28:16
    UINT32                                         :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TC_ZQCAL_STRUCT;
typedef union {
  struct {
    UINT32 ZQCAL_period                            :  10;  // Bits 9:0
    UINT32                                         :  12;  // Bits 21:10
    UINT32 serialize_zq                            :  1;  // Bits 22:22
    UINT32                                         :  5;  // Bits 27:23
    UINT32 serial_zq_between_sub_ch                :  1;  // Bits 28:28
    UINT32                                         :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_ZQCAL_CONTROL_STRUCT;

typedef MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_STRUCT MC0_CH0_CR_SCH1_DDR_MR_RESULT_LOW_STRUCT;
typedef union {
  struct {
    UINT32 Rank_occupancy                          :  4;  // Bits 3:0
    UINT32                                         :  4;  // Bits 7:4
    UINT32 SRX_reset                               :  1;  // Bits 8:8
    UINT32 LPDDR_current_FSP                       :  1;  // Bits 9:9
    UINT32 LPDDR_current_FSP_tracking              :  1;  // Bits 10:10
    UINT32                                         :  1;  // Bits 11:11
    UINT32 Deep_SRX_reset                          :  1;  // Bits 12:12
    UINT32                                         :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MC_INIT_STATE_STRUCT;
typedef union {
  struct {
    UINT32 VISAByteSel                             :  3;  // Bits 2:0
    UINT32                                         :  5;  // Bits 7:3
    UINT32 RefFSMRankSel                           :  2;  // Bits 9:8
    UINT32 Ref_counterSel                          :  4;  // Bits 13:10
    UINT32 Enable_Ref_Status_Sample                :  1;  // Bits 14:14
    UINT32 RHFSMRankSel                            :  2;  // Bits 16:15
    UINT32                                         :  15;  // Bits 31:17
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_WDB_VISA_SEL_STRUCT;
typedef union {
  struct {
    UINT32 RD_RETRAIN_INTERVAL_FSM_status          :  4;  // Bits 3:0
    UINT32 WR_RETRAIN_INTERVAL_FSM_status          :  4;  // Bits 7:4
    UINT32 TEMP_RD_FSM_status                      :  3;  // Bits 10:8
    UINT32 ZQCS_FSM_status                         :  3;  // Bits 13:11
    UINT32 EXE_REF_FSM_status                      :  3;  // Bits 16:14
    UINT32 MAIN_REF_FSM_status                     :  4;  // Bits 20:17
    UINT32 MAIN_SR_FSM_status                      :  4;  // Bits 24:21
    UINT32                                         :  7;  // Bits 31:25
    UINT32 counter_status                          :  8;  // Bits 39:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_REF_FSM_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 DIMM0_ACT_ENERGY                        :  8;  // Bits 7:0
    UINT32 DIMM1_ACT_ENERGY                        :  8;  // Bits 15:8
    UINT32                                         :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_PM_DIMM_ACT_ENERGY_STRUCT;
typedef union {
  struct {
    UINT32 dimm0_rd_energy                         :  8;  // Bits 7:0
    UINT32 dimm1_rd_energy                         :  8;  // Bits 15:8
    UINT32 read_vc0_rapl_limit                     :  5;  // Bits 20:16
    UINT32 dis_vc1_rd_energy                       :  1;  // Bits 21:21
    UINT32                                         :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_PM_DIMM_RD_ENERGY_STRUCT;
typedef union {
  struct {
    UINT32 dimm0_wr_energy                         :  8;  // Bits 7:0
    UINT32 dimm1_wr_energy                         :  8;  // Bits 15:8
    UINT32 write_rapl_limit                        :  5;  // Bits 20:16
    UINT32 write_rapl_limit_vc1priority            :  5;  // Bits 25:21
    UINT32                                         :  6;  // Bits 31:26
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_PM_DIMM_WR_ENERGY_STRUCT;
typedef union {
  struct {
    UINT32 WAKE_ON_ZQCAL                           :  1;  // Bits 0:0
    UINT32 WAKE_ON_TEMP_READ                       :  1;  // Bits 1:1
    UINT32 WAKE_ON_RD_RETRAINING                   :  1;  // Bits 2:2
    UINT32 WAKE_ON_WR_RETRAINING                   :  1;  // Bits 3:3
    UINT32 BLOCK_WR_UNTIL_RETRAIN_DONE             :  1;  // Bits 4:4
    UINT32 BLOCK_RD_UNTIL_RETRAIN_DONE             :  1;  // Bits 5:5
    UINT32 BLOCK_UNTIL_ZQ_DONE                     :  1;  // Bits 6:6
    UINT32                                         :  1;  // Bits 7:7
    UINT32 EN_SRX_ZQCAL                            :  1;  // Bits 8:8
    UINT32 EN_SRX_TEMP_READ                        :  1;  // Bits 9:9
    UINT32 EN_SRX_RD_RETRAINING                    :  1;  // Bits 10:10
    UINT32 EN_SRX_WR_RETRAINING                    :  1;  // Bits 11:11
    UINT32 EN_PWRDN_IN_SR                          :  1;  // Bits 12:12
    UINT32 force_sre_with_freq_change              :  1;  // Bits 13:13
    UINT32                                         :  18;  // Bits 31:14
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_PM_SREF_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 Dec_tCWL                                :  6;  // Bits 5:0
    UINT32 Add_tCWL                                :  6;  // Bits 11:6
    UINT32                                         :  20;  // Bits 31:12
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_WR_DELAY_STRUCT;
typedef union {
  struct {
    UINT32 ECC                                     :  8;  // Bits 7:0
    UINT32 RRD_DFT_Mode                            :  2;  // Bits 9:8
    UINT32 LFSR_Seed_Index                         :  5;  // Bits 14:10
    UINT32 Inversion_Mode                          :  1;  // Bits 15:15
    UINT32 AddressBased                            :  1;  // Bits 16:16
    UINT32 AddressBasedRRD_mcsel                   :  1;  // Bits 17:17
    UINT32 WDAR                                    :  1;  // Bits 18:18
    UINT32                                         :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_READ_RETURN_DFT_STRUCT;
typedef union {
  struct {
    UINT32 Bit_0                                   :  3;  // Bits 2:0
    UINT32 Bit_1                                   :  3;  // Bits 5:3
    UINT32 Bit_2                                   :  3;  // Bits 8:6
    UINT32 Bit_3                                   :  3;  // Bits 11:9
    UINT32 Bit_4                                   :  3;  // Bits 14:12
    UINT32 Bit_8                                   :  3;  // Bits 17:15
    UINT32 Bit_9                                   :  3;  // Bits 20:18
    UINT32 Bit_10                                  :  3;  // Bits 23:21
    UINT32 Bit_11                                  :  3;  // Bits 26:24
    UINT32 Bit_12                                  :  3;  // Bits 29:27
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SCH0_BIT_DESWIZZLE_STRUCT;

typedef MC0_CH0_CR_SCH0_BIT_DESWIZZLE_STRUCT MC0_CH0_CR_SCH1_BIT_DESWIZZLE_STRUCT;
typedef union {
  struct {
    UINT32                                         :  1;  // Bits 0:0
    UINT32 PBR_OOO_Dis                             :  1;  // Bits 1:1
    UINT32 ECS_REFab_Enable                        :  1;  // Bits 2:2
    UINT32 PBR_Disable_on_hot                      :  1;  // Bits 3:3
    UINT32 ECS_REFab_Period                        :  10;  // Bits 13:4
    UINT32                                         :  11;  // Bits 24:14
    UINT32 PBR_extended_disable_while_DQS_OSC      :  1;  // Bits 25:25
    UINT32 Disable_NoPanic_PBR_Cbit                :  1;  // Bits 26:26
    UINT32 PBR_Idle_Bank_Condition_WR_dis          :  1;  // Bits 27:27
    UINT32                                         :  4;  // Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SC_PBR_STRUCT;
typedef union {
  struct {
    UINT32 tVRCG_ENABLE                            :  11;  // Bits 10:0
    UINT32 tVRCG_DISABLE                           :  11;  // Bits 21:11
    UINT32 MRS_tFC_pointer                         :  4;  // Bits 25:22
    UINT32                                         :  5;  // Bits 30:26
    UINT32 DVFSQ                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_DVFSQ_CTL_STRUCT;
typedef union {
  struct {
    UINT32 tMRR                                    :  8;  // Bits 7:0
    UINT32 PREMRR                                  :  8;  // Bits 15:8
    UINT32 tMRD                                    :  7;  // Bits 22:16
    UINT32                                         :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TC_MR_STRUCT;

typedef MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_STRUCT MC0_CH0_CR_SCH1_DDR_MR_RESULT_HIGH_STRUCT;

typedef MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_STRUCT MC0_CH0_CR_SCH1_DDR_MR_RESULT_ECC_STRUCT;
typedef union {
  struct {
    UINT32 tREFI                                   :  18;  // Bits 17:0
    UINT32 tRFC                                    :  13;  // Bits 30:18
    UINT32                                         :  9;  // Bits 39:31
    UINT32 tRFCpb                                  :  11;  // Bits 50:40
    UINT32 tREFSBRD                                :  8;  // Bits 58:51
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_TC_RFTP_STRUCT;
typedef union {
  struct {
    UINT32 Run                                     :  1;  // Bits 0:0
    UINT32 Opcode                                  :  1;  // Bits 1:1
    UINT32 Sel_Type                                :  2;  // Bits 3:2
    UINT32 Sel_Index                               :  8;  // Bits 11:4
    UINT32                                         :  20;  // Bits 31:12
    UINT32 CR_Data                                 :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_MRS_CR_MAILBOX_STRUCT;
typedef union {
  struct {
    UINT32 Byte_0                                  :  2;  // Bits 1:0
    UINT32 Byte_1                                  :  2;  // Bits 3:2
    UINT32 Byte_2                                  :  2;  // Bits 5:4
    UINT32 Byte_3                                  :  2;  // Bits 7:6
    UINT32                                         :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_STRUCT;

typedef MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_STRUCT MC0_CH0_CR_SCH1_BYTE_DESWIZZLE_STRUCT;
typedef union {
  struct {
    UINT32 tXSR                                    :  13;  // Bits 12:0
    UINT32 tXSR_DS                                 :  15;  // Bits 27:13
    UINT32                                         :  4;  // Bits 31:28
    UINT32 lpmode4_srx_delay                       :  8;  // Bits 39:32
    UINT32 tSR                                     :  7;  // Bits 46:40
    UINT32                                         :  4;  // Bits 50:47
    UINT32 tXSDLL                                  :  13;  // Bits 63:51
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_TC_SREXITTP_STRUCT;
typedef union {
  struct {
    UINT32 RETRAINING_PERIOD                       :  16;  // Bits 15:0
    UINT32                                         :  2;  // Bits 17:16
    UINT32 EN_RD_RETRAINING                        :  1;  // Bits 18:18
    UINT32 EN_WR_RETRAINING                        :  1;  // Bits 19:19
    UINT32                                         :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_RETRAINING_OSCL_PARAMS_STRUCT;
typedef union {
  struct {
    UINT32 ca_bus                                  :  28;  // Bits 27:0
    UINT32 Generic_MRH_override                    :  1;  // Bits 28:28
    UINT32 two_cyc_command                         :  1;  // Bits 29:29
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MRH_GENERIC_COMMAND_STRUCT;
typedef union {
  struct {
    UINT32 tOSCO                                   :  8;  // Bits 7:0
    UINT32 WR_OSCL_RUNTIME                         :  12;  // Bits 19:8
    UINT32 RD_OSCL_RUNTIME                         :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_TC_RETRAINING_OSCL_STRUCT;
typedef union {
  struct {
    UINT32 FORCE_RD_RETRAINING                     :  4;  // Bits 3:0
    UINT32 FORCE_WR_RETRAINING                     :  4;  // Bits 7:4
    UINT32 FORCE_TEMP_READ                         :  4;  // Bits 11:8
    UINT32 FORCE_ZQ_CALIBRATION                    :  4;  // Bits 15:12
    UINT32 FORCE_REFRESH                           :  4;  // Bits 19:16
    UINT32                                         :  7;  // Bits 26:20
    UINT32 RD_RETRAINING_ABORTED                   :  1;  // Bits 27:27
    UINT32 WR_RETRAINING_ABORTED                   :  1;  // Bits 28:28
    UINT32 TEMP_READ_ABORTED                       :  1;  // Bits 29:29
    UINT32 ZQ_CALIBRATION_ABORTED                  :  1;  // Bits 30:30
    UINT32 REFRESH_ABORTED                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_STRUCT;
typedef union {
  struct {
    UINT32 DATALow                                 :  32;  // Bits 31:0
    UINT32 DATAHigh                                :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_WDB_RD_WR_DFX_DATA_STRUCT;
typedef union {
  struct {
    UINT32 GO                                      :  1;  // Bits 0:0
    UINT32 Chunk_Sel                               :  8;  // Bits 8:1
    UINT32 Op                                      :  1;  // Bits 9:9
    UINT32 SubCh                                   :  1;  // Bits 10:10
    UINT32 WID                                     :  7;  // Bits 17:11
    UINT32                                         :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_WDB_RD_WR_DFX_CTL_STRUCT;
typedef union {
  struct {
    UINT32 PASS                                    :  1;  // Bits 0:0
    UINT32 Complete                                :  1;  // Bits 1:1
    UINT32                                         :  6;  // Bits 7:2
    UINT32 Inject_Failure                          :  1;  // Bits 8:8
    UINT32                                         :  22;  // Bits 30:9
    UINT32 RUN_BUSY                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_WDB_MBIST_0_STRUCT;

typedef MC0_CH0_CR_WDB_MBIST_0_STRUCT MC0_CH0_CR_WDB_MBIST_1_STRUCT;

typedef MC0_CH0_CR_WDB_MBIST_0_STRUCT MC0_CH0_CR_RDB_MBIST_STRUCT;
typedef union {
  struct {
    UINT32 Count                                   :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_ECC_INJECT_COUNT_STRUCT;
typedef union {
  struct {
    UINT32 RDDATA_COUNTERLow                       :  32;  // Bits 31:0
    UINT32 RDDATA_COUNTERHigh                      :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_STRUCT;

typedef MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_STRUCT MC0_CH0_CR_PWM_DDR_SUBCH1_RDDATA_COUNTER_STRUCT;
typedef union {
  struct {
    UINT32 WRDATA_COUNTERLow                       :  32;  // Bits 31:0
    UINT32 WRDATA_COUNTERHigh                      :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_STRUCT;

typedef MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_STRUCT MC0_CH0_CR_PWM_DDR_SUBCH1_WRDATA_COUNTER_STRUCT;
typedef union {
  struct {
    UINT32 WDB_SC0_Current_Capture_Pointer         :  7;  // Bits 6:0
    UINT32                                         :  1;  // Bits 7:7
    UINT32 WDB_SC1_Current_Capture_Pointer         :  7;  // Bits 14:8
    UINT32                                         :  17;  // Bits 31:15
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_WDB_CAPTURE_STATUS_STRUCT;
typedef union {
  struct {
    UINT32 Enable_WDB_Capture                      :  1;  // Bits 0:0
    UINT32                                         :  7;  // Bits 7:1
    UINT32 WDB_Starting_Capture_Pointer            :  7;  // Bits 14:8
    UINT32                                         :  1;  // Bits 15:15
    UINT32 WDB_Ending_Capture_Pointer              :  7;  // Bits 22:16
    UINT32 WDB_Capture_Block_SubCh                 :  2;  // Bits 24:23
    UINT32                                         :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_WDB_CAPTURE_CTL_STRUCT;
typedef union {
  struct {
    UINT32 mrh_quiet_time_before_command           :  10;  // Bits 9:0
    UINT32 mrh_quiet_time_after_command            :  10;  // Bits 19:10
    UINT32                                         :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MRH_CONFIG_STRUCT;
typedef union {
  struct {
    UINT32 Enable                                  :  1;  // Bits 0:0
    UINT32 IncreaseRate                            :  2;  // Bits 2:1
    UINT32 MaxCount                                :  1;  // Bits 3:3
    UINT32 TemperatureAware                        :  1;  // Bits 4:4
    UINT32                                         :  27;  // Bits 31:5
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_STRUCT;
typedef union {
  struct {
    UINT32 LFSR                                    :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_RH_TRR_LFSR0_STRUCT;

typedef MC0_CH0_CR_RH_TRR_LFSR0_STRUCT MC0_CH0_CR_RH_TRR_LFSR1_STRUCT;
typedef union {
  struct {
    UINT32 send_preall                             :  1;  // Bits 0:0
    UINT32 do_zqcl                                 :  1;  // Bits 1:1
    UINT32 lpddr_switch_fsp                        :  1;  // Bits 2:2
    UINT32 do_dq_osc_start                         :  1;  // Bits 3:3
    UINT32 do_rd_retrain_start                     :  1;  // Bits 4:4
    UINT32 split_transition                        :  1;  // Bits 5:5
    UINT32 generic_mrs_fsm_device_loop_enable      :  1;  // Bits 6:6
    UINT32 do_pda_for_ecc                          :  1;  // Bits 7:7
    UINT32 generic_mrs_fsm_breakpoint_address      :  7;  // Bits 14:8
    UINT32                                         :  17;  // Bits 31:15
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MRS_FSM_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 cbit_mrs_ignore_rh_block                :  1;  // Bits 0:0
    UINT32 run_delay                               :  5;  // Bits 5:1
    UINT32 dis_mrs_fsm_auto_start                  :  1;  // Bits 6:6
    UINT32 cbit_count_each_rank                    :  1;  // Bits 7:7
    UINT32 mrs_vrcg_ovrd_en                        :  1;  // Bits 8:8
    UINT32 mrs_vrcg_ovrd_val                       :  1;  // Bits 9:9
    UINT32 dvfsc_outside_sagv_dis                  :  1;  // Bits 10:10
    UINT32                                         :  21;  // Bits 31:11
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_STRUCT;
typedef union {
  struct {
    UINT32 Run                                     :  1;  // Bits 0:0
    UINT32                                         :  31;  // Bits 31:1
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MRS_FSM_RUN_STRUCT;
typedef union {
  struct {
    UINT32 data_trace_mode                         :  2;  // Bits 1:0
    UINT32 UI_to_trace                             :  3;  // Bits 4:2
    UINT32 byte_to_trace                           :  3;  // Bits 7:5
    UINT32 ECC_EN                                  :  1;  // Bits 8:8
    UINT32 ECC_BYTE_replace                        :  3;  // Bits 11:9
    UINT32 trace_read_data                         :  1;  // Bits 12:12
    UINT32 trace_write_data                        :  1;  // Bits 13:13
    UINT32 DDRPL_Activate                          :  1;  // Bits 14:14
    UINT32 SCHTrace                                :  1;  // Bits 15:15
    UINT32                                         :  1;  // Bits 16:16
    UINT32 ddrpl_data_trace_width                  :  2;  // Bits 18:17
    UINT32 data_quarter_mode_byte_select           :  2;  // Bits 20:19
    UINT32 data_half_mode_byte_select              :  1;  // Bits 21:21
    UINT32                                         :  10;  // Bits 31:22
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_PL_AGENT_CFG_DTF_STRUCT;
typedef union {
  struct {
    UINT32 Spare_RW                                :  16;  // Bits 15:0
    UINT32 Spare_RW_V                              :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MCMNTS_SPARE2_STRUCT;
typedef union {
  struct {
    UINT32 Spare_RW                                :  8;  // Bits 7:0
    UINT32 DisBlockActDuringPanicRef               :  1;  // Bits 8:8
    UINT32 dis_trr_on_any_bank_rhblock             :  1;  // Bits 9:9
    UINT32 cbit_dont_sub_mrs_delay_from_mr_timing  :  1;  // Bits 10:10
    UINT32 cbit_block_mrs_4cycles_after_mnt        :  1;  // Bits 11:11
    UINT32 cbit_allow_timed_ABR                    :  1;  // Bits 12:12
    UINT32 DisSREXcnt                              :  1;  // Bits 13:13
    UINT32 pattern_match_enable                    :  1;  // Bits 14:14
    UINT32 Dis_Block_DQS_MR18_on_MRS               :  1;  // Bits 15:15
    UINT32 rf_fast_sleep_disable                   :  1;  // Bits 16:16
    UINT32 cbit_ranks_abr_to_clkgate               :  1;  // Bits 17:17
    UINT32 cbit_dis_ovrd_sagv_sre_srx_with_nop     :  1;  // Bits 18:18
    UINT32 cbit_signal_all_ch_sr_when_pde          :  1;  // Bits 19:19
    UINT32 cbit_allow_trr_on_pkgc_drain            :  1;  // Bits 20:20
    UINT32                                         :  3;  // Bits 23:21
    UINT32 Spare_RW_V                              :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_MCMNTS_SPARE_STRUCT;
typedef union {
  struct {
    UINT32 drainless_sagv_do_zq                    :  1;  // Bits 0:0
    UINT32 dis_separate_zq_block                   :  1;  // Bits 1:1
    UINT32 disregardzqonsrtime                     :  1;  // Bits 2:2
    UINT32 background_zq_mode                      :  1;  // Bits 3:3
    UINT32 discrossrankzqblock                     :  1;  // Bits 4:4
    UINT32                                         :  12;  // Bits 16:5
    UINT32 dis_zq                                  :  1;  // Bits 17:17
    UINT32                                         :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MC0_CH0_CR_ZQCAL_CONTROL_UARCH_STRUCT;

typedef MC0_CH0_CR_TC_PRE_STRUCT MC0_CH1_CR_TC_PRE_STRUCT;

typedef MC0_CH0_CR_TC_RDRD_STRUCT MC0_CH1_CR_TC_RDRD_STRUCT;

typedef MC0_CH0_CR_TC_RDWR_STRUCT MC0_CH1_CR_TC_RDWR_STRUCT;

typedef MC0_CH0_CR_TC_WRRD_STRUCT MC0_CH1_CR_TC_WRRD_STRUCT;

typedef MC0_CH0_CR_TC_WRWR_STRUCT MC0_CH1_CR_TC_WRWR_STRUCT;

typedef MC0_CH0_CR_PM_ADAPTIVE_CKE_STRUCT MC0_CH1_CR_PM_ADAPTIVE_CKE_STRUCT;

typedef MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_STRUCT MC0_CH1_CR_SC_ROUNDTRIP_LATENCY_STRUCT;

typedef MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_STRUCT MC0_CH1_CR_PM_PDWN_CONFIG_UARCH_STRUCT;

typedef MC0_CH0_CR_SCHED_CBIT_STRUCT MC0_CH1_CR_SCHED_CBIT_STRUCT;

typedef MC0_CH0_CR_SCHED_SECOND_CBIT_STRUCT MC0_CH1_CR_SCHED_SECOND_CBIT_STRUCT;

typedef MC0_CH0_CR_DFT_MISC_STRUCT MC0_CH1_CR_DFT_MISC_STRUCT;

typedef MC0_CH0_CR_SC_PCIT_STRUCT MC0_CH1_CR_SC_PCIT_STRUCT;

typedef MC0_CH0_CR_ECC_DFT_STRUCT MC0_CH1_CR_ECC_DFT_STRUCT;

typedef MC0_CH0_CR_ECC_DFT_UARCH_STRUCT MC0_CH1_CR_ECC_DFT_UARCH_STRUCT;

typedef MC0_CH0_CR_PM_PDWN_CONFIG_STRUCT MC0_CH1_CR_PM_PDWN_CONFIG_STRUCT;

typedef MC0_CH0_CR_ECCERRLOG0_STRUCT MC0_CH1_CR_ECCERRLOG0_STRUCT;

typedef MC0_CH0_CR_ECCERRLOG1_STRUCT MC0_CH1_CR_ECCERRLOG1_STRUCT;

typedef MC0_CH0_CR_TC_PWRDN_STRUCT MC0_CH1_CR_TC_PWRDN_STRUCT;

typedef MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_STRUCT MC0_CH1_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_STRUCT;

typedef MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_STRUCT MC0_CH1_CR_QUEUE_ENTRY_DISABLE_IPQ_STRUCT;

typedef MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_STRUCT MC0_CH1_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_STRUCT;

typedef MC0_CH0_CR_SC_WDBWM_STRUCT MC0_CH1_CR_SC_WDBWM_STRUCT;

typedef MC0_CH0_CR_TC_CAS_STRUCT MC0_CH1_CR_TC_CAS_STRUCT;

typedef MC0_CH0_CR_MCSCHEDS_SPARE_STRUCT MC0_CH1_CR_MCSCHEDS_SPARE_STRUCT;

typedef MC0_CH0_CR_TC_MPC_STRUCT MC0_CH1_CR_TC_MPC_STRUCT;

typedef MC0_CH0_CR_SC_ODT_MATRIX_STRUCT MC0_CH1_CR_SC_ODT_MATRIX_STRUCT;

typedef MC0_CH0_CR_DFT_BLOCK_STRUCT MC0_CH1_CR_DFT_BLOCK_STRUCT;

typedef MC0_CH0_CR_SC_GS_CFG_STRUCT MC0_CH1_CR_SC_GS_CFG_STRUCT;

typedef MC0_CH0_CR_SC_GS_CFG_UARCH_STRUCT MC0_CH1_CR_SC_GS_CFG_UARCH_STRUCT;

typedef MC0_CH0_CR_SC_PH_THROTTLING_0_STRUCT MC0_CH1_CR_SC_PH_THROTTLING_0_STRUCT;

typedef MC0_CH0_CR_SC_PH_THROTTLING_1_STRUCT MC0_CH1_CR_SC_PH_THROTTLING_1_STRUCT;

typedef MC0_CH0_CR_SC_PH_THROTTLING_2_STRUCT MC0_CH1_CR_SC_PH_THROTTLING_2_STRUCT;

typedef MC0_CH0_CR_SC_PH_THROTTLING_3_STRUCT MC0_CH1_CR_SC_PH_THROTTLING_3_STRUCT;

typedef MC0_CH0_CR_SC_WPQ_THRESHOLD_STRUCT MC0_CH1_CR_SC_WPQ_THRESHOLD_STRUCT;

typedef MC0_CH0_CR_SC_PR_CNT_CONFIG_STRUCT MC0_CH1_CR_SC_PR_CNT_CONFIG_STRUCT;

typedef MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_STRUCT MC0_CH1_CR_REUT_CH_MISC_CKE_CS_CTRL_STRUCT;

typedef MC0_CH0_CR_SC_GS_CFG_TRAINING_STRUCT MC0_CH1_CR_SC_GS_CFG_TRAINING_STRUCT;

typedef MC0_CH0_CR_SPID_LOW_POWER_CTL_STRUCT MC0_CH1_CR_SPID_LOW_POWER_CTL_STRUCT;

typedef MC0_CH0_CR_SCHED_THIRD_CBIT_STRUCT MC0_CH1_CR_SCHED_THIRD_CBIT_STRUCT;

typedef MC0_CH0_CR_DEADLOCK_BREAKER_STRUCT MC0_CH1_CR_DEADLOCK_BREAKER_STRUCT;

typedef MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_STRUCT MC0_CH1_CR_MCSCHEDS_GLOBAL_DRIVER_GATE_CFG_STRUCT;

typedef MC0_CH0_CR_SC_BLOCKING_RULES_CFG_STRUCT MC0_CH1_CR_SC_BLOCKING_RULES_CFG_STRUCT;

typedef MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_STRUCT MC0_CH1_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_STRUCT;

typedef MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_STRUCT MC0_CH1_CR_PWM_DDR_SUBCH1_REQ_OCCUPANCY_COUNTER_STRUCT;

typedef MC0_CH0_CR_TR_RRDVALID_CTRL_STRUCT MC0_CH1_CR_TR_RRDVALID_CTRL_STRUCT;

typedef MC0_CH0_CR_TR_RRDVALID_DATA_STRUCT MC0_CH1_CR_TR_RRDVALID_DATA_STRUCT;

typedef MC0_CH0_CR_TC_WCK_STRUCT MC0_CH1_CR_TC_WCK_STRUCT;

typedef MC0_CH0_CR_WMM_READ_CONFIG_STRUCT MC0_CH1_CR_WMM_READ_CONFIG_STRUCT;

typedef MC0_CH0_CR_MC2PHY_BGF_CTRL_STRUCT MC0_CH1_CR_MC2PHY_BGF_CTRL_STRUCT;

typedef MC0_CH0_CR_SC_ADAPTIVE_PCIT_STRUCT MC0_CH1_CR_SC_ADAPTIVE_PCIT_STRUCT;

typedef MC0_CH0_CR_ROWHAMMER_CTL_STRUCT MC0_CH1_CR_ROWHAMMER_CTL_STRUCT;

typedef MC0_CH0_CR_MERGE_REQ_READS_PQ_STRUCT MC0_CH1_CR_MERGE_REQ_READS_PQ_STRUCT;

typedef MC0_CH0_CR_DFI_CTL_STRUCT MC0_CH1_CR_DFI_CTL_STRUCT;

typedef MC0_CH0_CR_AUTO_PRE_CONTROL_STRUCT MC0_CH1_CR_AUTO_PRE_CONTROL_STRUCT;

typedef MC0_CH0_CR_TC_ACT_STRUCT MC0_CH1_CR_TC_ACT_STRUCT;

typedef MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_STRUCT MC0_CH1_CR_QUEUE_ENTRY_DISABLE_RPQ_STRUCT;

typedef MC0_CH0_CR_WCK_CONFIG_STRUCT MC0_CH1_CR_WCK_CONFIG_STRUCT;

typedef MC0_CH0_CR_SC_WDBWM_UARCH_STRUCT MC0_CH1_CR_SC_WDBWM_UARCH_STRUCT;

typedef MC0_CH0_CR_ROWHAMMER_CTL_UARCH_STRUCT MC0_CH1_CR_ROWHAMMER_CTL_UARCH_STRUCT;

typedef MC0_CH0_CR_RH_TRR_CONTROL_STRUCT MC0_CH1_CR_RH_TRR_CONTROL_STRUCT;

typedef MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_STRUCT MC0_CH1_CR_REUT_CH_MISC_REFRESH_CTRL_STRUCT;

typedef MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_STRUCT MC0_CH1_CR_REUT_CH_MISC_ZQ_CTRL_STRUCT;

typedef MC0_CH0_CR_TC_REFM_STRUCT MC0_CH1_CR_TC_REFM_STRUCT;

typedef MC0_CH0_CR_RH_TRR_CONTROL_UARCH_STRUCT MC0_CH1_CR_RH_TRR_CONTROL_UARCH_STRUCT;

typedef MC0_CH0_CR_DDR_MR_COMMAND_STRUCT MC0_CH1_CR_DDR_MR_COMMAND_STRUCT;

typedef MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_STRUCT MC0_CH1_CR_SCH0_DDR_MR_RESULT_LOW_STRUCT;

typedef MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_STRUCT MC0_CH1_CR_SCH0_DDR_MR_RESULT_HIGH_STRUCT;

typedef MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_STRUCT MC0_CH1_CR_SCH0_DDR_MR_RESULT_ECC_STRUCT;

typedef MC0_CH0_CR_RANK_TEMPERATURE_STRUCT MC0_CH1_CR_RANK_TEMPERATURE_STRUCT;

typedef MC0_CH0_CR_PM_CONFIG_THERM_STATUS_STRUCT MC0_CH1_CR_PM_CONFIG_THERM_STATUS_STRUCT;

typedef MC0_CH0_CR_MC_RDB_CREDITS_STRUCT MC0_CH1_CR_MC_RDB_CREDITS_STRUCT;

typedef MC0_CH0_CR_RH_TRR_ADDRESS_STRUCT MC0_CH1_CR_RH_TRR_ADDRESS_STRUCT;

typedef MC0_CH0_CR_MC_INIT_STATE_UARCH_STRUCT MC0_CH1_CR_MC_INIT_STATE_UARCH_STRUCT;

typedef MC0_CH0_CR_RFP_STRUCT MC0_CH1_CR_RFP_STRUCT;

typedef MC0_CH0_CR_RFP_UARCH_STRUCT MC0_CH1_CR_RFP_UARCH_STRUCT;

typedef MC0_CH0_CR_MC_REFRESH_STAGGER_STRUCT MC0_CH1_CR_MC_REFRESH_STAGGER_STRUCT;

typedef MC0_CH0_CR_TC_ZQCAL_STRUCT MC0_CH1_CR_TC_ZQCAL_STRUCT;

typedef MC0_CH0_CR_ZQCAL_CONTROL_STRUCT MC0_CH1_CR_ZQCAL_CONTROL_STRUCT;

typedef MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_STRUCT MC0_CH1_CR_SCH1_DDR_MR_RESULT_LOW_STRUCT;

typedef MC0_CH0_CR_MC_INIT_STATE_STRUCT MC0_CH1_CR_MC_INIT_STATE_STRUCT;

typedef MC0_CH0_CR_WDB_VISA_SEL_STRUCT MC0_CH1_CR_WDB_VISA_SEL_STRUCT;

typedef MC0_CH0_CR_REF_FSM_STATUS_STRUCT MC0_CH1_CR_REF_FSM_STATUS_STRUCT;

typedef MC0_CH0_CR_PM_DIMM_ACT_ENERGY_STRUCT MC0_CH1_CR_PM_DIMM_ACT_ENERGY_STRUCT;

typedef MC0_CH0_CR_PM_DIMM_RD_ENERGY_STRUCT MC0_CH1_CR_PM_DIMM_RD_ENERGY_STRUCT;

typedef MC0_CH0_CR_PM_DIMM_WR_ENERGY_STRUCT MC0_CH1_CR_PM_DIMM_WR_ENERGY_STRUCT;

typedef MC0_CH0_CR_PM_SREF_CONTROL_STRUCT MC0_CH1_CR_PM_SREF_CONTROL_STRUCT;

typedef MC0_CH0_CR_SC_WR_DELAY_STRUCT MC0_CH1_CR_SC_WR_DELAY_STRUCT;

typedef MC0_CH0_CR_READ_RETURN_DFT_STRUCT MC0_CH1_CR_READ_RETURN_DFT_STRUCT;

typedef MC0_CH0_CR_SCH0_BIT_DESWIZZLE_STRUCT MC0_CH1_CR_SCH0_BIT_DESWIZZLE_STRUCT;

typedef MC0_CH0_CR_SCH0_BIT_DESWIZZLE_STRUCT MC0_CH1_CR_SCH1_BIT_DESWIZZLE_STRUCT;

typedef MC0_CH0_CR_SC_PBR_STRUCT MC0_CH1_CR_SC_PBR_STRUCT;

typedef MC0_CH0_CR_DVFSQ_CTL_STRUCT MC0_CH1_CR_DVFSQ_CTL_STRUCT;

typedef MC0_CH0_CR_TC_MR_STRUCT MC0_CH1_CR_TC_MR_STRUCT;

typedef MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_STRUCT MC0_CH1_CR_SCH1_DDR_MR_RESULT_HIGH_STRUCT;

typedef MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_STRUCT MC0_CH1_CR_SCH1_DDR_MR_RESULT_ECC_STRUCT;

typedef MC0_CH0_CR_TC_RFTP_STRUCT MC0_CH1_CR_TC_RFTP_STRUCT;

typedef MC0_CH0_CR_MRS_CR_MAILBOX_STRUCT MC0_CH1_CR_MRS_CR_MAILBOX_STRUCT;

typedef MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_STRUCT MC0_CH1_CR_SCH0_BYTE_DESWIZZLE_STRUCT;

typedef MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_STRUCT MC0_CH1_CR_SCH1_BYTE_DESWIZZLE_STRUCT;

typedef MC0_CH0_CR_TC_SREXITTP_STRUCT MC0_CH1_CR_TC_SREXITTP_STRUCT;

typedef MC0_CH0_CR_RETRAINING_OSCL_PARAMS_STRUCT MC0_CH1_CR_RETRAINING_OSCL_PARAMS_STRUCT;

typedef MC0_CH0_CR_MRH_GENERIC_COMMAND_STRUCT MC0_CH1_CR_MRH_GENERIC_COMMAND_STRUCT;

typedef MC0_CH0_CR_TC_RETRAINING_OSCL_STRUCT MC0_CH1_CR_TC_RETRAINING_OSCL_STRUCT;

typedef MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_STRUCT MC0_CH1_CR_FORCE_DRAM_MNT_FLOW_STRUCT;

typedef MC0_CH0_CR_WDB_RD_WR_DFX_DATA_STRUCT MC0_CH1_CR_WDB_RD_WR_DFX_DATA_STRUCT;

typedef MC0_CH0_CR_WDB_RD_WR_DFX_CTL_STRUCT MC0_CH1_CR_WDB_RD_WR_DFX_CTL_STRUCT;

typedef MC0_CH0_CR_WDB_MBIST_0_STRUCT MC0_CH1_CR_WDB_MBIST_0_STRUCT;

typedef MC0_CH0_CR_WDB_MBIST_0_STRUCT MC0_CH1_CR_WDB_MBIST_1_STRUCT;

typedef MC0_CH0_CR_WDB_MBIST_0_STRUCT MC0_CH1_CR_RDB_MBIST_STRUCT;

typedef MC0_CH0_CR_ECC_INJECT_COUNT_STRUCT MC0_CH1_CR_ECC_INJECT_COUNT_STRUCT;

typedef MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_STRUCT MC0_CH1_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_STRUCT;

typedef MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_STRUCT MC0_CH1_CR_PWM_DDR_SUBCH1_RDDATA_COUNTER_STRUCT;

typedef MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_STRUCT MC0_CH1_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_STRUCT;

typedef MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_STRUCT MC0_CH1_CR_PWM_DDR_SUBCH1_WRDATA_COUNTER_STRUCT;

typedef MC0_CH0_CR_WDB_CAPTURE_STATUS_STRUCT MC0_CH1_CR_WDB_CAPTURE_STATUS_STRUCT;

typedef MC0_CH0_CR_WDB_CAPTURE_CTL_STRUCT MC0_CH1_CR_WDB_CAPTURE_CTL_STRUCT;

typedef MC0_CH0_CR_MRH_CONFIG_STRUCT MC0_CH1_CR_MRH_CONFIG_STRUCT;

typedef MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_STRUCT MC0_CH1_CR_PAGEOPEN_POLICY_LIMIT_STRUCT;

typedef MC0_CH0_CR_RH_TRR_LFSR0_STRUCT MC0_CH1_CR_RH_TRR_LFSR0_STRUCT;

typedef MC0_CH0_CR_RH_TRR_LFSR0_STRUCT MC0_CH1_CR_RH_TRR_LFSR1_STRUCT;

typedef MC0_CH0_CR_MRS_FSM_CONTROL_STRUCT MC0_CH1_CR_MRS_FSM_CONTROL_STRUCT;

typedef MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_STRUCT MC0_CH1_CR_MRS_FSM_CONTROL_UARCH_STRUCT;

typedef MC0_CH0_CR_MRS_FSM_RUN_STRUCT MC0_CH1_CR_MRS_FSM_RUN_STRUCT;

typedef MC0_CH0_CR_PL_AGENT_CFG_DTF_STRUCT MC0_CH1_CR_PL_AGENT_CFG_DTF_STRUCT;

typedef MC0_CH0_CR_MCMNTS_SPARE2_STRUCT MC0_CH1_CR_MCMNTS_SPARE2_STRUCT;

typedef MC0_CH0_CR_MCMNTS_SPARE_STRUCT MC0_CH1_CR_MCMNTS_SPARE_STRUCT;

typedef MC0_CH0_CR_ZQCAL_CONTROL_UARCH_STRUCT MC0_CH1_CR_ZQCAL_CONTROL_UARCH_STRUCT;

#pragma pack(pop)
#endif
