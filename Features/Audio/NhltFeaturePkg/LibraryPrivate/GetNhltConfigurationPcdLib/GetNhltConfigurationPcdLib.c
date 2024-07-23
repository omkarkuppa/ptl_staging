/** @file
  Library for getting NHLT configuration from PCD

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

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <NhltConfiguration.h>
#include <Nhlt.h>

/**
  Functions provide Nhlt configuration based on user input stored in Pcd.

  @param[in][out]     *NhltConfiguration      Pointer to Nhlt configuration.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
GetNhltConfiguration (
  IN OUT NHLT_CONFIGURATION  *NhltConfiguration
  )
{
  ZeroMem (NhltConfiguration, sizeof (NHLT_CONFIGURATION));

  switch (PcdGet8 (NhltDmicMonoEnabled)) {
    case NhltClock38_4MHz_Format24b_48kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX1Format24Bit48kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Mono 38.4MHz 48kHz 24bit enabled.\n"));
      break;
    case NhltClock38_4MHz_Format24b_96kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX1Format24Bit96kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Mono 38.4MHz 96kHz 24bit enabled.\n"));
      break;
    case NhltDisabled:
    case NhltFormatInvalid:
    default:
      break;
  }

  switch (PcdGet8 (NhltDmicStereoEnabled)) {
    case NhltClock38_4MHz_Format24b_48kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX2Format24Bit48kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Stereo 38.4MHz 48kHz 24bit enabled.\n"));
      break;
    case NhltClock38_4MHz_Format24b_96kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX2Format24Bit96kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Stereo 38.4MHz 96kHz 24bit enabled.\n"));
      break;
    case NhltDisabled:
    case NhltFormatInvalid:
    default:
      break;
  }

  switch (PcdGet8 (NhltDmicQuadEnabled)) {
    case NhltClock38_4MHz_Format24b_48kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX4Format24Bit48kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Quad 38.4MHz 48kHz 24bit enabled.\n"));
      break;
    case NhltClock38_4MHz_Format24b_96kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltDmicX4Format24Bit96kHzFreq38_4MHz] = TRUE;
      DEBUG ((DEBUG_INFO, "Nhlt for Dmic Quad 38.4MHz 96kHz 24bit enabled.\n"));
      break;
    case NhltDisabled:
    case NhltFormatInvalid:
    default:
      break;
  }

  if (PcdGetBool (NhltDmicStereoVpEnabled)) {
    NhltConfiguration->NhltConfigurationEnabled[NhltDmicX2Format24Bit48kHzFreq19_2MHzVP] = TRUE;
    DEBUG ((DEBUG_INFO, "Nhlt for Dmic Stereo VP 19.2MHz 48kHz 24bit enabled.\n"));
  }

  switch (PcdGet8 (NhltI2sAlc274Enabled)) {
    case NhltClock38_4MHz_Format24b_48kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltI2sAlc274Format24Bit48kHzFreq38_4MHz] = TRUE;
      DEBUG((DEBUG_INFO, "Nhlt for I2s Alc274 38.4MHz 48kHz 24bit enabled.\n"));
      break;
    case NhltClock38_4MHz_Format24b_96kHz:
      NhltConfiguration->NhltConfigurationEnabled[NhltI2sAlc274Format24Bit96kHzFreq38_4MHz] = TRUE;
      DEBUG((DEBUG_INFO, "Nhlt for I2s Alc274 38.4MHz 96kHz 24bit enabled.\n"));
      break;
    case NhltDisabled:
    case NhltFormatInvalid:
    default:
      break;
  }

  if (PcdGetBool (NhltBluetoothEnabled)) {
    NhltConfiguration->NhltConfigurationEnabled[NhltBtFreq38_4MHz] = TRUE;
    DEBUG ((DEBUG_INFO, "Nhlt for Bluetooth 38.4MHz enabled.\n"));
  }

  return EFI_SUCCESS;
}
