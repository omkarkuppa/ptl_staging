/** @file
  This file contains the tests for the SecureCPUConfiguration bit

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include "HstiIhvDxe.h"
#include <SetupVariable.h>
#include <Register/GenerationMsr.h>
#include <Register/TelemetryRegs.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/TcssInfoLib.h>
#include <Library/NguInfoLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/VmdInfoLib.h>
#include <Library/TdxInfoLib.h>
#include <Library/NguInfoLib.h>
#include <Library/CceInfoLib.h>
#include <ConfigBlock/Telemetry/TelemetryPeiConfig.h>

#define   B_SEAMRR_ADDR_MASK                (0x3FFFFE000000)
#define   B_SEAMRR_SIZE_MASK                (0x3FFFFFFFFFFF)
#define   B_SMRR_MASK_MASK                  (0xFFFFF000)
#define   B_SMRR_WORK_MASK                  (0xFFFFFFFF)
#define   B_PRMRR_WORK_MASK                 (0xFFFFFFFF)
#define   B_PRMRR_ADDR_MASK                 (0xFFFFFFFFFF000)

#define   NGU_IMR_PRMRR_PHYSMASK_MEMTYPE    (0x00000007)
#define   NGU_IMR_PRMRR_ALIGN_MASK          (0x3FFFFFFF000)
#define   MSR_PRMRR_ALIGN_MASK              (0x3FFFFFFF000)


#define   SMRR_1MB_ALIGN_MASK               (0x00000000000FF000)
#define   SMRR_BASE_MASK                    (0x00000000FFE00000)
#define   TSEG_MASK                         (0xFFFFFFFFFFF00000)
#define   IMR_EXCLUSION_RANGE_BITFIELD      (0x000003FFFFFFFC00)
#define   IA_EXCLUSION_RANGE_BITFIELD       (0x0000003FFFFFFC00)
#define   HBO_REMAP_BITFIELD                (0x000003FFFFF00000)

STATIC
BOOLEAN
ImrInExclusionRange (
  IN NGU_IMR  ImrName,
  IN IMR_EXCLUSION_OFFSET ExclusionRange
  )
{
  UINT64 ImrBase;
  UINT64 ImrLimit;
  UINT64 ExclusionRangeBase;
  UINT64 ExclusionRangeLimit;

  ImrBase = GetImrRegisterData(ImrName, IMR_BASE_BYTE);
  ImrLimit = ImrBase + GetImrRegisterData(ImrName, IMR_SIZE_BYTE);
  ExclusionRangeBase = MmioRead64 (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + GetExclusionRegisterOffset (ExclusionRange, ExclusionOffsetBase));
  ExclusionRangeBase = ExclusionRangeBase & IMR_EXCLUSION_RANGE_BITFIELD;
  ExclusionRangeLimit = ExclusionRangeBase + GetExclusionRegisterSize (ExclusionRange);

  if ((ImrBase < ExclusionRangeBase) || (ImrLimit > ExclusionRangeLimit)) {
    DEBUG ((DEBUG_INFO, " ImrBase: 0x%x\n", ImrBase));
    DEBUG ((DEBUG_INFO, " ImrLimit: 0x%x\n", ImrLimit));
    DEBUG ((DEBUG_INFO, " ExclusionRangeBase: 0x%x\n", ExclusionRangeBase));
    DEBUG ((DEBUG_INFO, " ExclusionRangeLimit: 0x%x\n", ExclusionRangeLimit));
    return FALSE;
  }

  return TRUE;
}

/**
  Run tests for SecureCPUConfiguration bit
**/
VOID
CheckSecureCpuConfiguration (
  VOID
  )
{
  EFI_STATUS                        Status;
  BOOLEAN                           Result;
  UINTN                             CpuNumber;
  UINTN                             CpuIndex;
  CPUID_VERSION_INFO_ECX            Ecx;
  UINT32                            TsegMB;
  UINT32                            Bgsm;
  UINT64                            ImrTsegPhysBase;
  UINT64                            ImrTsegPhysLimit;
  MSR_SMRR2_MASK_REGISTER           Smrr2PhysMask;
  UINT64                            PrimarySmrrBase;
  UINT64                            PrimarySmrrMask;
  UINT64                            SmrrLimit;
  MSR_PRMRR_BASE_0_REGISTER         PrmrrPhysBase;
  MSR_PRMRR_MASK_REGISTER           PrmrrPhysMask;
  BOOLEAN                           BspPrmrrEnabled;
  BOOLEAN                           PrmrrsConsistent;
  UINT64                            PrimaryPrmrrBase;
  UINT64                            PrimaryPrmrrMask;
  MEMORY_PLATFORM_DATA_HOB          *MemInfo;
  MSR_SMM_FEATURE_CONTROL_REGISTER  SmmFeatureControl;
  MSR_IA32_FEATURE_CONTROL_REGISTER Ia32FeatureControlMsr;
  MSR_MTRRCAP_REGISTER              MtrrCapMsr;
  MSR_SMRR_BASE_REGISTER            SmrrPhysBase;
  MSR_SMRR_MASK_REGISTER            SmrrPhysMask;
  MSR_IA32_BIOS_SIGN_ID_REGISTER    Ia32BiosSignIdMsr;
  MSR_BIOS_DONE_REGISTER            BiosDoneMsr;
  MSR_FEATURE_CONFIG_REGISTER       FeatureConfigMsr;
  UINT64                            ImrPrmrrPhysBase;
  UINT64                            ImrPrmrrPhysBaseRaw;
  UINT64                            ImrPrmrrPhysMask;
  BOOLEAN                           SeamrrEnabled;
  MSR_SEAMRR_BASE_REGISTER          MsrSeamrrPhysBase;
  MSR_SEAMRR_MASK_REGISTER          MsrSeamrrPhysMask;
  MSR_SEAMRR_BASE_REGISTER          ApMsrSeamrrPhysBase;
  MSR_SEAMRR_MASK_REGISTER          ApMsrSeamrrPhysMask;
  UINT64                            MsrSeamrrSize;
  UINT64                            ImrSeamrrPhysBase;
  UINT64                            ImrSeamrrSize;
  UINT64                            ImrSeamrrPhysMask;
  UINT64                            CodeReportedBitmap = 0;
  UINT64                            RangeBase;
  UINT64                            RangeLimit;
  UINT64                            RangeSize;
  UINT64                            TelemetryBaseAddress;
  SETUP_DATA                        SetupData;
  UINTN                             VarSize;

  BspPrmrrEnabled = FALSE;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_SECURE_CPU_CONFIGURATION) == 0) {
    return;
  }

  Result = TRUE;
  TsegMB = (UINT32) GetHostBridgeRegisterData(HostBridgeTsegMb, HostBridgeTsegMbBase);
  DEBUG ((DEBUG_INFO, "  CPU Security Configuration per CPU Number Tests\n"));

  CpuNumber = GetCpuNumber ();
  for (CpuIndex = 0; CpuIndex < CpuNumber; CpuIndex++) {
    DEBUG ((DEBUG_INFO, "    [CPU - 0x%x]\n", CpuIndex));

    DEBUG ((DEBUG_INFO, "      Microcode Update Revision Test\n"));
    ProcessorWriteMsr64 (CpuIndex, MSR_IA32_BIOS_SIGN_ID, 0);
    ProcessorCpuid (CpuIndex, CPUID_VERSION_INFO, NULL, NULL, NULL, NULL);
    Ia32BiosSignIdMsr.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_IA32_BIOS_SIGN_ID);
    if (Ia32BiosSignIdMsr.Bits.MicrocodeUpdateSignature == 0) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: Invalid Microcode Update Revision\n"));
      BuildAndAppendHstiUniqueStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1,
        &CodeReportedBitmap,
        BIT1
        );
      Result = FALSE;
    }

    DEBUG ((DEBUG_INFO, "      Sample Part Test\n"));

    if (IsSamplePart() == TRUE) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: This is a sample part\n"));

      BuildAndAppendHstiUniqueStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_2,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_2,
        &CodeReportedBitmap,
        BIT2
        );
      Result = FALSE;
    }

    DEBUG ((DEBUG_INFO, "      IA32_FEATURE_CONTROL MSR Lock Test\n"));

    Ia32FeatureControlMsr.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_IA32_FEATURE_CONTROL);
    if (Ia32FeatureControlMsr.Bits.Lock == 0) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: This is a sample part\n"));

      BuildAndAppendHstiUniqueStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_3,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_3,
        &CodeReportedBitmap,
        BIT3
        );
      Result = FALSE;
    }

    SmmFeatureControl.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_SMM_FEATURE_CONTROL);

    DEBUG ((DEBUG_INFO, "      SMM_CODE_CHK_EN Test\n"));

    if (SmmFeatureControl.Bits.SmmCodeChkEn == 0) {

      DEBUG ((DEBUG_INFO, "        Unexpected Status: SMM Code Fetch outside SMRAM detect feature disabled\n"));
      BuildAndAppendHstiUniqueStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_B,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_B,
        &CodeReportedBitmap,
        BIT11
        );
      Result = FALSE;
    }

    DEBUG ((DEBUG_INFO, "      SMM_FEATURE_CONTROL MSR Lock Test\n"));

    if (SmmFeatureControl.Bits.Lock == 0) {

      DEBUG ((DEBUG_INFO, "        Unexpected Status: SMM feature control MSR not locked\n"));

      BuildAndAppendHstiUniqueStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_4,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_4,
        &CodeReportedBitmap,
        BIT4
        );
      Result = FALSE;
    }

    DEBUG ((DEBUG_INFO, "      FEATURE_CONFIG MSR  Lock Test\n"));

    ProcessorCpuid (CpuIndex, CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);
    if (Ecx.Bits.AESNI == 1) {
      FeatureConfigMsr.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_FEATURE_CONFIG);
      if (FeatureConfigMsr.Bits.Lock == 0) {
        DEBUG ((DEBUG_INFO, "        Unexpected Status: Feature config MSR not locked\n"));

        BuildAndAppendHstiUniqueStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_5,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_5,
          &CodeReportedBitmap,
          BIT5
          );
        Result = FALSE;
      }
    }


    DEBUG ((DEBUG_INFO, "      BIOS DONE MSR set Test\n"));

    BiosDoneMsr.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_BIOS_DONE);

    if (BiosDoneMsr.Bits.EnableIaUntrusted == 0) {

      DEBUG ((DEBUG_INFO, "        Unexpected Status: BIOS Done MSR is not set\n"));

      BuildAndAppendHstiUniqueStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_C,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_C,
        &CodeReportedBitmap,
        BIT12
        );
      Result = FALSE;
    }

  }

  DEBUG ((DEBUG_INFO, "      FIT Patch Support Test\n"));

  if (!IsValiduCodeEntry ()) {
    BuildAndAppendHstiUniqueStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_6,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_6,
      &CodeReportedBitmap,
      BIT6
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "    SMM Security Configuration Tests\n"));

  DEBUG ((DEBUG_INFO, "      TSEGMB Alignment Test\n"));


  MemInfo = GetFirstGuidHob (&gSiMemoryPlatformDataGuid);
  ASSERT (MemInfo != NULL);
  if (MemInfo == NULL) {
    return;
  }

  if ((TsegMB & (LShiftU64 (MemInfo->Data.TsegSize,20) - 1)) != 0) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TSEGMB not size aligned, TSEG size: 0x%x\n",(LShiftU64 (MemInfo->Data.TsegSize,20) - 1)));

    BuildAndAppendHstiUniqueStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_9,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_9,
      &CodeReportedBitmap,
      BIT9
      );
    Result = FALSE;
  }

  PrimarySmrrBase = 0;
  PrimarySmrrMask = 0;
  for (CpuIndex = 0; CpuIndex < CpuNumber; CpuIndex++) {
      DEBUG((DEBUG_INFO, "      SMRR1 Configuration per CPU Number Test\n"));
      DEBUG ((DEBUG_INFO, "    [CPU - 0x%x]\n", CpuIndex));

    DEBUG ((DEBUG_INFO, "      SMRR1 are supported Test\n"));

    MtrrCapMsr.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_MTRRCAP);
    if (MtrrCapMsr.Bits.Smrr == 0) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: SMRR1 not supported\n"));

      BuildAndAppendHstiUniqueStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_24,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_24,
        &CodeReportedBitmap,
        BIT36
        );
      Result = FALSE;
    } else {
      DEBUG ((DEBUG_INFO, "      SMRR1 programmed consistently on all cores Test\n"));

      SmrrPhysBase.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_SMRR_BASE);
      SmrrPhysMask.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_SMRR_MASK);

      if (CpuIndex == 0) {
        PrimarySmrrBase = SmrrPhysBase.Uint64;
        PrimarySmrrMask = SmrrPhysMask.Uint64;
      } else {
        if ((SmrrPhysBase.Uint64 != PrimarySmrrBase) || (SmrrPhysMask.Uint64 != PrimarySmrrMask)) {
          DEBUG ((DEBUG_INFO, "        Unexpected Status: SMRR1 not programmed consistently across all cores\n"));

          BuildAndAppendHstiUniqueStatusString (
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_25,
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_25,
            &CodeReportedBitmap,
            BIT37
            );
          Result = FALSE;
        }
      }

      DEBUG ((DEBUG_INFO, "      SMRR1 enabled/correct Test\n"));

      if ((SmrrPhysMask.Bits.Vld == 0x0) ||
          (SmrrPhysMask.Bits.Assist != 0x0) ||
          (SmrrPhysBase.Bits.Memtype != 0x6)) {
        DEBUG ((DEBUG_INFO, "        Unexpected Status: SMRR1 not enabled/correct\n"));

        BuildAndAppendHstiUniqueStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_26,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_26,
          &CodeReportedBitmap,
          BIT38
          );

        Result = FALSE;
      }

      DEBUG ((DEBUG_INFO, "      SMRR1 and TSEGMB match Test\n"));

      DEBUG ((DEBUG_INFO, "        INFO: SMRR1:   0x%08x - 0x%08x\n", SmrrPhysBase.Uint64 & (SmrrPhysMask.Uint64 & B_SMRR_MASK_MASK), (UINT32) (~(SmrrPhysMask.Uint64 & B_SMRR_MASK_MASK) + 1)));
      DEBUG ((DEBUG_INFO, "        INFO: TSEGMB: 0x%08x\n", TsegMB));

      if ((SmrrPhysBase.Uint64 & (SmrrPhysMask.Uint64 & B_SMRR_MASK_MASK)) != TsegMB) {

        DEBUG ((DEBUG_INFO, "        Unexpected Status: SMRR1 != TSEGMB\n"));

        BuildAndAppendHstiUniqueStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_27,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_27,
          &CodeReportedBitmap,
          BIT39
          );
        Result = FALSE;
      }

      DEBUG ((DEBUG_INFO, "      SMRR1 size Test\n"));
      Bgsm = (UINT32) GetHostBridgeRegisterData(HostBridgeBgsm, HostBridgeBgsmBase);
      if ((UINT32) (~(SmrrPhysMask.Uint64 & B_SMRR_MASK_MASK) + 1) !=
          (Bgsm - TsegMB)) {
        DEBUG ((DEBUG_INFO, "        Unexpected Status: SMRR1 size != BGSM-TSEGMB\n"));

        BuildAndAppendHstiUniqueStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_28,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_28,
          &CodeReportedBitmap,
          BIT40
          );
        Result = FALSE;
      }

      DEBUG ((DEBUG_INFO, "      SMRR1 Work Test\n"));

      if (MmioRead32 (SmrrPhysBase.Uint64 & SmrrPhysMask.Uint64) != B_SMRR_WORK_MASK) {
        DEBUG ((DEBUG_INFO, "        Unexpected Status: SMRR1 not working, read succeeded\n"));

        BuildAndAppendHstiUniqueStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_29,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_29,
          &CodeReportedBitmap,
          BIT41
          );
        Result = FALSE;
      }

      DEBUG ((DEBUG_INFO, "      SMRR and TSEG IMR in SAF (NoC) is consistent Test\n"));

      ImrTsegPhysBase = GetImrTsegBaseOffset ();
      ImrTsegPhysLimit = GetImrTsegMask ();

      // Verify SmrrPhysBase and SmrrPhysMask addressed are 1MB aligned
      if (((SmrrPhysBase.Uint64 & SMRR_1MB_ALIGN_MASK) != 0) ||
      ((SmrrPhysMask.Uint64 & SMRR_1MB_ALIGN_MASK) != 0)) {

        DEBUG ((DEBUG_INFO, " Unable to perform consistency test, SmrrPhysBase or SmrrPhyMask bits 19:12 not equal to 0 (not 1MB aligned) \n"));
        DEBUG ((DEBUG_INFO, " SmrrPhysBase bits 19:12 = 0x%x \n", SmrrPhysBase.Uint64 & SMRR_1MB_ALIGN_MASK));
        DEBUG ((DEBUG_INFO, " SmrrPhysMask bits 19:12 = 0x%x \n", SmrrPhysMask.Uint64 & SMRR_1MB_ALIGN_MASK));

        BuildAndAppendHstiUniqueStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_2A,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_2A,
          &CodeReportedBitmap,
          BIT42
          );
        Result = FALSE;
      } else {
        SmrrLimit = (SmrrPhysBase.Uint64 & (SmrrPhysMask.Uint64 & B_SMRR_MASK_MASK)) + (((SmrrPhysMask.Uint64 & B_SMRR_MASK_MASK) ^ MAX_UINT32) + 1);

        if ((ImrTsegPhysBase != (SmrrPhysBase.Uint64 & (SmrrPhysMask.Uint64 & B_SMRR_MASK_MASK))) || (ImrTsegPhysLimit != SmrrLimit)) {
          DEBUG ((DEBUG_INFO, "        Unexpected Status:  SMRR and TSEG IMR in SAF (NoC) is not consistent\n"));
          DEBUG ((DEBUG_INFO, "        SMRR_PHYSBASE: 0x%llx\n", (SmrrPhysBase.Uint64 & (SmrrPhysMask.Uint64 & B_SMRR_MASK_MASK))));
          DEBUG ((DEBUG_INFO, "        SMRR_PHYSMASK: 0x%llx\n", SmrrPhysMask.Uint64));
          DEBUG ((DEBUG_INFO, "        SMRR Limit: 0x%llx\n", SmrrLimit));
          DEBUG ((DEBUG_INFO, "        IMR_TSEG_PHYSBASE: 0x%llx\n", ImrTsegPhysBase));
          DEBUG ((DEBUG_INFO, "        IMR_TSEG_PHYSLIMIT: 0x%llx\n", ImrTsegPhysLimit));

          BuildAndAppendHstiUniqueStatusString (
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_2A,
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_2A,
            &CodeReportedBitmap,
            BIT42
            );
          Result = FALSE;
        }
      }

      //
      // SMRR Lock tests
      //

      DEBUG ((DEBUG_INFO, "      SMRR locked Test\n"));
      if (MtrrCapMsr.Bits.SmrrLock == 1){
        if ((SmrrPhysMask.Bits.L == 0) ) {
          DEBUG ((DEBUG_INFO, "        Unexpected Status: SMRR is not locked\n"));
          DEBUG ((DEBUG_INFO, "        INFO: SMRR1_MASK:   0x%x\n", SmrrPhysMask.Uint64));

          BuildAndAppendHstiUniqueStatusString (
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_15,
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_15,
            &CodeReportedBitmap,
            BIT21
            );
          Result = FALSE;
        }

        DEBUG ((DEBUG_INFO, "      SMRR2 locked Test\n"));
        if (MtrrCapMsr.Bits.Smrr2 == 1) {

          Smrr2PhysMask.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_SMRR2_MASK);
          if (Smrr2PhysMask.Bits.L == 0) {
            DEBUG ((DEBUG_INFO, "        Unexpected Status: SMRR is not locked\n"));
            DEBUG ((DEBUG_INFO, "        INFO: SMRR2_MASK:   0x%x\n", Smrr2PhysMask.Uint64));

            BuildAndAppendHstiUniqueStatusString (
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_17,
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_17,
            &CodeReportedBitmap,
            BIT21
            );
          Result = FALSE;
          }
        } else {
          DEBUG ((DEBUG_INFO, "        INFO: SMRR2 is not supported\n"));
        }
      } else {
        DEBUG ((DEBUG_INFO, "        INFO: SMRR lock is not supported\n"));
      }
    }
  }

  DEBUG ((DEBUG_INFO, "    Protected Range Microcode Range Register (PRMRR) Security Configuration per CPU Number Tests\n"));

  PrimaryPrmrrBase = 0;
  PrimaryPrmrrMask = 0;

  //
  // Multicore PRMRR tests
  //
  for (CpuIndex = 0; CpuIndex < CpuNumber; CpuIndex++) {
    DEBUG ((DEBUG_INFO, "    [CPU - 0x%x]\n", CpuIndex));

    DEBUG ((DEBUG_INFO, "      PRMRR supported Test\n"));

    PrmrrPhysBase.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_PRMRR_BASE_0);
    if ((PrmrrPhysBase.Uint64 & B_PRMRR_ADDR_MASK) == 0) {
      DEBUG ((DEBUG_INFO, "        PRMRR BASE MSR is not configured skipping PRMRR tests\n"));
    } else {
      PrmrrPhysBase.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_PRMRR_BASE_0);
      PrmrrPhysMask.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_PRMRR_MASK);

      DEBUG ((DEBUG_INFO, "        PRMRR Configured bit = 0x%x \n", PrmrrPhysBase.Bits.Configured));
      DEBUG ((DEBUG_INFO, "        PRMRR Mask Offset Value = 0x%016lx \n", (PrmrrPhysMask.Uint64 & B_PRMRR_ADDR_MASK)));

      DEBUG ((DEBUG_INFO, " PRMRR Enabled Test\n"));
      if ((PrmrrPhysBase.Bits.Configured == 0) || ((PrmrrPhysMask.Uint64 & B_PRMRR_ADDR_MASK) == 0)) {
        DEBUG ((DEBUG_INFO, "        Unexpected Status: Processor Reserved Memory Range Register is Supported but Disabled\n"));

        BuildAndAppendHstiUniqueStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_E,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_E,
          &CodeReportedBitmap,
          BIT14
          );
        Result = FALSE;
      } else {

        DEBUG ((DEBUG_INFO, "      PRMRR programmed consistently on all cores Test\n"));
        PrmrrsConsistent = TRUE;
        if (CpuIndex == 0) {

          //
          // store BSP test result for use in IMR PRMRR test
          //
          BspPrmrrEnabled = TRUE;

          PrimaryPrmrrBase = PrmrrPhysBase.Uint64;
          PrimaryPrmrrMask = PrmrrPhysMask.Uint64;
        } else {
          if ((PrmrrPhysBase.Uint64 != PrimaryPrmrrBase) || (PrmrrPhysMask.Uint64 != PrimaryPrmrrMask)) {
            PrmrrsConsistent = FALSE;
            DEBUG ((DEBUG_INFO, "        Unexpected Status: PRMRR's not programmed consistently\n"));

            BuildAndAppendHstiUniqueStatusString (
              HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_20,
              HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_20,
              &CodeReportedBitmap,
              BIT32
              );
            Result = FALSE;
          }
        }

        DEBUG ((DEBUG_INFO, "      PRMRR correctly programmed & Memory Type initialized Tests\n"));
        if (PrmrrsConsistent) {
          if ((PrmrrPhysMask.Bits.Assist != 0x0) ||
              (PrmrrPhysMask.Bits.L == 0x0) ||
              (PrmrrPhysBase.Bits.Memtype != 0x6)) {
            DEBUG ((DEBUG_INFO, "        Unexpected Status: PRMRR's not programmed correctly or  Memory Type not initialized\n"));

            BuildAndAppendHstiUniqueStatusString (
              HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_21,
              HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_21,
              &CodeReportedBitmap,
              BIT33
              );
            Result = FALSE;
          }

          DEBUG ((DEBUG_INFO, "      PRMRR working Test\n"));

          if (PrmrrPhysMask.Bits.Vld == 0) {
            DEBUG ((DEBUG_INFO, "        PRMRR Vld Bit in not set. Skipping PRMRR working Test\n"));
          } else if (MmioRead32 (PrmrrPhysBase.Uint64 & PrmrrPhysMask.Uint64) != B_PRMRR_WORK_MASK) {
            DEBUG ((DEBUG_INFO, "        Unexpected Status: PRMRR's not working, able to read from range\n"));
            BuildAndAppendHstiUniqueStatusString (
              HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_22,
              HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_22,
              &CodeReportedBitmap,
              BIT34
              );
            Result = FALSE;
          }
        }
      }
    }
  }

 // Check if MK-TME is supported in the CPU SKU
  if (IsTmeSupported () == TRUE) {
    // Check if MK-TME Activate lock bit is set to 1 irrrespectively if this TME is enabled or not
    DEBUG ((DEBUG_INFO, "    MK-TME bit locked Test\n"));

    if ((IsTmeActivateLocked ()) == FALSE) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: MK-TME Active bit is unlocked\n"));

      BuildAndAppendHstiUniqueStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_10,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_10,
        &CodeReportedBitmap,
        BIT16
        );
      Result = FALSE;
    }
  } else {
    DEBUG ((DEBUG_INFO, "        HSTI MSR INFO: MK-TME Technology is not supported in CPU SKU\n"));
  }


  if (BspPrmrrEnabled) {
    PrmrrPhysBase.Uint64  = ProcessorReadMsr64 (0, MSR_PRMRR_BASE_0) & MSR_PRMRR_ALIGN_MASK;
    PrmrrPhysMask.Uint64  = ProcessorReadMsr64 (0, MSR_PRMRR_MASK) & MSR_PRMRR_ALIGN_MASK;
    ImrPrmrrPhysBase      = GetImrRegisterData (PRMRR, IMR_BASE_BYTE);
    ImrPrmrrPhysMask      = GetImrRegisterData (PRMRR, IMR_MASK_BYTE);

    //
    // PRMRR MSR and IMR in SAF (NoC) is consistent test
    //
    DEBUG ((DEBUG_INFO, "    PRMRR MSR and IMR in SAF (NoC) is consistent test\n"));
    if((PrmrrPhysBase.Uint64 != ImrPrmrrPhysBase) || (PrmrrPhysMask.Uint64 != ImrPrmrrPhysMask)) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: MSR and IMR PRMRR in SAF (NoC) is not consistent\n"));
      DEBUG ((DEBUG_INFO, "          MSR_PRMRR_BASE = 0x%llx\n", PrmrrPhysBase.Uint64));
      DEBUG ((DEBUG_INFO, "          MSR_PRMRR_MASK = 0x%llx\n", PrmrrPhysMask.Uint64));
      DEBUG ((DEBUG_INFO, "          IMR_PRMRR_BASE = 0x%llx\n", ImrPrmrrPhysBase));
      DEBUG ((DEBUG_INFO, "          IMR_PRMRR_MASK = 0x%llx\n", ImrPrmrrPhysMask));
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_2B,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_2B
        );
      Result = FALSE;
    } else {

      //
      // PRMRR SAF IMR enabled test
      //
      ImrPrmrrPhysBaseRaw   = GetImrRegisterData (PRMRR, IMR_BASE_RAW);
      DEBUG ((DEBUG_INFO, "    PRMRR SAF IMR enabled test\n"));
      if((ImrPrmrrPhysBaseRaw & BIT4) != 0) {
        DEBUG ((DEBUG_INFO, "        Unexpected Status: MSR PRMRRs in SAF (NoC) is not enabled\n"));
        BuildAndAppendHstiStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_36,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_36
          );
        Result = FALSE;
      }
    }
  }

  if (IsSeamModeSupported () == FALSE) {
    DEBUG ((DEBUG_INFO, "    SEAM Mode is not supported. SEAMRR Enabled test skipped.\n"));
  } else {
    //
    // SEAMRR Enabled test
    //
    MsrSeamrrPhysBase.Uint64 = AsmReadMsr64 (MSR_SEAMRR_BASE);
    MsrSeamrrPhysMask.Uint64 = AsmReadMsr64 (MSR_SEAMRR_MASK);
    SeamrrEnabled = TRUE;

    DEBUG ((DEBUG_INFO, "    SEAMRR enabled test\n"));
    if ((MsrSeamrrPhysBase.Uint64 == 0) || ((MsrSeamrrPhysMask.Uint64 & BIT11) == 0) || ((MsrSeamrrPhysMask.Uint64 & B_SEAMRR_ADDR_MASK) == 0)) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: SEAMRR MSRs are not enabled\n"));
      SeamrrEnabled = FALSE;
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_40,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_40
        );
      Result = FALSE;
    }

    if (SeamrrEnabled) {
      //
      // SEAMRR MSRs consistency test
      //
      for (CpuIndex = 0; CpuIndex < CpuNumber; CpuIndex++) {
        DEBUG ((DEBUG_INFO, "    [CPU - 0x%x]\n", CpuIndex));

        DEBUG ((DEBUG_INFO, "      SEAMRR MSRs consistency Test\n"));
        ApMsrSeamrrPhysBase.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_SEAMRR_BASE);
        ApMsrSeamrrPhysMask.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_SEAMRR_MASK);
        if ((MsrSeamrrPhysBase.Uint64 != ApMsrSeamrrPhysBase.Uint64) || (MsrSeamrrPhysMask.Uint64 != ApMsrSeamrrPhysMask.Uint64)) {
          DEBUG ((DEBUG_INFO, "        Unexpected Status: SEAMRR MSRs are not consistent across all cores\n"));
          BuildAndAppendHstiStatusString (
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_41,
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_41
            );
          Result = FALSE;
          break;
        }
      }

      //
      // SEAMRR MSR Range Configured test
      //

      DEBUG ((DEBUG_INFO, "    SEAMRR MSR Range Configured test\n"));
      if(MsrSeamrrPhysBase.Bits.Configured != 1) {
        DEBUG ((DEBUG_INFO, "        Unexpected Status: SEAMRR MSRs Range is not configured\n"));
        BuildAndAppendHstiStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_42,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_42
          );
        Result = FALSE;
      }
    }

    //
    // SEAMRR MSR locked test
    //
    DEBUG ((DEBUG_INFO, "    SEAMRR MSR locked test\n"));
    if((MsrSeamrrPhysMask.Uint64 & BIT10) == 0) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: SEAMRR MSRs bit is not locked\n"));
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_43,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_43
        );
      Result = FALSE;
    }

    if (SeamrrEnabled) {

      //
      // SEAMRR IMR Enabled test
      //
      ImrSeamrrPhysBase = GetImrRegisterData (SEAMRR, IMR_BASE_BYTE);
      ImrSeamrrPhysMask = GetImrRegisterData (SEAMRR, IMR_MASK_BYTE);

      DEBUG ((DEBUG_INFO, "    SEAMRR IMR Enabled test\n"));
      if((ImrSeamrrPhysBase != 0) && (ImrSeamrrPhysMask != 0) &&
      ((GetImrRegisterData (SEAMRR, IMR_BASE_RAW) & BIT4) != 0)) {
        DEBUG ((DEBUG_INFO, "        Unexpected Status: SEAMRR in IMR (2PowerN) Enabled Range bit is not set\n"));
        BuildAndAppendHstiStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_44,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_44
          );
        Result = FALSE;
      } else {

        //
        // SEAMRR MSRs & IMRs in SAF consistent test
        //
        DEBUG ((DEBUG_INFO, "    TDX SEAM MSR & IMR in SAF consistent test\n"));
        ImrSeamrrSize = GetImrRegisterData (SEAMRR, IMR_SIZE_BYTE);
        MsrSeamrrSize = ((MsrSeamrrPhysMask.Uint64 & B_SEAMRR_ADDR_MASK) ^ B_SEAMRR_SIZE_MASK) + 1;

        if(((MsrSeamrrPhysBase.Uint64 & B_SEAMRR_ADDR_MASK) != ImrSeamrrPhysBase) ||
        (ImrSeamrrSize != MsrSeamrrSize)) {
          DEBUG ((DEBUG_INFO, "        Unexpected Status: SEAMRR MSRs and SEAMRR IMRs are not consistent\n"));
          DEBUG ((DEBUG_INFO, "        ImrSeamrrPhysBase = 0x%llx\n", ImrSeamrrPhysBase));
          DEBUG ((DEBUG_INFO, "        MsrSeamrrPhysBase = 0x%llx\n", (MsrSeamrrPhysBase.Uint64)));
          DEBUG ((DEBUG_INFO, "        ImrSeamrrSize = 0x%llx\n", ImrSeamrrSize));
          DEBUG ((DEBUG_INFO, "        MsrSeamrrPhysMask = 0x%llx\n", (MsrSeamrrPhysMask.Uint64)));

          BuildAndAppendHstiStatusString (
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_45,
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_45
            );
          Result = FALSE;
        }

        DEBUG ((DEBUG_INFO, "      NOC Exclusion Range is aligned/overlap SEAMRR Test\n"));
        if (!ImrInExclusionRange (SEAMRR, NocExclusionRangeHigh)) {
          DEBUG ((DEBUG_INFO, "      Unexpected Status: SEAMRR IMR Range is not part of NoC Exclusion High Range\n"));

          BuildAndAppendHstiStatusString (
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1B,
            HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1B
            );
          Result = FALSE;
        }
      }
    }
  }


  //
  // Uncore Security Configuration
  //

  // Check if VMD Device Present and Enabled then verify VMD Lock bit is set to 1
  DEBUG ((DEBUG_INFO, "    VMD Supported Test\n"));
  if (IsVmdEnabled ()) {
    DEBUG ((DEBUG_INFO, "    VMD Lock Bit Test\n"));

    if (IsVmdLocked() == FALSE) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: Volume Management Device (VMD) bit is unlocked\n"));

      BuildAndAppendHstiUniqueStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_F,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_F,
        &CodeReportedBitmap,
        BIT15
        );
      Result = FALSE;
    }
  } else {
    DEBUG ((DEBUG_INFO, "        INFO: Volume Management Device (VMD) is not Present\n"));
  }

  //
  // SAF IMR Enabled tests
  //
  DEBUG ((DEBUG_INFO, "      GSM in IMR1M Enabled Range bit Test\n"));
  if (IsImrBgsmRangeEnabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: GSM in IMR1M Enabled Range bit is not set\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_30,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_30
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      DSM in IMR1M Enabled Range bit Test\n"));
  if (IsImrBdsmRangeEnabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: DSM in IMR1M Enabled Range bit is not set\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_31,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_31
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      CCS in IMR1M Enabled Range bit Test\n"));
  if (IsImrCcsRangeEnabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: CCS in IMR1M Enabled Range bit is not set\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_32,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_32
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      TSEG in IMR1M Enabled Range bit Test\n"));
  if (IsImrTsegRangeEnabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: TSEG in IMR1M Enabled Range bit is not set\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_34,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_34
      );
    Result = FALSE;
  }

  if (IsTraceHubEnabled () == TRUE) {
    DEBUG ((DEBUG_INFO, "      Trace Hub in IMR Enabled Range bit Test\n"));
    if (IsImrTraceHubBaseRangeEnabled () == FALSE) {
      DEBUG ((DEBUG_INFO, "      Unexpected Status: Trace Hub in IMR (2PowerN) Enabled Range bit is not set\n"));

      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_35,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_35
        );
      Result = FALSE;
    } else {
      DEBUG ((DEBUG_INFO, "      NOC Exclusion Range is aligned/overlap Trace Hub Test\n"));
      if (!ImrInExclusionRange (NPK, NocExclusionRangeHigh)) {
        DEBUG ((DEBUG_INFO, "      Unexpected Status: Trace Hub IMR Range is not part of NoC Exclusion High Range\n"));

        BuildAndAppendHstiStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1C,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1C
          );
        Result = FALSE;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "      PRMRR in IMR Enabled Range bit Test\n"));
  if (IsImrPrmrrBaseRangeEnabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: PRMRR in IMR (2PowerN) Enabled Range bit is not set\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_36,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_36
      );
    Result = FALSE;
  } else {
    DEBUG ((DEBUG_INFO, "      NOC Exclusion Range is aligned/overlap PRMRR Test\n"));
    if (!ImrInExclusionRange (PRMRR, NocExclusionRangeLow)) {
      DEBUG ((DEBUG_INFO, "      Unexpected Status: PRMRR IMR range is not part of NoC Exclusion Low range\n"));

      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1A,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1A
        );
      Result = FALSE;
    }
  }

  if (IsTseEnabled ()) {
    DEBUG ((DEBUG_INFO, "      TSE in IMR Enabled Range bit Test\n"));
    if (IsImrTseBaseRangeEnabled () == FALSE) {
      DEBUG ((DEBUG_INFO, "      Unexpected Status: TSE in IMR (2PowerN) Enabled Range bit is not set\n"));

      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_37,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_37
        );
      Result = FALSE;
    } else {
      DEBUG ((DEBUG_INFO, "      NOC Exclusion Range is aligned/overlap TSE Test\n"));
      if (!ImrInExclusionRange (TSE, NocExclusionRangeLow)) {
        DEBUG ((DEBUG_INFO, "      Unexpected Status: TSE IMR range is not part of NoC Exclusion Low range\n"));

        BuildAndAppendHstiStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_18,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_18
          );
        Result = FALSE;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "      NOC Exclusion Range is aligned/overlap CSE Test\n"));
  if (!ImrInExclusionRange (CSE, NocExclusionRangeLow)) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: CSE IMR range is not part of NoC Exclusion Low range\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_19,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_19
      );
    Result = FALSE;
  }


  DEBUG ((DEBUG_INFO, "      NOC Exclusion Range is aligned/overlap STREAMER Test\n"));
  VarSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &SetupData
                  );

  if (!EFI_ERROR (Status)) {
    TelemetryBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, TEL_BUS_NUM, TEL_DEV_NUM, TEL_FUN_NUM, 0);
    DEBUG ((DEBUG_INFO, "      Telemetry IP Status = 0x%x\n", PciSegmentRead16 (TelemetryBaseAddress + PCI_VENDOR_ID_OFFSET)));
    DEBUG ((DEBUG_INFO, "      Telemetry IP Mode = 0x%x\n", SetupData.StreamTracerMode));
    if ((PciSegmentRead16 (TelemetryBaseAddress + PCI_VENDOR_ID_OFFSET) != 0xFFFF) && (SetupData.StreamTracerMode != STREAMTRACER_DISABLED)) {
      if (!ImrInExclusionRange (STREAM_TRACER, NocExclusionRangeHigh)) {
        DEBUG ((DEBUG_INFO, "      Unexpected Status: STREAMER IMR range is part of NoC Exclusion high range\n"));

        BuildAndAppendHstiStatusString (
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1D,
          HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1D
          );
        Result = FALSE;
      }
    } else {
      DEBUG ((DEBUG_INFO, "Telemetry is unsupported or disabled\n"));
    }
  } else {
    DEBUG ((DEBUG_INFO, "Unable to read SETUP_DATA. Skippping STREAMER test\n"));
  }



  //
  // Remap Base/Limit are consistent Test
  //

  RangeBase = GetCceRemapBase (CCE0);
  RangeLimit = GetCceRemapLimit (CCE0);

  if (RangeBase == 0) {
    DEBUG ((DEBUG_INFO, "      CCE0 disabled. Skipping  HBO and CCE0 consistency tests\n"));
  } else {
    DEBUG ((DEBUG_INFO, "      HBO and CCE0 Remap Base Consistent test \n"));

    if (RangeBase != (GetRemapBase () & HBO_REMAP_BITFIELD)) {
      DEBUG ((DEBUG_INFO, "      Unexpected Status: REMAP_BASE and HBO registers values are not consistent in CCE 0\n"));
      DEBUG ((DEBUG_INFO, "      CceRemapBase: 0x%x\n", RangeBase));
      DEBUG ((DEBUG_INFO, "      RemapBase: 0x%x\n", (GetRemapBase () & HBO_REMAP_BITFIELD)));
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_38,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_38
        );
    Result = FALSE;
    }

    DEBUG ((DEBUG_INFO, "      HBO and CCE0 Remap Limit Consistent test \n"));
    if (RangeLimit != (GetRemapLimit () & HBO_REMAP_BITFIELD)) {
      DEBUG ((DEBUG_INFO, "      Unexpected Status: REMAP_LIMIT and HBO registers values are not consistent in CCE 0\n"));
      DEBUG ((DEBUG_INFO, "      CceRemapLimit: 0x%x\n", RangeLimit));
      DEBUG ((DEBUG_INFO, "      RemapLimit: 0x%x\n", (GetRemapLimit () & HBO_REMAP_BITFIELD)));
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_3A,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_3A
        );
      Result = FALSE;
    }
  }

  RangeBase = GetCceRemapBase (CCE1);
  RangeLimit = GetCceRemapLimit (CCE1);
  if (RangeBase == 0) {
    DEBUG ((DEBUG_INFO, "      CCE1 disabled. Skipping  HBO and CCE1 consistency tests\n"));
  } else {
    DEBUG ((DEBUG_INFO, "      HBO and CCE1 Remap Base Consistent test \n"));

    if (RangeBase != (GetRemapBase () & HBO_REMAP_BITFIELD)) {
      DEBUG ((DEBUG_INFO, "      Unexpected Status: REMAP_BASE and HBO registers values are not consistent in CCE 1\n"));
      DEBUG ((DEBUG_INFO, "      CceRemapBase: 0x%x\n", RangeBase));
      DEBUG ((DEBUG_INFO, "      RemapBase: 0x%x\n", (GetRemapBase () & HBO_REMAP_BITFIELD)));
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_39,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_39
        );
    Result = FALSE;
    }

    DEBUG ((DEBUG_INFO, "      HBO and CCE1 Remap Limit Consistent test \n"));
    if (RangeLimit != (GetRemapLimit () & HBO_REMAP_BITFIELD)) {
      DEBUG ((DEBUG_INFO, "      Unexpected Status: REMAP_LIMIT and HBO registers values are not consistent in CCE 1\n"));
      DEBUG ((DEBUG_INFO, "      CceRemapLimit: 0x%x\n", RangeLimit));
      DEBUG ((DEBUG_INFO, "      RemapLimit: 0x%x\n", (GetRemapLimit () & HBO_REMAP_BITFIELD)));
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_3B,
        HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_3B
        );
      Result = FALSE;
    }
  }

  DEBUG ((DEBUG_INFO, "      IA Exclusion Range is Consistent test \n"));
  RangeBase = MmioRead64 ((GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + GetExclusionRegisterOffset (IaExclusionRange, ExclusionOffsetBase)));
  RangeBase = RangeBase & IA_EXCLUSION_RANGE_BITFIELD;
  RangeSize = GetExclusionRegisterSize (IaExclusionRange);
  if ((RangeBase != GetImrRegisterData (CSE, IMR_BASE_BYTE)) || (RangeSize != GetImrRegisterData (CSE, IMR_SIZE_BYTE))) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: IA Exclusion Range is not consistent with CSE IMR range\n"));
    DEBUG ((DEBUG_INFO, "      IA Exclusion Range Base: 0x%016lx\n", RangeBase));
    DEBUG ((DEBUG_INFO, "      IA Exclusion Range Size: 0x%016lx\n", RangeSize));
    DEBUG ((DEBUG_INFO, "      CSE IMR Range Base: 0x%016lx\n", GetImrRegisterData (CSE, IMR_BASE_BYTE)));
    DEBUG ((DEBUG_INFO, "      CSE IMR Range Size: 0x%016lx\n", GetImrRegisterData (CSE, IMR_SIZE_BYTE)));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_3C,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_3C
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      IA Exclusion Range 1 is Consistent test \n"));
  RangeBase = MmioRead64 ((GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + GetExclusionRegisterOffset (IaExclusionRange1, ExclusionOffsetBase)));
  RangeBase = RangeBase & IA_EXCLUSION_RANGE_BITFIELD;
  RangeSize = GetExclusionRegisterSize (IaExclusionRange1);
  if ((RangeBase != GetImrRegisterData (CCS, IMR_BASE_BYTE)) || (RangeSize != GetImrRegisterData (CCS, IMR_SIZE_BYTE))) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: IA Exclusion Range 1 is not consistent with CCS IMR range\n"));
    DEBUG ((DEBUG_INFO, "      IA Exclusion Range 1 Base: 0x%016lx\n", RangeBase));
    DEBUG ((DEBUG_INFO, "      IA Exclusion Range 1 Size: 0x%016lx\n", RangeSize));
    DEBUG ((DEBUG_INFO, "      CCS IMR Range Base: 0x%016lx\n", GetImrRegisterData (CCS, IMR_BASE_BYTE)));
    DEBUG ((DEBUG_INFO, "      CCS IMR Range Size: 0x%016lx\n", GetImrRegisterData (CCS, IMR_SIZE_BYTE)));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_3D,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_3D
      );
    Result = FALSE;
  }

  //
  // TCSS TYPE C SW Configuration Locks Test
  //
#if FixedPcdGetBool(PcdTcssSupport) == 1
  DEBUG ((DEBUG_INFO, "      TCSS TYPE C SW Configuration Locks Test\n"));

  if (IsTcssTypeCSwCfg1Locked () == FALSE) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: TCSS TYPE C SW Configuration 1 Lock bit is not set\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_2C,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_2C
      );
    Result = FALSE;
  }

  if (IsTcssTypeCSwCfg3Locked () == FALSE) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: TCSS TYPE C SW Configuration 3 Lock bit is not set\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_2D,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_2D
      );
    Result = FALSE;
  }

  if (IsTcssTypeCSwCfg4Locked () == FALSE) {
    DEBUG ((DEBUG_INFO, "      Unexpected Status: TCSS TYPE C SW Configuration 4 Lock bit is not set\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_2E,
      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_2E
      );
    Result = FALSE;
  }
#endif      // FixedPcdGetBool(PcdITbtEnable) == 1

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               0,
               HSTI_BYTE0_SECURE_CPU_CONFIGURATION
               );
    CheckStatusForHstiLibSet (Status);
  }

  return;
}
