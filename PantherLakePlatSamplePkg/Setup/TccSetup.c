/** @file
  Time Coordinated Computing (TCC) Setup Routines

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include "SetupPrivate.h"
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/HiiString.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Include/Library/CpuPcieInfoLib.h>
#include <Library/HobLib.h>
#include <TccDataHob.h>
#include <Library/TccPolicyLib.h>
#include <Protocol/TccSetupProtocol.h>
#include <Library/Ptl/PcdInfoLib/PtlPcdInfoLib.h>
#include <TccSetup.h>

extern UINT8 AdvancedBin[];

/**
  This function looks for default value of question

  NOTE:  This function cannot get the default value when it is not
         hardcode value.
         Ex. default value = cond(get(SETUP_DATA.Rtd3Support) == 1 ? 2 : 1),

  @param[in]  QuestionStringId        Question String ID
  @param[out] DefaultValue            Default Value of Question

  @retval EFI_SUCCESS                 Successful found the default value
**/
EFI_STATUS
GetDefaultOneOfQuestionValue (
  IN EFI_STRING_ID       QuestionStringId,
  OUT UINT8              *DefaultValue
  )
{
#if FixedPcdGet8(PcdTccSupport) == 0x1
  EFI_HII_PACKAGE_HEADER *PackageHdr;
  CHAR8                  *BytePtr;
  EFI_IFR_OP_HEADER      *ParentIfr;
  EFI_IFR_DEFAULT        *DefaultPtr;
  EFI_IFR_ONE_OF_OPTION  *OptionPtr;
  UINTN                  PackageSize;
  EFI_IFR_ONE_OF         *OneOfPtr;

  //
  // add Array Length (UINT32) to point to package header.
  //
  PackageHdr = (EFI_HII_PACKAGE_HEADER *)(AdvancedBin + sizeof(UINT32));
  PackageSize = PackageHdr->Length;
  PackageSize = PackageSize & 0x00ffffff;
  BytePtr = (CHAR8 *) (PackageHdr + 1);

  //
  // loop to find one of op code with Question string id
  //
  while (TRUE) {
    if (BytePtr >= (CHAR8 *)PackageHdr + PackageSize) {
      DEBUG ((DEBUG_ERROR, "Couldn't find the target node to patch.\n"));
      return EFI_NOT_FOUND;
    }

    ParentIfr = (EFI_IFR_OP_HEADER *)BytePtr;
    if (ParentIfr->OpCode == EFI_IFR_ONE_OF_OP) {
      OneOfPtr = (EFI_IFR_ONE_OF *)ParentIfr;
      if(OneOfPtr->Question.Header.Prompt == QuestionStringId) {
        break;
      }
    }
    BytePtr += ParentIfr->Length;
  }

  //
  // advance to default OP code or default option.
  //
  while (TRUE) {
    if (BytePtr >= (CHAR8 *)PackageHdr + PackageSize) {
      DEBUG ((DEBUG_ERROR, "Couldn't find the target node to patch.\n"));
      return EFI_NOT_FOUND;
    }

    ParentIfr = (EFI_IFR_OP_HEADER *)BytePtr;
    if (ParentIfr->OpCode == EFI_IFR_DEFAULT_OP) {
      DefaultPtr = (EFI_IFR_DEFAULT *)ParentIfr;
      *DefaultValue = DefaultPtr->Value.u8;
      break;
    }

    if (ParentIfr->OpCode == EFI_IFR_ONE_OF_OPTION_OP) {
      OptionPtr = (EFI_IFR_ONE_OF_OPTION *) ParentIfr;
      if ((OptionPtr->Flags & EFI_IFR_OPTION_DEFAULT)  == EFI_IFR_OPTION_DEFAULT) {
        *DefaultValue = OptionPtr->Value.u8;
        break;
      }
    }

    BytePtr += ParentIfr->Length;
  }

  return EFI_SUCCESS;
#else
  return EFI_UNSUPPORTED;
#endif


}

