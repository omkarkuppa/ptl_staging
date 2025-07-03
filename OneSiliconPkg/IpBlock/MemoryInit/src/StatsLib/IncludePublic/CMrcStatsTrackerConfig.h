/** @file
  This file contains the defines for all custom keys and phases to configure the usage of the Stats API.

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

#ifndef CMRC_STATS_TRACKER_CONFIG_H_
#define CMRC_STATS_TRACKER_CONFIG_H_
// this header file should not be included from a .c source file.  It should be included indirectly via
// #include "CMrcStatsTrackerFramework.h" which defines KEY()


// Set MRC_ENABLE_STATS to a non-zero integer to request stats, set to the constant 0 to disable stats.
// The "request" to enable stats may be "denied" if the code does not have the ability to print messages.
// A request to disable stats will be honored.
//
// This scheme, as opposed to depending on the macro being defined or undefined, allows code constructs such as
// if (MRC_ENABLE_STATS && AnotherCondition()) {
//    code only needed when stats are enabled
// }
// The benefit of this construct is that the "code only needed when stats are enabled" is always compiled and checked
// for valid syntax.  A good optimizer will remove the code when MRC_ENABLE_STATS is zero since the code will
// never be executed.

// C99 standard states that if a preprocessor symbol is not defined, then it is replaced by 0 when processing #if
// constructs.  Some compilers raise this as a warning, and some configurations will raise the warning to an error.
// Here we explicitly set MRC_ENABLE_STATS to zero if it is not defined.

#include "CMrcInterfaceGlobalTypes.h"  // for MRC_DEBUG_PRINT

#if !defined(MRC_ENABLE_STATS) || !MRC_DEBUG_PRINT
  #undef  MRC_ENABLE_STATS
  #define MRC_ENABLE_STATS 0
#endif

// Do not print "Between Call Table" phase stats by default.
// Change this to 1 if needed.
#define MRC_PRINT_BETWEEN_CALL_TABLE_PHASE (0)


// define stats to be tracked
#define STATS_NON_TIMER            \
        /* Non-Timer Keys Begin */      \
        KEY(MRC_WAIT_NS)                \
        KEY(MRC_JEDEC_RESET_COUNT)      \
        KEY(MRC_BLUE_GET_SET_CALLS)     \
        KEY(MRC_GREEN_GET_SET_CALLS)    \
        KEY(MRC_READ_CR)                \
        KEY(MRC_WRITE_CR)               \
        KEY(MRC_CACHED_READ_CR)         \
        KEY(MRC_CACHED_WRITE_CR)

#define STATS_TIMER                \
        /* Timer Keys Begin */          \
        KEY(MRC_FORCE_RCOMP_TIME)       \
        KEY(MRC_MRH_TIME)               \
        KEY(MRC_MRS_TIME)               \
        KEY(MRC_CPGC_TIME)              \
        KEY(MRC_MTG_TIME)               \
        KEY(READ_CR_TIME)               \
        KEY(WRITE_CR_TIME)

// Define phases here (see big comment block below for issues about commenting out code here):
#define PHASES                                                                        \
        /*  Custom Phase Begin */                                                     \
        PHASE(CUSTOM_PHASE1) /* for unit tests, can be repurposed for development */  \
        PHASE(BETWEEN_CALL_TABLE)                                                     \
        /*  Call Table Task Phase */                                                  \
        PHASE(CALL_TABLE_PHASE)                                                       \
        /*  Global Phase     */                                                       \
        PHASE(GLOBAL_PHASE) // must be last, used as delimiter

#endif

#if 0
        Refer to the StatsFrameworkReadme.md file for the details of why we have
        to use specific commenting forms.

        To properly comment out one or more entries in the lists above, the /* */ form of comment
        must be used.  The // form of commenting is unlikely to work as you might expect.

        Here are a few examples of correctly commenting out items in the #define for PHASES.

        #define PHASES                           \
                /*  Custom Phase Begin */        \
                /* disable DEFAULT_PHASE1 and DEFAULT_PHASE_2 */ \
                /*
                PHASE(DEFAULT_PHASE_1)           \
                PHASE(DEFAULT_PHASE_2)           \
                */                               \
                /*  Call Table Task Phase */     \
                PHASE(CALL_TABLE_PHASE)          \
                /*  Global Phase     */          \
                PHASE(GLOBAL_PHASE)

        #define PHASES                           \
                /*  Custom Phase Begin */        \
                /* disable just DEFAULT_PHASE_2 */ \
                PHASE(DEFAULT_PHASE_1)           \
                /* PHASE(DEFAULT_PHASE_2) */     \
                /*  Call Table Task Phase */     \
                PHASE(CALL_TABLE_PHASE)          \
                /*  Global Phase     */          \
                PHASE(GLOBAL_PHASE)

#endif
