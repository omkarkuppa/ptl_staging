/**@file
  To perform CSME Unconfiguration
  To perform PSR update which are common required for both RPE & LPE support.

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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <PsParameter.h>
#include <MeBiosPayloadHob.h>
#include <Library/DxeMeLib.h>
#include <Library/HobLib.h>
#include <Library/TimerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PsCsmeAccessLib.h>

#define UNCONFIGURE_TIMEOUT       60     ///< Timeout is set to 60 seconds

/**
  Perform Unconfigure CSME without password and try to wait for completion.

  @retval EFI_SUCCESS  Successfully sent Command to CSME to perform Unconfiguration
  @retval EFI_ABORTED  Unconfiguration error
  @retval EFI_TIMEOUT  Unconfiguration is still in progress after time elapsed
**/
EFI_STATUS
EFIAPI
PsCsmeUnconfigure (
  VOID
  )
{
  UINT8      UnconfigStatus;
  UINT32     Timeout;
  EFI_STATUS Status;

  Timeout = UNCONFIGURE_TIMEOUT;

  DEBUG ((DEBUG_INFO, "PS: %a() Entry\n", __FUNCTION__));

  Status = HeciUnconfigWithoutPwd ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  do {
    Status = HeciGetUnconfigureStatus (&UnconfigStatus);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (UnconfigStatus == ME_UNCONFIG_ERROR) {
      return EFI_ABORTED;
    } else if ((UnconfigStatus == ME_UNCONFIG_NOT_IN_PROGRESS) || (UnconfigStatus == ME_UNCONFIG_FINISHED)) {
      return EFI_SUCCESS;
    }

    MicroSecondDelay (1000000);
  } while (Timeout--);

  return EFI_TIMEOUT;
}

/**
  Send Platform Sanitization Report to CSME to update PSR Record

  @param[in] PS_BOOT_PARAMETERS         Pointer to Boot Parameter structure.

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              Command Failed
**/
EFI_STATUS
EFIAPI
PsSummaryReportToPsr (
  IN PS_BOOT_PARAMETERS     *PsBootParameters
  )
{
  EFI_STATUS                Status;
  PSR_DEVICE_ERASE_REPORT   PsReportToPsr;
  ME_BIOS_PAYLOAD_HOB       *MbpHob;

  DEBUG ((DEBUG_INFO, "PS: %a() Entry\n", __FUNCTION__));

  MbpHob = NULL;
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);

  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Hob Not Found\n"));
    return EFI_NOT_FOUND;
  }

  if ((MbpHob->MeBiosPayload.Psr.Available == FALSE) || (MbpHob->MeBiosPayload.Psr.PsrData.PsrLogState == FALSE)) {
    DEBUG ((DEBUG_ERROR, "PSR Log not started\n"));
    return EFI_NOT_FOUND;
  }

  ZeroMem (&PsReportToPsr, sizeof (PSR_DEVICE_ERASE_REPORT));

  switch (PsBootParameters->TriggerSource) {
    case BootRpe:
      PsReportToPsr.EraseSource = 0;
      break;

    case BootLpe:
      PsReportToPsr.EraseSource = 1;
      break;

    default:
      PsReportToPsr.EraseSource = 0;
  }

  PsReportToPsr.EraseActionsAttempted = PsBootParameters->PsStatus.PsAttempted;
  PsReportToPsr.EraseActionsStatus    = PsBootParameters->PsStatus.PsAttemptResult;
  CopyMem (&(PsReportToPsr.EraseFailureReason),  &(PsBootParameters->PsStatus.PsFailReason), (sizeof (UINT8) * 32));

  DEBUG ((DEBUG_INFO, "PS:Reporting Platform Sanitize Summary to CSME...\n"));
  Status = HeciDeviceEraseReport (&PsReportToPsr);

  DEBUG ((DEBUG_INFO, "PS: %a() Exit, Status = %r\n", __FUNCTION__, Status));
  return Status;
}
