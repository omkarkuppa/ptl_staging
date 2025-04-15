/** @file
  CPU Setup Routines

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

#include <PlatformNvRamHookLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/RngLib.h>
#include <Library/CpuMailboxLib.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/MdeModuleHii.h>
#include <Library/BaseLib.h>
#include "Base.h"
#include <SetupPrivate.h>
#include "Platform.h"
#include "PlatformBoardId.h"
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <Protocol/CpuInfo.h>
#include <Library/PciSegmentLib.h>
#include <Library/BootGuardLib.h>
#include <Library/MeFwStsLib.h>
#include <Register/PchRegs.h>
#include <Library/SpiAccessLib.h>
#include <Protocol/MpService.h>
#include <VrDataHob.h>
#include <CmosMap.h>
#include <Library/CmosAccessLib.h>
#include <Register/GenerationMsr.h>
#include <CmosMap.h>
#include <Library/CmosAccessLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Txt.h>
#include <MemInfoHob.h>
#include <Library/PcdLib.h>
#include <Library/BaseFitAccessLib.h>

#if FixedPcdGetBool(PcdTcssSupport) == 1
#include <TcssDataHob.h>
#endif
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Protocol/CnvFormPlatformProtocol.h>
#include <Library/FixedPointLib.h>
#include <Register/B2pMailbox.h>

static EFI_HII_HANDLE     gHiiHandle;

static UINT8              mCoreRatioFinal[TURBO_RATIO_LIMIT_ARRAY_SIZE] = {0};

UINT16                    mIccMax[MAX_VR_NUM] = {0};

EFI_MP_SERVICES_PROTOCOL  *mMpService;

STATIC SETUP_CPU_FEATURES mSetupCpuFeatures;

extern UINT8 AdvancedBin[];

GLOBAL_REMOVE_IF_UNREFERENCED VR_DATA_HOB                    *mVrDataHob;

/**
   This function update Bit 4 of CMOS index 0x2A (index reg: 0x70, Data : 0x71)
   based on TXT setup option.
   If TXT is Enabled Bit 4 will be 1, else bit 4 will be 0

   @param[IN]       EFI_FORM_CALLBACK_PROTOCOL   *This
   @param[IN]       UINT16                       KeyValue
   @param[IN]       EFI_IFR_DATA_ARRAY           *Data,
   @param[IN]       EFI_HII_CALLBACK_PACKET      **Packet

   @retval          EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
TxtPolicyCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  );

/**
  Update the min, max, and default values for CpuRatio.

  @param[in] CpuRatioDefault      The CPU MNTR default.

  @retval EFI_SUCCESS             Values updated successfully.
  @retval EFI_NOT_FOUND           Failed to update it.
**/
EFI_STATUS
InitCpuMntrDefaultVfr (
  UINT8      CpuRatioDefault
  );

/**
  Initialize Boot Frequency and Turbo Mode defaults.

  @param[in] EFI_EVENT    Event
  @param[in] VOID         *Context
**/
VOID
EFIAPI
InitBootFrequencyDefault (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  UINTN                                             VariableSize;
  UINT32                                            Attributes;
  UINT32                                            CpuFeatureVarAttr;
  EFI_STATUS                                        Status;

  VariableSize = sizeof (SETUP_CPU_FEATURES);
  Status = gRT->GetVariable (
                  L"SetupCpuFeatures",
                  &gSetupVariableGuid,
                  &CpuFeatureVarAttr,
                  &VariableSize,
                  &mSetupCpuFeatures
                  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR : Unable to get SetupCpuFeatures Variables!\n"));
    ASSERT_EFI_ERROR(Status);
    return;
  }

  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  &Attributes,
                  &VariableSize,
                  &mCpuSetup
                  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR : Unable to get CpuSetup Variables!\n"));
    ASSERT_EFI_ERROR(Status);
    return;
  }

  if (mCpuSetup.IsBootFrequencyDefaultsInitialized == 0) {
    //
    // Init Boot Frequency default once
    //
    mCpuSetup.IsBootFrequencyDefaultsInitialized = 1;
    //
    // Init CPU default value for "Turbo Mode" and "Boot Frequency" according to capability
    //
    if (mSetupCpuFeatures.TurboModeAvailable == 0) {
      mCpuSetup.BootFrequency = 1;
      mCpuSetup.TurboMode = 0;
    }

    //
    // Init CPU Turbo capability default value
    //
    mCpuSetup.BootFrequencyDefault = mCpuSetup.BootFrequency;
    mCpuSetup.TurboModeDefault = mCpuSetup.TurboMode;

    Status = gRT->SetVariable (
                    L"CpuSetup",
                    &gCpuSetupVariableGuid,
                    Attributes,
                    VariableSize,
                    &mCpuSetup
                    );
    ASSERT_EFI_ERROR (Status);
  }
}

/**
  Initialize Turbo Core Ratio defaults.

  @param[in] EFI_EVENT    Event
  @param[in] VOID         *Context
**/
VOID
EFIAPI
InitTurboRatioDefault (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                                        Status;
  UINT8                                             Index;
  MSR_TURBO_RATIO_LIMIT_REGISTER                    MsrTurboRatioLimitRatio;
  MSR_TURBO_RATIO_LIMIT_CORES_REGISTER              MsrTurboRatioLimitNumCore;
  MSR_ATOM_TURBO_RATIO_LIMIT_REGISTER               AtomMsrTurboRatioLimitRatio;
  MSR_ATOM_TURBO_RATIO_LIMIT_CORES_REGISTER         AtomMsrTurboRatioLimitNumCore;
  UINTN                                             VariableSize;
  UINT32                                            Attributes;
  UINT8                                             MaxBusRatio;
  UINT8                                             MinBusRatio;

  MaxBusRatio = 0;
  MinBusRatio = 0;

  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  &Attributes,
                  &VariableSize,
                  &mCpuSetup
                  );

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR : Unable to get CpuSetup Variables!\n"));
    ASSERT_EFI_ERROR(Status);
    return;
  }

  if (mCpuSetup.IsTurboRatioDefaultsInitialized == 0) {
    ///
    /// Get Maximum Non-Turbo bus ratio (HFM) from Platform Info MSR Bits[15:8]
    ///
    GetBusRatio (&MaxBusRatio, &MinBusRatio);
    mCpuSetup.IsTurboRatioDefaultsInitialized = 1;
    mCpuSetup.FlexRatioOverrideDefault = MaxBusRatio;

    MsrTurboRatioLimitRatio.Uint64 = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT);
    mCpuSetup.RatioLimitRatioDefault[0] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit0;
    mCpuSetup.RatioLimitRatioDefault[1] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit1;
    mCpuSetup.RatioLimitRatioDefault[2] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit2;
    mCpuSetup.RatioLimitRatioDefault[3] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit3;
    mCpuSetup.RatioLimitRatioDefault[4] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit4;
    mCpuSetup.RatioLimitRatioDefault[5] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit5;
    mCpuSetup.RatioLimitRatioDefault[6] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit6;
    mCpuSetup.RatioLimitRatioDefault[7] = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit7;
    for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE; Index++) {
      mCpuSetup.RatioLimitRatio[Index] = mCpuSetup.RatioLimitRatioDefault[Index];
    }

    MsrTurboRatioLimitNumCore.Uint64 = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT_CORES);
    mCpuSetup.RatioLimitNumCoreDefault[0] = (UINT8) MsrTurboRatioLimitNumCore.Bits.CoreCount0;
    mCpuSetup.RatioLimitNumCoreDefault[1] = (UINT8) MsrTurboRatioLimitNumCore.Bits.CoreCount1;
    mCpuSetup.RatioLimitNumCoreDefault[2] = (UINT8) MsrTurboRatioLimitNumCore.Bits.CoreCount2;
    mCpuSetup.RatioLimitNumCoreDefault[3] = (UINT8) MsrTurboRatioLimitNumCore.Bits.CoreCount3;
    mCpuSetup.RatioLimitNumCoreDefault[4] = (UINT8) MsrTurboRatioLimitNumCore.Bits.CoreCount4;
    mCpuSetup.RatioLimitNumCoreDefault[5] = (UINT8) MsrTurboRatioLimitNumCore.Bits.CoreCount5;
    mCpuSetup.RatioLimitNumCoreDefault[6] = (UINT8) MsrTurboRatioLimitNumCore.Bits.CoreCount6;
    mCpuSetup.RatioLimitNumCoreDefault[7] = (UINT8) MsrTurboRatioLimitNumCore.Bits.CoreCount7;
    for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE; Index++) {
      mCpuSetup.RatioLimitNumCore[Index] = mCpuSetup.RatioLimitNumCoreDefault[Index];
    }

    if ((mCpuSetup.ActiveSmallCoreCount != 0) || (mCpuSetup.ActiveLpAtomCoreCount != 0)) {
      AtomMsrTurboRatioLimitRatio.Uint64 = AsmReadMsr64 (MSR_ATOM_TURBO_RATIO_LIMIT);
      mCpuSetup.AtomRatioLimitRatioDefault[0] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit0;
      mCpuSetup.AtomRatioLimitRatioDefault[1] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit1;
      mCpuSetup.AtomRatioLimitRatioDefault[2] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit2;
      mCpuSetup.AtomRatioLimitRatioDefault[3] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit3;
      mCpuSetup.AtomRatioLimitRatioDefault[4] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit4;
      mCpuSetup.AtomRatioLimitRatioDefault[5] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit5;
      mCpuSetup.AtomRatioLimitRatioDefault[6] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit6;
      mCpuSetup.AtomRatioLimitRatioDefault[7] = (UINT8) AtomMsrTurboRatioLimitRatio.Bits.RatioLimit7;
      for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE; Index++) {
        mCpuSetup.AtomRatioLimitRatio[Index] = mCpuSetup.AtomRatioLimitRatioDefault[Index];
      }

      AtomMsrTurboRatioLimitNumCore.Uint64 = AsmReadMsr64 (MSR_ATOM_TURBO_RATIO_LIMIT_CORES);
      mCpuSetup.AtomRatioLimitNumCoreDefault[0] = (UINT8) AtomMsrTurboRatioLimitNumCore.Bits.CoreCount0;
      mCpuSetup.AtomRatioLimitNumCoreDefault[1] = (UINT8) AtomMsrTurboRatioLimitNumCore.Bits.CoreCount1;
      mCpuSetup.AtomRatioLimitNumCoreDefault[2] = (UINT8) AtomMsrTurboRatioLimitNumCore.Bits.CoreCount2;
      mCpuSetup.AtomRatioLimitNumCoreDefault[3] = (UINT8) AtomMsrTurboRatioLimitNumCore.Bits.CoreCount3;
      mCpuSetup.AtomRatioLimitNumCoreDefault[4] = (UINT8) AtomMsrTurboRatioLimitNumCore.Bits.CoreCount4;
      mCpuSetup.AtomRatioLimitNumCoreDefault[5] = (UINT8) AtomMsrTurboRatioLimitNumCore.Bits.CoreCount5;
      mCpuSetup.AtomRatioLimitNumCoreDefault[6] = (UINT8) AtomMsrTurboRatioLimitNumCore.Bits.CoreCount6;
      mCpuSetup.AtomRatioLimitNumCoreDefault[7] = (UINT8) AtomMsrTurboRatioLimitNumCore.Bits.CoreCount7;
      for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE; Index++) {
        mCpuSetup.AtomRatioLimitNumCore[Index] = mCpuSetup.AtomRatioLimitNumCoreDefault[Index];
      }
    }

    Status = gRT->SetVariable (
                    L"CpuSetup",
                    &gCpuSetupVariableGuid,
                    Attributes,
                    VariableSize,
                    &mCpuSetup
                    );
    ASSERT_EFI_ERROR (Status);
  }
}

