/** @file
  Functional Safety policy.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _FUSA_CONFIG_H_
#define _FUSA_CONFIG_H_

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 3</b>:  - Add FusaConfigEnable switch, BIST scan and locksteps for module.
**/

#define FUSA_CONFIG_REVISION 3

extern EFI_GUID gFusaConfigGuid;

#pragma pack (push,1)

/**
  The FUSA_CONFIG block describes the expected configuration of Functional Safety
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Config Block Header
  /**
    Enable/Disable Fusa feature on Display
    Fusa configuration on Display <b>0: Disable</b>; 1: Enable.
    Enable/Disable Fusa feature on IOP MCA Check
    Fusa configuration on IOP MCA Check <b>0: Disable</b>; 1: Enable.
  **/
  UINT32    DisplayFusaConfigEnable                :  1;    ///< Enable/Disable FuSa feature on DE: 0=Disable, 1=Enable
  UINT32    GraphicFusaConfigEnable                :  1;    ///< Enable/Disable FuSa feature on GT: 0=Disable, 1=Enable
  UINT32    OpioFusaConfigEnable                   :  1;    ///< Enable/Disable FuSa feature on OPIO: 0=Disable, 1=Enable
  UINT32    PsfFusaConfigEnable                    :  1;    ///< Enable/Disable FuSa feature on PSF: 0=Disable, 1=Enable
  UINT32    IopFusaConfigEnable                    :  1;    ///< Enable/Disable FuSa feature on IOP: 0=Disable, 1=Enable
  UINT32    IopFusaMcaCheckEnable                  :  1;    ///< Enable/Disable FuSa feature on IOP MCA Check Enable: 0=Disable, 1=Enable
  UINT32    FusaConfigEnable                       :  1;    ///< Enable/Disable FuSa feature: 0=Disable, 1=Enable
  UINT32    FusaRunStartupArrayBist                :  1;    ///< Enabling this will execute startup array test during boot
  UINT32    FusaRunStartupScanBist                 :  1;    ///< Enabling this will execute startup scan test during boot
  UINT32    FusaRunPeriodicArrayBist               :  1;    ///< Enabling this will execute periodic array test during boot
  UINT32    FusaRunPeriodicScanBist                :  1;    ///< Enabling this will execute periodic scan test during boot
  UINT32    Module0Lockstep                        :  2;    ///< Enable/Disable Lockstep for Atom module 1, which has 4 cores;
                                                            ///< 0: Disable lockstep; 1: Enable lockstep for Core 0 with Core 1, Core 2 with Core 3;
                                                            ///< 2: Enable lockstep for Core 0 with Core 1; 3: Enable lockstep for Core 2 with Core 3
  UINT32    Module1Lockstep                        :  2;    ///< Enable/Disable Lockstep for Atom module 1, which has 4 cores;
                                                            ///< 0: Disable lockstep; 1: Enable lockstep for Core 0 with Core 1, Core 2 with Core 3;
                                                            ///< 2: Enable lockstep for Core 0 with Core 1; 3: Enable lockstep for Core 2 with Core 3
  UINT32    FusaRunStartupArrayBistMod0            :  1;    ///< Enabling this will execute startup array test Module 0 during boot
  UINT32    FusaRunStartupArrayBistMod1            :  1;    ///< Enabling this will execute startup array test Module 1 during boot
  UINT32    FusaRunStartupScanBistMod0             :  1;    ///< Enabling this will execute startup scan test Module 0 during boot
  UINT32    FusaRunStartupScanBistMod1             :  1;    ///< Enabling this will execute startup scan test Module 1 during boot
  UINT32    FusaRunPeriodicArrayBistMod0           :  1;    ///< Enabling this will execute periodic array test Module 0 during boot
  UINT32    FusaRunPeriodicArrayBistMod1           :  1;    ///< Enabling this will execute periodic array test Module 1 during boot
  UINT32    FusaRunPeriodicScanBistMod0            :  1;    ///< Enabling this will execute periodic scan test Module 0 during boot
  UINT32    FusaRunPeriodicScanBistMod1            :  1;    ///< Enabling this will execute periodic scan test Module 1 during boot
  UINT32    RsvdBits0                              :  9;    ///< Reserved bits
  UINT32    FusaStartupPatternAddr;  
  UINT32    FusaPeriodicPatternAddr;
} FUSA_CONFIG;

#pragma pack (pop)

#define MSR_FUSA_CONFIG                     0x000002D1
#define MSR_TRIGGER_STARTUP_SCAN_BIST       0x000002D2
#define MSR_STARTUP_SCAN_BIST_STATUS        0x000002D3
#define MSR_TRIGGER_STARTUP_MEM_BIST        0x000002D4
#define MSR_STARTUP_MEM_BIST_STATUS         0x000002D5
#define MSR_CONFIG_PERIODIC_SCAN_BIST       0x000002DA
#define MSR_TRIGGER_PERIODIC_SCAN_BIST      0x000002DB
#define MSR_PERIODIC_SCAN_BIST_STATUS       0x000002CA
#define MSR_FUSA_CAPABILITIES_A             0x000002D9

#define B_MSR_FUSA_CAP_STARTUP_SCAN_DIAGNOGSIS_MASK     BIT0
#define B_MSR_FUSA_CAP_STARTUP_ARRAY_DIAGNOGSIS_MASK    BIT1
#define B_MSR_FUSA_CAP_PERIODIC_ARRAY_DIAGNOGSIS_MASK   BIT2
#define B_MSR_FUSA_CAP_LOCKSTEP_MODE_MASK               BIT3
#define B_MSR_FUSA_CAP_PERIODIC_SCAN_DIAGNOGSIS_MASK    BIT4

#define B_MSR_FUSA_STARTUP_ARRAY_PASS_MASK              (BIT2 | BIT1)

#define MAX_AP_COUNT   24

typedef struct {
  BOOLEAN   GlobalApEnable;
  UINT8     CurrProcessor;
  BOOLEAN   LockStepEn[MAX_AP_COUNT];
} LOCKSTEP_CONFIG;

typedef struct {
  BOOLEAN   GlobalApEnable;
  UINT8     CurrProcessor;
  BOOLEAN   TestEn[MAX_AP_COUNT];
  UINT32    Index;
  UINT64    Data;
  UINT64    buffAddr;
  struct {
    UINT32    Module0Lockstep : 2;
    UINT32    Module1Lockstep : 2;
    UINT32    Reserved        : 28;
  } Lockstep;
} TEST_CONFIG;

/**
  LOCKSTEP ENABLE
**/

typedef enum {
  LockstepDisabled    = 0x0,
  LockstepC0C1_C2C3   = 0x1,
  LockstepC0C1        = 0x2,
  LockstepC2C3        = 0x3
} LOCKSTEP_MODE;

#define GRT_MOD8_LPID0_APIC_ID      0x30
#define GRT_MOD8_LPID1_APIC_ID      0x32
#define GRT_MOD8_LPID2_APIC_ID      0x34
#define GRT_MOD8_LPID3_APIC_ID      0x36
#define GRT_MOD9_LPID0_APIC_ID      0x38
#define GRT_MOD9_LPID1_APIC_ID      0x3A
#define GRT_MOD9_LPID2_APIC_ID      0x3C
#define GRT_MOD9_LPID3_APIC_ID      0x3E
#endif // _FUSA_CONFIG_H_
