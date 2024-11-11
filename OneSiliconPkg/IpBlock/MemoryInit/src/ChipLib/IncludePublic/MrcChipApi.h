/** @file
  .

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _MrcChipApi_h_
#define _MrcChipApi_h_

#include "MrcMcApi.h"

///
/// Defines and Macros
///
#define MAX_MR_GEN_FSM                     (128)  ///< Maximum number of MR Addresses that can be sent
#define MAX_STORAGE_GEN_FSM                (40)  ///< Maximum number of Gen MRS FSM Storage Registers
#define MAX_TIMING_GEN_FSM                 (4)   ///< Maximum number of Gen MRS FSM Timing Registers
#define GEN_MRS_FSM_BYTE_MAX               (MAX_STORAGE_GEN_FSM * 4)  ///< 16 Registers * 4 Bytes per register

//IsCbt parameter defines
#define MRC_CBT     (TRUE)
#define MRC_NOT_CBT (FALSE)

// Separation required by hardware.
#define TX_FIFO_SEPARATION  (-2)

#define MAX_ADD_RANK_DELAY  (7)
#define MAX_ADD_DELAY       (7)
#define MAX_DEC_DELAY       (15)

#define GMF_TIMING_FIELDS_PER_REGISTER  (3)

///
/// Struct and Types
///

// Generic MRS FSM Mailbox Opcode Type - based on MC0_CH0_CR_MRS_CR_MAILBOX_STRUCT.Opcode
typedef enum {
  GmfMailboxRead  = 0,
  GmfMailboxWrite = 1
} GmfMailboxOpcode;

// Generic MRS FSM Selection Type - based on MC0_CH0_CR_MRS_CR_MAILBOX_STRUCT.Sel_Type
typedef enum {
  GmfMailboxTypeControl = 0,
  GmfMailboxTypeStorage = 1,
  GmfMailboxTypeTiming  = 2
} GmfMailboxType;

// Generic MRS FSM CommandTypes
// Based on MC0_CH0_CR_GENERIC_MRS_FSM_CONTROL_0_P0_STRUCT.COMMAND_TYPE
// defintion:
// MRW   :  3'b000
// MPC   :  3'b001
// VREF  :  3'b010
// REFab :  3'b011
// PREall:  3'b100
typedef enum {
  GmfCmdMrw  = 0,
  GmfCmdMpc  = 1,
  GmfCmdVref = 2,
  GmfCmdREFab = 3,
  GmfCmdPreAll = 4,
  GmfCmdIndexMax
} GmfCmdType;

// Delay Timing Indexes available to Generic MRS FSM.
typedef enum {
  GmfTimingIndex0,
  GmfTimingIndex1,
  GmfTimingIndex2,
  GmfTimingIndex3,
  GmfTimingIndex4,
  GmfTimingIndex5,
  GmfTimingIndex6,
  GmfTimingIndex7,
  GmfTimingIndex8,
  GmfTimingIndex9,
  GmfTimingIndex10,
  GmfTimingIndexMax,
} GmfTimingIndex;

// Delay timing types for Generic MRS FSM
typedef enum {
  GmfDdr5Delay_tMRD    = GmfTimingIndex0,
  GmfDdr5Delay_tZQCAL  = GmfTimingIndex1,
  GmfDdr5Delay_tZQLAT  = GmfTimingIndex2,
  GmfDdr5Delay_tDFE    = GmfTimingIndex3,
  GmfDdr5Delay_tVREFCA = GmfTimingIndex4,
  GmfDdr5Delay_tMRW    = GmfTimingIndex5,
  GmfDdr5Delay_tPreAll = GmfTimingIndex6
} GmfDdr5DelayType;

// Delay timing types for Generic MRS FSM
typedef enum {
  GmfLpddr5Delay_tMRW     = GmfTimingIndex0,
  GmfLpddr5Delay_tODTUP   = GmfTimingIndex1,
  GmfLpddr5Delay_tVREFCA  = GmfTimingIndex2,
  GmfLpddr5Delay_tFC      = GmfTimingIndex3,
  GmfLpddr5Delay_tVRCG    = GmfTimingIndex4,
  GmfLpddr5Delay_tZQRESET = GmfTimingIndex5,
  GmfLpddr5Delay_MR28     = GmfTimingIndex6,
  GmfLpddr5Delay_tZQLAT   = GmfTimingIndex7,
  GmfLpddr5Delay_tMRD     = GmfTimingIndex8,
  GmfLpddr5Delay_tREFab   = GmfTimingIndex9,
  GmfLpddr5Delay_tPreAll  = GmfTimingIndex10
} GmfLpddr5DelayType;
typedef union {
  struct {
    UINT32 ADDRESS                                 :  8;  // Bits 7:0
    UINT32 GENERIC_MRS_STORAGE_POINTER             :  9;  // Bits 16:8
    UINT32                                         :  3;  // Bits 19:17
    UINT32 COMMAND_TYPE                            :  3;  // Bits 22:20
    UINT32 TIMING_VALUE_POINTER                    :  4;  // Bits 26:23
    UINT32 PER_DEVICE                              :  1;  // Bits 27:27
    UINT32 FSP_CONTROL                             :  2;  // Bits 29:28
    UINT32 PER_RANK                                :  1;  // Bits 30:30
    UINT32 ACTIVE                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GENERIC_MRS_FSM_CONTROL_STRUCT;
typedef union {
  struct {
    UINT32 SHADOW_0                                  :  8;  // Bits 7:0
    UINT32 SHADOW_1                                  :  8;  // Bits 15:8
    UINT32 SHADOW_2                                  :  8;  // Bits 23:16
    UINT32 SHADOW_3                                  :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GENERIC_MRS_FSM_STORAGE_VALUES_STRUCT;
typedef union {
  struct {
    UINT32 TIMING_FIELD_0                          :  10;  // Bits 9:0
    UINT32 TIMING_FIELD_1                          :  10;  // Bits 19:10
    UINT32 TIMING_FIELD_2                          :  10;  // Bits 29:20
    UINT32                                         :  2;   // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GENERIC_MRS_FSM_TIMING_STORAGE_STRUCT;

typedef struct {
  GmfTimingIndex DelayIndex;  ///< The unique delay index
  UINT16         MrAddr;
  UINT8          MrData;
  BOOLEAN        Valid;
  BOOLEAN        FspWrToggle;     ///< Defines that this MR write should toggle the FSP state for WR
  BOOLEAN        FspOpToggle;     ///< Defines that this MR write should toggle the FSP state for OP
  BOOLEAN        FreqSwitchPoint; ///< Defines that after this MR, we should execute the Frequency switch.
  BOOLEAN        PdaMr;
  GmfCmdType     CmdType;   ///< Defines the type of command to output. The default (0) is MRW
} MRC_GEN_MRS_FSM_MR_TYPE;

/**
  This function uses MC0_CH0_CR_MRS_CR_MAILBOX Register to Access Generic MRS FSM Control, Timing or Value registers.

  @param[in]     MrcData            - Pointer to MRC global data.
  @param[in]     Controller         - Handle to the controller
  @param[in]     McCh               - Handle to channel within the memory controller
  @param[in]     Opcode             - 0 - Read register or 1 - Write register
  @param[in]     SelectionType      - Select type of MRS register to access: 00 - control registers or 01 - storage registers or 10 - timing registers
  @param[in]     SelectionIndex     - Select which register index to access in each of the MRS registers types: control, data and timing.
  @param[in]     Data               - In case field 'Opcode' is set to Write, on run, data in this field will be writen to the register mentioned in fields 'Sel_Type' and 'Sel_Index'

  @retval mrcSuccess, or mrcTimeout if FSM is not responding
**/
MrcStatus
GenericMrsFsmMailboxAccess (
  IN MrcParameters        *MrcData,
  IN UINT32               Controller,
  IN UINT32               McCh,
  IN GmfMailboxOpcode     Opcode,
  IN GmfMailboxType       SelectionType,
  IN UINT32               SelectionIndex,
  IN OUT UINT32           *Data
  );

