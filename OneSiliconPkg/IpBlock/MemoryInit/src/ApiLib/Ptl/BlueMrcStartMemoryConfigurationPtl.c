/** @file
  SI specific MRC call table definitions.

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

#include "CMrcStartMemoryConfiguration.h"
#include "MrcGeneral.h"
#include "MrcPostCodes.h"
#include "MrcMcConfiguration.h"
#include "MrcSaveRestore.h"
#include "BlueMrcSpdProcessingApi.h"  // for MrcSpdProcessingStatic()
#include "MrcChipApi.h"
#include "MrcChipRouting.h"
#include "MrcUcManagement.h"
#include "MrcPmaApi.h"
#include "MrcReset.h"
#include "MrcTurnAround.h"
#include "MrcWriteDqDqs.h"
#include "MrcPpr.h"
#include "MrcRowHammer.h"
#include "MrcSagv.h"
#include "MrcMemoryScrub.h"
#include "MrcBdat.h"

//
// Functions:
//
const CallTableEntry  MrcCallTable[] = {
  ///
  /// The functions are executed in the following order, as the policy flag dictates.
  /// Mrctask, post_code, OEM command, policy_flag, iteration, debug_string
  ///
  {MrcFastBootPermitted,            MRC_FAST_BOOT_PERMITTED,       OemFastBootPermitted,   1,                             MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Fast boot permitted", TrainedParamNoPrint)},
  {MrcRestoreNonTrainingValues,     MRC_RESTORE_NON_TRAINING,      OemRestoreNonTraining,  1,           MF_WARM | MF_S3 | MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Restore non-training values", TrainedParamNoPrint)},
#ifdef MRC_DEBUG_PRINT
  {MrcPrintInputParameters,         MRC_PRINT_INPUT_PARAMS,        OemPrintInputParameters,1, MF_COLD | MF_WARM         | MF_FAST | MF_GV_FIRST                           ,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Print input parameters", TrainedParamNoPrint)},
#endif // MRC_DEBUG_PRINT
  {MrcSpdProcessingStatic,          MRC_SPD_PROCESSING_STATIC,     OemSpdProcessingRun,    1, MF_COLD                             | MF_GV_FIRST                           ,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("SPD PROCESSING STATIC", TrainedParamNoPrint)},
  {MrcSpdProcessingCalc,            MRC_SPD_PROCESSING_CALC,       OemSpdProcessingRun,    1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("SPD PROCESSING CALCULATION", TrainedParamNoPrint)},
  {MrcSetOverrides,                 MRC_SET_OVERRIDES,             OemSetOverride,         1, MF_COLD | MF_WARM | MF_S3 | MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("MRC Auto Updates and Overrides", TrainedParamNoPrint)},
  {MrcMcCapability,                 MRC_MC_CAPABILITY,             OemMcCapability,        1, MF_COLD | MF_WARM | MF_S3 | MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("MC Capabilities", TrainedParamNoPrint)},
  {MrcPmaConfig,                    MRC_PMA_CONFIG,                OemPmaConfig,           1, MF_COLD | MF_WARM | MF_S3 | MF_FAST | MF_GV_FIRST                           ,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("PMA Configuration", TrainedParamNoPrint)},
  {MrcDdr5CkdConfig,                MRC_DDR5_CKD_CONFIG,           OemDdr5CkdConfig,       1, MF_COLD                   | MF_FAST | MF_GV_FIRST                           ,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("CKD Configuration", TrainedParamNoPrint)},
  {MrcWorkPointLock,                MRC_FREQ_LOCK,                 OemFrequencySet,        1, MF_COLD | MF_WARM | MF_S3 | MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Work Point Lock", TrainedParamNoPrint)},
  {MrcMcConfiguration,              MRC_MC_CONFIG,                 OemMcInitRun,           1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("MC Config", TrainedParamNoPrint)},
  {MrcRestoreTrainingValues,        MRC_RESTORE_TRAINING,          OemRestoreTraining,     1,           MF_WARM | MF_S3 | MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Restore Training Values", TrainedParamNoPrint)},
  {MrcPreTraining,                  MRC_PRE_TRAINING,              OemPreTraining,         1, MF_COLD | MF_WARM | MF_S3 | MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("CPGC Pre-Training", TrainedParamNoPrint)},
  {MrcUcReset,                      MRC_UC_RESET,                  OemUcReset,             1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("UC Reset", TrainedParamNoPrint)},
  {MrcUcLoadGreenCode,              MRC_UC_LOAD_GREEN_CODE,        OemUcLoadGreenCode,     1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("UC Load Green Code", TrainedParamNoPrint)},
  {MrcUcLoadGreenData,              MRC_UC_LOAD_GREEN_DATA,        OemUcLoadGreenData,     1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST | MF_SKIP_STATS,   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("UC Load Green Data", TrainedParamNoPrint)},
  {MrcUcExecuteGreen,               MRC_UC_EXECUTE_GREEN,          OemUcExecuteGreen,      1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST | MF_SKIP_STATS,   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("UC Execute Green MRC", TrainedParamNoPrint)},
  {MrcResetSequence,                MRC_RESET_SEQUENCE_BLUE,       OemMcResetRun,          1,                             MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("JEDEC RESET on Fast flow", TrainedParamNoPrint)},

  {MrcSelfRefreshExitPostDfi,       MRC_SELF_REFRESH_EXIT,         OemSelfRefreshExit,     1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Force Self Refresh Exit Post DFI Switch", TrainedParamNoPrint)},
  {MrcRoundTripMatch,               MRC_ROUNDTRIP_MATCH,           OemRoundtripMatch,      1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Roundtrip Match", TrainedParamNoPrint)},
  {MrcMcProgramDeswizzleRegisters,  MRC_MC_DESWIZZLE_REG,          OemMcDeswizzleRegisters,1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Program Mc Deswizzle Register", TrainedParamNoPrint)},
  {MrcTurnAroundTimingOptimization, MRC_TURN_AROUND,               OemTurnAroundTimes,     1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST | MF_RMT | MF_TST, MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Turn Around Timing Optimization", TrainedParamTurnAroundTiming)},
  {MrcWriteDqDqsReTraining,         MRC_WRITE_DQ_DQS_RETRAINING,   OemWriteDqDqsReTraining,1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Write DQ/DQS Retraining", TrainedParamNoPrint)},
  {MrcPostPackageRepairEnable,      MRC_PPR_ENABLE,                OemPprEnable,           1, MF_COLD                   | MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Post Package Repair", TrainedParamNoPrint)},
  {MrcRhPrevention,                 MRC_RH_PREVENTION,             OemMrcRhPrevention,     1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Row Hammer Prevention", TrainedParamNoPrint)},
  {MrcMcFinalize,                   MRC_MC_FINALIZE,               OemMrcMcFinalize,       1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("MRC MC Finalize", TrainedParamNoPrint)},
  {MrcSaGvFinal,                    MRC_MR_FILL,                   OemSagvFinalize,        1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("SAGV Finalization", TrainedParamNoPrint)},
  {MrcIbecc,                        MRC_IBECC,                     OemIbecc,               1, MF_COLD                                                         | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK ("MRC In-band ECC", TrainedParamNoPrint)},
  {MrcAliasCheck,                   MRC_ALIAS_CHECK,               OemAliasCheck,          1, MF_COLD                                                         | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("MRC Memory alias check", TrainedParamNoPrint)},
  {MrcEccClean,                     MRC_ECC_CLEAN_START,           OemHwMemInit,           1, MF_COLD                                                         | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("MRC Memory Scrubbing", TrainedParamNoPrint)},
  {MrcSaveMCValues,                 MRC_SAVE_MC_VALUES,            OemSaveMCValues,        1, MF_COLD                             | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Save MC Values", TrainedParamNoPrint)},
  {MrcNormalMode,                   MRC_NORMAL_MODE,               OemNormalMode,          1, MF_COLD                                                         | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Normal Operation", TrainedParamNoPrint)},
  {MrcUpdateSavedMCValues,          MRC_UPDATE_SAVE_MC_VALUES,     OemUpdateSaveMCValues,  1,                             MF_FAST                             | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Update MC Values in Fast flow", TrainedParamNoPrint)},
  {MrcIbecc,                        MRC_IBECC,                     OemIbecc,               1,           MF_WARM | MF_S3 | MF_FAST                             | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK ("MRC In-band ECC", TrainedParamNoPrint)},
  {MrcEccClean,                     MRC_ECC_CLEAN_START,           OemHwMemInit,           1,                             MF_FAST                             | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("MRC Memory Scrubbing", TrainedParamNoPrint)},
  {MrcMarginLimitCheck,             MRC_MARGIN_LIMIT_CHECK,        OemMarginLimitCheck,    1,                             MF_FAST                             | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Margin Limit Check on Fast flow", TrainedParamNoPrint)},
  {MrcNormalMode,                   MRC_NORMAL_MODE,               OemNormalMode,          1,                             MF_FAST                             | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Normal Operation For Fast flow", TrainedParamNoPrint)},
  {MrcEccClean,                     MRC_ECC_CLEAN_START,           OemHwMemInit,           1,           MF_WARM                                               | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("MRC Memory Scrubbing", TrainedParamNoPrint)},
  {MrcNormalMode,                   MRC_NORMAL_MODE,               OemNormalMode,          1,           MF_WARM | MF_S3                                       | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("Normal Operation For Warm / S3 flow", TrainedParamNoPrint)},
  {MrcSaGvSwitch,                   MRC_SA_GV_SWITCH,              OemSaGvSwitch,          1, MF_COLD | MF_WARM | MF_S3 | MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("MRC SA GV Switch", TrainedParamNoPrint)},
  {MrcFillBdatStructure,            MRC_FILL_BDAT_STRUCTURE,       OemMrcFillBdat,         1, MF_COLD                   | MF_FAST | MF_GV_FIRST | MF_GV_OTHER | MF_GV_LAST,                   MRC_ITERATION_MAX, MRC_DEBUG_PRINTS_PER_TASK("MRC Fill BDAT Structure", TrainedParamNoPrint)},
};

/**
  this function use by the OEM to do dedicated task during the MRC.

  @param[in] MrcData           - include all the MRC data
  @param[in] OemStatusCommand  - A command that indicates the task to perform.

  @retval The status of the task.
**/
MrcStatus
MrcInternalCheckPoint (
  IN MrcParameters        *MrcData,
  IN MrcOemStatusCommand  OemStatusCommand
  )
{
  MrcInput           *Inputs;
  MRC_FUNCTION       *MrcCall;
  MrcOutput          *Outputs;
  MrcStatus          Status;
  TrainingStepsEn    *TrainingEnables;
  TrainingStepsEn3   *TrainingEnables3;
  BOOLEAN            SimicsFlag;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  BOOLEAN            IsLpddr;

  Status            = mrcSuccess;
  Inputs            = &MrcData->Inputs;
  ExtInputs         = Inputs->ExtInputs.Ptr;
  MrcCall           = Inputs->Call.Func;
  Outputs           = &MrcData->Outputs;
  TrainingEnables   = &ExtInputs->TrainingEnables;
  TrainingEnables3  = &ExtInputs->TrainingEnables3;
  SimicsFlag        = (ExtInputs->SimicsFlag != 0);

  IsLpddr  = Outputs->IsLpddr;

  switch (OemStatusCommand) {
    case OemPmaConfig:
    case OemMcInitRun:
    case OemMbist:
      if (Inputs->IsDdrIoGen1Tc) {
        Status = mrcFail; // Skip this training step
      }
      break;

    case OemPreTraining:
      if (!TrainingEnables->PRETRAIN || Inputs->IsDdrIoGen1Tc) {
        Status = mrcFail; // Skip this training step
      }
      break;

    case OemTurnAroundTimes:
      if (!TrainingEnables->TAT) {
        Status = mrcFail; // Skip this training step
      }
      break;

    case OemAliasCheck:
      if (!TrainingEnables->ALIASCHK) {
        Status = mrcFail; // Skip this training step
      }
      break;

    case OemIbecc:
      if (ExtInputs->ReplicateSagv) {
        Status = mrcFail; // Skip this step if ReplicateSagv is enabled, call MrcIbecc directly from MrcSagvSwitch
      }
      break;

    case OemHwMemInit:
      if (SimicsFlag) {
        Status = mrcFail; // Don't run on SIMICS.
      }
      break;

    case OemWriteDqDqsReTraining:
      if (!TrainingEnables3->WRTRETRAIN || Outputs->IsLpddr5) {
        Status = mrcFail; // Skip this training step
      }
      break;

    case OemMrcRhPrevention:
      if (ExtInputs->TargetedRowRefreshMode >= TrrDisabled && ExtInputs->DramRfmMode >= DramRfmDisabled) {
        Status = mrcFail; // Skip this training step
      }
      break;

    case OemPprEnable:
      if (!Inputs->PprEnable || (Outputs->SaGvPoint != Outputs->SaGvPprPoint)) {
        Status = mrcFail;
      }
      break;

    case OemMcResetRun:
      if (!TrainingEnables3->JEDECRESET) {
        Status = mrcFail; // Skip this training step
      }
      // Blue MRC runs JEDEC reset / init on Fast flow only
      // We don't need to run it unless it's the last SAGV point or a PPR-enabled point
      if (Outputs->SaGvPoint != Outputs->SaGvLast) {
        if ((Inputs->PprEnable && (Outputs->SaGvPoint != Outputs->SaGvPprPoint)) || !Inputs->PprEnable) {
          Status = mrcFail; // Skip this training step
        }
      }
      break;

    case OemRoundtripMatch:
      if (!TrainingEnables3->ROUNDTRIPMATCH) {
        Status = mrcFail; // Skip this training step
      }
      break;

    case OemNormalMode:
      if (ExtInputs->ReplicateSagv) {
        Status = mrcFail; // It will be called from MrcSaGvSwitch directly
      }
      break;

    case OemDdr5CkdConfig:
      if (IsLpddr || !Outputs->IsCkdSupported) {
        Status = mrcFail; // Skip this training step
      }
      break;

    case OemMarginLimitCheck:
      if (ExtInputs->MarginLimitCheck == Margin_Check_Disable) {
        Status = mrcFail; // Skip this training step
      }
      break;

#ifndef MRC_SIMICS_XTENSA_SUPPORT
    case OemUcReset:          // Skip UC operations on Simics when not supported
    case OemUcLoadGreenCode:  // Skip UC operations on Simics when not supported
    case OemUcLoadGreenData:  // Skip UC operations on Simics when not supported
    case OemUcExecuteGreen:   // Skip UC execution on Simics when not supported
      if (SimicsFlag) {
        Status = mrcFail;
      }
      break;
#endif // MRC_SIMICS_XTENSA_SUPPORT

    default:
      break;
  }
  //
  // Allow Overriding the internal checkpoint decision.
  //
  MrcCall->MrcCheckpoint (MrcData, OemStatusCommand, &Status);
  return Status;
}

/**

@brief

  Get the number of call table entries.

  @retval The number of call table entries.

**/
UINT16
MrcGetCallTableCount (
  VOID
  )
{
  return ARRAY_COUNT (MrcCallTable);
}
