/** @file
  Print whole MBP_DATA_HOB and serial out.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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
#include <MeBiosPayloadHob.h>

/**
  Print MbpHob data

  @param[in] MbpHob             Pointer to Mbp Hob
**/
VOID
PrintMbpData (
  IN ME_BIOS_PAYLOAD_HOB *MbpHob
  )
{
  if (MbpHob == NULL) {
    return;
  }
  DEBUG ((DEBUG_INFO, "\n------------------- MeBiosPayload Data HOB Print Begin -------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : 0x%x\n", MbpHob->Revision));
  DEBUG ((DEBUG_INFO, "MeBiosPayload FwVersionName ---\n"));
  DEBUG ((DEBUG_INFO, " ME FW MajorVersion  : %d\n", MbpHob->MeBiosPayload.FwVersionName.MajorVersion));
  DEBUG ((DEBUG_INFO, " ME FW MinorVersion  : %d\n", MbpHob->MeBiosPayload.FwVersionName.MinorVersion));
  DEBUG ((DEBUG_INFO, " ME FW HotfixVersion : %d\n", MbpHob->MeBiosPayload.FwVersionName.HotfixVersion));
  DEBUG ((DEBUG_INFO, " ME FW BuildVersion  : %d\n", MbpHob->MeBiosPayload.FwVersionName.BuildVersion));

  if (MbpHob->MeBiosPayload.FwCapsSku.Available == TRUE) {
    DEBUG ((DEBUG_INFO, "MeBiosPayload FwCapabilities ---\n"));
    DEBUG ((DEBUG_INFO, " FullMng         : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.FullMng));
    DEBUG ((DEBUG_INFO, " StdMng          : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.StdMng));
    DEBUG ((DEBUG_INFO, " Amt             : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt));
    DEBUG ((DEBUG_INFO, " OCR             : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.OCR));
    DEBUG ((DEBUG_INFO, " RPE             : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.RPE));
    DEBUG ((DEBUG_INFO, " PSR             : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.PSR));
    DEBUG ((DEBUG_INFO, " ISH             : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.ISH));
    DEBUG ((DEBUG_INFO, " IPV6            : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.IPV6));
    DEBUG ((DEBUG_INFO, " KVM             : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.KVM));
    DEBUG ((DEBUG_INFO, " TLS             : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.TLS));
    DEBUG ((DEBUG_INFO, " WLAN            : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.WLAN));
    DEBUG ((DEBUG_INFO, " TbtDock         : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.TbtDock));
    DEBUG ((DEBUG_INFO, " Upid            : 0x%x\n", MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Upid));
  }

  if (MbpHob->MeBiosPayload.FwFeaturesState.Available == TRUE) {
    DEBUG ((DEBUG_INFO, "MeBiosPayload FwFeaturesState ---\n"));
    DEBUG ((DEBUG_INFO, " FullMng         : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.FullMng));
    DEBUG ((DEBUG_INFO, " StdMng          : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.StdMng));
    DEBUG ((DEBUG_INFO, " Amt             : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Amt));
    DEBUG ((DEBUG_INFO, " OCR             : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.OCR));
    DEBUG ((DEBUG_INFO, " RPE             : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.RPE));
    DEBUG ((DEBUG_INFO, " PSR             : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.PSR));
    DEBUG ((DEBUG_INFO, " ISH             : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.ISH));
    DEBUG ((DEBUG_INFO, " IPV6            : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.IPV6));
    DEBUG ((DEBUG_INFO, " KVM             : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.KVM));
    DEBUG ((DEBUG_INFO, " TLS             : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.TLS));
    DEBUG ((DEBUG_INFO, " WLAN            : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.WLAN));
    DEBUG ((DEBUG_INFO, " TbtDock         : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.TbtDock));
    DEBUG ((DEBUG_INFO, " Upid            : 0x%x\n", MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Upid));
  }

  if (MbpHob->MeBiosPayload.FwPlatType.Available == TRUE) {
    DEBUG ((DEBUG_INFO, "MeBiosPayload ME Platform Type ---\n"));
    DEBUG ((DEBUG_INFO, " PlatformTargetUsageType : 0x%x\n", MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.PlatformTargetUsageType));
    DEBUG ((DEBUG_INFO, " SuperSku                : 0x%x\n", MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.SuperSku));
    DEBUG ((DEBUG_INFO, " IntelMeFwImageType      : 0x%x\n", MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType));
    DEBUG ((DEBUG_INFO, " PlatformBrand           : 0x%x\n", MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.PlatformBrand));
  }

  if (MbpHob->MeBiosPayload.HwaRequest.Available == TRUE) {
    DEBUG ((DEBUG_INFO, "MeBiosPayload HwaRequest ---\n"));
    DEBUG ((DEBUG_INFO, " MediaTablePush : 0x%x\n", MbpHob->MeBiosPayload.HwaRequest.Data.Fields.MediaTablePush));
  }

  if (MbpHob->MeBiosPayload.UnconfigOnRtcClearState.Available == TRUE) {
    DEBUG ((DEBUG_INFO, "MeBiosPayload UnconfigOnRtcClearState ---\n"));
    DEBUG ((DEBUG_INFO, " UnconfigOnRtcClearData : 0x%x\n", MbpHob->MeBiosPayload.UnconfigOnRtcClearState.UnconfigOnRtcClearData.DisUnconfigOnRtcClearState));
  }

  if (MbpHob->MeBiosPayload.ArbSvnState.Available == TRUE) {
    DEBUG ((DEBUG_INFO, "MeBiosPayload ArbSvnState ---\n"));
    DEBUG ((DEBUG_INFO, " Flags         : 0x%x\n", MbpHob->MeBiosPayload.ArbSvnState.ArbSvnData.Flags));
    DEBUG ((DEBUG_INFO, " MinCseArbSvn  : 0x%x\n", MbpHob->MeBiosPayload.ArbSvnState.ArbSvnData.MinCseArbSvn));
    DEBUG ((DEBUG_INFO, " CurrCseArbSvn : 0x%x\n", MbpHob->MeBiosPayload.ArbSvnState.ArbSvnData.CurrCseArbSvn));
  }

  if (MbpHob->MeBiosPayload.OemSphyData.Available == TRUE) {
    DEBUG ((DEBUG_INFO, "MeBiosPayload OemSphyData ---\n"));
    DEBUG ((DEBUG_INFO, " OemSphyData : 0x%lx\n", MbpHob->MeBiosPayload.OemSphyData.Data));
  }

  if (MbpHob->MeBiosPayload.OemNphyData.Available == TRUE) {
    DEBUG ((DEBUG_INFO, "MeBiosPayload OemNphyData ---\n"));
    DEBUG ((DEBUG_INFO, " OemNphyData : 0x%lx\n", MbpHob->MeBiosPayload.OemNphyData.Data));
  }

  if (MbpHob->MeBiosPayload.MeasuredBootSupport.Available == TRUE) {
    DEBUG ((DEBUG_INFO, "MeBiosPayload MeasuredBootSupport ---\n"));
    DEBUG ((DEBUG_INFO, " MeasuredBoot : 0x%x\n", MbpHob->MeBiosPayload.MeasuredBootSupport.MeasuredBootData.MeasuredBoot));
  }

  if (MbpHob->MeBiosPayload.Psr.Available == TRUE) {
    DEBUG ((DEBUG_INFO, "MeBiosPayload Platform Service Record ---\n"));
    DEBUG ((DEBUG_INFO, " ChassisIntrusionEvent : 0x%x\n", MbpHob->MeBiosPayload.Psr.PsrData.ChassisIntrusionEvent));
    DEBUG ((DEBUG_INFO, " PsrLogState           : 0x%x\n", MbpHob->MeBiosPayload.Psr.PsrData.PsrLogState));
    DEBUG ((DEBUG_INFO, " PsrCapabilities       : 0x%x\n", MbpHob->MeBiosPayload.Psr.PsrCapabilities));
  }

  DEBUG ((DEBUG_INFO, "\n------------------- MeBiosPayload Data HOB Print End -------------------\n"));
}

