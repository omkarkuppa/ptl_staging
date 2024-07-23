/** @file
  PSR package driver install protocols for CSME event Notification (Chassis Intrusion
  and Critical Temp) and PsrControlDxe driver locates and use them.

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
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PsrProtocol.h>
#include <ChassisIntrDetHob.h>
#include <Library/EcMiscLib.h>
#include <MeBiosPayloadHob.h>

PLATFORM_SERVICE_RECORD_PROTOCOL  *mPlatformServiceRecordProtocol = NULL;

/**
  Check for Chassis Intrusion and inform CSME if intrusion detected
**/
VOID
PsrUpdateChassisIntrusion (
  VOID
  ) {
  CHASSIS_INTR_DET_HOB                *ChassisIntrDetHob;
  EFI_STATUS                          Status;

  ChassisIntrDetHob    = NULL;

  ChassisIntrDetHob = GetFirstGuidHob (&gChassisIntrudeDetHobGuid);

  if ((ChassisIntrDetHob != NULL) && (ChassisIntrDetHob->Status)) {
    Status = mPlatformServiceRecordProtocol->NotifyChassisIntrusionDetection (mPlatformServiceRecordProtocol);
    DEBUG ((DEBUG_INFO, "Status of updating Chassis Intrusion to CSME - %r\n", Status));
  }
}

/**
  Check for Critical temperature shutdown and inform CSME
**/
VOID
PsrUpdateCriticalTempShutdown (
  VOID
  ) {
  EFI_STATUS  Status;

  //
  // Intel RVP refers EC Firmware to detect Critical Temperature shutdown.
  // In case if OEM choose to different method for Critical temperature detection, need to replace this function
  // with custom implementation.
  //
  if (CriticalTempShutdownHappened ()) {
    Status = mPlatformServiceRecordProtocol->NotifyCriticalTempShutdown (mPlatformServiceRecordProtocol);
    DEBUG ((DEBUG_INFO, "Status of updating Critical Temp Shutdown to CSME - %r\n", Status));
  }

}

/**
  EP function for PSR Control

  @param[in]  ImageHandle  Not used.
  @param[in]  SystemTable  Global system service table.

  @retval     EFI_SUCCESS  This constructor always return EFI_SUCCESS.
              It will ASSERT on errors.
**/
EFI_STATUS
EFIAPI
PsrControlEntryPoint (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  ) {
  EFI_STATUS            Status;
  UINT32                mPsrSupportedCapabilities;
  ME_BIOS_PAYLOAD_HOB   *MbpHob = NULL;

  mPsrSupportedCapabilities = PcdGet32 (VpdPcdPsrSupportedCapabilities);

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if ((MbpHob == NULL) || (MbpHob->MeBiosPayload.Psr.Available == FALSE)) {
    DEBUG ((DEBUG_INFO, "PSR not supported\n"));
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (&gPlatformServiceRecordProtocolGuid, NULL, (VOID **) &mPlatformServiceRecordProtocol);
  if (!EFI_ERROR (Status)) {
    if ((mPsrSupportedCapabilities & CHASSIS_INTRUSION) == CHASSIS_INTRUSION) {
      // Notify CSME of Chassis Intrusion Detection event if indicated in MbpHob
      if ((MbpHob->MeBiosPayload.Psr.PsrData.ChassisIntrusionEvent == 1)) {
        PsrUpdateChassisIntrusion ();
      }
    }
    if ((mPsrSupportedCapabilities & CRITICAL_TEMPERATURE) == CRITICAL_TEMPERATURE) {
      // Notify CSME of Critical Temp Shutdown event if PSR Logging is enabled
      if (MbpHob->MeBiosPayload.Psr.PsrData.PsrLogState != 0) {
        PsrUpdateCriticalTempShutdown ();
      }
    }
  }

  return Status;

}
