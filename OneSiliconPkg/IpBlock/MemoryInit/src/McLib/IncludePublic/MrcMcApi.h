/** @file
  Contains functions that are used outside of the MC Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef MRC_MC_API_H_
#define MRC_MC_API_H_

#include "CMrcTypes.h"  // for UINT32 (indirectly)
#include "MrcDdrCommon.h"  // for MrcZqType
#include "MrcDdrIoApi.h"

///
/// Defines
///
// Number of Scrambler Groups
#define MRC_NUM_SCRAM (4)

// MRR array index size
#define MRC_MRR_ARRAY_SIZE  (5)

#define RANK_0_MASK 0x1;
#define RANK_1_MASK 0x2;
#define RANK_0_1_MASK 0x3;
#define RANK_2_3_MASK 0xC;
#define RANK_0_1_2_MASK 0x7;
#define RANK_0_1_3_MASK 0xB;
#define RANK_0_2_3_MASK 0xD;
#define RANK_1_2_3_MASK 0xE;

// Based on MRH_GENERIC_COMMAND_0_0_0_MCHBAR
typedef union {
  struct {
    UINT32 CA_0 : 14;  // Bits 13:0
    UINT32 CA_1 : 14;  // Bits 27:14
    UINT32      :  4;
  } Ddr5;
  struct {
    UINT32 CA_0 : 7;  // Bits 6:0
    UINT32 CA_1 : 7;  // Bit 13:7
    UINT32 CA_2 : 7;  // Bit 20:14
    UINT32 CA_3 : 7;  // Bit 27:21
    UINT32      : 4;
  } Lpddr5;
  UINT32 Data;
} MRC_GEN_MRH_COMMAND;

typedef struct {
  INT32 TxRetrain;
  INT32 Mr4;
  INT32 SrxTxRetrain;
  INT32 SrxRead;
  INT32 MainGlbDrvGateDis;
} MRC_DDR5_CS_GEARDOWN_SAVE;

typedef enum {
  MrhCasWck2CkSyncOff,
  MrhCasWckFastSync,
  MrhCasOpcodeMax
} MrhCasOpcode;

/// MC Safe Modes
#define MC_SAFE_RESERVED                    (MRC_BIT0)
#define MC_SAFE_OPP_SR                      (MRC_BIT1)

/// Structure to store turnaround timings
typedef struct {
  UINT32 tRdRdsg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdRddg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrWrsg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrWrdg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdWrsg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdWrdg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrRdsg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrRddg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdRddr[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdRddd[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrWrdr[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrWrdd[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdWrdr[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdWrdd[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrRddr[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrRddd[MAX_CONTROLLER][MAX_CHANNEL];
} McTurnAroundTimings;


///
/// Functions
///
/**
  This function adjusts the relevant TAT when changing Read or Write ODT duration.
  TAT list rdOdt = { drrd2rd, ddrd2rd, drrd2wr, ddrd2wr, srrd2wr_sg, srrd2wr_dg };
  TAT list wrOdt = { drwr2wr, ddwr2wr, drwr2rd, ddwr2rd, srwr2rd_sg, srwr2rd_dg };

  @param[in, out] MrcData     - Include all MRC global data.
  @param[in]      Controller  - Controller to update.
  @param[in]      Channel     - Channel to update.
  @param[in]      OdtType     - rdodtd or wrodtd
  @param[in]      Offset      - signed offset
  @param[in]      UpdateHost  - Switch to update the host struct with the new value.

  @retval Nothing.
**/
void
MrcTatStretch (
  IN OUT MrcParameters *const MrcData,
  IN UINT32                   Controller,
  IN UINT32                   Channel,
  IN UINT8                    OdtType,
  IN INT8                     Offset,
  IN BOOLEAN                  UpdateHost
  );

/**
  This function initializes the Memory Controller: Scheduler, Timings, Address Decode,
  Odt Control, and refresh settings.

  @param[in, out] MrcData - MRC global data.

  @retval MrcStatus - mrcSuccess if successful or an error status.
**/
MrcStatus
MrcMcConfiguration (
  IN OUT MrcParameters *const MrcData
  );

