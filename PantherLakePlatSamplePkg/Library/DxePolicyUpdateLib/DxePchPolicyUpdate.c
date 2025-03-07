/** @file
  This file is the library for PCH DXE Policy initialization.

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

#include <PiDxe.h>
#include <SetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PchInfoLib.h>
#include <Library/CnviLib.h>
#include <CnvVfrSetupMenuHii.h>
#include <Protocol/PchPolicy.h>
#include <HdAudioConfig.h>
#include <PchPcieRpConfig.h>

#define PCI_CLASS_NETWORK             0x02
#define PCI_CLASS_NETWORK_ETHERNET    0x00
#define PCI_CLASS_NETWORK_OTHER       0x80

GLOBAL_REMOVE_IF_UNREFERENCED PCIE_DEVICE_OVERRIDE mPcieDeviceUpdateTable[] = {
  //
  // Intel PRO/Wireless
  //
  { 0x8086, 0x422b, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x422c, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x4238, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x4239, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel WiMAX/WiFi Link
  //
  { 0x8086, 0x0082, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0085, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0083, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0084, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0086, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0087, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0088, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0089, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x008F, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0090, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel Crane Peak WLAN NIC
  //
  { 0x8086, 0x08AE, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x08AF, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel Crane Peak w/BT WLAN NIC
  //
  { 0x8086, 0x0896, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0897, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel Kelsey Peak WiFi, WiMax
  //
  { 0x8086, 0x0885, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0886, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel Centrino Wireless-N 105
  //
  { 0x8086, 0x0894, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0895, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel Centrino Wireless-N 135
  //
  { 0x8086, 0x0892, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0893, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel Centrino Wireless-N 2200
  //
  { 0x8086, 0x0890, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0891, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel Centrino Wireless-N 2230
  //
  { 0x8086, 0x0887, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x0888, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel Centrino Wireless-N 6235
  //
  { 0x8086, 0x088E, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x088F, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel CampPeak 2 Wifi
  //
  { 0x8086, 0x08B5, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  { 0x8086, 0x08B6, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },
  //
  // Intel WilkinsPeak 1 Wifi
  //
  { 0x8086, 0x08B3, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2AndL1SubstatesOverride, 0x0158, 0x0000000F, 0, 0, 0, 0, 0 },
  { 0x8086, 0x08B4, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2AndL1SubstatesOverride, 0x0158, 0x0000000F, 0, 0, 0, 0, 0 },
  //
  // Intel Wilkins Peak 2 Wifi
  //
  { 0x8086, 0x08B1, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2AndL1SubstatesOverride, 0x0158, 0x0000000F, 0, 0, 0, 0, 0 },
  { 0x8086, 0x08B2, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2AndL1SubstatesOverride, 0x0158, 0x0000000F, 0, 0, 0, 0, 0 },
  //
  // Intel Wilkins Peak PF Wifi
  //
  { 0x8086, 0x08B0, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0, 0, 0, 0, 0, 0 },

  //
  // End of Table
  //
  { 0 }
};

/**
  Update HD Audio DXE policies.

  @param[in] PchPolicy  Pointer to PCH_POLICY_PROTOCOL
  @param[in] PchSetup   Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateHdAudioDxePolicy (
  IN PCH_POLICY_PROTOCOL  *PchPolicy,
  IN PCH_SETUP            *PchSetup
  )
{
  EFI_STATUS              Status;
  UINT32                  Index;
  HDAUDIO_DXE_CONFIG      *HdAudioDxeConfig;
  CNV_VFR_CONFIG_SETUP    CnvSetup;
  UINTN                   VarDataSize;

  Status = GetConfigBlock ((VOID *)PchPolicy, &gHdAudioDxeConfigGuid, (VOID *)&HdAudioDxeConfig);
  ASSERT_EFI_ERROR (Status);

  VarDataSize = sizeof (CNV_VFR_CONFIG_SETUP);
  ZeroMem (&CnvSetup, VarDataSize);
#if FixedPcdGetBool (PcdCnvFeatureEnable) == 1
  Status = gRT->GetVariable (
                  L"CnvSetup",
                  &gCnvFeatureSetupGuid,
                  NULL,
                  &VarDataSize,
                  &CnvSetup
                  );
  if (EFI_ERROR (Status)) {
    return;
  }
#endif
  for (Index = 0; Index < PCH_MAX_HDA_SNDW_LINK_NUM; Index++) {
    HdAudioDxeConfig->SndwConfig[Index].AutonomousClockStop            = PchSetup->PchHdaAutonomousClockStopSndw[Index];
    HdAudioDxeConfig->SndwConfig[Index].DataOnActiveIntervalSelect     = PchSetup->PchHdaDataOnActiveIntervalSelectSndw[Index];
    HdAudioDxeConfig->SndwConfig[Index].DataOnActiveIntervalExtSelect  = PchSetup->PchHdaDataOnActiveIntervalExtSelectSndw[Index];
    HdAudioDxeConfig->SndwConfig[Index].DataOnActiveIntervalExt2Select = PchSetup->PchHdaDataOnActiveIntervalExt2SelectSndw[Index];
    HdAudioDxeConfig->SndwConfig[Index].DataOnDelaySelect              = PchSetup->PchHdaDataOnDelaySelectSndw[Index];
    HdAudioDxeConfig->SndwConfig[Index].DataOnDelayExtSelect           = PchSetup->PchHdaDataOnDelaySelectExtSndw[Index];
    HdAudioDxeConfig->SndwConfig[Index].DataOnDelayExt2Select          = PchSetup->PchHdaDataOnDelaySelectExt2Sndw[Index];
    HdAudioDxeConfig->SndwConfig[Index].ClockLoopbackDelaySelectSndw   = PchSetup->PchHdaClockLoopbackDelaySelectSndw[Index];
    HdAudioDxeConfig->SndwConfig[Index].ClockLoopbackEnableSndw        = PchSetup->PchHdaClockLoopbackEnableSndw[Index];
    HdAudioDxeConfig->SndwConfig[Index].ClockLoopbackSourceSndw        = PchSetup->PchHdaClockLoopbackSourceSndw[Index];
  }

  for (Index = 0; Index < HDAUDIO_FEATURES; Index++) {
    if (PchSetup->PchHdAudioFeature[Index]) {
      HdAudioDxeConfig->DspFeatureMask |= (1 << Index);
    } else {
      HdAudioDxeConfig->DspFeatureMask &= ~(1 << Index);
    }
  }

  // if BT is not present or disabled, disable DSP BT related features
  if ((!CnviCrfModuleIsPresent () || CnvSetup.CnviMode == 0) && !PchSetup->HdaDiscBtOffEnabled) {
    // BT Intel HFP SCO, A2DP, LE
    HdAudioDxeConfig->DspFeatureMask &= ~(BIT5 | BIT6 | BIT9);
  }

  // Discrete BT Offload Variables
  HdAudioDxeConfig->HdaDiscBtOffload.HdaDiscBtOffEnabled = PchSetup->HdaDiscBtOffEnabled;
  HdAudioDxeConfig->HdaDiscBtOffload.HdaDiscBtOffSspLink = PchSetup->HdaDiscBtOffSspLink;

}

/**
  Update PCIE Rp DXE policies

  @param[in] PchPolicy  Pointer to PCH_POLICY_PROTOCOL
**/
STATIC
VOID
UpdatePchPcieRpDxePolicy (
  IN PCH_POLICY_PROTOCOL  *PchPolicy
  )
{
  EFI_STATUS                 Status;
  PCIE_RP_DXE_CONFIG         *PchPcieRpDxeConfig;

  Status = GetConfigBlock ((VOID*) PchPolicy, &gPchPcieRpDxeConfigGuid, (VOID*) &PchPcieRpDxeConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PchPcieRpDxeConfig->PcieDeviceOverrideTablePtr = mPcieDeviceUpdateTable;
}

/**
  Get data for PCH policy from setup options.

  @param[in] PchPolicy                 The pointer to get PCH Policy protocol instance

  @retval EFI_SUCCESS                  Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxePchPolicy (
  IN OUT  PCH_POLICY_PROTOCOL    *PchPolicy
  )
{
  EFI_STATUS              Status;
  UINTN                   VariableSize;
  PCH_SETUP               PchSetup;

  VariableSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                   L"PchSetup",
                   &gPchSetupVariableGuid,
                   NULL,
                   &VariableSize,
                   &PchSetup
                   );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  UpdateHdAudioDxePolicy (PchPolicy, &PchSetup);
  UpdatePchPcieRpDxePolicy (PchPolicy);

  return EFI_SUCCESS;
}

/**
  Get data for PCH policy from setup options.

  @param[in] PchPolicy                 The pointer to get PCH Policy protocol instance

  @retval EFI_SUCCESS                  Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxeAttachedPchPolicy (
  IN OUT  PCH_POLICY_PROTOCOL    *PchPolicy
  )
{
  EFI_STATUS              Status;
  UINTN                   VariableSize;
  PCH_SETUP               PchSetup;

  VariableSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                   L"PchSetup",
                   &gPchSetupVariableGuid,
                   NULL,
                   &VariableSize,
                   &PchSetup
                   );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  UpdateHdAudioDxePolicy (PchPolicy, &PchSetup);
  UpdatePchPcieRpDxePolicy (PchPolicy);

  return EFI_SUCCESS;
}
