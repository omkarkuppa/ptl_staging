/** @file
  This file contains power management configuration functions for processors.

  <b>Acronyms:</b>
     - PPM:  Processor Power Management
     - TM:   Thermal Monitor
     - IST:  Intel(R) Speedstep technology
     - HT:   Hyper-Threading Technology

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

#include "PowerMgmtCommon.h"
#include <Library/CpuCommonLib.h>
#include <Library/CpuPlatformLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/MsrFruLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Guid/CpuPmData.h>

#define CONFIG_TDP_LVL1_PKG_TDP_MASK                                  (0x7FFF)
#define PACKAGE_POWER_LIMIT_PKG_CLMP_LIM_1_MASK                       (BIT16)

//
//  ThETA Ibatt Feature MMIO register
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 IsysCurrentLimitL1        : 15;  ///< Bits[14:0]
    UINT32 IsysCurrentLimitL1Enable  : 1;   ///< Bits[15:15]
    UINT32 IsysCurrentL1Tau          : 7;   ///< Bits[22:16]
    UINT32 Reserved1                 : 9;   ///< Bits[31:23]
    UINT32 IsysCurrentLimitL2        : 15;  ///< Bits[46:32]
    UINT32 IsysCurrentLimitL2Enable  : 1;   ///< Bits[47:47]
    UINT32 Reserved2                 : 16;  ///< Bits[63:48]
  } Fields;
  UINT64 Data;
} THETA_IBATT_MMIO_REGISTER;

//
//  MMIO register which provides control over limitation of CPU due to platform temperature
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8   SkinTargetTemp           : 8;  ///< Bits[7:0]
    BOOLEAN SkinTempControlEnable    : 1;  ///< Bits[8:8]
    UINT8   SkinControlLoopGain      : 3;  ///< Bits[11:9]
    BOOLEAN SkinTempOverrideEnable   : 1;  ///< Bits[12:12]
    UINT8   Reserved                 : 3;  ///< Bits[15:13]
    UINT8   SkinMinPerformanceLevel  : 8;  ///< Bits[23:16]
    UINT8   SkinTempOverride         : 8;  ///< Bits[31:24]
  } Fields;
  UINT32 Data;
} SKIN_TEMP_CONTROL_MMIO_REGISTER;

///
/// This is default power unit which is not permitted to set
/// Default value is PowerUnit125MilliWatts, Unit is 125 milli watt
///
CONST UINT16 DefaultPowerUnit = 8;

/**
  Get Cpu PM data from Guid Hob.

  @param[out] CpuPmData       Pointer to Cpu PM data

  @retval EFI_NOT_FOUND       gCpuPmDataGuid Hob not found
  @retval EFI_SUCCESS         get CpuPmData successfully
**/
EFI_STATUS
GetCpuPmData (
  CPU_PM_DATA** CpuPmData
  )
{
  EFI_HOB_GUID_TYPE                     *GuidHob;
  POWER_MANAGEMENT_PARAM                ZeroBuffer;

  ZeroMem (&ZeroBuffer, sizeof(POWER_MANAGEMENT_PARAM));
  GuidHob = GetFirstGuidHob (&gCpuPmDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }

  *CpuPmData = (CPU_PM_DATA *)(GET_GUID_HOB_DATA (GuidHob));
  if (CompareMem(&((*CpuPmData)->PmData), &ZeroBuffer, sizeof(POWER_MANAGEMENT_PARAM)) == 0) {
    //
    // All PM data is zero, return not found.
    //
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

/**
  This function check if turbo-ratio can be overriden and returns final Turbo Ratio.

  @return Max Turbo Ratio
**/
UINT8
PmGetTurboBusRatio (
  VOID
  )
{
  MSR_FLEX_RATIO_REGISTER   FlexRatioMsr;
  UINT8                     OverclockingBins;
  UINT8                     TurboBusRatio;

  ///
  /// Read the overclocking bins
  ///
  FlexRatioMsr.Uint64 = AsmReadMsr64 (MSR_FLEX_RATIO);
  OverclockingBins = (UINT8) FlexRatioMsr.Bits.OcBins;
  TurboBusRatio = GetMaxTurboRatio ();
  ///
  /// Check if processor turbo-ratio can be overriden
  /// Check for Overclocking Lock bit
  ///
  if (((UINT8) IsCoreRatioLimitSupported ()) && (!FlexRatioMsr.Bits.OcLock)) {
    ///
    /// For fully unlocked CPU's, configure Turbo Ratio as 0xFF (max possible P-State)
    ///
    if (OverclockingBins == MAX_OVERCLOCKING_BINS) {
      TurboBusRatio  = 0xFF;
    } else {
      ///
      /// TurboBusRatio has been initialized to Fused Max Turbo Ratio in InitPowerManagementGlobalVariables().
      /// Max Turbo ratio or P0 = Fused Max Turbo Ratio + No of over clocking Bins.
      ///
      TurboBusRatio  += OverclockingBins;
    }
  }

  return TurboBusRatio;
}

/**
  Verify and fix Custom Power Limit values

  @param[in] CustomPowerLimit  Custom Power Limit value
  @param[in] CustomPlUnit  Custom Power Limit Unit
**/
UINT16
VerifyAndFixCustomPowerLimit (
  IN UINT32 CustomPowerLimit,
  IN UINT16 CustomPlUnit
  )
{
  UINT16                            ConvertedPowerLimit;
  UINT16                            PackageMaxPower;
  UINT16                            PackageMinPower;
  MSR_PACKAGE_POWER_SKU_REGISTER    PackagePowerSkuMsr;

  PackagePowerSkuMsr.Uint64     = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  PackageMaxPower = (UINT16) PackagePowerSkuMsr.Bits.PkgMaxPwr;
  ///
  ///  If PACKAGE_POWER_SKU_MSR [46:32] = 0 means there is no upper limit ( since this field is 15 bits, the max value is 2^15 - 1 )
  ///
  if (PackageMaxPower == 0) {
    PackageMaxPower = (1 << 15) - 1;
  }
  PackageMinPower = (UINT16) PackagePowerSkuMsr.Bits.PkgMinPwr;
  ConvertedPowerLimit = (UINT16) ((CustomPowerLimit * GetPowerUnit ()) / CustomPlUnit);

  if (ConvertedPowerLimit < PackageMinPower) {
    ConvertedPowerLimit = PackageMinPower;
  } else if (ConvertedPowerLimit > PackageMaxPower) {
    ConvertedPowerLimit = PackageMaxPower;
  }
  return ConvertedPowerLimit;
}

/**
  Verify and fix Custom Ratio values
  Custom Ratio should be between MaxTurboFrequency and LFM

  @param[in] CustomRatio  Custom Ratio value
**/
UINT8
VerifyAndFixCustomRatio (
  IN UINT8 CustomRatio
  )
{
  UINT8  MaxBusRatio;
  UINT8  MinBusRatio;
  UINT8  TurboBusRatio;

  GetBusRatio (&MaxBusRatio, &MinBusRatio);
  TurboBusRatio = GetMaxTurboRatio ();

  if (CustomRatio > TurboBusRatio) {
    ///
    /// Use HFM as max value if Turbo is not supported
    ///
    if (TurboBusRatio == 0) {
      CustomRatio = (UINT8) MaxBusRatio;
    } else {
      CustomRatio = (UINT8) TurboBusRatio;
    }
  } else if (CustomRatio < MinBusRatio) {
    ///
    /// Use LFM as min value
    ///
    CustomRatio = (UINT8) MinBusRatio;
  }

  return CustomRatio;
}

/**
  Check if CTDP should be applied

  @param[in]      CpuPowerDeliveryConfig     Pointer to CPU_POWER_DELIVERY_CONFIG

  @retval TRUE    CTDP should be applied
  @retval FALSE   CTDP should not be applied
**/
BOOLEAN
CheckCtdpApply (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  UINT8                             CpuConfigTdpLevels;
  CPU_PM_DATA                       *CpuPmData;
  EFI_STATUS                        Status;

  if (CpuPowerDeliveryConfig->ApplyConfigTdp != 1) {
    return FALSE;
  }

  ///
  /// Get the number of configurable TDP Levels supported
  ///
  CpuConfigTdpLevels = GetConfigTdpLevels ();

  DEBUG ((DEBUG_INFO, "PPM:: ConfigTDP Supported Levels=%d\n", CpuConfigTdpLevels));
  ///
  /// Return if ConfigTDP Levels not supported
  ///
  if (CpuConfigTdpLevels == 0) {
    DEBUG ((DEBUG_INFO, "PPM:: ConfigTDP Levels not supported\n"));
    return FALSE;
  }

  CpuPmData = NULL;
  Status = GetCpuPmData (&CpuPmData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Could not find CpuPmData\n"));
    return FALSE;
  }
  ASSERT (CpuPmData!= NULL);

  if (CpuPowerDeliveryConfig->ConfigTdpLevel >= 3) {
    DEBUG ((DEBUG_ERROR, "ERROR: Invalid CpuConfigTdpBootLevel=%x, and CpuConfigTdpBootLevel should be range in 0-2 \n", CpuPowerDeliveryConfig->ConfigTdpLevel));
    ASSERT (FALSE);
    return FALSE;
  }

  return TRUE;
}

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
  )
{
  MSR_CONFIG_TDP_NOMINAL_REGISTER   ConfigTdpNominalMsr;
  MSR_CONFIG_TDP_LEVEL1_REGISTER    ConfigTdpLevel1Msr;
  MSR_CONFIG_TDP_LEVEL2_REGISTER    ConfigTdpLevel2Msr;
  MSR_PACKAGE_POWER_SKU_REGISTER    PackagePowerSkuMsr;
  UINT16                            CpuConfigTdpNominalTdp;
  UINT16                            CpuConfigTdpLevel1Tdp;
  UINT16                            CpuConfigTdpLevel2Tdp;
  UINT32                            CpuConfigTdpNominalRatio;
  UINT32                            CpuConfigTdpLevel1Ratio;
  UINT32                            CpuConfigTdpLevel2Ratio;
  UINT8                             CpuConfigTdpLevels;
  CPU_PM_DATA                       *CpuPmData;
  EFI_STATUS                        Status;


  ///
  /// Get the number of configurable TDP Levels supported
  ///
  CpuConfigTdpLevels = GetConfigTdpLevels ();
  DEBUG ((DEBUG_INFO, "PPM:: ConfigTDP Supported Levels=%d\n", CpuConfigTdpLevels));
  ASSERT (CpuConfigTdpLevels != 0);

  ///
  /// Get PKG_TDP for Config TDP Nominal
  ///
  ConfigTdpNominalMsr.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_NOMINAL);
  PackagePowerSkuMsr.Uint64  = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  CpuConfigTdpNominalRatio = ConfigTdpNominalMsr.Bits.TdpRatio;
  CpuConfigTdpNominalTdp    = (UINT16) PackagePowerSkuMsr.Bits.PkgTdp;;
  DEBUG ((DEBUG_INFO, "PPM:: ConfigTDP Nominal Ratio=%d Tdp=%d\n", CpuConfigTdpNominalRatio, CpuConfigTdpNominalTdp));

  CpuPmData = NULL;
  Status = GetCpuPmData (&CpuPmData);
  ASSERT_EFI_ERROR(Status);
  ASSERT (CpuPmData!= NULL);

  *ConfigTdpBootModeIndex = (UINT8) CpuPowerDeliveryConfig->ConfigTdpLevel;
  ASSERT (*ConfigTdpBootModeIndex < 3);

  if (*ConfigTdpBootModeIndex == CONFIG_TDP_DEACTIVATE || *ConfigTdpBootModeIndex > CpuConfigTdpLevels) {
    DEBUG ((DEBUG_WARN, "WARN: unsupported CpuConfigTdpBootLevel=0x%x, set CTDP level to 0 \n", *ConfigTdpBootModeIndex));
    *ConfigTdpBootModeIndex = 0;
  }

  *ConfigTdpPowerLimitWindow = DT_POWER_LIMIT1_TIME_DEFAULT;
  if (CpuPmData->PmData.MsrPowerLimit1TimeWindow != 0) {
    *ConfigTdpPowerLimitWindow = CpuPmData->PmData.MsrPowerLimit1TimeWindow;
  }

  ///
  /// MSR 0x610 depends on three inputs
  ///   1. MSR (0x614 for nominal, 0x649 for level 1, 0x64a for level 2)
  ///   2. PPM override table
  ///   3. User overrides in setup
  /// MSR610.PL1 depends on inputs #1 and #3, MSR610.PL2 depends on inputs #2 and #3
  ///
  ///  Set Level0 as Tdp Nominal
  ///
  if (*ConfigTdpBootModeIndex == CONFIG_TDP_NOMINAL) {
    *ConfigTdpPowerLimit1 = (UINT16) PackagePowerSkuMsr.Bits.PkgTdp;
    *ConfigTdpPowerLimit2 = (UINT16) ((CpuPmData->PmData.CtdpNominalPowerLimit2 * GetPowerUnit ()) / 100);
    *ConfigTdpTar = (UINT8) (CpuConfigTdpNominalRatio - 1);
  } else{
    ///
    /// Get PKG_TDP and Ratio for Config TDP Level1
    ///
    ConfigTdpLevel1Msr.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL1);
    CpuConfigTdpLevel1Ratio = (UINT32) ConfigTdpLevel1Msr.Bits.TdpRatio;
    CpuConfigTdpLevel1Tdp = (UINT16) (ConfigTdpLevel1Msr.Bits.PkgTdp);
    DEBUG ((DEBUG_INFO, "PPM:: ConfigTDP Level1 Ratio=%d Tdp=%d\n", CpuConfigTdpLevel1Ratio, CpuConfigTdpLevel1Tdp));

    ///
    /// Set Level 1
    ///
    if ((*ConfigTdpBootModeIndex == CONFIG_TDP_LEVEL1) && (CpuConfigTdpLevel1Ratio != 0)) {
      *ConfigTdpPowerLimit1= CpuConfigTdpLevel1Tdp;
      *ConfigTdpPowerLimit2 = (UINT16) ((CpuPmData->PmData.CtdpDownPowerLimit2 * GetPowerUnit ()) / 100);
      *ConfigTdpTar = (UINT8) (CpuConfigTdpLevel1Ratio - 1);
    } else {
      ///
      /// If two levels are supported or Level1 was not valid
      /// then read Level2 registers
      /// Get PKG_TDP and Ratio for Config TDP Level2
      ///
      ConfigTdpLevel2Msr.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL2);
      CpuConfigTdpLevel2Ratio = (UINT32) ConfigTdpLevel2Msr.Bits.TdpRatio;
      CpuConfigTdpLevel2Tdp = (UINT16) (ConfigTdpLevel2Msr.Bits.PkgTdp);
      DEBUG ((DEBUG_INFO, "PPM:: ConfigTDP Level2 Ratio=%d Tdp=%d\n", CpuConfigTdpLevel2Ratio, CpuConfigTdpLevel2Tdp));
      ///
      /// Set Level2
      ///
      *ConfigTdpPowerLimit1 = CpuConfigTdpLevel2Tdp;
      *ConfigTdpPowerLimit2 = (UINT16) ((CpuPmData->PmData.CtdpUpPowerLimit2 * GetPowerUnit ()) / 100);
      *ConfigTdpTar = (UINT8) (CpuConfigTdpLevel2Ratio - 1);
    }
  }

  ///
  /// Override any custom ConfigTdp information if applicable. This will only update if a custom
  /// setting change is detected, otherwise the CPU based default cTDP  settings will be applied.
  ///

  ///
  /// Verify and fix Custom configured CTDP Levels PL1
  ///
  if (CpuPowerDeliveryConfig->CustomPowerLimit1 != 0) {
    *ConfigTdpPowerLimit1 = VerifyAndFixCustomPowerLimit (CpuPowerDeliveryConfig->CustomPowerLimit1, DefaultPowerUnit);
  }

  ///
  /// Ctdp PL1 Time Window
  ///
  if (CpuPowerDeliveryConfig->CustomPowerLimit1Time != 0) {
    *ConfigTdpPowerLimitWindow = (UINT8) CpuPowerDeliveryConfig->CustomPowerLimit1Time;
  }

  ///
  /// Verify and fix Custom configured CTDP Levels PL2
  ///
  if (CpuPowerDeliveryConfig->CustomPowerLimit2 != 0) {
    *ConfigTdpPowerLimit2 = VerifyAndFixCustomPowerLimit (CpuPowerDeliveryConfig->CustomPowerLimit2, DefaultPowerUnit);
  }

  ///
  /// cTDP Turbo Activation Ratio
  ///
  if (CpuPowerDeliveryConfig->CustomTurboActivationRatio != 0) {
    *ConfigTdpTar = VerifyAndFixCustomRatio ((UINT8) CpuPowerDeliveryConfig->CustomTurboActivationRatio + 1) - 1;
  }
}

