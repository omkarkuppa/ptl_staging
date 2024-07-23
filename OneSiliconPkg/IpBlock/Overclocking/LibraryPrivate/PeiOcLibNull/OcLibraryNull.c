/** @file
  Overclocking null library implementation.

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

#include <Library/PeiOcLib.h>

/**
  Gets the Voltage and Frequency information for a given CPU domain

  @param[out] *VfSettings
  @param[out] *LibStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
GetVoltageFrequencyItem (
  OUT VOLTAGE_FREQUENCY_ITEM *VfSettings,
  OUT UINT32                 *LibStatus
  )
{
  return EFI_SUCCESS;
}

/**
  Sets the Voltage and Frequency information for a given CPU domain

  @param[in]  *VfSettings
  @param[out] *LibStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
SetVoltageFrequencyItem (
  IN VOLTAGE_FREQUENCY_ITEM VfSettings,
  OUT UINT32                *LibStatus
  )
{
  return EFI_SUCCESS;
}

/**
  Update existing OC Voltage & Frequency item to the requested OC configuration.

  @param[in]  CurrentVfItem      The current domain VfSettings.
  @param[in]  RequestedVfItem    The requested domain VfSettings.
  @param[in]  WdtPei             watchdog timer for OC changes.

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
UpdateVoltageFrequencyItem (
  IN VOLTAGE_FREQUENCY_ITEM  *CurrentVfItem,
  IN VOLTAGE_FREQUENCY_ITEM  *RequestedVfItem,
  IN WDT_PPI                 *WdtPei           OPTIONAL
  )
{
  return EFI_SUCCESS;
}

/**
  Get the overclocking capabilities for a given CPU Domain

  @param[out] *OcCapabilities
  @param[out] *LibStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
GetOcCapabilities (
  OUT OC_CAPABILITIES_ITEM *OcCapabilities,
  IN  UINT8                DomainId,
  OUT UINT32               *LibStatus
  )
{
  return EFI_SUCCESS;
}

/**
  Converts the input votlage Offset to the fixed point S11.0.10 Volt format or
  to Binary millivolts representation based on the ConversionType.

  @param[in]  InputVoltageOffset
  @param[out] *OutputVoltageOffset
  @param[in]  ConversionType - 0:fixed point, 1:Signed Binary millivolts
**/
VOID
EFIAPI
ConvertVoltageOffset (
  IN INT16  InputVoltageOffset,
  OUT INT16 *OutputVoltageOffset,
  IN UINT8  ConversionType
  )
{
  return;
}


/**
  Converts the input data to valid mailbox command format based on CommandID

  @param[in]  InputData      Specific input data per command ID.
  @param[out] *MailboxData   Return the constructed mailbox data.
  @param[in]  CommandId      Command ID.

  @retval  EFI_UNSUPPORTED   Unsupported command ID.
  @retval  EFI_SUCCESS       Mailbox data is successfully constructed.
**/
EFI_STATUS
EFIAPI
ConvertToMailboxFormat (
  IN VOID             *InputData,
  OUT OC_MAILBOX_FULL *MailboxData,
  IN UINT32           CommandId
  )
{
  return EFI_SUCCESS;
}
