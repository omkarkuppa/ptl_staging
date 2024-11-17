/** @file
  This file contains code related to initializing and configuring the DDRIO Data Partition

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef MRC_DDRIO_H_
#define MRC_DDRIO_H_


#include "MrcDdrIoApi.h"  // for MRC_RX_DQS_DCC_SAVE


extern const UINT8 DDRCCC_ADJ_MAX_SREP_CNT_NIL[MAX_SYS_CHANNEL];
extern const UINT8 DDRCCC_ADJ_MAX_SREP_CNT_IL[MAX_SYS_CHANNEL];
extern const UINT8 DDRCCC_MIN_SREP_CNT[MAX_SYS_CHANNEL];
extern const INT8  DDRCCC_tCL4RXDQFIFORDEN_CH_SREP_IL[MAX_SYS_CHANNEL];
extern const INT8  DDRCCC_tCL4RXDQFIFORDEN_CH_SREP_NIL[MAX_SYS_CHANNEL];


/**
  This function setup DCC for POR operation

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcDccFinal (
  IN     MrcParameters *const MrcData
  );


/**
  Disable periodic DCC

  @param[in] MrcData - Pointer to MRC global data
**/
VOID
MrcDisablePeriodicDcc (
  IN MrcParameters *const MrcData
  );


/**
  Enable DLL use optimal phases for maintaining lock

  @param[in,out] MrcData - Include all MRC global data.
**/
void
DLLPhClkSelect (
  IN OUT MrcParameters *const MrcData
  );


/**
  Enable / Disable Phy Rx "always open" mode, allow it to get the continuous RX DQS although no RD commands are being sent.

  Also disable clock gating for proper ForceCompUpdate propagation.
  GsmIocDqsIdleModeDrvEn  DATA_CR_DDRCRDATACONTROL0.dqsidlemodedrven    = 0
  GsmIocInternalClocksOn  DATA_CR_DDRCRDATACONTROL0.InternalClocksOn    = 1
  GsmIocBiasPMCtrl        DATA_CR_DDRCRDATACONTROL0.BiasPMCtrl          = 0 (15ns delay after this one)
  GsmIocRxPiPwrDnDis      DATA_CR_DDRCRDATACONTROL5.RxPiOn              = 1
  GsmIocForceRxAmpOn      DATA_CR_DDRCRDATACONTROL0.ForceRxOn           = 1
  GsmIocForceRcvEnOn      DATA_CR_DDRCRDATACONTROL2.ForceRcvEnOn        = 1
  GsmIocForceOdtOn        DATA_CR_DDRCRDATACONTROL0.OdtForceQDrvEn      = 1
  GsmIocRptChRepClkOn     MCMISCS_WRITECFGCH[0..7].RptChRepClkOn        = 1
  GsmIocCompClkOn         DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL1.CompClkOn     = 1
  GsmIocDccClkGateDisable DDRSCRAM_CR_DDRMISCCONTROL1.dccclkgatedisable = 1

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      Enable        - Enable or disable Rx "always open" mode
  @param[in, out] RxDqsDccSave  - struct to save the original CR values when Enable is TRUE, or to restore the CR values from (when Enable = FALSE)
**/
void
SetPhyRxMode (
  IN OUT MrcParameters * const  MrcData,
  IN     BOOLEAN                Enable,
  IN OUT MRC_RX_DQS_DCC_SAVE    *RxDqsDccSave
  );


/**
  This function responsible for setting RX PI

  @param[in, out] MrcData - Include all MRC global data.
  @param[in] PiCode       - Linear delay (PI ticks) moves the RX DQS within the bitlane relative to the RX DQ signal.
  @param[in] Rank         - Rank to operate on.
  @param[in] Controller   - Controller to operate on.
  @param[in] Channel      - Channel to operate on.
  @param[in] Byte         - Byte to update the specified parameter.
**/
VOID
SetRxPI (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               PiCode,
  IN     UINT32               Rank,
  IN     UINT32               Controller,
  IN     UINT32               Channel,
  IN     UINT32               Byte
  );


/**
  This function toggles the LargeChangeReset bit for the requested DCC types (DATA/CLK/WCK) with a 10ns wait between setting and clearing
  as well as setting LargeChangeEnable for the requested DCC types.
  Will only target populated partitions (DATA/CLK/WCK) via GetSet multicast.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] DccTypeMask - DCC types Mask. BIT0 = DQ, BIT1 = DQS (Unused in this function), BIT2 = CLK, BIT3 = WCK
**/
VOID
MrcLargeChangeResetSetup (
  IN  MrcParameters *const MrcData,
  IN  UINT32  DccTypeMask
  );

/**
  Clear the top 32KB of Xtensa DRAM0 region.
  @param[in] MrcData - Include all MRC global data.
**/
VOID
BlueMrcClearXtensaDramTop (
  IN MrcParameters *const MrcData
  );

/**
  Set TerDcd Override Values

  @param[in] MrcData    - Include all MRC global data.
  @param[in]  Controller  - Memory controller in the processor socket (0-based).
  @param[in]  Channel     - DDR Channel Number within the memory controller (0-based).
  @param[in]  Byte        - DDR Byte (0-based).
  @param[in]  IsEnable    - TRUE if override values should be set

**/
VOID
MrcOverrideTerDcd (
  IN OUT MrcParameters* const MrcData,
  UINT8     Controller,
  UINT8     Channel,
  UINT8     Byte,
  BOOLEAN   IsEnable
  );

/**
  This function takes the offset to determine if a rank is populated.

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]      Offset  - Offset to be checked.

  @retval TRUE if rank is populated on associated offset
          FALSE if rank is not populated on associated offset

**/
BOOLEAN
MrcClockEnable (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32  Offset
  );

/**
  This function finds all the CKD ranks that are associated to the index 0 of Controller/Channel/Rank arrays
  @param[in, out] MrcData    - Include all MRC global data.
  @param[in, out] Controller - Stores the CKD ranks' Controller that are associated with index 0
  @param[in, out] Channel    - Stores the CKD ranks' Channel that are associated with index 0
  @param[in, out] Rank       - Stores the CKD ranks' Rank that are associated with index 0
  @param[out]     Loop       - Number of ranks that has the same CKD main clock
  
**/
VOID
MrcGetCkdRanks (
  IN OUT MrcParameters *const MrcData,
  IN OUT UINT32 Controller[MAX_CKD_PIN],
  IN OUT UINT32 Channel[MAX_CKD_PIN],
  IN OUT UINT32 Rank[MAX_CKD_PIN],
  OUT UINT32 *Loop 
  );
#endif // MRC_DDRIO_H_
