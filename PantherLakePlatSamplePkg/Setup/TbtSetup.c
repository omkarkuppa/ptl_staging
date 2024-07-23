/** @file
  Thunderbolt(TM) Setup Routines.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include "Base.h"
#include <Library/PciSegmentLib.h>
#include <SetupPrivate.h>
#include <TcssInfo.h>
#include <PcieRegs.h>
#include "PlatformBoardId.h"
#include <Library/TbtCommonLib.h>
#include <Guid/HobList.h>
#include <Library/TimerLib.h>
#include <Library/PcdLib.h>
#include <Library/CmosAccessLib.h>
#include <CmosMap.h>
#include <Library/Usb4CmMode.h>
#include <DTbtInfoHob.h>
#include "DiscreteTbtResourcesInitValues.h"

#define LTSSM_FSM_RESTORE 0xC74
#define TBT_5S_TIMEOUT                    50

GLOBAL_REMOVE_IF_UNREFERENCED DTBT_INFO_HOB   *gDTbtInfoHob          = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN         Usb4CmModeChangeEnable = FALSE;

/**
  Update the Rtd3 setting base on the board type.

  @param[IN]      UINT8                   Rtd3Support,
  @param[IN][OUT] SETUP_DATA              *SetupData

  @retval EFI_SUCCESS             Successfully completed updates.
  @retval Others                  Fail to update.
**/
EFI_STATUS
EFIAPI
UpdateRtd3Setting (
  IN      UINT8               Rtd3Support,
  IN OUT  SETUP_DATA          *SetupData
  );