/**
  Program DESWIZZLE_SCH0/1 registers for MR4 decoding on LP5

  @param[in] MrcData - The MRC global data.

  @retval MrcStatus - mrcSuccess if successful or an error status.
**/
MrcStatus
MrcMcProgramDeswizzleRegisters (
  IN MrcParameters *const MrcData
  );

/**
  Issue LPDDR5 or DDR5 MPC command using MRH (Mode Register Handler).

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - the controller to work on
  @param[in] Channel    - The channel to work on
  @param[in] Rank       - The rank to work on
  @param[in] Opcode     - MPC Opcode (LP5\DDR5: OP[7:0])
  @param[in] DebugPrint - When TRUE, will print debugging information

  @retval mrcSuccess    - MPC was sent successfully
  @retval mrcDeviceBusy - Timed out waiting for MRH
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcIssueMpc (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               Opcode,
  IN BOOLEAN              DebugPrint
);

/**
  Issue LPDDR & DDR5 MRW (Mode Register Write) command using MRH (Mode Register Handler).

  @param[in] MrcData      - Include all MRC global data.
  @param[in] Controller   - Controller to send MRW.
  @param[in] Channel      - the channel to work on
  @param[in] Rank         - the rank to work on
  @param[in] Address      - MRW address
  @param[in] Data         - MRW Data
  @param[in] DebugPrint   - when TRUE, will print debugging information

  @retval mrcSuccess    - MRW was sent successfully
  @retval mrcDeviceBusy - timed out waiting for MRH
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcIssueMrw (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               Address,
  IN UINT32               Data,
  IN BOOLEAN              DebugPrint
);

/**
  Issue LPDDR5 WS_FS/WS_OFF (Start/Stop WCK2CK Fast Sync operation) command using DRAM Command Register.

  @param[in] MrcData            - Include all MRC global data.
  @param[in] Controller         - Controller to send MRW.
  @param[in] Channel            - the channel to work on
  @param[in] Rank               - the rank to work on
  @param[in] IsFastSyncEnable   - MRC_DISABLE to disable WCK2CK Sync (send WS_OFF), MRC_ENABLE for enabling it (send WS_FS)
  @param[in] DebugPrint         - when TRUE, will print debugging information

  @retval mrcSuccess  - WS_FS/WS_CMD_OFF was sent successfully
  @retval mrcFail     - otherwise
**/
MrcStatus
MrcIssueWsMode (
  IN MrcParameters* const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN BOOLEAN              IsFastSyncEnable,
  IN BOOLEAN              DebugPrint
  );

/**
  Issue LPDDR & DDR5 Multicast MRW (Mode Register Write) command using DRAM Command Register

  @param[in] MrcData      - Include all MRC global data.
  @param[in] RankMask     - Bit mask of ranks to consider.
  @param[in] Address      - MRW address
  @param[in] Data         - MRW Data
  @param[in] DebugPrint   - when TRUE, will print debugging information

  @retval mrcSuccess    - MRW was sent successfully
  @retval mrcFail       - MRW was not sent successfully
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcIssueMrwMulticast (
  IN MrcParameters* const MrcData,
  IN UINT32               RankMask,
  IN UINT32               Address,
  IN UINT32               Data,
  IN BOOLEAN              DebugPrint
  );

/**
  Issue LPDDR MRR (Mode Register Read) command using MRH (Mode Register Handler).
  Use DQ mapping array to deswizzle the MR data.

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  Controller  - Controller to issue MRR.
  @param[in]  Channel     - the channel to work on
  @param[in]  Rank        - the rank to work on
  @param[in]  Address     - MRR address
  @param[out] Data        - MRR Data array per SDRAM device

  @retval mrcSuccess    - MRR was executed successfully
  @retval mrcDeviceBusy - timed out waiting for MRH
**/
MRC_IRAM0_FUNCTION
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcIssueMrr (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               Address,
  OUT UINT8               Data[MRC_MRR_ARRAY_SIZE]
);