/**
  Patch Fvid Table with Ctdp Tar ratio and Tar-1 Ratio

  @param[in] FvidPointer  Pointer to Fvid Table
  @param[in] ConfigTdpTar ConfigTdpTar
**/
VOID
CtdpPatchFvidTable (
  IN OUT FVID_TABLE *FvidPointer,
  IN UINT8          ConfigTdpTar
  )
{
  UINTN             PssIndex;
  UINTN             PssIndexP1;
  UINTN             TempRatio;
  UINT16            Turbo;

  ///
  /// Check and patch Fvid table for TAR ratios
  ///
  DEBUG ((DEBUG_INFO, "\nCtdpPatchFvidTable - Start \n"));

  Turbo = ((mPpmFlags & PPM_TURBO) ? 1 : 0);

  ///
  /// PSS Index for P1 (HFM) depends on Turbo Enabled/Disabled
  /// - Turbo Enabled:
  ///    -- PSS Index for P1 = 2
  ///-  Turbo Disabled:
  ///    -- PSS Index for P1 = 1
  ///
  if (Turbo) {
    PssIndexP1 = 2;
  } else {
    PssIndexP1 = 1;
  }


  TempRatio = ConfigTdpTar;
  ///
  /// Fvid Table's first entry is Header only, then state ratios start from the second entry.
  /// Size of table is EistStates plus one, but need to stop iterating one entry early, due to use of PssIndex + 1.
  ///
  for (PssIndex = PssIndexP1; PssIndex < FvidPointer[0].FvidHeader.EistStates; PssIndex++) {
    if (FvidPointer[PssIndex + 1].FvidState.BusRatio < TempRatio) {
      if (FvidPointer[PssIndex].FvidState.BusRatio != TempRatio) {
        ///
        /// If TAR not Found, Replace Turbo Activation Ratio at PssIndex
        ///
        CtdpReplaceFvidRatio (FvidPointer, PssIndex, TempRatio, FALSE);
        DEBUG ((DEBUG_INFO, " TAR Ratio Replace at Index %d with Ratio:%d \n", PssIndex, TempRatio));
      }
      break;
    }
  }

  ///
  /// Check and patch Fvid table for CTDP ratios.
  /// This is done separately to make sure Ctdp ratios are not override by TAR ratios
  /// when CTDP ratios are adjacent.
  ///

  ///
  /// PSS Index for P1 (HFM) depends on Turbo Enabled/Disabled
  /// - Turbo Enabled:
  ///    -- PSS Index for P1 = 2
  ///-  Turbo Disabled:
  ///    -- PSS Index for P1 = 1
  ///
  if (Turbo) {
    PssIndexP1 = 2;
  } else {
    PssIndexP1 = 1;
  }


  TempRatio = ConfigTdpTar + 1;
  ///
  /// Fvid Table's first entry is Header only, then state ratios start from the second entry.
  /// Size of table is EistStates plus one, but need to stop iterating one entry early, due to use of PssIndex + 1.
  ///
  for (PssIndex = PssIndexP1; PssIndex < FvidPointer[0].FvidHeader.EistStates; PssIndex++) {
    if (FvidPointer[PssIndex + 1].FvidState.BusRatio == TempRatio) {
      mConfigurablePpc = (UINT8) FvidPointer[PssIndex + 1].FvidState.State;
    }

    if (FvidPointer[PssIndex + 1].FvidState.BusRatio < TempRatio) {
      if (FvidPointer[PssIndex].FvidState.BusRatio == TempRatio) {
        ///
        /// Found Turbo Activation Ratio + 1 at PssIndex
        ///
        mConfigurablePpc = (UINT8) FvidPointer[PssIndex].FvidState.State;
        break;
      } else {
        ///
        /// If TAR+1 not Found, Replace Turbo Activation Ratio + 1 at PssIndex
        ///
        CtdpReplaceFvidRatio (FvidPointer, PssIndex, TempRatio, FALSE);
        mConfigurablePpc = (UINT8) FvidPointer[PssIndex].FvidState.State;
        DEBUG ((DEBUG_INFO, " CTDP Ratio Replace Index at %d with Ratio:%d \n", PssIndex, TempRatio));
        break;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "CtdpPatchFvidTable - End \n\n"));
}

/**
  Patch Fvid Table with Ctdp Tar ratio and Tar-1 Ratio for Legacy OS

  @param[in] FvidPointer  Pointer to Fvid Table
  @param[in] ConfigTdpTar ConfigTdpTar
**/
VOID
CtdpPatchFvidTableforLimit16Pstate (
  IN OUT FVID_TABLE *FvidPointer,
  IN UINT8          ConfigTdpTar
  )
{
  UINTN             PssIndex;
  UINTN             PssIndexP1;
  UINTN             TempRatio;
  UINT16            Turbo;

  ///
  /// Check and patch Fvid table for TAR ratios
  ///
  DEBUG ((DEBUG_INFO, "CtdpPatchFvidTableforLimit16Pstate - Start \n"));

  Turbo = ((mPpmFlags & PPM_TURBO) ? 1 : 0);
  ///
  /// PSS Index for P1 (HFM) depends on Turbo Enabled/Disabled
  /// - Turbo Enabled:
  ///    -- PSS Index for P1 = 2
  ///-  Turbo Disabled:
  ///    -- PSS Index for P1 = 1
  ///
  if (Turbo) {
    PssIndexP1 = 2;
  } else {
    PssIndexP1 = 1;
  }

  TempRatio = ConfigTdpTar;
  ///
  /// Fvid Table's first entry is Header only, then state ratios start from the second entry.
  /// Size of table is EistStates plus one, but need to stop iterating one entry early, due to use of PssIndex + 1.
  ///
  for (PssIndex = PssIndexP1; PssIndex < LPSS_FVID_MAX_STATES; PssIndex++) {
    if (FvidPointer[PssIndex + 1].FvidState.Limit16BusRatio < TempRatio) {
      if (FvidPointer[PssIndex].FvidState.Limit16BusRatio != TempRatio) {
        ///
        /// If TAR not Found, Replace Turbo Activation Ratio at PssIndex
        ///
        CtdpReplaceFvidRatio (FvidPointer, PssIndex, TempRatio, TRUE);
        DEBUG ((DEBUG_INFO, " TAR Ratio Replace Index at %d with Ratio:%d \n", PssIndex, TempRatio));
      }
      break;
    }
  }

  ///
  /// Check and patch Fvid table for CTDP ratios.
  /// This is done separately to make sure Ctdp ratios are not override by TAR ratios
  /// when CTDP ratios are adjacent.
  ///

  ///
  /// PSS Index for P1 (HFM) depends on Turbo Enabled/Disabled
  /// - Turbo Enabled:
  ///    -- PSS Index for P1 = 2
  ///-  Turbo Disabled:
  ///    -- PSS Index for P1 = 1
  ///
  if (Turbo) {
    PssIndexP1 = 2;
  } else {
    PssIndexP1 = 1;
  }


  TempRatio = ConfigTdpTar + 1;
  ///
  /// Fvid Table's first entry is Header only, then state ratios start from the second entry.
  /// Size of table is EistStates plus one, but need to stop iterating one entry early, due to use of PssIndex + 1.
  ///
  for (PssIndex = PssIndexP1; PssIndex < LPSS_FVID_MAX_STATES; PssIndex++) {
    if (FvidPointer[PssIndex + 1].FvidState.Limit16BusRatio == TempRatio) {
      mConfigurablePpc = (UINT8) FvidPointer[PssIndex + 1].FvidState.Limit16State;
    }

    if (FvidPointer[PssIndex + 1].FvidState.Limit16BusRatio < TempRatio) {
      if (FvidPointer[PssIndex].FvidState.Limit16BusRatio == TempRatio) {
        ///
        /// Found Turbo Activation Ratio + 1 at PssIndex
        ///
        mConfigurablePpc = (UINT8) FvidPointer[PssIndex].FvidState.Limit16State;
        break;
      } else {
        ///
        /// If TAR+1 not Found, Replace Turbo Activation Ratio + 1 at PssIndex
        ///
        CtdpReplaceFvidRatio (FvidPointer, PssIndex, TempRatio, TRUE);
        mConfigurablePpc = (UINT8) FvidPointer[PssIndex].FvidState.Limit16State;
        DEBUG ((DEBUG_INFO, " CTDP Ratio Replace Index at %d with Ratio:%d \n", PssIndex, TempRatio));
        break;
      }
    }
  }
  DEBUG ((DEBUG_INFO, "CtdpPatchFvidTableforLimit16Pstate - End \n\n"));
}

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
  )
{
  UINT64 wPower1;
  UINT64 wPower2;

  if (Limit16Pstate) {
    ///
    /// Replace new Ratio
    ///
    FvidPointer[PssIndex].FvidState.Limit16BusRatio = (UINT16) Ratio;

    ///
    /// Calculate relative Power (Limit16 States)
    ///
    wPower1 = (mMaxBusRatio - FvidPointer[PssIndex].FvidState.Limit16BusRatio) * 625;
    wPower1 = (110000 - wPower1);
    wPower1 = DivU64x32 (wPower1, 11);
    wPower1 = MultU64x64 (wPower1, wPower1);
    //
    // Power is calculated in milliwatts
    //
    wPower2 = (((FvidPointer[PssIndex].FvidState.Limit16BusRatio * 100) / mMaxBusRatio));
    wPower2 = DivU64x32 (MultU64x32 (MultU64x64 (wPower2, wPower1), mPackageTdpWatt), 10000000);
    FvidPointer[PssIndex].FvidState.Limit16Power = (UINT32) wPower2;

  } else {
    ///
    /// Replace new Ratio
    ///
    FvidPointer[PssIndex].FvidState.BusRatio = (UINT16) Ratio;

    ///
    /// Calculate relative Power
    ///
    wPower1 = (mMaxBusRatio - FvidPointer[PssIndex].FvidState.BusRatio) * 625;
    wPower1 = (110000 - wPower1);
    wPower1 = DivU64x32 (wPower1, 11);
    wPower1 = MultU64x64 (wPower1, wPower1);
    //
    // Power is calculated in milliwatts
    //
    wPower2 = (((FvidPointer[PssIndex].FvidState.BusRatio * 100) / mMaxBusRatio));
    wPower2 = DivU64x32 (MultU64x32 (MultU64x64 (wPower2, wPower1), mPackageTdpWatt), 10000000);
    FvidPointer[PssIndex].FvidState.Power = (UINT32) wPower2;

  }
}

