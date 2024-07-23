/** @file
  This file contains the implementation for the Stats Tracker API.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "CMrcStatsTracker.h"
#include "MrcStatsTrackerPrivate.h" // for private functions
#include "CMrcStatsTrackerConfig.h"  // for MRC_ENABLE_STATS
#include "MrcDebugPrint.h"
#include "CMrcStartMemoryConfiguration.h"


// If MRC_ENABLE_STATS not defined, the Stats struct will be stripped from the MrcData
// causing compilation issues with the implementations of these functions
// Similarly, if MRC_DEBUG_PRINT is not defined the code cannot produce output, so compiling
// the code makes no sense
//
#if MRC_ENABLE_STATS && !MRC_DEBUG_PRINT
  #error Enabling stats without a method to display them makes no sense and wastes space
#endif
#if MRC_ENABLE_STATS && MRC_DEBUG_PRINT

// redefine all macros to a stringifying macro in this file
// allows generation of string name array
#undef KEY
#undef PHASE
#define KEY(x)    #x,
#define PHASE(x)  #x,

// initialize key/phase names from defines to align with the data structures
static const char *StatNames[] = {STATS_NON_TIMER KEY(TIMER_KEYS_MARKER) STATS_TIMER};
static const char *PhaseNames[] = {PHASES};


/**
  Convert the Key parameter into a value that is appropriate to use as the second subscript of the PerPhaseData
  array.  Return -1 to indicate that Key has no appropriate value.

  @param[in] Key Key to return array index.

  @retval -1 the provided key is out of range.
  @return Position of Key in the PhaseTrackedPosition.
**/
INT32
MrcStatsCustomKeyEnumToArrayIdx (
  IN     MRC_STATISTIC Key
  )
{
  INT32 Result;

  if (Key >= GLOBAL_PHASE_TIME + 1 && Key < KEY_COUNT) {
    Result = Key - GLOBAL_PHASE_TIME - 1;
  } else {
    Result = -1;
  }

  return Result;
}


/**
  Return indication of whether the stats infrastructure is properly initialized.  This uses a hueristic which has
  approximately a 1 in 4 billion chance of being wrong.

  @param[in] MrcData Pointer to MRC global data.

  @retval TRUE stats framework is most likely properly initialized.
  @retval FALSE stats framework is not properly initialized or has been corrupted.
*/
BOOLEAN
MrcStatsAreInitialized (
    IN MrcParameters *MrcData
    )
{
  return MrcData->Outputs.StatsTracker.Signature == STATS_SIGNATURE_INIT ||
         MrcData->Outputs.StatsTracker.Signature == STATS_SIGNATURE_START;
}


/**
  Check if phase is active.

  @param[in]     MrcData            - Pointer to MRC global data.
  @param[in]     Phase              - Phase to check if active/inactive.

  @retval TRUE if active.
  @retval FALSE Phase is inactive (or invalid).
**/
BOOLEAN
MrcStatsIsPhaseActive (
  IN     MrcParameters  *const MrcData,
  IN     MRC_STATISTIC_PHASE Phase
  )
{
  // this is a private function; we assume the caller has checked that the framework is properly initialized

  if (Phase < 0 || Phase >= ARRAY_COUNT (MrcData->Outputs.StatsTracker.PhaseActive)) {
    MRC_DEBUG_ASSERT (FALSE, &MrcData->Outputs.Debug, "Phase out of range");
    return FALSE;
  }

  return MrcData->Outputs.StatsTracker.PhaseActive[Phase];
}


/**
  Check if Phase is valid.

  @param[in] Phase Phase to check.

  @retval TRUE the phase is valid.
  @retval FALSE the phase is not valid.
**/
BOOLEAN
MrcStatsIsPhaseValid (
  IN MRC_STATISTIC_PHASE Phase
  )
{
  return ((Phase < PHASE_COUNT) &&
          (Phase >= 0));
}


