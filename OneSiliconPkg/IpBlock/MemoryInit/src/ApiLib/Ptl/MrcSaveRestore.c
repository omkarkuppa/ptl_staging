/** @file

  Power state and boot mode save and restore data functions.

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

//
// Include files
//

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcCommon.h"
#include "MrcGeneral.h"
#include "MrcSagv.h"
#include "MrcSaveRestore.h"
#include "BlueMrcSpdProcessingApi.h"  // for MrcSpdCrcArea()
#include "MrcChipApi.h"
#include "MrcDdrIoDefines.h"
#include "MrcDdrIoApi.h"
#include "MrcDdrIoUtils.h"
#include "MrcRefreshConfiguration.h"
#include "MrcReset.h"

#define MRC_NUM_BYTE_GROUPS_UY  (8)
#define MRC_NUM_BYTE_GROUPS_H   (10)

#define SAVE_DATA_CONTROL_SIZE(x) (sizeof (x) / sizeof (SaveDataControl))

//
// ------- IMPORTANT NOTE --------
// MRC_REGISTER_COUNT_COMMON and MRC_REGISTER_COUNT_SAGV in MrcGenSaveRestoreRegCount.h should match these tables.
// Update these defines whenever you add/remove registers from the tables below.

const SaveDataControl SaveDataMcCommon32PerMc[] = {
  {0xd800, 0xd810}, // MC0_MAD_DRAM_HASH_0, MC0_MAD_DRAM_HASH_1, MC0_MAD_INTER_CHANNEL, MC0_MAD_INTRA_CHANNEL_0, MC0_MAD_INTRA_CHANNEL_1
  {0xd818, 0xd81c}, // MC0_MCDECS_CBIT, MC0_MCDECS_MISC
  {0xd824, 0xd82c}, // MC0_MAD_CHANNEL_HASH, MC0_MAD_SUB_CHANNEL_HASH, MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG
  {0xd838, 0xd83c}, // MC0_MC_RTL_VERSION, MC0_PM_OPP_SREF_IDLE_TIMER
  {0xd860, 0xd860}, // MC0_PM_OPP_SREF_ENABLE
  {0xd878, 0xd880}, // MC0_IPC_MC_ARB, MC0_IPC_MC_DEC_ARB, MC0_QUEUE_CREDIT_C
  {0xd8bc, 0xd8bc}, // MC0_TOLUD
  {0xd938, 0xd940}, // MC0_CLOCK_SPINE_GATE_CFG, MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG, MC0_PM_CONTROL
  {0xd954, 0xd954}, // MC0_MCDECS_SECOND_CBIT
  {0xd968, 0xd968}, // MC0_SC_QOS3
  {0xd970, 0xd974}, // MC0_MC_CPGC_CONTROL, MC0_MC_CPGC_CONTROL_UARCH
  {0xd9b4, 0xd9b8}, // MC0_MAD_MC_HASH, MC0_PARITY_CONTROL
  {0xda30, 0xda38}, // MC0_OS_TELEMETRY_CONTROL, MC0_PM_OPP_SR_POLICY_0, MC0_PM_OPP_SR_POLICY_1
  {0xda40, 0xda40}, // MC0_MC_CFI_FABRIC_CONTROL
  {0xda50, 0xda50}, // MC0_SC_QOS_UARCH
};

const SaveDataControl SaveDataMcCommon32PerCh[] = {
  {0xe024, 0xe034}, // MC0_CH0_CR_DFT_MISC, MC0_CH0_CR_PM_PDWN_CONFIG_UARCH,
                    // MC0_CH0_CR_SCHED_CBIT, MC0_CH0_CR_SCHED_SECOND_CBIT, MC0_CH0_CR_SC_PCIT
  {0xe05c, 0xe05c}, // MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ
  {0xe090, 0xe0a0}, // MC0_CH0_CR_SC_PH_THROTTLING_0, MC0_CH0_CR_SC_PH_THROTTLING_1, MC0_CH0_CR_SC_PH_THROTTLING_2, MC0_CH0_CR_SC_PH_THROTTLING_3,
                    // MC0_CH0_CR_SC_WPQ_THRESHOLD
  {0xe0b4, 0xe0b4}, // MC0_CH0_CR_SC_GS_CFG_TRAINING
  {0xe0c0, 0xe0c0}, // MC0_CH0_CR_SCHED_THIRD_CBIT
  {0xe0d0, 0xe0d0}, // MC0_CH0_CR_MCSCHEDS_GLOBAL_DRIVER_GATE_CFG
  {0xe100, 0xe100}, // MC0_CH0_CR_TR_RRDVALID_CTRL
  {0xe110, 0xe110}, // MC0_CH0_CR_WMM_READ_CONFIG
  {0xe128, 0xe12c}, // MC0_CH0_CR_DFI_CTL, MC0_CH0_CR_MERGE_REQ_READS_PQ
  {0xe158, 0xe15c}, // MC0_CH0_CR_ROWHAMMER_CTL_UARCH, MC0_CH0_CR_SC_WDBWM_UARCH
  {0xe400, 0xe400}, // MC0_CH0_CR_RH_TRR_CONTROL
  {0xe410, 0xe410}, // MC0_CH0_CR_RH_TRR_CONTROL_UARCH
  {0xe424, 0xe42c}, // MC0_CH0_CR_MC_RDB_CREDITS, MC0_CH0_CR_PM_CONFIG_THERM_STATUS, MC0_CH0_CR_RANK_TEMPERATURE
  {0xe438, 0xe438}, // MC0_CH0_CR_RFP
  {0xe440, 0xe444}, // MC0_CH0_CR_MC_REFRESH_STAGGER, MC0_CH0_CR_RFP_UARCH
  {0xe44c, 0xe44c}, // MC0_CH0_CR_ZQCAL_CONTROL
  {0xe454, 0xe454}, // MC0_CH0_CR_MC_INIT_STATE
  {0xe474, 0xe474}, // MC0_CH0_CR_PM_SREF_CONTROL
  {0xe480, 0xe488}, // MC0_CH0_CR_SCH0_BIT_DESWIZZLE, MC0_CH0_CR_SCH1_BIT_DESWIZZLE, MC0_CH0_CR_SC_PBR
  {0xe4b8, 0xe4bc}, // MC0_CH0_CR_SCH0_BYTE_DESWIZZLE, MC0_CH0_CR_SCH1_BYTE_DESWIZZLE
  {0xe528, 0xe52c}, // MC0_CH0_CR_MRH_CONFIG, MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT
  {0xe5d0, 0xe5d4}, // MC0_CH0_CR_MRS_FSM_CONTROL, MC0_CH0_CR_MRS_FSM_CONTROL_UARCH
  {0xe5e0, 0xe5e0}, // MC0_CH0_CR_PL_AGENT_CFG_DTF
  {0xe600, 0xe600}, // MC0_CH0_CR_ZQCAL_CONTROL_UARCH
};

const SaveDataControl SaveDataMcPerSaGv32PerMc[] = {
  {0xda44, 0xda44}, // MC0_MC_CFI_CONTROL
  {0xda4c, 0xda4c}, // MC0_DDRPL_CFG2_DTF
};

const SaveDataControl SaveDataMcPerSaGv32PerCh[] = {
  {0xe00c, 0xe020}, // MC0_CH0_CR_PM_ADAPTIVE_CKE, MC0_CH0_CR_SC_ROUNDTRIP_LATENCY, MC0_CH0_CR_TC_RDRD, MC0_CH0_CR_TC_RDWR, MC0_CH0_CR_TC_WRRD, MC0_CH0_CR_TC_WRWR
  {0xe070, 0xe070}, // MC0_CH0_CR_TC_CAS
  {0xe078, 0xe080}, // MC0_CH0_CR_MCSCHEDS_SPARE, MC0_CH0_CR_SC_ODT_MATRIX, MC0_CH0_CR_TC_MPC
  {0xe088, 0xe08c}, // MC0_CH0_CR_SC_GS_CFG, MC0_CH0_CR_SC_GS_CFG_UARCH
  {0xe0c4, 0xe0c4}, // MC0_CH0_CR_DEADLOCK_BREAKER
  {0xe104, 0xe104}, // MC0_CH0_CR_TR_RRDVALID_DATA
  {0xe114, 0xe114}, // MC0_CH0_CR_MC2PHY_BGF_CTRL
  {0xe40c, 0xe40c}, // MC0_CH0_CR_TC_REFM
  {0xe448, 0xe448}, // MC0_CH0_CR_TC_ZQCAL
  {0xe478, 0xe478}, // MC0_CH0_CR_SC_WR_DELAY
  {0xe48c, 0xe490}, // MC0_CH0_CR_DVFSQ_CTL, MC0_CH0_CR_TC_MR
  {0xe4c8, 0xe4c8}, // MC0_CH0_CR_RETRAINING_OSCL_PARAMS
  {0xe4d0, 0xe4d0}, // MC0_CH0_CR_TC_RETRAINING_OSCL
  {0xe5f8, 0xe5fc}, // MC0_CH0_CR_MCMNTS_SPARE2, MC0_CH0_CR_MCMNTS_SPARE
};

const SaveDataControl SaveDataMcPerSaGv64PerMc[] = {
  {0xd930, 0xd930}, // MC0_SC_QOS
  {0xd960, 0xd960}, // MC0_SC_QOS2
};

const SaveDataControl SaveDataVccClkPerSaGv32PerMc[] = {
  {0x4798, 0x4798}, // DDRVCCCLK_SBMEM0_CR_SPARE
};

const SaveDataControl SaveDataMcPerSaGv64PerCh[] = {
  {0xe000, 0xe000}, // MC0_CH0_CR_TC_PRE
  {0xe050, 0xe050}, // MC0_CH0_CR_TC_PWRDN
  {0xe0a8, 0xe0a8}, // MC0_CH0_CR_SC_PR_CNT_CONFIG
  {0xe0b8, 0xe0b8}, // MC0_CH0_CR_SPID_LOW_POWER_CTL
  {0xe108, 0xe108}, // MC0_CH0_CR_TC_WCK
  {0xe138, 0xe138}, // MC0_CH0_CR_TC_ACT
  {0xe148, 0xe148}, // MC0_CH0_CR_WCK_CONFIG
  {0xe4a0, 0xe4a0}, // MC0_CH0_CR_TC_RFTP
  {0xe4c0, 0xe4c0}, // MC0_CH0_CR_TC_SREXITTP
};

const SaveDataControl SaveDataMcCommon64PerCh[] = {
  {0xe040, 0xe040}, // MC0_CH0_CR_PM_PDWN_CONFIG
  {0xe068, 0xe068}, // MC0_CH0_CR_SC_WDBWM
  {0xe0e0, 0xe0e0}, // MC0_CH0_CR_SC_BLOCKING_RULES_CFG
  {0xe118, 0xe120}, // MC0_CH0_CR_ROWHAMMER_CTL, MC0_CH0_CR_SC_ADAPTIVE_PCIT
  {0xe130, 0xe130}, // MC0_CH0_CR_AUTO_PRE_CONTROL
};

const SaveDataControl SaveDataDdrioPerSaGv32PerByte[] = {
  {0x0, 0x7c}, // DATA0CH0_CR_DCCCALCCONTROL, DATA0CH0_CR_DCCFSMCONTROL, DATA0CH0_CR_DCCLANETARGET, DATA0CH0_CR_DCCPILUT0,
               // DATA0CH0_CR_DDRCRDATACONTROL2, DATA0CH0_CR_DDRCRDATAOFFSETTRAIN, DATA0CH0_CR_DDRCRDLLCONTROL0, DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0,
               // DATA0CH0_CR_DDRCRRANKMUXDELAY, DATA0CH0_CR_DDRCRXTALKBIASCONTROL1, DATA0CH0_CR_DDRDATADQRANK0LANE0, DATA0CH0_CR_DDRDATADQRANK0LANE1,
               // DATA0CH0_CR_DDRDATADQRANK0LANE2, DATA0CH0_CR_DDRDATADQRANK0LANE3, DATA0CH0_CR_DDRDATADQRANK0LANE4, DATA0CH0_CR_DDRDATADQRANK0LANE5,
               // DATA0CH0_CR_DDRDATADQRANK0LANE6, DATA0CH0_CR_DDRDATADQRANK0LANE7, DATA0CH0_CR_DDRDATADQRANK1LANE0, DATA0CH0_CR_DDRDATADQRANK1LANE1,
               // DATA0CH0_CR_DDRDATADQRANK1LANE2, DATA0CH0_CR_DDRDATADQRANK1LANE3, DATA0CH0_CR_DDRDATADQRANK1LANE4, DATA0CH0_CR_DDRDATADQRANK1LANE5,
               // DATA0CH0_CR_DDRDATADQRANK1LANE6, DATA0CH0_CR_DDRDATADQRANK1LANE7, DATA0CH0_CR_DDRDATADQSRANK0, DATA0CH0_CR_DDRDATADQSRANK1,
               // DATA0CH0_CR_REFPICONTROL, DATA0CH0_CR_SPARE, DATA0CH0_CR_TXCONTROL0RANK0, DATA0CH0_CR_TXCONTROL0RANK1
  {0x84, 0xa0}, // DATA0CH0_CR_DDRCRXTALKBIASCONTROL2, DATA0CH0_CR_DDRCRXTALKBIASCONTROL3, DATA0CH0_CR_DDRCRXTALKBIASCONTROL4,
                // DATA0CH0_CR_DDRCRXTALKBIASCONTROL5, DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4, DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1,
                // DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6, DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1
  {0xb4, 0xb4}, // DATA0CH0_CR_VCCCLKCOMPRXOFFSET
  {0xcc, 0xd0}, // DATA0CH0_CR_DDRCRWRRETRAININITRANK0, DATA0CH0_CR_DDRCRWRRETRAININITRANK1
  {0xdc, 0xf0}, // DATA0CH0_CR_DDRCRRDRETRAININITRANK0, DATA0CH0_CR_DDRCRRDRETRAININITRANK1, DATA0CH0_CR_DDRCRRXCTLE, DATA0CH0_CR_DDRDQREPLICACTLE,
                // DATA0CH0_CR_RXCONTROL0RANK0, DATA0CH0_CR_RXCONTROL0RANK1
  {0xf8, 0x120}, // DATA0CH0_CR_DDRCRCMDBUSTRAIN, DATA0CH0_CR_DDRCRDATACONTROL0, DATA0CH0_CR_DDRCRDATACONTROL1, DATA0CH0_CR_DDRCRDATACONTROL5,
                 // DATA0CH0_CR_DDRCRDATAOFFSETCOMP, DATA0CH0_CR_DDRCRDATATCOCONTROL0, DATA0CH0_CR_DDRCRDATATCOCONTROL1,
                 // DATA0CH0_CR_DDRCRRDRETRAINCONTROLSTATUS, DATA0CH0_CR_DDRCRRXEQTAP, DATA0CH0_CR_DDRCRRXEQTAP01, DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS
  {0x160, 0x164}, // DATA0CH0_CR_DDRCRADC, DATA0CH0_CR_DDRCRCOMPDQSDELAY
  {0x16c, 0x17c}, // DATA0CH0_CR_DDRCRDATAVREFPERBIT012, DATA0CH0_CR_DDRCRDATAVREFPERBIT345, DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI,
                  // DATA0CH0_CR_DDRDATADQRANK0LANE8, DATA0CH0_CR_DDRDATADQRANK1LANE8
};

const SaveDataControl SaveDataDdrioPerSaGv32[] = {
  {0x2680, 0x2698}, // DDRPHY_DDRCOMP_CR_DDRCRCACOMP, DDRPHY_DDRCOMP_CR_DDRCRCLKCOMP, DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL0, DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL1,
                    // DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL2, DDRPHY_DDRCOMP_CR_DDRCRCTLCOMP, DDRPHY_DDRCOMP_CR_DDRCRDATACOMP
  {0x26a0, 0x26a0}, // DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL3
  {0x26a8, 0x26b0}, // DDRPHY_DDRCOMP_CR_DDRCOMPDATA, DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL4, DDRPHY_DDRCOMP_CR_VCCCLKTARGET
  {0x26b8, 0x26c0}, // DDRPHY_DDRCOMP_CR_VCCCLKCOMPDATA, DDRPHY_DDRCOMP_CR_VCCCLKCOMPDATACCC, DDRPHY_DDRCOMP_CR_VCCCLKCOMPTEMPCODE
  {0x26d0, 0x26d4}, // DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER0, DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1
  {0x26e0, 0x26e8}, // DDRPHY_DDRCOMP_CR_DDRCRCOMPOVR0, DDRPHY_DDRCOMP_CR_LVRVOLTAGESENSOR, DDRPHY_DDRCOMP_CR_WRONGPHASECONTROL
  {0x26f0, 0x26f0}, // DDRPHY_DDRCOMP_CR_SPARE
  {0x2710, 0x2710}, // DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL
  {0x2718, 0x271c}, // DDRPHY_DDRCOMP_CR_COMPOVERRIDE, DDRPHY_DDRCOMP_CR_FSMCODEDELAYCTRL
  {0x2724, 0x2728}, // DDRPHY_DDRCOMP_CR_PM_MISCBONUS, DDRPHY_DDRCOMP_CR_VISA2VIEW
  {0x274C, 0x2750}, // DDRPHY_DDRCOMP_CR_CBB_BONUS, DDRPHY_DDRCOMP_CR_DDRCRBWSELCOMPDATACCC
  {0x2760, 0x2764}, // DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL, DDRPHY_DDRCOMP_CR_VCCDLLDCCCONTROL0
  {0x276c, 0x277c}, // DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0, DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL5, DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD,
                    // DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL2, DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL5
  {0x2790, 0x27b4}, // DDRDLL_COMPCR_DLLTIMERCONTROL1, DDRDLL_COMPCR_DLLTIMERCONTROL2, DDRPHY_DDRCOMP_CR_BWSELCOMPCONTROL0,
                    // DDRPHY_DDRCOMP_CR_BWSELCOMPCONTROL1, DDRPHY_DDRCOMP_CR_BWSELCOMPCONTROL2,
                    // DDRPHY_DDRCOMP_CR_COMPCTL6, DDRPHY_DDRCOMP_CR_DDRCRDVFSCOMPOVR0, DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL,
                    // DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL3, DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL4
  {0x27bc, 0x27bc}, // DDRPHY_DDRCOMP_CR_DDRCRSCOMP0
  {0x27cc, 0x27cc}, // DDRPHY_DDRCOMP_CR_VCCDLLCOMPCONTROL6
  {0x3e80, 0x3ee0}, // DDRPLL_CR_TXCLOCKGLOBAL, DDRSCRAM_CR_DDRMISCCONTROL0, DDRSCRAM_CR_DDRMISCCONTROL1, DDRSCRAM_CR_DDRMISCCONTROL2,
                    // MCMISCS_DDRWCKCONTROL, MCMISCS_READCFGCH0, MCMISCS_READCFGCH1, MCMISCS_READCFGCH2, MCMISCS_READCFGCH3,
                    // MCMISCS_READCFGCH4, MCMISCS_READCFGCH5, MCMISCS_READCFGCH6, MCMISCS_READCFGCH7, MCMISCS_RXDQFIFORDENCH01,
                    // MCMISCS_RXDQFIFORDENCH23, MCMISCS_RXDQFIFORDENCH45, MCMISCS_RXDQFIFORDENCH67, MCMISCS_WRITECFGCH0,
                    // MCMISCS_WRITECFGCH1, MCMISCS_WRITECFGCH2, MCMISCS_WRITECFGCH3, MCMISCS_WRITECFGCH4, MCMISCS_WRITECFGCH5,
                    // MCMISCS_WRITECFGCH6, MCMISCS_WRITECFGCH7
  {0x3eec, 0x3eec}, // DDRMCMISC_CR_DDRVDD2POWER
  {0x3f00, 0x3f10}, // DDRMISCS_CR_DDRPHY_DFI_LPCTRL0, DDRMISCS_CR_DDRPHY_DFI_LPCTRL1, DDRMISCS_CR_DDRPHY_DFI_LPCTRL2, DDRSCRAM_CR_VISA2VIEW,
                    // MCMISCS_CR_SPARE
  {0x3fa4, 0x3fa8}, // DDRSCRAM_CR_CS_GEARDOWN, DDRSCRAM_CR_TLCLKGATING
  {0x49a0, 0x49a0}, // DDRPHY_DDRCOMP_SBMEM_CR_SPARE
};

const SaveDataControl SaveDataDdrioPerSaGv32PerDataSbmem[] = { // DDRDATA_SBMEM needs to be separate group due to unique register offsets spacing 
  {0x4160, 0x4160}, // DDRDATA_SBMEM0_CR_SPARE
};

const SaveDataControl SaveDataDdrioPerSaGv32PerPG[] = { // Covers partitions PG[0...9] and PGTERM[0...1]
  {0x1e00, 0x1e00}, // DDRPGTERMCR0_CR_LVRVOLTAGESENSOR
  {0x1e08, 0x1e08}, // DDRPGTERMCR0_CR_SPARE
  {0x1e28, 0x1e28}, // DDRPGTERMCR0_CR_TLINETERMINATION
};

const SaveDataControl SaveDataDdrioPerSaGv32PerDataShared[] = {
  {0x2804, 0x2804}, // DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1
  {0x280c, 0x2810}, // DDRDATA_SHARED0_CR_PHCLOCKREPEATER0, DDRDATA_SHARED0_CR_PHCLOCKREPEATER1
  {0x2824, 0x2824}, // DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL
  {0x2830, 0x2838}, // DDRDATA_SHARED0_CR_DDRCRDLLOFFSETCONTROL, DDRDATA_SHARED0_CR_PHCLOCKREPEATER2, DDRDATA_SHARED0_CR_PHCLOCKREPEATER3
  {0x2840, 0x285c}, // DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0, DDRDATA_SHARED0_CR_DDRCRCCCCH1LANE0, DDRDATA_SHARED0_CR_DDRCRCCCPICODES,
                    // DDRDATA_SHARED0_CR_DDRCRDATACONTROL6, DDRDATA_SHARED0_CR_DDRCRWCKCONTROL, DDRDATA_SHARED0_CR_DDRCRWCKPERBITDESKEWRISEFALL,
                    // DDRDATA_SHARED0_CR_WCKDCCCALCCONTROL, DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL
  {0x286c, 0x2870}, // DDRDATA_SHARED0_CR_DDRCRCLKCOMP, DDRDATA_SHARED0_CR_DDRCRCLKCOMPOFFSET
  {0x2888, 0x2890}, // DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0, DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL1, DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2
  {0x28a0, 0x28f4}, // DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC, DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0, DDRDATA_SHARED0_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0,
                    // DDRDATA_SHARED0_CR_DDRCRCBBBONUS0, DDRDATA_SHARED0_CR_DDRCRCLOCKINGBONUSOVRD0, DDRDATA_SHARED0_CR_DDRCRDLLCBTUNEOVRD,
                    // DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0, DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL1, DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL2,
                    // DDRDATA_SHARED0_CR_DDRCRWCKDQSBUFFERBONUS0, DDRDATA_SHARED0_CR_DDR_PMBONUS0, DDRDATA_SHARED0_CR_DLLTIMERCONTROL,
                    // DDRDATA_SHARED0_CR_REFPIDQSWCKOFFSET, DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET, DDRDATA_SHARED0_DDRRXDLL_CR_PICODELUT0,
                    // DDRDATA_SHARED0_DDRRXDLL_CR_PICODELUT1, DDRDATA_SHARED0_DDRRXDLL_CR_PICODELUT2, DDRDATA_SHARED0_DDRRXDLL_CR_PICODELUT3,
                    // DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0, DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1, DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2,
                    // DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3
};

const SaveDataControl SaveDataDdrioPerSaGv32PerCCCShared[] = {
  {0x3200, 0x3204}, // DDRCCC_SHARED0_CR_DDRCRBWSELCOMPDATACCC, DDRCCC_SHARED0_CR_DDRCRVCCCLKCOMPOFFSET
  {0x3214, 0x3214}, // DDRCCC_SHARED0_CR_DDRCRDLLOFFSETCONTROL
  {0x321c, 0x3220}, // DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS, DDRCCC_SHARED0_CR_DDRCRVOLTAGEUSEDCLKCONTROL
  {0x3230, 0x3244}, // DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL0, DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL1, DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL2,
                    // DDRCCC_SHARED0_CR_PHCLOCKREPEATER0, DDRCCC_SHARED0_CR_PHCLOCKREPEATER1, DDRCCC_SHARED0_CR_SPARE
  {0x324c, 0x325c}, // DDRCCC_SHARED0_CR_CBB_BONUS1, DDRCCC_SHARED0_CR_DDRCRCCCDLLWAKEUPCONTROLS, DDRCCC_SHARED0_CR_DDRCRVCCDLLCOMPCCC,
                    // DDRCCC_SHARED0_CR_DDRCRVCCDLLCOMPCCCOFFSET, DDRCCC_SHARED0_CR_DDR_PMBONUS0
  {0x3268, 0x3270}, // DDRCCC_SHARED0_CR_DDRCRDLLCBTUNEOVRD, DDRCCC_SHARED0_CR_PHCLOCKREPEATER2, DDRCCC_SHARED0_CR_PHCLOCKREPEATER3
  {0x327c, 0x327c}, // DDRCCC_SHARED0_CR_DLLTIMERCONTROL
  {0x45dc, 0x45dc}, // DDRCCC_SBMEM0_CR_SPARE
};


const SaveDataControl SaveDataDdrioPerSaGv32PerCCC[] = {
  {0x360c, 0x3648}, // CH0CCC_CR_DCCCALCCONTROL, CH0CCC_CR_DCCFSMCONTROL, CH0CCC_CR_DDRCRCCCLANE0, CH0CCC_CR_DDRCRCCCLANE1,
                    // CH0CCC_CR_DDRCRCCCLANE2, CH0CCC_CR_DDRCRCCCLANE3, CH0CCC_CR_DDRCRCCCLANE4, CH0CCC_CR_DDRCRCCCLANE5,
                    // CH0CCC_CR_DDRCRCCCLANE6, CH0CCC_CR_DDRCRCCCLANE7, CH0CCC_CR_DDRCRCCCLANE8, CH0CCC_CR_DDRCRCCCLANE9,
                    // CH0CCC_CR_DDRCRCCCPICODES, CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET, CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL,
                    // CH0CCC_CR_REFPICONTROL
  {0x3684, 0x36a4}, // CH0CCC_CR_BONUS0, CH0CCC_CR_CBB_BONUS0, CH0CCC_CR_CCC_OVRDCTL, CH0CCC_CR_LPMODE4_FLYBY, CH0CCC_CR_REFPICLKOFFSET,
                    // CH0CCC_DDRTXDLL_CR_PICODELUT0, CH0CCC_DDRTXDLL_CR_PICODELUT1, CH0CCC_DDRTXDLL_CR_PICODELUT2, CH0CCC_DDRTXDLL_CR_PICODELUT3
  {0x36ac, 0x36b8}, // CH0CCC_CR_COMPUPDATEOVERRIDE, CH0CCC_CR_CS_GEARDOWN, CH0CCC_CR_DDRCRCCCDCCCLKLANE5, CH0CCC_CR_SPARE
};

const SaveDataControl SaveDataDdrioCommon32PerByte[] = {
  {0x124, 0x124}, // DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL
  {0x12c, 0x12c}, // DATA0CH0_CR_DDRCRRDRETRAINDELTARANK
};

const SaveDataControl SaveDataDdrioCommon32[] = {
  {0x26ec, 0x26ec}, // DDRPHY_DDRCOMP_CR_DDRCRVIEWCTL
  {0x3ee4, 0x3ee4}, // MCMISCS_COMPCONTROL
  {0x3ef0, 0x3efc}, // DDRSCRAM_CR_DDRSCRAMBLECH10, DDRSCRAM_CR_DDRSCRAMBLECH32, DDRSCRAM_CR_DDRSCRAMBLECH54, DDRSCRAM_CR_DDRSCRAMBLECH76
  {0x3f20, 0x3f30}, // DDRMISCS_CR_DVP_PKTZR_DTF_SRC_CONFIG, DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_0, DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_1,
                    // DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_2, DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_1_3
  {0x3f38, 0x3f38}, // DDRMISCS_CR_DVP_ENCDR_DTF_SRC_CONFIG
  {0x3f40, 0x3f40}, // DDRMISCS_CR_DVP_PKTZR_DTF_DVP_CTRL
  {0x3f48, 0x3f48}, // DDRMISCS_CR_DVP_PKTZR_DTF_PKTSTRM_CTRL_0
  {0x3f50, 0x3f5c}, // DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_0, DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_1, DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_2,
                    // DDRMISCS_CR_DVP_PKTZR_DTF_BYTE_LANE_CTRL_3
  {0x3f68, 0x3f78}, // DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_0_0, DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_0_1, DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_0_2,
                    // DDRMISCS_CR_DVP_PKTZR_TRIG_BYTE_LANE_CTRL_0_3, MCMISCS_DVP_MISC
  {0x3f80, 0x3f80}, // MCMISCS_CR_DUMMY
  {0x4000, 0x4000}, // DDRPHY_MISC_SAUG_CR_PHYPMOVRD
  {0x404c, 0x404c}, // DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL
  {0x4064, 0x4064}, // DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0
  {0x407c, 0x407c}, // DDRPHY_MISC_SAUG_CR_PHYPMOVRD3
};

const SaveDataControl SaveDataDdrIoCommon32PerCccShared[] = {
  {0x45BC, 0x45BC}, // DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0
};

const SaveDataLoopControl SaveDataLoop[] = {
  {SaveDataMcCommon32PerMc,             SAVE_DATA_CONTROL_SIZE (SaveDataMcCommon32PerMc),             PtMc,         StCommon, R32Bit, STR (SaveDataMcCommon32PerMc)},
  {SaveDataMcCommon32PerCh,             SAVE_DATA_CONTROL_SIZE (SaveDataMcCommon32PerCh),             PtCh,         StCommon, R32Bit, STR (SaveDataMcCommon32PerCh)},
  {NULL,                                0,                                                            PtGenFsm,     StCommon, R32Bit, STR ("GenericMrsFsm Control")},
  {NULL,                                0,                                                            PtGenFsm,     StSaGv,   R32Bit, STR ("GenericMrsFsm Storage/Timing")},
  {SaveDataMcPerSaGv32PerMc,            SAVE_DATA_CONTROL_SIZE (SaveDataMcPerSaGv32PerMc),            PtMc,         StSaGv,   R32Bit, STR (SaveDataMcPerSaGv32PerMc)},
  {SaveDataVccClkPerSaGv32PerMc,        SAVE_DATA_CONTROL_SIZE (SaveDataVccClkPerSaGv32PerMc),        PtVccClk,     StSaGv,   R32Bit, STR (SaveDataVccClkPerSaGv32PerMc)},
  {SaveDataMcPerSaGv32PerCh,            SAVE_DATA_CONTROL_SIZE (SaveDataMcPerSaGv32PerCh),            PtCh,         StSaGv,   R32Bit, STR (SaveDataMcPerSaGv32PerCh)},
  {SaveDataMcPerSaGv64PerMc,            SAVE_DATA_CONTROL_SIZE (SaveDataMcPerSaGv64PerMc),            PtMc,         StSaGv,   R64Bit, STR (SaveDataMcPerSaGv64PerMc)},
  {SaveDataMcPerSaGv64PerCh,            SAVE_DATA_CONTROL_SIZE (SaveDataMcPerSaGv64PerCh),            PtCh,         StSaGv,   R64Bit, STR (SaveDataMcPerSaGv64PerCh)},
  {SaveDataMcCommon64PerCh,             SAVE_DATA_CONTROL_SIZE (SaveDataMcCommon64PerCh),             PtCh,         StCommon, R64Bit, STR (SaveDataMcCommon64PerCh)},
  {SaveDataDdrioPerSaGv32PerByte,       SAVE_DATA_CONTROL_SIZE (SaveDataDdrioPerSaGv32PerByte),       PtByte,       StSaGv,   R32Bit, STR (SaveDataDdrioPerSaGv32PerByte)},
  {SaveDataDdrioPerSaGv32,              SAVE_DATA_CONTROL_SIZE (SaveDataDdrioPerSaGv32),              PtNone,       StSaGv,   R32Bit, STR (SaveDataDdrioPerSaGv32)},
  {SaveDataDdrioPerSaGv32PerCCC,        SAVE_DATA_CONTROL_SIZE (SaveDataDdrioPerSaGv32PerCCC),        PtCCC,        StSaGv,   R32Bit, STR (SaveDataDdrioPerSaGv32PerCCC)},
  {SaveDataDdrioPerSaGv32PerCCCShared,  SAVE_DATA_CONTROL_SIZE (SaveDataDdrioPerSaGv32PerCCCShared),  PtCCCShared,  StSaGv,   R32Bit, STR (SaveDataDdrioPerSaGv32PerCCCShared)},
  {SaveDataDdrioPerSaGv32PerDataShared, SAVE_DATA_CONTROL_SIZE (SaveDataDdrioPerSaGv32PerDataShared), PtDATAShared, StSaGv,   R32Bit, STR (SaveDataDdrioPerSaGv32PerDataShared)},
  {SaveDataDdrioPerSaGv32PerDataSbmem,  SAVE_DATA_CONTROL_SIZE (SaveDataDdrioPerSaGv32PerDataSbmem),  PtDATASbmem,  StSaGv,   R32Bit, STR (SaveDataDdrioPerSaGv32PerDataSbmem)},
  {SaveDataDdrioPerSaGv32PerPG,         SAVE_DATA_CONTROL_SIZE (SaveDataDdrioPerSaGv32PerPG),         PtPG,         StSaGv,   R32Bit, STR (SaveDataDdrioPerSaGv32PerPG)},
  {SaveDataDdrioCommon32PerByte,        SAVE_DATA_CONTROL_SIZE (SaveDataDdrioCommon32PerByte),        PtByte,       StCommon, R32Bit, STR (SaveDataDdrioCommon32PerByte)},
  {SaveDataDdrioCommon32,               SAVE_DATA_CONTROL_SIZE (SaveDataDdrioCommon32),               PtNone,       StCommon, R32Bit, STR (SaveDataDdrioCommon32)},
  {SaveDataDdrIoCommon32PerCccShared,   SAVE_DATA_CONTROL_SIZE (SaveDataDdrIoCommon32PerCccShared),   PtCCCShared,  StCommon, R32Bit, STR (SaveDataDdrIoCommon32PerCccShared)}
};

/**
  This function verifies that neither CPU fuses or DIMMs have changed.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess if fast boot is allowed, otherwise mrcColdBootRequired.
**/
MrcStatus
MrcFastBootPermitted (
  IN     MrcParameters *const MrcData
  )
{
  const MrcInput            *Inputs;
  const MrcControllerIn     *ControllerIn;
  const MrcChannelIn        *ChannelIn;
  const MrcDimmIn           *DimmIn;
  const UINT8               *CrcStart;
  MrcOutput                 *Outputs;
  MrcDebug                  *Debug;
  MrcSaveData               *Save;
  MrcContSave               *ControllerSave;
  MrcChannelSave            *ChannelSave;
  MrcDimmOut                *DimmSave;
  UINT32                    CrcSize;
  UINT16                    DimmCrc;
  UINT8                     Controller;
  UINT8                     Channel;
  UINT8                     Dimm;
  MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_STRUCT PmaFuseTechEn;
  MEMSS_PMA_CR_CAPABILITIES_MISC_STRUCT          PmaMcCap;

  CrcStart = NULL;
  CrcSize  = 0;
  Inputs   = &MrcData->Inputs;
  Save     = &MrcData->Save.Data;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;

  // Obtain the capabilities of the memory controller.
  PmaFuseTechEn.Data = MrcReadCR (MrcData, MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_REG);
  PmaMcCap.Data = MrcReadCR (MrcData, MEMSS_PMA_CR_CAPABILITIES_MISC_REG);

  if ((PmaMcCap.Data != Save->PmaMcCap) || (PmaFuseTechEn.Data != Save->PmaMcTechEnable)) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "Capabilities have changed, cold boot required\n '%X_%X' --> '%X_%X'\n",
      Save->PmaMcCap,
      Save->PmaMcTechEnable,
      PmaMcCap.Data,
      PmaFuseTechEn.Data
      );
    return mrcColdBootRequired;
  }
  // See if any of the DIMMs have changed.
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn   = &Inputs->Controller[Controller];
    ControllerSave = &Save->Controller[Controller];
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelIn   = &ControllerIn->Channel[Channel];
      ChannelSave = &ControllerSave->Channel[Channel];
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmIn   = &ChannelIn->Dimm[Dimm];
        DimmSave = &ChannelSave->Dimm[Dimm];
        if (DimmIn->Status == DIMM_DISABLED) {
          DimmCrc = 0;
        } else {
          CrcStart = MrcSpdCrcArea (MrcData, Controller, Channel, Dimm, &CrcSize);
          GetDimmCrc ((const UINT8 *const) CrcStart, CrcSize, &DimmCrc);
        }

        MRC_DEBUG_MSG (
          Debug,
          MSG_LEVEL_NOTE,
          "Channel %u Dimm %u DimmCrc %Xh, DimmSave->Crc %Xh\n",
          Channel,
          Dimm,
          DimmCrc,
          DimmSave->Crc
          );
        if (DimmCrc != DimmSave->Crc) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Dimm has changed, cold boot required\n");
          return mrcColdBootRequired;
        }
      }
    }
  }
  // Set RestoreMRs flag to use trained Opt Param Values for Power Savings.
  Outputs->RestoreMRs = TRUE;

  return mrcSuccess;
}