/**
  Check if Dual Tau boost is enabled.

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in

  @retval  TRUE   Dual Tau boost is enabled.
           FALSE  Dual Tau boost is disabled.
**/
STATIC
BOOLEAN
IsDualTauEnable (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  return ((GetCpuSku () == EnumCpuTrad) && CpuPowerDeliveryConfig->DualTauBoost);
}

/**
  Get power limit settings for dual tau boost

  @param[out] MsrPl1  The override MSR PL1 value
  @param[out] MsrPl2  The override MSR PL2 value
  @param[out] MsrTau  The override MSR tau value
  @param[out] MmioPl1 The override MMIO PL1 value
  @param[out] MmioPl2 The override MMIO PL2 value
  @param[out] MmioTau The override MMIO tau value

  @retval  TRUE   The override values are provided.
           FALSE  The override values are not provided.
**/
STATIC
BOOLEAN
DualTauGetPowerLimit (
  OUT UINT32 *MsrPl1,
  OUT UINT32 *MsrPl2,
  OUT UINT32 *MsrTau,
  OUT UINT32 *MmioPl1,
  OUT UINT32 *MmioPl2,
  OUT UINT32 *MmioTau
  )
{
  MSR_PACKAGE_POWER_SKU_REGISTER  PackagePowerSkuMsr;
  UINT16                          PackageTdpWatt;

  ///
  /// Get Processor TDP
  /// Get Maximum Power from Turbo Power Limit MSR Bits[46:32]
  /// and convert it to units specified by Package Power SKU
  /// Unit MSR [3:0]
  ///
  PackagePowerSkuMsr.Uint64     = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  PackageTdpWatt = (UINT16) DivU64x32 ((UINT16) PackagePowerSkuMsr.Bits.PkgTdp , GetPowerUnit ());

  switch (PackageTdpWatt) {
    //
    // For 35W TDP
    //
    case 35:
      if (MsrPl1 != NULL)  *MsrPl1  = 35;
      if (MsrPl2 != NULL)  *MsrPl2  = 70;
      if (MsrTau != NULL)  *MsrTau  = 224;
      if (MmioPl1 != NULL) *MmioPl1 = 45;
      if (MmioPl2 != NULL) *MmioPl2 = 70;
      if (MmioTau != NULL) *MmioTau = 28;
      return TRUE;
    //
    // For 65W TDP
    //
    case 65:
      if (MsrPl1 != NULL)  *MsrPl1  = 65;
      if (MsrPl2 != NULL)  *MsrPl2  = 129;
      if (MsrTau != NULL)  *MsrTau  = 224;
      if (MmioPl1 != NULL) *MmioPl1 = 85;
      if (MmioPl2 != NULL) *MmioPl2 = 129;
      if (MmioTau != NULL) *MmioTau = 28;
      return TRUE;
    //
    // For 125W TDP
    //
    case 125:
      if (MsrPl1 != NULL)  *MsrPl1  = 125;
      if (MsrPl2 != NULL)  *MsrPl2  = 250;
      if (MsrTau != NULL)  *MsrTau  = 224;
      if (MmioPl1 != NULL) *MmioPl1 = 165;
      if (MmioPl2 != NULL) *MmioPl2 = 250;
      if (MmioTau != NULL) *MmioTau = 28;
      return TRUE;
    default:
      return FALSE;
  }
}