/**
  Increments the data for given statistic in all currently active phases by amount specified.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Key The statistic to update for all active phases.
  @param[in] Value Amount to increment.

  @retval mrcSuccess Counts for stats were updated.
  @retval mrcFail No stats updated, Key is not valid or stats tracking is paused.
**/
MrcStatus
MrcStatsAddToData (
  IN OUT MrcParameters *const MrcData,
  IN MRC_STATISTIC Key,
  IN UINT32 Value
  )
{
  MrcStatsTracker   *StatsTracker;
  MrcOutput         *Outputs;
  UINT8              Phase;
  const INT32 IndexForKey = MrcStatsCustomKeyEnumToArrayIdx (Key);

  Outputs             = &MrcData->Outputs;
  StatsTracker        = &Outputs->StatsTracker;

  if (!MrcStatsAreInitialized (MrcData)) {
    return mrcFail;
  }
  if (StatsTracker->PauseStatsTracking) {
    return mrcFail;
  }
  if (IndexForKey < 0 || IndexForKey >= ARRAY_COUNT (StatsTracker->PerPhaseData[0])) {
    return mrcFail;
  }
  /* Increment key for all active phases.

     Special handling for end of phase (as signaled by Key == TIMER_KEYS_MARKER),
     just update PhaseTime so the value can be reported.  The value will be
     destroyed the next time any phase is finished, and there is no reason to keep the value around after it has
     been printed by MrcStatsPrintPhaseStats().

     The call flow is initiated by a call to MrcStatsEndPhase() with a phase as an argument.
         MrcStatsEndPhase() will call MrcStatsEndTimer() with the phase supplied
            MrcStatsEndTimer() finds the timer for the phase and determines the amount of elapsed time.
            MrcStatsEndTimer() calls this funtion (MrcStatsAddToData) with TIMER_KEYS_MARKER and the
              amount of elapsed time.  The special handling here stores the elapsed time in StatsTracker->PhaseTime
              and returns without any additional processing.
         MrcStatsEndPhase() calls MrcStatsPrintPhaseStats()
            MrcStatsPrintPhaseStats() uses the value of StatsTracker->PhaseTime to report the amount of time
              elapsed for the phase.
         MrcStatsEndPhase() does some othe processing and returns.
  */

  if (Key == TIMER_KEYS_MARKER) {
    StatsTracker->PhaseTime = Value;
    return mrcSuccess;
  }
  for (Phase = 0; Phase < TOTAL_PHASES; Phase++) {
    if (!MrcStatsIsPhaseActive (MrcData, Phase)) {
      continue;
    }

    StatsTracker->PerPhaseData[Phase][IndexForKey] += Value;
  }

  return mrcSuccess;
}


/**
  Increments the data for the statistic in all currently active phases by 1.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Key The statistic to update for all active phases.

  @return same as MrcStatsAddToData().
**/
MrcStatus
MrcStatsIncrementData (
  IN OUT MrcParameters *const MrcData,
  IN MRC_STATISTIC Key
  )
{
  // since this function just wraps another public interface, we will let the called function check that the
  // framework is properly initialized.
  return MrcStatsAddToData (MrcData, Key, 1);
}


/**
  Start a given phase by starting its timer and setting its active status to true.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Phase The phase to begin.
**/
void
MrcStatsBeginPhase (
  IN OUT MrcParameters *const MrcData,
  IN MRC_STATISTIC_PHASE Phase
  )
{
  MrcStatsTracker      *StatsTracker;
  MrcOutput            *Outputs;
  UINT32                PhaseKey;

  Outputs         = &MrcData->Outputs;
  StatsTracker    = &Outputs->StatsTracker;
  PhaseKey        = Phase; // avoid an to enum to enum conversion

  if (!MrcStatsAreInitialized (MrcData)) {
    return;
  }
  // check if phase valid and not already active
  if (StatsTracker->PauseStatsTracking ||
      !MrcStatsIsPhaseValid (Phase) ||
      MrcStatsIsPhaseActive (MrcData, Phase)) {
    return;
  }

  MrcStatsStartTimer (MrcData, PhaseKey);

  // set phase as active
  StatsTracker->PhaseActive[Phase] = TRUE;
}


