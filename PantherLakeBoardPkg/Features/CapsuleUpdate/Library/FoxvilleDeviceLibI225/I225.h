/** @file
  FmpDeviceLib instance to support Foxville I225/I226 update

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
  Foxville Ethernet Controller External Architecture Specification, version 0.99

**/

#ifndef I225_H
#define I225_H

#include <Uefi.h>
#include <Protocol/PciIo.h>

#define MAJORVERSION                                  1
#define MINORVERSION                                  0
#define DEV_SIGNATURE                                 SIGNATURE_32 ('P', 'R', '0', 'g')
#define DRIVER_ID_NAME                                L"UEFI x64 DXE CapsuleUpdate Driver"

#define IGC_ADAPTER_FROM_THIS(a)                      CR (a, IGC_ADAPTER, FirmwareManagement, DEV_SIGNATURE)

#ifndef LOBYTE
#define LOBYTE(_x) (UINT8)((_x)&0xFF)
#endif

#ifndef LOWORD
#define LOWORD(_x) ((UINT16)((_x)&0xFFFF))
#endif

#define MAKE_WORD(hi, low)                            ((UINT16) ((((UINT16)(hi)) << 8) | (low)))

#define IGC_FLSWCTL                                   0x12048
#define IGC_FLSWDATA                                  0x1204C
#define IGC_FLSWCNT                                   0x12050
#define IGC_FLFWUPDATE                                0x12108

#define IGC_FLSWCTL_CMD_WRITE                         (0x01000000)
#define IGC_FLSWCTL_CMD_SECTOR_ERASE                  (0x02000000)

#define IGC_FLFWUPDATE_AUTHEN_DONE                    0x20000000  /* Authentication cycle done */
#define IGC_FLFWUPDATE_UPDATE                         0x80000000  /* Request Flash Update */
#define IGC_FLFWUPDATE_AUT_FAIL                       0x40000000  /* Authentication failed */

#define IGC_FLSWCTL_DONE                              0x40000000
#define IGC_FLSWCTL_CMDV                              0x10000000
#define IGC_FLSWCTL_BUSY                              (1 << 29)

#define IGC_SHARED_FLASH_1M                           1024*1024
#define IGC_SHARED_FLASH_2M                           2*IGC_SHARED_FLASH_1M

#define IGC_NVM_VALIDITY_OFFSET                       0x25
#define IGC_SHARED_FLASH_VALID_EEPROM_BANK_SIGNATURE  0x40
#define IGC_SHARED_FLASH_EEPROM_BANK_SIGNATURE_MASK   0xC0

#define IGC_SHARED_FLASH_EEPROM_BANK_SIZE             4096

#define IGC_SE_FLASH_ROM_NULL                         0
#define IGC_SE_FLASH_PHY_SIZE                         131072
#define IGC_SE_FLASH_MAX_IMAGE_SIZE                   2048

#define IGC_FPA_PTR                                   0x0040
#define IGC_FPA_SIZE                                  0x0041

#define FLASH_ERASE_POLL                              100000

#define IGC_EECD                                      0x00010  /* EEPROM/Flash Control - RW */
#define IGC_EECD_FLASH_DETECTED                       0x00080000 /* FLASH detected */

#define IGC_EERD                                      0x12014  /* EEPROM Read - RW */
#define IGC_EEWR                                      0x12018  /* EEPROM Write Register - RW */

#define IGC_FLA                                       0x0001C  /* Flash Access - RW */
#define IGC_FLA_LOCKED                                0x00000040 /* Bit indicating that the NVM is in the Secure mode */

#define IGC_DEVICE_ID_WORD                            0xD
#define IGC_DEV_STARTER_WORD                          0x5
#define IGC_FW_MODULE_POINTER_OFFSET                  0x10
#define IGC_EXP_ROM_BOOT_CODE_SEC_PTR                 0x4A
#define IGC_PHY_POINTER                               0x7F0
#define IGC_SW_AREA_POINTER                           0x7F1