/**
  This function saves or restores register values based on the SaveRestore flag.

  @param[in, out] MrcData                 - Include all the MRC global data.
  @param[in]      Controller              - Handle to the controller
  @param[in]      McCh                    - Handle to channel within the memory controller.
  @param[in, out] McRegister              - Save or Restore registers value in mrc save data.
  @param[in]      SRScope                 - Control flag to save registers or to restore registers per SAGV/Common.
  @param[in]      SaveRestore             - Control flag to save registers or to restore registers.
  @param[in]      SkipPrint               - Debug prints control flag.

  @retval LocalRegLoopCnt - Returns number of registers saved/restored.
**/
UINT32
SaveRestoreGenFsmRegisters (
  IN OUT MrcParameters   *const MrcData,
  IN     UINT32                 Controller,
  IN     UINT32                 McCh,
  IN OUT UINT32                 **McRegister,
  IN     SaveRestoreScope       SRScope,
  IN     MrcSaveOrRestore       SaveRestore,
  IN     BOOLEAN                SkipPrint
  )
{
  MrcDebug          *Debug;
  MrcDebugMsgLevel  DebugLevel;
  UINT32            ControlRegIdx;
  UINT32            TimingRegIdx;
  UINT32            StorageRegIdx;
  UINT32            Data;
  UINT32            LocalRegLoopCnt;

  Debug       = &MrcData->Outputs.Debug;
  DebugLevel  = SkipPrint ? MSG_LEVEL_NONE : MSG_LEVEL_NOTE;

  // Gen MRS FSM Control registers are in Common scope; Timing / Storage registers are per SAGV

  Data = 0;
  LocalRegLoopCnt = 0;
  if (SRScope == StCommon) {
    for (ControlRegIdx = 0; ControlRegIdx < MAX_MR_GEN_FSM; ControlRegIdx++) {
      LocalRegLoopCnt++;
      if (SaveRestore == MrcSaveEnum) {
        GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxRead, GmfMailboxTypeControl, ControlRegIdx, &Data);
        *(*McRegister)++ = Data;
      } else {
        Data = *(*McRegister)++;
        GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxWrite, GmfMailboxTypeControl, ControlRegIdx, &Data);
      }
      MRC_DEBUG_MSG (Debug, DebugLevel, "\n ControlRegIdx: %d, Data: 0x%08x", ControlRegIdx, Data);
    } // Control Registers
  } else {
    for (TimingRegIdx = 0; TimingRegIdx < MAX_TIMING_GEN_FSM; TimingRegIdx++) {
      LocalRegLoopCnt++;
      if (SaveRestore == MrcSaveEnum) {
        GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxRead, GmfMailboxTypeTiming, TimingRegIdx, &Data);
        *(*McRegister)++ = Data;
      } else {
        Data = *(*McRegister)++;
        GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxWrite, GmfMailboxTypeTiming, TimingRegIdx, &Data);
      }
      MRC_DEBUG_MSG (Debug, DebugLevel, "\n TimingRegIdx: %d, Data: 0x%08x", TimingRegIdx, Data);
    } // Timing Registers
    for (StorageRegIdx = 0; StorageRegIdx < MAX_STORAGE_GEN_FSM; StorageRegIdx++) {
      LocalRegLoopCnt++;
      if (SaveRestore == MrcSaveEnum) {
        GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxRead, GmfMailboxTypeStorage, StorageRegIdx, &Data);
        *(*McRegister)++ = Data;
      } else {
        Data = *(*McRegister)++;
        GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxWrite, GmfMailboxTypeStorage, StorageRegIdx, &Data);
      }
      MRC_DEBUG_MSG (Debug, DebugLevel, "\n StorageRegIdx: %d, Data: 0x%08x", StorageRegIdx, Data);
    } // Storage Registers
  }

  return LocalRegLoopCnt;
}