/**
  Wrapper to start custom phase.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Phase The phase to start.
**/
void
MrcStatsBeginCustomPhase (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE  Phase
  )
{
  if (Phase == GLOBAL_PHASE || Phase == CALL_TABLE_PHASE || Phase == BETWEEN_CALL_TABLE) {
    return;
  }

  // Note MrcStatsBeginPhase() is expected to check that Phase is in range and that the stats framework appears
  // to be properly initialized; this expectation was confirmed when this comment was written.
  MrcStatsBeginPhase (MrcData, Phase);
}


/**
  Print out all data for phase.

  @param[in,out] MrcData Pointer to MRC global data.
  @param[in] Phase Phase to print out data.
  @param[in] TaskString String to use for phase when Phase is CALL_TABLE_PHASE, used for each entry in the
    call table(s).
**/
void
MrcStatsPrintPhaseStats (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE Phase,
  IN     char                *TaskString
  )
{
  UINT32 Index;
  UINT32 TimerKeysMarkerArrIdx;
  MrcStatsTracker *StatsTracker;
  const char * const PhaseStr = "Phase";
  const char * const ValueStr = "Value";
  const char * const TimeStr  = "Time";
  const char *AttributeStr;
  const char *SuffixStr;

  // since this is a private function, we assume that the caller has ensured that the stats framework is properly
  // initialized.
  StatsTracker = &MrcData->Outputs.StatsTracker;
  TimerKeysMarkerArrIdx = MrcStatsCustomKeyEnumToArrayIdx (TIMER_KEYS_MARKER);

  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_TIME, "<%s Name=\"%s\" %s=\"%u%s\">\n",
                                                           PhaseStr,
                                                           (Phase == CALL_TABLE_PHASE && TaskString != NULL) ? TaskString : PhaseNames[Phase],
                                                           TimeStr, UDIVIDEROUND (StatsTracker->PhaseTime, 1000), "ms");

  // Initial set of tags look like:                             <Stat Name="foo" Value="42"/>
  // After we hit the TimerKeysMarkerArrIdx the tags look like: <Stat Name="bar" Time="42uS"/>
  // Use AttributeStr to hold Value/Time string, and use SuffixStr to hold "" or "uS" and switch them at the right
  // time.
  AttributeStr = ValueStr;
  SuffixStr = "";
  for (Index = 0; Index < ARRAY_COUNT (StatsTracker->PerPhaseData[Phase]); Index++) {
    if (Index == TimerKeysMarkerArrIdx) {
      AttributeStr = TimeStr;
      SuffixStr = "uS";
      continue;
    }
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_TIME, "\t<Stat Name=\"%s\" %s=\"%u%s\"/>\n",
                                                              StatNames[Index], AttributeStr,
                                                              StatsTracker->PerPhaseData[Phase][Index],
                                                              SuffixStr);
  }
  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_TIME, "<%s/>\n", PhaseStr);
}


/**
  Reset data for phase.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Phase Phase to reset.
**/
void
MrcStatsResetPhase (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE Phase
  )
{
  const MRC_FUNCTION *MrcCall;
  MrcStatsTracker *StatsTracker;

  // since this is a private function we expect the caller to ensure the framework is properly initialized.

  MrcCall = MrcData->Inputs.Call.Func;
  StatsTracker = &MrcData->Outputs.StatsTracker;

  MrcCall->MrcSetMem ((UINT8 *) &StatsTracker->PerPhaseData[Phase][0], sizeof(StatsTracker->PerPhaseData[Phase]), 0);
}


