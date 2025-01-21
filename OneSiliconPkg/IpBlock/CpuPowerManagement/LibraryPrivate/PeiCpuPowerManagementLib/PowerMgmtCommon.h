/** @file
  This header file contains the processor power management definitions.

  Acronyms:
   - PPM   Processor Power Management
   - TM    Thermal Monitor
   - IST   Intel(R) Speedstep technology
   - HT    Hyper-Threading Technology

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#ifndef _POWER_MGMT_COMMON_H_
#define _POWER_MGMT_COMMON_H_

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <Register/GenerationMsr.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PciSegmentLib.h>
#include <CpuPowerManagementPrivateDataHob.h>
#include <CpuPowerManagementDataHob.h>
#include <Library/PmcLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/PeiVrLib.h>
#include <Ppi/MpServices2.h>
#include <VrDataHob.h>

#define PM_CST_LVL2                     0x14

#define EFI_IDIV_ROUND(r, s)  ((r) / (s) + (((2 * ((r) % (s))) < (s)) ? 0 : 1))

//
// Global variables
//
///
/// Power Managment policy configurations
///
extern CPU_POWER_MGMT_BASIC_CONFIG            *gCpuPowerMgmtBasicConfig;
extern CPU_POWER_MGMT_CUSTOM_CONFIG           *gCpuPowerMgmtCustomConfig;
extern CPU_POWER_MGMT_TEST_CONFIG             *gCpuPowerMgmtTestConfig;
extern EDKII_PEI_MP_SERVICES2_PPI             *mMpServices2Ppi;
extern CPU_INIT_CONFIG                        *mCpuInitConfig;
extern CPU_POWER_MANAGEMENT_DATA_HOB          *mCpuPowerMgmtDataHob;
//
// Values for FVID table calculate.
//
extern BOOLEAN mOver16Pstates;
extern UINT32  mPpmFlags;                     /// PPM Flags Values.
extern UINT16  mTurboBusRatio;
extern UINT8   mMaxBusRatio;                  /// Matches BUS_RATIO_MAX field in PERF_STS_MSR
extern UINT8   mMinBusRatio;
extern UINT16  mProcessorFlavor;
extern UINT16  mPackageTdpWatt;
extern UINT8   mConfigurablePpc;              /// Boot Mode vlues for _PPC
extern UINT8   mRatioLimitProgrammble;        /// Porgrammable Ratio Limit

///
/// PL1 Thermal Control structure layout
///
typedef union {
  struct {
   UINT8 Disable:1;      ///< [0] Disable
   UINT8 Reserved:7;     ///< [7:2] Reserved
   UINT8 FloorIa:8;        ///< [15:8] Percent Throttle for IA component 255-0; 255=0%, 0=100%
   UINT8 FloorGt:8;        ///< [23:16] Percent Throttle for GT component 255-0; 255=0%, 0=100%
   UINT8 FloorPch:8;       ///< [31:24] Percent Throttle for PCH component 255-0; 255=0%, 0=100%
  } Bits;
  UINT32 Uint32;
 } PL1_THERMAL_CONTROL;

//
// FVID Table Information
// Default FVID table
// One header field plus states
//
extern FVID_TABLE *mFvidPointer;

//
// Function prototypes
//
/**
  Initializes P States and Turbo Power management features
**/
VOID
InitPStates (
  VOID
  );

/**
  Initializes Resource Priority feature.
  **/
VOID
InitializeRpFeature (
  VOID
  );

/**
  Initializes C States Power management features
**/
VOID
InitCState (
  VOID
  );

/**
  Disable/Enable the CState Pre-Wake Feature
**/
VOID
InitCstatePreWake (
  VOID
  );

/**
  Enables C-State support as specified by the input flags on all logical
  processors and sets associated timing requirements in the chipset.

  @param[in] C3IoAddress   IO address to generate C3 states (PM base + 014 usually)
**/
VOID
EnableCStates (
  VOID
  );

