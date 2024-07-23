/** @file
  Populate the Dimm manifest

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PeiTdxLib.h>
#include <Library/TdxDataHob.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>

/**

  Gets DIMM information from registers and fills DIMM manifest for
  Alias Check Trusted Module (ACTM).

  @param[in] MrcData        - MRC Parameter Structure

  @retval EFI_SUCCESS if no error
  @retval EFI_ERROR if other operations fail
  @retval EFI_OUT_OF_RESOURCES if the HOB resource allocation fail

**/
EFI_STATUS
EFIAPI
PublishActmDimmManifest (
  IN MrcParameters   *const MrcData
  )
{
  ACTM_HEADER_DESCRIPTION Header = { ACTM_UUID, ACTM_EMPTY_SIZE, ACTM_VERSION, ACTM_RESERVED_SPACE };
  TDX_DATA_HOB            *TdxDataHob;
  EFI_STATUS              Status;

  DEBUG ((DEBUG_ERROR, "Inside %a %d  \n", __FUNCTION__, __LINE__));

  TdxDataHob = (TDX_DATA_HOB *) GetFirstGuidHob (&gTdxDataHobGuid);
  if (TdxDataHob == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "Assigning the header to Dimm manifest\n"));
  TdxDataHob->ActmDimmManifest.Header = Header;
  Status = ActmPopulateDimmManifest (MrcData, &TdxDataHob->ActmDimmManifest);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "[TDX] Failed to Populate the Dimm Manifest Status = %r\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "[TDX] Populating the Dimm Manifest is successful\n"));
  }

  if (TdxDataHob) {
    FreePool(TdxDataHob);
  }
  return Status;
}

/**
  Interrogates DIMM modules and store information from SPD to DIMM
  manifest for Alias Check Trusted Module (ACTM). The data retrieved
  from SPD represents the number of bits.

  @param[in] MrcData              - MRC Parameter Structure
  @param[out] ActmDimmManifest    - Pointer to header of DIMM manifest

  @retval EFI_SUCCESS if no error
**/
EFI_STATUS
EFIAPI
ActmPopulateDimmManifest (
  IN MrcParameters   *const MrcData,
  OUT ACTM_DIMM_MANIFEST     *ActmDimmManifest
 )
{
  MrcOutput               *Outputs;
  MrcControllerOut        *ControllerOut;
  MrcChannelOut           *ChannelOut;
  MrcDimmOut              *DimmOut;
  UINT8                   Controller;
  UINT8                   Channel;
  UINT8                   Dimm;
  UINT8                   Rank;
  UINT8                   Index = 0;
  DIMM_DESCRIPTION *      ActmDimmDescription = NULL;

  DEBUG ((DEBUG_INFO, "[TDX] Building dimm manifest..\n"));
  Outputs   = &MrcData->Outputs;

  ActmDimmDescription = (DIMM_DESCRIPTION *)ActmDimmManifest->ManifestBlob;

  // Save Static SPD DimmOut
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerOut = &Outputs->Controller[Controller];
    if (ControllerOut->Status != CONTROLLER_PRESENT)
      continue;
    for (Channel = 0; Channel < MAX_DDR5_CHANNEL; Channel++) {
      ChannelOut = &ControllerOut->Channel[Channel];
      if (ChannelOut->Status != CHANNEL_PRESENT)
        continue;
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmOut = &ChannelOut->Dimm[Dimm];
        if (DimmOut->Status != DIMM_PRESENT)
          continue;
        for (Rank = 0; Rank < DimmOut->RankInDimm; Rank++) {
          DEBUG ((DEBUG_INFO, "[TDX] ActmDimmDescription[%d] - MC = %d CH = %d Dimm = %d Rank = %d\n", Index, Controller, Channel, Dimm, Rank));
          ActmDimmDescription[Index].MemoryController   = Controller;
          ActmDimmDescription[Index].Channel            = Channel;
          ActmDimmDescription[Index].Dimm               = Dimm;
          ActmDimmDescription[Index].Rank               = Rank;
          ActmDimmDescription[Index].SubRankBits        = 0;                 // SubRankBits is Ignored
          ActmDimmDescription[Index].BankGroupBits      = BANK_GROUP_BITS;   // 2
          ActmDimmDescription[Index].BankAddressBits    = BANK_ADDRESS_BITS; // 2
          ActmDimmDescription[Index].ColumnBits         = COLUMN_BITS;       // 10
          ActmDimmDescription[Index].RowBits            = ROW_BITS;          // 16
          ActmDimmDescription[Index].ModeRegister58     = 0;                 // MR58 is Ignored
          ActmDimmDescription[Index].ModeRegister59     = 0;                 // MR59 is Ignored

          DEBUG ((DEBUG_INFO, "MemoryController         : %d\n", ActmDimmDescription[Index].MemoryController));
          DEBUG ((DEBUG_INFO, "Channel                  : %d\n", ActmDimmDescription[Index].Channel));
          DEBUG ((DEBUG_INFO, "Dimm                     : %d\n", ActmDimmDescription[Index].Dimm));
          DEBUG ((DEBUG_INFO, "Rank                     : %d\n", ActmDimmDescription[Index].Rank));
          DEBUG ((DEBUG_INFO, "SubRankBits              : %d\n", ActmDimmDescription[Index].SubRankBits));
          DEBUG ((DEBUG_INFO, "BankGroupBits            : %d\n", ActmDimmDescription[Index].BankGroupBits));
          DEBUG ((DEBUG_INFO, "BankAddressBits          : %d\n", ActmDimmDescription[Index].BankAddressBits));
          DEBUG ((DEBUG_INFO, "ColumnBits               : %d\n", ActmDimmDescription[Index].ColumnBits));
          DEBUG ((DEBUG_INFO, "RowBits                  : %d\n", ActmDimmDescription[Index].RowBits));
          DEBUG ((DEBUG_INFO, "ModeRegister58           : %d\n", ActmDimmDescription[Index].ModeRegister58));
          DEBUG ((DEBUG_INFO, "ModeRegister59           : %d\n", ActmDimmDescription[Index].ModeRegister59));
          Index++;
      } // for Rank
     } // for Dimm
    } // for Channel
  } // for Controller

  ActmDimmManifest->Header.Size = (Index * sizeof(DIMM_DESCRIPTION)) + SIZE_OF_MANIFEST_MAC;
  DEBUG ((DEBUG_INFO, "[TDX] Total Byte Size Manifest   : 0x%X\n", ActmDimmManifest->Header.Size ));
  return EFI_SUCCESS;
}