/**
  Ends a given phase by ending its current timer, adding the accumulated time to to its total time,
  setting its active status to false, and printing the stats accumulated for the phase.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Phase The phase to end.
  @param[in] TaskString Pointer to task string from the call table.

  @retval mrcSuccess successfully ended the phase specified.
  @retval mrcFail tracking is paused, phase is not valid, or phase is not active.  No changes made to the stats
  framework.
**/
MrcStatus
MrcStatsEndPhase (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE  Phase,
  IN     char                 *TaskString
  )
{
  MrcOutput            *Outputs;
  MrcStatsTracker      *StatsTracker;
  UINT32               PhaseKey;
  MrcStatus Status;

  Outputs           = &MrcData->Outputs;
  StatsTracker      = &Outputs->StatsTracker;
  PhaseKey          = Phase;

  if (!MrcStatsAreInitialized (MrcData)) {
    return mrcFail;
  }
  // check if phase valid and not already inactive
  if (StatsTracker->PauseStatsTracking ||
      !MrcStatsIsPhaseValid (Phase) ||
      !MrcStatsIsPhaseActive (MrcData, Phase)) {
    return mrcFail;
  }

  // End the current timer for the phase, and fail if there is an issue as that would be an indicator
  // that someone had messed with the timer for the phase so the data would be invalid.
  // Error handling strategy is to report the issue, set the phase's time to 0, and to proceed normally.
  // This causes the phase to be marked inactive so that a later start of the phase will work.
  if (MrcStatsEndTimer (MrcData, PhaseKey) != mrcSuccess) {
    StatsTracker->PhaseTime = 0;
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s%s Failed to end timer (0x%x) when ending same phase\n",
                   gErrString, __func__, Phase);
    Status = mrcFail;
  } else {
    Status = mrcSuccess;
  }
  if ((Phase != BETWEEN_CALL_TABLE) || MRC_PRINT_BETWEEN_CALL_TABLE_PHASE) {
    MrcStatsPrintPhaseStats (MrcData, Phase, TaskString);
  }
  MrcStatsResetPhase (MrcData, Phase);
  StatsTracker->PhaseActive[Phase] = FALSE;

  return Status;
}


/**
  Wrapper to end custom phase.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Phase The phase to end.

  @retval mrcFail the specified phase is not a custom phase.
  @return return status from MrcStatsEndPhase().
**/
MrcStatus
MrcStatsEndCustomPhase (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE  Phase
  )
{
  if (Phase == GLOBAL_PHASE || Phase == CALL_TABLE_PHASE || Phase == BETWEEN_CALL_TABLE) {
    return mrcFail;
  }

  // Since this function just wraps another public function, let the called function check stats framework is properly
  // initialized.
  return MrcStatsEndPhase (MrcData, Phase, NULL);
}


/**
  Start timer for the specified statistic. The number of timers that can be active at any time is limited.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Key Key for timer to start.

  @retval mrcSuccess Successfully started the timer.
  @retval mrcFail repeated call without call to MrcStatsEndTimer(), stats tracking is paused,
          stats not properly initialized, or buffer is full.
**/
MrcStatus
MrcStatsStartTimer (
  IN OUT MrcParameters *const MrcData,
  IN MRC_STATISTIC Key
  )
{
  MrcOutput            *Outputs;
  MrcStatsTracker      *StatsTracker;
  UINT8                 Index;
  UINT8                 OpenSlotIdx = 0xFF;
  const MRC_FUNCTION   *MrcCall;

  Outputs           = &MrcData->Outputs;
  StatsTracker      = &Outputs->StatsTracker;
  MrcCall           = (&MrcData->Inputs)->Call.Func;

  if (!MrcStatsAreInitialized (MrcData)) {
    return mrcFail;
  }

  if (StatsTracker->PauseStatsTracking) {
    return mrcFail;
  }

  if ((Key & 0xff) != Key) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s%s Timer value (0x%x) does not fit in 8 bits\n",
                   gErrString, __func__, Key);
    return mrcFail;
  }

  // linearly search through the whole buffer to find the first open slot and to ensure that the timer for this key hasn't already been started
  for (Index = 0; Index < TOTAL_TIMERS; Index++) {
    if (StatsTracker->TimeStampBuffer[Index].TimeStartKey == KEY_NOT_TRACKED && OpenSlotIdx == 0xFF) {
      OpenSlotIdx = Index;
    }

    // has timer already been started for this key?
    if (StatsTracker->TimeStampBuffer[Index].TimeStartKey == Key) {
      return mrcFail;
    }
  }

  if (OpenSlotIdx != 0xFF) {
    StatsTracker->TimeStampBuffer[OpenSlotIdx].TimeStartKey = Key;
    StatsTracker->TimeStampBuffer[OpenSlotIdx].TimeStartValue = MrcCall->MrcGetCpuTimeUs (MrcData);
    return mrcSuccess;
  }

  return mrcFail;
}


