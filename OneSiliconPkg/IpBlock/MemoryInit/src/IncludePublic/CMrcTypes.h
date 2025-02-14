/** @file

  Include the the general MRC types

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _MRC_TYPES_H_
#define _MRC_TYPES_H_

#ifdef MRC_MINIBIOS_BUILD
#include "MrcMiniBiosEfiDefs.h"
#else
#include <Base.h>
#endif // MRC_MINIBIOS_BUILD

//
// Data Types
//
#ifndef UINT64_STRUCT_T
#define UINT64_STRUCT_T
typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
  UINT16 Data16[4];
  UINT8  Data8[8];
} UINT64_STRUCT;
#endif // UINT64_STRUCT_T


typedef union {
  struct {
    INT32  Low;
    INT32  High;
  } Data32;
  INT64 Data;
} INT64_STRUCT;

typedef union {
  VOID    *Ptr;
  UINTN   DataN;
  UINT64  Data64;
} POINTER_STRUCT;

#define UNSUPPORT 0
#define SUPPORT   1

typedef enum {
  mrcSuccess,
  mrcFail,
  mrcWrongInputParameter,
  mrcTimingError,
  mrcSenseAmpErr,
  mrcReadMPRErr,
  mrcReadLevelingError,
  mrcWriteLevelingError,
  mrcDataTimeCentering1DErr,
  mrcWriteVoltage2DError,
  mrcReadVoltage2DError,
  mrcMiscTrainingError,
  mrcDimmNotSupport,
  mrcChannelNotSupport,
  mrcDeviceBusy,
  mrcCrcError,
  mrcFrequencyError,
  mrcDimmNotExist,
  mrcColdBootRequired,
  mrcMixedDimmSystem,
  mrcAliasDetected,
  mrcRetrain,
  mrcUnsupportedTechnology,
  mrcSocketNotSupported,
  mrcControllerNotSupported,
  mrcRankNotSupported,
  mrcParamSaturation,
  mrcComplexParamDone,
  mrcInstanceDoesNotExist,
  mrcRegisterSettingOutOfBounds,
  mrcResetRecovery,
  mrcOutputBufferFull,
  mrcResetFullTrain,
  mrcTimeout,
  mrcPmMsgErr,
  mrcEyeWidthError,
  mrcPPRBankRowUnavailable,
  mrcDramNotSupportEdvfsc,
  mrcStatusMax
} MrcStatus;

//
// general  macros
//
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(x)  (((x) < 0) ? (-(x)) : (x))
#endif


//
// Used in header files to ensure that they are only included in one location
// in our source code.
// In order to work around this restriction in our test environments, the test
// projects must have linker flags /FORCE:MULTIPLE (MSVC) or
// -z multdef/-allow-multiple-definition (LD)
//
#ifndef PRIVATE_HEADER_DEFENDER
#define PRIVATE_HEADER_DEFENDER(FileName) char FileName##SingleInclude = 'F';
#endif

//
// Return max field value based on its width attribute.
//
#define MAX_VALUE(FieldWidth) ((MRC_BIT0 << FieldWidth) - 1)

//
// Make sure x is inside the range of [a..b]
//
#ifndef RANGE
#define RANGE(x, a, b) (MIN ((b), MAX ((x), (a))))
#endif

#ifndef DIVIDECEIL
#define DIVIDECEIL(a, b)   (((a) + (b) - 1) / (b))
#endif

// Divide Up for postive and negative a values, b must be postive and non 0.
// If a is negative, round away from 0.
#ifndef DIVIDECEILNEG
#define DIVIDECEILNEG(a, b) ((a) / (b) + ((((a) < 0) ? -1 : 1) * ((ABS((a)) % (b) > 0) ? 1 : 0)))
#endif

#ifndef RD2EVEN
#define RD2EVEN(x) ((x) - ((x) % 2))
#endif

// WARNING: Only use this marco if BOTH inputs are Signed Integers, Cast if necessary
// if both inputs are unsigned, use UDIVIDEROUND
#ifndef DIVIDEROUND
#define DIVIDEROUND(a, b)  ((((a) < 0) != ((b) < 0)) ? ((a) - (b) / 2) / (b) : ((a) + (b) / 2) / (b))
#endif

// Use to make slightly smaller code when only working with unsigned numbers.
#ifndef UDIVIDEROUND
#define UDIVIDEROUND(a, b)  (((a) + (b) / 2) / (b))
#endif

#ifndef DIVIDEFLOOR
#define DIVIDEFLOOR(a, b)  ((a) / (b))
#endif

#ifndef OFFSET_OF
#define OFFSET_OF(TYPE, Field) ((UINTN) &(((TYPE *)0)->Field))
#endif

// @todo remove SUB_CH_MASK_CHECK, it's deprecated
#ifndef SUB_CH_MASK_CHECK
#define SUB_CH_MASK_CHECK(Mask, Channel, SubChannel) ((Mask) & (1 << ((Channel) * MAX_SUB_CHANNEL + (SubChannel))))
#endif

#ifndef MC_CH_IDX
#define MC_CH_IDX(Mc, Ch, MaxCh) ((Mc) * (MaxCh) + (Ch))
#endif

/// This define checks if this specific channel and controller is set in the BitMask.
/// It will return the & result of the operation.
#ifndef MC_CH_MASK_CHECK
#define MC_CH_MASK_CHECK(Mask, Mc, Ch, MaxCh) ((Mask) & (1 << MC_CH_IDX ((Mc), (Ch), (MaxCh))))
#endif
// Number of elements in a 1D array
#ifndef ARRAY_COUNT
#define ARRAY_COUNT(a) (sizeof (a) / sizeof (a[0]))
#endif
// Number of elements in a 2D array
#ifndef ARRAY_COUNT2D
#define ARRAY_COUNT2D(a) (sizeof (a) / sizeof (a[0][0]))
#endif
// Number of elements in a 3D array
#ifndef ARRAY_COUNT3D
#define ARRAY_COUNT3D(a) (sizeof (a) / sizeof (a[0][0][0]))
#endif
// Macros to define all [Controller/Channel/SubChannel/Rank] bits in the mask
#ifndef ALL_CONTROLLER_MASK
#define ALL_CONTROLLER_MASK ((1 << MAX_CONTROLLER) - 1)
#endif

#ifndef ALL_CHANNEL_MASK
#define ALL_CHANNEL_MASK ((1 << MAX_CHANNEL) - 1)
#endif

#ifndef ALL_SUB_CH_MASK
#define ALL_SUB_CH_MASK ((1 << MAX_SUB_CHANNEL) - 1)
#endif

#ifndef ALL_RANK_MASK
#define ALL_RANK_MASK  ((1 << MAX_RANK_IN_CHANNEL) - 1)
#endif

#if _MSC_EXTENSIONS
//
// Disable warning that make it impossible to compile at /W4
// This only works for Microsoft* tools
//
//
// Disabling bitfield type checking warnings.
//
#pragma warning (disable : 4214)
//
// Unreferenced formal parameter - We are object oriented, so we pass parameters even
//  if we don't need them.
//
#pragma warning (disable : 4100)
//
// ASSERT(FALSE) or while (TRUE) are legal constructs so supress this warning
//
#pragma warning(disable : 4127)
//
// The given function was selected for inline expansion, but the compiler did not perform the inlining.
//
#pragma warning(disable : 4710)

#endif // _MSC_EXTENSIONS
#define MRC_BIT0          0x00000001
#define MRC_BIT1          0x00000002
#define MRC_BIT2          0x00000004
#define MRC_BIT3          0x00000008
#define MRC_BIT4          0x00000010
#define MRC_BIT5          0x00000020
#define MRC_BIT6          0x00000040
#define MRC_BIT7          0x00000080
#define MRC_BIT8          0x00000100
#define MRC_BIT9          0x00000200
#define MRC_BIT10         0x00000400
#define MRC_BIT11         0x00000800
#define MRC_BIT12         0x00001000
#define MRC_BIT13         0x00002000
#define MRC_BIT14         0x00004000
#define MRC_BIT15         0x00008000
#define MRC_BIT16         0x00010000
#define MRC_BIT17         0x00020000
#define MRC_BIT18         0x00040000
#define MRC_BIT19         0x00080000
#define MRC_BIT20         0x00100000
#define MRC_BIT21         0x00200000
#define MRC_BIT22         0x00400000
#define MRC_BIT23         0x00800000
#define MRC_BIT24         0x01000000
#define MRC_BIT25         0x02000000
#define MRC_BIT26         0x04000000
#define MRC_BIT27         0x08000000
#define MRC_BIT28         0x10000000
#define MRC_BIT29         0x20000000
#define MRC_BIT30         0x40000000
#define MRC_BIT31         0x80000000
#define MRC_BIT32        0x100000000ULL
#define MRC_BIT33        0x200000000ULL
#define MRC_BIT34        0x400000000ULL
#define MRC_BIT35        0x800000000ULL
#define MRC_BIT36       0x1000000000ULL
#define MRC_BIT37       0x2000000000ULL
#define MRC_BIT38       0x4000000000ULL
#define MRC_BIT39       0x8000000000ULL
#define MRC_BIT40      0x10000000000ULL
#define MRC_BIT41      0x20000000000ULL
#define MRC_BIT42      0x40000000000ULL
#define MRC_BIT43      0x80000000000ULL
#define MRC_BIT44     0x100000000000ULL
#define MRC_BIT45     0x200000000000ULL
#define MRC_BIT46     0x400000000000ULL
#define MRC_BIT47     0x800000000000ULL
#define MRC_BIT48    0x1000000000000ULL
#define MRC_BIT49    0x2000000000000ULL
#define MRC_BIT50    0x4000000000000ULL
#define MRC_BIT51    0x8000000000000ULL
#define MRC_BIT52   0x10000000000000ULL
#define MRC_BIT53   0x20000000000000ULL
#define MRC_BIT54   0x40000000000000ULL
#define MRC_BIT55   0x80000000000000ULL
#define MRC_BIT56  0x100000000000000ULL
#define MRC_BIT57  0x200000000000000ULL
#define MRC_BIT58  0x400000000000000ULL
#define MRC_BIT59  0x800000000000000ULL
#define MRC_BIT60 0x1000000000000000ULL
#define MRC_BIT61 0x2000000000000000ULL
#define MRC_BIT62 0x4000000000000000ULL
#define MRC_BIT63 0x8000000000000000ULL

  #define MRC_DEADLOOP() { volatile int __iii; __iii = 1; while (__iii); }

#ifndef ASM
#define ASM __asm
#endif

///
/// Type Max/Min Values
///
#define MRC_INT32_MAX   (0x7FFFFFFF)
#define MRC_INT32_MIN   (0x80000000)
#define MRC_INT64_MAX   (0x7FFFFFFFFFFFFFFFLL)
#define MRC_INT64_MIN   (0x8000000000000000LL)
#define MRC_UINT8_MAX   (0xFF)
#define MRC_UINT16_MAX  (0xFFFF)
#define MRC_UINT32_MAX  (0xFFFFFFFF)
#define MRC_UINT64_MAX  (0xFFFFFFFFFFFFFFFFULL)
#define MRC_UINT_MIN    (0x0)

#define MRC_GET (1)
#define MRC_SET (0)
#define MRC_ENABLE  (1)
#define MRC_DISABLE (0)

#define TEN_MULTIPLIER         (10)
#define HUNDRED_MULTIPLIER     (100)
#define THOUSAND_MULTIPLIER    (1000)

#ifndef PRIxMAX

  #if defined(_WIN64) && defined(_MSC_VER)
    #define PRIdMAX "I64d"
    #define PRIxMAX "I64x"
    #define PRIXMAX "I64X"
    #define PRIdPTR "I64d"
    #define PRIxPTR "I64x"
    #define PRIXPTR "I64X"
  #elif defined(_LP64) && defined(__GNUC__)
    #define PRIdMAX "lld"
    #define PRIxMAX "llx"
    #define PRIXMAX "llX"
    #define PRIdPTR "ld"
    #define PRIxPTR "lx"
    #define PRIXPTR "lX"
  #else
    #define PRIdMAX "d"
    #define PRIxMAX "x"
    #define PRIXMAX "X"
    #define PRIdPTR "d"
    #define PRIxPTR "x"
    #define PRIXPTR "X"
  #endif

#endif

#endif // _MRC_TYPES_H_

