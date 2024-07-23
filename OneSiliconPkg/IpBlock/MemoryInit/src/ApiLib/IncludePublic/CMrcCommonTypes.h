/** @file
  This file contains the definitions common to the MRC API and other APIs.

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

@par Specification
**/
#ifndef MRC_COMMON_TYPES_H_
#define MRC_COMMON_TYPES_H_

#ifndef MRC_INT32_MIN
#define MRC_INT32_MIN                       (0x80000000)
#endif

#ifndef MRC_INT32_MAX
#define MRC_INT32_MAX                       (0x7FFFFFFF)
#endif

#ifndef MRC_INT16_MIN
#define MRC_INT16_MIN                       (0x8000)
#endif

#ifndef MRC_INT16_MAX
#define MRC_INT16_MAX                       (0x7FFF)
#endif

#ifndef MRC_INT8_MIN
#define MRC_INT8_MIN                       (0x80)
#endif

///
/// System boot mode.
///
#ifndef __MRC_BOOT_MODE__
#define __MRC_BOOT_MODE__                 //These values are cloned to MemInfoHob.h manually
typedef enum {
  bmCold,                                 ///< Cold boot
  bmWarm,                                 ///< Warm boot
  bmS3,                                   ///< S3 resume
  bmFast,                                 ///< Fast boot
  MrcBootModeMax,                         ///< MRC_BOOT_MODE enumeration maximum value.
  MrcBootModeDelim = MRC_INT32_MAX            ///< This value ensures the enum size is consistent on both sides of the PPI.
} MRC_BOOT_MODE;
#endif  //__MRC_BOOT_MODE__

///
/// DIMM memory package
/// This enum matches SPD Module Type - SPD byte 3, bits [3:0]
/// Note that DDR3, DDR4, DDR5, and LPDDR have different encoding for some module types
///
typedef enum {
  RDimmMemoryPackage          = 1,
  UDimmMemoryPackage          = 2,
  SoDimmMemoryPackage         = 3,
  LrDimmMemoryPackageDdr4     = 4,
  CuDimmMemoryPackage         = 5,
  CsoDimmMemoryPackage        = 6,
  MiniCDimmMemoryPackage      = 7,
  LpDimmMemoryPackage         = 7,
  CammMemoryPackage           = 8,
  SoUDimmEccMemoryPackageDdr4 = 9,
  SoCDimmEccMemoryPackage     = 10,
  LrDimmMemoryPackage         = 11,
  SoDimm16bMemoryPackage      = 12,
  SoDimm32bMemoryPackage      = 13,
  NonDimmMemoryPackage        = 14,
  MemoryPackageMax,                       ///< MEMORY_PACKAGE enumeration maximum value.
  MemoryPackageDelim = MRC_INT32_MAX          ///< This value ensures the enum size is consistent on both sides of the PPI.
} MEMORY_PACKAGE;

///
/// Memory training I/O levels.
///
typedef enum {
  DdrLevel   = 0,                         ///< Refers to frontside of DIMM.
  LrbufLevel = 1,                         ///< Refers to data level at backside of LRDIMM or AEP buffer.
  RegALevel  = 2,                         ///< Refers to cmd level at backside of register - side A.
  RegBLevel  = 3,                         ///< Refers to cmd level at backside of register - side B.
  GsmLtMax,                               ///< GSM_LT enumeration maximum value.
  GsmLtDelim = MRC_INT32_MAX                  ///< This value ensures the enum size is consistent on both sides of the PPI.
} GSM_LT;

typedef enum {
  GroupScopePerMc,
  GroupScopePerMcFreq,
  GroupScopePerNoScope,
  GroupScopePerMcCh,
  GroupScopePerMcChRank,
  GroupScopePerDdrioCmdGroup,
  GroupScopePerCcc,
  GroupScopePerCccNoRank,
  GroupScopePerCccLane,
  GroupScopePerChStrobe,
  GroupScopePerStrobe,
  GroupScopePerBit,
  GroupScopePerAll,
} GROUP_SCOPE_TYPE;

typedef enum {
  MrWrV,
  MrCmdV,
  MrCaOdt,
  MrDimmRon,
} MR_TYPE;

typedef enum {
  SigRasN   = 0,
  SigCasN   = 1,
  SigWeN    = 2,
  SigBa0    = 3,
  SigBa1    = 4,
  SigBa2    = 5,
  SigA0     = 6,
  SigA1     = 7,
  SigA2     = 8,
  SigA3     = 9,
  SigA4     = 10,
  SigA5     = 11,
  SigA6     = 12,
  SigA7     = 13,
  SigA8     = 14,
  SigA9     = 15,
  SigA10    = 16,
  SigA11    = 17,
  SigA12    = 18,
  SigA13    = 19,
  SigA14    = 20,
  SigA15    = 21,
  SigA16    = 22,
  SigA17    = 23,
  SigCs0N   = 24,
  SigCs1N   = 25,
  SigCs2N   = 26,
  SigCs3N   = 27,
  SigCs4N   = 28,
  SigCs5N   = 29,
  SigCs6N   = 30,
  SigCs7N   = 31,
  SigCs8N   = 32,
  SigCs9N   = 33,
  SigCke0   = 34,
  SigCke1   = 35,
  SigCke2   = 36,
  SigCke3   = 37,
  SigCke4   = 38,
  SigCke5   = 39,
  SigOdt0   = 40,     //could also be used for CA-ODT for LP4
  SigOdt1   = 41,     //could also be used for CA-ODT for LP4
  SigOdt2   = 42,
  SigOdt3   = 43,
  SigOdt4   = 44,
  SigOdt5   = 45,
  SigPar    = 46,
  SigAlertN = 47,
  SigBg0    = 48,
  SigBg1    = 49,
  SigActN   = 50,
  SigCid0   = 51,
  SigCid1   = 52,
  SigCid2   = 53,
  SigCk0    = 54,
  SigCk1    = 55,
  SigCk2    = 56,
  SigCk3    = 57,
  SigCk4    = 58,
  SigCk5    = 59,
  SigGnt0   = 60,
  SigGnt1   = 61,
  SigErid00 = 62,
  SigErid01 = 63,
  SigErid10 = 64,
  SigErid11 = 65,
  SigErr0   = 66,
  SigErr1   = 67,
  SigCa00   = 68,    // First instantiation of the CA bus for a given channel
  SigCa01   = 69,
  SigCa02   = 70,
  SigCa03   = 71,
  SigCa04   = 72,
  SigCa05   = 73,
  SigCa10   = 74,    // Second instantiation of the CA bus for a given channel
  SigCa11   = 75,
  SigCa12   = 76,
  SigCa13   = 77,
  SigCa14   = 78,
  SigCa15   = 79,
  SigCa06,      ///< LPDDR3 has 10 CA pins per bus.
  SigCa07,
  SigCa08,
  SigCa09,
  SigCa16,
  SigCa17,
  SigCa18,
  SigCa19,
  GsmCsnMax,
  GsmCsnDelim = MRC_INT32_MAX
} GSM_CSN;

typedef enum {
  PartitionPll,
  PartitionPg,
  PartitionVccClk,
  PartitionData,
  PartitionCcc,
  PartitionComp,
  MaxPartition
} PARTITION_BLOCKS;
#endif // MRC_COMMON_TYPES_H_

