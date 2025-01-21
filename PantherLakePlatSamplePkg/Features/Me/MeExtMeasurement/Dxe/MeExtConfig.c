/** @file
  This file Extends CSME Configuration to PCR[1].

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include "MeExtMeasurement.h"
#include "MeExtConfig.h"
#include <Library/MeShowBufferLib.h>

/**
  Ready To Boot Event call back function performs getting AMT Config data from CSME and extending to TPM-PCR[1]

  @param[in] Event         The event that triggered this notification function
  @param[in] Context       Pointer to the notification functions context
**/
VOID
EFIAPI
AmtConfigReadyToBootEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                  Status;
  AMT_CONFIG_DATA             AmtConfigData;
  PROV_DATA_STATE             AmtConfigProvState;
  UINT32                      AmtConfigSize;
  BIOS_REDIRECTION_SETTINGS   AmtConfigRedirSetting;
  ADMIN_SETUP_AUDIT_RECORD    AmtConfigProvAuditRecord;
  INTEL_ME_CONFIG_DATA        MeAmtEventConfigData;
  static CONST CHAR8          Signature[] = "IntelCSMEAmtConfig";

  gBS->CloseEvent (Event);

  ZeroMem (&(MeAmtEventConfigData), sizeof (MeAmtEventConfigData));
  ZeroMem (&(AmtConfigData), sizeof (AmtConfigData));
  ZeroMem (&(AmtConfigProvAuditRecord), sizeof (AmtConfigProvAuditRecord));
  CopyMem (&(MeAmtEventConfigData.Signature), Signature, AsciiStrLen (Signature));
  MeAmtEventConfigData.DataLength = AMT_CONFIG_RECORD_LENGTH;

  //
  // Get AMT Config from CSME
  //
  AmtConfigSize = sizeof (PROV_DATA_STATE);
  Status = HeciReadFileExMsg (McaAmtProvStateFileId, 0, &AmtConfigSize, 0, (UINT8 *) &AmtConfigProvState);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read AMT Provision state file, Status is %r \n", Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  //
  // Get BIOS redirection settings from CSME
  //
  AmtConfigSize = sizeof (BIOS_REDIRECTION_SETTINGS);
  Status = HeciReadFileExMsg (McaAmtBiosRedirFileId, 0, &AmtConfigSize, 0, (UINT8 *) &AmtConfigRedirSetting);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read AMT BIOS Redirection file, Status is %r \n", Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  //
  // Fill AMT Status in the structure 'AmtConfigData' as per file read data
  //
  AmtConfigData.AmtStatus.AmtGloballyEnabled  = (AmtConfigProvState.Bits.AmtGloballyEnabled == 1) ? 1 : 0;
  AmtConfigData.AmtStatus.MebxPwdSet          = (AmtConfigProvState.Bits.MebxPwdChanged == 1) ? 1 : 0;
  AmtConfigData.AmtStatus.AmtProvisioned      = (AmtConfigProvState.Bits.ProvisioningState == ProvisioningStatePost) ? 1 : 0;  // ProvisioningState 2 indicates AMT provisioning is successful
  AmtConfigData.AmtStatus.AmtProvisioningMode = (AmtConfigProvState.Bits.ClientControlMode == 1) ? 1 : 0;
  AmtConfigData.AmtStatus.ZtcEnabled          = (AmtConfigProvState.Bits.ZTCEnabled == 1) ? 1 : 0;

  AmtConfigData.AmtStatus.KvmEnabled          = (AmtConfigRedirSetting.KvmHostIf == 1) ? 1 : 0;
  AmtConfigData.AmtStatus.SolEnabled          = (AmtConfigRedirSetting.SolHostIf == 1) ? 1 : 0;
  AmtConfigData.AmtStatus.UsbrEnabled         = (AmtConfigRedirSetting.IderHostIf == 1) ? 1 : 0;

  //
  // Get Secure PKI DNS Suffix from CSME only if AmtStatus bit0 is set
  //
  if (AmtConfigData.AmtStatus.AmtGloballyEnabled == 1) {
    AmtConfigSize = SECURE_PKI_DNS_SUFFIX_LENGTH;
    Status = HeciReadFileExMsg (McaAmtSecPkiFileId, 0, &AmtConfigSize, 0, (UINT8 *) AmtConfigData.SecPkiDnsSfx);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to read Secure PKI DNS Suffix file, Status is %r \n", Status));
      ASSERT_EFI_ERROR (Status);
      return;
    }
  }

  //
  // Get AMT provisioning audit record from CSME only if AmtStatus bit2 is set & bit3 is clear
  //
  if ((AmtConfigData.AmtStatus.AmtProvisioned == 1) && (AmtConfigData.AmtStatus.AmtProvisioningMode == 0)) {
    AmtConfigSize = sizeof (ADMIN_SETUP_AUDIT_RECORD);
    Status = HeciReadFileExMsg (McaAmtProvAdtRecFileId, 0, &AmtConfigSize, 0, (UINT8 *) &AmtConfigProvAuditRecord);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to read AMT Provisioning Audit Record file, Status is %r \n", Status));
    }
    //
    // Fill PKI Provisioning leaf Certificate hash Algorithm & Finger print hash in the structure 'AmtConfigData' as per file read data
    //
    AmtConfigData.PkiProvLeafCertHashAlgm = (UINT32) AmtConfigProvAuditRecord.LeafHashAlgorithm;
    CopyMem (AmtConfigData.PkiProvLeafCertHashFingerPrint, AmtConfigProvAuditRecord.LeafCertFingerprint, PKI_PROV_LEAF_CERT_HASH_FINGERPRINT_LENGTH);
  }

  // Copy AMT Config Data to ME Event Config data structure
  CopyMem (MeAmtEventConfigData.ConfigData, &AmtConfigData, AMT_CONFIG_RECORD_LENGTH);

  // Print AMT Config data in ME Event Config data structure
  DEBUG ((DEBUG_INFO, "Extend AMT Config data to TPM-PCR[1]\n"));
  ShowBuffer ((UINT8 *) &MeAmtEventConfigData, sizeof (INTEL_ME_CONFIG_DATA));

  //
  // Extend AMT Config data to TPM PCR[1] & Event log to TPM log
  //
  Status = TpmMeasureAndLogData (
             1,
             EV_NONHOST_CONFIG,
             &MeAmtEventConfigData,
             sizeof (INTEL_ME_CONFIG_DATA),
             &MeAmtEventConfigData,
             sizeof (INTEL_ME_CONFIG_DATA)
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed AMT Config data to TPM-PCR[1]\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return;
}