/** @file
  This file contains Default data for Platform SMBIOS tables. This file should not
  be altered unless update in SMBIOS Specification.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Base.h>
#include <Uefi.h>
#include "SmbiosMiscDxeDefinition.h"
#include "SmbiosMiscDxeStrings.h"
#include "SmbiosPlatformInfoTable.h"

//
// String Macro Definitions for Default Data
//
#define TO_BE_FILLED                                0
#define DEFAULT_BIOSVENDOR_SEGMENT                  TO_BE_FILLED
#define DEFAULT_BIOSVENDOR_SIZE                     TO_BE_FILLED
/**
  MISC_BIOS_CHARACTERISTICS.BiosCharacteristics
  Bit0-1 Reserved1,
  Bit02 Unknown,
  Bit03 BiosCharacteristicsNotSupported,
  Bit04 IsaIsSupported,
  Bit05 McaIsSupported,
  Bit06 EisaIsSupported,
  Bit07 PciIsSupported,
  Bit08 PcmciaIsSupported,
  Bit09 PlugAndPlayIsSupported,
  Bit10 ApmIsSupported,
  Bit11 BiosIsUpgradable,
  Bit12 BiosShadowingAllowed,
  Bit13 VLVesaIsSupported,
  Bit14 EscdSupportIsAvailable,
  Bit15 BootFromCdIsSupported
  Bit16 SelectableBootIsSupported,
  Bit17 RomBiosIsSocketed,
  Bit18 BootFromPcmciaIsSupported,
  Bit19 EDDSpecificationIsSupported,
  Bit20 JapaneseNecFloppyIsSupported,
  Bit21 JapaneseToshibaFloppyIsSupported,
  Bit22 Floppy525_360IsSupported,
  Bit23 Floppy525_12IsSupported,
  Bit24 Floppy35_720IsSupported,
  Bit25 Floppy35_288IsSupported,
  Bit26 PrintScreenIsSupported,
  Bit27 Keyboard8042IsSupported,
  Bit28 SerialIsSupported,
  Bit29 PrinterIsSupported,
  Bit30 CgaMonoIsSupported,
  Bit31 NecPc98,
  Bit32-63 Reserved for BIOS/System Vendor
**/
#define DEFAULT_BIOSVENDOR_CHARS                    { 0,0,0,0,0,0,1,0,0,0,1,1,0,0,1,1,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0 }
/**
  MISC_BIOS_CHARACTERISTICS_EXTENSION.BiosReserved
  Bit0 AcpiIsSupported,
  Bit1 UsbLegacyIsSupported,
  Bit2 AgpIsSupported,
  Bit3 I20BootIsSupported,
  Bit4 Ls120BootIsSupported,
  Bit5 AtapiZipDriveBootIsSupported,
  Bit6 Boot1394IsSupported,
  Bit7 SmartBatteryIsSupported
**/
#define DEFAULT_BIOSVENDOR_CHARS_EXT_BIOS           (UINT8)(BIT0 | BIT1 | BIT4 | BIT5)
/**
  MISC_BIOS_CHARACTERISTICS_EXTENSION.SystemReserved
  Bit0 BiosBootSpecIsSupported,
  Bit1 FunctionKeyNetworkBootIsSupported,
  Bit2 TargetContentDistributionEnabled,
  Bit3 UefiSpecificationSupported,
  Bit4 VirtualMachineSupported,
  Bit5-7 ExtensionByte2Reserved
**/
#define DEFAULT_BIOSVENDOR_CHARS_EXT_SYSTEM         (UINT8)(BIT0 | BIT1 | BIT2 | BIT3)
#define DEFAULT_BIOSVENDOR_EC_RELEASE               0xFF
#define DEFAULT_SYSINFO_PRODUCTNAME                 L"Client Platform"
#define DEFAULT_SYSINFO_VERSION                     L"0.1"
#define DEFAULT_SYSINFO_FAMILY                      L"Client System"
#define DEFAULT_BOARDINFO_PRODUCTNAME               L"RVP/SDS"
#define DEFAULT_BOARDINFO_MANUFACTURER              L"Intel Corporation"
#define DEFAULT_BOARDINFO_VERSION                   L"FAB"
#define DEFAULT_BOARDINFO_FLAGS                     { 1,0,0,0,0,0 }
#define DEFAULT_BOARDINFO_CHASSIS                   L"Part Component"
#define DEFAULT_CHASSISINFO_MANUFACTURER            L"Intel Corporation"
#define DEFAULT_CHASSISINFO_VERSION                 L"0.1"
#define DEFAULT_CHASSISINFO_OEM                     { 0,0,0,0 }
#define DEFAULT_CHASSISINFO_HEIGHT                  0
#define DEFAULT_CHASSISINFO_POWER_CORDS_NUM         1
#define DEFAULT_PORT_INFO_INT_EXT_DESIGNATOR        L"None"
/*      Default Port Info                           { IsPublishRequired, InternalDesignator,                   InternalConnectorType,  ExternalDesignator,                   InternalConnectorType, PortType     } */
#define DEFAULT_PORT_INFO                           { Required,          DEFAULT_PORT_INFO_INT_EXT_DESIGNATOR, PortConnectorTypeNone,  DEFAULT_PORT_INFO_INT_EXT_DESIGNATOR, PortConnectorTypeNone, PortTypeNone }
#define DEFAULT_ONBOARD_DEV_INFO_DESCRIPTION        L"To Be Filled by O.E.M."
#define DEFAULT_CONFIG_OPTIONS_STRING               L"JUMPER:OPTIONS; NA:NA"
#define DEFAULT_BIOS_LANG_INFO_LANGUAGES            { L"enUS" }
#define DEFAULT_BIOS_LANG_INFO_INSTALL_LANGUAGES    MAX_LANGUAGES
#define DEFAULT_PORTABLE_BATTERY_LOCATION           L"Battery Location"
#define DEFAULT_PORTABLE_BATTERY_MANUFACTURER       L"Battery Manufacturer"
#define DEFAULT_PORTABLE_BATTERY_MANUFACTURER_DATE  L"Battery Manufacture Date"
#define DEFAULT_PORTABLE_BATTERY_SERIAL_NUMBER      L"Battery Serial Number"
#define DEFAULT_PORTABLE_BATTERY_DEVICE_NAME        L"Battery"
/*      Default Portable Battery Info               { IsPublishRequired, BatteryType, Location,            Manufacturer,            ManufactureDate,             SerialNumber,             DevName,    DevChemistry,                          DevCapacity, Voltage, SbdsVersion,     MaxErrorInBatteryData, SbdsSerialNumber, SbdsManufactureDate, SbdsDeviceChemistry,      CapacityMultiplier, Oem }*/
#define DEFAULT_PORTABLE_BATTERY_INFO               { Required,          Unknown,     L"Battery Location", L"Battery Manufacturer", L"Battery Manufacture Date", L"Battery Serial Number", L"Battery", PortableBatteryDeviceChemistryUnknown, 0,            0,      L"SBDS Version", 0,                     0,                0,                   L"SBDS Device Chemistry", 1,                  0,  }
/*      Default Cooling Device Info                 { IsPublishRequired, Type,                     Status,                     Group,  Oem, Speed,  Description       } */
#define DEFAULT_COOLING_DEVICE_INFO                 { Required,          CoolingDeviceTypeUnknown, CoolingDeviceStatusUnknown, 0x01,   0x0, 0x8000, L"Cooling Device" }
#define DEFAULT_COOLING_DEVICE_DESC                 L"Cooling Device"
#define TO_BE_FILLED_BY_OEM_STR                     L"To Be Filled by O.E.M."
#define DEFAULT_POWER_SUPPLY_INPUT_VOLT_PROBE       0
#define DEFAULT_POWER_SUPPLY_COOLING_DEVICE         0x0A
#define DEFAULT_POWER_SUPPLY_INPUT_CURR_PROBE       0
#define DEFAULT_POWER_SUPPLY_INFO_GROUP             0x1
#define DEFAULT_POWER_SUPPLY_INFO_POWER_CAPACITY    0x8000
/* Default Power Supply Characteristics             { Bit0 HotReplaceable, Bit1 Present, Bit2 Unplugged, Bit3-6 InputVoltageRangeSwitch, Bit7-9 Status,            Bit10-13 Type,          Bit14-15 Reserved } */
#define DEFAULT_POWER_SUPPLY_INFO_CHARACTERISTICS   { FALSE,               TRUE,         FALSE,          InputVoltRangeSwitchUnknown,    PowerSupplyStatusUnknown, PowerSupplyTypeUnknown, 0x0               }
/*      Default OnBoard Device Extended Info        { IsPublishRequired, Designation,                           DeviceType,                       DeviceStatus, DeviceInstance, Segment, Bus,  DevFunc } */
#define DEFAULT_ONBOARD_DEV_EXT_INFO                { Required,          DEFAULT_ONBOARD_DEV_INFO_DESCRIPTION,  OnBoardDeviceExtendedTypeUnknown, Enabled,      0,              0xFF,    0xFF, 0xFF    }
#define DEFAULT_SLOT_DESIGN                         L"PCI-Express #"    // Slot Designation
#define DEFAULT_EXTENDED_BIOS_ROM_SIZE              0
#define DEFAULT_EXTENDED_BIOS_ROM_UNIT              0

