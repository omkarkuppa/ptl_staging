/** @file
  Overclocking library implementation.

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
#include <Library/OcMailboxLib.h>
#include <Ppi/Wdt.h>
#include <Library/OcCommonLib.h>

/**
  Gets the Voltage and Frequency information for a given CPU domain

  @param[out] *VfSettings
  @param[out] *MailboxStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
GetVoltageFrequencyItem (
  OUT VOLTAGE_FREQUENCY_ITEM *VfSettings,
  OUT UINT32                 *MailboxStatus
  )
{
  EFI_STATUS      Status;
  UINT32          CommandId;
  UINT16          TempVoltageTarget;
  INT16           TempVoltageOffset;
  OC_MAILBOX_FULL VfMsg;

  Status = EFI_SUCCESS;

  ZeroMem (&VfMsg, sizeof (VfMsg));
  ///
  /// Convert v/f command to Mailbox command format
  ///
  CommandId = MAILBOX_OC_CMD_GET_VOLTAGE_FREQUENCY;

  Status = ConvertToMailboxFormat ((VOID *) VfSettings, &VfMsg, CommandId);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  ///  Read From the OC Library
  ///
  Status = OcMailboxRead (VfMsg.Interface.InterfaceData, &VfMsg.Data, MailboxStatus);

  ///
  ///  Copy mailbox data to VfSettings
  ///
  if ((Status == EFI_SUCCESS) && (*MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    VfSettings->VfSettings.MaxOcRatio = (UINT8) (VfMsg.Data & MAX_RATIO_MASK);
    VfSettings->VfSettings.VoltageTargetMode = (UINT8) ((VfMsg.Data & VOLTAGE_MODE_MASK) >> VOLTAGE_MODE_OFFSET);

    TempVoltageTarget = (UINT16) ((VfMsg.Data  & VOLTAGE_TARGET_MASK) >> VOLTAGE_TARGET_OFFSET);
    ConvertVoltageTarget (TempVoltageTarget, &VfSettings->VfSettings.VoltageTarget, CONVERT_TO_BINARY_MILLIVOLT);

    TempVoltageOffset = (INT16) ((VfMsg.Data  & VOLTAGE_OFFSET_MASK) >> VOLTAGE_OFFSET_OFFSET);
    ConvertVoltageOffset (TempVoltageOffset, &VfSettings->VfSettings.VoltageOffset, CONVERT_TO_BINARY_MILLIVOLT);
  }

  return Status;
}

/**
  Sets the Voltage and Frequency information for a given CPU domain

  @param[in]  *VfSettings
  @param[out] *MailboxStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
SetVoltageFrequencyItem (
  IN VOLTAGE_FREQUENCY_ITEM VfSettings,
  OUT UINT32                *MailboxStatus
  )
{
  EFI_STATUS      Status;
  UINT32          CommandId;
  OC_MAILBOX_FULL VfMsg;

  Status = EFI_SUCCESS;

  ///
  /// Convert v/f Commands to Mailbox command format
  ///
  CommandId = MAILBOX_OC_CMD_SET_VOLTAGE_FREQUENCY;
  Status = ConvertToMailboxFormat ((VOID *)&VfSettings, &VfMsg, CommandId);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Write the v/f Settings to the OC Mailbox
  ///
  Status = OcMailboxWrite (VfMsg.Interface.InterfaceData, VfMsg.Data, MailboxStatus);

  return Status;
}

/**
  Update existing OC Voltage & Frequency item to the requested OC configuration.

  @param[in]  CurrentVfItem      The current domain VfSettings.
  @param[in]  RequestedVfItem    The requested domain VfSettings.
  @param[in]  WdtPei             Provide watchdog timer for OC changes.

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
  UINT32                  MailboxStatus;
  EFI_STATUS              Status;

  Status = EFI_SUCCESS;

  if (CurrentVfItem->DomainId != RequestedVfItem->DomainId || CurrentVfItem->VfSettings.VfPointIndex != RequestedVfItem->VfSettings.VfPointIndex) {
    return EFI_INVALID_PARAMETER;
  }

  if (CompareMem ((VOID *)RequestedVfItem, (VOID *)CurrentVfItem, sizeof (VOLTAGE_FREQUENCY_ITEM))) {
    if (WdtPei != NULL) {
      ///
      /// Arm watchdog timer for OC changes
      ///
      Status = WdtPei->ReloadAndStart (WDT_TIMEOUT_BETWEEN_PEI_DXE);

      ///
      /// Need to update the requested voltage/frequency values
      ///
      DEBUG ((DEBUG_INFO, "(OC) Update DomainId                              = %d\n", RequestedVfItem->DomainId));
      DEBUG ((DEBUG_INFO, "(OC) Update VfPointIndex                          = %d\n", RequestedVfItem->VfSettings.VfPointIndex));

      DEBUG ((DEBUG_INFO, "(OC) RequestedVfItem.VfSettings.MaxOcRatio        = %d\n", RequestedVfItem->VfSettings.MaxOcRatio));
      DEBUG ((DEBUG_INFO, "(OC) RequestedVfItem.VfSettings.VoltageTargetMode = %d\n", RequestedVfItem->VfSettings.VoltageTargetMode));
      DEBUG ((DEBUG_INFO, "(OC) RequestedVfItem.VfSettings.VoltageTarget     = %d\n", RequestedVfItem->VfSettings.VoltageTarget));
      DEBUG ((DEBUG_INFO, "(OC) RequestedVfItem.VfSettings.VoltageOffset     = %d\n", RequestedVfItem->VfSettings.VoltageOffset));
      DEBUG ((DEBUG_INFO, "(OC) CurrentVfItem.VfSettings.MaxOcRatio          = %d\n", CurrentVfItem->VfSettings.MaxOcRatio));
      DEBUG ((DEBUG_INFO, "(OC) CurrentVfItem.VfSettings.VoltageTargetMode   = %d\n", CurrentVfItem->VfSettings.VoltageTargetMode));
      DEBUG ((DEBUG_INFO, "(OC) CurrentVfItem.VfSettings.VoltageTarget       = %d\n", CurrentVfItem->VfSettings.VoltageTarget));
      DEBUG ((DEBUG_INFO, "(OC) CurrentVfItem.VfSettings.VoltageOffset       = %d\n", CurrentVfItem->VfSettings.VoltageOffset));
    }

    Status = SetVoltageFrequencyItem (*RequestedVfItem, &MailboxStatus);
    if (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS) {
      Status = EFI_DEVICE_ERROR;
    }
  }

  return Status;
}


/**
  Get the overclocking capabilities for a given CPU Domain

  @param[out] *OcCapabilities
  @param[out] *MailboxStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
GetOcCapabilities (
  OUT OC_CAPABILITIES_ITEM *OcCapabilities,
  IN  UINT8                DomainId,
  OUT UINT32               *MailboxStatus
  )
{
  EFI_STATUS               Status;
  OC_MAILBOX_INTERFACE     OcMailboxReadCommand;

  Status = EFI_SUCCESS;

  ///
  ///  Read From the OC Library
  ///
  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_OC_CAPABILITIES;
  OcMailboxReadCommand.Fields.Param_4_0 = DomainId;

  ZeroMem (OcCapabilities, sizeof (OC_CAPABILITIES_ITEM));
  Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &OcCapabilities->Data, MailboxStatus);
  ///
  ///  Copy mailbox data to OC Capabilities structure
  ///
  if ((Status == EFI_SUCCESS) && (*MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    DEBUG ((DEBUG_INFO, "(OC) For DomainId %d: OC Capabilities MaxOcRatioLimit is %d \n", DomainId, OcCapabilities->Fields.MaxOcRatioLimit));
    DEBUG ((DEBUG_INFO, "(OC) For DomainId %d: OC Capabilities RatioOcSupported is 0x%x \n", DomainId, OcCapabilities->Fields.RatioOcSupported));
    DEBUG ((DEBUG_INFO, "(OC) For DomainId %d: OC Capabilities VoltageOverridesSupported is 0x%x \n", DomainId, OcCapabilities->Fields.VoltageOverridesSupported));
    DEBUG ((DEBUG_INFO, "(OC) For DomainId %d: OC Capabilities VoltageOffsetSupported is 0x%x \n", DomainId, OcCapabilities->Fields.VoltageOffsetSupported));
  } else {
    DEBUG ((DEBUG_ERROR, "(OC) GetOcCapabilities message failed. EFI Status = %r, Mailbox Status = 0x%X, Domain = 0x%X\n", Status, MailboxStatus, DomainId));
  }
  return Status;
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
  BOOLEAN NumIsNegative;
  UINT32  Remainder;
  UINT64  Voltage;
  ///  Fixed point representation:
  ///
  ///  S11.0.10V format
  ///  | | | |
  ///  | | | v
  ///  | | v Exponent
  ///  | v Significand Size
  ///  v Size
  ///  Signed/Unsigned
  ///
  ///  Float Value = Significand x (Base ^ Exponent)
  ///  (Base ^ Exponent) = 2 ^ 10 = 1024
  ///
  *OutputVoltageOffset = 0;
  NumIsNegative = FALSE;
  Remainder = 0;

  if (InputVoltageOffset == 0) {
    *OutputVoltageOffset = 0;
    return;
  }

  if (ConversionType == CONVERT_TO_FIXED_POINT_VOLTS) {
    ///
    /// Input Voltage is in INT16 representation. Check if numenr is negative
    ///
    if ((InputVoltageOffset & INT16_SIGN_BIT_MASK) != 0) {
      NumIsNegative = TRUE;
      ///
      /// Need to 2's complement adjust to make this number positive for
      /// voltage calculation
      ///
      InputVoltageOffset = (~InputVoltageOffset+1) & (INT16_SIGN_BIT_MASK -1);
    }

    ///
    /// Clip the input Voltage Offset to 500mv
    ///
    if (InputVoltageOffset > MAX_OFFSET_MV) {
      InputVoltageOffset = MAX_OFFSET_MV;
    }

    ///
    /// Convert to fixed point representation
    ///
    Voltage = MultU64x32 (1024, InputVoltageOffset);
    *OutputVoltageOffset = (UINT16) DivU64x32Remainder (Voltage, MILLIVOLTS_PER_VOLT,&Remainder);

    if (Remainder >= 500) {
      *OutputVoltageOffset += 1;
    }

    if (NumIsNegative) {
      //
      // 2's complement back to a negative number
      //
      *OutputVoltageOffset = ~(*OutputVoltageOffset) + 1;
    }
  } else if (ConversionType == CONVERT_TO_BINARY_MILLIVOLT) {
    ///
    /// Input Voltage is in fixed point representation. Check if number negative
    ///
    if ((InputVoltageOffset & FIXED_POINT_SIGN_BIT_MASK) != 0) {
      NumIsNegative = TRUE;
      ///
      /// Need to 2's complement adjust to make this number positive for
      /// voltage calculation
      ///
      InputVoltageOffset = (~InputVoltageOffset+1) & (FIXED_POINT_SIGN_BIT_MASK -1);
    }

    ///
    ///  Convert to INT16 representation in millivolts
    ///
    Voltage = MultU64x32 (MILLIVOLTS_PER_VOLT, InputVoltageOffset);
    *OutputVoltageOffset = (UINT16) DivU64x32Remainder (Voltage, 1024,&Remainder);

    if (Remainder >= 500) {
      *OutputVoltageOffset += 1;
    }

    if (NumIsNegative) {
      //
      // 2's complement back to a negative number
      //
      *OutputVoltageOffset = ~(*OutputVoltageOffset) + 1;
    }
  }

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
  IN VOID                 *InputData,
  OUT OC_MAILBOX_FULL     *MailboxData,
  IN UINT32               CommandId
  )
{
  VOLTAGE_FREQUENCY_ITEM  *VfItem;
  VF_MAILBOX_COMMAND_DATA VfMailboxCommandData;
  UINT16                  TempVoltage;

  ///
  ///  Initialize local varaibles and mailbox data
  ///
  ZeroMem ((UINT32 *) MailboxData, sizeof (OC_MAILBOX_FULL));

  ///
  /// Then make a decision based on CommandId how to format
  ///
  switch (CommandId) {
    case MAILBOX_OC_CMD_GET_VOLTAGE_FREQUENCY:
      VfItem = (VOLTAGE_FREQUENCY_ITEM *) InputData;
      ///
      /// Voltage Frequency Settings are on a per domain basis
      ///
      MailboxData->Data = 0;
      MailboxData->Interface.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_VOLTAGE_FREQUENCY;
      MailboxData->Interface.Fields.Param_4_0 = VfItem->DomainId;
      break;

    case MAILBOX_OC_CMD_SET_VOLTAGE_FREQUENCY:
      VfItem = (VOLTAGE_FREQUENCY_ITEM *) InputData;
      ///
      /// Voltages are stored in a fixed point format
      ///
      VfMailboxCommandData.MaxOcRatio = VfItem->VfSettings.MaxOcRatio;

      TempVoltage = 0;
      ConvertVoltageTarget (VfItem->VfSettings.VoltageTarget, &TempVoltage, CONVERT_TO_FIXED_POINT_VOLTS);
      VfMailboxCommandData.VoltageTargetU12 = TempVoltage;

      VfMailboxCommandData.TargetMode = VfItem->VfSettings.VoltageTargetMode;

      TempVoltage = 0;
      ConvertVoltageOffset (VfItem->VfSettings.VoltageOffset, (INT16 *) &TempVoltage, CONVERT_TO_FIXED_POINT_VOLTS);
      VfMailboxCommandData.VoltageOffsetS11 = TempVoltage;

      //
      // When Param2 is non-zero for VF points voltage offset, then Data[7:0] has to clear to 0
      //
      if (VfItem->VfSettings.VfPointIndex > 0) {
        VfMailboxCommandData.MaxOcRatio = 0;
      }
      CopyMem (&MailboxData->Data, &VfMailboxCommandData, sizeof (VfMailboxCommandData));
      MailboxData->Interface.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_VOLTAGE_FREQUENCY;
      MailboxData->Interface.Fields.Param_4_0 = VfItem->DomainId;
      break;

    case MAILBOX_OC_CMD_GET_DDR_CAPABILITIES:
      MailboxData->Data = 0;
      MailboxData->Interface.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_DDR_CAPABILITIES;
      MailboxData->Interface.Fields.Param_4_0 = 0;
      break;

    default:
      return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}