/**
  Initialize Txt defaults. Disable Txt if FIT Record 0xA absent
**/
VOID
InitTxtDefault (
  VOID
  )
{
  EFI_STATUS            Status;
  UINT32                VariableAttr;
  UINTN                 VariableSize;

  VariableAttr = 0;
  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  &VariableAttr,
                  &VariableSize,
                  &mCpuSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "ERROR : Unable to get CpuSetup Variables!\n"));
    ASSERT_EFI_ERROR(Status);
    return;
  }

  if (NULL == FindFitEntryData(FIT_TYPE_0A_TXT_POLICY)) {
      DEBUG((DEBUG_INFO, "INFO : FIT Record 0xA absent hence Disabled Txt!\n"));
      mCpuSetup.FitRecTxt = 0;
  } else {
      mCpuSetup.FitRecTxt = 1;
  }

  Status = gRT->SetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  VariableAttr,
                  VariableSize,
                  &mCpuSetup
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  Initialize AcheckRequest defaults. Always reset to Disable
**/
VOID
InitTxtAcheckDefault (
  VOID
  )
{
  EFI_STATUS            Status;
  UINT32                VariableAttr;
  UINTN                 VariableSize;

  VariableAttr = 0;
  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  &VariableAttr,
                  &VariableSize,
                  &mCpuSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "ERROR : Unable to get CpuSetup Variables!\n"));
    ASSERT_EFI_ERROR(Status);
    return;
  }

  mCpuSetup.AcheckRequest = 0;

  Status = gRT->SetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  VariableAttr,
                  VariableSize,
                  &mCpuSetup
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
   This function update Bit 4 of CMOS index 0x2A (index reg: 0x70, Data : 0x71)
   based on TXT setup option.
   If TXT is Enabled Bit 4 will be 1, else bit 4 will be 0

   @param[IN]       EFI_FORM_CALLBACK_PROTOCOL   *This
   @param[IN]       UINT16                       KeyValue
   @param[IN]       EFI_IFR_DATA_ARRAY           *Data,
   @param[IN]       EFI_HII_CALLBACK_PACKET      **Packet

   @retval          EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
TxtPolicyCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  EFI_STATUS              Status;
  CPU_SETUP               *CpuSetup;
  UINTN                   VarSize;
  UINT8                   TxtStatus;
  UINT8                   NewTxtStatus;
  EFI_STRING              RequestString;

  Status = EFI_SUCCESS;
  RequestString = NULL;

  //
  // Only process KEY_TXT_POLICY_FIT callback
  //
  if (KeyValue != KEY_TXT_POLICY_FIT) {
    return EFI_UNSUPPORTED;
  }

  switch (Action) {
  case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
    //
    // Set Txt knob value to the default value (Disabled)
    //
    Value->u8 = 0;
    break;

  case EFI_BROWSER_ACTION_CHANGED:
  case EFI_BROWSER_ACTION_CHANGING:
    break;

  default:
    return EFI_UNSUPPORTED;
    break;
  }

  VarSize   = sizeof (CPU_SETUP);
  CpuSetup  = AllocatePool (VarSize);
  ASSERT (CpuSetup != NULL);

  if (CpuSetup == NULL) {
    DEBUG ((DEBUG_ERROR, "TxtPolicyCallBackFunction: Out of resources allocating CpuSetup!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // GetBrowserData by VarStore Name (Setup)
  //
  if (!HiiGetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup)) {
    FreePool (CpuSetup);
    DEBUG ((DEBUG_ERROR, "TxtPolicyCallBackFunction: CpuSetup not found!\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Read current TXT status from CMOS
  //
  NewTxtStatus = TxtStatus = CmosRead8 (FIT_REC_TXT_POLICY_TYPE_A);

  //
  // Handle restoring TXT to default state (DISABLED)
  //
  if (Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
    CpuSetup->Txt = FALSE;
  }

  if (CpuSetup->Txt == FALSE) {
    DEBUG ((DEBUG_INFO, "TxtPolicyCallBackFunction: Setting TXT to disabled.\n"));
    NewTxtStatus &= ~BIT4;
  } else {
    DEBUG ((DEBUG_INFO, "TxtPolicyCallBackFunction: Setting TXT to enabled.\n"));
    NewTxtStatus |=  BIT4;
  }

  //
  // Sync CMOS with new Txt Status
  //
  if (NewTxtStatus != TxtStatus) {
    CmosWrite8 (FIT_REC_TXT_POLICY_TYPE_A, NewTxtStatus);
  }

  //
  // Update menu string with new value
  //
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, Txt), sizeof (CpuSetup->Txt));

  if (RequestString != NULL) {
    if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup, RequestString)) {
      DEBUG ((DEBUG_ERROR, "TxtPolicyCallBackFunction: Setting Hii string ERROR!\n"));
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);
    FreePool (RequestString);
  }

  FreePool (CpuSetup);
  return Status;
}

#if FixedPcdGetBool(PcdTcssSupport) == 1
/**
  Limit CPU Package Cx limit to C2 if TCSS IOM is not ready.
**/
VOID
OverrideCpuCxLimitForTcssIom (
  VOID
  )
{
  TCSS_DATA_HOB *TcssHob;

  TcssHob = NULL;
  ///
  /// Locate HOB for TCSS Data
  ///
  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    DEBUG ((DEBUG_ERROR, "SA TSCC Data HOB not found\n"));
    return;
  }

  ///
  /// Check IOM ready state
  /// - Limit Package C state to PC2 when IOM is not ready
  /// - Keep user setting when IOM is ready, and return
  ///
  if (TcssHob->TcssData.IOMReady == 1) {
    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_IOM_STATE),
      L""
    );
    return;
  }

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_IOM_STATE),
    L"Warming! Limit Package C state to C2 due to IOM is not ready"
  );
  DEBUG ((DEBUG_INFO, "CpuSetup warming: Limit Package C state to PC2 due to IOM is not ready\n"));
}
#endif // FixedPcdGetBool(PcdTcssSupport) == 1

///
///

GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mAcLoadlineInfoStr[] = {
  STRING_TOKEN(STR_VR_AC_LOADLINE_INFO_VALUE0),
  STRING_TOKEN(STR_VR_AC_LOADLINE_INFO_VALUE1),
  STRING_TOKEN(STR_VR_AC_LOADLINE_INFO_VALUE2),
  STRING_TOKEN(STR_VR_AC_LOADLINE_INFO_VALUE3)
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mDcLoadlineInfoStr[] = {
  STRING_TOKEN(STR_VR_DC_LOADLINE_INFO_VALUE0),
  STRING_TOKEN(STR_VR_DC_LOADLINE_INFO_VALUE1),
  STRING_TOKEN(STR_VR_DC_LOADLINE_INFO_VALUE2),
  STRING_TOKEN(STR_VR_DC_LOADLINE_INFO_VALUE3)
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mPs1ThresholdInfoStr[] = {
  STRING_TOKEN(STR_VR_PS1_THRESHOLD_INFO_VALUE0),
  STRING_TOKEN(STR_VR_PS1_THRESHOLD_INFO_VALUE1),
  STRING_TOKEN(STR_VR_PS1_THRESHOLD_INFO_VALUE2),
  STRING_TOKEN(STR_VR_PS1_THRESHOLD_INFO_VALUE3)
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mPs2ThresholdInfoStr[] = {
  STRING_TOKEN(STR_VR_PS2_THRESHOLD_INFO_VALUE0),
  STRING_TOKEN(STR_VR_PS2_THRESHOLD_INFO_VALUE1),
  STRING_TOKEN(STR_VR_PS2_THRESHOLD_INFO_VALUE2),
  STRING_TOKEN(STR_VR_PS2_THRESHOLD_INFO_VALUE3)
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mPs3ThresholdInfoStr[] = {
  STRING_TOKEN(STR_VR_PS3_THRESHOLD_INFO_VALUE0),
  STRING_TOKEN(STR_VR_PS3_THRESHOLD_INFO_VALUE1),
  STRING_TOKEN(STR_VR_PS3_THRESHOLD_INFO_VALUE2),
  STRING_TOKEN(STR_VR_PS3_THRESHOLD_INFO_VALUE3)
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mImonOffsetInfoStr[] = {
  STRING_TOKEN(STR_VR_IMON_OFFSET_INFO_VALUE0),
  STRING_TOKEN(STR_VR_IMON_OFFSET_INFO_VALUE1),
  STRING_TOKEN(STR_VR_IMON_OFFSET_INFO_VALUE2),
  STRING_TOKEN(STR_VR_IMON_OFFSET_INFO_VALUE3)
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mImonSlopeInfoStr[] = {
  STRING_TOKEN(STR_VR_IMON_SLOPE_INFO_VALUE0),
  STRING_TOKEN(STR_VR_IMON_SLOPE_INFO_VALUE1),
  STRING_TOKEN(STR_VR_IMON_SLOPE_INFO_VALUE2),
  STRING_TOKEN(STR_VR_IMON_SLOPE_INFO_VALUE3)
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mIccMaxInfoStr[] = {
  STRING_TOKEN(STR_VR_ICCMAX_INFO_VALUE0),
  STRING_TOKEN(STR_VR_ICCMAX_INFO_VALUE1),
  STRING_TOKEN(STR_VR_ICCMAX_INFO_VALUE2),
  STRING_TOKEN(STR_VR_ICCMAX_INFO_VALUE3)
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mIccLimitInfoStr[] = {
  STRING_TOKEN(STR_VR_ICCLIMIT_INFO_VALUE0),
  STRING_TOKEN(STR_VR_ICCLIMIT_INFO_VALUE1),
  STRING_TOKEN(STR_VR_ICCLIMIT_INFO_VALUE2),
  STRING_TOKEN(STR_VR_ICCLIMIT_INFO_VALUE3),
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mTdcCurrentLimitInfoStr[] = {
  STRING_TOKEN(STR_VR_TDC_CURRENT_LIMIT_INFO_VALUE0),
  STRING_TOKEN(STR_VR_TDC_CURRENT_LIMIT_INFO_VALUE1),
  STRING_TOKEN(STR_VR_TDC_CURRENT_LIMIT_INFO_VALUE2),
  STRING_TOKEN(STR_VR_TDC_CURRENT_LIMIT_INFO_VALUE3)
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mVoltageLimitInfoStr[] = {
  STRING_TOKEN(STR_VR_VOLTAGE_LIMIT_INFO_VALUE0),
  STRING_TOKEN(STR_VR_VOLTAGE_LIMIT_INFO_VALUE1),
  STRING_TOKEN(STR_VR_VOLTAGE_LIMIT_INFO_VALUE2),
  STRING_TOKEN(STR_VR_VOLTAGE_LIMIT_INFO_VALUE3)
};

/**
  Display the CPU VR menu programmed defaults.
**/
VOID
InitCpuVrConfig (
  VOID
  )
{
  EFI_STATUS                         Status;
  UINT32                             MailboxStatus;
  UINT64                             TempAcLoadline;
  UINT64                             TempDcLoadline;
  INT64                              ConvertedImonOffset;
  UINT64                             ConvertedImonSlope;
  UINT8                              TempVrAddress;
  UINTN                              VrIndex;
  UINTN                              VariableSize;
  UINT32                             Attributes;
  BOOLEAN                            SvidEnabled;
  PCODE_MAILBOX_INTERFACE            MailboxCommand;
  MAILBOX_DATA_VR_TDC_CONFIG         VrTdcMailboxData;
  MAILBOX_DATA_IMON_CONFIG           ImonMailboxData;
  MAILBOX_DATA_PS_CUTOFF             PsCutoffMailboxData;
  MAILBOX_DATA_ACDC_LOADLINE         AcDcLoadlineMailboxData;
  MAILBOX_DATA_ICC_MAX               IccMaxMailboxData;
  MAILBOX_DATA_FAST_VMODE_ICC_LIMIT  FastVIccMaxMailboxData;
  MAILBOX_DATA_VOLTAGE_LIMIT         VrVoltageLimitMailboxData;
  UINT64                             ConvertedVoltageLimit;

  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  &Attributes,
                  &VariableSize,
                  &mCpuSetup
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "InitCpuVrConfig ()\n"));

  if (mVrDataHob == NULL) {
    return;
  }

  //
  // Avoid array overflow while looping through VR arrays.
  //
  ASSERT (mVrDataHob->MaxNumVrs <= MAX_VR_NUM);
  if (mVrDataHob->MaxNumVrs > MAX_VR_NUM) {
    return;
  }

  ///
  /// VR MSR mailbox
  ///
  for (VrIndex = 0; VrIndex < mVrDataHob->MaxNumVrs; VrIndex++) {
    //
    // If VrIndex is over the count of InfoStr array, end the loop to avoid overflow.
    //
    if (VrIndex >= (sizeof (mIccMaxInfoStr) / sizeof (mIccMaxInfoStr[0])) ||
        VrIndex >= (sizeof (mIccLimitInfoStr) / sizeof (mIccLimitInfoStr[0]))) {
      ASSERT (FALSE);
      break;
    }

    TempVrAddress = mVrDataHob->VrAddress[VrIndex];
    if (VrIndex == 0) {
      SvidEnabled = (mVrDataHob->SvidEnabled & BIT0) != 0;
    } else if (VrIndex == 1) {
      SvidEnabled = (mVrDataHob->SvidEnabled & BIT1) != 0;
    } else if (VrIndex == 2) {
      SvidEnabled = (mVrDataHob->SvidEnabled & BIT2) != 0;
    } else if (VrIndex == 3) {
      SvidEnabled = (mVrDataHob->SvidEnabled & BIT3) != 0;
    } else {
      SvidEnabled = FALSE;
    }

    if ((mCpuSetup.VrConfigEnable[VrIndex] == 1) && SvidEnabled) {
      ///
      /// AC / DC Loadline
      ///
      MailboxCommand.InterfaceData = 0;
      MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
      MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_GET_ACDC_LOADLINE;
      MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
      AcDcLoadlineMailboxData.Data32 = 0;
      Status = MailboxRead (MailboxCommand.InterfaceData, &AcDcLoadlineMailboxData.Data32, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "VR: Error Reading AC/DC Loadline. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
      }
      TempAcLoadline = AcDcLoadlineMailboxData.Fields.AcLoadLine;
      TempDcLoadline = AcDcLoadlineMailboxData.Fields.DcLoadLine;
      ///
      ///  Loadline is 1/100 mOhm units. Mailbox interface requires Loadline in U-4.20 Ohms format.
      ///  After multiplying by 2^20, we must divide the result by 100,000 to convert to Ohms.
      ///  Adding half of divisor to dividend to account for rounding errors in fixed point arithmetic.
      ///
      TempAcLoadline = MultU64x32 (TempAcLoadline, 100000);
      TempAcLoadline = (UINT16) ((TempAcLoadline + (1 << 19)) >> 20);
      TempDcLoadline = MultU64x32 (TempDcLoadline, 100000);
      TempDcLoadline = (UINT16) ((TempDcLoadline + (1 << 19)) >> 20);
      InitString (
        gHiiHandle,
        mAcLoadlineInfoStr[VrIndex],
        L"%d",
        TempAcLoadline
        );
      InitString (
        gHiiHandle,
        mDcLoadlineInfoStr[VrIndex],
        L"%d",
        TempDcLoadline
        );

      ///
      /// PS Cutoff Current
      ///
      MailboxCommand.InterfaceData = 0;
      MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
      MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_GET_PS_CUTOFF;
      MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
      PsCutoffMailboxData.Data32 = 0;
      Status = MailboxRead (MailboxCommand.InterfaceData, &PsCutoffMailboxData.Data32, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "VR: Error Reading PS Cutoff Current. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
      }
      InitString (
        gHiiHandle,
        mPs1ThresholdInfoStr[VrIndex],
        L"%d",
        (UINT16) (PsCutoffMailboxData.Fields.Ps1CutOff)
        );
      InitString (
        gHiiHandle,
        mPs2ThresholdInfoStr[VrIndex],
        L"%d",
        (UINT16) (PsCutoffMailboxData.Fields.Ps2CutOff)
        );
      InitString (
        gHiiHandle,
        mPs3ThresholdInfoStr[VrIndex],
        L"%d",
        (UINT16) (PsCutoffMailboxData.Fields.Ps3CutOff)
        );

      ///
      /// IMON Config
      /// -Policy Imon offset is defined in 1/1000 increments
      /// -Policy Imon slope is defined in 1/100 increments
      /// -Mailbox ImonOffset = (PlatPolicyImonOffset * 2^8)/1000
      /// -Mailbox ImonSlope = (PlatPolicyImonSlope * 2^15)/100
      /// -Adding half of divisor to dividend to account for rounding errors in fixed point arithmetic.
      ///
      MailboxCommand.InterfaceData = 0;
      MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
      MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_GET_IMON_CONFIG;
      MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
      ImonMailboxData.Data32 = 0;
      Status = MailboxRead (MailboxCommand.InterfaceData, &ImonMailboxData.Data32, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "VR: Error Reading IMON Config. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
      }
      FromSignedFixedPoint16 ((UINT16) (ImonMailboxData.Fields.ImonOffset), 1000, 7, 8, &ConvertedImonOffset, NULL);
      FromUnsignedFixedPoint16 ((UINT16) (ImonMailboxData.Fields.ImonSlope), 100, 1, 15, &ConvertedImonSlope, NULL);
      InitString (
        gHiiHandle,
        mImonOffsetInfoStr[VrIndex],
        L"%ld",
        ABS (ConvertedImonOffset)
        );
      InitString (
        gHiiHandle,
        mImonSlopeInfoStr[VrIndex],
        L"%ld",
        ConvertedImonSlope
        );

      ///
      /// Icc Max
      ///
      MailboxCommand.InterfaceData = 0;
      MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
      MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_GET_MAX_ICC;
      MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
      IccMaxMailboxData.Data32 = 0;
      Status = MailboxRead (MailboxCommand.InterfaceData, &IccMaxMailboxData.Data32, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "VR: Error Reading ICC Max. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
      }
      mIccMax[VrIndex] = (UINT16) (IccMaxMailboxData.Fields.MaxIcc);
      InitString (
        gHiiHandle,
        mIccMaxInfoStr[VrIndex],
        L"%d",
        mIccMax[VrIndex]
        );
      DEBUG ((DEBUG_INFO, "InitCpuVrConfig: mIccMax[%d] - %d\n", VrIndex, mIccMax[VrIndex]));

     ///
      /// Icc Limit of Fast Vmode
      ///
      if (mCpuSetup.EnableFastVmode[VrIndex] == 1) {
        MailboxCommand.InterfaceData = 0;
        MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
        MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_GET_FAST_VMODE_ICC_LIMIT;
        MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
        FastVIccMaxMailboxData.Data32 = 0;
        Status = MailboxRead (MailboxCommand.InterfaceData, &FastVIccMaxMailboxData.Data32, &MailboxStatus);
        if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
          DEBUG ((DEBUG_ERROR, "VR: Error Reading ICC Limit. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
        }
        InitString (
          gHiiHandle,
          mIccLimitInfoStr[VrIndex],
          L"%d",
          (UINT16) (FastVIccMaxMailboxData.Fields.FastVModeIccLimit)
          );
        DEBUG ((DEBUG_INFO, "VR: IccLimit[%x] = %d\n", VrIndex, (UINT16)(FastVIccMaxMailboxData.Fields.FastVModeIccLimit)));
      }

      if (mVrDataHob->FastVmodeSupported[VrIndex] == 1 && mCpuSetup.EnableFastVmode[VrIndex] == 1 && mCpuSetup.IccLimit[VrIndex] == 0) {
        //
        // Display/Set the default IccLimit to IccMaxItrip
        //
        mCpuSetup.IccLimit[VrIndex] = mVrDataHob->IccMaxItrip[VrIndex];
      }

      if (mVrDataHob->FastVmodeSupported[VrIndex] == 0) {
        //
        // For some SKUs, default EnableFastVmode has patched to 1, if CPU doesn't support FVM, write back to 0.
        //
        mCpuSetup.EnableFastVmode[VrIndex] = 0;
      }

      ///
      /// VR TDC Settings
      /// -Mailbox TDC Current Limit defined as U15.12.3, Range 0-4095A
      ///  -Policy defined in 1/8 A increments
      ///
      MailboxCommand.InterfaceData = 0;
      MailboxCommand.Fields.Command = MAILBOX_VR_CMD_READ_VR_TDC_CONFIG;
      MailboxCommand.Fields.Param1 = (TempVrAddress & VR_ADDRESS_MASK);
      VrTdcMailboxData.Data32 = 0;
      Status = MailboxRead (MailboxCommand.InterfaceData, &VrTdcMailboxData.Data32, &MailboxStatus);

      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "VR: Error Reading VR TDC Config. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
      }

      mCpuSetup.IrmsSupported[VrIndex] = (UINT8) VrTdcMailboxData.Fields.IrmsSupported;
      DEBUG ((DEBUG_INFO, "mCpuSetup.IrmsSupported[%d] - %d\n", VrIndex, mCpuSetup.IrmsSupported[VrIndex]));

      InitString (
        gHiiHandle,
        mTdcCurrentLimitInfoStr[VrIndex],
        L"%d",
        (UINT16) (VrTdcMailboxData.Fields.CurrentLimit)
        );
      DEBUG ((DEBUG_INFO, "VR: TDC[%x] = %d\n", VrIndex, (UINT16)(VrTdcMailboxData.Fields.CurrentLimit)));

      ///
      /// VR Voltage Limit
      /// -Mailbox Voltage Limit defined as U16.3.13, Range 0-7.999V
      /// -Policy defined in mV, Range 0-7999mV
      ///
      MailboxCommand.InterfaceData = 0;
      MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
      MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_GET_VOLTAGE_LIMIT;
      MailboxCommand.Fields.Param2 = (TempVrAddress & VR_ADDRESS_MASK);
      VrVoltageLimitMailboxData.Data32 = 0;
      Status = MailboxRead (MailboxCommand.InterfaceData, &VrVoltageLimitMailboxData.Data32, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "VR: Error Reading VR Voltage Config. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
      }
      FromUnsignedFixedPoint16 ((UINT16) (VrVoltageLimitMailboxData.Fields.VoltageLimit), 1000, 3, 13, &ConvertedVoltageLimit, NULL);
      InitString (
        gHiiHandle,
        mVoltageLimitInfoStr[VrIndex],
        L"%d",
        (UINT16) ConvertedVoltageLimit
        );
      DEBUG ((DEBUG_INFO, "VR: Voltage Limit[%x] = %d\n", VrIndex, (UINT16) ConvertedVoltageLimit));
    }
  }

  Status = gRT->SetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  Attributes,
                  VariableSize,
                  &mCpuSetup
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  Initialize Turbo Ratio strings.

**/
VOID
InitTurboRatioInfo (
  VOID
  )
{
  UINT8                                             TurboRatioLimitRatio;
  MSR_TURBO_RATIO_LIMIT_REGISTER                    MsrTurboRatioLimitRatio;
  MSR_TURBO_RATIO_LIMIT_CORES_REGISTER              MsrTurboRatioLimitNumCore;
  MSR_ATOM_TURBO_RATIO_LIMIT_REGISTER               AtomMsrTurboRatioLimitRatio;
  MSR_ATOM_TURBO_RATIO_LIMIT_CORES_REGISTER         AtomMsrTurboRatioLimitNumCore;

  //
  // Turbo Ratio Limit Ratio
  //
  MsrTurboRatioLimitRatio.Uint64 = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT);
  TurboRatioLimitRatio = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit0;
  if (mCoreRatioFinal[0] == 0) {
    mCoreRatioFinal[0] = TurboRatioLimitRatio;
  }

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_RATIO0_VALUE),
    L"%d",
    TurboRatioLimitRatio
    );

  TurboRatioLimitRatio = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit1;
  if (mCoreRatioFinal[1] == 0) {
    mCoreRatioFinal[1] = TurboRatioLimitRatio;
  }

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_RATIO1_VALUE),
    L"%d",
    TurboRatioLimitRatio
    );

  TurboRatioLimitRatio = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit2;
  if (mCoreRatioFinal[2] == 0) {
    mCoreRatioFinal[2] = TurboRatioLimitRatio;
  }

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_RATIO2_VALUE),
    L"%d",
    TurboRatioLimitRatio
    );

  TurboRatioLimitRatio = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit3;
  if (mCoreRatioFinal[3] == 0) {
    mCoreRatioFinal[3] = TurboRatioLimitRatio;
  }
  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_RATIO3_VALUE),
    L"%d",
    TurboRatioLimitRatio
    );

  TurboRatioLimitRatio = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit4;
  if (mCoreRatioFinal[4] == 0) {
    mCoreRatioFinal[4] = TurboRatioLimitRatio;
  }
  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_RATIO4_VALUE),
    L"%d",
    TurboRatioLimitRatio
    );

  TurboRatioLimitRatio = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit5;
  if (mCoreRatioFinal[5] == 0) {
    mCoreRatioFinal[5] = TurboRatioLimitRatio;
  }
  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_RATIO5_VALUE),
    L"%d",
    TurboRatioLimitRatio
    );

  TurboRatioLimitRatio = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit6;
  if (mCoreRatioFinal[6] == 0) {
    mCoreRatioFinal[6] = TurboRatioLimitRatio;
  }
  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_RATIO6_VALUE),
    L"%d",
    TurboRatioLimitRatio
    );

  TurboRatioLimitRatio = (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit7;
  if (mCoreRatioFinal[7] == 0) {
    mCoreRatioFinal[7] = TurboRatioLimitRatio;
  }
  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_RATIO7_VALUE),
    L"%d",
    TurboRatioLimitRatio
    );

  //
  // Turbo Ratio Limit Num Core
  //
  MsrTurboRatioLimitNumCore.Uint64 = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT_CORES);

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_NUMCORE0_VALUE),
    L"%d",
    MsrTurboRatioLimitNumCore.Bits.CoreCount0
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_NUMCORE1_VALUE),
    L"%d",
    MsrTurboRatioLimitNumCore.Bits.CoreCount1
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_NUMCORE2_VALUE),
    L"%d",
    MsrTurboRatioLimitNumCore.Bits.CoreCount2
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_NUMCORE3_VALUE),
    L"%d",
    MsrTurboRatioLimitNumCore.Bits.CoreCount3
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_NUMCORE4_VALUE),
    L"%d",
    MsrTurboRatioLimitNumCore.Bits.CoreCount4
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_NUMCORE5_VALUE),
    L"%d",
    MsrTurboRatioLimitNumCore.Bits.CoreCount5
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_NUMCORE6_VALUE),
    L"%d",
    MsrTurboRatioLimitNumCore.Bits.CoreCount6
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TURBO_RATIO_LIMIT_NUMCORE7_VALUE),
    L"%d",
    MsrTurboRatioLimitNumCore.Bits.CoreCount7
    );

  //
  // ATOM Turbo Ratio Limit Ratio
  //
  AtomMsrTurboRatioLimitRatio.Uint64 = AsmReadMsr64 (MSR_ATOM_TURBO_RATIO_LIMIT);

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_RATIO0_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit0
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_RATIO1_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit1
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_RATIO2_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit2
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_RATIO3_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit3
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_RATIO4_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit4
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_RATIO5_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit5
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_RATIO6_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit6
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_RATIO7_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitRatio.Bits.RatioLimit7
    );

  //
  // ATOM Turbo Ratio Limit Num Core
  //
  AtomMsrTurboRatioLimitNumCore.Uint64 = AsmReadMsr64 (MSR_ATOM_TURBO_RATIO_LIMIT_CORES);

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_NUMCORE0_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitNumCore.Bits.CoreCount0
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_NUMCORE1_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitNumCore.Bits.CoreCount1
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_NUMCORE2_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitNumCore.Bits.CoreCount2
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_NUMCORE3_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitNumCore.Bits.CoreCount3
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_NUMCORE4_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitNumCore.Bits.CoreCount4
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_NUMCORE5_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitNumCore.Bits.CoreCount5
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_NUMCORE6_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitNumCore.Bits.CoreCount6
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_ATOM_TURBO_RATIO_LIMIT_NUMCORE7_VALUE),
    L"%d",
    AtomMsrTurboRatioLimitNumCore.Bits.CoreCount7
    );
}