#define SIZEOF_SMBIOS_STRINGS(Type)                 (sizeof(SMBIOS_TABLE_TYPE ## Type ## _STRINGS) / sizeof (CHAR16*))

//
// Default Bios Vendor Info for SMBIOS Type 0
//
extern SMBIOS_TABLE_TYPE0  mDefaultBiosVendorInfo;

extern SMBIOS_TABLE_TYPE00_STRINGS  mDefaultBiosVendorStrings;

//
// Default System Info for SMBIOS Type 1
//
extern SMBIOS_TABLE_TYPE1 mDefaultSysInfo;

extern SMBIOS_TABLE_TYPE01_STRINGS  mDefaultSysStrings;


//
// Default Base Board info for RVP Board data
//
#define DEFAULT_BASE_BOARD_INFO  { DEFAULT_BOARDINFO_PRODUCTNAME, DEFAULT_BOARDINFO_MANUFACTURER, DEFAULT_BOARDINFO_VERSION, 0x1, BaseBoardTypeMotherBoard, DEFAULT_BOARDINFO_CHASSIS }
extern BASE_BOARD_INFO mDefaultBaseBoardInfo[];
//
// Default BaseBoard Info for SMBIOS Type 2
//
extern SMBIOS_TABLE_TYPE2 mDefaultBaseBoardsInfo;

