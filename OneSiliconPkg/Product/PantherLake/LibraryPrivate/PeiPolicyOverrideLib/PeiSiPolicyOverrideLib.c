/** @file
  Provide polciy override functions in PostMem to enable specific silicon feature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/HobLib.h>
#include <Library/PeiMeLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/PeiAmtPolicyLib.h>
#include <Library/PeiCnviPolicyLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/DebugToken.h>
#include <DebugTokenDataHob.h>
#include <MeBiosPayloadHob.h>
#include <AmtConfig.h>
#include <CnviConfig.h>
#include <TcssDataHob.h>

/**
  Override CNVi policies by checking function capability.

  @param[in] SiPolicyPpi               The Silicon PostMem Policy PPI instance
**/
STATIC
VOID
CnviPolicyOverride (
  IN  SI_POLICY_PPI                    *SiPolicyPpi
  )
{
  EFI_STATUS  Status;
  CNVI_CONFIG *CnviConfig;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCnviConfigGuid, (VOID *) &CnviConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a ()- not supported\n", __FUNCTION__));
    return;
  }

  //
  // If CnviConfig->Mode is disabled, Wifi/BT core should be forced to disable.
  //
  if (CnviConfig->Mode == CnviModeDisabled) {
    CnviConfig->WifiCore = FALSE;
    CnviConfig->BtCore   = FALSE;
    DEBUG ((DEBUG_INFO, "-- CNVi Config Override --\n"));
    CnviPrintConfig (SiPolicyPpi);
  }

  return;
}

/**
  Override AMT policies by MBP HOB and feature capability.
  AMT related config should be disabled if CSME does not support AMT.

  @param[in] SiPolicyPpi               The Silicon PostMem Policy PPI instance
**/
STATIC
VOID
AmtPolicyOverride (
  IN  SI_POLICY_PPI                    *SiPolicyPpi
  )
{
#if FixedPcdGetBool(PcdAmtEnable) == 1
  EFI_STATUS           Status;
  AMT_PEI_CONFIG       *AmtPeiConfig;
  ME_BIOS_PAYLOAD_HOB  *MbpHob;
  MEFWCAPS_SKU         FwCapsSku;
  BOOLEAN              IsAmtEnabled;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  MbpHob         = NULL;
  FwCapsSku.Data = 0;
  IsAmtEnabled   = FALSE;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gAmtPeiConfigGuid, (VOID *) &AmtPeiConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a ()- not supported\n", __FUNCTION__));
    return;
  }

  if (!MeClientIsCorporate ()) {
    AmtPeiConfig->AmtEnabled = 0;
  }

  if (AmtPeiConfig->AmtEnabled == 1) {
    MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
    if (MbpHob != NULL) {
      if (MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt == 1 &&
          MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Amt == 1) {
        IsAmtEnabled = TRUE;
      }
    } else {
      //
      // When MBP is not available (e.g S3 resume), we need
      // to retrieve Feature Capabilities and State from CSME
      //
      Status = PeiHeciGetFwCapsSkuMsg (&FwCapsSku);
      if (!EFI_ERROR (Status) && FwCapsSku.Fields.Amt == 1) {
        Status = PeiHeciGetFwFeatureStateMsg (&FwCapsSku);
        if (!EFI_ERROR (Status) && FwCapsSku.Fields.Amt == 1) {
          IsAmtEnabled = TRUE;
        }
      }
    }
  }

  if (!IsAmtEnabled) {
    //
    // Disable below settings since they have dependency on AMT.
    //
    AmtPeiConfig->WatchDogEnabled   = 0;
    AmtPeiConfig->WatchDogTimerBios = 0;
    AmtPeiConfig->WatchDogTimerOs   = 0;
    AmtPeiConfig->AmtSolEnabled     = 0;
    DEBUG ((DEBUG_INFO, "-- AMT_PEI_CONFIG Override --\n"));
    AmtPrintPolicyPpi (SiPolicyPpi);
  }
#endif

  return;
}

/**
  Override policies by Debug Token

  @param[in] SiPolicyPpi               The Silicon PostMem Policy PPI instance
**/
STATIC
VOID
DebugTokenPolicyOverride (
  IN  SI_POLICY_PPI                    *SiPolicyPpi
  )
{
  EFI_STATUS                       Status;
  DEBUG_TOKEN_DATA_HOB             *DebugTokenHob;
  EFI_HOB_GUID_TYPE                *GuidHob;
  DEBUG_TOKEN_KNOB_DATA            DebugToken;
  CPU_TEST_CONFIG                  *CpuTestConfig;

  GuidHob = GetFirstGuidHob (&gDebugTokenDataHobGuid);
  if (GuidHob == NULL) {
    return;
  }
  DebugTokenHob = (DEBUG_TOKEN_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
  DebugToken.Data = DebugTokenHob->DebugTokenData;
  DEBUG ((DEBUG_INFO, "DebugToken.Data = %x\n", DebugToken.Data));

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuTestConfigGuid, (VOID *) &CpuTestConfig);
  ASSERT_EFI_ERROR (Status);
  if (DebugToken.Fields.ProcessorTraceEnable == 1) {
    CpuTestConfig->ProcessorTraceOutputScheme = 0;
    CpuTestConfig->ProcessorTraceEnable = 1;
    CpuTestConfig->ProcessorTraceMemSize = 0;
  }
}

/**
  Policy override in PostMem phase

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
  @param[in] SiPolicyPpi               The Silicon PostMem Policy PPI instance
**/
VOID
PeiSiPolicyOverride (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi,
  IN  SI_POLICY_PPI                    *SiPolicyPpi
  )
{
  DEBUG ((DEBUG_INFO, "PeiSiPolicyOverride ()\n"));
  CnviPolicyOverride (SiPolicyPpi);
  DebugTokenPolicyOverride (SiPolicyPpi);
  AmtPolicyOverride (SiPolicyPpi);
}