/**
  Issue NOP command using MRH (Mode Register Handler).

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - the controller to work on
  @param[in] Channel    - The channel to work on
  @param[in] Rank       - The rank to work on
  @param[in] DebugPrint - When TRUE, will print debugging information

  @retval mrcSuccess               - NOP was sent successfully
  @retval mrcDeviceBusy            - Timed out waiting for MRH
  @retval mrcUnsupportedTechnology - The memory technology is not supported
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcIssueNop (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN BOOLEAN              DebugPrint
);

/**
  Issue VREF CA/CS command using MRH (Mode Register Handler).

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - the controller to work on
  @param[in] Channel    - The channel to work on
  @param[in] Rank       - The rank to work on
  @param[in] Opcode     - VREF CA/CS Opcode OP[7:0]
  @param[in] DebugPrint - When TRUE, will print debugging information

  @retval mrcSuccess               - VREF CA/CS was sent successfully
  @retval mrcDeviceBusy            - Timed out waiting for MRH
  @retval mrcUnsupportedTechnology - The memory technology is not supported
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcIssueVrefCmd (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               Opcode,
  IN BOOLEAN              DebugPrint
);

/**
  Issue CAS command using MRH (Mode Register Handler).

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - the controller to work on
  @param[in] Channel    - The channel to work on
  @param[in] Rank       - The rank to work on
  @param[in] Opcode     - MPC Opcode (OP[7:0])
  @param[in] DebugPrint - When TRUE, will print debugging information

  @retval mrcSuccess    - MPC was sent successfully
  @retval mrcDeviceBusy - Timed out waiting for MRH
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcIssueCas (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT8                OpcodeEnum,
  IN BOOLEAN              DebugPrint
);

/**
 This function issues the PDA Select ID MPC.  The device ID's selected to listen to following
 MR/CAVref/MPC commands will be defined by Index.
 This will wait tMPC_Delay after sending the PDA Select ID MPC such that the caller does not need to wait
 to send the next command. To enable normal Rank operation mode for MR/CAVref/MPC commands, the caller
 must issue the PDA Select ID MPC after finishing the MR/CAVref/MPC updates with a value of 15 in Index.

 @param[in]  MrcData    - Pointer to global MRC data.
 @param[in]  Controller - 0-Based index of the Controller to access.
 @param[in]  Channel    - 0-Based index of the Channel to access.
 @param[in]  Rank       - 0-Based index of the Rank to access.
 @param[in]  Index      - 0-Based number of the PDA ID to select. 15 enables all devices
 @param[in]  DebugPrint - When TRUE, will print debugging information

 @retval MrcStatus - mrcTimeout if the FSM does not complete after 1s.
 @retval MrcStatus - mrcSuccess if the MPC is sent successfuly.
 @retval MrcStatus - mrcFail for unexepected failures.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcPdaSelect (
  IN  MrcParameters * const MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT8                 Index,
  IN  BOOLEAN               DebugPrint
);

/**
  Executes Generic MRH Command

  @param[in] MrcData           - Pointer to global data
  @param[in] Controller        - Targeted controller
  @param[in] Channel           - Targeted channel
  @param[in] Rank              - Targeted rank
  @param[in] Ca_Bus            - Command bus instruction
  @param[in] SpidWrCmdOverride - Force WrCmd on SPID_cmd_type
  @param[in] TwoCycleCommand   - Determines whether command should be two cycles or not.

  @retval mrsStatus
**/
MrcStatus
MrcRunGenericMrh (
  IN MrcParameters *const        MrcData,
  IN UINT32                      Controller,
  IN UINT32                      Channel,
  IN UINT32                      Rank,
  IN MRC_GEN_MRH_COMMAND  const  Ca_Bus,
  IN BOOLEAN                     SpidWrCmdOverride,
  IN BOOLEAN                     TwoCycleCommand
  );

