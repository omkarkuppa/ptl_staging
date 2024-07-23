/** @file
  This file contains the defines and enums that create the Stats Framework.

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

@par Specification
**/
#ifndef CMrcStatsTrackerFramework_h_
#define CMrcStatsTrackerFramework_h_

#include "CMrcTypes.h"  // for UINT8 (indirectly)
#include "CMrcStatsTrackerConfig.h"  // for PHASES


#define TOTAL_TIMERS                   (5) // increase if a greater number of simultaneous timers need to be kept active

/*
   Note for TOTAL_CUSTOM_KEYS: the #define represents the number of custom keys used for each phase in PerPhaseData.
        A partial ordering of the enums used in the expression are:
             ..., GLOBAL_PHASE_TIME, <contents of STATS_NON_TIMER>, TIMER_KEYS_MARKER,
            <contents of STATS_TIMER>, KEY_COUNT
   By way of example, if GLOBAL_PHASE_TIME = 10, and STATS_NON_TIMER has 4 elements,
   and STATS_TIMER has 2 elements, then elements of STATS_NON_TIMER have values 11 .. 14,
   TIMER_KEYS_MARKER = 15, elements of STATS_TIMER have values 16..17, and KEY_COUNT = 18.
   TOTAL_CUSTOM_KEYS will be (18 - 10 - 1) --> 7

   PerPhaseData[Phase][Stat] is referenced for clearing (non-interesting), updated in MrcStatsAddToData(), and
   printed in MrcStatsPrintPhaseStats().

   In MrcStatsAddToData, the second index ("Stat") is actually the result of calling
   MrcStatsCustomKeyEnumToArrayIdx() which converts the statistic (Key) by returning "Key - GLOBAL_PHASE_TIME -1"
   which corresponds to 0 .. 3 (for the example values in STATS_NON_TIMER), and 5..6 for the values in
   STATS_TIMER.

   The printing code iterates over index values 0..TOTAL_CUSTOM_KEYS, it skips (continue's) on the iteration
   corresponding to TIMER_KEYS_MARKER (as converted by MrcStatsCustomKeyEnumToArrayIdx()).
*/
#define TOTAL_CUSTOM_KEYS                   (KEY_COUNT - GLOBAL_PHASE_TIME - 1)
#define TOTAL_PHASES                        (PHASE_COUNT) // redudant like macro is for consistency in macro names "TOTAL_Xxx.."
#define TOTAL_KEYS                          (KEY_COUNT) // we'll count the TIMER_KEYS_MARKER as it is a global only key at the cost of 4 bytes

#define KEY_NOT_TRACKED                     (0xFFu) // for UINT8 memset and assignment - caution in changing this value.

// defines for Stats tracker lists
// since we are generating enums for both Keys and Phases by leveraging off one list of defines, we add a "_TIME" at the end of the enum name
// in order to create distinct enums.
#define KEY(x)                        x,
#define PHASE(x)                      x##_TIME, // create the timer keys from custom phases including the global phase timer


// generate enums sequentially from the following list of defines:
// 1) PHASES - user defined phases. Leveraging off this list to define timer keys for these phases.
//    a) The CALL_TABLE_PHASE is defined by default. At any given time there is at most only one active call table task.
//       The exception to the one active call table tasks is the transition between blue and green phases where the
//       line is blurred.
//       Each phase should be started and ended in conjunction with a call table task. When a phase is ended,
//       the data for it will be printed and then cleared.
// 2) STATS - defined in the config file, these are all the custom timer and non-timer keys
//
// Additionally,
// 1) TIMER_KEYS_MARKER - used to separate the timer and non-timer keys
// 2) KEY_COUNT - total number of keys
// 3) IGNORE_KEY - to be passed around to determine if the Key is to be ignored. Declared here to maintain the
//    MRC_STATISTIC type.
typedef enum {
  PHASES
  /* Custom Keys range start */
  STATS_NON_TIMER
  TIMER_KEYS_MARKER,
  STATS_TIMER
  /* Custom Keys range end */
  KEY_COUNT,
  IGNORE_KEY = 0xFF
} MRC_STATISTIC;

// redef as we no longer need the "TIME" string appended at the end
#undef PHASE
#define PHASE(x)                      x,

typedef enum {
  PHASES
  PHASE_COUNT
} MRC_STATISTIC_PHASE;

#pragma pack (push,1)
typedef struct {
  UINT8   TimeStartKey;
  UINT32  TimeStartValue;
} TimerData;

/*
    We want the MrcStatsTracker structure to be a multiple of 4 bytes to keep things aligned.
    The PerPhaseData array will always be a multiple of 4 bytes, so we can ignore it.
    Each TimerData takes 5 bytes of storage.
    Each PhaseActive array element takes 1 byte of storage
    The PauseStatsTracking member takes 1 byte of storage

    Without the padding (and ignoring the size of PerPhaseData), the size of the structure is:
           (5 * TOTAL_TIMERS + TOTAL_PHASES + 1)
    The smallest number of byte in excess of our desired size (multiple of 4-byte DWORDS) is
           (5 * TOTAL_TIMERS + TOTAL_PHASES + 1) % 4
    That means the smallest non-zero number of bytes to add to get to a multiple of 4-bytes is
      4 - ((5 * TOTAL_TIMERS + TOTAL_PHASES + 1) % 4)
    Since TOTAL phases references an enum value, we cannot optimize out the case where no padding is needed.  The
    pre-processing code does not know the value of the enum.
*/
#define DWORD_ALIGNMENT_BUFFER              (4 - ((TOTAL_PHASES + TOTAL_TIMERS + 1) % 4)) // account for all the extra UINT8s causing misalignment; +4 to avoid zero size array
typedef struct  {
  /// Per phase stats; first index should be item on "PHASES" list/#define.
  /// Second index corresponds to **mapped** value from a member of the STATS_NON_TIMER, STATS_TIMER lists/
  /// #define, and the value TIMER_KEYS_MARKER.
  ///
  /// Note well, the second index/subscript is a **mapped** value, not one of the values from the lists mentioned
  UINT32      PerPhaseData[TOTAL_PHASES][TOTAL_CUSTOM_KEYS];

  UINT32      PhaseTime;  ///< hold time spent in phase currently ending
  TimerData   TimeStampBuffer[TOTAL_TIMERS]; ///< buffer to hold active timers for Phase times or timer keys
  BOOLEAN     PhaseActive[TOTAL_PHASES]; ///< keep track which phases are active
  BOOLEAN     PauseStatsTracking;  ///< FALSE if stats tracking is enabled; TRUE if disabled
  // added the next few lines when something was going wrong in the green code; it seems that the problem was related
  // to out of date object files
  // We may decide this is something we want to remove.  However, code now uses this to detect potential double
  // initialization as well as finalize without initialization.
  UINT32      Signature;
  #define STATS_SIGNATURE_INIT 0x2b514c8fU  ///< random value used to indicate data initialized
  #define STATS_SIGNATURE_START 0x3a7ceb9U  ///< random value used to indicate started global phase
  #define STATS_SIGNATURE_FINALIZED 0x938b7190U ///< random value used to indicate finished global phase
  UINT8       DWordAlignmentBuffer[DWORD_ALIGNMENT_BUFFER]; ///< ensure struct is UINT32 aligned
} MrcStatsTracker;
#undef DWORD_ALIGNMENT_BUFFER // value no longer needed, remove it from the namespace

#pragma pack (pop)

#endif
