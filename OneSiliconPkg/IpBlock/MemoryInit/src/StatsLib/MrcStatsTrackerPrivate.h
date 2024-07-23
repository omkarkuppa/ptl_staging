/** @file
  This file includes all the private/internal function definitions for the Stats Tracker API.
  This is primarily for the usage of these functions in the Unit Tests.

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


#ifndef MRC_STATS_TRACKER_PRIVATE_H_
#define MRC_STATS_TRACKER_PRIVATE_H_


#include "CMrcStartMemoryConfiguration.h"
#include "CMrcTypes.h"


// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER (MrcStatsTrackerPrivate)


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
  );


/**
  Convert the Key parameter into a value that is appropriate to use as the second subscript of the PerPhaseData
  array.  Return -1 to indicate that the key has no appropriate value.

  @param[in] Key Key to convert to array index.

  @retval -1 the provided key is out of range.
  @return Position of Key in the PhaseTrackedPosition.
**/
INT32
MrcStatsCustomKeyEnumToArrayIdx (
  IN     MRC_STATISTIC Key
  );


/**
  Check if phase is active.

  @param[in] MrcData Pointer to MRC global data.
  @param[in] Phase Phase to check if active/inactive.

  @retval TRUE Phase is active.
  @retval FALSE Phase is inactive (or invalid).
**/
BOOLEAN
MrcStatsIsPhaseActive (
  IN     MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE Phase
  );


/**
  Check if phase is valid.

  @param[in] Phase Phase to check.

  @retval TRUE Phase is valid.
  @retval FALSE Phase is invalid.
**/
BOOLEAN
MrcStatsIsPhaseValid (
  IN MRC_STATISTIC_PHASE Phase
  );


/**
  Print out all data for phase.

  @param[in,out] MrcData Pointer to MRC global data.
  @param[in] Phase Phase to print out data
  @param[in] TaskString String to use for phase when Phase is CALL_TABLE_PHASE, used for each entry in the
    call table(s).
**/
VOID
MrcStatsPrintPhaseStats (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE Phase,
  IN     char *TaskString
  );


/**
  Reset data for phase.

  @param[in,out] MrcData Pointer to MRC global data.
  @param[in] Phase Phase to reset.

**/
VOID
MrcStatsResetPhase (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_STATISTIC_PHASE Phase
  );

#endif // MRC_STATS_TRACKER_PRIVATE_H_
