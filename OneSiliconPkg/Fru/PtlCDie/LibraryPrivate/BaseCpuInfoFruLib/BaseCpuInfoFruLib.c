/** @file
  This file contains the Cpu Information for specific generation.

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

@par Specification
**/
#include <Register/CpuGenRegs.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Register/Cpuid.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuCommonLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Register/GenerationMsr.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/SmBios.h>
#include <Library/PcdLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/HobLib.h>
#include <OverclockingConfig.h>
#include <OcDataHob.h>
#include <Library/CpuLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Pi/PiMultiPhase.h>
#include <Protocol/MpService.h>
#include <CpuPowerManagementPrivateDataHob.h>
#include <Library/CpuMailboxLib.h>
#include <Library/FixedPointLib.h>
#include <Register/B2pMailbox.h>

STATIC CONST CHAR8 mCpuFamilyString[] = "PantherLake";
STATIC CONST CHAR8 mCpuFamilyStringWcl[] = "WildcatLake";
STATIC CHAR8 mPtlSocketTypeString[] = "BGA2540";
STATIC CHAR8 mWclSocketTypeString[] = "BGA1516";

#define NOCARE 0xFF

typedef struct {
  UINT32  CPUID;
  UINT8   SRID;
  CHAR8   String[16];
} CPU_REV;

GLOBAL_REMOVE_IF_UNREFERENCED CONST CPU_REV  mProcessorRevisionTable[] = {
  //
  //                        CPUID                              SRID      String
  //
  {CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE  + EnumPtlHA0,  NOCARE,   "A0"},
  {CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE  + EnumPtlUA0,  NOCARE,   "A0"},
  {CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE  + EnumPtlUHB0, NOCARE,   "B0"},
  {CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE  + EnumPtlUA1,  NOCARE,   "A1"},
};

//
// PTL PLX Profile
// Caution: The order of profile enum ID should match the order in profiles array.
//
enum {
  ProfilePtlU15W40,            // PTL U      15W 4+0
  ProfilePtlU15W20,            // PTL U      15W 2+0
  ProfilePtlH12Xe25W48,        // PTL H 12Xe 25W 4+8
  ProfilePtlH12Xe25W44,        // PTL H 12Xe 25W 4+4
  ProfilePtlH12Xe25W28,        // PTL H 12Xe 25W 2+8
  ProfilePtlH4Xe25W48,         // PTL H 4Xe  25W 4+8
  ProfilePtlH4Xe25W44,         // PTL H 4Xe  25W 4+4
  ProfilePtlH4Xe25W28,         // PTL H 4Xe  25W 2+8
  ProfilePtlH4Xe65W48Pd2,      // PTL H 4Xe  65W 4+8 (CTDP UP, PD2)
  ProfilePtlH12Xe65W48Pd2,     // PTL H 12Xe 65W 4+8 (CTDP UP, PD2)
  ProfilePtlH4Xe65W48Pd3,      // PTL H 4Xe  65W 4+8 (CTDP UP, PD3)
  ProfilePtlH12Xe65W48Pd3,     // PTL H 12Xe 65W 4+8 (CTDP UP, PD3)
  ProfilePtlMax
};

//
// WCL PLX Profile
// Caution: The order of profile enum ID should match the order in profiles array.
//
enum {
  ProfileWclU15W10,     // WCL U      15W 1+0
  ProfileWclU15W20,     // WCL U      15W 2+0
  ProfileWclMax
};