#define IGC_SE_FLASH_ROM_SIZE                         524288

#define IGC_FLASH_4K_SECTOR_SIZE                      0x1000
#define IGC_FLSW_MAX_FLASH_DWORDS_TO_WRITE            (256 >> 2)

#define IGC_NVM_RW_REG_DONE                           2    /* Offset to READ/WRITE done bit */
#define IGC_EECD_FLUPD_I225                           0x00800000 /* Update FLASH */
#define IGC_EECD_FLUDONE_I225                         0x04000000 /* Update FLASH done */
#define IGC_EECD_SEC1VAL_I225                         0x02000000 /* Sector One Valid */
#define IGC_FWSM_FW_VALID                             0x8000 /* FW valid bit */
#define IGC_NVM_RW_ADDR_SHIFT                         2    /* Shift to the address bits */
#define IGC_NVM_RW_REG_START                          1    /* Start operation */
#define IGC_SWFW_EEP_SM                               0x1
#define IGC_NVM_RW_REG_DATA                           16
#define IGC_NVM_GRANT_ATTEMPTS                        1000 /* NVM # attempts to gain grant */
#define IGC_I225_FLSECU                               0x12114 /* FLASH Security */
#define IGC_FLSECU_BLK_SW_ACCESS_I225                 0x00000004 /* Block Protected SW access */

#define NVM_CHECKSUM_REG                              0x003F
#define NVM_SUM                                       0xBABA

/* Shadow Ram Write Register - RW */
#define IGC_SRWR                                      0x12018
#define IGC_FWSM                                      0x05B54  /* FW Semaphore */

#define IGC_EEPROM_SIZE                               4096
#define IGC_FLASH_SIZE_WORD                           0x11
#define IGC_FLASH_SIZE_MASK                           0x7
#define IGC_FLASH_SIZE_BIT_1M                         0x4
#define IGC_FLASH_SIZE_BIT_2M                         0x5

#define IGC_FLASHOP                                   0x12054
#define IGC_S_SECT_ERASE                              0x20
#define IGC_CMD_ERASE                                 0xC7
#define IGC_SECTOR_SIZE                               4096
#define IGC_POINTER_4K_GRANULARITY                    0x8000

#define IGC_SPT_CAPABLE                               0x0200
#define IGC_MANAGEABILITY_MASK                        0x0700
#define IGC_FLASH_4K_SECTOR_MASK                      0xFFFFF000
#define IGC_FLSW_MAX_SECTORS_TO_ERASE                 4

#define IGC_SWSM                                      0x05B50 /* SW Semaphore */
#define IGC_SWSM_SMBI                                 0x00000001 /* Driver Semaphore bit */
#define IGC_SWSM_SWESMBI                              0x00000002 /* FW Semaphore bit */

#define IGC_SW_FW_SYNC                                0x05B5C /* SW-FW Synchronization - RW */
#define IGC_I225_ERASE_CMD_OPCODE                     0x02000000
#define IGC_I225_WRITE_CMD_OPCODE                     0x01000000
#define IGC_FLUDONE_ATTEMPTS                          20000

#define IGC_OPTION_ROM_PTR                            0x4A
#define IGC_SE_FLASH_ROM_NULL                         0
#define IGC_SECURED_MODULE_SIZE_WITH_OROM             1028096
#define IGC_SECURED_MODULE_SIZE_NO_OROM               503808

#define IGC_FW_MODULE_POINTER                         0x51
#define IGC_FW_COMMON_PARMS_OFFSET                    0x3
#define IGC_FW_SIDEBAND_CONFIG_OFFSET                 0x6
#define IGC_FW_FLEX_FILTERS_OFFSET                    0x7
#define IGC_FW_TRAFFIC_TYPES_OFFSET                   0xB
#define IGC_FW_MNG_MODE_OFFSET                        0x1
#define IGC_PT_LAN0_POINTER                           0x5