/**
  This function sets  CPU, PCH, and SA configuration to default values
  base off TCC mode and DSO Tuning.

  @param[in]     TccModeDefault         TRUE if you want TCC Mode Defaults
  @param[in,out] CpuSetup               Cpu Setup Configuration
  @param[in,out] PchSetup               Pch Setup Configuration
  @param[in,out] SaSetup                SA Setup Configuration
  @param[in,out] SetupData              Setup Data Configuration
**/
VOID
TccRestoreDefaultConfig (
  IN     BOOLEAN               TccModeDefault,
  IN OUT CPU_SETUP             *CpuSetup,
  IN OUT PCH_SETUP             *PchSetup,
  IN OUT SA_SETUP              *SaSetup,
  IN OUT SETUP_DATA            *SetupData
  )
{
#if FixedPcdGet8(PcdTccSupport) == 0x1

  // Common Cpu Setup
  GetDefaultOneOfQuestionValue (STRING_TOKEN(STR_CPU_IST_PROMPT), &CpuSetup->EnableGv);
  GetDefaultOneOfQuestionValue (STRING_TOKEN(STR_CPU_ISST_PROMPT), &CpuSetup->EnableHwp);
  GetDefaultOneOfQuestionValue (STRING_TOKEN(STR_CX_PROMPT), &CpuSetup->EnableCx);

  // Common Pch setup
  GetDefaultOneOfQuestionValue (STRING_TOKEN(STR_PCH_DMI_ASPM_PROMPT), &PchSetup->DmiAspm);
  GetDefaultOneOfQuestionValue (STRING_TOKEN(STR_PCH_LEGACY_IO_LOW_LATENCY), &PchSetup->PchLegacyIoLowLatency);

  // Common SaSetup
  GetDefaultOneOfQuestionValue (STRING_TOKEN (STR_SAGV_PROMPT), &SaSetup->SaGv);
  GetDefaultOneOfQuestionValue (STRING_TOKEN(STR_PAGE_IDLE_TIMEOUT_PROMPT), &SaSetup->DisPgCloseIdleTimeout);

  GetDefaultOneOfQuestionValue (STRING_TOKEN(STR_VIDEO_RS2_PROMPT), &SaSetup->EnableRenderStandby);

  GetDefaultOneOfQuestionValue (STRING_TOKEN(STR_POWER_DOWN_MODE_PROMPT), &SaSetup->PowerDownMode);

  GetDefaultOneOfQuestionValue (STRING_TOKEN(STR_LOW_POWER_S0_IDLE), &SetupData->LowPowerS0Idle);

#endif
}

/**
  This function sets CPU, PCH, and SA configuration
  base off TCC mode.

  @param[in,out] CpuSetup               Cpu Setup Configuration
  @param[in,out] PchSetup               Pch Setup Configuration
  @param[in,out] SaSetup                SA Setup Configuration
  @param[in,out] SetupData              Setup Data Configuration
**/
VOID
GetTccModeConfig (
  IN OUT CPU_SETUP             *CpuSetup,
  IN OUT PCH_SETUP             *PchSetup,
  IN OUT SA_SETUP              *SaSetup,
  IN OUT SETUP_DATA            *SetupData
  )
{
#if FixedPcdGet8(PcdTccSupport) == 0x1

  if (SetupData->TccMode == 1) {
    GetTccModeConfigPolicy (CpuSetup, PchSetup, SaSetup, SetupData);
  } else {
    TccRestoreDefaultConfig (TRUE, CpuSetup, PchSetup, SaSetup, SetupData);
  }

#endif
}

/**
  Update Hii Browser with tcc data

  @param[in] CpuSetup               Cpu Setup Configuration
  @param[in] PchSetup               Pch Setup Configuration
  @param[in] SaSetup                SA Setup Configuration
  @param[in] SetupData              Setup Data Configuration

  @retval  EFI_SUCCESS              Successful update browser data
  @retval  EFI_INVALID_PARAMETER    Parameter is null pointer
  @retval  EFI_NOT_FOUND            Failed to update browser data
**/
EFI_STATUS
TccUpdateHiiBrowser (
  IN CPU_SETUP             *CpuSetup,
  IN PCH_SETUP             *PchSetup,
  IN SA_SETUP              *SaSetup,
  IN SETUP_DATA            *SetupData
  )
{
#if FixedPcdGet8(PcdTccSupport) == 0x1
  BOOLEAN                 IsDataUpdated;
  EFI_STRING              RequestString;

  if ((CpuSetup == NULL) || (PchSetup == NULL) || (SaSetup == NULL) || (SetupData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  RequestString = NULL;

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, EnableGv), sizeof (CpuSetup->EnableGv));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, EnableHwp), sizeof (CpuSetup->EnableHwp));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, EnableCx), sizeof (CpuSetup->EnableCx));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, TurboMode), sizeof (CpuSetup->TurboMode));

  IsDataUpdated = HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", sizeof (CPU_SETUP), (UINT8 *) CpuSetup, RequestString);
  FreePool (RequestString);
  RequestString = NULL;

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (PCH_SETUP, DmiAspm), sizeof (PchSetup->DmiAspm));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (PCH_SETUP, PchLegacyIoLowLatency), sizeof (PchSetup->PchLegacyIoLowLatency));

  IsDataUpdated &= HiiSetBrowserData (&gPchSetupVariableGuid, L"PchSetup", sizeof (PCH_SETUP), (UINT8 *) PchSetup, RequestString);
  FreePool (RequestString);
  RequestString = NULL;

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, SaGv), sizeof (SaSetup->SaGv));

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, DisPgCloseIdleTimeout), sizeof (SaSetup->DisPgCloseIdleTimeout));

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, PowerDownMode), sizeof (SaSetup->PowerDownMode));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, EnableRenderStandby), sizeof (SaSetup->EnableRenderStandby));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, PowerDownMode), sizeof (SaSetup->PowerDownMode));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, EnableRenderStandby), sizeof (SaSetup->EnableRenderStandby));

  IsDataUpdated &= HiiSetBrowserData (&gSaSetupVariableGuid, L"SaSetup", sizeof (SA_SETUP), (UINT8 *) SaSetup, RequestString);
  FreePool (RequestString);
  RequestString = NULL;

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, Rtd3Support), sizeof (SetupData->Rtd3Support));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, StorageRtd3Support), sizeof (SetupData->StorageRtd3Support));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, LowPowerS0Idle), sizeof (SetupData->LowPowerS0Idle));

  IsDataUpdated &= HiiSetBrowserData (&gSetupVariableGuid, L"Setup", sizeof (SETUP_DATA), (UINT8 *) SetupData, RequestString);
  FreePool (RequestString);
  RequestString = NULL;

  if (!IsDataUpdated) {
    return EFI_NOT_FOUND;
  }

