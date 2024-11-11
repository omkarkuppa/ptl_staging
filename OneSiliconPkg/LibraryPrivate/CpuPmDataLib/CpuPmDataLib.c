/** @file
  Source file for CpuPmDataLib.

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

#include <Base.h>
#include <Guid/CpuPmData.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuInfoFruLib.h>
#include <ConfigBlock/VoltageRegulator/CpuPowerMgmtVrConfig.h>
#include <Register/Cpuid.h>
#include <PiPei.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Register/GenerationMsr.h>
#include <Library/HobLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/PeiVrLib.h>

/**
  Get VrData from Fru layer

  @param[in] CpuIdentifier - Non-Zero value to override the Cpu Identifier used to look for CpuPM data
  @param[in] VrCount       - Count of List VrData
  @param[out] VrData       - VrData of the given CpuIdentifier

  @return RETURN_SUCCESS  if the Hob is created successfully
  @return others          if this function is not supported or implemented
**/
RETURN_STATUS
EFIAPI
GetVrOverridesFru (
  IN     UINT64                   CpuIdentifier,
  IN     UINTN                    VrCount,
  OUT    VOLTAGE_REGULATOR_PARAM  *VrData
  );

/**
  Create Guid Hob which contains CpuPM data

  @param[in] CpuIdentifier - Non-Zero value to override the Cpu Identifier used to look for CpuPM data

  @return RETURN_SUCCESS  if the Hob is created successfully
  @return others          if this function is not supported or implemented
**/
RETURN_STATUS
EFIAPI
CreateCpuPmDataHob (
  IN  UINTN  CpuIdentifier OPTIONAL
  )
{
  CPU_PM_DATA                          *BasicData;
  UINTN                                NoOfOverrides;
  UINTN                                PpmIndex;
  PPM_OVERRIDE_TABLE                   *PpmOverrideTable;
  EFI_STATUS                           Status;
  UINT16                               CpuDid;
  UINT32                               CpuFamilyModel;
  CPUID_VERSION_INFO_EAX               Eax;
  UINT16                               PackageTdp;
  MSR_PACKAGE_POWER_SKU_REGISTER       PackagePowerSku;
  MSR_PACKAGE_POWER_SKU_UNIT_REGISTER  PackagePowerSkuUnit;
  UINTN                                VrCount;
  UINT8                                *Hob;

  Hob = GetFirstGuidHob (&gCpuPmDataGuid);
  if (Hob != NULL) {
    return RETURN_ALREADY_STARTED;
  }

  VrCount = GetMaxNumVrsFru ();
  Hob = BuildGuidHob (&gCpuPmDataGuid, sizeof (VOLTAGE_REGULATOR_PARAM)*VrCount + sizeof (CPU_PM_DATA));
  ASSERT (Hob != NULL);
  if (Hob == NULL) {
    return RETURN_BUFFER_TOO_SMALL;
  }

  ZeroMem (Hob, sizeof (VOLTAGE_REGULATOR_PARAM)*VrCount + sizeof (CPU_PM_DATA));
  BasicData              = (CPU_PM_DATA *)Hob;
  BasicData->VrDataCount = (UINT32)VrCount;
  ///
  /// Find Package TDP value
  ///
  PackagePowerSkuUnit.Uint64 = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU_UNIT);
  PackagePowerSku.Uint64     = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  PackageTdp                 = (UINT16)PackagePowerSku.Bits.PkgTdp / (1 << (UINT8)PackagePowerSkuUnit.Bits.PwrUnit);

  ///
  /// Read the CPUID & DID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Eax.Uint32, NULL, NULL, NULL);
  CpuFamilyModel = Eax.Uint32 & CPUID_FULL_FAMILY_MODEL;
  CpuDid         =  (UINT16)GetHostBridgeRegisterData (HostBridgeDeviceId, HostBridgeDeviceIdData);

  BasicData->FamilyModel = CpuFamilyModel;
  BasicData->SaDid0      = CpuDid;
  BasicData->Tdp         = PackageTdp;
  BasicData->Type        = GetCpuSkuInfo (CpuFamilyModel, CpuDid);
  Status                 = GetCpuFusedCoreCountFru (0, (UINT8 *)&BasicData->PCoreCount, (UINT8 *)&BasicData->ECoreCount, (UINT8 *)&BasicData->LpECoreCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Cpu Identifier used for PM overrides
  ///
  if (CpuIdentifier == 0) {
    CpuIdentifier = GetCpuIdentifier ();
  }

  PpmOverrideTable = GetFruPowerLimits (&NoOfOverrides);
  for (PpmIndex = 0; PpmIndex < NoOfOverrides; PpmIndex++) {
    if ((PpmOverrideTable[PpmIndex].CpuIdentifier == CpuIdentifier) && (PpmOverrideTable[PpmIndex].Plx != NULL)) {
      break;
    }
  }

  if (PpmIndex == NoOfOverrides) {
    return RETURN_NOT_FOUND;
  }

  if (PpmOverrideTable[PpmIndex].Plx->MsrPowerLimit1 != 0) {
    BasicData->PmData.MsrPowerLimit1 = (UINT32)PpmOverrideTable[PpmIndex].Plx->MsrPowerLimit1;
  } else {
    BasicData->PmData.MsrPowerLimit1 = (UINT32)PackageTdp * 100;
  }
  BasicData->PmData.MsrPowerLimit2           = (UINT32) PpmOverrideTable[PpmIndex].Plx->MsrPowerLimit2;
  BasicData->PmData.CtdpUpPowerLimit2        = (UINT32) PpmOverrideTable[PpmIndex].Plx->CtdpUpPowerLimit2;
  BasicData->PmData.CtdpNominalPowerLimit2   = (UINT32) PpmOverrideTable[PpmIndex].Plx->CtdpNominalPowerLimit2;
  BasicData->PmData.CtdpDownPowerLimit2      = (UINT32) PpmOverrideTable[PpmIndex].Plx->CtdpDownPowerLimit2;
  BasicData->PmData.MsrPowerLimit4           = (UINT32) PpmOverrideTable[PpmIndex].Plx->MsrPowerLimit4;
  BasicData->PmData.MsrPowerLimit4DisableFvm = (UINT32) PpmOverrideTable[PpmIndex].Plx->MsrPowerLimit4DisableFvm;
  BasicData->PmData.MsrPowerLimit1TimeWindow = (UINT8) PpmOverrideTable[PpmIndex].Plx->MsrPowerLimit1TimeWindow;
  BasicData->PmData.IsysCurrentL1Tau         = (UINT8) PpmOverrideTable[PpmIndex].Plx->IsysCurrentL1Tau;
  BasicData->PmData.IsysCurrentLimitL1       = (UINT16) PpmOverrideTable[PpmIndex].Plx->IsysCurrentLimitL1;
  BasicData->PmData.IsysCurrentLimitL2       = (UINT16) PpmOverrideTable[PpmIndex].Plx->IsysCurrentLimitL2;
  BasicData->PmData.VsysMax                  = (UINT16) PpmOverrideTable[PpmIndex].Plx->VsysMax;
  BasicData->PmData.PsysPmax                 = (UINT16) PpmOverrideTable[PpmIndex].Plx->PsysPmax;

  GetVrOverridesFru (CpuIdentifier, BasicData->VrDataCount, (VOLTAGE_REGULATOR_PARAM *)BasicData->VrData);

  return RETURN_SUCCESS;
}