/**
  Initialize Power Limit 4 Boost string.

**/
VOID
InitPl4BoostInfo (
  VOID
  )
{
  UINT32                  MailboxStatus;
  UINT32                  MailboxData;
  EFI_STATUS              Status;
  PCODE_MAILBOX_INTERFACE MailboxCommand;
  UINT16                  Pl4BoostInteger;
  UINT16                  Pl4BoostFraction;

  //
  // Get current PL4_BOOST value from PCODE mailbox.
  //
  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.Command = MAILBOX_BIOS_CMD_PLATFORM_CONFIG;
  MailboxCommand.Fields.Param1 = MAILBOX_BIOS_SUBCMD_READ_PL4_BOOST_POWER;
  MailboxData = 0;
  Status = MailboxRead (MailboxCommand.InterfaceData, &MailboxData, &MailboxStatus);
  if (Status != EFI_SUCCESS || MailboxStatus != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Get PL4 Boost config failed. MailboxStatus = %x, Mailbox command return status: %r\n", MailboxStatus, Status));
  }

  //
  // Bit[12:0] represents PL4_BOOST value in 0.125 W increments.
  //
  Pl4BoostInteger = Pl4BoostFraction = (UINT16) (MailboxData & 0x1FFF);

  Pl4BoostInteger = Pl4BoostInteger / 8;
  Pl4BoostFraction = (Pl4BoostFraction % 8) * 125;

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_CURRENT_POWER_LIMIT_4_BOOST_VALUE),
    L"%d.%d",
    Pl4BoostInteger,
    Pl4BoostFraction
    );
}

