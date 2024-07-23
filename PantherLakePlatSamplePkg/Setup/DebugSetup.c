/**@file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <Uefi/UefiSpec.h>
#include <SetupPrivate.h>
#include <Library/CpuPlatformLib.h>
#include <Library/HobLib.h>
#include <ConfigBlock.h>
#include <Ppi/SiPolicy.h>
#include <TraceHubConfig.h>
#include <MemInfoHob.h>
#include <Protocol/MpService.h>
#include <Register/HeciRegs.h>
#include <DciConfig.h>
#include <Library/MeInfoLib.h>
#include <Library/DebugToken.h>

/*
  Get Debug Token Data

  @param[out]  DebugToken       Pointer to Token data

  @retval      TRUE             Debug Token is existed
  @retval      FALSE            Debug Token is not existed
*/
BOOLEAN
GetDebugTokenData (
  OUT DEBUG_TOKEN_KNOB_DATA     *DebugToken
  )
{
  DEBUG_TOKEN_DATA_HOB             *DebugTokenHob;
  EFI_HOB_GUID_TYPE                *GuidHob;

  GuidHob = GetFirstGuidHob (&gDebugTokenDataHobGuid);
  if (GuidHob == NULL) {
    return FALSE;
  }
  DebugTokenHob = (DEBUG_TOKEN_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
  DebugToken->Data = DebugTokenHob->DebugTokenData;
  DEBUG ((DEBUG_INFO, "DebugToken->Data = %x\n", DebugToken->Data));
  return TRUE;
}


/**
  Initialize CrashLog Unique_ID Collection Control option with checking if End Of Manufacturing.
  If EOM, gray out the option; otherwise, show in CrashLog setup menu.
**/
VOID
InitCrashLogPrivacyConsentOption (
  VOID
  )
{
  EFI_STATUS                  Status;
  UINTN                       PchVarSize;
  UINT32                      PchVarAttr;
  HECI_FW_STS6_REGISTER       MeFirmwareStatus;

  PchVarSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  &PchVarAttr,
                  &PchVarSize,
                  &mPchSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a (): Get PchSetup Fail. Status = %r\n", __FUNCTION__, Status));
  }

  //
  // Checking if End Of Manufacturing
  //
  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS_6);
  mPchSetup.PmcPrivacyConsentSupport = (MeFirmwareStatus.r.FpfSocConfigLock) ? FALSE : TRUE;

  Status = gRT->SetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  PchVarAttr,
                  PchVarSize,
                  &mPchSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a (): Set PchSetup Fail. Status = %r\n", __FUNCTION__, Status));
  }
}

/*
  Create and update DebugSetupVolatile Data
*/
VOID
InitDebugSetupVolatileData (
  VOID
  )
{
  EFI_STATUS                      Status;
  DEBUG_SETUP_VOLATILE_DATA       DebugSetupVolData;
  UINT32                          DebugSetupVolVarAttr;
  UINTN                           VariableSize;
  DEBUG_TOKEN_KNOB_DATA           DebugToken;
  BOOLEAN                         DebugTokenExisted;

  ZeroMem (&DebugSetupVolData, sizeof (DebugSetupVolData));
  DebugSetupVolVarAttr = EFI_VARIABLE_BOOTSERVICE_ACCESS;
  VariableSize = sizeof (DEBUG_SETUP_VOLATILE_DATA);
  Status = gRT->GetVariable (
                  L"DebugSetupVolatileData",
                  &gSetupVariableGuid,
                  &DebugSetupVolVarAttr,
                  &VariableSize,
                  &DebugSetupVolData
                  );
  if (Status == EFI_NOT_FOUND) {
    DebugTokenExisted = GetDebugTokenData (&DebugToken);
    if (DebugTokenExisted) {
      if (DebugToken.Fields.StreamingTraceSink != SetupMenuDefault) {
        DebugSetupVolData.StreamingTraceSink = 1;
      }
    }
    if (DebugToken.Fields.ProcessorTraceEnable == TRUE) {
      DebugSetupVolData.ProcessorTraceEnable = 1;
    }
    DebugSetupVolData.PchIsAttached = (UINT8) FALSE;
    Status = gRT->SetVariable (
                    L"DebugSetupVolatileData",
                    &gSetupVariableGuid,
                    DebugSetupVolVarAttr,
                    sizeof (DebugSetupVolData),
                    &DebugSetupVolData
                    );
    ASSERT_EFI_ERROR (Status);
  }
}

