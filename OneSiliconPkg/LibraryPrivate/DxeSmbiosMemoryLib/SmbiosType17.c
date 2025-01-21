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
/// Memory Device (Type 17) data
///
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TABLE_TYPE17 SmbiosTableType17Data = {
  { EFI_SMBIOS_TYPE_MEMORY_DEVICE, sizeof (SMBIOS_TABLE_TYPE17), 0 },
  TO_BE_FILLED, ///< MemoryArrayHandle
  0xFFFE,       ///< MemoryErrorInformationHandle
  TO_BE_FILLED, ///< TotalWidth
  TO_BE_FILLED, ///< DataWidth
  TO_BE_FILLED, ///< Size
  TO_BE_FILLED, ///< FormFactor
  0,            ///< DeviceSet
  STRING_1,     ///< DeviceLocator
  STRING_2,     ///< BankLocator
  TO_BE_FILLED, ///< MemoryType
  {             ///< TypeDetail
    0,            ///< Reserved     :1;
    0,            ///< Other        :1;
    0,            ///< Unknown      :1;
    0,            ///< FastPaged    :1;
    0,            ///< StaticColumn :1;
    0,            ///< PseudoStatic :1;
    TO_BE_FILLED, ///< Rambus       :1;
    TO_BE_FILLED, ///< Synchronous  :1;
    0,            ///< Cmos         :1;
    0,            ///< Edo          :1;
    0,            ///< WindowDram   :1;
    0,            ///< CacheDram    :1;
    0,            ///< Nonvolatile  :1;
    0,            ///< Registered   :1;
    0,            ///< Unbuffered   :1;
    0,            ///< Reserved1    :1;
  },
  TO_BE_FILLED, ///< Speed
  TO_BE_FILLED, ///< Manufacturer
  TO_BE_FILLED, ///< SerialNumber
  TO_BE_FILLED, ///< AssetTag
  TO_BE_FILLED, ///< PartNumber
  TO_BE_FILLED, ///< Attributes
  TO_BE_FILLED, ///< ExtendedSize
  TO_BE_FILLED, ///< ConfiguredMemoryClockSpeed
  TO_BE_FILLED, ///< MinimumVoltage
  TO_BE_FILLED, ///< MaximumVoltage
  TO_BE_FILLED, ///< ConfiguredVoltage
  TO_BE_FILLED, ///< MemoryTechnology
  {             ///< MemoryOperatingModeCapability
    {           ///< Bits
      0,            ///< Reserved                         :1;
      0,            ///< Other                            :1;
      0,            ///< Unknown                          :1;
      TO_BE_FILLED, ///< VolatileMemory                   :1;
      0,            ///< ByteAccessiblePersistentMemory   :1;
      0,            ///< BlockAccessiblePersistentMemory  :1;
      0,            ///< Reserved2                        :10;
    }
  },
  TO_BE_FILLED, ///< FirmwareVersion
  TO_BE_FILLED, ///< ModuleManufacturerID
  TO_BE_FILLED, ///< ModuleProductID
  TO_BE_FILLED, ///< MemorySubsystemControllerManufacturerID
  TO_BE_FILLED, ///< MemorySubsystemControllerProductID
  TO_BE_FILLED, ///< NonVolatileSize
  TO_BE_FILLED, ///< VolatileSize
  TO_BE_FILLED, ///< CacheSize
  TO_BE_FILLED  ///< LogicalSize
};
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TYPE17_STRING_ARRAY SmbiosTableType17Strings = {
  TO_BE_FILLED_STRING,  ///< DeviceLocator
  TO_BE_FILLED_STRING,  ///< BankLocator
  TO_BE_FILLED_STRING,  ///< Manufacturer
  TO_BE_FILLED_STRING,  ///< SerialNumber
  TO_BE_FILLED_STRING,  ///< AssetTag
  TO_BE_FILLED_STRING,  ///< PartNumber
  TO_BE_FILLED_STRING   ///< FirmwareVersion
};