/**
  This will perform Miscellaneous Power Management related programming.

  @param[in]      CpuPowerDeliveryConfig     Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
InitMiscFeatures (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Configures PowerLimits and Config TDP values

  @param[in] ApplyConfigTdp             ApplyConfigTdp
  @param[in] PowerLimit1                PowerLimit1
  @param[in] PowerLimit2                PowerLimit2
  @param[in] PowerLimitWindow           PowerLimitWindow
  @param[in] Tar                        Tar
  @param[in] ConfigTdpBootModeIndex     ConfigTdpBootModeIndex
  @param[in] CpuPowerDeliveryConfig     CpuPowerDeliveryConfig
**/
VOID
ConfigurePowerValue (
  IN BOOLEAN                   ApplyConfigTdp,
  IN UINT16                    PowerLimit1,
  IN UINT16                    PowerLimit2,
  IN UINT8                     PowerLimitWindow,
  IN UINT8                     Tar,
  IN UINT8                     ConfigTdpBootModeIndex,
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  This will sync thermal features from BSP to all logical processors.
**/
VOID
SyncThermal (
  VOID
  );

/**
  This will perform general thermal initialization on BSP. Include Below features:

  1. Configure Adaptive thermal monitor
  2. Enable Critical Temperature Interrupt

  @param[in] CpuPowerDeliveryConfig       Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
EarlyThermalInitOnBsp (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Locks down all settings.
**/
VOID
PpmLockDown (
  VOID
  );

/**
  Lock MSR_BROADWELL_PKG_CST_CONFIG_CONTROL
  This function must be MP safe.

  @param[in out] Buffer    Pointer to the function parameters passed in.
**/
VOID
EFIAPI
ApSafeLockDown (
  IN OUT VOID *Buffer
  );

/**
  Configures following fields of MSR 0x610 based on user configuration:
    Configures Long duration Turbo Mode (power limit 1) power level and time window
    Configures Short duration turbo mode (power limit 2)

  @param[out] PowerLimit1            Pointer to PowerLimit1
  @param[out] PowerLimit2            Pointer to PowerLimit2
  @param[out] PowerLimitWindow       Pointer to PowerLimitWindow
  @param[out] Tar                    Pointer to Tar
  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/
VOID
InitNonConfigurableTdpSettings (
  OUT UINT16                    *PowerLimit1,
  OUT UINT16                    *PowerLimit2,
  OUT UINT8                     *PowerLimitWindow,
  OUT UINT8                     *Tar,
  IN  CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Configures following fields of MSR 0x601
    Configures power limit 4 power level and lock

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/
VOID
ConfigurePl4PowerLimits (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Configures following fields of MSR 0x618 based on user configuration:
    Configures Long duration Turbo Mode (power limit 1) power level and time window for DDR domain
    Configures Short duration Turbo mode (power limit 2)
**/
VOID
ConfigureDdrPowerLimits (
  VOID
  );

/**
  Configures MSR 0x65C platform power limits (PSys)
    -Configures Platform Power Limit 1 Enable, power and time window
    -Configures Platform Power Limit 2 Enable, power

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/
VOID
ConfigurePlatformPowerLimits (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Verify and fix Custom Power Limit values

  @param[in] CustomPowerLimit  Custom Power Limit value
  @param[in] CustomPlUnit  Custom Power Limit Unit

  @retval Verified Custom power limit value
**/
UINT16
VerifyAndFixCustomPowerLimit (
  IN UINT32 CustomPowerLimit,
  IN UINT16 CustomPlUnit
  );

/**
  Verify and fix Custom Ratio values
  Custom Ratio should be between MaxTurboFrequency and LFM

  @param[in] CustomPowerLimit  Custom Power Ratio Value

  @retval Verified Custom Ratio value
**/
UINT8
VerifyAndFixCustomRatio (
  IN UINT8 CustomRatio
  );

/**
  Initalizes CTDP BIOS settings from silicon defaults and overrides custom cTDP settings if needed

  @param[in, out] ConfigTdpPowerLimit1       Pointer to ConfigTdpPowerLimit1
  @param[in, out] ConfigTdpPowerLimit2       Pointer to ConfigTdpPowerLimit2
  @param[in, out] ConfigTdpPowerLimitWindow  Pointer to ConfigTdpPowerLimitWindow
  @param[in, out] ConfigTdpTar               Pointer to ConfigTdpTar
  @param[in]      CpuPowerDeliveryConfig     Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
InitConfigurableTdpSettings (
  IN OUT UINT16                *ConfigTdpPowerLimit1,
  IN OUT UINT16                *ConfigTdpPowerLimit2,
  IN OUT UINT8                 *ConfigTdpPowerLimitWindow,
  IN OUT UINT8                 *ConfigTdpTar,
  IN OUT UINT8                 *ConfigTdpBootModeIndex,
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Check if CTDP should be applied

  @param[in]      CpuPowerDeliveryConfig     Pointer to CPU_POWER_DELIVERY_CONFIG

  @retval TRUE    CTDP should be applied
  @retval FALSE   CTDP should not be applied
**/
BOOLEAN
CheckCtdpApply (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Configures following fields of MSR 0x610
    Configures Long duration Turbo Mode (power limit 1) power level and time window
    Configures Short duration turbo mode (power limit 2)

  @param[in] ConfigTdpPowerLimit1       Pointer to ConfigTdpPowerLimit1
  @param[in] ConfigTdpPowerLimit2       Pointer to ConfigTdpPowerLimit2
  @param[in] ConfigTdpPowerLimitWindow  Pointer to ConfigTdpPowerLimitWindow
  @param[in] CpuPowerDeliveryConfig     Pointer to the function parameters passed in
**/
VOID
ConfigureMsrPowerLimits (
  IN BOOLEAN                   ApplyConfigTdp,
  IN UINT16                    PowerLimit1,
  IN UINT16                    PowerLimit2,
  IN UINT8                     PowerLimitWindow
  );

/**
  Patch Fvid Table with Ctdp Tar ratio and Tar-1 Ratio

  @param[in] FvidPointer  Pointer to Fvid Table
  @param[in] ConfigTdpTar ConfigTdpTar
**/
VOID
CtdpPatchFvidTable (
  IN OUT FVID_TABLE *FvidPointer,
  IN UINT8          ConfigTdpTar
  );

/**
  Patch Fvid Table with Ctdp Tar ratio and Tar-1 Ratio for Legacy OS

  @param[in] FvidPointer  Pointer to Fvid Table
  @param[in] ConfigTdpTar ConfigTdpTar
**/
VOID
CtdpPatchFvidTableforLimit16Pstate (
  IN OUT FVID_TABLE *FvidPointer,
  IN UINT8          ConfigTdpTar
  );

/**
  Replace P state with given ratio

  @param[in out] FvidPointer   Pointer to Fvid Table
  @param[in]     PssIndex      FVID table index of P state to be replaced
  @param[in]     Ratio         Target Ratio to put in
  @param[in]     Limit16Pstate FVID table for LPSS
**/
VOID
CtdpReplaceFvidRatio (
  IN OUT FVID_TABLE *FvidPointer,
  UINTN             PssIndex,
  UINTN             Ratio,
  BOOLEAN           Limit16Pstate
  );

/**
  Initializes HWP
**/
VOID
InitializeHwp (
  VOID
  );

/**
  Initialize Per Core P State OS control mode.
  Initialize HwP Autonomous Per Core P State.
  Initialize HwP Autonomous EPP grouping
  Initialize EPB override over PECI.
  Enable/Disable Fast Msr Support for IA32_HWP_REQUEST.
**/
VOID
InitializeHwpMiscFeatures (
  VOID
  );

/**
  Dump FVID Table.

  @param[in out] FvidPointer         Pointer to a table to be updated
  @param[in]     LpssNumberOfStates  Number of entries in the table pointed to by FvidPointer for LPSS
  @param[in]     LpssNumberOfStates  Number of entries in the table pointed to by FvidPointer for TPSS
**/
VOID
DumpFvidTable (
  IN OUT FVID_TABLE *FvidPointer,
  IN UINT16         LpssNumberOfStates,
  IN UINT16         TpssNumberOfStates
  );
#endif