/**
  Initialize Debug strings.

  @param[in] EFI_HII_HANDLE   HiiHandle
  @param[in] UINT16           Class
**/
VOID
InitDebugStrings (
  EFI_HII_HANDLE HiiHandle,
  UINT16         Class
  )
{
  SI_SETUP                    SiSetup;
  UINTN                       SiVarSize;
  UINT32                      SiVarAttr;
  UINTN                       PchVarSize;
  UINT32                      PchVarAttr;
  UINTN                       CpuVarSize;
  UINT32                      CpuVarAttr;
  EFI_STATUS                  Status;
  DEBUG_TOKEN_KNOB_DATA       DebugToken;
  BOOLEAN                     DebugTokenExisted;
  BIOS_PAYLOAD                BiosPayload;
  UINTN                       BiosPayloadVarSize;
  UINT32                      BiosPayloadVarAttr;

  DebugTokenExisted = GetDebugTokenData (&DebugToken);

  InitCrashLogPrivacyConsentOption ();

  SiVarSize = sizeof (SI_SETUP);
  Status = gRT->GetVariable (L"SiSetup", &gSiSetupVariableGuid, &SiVarAttr, &SiVarSize, &SiSetup);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get SiSetup Fail. Status = %r\n", Status));
    return;
  }

  PchVarSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (L"PchSetup", &gPchSetupVariableGuid, &PchVarAttr, &PchVarSize, &mPchSetup);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get PchSetup Fail. Status = %r\n", Status));
    return;
  }

  CpuVarSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (L"CpuSetup", &gCpuSetupVariableGuid, &CpuVarAttr, &CpuVarSize, &mCpuSetup);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get CpuSetup Fail. Status = %r\n", Status));
    return;
  }

  BiosPayloadVarSize = sizeof (BIOS_PAYLOAD);
  BiosPayload.TokenExist = 0;
  BiosPayload.DebugSetupBackup.PlatformDebugOption = DisableDebug;
  Status = gRT->GetVariable (BIOS_PAYLOAD_VARIABLE_NAME, &gBiosPayloadVarGuid, &BiosPayloadVarAttr, &BiosPayloadVarSize, &BiosPayload);
  if (Status == EFI_NOT_FOUND) {
    ZeroMem (&BiosPayload, BiosPayloadVarSize);
    BiosPayloadVarAttr = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
    gRT->SetVariable (BIOS_PAYLOAD_VARIABLE_NAME, &gBiosPayloadVarGuid, BiosPayloadVarAttr, BiosPayloadVarSize, (VOID *) &BiosPayload);
  }
  if (DebugTokenExisted) {
    if (BiosPayload.TokenExist == 0) {
      //
      // Token injection, backup Debug Setup
      //
      DEBUG ((DEBUG_INFO, "Token is injected, backup Debug Setup\n"));
      BiosPayload.DebugSetupBackup.PlatformDebugOption = SiSetup.PlatformDebugOption;
      BiosPayload.TokenExist = 1;
      gRT->SetVariable (BIOS_PAYLOAD_VARIABLE_NAME, &gBiosPayloadVarGuid, BiosPayloadVarAttr, BiosPayloadVarSize, (VOID *) &BiosPayload);
      DEBUG ((DEBUG_INFO, "Backup PlatformDebugOption = %x\n", SiSetup.PlatformDebugOption));
    }
    InitString(
      HiiHandle, STRING_TOKEN(STR_DEBUG_TOKEN_PRESENCE_VALUE),
      L"Yes"
    );

    if (DebugToken.Fields.StreamingTraceSink != SetupMenuDefault) {
      switch (DebugToken.Fields.StreamingTraceSink) {
        case TraceActive:
          SiSetup.PlatformDebugOption = EnabledTraceActive;
          break;

        case TraceReady:
          SiSetup.PlatformDebugOption = EnabledTraceReady;
          break;

        case TracePowerOff:
          SiSetup.PlatformDebugOption = EnabledTracePowerOff;
          break;

        case ForceDisabled:
          SiSetup.PlatformDebugOption = DisableDebug;
          break;
      }
      Status = gRT->SetVariable (L"SiSetup", &gSiSetupVariableGuid, SiVarAttr, SiVarSize, &SiSetup);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Set SiSetup Fail. Status = %r\n", Status));
      }
    }
  } else {
    if (BiosPayload.TokenExist == 1) {
      //
      // Token erased, expired or injection failure, rollback Debug Setup to backup values
      //
      DEBUG ((DEBUG_INFO, "Token is erased, expired or injection failed, rollback Debug Setup to backup values\n"));
      if ((BiosPayload.DebugSetupBackup.PlatformDebugOption != Manual) && \
          (BiosPayload.DebugSetupBackup.PlatformDebugOption != EnabledTraceActive) && \
          (BiosPayload.DebugSetupBackup.PlatformDebugOption != EnabledTraceReady) && \
          (BiosPayload.DebugSetupBackup.PlatformDebugOption != EnabledTracePowerOff) && \
          (BiosPayload.DebugSetupBackup.PlatformDebugOption != DisableDebug)) {
         BiosPayload.DebugSetupBackup.PlatformDebugOption = 0;
         DEBUG ((DEBUG_ERROR, "Invalid BiosPayload.DebugSetupBackup.PlatformDebugOption = %d\n", BiosPayload.DebugSetupBackup.PlatformDebugOption));
      } else {
        SiSetup.PlatformDebugOption = BiosPayload.DebugSetupBackup.PlatformDebugOption;
        DEBUG ((DEBUG_INFO, "Rollback PlatformDebugOption to %x\n", SiSetup.PlatformDebugOption));
        Status = gRT->SetVariable (L"SiSetup", &gSiSetupVariableGuid, SiVarAttr, SiVarSize, &SiSetup);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Set SiSetup Fail. Status = %r\n", Status));
        }
      }
      BiosPayload.TokenExist = 0;
      gRT->SetVariable (BIOS_PAYLOAD_VARIABLE_NAME, &gBiosPayloadVarGuid, BiosPayloadVarAttr, BiosPayloadVarSize, (VOID *) &BiosPayload);
    }
    InitString(
      HiiHandle, STRING_TOKEN(STR_DEBUG_TOKEN_PRESENCE_VALUE),
      L"No"
    );
  }

  //
  // Do NOT update options for PDC Manual
  //
  if (SiSetup.PlatformDebugOption == Manual) {
    return;
  }

  if (Class == ADVANCED_FORM_SET_CLASS) {
    DEBUG ((DEBUG_INFO, "Init Debug defaults ()\n"));

    switch (SiSetup.PlatformDebugOption) {
      case DisableDebug:
        mPchSetup.DciEn                    = FALSE;
        mPchSetup.DciClkEnable             = TRUE;
        mPchSetup.DciDbcMode               = DciDbcUsb2;
        mPchSetup.KeepEarlyTrace           = KetDisabled;
        mPchSetup.SocTraceHubMode          = TraceHubModeDisabled;
        mPchSetup.SocTraceHubMemReg0Size   = TraceBufferNone;
        mPchSetup.SocTraceHubMemReg1Size   = TraceBufferNone;
        mPchSetup.IoeDebugEn               = FALSE;
        mPchSetup.PmodeClkEn               = FALSE;
        mCpuSetup.DfdEnable                = FALSE;
        break;

      case EnabledTracePowerOff:
        mPchSetup.DciEn                    = TRUE;
        mPchSetup.DciClkEnable             = FALSE;
        mPchSetup.DciDbcMode               = DciDbcUsb2;
        mPchSetup.KeepEarlyTrace           = KetDisabled;
        mPchSetup.SocTraceHubMode          = TraceHubModeDisabled;
        mPchSetup.SocTraceHubMemReg0Size   = TraceBufferNone;
        mPchSetup.SocTraceHubMemReg1Size   = TraceBufferNone;
        mPchSetup.IoeDebugEn               = FALSE;
        mPchSetup.PmodeClkEn               = FALSE;
        mCpuSetup.DfdEnable                = TRUE;
        break;

      case EnabledTraceReady:
        mPchSetup.DciEn                    = TRUE;
        mPchSetup.DciClkEnable             = TRUE;
        mPchSetup.DciDbcMode               = DciDbcUsb2;
        mPchSetup.KeepEarlyTrace           = KetDisabled;
        mPchSetup.SocTraceHubMode          = TraceHubModeEnabled;
        mPchSetup.SocTraceHubMemReg0Size   = TraceBuffer8M;
        mPchSetup.SocTraceHubMemReg1Size   = TraceBuffer8M;
        mPchSetup.IoeDebugEn               = TRUE;
        mPchSetup.PmodeClkEn               = TRUE;
        mCpuSetup.DfdEnable                = TRUE;
        break;

      case EnabledTraceActive:
        mPchSetup.DciEn                    = TRUE;
        mPchSetup.DciClkEnable             = TRUE;
        mPchSetup.DciDbcMode               = DciDbcBoth;
        mPchSetup.KeepEarlyTrace           = KetEnabled;
        mPchSetup.SocTraceHubMode          = TraceHubModeEnabled;
        mPchSetup.SocTraceHubMemReg0Size   = TraceBuffer8M;
        mPchSetup.SocTraceHubMemReg1Size   = TraceBuffer8M;
        mPchSetup.IoeDebugEn               = TRUE;
        mPchSetup.PmodeClkEn               = TRUE;
        mCpuSetup.DfdEnable                = TRUE;
        break;

      default:
        break;
    }

    if (DebugTokenExisted && (DebugToken.Fields.ProcessorTraceEnable == TRUE)) {
      mCpuSetup.ProcessorTraceMemSize = 0;
      mCpuSetup.ProcessorTraceEnable = 1;
    }

    Status = gRT->SetVariable (L"PchSetup", &gPchSetupVariableGuid, PchVarAttr, PchVarSize, &mPchSetup);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set PchSetup Fail. Status = %r\n", Status));
    }

    Status = gRT->SetVariable (L"CpuSetup", &gCpuSetupVariableGuid, CpuVarAttr, CpuVarSize, &mCpuSetup);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set CpuSetup Fail. Status = %r\n", Status));
    }
  }
}