/**
  This function initializes all the necessary registers for basic training by
  Activating and initializing CPGC Engine
  Enables CKE_On for CPGC usage

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcPreTraining (
  IN MrcParameters *const MrcData
  );


/**
  This function configures the Generic MRS FSM shadow registers based on the MrData inputs.
  It will determine if it needs to use the per-rank feature if the MR value differs across ranks.

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] MrData  - Pointer to an array of MR data to configure the MRS FSM with.
  @param[in] SagvConfig - TRUE if Configure the Generic FSM for SAGV transition
  @param[out] MrPerRankLookup - Optional mrEndOfSequence terminated array specifying MR addresses that must
  be sent per-rank, all other registers use the same data for all ranks on the
  same channel. If this pointer is NULL, then only MRs with different values in
  each rank are configued as per-rank Generic MRS FSM entries.

  @retval mrcSuccess if successful.
  @retval mrcFail if MrData pointer is null, the timing or per-rank registers are out of free entries.
**/
MrcStatus
MrcGenMrsFsmConfig (
  IN  MrcParameters *MrcData,
  IN  MRC_GEN_MRS_FSM_MR_TYPE MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM],
  IN  BOOLEAN                 SagvConfig,
  OUT const MrcModeRegister   *MrPerRankLookup OPTIONAL
  );

/**
  This function executes the MRS FSM and waits for the FSM to complete.
  If the FSM does not complete after 10 seconds, it will return an error message.

  @param[in] MrcData - Pointer to MRC global data.

  @retval mrcFail if the FSM is not idle.
  @retval mrcSuccess otherwise.
**/
MrcStatus
MrcGenMrsFsmRun (
  IN  MrcParameters *MrcData
  );