/**
  End timer and add the elapsed time to the accumulated time for the timer in all currently active phases.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Key Timer to end.

  @retval mrcSuccess Succesfully ended timer.
  @retval mrcFail specified timer is not running, stats tracking is paused, or stats framework is not
          initialized.
**/
MrcStatus
MrcStatsEndTimer (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC        Key
  )
{
  MrcOutput            *Outputs;
  MrcStatsTracker      *StatsTracker;
  UINT8                 Index;
  UINT32                TimeElapsed;
  const MRC_FUNCTION   *MrcCall;

  Outputs           = &MrcData->Outputs;
  StatsTracker      = &Outputs->StatsTracker;
  MrcCall           = (&MrcData->Inputs)->Call.Func;

  if (!MrcStatsAreInitialized (MrcData)) {
    return mrcFail;
  }
  if (StatsTracker->PauseStatsTracking) {
    return mrcFail;
  }

  // linearly search for the matching Key
  for (Index = 0; Index < TOTAL_TIMERS; Index++) {
    if (StatsTracker->TimeStampBuffer[Index].TimeStartKey == (UINT8) Key) {
      // UINT32 max value in microseconds will hold up to 71 minutes
      TimeElapsed = MrcCall->MrcGetCpuTimeUs (MrcData) - StatsTracker->TimeStampBuffer[Index].TimeStartValue;

      if (Key <= GLOBAL_PHASE_TIME) {
        MrcStatsAddToData (MrcData, TIMER_KEYS_MARKER, TimeElapsed);
      } else {
        MrcStatsAddToData (MrcData, Key, TimeElapsed);
      }

      // reset struct
      StatsTracker->TimeStampBuffer[Index].TimeStartKey = KEY_NOT_TRACKED;
      StatsTracker->TimeStampBuffer[Index].TimeStartValue = 0;
      return mrcSuccess;
    }
  }
  return mrcFail;
}


/**
  Function wrapper for starting call table phase.

  @param[in, out] MrcData Pointer to MRC global data.
**/
void
MrcStatsBeginCallTablePhase (
  IN OUT MrcParameters *const MrcData
  )
{
  // Defer checking that framework is initialized to the called function since this is a wrapper calling
  // another public API.
  MrcStatsEndPhase (MrcData, BETWEEN_CALL_TABLE, NULL);
  MrcStatsBeginPhase (MrcData, CALL_TABLE_PHASE);
}


/**
  Function wrapper for ending call table phases.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] TaskString String indicating the call table entry just finished.
**/
void
MrcStatsEndCallTablePhase (
  IN OUT MrcParameters *const MrcData,
  IN     char                 *TaskString
  )
{
  // Defer checking that framework is initialized to the called function since this is a wrapper calling
  // another public API.
  if (MrcStatsEndPhase (MrcData, CALL_TABLE_PHASE, TaskString) != mrcSuccess) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s MrcStatsEndPhase(%s) failed\n",
                   gErrString, TaskString == NULL ? "<NULL>" : TaskString);
  }
  MrcStatsBeginPhase (MrcData, BETWEEN_CALL_TABLE);
}