EFI_STATUS
EFIAPI
DTbtFormCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  SA_SETUP                    *SaSetup;
  PCH_SETUP                   *PchSetup;
  SETUP_DATA                  *SetupData;
  UINTN                       VarSize;
  EFI_STATUS                  Status;
  EFI_STRING                  RequestString;
  EFI_STRING                  SaRequestString;
  EFI_STRING                  PchRequestString;
  UINT8                       RootPortSelected;
  UINT8                       Index;

  Status                  = EFI_SUCCESS;
  RequestString           = NULL;
  SaRequestString         = NULL;
  PchRequestString        = NULL;
  RootPortSelected        = 0;

  DEBUG ((DEBUG_INFO, "DTbtFormCallBackFunction\n"));

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED)
    return EFI_UNSUPPORTED;

  if (Action == EFI_BROWSER_ACTION_CHANGING)
    return EFI_SUCCESS;

  if ((Value == NULL) || (ActionRequest == NULL))
    return EFI_INVALID_PARAMETER;

  VarSize = sizeof (SETUP_DATA);
  SetupData = AllocatePool (VarSize);
  ASSERT (SetupData != NULL);
  if (SetupData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (!HiiGetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) SetupData)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  VarSize = sizeof (SA_SETUP);
  SaSetup = AllocatePool (VarSize);
  ASSERT (SaSetup != NULL);
  if (SaSetup == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (!HiiGetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VarSize, (UINT8 *) SaSetup)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  VarSize = sizeof (PCH_SETUP);
  PchSetup = AllocatePool (VarSize);
  ASSERT (PchSetup != NULL);
  if (PchSetup == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (!HiiGetBrowserData (&gPchSetupVariableGuid, L"PchSetup", VarSize, (UINT8 *) PchSetup)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  IoWrite16(0x80, (UINT16)KeyValue);

  DEBUG ((DEBUG_INFO, "Discrete Thunderbolt(TM) call back actions begin\n"));

  switch (KeyValue) {
    case KEY_DTBT_SUPPORT:
      DEBUG ((DEBUG_INFO, "KEY_DTBT_SUPPORT CallBack\n"));
      if (SetupData->DiscreteTbtSupport == 0) {
        SetupData->DTbtController[0] = 0;
        SetupData->DTbtController[1] = 0;
      }
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtController[0]), sizeof (SetupData->DTbtController[0]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtController[1]), sizeof (SetupData->DTbtController[1]));
      break;
    case KEY_DTBT_CONTROLLER0:
    case KEY_DTBT_CONTROLLER1:
      DEBUG ((DEBUG_INFO, " KEY_TBT_CONTROLLER0 / KEY_TBT_CONTROLLER1  CallBack\n"));
      Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, (VOID **) &gDTbtInfoHob);
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Searching Thunderbolt(TM) information Hob...\n"));
        gDTbtInfoHob = GetNextGuidHob (&gDTbtInfoHobGuid, gDTbtInfoHob);
      }
      if (gDTbtInfoHob != NULL) {
        for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++) {
          if (SetupData->DTbtController[Index] != 0x01) {
            continue;
          }
          RootPortSelected = (gDTbtInfoHob->DTbtControllerConfig[Index].PcieRpNumber - 1);
          if ((RootPortSelected >= sizeof (PchSetup->PcieRootPortHPE) / sizeof (UINT8)) ||
              (RootPortSelected >= sizeof (SaSetup->PegRootPortHPE) / sizeof (UINT8)) ||
              (RootPortSelected >= sizeof (SaSetup->PegMaxPayload) / sizeof (UINT8)))
          {
            DEBUG ((DEBUG_ERROR, "%a - RootPortSelected (%d) is weird\n", __FUNCTION__, RootPortSelected));
            continue;
          }

          if (gDTbtInfoHob->DTbtControllerConfig[Index].RpType == PCIE_RP_TYPE_PCH) {
            //
            //if PCH-Rootport is supported for TBT, check for the Port which is selected
            //
            if (SetupData->DiscreteTbtSupport == 1) {
              PchSetup->PcieRootPortHPE[RootPortSelected] = 1;
            } else {
              PchSetup->PcieRootPortHPE[RootPortSelected] = 0;
            }
            PchRequestString = HiiConstructRequestString (PchRequestString, OFFSET_OF (PCH_SETUP, PcieRootPortHPE[RootPortSelected]), sizeof (PchSetup->PcieRootPortHPE[RootPortSelected]));
          } else if (gDTbtInfoHob->DTbtControllerConfig[Index].RpType == PCIE_RP_TYPE_CPU) {
            if (SetupData->DiscreteTbtSupport == 1) {
              SaSetup->PegRootPortHPE[RootPortSelected] = 1;
              SaSetup->PegMaxPayload[RootPortSelected] = 0;
              if (RootPortSelected == 0) {
                SaSetup->Peg0Enable = 1;
                SaSetup->Peg0PowerDownUnusedLanes = 0;
              } else if (RootPortSelected == 1) {
                SaSetup->Peg1Enable = 1;
                SaSetup->Peg1PowerDownUnusedLanes = 0;
              } else if (RootPortSelected == 2) {
                SaSetup->Peg2Enable = 1;
                SaSetup->Peg2PowerDownUnusedLanes = 0;
              } else if (RootPortSelected == 3) {
                SaSetup->Peg3Enable = 1;
                SaSetup->Peg3PowerDownUnusedLanes = 0;
              }
            } else {
              SaSetup->PegRootPortHPE[RootPortSelected] = 0;
              SaSetup->PegMaxPayload[RootPortSelected] = 1;
              if (RootPortSelected == 0) {
                SaSetup->Peg0Enable = 0;
                SaSetup->Peg0PowerDownUnusedLanes = 1;
              } else if (RootPortSelected == 1) {
                SaSetup->Peg1Enable = 0;
                SaSetup->Peg1PowerDownUnusedLanes = 1;
              } else if (RootPortSelected == 2) {
                SaSetup->Peg2Enable = 0;
                SaSetup->Peg2PowerDownUnusedLanes = 1;
              } else if (RootPortSelected == 3) {
                SaSetup->Peg3Enable = 0;
                SaSetup->Peg3PowerDownUnusedLanes = 1;
              }
            }
            SaRequestString = HiiConstructRequestString (SaRequestString, OFFSET_OF (SA_SETUP, Peg0Enable), sizeof (SaSetup->Peg0Enable));
            SaRequestString = HiiConstructRequestString (SaRequestString, OFFSET_OF (SA_SETUP, Peg1Enable), sizeof (SaSetup->Peg1Enable));
            SaRequestString = HiiConstructRequestString (SaRequestString, OFFSET_OF (SA_SETUP, Peg2Enable), sizeof (SaSetup->Peg2Enable));
            SaRequestString = HiiConstructRequestString (SaRequestString, OFFSET_OF (SA_SETUP, Peg3Enable), sizeof (SaSetup->Peg3Enable));
            SaRequestString = HiiConstructRequestString (SaRequestString, OFFSET_OF (SA_SETUP, PegRootPortHPE[RootPortSelected]), sizeof (SaSetup->PegRootPortHPE[Index]));
            SaRequestString = HiiConstructRequestString (SaRequestString, OFFSET_OF (SA_SETUP, Peg0PowerDownUnusedLanes), sizeof (SaSetup->Peg0PowerDownUnusedLanes));
          }
        }
      }
      break;
    case KEY_DTBT_CONTROLLER0_HOSTROUTER:
      DEBUG ((DEBUG_INFO, "KEY_TBT_HOSTROUTER 0 CallBack\n"));
      //
      //As per the Bios implementation Guide
      //(Table 2.3: Host Router resource allocation/reservation scheme) ,
      //we need to allocate enough  resources based on Host router type
      //one port or two port
      //
      if (SetupData->DTbthostRouterPortNumber[0] == 2) {
        SetupData->DTbtPcieExtraBusRsvd[0] = DTBT_PCIE_EXTRA_BUS_RSVD_TWO_PORT_DEFAULT;
        SetupData->DTbtPcieMemRsvd[0] = DTBT_PCIE_MEM_RSVD_TWO_PORT_DEFAULT;
        SetupData->DTbtPcieMemAddrRngMax[0] = DTBT_PCIE_MEM_ADDRRNGMAX_TWO_PORT_DEFAULT;
        SetupData->DTbtPciePMemRsvd[0] = DTBT_PCIE_PMEM_RSVD_TWO_PORT_DEFAULT;
        SetupData->DTbtPciePMemAddrRngMax[0] = DTBT_PCIE_PMEM_ADDRRNGMAX_TWO_PORT_DEFAULT;
      } else {
        SetupData->DTbtPcieExtraBusRsvd[0] = DTBT_PCIE_EXTRA_BUS_RSVD_ONE_PORT_DEFAULT;
        SetupData->DTbtPcieMemRsvd[0] = DTBT_PCIE_MEM_RSVD_ONE_PORT_DEFAULT;
        SetupData->DTbtPcieMemAddrRngMax[0] = DTBT_PCIE_MEM_ADDRRNGMAX_ONE_PORT_DEFAULT;
        SetupData->DTbtPciePMemRsvd[0] = DTBT_PCIE_PMEM_RSVD_ONE_PORT_DEFAULT;
        SetupData->DTbtPciePMemAddrRngMax[0] = DTBT_PCIE_PMEM_ADDRRNGMAX_ONE_PORT_DEFAULT;
      }
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtPcieExtraBusRsvd[0]), sizeof (SetupData->DTbtPcieExtraBusRsvd[0]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtPcieMemRsvd[0]), sizeof (SetupData->DTbtPcieMemRsvd[0]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtPcieMemAddrRngMax[0]), sizeof (SetupData->DTbtPcieMemAddrRngMax[0]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtPciePMemRsvd[0]), sizeof (SetupData->DTbtPciePMemRsvd[0]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtPciePMemAddrRngMax[0]), sizeof (SetupData->DTbtPciePMemAddrRngMax[0]));
      break;
    case KEY_DTBT_CONTROLLER1_HOSTROUTER:
      DEBUG ((DEBUG_INFO, "KEY_TBT_HOSTROUTER 1 CallBack\n"));
      //
      //As per the Bios implementation Guide
      //(Table 2.3: Host Router resource allocation/reservation scheme) ,
      //we need to allocate enough  resources based on Host router type
      //one port or two port
      //
      if (SetupData->DTbthostRouterPortNumber[1] == 2) {
        SetupData->DTbtPcieExtraBusRsvd[1] = DTBT_PCIE_EXTRA_BUS_RSVD_TWO_PORT_DEFAULT;
        SetupData->DTbtPcieMemRsvd[1] = DTBT_PCIE_MEM_RSVD_TWO_PORT_DEFAULT;
        SetupData->DTbtPcieMemAddrRngMax[1] = DTBT_PCIE_MEM_ADDRRNGMAX_TWO_PORT_DEFAULT;
        SetupData->DTbtPciePMemRsvd[1] = DTBT_PCIE_PMEM_RSVD_TWO_PORT_DEFAULT;
        SetupData->DTbtPciePMemAddrRngMax[1] = DTBT_PCIE_PMEM_ADDRRNGMAX_TWO_PORT_DEFAULT;
      } else {
        SetupData->DTbtPcieExtraBusRsvd[1] = DTBT_PCIE_EXTRA_BUS_RSVD_ONE_PORT_DEFAULT;
        SetupData->DTbtPcieMemRsvd[1] = DTBT_PCIE_MEM_RSVD_ONE_PORT_DEFAULT;
        SetupData->DTbtPcieMemAddrRngMax[1] = DTBT_PCIE_MEM_ADDRRNGMAX_ONE_PORT_DEFAULT;
        SetupData->DTbtPciePMemRsvd[1] = DTBT_PCIE_PMEM_RSVD_ONE_PORT_DEFAULT;
        SetupData->DTbtPciePMemAddrRngMax[1] = DTBT_PCIE_PMEM_ADDRRNGMAX_ONE_PORT_DEFAULT;
      }
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtPcieExtraBusRsvd[1]), sizeof (SetupData->DTbtPcieExtraBusRsvd[1]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtPcieMemRsvd[1]), sizeof (SetupData->DTbtPcieMemRsvd[1]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtPcieMemAddrRngMax[1]), sizeof (SetupData->DTbtPcieMemAddrRngMax[1]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtPciePMemRsvd[1]), sizeof (SetupData->DTbtPciePMemRsvd[1]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DTbtPciePMemAddrRngMax[1]), sizeof (SetupData->DTbtPciePMemAddrRngMax[1]));
      break;
    default:
      DEBUG ((DEBUG_INFO, "Continue\n"));
      break;
  }

  DEBUG ((DEBUG_INFO, "Thunderbolt(TM) call back - HiiSetBrowserData begin\n"));
  if (RequestString != NULL) {
    VarSize = sizeof (SETUP_DATA);
    if (!HiiSetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) SetupData, RequestString)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);
    FreePool (RequestString);
  }
  if (SaRequestString != NULL) {
    VarSize = sizeof (SA_SETUP);
    if (!HiiSetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VarSize, (UINT8 *) SaSetup, SaRequestString)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);
    FreePool (SaRequestString);
  }
  if (PchRequestString != NULL) {
    VarSize = sizeof (PCH_SETUP);
    if (!HiiSetBrowserData (&gPchSetupVariableGuid, L"PchSetup", VarSize, (UINT8 *) PchSetup, PchRequestString)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);
    FreePool (PchRequestString);
  }
  DEBUG ((DEBUG_INFO, "Thunderbolt(TM) call back - HiiSetBrowserData end\n"));
  FreePool (SaSetup);
  FreePool (PchSetup);
  FreePool (SetupData);

  *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;

  DEBUG ((DEBUG_INFO, "Thunderbolt(TM) call back actions end\n"));