GLOBAL_REMOVE_IF_UNREFERENCED PPM_PLX_PROFILE mPtlPlx[] = {
  //
  // MSR    MSR    TdpUp  TdpNominal  TdpDown  MSR w/ Fvm  MSR w/o Fvm  TimeWindow  IsysL1Tau  IsysLimitL1  IsysLimitL2  VsysMax  PsysPmax
  // PL1    PL2     PL2       PL2      PL2        PL4         PL4        PL1            Sec        1/8A         1/8A       mV      1/8W
  {  2500,  5500,  5500,     5500,     5500,     16300,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlU15W40
  {  2500,  5500,  5500,     5500,     5500,     16300,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlU15W20
  {  2500,  6400,  6400,     6500,     6400,     17500,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlH12Xe25W48
  {  2500,  6500,  6500,     6500,     6500,     16000,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlH12Xe25W44
  {  2500,  6500,  6500,     6500,     6500,     16000,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlH12Xe25W28
  {  2500,  6400,  6400,     6500,     6400,     17500,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlH4Xe25W48
  {  2500,  6500,  6500,     6500,     6500,     16000,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlH4Xe25W44
  {  2500,  6500,  6500,     6500,     6500,     16000,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlH4Xe25W28
  {  6500,  8000,  8000,     8000,     8000,     23900,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlH4Xe65W48Pd2
  {  6500,  8000,  8000,     8000,     8000,     23900,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlH12Xe65W48Pd2
  {  6500,  9500,  9500,     9500,     9500,     23900,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlH4Xe65W48Pd3
  {  6500,  9500,  9500,     9500,     9500,     23900,        0,        28,            28,         48,         120,     24000,    2800}, // ProfilePtlH12Xe65W48Pd3
};
//
// Catch the mismatch of number of profile enum IDs and number of profiles.
// But the order mismatch cannot be caught.
//
STATIC_ASSERT (ARRAY_SIZE (mPtlPlx) == ProfilePtlMax, "PPM Profile count mismatch!");

GLOBAL_REMOVE_IF_UNREFERENCED PPM_PLX_PROFILE mWclPlx[] = {
  //
  // MSR    MSR    TdpUp  TdpNominal  TdpDown  MSR w/ Fvm  MSR w/o Fvm  TimeWindow  IsysL1Tau  IsysLimitL1  IsysLimitL2  VsysMax  PsysPmax
  // PL1    PL2     PL2       PL2      PL2        PL4         PL4        PL1            Sec        1/8A         1/8A       mV      1/8W
  {  1500,     0,     0,        0,        0,        0,        0,         28,            28,         48,         120,     24000,    0},  // ProfileWclU15W10
  {  2500,  3500,  3500,     3500,     3500,     4400,        0,         28,            28,         48,         120,     24000,    0}   // ProfileWclU15W20
};
//
// Catch the mismatch of number of profile enum IDs and number of profiles.
// But the order mismatch cannot be caught.
//
STATIC_ASSERT (ARRAY_SIZE (mWclPlx) == ProfileWclMax, "PPM Profile count mismatch!");


///
/// PowerLimits Override table for all SKUs. Non-cTDP parts would have '0' data for TDP level information.
///
GLOBAL_REMOVE_IF_UNREFERENCED PPM_OVERRIDE_TABLE mPowerLimitsOverrideTable[] = {
  //
  //  Identifier                     PLx
  //
  //
  //  PTL U, H 12Xe/4Xe
  //
  { EnumPtlU15Watt40CpuId,           &mPtlPlx[ProfilePtlU15W40]           },
  { EnumPtlU15Watt20CpuId,           &mPtlPlx[ProfilePtlU15W20]           },
  { EnumPtlH12Xe25Watt48CpuId,       &mPtlPlx[ProfilePtlH12Xe25W48]       },
  { EnumPtlH12Xe25Watt44CpuId,       &mPtlPlx[ProfilePtlH12Xe25W44]       },
  { EnumPtlH12Xe25Watt28CpuId,       &mPtlPlx[ProfilePtlH12Xe25W28]       },
  { EnumPtlH4Xe25Watt48CpuId,        &mPtlPlx[ProfilePtlH4Xe25W48]        },
  { EnumPtlH4Xe25Watt44CpuId,        &mPtlPlx[ProfilePtlH4Xe25W44]        },
  { EnumPtlH4Xe25Watt28CpuId,        &mPtlPlx[ProfilePtlH4Xe25W28]        },
  { EnumPtlH4Xe65Watt48Pd2CpuId,     &mPtlPlx[ProfilePtlH4Xe65W48Pd2]     },
  { EnumPtlH12Xe65Watt48Pd2CpuId,    &mPtlPlx[ProfilePtlH12Xe65W48Pd2]    },
  { EnumPtlH4Xe65Watt48Pd3CpuId,     &mPtlPlx[ProfilePtlH4Xe65W48Pd3]     },
  { EnumPtlH12Xe65Watt48Pd3CpuId,    &mPtlPlx[ProfilePtlH12Xe65W48Pd3]    },
  ///
  /// WCL U
  ///
  { EnumWclU15Watt10CpuId,     &mWclPlx[ProfileWclU15W10] },
  { EnumWclU15Watt20CpuId,     &mWclPlx[ProfileWclU15W20] }
};

typedef struct {
  CPU_SKU   CpuSku;
  UINT32    CpuIdFamilyModel;
  UINT16    SaDeviceId;
  UINT8     BigCoreCount;
  UINT8     AtomCoreCount;
} CPU_SKU_INFO;

CPU_SKU_INFO mCpuSkuInfo[] = {
  //
  // CpuSku,    CpuIdFamilyModel,    SaDeviceId,    BigCoreCount,    AtomCoreCount
  //
  //
  // PTL U, H 4Xe/12Xe
  //
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_U_SA_DEVICE_ID_4C,             4, 0},  // PTL U      4+0
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_U_SA_DEVICE_ID_2C,             2, 0},  // PTL U      2+0
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_12XE_SA_DEVICE_ID_4C_8A,     4, 8},  // PTL H 12Xe 4+8
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_12XE_SA_DEVICE_ID_4C_4A,     4, 4},  // PTL H 12Xe 4+4
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_12XE_SA_DEVICE_ID_2C_8A,     2, 8},  // PTL H 12Xe 2+8
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_12XE_HH_SA_DEVICE_ID_2C_8A,  2, 8},  // PTL H 12Xe 2+8 (HH)
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_12XE_SA_DEVICE_ID_2C_4A,     2, 4},  // PTL H 12Xe 2+4
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_4XE_SA_DEVICE_ID_4C_8A,      4, 8},  // PTL H 4Xe  4+8
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_4XE_SA_DEVICE_ID_4C_4A,      4, 4},  // PTL H 4Xe  4+4
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_4XE_SA_DEVICE_ID_2C_8A,      2, 8},  // PTL H 4Xe  2+8
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_4XE_SA_DEVICE_ID_2C_4A,      2, 4},  // PTL H 4Xe  2+4
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_4XE_SA_DEVICE_ID_4C,         4, 0},  // PTL H      4+0
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_U_NEX_SA_DEVICE_ID_4C,         4, 0},  // PTL U      4+0 (NEX)
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_U_NEX_SA_DEVICE_ID_2C,         2, 0},  // PTL U      2+0 (NEX)
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_12XE_NEX_SA_DEVICE_ID_4C_8A, 4, 8},  // PTL H 12Xe 4+8 (NEX)
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_12XE_NEX_SA_DEVICE_ID_4C_4A, 4, 4},  // PTL H 12Xe 4+4 (NEX)
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_12XE_NEX_SA_DEVICE_ID_2C_8A, 2, 8},  // PTL H 12Xe 2+8 (NEX)
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_12XE_NEX_SA_DEVICE_ID_2C_4A, 2, 4},  // PTL H 12Xe 2+4 (NEX)
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_4XE_NEX_SA_DEVICE_ID_4C_8A,  4, 8},  // PTL H 4Xe  4+8 (NEX)
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_4XE_NEX_SA_DEVICE_ID_4C_4A,  4, 4},  // PTL H 4Xe  4+4 (NEX)
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_4XE_NEX_SA_DEVICE_ID_2C_8A,  2, 8},  // PTL H 4Xe  2+8 (NEX)
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE, PTL_H_4XE_NEX_SA_DEVICE_ID_2C_4A,  2, 4},  // PTL H 4Xe  2+4 (NEX)


  //
  // WCL
  //
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_WILDCATLAKE_MOBILE, WCL_SA_DEVICE_ID_2C_4LP,      2, 0}, // WCL
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_WILDCATLAKE_MOBILE, WCL_SA_DEVICE_ID_2C_2LP,      2, 0}, // WCL
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_WILDCATLAKE_MOBILE, WCL_SA_DEVICE_ID_2C,          2, 0}, // WCL
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_WILDCATLAKE_MOBILE, WCL_SA_DEVICE_ID_1C_4LP,      1, 0}, // WCL
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_WILDCATLAKE_MOBILE, WCL_SA_DEVICE_ID_4LP,         0, 0}, // WCL
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_WILDCATLAKE_MOBILE, WCL_SA_DEVICE_ID_2C_4LP_NEX,  2, 0}, // WCL
  { EnumCpuUlt, CPUID_FULL_FAMILY_MODEL_WILDCATLAKE_MOBILE, WCL_SA_DEVICE_ID_1C_4LP_NEX,  1, 0}  // WCL
};