/**
  Configure Dual Tau boost if it's enabled.

  @param[in] ApplyConfigTdp  The CPU part whether apply Ctdp feature.

  @retval TRUE    DualTauBoostOverride is applied
  @retval FALSE   DualTauBoostOverride is not applied
**/
STATIC
BOOLEAN
ConfigureDualTauBoostOverride (
  IN BOOLEAN ApplyConfigTdp
  )
{
  BOOLEAN                         DualTauPowerLimitValueProvided;
  UINT32                          DualTauMsrPl1;
  UINT32                          DualTauMsrPl2;
  UINT32                          DualTauMsrTau;
  UINT32                          DualTauMmioPl1;
  UINT32                          DualTauMmioPl2;
  UINT32                          DualTauMmioTau;
  UINT64                          MchBar;
  MSR_PACKAGE_RAPL_LIMIT_REGISTER PackageRaplLimitMsr;
  MSR_PACKAGE_RAPL_LIMIT_REGISTER PackageRaplLimitMmio;

  DEBUG ((DEBUG_INFO, "ConfigureDualTauBoostOverride Start\n"));
  //
  // Override power limit for Dual Tau Boost
  //
  DualTauPowerLimitValueProvided = DualTauGetPowerLimit (
                                     &DualTauMsrPl1,
                                     &DualTauMsrPl2,
                                     &DualTauMsrTau,
                                     &DualTauMmioPl1,
                                     &DualTauMmioPl2,
                                     &DualTauMmioTau
                                     );
  if (DualTauPowerLimitValueProvided) {
    PackageRaplLimitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_RAPL_LIMIT);
    DEBUG ((DEBUG_INFO, "ConfigureDualTauBoostOverride original MSR 610 = %016lx\n", PackageRaplLimitMsr.Uint64));
    //
    // Override MSR PL1
    //
    PackageRaplLimitMsr.Bits.PkgPwrLim1 = DualTauMsrPl1 * GetPowerUnit ();
    //
    // Override MSR Tau
    //
    PackageRaplLimitMsr.Bits.PkgPwrLim1Time = GetConvertedTime (DualTauMsrTau * 1000);

    if (ApplyConfigTdp) {
      //
      // Override MSR PL2
      //
      PackageRaplLimitMsr.Bits.PkgPwrLim2 = DualTauMsrPl2 * GetPowerUnit ();
    }
    AsmWriteMsr64 (MSR_PACKAGE_RAPL_LIMIT, PackageRaplLimitMsr.Uint64);
    DEBUG ((DEBUG_INFO, "ConfigureDualTauBoostOverride modified MSR 610 = %016lx\n", PackageRaplLimitMsr.Uint64));

    MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
    DEBUG ((DEBUG_INFO, "ConfigureDualTauBoostOverride original 59A0 = 0x%x\n", MmioRead32 (MchBar + MMIO_TURBO_POWER_LIMIT)));
    DEBUG ((DEBUG_INFO, "ConfigureDualTauBoostOverride original 59A4 = 0x%x\n", MmioRead32 (MchBar + MMIO_TURBO_POWER_LIMIT_HIGH)));

    //
    // Reuse the MSR_PACKAGE_RAPL_LIMIT_REGISTER structure for MMIO register since they are identical.
    //
    PackageRaplLimitMmio.Uint64 = LShiftU64 (MmioRead32 (MchBar + MMIO_TURBO_POWER_LIMIT_HIGH), 32) + MmioRead32 (MchBar + MMIO_TURBO_POWER_LIMIT);
    //
    // Override MMIO PL1
    //
    PackageRaplLimitMmio.Bits.PkgPwrLim1 = DualTauMmioPl1 * GetPowerUnit ();
    //
    // Override MMIO Tau
    //
    PackageRaplLimitMmio.Bits.PkgPwrLim1Time = GetConvertedTime (DualTauMmioTau * 1000);
    //
    // Program MMIO PowerLimit enable bit for PL1_EN
    //
    PackageRaplLimitMmio.Bits.PkgPwrLim1En = 1;
    MmioWrite32 (MchBar + MMIO_TURBO_POWER_LIMIT, (UINT32) PackageRaplLimitMmio.Uint64);

    if (ApplyConfigTdp) {
      //
      // Override MMIO PL2
      //
      PackageRaplLimitMmio.Bits.PkgPwrLim2 = DualTauMmioPl2 * GetPowerUnit ();
    } else {
      //
      // non-cTDP MMIO PL2 = MSR PL2
      //
      PackageRaplLimitMmio.Bits.PkgPwrLim2 = PackageRaplLimitMsr.Bits.PkgPwrLim2;
    }
    PackageRaplLimitMmio.Bits.PkgPwrLim2En = 1;
    MmioWrite32 (MchBar + MMIO_TURBO_POWER_LIMIT_HIGH, (UINT32) RShiftU64 (PackageRaplLimitMmio.Uint64, 32));
    DEBUG ((DEBUG_INFO, "ConfigureDualTauBoostOverride modified 59A0 = 0x%x\n", MmioRead32 (MchBar + MMIO_TURBO_POWER_LIMIT)));
    DEBUG ((DEBUG_INFO, "ConfigureDualTauBoostOverride modified 59A4 = 0x%x\n", MmioRead32 (MchBar + MMIO_TURBO_POWER_LIMIT_HIGH)));
    return TRUE;
  }
  return FALSE;
}

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
  )
{
  MSR_PACKAGE_POWER_SKU_REGISTER  PackagePowerSkuMsr;
  CPU_PM_DATA                     *CpuPmData;
  EFI_STATUS                      Status;

  CpuPmData = NULL;
  Status = GetCpuPmData (&CpuPmData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Could not find CpuPmData\n"));
  }

  ///
  /// Initialize the Power Limit 1 and Power Limit 1 enable bit
  ///  - Power Limit 1: Turbo Power Limit MSR  [14:0]
  ///  - Power Limit 1 Enable: Turbo Power Limit MSR  [15]
  ///
  ///
  /// By default, program Power Limit 1 to Package TDP limit
  ///
  PackagePowerSkuMsr.Uint64  = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  *PowerLimit1 = (UINT16) PackagePowerSkuMsr.Bits.PkgTdp;

  if (CpuPowerDeliveryConfig->PowerLimit1 != AUTO) {
    ///
    /// CpuPowerDeliveryConfig->PowerLimit1's unit is 125 milli watt
    ///
    *PowerLimit1 = VerifyAndFixCustomPowerLimit(CpuPowerDeliveryConfig->PowerLimit1, DefaultPowerUnit);
  } else {
    DEBUG ((DEBUG_INFO, "Power Limit 1 = AUTO\n"));
    if ((CpuPmData != NULL) && (CpuPmData->PmData.MsrPowerLimit1 != 0)) {
      ///
      /// CpuPowerDeliveryConfig->PowerLimit1's unit is 10 milli watt
      ///
      *PowerLimit1 = VerifyAndFixCustomPowerLimit(CpuPmData->PmData.MsrPowerLimit1, 100);
    }
  }

  *PowerLimitWindow  = (UINT8) CpuPowerDeliveryConfig->PowerLimit1Time;
  ///
  /// Program Power Limit 1 (Long Duration Turbo) Time Window
  ///
  if (*PowerLimitWindow == AUTO) {
    *PowerLimitWindow = DT_POWER_LIMIT1_TIME_DEFAULT;
    if ((CpuPmData != NULL) && (CpuPmData->PmData.MsrPowerLimit1TimeWindow != 0)) {
      *PowerLimitWindow = CpuPmData->PmData.MsrPowerLimit1TimeWindow;
    }
  }

  ///
  ///  Initialize Short Duration Power limit and enable bit
  ///    Short duration Power Limit: Turbo Power Limit MSR (0x450h) [46:32]
  ///    Short duration Power Limit Enable:Turbo Power Limit MSR (0x450h) [47]
  ///
  *PowerLimit2 = 0;
  if (CpuPowerDeliveryConfig->PowerLimit2 == TRUE) {
    ///
    /// CpuPowerDeliveryConfig->PowerLimit2Power value is in mW or watts. We need to convert it to
    /// CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
    ///
    *PowerLimit2 = VerifyAndFixCustomPowerLimit(CpuPowerDeliveryConfig->PowerLimit2Power, DefaultPowerUnit);
    if (CpuPowerDeliveryConfig->PowerLimit2Power == AUTO) {
      ///
      /// If Power Limit 2 is set to AUTO, then program PL2 defaults by sku
      ///
      if ((CpuPmData != NULL) && (CpuPmData->PmData.MsrPowerLimit2 != 0)) {
        DEBUG ((DEBUG_INFO, "Power Limit 2 overrides\n"));
        *PowerLimit2 = VerifyAndFixCustomPowerLimit(CpuPmData->PmData.MsrPowerLimit2, 100);
      }
    }
  } else {
    ///
    /// When we disable Power Limit 2,then program PL2 defaults by sku
    ///
    if ((CpuPmData != NULL) && (CpuPmData->PmData.MsrPowerLimit2 != 0)) {
      *PowerLimit2 = VerifyAndFixCustomPowerLimit(CpuPmData->PmData.MsrPowerLimit2, 100);
    }
  }

  *Tar = (UINT8) PmGetTurboBusRatio();
}