#endif
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ITbtFormCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  SETUP_DATA                  *SetupData;
  UINTN                       VarSize;
  EFI_STATUS                  Status;
  EFI_STRING                  RequestString;

  Status                  = EFI_SUCCESS;
  RequestString           = NULL;

  DEBUG ((DEBUG_INFO, "TbtFormCallBackFunction \n"));

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED)
    return EFI_UNSUPPORTED;

  if (Action == EFI_BROWSER_ACTION_CHANGING)
    return EFI_SUCCESS;

  if ((Value == NULL) || (ActionRequest == NULL))
    return EFI_INVALID_PARAMETER;

  VarSize = sizeof (SETUP_DATA);
  SetupData = AllocatePool (VarSize);
  ASSERT (SetupData != NULL);
  if (SetupData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (!HiiGetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) SetupData)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  IoWrite16 (0x80, (UINT16)KeyValue);

  DEBUG ((DEBUG_INFO, "Thunderbolt(TM) call back actions begin \n"));

  switch (KeyValue) {
    case KEY_ITBT_SUPPORT:
      DEBUG ((DEBUG_INFO, "KEY_ITBT_SUPPORT CallBack \n"));
      if (SetupData->IntegratedTbtSupport == 1) {
        SetupData->ITbtRtd3 = 1;
      } else {
        SetupData->ITbtRtd3 = 0;
      }
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, ITbtRtd3), sizeof (SetupData->ITbtRtd3));
      break;

    default:
      return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "Thunderbolt(TM) call back - HiiSetBrowserData begin \n"));
  if (RequestString != NULL) {
    VarSize = sizeof (SETUP_DATA);
    if (!HiiSetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) SetupData, RequestString)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);
    FreePool (RequestString);
  }

  DEBUG ((DEBUG_INFO, "Thunderbolt(TM) call back - HiiSetBrowserData end \n"));
  FreePool (SetupData);

  *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;

  DEBUG ((DEBUG_INFO, "Thunderbolt(TM) call back actions end \n"));

  return EFI_SUCCESS;
}