/**
  Retrieves the OEM custom string for the SMBIOS Type 17 Table DeviceLocator field.
  Implementation of this function is optional, if this function pointer is NULL then
  the reference implementation of DeviceLocator will be used.

  @param[in]  Controller                    Desired Controller to get a DeviceLocator string for.
  @param[in]  Dimm                          Desired DIMM to get a DeviceLocator string for.
  @param[in]  MdSocket                      0 = Memory Down, 1 = Socketed.

  @retval                                   The DeviceLocator string
  @retval     NULL                          If the return value is NULL, the default value will be used.
**/
typedef
CHAR8*
(EFIAPI *GET_DEVICE_LOCATOR_STRING)(
  IN          UINT8               Controller,
  IN          UINT8               Dimm,
  IN          UINT8               MdSocket
  );

/**
  Retrieves string for the SMBIOS Type 17 Table DeviceLocator field.

  @param[in]  Controller                    Desired Controller to get a DeviceLocator string for.
  @param[in]  Dimm                          Desired DIMM to get a DeviceLocator string for.
  @param[in]  MdSocket                      0 = Memory Down, 1 = Socketed.

  @retval                                   The DeviceLocator string
**/
CHAR8*
GetDeviceLocatorString (
  IN  UINT8               ControllerIndex,
  IN  UINT8               Dimm,
  IN  UINT8               MdSocket
  )
{
  CHAR8     *DeviceLocator;
  GET_DEVICE_LOCATOR_STRING  PlatformDeviceLocatorString;

  PlatformDeviceLocatorString = (GET_DEVICE_LOCATOR_STRING) PcdGet64 (PcdGetPlatformDeviceLocatorString);

  if (PlatformDeviceLocatorString != NULL) {
    DeviceLocator = PlatformDeviceLocatorString (ControllerIndex, Dimm, MdSocket);
    if (DeviceLocator != NULL) {
      return DeviceLocator;
    }
  }
  if (MdSocket) {
    return DeviceLocatorStringsDimm[ControllerIndex][Dimm];
  } else {
    return DeviceLocatorStringsMemoryDown[ControllerIndex][Dimm];
  }
}

/**
  Retrieves string for the SMBIOS Type 17 Table BankLocator field.

  @param[in]  Controller                    Desired Controller to get a BankLocator string for.
  @param[in]  Dimm                          Desired DIMM to get a BankLocator string for.
  @param[in]  MdSocket                      0 = Memory Down, 1 = Socketed.

  @retval                                   The BankLocator string
**/
CHAR8*
GetBankLocatorString (
  IN  UINT8               ControllerIndex,
  IN  UINT8               Dimm,
  IN  UINT8               MdSocket
  )
{
  CHAR8     *BankLocator;
  GET_DEVICE_LOCATOR_STRING  PlatformBankLocatorString;

  PlatformBankLocatorString = (GET_DEVICE_LOCATOR_STRING) PcdGet64 (PcdGetPlatformDeviceLocatorString);

  if (PlatformBankLocatorString != NULL) {
    BankLocator = PlatformBankLocatorString (ControllerIndex, Dimm, MdSocket);
    if (BankLocator != NULL) {
      return BankLocator;
    }
  }
  return BankLocatorStrings[Dimm >> 1];
}