/**
  Setup callback executed when user selects a Debug Enable from the BIOS UI.
  Changes visibility and settings of other options.

  @param[in] This               Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action             Specifies the type of action taken by the browser.
  @param[in] KeyValue           A unique value which is sent to the original exporting driver
                                so that it can identify the type of data to expect.
  @param[in] Type               The type of value for the question.
  @param[in] Value              A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest     On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS           The callback successfully handled the action
  @retval EFI_UNSUPPORTED       The specified action is not supported by the callback
  @retval EFI_NOT_FOUND         Failed to retrive protocol/data required for execution
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate resources.
**/
EFI_STATUS
EFIAPI
DebugFormCallBackFunction(
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  SI_SETUP                SiSetup;
  UINTN                   SiVarSize;
  EFI_STRING              SiRequestString;
  EFI_STRING              TmpString;
  PCH_SETUP               PchSetup;
  UINTN                   PchVarSize;
  EFI_STRING              PchRequestString;
  CPU_SETUP               CpuSetup;
  UINTN                   CpuVarSize;
  EFI_STRING              CpuRequestString;
  BOOLEAN                 StatusBool;

  PchRequestString       = NULL;
  SiRequestString        = NULL;
  TmpString              = NULL;
  CpuRequestString       = NULL;

  switch (Action) {
    case EFI_BROWSER_ACTION_CHANGED:
    case EFI_BROWSER_ACTION_CHANGING:
      break;

    default:
      return EFI_UNSUPPORTED;
      break;
  }
  DEBUG ((DEBUG_INFO, "DebugFormCallBackFunction()\n"));

  SiVarSize = sizeof (SI_SETUP);
  if (!HiiGetBrowserData (&gSiSetupVariableGuid, L"SiSetup", SiVarSize, (UINT8 *) &SiSetup)) {
    ASSERT (FALSE);
    return EFI_SUCCESS;
  }
  //
  // Do NOT update options for PDC Manual
  //
  if (SiSetup.PlatformDebugOption == Manual) {
    return EFI_SUCCESS;
  }

  PchVarSize = sizeof (PCH_SETUP);
  if (!HiiGetBrowserData (&gPchSetupVariableGuid, L"PchSetup", PchVarSize, (UINT8 *) &PchSetup)) {
    ASSERT (FALSE);
    return EFI_SUCCESS;
  }

  CpuVarSize = sizeof (CPU_SETUP);
  if (!HiiGetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", CpuVarSize, (UINT8 *) &CpuSetup)) {
    ASSERT (FALSE);
    return EFI_SUCCESS;
  }

  switch (KeyValue) {
    case PLATFORM_DEBUG_OPTION_KEY:
      switch (SiSetup.PlatformDebugOption) {
        case DisableDebug:
          PchSetup.DciEn                     = FALSE;
          PchSetup.DciClkEnable              = TRUE;
          PchSetup.DciDbcMode                = DciDbcUsb2;
          PchSetup.KeepEarlyTrace            = KetDisabled;
          PchSetup.SocTraceHubMode           = TraceHubModeDisabled;
          PchSetup.SocTraceHubMemReg0Size    = TraceBufferNone;
          PchSetup.SocTraceHubMemReg1Size    = TraceBufferNone;
          PchSetup.IoeDebugEn                = FALSE;
          PchSetup.PmodeClkEn                = FALSE;
          CpuSetup.DfdEnable                 = FALSE;
          break;

        case EnabledTracePowerOff:
          PchSetup.DciEn                     = TRUE;
          PchSetup.DciClkEnable              = FALSE;
          PchSetup.DciDbcMode                = DciDbcUsb2;
          PchSetup.KeepEarlyTrace            = KetDisabled;
          PchSetup.SocTraceHubMode           = TraceHubModeDisabled;
          PchSetup.SocTraceHubMemReg0Size    = TraceBufferNone;
          PchSetup.SocTraceHubMemReg1Size    = TraceBufferNone;
          PchSetup.IoeDebugEn                = FALSE;
          PchSetup.PmodeClkEn                = FALSE;
          CpuSetup.DfdEnable                 = TRUE;
          break;

        case EnabledTraceReady:
          PchSetup.DciEn                     = TRUE;
          PchSetup.DciClkEnable              = TRUE;
          PchSetup.DciDbcMode                = DciDbcUsb2;
          PchSetup.KeepEarlyTrace            = KetDisabled;
          PchSetup.SocTraceHubMode           = TraceHubModeEnabled;
          PchSetup.SocTraceHubMemReg0Size    = TraceBuffer8M;
          PchSetup.SocTraceHubMemReg1Size    = TraceBuffer8M;
          PchSetup.IoeDebugEn                = TRUE;
          PchSetup.PmodeClkEn                = TRUE;
          CpuSetup.DfdEnable                 = TRUE;
          break;

        case EnabledTraceActive:
          PchSetup.DciEn                     = TRUE;
          PchSetup.DciClkEnable              = TRUE;
          PchSetup.DciDbcMode                = DciDbcBoth;
          PchSetup.KeepEarlyTrace            = KetEnabled;
          PchSetup.SocTraceHubMode           = TraceHubModeEnabled;
          PchSetup.SocTraceHubMemReg0Size    = TraceBuffer8M;
          PchSetup.SocTraceHubMemReg1Size    = TraceBuffer8M;
          PchSetup.IoeDebugEn                = TRUE;
          PchSetup.PmodeClkEn                = TRUE;
          CpuSetup.DfdEnable                 = TRUE;
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  SiRequestString = HiiConstructRequestString (SiRequestString, OFFSET_OF (SI_SETUP, PlatformDebugOption), sizeof (SiSetup.PlatformDebugOption));
  if (SiRequestString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  StatusBool = HiiSetBrowserData (&gSiSetupVariableGuid, L"SiSetup", SiVarSize, (UINT8 *) &SiSetup, SiRequestString);
  FreePool (SiRequestString);
  if (!StatusBool) {
    return EFI_NOT_FOUND;
  }

  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, DciEn), sizeof (PchSetup.DciEn));
  if (PchRequestString == NULL) {
    goto ExitNoSpace;
  }
  TmpString = PchRequestString;
  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, DciClkEnable), sizeof (PchSetup.DciClkEnable));
  if (PchRequestString == NULL) {
    goto ExitNoSpace;
  }
  TmpString = PchRequestString;
  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, DciDbcMode), sizeof (PchSetup.DciDbcMode));
  if (PchRequestString == NULL) {
    goto ExitNoSpace;
  }
  TmpString = PchRequestString;
  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, KeepEarlyTrace), sizeof (PchSetup.KeepEarlyTrace));
  if (PchRequestString == NULL) {
    goto ExitNoSpace;
  }
  TmpString = PchRequestString;
  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, IoeDebugEn), sizeof (PchSetup.IoeDebugEn));
  if (PchRequestString == NULL) {
    goto ExitNoSpace;
  }
  TmpString = PchRequestString;
  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, PmodeClkEn), sizeof (PchSetup.PmodeClkEn));
  if (PchRequestString == NULL) {
    goto ExitNoSpace;
  }
  TmpString = PchRequestString;
  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, SocTraceHubMode), sizeof (PchSetup.SocTraceHubMode));
  if (PchRequestString == NULL) {
    goto ExitNoSpace;
  }
  TmpString = PchRequestString;
  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, SocTraceHubMemReg0Size), sizeof (PchSetup.SocTraceHubMemReg0Size));
  if (PchRequestString == NULL) {
    goto ExitNoSpace;
  }
  TmpString = PchRequestString;
  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, SocTraceHubMemReg1Size), sizeof (PchSetup.SocTraceHubMemReg1Size));
  if (PchRequestString == NULL) {
    goto ExitNoSpace;
  }
  StatusBool = HiiSetBrowserData (&gPchSetupVariableGuid, L"PchSetup", PchVarSize, (UINT8 *) &PchSetup, PchRequestString);
  FreePool (PchRequestString);
  if (!StatusBool) {
    return EFI_NOT_FOUND;
  }

  CpuRequestString = HiiConstructRequestString (CpuRequestString, OFFSET_OF (CPU_SETUP, DfdEnable), sizeof (CpuSetup.DfdEnable));
  if (CpuRequestString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  StatusBool = HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", CpuVarSize, (UINT8 *) &CpuSetup, CpuRequestString);
  FreePool (CpuRequestString);
  if (!StatusBool) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;

  ExitNoSpace:
    if (TmpString != NULL) {
      FreePool (TmpString);
    }
    return EFI_OUT_OF_RESOURCES;
}

