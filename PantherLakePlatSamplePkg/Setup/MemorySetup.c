/** @file
  This file implements a Setup callback function to update Memory configuration

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <OverClockSetup.h>
#include <Protocol/SmbusHc.h>
#include <Library/HiiLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Register/GenerationMsr.h>
//#include <Protocol/MemoryAddressEncodeDecode.h>

typedef struct {
  UINT32  Rsvd;
} DRAM_ADDRESS;

EFI_STATUS
EFIAPI
DecodePhysicalAddress(
  IN  UINT64  PprRepairPhysicalAddress,
  OUT DRAM_ADDRESS  *DramAddress
  )
{
  /*
  MEMORY_ADDRESS_ENCODE_DECODE_PROTOCOL *MemEncodeDecodeProtocol;
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gMemoryAddressEncodeDecodeProtocolGuid, NULL, (VOID **) &MemEncodeDecodeProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Can't find Address Decoder protocol\n"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  Status = MemEncodeDecodeProtocol->MemoryAddressDecode (PprRepairPhysicalAddress, DramAddress);
  */

  return EFI_SUCCESS;
}

/**
  This function converts memory Configuration parameter values

  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Action          Specifies the type of action taken by the browser.
  @param QuestionId      A unique value which is sent to the original exporting driver
                         so that it can identify the type of data to expect.
  @param Type            The type of value for the question.
  @param Value           A pointer to the data being sent to the original exporting driver.
  @param ActionRequest   On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS - The function completed successfully
  @retval EFI_UNSUPPORTED - The Setup Menu was not updated
  @retval EFI_NOT_FOUND - There is an error in Locating protocols or retrieving browser data
**/
EFI_STATUS
EFIAPI
FormMemoryPPRCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  EFI_STATUS                                Status;
  SA_SETUP                                  SaSetup;
  UINTN                                     VariableSize;
  EFI_STRING                                RequestString;
  DRAM_ADDRESS                              DramAddress;

  DEBUG ((DEBUG_INFO, "FormMemoryPPRCallBackFunction\n"));

  RequestString = NULL;

  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  VariableSize = sizeof (SA_SETUP);
  if (!HiiGetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VariableSize, (UINT8 *) &SaSetup)) {
    DEBUG ((DEBUG_INFO, "Can't find SaSetup\n"));
    return EFI_NOT_FOUND;
  }

  if (SaSetup.PprRepairPhysicalAddress == 0) {
    return EFI_SUCCESS;
  }

  Status = DecodePhysicalAddress(SaSetup.PprRepairPhysicalAddress, &DramAddress);
  if (EFI_ERROR (Status)) {
    // Set Controller to a bad value in case PPR forces to repair it.
    SaSetup.PprRepairController = 0xFF;
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, PprRepairController), sizeof (SaSetup.PprRepairController));
    goto Done;
  }

  /*
  DEBUG ((DEBUG_INFO, "Decode PPR address 0x%llx: Controller:%d Channel:%d Dimm:%d Rank:%d BG:%d Bank:%d Row:0x%X\n", SaSetup.PprRepairPhysicalAddress, DramAddress.Controller, DramAddress.ChannelNumber, DramAddress.DimmNumber, DramAddress.Rank, DramAddress.BankGroup, DramAddress.Bank, DramAddress.Ras));
  SaSetup.PprRepairController = (UINT8) DramAddress.Controller;
  SaSetup.PprRepairChannel = (UINT8) DramAddress.ChannelNumber;
  SaSetup.PprRepairDimm = (UINT8) DramAddress.DimmNumber;
  SaSetup.PprRepairRank = (UINT8) DramAddress.Rank;
  SaSetup.PprRepairBankGroup = (UINT8) DramAddress.BankGroup;
  SaSetup.PprRepairBank = (UINT8) DramAddress.Bank;
  SaSetup.PprRepairRow = DramAddress.Ras;
  */

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, PprRepairController), sizeof (SaSetup.PprRepairController));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, PprRepairChannel), sizeof (SaSetup.PprRepairChannel));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, PprRepairDimm), sizeof (SaSetup.PprRepairDimm));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, PprRepairRank), sizeof (SaSetup.PprRepairRank));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, PprRepairBankGroup), sizeof (SaSetup.PprRepairBankGroup));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, PprRepairBank), sizeof (SaSetup.PprRepairBank));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, PprRepairRow), sizeof (SaSetup.PprRepairRow));

Done:
  if (RequestString != NULL) {
    if (!HiiSetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VariableSize, (UINT8 *) &SaSetup, RequestString)) {
      return EFI_NOT_FOUND;
    }
    FreePool (RequestString);
  }

  return Status;
}
