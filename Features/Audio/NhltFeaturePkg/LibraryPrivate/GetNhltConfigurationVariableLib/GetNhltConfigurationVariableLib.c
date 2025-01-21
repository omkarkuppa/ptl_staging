/** @file
  Library for getting NHLT configuration

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#include <Pi/PiFirmwareFile.h>
#include <Library/DxeServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/InstallBtConfigurationVariable.h>
#include <NhltConfiguration.h>
#include <NhltEndpointsConfigurationVariable.h>
#include <Nhlt.h>

#if FixedPcdGetBool (NhltConfigurationByPcdEnabled)
/**
  Function sets NHLT endpoint table based on the PCDs configuration values

  @param[in][out]  *NhltConfigurationVariable      Pointer to Nhlt configuration variable
**/
VOID
NhltEndpointTableLoadPcdConfiguration (
  IN OUT NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE  *NhltConfigurationVariable
  )
{
  NhltConfigurationVariable->NhltDmicStereoVpEnabled = PcdGetBool (NhltDmicStereoVpEnabled);
  NhltConfigurationVariable->NhltDmicMonoEnabled     = PcdGet8 (NhltDmicMonoEnabled);
  NhltConfigurationVariable->NhltDmicStereoEnabled   = PcdGet8 (NhltDmicStereoEnabled);
  NhltConfigurationVariable->NhltDmicQuadEnabled     = PcdGet8 (NhltDmicQuadEnabled);
  NhltConfigurationVariable->NhltI2sAlc274Enabled    = PcdGet8 (NhltI2sAlc274Enabled);
  NhltConfigurationVariable->NhltBluetoothEnabled    = PcdGet8 (NhltBluetoothEnabled);
}
#endif

