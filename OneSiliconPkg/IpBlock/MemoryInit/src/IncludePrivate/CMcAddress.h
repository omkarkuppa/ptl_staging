/** @file
  The contents of this file has all the memory controller register addresses
  and register bit fields for the MRC.

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
#ifndef __McAddress_h__
#define __McAddress_h__

#define MRC_IGNORE_ARG_8      (0xFF)
#define MRC_IGNORE_ARG_16     (0xFFFF)
#define MRC_IGNORE_ARG_16_2   (0xDEAD)
#define MRC_IGNORE_ARG        (0xDEADBEEF)
#define MRC_CHANNEL_MULTICAST (0xFFFFFFFF)
#define MRC_BYTE_MULTICAST    (0xFFFFFFFF)
#define SOCKET_0              (0)
#define CONTROLLER_0          (0)

#include "CMrcTypes.h"
#include "MrcRegister.h"
#include "MrcRegisterStruct.h"

#include "Pci000Tgl.h"
#include "MrcHalApi.h"


///
/// The following is a copy of M_PCU_CR_SSKPD_PCU_STRUCT, modified to add in the
/// definition of the scratch pad bit fields.
///
typedef union {
  struct {
    UINT64 Reserved0                   : 1;  // Bits 0:0
    UINT64 MRC_RUNNING                 : 1;  // Bits 1:1
    UINT64 MEM_BASICMEMORYTEST_FAIL    : 1;  // Bits 2:2
    UINT64 MrcDisableChannel           : 8;  // Bits 10:3
    UINT64 Reserved11to23              : 13; // Bits 23:11
    UINT64 LastIbeccOpMode             : 2;  // Bits 25:24
    UINT64 Reserved26to63              : 38; // Bits 63:26
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} M_PCU_CR_SSKPD_PCU_STRUCT;

#define SSKPD_PCU_SKPD_MRC_RUNNING              0x2ULL   // BIT1
#define SSKPD_PCU_SKPD_MEM_BASICMEMORYTEST_FAIL 0x4ULL   // BIT2

#define PCIE_CR_OPIO_RX_DLL_GLOBAL3_DMIBAR_REG  (0xB1C)

typedef union {
  struct {
    UINT32 Reserved           : 30; ///< Bits 29:0
    UINT32 select_vccio_level :  2; ///< Bits 31:30  0 = 0.85v, 1 = 0.95v
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIE_CR_OPIO_RX_DLL_GLOBAL3_STRUCT;

#define MAX_NUMBER_DQDB_CACHELINES           (MCHBAR_CH0_CR_REUT_CH_PAT_WDB_CL_RD_STATUS_WDB_Current_Read_Pointer_MAX + 1)
#define MAX_DQDB_INC_RATE_EXPONENTIAL_VAL    (42)
#define MAX_DQDB_INC_RATE_LINEAR_VAL         (1 + MCHBAR_CH0_CR_REUT_CH_PAT_WDB_CL_CTRL_WDB_Increment_Rate_MAX)
#define MAX_NUMBER_DQDB_UNISEQS              (MrcHalWdbUniseqIndexMax)
#define MAX_DQDB_UNISEQ_SEED_VAL             (MCHBAR_CH0_CR_REUT_CH_PAT_WDB_CL_MUX_PB_RD_0_Pattern_Buffer_MAX)
#define MAX_DQDB_UNISEQ_L_VAL                (MCHBAR_CH0_CR_REUT_CH_PAT_WDB_CL_MUX_LMN_L_counter_MAX)
#define MAX_DQDB_UNISEQ_M_VAL                (MCHBAR_CH0_CR_REUT_CH_PAT_WDB_CL_MUX_LMN_M_counter_MAX)
#define MAX_DQDB_UNISEQ_N_VAL                (MCHBAR_CH0_CR_REUT_CH_PAT_WDB_CL_MUX_LMN_N_counter_MAX)
#define MAX_DQDB_UNISEQ_SEED_RELOAD_RATE_VAL (MCHBAR_CH0_CR_REUT_CH_PAT_WDB_CL_MUX_CFG_Reload_LFSR_Seed_Rate_MAX)
#define MAX_DQDB_UNISEQ_SEED_SAVE_RATE_VAL   (MCHBAR_CH0_CR_REUT_CH_PAT_WDB_CL_MUX_CFG_Save_LFSR_Seed_Rate_MAX)
#define MAX_DQDB_INVERT_DC_SHIFT_RATE_EXPONENTIAL (MCHBAR_CH0_CR_REUT_CH_PAT_WDB_INV_Inv_or_DC_Shift_Rate_MAX)
#define MAX_NUMBER_CADB_ROWS                 (MCHBAR_CH0_CR_REUT_CH_PAT_CADB_WRITE_POINTER_CADB_Write_Pointer_MAX)
#define MAX_NUMBER_CADB_UNISEQS              (MrcHalCadbUniseqIndexMax)
#define MAX_CADB_UNISEQ_SEED_VAL             (MCSCHEDS_CR_REUT_CH_PAT_CADB_MUX_PB_0_Pattern_Buffer_MAX)
#define MAX_CADB_UNISEQ_L_VAL                (MCSCHEDS_CR_REUT_CH_PAT_CADB_CL_MUX_LMN_L_counter_MAX)
#define MAX_CADB_UNISEQ_M_VAL                (MCSCHEDS_CR_REUT_CH_PAT_CADB_CL_MUX_LMN_M_counter_MAX)
#define MAX_CADB_UNISEQ_N_VAL                (MCSCHEDS_CR_REUT_CH_PAT_CADB_CL_MUX_LMN_N_counter_MAX)
#define MAX_DUMMY_READ_L_VAL                 (MCDFXS_CR_REUT_CH_SEQ_DUMMYREAD_CTL_MCMAIN_0_L_counter_MAX)
#define MAX_DUMMY_READ_M_VAL                 (MCDFXS_CR_REUT_CH_SEQ_DUMMYREAD_CTL_MCMAIN_0_M_counter_MAX)
#define MAX_DUMMY_READ_N_VAL                 (MCDFXS_CR_REUT_CH_SEQ_DUMMYREAD_CTL_MCMAIN_0_N_Counter_MAX)
#define MAX_START_DELAY_VAL                  (MCDFXS_CR_REUT_CH_SEQ_CFG_MCMAIN_0_Start_Test_Delay_MAX)
#define IS_LOOP_COUNT_EXPONENTIAL            (FALSE)
#define MAX_LOOP_COUNT_VAL                   (0xFFFFFFFF) //  (2^32)-1
#define MAX_NUMBER_SUBSEQS                   (1 + MCDFXS_CR_REUT_CH_SEQ_CFG_MCMAIN_0_Subsequence_Start_Pointer_MAX)
#define MAX_BURST_LENGTH_EXPONENTIAL_VAL     (15)
#define MAX_BURST_LENGTH_LINEAR_VAL          (MCDFXS_CR_REUT_CH0_SUBSEQ_CTL_MCMAIN_0_Number_of_Cachelines_MAX)
#define MAX_INTER_SUBSEQ_WAIT_VAL            (MCDFXS_CR_REUT_CH0_SUBSEQ_CTL_MCMAIN_0_Subsequence_Wait_MAX)
#define MAX_OFFSET_ADDR_UPDATE_RATE_VAL      (MCDFXS_CR_REUT_CH0_SUBSEQ_OFFSET_CTL_MCMAIN_0_Offset_Address_Update_Rate_MAX)
#define MAX_ADDR_INVERT_RATE_VAL             (MCDFXS_CR_REUT_CH_SEQ_BASE_ADDR_ORDER_CARRY_INVERT_CTL_MCMAIN_0_Base_Address_Invert_Rate_MAX)
#define MAX_RANK_ADDR_INC_RATE_EXPONENT_VAL  (23)
#define MAX_RANK_ADDR_INC_RATE_LINEAR_VAL    (1 + MCDFXS_CR_REUT_CH_SEQ_BASE_ADDR_INC_CTL_MCMAIN_0_Rank_Base_Address_Update_Rate_MAX)
#define MIN_RANK_ADDR_INC_VAL                (0)
#define MAX_RANK_ADDR_INC_VAL                (MCDFXS_CR_REUT_CH_SEQ_BASE_ADDR_INC_CTL_MCMAIN_0_Rank_Base_Address_Increment_MAX)
#define MAX_BANK_ADDR_INC_RATE_EXPONENT_VAL  (23)
#define MAX_BANK_ADDR_INC_RATE_LINEAR_VAL    (1 + MCDFXS_CR_REUT_CH_SEQ_BASE_ADDR_INC_CTL_MCMAIN_0_Bank_Base_Address_Update_Rate_MAX)
#define MIN_BANK_ADDR_INC_VAL                (0)
#define MAX_BANK_ADDR_INC_VAL                (MCDFXS_CR_REUT_CH_SEQ_BASE_ADDR_INC_CTL_MCMAIN_0_Bank_Base_Address_Increment_MAX)
#define MAX_ROW_ADDR_INC_RATE_EXPONENT_VAL   (10)
#define MAX_ROW_ADDR_INC_RATE_LINEAR_VAL     (1 + MCDFXS_CR_REUT_CH_SEQ_BASE_ADDR_INC_CTL_MCMAIN_0_Row_Base_Address_Update_Rate_MAX)
#define MIN_ROW_ADDR_INC_VAL                 (0)
#define MAX_ROW_ADDR_INC_VAL                 (MCDFXS_CR_REUT_CH_SEQ_BASE_ADDR_INC_CTL_MCMAIN_0_Row_Base_Address_Increment_MAX)
#define MAX_COL_ADDR_INC_RATE_EXPONENT_VAL   (20)
#define MAX_COL_ADDR_INC_RATE_LINEAR_VAL     (1 + MCDFXS_CR_REUT_CH_SEQ_BASE_ADDR_INC_CTL_MCMAIN_0_Column_Base_Address_Update_Rate_MAX)
#define MIN_COL_ADDR_INC_VAL                 (0)
#define MAX_COL_ADDR_INC_VAL                 (MCDFXS_CR_REUT_CH_SEQ_BASE_ADDR_INC_CTL_MCMAIN_0_Column_Base_Address_Increment_MAX)
#define MAX_NUMBER_RANK_MAP_ENTRIES          (8)
#define MAX_NUMBER_BANK_MAP_ENTRIES          (16)
#define MAX_NUMBER_ROW_ADDR_SWIZZLE_ENTRIES  (6)
#define MAX_ROW_ADDR_SWIZZLE_VAL             (MCDFXS_CR_REUT_CH_SEQ_ROW_ADDR_SWIZZLE_LOWER_MCMAIN_0_Logical_to_Physical_Row0_Swizzle_MAX)
#define MAX_STOP_ON_NTH_ERROR_COUNT_VAL      (MCMNTS_CR_REUT_CH_ERR_CTL_Stop_on_Nth_Error_MAX)
#define MAX_NUMBER_ERROR_COUNTERS            (9)
#define MAX_SCRAMBLER_KEY_VAL                (DDRSCRAM_CR_DDRSCRAMBLECH0_ScramKey_MAX)
#define MAX_REFRESH_IDLE_TIMER_VAL           (MCDECS_CR_PM_SREF_CONFIG_MCMAIN_Idle_timer_MAX)
#define MAX_NUMBER_CADB_UNISEQ_SEED          (3)
#define MAX_NUMBER_CMD_PI_CODES              (2)

#endif // __McAddress_h__
