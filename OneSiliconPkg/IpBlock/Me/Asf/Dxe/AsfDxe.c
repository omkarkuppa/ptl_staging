/** @file
  Alert Standard Format Initialization driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2005 Intel Corporation.

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

#include <PiDxe.h>
#include <Pi/PiStatusCode.h>
#include <IndustryStandard/AlertStandardFormatTable.h>
#include <Library/HobLib.h>
#include <Library/DxeAsfHeciLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IntelRcStatusCode.h>
#include <Library/MeUtilsLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeInfoLib.h>
#include <Protocol/AsfProtocol.h>
#include <Protocol/OneClickRecoveryProtocol.h>
#include <Register/HeciRegs.h>
#include <IndustryStandardAsf.h>
#include <IntelAsfExtensions.h>
#include <MeBiosPayloadHob.h>
#include <MeState.h>

GLOBAL_REMOVE_IF_UNREFERENCED ALERT_STANDARD_FORMAT_PROTOCOL  mAsfProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED ASF_BOOT_OPTIONS                mAsfOcrBootOptions;

/**
  Register AsfOcrBootOptions callback function to avoid AsfDxe without saving UEFI boot options to OCR.

  @param[in]  Event         The Event this notify function registered to.
  @param[in]  Context       Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
AsfOcrBootOptionsCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                   Status;
  ONE_CLICK_RECOVERY_PROTOCOL  *OneClickRecoveryProtocol;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol (&gOneClickRecoveryProtocolGuid, NULL, (VOID **) &OneClickRecoveryProtocol);
  if (EFI_ERROR (Status)) {
    return;
  }
  OneClickRecoveryProtocol->OcrSaveUefiBootOption (mAsfOcrBootOptions);
  ZeroMem ((VOID *) &mAsfOcrBootOptions, sizeof (ASF_BOOT_OPTIONS));

  //
  // Execute ClearBootOptions after the OCR driver gets and saves Uefi Boot Option.
  //
  Status = ClearBootOptions ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: %a() Exit - Error by ClearBootOptions, Status: %r\n", __FUNCTION__, Status));
    return;
  }
}

/**
  Retrieve Boot Options from CSME.

  @retval EFI_SUCCESS             Boot options copied
  @retval EFI_NOT_READY           No boot options
**/
STATIC
EFI_STATUS
AsfGetBootOptions (
  VOID
  )
{
  EFI_STATUS                          Status;
  UINT32                              MeStatus;
  UINT32                              MeMode;
  VOID                                *Registration;
  EFI_EVENT                           Event;

  DEBUG ((DEBUG_INFO, "ME-BIOS: %a() Entry.\n", __FUNCTION__ ));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ENTRY); //PostCode (0xE09)

  mAsfProtocol.AsfBootOptions.SubCommand = ASF_BOOT_OPTIONS_NOT_PRESENT;
  mAsfProtocol.AsfBootOptions.Version    = ASF_VERSION;

  ///
  /// Get ME Status
  ///
  Status = MeBiosGetMeStatus (&MeStatus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Get ME Operation Mode
  ///
  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Only Send ASF Get Boot Options message when CSME is ready and FW INIT is completed
  ///
  if (ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT (MeStatus) && (MeMode == ME_MODE_NORMAL)) {
    if (WaitFirmwareInitComplete () != EFI_TIMEOUT) {
      Status = GetBootOptions (&mAsfProtocol.AsfBootOptions);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ME-BIOS: %a() Exit - Error by HECI access fail.\n", __FUNCTION__));
        REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ERR_HECI_ACSFAIL); //PostCode (0xEE9)
        return Status;
      }

      ///
      /// If boot option is present and retrieved, BIOS should keep it and send Clear Boot Options message.
      ///
      if (mAsfProtocol.AsfBootOptions.SubCommand == ASF_BOOT_OPTIONS_PRESENT) {

        ///
        /// The OCR protocol has not yet been installed because the OCR driver uses the DxeAsfLib with gAlertStandardFormatProtocolGuid dependency.
        /// Register AsfOcrBootOptions callback function to save UEFI boot options to OCR when OCR is ready.
        ///
        if ((mAsfProtocol.AsfBootOptions.SpecialCommand == ASF_INTEL_OEM_FORCE_HTTPS_BOOT_CMD) ||
            (mAsfProtocol.AsfBootOptions.SpecialCommand == ASF_INTEL_OEM_FORCE_PBA_BOOT_CMD)) {
          CopyMem (&mAsfOcrBootOptions, (CHAR8 *)&mAsfProtocol.AsfBootOptions, sizeof (ASF_BOOT_OPTIONS));

          Status = gBS->CreateEvent (
                          EVT_NOTIFY_SIGNAL,
                          TPL_CALLBACK,
                          AsfOcrBootOptionsCallback,
                          NULL,
                          &Event
                          );
          if (EFI_ERROR (Status)) {
            return Status;
          }

          Status = gBS->RegisterProtocolNotify (
                          &gOneClickRecoveryProtocolGuid,
                          Event,
                          &Registration
                          );
          if (EFI_ERROR (Status)) {
            return Status;
          }
        }

        if (mAsfProtocol.AsfBootOptions.SpecialCommand == ASF_INTEL_OEM_CMD &&
            mAsfProtocol.AsfBootOptions.SpecialCommandParamLowByte & ENABLE_REMOTE_PLATFORM_ERASE) {
          //
          // When Remote Platform Erase boot option is set, don't clear boot options, but discard
          // any other boot option except KVM and SOL
          //
          DEBUG ((DEBUG_INFO, "ASF Clear Boot Options skipped due to Remote Platform Erase command\n"));
          DEBUG ((DEBUG_INFO, "Dumping SpecialCommandParam, BootOptions and OemParameters before discarding\n"));
          DEBUG ((DEBUG_INFO, "SpecialCommandParamHighByte = 0x%x\n", mAsfProtocol.AsfBootOptions.SpecialCommandParamHighByte));
          DEBUG ((DEBUG_INFO, "SpecialCommandParamLowByte  = 0x%x\n", mAsfProtocol.AsfBootOptions.SpecialCommandParamLowByte));
          DEBUG ((DEBUG_INFO, "BootOptionsMaskByte1        = 0x%x\n", mAsfProtocol.AsfBootOptions.BootOptionsMaskByte1));
          DEBUG ((DEBUG_INFO, "BootOptionsMaskByte2        = 0x%x\n", mAsfProtocol.AsfBootOptions.BootOptionsMaskByte2));
          DEBUG ((DEBUG_INFO, "OemParameterByte1           = 0x%x\n", mAsfProtocol.AsfBootOptions.OemParameterByte1));
          DEBUG ((DEBUG_INFO, "OemParameterByte2           = 0x%x\n", mAsfProtocol.AsfBootOptions.OemParameterByte2));
          DEBUG ((DEBUG_INFO, "End\n\n" ));
          mAsfProtocol.AsfBootOptions.SpecialCommandParamLowByte &= (USE_KVM | ENABLE_REMOTE_PLATFORM_ERASE);
          mAsfProtocol.AsfBootOptions.OemParameterByte1          &= USE_SOL;
          mAsfProtocol.AsfBootOptions.BootOptionsMaskByte1        = 0;
          mAsfProtocol.AsfBootOptions.BootOptionsMaskByte2        = 0;
        } else if ((mAsfProtocol.AsfBootOptions.SpecialCommand != ASF_INTEL_OEM_FORCE_HTTPS_BOOT_CMD) &&
                   (mAsfProtocol.AsfBootOptions.SpecialCommand != ASF_INTEL_OEM_FORCE_PBA_BOOT_CMD)) {
          //
          // When SpecialCommand is Http Boot or PBA Boot, ClearBootOptions would execute in AsfOcrBootOptionsCallback.
          // ClearBootOptions should execute after the OCR driver gets and saves Uefi Boot Option.
          //
          Status = ClearBootOptions ();
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "ME-BIOS: %a() Exit - Error by ClearBootOptions, Status: %r\n", __FUNCTION__, Status));
            REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ERR_CLRBOOTOPT); //PostCode (0xEC9)
            return Status;
          }
        }
      }
    }
  }

  DEBUG ((DEBUG_INFO, "AsfBootOptions.SubCommand                  = 0x%x\n", mAsfProtocol.AsfBootOptions.SubCommand));
  DEBUG ((DEBUG_INFO, "AsfBootOptions.Version                     = 0x%x\n", mAsfProtocol.AsfBootOptions.Version));
  DEBUG ((DEBUG_INFO, "AsfBootOptions.IanaId                      = 0x%x\n", mAsfProtocol.AsfBootOptions.IanaId));
  DEBUG ((DEBUG_INFO, "AsfBootOptions.SpecialCommand              = 0x%x\n", mAsfProtocol.AsfBootOptions.SpecialCommand));
  DEBUG ((DEBUG_INFO, "AsfBootOptions.SpecialCommandParamHighByte = 0x%x\n", mAsfProtocol.AsfBootOptions.SpecialCommandParamHighByte));
  DEBUG ((DEBUG_INFO, "AsfBootOptions.SpecialCommandParamLowByte  = 0x%x\n", mAsfProtocol.AsfBootOptions.SpecialCommandParamLowByte));
  DEBUG ((DEBUG_INFO, "AsfBootOptions.BootOptionsMaskByte1        = 0x%x\n", mAsfProtocol.AsfBootOptions.BootOptionsMaskByte1));
  DEBUG ((DEBUG_INFO, "AsfBootOptions.BootOptionsMaskByte2        = 0x%x\n", mAsfProtocol.AsfBootOptions.BootOptionsMaskByte2));
  DEBUG ((DEBUG_INFO, "AsfBootOptions.OemParameterByte1           = 0x%x\n", mAsfProtocol.AsfBootOptions.OemParameterByte1));
  DEBUG ((DEBUG_INFO, "AsfBootOptions.OemParameterByte2           = 0x%x\n", mAsfProtocol.AsfBootOptions.OemParameterByte2));

  DEBUG ((DEBUG_INFO, "ME-BIOS: %a() Exit - Success.\n", __FUNCTION__));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_EXIT); //PostCode (0xE29)
  return EFI_SUCCESS;
}