/**
  This function installs SMBIOS Table Type 17 (Memory Device).
  This function installs one table per memory device slot, whether populated or not.

  @param[in] SmbiosProtocol     - Instance of Smbios Protocol

  @retval EFI_SUCCESS           - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES  - if not able to get resources.
  @retval EFI_INVALID_PARAMETER - if a required parameter in a subfunction is NULL.
  @retval EFI_NOT_FOUND         - if PcdSlotMapAddress not to set before calling.
**/
EFI_STATUS
InstallSmbiosType17 (
  IN  EFI_SMBIOS_PROTOCOL *SmbiosProtocol
  )
{
  EFI_STATUS                      Status;
  UINT16                          Index;
  UINT32                          DimmMemorySizeInMB;
  UINT8                           Dimm;
  UINT8                           *SmbusBuffer;
  CHAR8                           *StringBuffer;
  CHAR8                           *StringBufferStart;
  UINTN                           StringBufferSize;
  CHAR8                           StringBuffer2[4];
  UINT8                           IndexCounter;
  UINTN                           IdListIndex;
  UINT16                          MemoryTotalWidth;
  UINT16                          MemoryDataWidth;
  BOOLEAN                         FoundManufacturer;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  UINT16                          ManufactureIdLsb;
  UINT16                          ManufactureIdMsb;
  UINT16                          PartNoStartByte;
  UINT16                          PartNoEndByte;
  UINT16                          SerialNoStartByte;
  UINT16                          SerialNoEndByte;
  UINT8                           MemoryBusWidthByte;
  UINT8                           DramDeviceType;
  UINT8                           ModuleType;
  UINTN                           StrBuffLen;
  UINT8                           ChannelIndex;
  UINT8                           DimmIndex;
  UINT8                           ControllerIndex;
  UINT8                           MdSocket;
  MRC_SLOTMAP                     *MrcSlotMap;
  MEMORY_DEVICE_OPERATING_MODE_CAPABILITY MemoryDeviceOperatingModeCapability;

  MrcSlotMap = (VOID*)(UINTN)PcdGet64 (PcdSlotMapAddress);
  if (MrcSlotMap == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = EFI_SUCCESS;
  MemoryDeviceOperatingModeCapability.Uint16 = 0;

  /**
    StringBuffer should only use around 50 to 60 characters max.
    Therefore, allocate around double that, as a saftey margin
  **/
  StringBufferSize  = (sizeof (CHAR8)) * 100;
  StringBufferStart = AllocateZeroPool (StringBufferSize);
  if (StringBufferStart == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  /**
    Initialize variables for retrieving required data from SPD buffer.
    Since the first SPD byte in buffer is SPD_MANUFACTURER_ID_LSB,
    all fields offset should be subtracted by SPD_MANUFACTURER_ID_LSB.
    DDR4 offsets for the values below are good also for DDR5/LP4/LP5.
  **/
  ManufactureIdLsb = 0;
  ManufactureIdMsb  = DDR4_SPD_MANUFACTURER_ID_MSB  - DDR4_SPD_MANUFACTURER_ID_LSB;
  PartNoStartByte   = DDR4_SPD_PART_NO_START_BYTE   - DDR4_SPD_MANUFACTURER_ID_LSB;
  PartNoEndByte     = DDR4_SPD_PART_NO_END_BYTE     - DDR4_SPD_MANUFACTURER_ID_LSB;
  SerialNoStartByte = DDR4_SPD_SERIAL_NO_START_BYTE - DDR4_SPD_MANUFACTURER_ID_LSB;
  SerialNoEndByte   = DDR4_SPD_SERIAL_NO_END_BYTE   - DDR4_SPD_MANUFACTURER_ID_LSB;

  SmbusBuffer     = NULL;
  ///
  /// Each instance of table type 17 has the same MemoryArrayHandle
  ///
  SmbiosTableType17Data.MemoryArrayHandle = mSmbiosType16Handle;

  /**
    For each socket that exists, whether it is populated or not, generate Type 17.

    The Desktop and mobile only support 2 channels * 2 slots per channel = 4 sockets totally
    So there is rule here for Desktop and mobile that there are no more 4 DIMMS totally in a system:
     - Channel A/ Slot 0 --> SpdAddressTable[0] --> DeviceLocatorStringsDimm[0] --> MemInfoHobProtocol.MemInfoData.dimmSize[0]
     - Channel A/ Slot 1 --> SpdAddressTable[1] --> DeviceLocatorStringsDimm[1] --> MemInfoHobProtocol.MemInfoData.dimmSize[1]
     - Channel B/ Slot 0 --> SpdAddressTable[2] --> DeviceLocatorStringsDimm[2] --> MemInfoHobProtocol.MemInfoData.dimmSize[2]
     - Channel B/ Slot 1 --> SpdAddressTable[3] --> DeviceLocatorStringsDimm[3] --> MemInfoHobProtocol.MemInfoData.dimmSize[3]
  **/
  for (ControllerIndex = 0; ControllerIndex < MEM_CFG_MAX_CONTROLLERS; ControllerIndex++) {
    for (Dimm = 0; Dimm < (MEM_CFG_MAX_SOCKETS/MEM_CFG_MAX_CONTROLLERS); Dimm++) {
      ChannelIndex = Dimm >> 1;
      DimmIndex = Dimm & 0x1;
      if (MrcSlotMap->MrcSlotMap[ControllerIndex][ChannelIndex] == 0) {
        continue;
      }
      if ((mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].Status == DIMM_PRESENT) && (mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].DimmCapacity > 0)) {
        ///
        /// Generate Memory Device info (Type 17)
        ///
        DramDeviceType = mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].SpdDramDeviceType;
        ModuleType = mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].SpdModuleType;
        MemoryBusWidthByte = mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].SpdModuleMemoryBusWidth;
        SmbiosTableType17Strings.SerialNumber = (CHAR8*) PcdGetPtr (PcdSmbios17SerialNumber);

        ///
        /// MRC save/restore hob only provides these SPD bytes: Non-JEDEC LPDDR3/DDR3 - 117 ~ 145; JEDEC LPDDR3/DDR4 - 320 ~ 348
        ///
        SmbusBuffer = (UINT8 *) &(mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].SpdSave);

        ///
        /// Use SPD data to generate Device Type info
        ///
        MdSocket = mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].MdSocket;
        SmbiosTableType17Strings.DeviceLocator  = GetDeviceLocatorString (ControllerIndex, Dimm, MdSocket);
        SmbiosTableType17Strings.BankLocator    = GetBankLocatorString (ControllerIndex, Dimm, MdSocket);

        ///
        /// Reset StringBuffer
        ///
        StringBuffer = StringBufferStart;

        ///
        /// Show name for known manufacturer or ID for unknown manufacturer
        ///
        FoundManufacturer = FALSE;

        ///
        /// Calculate index counter
        /// Clearing Bit7 as it is the Parity Bit for Byte 117
        ///
        IndexCounter = SmbusBuffer[ManufactureIdLsb] & (~0x80);

        ///
        /// Convert memory manufacturer ID to string
        ///
        for (IdListIndex = 0; MemoryModuleManufactureList[IdListIndex].Index != 0xff; IdListIndex++) {
          if (MemoryModuleManufactureList[IdListIndex].Index == IndexCounter &&
              MemoryModuleManufactureList[IdListIndex].ManufactureId == SmbusBuffer[ManufactureIdMsb]
              ) {
            SmbiosTableType17Strings.Manufacturer = MemoryModuleManufactureList[IdListIndex].ManufactureName;
            FoundManufacturer = TRUE;
            break;
          }
        }
        ///
        /// Use original data if no conversion information in conversion table
        ///
        StrBuffLen = StringBufferSize / sizeof (CHAR8);
        if (!(FoundManufacturer)) {
          AsciiStrCpyS (StringBuffer, StrBuffLen, "0x");
          for (Index = ManufactureIdLsb; Index <= ManufactureIdMsb; Index++) {
            if (Index == ManufactureIdLsb) {
              ///
              /// Clear the parity bit on the continuation counter for added readability
              ///
              AsciiValueToStringS (
                StringBuffer2,
                sizeof (StringBuffer2),
                PREFIX_ZERO | RADIX_HEX,
                (UINT8) (SmbusBuffer[Index] & (~0x80)),
                2);
            } else {
              AsciiValueToStringS (StringBuffer2, sizeof (StringBuffer2), PREFIX_ZERO | RADIX_HEX, SmbusBuffer[Index], 2);
            }
            AsciiStrCatS (StringBuffer, StrBuffLen, StringBuffer2);
          }
          SmbiosTableType17Strings.Manufacturer = StringBuffer;
          StrBuffLen -= AsciiStrSize (StringBuffer);
          StringBuffer += AsciiStrSize (StringBuffer);
        }
        AsciiStrCpyS (StringBuffer, StrBuffLen, "");
        for (Index = SerialNoStartByte; Index <= SerialNoEndByte; Index++) {
          AsciiValueToStringS (StringBuffer2, sizeof (StringBuffer2), PREFIX_ZERO | RADIX_HEX, SmbusBuffer[Index], 2);
          AsciiStrCatS (StringBuffer, StrBuffLen, StringBuffer2);
        }
        SmbiosTableType17Strings.SerialNumber = StringBuffer;
        StrBuffLen -= AsciiStrSize (StringBuffer);
        StringBuffer += AsciiStrSize (StringBuffer);

        AsciiStrCpyS (StringBuffer, StrBuffLen, "");
        for (Index = PartNoStartByte; Index <= PartNoEndByte; Index++) {
          AsciiSPrint (StringBuffer2, 4, "%c", SmbusBuffer[Index]);
          AsciiStrCatS (StringBuffer, StrBuffLen, StringBuffer2);
        }
        SmbiosTableType17Strings.PartNumber = StringBuffer;
        ASSERT ((StringBuffer + AsciiStrSize (StringBuffer)) < (StringBufferStart + StringBufferSize));

        SmbiosTableType17Data.Manufacturer    = STRING_3;
        SmbiosTableType17Data.SerialNumber    = STRING_4;
        SmbiosTableType17Strings.AssetTag     = (CHAR8 *) PcdGetPtr (PcdSmbios17AssetTag);
        SmbiosTableType17Data.AssetTag        = STRING_5;
        SmbiosTableType17Data.PartNumber      = STRING_6;

        ///
        /// Get the Memory TotalWidth and DataWidth info for DDR3
        /// refer to DDR3 SPD 1.0 spec, Byte 8: Module Memory Bus Width
        /// SPD Offset 8 Bits [2:0] DataWidth aka Primary Bus Width
        /// SPD Offset 8 Bits [4:3] Bus Width extension for ECC
        ///
        MemoryDataWidth = 8 * (1 << (MemoryBusWidthByte & 0x07));
        MemoryTotalWidth = MemoryDataWidth + (MemoryBusWidthByte & 0x18);
        SmbiosTableType17Data.TotalWidth = MemoryTotalWidth;
        SmbiosTableType17Data.DataWidth = MemoryDataWidth;

        DimmMemorySizeInMB = mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].DimmCapacity;
        ///
        /// See the DMTF SMBIOS Specification 2.7.1, section 7.18.4
        /// regarding use of the ExtendedSize field.
        ///
        if (DimmMemorySizeInMB < SMBIOS_TYPE17_USE_EXTENDED_SIZE) {
          SmbiosTableType17Data.Size = (UINT16) DimmMemorySizeInMB;
          SmbiosTableType17Data.ExtendedSize = 0;
        } else {
          SmbiosTableType17Data.Size = SMBIOS_TYPE17_USE_EXTENDED_SIZE;
          SmbiosTableType17Data.ExtendedSize = DimmMemorySizeInMB;
        }

        if (DramDeviceType == DDR_DTYPE_LPDDR5 || DramDeviceType == DDR_DTYPE_LPDDR5X) {
          switch (ModuleType & DDR_MTYPE_SPD_MASK) {
            case DDR_MTYPE_LPDIMM_LP5:
              SmbiosTableType17Data.FormFactor = 0x11;
              break;

            case DDR_MTYPE_MEM_DOWN_LP5:
            default:
              SmbiosTableType17Data.FormFactor = MemoryFormFactorRowOfChips;
              break;
          }
        } else {
          switch (ModuleType & DDR_MTYPE_SPD_MASK) {
            case DDR_MTYPE_SODIMM:
              /// Legacy non-JEDEC LPDDR3 SPD images use SODIMM module type which should be soldered down form factor.
              SmbiosTableType17Data.FormFactor = (DramDeviceType == DDR_DTYPE_LPDDR3) ? MemoryFormFactorRowOfChips : MemoryFormFactorSodimm;
              break;

            case DDR_MTYPE_RDIMM:
              SmbiosTableType17Data.FormFactor = MemoryFormFactorRimm;
              break;

            case DDR_MTYPE_CSODIMM:
              SmbiosTableType17Data.FormFactor = MemoryFormFactorSodimm;
              break;

            case DDR_MTYPE_MEM_DOWN:
              SmbiosTableType17Data.FormFactor = MemoryFormFactorRowOfChips;
              break;

            case DDR_MTYPE_CAMM:
              SmbiosTableType17Data.FormFactor = 0x11;
              break;

            case DDR_MTYPE_UDIMM:
            case DDR_MTYPE_LR_DIMM:
            case DDR_MTYPE_CUDIMM:
            case DDR_MTYPE_MRDIMM:
            case DDR_MTYPE_DDIMM:
            default:
              SmbiosTableType17Data.FormFactor = MemoryFormFactorDimm;
              break;
          }
        }

        switch (DramDeviceType) {
          case DDR_DTYPE_DDR3:
            SmbiosTableType17Data.MemoryType = (UINT8) MemoryTypeDdr3;
            break;

          case DDR_DTYPE_DDR4:
            SmbiosTableType17Data.MemoryType = (UINT8) MemoryTypeDdr4;
            break;

          case DDR_DTYPE_DDR5:
            SmbiosTableType17Data.MemoryType = (UINT8) MemoryTypeDdr5;
            break;

          case DDR_DTYPE_LPDDR5:
          case DDR_DTYPE_LPDDR5X:
            SmbiosTableType17Data.MemoryType = (UINT8) MemoryTypeLpddr5;
            break;

          case DDR_DTYPE_LPDDR3:
          case DDR_DTYPE_JEDEC_LPDDR3:
            SmbiosTableType17Data.MemoryType = (UINT8) MemoryTypeLpddr3;
            break;

          default:
            SmbiosTableType17Data.MemoryType = (UINT8) MemoryTypeLpddr4;
            break;
        }

        if (SmbiosTableType17Data.FormFactor == MemoryFormFactorRimm) {
          SmbiosTableType17Data.TypeDetail.Rambus = 1;
        } else {
          SmbiosTableType17Data.TypeDetail.Rambus = 0;
        }
        SmbiosTableType17Data.TypeDetail.Synchronous  = 1;

        SmbiosTableType17Data.Speed = mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].Speed;
        SmbiosTableType17Data.Attributes = mMemInfo->Controller[ControllerIndex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].RankInDimm & 0x0F;

        SmbiosTableType17Data.ConfiguredMemoryClockSpeed = (UINT16) mMemInfo->ConfiguredMemoryClockSpeed;
        SmbiosTableType17Data.MinimumVoltage             = 0;
        SmbiosTableType17Data.MaximumVoltage             = 0;
        SmbiosTableType17Data.ConfiguredVoltage          = (UINT16) mMemInfo->VddVoltage[mMemInfo->MemoryProfile];

        SmbiosTableType17Data.FirmwareVersion                           = NO_STRING_AVAILABLE;
        SmbiosTableType17Strings.FirmwareVersion                        = NULL;

        SmbiosTableType17Data.MemoryTechnology                          = MemoryTechnologyDram;
        MemoryDeviceOperatingModeCapability.Bits.VolatileMemory         = 1;
        SmbiosTableType17Data.MemoryOperatingModeCapability             = MemoryDeviceOperatingModeCapability;
        SmbiosTableType17Data.ModuleManufacturerID                      = SmbusBuffer[ManufactureIdLsb] | (SmbusBuffer[ManufactureIdMsb] << 8);
        SmbiosTableType17Data.ModuleProductID                           = 0;
        SmbiosTableType17Data.MemorySubsystemControllerManufacturerID   = 0;
        SmbiosTableType17Data.MemorySubsystemControllerProductID        = 0;
        SmbiosTableType17Data.NonVolatileSize                           = 0;
        SmbiosTableType17Data.VolatileSize                              = ((UINT64)DimmMemorySizeInMB) << 20;  // Convert from MB to Bytes
        SmbiosTableType17Data.CacheSize                                 = 0;
        SmbiosTableType17Data.LogicalSize                               = 0;

      } else if ((BIT0 << DimmIndex) & MrcSlotMap->MrcSlotMap[ControllerIndex][ChannelIndex]) {
        ///
        /// Slot exists but Memory is not Populated in this slot.
        ///
        SmbiosTableType17Strings.DeviceLocator       = GetDeviceLocatorString (ControllerIndex, Dimm, 1);
        SmbiosTableType17Strings.BankLocator         = GetBankLocatorString (ControllerIndex, Dimm, 1);

        SmbiosTableType17Strings.Manufacturer        = NULL;
        SmbiosTableType17Strings.SerialNumber        = NULL;
        SmbiosTableType17Strings.PartNumber          = NULL;
        SmbiosTableType17Strings.FirmwareVersion     = NULL;

        SmbiosTableType17Data.Manufacturer           = NO_STRING_AVAILABLE;
        SmbiosTableType17Data.SerialNumber           = NO_STRING_AVAILABLE;
        SmbiosTableType17Data.AssetTag               = NO_STRING_AVAILABLE;
        SmbiosTableType17Data.PartNumber             = NO_STRING_AVAILABLE;

        SmbiosTableType17Data.TotalWidth             = 0;
        SmbiosTableType17Data.DataWidth              = 0;
        SmbiosTableType17Data.Size                   = 0;
        SmbiosTableType17Data.FormFactor             = MemoryFormFactorUnknown;
        SmbiosTableType17Data.MemoryType             = MemoryTypeUnknown;
        SmbiosTableType17Data.TypeDetail.Rambus      = 0;
        SmbiosTableType17Data.TypeDetail.Synchronous = 0;
        SmbiosTableType17Data.Speed                  = 0;
        SmbiosTableType17Data.Attributes             = 0;
        SmbiosTableType17Data.ExtendedSize           = 0;
        SmbiosTableType17Data.MinimumVoltage         = 0;
        SmbiosTableType17Data.MaximumVoltage         = 0;
        SmbiosTableType17Data.ConfiguredVoltage      = 0;

        SmbiosTableType17Data.ConfiguredMemoryClockSpeed = 0;
        SmbiosTableType17Data.MemoryTechnology                          = 0;
        SmbiosTableType17Data.MemoryOperatingModeCapability             = MemoryDeviceOperatingModeCapability;
        SmbiosTableType17Data.ModuleManufacturerID                      = 0;
        SmbiosTableType17Data.ModuleProductID                           = 0;
        SmbiosTableType17Data.MemorySubsystemControllerManufacturerID   = 0;
        SmbiosTableType17Data.MemorySubsystemControllerProductID        = 0;
        SmbiosTableType17Data.NonVolatileSize                           = 0;
        SmbiosTableType17Data.VolatileSize                              = 0;
        SmbiosTableType17Data.CacheSize                                 = 0;
        SmbiosTableType17Data.LogicalSize                               = 0;
      } else {
        ///
        /// Slot doesn't exist. Continue without adding Type 17 record for this slot.
        ///
        continue;
      }

      ///
      /// Generate Memory Device info (Type 17)
      ///
      Status = AddSmbiosEntry (
                 (EFI_SMBIOS_TABLE_HEADER *) &SmbiosTableType17Data,
                 (CHAR8 **) &SmbiosTableType17Strings,
                 SMBIOS_TYPE17_NUMBER_OF_STRINGS,
                 SmbiosProtocol,
                 &SmbiosHandle);
      if (EFI_ERROR (Status)) {
        goto CleanAndExit;
      }
    }
  }
CleanAndExit:
  FreePool (StringBufferStart);
  return Status;
}