/**
  Configures following fields of MSR 0x615
    Configures power limit 3 power level and time window

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/
VOID
EFIAPI
ConfigurePl3PowerLimits (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  MSR_PL3_CONTROL_REGISTER PowerLimit3ControlMsr;

  UINT16       ConvertedPowerLimit3;
  UINT8        ConvertedPowerLimit3Time;

  ///
  /// PL3 is supported
  ///
  PowerLimit3ControlMsr.Uint64 = AsmReadMsr64 (MSR_PL3_CONTROL);
  DEBUG ((DEBUG_INFO," PL3 MSR 615 Before Writing 0x%x \n",PowerLimit3ControlMsr.Uint32));
  ///
  /// Configure PL3 Power Limit if custom value is available
  ///
  if (CpuPowerDeliveryConfig->PowerLimit3 != AUTO) {
    ConvertedPowerLimit3 = (UINT16) ((CpuPowerDeliveryConfig->PowerLimit3 * GetPowerUnit ()) / DefaultPowerUnit);
    PowerLimit3ControlMsr.Bits.PowerLimit = (UINT32) (ConvertedPowerLimit3);
    PowerLimit3ControlMsr.Bits.Enable = 1;
  }

  ///
  /// Configure PL3 Time window if custom value is available
  ///
  if (CpuPowerDeliveryConfig->PowerLimit3Time != AUTO) {
    ConvertedPowerLimit3Time = GetConvertedTime (CpuPowerDeliveryConfig->PowerLimit3Time);
    PowerLimit3ControlMsr.Bits.Timewindow = (UINT32) ConvertedPowerLimit3Time;
    PowerLimit3ControlMsr.Bits.Enable = 1;
  }

  ///
  /// Configure PL3 Duty Cycle if custom value is available
  ///
  if (CpuPowerDeliveryConfig->PowerLimit3DutyCycle != AUTO) {
    PowerLimit3ControlMsr.Bits.Dutycyle = (UINT32) CpuPowerDeliveryConfig->PowerLimit3DutyCycle;
    PowerLimit3ControlMsr.Bits.Enable = 1;
  }

  //
  // Enable/Disable PL3 lock
  //
  if (CpuPowerDeliveryConfig->PowerLimit3Lock == TRUE) {
    PowerLimit3ControlMsr.Bits.Lock = 1;
  } else {
    PowerLimit3ControlMsr.Bits.Lock = 0;
  }

  //
  // ResponseMode is PL3 CONTROL MSR 0x615 BIT16, it indicates how quickly Pcode reduces power consumption to a low (safe) level
  //
  if (CpuPowerDeliveryConfig->ResponseMode == TRUE) {
    PowerLimit3ControlMsr.Bits.ResponseMode = 1;
  } else {
    PowerLimit3ControlMsr.Bits.ResponseMode = 0;
  }

  if ((CpuPowerDeliveryConfig->PowerLimit3DutyCycle == AUTO) &&
      (CpuPowerDeliveryConfig->PowerLimit3Time == AUTO) &&
      (CpuPowerDeliveryConfig->PowerLimit3 == AUTO)) {
    //
    // Explicitly disable PL3 if all options are set to AUTO
    //
    PowerLimit3ControlMsr.Bits.PowerLimit = 0;
    PowerLimit3ControlMsr.Bits.Enable = 0;
  }

  AsmWriteMsr64 (MSR_PL3_CONTROL, PowerLimit3ControlMsr.Uint64);
  DEBUG ((DEBUG_INFO," PL3 MSR 615 After Writing 0x%x \n",PowerLimit3ControlMsr.Uint32));
}

/**
  Configure PL4 limits by programming the CURRENT LIMIT and LOCK fields of MSR 601

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/
VOID
ConfigurePl4PowerLimits (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  MSR_VR_CURRENT_CONFIG_REGISTER PowerLimit4Msr;

  UINT16                   ConvertedPowerLimit4;
  UINTN                    Index;
  VOID                     *Hob;
  VR_DATA_HOB              *VrDataHob;
  BOOLEAN                  FvmDisable;

  EFI_STATUS               Status;
  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi;
  CPU_POWER_MGMT_VR_CONFIG *CpuPowerMgmtVrConfig;
  CPU_PM_DATA              *CpuPmData;

  CpuPmData     = NULL;
  Hob           = NULL;
  VrDataHob     = NULL;
  FvmDisable    = TRUE;
  SiPreMemPolicyPpi    = NULL;
  CpuPowerMgmtVrConfig = NULL;

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID*) SiPreMemPolicyPpi, &gCpuPowerMgmtVrConfigGuid, (VOID *) &CpuPowerMgmtVrConfig);
  ASSERT_EFI_ERROR (Status);

  Hob = (VR_DATA_HOB *) GetFirstGuidHob (&gVrDataHobGuid);
  if (Hob == NULL) {
    DEBUG((DEBUG_ERROR, "CPU Data HOB is not available\n"));
    ASSERT (Hob != NULL);
  }
  VrDataHob = (VR_DATA_HOB *)((UINTN)Hob + sizeof (EFI_HOB_GUID_TYPE));
  if (VrDataHob == NULL) {
    ASSERT (VrDataHob != NULL);
    return;
  }

  for (Index = 0; Index < MAX_NUM_VRS; Index++) {
    if (VrDataHob->FastVmodeSupported[Index] != 0) {
      //
      // IccLimit in Policy config data was only updated during Policy update Lib, it can't be changed
      // after that. But later, we might still enabled the FVM according the user config (EnableFastVmode
      // & IccLimit). So, still need check here!!!
      //
      if (CpuPowerMgmtVrConfig->IccLimit[Index] != 0 || ((CpuPowerMgmtVrConfig->EnableFastVmode[Index] == 1) && VrDataHob->IccMaxItrip[Index] != 0)) {
        FvmDisable = FALSE;
        break;
      }
    }
  }

  PowerLimit4Msr.Uint64 = AsmReadMsr64 (MSR_VR_CURRENT_CONFIG);
  PowerLimit4Msr.Bits.CurrentLimit = 0;
  DEBUG ((DEBUG_INFO," PL4 MSR 601 Before Writing 0x%x \n", PowerLimit4Msr.Uint32));

  if (CpuPowerDeliveryConfig->PowerLimit4 != AUTO) {
    ///
    /// User defined PL4
    ///
    ConvertedPowerLimit4 = (UINT16) ((CpuPowerDeliveryConfig->PowerLimit4 * GetPowerUnit ()) / DefaultPowerUnit);
    PowerLimit4Msr.Bits.CurrentLimit = (UINT32) (ConvertedPowerLimit4);
  } else {
    //
    // Set PL4 power limit = 0 when AUTO is selected
    //
    if (!IsOcEnabled ()) {
      Status = GetCpuPmData (&CpuPmData);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Error: Could not find CpuPmData\n"));
      } else {
        ASSERT (CpuPmData!= NULL);
        if (CpuPmData->PmData.MsrPowerLimit4DisableFvm != 0 && FvmDisable) {
          PowerLimit4Msr.Bits.CurrentLimit = (UINT32) (CpuPmData->PmData.MsrPowerLimit4DisableFvm * GetPowerUnit ()) / 100;
        } else if (CpuPmData->PmData.MsrPowerLimit4 != 0) {
          PowerLimit4Msr.Bits.CurrentLimit = (UINT32) (CpuPmData->PmData.MsrPowerLimit4 * GetPowerUnit ()) / 100;
        }
      }

    } else {
      ///
      /// Disable PL4 when AUTO is selected and overclocking is enabled.
      ///
      PowerLimit4Msr.Uint32 = 0;
    }
  }

  //
  // Enable/Disable PL4 lock
  //
  if (CpuPowerDeliveryConfig->PowerLimit4Lock == TRUE) {
    PowerLimit4Msr.Bits.Lock = 1;
  } else {
    PowerLimit4Msr.Bits.Lock = 0;
  }

  AsmWriteMsr64 (MSR_VR_CURRENT_CONFIG, PowerLimit4Msr.Uint64);
  DEBUG ((DEBUG_INFO," PL4 MSR 601 After Writing 0x%x \n ", PowerLimit4Msr.Uint32));
}

/**
  Configures following fields of MSR 0x610
    Configures Long duration Turbo Mode (power limit 1) power level and time window
    Configures Short duration turbo mode (power limit 2)

  @param[in] ApplyConfigTdp    IsCtdpApplied
  @param[in] PowerLimit1       PowerLimit1
  @param[in] PowerLimit2       PowerLimit2
  @param[in] PowerLimitWindow  PowerLimitWindow
**/
VOID
ConfigureMsrPowerLimits (
  IN BOOLEAN                   ApplyConfigTdp,
  IN UINT16                    PowerLimit1,
  IN UINT16                    PowerLimit2,
  IN UINT8                     PowerLimitWindow
  )
{
  MSR_PACKAGE_RAPL_LIMIT_REGISTER PackageRaplLimitMsr;
  MSR_PACKAGE_POWER_SKU_REGISTER  PackagePowerSkuMsr;
  UINT16                          ConvertedPowerLimit1;
  UINT8                           ConvertedPowerLimit1Time;
  UINT64                          MchBar;
  UINT16                          PackageMaxPower;

  //
  // For ConfigTdp enabled skus
  //
  PackagePowerSkuMsr.Uint64  = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  PackageMaxPower = (UINT16) PackagePowerSkuMsr.Bits.PkgMaxPwr;
  ///
  /// Initialize the Power Limit 1/2 and Power Limit 2 enable bit in MSR
  ///  Power Limit 1: Turbo Power Limit MSR  [14:0] and Power Limit 2: Turbo Power Limit MSR  [46:32]
  ///
  if (ApplyConfigTdp && PackageMaxPower) {
    ConvertedPowerLimit1 = PackageMaxPower;
  } else {
    ConvertedPowerLimit1 = PowerLimit1;
  }

  ///
  /// Program Power Limit 1 (Long Duration Turbo) Time Window
  ///
  ConvertedPowerLimit1Time = GetConvertedTime (PowerLimitWindow * 1000);

  ///
  /// Get the MCH space base address.
  ///
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  ///
  /// Program MMIO PL1 clamp enable
  ///
  MmioOr32 (MchBar + MMIO_TURBO_POWER_LIMIT, PACKAGE_POWER_LIMIT_PKG_CLMP_LIM_1_MASK);

  ///
  ///  Configure Power Limit 1 (Long Duration Turbo) time windows: Turbo Power Limit MSR [23:17]
  ///
  PackageRaplLimitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_RAPL_LIMIT);
  PackageRaplLimitMsr.Bits.PkgPwrLim1Time = (UINT32) ConvertedPowerLimit1Time;
  PackageRaplLimitMsr.Bits.PkgPwrLim1En = 1;
  PackageRaplLimitMsr.Bits.PkgPwrLim2En = (PowerLimit2 == 0 ? 0 : 1);
  PackageRaplLimitMsr.Bits.PkgClmpLim1 = 1;
  PackageRaplLimitMsr.Bits.PkgPwrLim1 = (UINT32) ConvertedPowerLimit1;
  PackageRaplLimitMsr.Bits.PkgPwrLim2 = (UINT32) PowerLimit2;
  AsmWriteMsr64 (MSR_PACKAGE_RAPL_LIMIT, PackageRaplLimitMsr.Uint64);
}

