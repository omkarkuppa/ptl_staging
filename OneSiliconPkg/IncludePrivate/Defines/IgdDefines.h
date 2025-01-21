/** @file
  Register names for IGD block
  <b>Conventions</b>:
  - Prefixes:
    - Definitions beginning with "R_" are registers
    - Definitions beginning with "B_" are bits within registers
    - Definitions beginning with "V_" are meaningful values of bits within the registers
    - Definitions beginning with "S_" are register sizes
    - Definitions beginning with "N_" are the bit position

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef _IGD_DEFINES_
#define _IGD_DEFINES_

///
/// Device 2 Register Equates
///
//
// The following equates must be reviewed and revised when the specification is ready.
//
#define IGD_BUS_NUM          0x00
#define IGD_DEV_NUM          0x02
#define IGD_FUN_NUM          0x00

#define SA_GT_APERTURE_SIZE_256MB    1      ///< 256MB is the recommanded GT Aperture Size as per BWG.

#define V_SA_PCI_DEV_2_GTM_LNL_ULX_ID      0x64A0 ///< Dev2 LNL-M GTM

///
/// GTTMMADR aligned to 16MB (Base address = [38:24])
///
#define R_SA_IGD_GTTMMADR          0x10
#define R_SA_IGD_LMEMBAR           0x18
#define R_SA_IGD_IOBAR             0x20

#define R_SA_IGD_SWSCI_OFFSET                      0x00E8
#define R_SA_IGD_ASLS_OFFSET                       0x00FC  ///< ASL Storage

#define B_SA_IGD_RC6_CTX_BASE_ADDRESS_MASK         0xFFFFF000

#define R_SA_GTTMMADR_PP_CONTROL                   0xC7204
#define R_SA_GTTMMADR_PP_CONTROL_2                 0xC7304

///
/// GT MMIO Registers for MemSS Global Configuration
///
#define V_SA_GTTMMADR_MEM_SS_INFO_GLOBAL_DDRTYPE_DDR4   0
#define V_SA_GTTMMADR_MEM_SS_INFO_GLOBAL_DDRTYPE_DDR5   1
#define V_SA_GTTMMADR_MEM_SS_INFO_GLOBAL_DDRTYPE_LPDDR5 2
#define V_SA_GTTMMADR_MEM_SS_INFO_GLOBAL_DDRTYPE_LPDDR4 3
#define V_SA_GTTMMADR_MEM_SS_INFO_GLOBAL_DDRTYPE_DDR3   4
#define V_SA_GTTMMADR_MEM_SS_INFO_GLOBAL_DDRTYPE_LPDDR3 5

typedef union {
  struct {
    UINT32 DdrType                   :  4;  // Bits 3:0
    UINT32 NumberOfPopulatedChannels :  4;  // Bits 7:4
    UINT32 NumberOfEnabledQgvPoints  :  4;  // Bits 11:8
    UINT32                           : 20;  // Bits 31:12
  } Bits;
  UINT32 Data;
} R_SA_GTTMMADR_MEM_SS_INFO_GLOBAL_STRUCT;


typedef union {
  struct {
    UINT32 Dclk   : 16;  // Bits 15:0
    UINT32 TRP    :  8;  // Bits 23:16
    UINT32 TRCD   :  8;  // Bits 31:24
    UINT32 TRDPRE :  8;  // Bits 39:32
    UINT32 TRAS   :  8;  // Bits 47:40
    UINT32        : 16;  // Bits 63:48
  } Bits;
  UINT64 Data;
} R_SA_GTTMMADR_MEM_SS_INFO_QGV_POINT_STRUCT;

#define R_SA_GTTMMADR_GCD_RC6CTXBASE_OFFSET        0xD48
#define R_SA_GTTMMADR_GCD_RC6CTXBASE_HIGH_OFFSET   0xD4C


typedef union {
  struct {
    UINT32 FlatCcsEnable             :  1;  // Bits 0
    UINT32 CcsReplicationEnable      :  3;  // Bits 3:1
    UINT32 FlatCcsLock               :  1;  // Bits 4
    UINT32 Reserved1                 :  3;  // Bits 7:5
    UINT32 Reserved2                 :  24; // Bits 31:8
  } Bits;
  UINT32 Data;
} R_SA_GTTMMADR_FLAT_CCS_GAM_STRUCT;

typedef union {
  struct {
    UINT64 Enable                    :  1;  // Bits 0
    UINT64 Reserved                  :  5;  // Bits 5:1
    UINT64 CcsBase                   :  36; // Bits 41:6
    UINT64 Reserved1                 :  4;  // Bits 45:42
    UINT64 CcsSize                   :  14; // Bits 59:46
    UINT64 Reserved2                 :  4;  // Bits 63:60
  } Bits;
  UINT64 Data;
} R_SA_GTTMMADR_FLAT_CCS_BASE_ADDRESS_STRUCT;

#define GCD_WOPCM_SIZE_2_MB       2
#define SA_MEDIA_WOPCM_SIZE_2_MB  2
#define WOPCM_TOTAL_SIZE_4_MB     4
#define WOPCM_TOTAL_SIZE_8_MB     8

//
// GT Ways Mask MSRs
//
#define MSR_GT_COS_WAYS_MASK0                      0x18B0
#define MSR_GT_COS_WAYS_MASK1                      0x18B1
#define MSR_GT_COS_WAYS_MASK2                      0x18B2
#define MSR_GT_COS_WAYS_MASK3                      0x18B3

#define N_MSR_GT_COS_WAYS_OFFSET                   0
#define B_MSR_GT_COS_WAYS_MASK                     0x00000FFF
#define V_MSR_GT_COS_WAYS_1_WAYS                   0x00000001

//
// GT Tag Ways Mask MSRs
//
#define MSR_GT_COS_TAG_WAYS_MASK0                  0x1894
#define MSR_GT_COS_TAG_WAYS_MASK1                  0x1895
#define MSR_GT_COS_TAG_WAYS_MASK2                  0x1896
#define MSR_GT_COS_TAG_WAYS_MASK3                  0x1897

#define B_MSR_GT_COS_TAG_WAYS_MASK                 0x0FFFFFFF
#define V_MSR_GT_COS_TAG_WAYS_1_WAYS               0x00000001

/**
 Description of GGCLCK (0:0)
 - When set to 1b, this bit will lock all bits in this register.
**/
#define B_SA_IGPU_GGC_MASK        (0xffc0)