/**
  Function provide Nhlt configuration based on user input stored in either Efi Variable or PCDs

  @param[in][out] *NhltConfiguration   Pointer to Nhlt configuration

  @retval         EFI_SUCCESS          The function completed successfully
  @retval         EFI_NOT_FOUND        NHLT user configuration is corrupted
**/
EFI_STATUS
GetNhltConfiguration (
  IN OUT NHLT_CONFIGURATION  *NhltConfiguration
  )
{
  UINTN                                        VariableSize;
  NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE  NhltConfigurationVariable;
  UINT32                                       NhltConfigurationVariableAttr;
  EFI_STATUS                                   Status;

  ZeroMem (NhltConfiguration, sizeof (NHLT_CONFIGURATION));
  ZeroMem (&NhltConfigurationVariable, sizeof (NhltConfigurationVariable));

#if FixedPcdGetBool (NhltConfigurationByPcdEnabled) == 1
  NhltEndpointTableLoadPcdConfiguration (&NhltConfigurationVariable);
  DEBUG ((DEBUG_ERROR, "%a (): Nhlt Endpoint Table configured based on the PCDs.\n", __FUNCTION__));
#else
  VariableSize = sizeof (NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE);
  Status = gRT->GetVariable (
                  NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_NAME,
                  &gNhltEndpointsTableConfigurationVariableGuid,
                  &NhltConfigurationVariableAttr,
                  &VariableSize,
                  &NhltConfigurationVariable
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a (): Failed to get NHLT configuration variable - End.\n", __FUNCTION__));
    return Status;
  }

  DEBUG ((DEBUG_ERROR, "%a (): Nhlt Endpoint Table configured based on the EFI variable.\n", __FUNCTION__));
#endif

  switch (NhltConfigurationVariable.NhltDmicMonoEnabled) {
    case NhltDmicClock38_4MHz_Format24b_48kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX1Format24Bit48kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Mono 38.4MHz 48kHz 24bit enabled.\n"));
      break;
    case NhltDmicClock38_4MHz_Format24b_96kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX1Format24Bit96kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Mono 38.4MHz 96kHz 24bit enabled.\n"));
      break;
    case NhltDmicDisabled:
    case NhltDmicFormatInvalid:
    default:
      break;
  }

  switch (NhltConfigurationVariable.NhltDmicStereoEnabled) {
    case NhltDmicClock38_4MHz_Format24b_48kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX2Format24Bit48kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Stereo 38.4MHz 48kHz 24bit enabled.\n"));
      break;
    case NhltDmicClock38_4MHz_Format24b_96kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX2Format24Bit96kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Stereo 38.4MHz 96kHz 24bit enabled.\n"));
      break;
    case NhltDmicDisabled:
    case NhltDmicFormatInvalid:
    default:
      break;
  }

  switch (NhltConfigurationVariable.NhltDmicQuadEnabled) {
    case NhltDmicClock38_4MHz_Format24b_48kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX4Format24Bit48kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Quad 38.4MHz 48kHz 24bit enabled.\n"));
      break;
    case NhltDmicClock38_4MHz_Format24b_96kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX4Format24Bit96kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Quad 38.4MHz 96kHz 24bit enabled.\n"));
      break;
    case NhltDmicDisabled:
    case NhltDmicFormatInvalid:
    default:
      break;
  }

  if (NhltConfigurationVariable.NhltDmicStereoVpEnabled) {
    NhltConfiguration->NhltConfigurationEnabled[NhltDmicX2Format24Bit48kHzFreq19_2MHzVP] = TRUE;
    DEBUG ((DEBUG_INFO, "Nhlt for Dmic Stereo VP 19.2MHz 48kHz 24bit enabled.\n"));
  }

  switch (NhltConfigurationVariable.NhltI2sAlc274Enabled) {
    case NhltI2sClock38_4MHz_Format24b_48kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltI2sAlc274Format24Bit48kHzFreq38_4MHz] = TRUE;
      DEBUG((DEBUG_INFO, "Nhlt for I2s Alc274 38.4MHz 48kHz 24bit enabled.\n"));
      break;
    case NhltI2sClock38_4MHz_Format24b_96kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltI2sAlc274Format24Bit96kHzFreq38_4MHz] = TRUE;
      DEBUG((DEBUG_INFO, "Nhlt for I2s Alc274 38.4MHz 96kHz 24bit enabled.\n"));
      break;
    case NhltI2sDisabled:
    case NhltI2sFormatInvalid:
    default:
      break;
  }

  switch (NhltConfigurationVariable.NhltBluetoothEnabled) {
    case NhltBtClockDrivenByBtInHfp:
      NhltConfiguration->NhltConfigurationEnabled[NhltBtFreq38_4MHzBtDrivesI2sClockHfp] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Bluetooth 38.4MHz, where BT drives I2S clock for HFP is enabled.\n"));
      break;
    case NhltBtClockDrivenByDspInHfp:
      NhltConfiguration->NhltConfigurationEnabled[NhltBtFreq38_4MHzDspDrivesI2sClockHfp] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Bluetooth 38.4MHz, where DSP drives I2S clock for HFP is enabled.\n"));
      break;
    case NhltBtDisabled:
    case NhltBtFormatInvalid:
    default:
      break;
  }

#if FixedPcdGetBool (NhltConfigurationByPcdEnabled) == 0
  if (NhltConfigurationVariable.Revision != NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_REVISION) {
    NhltConfigurationVariable.Revision = NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_REVISION;
    Status = gRT->SetVariable (
                    NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_NAME,
                    &gNhltEndpointsTableConfigurationVariableGuid,
                    NhltConfigurationVariableAttr,
                    VariableSize,
                    &NhltConfigurationVariable
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a (): Cannot update NHLT UEFI variable. Status = 0x%r.\n", __FUNCTION__, Status));
      return Status;
    }
  }
#endif

  Status = InstallNhltBtVariable (NhltConfigurationVariable.NhltBluetoothEnabled);
  if (EFI_ERROR (Status)) {
    return Status;
  }


  return EFI_SUCCESS;
}