/**
  This function returns Package TDP in Milli Watts

  @param[in] MsrTdp    - Package TDP from MSR

  @retval PackageTdp   - Package TDP in Milli Watts
**/
STATIC
UINT32
GetTdpInMilliWatt (
  IN UINT16 MsrTdp
  )
{
  MSR_PACKAGE_POWER_SKU_UNIT_REGISTER   PackagePowerSkuUnitMsr;

  PackagePowerSkuUnitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU_UNIT);

  return MsrTdp * 1000 / (1 << (UINT8) PackagePowerSkuUnitMsr.Bits.PwrUnit);
}

/**
  To prevent thermal issues, adjust Ctdp Level when user sets Power limit which is lower than Tdp of selected Ctdp level which can prevent potential thermal issue

  @param[in] ConfigTdpLevel               ConfigTdpLevel policy set by user
  @param[in] CustomPowerLimit1InMilliWatt Power limit in Milli Watts set by user

  @retval AdjustedCtdpLevel               Adjusted Ctdp level
**/
UINT8
AdjustConfigTdpLevel (
  IN UINT8  ConfigTdpLevel,
  IN UINT16 CustomPowerLimit1InMilliWatt
  )
{
  MSR_CONFIG_TDP_LEVEL1_REGISTER    ConfigTdpLevel1Msr;
  MSR_CONFIG_TDP_LEVEL2_REGISTER    ConfigTdpLevel2Msr;
  MSR_PACKAGE_POWER_SKU_REGISTER    PackagePowerMsr;
  UINT32                            TdpInMilliWatt[TDP_MAX_LEVEL];
  UINT8                             Index;
  UINT32                            TempTdpInMilliWatt;
  UINT8                             AdjustedCtdpLevel;

  ASSERT (CustomPowerLimit1InMilliWatt != 0);
  ASSERT (TDP_MAX_LEVEL == 3);

  ConfigTdpLevel1Msr.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL1);
  ConfigTdpLevel2Msr.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL2);
  PackagePowerMsr.Uint64    = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);

  TdpInMilliWatt[CONFIG_TDP_NOMINAL] = GetTdpInMilliWatt ((UINT16) PackagePowerMsr.Bits.PkgTdp);
  TdpInMilliWatt[CONFIG_TDP_LEVEL1]  = GetTdpInMilliWatt ((UINT16) ConfigTdpLevel1Msr.Bits.PkgTdp);
  TdpInMilliWatt[CONFIG_TDP_LEVEL2]  = GetTdpInMilliWatt ((UINT16) ConfigTdpLevel2Msr.Bits.PkgTdp);
  //
  // If TdpInMilliWatt[Index] is 0 (corresponding Ctdp Level is not supported), set it to MAX_UINT32
  //
  for (Index = 0; Index < TDP_MAX_LEVEL; Index++) {
    if (TdpInMilliWatt[Index] == 0) {
      TdpInMilliWatt[Index] = MAX_UINT32;
    }
  }

  //
  // If TdpInMilliWatt is lower than CustomPowerLimit1InMilliWatt, return ConfigTdpLevel
  //
  if (TdpInMilliWatt[ConfigTdpLevel] <= CustomPowerLimit1InMilliWatt) {
    return ConfigTdpLevel;
  }

  TempTdpInMilliWatt = 0;
  AdjustedCtdpLevel = ConfigTdpLevel;
  //
  // Find temp Tdp which is lower than CustomPowerLimit1InMilliWatt and higher than Tdps of other Ctdp levels and then get adjusted Ctdp Level
  //
  for (Index = 0; Index < TDP_MAX_LEVEL; Index++) {
    if ((TdpInMilliWatt[Index] > TempTdpInMilliWatt) && (TdpInMilliWatt[Index] <= CustomPowerLimit1InMilliWatt)) {
      TempTdpInMilliWatt = TdpInMilliWatt[Index];
      AdjustedCtdpLevel = Index;
    }
  }

  //
  // It's unexpected when CustomPowerLimit1InMilliWatt is lower than Tdp of all Ctdp Levels
  //
  if (TempTdpInMilliWatt == 0) {
    DEBUG ((DEBUG_ERROR, "ASSERT: Behavior is unexpected! CustomPowerLimit1 is lower than TDP of all Ctdp Levels\n"));
    ASSERT(TempTdpInMilliWatt == 0);
  }
  return AdjustedCtdpLevel;
}

