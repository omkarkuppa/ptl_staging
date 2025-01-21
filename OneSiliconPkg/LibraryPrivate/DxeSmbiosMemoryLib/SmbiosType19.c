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
/// Memory Array Mapped Address (Type 19) data
///
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TABLE_TYPE19 SmbiosTableType19Data = {
  { EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS, sizeof (SMBIOS_TABLE_TYPE19), 0 },
  0,            ///< StartingAddress
  TO_BE_FILLED, ///< EndingAddress
  TO_BE_FILLED, ///< MemoryArrayHandle
  TO_BE_FILLED, ///< PartitionWidth
  0,            ///< ExtendedStartingAddress
  0,            ///< ExtendedEndingAddress
};


/**
  This function installs SMBIOS Table Type 19 (Physical Memory Array).

  @param[in] SmbiosProtocol    - Instance of Smbios Protocol

  @retval EFI_SUCCESS          - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES - if not able to get resources.
**/
EFI_STATUS
InstallSmbiosType19 (
  IN  EFI_SMBIOS_PROTOCOL *SmbiosProtocol
  )
{
  EFI_STATUS                      Status;
  UINT64                          TotalMemorySizeInKb;
  UINT8                           Dimm;
  UINT8                           MaxSockets;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  UINT8                           ChannelIndex;
  UINT8                           DimmIndex;
  UINT8                           ControllerIndex;

  TotalMemorySizeInKb = 0;

  ///
  /// Get total memory from HOB.
  ///
  TotalMemorySizeInKb = (UINT64) mMemInfo->TotalPhysicalMemorySize * 1024;  // Total memory in kilobytes.
  ///
  /// Count the DIMMs present.
  ///
  MaxSockets = 0;
  // @todo - Update for 2xMC
  for (ControllerIndex = 0; ControllerIndex < MEM_CFG_MAX_CONTROLLERS; ControllerIndex++) {
   for (Dimm = 0; Dimm < (MEM_CFG_MAX_SOCKETS/MEM_CFG_MAX_CONTROLLERS); Dimm++) {
     ChannelIndex = Dimm >> 1;
     DimmIndex = Dimm & 0x1;
     if ((mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].Status == DIMM_PRESENT) && (mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].DimmCapacity > 0)) {
       MaxSockets++;
     }
   }
  }
  ///
  /// Populate table.
  ///
  if (TotalMemorySizeInKb > SMBIOS_TYPE19_USE_EXTENDED_ADDRESSES) {
    SmbiosTableType19Data.StartingAddress = SMBIOS_TYPE19_USE_EXTENDED_ADDRESSES;
    SmbiosTableType19Data.EndingAddress = SMBIOS_TYPE19_USE_EXTENDED_ADDRESSES;
    SmbiosTableType19Data.ExtendedEndingAddress = MultU64x64 (TotalMemorySizeInKb, 1024) - 1;
  } else {
    SmbiosTableType19Data.EndingAddress = (UINT32) (TotalMemorySizeInKb - 1);
  }
  SmbiosTableType19Data.MemoryArrayHandle = mSmbiosType16Handle;
  SmbiosTableType19Data.PartitionWidth = MaxSockets;
  ///
  /// Generate Memory Array Mapped Address info (TYPE 19)
  ///
  Status = AddSmbiosEntry ((EFI_SMBIOS_TABLE_HEADER *) &SmbiosTableType19Data, NULL, 0, SmbiosProtocol, &SmbiosHandle);
  return Status;
}