/**
  Setup callback executed when user change TraceHub memory buffer size from the BIOS UI.
  If total buffer size is larger than half of system memory, rollback buffer size to default.

  @param[in] This               Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action             Specifies the type of action taken by the browser.
  @param[in] KeyValue           A unique value which is sent to the original exporting driver
                                so that it can identify the type of data to expect.
  @param[in] Type               The type of value for the question.
  @param[in] Value              A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest     On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS           The callback successfully handled the action
  @retval EFI_UNSUPPORTED       The specified action is not supported by the callback
  @retval EFI_NOT_FOUND         Failed to retrive protocol/data required for execution
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate resources.
**/
EFI_STATUS
EFIAPI
TraceHubMemRegSizeCheckCallBack(
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  SI_SETUP                SiSetup;
  UINTN                   SiVarSize;
  EFI_STRING              SiRequestString;
  PCH_SETUP               PchSetup;
  UINTN                   PchVarSize;
  EFI_STRING              PchRequestString;
  EFI_STRING              TmpString;
  EFI_HOB_GUID_TYPE       *GuidHob;
  MEMORY_INFO_DATA_HOB    *MemInfo;
  UINT32                  TotalTraceBufferSize;
  UINT64                  TotalPhysicalMemorySize;
  EFI_STATUS              StatusBool;

  PchRequestString        = NULL;
  SiRequestString         = NULL;
  TmpString               = NULL;
  GuidHob                 = NULL;
  MemInfo                 = NULL;
  TotalTraceBufferSize    = 0;
  TotalPhysicalMemorySize = 0;
  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  GuidHob = GetFirstGuidHob (&gSiMemoryInfoDataGuid);
  if (GuidHob != NULL) {
    MemInfo = (MEMORY_INFO_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
    TotalPhysicalMemorySize = MemInfo->TotalPhysicalMemorySize;
  } else {
    return EFI_NOT_FOUND;
  }

  switch (Action) {
    case EFI_BROWSER_ACTION_CHANGED:
    case EFI_BROWSER_ACTION_CHANGING:
      break;

    default:
      return EFI_UNSUPPORTED;
      break;
  }
  DEBUG ((DEBUG_INFO, "TraceHubMemRegSizeCheckCallBack()\n"));

  SiVarSize = sizeof (SI_SETUP);
  if (!HiiGetBrowserData (&gSiSetupVariableGuid, L"SiSetup", SiVarSize, (UINT8 *) &SiSetup)) {
    ASSERT (FALSE);
    return EFI_SUCCESS;
  }

  PchVarSize = sizeof (PCH_SETUP);
  if (!HiiGetBrowserData (&gPchSetupVariableGuid, L"PchSetup", PchVarSize, (UINT8 *) &PchSetup)) {
    ASSERT (FALSE);
    return EFI_SUCCESS;
  }

  if (KeyValue == TRACEHUB_BUFFER_SIZE_KEY || KeyValue == TRACEHUB_BUFFER_SIZE_KEY1 || KeyValue == TRACEHUB_BUFFER_SIZE_KEY2 || KeyValue == TRACEHUB_BUFFER_SIZE_KEY3) {
    if (SiSetup.PlatformDebugOption == Manual) {
      if (PchSetup.SocTraceHubMode != TraceHubModeDisabled) {
        if (PchSetup.SocTraceHubMemReg0Size != TraceBufferNone) {
          TotalTraceBufferSize += (1 << PchSetup.SocTraceHubMemReg0Size);
        }
        if (PchSetup.SocTraceHubMemReg1Size != TraceBufferNone) {
          TotalTraceBufferSize += (1 << PchSetup.SocTraceHubMemReg1Size);
        }
      }
      //
      // Make TraceHub total size to be power of 2
      //
      if (TotalTraceBufferSize > GetPowerOfTwo32 (TotalTraceBufferSize)) {
        DEBUG ((DEBUG_INFO, "Total TraceHub memory = 0x%lx (MB) is not power of 2\n", TotalTraceBufferSize));
        TotalTraceBufferSize = 2 * GetPowerOfTwo32 (TotalTraceBufferSize);
        DEBUG ((DEBUG_INFO, "Required total TraceHub memory = 0x%lx (MB)\n", TotalTraceBufferSize));
      }

      if (TotalTraceBufferSize > (TotalPhysicalMemorySize / 2)) {
        DEBUG ((DEBUG_INFO, "Total TraceHub buffer size(0x%x) is larger than half of system memory size(0x%x), rollback buffer size to default value\n", TotalTraceBufferSize, TotalPhysicalMemorySize));
        if (PchSetup.SocTraceHubMemReg0Size == TraceBufferNone) {
        } else if (PchSetup.SocTraceHubMemReg0Size == TraceBuffer1M) {
        } else {
          PchSetup.SocTraceHubMemReg0Size = TraceBuffer8M;
        }
        if (PchSetup.SocTraceHubMemReg1Size == TraceBufferNone) {
        } else if (PchSetup.SocTraceHubMemReg1Size == TraceBuffer1M) {
        } else {
          PchSetup.SocTraceHubMemReg1Size = TraceBuffer8M;
        }
      }
    }
  }

  SiRequestString = HiiConstructRequestString (SiRequestString, OFFSET_OF (SI_SETUP, PlatformDebugOption), sizeof (SiSetup.PlatformDebugOption));
  if (SiRequestString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  StatusBool = HiiSetBrowserData (&gSiSetupVariableGuid, L"SiSetup", SiVarSize, (UINT8 *) &SiSetup, SiRequestString);
  FreePool (SiRequestString);
  if (!StatusBool) {
    return EFI_NOT_FOUND;
  }

  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, SocTraceHubMemReg0Size), sizeof (PchSetup.SocTraceHubMemReg0Size));
  if (PchRequestString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  TmpString = PchRequestString;
  PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, SocTraceHubMemReg1Size), sizeof (PchSetup.SocTraceHubMemReg1Size));
  if (PchRequestString == NULL) {
    FreePool (TmpString);
    return EFI_OUT_OF_RESOURCES;
  }
  StatusBool = HiiSetBrowserData (&gPchSetupVariableGuid, L"PchSetup", PchVarSize, (UINT8 *) &PchSetup, PchRequestString);
  FreePool (PchRequestString);
  if (!StatusBool) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  Setup callback executed when user change processor trace memory buffer size from the BIOS UI.
  If total buffer size is larger than half of system memory, rollback buffer size to last value.

  @param[in] This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action          Specifies the type of action taken by the browser.
  @param[in] KeyValue        A unique value which is sent to the original exporting driver
                             so that it can identify the type of data to expect.
  @param[in] Type            The type of value for the question.
  @param[in] Value           A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest  On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS        The callback successfully handled the action
  @retval EFI_UNSUPPORTED    The specified action is not supported by the callback