/**
  This function cleans the MRS FSM valid control on all Controllers Channels.

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] MrData  - Pointer to an array of MR data to configure the MRS FSM with.
  @param[in] CleanAll - If set to TRUE, MrData values will be ignored and all Control Registers will be cleared

  @retval mrcFail if clean failed.
  @retval mrcSuccess otherwise.
**/
MrcStatus
MrcGenMrsFsmClean (
  IN  MrcParameters *MrcData,
  IN  MRC_GEN_MRS_FSM_MR_TYPE MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM],
  IN  BOOLEAN       CleanAll
  );


/**
  This function configures the DDRCRCMDBUSTRAIN register to values for normal mode.

  @param[in]  MrcData - Pointer to global MRC data.

  @retval none.
**/
VOID
MrcSetWritePreamble (
  IN  MrcParameters *const  MrcData
  );

/**
  Returns the currently configured DRAM Command Intput Rate NMode

  @param[in] MrcData    - Include all MRC global data.

  @retval 1 = 1N Mode
  @retval 2 = 2N Mode
**/
UINT32
MrcGetNMode (
  IN MrcParameters *const MrcData
  );

/**
  Configure the MC to issue multicycle CS_n MPC commands.
  The DRAM must be configured separately by either setting
  DDR5 MR2.OP[4] = 0 or by resetting the DRAM.

  @param[in] MrcData    - Include all MRC global data.

  @retval None
**/
void
EnableMcMulticycleCs (
  IN MrcParameters *const MrcData
  );

/**
  Configure the MC and DRAM for single cycle CS_n MPC commands.
  An MRW is issued to the DRAM to configure DDR5 MR2[4] = 1.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] McOnly     - TRUE: program MC side only; FALSE: program both MC and DRAM.

  @retval None
**/
VOID
DisableMcMulticycleCs (
  IN MrcParameters *const MrcData,
  IN const BOOLEAN        McOnly
  );

/**
  Use MRS Burst FSM to program all relevant MR registers in one quick operation.
  Can only be used on ULT/ULX (up to 2 ranks, no ECC).
  Runs on all populated controllers, channels, and ranks.

  @param[in]  MrcData - Include all MRC global data.

  @retval mrcSuccess    - MRS FSM was executed successfully
  @retval mrcDeviceBusy - timed out waiting for MRH or MRS FSM
**/
MrcStatus
MrcProgramMrsFsm (
  IN MrcParameters *const MrcData
  );

/**
  This function implements the flow to properly switch MC PLL during run time.

  @params[in] MrcData     - Pointer to MRC global data.
  @params[in] NewFreq     - The new frequency to lock MC PLL.
  @params[in] DebugPrint  - Boolean parameter to enable/disable debug messages for the callee.

  @retval MrcStatus - mrcSuccess if frequency is updated properly, otherwise an error status.
**/
MrcStatus
MrcFrequencySwitch (
  IN  MrcParameters *const  MrcData,
  IN  MrcFrequency          NewFreq,
  IN  BOOLEAN               DebugPrint
  );

/**
  This function configures the SOC IP's to drive WCK for CBT mode based on the JEDEC Spec
  For Entering CBT:
    WCK will drive differentially low during IDLE
    WCK will toggle indefinitely when the pulse is started.
  For Exiting CBT:
    WCK will stop toggling.
    WCK will no longer drive drifferentially low.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] IsCbtEnter - CBT Enter/Exit have different WCK actions and HW will handle it accoringly to this parameter.
**/
VOID
MrcLpddrCbtWckMode (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              IsCbtEnter
  );

/**
  This function sets up MC, DDRIO and DRAM registers into CS 2N Gear Down.
  Will check if CS 2N Gear Down should be enabled prior to any register changes.

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if CS2N Switch was successful
**/
MrcStatus
Cs2NCheckAndSwitch (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function sets up MC, DDRIO and DRAM registers into CA 2N or 1N mode

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     IsEnable - Whether it is 2N (MRC_ENABLE) or 1N (MRC_DISABLE).

  @retval mrcSuccess setting Ca2N in the MC, DDRIO, and DRAM is successful
**/
MrcStatus
SetCa2N (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN              IsEnable
  );

/**
  Configure the DRAM for multi cycle CS_n MPC commands.
  An MRW is issued to the DRAM to configure DDR5 MR2[4] = 0.

  @param[in] MrcData  - Include all MRC global data.
**/
VOID
EnableDdr5MulticycleCs (
  IN MrcParameters *const MrcData
  );

/**
  This function sets up DRAM into CS Gear Down Mode

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if DRAM set up correctly, otherwise mrcFail.
**/
MrcStatus
Ddr5EnableCsGearDown (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function sets workpoint data for MC and PHY.

  @param[in, out] MrcData - MRC global data.

  @retval MrcStatus - mrcSuccess if workpoint data is set correctly, otherwise an error status.
**/
MrcStatus
SetWorkPointData (
  IN OUT MrcParameters *const MrcData
  );

/**
  Set up DQ Loopback mode and run margin tests

  @param[in] MrcData - Pointer to MRC global data.

  @retval mrcSuccess - In case of success
**/
MrcStatus
MrcDqLoopbackTest (
  IN MrcParameters *const MrcData
  );

#endif // _MrcChipApi_h_