/**
  Configure cTDP BIOS MSRs to Boot Ctdp values
    - Configures CONFIG_TDP_CONTROL MSR
    - Configures TURBO_ACTIVATION_RATIO MSR

  @param[in] CpuConfigTdpBootLevel   ConfigTdpBootLevel policy setting by user
  @param[in] ConfigTdpTar            Pointer to ConfigTdpTar
  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/
VOID
ConfigureCtdpLevelAndTar (
  IN UINT8                     CpuConfigTdpBootLevel,
  IN UINT8                     ConfigTdpTar,
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  MSR_CONFIG_TDP_CONTROL_REGISTER     ConfigTdpControlMsr;
  MSR_TURBO_ACTIVATION_RATIO_REGISTER TurboActivationRatioMsr;
  UINT8                               CpuConfigTdpBootRatio;
  UINT16                              CustomPowerLimit1InMilliWatt;

  ///
  /// Config TDP Boot settings
  ///
  CpuConfigTdpBootRatio = ConfigTdpTar + 1;
  ///
  /// Program the selected level 00:nominal,01:level1,10:level2 to
  /// CONFIG TDP CONTROL MSR.
  ///
  ConfigTdpControlMsr.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_CONTROL);
  if (ConfigTdpControlMsr.Bits.ConfigTdpLock == 0) {
    ConfigTdpControlMsr.Bits.TdpLevel = (UINT32) CpuConfigTdpBootLevel;
    CustomPowerLimit1InMilliWatt = CpuPowerDeliveryConfig->CustomPowerLimit1;
    ///
    /// To prevent thermal issues, adjust Ctdp Level when Power limit set by user
    /// is lower than Tdp of selected Ctdp level
    ///
    if (CustomPowerLimit1InMilliWatt != 0) {
      ConfigTdpControlMsr.Bits.TdpLevel = AdjustConfigTdpLevel (
        CpuConfigTdpBootLevel,
        CustomPowerLimit1InMilliWatt
      );
    }
    ///
    /// Lock Ctdp if Ctdp level won't be restored before RESET_CPL at Post-memory phase
    ///
    if ((CpuPowerDeliveryConfig->ConfigTdpLock == TRUE) && (ConfigTdpControlMsr.Bits.TdpLevel == CpuConfigTdpBootLevel)) {
      ConfigTdpControlMsr.Bits.ConfigTdpLock = 1;
      DEBUG ((DEBUG_INFO, "PPM:: ConfigTDP MSR_CONFIG_TDP_CONTROL is locked\n"));
    }
    AsmWriteMsr64 (MSR_CONFIG_TDP_CONTROL, ConfigTdpControlMsr.Uint64);
    DEBUG ((DEBUG_INFO, "PPM:: ConfigTDP MSR_CONFIG_TDP_CONTROL=0x%x\n", ConfigTdpControlMsr.Uint64));
  } else {
    DEBUG ((DEBUG_INFO, "PPM:: Could not write MSR_CONFIG_TDP_CONTROL\n"));
  }
  ///
  /// Program the max non-turbo ratio corresponding to default selected level
  /// in TURBO_ACTIVATION_RATIO MSR.
  ///
  TurboActivationRatioMsr.Uint64 = AsmReadMsr64 (MSR_TURBO_ACTIVATION_RATIO);
  if (TurboActivationRatioMsr.Bits.TurboActivationRatioLock == 0) {
    TurboActivationRatioMsr.Bits.MaxNonTurboRatio = (UINT32) (CpuConfigTdpBootRatio - 1);
    if (CpuPowerDeliveryConfig->ConfigTdpLock == TRUE) {
      TurboActivationRatioMsr.Bits.TurboActivationRatioLock = 1;
      DEBUG ((DEBUG_INFO, "PPM:: ConfigTDP MSR_TURBO_ACTIVATION_RATIO is locked\n"));
    }
    AsmWriteMsr64 (MSR_TURBO_ACTIVATION_RATIO, TurboActivationRatioMsr.Uint64);
    DEBUG ((DEBUG_INFO, "PPM:: ConfigTDP MSR_TURBO_ACTIVATION_RATIO=0x%x\n", TurboActivationRatioMsr.Uint64));
  } else {
    DEBUG ((DEBUG_INFO, "PPM:: Could not write MSR_TURBO_ACTIVATION_RATIO\n"));
  }
}

/**
  Configures the TURBO_POWER_LIMIT MMIO for Boot ConfigTdp Level

  @param[in] CpuConfigTdpBootLevel ConfigTdpBootLevel policy setting by user
  @param[in] ConfigTdpPowerLimit1  ConfigTdpPowerLimit1
  @param[in] ConfigTdpPowerLimit2  ConfigTdpPowerLimit2
  @param[in] PowerLimitWindow      PowerLimitWindow
**/
VOID
ConfigureMmioPowerLimits (
  IN UINT8                     CpuConfigTdpBootLevel,
  IN UINT16                    ConfigTdpPowerLimit1,
  IN UINT16                    ConfigTdpPowerLimit2,
  IN UINT8                     PowerLimitWindow
  )
{
  UINT64       MchBar;
  UINT32       Data32And;
  UINT32       Data32Or;
  MSR_PACKAGE_RAPL_LIMIT_REGISTER PackageRaplLimitMsr;

  ///
  /// Check if Power Limits are initalized
  ///
  if (ConfigTdpPowerLimit1 != 0 && ConfigTdpPowerLimit2 != 0) {
    ///
    /// Get the MCH space base address.
    /// Program Turbo Power Limit MMIO register MCHBAR+0x59A0 Bits [14:0] and [46:32]
    /// for ConfigTdp mode PL1 and PL2
    ///
    MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
    ///
    /// Read PowerLimit MSR
    ///
    PackageRaplLimitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_RAPL_LIMIT);
    ASSERT (PackageRaplLimitMsr.Bits.PkgPwrLim1Time == (UINT32) GetConvertedTime ((UINT32)PowerLimitWindow * 1000));

    ///
    /// Program cTDP Power Limit1
    ///
    Data32And = (UINT32) ~(CONFIG_TDP_LVL1_PKG_TDP_MASK);
    Data32Or  = (UINT32) (ConfigTdpPowerLimit1 | (PackageRaplLimitMsr.Uint32 & ~(CONFIG_TDP_LVL1_PKG_TDP_MASK)));
    MmioAndThenOr32 (MchBar + MMIO_TURBO_POWER_LIMIT, Data32And, Data32Or);
    ///
    /// Program cTDP Power Limit2
    ///
    Data32And = (UINT32) ~(CONFIG_TDP_LVL1_PKG_TDP_MASK);
    Data32Or  = (UINT32) (ConfigTdpPowerLimit2 | ((UINT32) RShiftU64 (PackageRaplLimitMsr.Uint64, 32) & ~(CONFIG_TDP_LVL1_PKG_TDP_MASK)));
    MmioAndThenOr32 (MchBar + MMIO_TURBO_POWER_LIMIT_HIGH, Data32And, Data32Or);
  }
}

/**
  Configures following fields of MSR 0x618 based on corresponding MMIO register (MCHBAR+0x58E0):
    Configures Long duration Turbo Mode (power limit 1) power level and time window for DDR domain
    Configures Short duration Turbo Mode (power limit 2) power level and time window for DDR domain
**/
VOID
ConfigureDdrPowerLimits (
  VOID
  )
{
  MSR_DDR_RAPL_LIMIT_REGISTER DdrRaplLimitMsr;

  UINT64         MchBar;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  DdrRaplLimitMsr.Uint64 = (MmioRead32 (MchBar + MMIO_DDR_RAPL_LIMIT) &~BIT0) + LShiftU64 (MmioRead32 (MchBar + MMIO_DDR_RAPL_LIMIT + 4), 32);

  DEBUG (
    (DEBUG_INFO,
     "DDR Power Limit 1 = %d\n",
     DdrRaplLimitMsr.Bits.Limit1Power)
    );
  DEBUG (
    (DEBUG_INFO,
     "DDR Power Limit 2 = %d\n",
     DdrRaplLimitMsr.Bits.Limit2Power)
    );

  AsmWriteMsr64 (MSR_DDR_RAPL_LIMIT, DdrRaplLimitMsr.Uint64);
}