/**
  Initialize CPU strings and VR topology.

  @param[in] EFI_EVENT    Event
  @param[in] VOID         *Context
**/
VOID
InitCPUInfo (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                           Status;
  UINTN                                VariableSize;
  UINT32                               VariableAttributes;
  UINT32                               Index;
  CHAR8                                String[15];
  MSR_BIOS_SIGN_ID_REGISTER            MsrSignId;
  UINT16                               PowerLimitInteger;
  UINT16                               PowerLimitFraction;
  UINT16                               PowerLimitInteger2;
  UINT16                               PowerLimitFraction2;
  UINT8                                PowerUnit;
  CPUID_VERSION_INFO_ECX               Ecx;
  UINT8                                CtdpLevels;
  UINT8                                CtdpRatio;
  UINT16                               CtdpTdp;
  UINT32                               CtdpAddress;
  CPU_INFO_PROTOCOL                    *DxeCpuInfo;
  UINT32                               CpuSetupVolVarAttr;
  CPU_SETUP_VOLATILE_DATA              CpuSetupVolData;
  CONST CHAR8                          *GenerationString = NULL;
  CONST CHAR8                          *SkuString = NULL;
  MSR_IA32_DEBUG_INTERFACE_REGISTER    DebugInterfaceReg = {0};
  VOID                                 *Hob;
  MSR_PACKAGE_POWER_SKU_REGISTER       PackagePowerSkuMsr;
  MSR_PACKAGE_POWER_SKU_UNIT_REGISTER  PackagePowerSkuUnitMsr;
  MSR_PACKAGE_RAPL_LIMIT_REGISTER      PackageRaplLimitMsr;
  MSR_CONFIG_TDP_NOMINAL_REGISTER      ConfigTdpNominalMsr;
  MSR_CONFIG_TDP_LEVEL1_REGISTER       ConfigTdpLevel1Msr;
  MSR_CONFIG_TDP_LEVEL2_REGISTER       ConfigTdpLevel2Msr;
  MSR_TURBO_ACTIVATION_RATIO_REGISTER  TurboActivationRatioMsr;
  UINT32                               CpuFeatureVarAttr;
  UINTN                                NumberOfProcessors;
  UINTN                                NumberOfEnabledProcessors;
  EFI_PROCESSOR_INFORMATION            *ProcessorInfo;
  UINT8                                LpAtomCoreCount;
  UINT8                                LpAtomThreadCount;

  PowerLimitInteger = PowerLimitFraction = 0;
  PowerUnit                              = 0;

  mVrDataHob = NULL;

  VariableSize  = sizeof (SETUP_VOLATILE_DATA);
  Status = gRT->GetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &mSetupVolatileData
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return ;
  }

  ///
  /// Locate DxeCpuInfo protocol instance and gather CPU information
  ///
  Status = gBS->LocateProtocol (&gCpuInfoProtocolGuid, NULL, (VOID **) &DxeCpuInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,"Failed to locate DxeCpuInfo Protocol\n"));
    return;
  }

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **) &mMpService);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,"Failed to locate EfiMpService Protocol\n"));
    return;
  }
  ASSERT_EFI_ERROR (Status);

  Status = mMpService->GetNumberOfProcessors (
                        mMpService,
                        &NumberOfProcessors,
                        &NumberOfEnabledProcessors
                        );
  ASSERT_EFI_ERROR (Status);

  // Prepare zero memory for ProcessorInfo
  ProcessorInfo = (EFI_PROCESSOR_INFORMATION *) AllocateZeroPool (sizeof (EFI_PROCESSOR_INFORMATION) * NumberOfProcessors);

  if (ProcessorInfo == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR,"Failed to Allocate memory for ProcessorInfo\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  for (Index = 0; Index < NumberOfProcessors; Index++) {
    Status = mMpService->GetProcessorInfo (
                          mMpService,
                          Index | CPU_V2_EXTENDED_TOPOLOGY,
                          &ProcessorInfo[Index]
                          );
    ASSERT_EFI_ERROR (Status);
  }

  ///
  /// Update current Debug interface MSR enable/disable state
  ///
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);
  if (Ecx.Bits.SDBG == 1) {
    DebugInterfaceReg.Uint64 = AsmReadMsr64 (MSR_IA32_DEBUG_INTERFACE);
  }

  if (DebugInterfaceReg.Bits.DebugOccurred) {
    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_DEBUG_MSR_INTERFACE_STATUS_VALUE),
      L"%a",
      "Enabled"
      );
  } else {
    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_DEBUG_MSR_INTERFACE_STATUS_VALUE),
      L"%a",
      "Disabled"
      );
  }

  if (DxeCpuInfo->CpuInfo[BIG_CORE].NumCores != 0){
    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_PROCESSOR_VERSION_VALUE),
      L"%a",
      DxeCpuInfo->CpuInfo[BIG_CORE].BrandString
      );

    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_PROCESSOR_SPEED_VALUE),
      L"%d MHz",
      DxeCpuInfo->CpuInfo[BIG_CORE].IntendedFreq
      );

    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_PROCESSOR_ID_VALUE),
      L"0x%X",
      DxeCpuInfo->CpuInfo[BIG_CORE].CpuSignature
      );
  } else {

    ///
    /// Update with small core data
    ///
    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_PROCESSOR_VERSION_VALUE),
      L"%a",
      DxeCpuInfo->CpuInfo[SMALL_CORE].BrandString
      );

    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_PROCESSOR_SPEED_VALUE),
      L"%d MHz",
      DxeCpuInfo->CpuInfo[SMALL_CORE].IntendedFreq
      );

    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_PROCESSOR_ID_VALUE),
      L"0x%X",
      DxeCpuInfo->CpuInfo[SMALL_CORE].CpuSignature
      );
  }

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_PACKAGE_VALUE),
    L"%a",
    "Not Implemented Yet"
    );

  //
  // Update the Microcode Revision
  //
  MsrSignId.Uint64 = AsmReadMsr64 (MSR_BIOS_SIGN_ID);
  if (MsrSignId.Bits.PatchId != 0) {
    InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_MICROCODE_VALUE),
    L"%x",
    MsrSignId.Bits.PatchId
    );
  }

  Status = GetLpECoreThreadCountInfo (ProcessorInfo, NumberOfProcessors, &LpAtomCoreCount, &LpAtomThreadCount);
  ASSERT_EFI_ERROR (Status);

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_ATOM_COUNT_VALUE),
    L"%dCore(s) / %dThread(s)",
    DxeCpuInfo->CpuInfo[SMALL_CORE].NumCores - LpAtomCoreCount,
    DxeCpuInfo->CpuInfo[SMALL_CORE].NumCores * DxeCpuInfo->CpuInfo[SMALL_CORE].NumHts - LpAtomThreadCount
    );

  if (ProcessorInfo != NULL) {
    // Deallocate ProcessorInfo
    FreePool (ProcessorInfo);
    ProcessorInfo = NULL;
  }

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_LP_ATOM_COUNT_VALUE),
    L"%dCore(s) / %dThread(s)",
    LpAtomCoreCount,
    LpAtomThreadCount
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_CORE_COUNT_VALUE),
    L"%dCore(s) / %dThread(s)",
    DxeCpuInfo->CpuInfo[BIG_CORE].NumCores,
    DxeCpuInfo->CpuInfo[BIG_CORE].NumCores * DxeCpuInfo->CpuInfo[BIG_CORE].NumHts
    );

  if (DxeCpuInfo->CpuInfo[BIG_CORE].NumCores != 0){
    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_PROCESSOR_BRAND_VALUE),
      L"%a",
      DxeCpuInfo->CpuInfo[BIG_CORE].BrandString
      );
  } else {
    //
    // Update processor brand with small core data
    //
    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_PROCESSOR_BRAND_VALUE),
      L"%a",
      DxeCpuInfo->CpuInfo[SMALL_CORE].BrandString
      );
  }

  for (Index = 0; Index <= DxeCpuInfo->CpuInfo[SMALL_CORE].MaxCacheSupported; ++Index) {
    switch (DxeCpuInfo->CpuInfo[SMALL_CORE].CacheInfo[Index].Level) {
    case 1:
      //
      // L1 cache size is per core. Display the size per core times number of cores.
      // If there's only 1 enabled core, simply display the size per core.
      //
      if (DxeCpuInfo->CpuInfo[SMALL_CORE].CacheInfo[Index].Type == 1) {
        InitString (
          gHiiHandle,
          STRING_TOKEN (STR_ATOM_CORE_L1_DATA_CACHE_VALUE),
          L"%d KB",
          DxeCpuInfo->CpuInfo[SMALL_CORE].CacheInfo[Index].Size
          );
      } else if (DxeCpuInfo->CpuInfo[SMALL_CORE].CacheInfo[Index].Type == 2) {
        InitString (
          gHiiHandle,
          STRING_TOKEN (STR_ATOM_CORE_L1_INSTR_CACHE_VALUE),
          L"%d KB",
          DxeCpuInfo->CpuInfo[SMALL_CORE].CacheInfo[Index].Size
          );
      }
      break;

    case 2:
      //
      // L2 cache size is shared per 4 atom cores. Display the size per core times number of cores.
      // If there's less than 4 enabled core, simply display the size per core.
      //
      InitString (
        gHiiHandle,
        STRING_TOKEN (STR_ATOM_CORE_L2_CACHE_VALUE),
        L"%d KB",
        DxeCpuInfo->CpuInfo[SMALL_CORE].CacheInfo[Index].Size
        );
      break;

    case 3:
      //
      // L3 cache size is not per core. It is shared between cores.
      //
      InitString (
        gHiiHandle,
        STRING_TOKEN (STR_ATOM_CORE_L3_CACHE_VALUE),
        L"%d MB",
        DxeCpuInfo->CpuInfo[SMALL_CORE].CacheInfo[Index].Size / 1024
        );
      break;
    }
  }

  for (Index = 0; Index <= DxeCpuInfo->CpuInfo[BIG_CORE].MaxCacheSupported; ++Index) {
    switch (DxeCpuInfo->CpuInfo[BIG_CORE].CacheInfo[Index].Level) {
    case 1:
      //
      // L1 cache size is per core. Display the size per core times number of cores.
      // If there's only 1 enabled core, simply display the size per core.
      //
      if (DxeCpuInfo->CpuInfo[BIG_CORE].CacheInfo[Index].Type == 1) {
        InitString (
          gHiiHandle,
          STRING_TOKEN (STR_CORE_L0_DATA_CACHE_VALUE),
          L"%d KB",
          DxeCpuInfo->CpuInfo[BIG_CORE].CacheInfo[Index].Size
          );
      } else if (DxeCpuInfo->CpuInfo[BIG_CORE].CacheInfo[Index].Type == 2) {
        InitString (
          gHiiHandle,
          STRING_TOKEN (STR_CORE_L1_INSTR_CACHE_VALUE),
          L"%d KB",
          DxeCpuInfo->CpuInfo[BIG_CORE].CacheInfo[Index].Size
          );
      }
      break;

    case 2:
      //
      // L2 cache size is per core. Display the size per core times number of cores.
      // If there's only 1 enabled core, simply display the size per core.
      //
      InitString (
        gHiiHandle,
        STRING_TOKEN (STR_CORE_L2_CACHE_VALUE),
        L"%d KB",
        DxeCpuInfo->CpuInfo[BIG_CORE].CacheInfo[Index].Size
        );
      break;

    case 3:
      //
      // L3 cache size is not per core. It is shared between cores.
      //
      InitString (
        gHiiHandle,
        STRING_TOKEN (STR_CORE_L3_CACHE_VALUE),
        L"%d MB",
        DxeCpuInfo->CpuInfo[BIG_CORE].CacheInfo[Index].Size / 1024
        );
      break;
    }
  }

  GenerationString = GetGenerationString ();
  SkuString = GetSkuString ();
  if ((GenerationString != NULL) && (SkuString != NULL)) {
    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_PROCESSOR_VALUE),
      L"%a %a",
      GenerationString,
      SkuString
    );
  } else {
    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_PROCESSOR_VALUE),
      L"%a",
      "Unknown"
    );
  }

  VariableSize = sizeof (SETUP_CPU_FEATURES);
  Status = gRT->GetVariable (
                  L"SetupCpuFeatures",
                  &gSetupVariableGuid,
                  &CpuFeatureVarAttr,
                  &VariableSize,
                  &mSetupCpuFeatures
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return ;
  }

  VariableSize = sizeof (CPU_SETUP);
  gRT->GetVariable (
         L"CpuSetup",
         &gCpuSetupVariableGuid,
         &VariableAttributes,
         &VariableSize,
         &mCpuSetup
         );

  //
  // Init CPU the number of all fused big, small cores and Atom cores off Ring.
  //
  GetCpuFusedCoreCount (
    0,
    (mCpuSetup.AllBigCoreCount == 0) ? &mCpuSetup.AllBigCoreCount :NULL,
    (mCpuSetup.AllSmallCoreCount == 0)? &mCpuSetup.AllSmallCoreCount : NULL,
    (mCpuSetup.AllLpAtomCoreCount == 0)? &mCpuSetup.AllLpAtomCoreCount :NULL
  );

  if (mSetupCpuFeatures.VTAvailable) {
    AsciiSPrint (String, 15, "Supported");
  } else {
    AsciiSPrint (String, 15, "Not Supported");
  }

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_VMX_VALUE),
    L"%a",
    String
    );

  if (mSetupCpuFeatures.TXTAvailable) {
    AsciiSPrint (String, 15, "Supported");
  } else {
    AsciiSPrint (String, 15, "Not Supported");
  }

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_SMX_VALUE),
    L"%a",
    String
    );

   //
   // TXT/BTG related information
   //
  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_CRASH_REGISTER_VALUE),
    L"0x%08x",
    MmioRead32(TXT_PUBLIC_BASE + TXT_CRASHCODE_REG_OFF)
    );

   InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_SPAD_REGISTER_VALUE),
    L"0x%016lx",
    MmioRead64(TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_BOOTSTATUS)
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_BTG_STATUS_VALUE),
    L"0x%08x",
    MmioRead32(TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_ACM_STATUS)
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_BTG_ACM_POL_STS_VALUE),
    L"0x%016lx",
    MmioRead64(TXT_PUBLIC_BASE + R_CPU_ACM_POLICY_STATUS)
    );

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_PROCESSOR_BTG_SACM_INFO_VALUE),
    L"0x%016lx",
    AsmReadMsr64(MSR_BOOT_GUARD_SACM_INFO)
    );

  //
  // Package Power SKU Unit
  //
  PackagePowerSkuUnitMsr.Uint64  = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU_UNIT);
  PowerUnit = (UINT8) PackagePowerSkuUnitMsr.Bits.PwrUnit;
  PowerUnit = (UINT8) LShiftU64 (2, (PowerUnit - 1));

  if (PowerUnit == 0 ) {
    ASSERT (FALSE);
    return;
  }

  //
  // Min and Max Turbo Power Limit
  //
  PackagePowerSkuMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  PowerLimitFraction = PowerLimitInteger = (UINT16) PackagePowerSkuMsr.Bits.PkgMaxPwr;

  //
  // 0 for Max Power Limit means no limit, so set the value to 0x7FFF (max value possible)
  //
  if (PowerLimitInteger == 0 ) {
    PowerLimitFraction = PowerLimitInteger= 0x7FFF;
  }

  PowerLimitInteger = PowerLimitInteger / PowerUnit;
  PowerLimitFraction = PowerLimitFraction % PowerUnit;

  // Now convert fractional part into 3 decimal place
  PowerLimitFraction = (PowerLimitFraction * 1000 ) / PowerUnit;

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_MAX_TURBO_POWER_LIMIT_VALUE),
    L"%d.%d",
    PowerLimitInteger,PowerLimitFraction
    );

  PowerLimitFraction = PowerLimitInteger = (UINT16) PackagePowerSkuMsr.Bits.PkgMinPwr;

  PowerLimitInteger = PowerLimitInteger / PowerUnit;
  PowerLimitFraction = PowerLimitFraction % PowerUnit;

  // Now convert fractional part into 3 decimal place
  PowerLimitFraction = (PowerLimitFraction * 1000 ) / PowerUnit;

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_MIN_TURBO_POWER_LIMIT_VALUE),
    L"%d.%d",
    PowerLimitInteger,PowerLimitFraction
    );

  //
  // Package TDP Limit
  //
  PowerLimitFraction = PowerLimitInteger = (UINT16) PackagePowerSkuMsr.Bits.PkgTdp;

  PowerLimitInteger = PowerLimitInteger / PowerUnit;
  PowerLimitFraction = PowerLimitFraction % PowerUnit;

  // Now convert fractional part into 3 decimal place
  PowerLimitFraction = (PowerLimitFraction * 1000 ) / PowerUnit;

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_TDP_LIMIT_VALUE),
    L"%d.%d",
    PowerLimitInteger,PowerLimitFraction
    );

  //
  // Turbo Power Limit
  //
  PackageRaplLimitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_RAPL_LIMIT);
  //
  // Long duration power limit
  //
  PowerLimitFraction = PowerLimitInteger = (UINT16) PackageRaplLimitMsr.Bits.PkgPwrLim1;

  PowerLimitInteger = PowerLimitInteger / PowerUnit;
  PowerLimitFraction = PowerLimitFraction % PowerUnit;

  // Now convert fractional part into 3 decimal place
  PowerLimitFraction = (PowerLimitFraction * 1000 ) / PowerUnit;

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_LONG_DUR_PWR_LIMIT_VALUE),
    L"%d.%d",
    PowerLimitInteger,
    PowerLimitFraction
    );

  //
  // Short duration power limit
  //    Notice, we are using TempMsr.Dwords.High, so reusing Power Limit 1 Mask
  //
  PowerLimitFraction = PowerLimitInteger = (UINT16) PackageRaplLimitMsr.Bits.PkgPwrLim2;

  PowerLimitInteger = PowerLimitInteger / PowerUnit;
  PowerLimitFraction = PowerLimitFraction % PowerUnit;

  // Now convert fractional part into 3 decimal place
  PowerLimitFraction = (PowerLimitFraction * 1000 ) / PowerUnit;

  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_SHORT_DUR_PWR_LIMIT_VALUE),
    L"%d.%d",
    PowerLimitInteger,
    PowerLimitFraction
    );

  //
  // Initialize Turbo Ratio strings
  //
  InitTurboRatioInfo ();

  //
  // Get the number of configurable TDP Levels supported
  //
  CtdpLevels = GetConfigTdpLevels ();
  InitString (
    gHiiHandle,
    STRING_TOKEN (STR_CTDP_LEVELS_VALUE),
    L"%d",
    CtdpLevels + 1
    );

  if (CtdpLevels != 0) {

    //
    // Get Nominal Ratio
    //
    ConfigTdpNominalMsr.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_NOMINAL);
    CtdpRatio = (UINT8) ConfigTdpNominalMsr.Bits.TdpRatio;

    // Package TDP Limit
    PowerLimitFraction                      = PowerLimitInteger = (UINT16) PackagePowerSkuMsr.Bits.PkgTdp;
    PowerLimitInteger                       = PowerLimitInteger / PowerUnit;
    PowerLimitFraction                      = PowerLimitFraction % PowerUnit;
    // Now convert fractional part into 3 decimal place
    PowerLimitFraction = (PowerLimitFraction * 1000) / PowerUnit;

    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_CTDP_NOMINAL_VALUE),
      L"Ratio:%d TAR:%d PL1:%d.%dW",
      CtdpRatio,
      CtdpRatio - 1,
      PowerLimitInteger,
      PowerLimitFraction
      );

    //
    // Get Level1 Ratio and TDP
    //
    ConfigTdpLevel1Msr.Uint64   = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL1);
    CtdpRatio = (UINT8) ConfigTdpLevel1Msr.Bits.TdpRatio;

    PowerLimitFraction                      = PowerLimitInteger = (UINT16) ConfigTdpLevel1Msr.Bits.PkgTdp;
    PowerLimitInteger                       = PowerLimitInteger / PowerUnit;
    PowerLimitFraction                      = PowerLimitFraction % PowerUnit;
    // Now convert fractional part into 3 decimal place
    PowerLimitFraction = (PowerLimitFraction * 1000) / PowerUnit;

    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_CTDP_LEVEL1_VALUE),
      L"Ratio:%d TAR:%d PL1:%d.%dW",
      CtdpRatio,
      CtdpRatio - 1,
      PowerLimitInteger,
      PowerLimitFraction
      );

    //
    // Get Level2 Ratio and TDP
    //
    ConfigTdpLevel2Msr.Uint64   = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL2);
    CtdpRatio = (UINT8) ConfigTdpLevel2Msr.Bits.TdpRatio;

    PowerLimitFraction                      = PowerLimitInteger = (UINT16) ConfigTdpLevel2Msr.Bits.PkgTdp;
    PowerLimitInteger                       = PowerLimitInteger / PowerUnit;
    PowerLimitFraction                      = PowerLimitFraction % PowerUnit;
    // Now convert fractional part into 3 decimal place
    PowerLimitFraction = (PowerLimitFraction * 1000) / PowerUnit;

    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_CTDP_LEVEL2_VALUE),
      L"Ratio:%d TAR:%d PL1:%d.%dW",
      CtdpRatio,
      CtdpRatio - 1,
      PowerLimitInteger,
      PowerLimitFraction
      );

    //
    // Compare TDP ratio to check if Min or Max cTDP Level
    //
    if (ConfigTdpLevel1Msr.Bits.TdpRatio > ConfigTdpNominalMsr.Bits.TdpRatio) {
      mCpuSetup.CompareCtdpRatio = 1;
    } else {
      mCpuSetup.CompareCtdpRatio = 0;
    }

    if (ConfigTdpLevel2Msr.Bits.TdpRatio > ConfigTdpNominalMsr.Bits.TdpRatio) {
      mCpuSetup.CompareCtdpRatio2 = 1;
    } else {
      mCpuSetup.CompareCtdpRatio2 = 0;
    }

    //
    // Get MMIO Power Limit 1
    //
    CtdpAddress = (UINT32) ((UINTN) PcdGet64 (PcdMchBaseAddress) + 0x59A0);
    CtdpTdp = (UINT16) ((*((UINT32*) ((UINTN) CtdpAddress))) & 0x7FFF);

    PowerLimitFraction                      = PowerLimitInteger = CtdpTdp;
    PowerLimitInteger                       = PowerLimitInteger / PowerUnit;
    PowerLimitFraction                      = PowerLimitFraction % PowerUnit;
    // Now convert fractional part into 3 decimal place
    PowerLimitFraction = (PowerLimitFraction * 1000) / PowerUnit;

    //
    // Get MSR Power Limit 1
    //
    PackageRaplLimitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_RAPL_LIMIT);
    PowerLimitFraction2 = PowerLimitInteger2 = (UINT16) PackageRaplLimitMsr.Bits.PkgPwrLim1;
    PowerLimitInteger2                      = PowerLimitInteger2 / PowerUnit;
    PowerLimitFraction2                     = PowerLimitFraction2 % PowerUnit;
    // Now convert fractional part into 3 decimal place
    PowerLimitFraction2 = (PowerLimitFraction2 * 1000) / PowerUnit;

    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_CTDP_PWR_LIMIT1_VALUE),
      L"%d.%dW (MSR:%d.%d)",
      PowerLimitInteger,
      PowerLimitFraction,
      PowerLimitInteger2,
      PowerLimitFraction2
      );

    //
    // Get MMIO Power Limit 2
    //
    CtdpAddress = (UINT32) ((UINTN) PcdGet64 (PcdMchBaseAddress) + 0x59A4);
    CtdpTdp = (UINT16) ((*((UINT32*) ((UINTN) CtdpAddress))) & 0x7FFF);

    PowerLimitFraction                      = PowerLimitInteger = CtdpTdp;
    PowerLimitInteger                       = PowerLimitInteger / PowerUnit;
    PowerLimitFraction                      = PowerLimitFraction % PowerUnit;
    // Now convert fractional part into 3 decimal place
    PowerLimitFraction = (PowerLimitFraction * 1000) / PowerUnit;

    //
    // Get MSR Power Limit 2
    //
    PowerLimitFraction2                     = PowerLimitInteger2 = (UINT16) PackageRaplLimitMsr.Bits.PkgPwrLim2;
    PowerLimitInteger2                      = PowerLimitInteger2 / PowerUnit;
    PowerLimitFraction2                     = PowerLimitFraction2 % PowerUnit;
    // Now convert fractional part into 3 decimal place
    PowerLimitFraction2 = (PowerLimitFraction2 * 1000) / PowerUnit;

    InitString (
      gHiiHandle,
      STRING_TOKEN (STR_CTDP_PWR_LIMIT2_VALUE),
      L"%d.%dW (MSR:%d.%d)",
      PowerLimitInteger,
      PowerLimitFraction,
      PowerLimitInteger2,
      PowerLimitFraction2
      );

    //
    // Get TAR value
    //
    TurboActivationRatioMsr.Uint64   = AsmReadMsr64 (MSR_TURBO_ACTIVATION_RATIO);
    if (TurboActivationRatioMsr.Bits.TurboActivationRatioLock) {
      InitString (
        gHiiHandle,
        STRING_TOKEN (STR_CTDP_TAR_VALUE),
        L"%d (Locked)",
        TurboActivationRatioMsr.Bits.MaxNonTurboRatio
        );
    } else {
      InitString (
        gHiiHandle,
        STRING_TOKEN (STR_CTDP_TAR_VALUE),
        L"%d (Unlocked)",
        TurboActivationRatioMsr.Bits.MaxNonTurboRatio
        );
    }
  }

  gRT->SetVariable (
        L"CpuSetup",
        &gCpuSetupVariableGuid,
        VariableAttributes,
        sizeof (mCpuSetup),
        &mCpuSetup
        );

  VariableSize = sizeof (CPU_SETUP_VOLATILE_DATA);
  Status = gRT->GetVariable (
                  L"CpuSetupVolatileData",
                  &gCpuSetupVariableGuid,
                  &CpuSetupVolVarAttr,
                  &VariableSize,
                  &CpuSetupVolData
                  );
  if (EFI_ERROR (Status)) {
    CpuSetupVolVarAttr = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
  }
  ASSERT_EFI_ERROR (Status);

 ///
 /// Get CPU VR topology
 ///
  Hob = (VR_DATA_HOB *) GetFirstGuidHob (&gVrDataHobGuid);
  mVrDataHob = (VR_DATA_HOB *)((UINTN)Hob + sizeof (EFI_HOB_GUID_TYPE));

  if (mVrDataHob == NULL) {
    DEBUG((DEBUG_ERROR, "Vr Data HOB not available\n"));
    ASSERT (mVrDataHob != NULL);
  }

  //
  // Lock the VR submenu if it does not use SVID.
  // VR Type. 1 - No SVID VR, 0 - SVID VR
  //
  if (mVrDataHob == NULL) {
    CpuSetupVolData.CoreVrLocked = 0;
    CpuSetupVolData.GtVrLocked   = 0;
    CpuSetupVolData.SaVrLocked   = 0;
    CpuSetupVolData.AtomVrLocked = 0;
  } else {
    CpuSetupVolData.CoreVrLocked = (mVrDataHob->SvidEnabled & BIT0) == 0;
    CpuSetupVolData.GtVrLocked   = (mVrDataHob->SvidEnabled & BIT1) == 0;
    CpuSetupVolData.SaVrLocked   = (mVrDataHob->SvidEnabled & BIT2) == 0;
    CpuSetupVolData.AtomVrLocked = (mVrDataHob->SvidEnabled & BIT3) == 0;
  }

  Status = gRT->SetVariable (
                  L"CpuSetupVolatileData",
                  &gCpuSetupVariableGuid,
                  CpuSetupVolVarAttr,
                  sizeof (CpuSetupVolData),
                  &CpuSetupVolData
                  );
  ASSERT_EFI_ERROR (Status);

  InitPl4BoostInfo ();
}

