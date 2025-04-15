/** @file
  Definition of Foxville register.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef __FOXVILLE_REGS_H__
#define __FOXVILLE_REGS_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>

#pragma pack (push, 1)

//
// 0x00020 (RW): Media Dependent Interface (MDI) Control Register.
//
#define R_FOXVILLE_MDIC  0x00020

#define V_MDIC_REG_STD_MMD_CTRL  0x0D
#define V_MDIC_REG_STD_MMD_DATA  0x0E
#define V_MDIC_OP_CODE_WRITE     0x01
#define V_MDIC_OP_CODE_READ      0x02

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits 15: 0] Data used for the transaction.
    ///
    UINT32    Data                : 16;
    ///
    /// [Bits 20:16] PHY register address.
    ///
    UINT32    RegAddress          :  5;
    ///
    /// [Bits 25:21] Reserved field.
    ///
    UINT32    Reserved1           :  5;
    ///
    /// [Bits 27:26] Operation code.
    ///
    UINT32    OpCode              :  2;
    ///
    /// [Bits    28] Indicate the device at the end of the MDI transaction.
    ///
    UINT32    IsReady             :  1;
    ///
    /// [Bits    29] Indicate the interrupt is generated at the end of an MDI cycle.
    ///
    UINT32    MdiInterruptEnabled :  1;
    ///
    /// [Bits    30] Indicate the failure to complete an MDI read.
    ///              (Set by HW, only valid when IsReady bit is set.)
    ///
    UINT32    MdiError            :  1;
    ///
    /// [Bits    31] Reserved field.
    ///
    UINT32    Reserved2           :  1;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_MDIC_REGISTER;

//
// 0x00E04 (RW): MDC/MDIO Configuration Register
//
#define R_FOXVILLE_MDICNFG  0x00E04

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits 20: 0] Reserved field.
    ///
    UINT32    Reserved1       : 21;
    ///
    /// [Bits 25:21] Indicate the PHY address.
    ///
    UINT32    PhyAddress      :  5;
    ///
    /// [Bits    26] Indicate the clause 45 enabled.
    ///
    UINT32    Clause45Enabled :  1;
    ///
    /// [Bits 31:27] Reserved field.
    ///
    UINT32    Reserved2       :  5;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_MDICNFG_REGISTER;

//
// 0x05B50 (RW): Software Semaphore Register.
//
#define R_FOXVILLE_SWSM  0x05B50

#define V_SWSM_SMBI_CLEAR      0x0
#define V_SWSM_SMBI_SET        0x1
#define V_SWSM_SWE_SMBI_CLEAR  0x0
#define V_SWSM_SWE_SMBI_SET    0x1

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits     0] SW/SW semaphore bit.
    ///              Set by HW when this register is read by the SW device driver.
    ///              Clear when the host driver writes a 0b to it.
    ///
    UINT32    Smbi     :  1;
    ///
    /// [Bits     1] SW/HW semaphore bit.
    ///              Set only by the SW device driver (read only to FW).
    ///              SW device driver should set this bit and then read it to verify.
    ///              SW device driver can access SW_FW_SYNC register when set.
    ///
    UINT32    SweSmbi  :  1;
    ///
    /// [Bits 31: 2] Reserved field.
    ///
    UINT32    Reserved : 30;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_SWSM_REGISTER;

//
// 0x05B54 (Host-RO; FW-RW): Firmware Semaphore Register.
//
#define R_FOXVILLE_FWSM  0x05B54

#define V_FWSM_FW_VALID_INVALID  0x0
#define V_FWSM_FW_VALID_VALID    0x1

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits     0] SW/FW semaphore bit.
    ///              FW shall set this bit before accessing SW_FW_SYNC register.
    ///              FW shall clear this bit after modifying SW_FW_SYNC register.
    ///
    UINT32    FweSmbi                   :  1;
    ///
    /// [Bits  3: 1] Indicate the mode of firmware.
    ///
    UINT32    FwMode                    :  3;
    ///
    /// [Bits  5: 4] Reserved field.
    ///
    UINT32    Reserved1                 :  2;
    ///
    /// [Bits     6] Indicate the NVM reloaded.
    ///
    UINT32    NvmReloadIndicate         :  1;
    ///
    /// [Bits 14: 7] Reserved field.
    ///
    UINT32    Reserved2                 :  8;
    ///
    /// [Bits    15] Indicate the validity of firmware.
    ///
    UINT32    FwValid                   :  1;
    ///
    /// [Bits 18:16] Indicate the number of firmware reset.
    ///
    UINT32    ResetCounter              :  3;
    ///
    /// [Bits 24:19] Indicate the external error.
    ///
    UINT32    ExtErrorIndicate          :  6;
    ///
    /// [Bits    25] Indicate the PCIe configuration error.
    ///
    UINT32    PcieConfigErrorIndicate   :  1;
    ///
    /// [Bits 30:26] Reserved field.
    ///
    UINT32    Reserved3                 :  5;
    ///
    /// [Bits    31] Indicate the MAC address restored into factory value.
    ///
    UINT32    FactoryMacAddressRestored :  1;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_FWSM_REGISTER;

//
// 0x05B5C (RWM): Software-Firmware Synchronization.
//
#define R_FOXVILLE_SW_FW_SYNC  0x05B5C

#define V_SW_FW_SYNC_SW_FLASH_SM_CLEAR  0x0
#define V_SW_FW_SYNC_SW_FLASH_SM_SET    0x1
#define V_SW_FW_SYNC_FW_FLASH_SM_CLEAR  0x0
#define V_SW_FW_SYNC_FW_FLASH_SM_SET    0x1

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits     0] Indicate the NVM access is owned by SW.
    ///
    UINT32    SwFlashSm  :  1;
    ///
    /// [Bits     1] Indicate the PHY access is owned by SW.
    ///
    UINT32    SwPhySm    :  1;
    ///
    /// [Bits     2] Indicate the I2C access register set (I2CCMD) is owned by SW.
    ///
    UINT32    SwI2cSm    :  1;
    ///
    /// [Bits     3] Indicate the shared CSRs is owned by SW. (Not in use.)
    ///
    UINT32    SwMacCsrSm :  1;
    ///
    /// [Bits  6: 4] Reserved field.
    ///
    UINT32    Reserved1  :  3;
    ///
    /// [Bits     7] Indicate the SVR/LVR control registers are owned by SW.
    ///
    UINT32    SwSvrSm    :  1;
    ///
    /// [Bits     8] Indicate the SWMBWR mailbox write register is owned by SW.
    ///
    UINT32    SwMbSm     :  1;
    ///
    /// [Bits     9] Reserved field.
    ///
    UINT32    Reserved2  :  1;
    ///
    /// [Bits    10] Indicate the management host interface is owned by the port driver.
    ///
    UINT32    SwMngSm    :  1;
    ///
    /// [Bits 15:11] Reserved field.
    ///
    UINT32    Reserved3  :  5;
    //
    /// [Bits    16] Indicate the NVM access is owned by FW.
    ///
    UINT32    FwFlashSm  :  1;
    ///
    /// [Bits    17] Indicate the PHY access is owned by FW.
    ///
    UINT32    FwPhySm    :  1;
    ///
    /// [Bits    18] Indicate the I2C access register set (I2CCMD) is owned by FW.
    ///
    UINT32    FwI2cSm    :  1;
    ///
    /// [Bits    19] Indicate the shared CSRs is owned by FW. (Not in use.)
    ///
    UINT32    FwMacCsrSm :  1;
    ///
    /// [Bits 22:20] Reserved field.
    ///
    UINT32    Reserved4  :  3;
    ///
    /// [Bits    23] Indicate the SVR/LVR control registers are owned by FW.
    ///
    UINT32    FwSvrSm    :  1;
    ///
    /// [Bits 31:24] Reserved field.
    ///
    UINT32    Reserved5  :  8;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_SW_FW_SYNC_REGISTER;

//
// 0x12010 (RW): EEPROM-Mode Control Register
//
#define R_FOXVILLE_EEC  0x12010

#define V_EEC_FLASH_DETECTED_CLEAR     0x0
#define V_EEC_FLASH_DETECTED_SET       0x1
#define V_EEC_FLASH_UPDATE_CLEAR       0x0
#define V_EEC_FLASH_UPDATE_SET         0x1
#define V_EEC_FLASH_UPDATE_DONE_CLEAR  0x0
#define V_EEC_FLASH_UPDATE_DONE_SET    0x1
#define V_EEC_SECTOR_1_VALID_CLEAR     0x0
#define V_EEC_SECTOR_1_VALID_SET       0x1

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits  5: 0] Reserved field.
    ///
    UINT32    Reserved1       :  6;
    ///
    /// [Bits     6] Indicate NVM is present with a valid signature and
    ///              HW was programmed from the NVM.
    ///
    UINT32    FlashInUse      :  1;
    ///
    /// [Bits     7] Reserved field.
    ///
    UINT32    Reserved2       :  1;
    ///
    /// [Bits     8] Indicate Flash is presented and has the correct signature field,
    ///              and the shadow RAM contains the auto-load information.
    ///              (Valid only when AutoReadDone = 1.)
    ///
    UINT32    EepromPresented :  1;
    ///
    /// [Bits     9] Indicate NVM auto-read by hardware is done.
    ///              (Bit would be set even NVM is not found or signature is invalid.)
    ///
    UINT32    AutoReadDone    :  1;
    ///
    /// [Bits    10] Reserved field.
    ///
    UINT32    Reserved3       :  1;
    ///
    /// [Bits 14:11] Indicate the size of NVM accessible via EEPROM-Mode
    ///              (Encoded in power of 2 defined in K bit units.)
    ///
    UINT32    EepromSize      :  4;
    ///
    /// [Bits 18:15] Reserved field.
    ///
    UINT32    Reserved4       :  4;
    ///
    /// [Bits    19] Indicate the flash responded as not busy to a read status
    ///              and returned a manufacturer ID.
    ///
    UINT32    FlashDetected   :  1;
    ///
    /// [Bits 21:20] Reserved field.
    ///
    UINT32    Reserved5       :  2;
    ///
    /// [Bits    22] Indicate the shadow RAM was modified.
    ///
    UINT32    ShadowModified  :  1;
    ///
    /// [Bits    23] Set the bit into 1 to write the shadow RAM into flash device.
    ///              HW would issue an interrupt to FW.
    ///              Bit would be self-cleared immediately.
    ///
    UINT32    FlashUpdate     :  1;
    ///
    /// [Bits    24] Reserved field.
    ///
    UINT32    Reserved6       :  1;
    ///
    /// [Bits    25] Indicate the validity of sector 1.
    ///              (Only valid when EepromPresented = 1 and FlashInUse = 1.)
    ///
    UINT32    Sector1Valid    :  1;
    ///
    /// [Bits    26] Indicate the flash update process was initiated by setting FlashUpdate bit had completed.
    ///              HW would clear this bit when FlashUpdate is set by host.
    ///
    UINT32    FlashUpdateDone :  1;
    ///
    /// [Bits 31:27] Reserved field.
    ///
    UINT32    Reserved7       :  5;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_EEC_REGISTER;

//
// 0x12014 (RW): EEPROM-Mode Read Register.
//
#define R_FOXVILLE_EERD  0x12014

#define V_EERD_COMMAND_REJECTED  0x0
#define V_EERD_COMMAND_VALID     0x1
#define V_EERD_READ_IN_PROGRESS  0x0
#define V_EERD_READ_DONE         0x1

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits     0] Command valid. (Cleared by hardware in case of request was rejected.)
    ///
    UINT32    Valid    :  1;
    ///
    /// [Bits     1] Indicate the read done. (Writes by software are ignored.)
    ///
    UINT32    ReadDone :  1;
    ///
    /// [Bits 12: 2] Indicate the address of the word to read.
    ///
    UINT32    Address  : 11;
    ///
    /// [Bits 15:13] Reserved field.
    ///
    UINT32    Reserved :  3;
    ///
    /// [Bits 31:16] Data to be returned from shadow RAM.
    ///
    UINT32    Data     : 16;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_EERD_REGISTER;

//
// 0x12018 (RW): EEPROM-Mode Write Register.
//
#define R_FOXVILLE_EEWR  0x12018

#define V_EEWR_COMMAND_REJECTED   0x0
#define V_EEWR_COMMAND_VALID      0x1
#define V_EEWR_WRITE_IN_PROGRESS  0x0
#define V_EEWR_WRITE_DONE         0x1

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits     0] Command valid. (Cleared by hardware in case of request was rejected.)
    ///
    UINT32    Valid     :  1;
    ///
    /// [Bits     1] Indicate the write done. (Writes by software are ignored.)
    ///
    UINT32    WriteDone :  1;
    ///
    /// [Bits 12: 2] Indicate the address of the word to write.
    ///
    UINT32    Address   : 11;
    ///
    /// [Bits 15:13] Reserved field.
    ///
    UINT32    Reserved  :  3;
    ///
    /// [Bits 31:16] Data to be written into shadow RAM.
    ///
    UINT32    Data      : 16;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_EEWR_REGISTER;

//
// 0x1201C (RW): Flash Access Register.
//
#define R_FOXVILLE_FLA  0x1201C

#define V_FLA_LOCKED_CLEAR  0x0
#define V_FLA_LOCKED_SET    0x1

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits     0] Clock input to flash.
    ///
    UINT32    FlSck         :  1;
    ///
    /// [Bits     1] Chip select to flash.
    ///
    UINT32    FlCs          :  1;
    ///
    /// [Bits     2] Data input to flash.
    ///
    UINT32    FlSi          :  1;
    ///
    /// [Bits     3] Data output bit from flash.
    ///
    UINT32    FlSo          :  1;
    ///
    /// [Bits     4] Request flash access.
    ///
    UINT32    FlReq         :  1;
    ///
    /// [Bits     5] Grant flash access.
    ///
    UINT32    FlGnt         :  1;
    ///
    /// [Bits     6] Indicate the NVM is in Secure mode.
    ///
    UINT32    Locked        :  1;
    ///
    /// [Bits     7] Indicate the flash access was aborted due to the deadlock avoidance.
    ///              (Set by HW.)
    ///
    UINT32    FlaAbort      :  1;
    ///
    /// [Bits     8] Clear flash access error.
    ///
    UINT32    FlaClrErr     :  1;
    ///
    /// [Bits     9] Value of the SPI Request signal from the MAC to the PHY during idle.
    ///
    UINT32    SpiReq        :  1;
    ///
    /// [Bits    10] Chicken-Bit for flsh_si_e transition not aligned with flsh_sck_e (delayed).
    ///
    UINT32    FlaClkAlignCb :  1;
    ///
    /// [Bits    11] Chicken-Bit for RTL is not sampling right data coming from FLASH memory in MODE_2_5G.
    ///
    UINT32    Hold2ActiveCb :  1;
    ///
    /// [Bits 13:12] Shfit value of SPI_SI.
    ///
    UINT32    SoShiftVal    :  2;
    ///
    /// [Bits 15:14] Shift value of SPI_SO.
    ///
    UINT32    SiShiftVal    :  2;
    ///
    /// [Bits    16] Indicate the flash is in a sector erase cycle.
    ///
    UINT32    Eip           :  1;
    ///
    /// [Bits 19:17] Value of flash size.
    ///
    UINT32    FlSize        :  3;
    ///
    /// [Bits 27:20] Reserved field.
    ///
    UINT32    Reserved1     :  8;
    ///
    /// [Bits    28] Host memory BAR or expansion ROM BAR flash access error.
    ///
    UINT32    BarErr        :  1;
    ///
    /// [Bits    29] Indicate the flash is busy processing a flash transaction.
    ///
    UINT32    FlBusy        :  1;
    ///
    /// [Bits    30] Indicate the BAR write done. (0 indicate it is done.)
    ///
    UINT32    FlBarBusy     :  1;
    ///
    /// [Bits    31] Reserved field.
    ///
    UINT32    Reserved2     :  1;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_FLA_REGISTER;

//
// 0x12048 (RW): Software Flash Control Register.
//
#define R_FOXVILLE_FLSWCTL  0x12048

#define V_FLSWCTL_ADDRESS_MAX           0x00FFFFFF
#define V_FLSWCTL_BURST_BYTES_SIZE_MAX  256

#define V_FLSWCTL_CMD_WRITE                  0x1
#define V_FLSWCTL_CMD_FLASH_SECTOR_4K_ERASE  0x2
#define V_FLSWCTL_CMD_VALID_CLEAR            0x0
#define V_FLSWCTL_CMD_VALID_SET              0x1
#define V_FLSWCTL_FLASH_BUSY_CLEAR           0x0
#define V_FLSWCTL_FLASH_BUSY_SET             0x1
#define V_FLSWCTL_DONE_CLEAR                 0x0
#define V_FLSWCTL_DONE_SET                   0x1

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits 23: 0] Indicate the flash address in byte.
    ///
    UINT32    Address    : 24;
    ///
    /// [Bits 27:24] Indicate the command to be executed.
    ///
    UINT32    Cmd        :  4;
    ///
    /// [Bits    28] Indicate the validity of last command.
    ///
    UINT32    CmdValid   :  1;
    ///
    /// [Bits    29] Indicate the flash is busy processing a flash transaction.
    ///
    UINT32    FlashBusy  :  1;
    ///
    /// [Bits    30] Indicate the single flash transaction done.
    ///
    UINT32    Done       :  1;
    ///
    /// [Bits    31] Indicate all the flash transactions issued to the flash device.
    ///              Bit is cleared when SW written into the FLSWCTL register.
    ///
    UINT32    GlobalDone :  1;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_FLSWCTL_REGISTER;

//
// 0x1204C (RW): Software Flash Burst Data Register.
//
#define R_FOXVILLE_FLSWDATA  0x1204C

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    //
    // [Bits  7: 0] Byte 0 of data used to read from NVM or write into NVM.
    //              (Last valid byte if not aligned with multiple of 4.)
    //
    UINT32    Byte0 :  8;
    //
    // [Bits 15: 8] Byte 1 of data used to read from NVM or write into NVM.
    //
    UINT32    Byte1 :  8;
    //
    // [Bits 23:16] Byte 2 of data used to read from NVM or write into NVM.
    //
    UINT32    Byte2 :  8;
    //
    // [Bits 31:24] Byte 3 of data used to read from NVM or write into NVM.
    //
    UINT32    Byte3 :  8;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_FLSWDATA_REGISTER;

//
// 0x12050 (RW): Software Flash Burst Access Counter Register.
//
#define R_FOXVILLE_FLSWCNT  0x12050

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits 12: 0] Indicate the size in bytes of the flash burst read or write.
    ///
    UINT32    Count    : 13;
    ///
    /// [Bits 31:13] Reserved field.
    ///
    UINT32    Reserved : 19;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_FLSWCNT_REGISTER;

//
// 0x12054 (RO in Secured Mode): Flash Op-code Register.
//
#define R_FOXVILLE_FLASHOP  0x12054

#define FLASHOP_WRITE_ENABLE               0x06
#define FLASHOP_READ_JEDEC_MANUFACTURE_ID  0x9F
#define FLASHOP_WRITE_STATUS               0x01
#define FLASHOP_FLASH_READ                 0x03
#define FLASHOP_READ_FLASH_STATUS          0x05
#define FLASHOP_SECTOR_ERASE               0x20
#define FLASHOP_FLASH_ERASE                0xC7
#define FLASHOP_PAGE_PROGRAM               0x02
#define FLASHOP_FAST_READ                  0x0B

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits  7: 0] Hold the op-code for erasing the entire Flash device.
    ///
    UINT32    FlashEraseOp :  8;
    ///
    /// [Bits 15: 8] Hold the op-code for suspending the program/erase operation in the Flash.
    ///
    UINT32    SuspendOp    :  8;
    ///
    /// [Bits 23:16] Hold the op-code for resuming the program/erase operation that was suspended.
    ///
    UINT32    ResumeOp     :  8;
    ///
    /// [Bits 31:17] Hold the op-code that is issued in a Read command when Fast Read Mode Is set.
    ///
    UINT32    FastReadOp   :  8;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_FLASHOP_REGISTER;

//
// 0x12108 (RW): Flash Firmware Code Update Register.
//
#define R_FOXVILLE_FLFWUPDATE  0x12108

#define V_FLFWUPDATE_AUTH_DONE_CLEAR    0x0
#define V_FLFWUPDATE_AUTH_DONE_SET      0x1
#define V_FLFWUPDATE_AUTH_FAILED_CLEAR  0x0
#define V_FLFWUPDATE_AUTH_FAILED_SET    0x1
#define V_FLFWUPDATE_UPDATE_CLEAR       0x0
#define V_FLFWUPDATE_UPDATE_SET         0x1

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits 11: 0] Write by host when authentication done.
    ///
    UINT32    NewFwAdd   : 12;
    ///
    /// [Bits 28:12] Reserved field.
    ///
    UINT32    Reserved   : 17;
    ///
    /// [Bits    29] Indicate the authentication cycle done.
    ///              (Self-clear once Update field request is set 1.)
    ///
    UINT32    AuthDone   :  1;
    ///
    /// [Bits    30] Indicate the authentication failed.
    ///
    UINT32    AuthFailed :  1;
    ///
    /// [Bits    31] Request authentication of the new secure section written.
    ///              (This bit is self-cleared, always read as 0.)
    ///
    UINT32    Update     :  1;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_FLFWUPDATE_REGISTER;

//
// 0x12114 (Host-RO; FW-RW): Flash Security Register.
//
#define R_FOXVILLE_FLSECU  0x12114

#define V_FLSECU_BLOCK_PROTECTED_SW_ACCESS_CLEAR  0x0
#define V_FLSECU_BLOCK_PROTECTED_SW_ACCESS_SET    0x1

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits     0] Reserved field.
    ///
    UINT32    Reserved1              :  1;
    ///
    /// [Bits     1]
    ///
    UINT32    BlockAllSwAccess       :  1;
    ///
    /// [Bits     2]
    ///
    UINT32    BlockProtectedSwAccess :  1;
    ///
    /// [Bits 31: 3] Reserved field.
    ///
    UINT32    Reserved2              : 29;
  } Bits;
  ///
  /// All bit fields as a 32-bit value.
  ///
  UINT32    Uint32;
} FOXVILLE_FLSECU_REGISTER;

#pragma pack (pop)

#endif