#define V_SA_GGC_GGMS_8MB     3

///
/// Enums for Graphics MTL GMD Architecture ID
///
typedef enum {
  ///
  /// MTL GMD Architecture GMD REV ID
  ///
  EnumGmdRevIdA0 = 0,
  EnumGmdRevIdB0 = 4,
  EnumGmdRevIdC0 = 8,
  EnumGmdRevIdUnkown = 0xFF
} GRAPHICS_STEPPING;

///
/// Enums for Graphics IP Type
///
typedef enum {
  EnumGfxIpGt = 0,
  EnumGfxIpMedia,
  EnumGfxIpDisplay
} GRAPHICS_IP_TYPE;

//
// Enum for GT Type
//
typedef enum {
  EnumNotRequired,
  EnumLnlGt
} GT_FAMILY_TYPE;

//
// Graphics Architecture Info
//
typedef struct {
  UINT32 IpType;
  UINT32 GmdArch;
  UINT32 GmdRelease;
  UINT8  GmdRevId;
  CHAR8  SteppingString[16];
  UINT32 GtFamily;
} GRAPHICS_ARCH_INFO;

#define V_GT_GMD_ARCHITECTURE         20
#define V_GT_GMD_RELEASE_ID_1         0x4

#define V_MEDIA_GMD_ARCHITECTURE      20
#define V_MEDIA_GMD_RELEASE           0x0

#define V_DISPLAY_GMD_ARCHITECTURE    20
#define V_DISPLAY_GMD_RELEASE         0x0


//
// GMD Architecture
//
typedef union {
  struct {
    UINT32 GMDRevId        : 6;  // Bits 5:0
    UINT32 Rsvd            : 8;  // Bits 13:6
    UINT32 GMDRelease      : 8;  // Bits 21:14
    UINT32 GMDArchitecture : 10; // Bits 31:22
  } Bits;
  UINT32 Data;
} MIRROR_GMD_ID_STRUCT;

#endif
