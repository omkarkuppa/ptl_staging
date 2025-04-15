/** @file
  This library will determine memory configuration information from the chipset
  and memory and create SMBIOS memory structures appropriately.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include "SmbiosMemory.h"
#include "MemoryConfig.h"

///
/// Physical Memory Array (Type 16) data
///
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TABLE_TYPE16 SmbiosTableType16Data = {
  { EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY, sizeof (SMBIOS_TABLE_TYPE16), 0 },
  MemoryArrayLocationSystemBoard, ///< Location
  MemoryArrayUseSystemMemory,     ///< Use
  TO_BE_FILLED,                   ///< MemoryErrorCorrection
  TO_BE_FILLED,                   ///< MaximumCapacity
  0xFFFE,                         ///< MemoryErrorInformationHandle
  TO_BE_FILLED,                   ///< NumberOfMemoryDevices
  0,                              ///< ExtendedMaximumCapacity
};


/**
  This function installs SMBIOS Table Type 16 (Physical Memory Array).

  @param[in] SmbiosProtocol    - Instance of Smbios Protocol

  @retval EFI_SUCCESS          - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES - if not able to get resources.
  @retval EFI_NOT_FOUND        - if PcdSlotMapAddress not to set before calling.
**/
EFI_STATUS
InstallSmbiosType16 (
  IN  EFI_SMBIOS_PROTOCOL *SmbiosProtocol
  )
{
  EFI_STATUS                      Status;
  UINT32                          MaximumCapacity;
  UINT32                          MaxRankCapacity;
  UINT16                          MaxSockets;
  UINT8                           ModuleType;
  UINT8                           ChannelIndex;
  UINT8                           DimmIndex;
  UINT8                           Dimm;
  DIMM_INFO                       *DimmInfo;
  UINT8                           ControllerIndex;
  MRC_SLOTMAP                     *MrcSlotMap;
  UINT32                          MaxDimmCountPerChannel;
  UINT32                          MaxRankinDimm;

  ///
  /// Configure the data for TYPE 16 SMBIOS Structure
  ///
  MrcSlotMap = (VOID*)(UINTN)PcdGet64 (PcdSlotMapAddress);
  if (MrcSlotMap == NULL) {
    return EFI_NOT_FOUND;
  }

  ///
  /// Create physical array and associated data for all mainboard memory
  ///

  ///
  /// Update ECC Support
  ///
  if (mMemInfo->EccSupport) {
    SmbiosTableType16Data.MemoryErrorCorrection = MemoryErrorCorrectionSingleBitEcc;
  } else {
    SmbiosTableType16Data.MemoryErrorCorrection = MemoryErrorCorrectionNone;
  }
  ///
  /// Get the Memory DIMM info from policy protocols
  ///
  MaxSockets = 0;
  ModuleType = 0;
  MaximumCapacity = 0;
  MaxRankCapacity = mMemInfo->MaxRankCapacity;
  for (ControllerIndex = 0; ControllerIndex < MEM_CFG_MAX_CONTROLLERS; ControllerIndex++) {
    for (Dimm = 0; Dimm < (MEM_CFG_MAX_SOCKETS/MEM_CFG_MAX_CONTROLLERS); Dimm++) {
      ChannelIndex = Dimm >> 1;
      DimmIndex = Dimm & 0x1;
      DimmInfo = &mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex];
      if ((DimmInfo->Status == DIMM_PRESENT) && (DimmInfo->DimmCapacity > 0)) {
        ModuleType = DimmInfo->SpdModuleType;
         // If Memory Down is detected, report System Memory Size instead of Maximum Capacity
        if ((ModuleType & DDR_MTYPE_SPD_MASK) == DDR_MTYPE_MEM_DOWN_LP5) {
          MaximumCapacity += (DimmInfo->DimmCapacity * 1024); // Convert from MB to KB
        }
      }
      if ((BIT0 << DimmIndex) & MrcSlotMap->MrcSlotMap[ControllerIndex][ChannelIndex]) {
        MaxSockets++;
      }
    }
  }

  // If Memory Down is not detected, report Maximum Capacity based on Memory configuration
  if (((ModuleType & DDR_MTYPE_SPD_MASK) != DDR_MTYPE_MEM_DOWN_LP5)) {
    MaxDimmCountPerChannel = mMemInfo->Controller[0].ChannelInfo[0].DimmCount;
    MaxRankinDimm = MEM_CFG_MAX_RANKS_PER_DIMM; //it will be always 2
    MaximumCapacity = (MaxRankCapacity * MaxRankinDimm * MaxDimmCountPerChannel * MaxSockets) * 1024 * 1024;
  }

  if (MaximumCapacity < SMBIOS_TYPE16_USE_EXTENDED_MAX_CAPACITY) {
    SmbiosTableType16Data.MaximumCapacity = MaximumCapacity;
  } else {
    SmbiosTableType16Data.MaximumCapacity = SMBIOS_TYPE16_USE_EXTENDED_MAX_CAPACITY;
    SmbiosTableType16Data.ExtendedMaximumCapacity = ((UINT64) MaximumCapacity) * 1024; // Convert from KB to Byte
  }

  // This is Max Socket in system irrespective of DIMM populated or not
  SmbiosTableType16Data.NumberOfMemoryDevices = MaxSockets;

  ///
  /// Install SMBIOS Table Type 16
  ///
  Status = AddSmbiosEntry ((EFI_SMBIOS_TABLE_HEADER *) &SmbiosTableType16Data, NULL, 0, SmbiosProtocol, &mSmbiosType16Handle);
  return Status;
}