/**
  Configures MSR 0x65C platform power limits (PSys)
    -Configures Platform Power Limit 1 Enable, power and time window
    -Configures Platform Power Limit 2 Enable, power
    -Platform power limits are limited by the Package Max and Min power

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/
VOID
ConfigurePlatformPowerLimits (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  UINT32                            ConvertedPowerLimit1;
  BOOLEAN                           ConvertedPowerLimit1En;
  UINT32                            ConvertedPowerLimit1Time;
  UINT32                            ConvertedPowerLimit2;
  BOOLEAN                           ConvertedPowerLimit2En;
  UINT16                            Multiplier;
  UINT32                            PsysPowerLimit1Time;
  CPU_SKU                           CpuSku;
  MSR_PLATFORM_POWER_LIMIT_REGISTER PlatformPowerLimitMsr;
  MSR_PACKAGE_POWER_SKU_REGISTER    PackagePowerSkuMsr;
  UINT16                            PackageTdp;

  ConvertedPowerLimit1            = 0;
  ConvertedPowerLimit1En          = FALSE;
  ConvertedPowerLimit1Time        = 0;
  ConvertedPowerLimit2            = 0;
  ConvertedPowerLimit2En          = FALSE;
  PsysPowerLimit1Time             = CpuPowerDeliveryConfig->PsysPowerLimit1Time;
  ///
  /// By default, for Mobile & Desktop Processors: Platform Power Limit 2 = 1.25 * Package TDP
  ///
  Multiplier = 125;
  PackagePowerSkuMsr.Uint64  = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  PackageTdp      = (UINT16) PackagePowerSkuMsr.Bits.PkgTdp;

  ///
  /// Initialize the Power Limit 1 and Power Limit 1 enable bit
  ///  - Power Limit 1: Platform Power Limit MSR  [14:0]
  ///  - Power Limit 1 Enable: Platform Power Limit MSR  [15]
  ///
  ///
  /// By default, program Power Limit 1 to Package TDP limit
  ///
  ConvertedPowerLimit1 = (UINT32) PackageTdp;
  CpuSku = GetCpuSku ();
  if (CpuPowerDeliveryConfig->PsysPowerLimit1Power != AUTO) {
    ConvertedPowerLimit1 = VerifyAndFixCustomPowerLimit (CpuPowerDeliveryConfig->PsysPowerLimit1Power, DefaultPowerUnit);
  }

  ///
  /// Update Platform Power Limit 1 enable bit
  ///
  if (CpuPowerDeliveryConfig->PsysPowerLimit1) {
    ConvertedPowerLimit1En = (BOOLEAN) CpuPowerDeliveryConfig->PsysPowerLimit1;
  } else {
    ///
    /// When we disable Platform Power Limit we need to write power limit = 0
    ///
    ConvertedPowerLimit1 = 0;
    ConvertedPowerLimit1En = 0;
  }

  ///
  /// Program Platform Power Limit 1 Time Window
  ///  If PlatformPowerLimit1Time is AUTO program default values
  ///
  if (CpuPowerDeliveryConfig->PsysPowerLimit1Time == AUTO) {
    if (EnumCpuUlx == CpuSku || EnumCpuUlt == CpuSku ||
        EnumCpuHalo == CpuSku) {
      ///
      /// For Mobile, default value is 28 seconds
      ///
      PsysPowerLimit1Time = MB_POWER_LIMIT1_TIME_DEFAULT;
    } else {
      ///
      /// For Desktop, default value is 1 second
      ///
      PsysPowerLimit1Time = DT_POWER_LIMIT1_TIME_DEFAULT;
    }
  }

  ConvertedPowerLimit1Time = (UINT32) GetConvertedTime (PsysPowerLimit1Time * 1000);

  ///
  ///  Initialize Short Duration Power limit and enable bit
  ///    Platform Power Limit 2: Platform Power Limit MSR (0x450h) [46:32]
  ///    Platform Power Limit 2 Enable: Platform Power Limit MSR (0x450h) [47]
  ///
  if (CpuPowerDeliveryConfig->PsysPowerLimit2Power == AUTO) {
    ConvertedPowerLimit2 = (UINT32) EFI_IDIV_ROUND ((Multiplier * PackageTdp), 100);
  } else {
    ConvertedPowerLimit2 = VerifyAndFixCustomPowerLimit (CpuPowerDeliveryConfig->PsysPowerLimit2Power, DefaultPowerUnit);
  }

  if (CpuPowerDeliveryConfig->PsysPowerLimit2) {
    ConvertedPowerLimit2En = (BOOLEAN) CpuPowerDeliveryConfig->PsysPowerLimit2;
  } else {
    ///
    /// When we disable Platform Power Limit we need to write power limit = 0
    ///
    ConvertedPowerLimit2 = 0;
    ConvertedPowerLimit2En = 0;
  }

  DEBUG (
          (DEBUG_INFO,
           "New Platform Power Limit 1  %d watt (%d in CPU power unit)\n",
           CpuPowerDeliveryConfig->PsysPowerLimit1Power,
           (UINT16) ConvertedPowerLimit1)
          );

  DEBUG (
          (DEBUG_INFO,
           "Platform Power Limit 2 Power = %d Watts (%d in CPU power unit)\n",
           CpuPowerDeliveryConfig->PsysPowerLimit2Power,
           (UINT16) ConvertedPowerLimit2)
          );

  PlatformPowerLimitMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_POWER_LIMIT);

  PlatformPowerLimitMsr.Bits.PowerLimit1 = ConvertedPowerLimit1;
  PlatformPowerLimitMsr.Bits.PowerLimit1En = (UINT32) ConvertedPowerLimit1En;
  //
  // Always set to allows processor to go below the OS requested P States.
  //
  PlatformPowerLimitMsr.Bits.CriticalPowerClamp1 = 1;
  PlatformPowerLimitMsr.Bits.PowerLimit1Time = ConvertedPowerLimit1Time;
  PlatformPowerLimitMsr.Bits.PowerLimit2 = ConvertedPowerLimit2;
  PlatformPowerLimitMsr.Bits.PowerLimit2En = (UINT32) ConvertedPowerLimit2En;

  AsmWriteMsr64 (MSR_PLATFORM_POWER_LIMIT, PlatformPowerLimitMsr.Uint64);

  ///
  /// Based on power source type update Psys telemetry it receives is in "Watts" or "% Capacity"
  ///
  ProgramTelemetryTypeforPsys (CpuPowerDeliveryConfig->PlatformAtxTelemetryUnit);
}

/**
  Configures PowerLimits and Config TDP values

  @param[in] ApplyConfigTdp             IsCtdpApplied
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
  )
{
  BOOLEAN DualTauBoostOverrideApplied;

  DualTauBoostOverrideApplied = FALSE;
  ///
  /// Configure CTDP power limits.Refer Rev 0.6.0 BWG sec 16.7.1: Enabling Intel Configurable TDP support
  ///
  if (IsXETdcTdpLimitSupported ()) {
    ConfigureMsrPowerLimits (ApplyConfigTdp, PowerLimit1, PowerLimit2, PowerLimitWindow);
  }

  //
  // Configure Dual Tau Boost
  //
  if (IsDualTauEnable (CpuPowerDeliveryConfig)) {
    DualTauBoostOverrideApplied = ConfigureDualTauBoostOverride (ApplyConfigTdp);
  }

  if (ApplyConfigTdp) {
    if (!DualTauBoostOverrideApplied) {
      ConfigureMmioPowerLimits (ConfigTdpBootModeIndex, PowerLimit1, PowerLimit2, PowerLimitWindow);
    }
    ConfigureCtdpLevelAndTar (ConfigTdpBootModeIndex, Tar, CpuPowerDeliveryConfig);
  }
}

/**
  Configures Isys Control MMIO register.
    -Configures Isys Platform Power Limit 1 Enable, current and time window
    -Configures Isys Platform Power Limit 2 Enable, current

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/

VOID
EFIAPI
ConfigureIsysControl (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  EFI_STATUS                     Status;
  SI_POLICY_PPI                  *SiPolicyPpi;
  UINT64                         MchBar;
  THETA_IBATT_MMIO_REGISTER      IsysControlRegister;

  SiPolicyPpi              = NULL;
  IsysControlRegister.Data = 0;

  Status = PeiServicesLocatePpi(&gSiPolicyPpiGuid, 0, NULL, (VOID**)&SiPolicyPpi);
  ASSERT_EFI_ERROR(Status);

  //
  // Program registers and Mchbar related to Isys Control registers.
  //
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  //
  //  If ThETA Ibatt Feature is Enable, configure below registers-
  //  BIT 14:0  - IsysCurrentLimitL1
  //  BIT 22:16 - IsysCurrentL1Tau
  //  BIT 46:32 - IsysCurrentLimitL2
  //

  if (CpuPowerDeliveryConfig->ThETAIbattEnable) {
    if (CpuPowerDeliveryConfig->IsysCurrentLimitL1Enable) {
      IsysControlRegister.Fields.IsysCurrentLimitL1 = CpuPowerDeliveryConfig->IsysCurrentLimitL1;
      IsysControlRegister.Fields.IsysCurrentL1Tau   = (UINT32) GetConvertedTime (CpuPowerDeliveryConfig->IsysCurrentL1Tau * 1000);
      if (!CpuPowerDeliveryConfig->AcDcPowerState) {
        IsysControlRegister.Fields.IsysCurrentLimitL1Enable = TRUE;
      } else {
        IsysControlRegister.Fields.IsysCurrentLimitL1Enable = FALSE;
      }
    }

    if (CpuPowerDeliveryConfig->IsysCurrentLimitL2Enable) {
      IsysControlRegister.Fields.IsysCurrentLimitL2 = CpuPowerDeliveryConfig->IsysCurrentLimitL2;
      if (!CpuPowerDeliveryConfig->AcDcPowerState) {
        IsysControlRegister.Fields.IsysCurrentLimitL2Enable = TRUE;
      } else {
        IsysControlRegister.Fields.IsysCurrentLimitL2Enable = FALSE;
      }
    }
    // Update Isys current L1,L2 and Tau for Isys register MMIO
    //
    MmioWrite64 ((MchBar + MMIO_ISYS_CONTROL_0_0_0_MCHBAR), IsysControlRegister.Data);
  }
}

/**
  Configures SoC firmware based skin temperature control MMIO register.
    - Configures these MMIO option to set 3 sensors: thresholds, gain, min perf
    - Controls setting enable/disable sensor temperature and ranges

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/

VOID
EFIAPI
ConfigureSkinTempControl (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  UINTN                            MchBar;
  UINTN                            Index;
  SKIN_TEMP_CONTROL_MMIO_REGISTER  SkinTempControlRegister[SKIN_TEMP_CONTROL_SENSOR];

  ///
  /// Get the MCH space base address.
  ///
  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  //
  //  Update skin control MMIO registers which provides control over limitation of CPU due to skin temperature
  //  All fields of skin temperature control MMIO registers are for 3 sensors
  //
  if (CpuPowerDeliveryConfig->SkinTempControl) {
    for (Index = 0; Index < SKIN_TEMP_CONTROL_SENSOR; Index++) {
      SkinTempControlRegister[Index].Data = 0;
      SkinTempControlRegister[Index].Fields.SkinTargetTemp = CpuPowerDeliveryConfig->SkinTargetTemp[Index];
      SkinTempControlRegister[Index].Fields.SkinTempControlEnable = CpuPowerDeliveryConfig->SkinTempControlEnable[Index];
      SkinTempControlRegister[Index].Fields.SkinControlLoopGain = CpuPowerDeliveryConfig->SkinControlLoopGain[Index];
      SkinTempControlRegister[Index].Fields.SkinTempOverrideEnable = CpuPowerDeliveryConfig->SkinTempOverrideEnable[Index];
      SkinTempControlRegister[Index].Fields.SkinMinPerformanceLevel = CpuPowerDeliveryConfig->SkinMinPerformanceLevel[Index];
      SkinTempControlRegister[Index].Fields.SkinTempOverride = CpuPowerDeliveryConfig->SkinTempOverride[Index];
    }

    //
    // Update skin control MMIO registers for 3 different sensors
    //
    MmioWrite32 ((MchBar + MMIO_PLATFORM_TEMP_CONTROL_0_0_0_0_MCHBAR), SkinTempControlRegister[0].Data);
    MmioWrite32 ((MchBar + MMIO_PLATFORM_TEMP_CONTROL_1_0_0_0_MCHBAR), SkinTempControlRegister[1].Data);
    MmioWrite32 ((MchBar + MMIO_PLATFORM_TEMP_CONTROL_2_0_0_0_MCHBAR), SkinTempControlRegister[2].Data);
  }
}