/**

   This function is called to set CM mode value to CMOS

   @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
   @param  Action                 Specifies the type of action taken by the browser.
   @param  QuestionId             A unique value which is sent to the original
                                  exporting driver so that it can identify the type
                                  of data to expect. The format of the data tends to
                                  vary based on the opcode that generated the callback.
   @param  Type                   The type of value for the question.
   @param  Value                  A pointer to the data being sent to the original
                                  exporting driver.
   @param  ActionRequest          On return, points to the action requested by the
                                  callback function.

   @retval EFI_SUCCESS            The callback successfully handled the action.
   @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                  variable and its data.
   @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                  callback.
   @retval EFI_DEVICE_ERROR       The variable could not be saved to browser.
   @retval Other                  Other errors occur.
 **/
EFI_STATUS
EFIAPI
Usb4CmModeCallBack (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  SETUP_DATA                 *SetupData;
  UINTN                      SetupDataVarSize;
  UINT32                     SetupDataVarAttributes;
  UINT8                      Usb4CmModeCmosValue;
  UINT8                      TempITbtRtd3;
  EFI_STATUS                 Status;

  DEBUG ((EFI_D_INFO, "Usb4CmModeCallBack() Start\n"));

  switch (Action) {
    case EFI_BROWSER_ACTION_CHANGED:
      Usb4CmModeChangeEnable = TRUE;
      break;

    case EFI_BROWSER_ACTION_SUBMITTED:

      SetupDataVarAttributes = 0;
      SetupData = NULL;
      SetupDataVarSize = sizeof (SETUP_DATA);
      SetupData = AllocatePool (SetupDataVarSize);
      if (SetupData == NULL) {
        DEBUG ((EFI_D_ERROR, "Usb4CmModeCallBack() - Allocate setup variable memory fail\n"));
        return EFI_OUT_OF_RESOURCES;
      }

      Status = gRT->GetVariable (
                      L"Setup",
                      &gSetupVariableGuid,
                      &SetupDataVarAttributes,
                      &SetupDataVarSize,
                      SetupData
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Get SetupData fail, Status = %r\n", Status));
        FreePool (SetupData);
        return Status;
      }

      if (Usb4CmModeChangeEnable == TRUE) {
        Usb4CmModeChangeEnable = FALSE;

        //
        // OS dependent mode -> set SW CM value to CMOS
        //
        if (SetupData->Usb4CmMode == USB4_SETUP_CM_MODE_OS) {
          CmosWrite8 (CMOS_USB4_CM_MODE_REG, USB4_CM_MODE_SW_CM);
        }
      }

      Usb4CmModeCmosValue = CmosRead8 (CMOS_USB4_CM_MODE_REG);
      if ((SetupData->Usb4CmMode == USB4_SETUP_CM_MODE_FW) ||
          ((SetupData->Usb4CmMode == USB4_SETUP_CM_MODE_OS) && (Usb4CmModeCmosValue == USB4_CM_MODE_FW_CM))) {
        TempITbtRtd3 = SetupData->ITbtRtd3;
      } else {
        TempITbtRtd3 = 1;
      }

      if (SetupData->ITbtRtd3 != TempITbtRtd3) {
        SetupData->ITbtRtd3 = TempITbtRtd3;
        Status = gRT->SetVariable (
                        L"Setup",
                        &gSetupVariableGuid,
                        SetupDataVarAttributes,
                        SetupDataVarSize,
                        SetupData
                        );
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Store SetupData fail, Status = %r\n", Status));
          FreePool (SetupData);
          return Status;
        }
      }

      FreePool (SetupData);
      break;

    default:
      return EFI_UNSUPPORTED;
      break;
  }

  DEBUG ((EFI_D_INFO, "Usb4CmModeCallBack() End\n"));

  return EFI_SUCCESS;
}
