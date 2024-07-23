/** @file
  This files contains NULL PCH DMI services for RCs usage

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Library/PeiPchDmiLib.h>

enum PCH_PCIE_MAX_PAYLOAD {
  PchPcieMaxPayload128 = 0,
  PchPcieMaxPayload256,
  PchPcieMaxPayload512,
  PchPcieMaxPayload64 = 7,
  PchPcieMaxPayloadMax
};

/**
  Get PCH default TC VC Mapping settings. This funciton returns the default PCH setting
  System Agent can update the settings according to polices.

  @param[in, out] PchDmiTcVcMap         Buffer for PCH_DMI_TC_VC_MAP instance.

**/
VOID
PchDmiTcVcMapInit (
  IN OUT  PCH_DMI_TC_VC_MAP  *PchDmiTcVcMap
  )
{
  PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc0].Vc   = PchDmiVcTypeVc0;
  PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc0].TcId = PchDmiTcTypeTc0;

  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc0].Enable          = TRUE;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc0].VcId            = 0;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc1].Enable          = FALSE;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVcm].Enable          = FALSE;
}

/**
  This function reads Supported Link Speeds Vector from Link Capabilities 2 register.

  @return A bitmap of supported link speeds (gens) is returned.
**/
UINT8
PchDmiGetSupportedLinkSpeedsVector (
  VOID
  )
{
  //
  // Return GEN1 to avoid training to higher link speeds
  //
  return 0x1;
}

/**
  This function provides Max Payload Size Supported by PCH in DMI.

  @return A value encoded in PCIe Capabilities is returned, i.e. 1 - 128B, 2 - 256B, 3 - 512B, etc.
**/
UINT8
PchDmiGetMaxPayloadSizeSupported (
  VOID
  )
{
  return PchPcieMaxPayload256;
}