/**
  Setup stats framework so that other functions can safely be called.
  This is used because there is not really a good spot to temporally wrap with the MrcStatsInit()
  and MrcStatsFinalize() functions and also be "wide" enough temporally that it will encompass all
  the code that might call any of the stats function.

  This function is expected to be called nearly as soon as the MrcData is setup since all the other
  stats functions require a pointer to MrcData.
  Using the MrcStatsInit function is also undesirable as it starts the global phase which would cause problems
  when MrcStatsInit was invoked as the wrapper for the main MRC code since the global phase would already
  be started.

  @param[out] MrcData Pointer to the MRC global data; used to initialize the stats related members.
*/
void
MrcStatsInitAndPause (
  OUT MrcParameters *const MrcData
  )
{
  MrcStatsTracker *StatsTracker;

  StatsTracker = &MrcData->Outputs.StatsTracker;

  if (MrcStatsAreInitialized (MrcData) && MrcStatsIsPhaseActive (MrcData, GLOBAL_PHASE)) {
    /* it appears that we are essentially in the state of running the global phase, so reinitializing is a bad
       idea.
    */
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR,
      "%s possible data loss, signature indicated init while global phase active\n", gWarnString);
  }

  // Initialize structure, cannot use any other functions as they might call back into stats framework before
  // everything is initialized.  Even if those functions do not call back into stats framework, not using those
  // functions removes an ordering dependency (function pointers must be setup before stats setup), ick! :-)
  for (int Phase = 0; Phase < ARRAY_COUNT (StatsTracker->PerPhaseData); Phase++) {
    for (int Stat = 0; Stat < ARRAY_COUNT (StatsTracker->PerPhaseData[0]); Stat++) {
      StatsTracker->PerPhaseData[Phase][Stat] = 0;
    }
  }

  // No need to initialize PhaseTime, it is only used to hold the elapsed time of a phase when the phase is ending

  for (int Timer = 0; Timer < ARRAY_COUNT (StatsTracker->TimeStampBuffer); Timer++) {
    StatsTracker->TimeStampBuffer[Timer].TimeStartKey = KEY_NOT_TRACKED;
    // no need to initialize ...TimeStampBuffer[Timer].TimeStart value because the value is only used if the timer
    // identifier matches the value in ...TimeStartKey, but that is KEY_NOT_TRACKED which is an invalid value.  The
    // array entry will be used when starting a timer, and that code will initialize ....TimeStart.
  }

  for (int Phase = 0; Phase < ARRAY_COUNT(StatsTracker->PhaseActive); Phase++) {
    StatsTracker->PhaseActive[Phase] = FALSE;
  }

  // no need to initialize PauseStatsTracking directly in this function,
  // call to MrcStatsPauseTracking() does that
  MrcStatsPauseTracking (MrcData);

  StatsTracker->Signature = STATS_SIGNATURE_INIT;
  // No need to initialize DWordAlignmentBuffer[], it exists solely to pad the structure to a 4 byte boundary
}


/**
  Initialize the data structures and start the global phase.

  @param[in, out] MrcData Pointer to MRC global data.

  @retval mrcSuccess Stats framework initialized.
**/
MrcStatus
MrcStatsInit (
  IN OUT MrcParameters  *const MrcData
  )
{
  // No need to check framework is initialized since this will initialize the framework.

  // Need to call MrcStatsInitAndPause() to ensure proper behavior if MrcStatsInit() is called more than
  // once without a reboot.
  MrcStatsInitAndPause (MrcData);
  MrcStatsContinueTracking (MrcData);
  MrcData->Outputs.StatsTracker.Signature = STATS_SIGNATURE_START;

  // start the global phase and inbetween call table phase
  MrcStatsBeginPhase (MrcData, GLOBAL_PHASE);
  MrcStatsBeginPhase (MrcData, BETWEEN_CALL_TABLE);

  return mrcSuccess;
}


