/** @file
  This header file define the converged structure used by crashlog

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

@par Specification Reference:
**/

#ifndef _CONVERGED_STRUCT_H_
#define _CONVERGED_STRUCT_H_


#define CRASHLOG_MECH_LEGACY                          0      // Get crash log by using legacy mode
#define CRASHLOG_MECH_DESCRIPTOR_TABLE                1      // Get crash log by using descriptor table
#define CRASHLOG_MECH_DVSEC                           2      // Get crash log by DVSec information

#define CRASH_TYPE_CLIENT                             0
#define CRASH_TYPE_SERVER                             1

#define CRASHLOG_INVALID_CHECK_0                      0
#define CRASHLOG_INVALID_CHECK_DEAD_BEEF              0xdeadbeef

#pragma pack(1)
//-----------------------------------------------------
// structure of converged Crashlog record header
//-----------------------------------------------------
//
// Common crash record header
//
typedef struct {
  UINT32    Revision   : 8;  // Revision
  UINT32    HeaderType : 4;  // Crash record header type
  UINT32    ProductId  : 12; // Product ID
  UINT32    CrashType  : 6;  // Crash record type
  UINT32    Rsvd       : 1;  // Reserved
  UINT32    Consumed   : 1;  // Consumed
} CRASH_RECORD_VERSION;

typedef struct {
  CRASH_RECORD_VERSION    Version;
  UINT32                  Size;     // Crashlog size
} COMMON_CRASH_RECORD_HEADER;

//
// Crash record header type 1
//
typedef union {
  struct {
    COMMON_CRASH_RECORD_HEADER    Header;
  } Field;
  UINT32    Data32[2];
} CRASH_RECORD_HEADER_TYPE1;

//
// Crash record header type 2
//
typedef union {
  struct {
    COMMON_CRASH_RECORD_HEADER    Header;
    UINT32                        TimeStamp1;
    UINT32                        TimeStamp2;
    UINT32                        AgentVersion;
    UINT32                        Reason;
  } Field;
  UINT32    Data32[6];
} CRASH_RECORD_HEADER_TYPE2;

//
// Crash record header type 3
//
typedef union {
  struct {
    COMMON_CRASH_RECORD_HEADER    Header;
    UINT32                        TimeStamp1;
    UINT32                        TimeStamp2;
    UINT32                        AgentVersion;
    UINT32                        Reason;
    UINT32                       CompletionStatus;
  } Field;
  UINT32    Data32[7];
} CRASH_RECORD_HEADER_TYPE3;


//-----------------------------------------------------------
// Crashlog converged structure phase 1
//-----------------------------------------------------------
typedef struct {
  // Capability
  UINT32    Avail           : 1;        ///< CrashLog feature supported
  UINT32    Dis             : 1;        ///< Disable crashlog supported
  UINT32    Mech            : 2;        ///< CrashLog mechanism, 0=legacy, 1=descriptor table, 2=DVSec, 3=Reserved
  UINT32    ManTri          : 1;        ///< Manual trigger supported
  UINT32    Clr             : 1;        ///< Clear record supported
  UINT32    TriAllRst       : 1;        ///< Trigger on all reset supported
  UINT32    ReArm           : 1;        ///< Re-arm on reset supported
  UINT32    GlbRstTriMsk    : 1;        ///< Global reset trigger mask supported
  UINT32    Rsvd            : 18;       ///< reserved
  // Status
  UINT32    GlbRstTriMskSts : 1;         ///< Global reset trigger mask status
  UINT32    ClSts           : 1;         ///< 0=not in progress, 1=in progress
  UINT32    ReArmSts        : 1;         ///< Re-arm on reset status
  UINT32    AllRstSts       : 1;         ///< Trigger on all resets status
  UINT32    DisSts          : 1;         ///< Crash log disabled status
} CAPABILITY_STATUS_PHASE_1;

