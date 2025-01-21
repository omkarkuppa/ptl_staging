/** @file
  Instance of Platform FSP MultiPhase hook library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <PiPei.h>
#include <FspmUpd.h>
#include <FspsUpd.h>
#include <FspGlobalData.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/FspCommonLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/FspSwitchStackLib.h>
#include <Library/PlatformFspMultiPhaseLib.h>

EFI_STATUS
EFIAPI
TcssPeiInitDoneNotify (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  SetFspApiReturnStatus (EFI_SUCCESS);
  Pei2LoaderSwitchStack ();
  return EFI_SUCCESS;
}

STATIC
EFI_PEI_NOTIFY_DESCRIPTOR  mTcssPeiInitDonePpiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gTcssPeiInitDonePpiGuid,
  TcssPeiInitDoneNotify
};

/**
  FSP MultiPhase Platform Get Number Of Phases Function.

  Allows an FSP binary to dynamically update the number of phases at runtime.
  For example, UPD settings could negate the need to enter the multi-phase flow
  in certain scenarios. If this function returns FALSE, the default number of phases
  provided by PcdMultiPhaseNumberOfPhases will be returned to the bootloader instead.

  @param[in] ApiIdx                  - Internal index of the FSP API.
  @param[in] NumberOfPhasesSupported - How many phases are supported by current FSP Component.

  @retval  TRUE  - NumberOfPhases are modified by Platform during runtime.
  @retval  FALSE - The Default build time NumberOfPhases should be used.

**/
BOOLEAN
EFIAPI
FspMultiPhasePlatformGetNumberOfPhases (
  IN     UINTN   ApiIdx,
  IN OUT UINT32  *NumberOfPhasesSupported
  )
{
  FSP_GLOBAL_DATA              *FspGlobalData;
  FSPM_UPD                     *FspmUpd;
  FSPS_UPD                     *FspsUpd;
  FSPS_ARCH_UPD                *FspsArchUpdPtr;

  FspGlobalData = GetFspGlobalDataPointer ();
  FspmUpd = (FSPM_UPD *) FspGlobalData->MemoryInitUpdPtr;
  FspsUpd = (FSPS_UPD *) FspGlobalData->SiliconInitUpdPtr;

  if (ApiIdx == FspMultiPhaseMemInitApiIndex) {
    *NumberOfPhasesSupported = 0;
    return TRUE;
  }
  if (ApiIdx == FspMultiPhaseSiInitApiIndex) {
    if (FspsUpd != NULL) {
      FspsArchUpdPtr = (FSPS_ARCH_UPD *) &FspsUpd->FspsArchUpd;
      if ((FspmUpd != NULL) && (FspmUpd->FspUpdHeader.Signature == FSPM_UPD_SIGNATURE)) {
        if (FspsArchUpdPtr->Revision >= 2) {
          if (FspmUpd->FspmConfig.TcssXhciEn) {
            *NumberOfPhasesSupported = 1;
          } else {
            *NumberOfPhasesSupported = 0;
          }
          return TRUE;
        } else {
          *NumberOfPhasesSupported = (FspsArchUpdPtr->EnableMultiPhaseSiliconInit && FspmUpd->FspmConfig.TcssXhciEn) ? 1:0;
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}

/**
  This function supports Platform FspMultiPhase implementation in memory initialization.

  @retval EFI_SUCCESS  if successful

**/
EFI_STATUS
EFIAPI
FspMultiPhaseMemInitPlatformWorker (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  This function supports Platform FspMultiPhase implementation in silicon initialization.

  @retval EFI_SUCCESS  if successful

**/
EFI_STATUS
EFIAPI
FspMultiPhaseSiInitPlatformWorker (
  VOID
  )
{
  EFI_STATUS                     Status;
  UINT8                          EnableMultiPhaseSiliconInit;
  UINT32                         NumberOfPhasesSupported;
  FSP_GLOBAL_DATA                *FspGlobalData;
  FSPS_UPD                       *FspsUpd;
  FSPS_ARCH_UPD                  *FspsArchUpdPtr;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));
  FspGlobalData = GetFspGlobalDataPointer ();
  EnableMultiPhaseSiliconInit = 0;
  FspsArchUpdPtr = NULL;
  FspsUpd = NULL;
  Status = EFI_SUCCESS;

  if ((FspGlobalData == NULL) ||
      ((UINTN)FspGlobalData == MAX_ADDRESS) ||
      ((UINTN)FspGlobalData == MAX_UINT32) ||
      (FspGlobalData->Signature != FSP_GLOBAL_DATA_SIGNATURE)) {
    return EFI_INVALID_PARAMETER;
  }

  if (FspGlobalData->SiliconInitUpdPtr != NULL) {
    FspsUpd = FspGlobalData->SiliconInitUpdPtr;
    if (FspsUpd->FspUpdHeader.Signature == FSPS_UPD_SIGNATURE) {
      FspsArchUpdPtr = (FSPS_ARCH_UPD *) &FspsUpd->FspsArchUpd;
    }
  }

  if (FspsArchUpdPtr != NULL) {
    if (FspsArchUpdPtr->Revision >= 2) {
      if (FspMultiPhasePlatformGetNumberOfPhases (FspMultiPhaseSiInitApiIndex, &NumberOfPhasesSupported) == TRUE) {
        EnableMultiPhaseSiliconInit = (NumberOfPhasesSupported != 0) ? 1:0;
      } else {
        EnableMultiPhaseSiliconInit = (PcdGet32 (PcdMultiPhaseNumberOfPhases) != 0) ? 1:0;
      }
    } else {
      EnableMultiPhaseSiliconInit = FspsArchUpdPtr->EnableMultiPhaseSiliconInit;
    }
  } else {
    return EFI_INVALID_PARAMETER;
  }
  if (EnableMultiPhaseSiliconInit) {
    Status = PeiServicesNotifyPpi (&mTcssPeiInitDonePpiNotifyList);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
    }
  }

  return Status;
}