**/
EFI_STATUS
EFIAPI
ProcessorTraceMemSizeCheckCallBack(
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  EFI_STATUS                Status;
  UINT64                    TotalBufferSize;
  UINT64                    TotalMemorySize;
  EFI_PEI_HOB_POINTERS      Hob;
  EFI_INPUT_KEY             Key;
  UINTN                     NumberOfProcessors;
  UINTN                     NumberOfEnabledProcessors;
  EFI_MP_SERVICES_PROTOCOL  *MpService;

  TotalBufferSize           = 0;
  TotalMemorySize           = 0;
  NumberOfProcessors        = 0;
  NumberOfEnabledProcessors = 0;

  if ((Action == EFI_BROWSER_ACTION_CHANGING) && (KeyValue == PROCESSOR_TRACE_MEM_SIZE_KEY))
  {
    //
    // Search for the Memory Descriptior HOB,
    // get the max range to reverse memory for IPT.
    //
    TotalMemorySize = 0;
    Hob.Raw = GetFirstHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR);
    while (Hob.Raw != NULL) {
      if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY)) {
        if ((Hob.ResourceDescriptor->PhysicalStart < SIZE_4GB)
           && (TotalMemorySize < Hob.ResourceDescriptor->ResourceLength)) {
          TotalMemorySize = Hob.ResourceDescriptor->ResourceLength;
        }
      }
      Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, GET_NEXT_HOB (Hob));
    }

    Status = gBS->LocateProtocol (
                    &gEfiMpServiceProtocolGuid,
                    NULL,
                    (VOID **) &MpService
                    );

    DEBUG ((DEBUG_INFO, "ProcessorTraceMemSizeCheckCallBack TotalMemorySize: 0x%016lx\n", TotalMemorySize));
    if (TotalMemorySize == 0) {
      return EFI_NOT_FOUND;
    }

    if (Value->u8 < 0xFF) {
      Status = MpService->GetNumberOfProcessors (MpService, &NumberOfProcessors, &NumberOfEnabledProcessors);
      ASSERT_EFI_ERROR (Status);

      TotalBufferSize = LShiftU64(1, (Value->u8 + 12)) * NumberOfEnabledProcessors;
    }

    if (TotalBufferSize >= (TotalMemorySize / 2)) {
      DEBUG ((DEBUG_INFO, "Total IPT buffer size(0x%016lx) is larger than half of system memory size(0x%016lx)\n", TotalBufferSize, TotalMemorySize));
      //
      // Popup a menu to notice user
      //
      do {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"System memory is not enough to support this memory size. Revert this operation",
          L" ",
          L"Press any key to continue",
          NULL
          );
      } while ((Key.UnicodeChar != CHAR_CARRIAGE_RETURN) && (Key.ScanCode != SCAN_ESC));

      return EFI_ABORTED;
    }
  }

  return EFI_SUCCESS;
}