extern SMBIOS_TABLE_TYPE02_STRINGS  mDefaultBaseBoardsStrings;


//
// Default Chassis info for RVP Board data
//
#define DEFAULT_CHASSIS_INFO { DEFAULT_CHASSISINFO_MANUFACTURER, MiscChassisTypeOther, DEFAULT_CHASSISINFO_VERSION, ChassisStateSafe, ChassisStateSafe, ChassisStateOther, ChassisSecurityStatusOther, DEFAULT_CHASSISINFO_HEIGHT, DEFAULT_CHASSISINFO_POWER_CORDS_NUM }
extern CHASSIS_INFO mDefaultChssisInfo [];
//
// Default Chassis Info for SMBIOS Type 3
//
extern SMBIOS_TABLE_TYPE3 mDefaultChssisInfoInfo;

extern SMBIOS_TABLE_TYPE03_STRINGS  mDefaultChssisInfoStrings;

//
// Default Port Info for SMBIOS Type 8
//

extern SMBIOS_TABLE_TYPE8 mDefaultPortConnectorsInfo;

extern SMBIOS_TABLE_TYPE8_STRINGS mDefaultPortConnectorsStrings;

//
// Default Supported Port Connectors for RVP Board data
//
#define DEFAULT_PORT_CONNECTORS_SUPPORT    0x1FFFFFFFFFF // Set from Bit0 to Bit 40 in order to enable all port connectors by default.