/**
  Cpu setup callback

  @param[in] EFI_EVENT    Event
  @param[in] VOID         *Context
**/
VOID
EFIAPI
CpuSetupCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                Status;
  UINTN                     VariableSize;

  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &mCpuSetup
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return ;
  }

  SaveCpuRatio (mCpuSetup.CpuRatio);
  ///
  /// Save the Alias Check request to use in early PEI phase
  ///
  SaveTxtAliasCheck (mCpuSetup.AcheckRequest);

}

/**
  Initialize CPU strings.

  @param[in] EFI_HII_HANDLE   HiiHandle
  @param[in] UINT16           Class
**/
VOID
InitCPUStrings (
  EFI_HII_HANDLE HiiHandle,
  UINT16         Class
  )
{
  EFI_EVENT             SetupNvramCallbackEvt;
  VOID                  *SetupNvramCallbackReg;
  CONST CHAR8           *RevisionTableString = NULL;

  if ((Class == MAIN_FORM_SET_CLASS) || (Class == ADVANCED_FORM_SET_CLASS)) {
    DEBUG ((DEBUG_INFO, "<InitCPUStrings>"));
    gHiiHandle  = HiiHandle;

    InitBootFrequencyDefault (NULL, NULL);
    InitTurboRatioDefault (NULL, NULL);
    InitTxtDefault();
    InitTxtAcheckDefault ();
    InitCPUInfo (NULL, NULL);
///
///

    SetupNvramCallbackEvt = EfiCreateProtocolNotifyEvent (
                              &gSetupNvramUpdateGuid,
                              TPL_CALLBACK,
                              CpuSetupCallback,
                              NULL,
                              &SetupNvramCallbackReg
                              );
    ASSERT (SetupNvramCallbackEvt != NULL);
  }

  if (Class == MAIN_FORM_SET_CLASS) {
    RevisionTableString = GetRevisionTable ();
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_PROCESSOR_STEPPING_VALUE),
      L"%a",
      RevisionTableString
      );
  }
  InitCpuVrConfig (); ///< Display CPU VR config default programmed values.
#if FixedPcdGetBool(PcdTcssSupport) == 1
  OverrideCpuCxLimitForTcssIom ();
#endif // FixedPcdGetBool(PcdTcssSupport) == 1
}