/**
  This function saves or restores register values based on the SaveRestore flag.

  @param[in, out] MrcData                 - Include all the MRC global data.
  @param[in]      SaveRestoreDataCtrlPtr  - Registers data group to be saved or restored.
  @param[in]      SizeOfData              - Total size of the register group.
  @param[in]      RegOffset               - Register offset increment of the register group.
  @param[in, out] McRegister              - Save or Restore registers value in mrc save data.
  @param[in]      SaveRestore             - Control flag to save registers or to restore registers.
  @param[in]      Is32Bit                 - Flag that indicates weather the register range is 32 BIT or 64 BIT.
  @param[in]      SkipPrint               - Debug prints control flag.

  @retval LocalRegLoopCnt - Returns number of registers saved/restored.
**/
UINT32
SaveRestoreRegisters (
  IN OUT MrcParameters   *const MrcData,
  IN     SaveDataControl  const *SaveRestoreDataCtrlPtr,
  IN     UINT32                 SizeOfData,
  IN     UINT32                 RegOffset,
  IN OUT UINT32                 **McRegister,
  IN     MrcSaveOrRestore       SaveRestore,
  IN     BOOLEAN                Is32Bit,
  IN     BOOLEAN                SkipPrint
  )
{
  UINT32            Offset;
  UINT32            Index;
  UINT32            Value;
  UINT32            LocalRegLoopCnt;
  UINT32            SizeIncrement;
  UINT64            Value64;
  MrcDebug          *Debug;
  MrcDebugMsgLevel  DebugLevel;

  Debug         = &MrcData->Outputs.Debug;
  DebugLevel    = SkipPrint ? MSG_LEVEL_NONE : MSG_LEVEL_NOTE;
  SizeIncrement = Is32Bit ? sizeof (UINT32) : sizeof (UINT64);

  LocalRegLoopCnt = 0;
  for (Index = 0; Index < SizeOfData; Index++) {
    for (Offset = SaveRestoreDataCtrlPtr->StartMchbarOffset; Offset <= SaveRestoreDataCtrlPtr->EndMchbarOffset; Offset += SizeIncrement) {
      LocalRegLoopCnt++;
      if (SaveRestore == MrcSaveEnum) {
        if (!Is32Bit) {
          Value64 = MrcReadCR64 (MrcData, RegOffset + Offset);
          MRC_DEBUG_MSG (Debug, DebugLevel, "\n Index: %02d, Offset: %08x, Value: 0x%016llx,  ", Index, Offset + RegOffset, Value64);
          *(UINT64 *)(*McRegister)++ = Value64;
          (*McRegister)++;
        } else {
          Value = MrcReadCR (MrcData, RegOffset + Offset);
          MRC_DEBUG_MSG (Debug, DebugLevel, "\n Index: %02d, Offset: %08x, Value: 0x%08x, ", Index, Offset + RegOffset, Value);
          *(*McRegister)++ = Value;
        }
      } else {
        if (!Is32Bit) {
          Value64 = *(UINT64 *)(*McRegister)++;
          MrcWriteCR64 (MrcData, RegOffset + Offset, Value64);
          MRC_DEBUG_MSG (Debug, DebugLevel, "\n Index: %02d, Offset: %08x, Value: 0x%016llx,  ", Index, Offset + RegOffset, Value64);
          (*McRegister)++;
        } else {
          Value = *(*McRegister)++;
          MrcWriteCR (MrcData, RegOffset + Offset, Value);
          MRC_DEBUG_MSG (Debug, DebugLevel, "\n Index: %02d, Offset: %08x, Value: 0x%08x, ", Index, Offset + RegOffset, Value);
        }
      }
    }
    SaveRestoreDataCtrlPtr++;
  }
  return LocalRegLoopCnt;
}