/**
  The driver entry point - detect ASF support or not, if support, will install relative protocol.

  @param[in] ImageHandle          Handle for this drivers loaded image protocol.
  @param[in] SystemTable          EFI system table.

  @retval EFI_SUCCESS             The driver installed without error.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
**/
EFI_STATUS
EFIAPI
AsfDxeEntryPoint (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                      Status;
  ME_BIOS_PAYLOAD_HOB             *MbpHob;
  HECI_FWS_REGISTER               MeFirmwareStatus;

  DEBUG ((DEBUG_INFO, "%a() start\n", __FUNCTION__));

  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);
  if (MeFirmwareStatus.ul == 0xFFFFFFFF){
    DEBUG ((DEBUG_INFO, "%a() - not supported\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "No MBP Data Protocol available\n"));
    return EFI_UNSUPPORTED;
  }

  if (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType != IntelMeCorporateFw) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem ((VOID *) &mAsfProtocol,       sizeof (ALERT_STANDARD_FORMAT_PROTOCOL));
  ZeroMem ((VOID *) &mAsfOcrBootOptions, sizeof (ASF_BOOT_OPTIONS));

  Status = AsfGetBootOptions ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Boot Options from CSME fail\n"));
    return EFI_UNSUPPORTED;
  }

  ///
  /// Install the AlertStandardFormat interface
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gAlertStandardFormatProtocolGuid,
                  &mAsfProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

