/** @file
  This file includes all the function definitions for the Stats Tracker API.

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

#ifndef CMRC_STATS_TRACKER_H_
#define CMRC_STATS_TRACKER_H_

#include "CMrcTypes.h" // for MrcStatus
#include "CMrcStatsTrackerConfig.h" // for MRC_ENABLE_STATS
#include "CMrcStatsTrackerFramework.h" // for MRC_STATISTIC
#include "CMrcInterface.h"  // for MrcParameters

#if !MRC_ENABLE_STATS

// provide stub/null/inline stats functions

static inline void
MrcStatsAddToData (
  IN OUT MrcParameters  *const MrcData,
  IN MRC_STATISTIC Key,
  IN UINT32 Value
  )
{
  return;
}


static inline void
MrcStatsIncrementData (
  IN OUT MrcParameters  *const MrcData,
  IN MRC_STATISTIC Key
  )
{
  return;
}

static inline void
MrcStatsBeginPhase (
  IN OUT MrcParameters  *const MrcData,
  IN MRC_STATISTIC_PHASE Phase
  )
{
  // this function intentionally left blank :-)
}

static inline void
MrcStatsBeginCustomPhase (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE  Phase
  )
{
  // this function intentionally left mostly blank :-)
}

static inline void
MrcStatsEndPhase (
  IN OUT MrcParameters  *const MrcData,
  IN MRC_STATISTIC_PHASE       Phase,
  IN     char                  *TaskString
  )
{
  return;
}

static inline void
MrcStatsEndCustomPhase (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE  Phase
  )
{
  return;
}

static inline void
MrcStatsBeginCallTablePhase (
  IN OUT MrcParameters *const MrcData
  )
{
  return;
}

static inline void
MrcStatsEndCallTablePhase (
  IN OUT MrcParameters *const MrcData,
  IN     char                 *TaskString
  )
{
  return;
}

static inline void
MrcStatsStartTimer (
  IN OUT MrcParameters *const MrcData,
  IN MRC_STATISTIC Key
  )
{
  return;
}


static inline void
MrcStatsEndTimer (
  IN OUT MrcParameters *const MrcData,
  IN MRC_STATISTIC Key
  )
{
  return;
}

static inline void
MrcStatsInitAndPause (
  OUT MrcParameters *const MrcData
  )
{
  return;
}

static inline void
MrcStatsInit (
  IN OUT MrcParameters  *const MrcData
  )
{
  return;
}

static inline void
MrcStatsFinalize (
  IN OUT MrcParameters *const MrcData
  )
{
  return;
}

static inline void
MrcStatsPauseTracking (
  IN OUT MrcParameters *const MrcData
  )
{
  return;
}

static inline void
MrcStatsContinueTracking (
  IN OUT MrcParameters *const MrcData
  )
{
  return;
}
#else // MRC_ENABLE_STATS
// Provide normal function declarations

/**
  Increments the data for given statistic in all currently active phases by amount specified.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Key The statistic to update for all active phases.
  @param[in] Value Amount to increment.
**/
void
MrcStatsAddToData (
  IN OUT MrcParameters  *const MrcData,
  IN MRC_STATISTIC Key,
  IN UINT32 Value
  );


/**
  Increments the data for the statistic in all currently active phases by 1.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Key The statistic to update for all active phases.
**/
void
MrcStatsIncrementData (
  IN OUT MrcParameters  *const MrcData,
  IN MRC_STATISTIC Key
  );


/**
  Start a given phase by starting its timer and setting its active status to true.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Phase The phase to begin.
**/
void
MrcStatsBeginPhase (
  IN OUT MrcParameters  *const MrcData,
  IN MRC_STATISTIC_PHASE Phase
  );

/**
  Wrapper to start custom phase.

  @param[in,out] MrcData Pointer to MRC global data.
  @param[in] Phase The phase to start.
**/
void
MrcStatsBeginCustomPhase (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE  Phase
  );


/**
  Ends a given phase by ending its current timer, adding the accumulated time to to its total time,
  setting its active status to false, and printing the stats accumulated for the phase.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Phase The phase to end.
  @param[in] TaskString Pointer to task string from the call table.
**/
void
MrcStatsEndPhase (
  IN OUT MrcParameters  *const MrcData,
  IN MRC_STATISTIC_PHASE       Phase,
  IN     char                  *TaskString
  );

/**
  Wrapper to end custom phase.

  @param[in,out] MrcData Pointer to MRC global data.
  @param[in] Phase The phase to end.
**/
void
MrcStatsEndCustomPhase (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE  Phase
  );

/**
  Function wrapper for starting call table phase.

  @param[in,out] MrcData Pointer to MRC global data.
**/
void
MrcStatsBeginCallTablePhase (
  IN OUT MrcParameters *const MrcData
  );


/**
  Function wrapper for ending call table phases.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] TaskString String indicating the call table entry just finished.
**/
void
MrcStatsEndCallTablePhase (
  IN OUT MrcParameters *const MrcData,
  IN     char                 *TaskString
  );


/**
  Start timer for the specified statistic. The number of timers that can be active at any time is limited.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Key Key for timer to start.
**/
void
MrcStatsStartTimer (
  IN OUT MrcParameters *const MrcData,
  IN MRC_STATISTIC Key
  );


/**
  End timer and add the elapsed time to the accumulated time for the timer in all currently active phases.

  @param[in, out] MrcData Pointer to MRC global data.
  @param[in] Key Timer to end.
**/
void
MrcStatsEndTimer (
  IN OUT MrcParameters *const MrcData,
  IN MRC_STATISTIC Key
  );


/**
  Setup stats framework so that other functions can safely be called.
  This is used because there is not really a good spot to temporaly wrap with the MrcStatsInit()
  and MrcStatsFinalize() functions and also be "wide" enough temporaly that it will encompass all
  the code that might call any of the stats function.

  This function is expected to be called nearly as soon as the MrcData is setup since all the other
  stats functions require a pointer to that.
  Using the MrcStatsInit function is also undesirable as it starts the global phase which would cause problems
  when MrcStatsInit was invoked as the wrapper for the main MRC code since the global phase would already
  be started.

  @param[out] MrcData Pointer to the MRC global data; used to initialize the stats related members.
*/
void
MrcStatsInitAndPause (
  OUT MrcParameters *const MrcData
  );


/**
  Initialize the data structures and start the global phase.

  @param[in, out] MrcData Pointer to MRC global data.
**/
void
MrcStatsInit (
  IN OUT MrcParameters  *const MrcData
  );


/**
  Ends the global phase and checks if there are any active timers/phases.  If there are any active timers and/or
  phases, the code will log a warning.  This function makes not attempt to remedy these issues.  The expected usage
  model is that a human will read the output as there are no practical ways for code to remedy these issues in
  a manner that will always be desirable.

  @param[in,out] MrcData Pointer to MRC global data.
**/
void
MrcStatsFinalize (
  IN OUT MrcParameters *const MrcData
  );


/**
  Turn off stats tracking until reenabled/continued.

  @param[in, out] MrcData Pointer to MRC global data.
**/
void
MrcStatsPauseTracking (
  IN OUT MrcParameters *const MrcData
  );


/**
  Turn on stats tracking until disabled/finalized.

  @param[in, out] MrcData Pointer to MRC global data.
**/
void
MrcStatsContinueTracking (
  IN OUT MrcParameters *const MrcData
  );

#endif  // !MRC_ENABLE_STATS

#endif // CMRC_STATS_TRACKER_H_