//-----------------------------------------------------------
// Crashlog converged structure phase 2 (CTF2.0)
//-----------------------------------------------------------
typedef struct {
  UINT32    DonCare       : 20;       ///< The other fields
  // Capability
  UINT32    Clr           : 1;        ///< Clear record supported
  UINT32    StorageOff    : 1;        ///< Storage off supported
  UINT32    Rsvd          : 2;        ///< reserved
  // Status
  UINT32    StorageOffSts : 1;        ///< Storage off status
  UINT32    ReArmSts      : 1;        ///< Re-arm on reset status
  UINT32    ClErr         : 1;        ///< Crashlog error
  UINT32    ClConsumed    : 1;        ///< Crashlog consumed
  UINT32    ClDis         : 1;        ///< Crashlog disabled
  UINT32    ClrSts        : 1;        ///< Crashlog cleared
  UINT32    ClInProgress  : 1;        ///< Crashlog in progress
  UINT32    ClComplete    : 1;        ///< Crashlog complete
} CAPABILITY_STATUS_PHASE_2;

//-----------------------------------------------------------
// Crashlog structure version 3
//-----------------------------------------------------------
typedef struct {
  UINT32    DonCare       : 20;       ///< The other fields
  // Status
  UINT32    TorDisable    : 1;        ///< TOR Crash Log is disabled
  UINT32    UncoreDisable : 1;        ///< Uncore Crash Log is disabled
  UINT32    CoreDisable   : 1;        ///< Core Crash Log is disabled
  UINT32    McaDisable    : 1;        ///< Trigger MCA Crash Log is disabled
  UINT32    Gated         : 1;        ///< Crashlog gated
  UINT32    ReArmSts      : 1;        ///< Re-arm on reset status
  UINT32    ClErr         : 1;        ///< Crashlog error
  UINT32    ClConsumed    : 1;        ///< Crashlog consumed
  UINT32    ClDis         : 1;        ///< Crashlog disabled
  UINT32    ClrSts        : 1;        ///< Crashlog cleared
  UINT32    ClInProgress  : 1;        ///< Crashlog in progress
  UINT32    ClComplete    : 1;        ///< Crashlog complete
} CAPABILITY_STATUS_VER_3;

//-----------------------------------------------------------
// Converged Crashlog discovery structure version 0
//-----------------------------------------------------------
typedef union {
  struct {
    struct {
      UINT32    AccessType : 4;        // Access Type
      UINT32    CrashType  : 4;        // Crash Type
      UINT32    Count      : 8;        // number of child crashlog records
      UINT32    Version    : 4;        // version
      UINT32    Rsvd       : 12;
    } DW0;
    UINT32    DW1;        // Capability and status
    struct {
      UINT32    Cmd        : 8;
      UINT32    Param      : 8;
      UINT32    Rsvd       : 15;
      UINT32    Busy       : 1;
    } DW2;        // mailbox interface
    UINT32    DW3;        // mailbox data
  } Header;
  UINT32    Data32[4];
} DISCOVERY_STRUCT_VER_0;

//-----------------------------------------------------------
// Converged Crashlog discovery structure version 1
//-----------------------------------------------------------
typedef union {
  struct {
    struct {
      UINT32    AccessType : 4;        // Access Type
      UINT32    CrashType  : 4;        // Crash Type
      UINT32    Count      : 8;        // number of child crashlog records
      UINT32    Version    : 4;        // version
      UINT32    CapSts     : 12;        // Capability and status
    } DW0;
    UINT32    DW1;        // Guid
    UINT32    DW2;        // Offset
    UINT32    DW3;        // Size
    UINT32    DW4;        // Based on access type
    UINT32    DW5;        // Control interface
  } Header;
  UINT32    Data32[6];
} DISCOVERY_STRUCT_VER_1;

//-----------------------------------------------------------
// Converged Crashlog discovery structure version 3
//-----------------------------------------------------------
typedef union {
  struct {
    struct {
      UINT32    AccessType : 4;        // Access Type
      UINT32    CrashType  : 4;        // Crash Type
      UINT32    Count      : 8;        // number of child crashlog records
      UINT32    Version    : 4;        // version
      UINT32    CapSts     : 12;       // Capability and status
    } DW0;
    UINT32    DW1;        // Guid
    UINT32    DW2;        // Offset
    UINT32    DW3;        // Size
  } Header;
  UINT32    Data32[4];
} DISCOVERY_STRUCT_VER_3;
#pragma pack()

#endif // _CONVERGED_STRUCT_H_