/**
  This function is a wrapper to save or restore registers in Common or SaGv scope.

  @param[in, out] MrcData      - Include all the MRC global data.
  @param[in]      Scope        - Scope of registers, SaGv or Common.
  @param[in]      SaveType     - Indicator of Saving or Restoring.
  @param[in]      SkipPrint    - Debug prints control flag.

  @retval  MrcStatus - mrcSuccess if the save occurred with no errors, otherwise returns an error code.
**/
MrcStatus
SaveRestoreExecution (
  IN OUT  MrcParameters  *const MrcData,
  IN      SaveRestoreScope      Scope,
  IN      MrcSaveOrRestore      SaveType,
  IN      BOOLEAN               SkipPrint
)
{
  const SaveDataLoopControl  *LoopControl;
  MrcSaveData   *SaveData;
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  MrcDebugMsgLevel  DebugLevel;
  UINT32        *McRegister;
  UINT32        *McRegisterStart;
  UINT32        LocalRegLoopCnt;
  UINT32        TotalRegCnt;
  UINT32        RegOffset;
  UINT32        RegCountLimit;
  UINT32        RegCountDifference;
  UINT8         Index;
  UINT8         Controller;
  UINT8         Channel;
  UINT8         PartIdx;
  UINT8         Byte;
  BOOLEAN       Is32Bit;
  MrcStatus     Status;
  char          *IterTypeName;
  char          *SaveTypeName;

  SaveData    = &MrcData->Save.Data;
  Outputs     = &MrcData->Outputs;
  Debug       = &MrcData->Outputs.Debug;
  DebugLevel  = SkipPrint ? MSG_LEVEL_NONE : MSG_LEVEL_NOTE;

  Status        = mrcSuccess;
  LoopControl   = NULL;
  TotalRegCnt   = 0;
  IterTypeName  = (Scope == StCommon) ? "Common" : "SaGv";
  SaveTypeName  = (SaveType == MrcSaveEnum) ? "Save" : "Restore";
  RegCountLimit = (Scope == StCommon) ? MRC_REGISTER_COUNT_COMMON : MRC_REGISTER_COUNT_SAGV;

  if (Scope == StSaGv) {
    McRegister = McRegisterStart = SaveData->SaGvRegSave[Outputs->SaGvPoint];
  } else {
    McRegister = McRegisterStart = SaveData->RegSaveCommon;
  }

  for (Index = 0; Index < ARRAY_COUNT(SaveDataLoop); Index++) {
    LoopControl = &SaveDataLoop[Index];
    LocalRegLoopCnt = 0;
    if (Scope != LoopControl->SRScope) {
      continue;
    }

    if (MrcData->Inputs.ExtInputs.Ptr->SimicsFlag) {
      if ( LoopControl->PScope == PtNone
        || LoopControl->PScope == PtByte
        || LoopControl->PScope == PtCCC
        || LoopControl->PScope == PtCCCShared
        || LoopControl->PScope == PtDATAShared
        || LoopControl->PScope == PtDATASbmem
        || LoopControl->PScope == PtVccClk
        || LoopControl->PScope == PtPG) {
        continue;
      }
    }
    Is32Bit = (LoopControl->RegSize == R32Bit) ? TRUE : FALSE;

    switch(LoopControl->PScope) {
      case PtGenFsm:
        for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
          for (Channel = 0; Channel < MAX_CHANNEL_SHARE_REGS; Channel++) {
            if (!MrcChannelExist (MrcData, Controller, Channel)) {
              continue;
            }
            MRC_DEBUG_MSG (Debug, DebugLevel, "\nMC%u C%u:", Controller, Channel);
            LocalRegLoopCnt += SaveRestoreGenFsmRegisters (MrcData, Controller, Channel, &McRegister, LoopControl->SRScope, SaveType, SkipPrint);
          } // for Channel
        } // for Controller
        TotalRegCnt += LocalRegLoopCnt;
        break;

      case PtNone:
        RegOffset = 0;
        LocalRegLoopCnt = SaveRestoreRegisters (MrcData, LoopControl->SaveDataControlData, LoopControl->DataSize, RegOffset, &McRegister, SaveType, Is32Bit, SkipPrint);
        TotalRegCnt += LocalRegLoopCnt;
        break;

      case PtMc:
        for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
          if (!MrcControllerExist (MrcData, Controller)) {
            continue;
          }
          MRC_DEBUG_MSG (Debug, DebugLevel, "\nMC%u:", Controller);
          RegOffset = INC_OFFSET_CALC_CH (MC0_MAD_INTER_CHANNEL_REG, MC1_MAD_INTER_CHANNEL_REG, Controller);
          LocalRegLoopCnt += SaveRestoreRegisters (MrcData, LoopControl->SaveDataControlData, LoopControl->DataSize, RegOffset, &McRegister, SaveType, Is32Bit, SkipPrint);
        } // for Controller
        TotalRegCnt += LocalRegLoopCnt;
        break;

      case PtCh:
        for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
          if (!MrcControllerExist (MrcData, Controller)) {
            continue;
          }
          for (Channel = 0; Channel < MAX_CHANNEL_SHARE_REGS; Channel++) {
            MRC_DEBUG_MSG (Debug, DebugLevel, "\nMC%u C%u:", Controller, Channel);
            RegOffset = INC_OFFSET_CALC_MC_CH (MC0_CH0_CR_WMM_READ_CONFIG_REG, MC1_CH0_CR_WMM_READ_CONFIG_REG, Controller, MC0_CH1_CR_WMM_READ_CONFIG_REG, Channel);
            LocalRegLoopCnt += SaveRestoreRegisters (MrcData, LoopControl->SaveDataControlData, LoopControl->DataSize, RegOffset, &McRegister, SaveType, Is32Bit, SkipPrint);
          } // for Channel
        } // for Controller
        TotalRegCnt += LocalRegLoopCnt;
        break;

      case PtByte:
        for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
          for (Channel = 0; Channel < MrcData->Outputs.MaxChannels; Channel++) {
            if (!MrcGetHwChannelExists (MrcData, Controller, Channel)) {
                continue;
            }
            for (Byte = 0; Byte < MrcData->Outputs.SdramCount; Byte++) {
              MRC_DEBUG_MSG (Debug, DebugLevel, "\nMC%u C%u B%u:", Controller, Channel, Byte);
              RegOffset = MrcGetDataOffset (MrcData, DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG, Controller, Channel, Byte) - DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG;
              LocalRegLoopCnt += SaveRestoreRegisters (MrcData, LoopControl->SaveDataControlData, LoopControl->DataSize, RegOffset, &McRegister, SaveType, Is32Bit, SkipPrint);
            } // for Byte
          } // for Controller
        } // for Channel
        TotalRegCnt += LocalRegLoopCnt;
        break;

      case PtCCC:
        for (PartIdx = 0; PartIdx < MRC_CCC_NUM; PartIdx++) {
          if (!(MrcGetHwPartitionExists (MrcData, PartitionCcc, PartIdx, MRC_IGNORE_ARG))) {
            continue;
          }
          MRC_DEBUG_MSG (Debug, DebugLevel, "\nCCC%u", PartIdx);
          RegOffset = INC_OFFSET_CALC_INDEX (CH0CCC_CR_DDRCRCLKCOMP_REG, CH1CCC_CR_DDRCRCLKCOMP_REG, PartIdx);
          LocalRegLoopCnt += SaveRestoreRegisters (MrcData, LoopControl->SaveDataControlData, LoopControl->DataSize, RegOffset, &McRegister, SaveType, Is32Bit, SkipPrint);
        } // for PartIdx
        TotalRegCnt += LocalRegLoopCnt;
        break;

      case PtCCCShared:
        for (PartIdx = 0; PartIdx < MRC_CCC_SHARED_NUM; PartIdx++) {
          if (!(MrcGetHwPartitionExists (MrcData, PartitionCccShared, PartIdx, MRC_IGNORE_ARG))) {
            continue;
          }
          MRC_DEBUG_MSG (Debug, DebugLevel, "\nCCCShared%u", PartIdx);
          RegOffset = INC_OFFSET_CALC_INDEX (DDRCCC_SHARED0_CR_DDRCRVCCCLKCOMPDATACCC_REG, DDRCCC_SHARED1_CR_DDRCRVCCCLKCOMPDATACCC_REG, PartIdx);
          LocalRegLoopCnt += SaveRestoreRegisters (MrcData, LoopControl->SaveDataControlData, LoopControl->DataSize, RegOffset, &McRegister, SaveType, Is32Bit, SkipPrint);
        } // for PartIdx
        TotalRegCnt += LocalRegLoopCnt;
        break;

      case PtDATAShared:
        for (PartIdx = 0; PartIdx < MRC_DATA_SHARED_NUM; PartIdx++) {
          if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, PartIdx, MRC_IGNORE_ARG))) {
            continue;
          }
          MRC_DEBUG_MSG (Debug, DebugLevel, "\nDATAShared%u", PartIdx);
          RegOffset = INC_OFFSET_CALC_INDEX (DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_REG, DDRDATA_SHARED1_CR_DDRCRDLLCONTROL1_REG, PartIdx);
          LocalRegLoopCnt += SaveRestoreRegisters (MrcData, LoopControl->SaveDataControlData, LoopControl->DataSize, RegOffset, &McRegister, SaveType, Is32Bit, SkipPrint);
        } // for PartIdx
        TotalRegCnt += LocalRegLoopCnt;
        break;

      case PtDATASbmem:
        for (PartIdx = 0; PartIdx < MRC_DATA_SHARED_NUM; PartIdx++) {
          if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, PartIdx, MRC_IGNORE_ARG))) {
            continue;
          }
          MRC_DEBUG_MSG (Debug, DebugLevel, "\nDATASbmem%u", PartIdx);
          RegOffset = INC_OFFSET_CALC_INDEX (DDRDATA_SBMEM0_CR_SPARE_REG, DDRDATA_SBMEM1_CR_SPARE_REG, PartIdx);
          LocalRegLoopCnt += SaveRestoreRegisters (MrcData, LoopControl->SaveDataControlData, LoopControl->DataSize, RegOffset, &McRegister, SaveType, Is32Bit, SkipPrint);
        } // for PartIdx
        TotalRegCnt += LocalRegLoopCnt;
        break;

      case PtVccClk:
        for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
          if (!MrcGetHwControllerExists (MrcData, Controller)) {
            continue;
          }
          MRC_DEBUG_MSG (Debug, DebugLevel, "\nVccClk%u:", Controller);
          RegOffset = INC_OFFSET_CALC_CH (DDRVCCCLK_SBMEM0_CR_SPARE_REG, DDRVCCCLK_SBMEM1_CR_SPARE_REG, Controller);
          LocalRegLoopCnt += SaveRestoreRegisters (MrcData, LoopControl->SaveDataControlData, LoopControl->DataSize, RegOffset, &McRegister, SaveType, Is32Bit, SkipPrint);
        } // for Controller
        TotalRegCnt += LocalRegLoopCnt;
        break;

      case PtPG:
        for (PartIdx = 0; PartIdx < MRC_PG_NUM; PartIdx++) {
          if (!(MrcGetHwPartitionExists (MrcData, PartitionPg, PartIdx, MRC_IGNORE_ARG))) {
            continue;
          }
          MRC_DEBUG_MSG (Debug, DebugLevel, "\nPG%u", PartIdx);
          RegOffset = INC_OFFSET_CALC_INDEX (DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_REG, DDRPGCR0_CR_LVRVOLTAGESENSOR_REG, PartIdx);
          LocalRegLoopCnt += SaveRestoreRegisters (MrcData, LoopControl->SaveDataControlData, LoopControl->DataSize, RegOffset, &McRegister, SaveType, Is32Bit, SkipPrint);
        } // for PartIdx
        TotalRegCnt += LocalRegLoopCnt;
        break;

      default:
        Status = mrcFail;
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "\nParition Type not valid!\n");
        break;
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n\t%s RegCnt : %04d, TotalRegCnt : %04d", LoopControl->ArrayName, LocalRegLoopCnt, TotalRegCnt);
  } // for Index

  // Final space check
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n%s section: %s %" PRIdPTR " bytes\n", IterTypeName, SaveTypeName, (McRegister - McRegisterStart) * 4);
  RegCountDifference = (UINT32) (McRegister - McRegisterStart);
  if (RegCountDifference > RegCountLimit) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "\nERROR: RegSaveRestore %s overflow by %d bytes!\n", IterTypeName, RegCountDifference);
    return mrcFail;
  }

  return Status;
}