/**
  Return CPU Sku

  @param[in]  UINT32             CpuFamilyModel
  @param[in]  UINT16             CpuDid

  @retval     UINT8              CPU Sku
**/
UINT8
EFIAPI
GetCpuSkuInfo (
  IN UINT32 CpuFamilyModel,
  IN UINT16 CpuDid
  )
{
  UINTN              Index;

  for (Index = 0; Index < ARRAY_SIZE (mCpuSkuInfo); Index++) {
    if ((CpuFamilyModel == mCpuSkuInfo[Index].CpuIdFamilyModel) && (CpuDid == mCpuSkuInfo[Index].SaDeviceId)) {
      return mCpuSkuInfo[Index].CpuSku;
    }
  }

  DEBUG ((DEBUG_ERROR, "Unsupported CPU SKU, Device ID: 0x%04X, CPUID: 0x%08X!\n", CpuDid, CpuFamilyModel));
  ASSERT (FALSE);

  return EnumCpuUnknown;
}

/**
  This function returns the HUB DIE ID.

  @param[out] *HubDieId             - variable that will store the DIE ID for HUB DIE.

  @retval     EFI_NOT_FOUND         No Hub DIE in current product
**/
EFI_STATUS
EFIAPI
GetHubDieIdFru (
  OUT UINT8 *HubDieId
){
  return EFI_NOT_FOUND;
}

