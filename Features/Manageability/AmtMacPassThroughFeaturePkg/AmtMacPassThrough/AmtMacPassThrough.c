/**@file
  Send AMT MAC PassThrough Command.

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

#include <MeBiosPayloadHob.h>
#include <Uefi.h>
#include <AmtMacPassThroughConfigVariable.h>
#include <Protocol/Spi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  Get mac address from Gbe region

  @param[out]   MacBuffer    Buffer for the MAC, if return error, buffers value won't be change.

  @retval       EFI_SUCCESS   successful
  @retval       Other Vals    something wrong

**/
EFI_STATUS
AmtGetGbeMac (
  OUT UINT8        *MacBuffer
  )
{
  EFI_STATUS       Status;
  PCH_SPI_PROTOCOL *SpiProtocol;
  UINT32           *TmpPtr;
  UINT8            TempBuffer[MAC_ADDRESS_SIZE];
  UINT8            Index;

  if (MacBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Locate the SPI protocol.
  //
  Status = gBS->LocateProtocol (
                  &gPchSpiProtocolGuid,
                  NULL,
                  (VOID **) &SpiProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Read MAC address from GbE region
  //
  Status = SpiProtocol->FlashRead (
                          SpiProtocol,
                          FlashRegionGbE,
                          (UINT32)0,
                          (UINT32)MAC_ADDRESS_SIZE,
                          TempBuffer
                          );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TmpPtr = (UINT32 *) TempBuffer;
  if (((*TmpPtr & 0x00FFFFFF) == 0x00888888) || ((*TmpPtr & 0x00FFFFFF) == 0x00000000) || ((*TmpPtr & 0x00FFFFFF) == 0x00FFFFFF)) {
    // Invalid according to OUI/MA-L list from IEEE
    DEBUG ((DEBUG_INFO, "AMT GbE MAC: first 24 bits are invalid - %06x\n", (*TmpPtr & 0x00FFFFFF)));
    return EFI_NOT_FOUND;
  }
  for (Index = 0; Index < MAC_ADDRESS_SIZE; Index++) {
    MacBuffer[Index] = TempBuffer[Index];
  }

  return Status;
}

/**
  Checks if the AMT MAC Pass-Through enforcement is requested.

  This function retrieves the ME BIOS Payload (MBP) data
  and checks if Media Table Push is requested by CSME.

  @retval TRUE   AMT MacPassThrough command shall be sent.
  @retval FALSE  There is no need for sending AMT MacPassThrough command.
**/
BOOLEAN
IsAmtMacPassThroughEnforceRequested (
  VOID
  )
{
  ME_BIOS_PAYLOAD_HOB     *MbpHob;

  DEBUG ((DEBUG_INFO, "%a() enter\n", __FUNCTION__));
  MbpHob = NULL;

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_WARN, "HwAsset: No MBP Data HOB available\n"));
    return FALSE;
  }

  return MbpHob->MeBiosPayload.HwaRequest.Available && (BOOLEAN) MbpHob->MeBiosPayload.HwaRequest.Data.Fields.MediaTablePush;
}

/**
  This event will send MAC address to CSME

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
AmtSendMacAddress (
  IN EFI_EVENT               Event,
  IN VOID                    *Context
  )
{
  EFI_STATUS                 Status;
  UINTN                      DataSize;
  AMT_MAC_PASSTHROUGH_CONFIG MacConfig;
  UINT32                     Attributes;
  //
  // Initialization MAC default address.
  //
  UINT8                      MacBuffer[MAC_ADDRESS_SIZE] = {0x88, 0x88, 0x88, 0x88, 0x87, 0x88};

  DataSize = sizeof (AMT_MAC_PASSTHROUGH_CONFIG);
  ZeroMem (&MacConfig, sizeof (AMT_MAC_PASSTHROUGH_CONFIG));
  Status = gRT->GetVariable (AMT_MAC_PASSTHROUGH_CONFIG_NAME, &gAmtMacPassThroughConfigGuid, &Attributes, &DataSize, &MacConfig);
  if (EFI_ERROR (Status)) {
    ZeroMem (&MacConfig, sizeof (AMT_MAC_PASSTHROUGH_CONFIG));
    Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
    Status = gRT->SetVariable (
                    AMT_MAC_PASSTHROUGH_CONFIG_NAME,
                    &gAmtMacPassThroughConfigGuid,
                    Attributes,
                    DataSize,
                    (VOID *) &MacConfig
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "AMT: AmtMacPassThroughConfig failed.\n"));
    }
  }

  Status = AmtGetGbeMac (MacBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AMT: Can not read legal value for GBE, use default value.\n"));
  }

  //
  // Send the command when:
  //   - feature state changes
  //   - feature is enabled and MAC changes
  //   - CSME requests HWA tables
  //
  if (MacConfig.AmtMacPassThroughState != MacConfig.AmtMacPassThroughPreviousBootState ||
     (MacConfig.AmtMacPassThroughState == 1 && CompareMem (MacConfig.MacBuffer, MacBuffer, sizeof (MacBuffer)) != 0) ||
     IsAmtMacPassThroughEnforceRequested ()) {
    //
    // Send command only if the required change is different from the stored values.
    //
    Status = AmtSetMacPassthrough (MacConfig.AmtMacPassThroughState, MacBuffer);
    if (!EFI_ERROR (Status)) {
      MacConfig.AmtMacPassThroughPreviousBootState = MacConfig.AmtMacPassThroughState;
      CopyMem (MacConfig.MacBuffer, MacBuffer, sizeof (MacBuffer));
      DataSize = sizeof (AMT_MAC_PASSTHROUGH_CONFIG);
      Status = gRT->SetVariable (
                      AMT_MAC_PASSTHROUGH_CONFIG_NAME,
                      &gAmtMacPassThroughConfigGuid,
                      Attributes,
                      DataSize,
                      (VOID *) &MacConfig
                      );
      ASSERT_EFI_ERROR (Status);
    }
  }
  gBS->CloseEvent (Event);
}

/**
  Checks if AMT MAC PassThrough features is supported.

  @retval TRUE        Feature is supported
  @retval FALSE       Feature is not supported
**/
STATIC
BOOLEAN
IsAmtMacPassThroughSupported (
  VOID
  )
{
  ME_BIOS_PAYLOAD_HOB *MbpHob;

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    return FALSE;
  }

  if (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType != IntelMeCorporateFw ||
      MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Amt != 1 ||
      MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt != 1 ) {
    return FALSE;
  }

  return TRUE;
}

/**
  Entry point for the AMT MAC PassThrough Driver.
  Sends AMT Set MAC PassThrough command when Ready To Boot event is signaled.

  @param[in] ImageHandle   - Pointer to the loaded image protocol for this driver
  @param[in] SystemTable   - Pointer to the EFI System Table

  @retval EFI_SUCCESS      - Event was created.
  @retval Others           - Event was not created.
**/
EFI_STATUS
EFIAPI
AmtMacPassThroughEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_EVENT           Event;
  EFI_STATUS          Status;

  if (!IsAmtMacPassThroughSupported ()) {
    DEBUG ((DEBUG_WARN, "AmtMacPassThrough not supported on this configuration\n"));
    return EFI_UNSUPPORTED;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AmtSendMacAddress,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