/**
  This function saves any values that need to be used during non-cold boots.

  @param[in, out] MrcData - Include all the MRC global data.

  @retval mrcSuccess if the save occurred with no errors, otherwise returns an error code.
**/
MrcStatus
MrcSaveMCValues (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcSpd          *SpdIn;
  const MRC_FUNCTION    *MrcCall;
  MrcOutput             *Outputs;
  MrcDebug              *Debug;
  MrcSaveData           *SaveData;
  MrcSaveHeader         *SaveHeader;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcContSave           *ControllerSave;
  MrcChannelSave        *ChannelSave;
  MrcProfile            Profile;
  MrcStatus             Status;
  UINT8                 *SpdBegin;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  MrcSaveOrRestore      SaveRestoreType;  // @todo used for future simplification of single save/restore function call.
  BOOLEAN               SkipPrint;

  // Copy channel and DIMM information to the data area that will be saved.
  Inputs      = &MrcData->Inputs;
  MrcCall     = Inputs->Call.Func;
  Outputs     = &MrcData->Outputs;
  SaveData    = &MrcData->Save.Data;
  SaveHeader  = &MrcData->Save.Header;
  Debug       = &Outputs->Debug;

  SaveRestoreType  = MrcSaveEnum; // @todo used for future simplification of single save/restore function call. Filled from input parameter.
  SkipPrint        = TRUE;

  MrcFlushRegisterCachedData (MrcData);

  SaveData->Ratio = Outputs->Ratio;

  if (Outputs->SaGvPoint == MrcSaGvPoint0 || Outputs->FreqMax >= SaveData->Frequency[Outputs->SaGvPoint - 1]) {
    // Save Timing only when current frequency is higher than previous Sagv points.
    for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
      MrcCall->MrcCopyMem ((UINT8 *) &SaveData->Timing[Profile], (UINT8 *) &Outputs->Timing[Profile], sizeof (MrcTiming));
    }
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn                  = &Inputs->Controller[Controller];
    ControllerOut                 = &Outputs->Controller[Controller];
    ControllerSave                = &SaveData->Controller[Controller];
    ControllerSave->ChannelCount  = ControllerOut->ChannelCount;
    ControllerSave->Status        = ControllerOut->Status;
    ControllerSave->ValidChBitMask = ControllerOut->ValidChBitMask;
    ControllerSave->FirstPopCh     = ControllerOut->FirstPopCh;

    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelIn   = &ControllerIn->Channel[Channel];
      ChannelOut  = &ControllerOut->Channel[Channel];
      ChannelSave                   = &ControllerSave->Channel[Channel];
      ChannelSave->DimmCount        = ChannelOut->DimmCount;
      ChannelSave->ValidRankBitMask = ChannelOut->ValidRankBitMask;
      ChannelSave->ValidSubChBitMask= ChannelOut->ValidSubChBitMask;
      ChannelSave->ValidByteMask    = ChannelOut->ValidByteMask;
      ChannelSave->Status           = ChannelOut->Status;

      SaveData->IsMr10PdaEnabled |= (ChannelOut->IsMr10PdaEnabled);
      SaveData->IsMr11PdaEnabled |= (ChannelOut->IsMr11PdaEnabled);
      SaveData->IsMr48PdaEnabled |= (ChannelOut->IsMr48PdaEnabled);
      SaveData->MrXPdaDfeTap1Enabled |= (ChannelOut->MrXPdaDfeTap1Enabled);
      SaveData->MrXPdaDfeTap2Enabled |= (ChannelOut->MrXPdaDfeTap2Enabled);
      SaveData->MrXPdaDfeTap3Enabled |= (ChannelOut->MrXPdaDfeTap3Enabled);
      SaveData->MrXPdaDfeTap4Enabled |= (ChannelOut->MrXPdaDfeTap4Enabled);
      MrcCall->MrcCopyMem ((UINT8 *) ChannelSave->DqsMapCpu2Dram, (UINT8 *) ChannelIn->DqsMapCpu2Dram, sizeof (ChannelIn->DqsMapCpu2Dram));
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        MrcCall->MrcCopyMem ((UINT8 *) &ChannelSave->Dimm[Dimm], (UINT8 *) &ChannelOut->Dimm[Dimm], sizeof (MrcDimmOut));
        SpdIn = &ChannelIn->Dimm[Dimm].Spd.Data;
        if (Outputs->IsDdr5) {
          SpdBegin = (UINT8 *) &SpdIn->Ddr5.ManufactureInfo;
          ChannelSave->DimmSpdSave[Dimm].SpdDramDeviceType = SpdIn->Ddr5.Base.DramDeviceType.Data;
          ChannelSave->DimmSpdSave[Dimm].SpdModuleType = SpdIn->Ddr5.Base.ModuleType.Data;
          ChannelSave->DimmSpdSave[Dimm].SpdModuleMemoryBusWidth = SpdIn->Ddr5.ModuleCommon.ModuleMemoryBusWidth.Data;
          ChannelSave->DimmSpdSave[Dimm].CkdMfgID = SpdIn->Ddr5.ModuleCommon.ModuleSpecific.Unbuffered.DeviceInfoRegister.ManufacturerId.Data;
          ChannelSave->DimmSpdSave[Dimm].CkdDeviceRev = SpdIn->Ddr5.ModuleCommon.ModuleSpecific.Unbuffered.DeviceInfoRegister.DeviceRevision;
          ChannelSave->DimmSpdSave[Dimm].DramMfgID = SpdIn->Ddr5.ManufactureInfo.DramIdCode.Data;
        } else if (Outputs->IsLP5Camm2) {
          SpdBegin = (UINT8*)&SpdIn->JedecLpddr5.ManufactureInfo;
          ChannelSave->DimmSpdSave[Dimm].SpdDramDeviceType = SpdIn->JedecLpddr5.Base.DramDeviceType.Data;
          ChannelSave->DimmSpdSave[Dimm].SpdModuleType = SpdIn->JedecLpddr5.Base.ModuleType.Data;
          ChannelSave->DimmSpdSave[Dimm].SpdModuleMemoryBusWidth = SpdIn->JedecLpddr5.Base.SystemChannelBusWidth.Data;
          ChannelSave->DimmSpdSave[Dimm].DramMfgID = SpdIn->JedecLpddr5.ManufactureInfo.DramIdCode.Data;
        } else { // LP5
          SpdBegin = (UINT8 *) &SpdIn->Lpddr.ManufactureInfo;
          ChannelSave->DimmSpdSave[Dimm].SpdDramDeviceType = SpdIn->Lpddr.Base.DramDeviceType.Data;
          ChannelSave->DimmSpdSave[Dimm].SpdModuleType = SpdIn->Lpddr.Base.ModuleType.Data;
          ChannelSave->DimmSpdSave[Dimm].SpdModuleMemoryBusWidth = SpdIn->Lpddr.Base.ModuleMemoryBusWidth.Data;
          ChannelSave->DimmSpdSave[Dimm].DramMfgID = SpdIn->Lpddr.ManufactureInfo.DramIdCode.Data;
        }
        // Save just enough SPD information so it can be restored during non-cold boot.
        MrcCall->MrcCopyMem ((UINT8 *) &ChannelSave->DimmSpdSave[Dimm].SpdSave[0], SpdBegin, sizeof (ChannelSave->DimmSpdSave[Dimm].SpdSave));

      } // for Dimm
    } // for Channel
  } // for Controller

  MrcCall->MrcCopyMem ((UINT8 *) &SaveData->VddVoltage[0], (UINT8 *) &Outputs->VddVoltage[0], sizeof (Outputs->VddVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &SaveData->VddqVoltage[0], (UINT8 *) &Outputs->VddqVoltage[0], sizeof (Outputs->VddqVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &SaveData->VppVoltage[0], (UINT8 *) &Outputs->VppVoltage[0], sizeof (Outputs->VppVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &SaveData->MemFrequency[0], (UINT8 *) &Outputs->MemFrequency[0], sizeof (Outputs->MemFrequency));
  MrcCall->MrcCopyMem ((UINT8 *) &SaveData->CkdShift, (UINT8 *) &Outputs->CkdShift, sizeof (Outputs->CkdShift));

  SaveData->ValidRankMask  = Outputs->ValidRankMask;
  SaveData->ValidChBitMask = Outputs->ValidChBitMask;
  SaveData->ValidMcBitMask = Outputs->ValidMcBitMask;

  // Copy specified memory controller MMIO registers to the data area that will be saved.
  // Start with the common section.
  if ((MrcIsSaGvEnabled (MrcData)) && (Outputs->SaGvPoint != Outputs->SaGvLast)) {
     // If SA GV is enabled, only save the Common registers at the last point (currently High).
  } else {
    Status = SaveRestoreExecution (MrcData, StCommon, SaveRestoreType, SkipPrint);
    if (Status != mrcSuccess) {
      return Status;
    }
  } // if SAGV and Low point

  // SAGV Register Save
  Status = SaveRestoreExecution (MrcData, StSaGv, SaveRestoreType, SkipPrint);
  if (Status != mrcSuccess) {
    return Status;
  }

  MrcVersionGet (MrcData, &SaveData->Version);
  SaveData->McIpVersion.Data       = Inputs->McIpVersion.Data;
  SaveData->DdrIoIpVersion.Data    = Inputs->DdrIoIpVersion.Data;
  SaveData->Frequency[Outputs->SaGvPoint] = Outputs->Frequency;
  SaveData->BurstLength            = Outputs->BurstLength;
  SaveData->MemoryClock            = Outputs->MemoryClock;
  SaveData->EccSupport             = Outputs->EccSupport;
  SaveData->DdrType                = Outputs->DdrType;
  SaveData->IsLpddr                = Outputs->IsLpddr;
  SaveData->IsLpddr5               = Outputs->IsLpddr5;
  SaveData->IsDdr5                 = Outputs->IsDdr5;
  SaveData->IsLP5Camm2             = Outputs->IsLP5Camm2;
  SaveData->LpX                    = Outputs->LpX;
  SaveData->LpByteMode             = Outputs->LpByteMode;
  SaveData->EnhancedChannelMode    = Outputs->EnhancedChannelMode;
  SaveData->XmpProfileEnable       = Outputs->XmpProfileEnable;
  SaveData->XmpConfigWarning       = (SaveData->XmpConfigWarning || Outputs->XmpConfigWarning);
  SaveData->MaxChannels            = Outputs->MaxChannels;
  SaveData->FirstPopController     = Outputs->FirstPopController;
  SaveData->Vdd2Mv                 = Outputs->Vdd2Mv;
  SaveData->MeStolenSize           = Inputs->MeStolenSize;
  SaveData->ImrAlignment           = Inputs->ImrAlignment;
  SaveData->GearMode[Outputs->SaGvPoint] = Outputs->GearMode;
  SaveData->FreqMax                = Outputs->FreqMax;
  SaveData->MaxDqBits              = Outputs->MaxDqBits;
  SaveData->IsCkdSupported         = Outputs->IsCkdSupported;
  SaveData->PmaCceConfig           = (UINT8) Inputs->PmaCceConfig;
  SaveData->TmeEnable              = (UINT8) Inputs->TmeEnable;
  SaveData->Ibecc                  = (UINT8) Inputs->ExtInputs.Ptr->Ibecc;

  MrcCall->MrcCopyMem ((UINT8 *) &SaveData->OffsetKnobs, (UINT8 *) &Inputs->ExtInputs.Ptr->OffsetKnobs, sizeof (McRegOffsets));
  SaveData->SaMemCfgCrc = Inputs->SaMemCfgCrcForSave;
  SaveHeader->Crc       = MrcCalculateCrc32 ((UINT8 *) SaveData, sizeof (MrcSaveData));
  MrcData->Save.Size    = sizeof (MrcSave);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Saved data CRC = %xh\n", SaveHeader->Crc);

  return Status;
}

/**
  This function saves any updates to values that need to be used during non-cold boots.

  @param[in, out] MrcData - Include all the MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcUpdateSavedMCValues (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  MrcSaveData           *SaveData;
  MrcSaveHeader         *SaveHeader;
  MrcStatus             Status;

  Inputs      = &MrcData->Inputs;
  SaveData    = &MrcData->Save.Data;
  SaveHeader  = &MrcData->Save.Header;
  Status      = mrcSuccess;

  // In Fast Boot, MeStolenSize may have changed. This should be updated within Save Data structure.
  SaveData->MeStolenSize           = Inputs->MeStolenSize;
  SaveData->ImrAlignment           = Inputs->ImrAlignment;
  SaveData->SaMemCfgCrc = Inputs->SaMemCfgCrcForSave;
  SaveHeader->Crc       = MrcCalculateCrc32 ((UINT8 *) SaveData, sizeof (MrcSaveData));
  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Saved data CRC = %xh\n", SaveHeader->Crc);

  return Status;
}

/**
  This function copies the non-training information that needs to be restored
  from the 'save' data structure to the 'Output' data structure.

  @param[in, out] MrcData - include all the MRC global data.

  @retval mrcSuccess if the copy completed with no errors, otherwise returns an error code.
**/
MrcStatus
MrcRestoreNonTrainingValues (
  IN OUT MrcParameters *const MrcData
  )
{
  MRC_FUNCTION      *MrcCall;
  MrcInput          *Inputs;
  MrcControllerIn   *ControllerIn;
  MrcChannelIn      *ChannelIn;
  MrcSaveData       *SaveData;
  MrcContSave       *ControllerSave;
  MrcChannelSave    *ChannelSave;
  MrcDimmOut        *DimmSave;
  MrcOutput         *Outputs;
  MrcDebug          *Debug;
  MrcControllerOut  *ControllerOut;
  MrcChannelOut     *ChannelOut;
  MrcDimmOut        *DimmOut;
  MrcSpd            *SpdIn;
  UINT8             *SpdBegin;
  MrcProfile        Profile;
  MrcSaGvPoint      SaGvPoint;
  UINT8             Controller;
  UINT8             Channel;
  UINT8             Dimm;

  SaveData    = &MrcData->Save.Data;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;
  Inputs      = &MrcData->Inputs;
  MrcCall     = Inputs->Call.Func;
  SaGvPoint   = Outputs->SaGvPoint;

  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    MrcCall->MrcCopyMem ((UINT8 *) &Outputs->Timing[Profile], (UINT8 *) &SaveData->Timing[Profile], sizeof (MrcTiming));
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn                = &Inputs->Controller[Controller];
    ControllerSave              = &SaveData->Controller[Controller];
    ControllerOut               = &Outputs->Controller[Controller];
    ControllerOut->ChannelCount = ControllerSave->ChannelCount;
    ControllerOut->ValidChBitMask = ControllerSave->ValidChBitMask;
    ControllerOut->Status       = ControllerSave->Status;
    ControllerOut->FirstPopCh   = ControllerSave->FirstPopCh;
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelIn                     = &ControllerIn->Channel[Channel];
      ChannelSave                   = &ControllerSave->Channel[Channel];
      ChannelOut                    = &ControllerOut->Channel[Channel];
      ChannelOut->DimmCount         = ChannelSave->DimmCount;
      ChannelOut->ValidRankBitMask  = ChannelSave->ValidRankBitMask;
      ChannelOut->Status            = ChannelSave->Status;
      ChannelOut->ValidSubChBitMask = ChannelSave->ValidSubChBitMask;
      ChannelOut->ValidByteMask     = ChannelSave->ValidByteMask;

      if (Inputs->EnablePda) {
        ChannelOut->Mr3PdaEnabled    = TRUE;
        ChannelOut->Mr7PdaEnabled    = TRUE;
        Outputs->BitByteSwizzleFound = TRUE;
        ChannelOut->IsMr10PdaEnabled = SaveData->IsMr10PdaEnabled;
        ChannelOut->IsMr11PdaEnabled = SaveData->IsMr11PdaEnabled;
        ChannelOut->IsMr48PdaEnabled = SaveData->IsMr48PdaEnabled;
        ChannelOut->MrXPdaDfeTap1Enabled = SaveData->MrXPdaDfeTap1Enabled;
        ChannelOut->MrXPdaDfeTap2Enabled = SaveData->MrXPdaDfeTap2Enabled;
        ChannelOut->MrXPdaDfeTap3Enabled = SaveData->MrXPdaDfeTap3Enabled;
        ChannelOut->MrXPdaDfeTap4Enabled = SaveData->MrXPdaDfeTap4Enabled;
      }
      MrcCall->MrcCopyMem ((UINT8 *) ChannelIn->DqsMapCpu2Dram, (UINT8 *) ChannelSave->DqsMapCpu2Dram, sizeof (ChannelSave->DqsMapCpu2Dram));
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmSave = &ChannelSave->Dimm[Dimm];
        DimmOut  = &ChannelOut->Dimm[Dimm];
        if (DimmSave->Status == DIMM_PRESENT || DimmSave->Status == DIMM_DISABLED) {
          SpdIn   = &ChannelIn->Dimm[Dimm].Spd.Data;
          MrcCall->MrcCopyMem ((UINT8 *) DimmOut, (UINT8 *) DimmSave, sizeof (MrcDimmOut));
          if (SaveData->IsDdr5) {
            SpdBegin = (UINT8 *) &SpdIn->Ddr5.ManufactureInfo;
            SpdIn->Ddr5.Base.DramDeviceType.Data = ChannelSave->DimmSpdSave[Dimm].SpdDramDeviceType;
            SpdIn->Ddr5.Base.ModuleType.Data     = ChannelSave->DimmSpdSave[Dimm].SpdModuleType;
            SpdIn->Ddr5.ModuleCommon.ModuleMemoryBusWidth.Data = ChannelSave->DimmSpdSave[Dimm].SpdModuleMemoryBusWidth;
            SpdIn->Ddr5.ModuleCommon.ModuleSpecific.Unbuffered.DeviceInfoRegister.ManufacturerId.Data = ChannelSave->DimmSpdSave[Dimm].CkdMfgID;
            SpdIn->Ddr5.ModuleCommon.ModuleSpecific.Unbuffered.DeviceInfoRegister.DeviceRevision = ChannelSave->DimmSpdSave[Dimm].CkdDeviceRev;
            SpdIn->Ddr5.ManufactureInfo.DramIdCode.Data = ChannelSave->DimmSpdSave[Dimm].DramMfgID;
          } else if (SaveData->IsLP5Camm2) {
            SpdBegin = (UINT8*)&SpdIn->JedecLpddr5.ManufactureInfo;
            SpdIn->JedecLpddr5.Base.DramDeviceType.Data = ChannelSave->DimmSpdSave[Dimm].SpdDramDeviceType;
            SpdIn->JedecLpddr5.Base.ModuleType.Data = ChannelSave->DimmSpdSave[Dimm].SpdModuleType;
            SpdIn->JedecLpddr5.Base.SystemChannelBusWidth.Data = ChannelSave->DimmSpdSave[Dimm].SpdModuleMemoryBusWidth;
            SpdIn->JedecLpddr5.ManufactureInfo.DramIdCode.Data = ChannelSave->DimmSpdSave[Dimm].DramMfgID;
          } else { // LP5
            SpdBegin = (UINT8 *) &SpdIn->Lpddr.ManufactureInfo;
            SpdIn->Lpddr.Base.DramDeviceType.Data = ChannelSave->DimmSpdSave[Dimm].SpdDramDeviceType;
            SpdIn->Lpddr.Base.ModuleType.Data     = ChannelSave->DimmSpdSave[Dimm].SpdModuleType;
            SpdIn->Lpddr.Base.ModuleMemoryBusWidth.Data = ChannelSave->DimmSpdSave[Dimm].SpdModuleMemoryBusWidth;
            SpdIn->Lpddr.ManufactureInfo.DramIdCode.Data = ChannelSave->DimmSpdSave[Dimm].DramMfgID;
          }
          // Restore just enough SPD information so it can be passed out in the HOB.
          // If SAGV enabled, only do this on the last pass, due to LPDDR VendorId patching.
          if ((!MrcIsSaGvEnabled (MrcData)) || (SaGvPoint == Outputs->SaGvLast)) {
            MrcCall->MrcCopyMem (SpdBegin, (UINT8 *) &ChannelSave->DimmSpdSave[Dimm].SpdSave[0], sizeof (ChannelSave->DimmSpdSave[Dimm].SpdSave));
          }
        } else {
          DimmOut->Status = DimmSave->Status;
        }
      } // for Dimm
    } // for Channel
  } // for Controller

  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->VddVoltage[0], (UINT8 *)&SaveData->VddVoltage[0], sizeof (Outputs->VddVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->VddqVoltage[0], (UINT8 *)&SaveData->VddqVoltage[0], sizeof (Outputs->VddqVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->VppVoltage[0], (UINT8 *)&SaveData->VppVoltage[0], sizeof (Outputs->VppVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->MemFrequency[0], (UINT8 *)&SaveData->MemFrequency[0], sizeof (Outputs->MemFrequency));
  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->CkdShift, (UINT8 *) &SaveData->CkdShift, sizeof (Outputs->CkdShift));

  Outputs->ValidRankMask  = SaveData->ValidRankMask;
  Outputs->ValidChBitMask = SaveData->ValidChBitMask;
  Outputs->ValidMcBitMask = SaveData->ValidMcBitMask;
  Outputs->McChBitMask    = Outputs->ValidChBitMask;

// ------- IMPORTANT NOTE --------
// MeStolenSize should not be saved/restored (except on S3).  There is no rule stating that ME FW cannot request
// a different amount of ME UMA space from one boot to the next.  Also, if ME FW is updated/changed, the UMA
// Size requested from the previous version should not be restored.
//
  Inputs->DdrIoIpVersion.Data     = SaveData->DdrIoIpVersion.Data;
  Inputs->McIpVersion.Data        = SaveData->McIpVersion.Data;
  Outputs->FreqMax                = SaveData->FreqMax;
  Outputs->MemoryClock            = SaveData->MemoryClock;
  Outputs->BurstLength            = SaveData->BurstLength;
  Outputs->Ratio                  = SaveData->Ratio;
  Outputs->EccSupport             = SaveData->EccSupport;
  Outputs->DdrType                = SaveData->DdrType;
  Outputs->IsLpddr                = SaveData->IsLpddr;
  Outputs->IsLpddr5               = SaveData->IsLpddr5;
  Outputs->IsDdr5                 = SaveData->IsDdr5;
  Outputs->IsLP5Camm2             = SaveData->IsLP5Camm2;
  Outputs->EnhancedChannelMode    = SaveData->EnhancedChannelMode;
  Outputs->LpX                    = SaveData->LpX;
  Outputs->LpByteMode             = SaveData->LpByteMode;
  Outputs->XmpProfileEnable       = SaveData->XmpProfileEnable;
  Outputs->XmpConfigWarning       = SaveData->XmpConfigWarning;
  Outputs->MaxChannels            = SaveData->MaxChannels;
  Outputs->FirstPopController     = SaveData->FirstPopController;
  Outputs->Vdd2Mv                 = SaveData->Vdd2Mv;
  Outputs->GearMode               = SaveData->GearMode[SaGvPoint];
  Outputs->Frequency              = SaveData->Frequency[SaGvPoint];
  Outputs->MaxDqBits              = SaveData->MaxDqBits;
  Outputs->IsCkdSupported         = SaveData->IsCkdSupported;
  Outputs->HighFrequency          = Outputs->Frequency;
  if (Outputs->GearMode == 1) {
    Outputs->Gear4Ever = 1;
  }

  Outputs->MemoryClock      = ConvertFreq2Clock (MrcData, Outputs->Frequency);
  Outputs->Ratio            = MrcFrequencyToRatio (MrcData, Outputs->Frequency);
  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "SAGV point %u: Frequency=%u, tCK=%ufs, Ratio=%u, Gear%d, Outputs->RefClk=%d\n",
    SaGvPoint,
    Outputs->Frequency,
    Outputs->MemoryClock,
    Outputs->Ratio,
    Outputs->GearMode ? 4 : 2,
    REF_FREQ
  );

  Outputs->EctDone                  = TRUE;
  Outputs->LowFreqCsCmd2DSweepDone[SaGvPoint]  = TRUE;

  if(Inputs->BootMode == bmS3) {
    Inputs->MeStolenSize      = SaveData->MeStolenSize;
    Inputs->ImrAlignment      = SaveData->ImrAlignment;
  }

  return mrcSuccess;
}

