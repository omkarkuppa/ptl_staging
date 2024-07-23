/** @file
      Common defines and definitions for a dxe driver.
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

#ifndef DEFINES_H
#define DEFINES_H

#include <Protocol/FirmwareManagement.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/PciIo.h>
#include <IndustryStandard/Pci.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <SetupVariable.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/I2cIo.h>
#include <Protocol/I2cPssProtocol.h>
#include <Include/MonzaPssChip.h>
#include <Library/IoLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PciSegmentLib.h>

#define DEBUG_INFO                                                0x00000040
#define GBE_FW_UPDATE_CTRL_EXT                                    0x00018
#define GBE_FW_UPDATE_CTRL_EXT_EE_RST                             0x00002000
#define GBE_FW_UPDATE_SHADOW_RAM_WORDS                            2048
#define GBE_FW_UPDATE_ICH_NVM_SIG_WORD                            0x13
#define GBE_FW_UPDATE_ICH_NVM_VALID_SIG_MASK                      0xC0
#define GBE_FW_UPDATE_ICH_NVM_SIG_VALUE                           0x80
#define GBE_FW_UPDATE_ICH_NVM_SIG_MASK                            0xC000
#define ICH_FLASH_LINEAR_ADDR_MASK                                0x00FFFFFF
#define ICH_FLASH_HSFSTS                                          0x0004
#define ICH_CYCLE_READ                                            0
#define ICH_CYCLE_WRITE                                           2
#define ICH_CYCLE_ERASE                                           3
#define ICH_FLASH_FADDR                                           0x0008
#define ICH_FLASH_READ_COMMAND_TIMEOUT                            10000000
#define ICH_FLASH_WRITE_COMMAND_TIMEOUT                           10000000
#define ICH_FLASH_FDATA0                                          0x0010
#define ICH_FLASH_CYCLE_REPEAT_COUNT                              10
#define ICH_FLASH_SEG_SIZE_256                                    256
#define ICH_FLASH_SEG_SIZE_4K                                     4096
#define ICH_FLASH_SEG_SIZE_8K                                     8192
#define ICH_FLASH_SEG_SIZE_64K                                    65536
#define EEPROM_SIZE_WORD                                          0x00
#define NVM_SUM                                                   0xBABA
#define NVM_CHECKSUM_REG                                          0x003F
#define NVM_SIZE_MULTIPLIER                                       4096
#define GBE_FW_UPDATE_FLASH_BASE_ADDR                             0xE000
#define ICH_FLASH_ERASE_COMMAND_TIMEOUT                           10000000
#define GBE_FW_UPDATE_STRAP                                       0x0000C
#define R_GBE_CFG_PM1                                             0x000000ccU      ///< PM1
#define B_PCI_BAR_MEMORY_TYPE_MASK                                (BIT1 | BIT2)
#define B_PCI_BAR_MEMORY_TYPE_64                                  BIT2

#define PSS_MONZA_SIGNATURE                                       SIGNATURE_32 ('P', 'S', 'S', 'M')
#define PSS_MONZA_VERSION                                         0x00000000
#define PSS_MONZA_CONTEXT_FROM_PROTOCOL(a)                        CR (a, PSS_MONZA_PRIVATE_CONTEXT, PssProtocol, PSS_MONZA_SIGNATURE)

#define R_GBE_MEM_RAL_0                                           0x00005400U      ///< RAL_0
#define R_GBE_MEM_RAH_0                                           0x00005404U      ///< RAH_0
#define B_GBE_MEM_RAH_0_RAH                                       0x0000ffffU

typedef  INT32 GBE_FW_UPDATE_STATUS;

//
// Error Codes
//
#define GBE_FW_UPDATE_SUCCESS                                     0
#define GBE_FW_UPDATE_INVALID_PARAMETER                           2

//
// ICH GbE Flash Hardware Sequencing Flash Status Register bit breakdown
// Offset 04h HSFSTS
//
union FLASH_STATUS {
  struct ICH8_HSFSTS {
    UINT16 FCDone     : 1; // bit 0 Flash Cycle Done
    UINT16 FCErr      : 1; // bit 1 Flash Cycle Error
    UINT16 DAEL       : 1; // bit 2 Direct Access error Log
    UINT16 berasesz   : 2; // bit 4:3 Sector Erase Size
    UINT16 FCInProg   : 1; // bit 5 flash cycle in Progress
    UINT16 reserved1  : 2; // bit 13:6 Reserved
    UINT16 reserved2  : 6; // bit 13:6 Reserved
    UINT16 FDescValid : 1; // bit 14 Flash Descriptor Valid
    UINT16 flockdn    : 1; // bit 15 Flash Config Lock-Down
  } HSF_STATUS;
  UINT16 RegVal;
};

//
// ICH GbE Flash Hardware Sequencing Flash control Register bit breakdown
// Offset 06h FLCTL
//
union FLASH_CTRL {
  struct ICH8_HSFLCTL {
    UINT16 FCGo     : 1;   // 0 Flash Cycle Go
    UINT16 FCycle   : 2;   // 2:1 Flash Cycle
    UINT16 reserved : 5;   // 7:3 Reserved
    UINT16 FDBCount : 2;   // 9:8 Flash Data Byte Count
    UINT16 flockdn  : 6;   // 15:10 Reserved
  } HSF_CTRL;
  UINT16 RegVal;
};

//
// PCI Data structure with PCI device details and hardware details.
//
typedef struct _PCI_CONFIG_HEADER {
  UINT16 VendorID;
  UINT16 DeviceID;
  UINT16 Command;
  UINT16 Status;
  UINT8  RevID;
  UINT8  ProgIF;
  UINT8  Subclass;
  UINT8  Class;
  UINT8  CacheLineSize;
  UINT8  LatencyTimer;
  UINT8  HeaderType;
  UINT8  BIST;
  UINT32 BaseAddressReg_0;
  UINT32 BaseAddressReg_1;
  UINT32 BaseAddressReg_2;
  UINT32 BaseAddressReg_3;
  UINT32 BaseAddressReg_4;
  UINT32 BaseAddressReg_5;
  UINT32 CardBusCISPtr;
  UINT16 SubVendorID;
  UINT16 SubSystemID;
  UINT32 ExpansionROMBaseAddr;
  UINT8  CapabilitiesPtr;
  UINT8  reserved1;
  UINT16 Reserved2;
  UINT32 Reserved3;
  UINT8  int_line;
  UINT8  int_pin;
  UINT8  Min_gnt;
  UINT8  Max_lat;
} PCI_CONFIG_HEADER;

//
// Gbe Hw details
//
typedef struct {
  EFI_HANDLE handle;
  EFI_FIRMWARE_MANAGEMENT_PROTOCOL FirmwareManagement;
  EFI_PCI_IO_PROTOCOL* PciIo;
  UINTN Signature;
  UINT32 PciConfig[MAX_PCI_CONFIG_LEN];
  struct GBE_FW_UPDATE_HW {
    void* Back;
    UINT8* HwAddr;
    UINT8* FlashAddr;
    UINT16 VendorId;
    UINT16 DeviceId;
    struct GBE_FW_UPDATE_SHADOW_RAM {
      UINT16 Value;
      BOOLEAN Modified;
    } ShadowRam[GBE_FW_UPDATE_SHADOW_RAM_WORDS];
    UINT32 FlashBankSize;
  } Hw;
} ADAPTER_INFO;
//
// List of suported devices for each platform
//
struct SUPPORTED_IDS {
  UINT16 DeviceId; // Supported Device ID
};

//
// PSS Monza details
//
typedef struct {
  UINTN                                 Signature;
  EFI_HANDLE                            ControllerHandle;
  EFI_UNICODE_STRING_TABLE              *ControllerNameTable;
  EFI_I2C_IO_PROTOCOL                   *I2cIoProtocol;
  I2C_PLATFORM_SECURE_STORAGE_PROTOCOL  PssProtocol;
} PSS_MONZA_PRIVATE_CONTEXT;

VOID WriteFlashReg (IN struct GBE_FW_UPDATE_HW* Hw, IN UINT32 Reg, IN UINT32 Data);
UINT32 ReadFlashReg (IN struct GBE_FW_UPDATE_HW* Hw, IN UINT32 Reg);
VOID WriteMacReg (IN struct GBE_FW_UPDATE_HW* Hw, IN UINT32 Reg, IN UINT32 Data);
UINT32 ReadMacReg (IN struct GBE_FW_UPDATE_HW* Hw, IN UINT32 Reg);
UINT16 ReadFlashReg16 (IN struct GBE_FW_UPDATE_HW* Hw, IN UINT32 Reg);
GBE_FW_UPDATE_STATUS ReadNVM (IN struct GBE_FW_UPDATE_HW* Hw, IN UINT16 Offset, IN UINT16 Words, OUT UINT16* Data);
GBE_FW_UPDATE_STATUS SaveNVMData (IN struct GBE_FW_UPDATE_HW* Hw, IN UINT16 Offset, IN UINT16 Words, OUT UINT16* Data);
UINT32 GetNVMSize (IN struct GBE_FW_UPDATE_HW* Hw);
EFI_STATUS WriteEepromImage (IN ADAPTER_INFO* Adapter,
  IN CONST UINT16* ImageToWrite,
  IN UINTN ImageToWriteSize,
  IN BOOLEAN WriteMacAddress);
GBE_FW_UPDATE_STATUS UpdateEepromChecksum (IN ADAPTER_INFO* Adapter);
GBE_FW_UPDATE_STATUS ReloadEeprom (IN  struct GBE_FW_UPDATE_HW* Hw);
BOOLEAN CheckSupported (IN UINT16 DeviceId);
VOID NumToString (IN UINT16 Num, OUT CHAR16 Vers[6]);
GBE_FW_UPDATE_STATUS UpdateNVM (IN struct GBE_FW_UPDATE_HW* Hw);
EFI_STATUS CheckValidImage (IN CONST VOID* Image);
EFI_STATUS ByteSwapping_call (IN UINT16  *MacData , IN UINT16 length, OUT UINT16 *Mac_NormalOrder);
#endif // DEFINES_H