#endif

  return EFI_SUCCESS;
}

/**
  This function toggles TCC mode on the platform by
  modifying CPU, SA, and PCH settings related with TCC.

  @param[in] This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action          Specifies the type of action taken by the browser.
  @param[in] QuestionId      A unique value which is sent to the original exporting driver
                             so that it can identify the type of data to expect.
  @param[in] Type            The type of value for the question.
  @param[in] Value           A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest  On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS        Call Back Function executed successfully
  @retval EFI_UNSUPPORTED    Browser Action isnt supported.
  @retval EFI_NOT_FOUND      Failed to get or update browser data
**/
EFI_STATUS
EFIAPI
TccModeCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
#if FixedPcdGet8(PcdTccSupport) == 0x1
  CPU_SETUP               *CpuSetup;
  PCH_SETUP               *PchSetup;
  SA_SETUP                *SaSetup;
  SETUP_DATA              *SetupData;
  UINTN                   CpuSetupSize;
  UINTN                   PchSetupSize;
  UINTN                   SaSetupSize;
  UINTN                   SetupDataSize;
  EFI_STATUS              Status;
  BOOLEAN                 IsDataRetrived;

  Status        = EFI_SUCCESS;
  CpuSetup      = NULL;
  PchSetup      = NULL;
  SaSetup       = NULL;
  SetupData     = NULL;

  ///
  /// No action to perform on Action Changing
  ///
  if (Action == EFI_BROWSER_ACTION_CHANGING) {
    return EFI_SUCCESS;
  }

  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  CpuSetupSize  = sizeof (CPU_SETUP);
  PchSetupSize  = sizeof (PCH_SETUP);
  SaSetupSize   = sizeof (SA_SETUP);
  SetupDataSize = sizeof (SETUP_DATA);
  CpuSetup = AllocatePool (CpuSetupSize);
  if (CpuSetup == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
  }
  PchSetup = AllocatePool (PchSetupSize);
  if (PchSetup == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
  }
  SaSetup  = AllocatePool (SaSetupSize);
  if (SaSetup == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
  }
  SetupData = AllocatePool (SetupDataSize);
  if (SetupData == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
  }

  if (!EFI_ERROR (Status)) {
    ///
    /// GetBrowserData by VarStore Name (Setup)
    ///
    IsDataRetrived =  HiiGetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup",     CpuSetupSize,     (UINT8 *) CpuSetup);
    IsDataRetrived &= HiiGetBrowserData (&gPchSetupVariableGuid, L"PchSetup",     PchSetupSize,     (UINT8 *) PchSetup);
    IsDataRetrived &= HiiGetBrowserData (&gSaSetupVariableGuid,  L"SaSetup",      SaSetupSize,      (UINT8 *) SaSetup);
    IsDataRetrived &= HiiGetBrowserData (&gSetupVariableGuid,    L"Setup",        SetupDataSize,    (UINT8 *) SetupData);

    if (!IsDataRetrived) {
      return EFI_NOT_FOUND;
    }

    switch (KeyValue) {
      case KEY_TCC_MODE:
        // Get TCC Mode Setting Configuration
        GetTccModeConfig (CpuSetup, PchSetup, SaSetup, SetupData);
        Status = TccUpdateHiiBrowser (CpuSetup, PchSetup, SaSetup, SetupData);
        break;
      default:
        Status = EFI_UNSUPPORTED;
    }
  }

  if (CpuSetup != NULL) {
    FreePool (CpuSetup);
  }
  if (PchSetup != NULL) {
    FreePool (PchSetup);
  }
  if (SaSetup != NULL) {
    FreePool (SaSetup);
  }
  if (SetupData != NULL) {
    FreePool (SetupData);
  }

  return Status;
#else
  return EFI_SUCCESS;
#endif
}