/**
  This function restores host structure data for CmdPiCode/CtlPiCode and update cache for RcvEn, TxDqs, TxDq
  This is needed to do CmdT and CmdV margining in RMT on Fast Flow and for LP5 since we are using Low/POR frequency switch during JEDEC reset.

  @param[in] MrcData - Include all the MRC global data.

  @retval mrcSuccess if the memory controller write back completed with no errors, otherwise returns an error code.
**/
MrcStatus
MrcRestoreCCCHostStructure (
  IN     MrcParameters *const MrcData
  )
{
  UINT32                Controller;
  UINT32                Channel;
  UINT32                Byte;
  UINT32                Rank;
  UINT8                 CmdGroup;
  UINT8                 CmdGroupMax;
  INT64                 GetSetVal;
  MrcIntCmdTimingOut    *IntCmdTiming;
  MrcOutput             *Outputs;

  Outputs     = &MrcData->Outputs;
  CmdGroupMax = MrcGetCmdGroupMax (MrcData);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      IntCmdTiming = &Outputs->Controller[Controller].CmdTiming[Channel];
      // Restore CMD/CTL PI settings, for Margin Limit Check
      for (CmdGroup = 0; CmdGroup < CmdGroupMax; CmdGroup++) {
        MrcGetSetCcc (MrcData, Controller, Channel, MRC_IGNORE_ARG, CmdGroup, CmdGrpPi, ReadCached, &GetSetVal);
        IntCmdTiming->CmdPiCode[CmdGroup] = (UINT16) GetSetVal;
      }
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        MrcGetSetCcc (MrcData, Controller, Channel, Rank, 0, CtlGrpPi, ReadCached, &GetSetVal);
        IntCmdTiming->CtlPiCode[Rank] = (UINT16) GetSetVal;
        for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
          // Bring RcvEn,TxDqs,TxDq  to CR cache, we shift CmdT for Margin check
          MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, RecEnDelay, ReadCached, &GetSetVal);
          MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, TxDqsDelay, ReadCached, &GetSetVal);
          MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, TxDqDelay,  ReadCached, &GetSetVal);
        } // Byte
      } // for Rank
    } // for Channel
  } // for Controller
  return mrcSuccess;
}