//
// Default System Config for SMBIOS Type 12
//

extern SMBIOS_TABLE_TYPE12 mDefaultSysConfigInfo;

extern SMBIOS_TABLE_TYPE12_STRINGS mDefaultSysConfigStrings;
//
// Default Language Info for SMBIOS Type 13
//

extern SMBIOS_TABLE_TYPE13 mDefaultBiosLangInfo;
extern SMBIOS_TABLE_TYPE13_STRINGS mDefaultBiosLangStrings[]; //CurrentLanguages
//
// Default Portable Battery Info for SMBIOS Type 22
//

extern SMBIOS_TABLE_TYPE22 mDefaultPortableBatteryInfo;
extern SMBIOS_TABLE_TYPE22_STRINGS  mDefaultPortableBatteryStrings;


//
// Default Cooling device info for RVP Board data
//
#define DEFAULT_COOLING_DEV_SUPPORT  (BIT0 | BIT1)
#define DEFAULT_COOLING_DEV          { CoolingDeviceTypeUnknown, L"Cooling Device" }

extern COOLING_DEV mDefaultCoolingDev [];
//
// Default Cooling Device Info for SMBIOS Type 27
//
extern SMBIOS_TABLE_TYPE27 mDefaultCoolingDeviceInfo;

extern SMBIOS_TABLE_TYPE27_STRINGS  mDefaultCoolingDeviceStrings;

//
// Default Boot Info for SMBIOS Type 32
//

extern SMBIOS_TABLE_TYPE32 mDefaultBootInfo;
//
// Default System Power Supply Info for SMBIOS Type 39
//

extern SMBIOS_TABLE_TYPE39 mDefaultPowerSupplyInfo;

extern SMBIOS_TABLE_TYPE39_STRINGS mDefaultPowerSupplyStrings;



//
// Default OnBoard Device info for RVP Board data
//
#define DEFAULT_ONBOARD_DEV_SUPPORT           (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7) // Enable all bits to support all On-board devices
#define DEFAULT_ONBOARD_DEV_ADDR              { 0, 0xFF, 0xFF, 0xFF }

extern ONBOARD_DEV_ADDR mDefaultOnboardDevAddr [];
//
// Default OnBoard Device Extended Info for SMBIOS Type 41
//
extern SMBIOS_TABLE_TYPE41 mDefaultOnBoardDeviceExtendedRecord;

extern SMBIOS_TABLE_TYPE41_STRINGS   mDefaultOnBoardDeviceExtendedStrings;

extern SMBIOS_TABLE_TYPE133_STRINGS  mDefaultOemType133Strings;

//
// Default OEM Type 133
//
extern SMBIOS_TABLE_TYPE133 mDefaultOemType133;

//
// The contain of this Product ID info is just for Intel RVP. The table values can be modified
// accordingly to OEM/Customer product.
//
extern SMBIOS_TABLE_TYPE133_WWAN mDefaultOemType133Wwan;

//
// Default OEM Type 136
//
extern SMBIOS_TABLE_TYPE136 mDefaultOemType136;

//
// Default System Slot Info for SMBIOS Type 9
//
extern SMBIOS_TABLE_TYPE9 mDefaultSystemSlotRecord;

extern SMBIOS_TABLE_TYPE9_STRINGS   mDefaultSystemSlotStrings;

//
// Default Smbios Platform Info table
//
#define DEFAULT_SMBIOS_PLATFORM_INFO_SIZE 16

extern SMBIOS_PLATFORM_INFO mDefaultSmbiosPlatformInfo[];
//
// Default Smbios Board info for RVP Board data
//
extern SMBIOS_PLATFORM_BOARD_INFO mDefaultSmbiosBoardInfo;