/**
  This function returns the fused cores count for specific DIE.

  @param[in]  DieId                        - DIE ID.
  @param[out] *NumberOfSupportedCores      - variable that will store fuse P cores in current DIE.
  @param[out] *NumberOfSupportedAtomCores  - variable that will store fuse E cores in current DIE.
  @param[out] *NumberOfSupportedAtomCores  - variable that will store fuse LP cores in current DIE.

  @retval     EFI_SUCCESS      Get the fused core count successfully.
  @retval     EFI_NOT_FOUND    Current DIE ID doesn't exist in fuse config
  @retval     EFI_UNSUPPORTED  Doesn't support to get the fused core count.
**/
EFI_STATUS
EFIAPI
GetCpuFusedCoreCountFru (
  IN  UINT8                     DieId,
  OUT UINT8                     *NumberOfFusedPCore,     OPTIONAL
  OUT UINT8                     *NumberOfFusedECore,     OPTIONAL
  OUT UINT8                     *NumberOfFusedLpECore    OPTIONAL
  )
{
  UINTN                   Index;
  UINT16                  SaDeviceId;
  UINT32                  CpuIdFamilyModel;
  CPUID_VERSION_INFO_EAX  Eax;

  if (DieId != 0) {
    return EFI_NOT_FOUND;
  }

  if ((NumberOfFusedPCore == NULL) && (NumberOfFusedECore == NULL) && (NumberOfFusedLpECore == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  ///
  /// Read DID information
  ///
  SaDeviceId = (UINT16) GetHostBridgeRegisterData (HostBridgeDeviceId, HostBridgeDeviceIdData);
  if (NumberOfFusedLpECore != NULL) {
  //
  // PTL always has fuse 4 LP cores
  //
   *NumberOfFusedLpECore = 4;
    if (GetCpuFamilyModel () == CPUID_FULL_FAMILY_MODEL_WILDCATLAKE_MOBILE) {
      if (SaDeviceId == WCL_SA_DEVICE_ID_2C_2LP) {
        *NumberOfFusedLpECore = 2;
      } else if (SaDeviceId == WCL_SA_DEVICE_ID_2C) {
        *NumberOfFusedLpECore = 0;
      } else {
        *NumberOfFusedLpECore = 4;
      }
    }
  }

  ///
  /// Read CPUID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Eax.Uint32, NULL, NULL, NULL);
  CpuIdFamilyModel = Eax.Uint32 & CPUID_FULL_FAMILY_MODEL;

  for (Index = 0; Index < ARRAY_SIZE (mCpuSkuInfo); Index++) {
    if ((CpuIdFamilyModel == mCpuSkuInfo[Index].CpuIdFamilyModel) && (SaDeviceId == mCpuSkuInfo[Index].SaDeviceId)) {
      if (NumberOfFusedPCore != NULL) {
        *NumberOfFusedPCore = mCpuSkuInfo[Index].BigCoreCount;
      }
      if (NumberOfFusedECore != NULL) {
        *NumberOfFusedECore = mCpuSkuInfo[Index].AtomCoreCount;
      }
      return EFI_SUCCESS;
    }
  }
  DEBUG ((DEBUG_ERROR, "Unsupported CPU SKU, Device ID: 0x%04X, CPUID: 0x%08X!\n", SaDeviceId, CpuIdFamilyModel));
  ASSERT (FALSE);

  return EFI_UNSUPPORTED;
}

/**
  Get the fused CDIE list.
  Caller need to prepare the buffer and pass the list count in the first parameter

  @param[in,out]  NumberOfFusedCDie   - As input, it is the max list count of CDieIds.
                                      - As outout, it is the number of fused CDIES
  @param[out]     *CDieIds            - Output CDie ID list.

  @retval     EFI_SUCCESS             Get the fused CDIE list successfully.
  @retval     EFI_INVALID_PARAMETER   Input parameter is invalid
  @retval     EFI_BUFFER_TOO_SMALL    Input list count is too small
  @retval     EFI_UNSUPPORTED         Doesn't support to get the CDIE list.
**/
EFI_STATUS
EFIAPI
GetFusedCDieIdsFru (
  IN OUT UINTN                    *NumberOfFusedCDie,
     OUT UINT8                    *CDieIds
  )
{
  if ((NumberOfFusedCDie == NULL) || (CDieIds == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (*NumberOfFusedCDie <1){
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // PTL only has one CDIE, and DIE is 0.
  //
  *NumberOfFusedCDie = 1;
  CDieIds[0] = 0;
  return EFI_SUCCESS;
}

/**
  This function is used to get enabled LP Atom cores and threads off ring

  @param[in]  *ProcessorInfo         - Pointer to EFI_PROCESSOR_INFORMATION
  @param[in]  NumberOfProcessors     - Number of processors
  @param[out] *NumberOfLpAtomCores   - variable that will store enabled cores off ring
  @param[out] *NumberOfLpAtomThreads - variable that will store enabled threads off ring
**/
VOID
EFIAPI
GetLpECoreThreadCountInfoFru (
  IN  CONST EFI_PROCESSOR_INFORMATION *ProcessorInfo,
  IN  UINTN                           NumberOfProcessors,
  OUT UINT8                           *NumberOfLpAtomCores,   OPTIONAL
  OUT UINT8                           *NumberOfLpAtomThreads  OPTIONAL
  )
{
  UINTN  Index;

  if (NumberOfLpAtomCores != NULL) {
    *NumberOfLpAtomCores = 0;
  }
  if (NumberOfLpAtomThreads != NULL) {
    *NumberOfLpAtomThreads = 0;
  }

  for (Index = 0; Index < NumberOfProcessors; Index++) {
    //
    // There are 4 DKT in NOC, BIOS uses ApicId(0x40/0x42/0x44/0x46) to detect the enabled threads in NOC.
    //
    switch (ProcessorInfo[Index].ProcessorId) {
      case 0x40:
      case 0x42:
      case 0x44:
      case 0x46:
        if (NumberOfLpAtomCores != NULL) {
          (*NumberOfLpAtomCores) ++;
        }
        if (NumberOfLpAtomThreads != NULL) {
          (*NumberOfLpAtomThreads) ++;
        }
        break;
      default:
        break;
    }
  }

  if (NumberOfLpAtomCores != NULL) {
    DEBUG ((DEBUG_INFO, "Total enabled low performance Atom Cores - 0x%x\n", *NumberOfLpAtomCores));
  }
  if (NumberOfLpAtomThreads != NULL) {
    DEBUG ((DEBUG_INFO, "Total enabled low performance Atom Threads - 0x%x\n", *NumberOfLpAtomThreads));
  }
  return;
}

/**
  Returns Generation string of the respective CPU

  @param[in]   CpuFamilyId

  @retval      Character pointer of Generation string
**/
CONST CHAR8*
EFIAPI
GetFruGenerationString (
  IN   UINT32   CpuFamilyId
  )
{
  switch (CpuFamilyId) {
    case CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE:
      return mCpuFamilyString;
    case CPUID_FULL_FAMILY_MODEL_WILDCATLAKE_MOBILE:
      return mCpuFamilyStringWcl;
    default:
      return NULL;
  }
}

/**
  Return MCP stepping based on compute die SRID
  @retval  MCP stepping
**/
UINT8
McpGetStepping (
  VOID
  )
{
   ///
    /// WCL Package M Steppings
    ///
    /*
    |  MCP Step   |  MCP Step  | FUSE_SRID   |
    |  InternaL   |  External  |  Compute    |
    +-------------+------------------------- +
    |  WCL A0     |   WCL A0   |     0x0     |
    |  WCL A1     |   WCL A1   |     0x1     |
    |  WCL A4     |   WCL A1   |     0x2     |
    |  WCL B0     |   WCL B0   |     0x4     |
    */

  //
  // Read SRID from HostBridge (0 / 0 / 0)
  //
    return PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0) + PCI_REVISION_ID_OFFSET) & 0xF;
}

/**
  Returns Revision Table string

  @param[in]   CpuId

  @retval      Character pointer of Revision Table string
**/
CONST CHAR8*
EFIAPI
GetRevisionTableString (
  UINT32                CpuId
  )
{
  UINTN                 Index = 0;
  UINTN                 Count;
  UINT8                 SrId;

  SrId = McpGetStepping ();

  Count = ARRAY_SIZE (mProcessorRevisionTable);

  if ((CpuId == CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE + 2) && 
      ((SrId == 0xB) || (SrId == 0xC) || (SrId == 0xE) || (SrId == 0xF))) {
    return "A0";
  }
  
  for (Index = 0; Index < Count; Index++) {
    if ((CpuId == mProcessorRevisionTable[Index].CPUID) &&
        ((SrId == mProcessorRevisionTable[Index].SRID) || (mProcessorRevisionTable[Index].SRID == NOCARE))) {
      return mProcessorRevisionTable[Index].String;
    }
  }
  return NULL;
}

/**
  Clear all MCA bank status.
**/
VOID
EFIAPI
McaBankStatusClear (
  VOID
  )
{
}

/**
  Returns power limits Table

  @param[in]      NoOfOverrides

  @retval         override table pointer of power limits Table
**/
PPM_OVERRIDE_TABLE*
EFIAPI
GetFruPowerLimits (
  IN UINTN                   *NoOfOverrides
  )
{
  *NoOfOverrides = ARRAY_SIZE (mPowerLimitsOverrideTable);
  return mPowerLimitsOverrideTable;
}

/**
  Return CPU Identifier used to identify various CPU types

  @param[in]  CPU_SKU                       CpuSku
  @param[in]  CPU_STEPPING                  CpuStepping

  @retval CPU_IDENTIFIER           CPU Identifier
**/
CPU_IDENTIFIER
EFIAPI
GetCpuSkuIdentifier (
  IN  CPU_SKU         CpuSku,
  IN  CPU_STEPPING    CpuStepping
  )
{
  UINT16                                PackageTdp;
  MSR_PACKAGE_POWER_SKU_REGISTER        PackagePowerSku;
  MSR_PACKAGE_POWER_SKU_UNIT_REGISTER   PackagePowerSkuUnit;
  CPU_IDENTIFIER                        CpuIdentifier;
  UINT16                                CpuDid;

  ///
  /// Find Package TDP value in 1/100 Watt units
  ///
  PackagePowerSkuUnit.Uint64 = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU_UNIT);
  PackagePowerSku.Uint64     = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  PackageTdp = (UINT16) PackagePowerSku.Bits.PkgTdp * 100 / (1 << (UINT8)PackagePowerSkuUnit.Bits.PwrUnit);

  CpuIdentifier = EnumUnknownCpuId;
  CpuDid = (UINT16) GetHostBridgeRegisterData (HostBridgeDeviceId, HostBridgeDeviceIdData);

  DEBUG ((DEBUG_INFO, "GetCpuIdentifier () - CpuSku                  = 0x%X\n", CpuSku));
  DEBUG ((DEBUG_INFO, "                    - PackageTdp (1/100 Watt) = %d\n", PackageTdp));
  DEBUG ((DEBUG_INFO, "                    - CpuStepping             = %d\n", CpuStepping));
  DEBUG ((DEBUG_INFO, "                    - CpuDID                  = 0x%X\n", CpuDid));

  ///
  /// Logic to determine the CPU Identifier
  ///
  switch (CpuSku) {
    case EnumCpuUlt:
      switch (CpuDid) {
        ///
        /// PTL Package U
        ///
        case PTL_U_SA_DEVICE_ID_4C:
        case PTL_U_NEX_SA_DEVICE_ID_4C:
          if (PackageTdp == CPU_TDP_15_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-U 4+0 15W\n"));
            CpuIdentifier = EnumPtlU15Watt40CpuId;
          } else if (PackageTdp == CPU_TDP_25_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-U 4+0 25W\n"));
            CpuIdentifier = EnumPtlU15Watt40CpuId;
          }
          break;
        case PTL_U_SA_DEVICE_ID_2C:
        case PTL_U_NEX_SA_DEVICE_ID_2C:
          if (PackageTdp == CPU_TDP_15_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-U 2+0 15W\n"));
            CpuIdentifier = EnumPtlU15Watt20CpuId;
          } else if (PackageTdp == CPU_TDP_25_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-U 2+0 25W\n"));
            CpuIdentifier = EnumPtlU15Watt20CpuId;
          }
          break;
        ///
        /// PTL Package H 12Xe
        ///
        case PTL_H_12XE_SA_DEVICE_ID_4C_8A:
        case PTL_H_12XE_NEX_SA_DEVICE_ID_4C_8A:
          if (PackageTdp == CPU_TDP_28_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 12Xe 4+8 28W\n"));
            CpuIdentifier = EnumPtlH12Xe25Watt48CpuId;
          } else if (PackageTdp == CPU_TDP_25_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 12Xe 4+8 25W\n"));
            CpuIdentifier = EnumPtlH12Xe25Watt48CpuId;
          }
          break;
        case PTL_H_12XE_SA_DEVICE_ID_4C_4A:
        case PTL_H_12XE_NEX_SA_DEVICE_ID_4C_4A:
          if (PackageTdp == CPU_TDP_45_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 12Xe 4+4 28W\n"));
            CpuIdentifier = EnumPtlH12Xe25Watt44CpuId;
          } else if (PackageTdp == CPU_TDP_25_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 12Xe 4+4 25W\n"));
            CpuIdentifier = EnumPtlH12Xe25Watt44CpuId;
          }
          break;
        case PTL_H_12XE_SA_DEVICE_ID_2C_8A:
        case PTL_H_12XE_NEX_SA_DEVICE_ID_2C_8A:
          if (PackageTdp == CPU_TDP_28_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 12Xe 2+8 28W\n"));
            CpuIdentifier = EnumPtlH12Xe25Watt28CpuId;
          } else if (PackageTdp == CPU_TDP_25_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 12Xe 2+8 25W\n"));
            CpuIdentifier = EnumPtlH12Xe25Watt28CpuId;
          }
          break;
        case PTL_H_12XE_HH_SA_DEVICE_ID_2C_8A:
          if (PackageTdp == CPU_TDP_28_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 12Xe 2+8 HH 28W\n"));
            CpuIdentifier = EnumPtlH12Xe25Watt28CpuId;
          } else if (PackageTdp == CPU_TDP_25_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 12Xe 2+8 HH 25W\n"));
            CpuIdentifier = EnumPtlH12Xe25Watt28CpuId;
          }
          break;
        case PTL_H_12XE_SA_DEVICE_ID_2C_4A:
        case PTL_H_12XE_NEX_SA_DEVICE_ID_2C_4A:
          if (PackageTdp == CPU_TDP_28_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 12Xe 2+4 28W, replace it with PTL Pkg-H 12Xe 2+8 28W\n"));
            CpuIdentifier = EnumPtlH12Xe25Watt28CpuId;
          }
          break;
        ///
        /// PTL Package H 4Xe
        ///
        case PTL_H_4XE_SA_DEVICE_ID_4C_8A:
        case PTL_H_4XE_NEX_SA_DEVICE_ID_4C_8A:
          if (PackageTdp == CPU_TDP_45_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 4Xe 4+8 45W\n"));
            CpuIdentifier = EnumPtlH4Xe25Watt48CpuId;
          } else if (PackageTdp == CPU_TDP_25_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 4Xe 4+8 25W\n"));
            CpuIdentifier = EnumPtlH4Xe25Watt48CpuId;
          }
          break;
        case PTL_H_4XE_SA_DEVICE_ID_4C_4A:
        case PTL_H_4XE_NEX_SA_DEVICE_ID_4C_4A:
          if (PackageTdp == CPU_TDP_45_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 4Xe 4+4 45W\n"));
            CpuIdentifier = EnumPtlH4Xe25Watt44CpuId;
          } else if (PackageTdp == CPU_TDP_25_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 4Xe 4+4 25W\n"));
            CpuIdentifier = EnumPtlH4Xe25Watt44CpuId;
          }
          break;
        case PTL_H_4XE_SA_DEVICE_ID_2C_8A:
        case PTL_H_4XE_NEX_SA_DEVICE_ID_2C_8A:
          if (PackageTdp == CPU_TDP_45_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 4Xe 2+8 45W\n"));
            CpuIdentifier = EnumPtlH4Xe25Watt28CpuId;
          } else if (PackageTdp == CPU_TDP_25_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 4Xe 2+8 25W\n"));
            CpuIdentifier = EnumPtlH4Xe25Watt28CpuId;
          }
          break;
        case PTL_H_4XE_SA_DEVICE_ID_2C_4A:
        case PTL_H_4XE_NEX_SA_DEVICE_ID_2C_4A:
          if (PackageTdp == CPU_TDP_45_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 4Xe 2+4 45W, replace it with PTL Pkg-H 4Xe 2+8 45W\n"));
            CpuIdentifier = EnumPtlH4Xe25Watt28CpuId;
          }
          break;
        case PTL_H_4XE_SA_DEVICE_ID_4C:
          if (PackageTdp == CPU_TDP_15_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 4+0 15W\n"));
            CpuIdentifier = EnumPtlU15Watt40CpuId;
          } else if (PackageTdp == CPU_TDP_25_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = PTL Pkg-H 4+0 25W\n"));
            CpuIdentifier = EnumPtlU15Watt40CpuId;
          }
          break;
        ///
        /// WCL U
        ///
        case WCL_SA_DEVICE_ID_2C_4LP:
          if (PackageTdp == CPU_TDP_15_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = WCL Pkg-U 2+0 15W\n"));
            CpuIdentifier = EnumWclU15Watt20CpuId;
          }
          break;
        case WCL_SA_DEVICE_ID_1C_4LP:
          if (PackageTdp == CPU_TDP_15_WATTS) {
            DEBUG ((DEBUG_INFO, "CPU Identifier = WCL Pkg-U 1+0 15W\n"));
            CpuIdentifier = EnumWclU15Watt10CpuId;
          }
          break;
        default:
          break;
      }
      break;

    case EnumCpuUlx:
    case EnumCpuHalo:
    case EnumCpuTrad:
      break;


    default:
      CpuIdentifier = EnumUnknownCpuId;
      break;
    }

  return CpuIdentifier;
}

/**
  This function is used to Patch SmmSupovrStateLock.

  @retval This corresponds to bit 2 of MSR_SMM_SUPOVR_STATE_LOCK_REGISTER. When set, prevents WRMSR to IA32_SMM_MONITOR_CTL (aka MSEG) MSR.

**/
BOOLEAN
EFIAPI
SmmSupovrStateLockPatch (
  VOID
  )
{
  return 1;
}

/**
  This function returns the supported Physical Address Size

  @retval returns the supported Physical Address Size.
**/
UINT8
EFIAPI
GetMaxPhysicalAddressSizeFru (
  VOID
  )
{
  //
  // Even though CPUID Leaf CPUID_VIR_PHY_ADDRESS_SIZE (0x80000008) MAX_PA will report 46,
  // BIOS will return Memory expansion of 42 bit (0 - 41) and MKTME key is in high 4 bits (42 - 45)
  //
  return 42;
}

/**
  Check Overclocking Enabled or Disabled

  @retval TRUE   OC is enabled.
  @retval FALSE  OC is disabled.
**/
BOOLEAN
EFIAPI
IsOcEnabled (
  VOID
  )
{
  VOID                  *Hob;
  OC_DATA_HOB           *OcDataHob;

  Hob = GetFirstGuidHob (&gOcDataHobGuid);
  if (Hob != NULL) {
    OcDataHob = GET_GUID_HOB_DATA (Hob);
    if ((OcDataHob != NULL) && (OcDataHob->OverclockingEnabled == 1)) {
      DEBUG((DEBUG_INFO, "(OC) OC is Enabled\n"));
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Return IPC scaling * 100 per processor.

  @param[in]  ProcessorInfo      Pointer to EFI_PROCESSOR_INFORMATION
  @param[out] IpcScaling         Pointer to each processor's IpcScaling
  @param[in]  NumberOfProcessors Number of processors
**/
VOID
EFIAPI
GetIpcScalingFru (
  IN  CONST EFI_PROCESSOR_INFORMATION  *ProcessorInfo,
  OUT   UINT8                          *IpcScaling,
  IN        UINTN                      NumberOfProcessors
  )
{
  UINTN                                 Index;
  EFI_STATUS                            Status;
  PCODE_MAILBOX_INTERFACE               MailboxCommand;
  UINT32                                MailboxStatus;
  MAILBOX_DATA_FORMAT_SCALING_FACTOR    ScalingFactorData;
  UINT64                                ScalingFactor;

  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.Command = MAILBOX_BIOS_CMD_READ_CORE_SCALING_FACTOR;
  ScalingFactorData.Data = 0;

  for (Index = 0; Index < NumberOfProcessors; Index ++) {
    MailboxCommand.Fields.Param1 = ProcessorInfo[Index].ExtendedInformation.Location2.Module;
    Status = MailboxRead (MailboxCommand.InterfaceData, &ScalingFactorData.Data, &MailboxStatus);

    if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
      if (ScalingFactorData.Fields.ScalingFactor == 0) {
        IpcScaling[Index] = 100; // Hardcode the value as 100 if scaling fator from pcode returns 0.
      } else {
        //
        // Scaling factor result from pcode is U8.8 Format, converting data to UINT8
        //
        FromUnsignedFixedPoint16 ((UINT16) (ScalingFactorData.Fields.ScalingFactor), 100, 8, 8, &ScalingFactor, NULL);
        IpcScaling[Index] = (UINT8) ScalingFactor;
      }
      DEBUG ((DEBUG_INFO, "Scaling factor from pcode = %d\n", IpcScaling[Index]));
    } else {
      DEBUG ((DEBUG_ERROR, "Error Reading Scaling factor. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }
  }
}

/**
  Get Pm Logic ID. If not support Pm Logic ID, return -1.

  @retval Pm Logic ID
**/
UINT64
EFIAPI
GetPmLogicId (
  VOID
  )
{
  return (UINT64)-1;
}

/**
  Return the per processor P1 ratio for each processor.

  @param[in]  ProcessorInfo      Pointer to EFI_PROCESSOR_INFORMATION
  @param[out] P1Ratio        Pointer to each processor's P1 ratio
  @param[in]  NumberOfProcessors Number of processors
**/
VOID
GetP1RatioFru (
  IN  EFI_PROCESSOR_INFORMATION  *ProcessorInfo,
  OUT  UINT8                     *P1Ratio,
  IN  UINTN                      NumberOfProcessors
  )
{
  UINTN                                              Index;
  EFI_STATUS                                         Status;
  PCODE_MAILBOX_INTERFACE                            MailboxCommand;
  MAILBOX_BIOS_CMD_DFX_PARAM2                        MailboxCommandParam2;
  MAILBOX_BIOS_SUB_COMMAND_DFX_GET_FREQUENCIES_DATA  Frequency;
  UINT32                                             MailboxStatus;
  UINTN                                              PrevIndex;

  ASSERT (P1Ratio != NULL);

  //
  // MAILBOX_BIOS_CMD_DFX:
  //  Param1 = Sub command
  //  Param2[3:0] = 1 (Cdie)
  //  Param2[11:4] = Logical CCP ID (Module ID)
  //
  MailboxCommand.Fields.Command = MAILBOX_BIOS_CMD_DFX;
  MailboxCommand.Fields.Param1  = MAILBOX_BIOS_SUB_COMMAND_DFX_GET_FREQUENCIES;

  for (Index = 0; Index < NumberOfProcessors; Index++) {
    for (PrevIndex = 0; PrevIndex < Index; PrevIndex++) {
      if ((ProcessorInfo[Index].ExtendedInformation.Location2.Die == ProcessorInfo[PrevIndex].ExtendedInformation.Location2.Die) &&
          (ProcessorInfo[Index].ExtendedInformation.Location2.Module == ProcessorInfo[PrevIndex].ExtendedInformation.Location2.Module)) {
        //
        // Same module, copy the P1 ratio
        //
        P1Ratio[Index] = P1Ratio[PrevIndex];
        break;
      }
    }
    if (PrevIndex < Index) {
      //
      // Same module, skip the rest of the loop
      //
      continue;
    }

    //
    // Set P1 ratio to the max non turbo ratio by default
    //
    P1Ratio[Index] = GetMaxNonTurboRatio ();
    //
    // Different module, read the P1 ratio from mailbox
    //
    MailboxCommandParam2.Uint16            = 0;
    MailboxCommandParam2.Bits.LogicalCcpId = (UINT16) ProcessorInfo[Index].ExtendedInformation.Location2.Module;
    //
    // From spec only CDIE for Die selection
    //
    MailboxCommandParam2.Bits.DieSelection = MAILBOX_BIOS_SUB_COMMAND_DFX_GET_FREQUENCIES_DIE;

    MailboxCommand.Fields.Param2 = MailboxCommandParam2.Uint16;
    Status = MailboxRead (MailboxCommand.InterfaceData, &Frequency.Uint32, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((
        DEBUG_ERROR, "Failed to read frequency from mailbox. Status = %r, MailboxStatus = %x\n",
        Status, MailboxStatus
        ));
      continue;
    } else {
      //
      // Set P1Ratio only when MailboxRead is success, else use above default value by GetMaxNonTurboRatio ().
      //
      P1Ratio[Index] = (UINT8) Frequency.Bits.P1;
    }
    DEBUG ((
      DEBUG_INFO, "DieSelection = %d, Logical CCP ID = %d, P0/P1 ratio = %d/%d\n",
      MailboxCommandParam2.Bits.DieSelection, MailboxCommandParam2.Bits.LogicalCcpId,
      Frequency.Bits.P0, Frequency.Bits.P1
      ));
  }
}

/**
  Return TRUE when the Operand is exactly power of 2.

  @retval TRUE  Operand is exactly power of 2.
  @retval FALSE Operand is not power of 2.
**/
STATIC
BOOLEAN
IsPowerOfTwo (
  IN  UINT32        Operand
)
{
  ASSERT (Operand != 0);
  return (BOOLEAN) ((Operand & (Operand - 1)) == 0);
}

/**
  Function will return silicon recommended PRMRR size

  @param[in]  TdxEnable    If Tdx is enabled

  @retval Recommended PrmrrSize
**/
UINT32
EFIAPI
CpuGetRecommendPrmrrSize (
  IN BOOLEAN  TdxEnable
  )
{
  /*
  ------------+--------------------------+-----------------------------
              |                     PRMRR Size                        |
     Segment  |--------------------------+-----------------------------
              |       TDX Unsupported    |       TDX Supported        |
  ------------+--------------------------+-----------------------------
              |                          | TDX Disabled | TDX Enabled |
  ------------+--------------------------+-----------------------------
    PTL U/P/H |          2MB             |      2MB     |    4MB      |
  ------------+--------------------------+-----------------------------
  */
  // TDX unsupported
  if ((IsSeamModeSupported () == FALSE)) {
    return SIZE_2MB;
  } else {
    // TDX supported and enabled
    if (TdxEnable) {
      return SIZE_4MB;
    } else {
      // TDX supported and disabled
      return SIZE_2MB;
    }
  }
}

/**
  Check if PrmrrSize is valid

  @param[in]  PrmrrSize    Size of Prmrr

  @retval TRUE             PrmrrSize is valid
  @retval FALSE            PrmrrSize is invalid
**/
BOOLEAN
EFIAPI
IsPrmrrSizeValid (
  IN UINT32  PrmrrSize
  )
{
  // Valid PrmrrSize is power of 2 and at least 1MB
  if ((PrmrrSize < SIZE_1MB) || (!IsPowerOfTwo (PrmrrSize))) {
    DEBUG ((DEBUG_INFO, "PrmrrSize is smaller than 1MB or not power of 2\n"));
    return FALSE;
  }

  // Bits 0 through 31 indicate which sizes are supported. Each bit represents one size
  // A bit at position n indicates whether size 2^(n+20) bytes is supported
  if ((LShiftU64 (AsmReadMsr64 (MSR_PRMRR_VALID_CONFIG), 20) & PrmrrSize) != 0) {
    return TRUE;
  }
  return FALSE;
}

/**
  Function is used to check and verify PrmrrSize.
  1. if PrmrrSize is zero, return silicon recommended value
  2. if PrmrrSize is non-zero and valid, return this valid PrmrrSize
  3. if PrmrrSize is non-zero and invalid, return 0

  @param[in]  TdxEnable    If Tdx is enabled
  @param[in]  PrmrrSize    Size of Prmrr

  @retval Recommended PrmrrSize
**/
UINT32
EFIAPI
CpuGetAndVerifyPrmrrSize (
  IN BOOLEAN  TdxEnable,
  IN UINT32   PrmrrSize
  )
{
  if (PrmrrSize == 0) {
    return (UINT32) CpuGetRecommendPrmrrSize (TdxEnable);
  } else if (IsPrmrrSizeValid (PrmrrSize)) {
    return PrmrrSize;
  } else {
    DEBUG ((DEBUG_INFO, "PrmrrSize is invalid, disable PrmrrSize\n"));
    return 0;
  }
}

/**
  Returns Socket Type String

  @param[in]   CpuFamilyId

  @retval      Character pointer of Socket Type String
**/
CHAR8*
EFIAPI
GetSocketTypeFru (
  IN UINT32 CpuFamilyId
  )
{
  switch (CpuFamilyId) {
    case CPUID_FULL_FAMILY_MODEL_PANTHERLAKE_MOBILE:
      return mPtlSocketTypeString;
    // Reserve for WCL
    case CPUID_FULL_FAMILY_MODEL_WILDCATLAKE_MOBILE:
      return mWclSocketTypeString;
    default:
      return NULL;
  }
}