/**
  Ends the global phase and checks if there are any active timers/phases.  If there are any active timers and/or
  phases, the code will log a warning.  This function makes no attempt to remedy these issues.  The expected usage
  model is that a human will read the output as there are no practical ways for code to remedy these issues in
  a manner that will always be desirable.

  @param[in, out] MrcData Pointer to MRC global data.
**/
void
MrcStatsFinalize (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcStatsTracker *StatsTracker;
  MrcOutput       *Outputs;
  MrcDebug        *Debug;
  UINT32           Index;
  UINT32           Key;
  static const char * const Msg =
    "%s %s %s is active at end of MRC.  Data will not be accurately reported.  Ensure the %s is properly ended.\n";
  static const char * const Timer = "Timer key";
  static const char * const Phase = "Phase";
  static const char * const FailedToEndPhase = "%s failed to end the %s\n";
  static const char * const GlobalPhase = "GLOBAL_PHASE";
  static const char * const BetweenCallTable = "BETWEEN_CALL_TABLE";

  Outputs      = &MrcData->Outputs;
  Debug        = &Outputs->Debug;
  StatsTracker = &Outputs->StatsTracker;


  if (!MrcStatsAreInitialized (MrcData)) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s: signature %x corrupt\n",
      gWarnString, __func__, StatsTracker->Signature);
    return;
  }

  /*  Assumption:  it is an error to start a call table phase and not end it.  One
      Side effect of stopping a call table phase is that the BETWEEN_CALL_TABLE phase
      is started.  So BETWEEN_CALL_TABLE should be active
  */
  if (MrcStatsEndPhase(MrcData, BETWEEN_CALL_TABLE, NULL) != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR,
        FailedToEndPhase,
        gWarnString, BetweenCallTable);
  }

  if (MrcStatsEndPhase(MrcData, GLOBAL_PHASE, NULL) != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR,
        FailedToEndPhase,
        gWarnString, GlobalPhase);
  }

  // check if there are still active keys in the time stamp buffer
  for (Index = 0; Index < TOTAL_TIMERS; Index++) {
    Key = StatsTracker->TimeStampBuffer[Index].TimeStartKey;
    if (Key >= ARRAY_COUNT (StatNames) && Key != KEY_NOT_TRACKED) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s key %d out of range 0..%llu\n",
        gErrString, Key, (long long unsigned int)ARRAY_COUNT (StatNames) - 1);
      // continue so we do not do something stupid if Key has an invalid value, this also ensures that all
      // potential issues are reported and the MrcStatsTracker is reset to indicate finalize has been called
      continue;
    }
    if (Key != KEY_NOT_TRACKED) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR,
        Msg, gWarnString, Timer, StatNames[Key], Timer);
    }
  }

  // check if there are still active phases
  for (Index = 0; Index < ARRAY_COUNT (StatsTracker->PhaseActive); Index++) {
    if (StatsTracker->PhaseActive[Index]) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR,
        Msg, gWarnString, Phase, PhaseNames[Index], Phase);
    }
  }

  StatsTracker->Signature = STATS_SIGNATURE_FINALIZED;
}


/**
  Turn off stats tracking until reenabled/continued.

  @param[in, out] MrcData Pointer to MRC global data.
**/
void
MrcStatsPauseTracking (
  IN OUT MrcParameters *const MrcData
  )
{
  if (!MrcStatsAreInitialized (MrcData)) {
    return;
  }
  MrcData->Outputs.StatsTracker.PauseStatsTracking = TRUE;
}


/**
  Turn on stats tracking until disabled/finalized.

  @param[in, out] MrcData Pointer to MRC global data.
**/
void
MrcStatsContinueTracking (
  IN OUT MrcParameters *const MrcData
  )
{
  if (!MrcStatsAreInitialized (MrcData)) {
    return;
  }
  MrcData->Outputs.StatsTracker.PauseStatsTracking = FALSE;
}

#endif // MRC_ENABLE_STATS && MRC_DEBUG_PRINT