/**
  This function executes the requested ZQ command.
  This function will wait the appropriate wait time as specified in the JEDEC spec.
  ZQ is sent to all populated controllers, channels, and ranks.

  @param[in]  MrcData   - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSendZqLpddr5 (
  IN  MrcParameters* const  MrcData
  );

/**
  This function executes the requested ZQ command.
  This function will wait the appropriate wait time as specified in the JEDEC spec.
  ZQ is sent to all populated controllers, channels, and ranks.

  @param[in]  MrcData   - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSendZqDdr5 (
  IN  MrcParameters* const  MrcData
  );

/**
  Issue ZQ calibration command on all populated controller, channels and ranks.
  When done, wait appropriate delay depending on the ZQ type.

  @param[in] MrcData  - include all the MRC data

  @retval MrcStatus - mrcSuccess if passes, otherwise an error status
**/
MrcStatus
MrcIssueZQ (
  IN  MrcParameters* const  MrcData
  );

/**
  This function sets MC IP Version, Derivative, Segment, Stepping based on MC0_MC_IP_VERSION_REG.

  @param[in]  MrcData     - Include all MRC global data.

  @retval mrcSuccess if the IP Segment and IP Version is valid.  Otherwise mrcFail
**/
MrcStatus
MrcSetupMcIpInfo (
  OUT MrcParameters *const MrcData
  );


/**
  This function sets up MC set of registers for CS 1N or 2N modes

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     IsEnable - Whether it is 2N (MRC_ENABLE) or 1N (MRC_DISABLE).
**/
VOID
McCs2NEnableOrDisable (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN  IsEnable
  );

/**
  This function sets up MC set of registers for CA 1N or 2N modes

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     IsEnable - Whether it is 2N (MRC_ENABLE) or 1N (MRC_DISABLE).
**/
VOID
McCa2NEnableOrDisable (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN  IsEnable
  );

/**
  This function forces Self Refresh Entry (with Frequency change) due to CS 2N Gear Down

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess self refresh entry was successful
  @retval mrcFail Timeout exceeded when trying to go into self refresh
**/
MrcStatus
MrcCs2NSrefEntry (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function forces Self Refresh Exit

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess self refresh exit was successful.
**/
MrcStatus
MrcCs2NForceSrExit (
  IN MrcParameters *const MrcData
  );

/**
  This function saves and restores Write Retrain and MR4 Periodic Reads as well as Self Refresh Exit runs with them for CS 2N Gear Down Mode.
  During saving portion, both Write Retrain and MR4 Periodic Reads will be disabled.

  @param[in, out] MrcData           - Include all MRC global data.
  @param[in, out] SaveData          - Pointer to storage struct to keep original values.
  @param[in]      SaveOrRestore     - Selects between saving the values or restoring the values.
**/
void
SetMcForCs2nTransition (
  IN OUT MrcParameters *const MrcData,
  IN OUT MRC_DDR5_CS_GEARDOWN_SAVE *SaveData,
  IN     MrcSaveOrRestore     SaveOrRestore
  );


/**
  This function generates the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller    - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel       - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Dqs data group within the rank (0-based).
  @param[in]      Lane          - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex     - Index supporting multiple operating frequencies.
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Pointer to return the Register volatile mask.

  @retval MrcStatus
**/
MrcStatus
MrcGetMcTimingHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  );


/**
  This function returns the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller    - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel       - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Dqs data group within the rank (0-based).
  @param[in]      Lane          - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex     - Index supporting multiple operating frequencies.
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Pointer to return the Register volatile mask.

  @retval MrcStatus
**/
MrcStatus
MrcGetMcConfigHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  );


/**
  Function used to get the CR Offset for Memory Controller Timings.

  @param[in]  Group     - MC Timing Group being accessed.
  @param[in]  Socket    - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]  Controller - Memory controller in the processor socket (0-based).
  @param[in]  Channel    - DDR Channel Number within the memory controller (0-based).
  @param[in]  FreqIndex - Index supporting multiple operating frequencies. (Not used in Client CPU's)

  @retval CR Offset.
**/
UINT32
MrcGetMcTimingRegOffset (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Socket,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  );


/**
  This function returns the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller    - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel       - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Dqs data group within the rank (0-based).
  @param[in]      Lane          - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex     - Index supporting multiple operating frequencies.
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Pointer to return the Register volatile mask.

  @retval MrcStatus
**/
MrcStatus
MrcGetPmaHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  );