#define IGC_MDICNFG                                   0x0E04
#define IGC_MDIC                                      0x0020

#define IGC_MDI_WRITE_CMD                             0x04000000
#define IGC_MDI_READ_CMD                              0x08000000
#define IGC_MMD_ACC_CTRL                              0xD
#define IGC_MMD_ACC_DATA                              0xE
#define IGC_REG_MMD_SHIFT                             16

typedef INT32                                         IGC_STATUS;

/* Error codes */
#define IGC_SUCCESS                                   0x0
#define IGC_UNKOWN_FLASH_SIZE                         0x1
#define IGC_ERR_NVM                                   0x2
#define IGC_ERR_SWFW_SYNC                             0x3
#define IGC_ERR_INVALID_ARGUMENT                      0x4
#define IGC_DEVICE_ERROR                              0x5
#define WRITE_PROTECTED_REGION                        0x6
#define IGC_FLASH_FW_AUTHENTICATION_TIMEOUT           0x7
#define IGC_FLASH_AUTHENTICATION_FAILED               0x8
#define IGC_EEPROM_CALCULATION_FAILED                 0x9

typedef struct {
  UINT16    VendorID;
  UINT16    DeviceID;
  UINT16    Command;
  UINT16    Status;
  UINT8     RevID;
  UINT8     ProgIF;
  UINT8     Subclass;
  UINT8     Class;
  UINT8     CacheLineSize;
  UINT8     LatencyTimer;
  UINT8     HeaderType;
  UINT8     BIST;
  UINT32    BaseAddressReg_0;
  UINT32    BaseAddressReg_1;
  UINT32    BaseAddressReg_2;
  UINT32    BaseAddressReg_3;
  UINT32    BaseAddressReg_4;
  UINT32    BaseAddressReg_5;
  UINT32    CardBusCISPtr;
  UINT16    SubVendorID;
  UINT16    SubSystemID;
  UINT32    ExpansionROMBaseAddr;
  UINT8     CapabilitiesPtr;
  UINT8     reserved1;
  UINT16    Reserved2;
  UINT32    Reserved3;
  UINT8     int_line;
  UINT8     int_pin;
  UINT8     Min_gnt;
  UINT8     Max_lat;
} PCI_CONFIG_HEADER;

typedef enum _IGC_FIRMWARE_STATUS
{
  IGC_FIRMWARE_VALID   = 0,      /**< 0 - Valid */
  IGC_FIRMWARE_INVALID = 1,      /**< 1 - Invalid */
} IGC_FIRMWARE_STATUS;

typedef enum _FLASH_MODE
{
  PROTECTED_FLASH_MODE   = 0,        /**< 0 - Normal programming mode */
  UNPROTECTED_FLASH_MODE = 1,        /**< 1 - Unprotected Flash programming mode */
  NO_FLASH_DETECTED,                 /**< 2 - No flash detected */
  BLANK_FLASH_MODE,                  /**< 3 - Blank Flash programming mode */
} FLASH_MODE;

typedef enum _IGC_FLASH_MODULES
{
  IGC_FLASH_MODULE_SHADOW_RAM = 0,              /**< 0 - Shadow RAM */
  IGC_FLASH_MODULE_EMP_IMAGE,                   /**< 1 - Embedded Microprocessor Firmware Image */ /* keep it last, after this update FW reset will be done */
  IGC_FLASH_MODULE_1ST_FREE_AREA,               /**< 2 - First free provisioning area */
  IGC_FLASH_MODULE_SECURED_SETTINGS,            /**< 3 - Secured module pointer */
  IGC_FLASH_MODULE_OPTION_ROM
} IGC_FLASH_MODULES;

typedef struct {
  UINT32                      FlashSize;              /**< Real size of the equipped flash */
  FLASH_MODE                  FlashMode;
} IGC_FLASH_INFO;