/**
  This function validates the Flex Ratio setup value and Turbo capability

  @param[in] EFI_FORM_CALLBACK_PROTOCOL   *This
  @param[in] UINT16                       KeyValue
  @param[in] EFI_IFR_DATA_ARRAY           *Data,
  @param[in] EFI_HII_CALLBACK_PACKET      **Packet

  @retval EFI_SUCCESS        Call Back Function executed successfully
**/
EFI_STATUS
EFIAPI
CpuFormCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  CPU_SETUP                  *CpuSetup;
  UINT8                      RatioLimitMin;
  UINTN                      VarSize;
  UINTN                      VariableSize;
  EFI_STATUS                 Status;
  EFI_STRING                 RequestString;
  UINT8                      MaxBusRatio;
  UINT8                      MinBusRatio;
  CNV_FORM_PLATFORM_PROTOCOL *CnvFormPlatformProtocol;

  MaxBusRatio = 0;
  MinBusRatio = 0;

  Status = EFI_UNSUPPORTED;
  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    if (Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
      Status = EFI_SUCCESS;
      switch (KeyValue) {
        case KEY_CpuRatioLimit:
          break;

        case KEY_RatioLimitRatio0:
          Value->u8 = mCpuSetup.RatioLimitRatioDefault[0];
          break;

        case KEY_RatioLimitRatio1:
          Value->u8 = mCpuSetup.RatioLimitRatioDefault[1];
          break;

        case KEY_RatioLimitRatio2:
          Value->u8 = mCpuSetup.RatioLimitRatioDefault[2];
          break;

        case KEY_RatioLimitRatio3:
          Value->u8 = mCpuSetup.RatioLimitRatioDefault[3];
          break;

        case KEY_RatioLimitRatio4:
          Value->u8 = mCpuSetup.RatioLimitRatioDefault[4];
          break;

        case KEY_RatioLimitRatio5:
          Value->u8 = mCpuSetup.RatioLimitRatioDefault[5];
          break;

        case KEY_RatioLimitRatio6:
          Value->u8 = mCpuSetup.RatioLimitRatioDefault[6];
          break;

        case KEY_RatioLimitRatio7:
          Value->u8 = mCpuSetup.RatioLimitRatioDefault[7];
          break;

        case KEY_RatioLimitNumCore0:
          Value->u8 = mCpuSetup.RatioLimitNumCoreDefault[0];
          break;

        case KEY_RatioLimitNumCore1:
          Value->u8 = mCpuSetup.RatioLimitNumCoreDefault[1];
          break;

        case KEY_RatioLimitNumCore2:
          Value->u8 = mCpuSetup.RatioLimitNumCoreDefault[2];
          break;

        case KEY_RatioLimitNumCore3:
          Value->u8 = mCpuSetup.RatioLimitNumCoreDefault[3];
          break;

        case KEY_RatioLimitNumCore4:
          Value->u8 = mCpuSetup.RatioLimitNumCoreDefault[4];
          break;

        case KEY_RatioLimitNumCore5:
          Value->u8 = mCpuSetup.RatioLimitNumCoreDefault[5];
          break;

        case KEY_RatioLimitNumCore6:
          Value->u8 = mCpuSetup.RatioLimitNumCoreDefault[6];
          break;

        case KEY_RatioLimitNumCore7:
          Value->u8 = mCpuSetup.RatioLimitNumCoreDefault[7];
          break;

        case KEY_AtomRatioLimitRatio0:
          Value->u8 = mCpuSetup.AtomRatioLimitRatioDefault[0];
          break;

        case KEY_AtomRatioLimitRatio1:
          Value->u8 = mCpuSetup.AtomRatioLimitRatioDefault[1];
          break;

        case KEY_AtomRatioLimitRatio2:
          Value->u8 = mCpuSetup.AtomRatioLimitRatioDefault[2];
          break;

        case KEY_AtomRatioLimitRatio3:
          Value->u8 = mCpuSetup.AtomRatioLimitRatioDefault[3];
          break;

        case KEY_AtomRatioLimitRatio4:
          Value->u8 = mCpuSetup.AtomRatioLimitRatioDefault[4];
          break;

        case KEY_AtomRatioLimitRatio5:
          Value->u8 = mCpuSetup.AtomRatioLimitRatioDefault[5];
          break;

        case KEY_AtomRatioLimitRatio6:
          Value->u8 = mCpuSetup.AtomRatioLimitRatioDefault[6];
          break;

        case KEY_AtomRatioLimitRatio7:
          Value->u8 = mCpuSetup.AtomRatioLimitRatioDefault[7];
          break;

        case KEY_AtomRatioLimitNumCore0:
          Value->u8 = mCpuSetup.AtomRatioLimitNumCoreDefault[0];
          break;

        case KEY_AtomRatioLimitNumCore1:
          Value->u8 = mCpuSetup.AtomRatioLimitNumCoreDefault[1];
          break;

        case KEY_AtomRatioLimitNumCore2:
          Value->u8 = mCpuSetup.AtomRatioLimitNumCoreDefault[2];
          break;

        case KEY_AtomRatioLimitNumCore3:
          Value->u8 = mCpuSetup.AtomRatioLimitNumCoreDefault[3];
          break;

        case KEY_AtomRatioLimitNumCore4:
          Value->u8 = mCpuSetup.AtomRatioLimitNumCoreDefault[4];
          break;

        case KEY_AtomRatioLimitNumCore5:
          Value->u8 = mCpuSetup.AtomRatioLimitNumCoreDefault[5];
          break;

        case KEY_AtomRatioLimitNumCore6:
          Value->u8 = mCpuSetup.AtomRatioLimitNumCoreDefault[6];
          break;

        case KEY_AtomRatioLimitNumCore7:
          Value->u8 = mCpuSetup.AtomRatioLimitNumCoreDefault[7];
          break;

        case KEY_BootFrequency:
          Value->u8 = mCpuSetup.BootFrequencyDefault;
          break;

        case KEY_TurboMode:
          Value->u8 = mCpuSetup.TurboModeDefault;
          break;

        default:
          Status = EFI_UNSUPPORTED;
      }
    }
    return Status;
  }

  RequestString = NULL;
  Status = EFI_SUCCESS;
  VarSize = sizeof (CPU_SETUP);
  CpuSetup = AllocatePool (VarSize);
  ASSERT (CpuSetup != NULL);
  if (CpuSetup == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // GetBrowserData by VarStore Name (Setup)
  if (!HiiGetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  //
  // Get Maximum Non-Turbo bus ratio (HFM) from Platform Info MSR Bits[15:8]
  //
  GetBusRatio (&MaxBusRatio, &MinBusRatio);

  //
  // Save the Max Non Turbo Ratio
  //
  if (MaxBusRatio > CpuSetup->MaxNonTurboRatio) {
    CpuSetup->MaxNonTurboRatio = MaxBusRatio;
  } else {
    MaxBusRatio = CpuSetup->MaxNonTurboRatio;
  }

  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &mCpuSetup
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // If Flex Ratio Override is disabled, use the original fused value.
  // Otherwise, use the current Flex Ratio value.
  //
  if (mCpuSetup.CpuRatioOverride == 0) {
    RatioLimitMin = mCpuSetup.FlexRatioOverrideDefault;
  } else {
    RatioLimitMin = mCpuSetup.CpuRatio;
  }

  switch (KeyValue) {
    case KEY_CpuRatioLimit:
      break;

    case KEY_RatioLimitRatio0:
      if (CpuSetup->RatioLimitRatio[0] < RatioLimitMin) {
        CpuSetup->RatioLimitRatio[0] = mCoreRatioFinal[0];
      }
      mCoreRatioFinal[0] = CpuSetup->RatioLimitRatio[0];
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, RatioLimitRatio[0]), sizeof (CpuSetup->RatioLimitRatio[0]));
      break;

    case KEY_RatioLimitRatio1:
      if (CpuSetup->RatioLimitRatio[1] < RatioLimitMin) {
        CpuSetup->RatioLimitRatio[1] = mCoreRatioFinal[1];
      }
      mCoreRatioFinal[1] = CpuSetup->RatioLimitRatio[1];
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, RatioLimitRatio[1]), sizeof (CpuSetup->RatioLimitRatio[1]));
      break;

    case KEY_RatioLimitRatio2:
      if (CpuSetup->RatioLimitRatio[2] < RatioLimitMin) {
        CpuSetup->RatioLimitRatio[2] = mCoreRatioFinal[2];
      }
      mCoreRatioFinal[2] = CpuSetup->RatioLimitRatio[2];
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, RatioLimitRatio[2]), sizeof (CpuSetup->RatioLimitRatio[2]));
      break;

    case KEY_RatioLimitRatio3:
      if (CpuSetup->RatioLimitRatio[3] < RatioLimitMin) {
        CpuSetup->RatioLimitRatio[3] = mCoreRatioFinal[3];
      }
      mCoreRatioFinal[3] = CpuSetup->RatioLimitRatio[3];
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, RatioLimitRatio[3]), sizeof (CpuSetup->RatioLimitRatio[3]));
      break;

    case KEY_RatioLimitRatio4:
      if (CpuSetup->RatioLimitRatio[4] < RatioLimitMin) {
        CpuSetup->RatioLimitRatio[4] = mCoreRatioFinal[4];
      }
      mCoreRatioFinal[4] = CpuSetup->RatioLimitRatio[4];
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, RatioLimitRatio[4]), sizeof (CpuSetup->RatioLimitRatio[4]));
      break;

    case KEY_RatioLimitRatio5:
      if (CpuSetup->RatioLimitRatio[5] < RatioLimitMin) {
        CpuSetup->RatioLimitRatio[5] = mCoreRatioFinal[5];
      }
      mCoreRatioFinal[5] = CpuSetup->RatioLimitRatio[5];
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, RatioLimitRatio[5]), sizeof (CpuSetup->RatioLimitRatio[5]));
      break;

    case KEY_RatioLimitRatio6:
      if (CpuSetup->RatioLimitRatio[6] < RatioLimitMin) {
        CpuSetup->RatioLimitRatio[6] = mCoreRatioFinal[6];
      }
      mCoreRatioFinal[6] = CpuSetup->RatioLimitRatio[6];
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, RatioLimitRatio[6]), sizeof (CpuSetup->RatioLimitRatio[6]));
      break;

    case KEY_RatioLimitRatio7:
      if (CpuSetup->RatioLimitRatio[7] < RatioLimitMin) {
        CpuSetup->RatioLimitRatio[7] = mCoreRatioFinal[7];
      }
      mCoreRatioFinal[7] = CpuSetup->RatioLimitRatio[7];
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, RatioLimitRatio[7]), sizeof (CpuSetup->RatioLimitRatio[7]));
      break;

    case KEY_DLVR_RFIM:
      Status = gBS->LocateProtocol (&gCnvFormPlatformProtocolGuid, NULL, (VOID **) &CnvFormPlatformProtocol);
      ASSERT_EFI_ERROR (Status);
      PcdSet8S (PcdGlobalCnvDlvrRfim, CpuSetup->DlvrRfiEnable);
      break;

    default:
      break;
  }

  if (RequestString != NULL) {
    VarSize = sizeof (CPU_SETUP);
    if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup, RequestString)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);
    FreePool (RequestString);
  }

  FreePool (CpuSetup);

  return EFI_SUCCESS;
}

/**
  Initial CPU MNTR default in SETUP variable and VFR.

  @retval EFI_SUCCESS             The initialization is done.
  @retval EFI_NOT_FOUND           Failed to initial CPU MNTR.
**/
EFI_STATUS
EFIAPI
InitCpuMntrDefault (
  VOID
  )
{
  EFI_STATUS    Status;
  UINTN         VariableSize;
  UINT32        Attributes;
  UINT8         MaxBusRatio;
  UINT8         MinBusRatio;

  MaxBusRatio = 0;
  MinBusRatio = 0;

  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  &Attributes,
                  &VariableSize,
                  &mCpuSetup
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GetBusRatio (&MaxBusRatio, &MinBusRatio);
  if (mCpuSetup.CpuRatioOverride == 0) {
    mCpuSetup.CpuDefaultRatio = MaxBusRatio;
  }
  mCpuSetup.CpuRatio = MaxBusRatio;

  Status = gRT->SetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  Attributes,
                  VariableSize,
                  &mCpuSetup
                  );
  Status = InitCpuMntrDefaultVfr (mCpuSetup.CpuDefaultRatio);

  return EFI_SUCCESS;
}


/**
  Update the min, max, and default values for CpuRatio.

  @param[in] CpuRatioDefault      The CPU MNTR default.

  @retval EFI_SUCCESS             Values updated successfully.
  @retval EFI_NOT_FOUND           Failed to update it.
**/
EFI_STATUS
InitCpuMntrDefaultVfr (
  UINT8      CpuRatioDefault
  )
{
  EFI_HII_PACKAGE_HEADER *PackageHdr;
  CHAR8                  *BytePtr;
  EFI_IFR_OP_HEADER      *ParentIfr;
  EFI_IFR_GUID_LABEL     *LabelPtr;
  EFI_IFR_DEFAULT        *DefaultPtr;
  UINTN                  PackageSize;
  EFI_IFR_NUMERIC        *NumericPtr;

  //
  // add Array Length (UINT32) to point to package header.
  //
  PackageHdr = (EFI_HII_PACKAGE_HEADER *) (AdvancedBin + sizeof (UINT32));
  PackageSize = PackageHdr->Length;
  PackageSize = PackageSize & 0x00ffffff;
  BytePtr = (CHAR8 *) (PackageHdr + 1);

  //
  // loop to find CPU RATIO label.
  //
  while (TRUE) {
    if (BytePtr >= (CHAR8 *) PackageHdr + PackageSize) {
      DEBUG ((DEBUG_ERROR, "Couldn't find the target node to patch.\n"));
      return EFI_NOT_FOUND;
    }

    ParentIfr = (EFI_IFR_OP_HEADER *) BytePtr;
    if (ParentIfr->OpCode == EFI_IFR_GUID_OP) {
      LabelPtr = (EFI_IFR_GUID_LABEL *) ParentIfr;
      if (CompareGuid ((EFI_GUID *) (VOID *) &LabelPtr->Guid, &gEfiIfrTianoGuid) && (LabelPtr->Number == LABEL_CPU_RATIO)) {
        BytePtr += sizeof (EFI_IFR_GUID_LABEL);
        break;
      }
    }
    BytePtr += ParentIfr->Length;
  }

  //
  // loop to find CPU ratio numeric OP code.
  //
  while (TRUE) {
    if (BytePtr >= (CHAR8 *) PackageHdr + PackageSize) {
      DEBUG ((DEBUG_ERROR, "Couldn't find the target node to patch.\n"));
      return EFI_NOT_FOUND;
    }

    ParentIfr = (EFI_IFR_OP_HEADER *) BytePtr;
    if (ParentIfr->OpCode == EFI_IFR_NUMERIC_OP) {
      NumericPtr = (EFI_IFR_NUMERIC *) ParentIfr;
      NumericPtr->data.u8.MinValue = GetMaxEfficiencyRatio ();
      NumericPtr->data.u8.MaxValue = CpuRatioDefault;
      break;
    }
    BytePtr += ParentIfr->Length;
  }

  //
  // advance to numeric default OP code.
  //
  while (TRUE) {
    if (BytePtr >= (CHAR8 *) PackageHdr + PackageSize) {
      DEBUG ((DEBUG_ERROR, "Couldn't find the target node to patch.\n"));
      return EFI_NOT_FOUND;
    }

    ParentIfr = (EFI_IFR_OP_HEADER *) BytePtr;
    if (ParentIfr->OpCode == EFI_IFR_DEFAULT_OP) {
      DefaultPtr = (EFI_IFR_DEFAULT *) ParentIfr;
      break;
    }
    BytePtr += ParentIfr->Length;
  }

  //
  // Now the Default value is found. Patch it!
  //
  DefaultPtr->Value.u8 = CpuRatioDefault;
  return EFI_SUCCESS;
}