/**
  Function used to get the CR Offset for Memory Controller Configuration Settings.

  @param[in]  Group     - MC Timing Group being accessed.
  @param[in]  Socket    - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]  Controller - Memory controller in the processor socket (0-based).
  @param[in]  Channel    - DDR Channel Number within the memory controller (0-based).
  @param[in]  Rank       - Rank number within a channel (0-based).
  @param[in]  Strobe     - Strobe number within a channel (0-based).
  @param[in]  FreqIndex  - Index supporting multiple operating frequencies. (Not used in Client CPU's)

  @retval CR Offset.
**/
UINT32
MrcGetMcConfigRegOffset (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Socket,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Strobe,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  Function used to get the CR Offset for MEMSS PMA registers.

  @param[in]  Group     - PMA Group being accessed.

  @retval CR Offset.
**/
UINT32
MrcGetPmaRegOffset (
  IN  MrcParameters *MrcData,
  IN  GSM_GT        Group,
  IN  UINT32        FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  );


/**
  This function returns the offset for the MRS FSM Control register.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  Controller  - Controller index.
  @param[in]  Channel     - Channel index.

  @retval Register Offset.
**/
UINT32
MrcGetMrsFsmCtlOffset (
  IN  MrcParameters *MrcData,
  IN  UINT32        Controller,
  IN  UINT32        Channel
  );


/**
  This function returns the offset for MRS run FSM Control register.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  Controller  - Controller index.
  @param[in]  Channel     - Channel index.

  @retval Register Offset.
**/
UINT32
MrcGetMrsRunFsmOffset (
  IN  MrcParameters *MrcData,
  IN  UINT32        Controller,
  IN  UINT32        Channel
  );


/**
  This function returns MC Timing group limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetMcTimingGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  );


/**
  This function returns MC Config register limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetMcConfigGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  );


/**
  This function returns PMA register limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - PMA group to access.
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus
**/
MrcStatus
MrcGetPmaGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  );

/**
  This function sets workpoint data for MC.

  @param[in, out] MrcData - MRC global data.
**/
VOID
SetWorkPointDataForMc (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function calculates the Power Metering weights.

  @param[in]  MrcData     - Include all MRC global data.
  @param[out] ReadPower   - DDRPHY Read energy weight.
  @param[out] WritePower  - DDRPHY Write energy weight.
  @param[out] IdlePower   - DDRPHY Idle energy weight.
  @param[out] CkePower    - DDRPHY Cke energy weight.

  @retval Nothing.
**/
VOID MrcGetDdrPhyWeights (
  IN  MrcParameters   *const MrcData,
  OUT UINT32          *const ReadPower,
  OUT UINT32          *const WritePower,
  OUT UINT32          *const IdlePower,
  OUT UINT32          *const CkePower
  );

/**
  static_pma1 sequence - platform specific registers

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcStaticPma1PlatformSpecific (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function configures the Power Metering weights.

  @param[in out]  MrValue     - MR value.
  @param[in]      BusValue     - The filed bus value to be programmed.

  @retval Nothing.
**/
VOID
MrcSetBusCmdType (
  IN UINT32         *MrValue,
  IN UINT32         BusValue
  );

/**
  Issue PREA command using MPTU, on all populated ranks / channels

  @param[in] MrcData    - Include all MRC global data.

  @retval mrcSuccess    - if command was sent successfully
  @retval mrcFail       - if command was not sent successfully
**/
MrcStatus
MrcIssuePrechargeAll (
  IN MrcParameters* const MrcData
  );

/**
  This function implements Turn Around Timing Optimization.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - If it succeeds return mrcSuccess.
**/
extern
MrcStatus
MrcTurnAroundTimingOptimization (
  IN MrcParameters * const MrcData
  );

/**
  This function calculates the Turnaround timings based on training results and DRAM Spec

  @param[in] MrcData - Include all MRC global data.
  @param[in] IsMcInit - TRUE is calculating TAT values for Mc Init, FALSE if TAT Optimization

  @retval MrcStatus - If it succeeds return mrcSuccess.
**/
MrcStatus
SetTurnAroundTiming (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsMcInit
  );

#endif // MRC_MC_API_H_
