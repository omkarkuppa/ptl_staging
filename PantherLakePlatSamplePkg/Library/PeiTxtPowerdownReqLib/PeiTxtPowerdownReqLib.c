/** @file
  This file contains functions for creating TPM Event Log

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

@par Specification
**/

#include <PiPei.h>

#include <IndustryStandard/UefiTcgPlatform.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <Guid/TcgEventHob.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/HobLib.h>
#include <Library/BootGuardLib.h>
#include <Library/BaseMemoryLib.h>
#include <PchResetPlatformSpecific.h>

#include <TxtInfoHob.h>
#include <Library/Tpm2CommandLib.h>
#include <Library/BaseBpmAccessLib.h>

/**
  This function is called after PEI core discover memory and finish migration.
  Handle Memory Powerdown request by MRC or BIOS knob

  @param[in] PeiServices    Pointer to PEI Services Table.
  @param[in] NotifyDesc     Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in] InvokePpi      Pointer to the PPI data associated with this function.

  @retval EFI_STATUS        return EFI_SUCCESS, EFI_NOT_FOUND
**/
EFI_STATUS
EFIAPI
HandleTxtPowerDownRequest (
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDesc,
  IN  VOID                          *InvokePpi
  )
{
  EFI_STATUS                     Status      = EFI_SUCCESS;
  TXT_INFO_HOB                   *TxtInfoHob = NULL;
  FIRMWARE_INTERFACE_TABLE_ENTRY *FitEntry;
  BOOT_POLICY_MANIFEST_HEADER    *Bpm;
  PLATFORM_CONFIG_DATA_ELEMENT   *Pcde;
  POWER_DOWN_REQUEST_STRUCTURE   *Pdrs;
  TPM_NV_ADDRESS_STRUCTURE       *Tnvs;
  TPMI_RH_NV_INDEX                NvIndex;
  TPM2B_MAX_BUFFER                OutData;
  TPM2B_MAX_BUFFER                InData;

  DEBUG ((DEBUG_INFO, "HandleTxtPowerDownRequest Entry\n"));

  TxtInfoHob = GetFirstGuidHob (&gTxtInfoHobGuid);
  if (TxtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "TXTPEI::TxtInfoHob not found\n"));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "TXTPEI::TxtInfoHob->Data.TxtPowerdownRequest = %d\n", TxtInfoHob->Data.TxtPowerdownRequest));

  ZeroMem (&NvIndex, sizeof (NvIndex));
  ZeroMem (&OutData, sizeof (OutData));
  ZeroMem (&InData,  sizeof (InData));

  //
  // If no TXT power down request has been signaled, return.
  //
  if (TxtInfoHob->Data.TxtPowerdownRequest == FALSE) {
    return EFI_SUCCESS;
  }

  // Status initialized to error condition
  Status = EFI_NOT_FOUND;

  //
  // Locate POWER_DOWN_REQUEST_STRUCTURE
  //
  while (TRUE) {
    FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FindBpm ();
    if (FitEntry == NULL) {
      DEBUG ((DEBUG_ERROR, "Bpm FIT entry not found inside the FIT!\n"));
      break;
    }

    Bpm = (BOOT_POLICY_MANIFEST_HEADER *)(VOID *)(UINTN)FitEntry;
    if (*(UINT64*)Bpm->StructureId != BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_ID) {
      DEBUG ((DEBUG_ERROR, "PeiTxtPowerDownRequest: %a, Bpm not found\n", __FUNCTION__));
      break;
    }

    Pcde  = (PLATFORM_CONFIG_DATA_ELEMENT *)FindBpmPcde (Bpm);
    if (*(UINT64*)Pcde->StructureId!=BOOT_POLICY_MANIFEST_PLATFORM_CONFIG_DATA_ELEMENT_STRUCTURE_ID) {
      DEBUG ((DEBUG_ERROR, "PeiTxtPowerDownRequest: %a, Pcde not found in Bpm\n", __FUNCTION__));
      break;
    }

    Pdrs  = (POWER_DOWN_REQUEST_STRUCTURE *)(VOID *)(UINT8 *)&(Pcde->PdrsData);
    if (*(UINT64*)Pdrs->StructureId != BPM_PDRS_ELEMENT_STRUCTURE_ID) {
      DEBUG ((DEBUG_ERROR, "PeiTxtPowerDownRequest: %a, Pdrs not found in Bpm\n", __FUNCTION__));
      break;
    }

    // Structure located, update status to success.
    Status = EFI_SUCCESS;
    break;
  }

  //
  // Gracefully handle condition where POWER_DOWN_REQUEST_STRUCTURE is not found.
  //
  if (Status != EFI_SUCCESS) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Verify if Power down request media type is TPM
  //
  Tnvs = (TPM_NV_ADDRESS_STRUCTURE *)(VOID *)(UINT8 *)&(Pdrs->Data);

  if (Tnvs->MediaType == PDR_MEDIA_TPM_20) {
    //
    // Attempt to read NV Index
    //
    NvIndex = (TPMI_RH_NV_INDEX)(UINT32)Tnvs->NvIndex;
    Status = Tpm2NvRead (NvIndex, NvIndex, NULL, 1, Tnvs->ByteOffset, &OutData);
    if (Status == EFI_NOT_FOUND) {
      //
      // TPM NvIndex not found
      //
      DEBUG ((DEBUG_ERROR, "PeiTxtPowerDownRequest: %a, TpmNvRead - TPM NvIndex not found\n", __FUNCTION__));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PeiTxtPowerDownRequest: %a, TpmNvRead - TPM NvIndex device error: %r\n", __FUNCTION__, Status));
      return Status;
    }

    //
    // TPM NV Index found
    //
    DEBUG ((DEBUG_INFO, "PeiTxtPowerDownRequest: %a, TpmNvRead - TPM NvIndex read successful\n", __FUNCTION__));

    OutData.buffer[0] |= 0x02;  // update bits[2:0] in TPM
    InData.buffer[0]  = OutData.buffer[0];
    InData.size = 1;

    // Perform NV Write
    Status = Tpm2NvWrite (NvIndex, NvIndex, NULL, &InData, Tnvs->ByteOffset);
    if (Status == EFI_NOT_FOUND) {
      //
      // TPM NvIndex not found
      //
      DEBUG ((DEBUG_ERROR, "PeiTxtPowerDownRequest: %a, TpmNvWrite - TPM NvIndex not found\n", __FUNCTION__));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PeiTxtPowerDownRequest: %a, TpmNvWrite - TPM NvIndex device error: %r\n", __FUNCTION__, Status));
      return Status;
    }

    DEBUG ((DEBUG_INFO, "PeiTxtPowerDownRequest: %a, TpmNvRead - TPM NvIndex write successful\n", __FUNCTION__));
    // Clear the Trigger
    TxtInfoHob->Data.TxtPowerdownRequest = 0;
  } else {
    // Handle Non TPM use case here
  }

  //
  // Reset the platform
  //
  (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetCold, EFI_SUCCESS, 0, NULL);
  CpuDeadLoop ();

  return Status;
}

EFI_PEI_NOTIFY_DESCRIPTOR mHandleTxtPowerDownRequestNotifyList = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiMemoryDiscoveredPpiGuid,
  HandleTxtPowerDownRequest
};

/**
   The library constructor will register a callback to handle TXT Power down request

   @param  FileHandle   The handle of FFS header the loaded driver.
   @param  PeiServices  The pointer to the PEI services.

   @retval EFI_SUCCESS  The constructor always returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
PeiTxtPowerDownRequestConstructor (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                      Status;

  DEBUG ((DEBUG_INFO, "Registering PeiTxtPowerDownRequest Handler...\n"));
  PeiServicesNotifyPpi (&mHandleTxtPowerDownRequestNotifyList);

  Status = EFI_SUCCESS;

  return Status;
}