/**
  This function resets the dynamic VR data when the VrPowerDelivery is updated

  @param[in] EFI_FORM_CALLBACK_PROTOCOL   *This
  @param[in] UINT16                       KeyValue
  @param[in] EFI_IFR_DATA_ARRAY           *Data,
  @param[in] EFI_HII_CALLBACK_PACKET      **Packet

  @retval EFI_SUCCESS        Call Back Function executed successfully
**/
EFI_STATUS
EFIAPI
CpuVrConfigCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  CPU_SETUP               CpuSetup;
  UINTN                   VarSize;
  EFI_STATUS              Status;
  EFI_STRING              RequestString;
  EFI_HII_POPUP_PROTOCOL  *HiiPopUp;
  EFI_HII_POPUP_SELECTION PopUpSelect;

  Status = EFI_SUCCESS;
  RequestString = NULL;

  Status = gBS->LocateProtocol (&gEfiHiiPopupProtocolGuid, NULL, (VOID**) &HiiPopUp);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Can't find Form PopUp protocol. Exit (%r)\n", Status));
    return Status;
  }

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  VarSize  = sizeof (CPU_SETUP);
  // GetBrowserData by VarStore Name (Setup)
  if (!HiiGetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) &CpuSetup)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  switch (KeyValue) {
    case KEY_CoreVrEnableFastVmode:
      if (Value->u8 == 0) {
        //
        // Disable the FVM, set the IccLimit to 0.
        //
        CpuSetup.IccLimit[0] = 0;
      } else {
        //
        // Set to default IccLimit (IccMaxItrip) when FVM enable (Spec defined).
        //
        CpuSetup.IccLimit[0] = MIN (mVrDataHob->IccMaxItrip[0], MAX_ICC_LIMIT);
      }
      DEBUG ((DEBUG_INFO, "CpuVrConfigCallBackFunction (knob): CpuSetup.IccLimit[0] - %d\n", CpuSetup.IccLimit[0]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, IccLimit[0]), sizeof (CpuSetup.IccLimit[0]));
      if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) &CpuSetup, RequestString)) {
        Status = EFI_NOT_FOUND;
      }
      ASSERT_EFI_ERROR (Status);
      break;

    case KEY_GtVrEnableFastVmode:
      if (Value->u8 == 0) {
        //
        // Disable the FVM, set the IccLimit to 0.
        //
        CpuSetup.IccLimit[1] = 0;
      } else {
        //
        // Set to default IccLimit (IccMaxItrip) when FVM enable (Spec not defined yet).
        //
        CpuSetup.IccLimit[1] = MIN (mVrDataHob->IccMaxItrip[1], MAX_ICC_LIMIT);
      }
      DEBUG ((DEBUG_INFO, "CpuVrConfigCallBackFunction (knob): CpuSetup.IccLimit[1] - %d\n", CpuSetup.IccLimit[1]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, IccLimit[1]), sizeof (CpuSetup.IccLimit[1]));
      if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) &CpuSetup, RequestString)) {
        Status = EFI_NOT_FOUND;
      }
      ASSERT_EFI_ERROR (Status);
      break;

    case KEY_SaVrEnableFastVmode:
      if (Value->u8 == 0) {
        //
        // Disable the FVM, set the IccLimit to 0.
        //
        CpuSetup.IccLimit[2] = 0;
      } else {
        //
        // Set to default IccLimit (IccMaxItrip) when FVM enable (Spec not defined yet).
        //
        CpuSetup.IccLimit[2] = MIN (mVrDataHob->IccMaxItrip[2], MAX_ICC_LIMIT);
      }
      DEBUG ((DEBUG_INFO, "CpuVrConfigCallBackFunction (knob): CpuSetup.IccLimit[2] - %d\n", CpuSetup.IccLimit[2]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, IccLimit[2]), sizeof (CpuSetup.IccLimit[2]));
      if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) &CpuSetup, RequestString)) {
        Status = EFI_NOT_FOUND;
      }
      ASSERT_EFI_ERROR (Status);
      break;

    case KEY_IccLimit0:
      if (CpuSetup.IccLimit[0] > MIN (mIccMax[0], MAX_ICC_LIMIT)) {
        HiiPopUp->CreatePopup (
          HiiPopUp,
          EfiHiiPopupStyleError,
          EfiHiiPopupTypeOk,
          gHiiHandle,
          STRING_TOKEN (STR_VR_FAST_VMODE_ICC_LIMIT_POPUP_WARN_MAX),
          &PopUpSelect
          );

        CpuSetup.IccLimit[0] = MIN (mIccMax[0], MAX_ICC_LIMIT);
      }

      DEBUG ((DEBUG_INFO, "CpuVrConfigCallBackFunction (value): CpuSetup.IccLimit[0] - %d\n", CpuSetup.IccLimit[0]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, IccLimit[0]), sizeof (CpuSetup.IccLimit[0]));
      if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) &CpuSetup, RequestString)) {
        Status = EFI_NOT_FOUND;
      }
      ASSERT_EFI_ERROR (Status);
      break;

    case KEY_IccLimit1:
      if (CpuSetup.IccLimit[1] > MIN (mIccMax[1], MAX_ICC_LIMIT)) {
        HiiPopUp->CreatePopup (
          HiiPopUp,
          EfiHiiPopupStyleError,
          EfiHiiPopupTypeOk,
          gHiiHandle,
          STRING_TOKEN (STR_VR_FAST_VMODE_ICC_LIMIT_POPUP_WARN_MAX),
          &PopUpSelect
          );

        CpuSetup.IccLimit[1] = MIN (mIccMax[1], MAX_ICC_LIMIT);
      }

      DEBUG ((DEBUG_INFO, "CpuVrConfigCallBackFunction (value): CpuSetup.IccLimit[1] - %d\n", CpuSetup.IccLimit[1]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, IccLimit[1]), sizeof (CpuSetup.IccLimit[1]));
      if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) &CpuSetup, RequestString)) {
        Status = EFI_NOT_FOUND;
      }
      ASSERT_EFI_ERROR (Status);
      break;

    case KEY_IccLimit2:
      if (CpuSetup.IccLimit[2] > MIN (mIccMax[2], MAX_ICC_LIMIT)) {
        HiiPopUp->CreatePopup (
          HiiPopUp,
          EfiHiiPopupStyleError,
          EfiHiiPopupTypeOk,
          gHiiHandle,
          STRING_TOKEN (STR_VR_FAST_VMODE_ICC_LIMIT_POPUP_WARN_MAX),
          &PopUpSelect
          );

        CpuSetup.IccLimit[2] = MIN (mIccMax[2], MAX_ICC_LIMIT);
      }

      DEBUG ((DEBUG_INFO, "CpuVrConfigCallBackFunction (value): CpuSetup.IccLimit[2] - %d\n", CpuSetup.IccLimit[2]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, IccLimit[2]), sizeof (CpuSetup.IccLimit[2]));
      if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) &CpuSetup, RequestString)) {
        Status = EFI_NOT_FOUND;
      }
      ASSERT_EFI_ERROR (Status);
      break;

    default:
      ASSERT(FALSE);
  }

  if (RequestString != NULL) {
    FreePool (RequestString);
  }

  return EFI_SUCCESS;
}

/**
  This function updates any configuration parameter affected by changing MKTME
  setting. At the moment, the CpuSetup.TdxEnable needs to be set to "DISABLE"
  if MKTME is disabled.
  @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action           Specifies the type of action taken by the browser.
  @param[in] QuestionId       A unique value which is sent to the original
                              exporting driver so that it can identify the type
                              of data to expect.
  @param[in] Type             The type of value for the question.
  @param[in] Value            A pointer to the data being sent to the original
                              exporting driver.
  @param[out] ActionRequest   On return, points to the action requested by the
                              callback function.

  @retval EFI_SUCCESS     - The function completed successfully
  @retval EFI_UNSUPPORTED - The Setup Menu was not updated
  @retval EFI_NOT_FOUND   - There is an error in Locating protocols or retrieving
                          browser data
**/
EFI_STATUS
EFIAPI
MktmeCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  EFI_STATUS              Status;
  CPU_SETUP               *CpuSetup;
  UINTN                   VarSize;
  EFI_STRING              RequestString;

///
///

  Status = EFI_SUCCESS;
  RequestString = NULL;

  DEBUG ((DEBUG_INFO, "Inside MktmeCallBackFunction!\n"));
  //
  // Only process MKTME_STATUS_QUESTION_ID callback
  //
  if (KeyValue != MKTME_STATUS_QUESTION_ID) {
    return EFI_UNSUPPORTED;
  }

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  VarSize   = sizeof (CPU_SETUP);
  CpuSetup  = AllocatePool (VarSize);
  ASSERT (CpuSetup != NULL);

  if (CpuSetup == NULL) {
    DEBUG ((DEBUG_ERROR, "MktmeCallBackFunction: Out of resources allocating CpuSetup!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // GetBrowserData by VarStore Name (Setup)
  //
  if (!HiiGetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup)) {
    FreePool (CpuSetup);
    DEBUG ((DEBUG_ERROR, "MktmeCallBackFunction: CpuSetup not found!\n"));
    return EFI_NOT_FOUND;
  }

  //
  //  The only time we change CpuSetup.TdxEnable is when
  //  The MKTME setting is being set to "DISABLE"
  //
  if (Value->u8 == 0) {

    //
    // TME encrypts the entire system memory, including the memory used by the hypervisor and guest VMs.
    // TDX has a dependency on TME to provide a secure environment for virtual machines.
    // Making sure If MKTME is disabled, Tdx must be disabled.
    //

    // Check to see if TdxEnable is 1, then set it to 0
    if (CpuSetup->TdxEnable == 1) {
      CpuSetup->TdxEnable = 0;
    }

    //
    //  Construct the Browser string for CpuSetup.TdxEnable and then set it.
    //
    RequestString = HiiConstructRequestString (
                      RequestString,
                      OFFSET_OF (CPU_SETUP, TdxEnable),
                      sizeof (CpuSetup->TdxEnable)
                      );

    if (RequestString != NULL) {
      if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup, RequestString)) {
        DEBUG ((DEBUG_ERROR, "MktmeCallBackFunction: Setting Hii string ERROR!\n"));
        Status = EFI_NOT_FOUND;
      }

      FreePool (RequestString);
    }

///
///
  }

  FreePool (CpuSetup);

  return Status;
}

/**
  This function updates any configuration parameter affected by changing VMX
  setting. At the moment, the CpuSetup.TdxEnable needs to be set to "DISABLE"
  if VMX is disabled.

  @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action           Specifies the type of action taken by the browser.
  @param[in] QuestionId       A unique value which is sent to the original
                              exporting driver so that it can identify the type
                              of data to expect.
  @param[in] Type             The type of value for the question.
  @param[in] Value            A pointer to the data being sent to the original
                              exporting driver.
  @param[out] ActionRequest   On return, points to the action requested by the
                              callback function.

  @retval EFI_SUCCESS     - The function completed successfully
  @retval EFI_UNSUPPORTED - The Setup Menu was not updated
  @retval EFI_NOT_FOUND   - There is an error in Locating protocols or retrieving
                          browser data
**/
EFI_STATUS
EFIAPI
VTCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  EFI_STATUS              Status;
  CPU_SETUP               *CpuSetup;
  UINTN                   VarSize;
  EFI_STRING              RequestString;

  Status = EFI_SUCCESS;
  RequestString = NULL;

  DEBUG ((DEBUG_INFO, "Inside VTCallBackFunction!\n"));
  //
  // Only process VT_QUESTION_ID callback
  //
  if (KeyValue != VT_QUESTION_ID) {
    return EFI_UNSUPPORTED;
  }

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  VarSize   = sizeof (CPU_SETUP);
  CpuSetup  = AllocatePool (VarSize);
  ASSERT (CpuSetup != NULL);

  if (CpuSetup == NULL) {
    DEBUG ((DEBUG_ERROR, "VTCallBackFunction: Out of resources allocating CpuSetup!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // GetBrowserData by VarStore Name (Setup)
  //
  if (!HiiGetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup)) {
    FreePool (CpuSetup);
    DEBUG ((DEBUG_ERROR, "VTCallBackFunction: CpuSetup not found!\n"));
    return EFI_NOT_FOUND;
  }

  //
  //  The only time we change CpuSetup.TdxEnable is when
  //  The VT setting is being set to "DISABLE"
  //
  if (Value->u8 == 0) {

    //
    // TDX has a dependency on VMX to provide a secure environment for virtual machines.
    // Making sure If VT is disabled, Tdx must be disabled.
    //

    // Check to see if TdxEnable is 1, then set it to 0
    if (CpuSetup->TdxEnable == 1) {
      CpuSetup->TdxEnable = 0;
    }

    //
    //  Construct the Browser string for CpuSetup.TdxEnable and then set it.
    //
    RequestString = HiiConstructRequestString (
                      RequestString,
                      OFFSET_OF (CPU_SETUP, TdxEnable),
                      sizeof (CpuSetup->TdxEnable)
                      );

    if (RequestString != NULL) {
      if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup, RequestString)) {
        DEBUG ((DEBUG_ERROR, "VTCallBackFunction: Setting Hii string ERROR!\n"));
        Status = EFI_NOT_FOUND;
      }

      FreePool (RequestString);
    }

  }

  FreePool (CpuSetup);

  return Status;
}