/**
  This function writes the previously determined training values back to the memory controller.
  We also have SAGV flow for S3/Warm/Fast boot here.

  @param[in] MrcData - Include all the MRC global data.

  @retval mrcSuccess if the memory controller write back completed with no errors, otherwise returns an error code.
**/
MrcStatus
MrcRestoreTrainingValues (
  IN     MrcParameters *const MrcData
  )
{
  const MrcInput   *Inputs;
  MrcStatus        Status;
  BOOLEAN          SkipPrint;
  MrcSaveOrRestore SaveRestoreType;
  INT64            GetSetVal;
  INT64            GetSetSave;
  INT32            GetSetRegVal;

  Inputs           = &MrcData->Inputs;
  SkipPrint        = TRUE;
  SaveRestoreType  = MrcRestoreEnum;

  // @todo - RCOMP must be executed by MRC first.

  // First restore the SAGV section to restore NIL/IL
  // SaGv Register Restore
  Status = SaveRestoreExecution (MrcData, StSaGv, SaveRestoreType, SkipPrint);
  if (Status != mrcSuccess) {
    return Status;
  }

  // Common Register Restore
  Status = SaveRestoreExecution (MrcData, StCommon, SaveRestoreType, SkipPrint);
  if (Status != mrcSuccess) {
    return Status;
  }
  GetSetRegVal = MrcReadCR (MrcData, DDRPHY_DDRCOMP_CR_COMPBACKUP1_REG);
  MrcWriteCR (MrcData, DDRPHY_DDRCOMP_CR_COMPBACKUP1_REG, GetSetRegVal);

  // Invalidate CR cache, in case it got some registers cached before they were restored from MrcSave
  InitializeRegisterCache (MrcData);

  // Enable DCLK
  // dclk_enable lives in MC_INIT_STATE which is not part of the save \ restore tables
  GetSetVal = 1;
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableDclk, WriteCached, &GetSetVal);

  // Restore SRX_reset before DDRPHY asserts InitComplete on Warm/S3 flow
  if ((Inputs->BootMode == bmWarm) || (Inputs->BootMode == bmS3)) {
    MrcSelfRefreshExit (MrcData, TRUE);
  }


  // Restore Host structure data for CmdPiCode/CtlPiCode and update cache for RcvEn, TxDqs, TxDq
  // This is needed for MarginLimitCheck / RMT on Fast flow.
  if (Inputs->BootMode == bmFast && (Inputs->ExtInputs.Ptr->MarginLimitCheck != Margin_Check_Disable)) {
    MrcRestoreCCCHostStructure (MrcData);
  }

  // Sync CCC PI.
  MrcBlockTrainResetToggle (MrcData, FALSE);
  IoReset (MrcData);
  MrcBlockTrainResetToggle (MrcData, TRUE);

  // If WrDqDqsRetraining is enabled, WrRetrainOvrd is cleared during DdrIoFinalize
  // WrRetrainOvrd is required to be set before LastCR, else initcomplete will never assert
  // Save the WrRetrainOvrd, then set to 1, and restore after LastCR
  MrcGetSetNoScope (MrcData, GsmIocWrRetrainOvrd, ReadFromCache, &GetSetSave);
  MrcGetSetNoScope (MrcData, GsmIocWrRetrainOvrd, WriteCached, &GetSetVal);

  Status  = MrcSetLastCR (MrcData);
  if (Status != mrcSuccess) {
    return Status;
  }

  MrcGetSetNoScope (MrcData, GsmIocWrRetrainOvrd, WriteCached, &GetSetSave);

  // Clear CR Cache
  InitializeRegisterCache (MrcData);

  // Perform DLL reset, followed by an IoReset
  ToggleDllReset (MrcData, 1);
  MrcWait (MrcData, MRC_TIMER_1NS * 100);
  ToggleDllReset (MrcData, 0);
  MrcWait (MrcData, MRC_TIMER_1US);
  IoReset (MrcData);

  return mrcSuccess;
}

/**
  Calculates a CRC-32 of the specified data buffer.

  @param[in] Data     - Pointer to the data buffer.
  @param[in] DataSize - Size of the data buffer, in bytes.

  @retval The CRC-32 value.
**/
UINT32
MrcCalculateCrc32 (
  IN     const UINT8       *const Data,
  IN     const UINT32      DataSize
  )
{
  UINT32 i;
  UINT32 j;
  UINT32 crc;
  UINT32 CrcTable[256];

  crc = (UINT32) (-1);

  // Initialize the CRC base table.
  for (i = 0; i < 256; i++) {
    CrcTable[i] = i;
    for (j = 8; j > 0; j--) {
      CrcTable[i] = (CrcTable[i] & 1) ? (CrcTable[i] >> 1) ^ 0xEDB88320 : CrcTable[i] >> 1;
    }
  }
  // Calculate the CRC.
  for (i = 0; i < DataSize; i++) {
    crc = (crc >> 8) ^ CrcTable[(UINT8) crc ^ (Data)[i]];
  }

  return ~crc;
}