typedef struct {
  VOID*           Back;
  UINT8*          HwAddr;
  UINT8*          FlashAddr;
  UINT16          VendorId;
  UINT16          DeviceId;
  BOOLEAN         clear_semaphore_once;
  IGC_FLASH_INFO  *FlashInfo;
  UINT32          FlashBankSize;
} IGC_HW;

typedef struct {
  UINTN                            Signature;
  EFI_PCI_IO_PROTOCOL              *PciIo;
  UINT32                           PciConfig[MAX_PCI_CONFIG_LEN];
  IGC_HW                           Hw;
} FOXVILLE_CORE_INSTANCE;

typedef struct {
  UINT16 DeviceId;    /* Supported Device ID */
} SupportedIDs;

/**
  Reads the current NVM version from the flash

  @param[in]  Hw                          - Pointer to hardware instance
  @param[out] NVMVersion                  - Pointer to be filled by the read version

  @retval     IGC_SUCCESS                 - NVM version was read successfully

**/
IGC_STATUS
IgcGetNVMVersion (
  IN IGC_HW   *Hw,
  OUT UINT16  *NVMVersion
 );

/**
  Write whole flash region in the flash

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Image                       - Pointer to the data to write
  @param[in]  ImageSize                   - Size of the data to write
  @param[in]  RegionOffset                - Starting offset of the region in the flash

  @retval     IGC_SUCCESS                 - Region was written successfully
  @retval     IGC_ERR_INVALID_ARGUMENT    - Given parameters are incorrect

**/
IGC_STATUS
WriteFlashRegion (
  IN IGC_HW *Hw,
  IN UINT8* Image,
  IN UINT32 ImageSize,
  IN UINT32 RegionOffset
 );

/**
  Writes the new image to the flash

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Image                       - Pointer to the new image
  @param[in]  ImageSize                   - The size of the new image
  @param[in]  WriteMacAddress             - Boolean value to determine whether saving the
                                            old MAC address is required

  @retval     IGC_SUCCESS                 - Writing the image to the flash was successful
  @retval     IGC_DEVICE_ERROR            - Not enough memory for the update/image is corrupted

**/
IGC_STATUS
WriteFlashImage (
  IN  IGC_HW      *Hw,
  IN  UINT8       *Image,
  IN  UINTN       ImageSize,
  IN  BOOLEAN     WriteMacAddress
 );

/**
  Performs GPHY FW reload.

  This function contains a series writes and reads via MMD MDIO interface to PHY
  registers that causes the GPHY FW to reset and reload. The written values
  are not significant and this needs to be considered as a black box.

  @param[in]  Hw                          - Pointer to hardware instance

  @retval     IGC_SUCCESS                 - Region was written successfully
  @retval     IGC_ERR_INVALID_ARGUMENT    - Given parameters are incorrect

**/
IGC_STATUS
IgcResetPHYFW (
  IN IGC_HW *Hw
 );

/**
  Getting the size of the flash 1/2Mb.

  @param[in]  Hw                          - Pointer to hardware instance

  @retval     Flash size                  - If the data in the MAC register is correct
  @retval     IGC_UNKOWN_FLASH_SIZE       - The size couldn't be identified as 1/2Mb

**/
UINT32
GetFlashSize (
  IN IGC_HW *Hw
 );

/**
  Identifies Flash programming mode by reading EECD and FLA registers.

  @param[in]  Hw                          - Pointer to hardware instance

  @retval     FLASH MODE                  - According to the MAC registers

**/
FLASH_MODE
GetFlashProgrammingMode (
  IN  IGC_HW *Hw
 );

/**
  This function writes to a register in the address space.

  @param[in] Hw                    - Pointer to hardware instance.
  @param[in] Reg                   - Which port to write to.
  @param[in] Data                  - Data to write to Port.

**/
VOID
WriteMacReg (
  IN IGC_HW *Hw,
  IN UINT32 Reg,
  IN UINT32 Data
 );

#endif /* I225_H */
