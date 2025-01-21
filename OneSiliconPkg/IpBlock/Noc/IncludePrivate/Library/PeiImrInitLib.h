/** @file
  Library header file for IMR Initalization for NOC & OPM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _PEI_IMR_INIT_LIB_H_
#define _PEI_IMR_INIT_LIB_H_

#include <Library/PeiCmiInitLib.h>

#define INVALID_CP_OFFSET         0xFFFF
#define KEEP_SI_DEFAULT_VALUE     0x0
#define IMR_NOT_BIOS_PROGRAMMABLE 0xFFFF
#define RELATIVE_MASK_OFFSET      0x8
#define RELATIVE_RAC_OFFSET       0x20
#define RELATIVE_WAC_OFFSET       0x28

#define N_IMR_BASE_LIMIT_ADDRESS_OFFSET    10
#define N_IMR_1M_BASE_LIMIT_ADDRESS_OFFSET 20


typedef union {
  struct {
    UINT64 Reserved1                   :  3;  // Bits 2:0
    UINT64 RW                          :  1;  // Bits 3:3
    UINT64 Disable                     :  1;  // Bits 4:4
    UINT64 Reserved0                   :  1;  // Bits 5:5
    UINT64 IMRn_BASE_RO                :  4;  // Bits 9:6
    UINT64 IMRn_BASE_RW                :  32; // Bits 41:10
    UINT64 Unused                      :  22; // Bits 63:42
  } Bits;
  UINT64 Data;
} IMRnBASE_0_0_0_MCHBAR_IMPH;

typedef union {
  struct {
    UINT64 Reserved1                   :  3;  // Bits 2:0
    UINT64 R_Wn                        :  1;  // Bits 3:3
    UINT64 Disable                     :  1;  // Bits 4:4
    UINT64 Reserved0                   :  1;  // Bits 5:5
    UINT64 IMR1Mn_BASE_RO              :  14; // Bits 19:6
    UINT64 IMR1Mn_BASE_RW              :  22; // Bits 41:20
    UINT64 Unused                      :  22; // Bits 63:42
  } Bits;
  UINT64 Data;
} IMR1MnBASE_0_0_0_MCHBAR_IMPH;

typedef union {
  struct {
    UINT64 Reserved1                   :  3;  // Bits 2:0
    UINT64 R_Wn                        :  1;  // Bits 3:3
    UINT64 Reserved0                   :  2;  // Bits 5:4
    UINT64 IMRn_MASK_RO                :  4;  // Bits 9:6
    UINT64 IMRn_MASK_RW                :  32; // Bits 41:10
    UINT64 Unused                      :  22; // Bits 63:42
  } Bits;
  UINT64 Data;
} IMRnMASK_0_0_0_MCHBAR_IMPH;

typedef union {
  struct {
    UINT64 Reserved1                   :  3;  // Bits 2:0
    UINT64 R_Wn                        :  1;  // Bits 3:3
    UINT64 Reserved0                   :  2;  // Bits 5:4
    UINT64 IMR1Mn_Limit_RO             :  14; // Bits 19:6
    UINT64 IMR1Mn_Limit_RW             :  22; // Bits 41:20
    UINT64 Unused                      :  22; // Bits 63:42
  } Bits;
  UINT64 Data;
} IMR1MnLimit_0_0_0_MCHBAR_IMPH;

//
// BIOS Programmable IMRs
//
typedef enum {
// IMR
  IMR_NPK = 0,         // 0
  IMR_PRMRR,           // 1
  IMR_TSE,             // 2
  IMR_STREAM_TRACER,   // 3
  IMR_SEAMRR,          // 4
//IMR_1M
  IMR_1M_GSM,          // 5
  IMR_1M_DSM,          // 6
  IMR_1M_CCS,          // 7
  IMR_1M_DPR,          // 8
  IMR_1M_TSEG,         // 9
  IMR_1M_IMR11,        // 10
  IMR_MAX              // 11
} IMR_ID;

//
// IMR Table Entry.
//
typedef struct {
  IMR_ID      ImrId;
  UINT64      Rac;
  UINT64      Wac;
  UINT32      BaseAddrRegOffset;
  UINT32      Cp;
} IMR_ENTRY;

/**
  Program IMR registers.

  @param[in] ImrId     - IMR ID number
  @param[in] Base      - Byte aligned Base Address value to be programmed in IMR
  @param[in] Size      - Size of the region

  @retval EFI_SUCCESS            - IMR Programming completed successfully.
  @retval EFI_INVALID_PARAMETER  - The input parameter is invalid
  @retval EFI_UNSUPPORTED          IMR is not supported for BIOS programming.
**/
EFI_STATUS
EFIAPI
SetImr (
  IN   IMR_ID      ImrId,
  IN   UINT64      Base,
  IN   UINT64      Size
  );

/**
  Program IMR and IMR_1M for all IPs.

  @param[in] TsegSize      - Size of TSEG
**/
VOID
ImrInit (
  IN UINT32   TsegSize
  );

#endif
