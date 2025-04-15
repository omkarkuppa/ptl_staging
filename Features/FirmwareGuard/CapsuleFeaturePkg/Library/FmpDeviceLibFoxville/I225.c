/** @file
  FoxvilleDeviceLib instance to support Foxville I225/I226 update

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/Pci.h>
#include "FoxvilleNvmMap.h"
#include "I225.h"

//  CRC table of the Foxville device firmware. This is for the integrity check of the firmware image.
const UINT8 Global_CrcTable[] = {
  0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, 0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,
  0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65, 0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d,
  0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5, 0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
  0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85, 0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd,
  0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2, 0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea,
  0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2, 0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
  0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32, 0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a,
  0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42, 0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a,
  0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c, 0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
  0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec, 0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4,
  0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c, 0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44,
  0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c, 0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
  0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b, 0x76, 0x71, 0x78, 0x7f, 0x6a, 0x6d, 0x64, 0x63,
  0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b, 0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13,
  0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb, 0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
  0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb, 0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3
};

/**
  Updates secured flash module

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  ModuleOffset                - The offset of the module in the flash
  @param[in]  Buffer                      - A struct to read data from
  @param[in]  BufferSize                  - The size of the data to update

  @retval     IGC_SUCCESS                 - Updating the secured module was successful
  @retval     IGC_DEVICE_ERROR            - Required memory allocation failed
  @retval     IGC_ERR_INVALID_ARGUMENT    - The offset parameters are incorrect

**/
IGC_STATUS
UpdateSecuredModule (
  IN IGC_HW              *Hw,
  IN UINT32              ModuleOffset,
  IN UINT8               *Buffer,
  IN UINT32              BufferSize
 );

/**
  Writes an image to the EEPROM

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  ImageToWrite                - Pointer to the data to write
  @param[in]  ImageToWriteSize            - Size of the data to write
  @param[in]  WriteMacAddress             - Boolean value whether to update the MAC address

  @retval     IGC_SUCCESS                 - EEPROM was written successfully
  @retval     IGC_ERR_INVALID_ARGUMENT    - Given parameters are incorrect

**/
IGC_STATUS
WriteEepromImage (
  IN     IGC_HW             *Hw,
  IN     UINT16             *ImageToWrite,
  IN     UINT32             ImageToWriteSize,
  IN     BOOLEAN            WriteMacAddress
 );

/**
  Updates all the MNG sections CRCs

  @param[in]  Hw                              - Pointer to hardware instance

  @retval     IGC_SUCCESS                     - CRCs were updated successfully
  @retval     IGC_EEPROM_CALCULATION_FAILED   - Calculation of the CRC failed
  @retval     IGC_DEVICE_ERROR                - Couldn't read/write the NVM word

**/
IGC_STATUS
UpdateMngCrcs (
  IN  IGC_HW      *Hw
 );

/**
  This function calls the MemIo callback to read a DWord from the device's
  address space.

  @param[in]  Hw            - Pointer to hardware instance.
  @param[in]  Reg           - Which register to read from.

  @retval     Results       - The Data read from the register.

**/
UINT32
ReadMacReg (
  IN IGC_HW    *Hw,
  IN UINT32    reg
 )
{
  UINT32                      Results;
  FOXVILLE_CORE_INSTANCE      *PrivateData;

  PrivateData = Hw->Back;

  MemoryFence ();

  PrivateData->PciIo->Mem.Read (
                        PrivateData->PciIo,
                        EfiPciIoWidthUint32,
                        0,
                        reg,
                        1,
                        (VOID*) &Results
                        );

  MemoryFence ();

  return Results;
}

/**
  This function writes to a register in the address space.

  @param[in] Hw                    - Pointer to hardware instance.
  @param[in] Reg                   - Which port to write to.
  @param[in] Data                  - Data to write to Port.

**/
VOID
WriteMacReg (
  IN IGC_HW    *Hw,
  IN UINT32    Reg,
  IN UINT32    Data
 )
{
  UINT32                      Value;
  FOXVILLE_CORE_INSTANCE      *PrivateData;

  PrivateData = Hw->Back;
  Value       = Data;

  MemoryFence ();

  PrivateData->PciIo->Mem.Write (
                        PrivateData->PciIo,
                        EfiPciIoWidthUint32,
                        0,
                        Reg,
                        1,
                        (VOID*) &Value
                        );

  MemoryFence();

  return;
}

/**
  This function reads data from a register in the flash space.

  @param[in] Hw               - Pointer to hardware instance.
  @param[in] Reg              - Which port to read from.

  @retval    Results          - The Data read from the port.

**/
UINT32
ReadFlashReg (
  IN IGC_HW    *Hw,
  IN UINT32    reg
 )
{
  UINT32                      Results;
  FOXVILLE_CORE_INSTANCE      *PrivateData;

  PrivateData = Hw->Back;

  MemoryFence ();

  PrivateData->PciIo->Mem.Read (
                        PrivateData->PciIo,
                        EfiPciIoWidthUint32,
                        1,
                        reg,
                        1,
                        (VOID*) &Results
                        );

  MemoryFence();

  return Results;
}

/**
  This function writes to a register in the flash space.

  @param[in]  Hw                - Pointer to hardware instance.
  @param[in]  Data              - Data to write to Port.
  @param[in]  Reg               - Which register to write to.

  @retval     none

**/
VOID
WriteFlashReg (
  IN IGC_HW    *Hw,
  IN UINT32    Reg,
  IN UINT32    Data
 )
{
  UINT32                            Value;
  FOXVILLE_CORE_INSTANCE      *PrivateData;

  PrivateData = Hw->Back;
  Value       = Data;

  MemoryFence();

  PrivateData->PciIo->Mem.Write (
                        PrivateData->PciIo,
                        EfiPciIoWidthUint32,
                        1,
                        Reg,
                        1,
                        (VOID*) &Value
                        );

  MemoryFence();

  return;
}

/**
  This function reads a register in the PHY via MMD interface.
  The MMD access via MAC registers is built by combining the OPCODE (bits 26:27),
  The register (bits 16:20), and the data to write (bits 0:15).

  In the capsule code the read function is a part of the reload GPHY FW flow
  and the read value is not important for us, this is why we don't return
  anything from this function.

  @param[in]  Hw                - Pointer to hardware instance.
  @param[in]  Reg               - Which register to write to.
  @param[in]  Data              - Data to write to register.

  @retval     none

**/
VOID
ReadPhyMmdReg (
  IN IGC_HW    *Hw,
  IN UINT16    Reg,
  IN UINT16    Data
 )
{
  UINT32 Value;

  Value = IGC_MDI_READ_CMD | (Reg << IGC_REG_MMD_SHIFT) | Data;

  WriteMacReg(Hw, IGC_MDIC, Value);
  gBS->Stall (100);
}

/**
  This function write a register in the PHY via MMD interface.
  The MMD access via MAC registers is built by combining the OPCODE (bits 26:27),
  The register (bits 16:20), and the data to write (bits 0:15).

  @param[in]  Hw                - Pointer to hardware instance.
  @param[in]  Reg               - Which register to write to.
  @param[in]  Data              - Data to write to register.

  @retval     none

**/
VOID
WritePhyMmdReg (
  IN IGC_HW *Hw,
  IN UINT16 Reg,
  IN UINT16 Data
 )
{
  UINT32 Value;

  Value = IGC_MDI_WRITE_CMD | (Reg << IGC_REG_MMD_SHIFT) | Data;

  WriteMacReg (Hw, IGC_MDIC, Value);
  gBS->Stall (100);
}

/**
  Releases hardware semaphore to access the NVM

  @param[in]  Hw              - Pointer to hardware instance.

  @retval     none

**/
VOID
ReleaseHwSemaphore (
  IN IGC_HW *Hw
 )
{
  UINT32 Swsm;

  Swsm = ReadMacReg (Hw, IGC_SWSM);
  Swsm &= ~(IGC_SWSM_SMBI | IGC_SWSM_SWESMBI);
  WriteMacReg (Hw, IGC_SWSM, Swsm);
}

/**
  Acquire hardware semaphore to access the NVM.

  @param[in]  Hw              - Pointer to hardware instance.

  @retval     IGC_SUCCESS     - The semaphore was successfully acquired.
  @retval     IGC_ERR_NVM     - The semaphore wasn't acquired.

**/
STATIC
IGC_STATUS
AcquireHwSemaphore (
  IN IGC_HW    *Hw
 )
{
  UINT32 Swsm;
  INT32  Timeout;
  INT32  Times;

  Timeout = (1 >> 15) + 1;
  Times = 0;

  /* Get the SW semaphore */
  while (Times < Timeout) {
    Swsm = ReadMacReg (Hw, IGC_SWSM);
    if (!(Swsm & IGC_SWSM_SMBI)) {
      break;
    }

    gBS->Stall (50);
    Times++;
  }

  /* If we do not have the semaphore here, we have to give up. */
  if (Times == Timeout) {
    DEBUG ((DEBUG_ERROR, "Driver can't access device -\n"));
    return -IGC_ERR_NVM;
  }

  /* Get the FW semaphore. */
  for (Times = 0; Times < Timeout; Times++) {
    Swsm = ReadMacReg (Hw, IGC_SWSM);
    WriteMacReg (Hw, IGC_SWSM, Swsm | IGC_SWSM_SWESMBI);

    /* Semaphore acquired if bit latched */
    if (ReadMacReg (Hw, IGC_SWSM) & IGC_SWSM_SWESMBI) {
      break;
    }
    gBS->Stall (50);
  }

  if (Times == Timeout) {
    /* Release semaphores */
    ReleaseHwSemaphore (Hw);
    DEBUG ((DEBUG_ERROR, "Driver can't access the NVM\n"));
    return -IGC_ERR_NVM;
  }

  return IGC_SUCCESS;
}

/**
  Acquire SW/FW semaphore to access the PHY or NVM.

  @param[in]  Hw                  - Pointer to hardware instance.

  @retval     IGC_SUCCESS         - The SWFW semaphore was acquired successfully.
  @retval     IGC_ERR_SWFW_SYNC   - The semaphore couldn't be acquired.

**/
IGC_STATUS
AcquireSwFwSemaphore (
  IN IGC_HW    *Hw
 )
{
  UINT32      SwFwSync;
  UINT32      FwMask;
  IGC_STATUS  Status;
  INT32       Times;
  INT32       Timeout;

  FwMask      = IGC_SWFW_EEP_SM << 16;
  Status      = IGC_SUCCESS;
  Times       = 0;
  Timeout     = 200;

  while (Times < Timeout) {
    if (AcquireHwSemaphore (Hw)) {
      Status = -IGC_ERR_SWFW_SYNC;
      goto AcquireSwFwSemaphoreExit;
    }

    SwFwSync = ReadMacReg (Hw, IGC_SW_FW_SYNC);
    if (!(SwFwSync & (FwMask | IGC_SWFW_EEP_SM))) {
      break;
    }

    /* Firmware currently using resource (FwMask)
      * or other software thread using resource (IGC_SWFW_EEP_SM)
      */
    ReleaseHwSemaphore (Hw);
    gBS->Stall (5 * 1000);
    Times++;
  }

  if (Times == Timeout) {
    DEBUG ((DEBUG_ERROR, "Driver can't access resource, SW_FW_SYNC timeout.\n"));
    Status = -IGC_ERR_SWFW_SYNC;
    goto AcquireSwFwSemaphoreExit;
  }

  SwFwSync |= IGC_SWFW_EEP_SM;
  WriteMacReg (Hw, IGC_SW_FW_SYNC, SwFwSync);
  ReleaseHwSemaphore (Hw);

AcquireSwFwSemaphoreExit:
  return Status;
}

/**
  Release SW/FW semaphore

  @param[in]  Hw                  - Pointer to hardware instance.

  @retval     none

**/
VOID
ReleaseSwFw (
    IN IGC_HW    *Hw
 )
{
  UINT32 SwFwSync;

  while (TRUE) {
    if (AcquireHwSemaphore (Hw) == IGC_SUCCESS) {
      break;
    }
  }

  SwFwSync = ReadMacReg (Hw, IGC_SW_FW_SYNC);
  SwFwSync &= ~IGC_SWFW_EEP_SM;
  WriteMacReg (Hw, IGC_SW_FW_SYNC, SwFwSync);
  ReleaseHwSemaphore (Hw);
}

/**
  Poll FLUDONE status

  @param[in]  Hw                  - Pointer to hardware instance.

  @retval     IGC_SUCCESS         - FLUDONE was received successfully

**/
IGC_STATUS
PollFlashUpdateDone (
  IN IGC_HW *Hw
 )
{
  IGC_STATUS Status;
  UINT32     Index;
  UINT32     Reg;

  Status = -IGC_ERR_NVM;

  for (Index = 0; Index < IGC_FLUDONE_ATTEMPTS; Index++) {
    Reg = ReadMacReg (Hw, IGC_EECD);
    if (Reg & IGC_EECD_FLUDONE_I225) {
      Status = IGC_SUCCESS;
      break;
    }
    gBS->Stall (5);
  }

  return Status;
}

/**
  Polls for EE Read done.

  @param[in]  Hw                  - Pointer to hardware instance.

  @retval     IGC_SUCCESS         - EERD bit was raised Successfully
  @retval     IGC_ERR_NVM         - Timeout occurred while waiting for EERD signal

**/
IGC_STATUS
PollEerdDone (
  IN IGC_HW *Hw
 )
{
  UINT32 Attempts;
  UINT32 Index;
  UINT32 Reg;

  Attempts = 100000;
  Reg = 0;

  for (Index = 0; Index < Attempts; Index++) {
    Reg = ReadMacReg (Hw, IGC_EERD);
    if (Reg & IGC_NVM_RW_REG_DONE) {
      return IGC_SUCCESS;
    }
    gBS->Stall (5);
  }

  return -IGC_ERR_NVM;
}

/**
  Writes data to Shadow Ram at offset using EEWR register.

  @param[in]  Hw                  - Pointer to hardware instance.
  @param[in]  Offset              - offset within the Shadow Ram to be written to
  @param[in]  Data                - 16 bit word(s) to be written to the Shadow Ram
  @param[in]  Semaphore           - a boolean value if acquiring a semaphore is required

  @retval     IGC_SUCCESS         - The write was done
  @retval     IGC_ERR_NVM         - Either the parameters to this function is out of bounds,
                                    or a timeout occurred while waiting for the write command
                                    to be completed.

**/
IGC_STATUS
WriteNvmSrwr (
  IN IGC_HW     *Hw,
  IN UINT32     Offset,
  IN UINT16     *Data,
  IN BOOLEAN    Semaphore
 )
{
  UINT32     Index;
  UINT32     Eewr;
  UINT32     Attempts;
  IGC_STATUS Status;
  UINT32     WordSize;

  Eewr     = 0;
  Attempts = 100000;
  Status   = IGC_ERR_NVM;
  WordSize = Hw->FlashInfo->FlashSize / 16;

  if (Semaphore) {
    Status = AcquireSwFwSemaphore (Hw);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteNvmSrwr: Failed to acquire the SW/FW semaphore\n"));
      return Status;
    }
  }

  /* A check for invalid values:  offset too large, too many words,
    * too many words for the offset, and not enough words.
    */
  if (Offset >= WordSize) {
    DEBUG ((DEBUG_ERROR, "NVM parameter(s) out of bounds\n"));
    goto WriteNvmSrwrExit;
  }

  Eewr = (Offset << IGC_NVM_RW_ADDR_SHIFT) | (*Data << IGC_NVM_RW_REG_DATA) | IGC_NVM_RW_REG_START;

  WriteMacReg (Hw, IGC_SRWR, Eewr);

  for (Index = 0; Index < Attempts; Index++) {
    if (IGC_NVM_RW_REG_DONE & ReadMacReg (Hw, IGC_SRWR)) {
      Status = IGC_SUCCESS;
      break;
    }
    gBS->Stall (5);
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Shadow RAM write EEWR timed out\n"));
  }

WriteNvmSrwrExit:
  if (Semaphore) {
    ReleaseSwFw (Hw);
  }

  return Status;
}

/**
  Reads EEPROM using EERD register

  @param[in]  Hw                  - Pointer to hardware instance.
  @param[in]  Offset              - offset within the Shadow Ram to be written to
  @param[in]  Data                - 16 bit word(s) to be written to the Shadow Ram
  @param[in]  Semaphore           - a boolean value to check if a semaphore is required

  @retval     IGC_SUCCESS         - Reading the word from the EEPROM was done successfully.
  @retval     IGC_ERR_NVM         - Couldn't read the data from the EEPROM offset,
                                    or the offset is out of bounds.

**/
IGC_STATUS
ReadNvmEerd (
  IN  IGC_HW     *Hw,
  IN  UINT16     Offset,
  OUT UINT16     *Data,
  IN  BOOLEAN    Semaphore
 )
{
  UINT32     Eerd;
  IGC_STATUS Status;

  Eerd = 0;
  Status = IGC_SUCCESS;

  if (Semaphore) {
    if (AcquireSwFwSemaphore (Hw) == IGC_SUCCESS) {
      /* A check for invalid values:  offset too large, too many words,
      * too many words for the offset, and not enough words.
      */
      if (Offset >= IGC_EEPROM_SIZE) {
          DEBUG ((DEBUG_ERROR, "NVM parameter(s) out of bounds\n"));
          return -IGC_ERR_NVM;
      }
    }
    else {
      DEBUG ((DEBUG_ERROR, "Couldn't acquire SWFW semaphore\n"));
      return IGC_ERR_SWFW_SYNC;
    }
  }

  Eerd = (Offset << IGC_NVM_RW_ADDR_SHIFT) + IGC_NVM_RW_REG_START;
  WriteMacReg (Hw, IGC_EERD, Eerd);
  Status = PollEerdDone (Hw);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Data = (ReadMacReg (Hw, IGC_EERD) >> IGC_NVM_RW_REG_DATA);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NVM read error: %d\n", Status));
  }

  if (Semaphore) {
    ReleaseSwFw (Hw);
  }

  return Status;
}

/**
    write/erase to a sector region on a given address.

  @param[in]  Hw                  - Pointer to hardware instance.
  @param[in]  Opcode              - opcode to be used for the write command.
  @param[in]  Address             - the offset to write into the FLASH image/

  @retval     IGC_SUCCESS         - Write/erase to a sector region succeeded.
  @retval     IGC_ERR_NVM         - Flash transaction failed.

**/
IGC_STATUS
SendFlashCommand (
  IN IGC_HW    *Hw,
  IN UINT32    Opcode,
  IN UINT32    Address
 )
{
  UINT32      Flswctl;
  UINT32      Timeout;
  IGC_STATUS  Status;

  Flswctl = 0;
  Timeout = IGC_NVM_GRANT_ATTEMPTS;
  Status  = IGC_SUCCESS;

  Flswctl = ReadMacReg (Hw, IGC_FLSWCTL);
  /* Polling done bit on FLSWCTL register */
  while (Timeout) {
    if (Flswctl & IGC_FLSWCTL_DONE) {
      break;
    }
    gBS->Stall (5);
    Flswctl = ReadMacReg (Hw, IGC_FLSWCTL);
    Timeout--;
  }

  if (!Timeout) {
    DEBUG ((DEBUG_ERROR, "Flash transaction was not done\n"));
    return -IGC_ERR_NVM;
  }

  /* Build and issue command on FLSWCTL register */
  Flswctl = Address | Opcode;
  WriteMacReg (Hw, IGC_FLSWCTL, Flswctl);

  /* Check if issued command is valid on FLSWCTL register */
  Flswctl = ReadMacReg (Hw, IGC_FLSWCTL);
  if (!(Flswctl & IGC_FLSWCTL_CMDV)) {
    DEBUG ((DEBUG_ERROR, "Write flash command failed\n"));
    Status = IGC_ERR_NVM;
  }

  return Status;
}

/**
  Sets FLSW NVM burst counter in FLSWCNT register.

  @param[in]  Hw                        - Pointer to hardware instance.
  @param[in]  BurstCounter              - Size in bytes of the flash burst to read/write

  @retval     IGC_SUCCESS               - The burst was set successfully.
  @retval     IGC_ERR_INVALID_PARAMETER - The given BurstCounter exceeds EEPROM size.
**/
IGC_STATUS
SetFlswBurstCounter (
  IN IGC_HW    *Hw,
  IN UINT32    BurstCounter
 )
{
  IGC_STATUS Status;

  Status = IGC_SUCCESS;

  /* Validate input data */
  if (BurstCounter < IGC_EEPROM_SIZE) {
    /* Write FLSWCNT - burst counter */
    WriteMacReg (Hw, IGC_FLSWCNT, BurstCounter);
  }
  else {
    Status = IGC_ERR_INVALID_ARGUMENT;
  }

  return Status;
}

/**
  Commit EEPROM to the flash if fw_valid bit is set, FW is active.
  Setting FLUPD bit in EEC register makes the FW load the internal
  shadow RAM into the flash. Otherwise, fw valid bit is 0.
  if FL_SECU.BlockProtectedSw = 0 then FW is not active so the SW
  is responsible shadow RAM dump.

  @param[in]  Hw                        - Pointer to hardware instance.

  @retval     IGC_SUCCESS               - Succeed to commit the EEPROM to the flash.

**/
IGC_STATUS
CommitEepromToFlash (
  IN IGC_HW    *Hw
 )
{
  UINT16     CurrentOffsetData;
  UINT32     BlockSwProtect;
  UINT16     BaseAddress;
  UINT32     Index;
  UINT32     FwValidBit;
  UINT16     CurrentOffset;
  IGC_STATUS Status;
  UINT32     Flup;

  CurrentOffsetData = 0;
  BlockSwProtect    = 1;
  BaseAddress       = 0x0;
  Status            = IGC_SUCCESS;

  BlockSwProtect = ReadMacReg (Hw, IGC_I225_FLSECU) & IGC_FLSECU_BLK_SW_ACCESS_I225;
  FwValidBit = ReadMacReg (Hw, IGC_FWSM) & IGC_FWSM_FW_VALID;
  if (FwValidBit) {
    Status = PollFlashUpdateDone (Hw);
    if (Status == -IGC_ERR_NVM) {
      DEBUG ((DEBUG_ERROR, "Flash update time out\n"));
      goto CommitEepromToFlashExit;
    }

    Flup = ReadMacReg (Hw, IGC_EECD) | IGC_EECD_FLUPD_I225;
    WriteMacReg (Hw, IGC_EECD, Flup);

    Status = PollFlashUpdateDone (Hw);
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Flash update complete\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "Flash update time out\n"));
    }
  }
  else if (!BlockSwProtect) {
    /* FW is not active and security protection is disabled.
      * therefore, SW is in charge of shadow RAM dump.
      * Check which sector is valid. if sector 0 is valid,
      * base address remains 0x0. otherwise, sector 1 is
      * valid and it's base address is 0x1000
      */
    if (ReadMacReg (Hw, IGC_EECD) & IGC_EECD_SEC1VAL_I225) {
      BaseAddress = 0x1000;
    }

    /* Valid sector erase */
    Status = SendFlashCommand (Hw, IGC_I225_ERASE_CMD_OPCODE, BaseAddress);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Sector erase failed\n"));
      goto CommitEepromToFlashExit;
    }

    CurrentOffset = BaseAddress;

    /* Write */
    for (Index = 0; Index < IGC_EEPROM_SIZE / 2; Index++) {
      /* Set burst write length */
      Status = SetFlswBurstCounter (Hw, 0x2);
      if (EFI_ERROR (Status)) {
        break;
      }

      /* Set address and opcode */
      Status = SendFlashCommand (Hw, IGC_I225_WRITE_CMD_OPCODE, 2 * CurrentOffset);
      if (EFI_ERROR (Status)) {
        break;
      }

      Status = ReadNvmEerd (Hw, CurrentOffset, &CurrentOffsetData, TRUE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Failed to read from EEPROM\n"));
        goto CommitEepromToFlashExit;
      }

      /* Write CurrentOffsetData to FLSWDATA register */
      WriteMacReg (Hw, IGC_FLSWDATA, CurrentOffsetData);
      CurrentOffset++;

      /* Wait till operation has finished */
      Status = PollEerdDone (Hw);
      if (EFI_ERROR (Status)) {
        break;
      }

      gBS->Stall (1000);
    }
  }

CommitEepromToFlashExit:
  return Status;
}

/**
  Poll FLSWCTL to get flash operation done signal.

  @param[in]  Hw                        - Pointer to hardware instance.

  @retval     IGC_SUCCESS               - Succeed to commit the EEPROM to the flash.
  @retval     IGC_DEVICE_ERROR          - Flash operation done bit wasn't raised.

**/
IGC_STATUS
GetFlswFlashOperationDone (
  IN IGC_HW    *Hw
 )
{
  UINT32 Flswctl;
  UINT32 Timeout;
  IGC_STATUS Status;

  Flswctl = 0;
  Timeout = FLASH_ERASE_POLL * 100;  /* 100 second */
  Status = IGC_DEVICE_ERROR;

  do {
    /* Read FLSWCNT register */
    Flswctl = ReadMacReg (Hw, IGC_FLSWCTL);

    /* Check if done bit is set */
    if ((Flswctl & (IGC_FLSWCTL_BUSY | IGC_FLSWCTL_DONE)) == IGC_FLSWCTL_DONE) {
      Status = IGC_SUCCESS;
      break;
    }

    /* Wait for 1 microsecond */
    gBS->Stall (1);
    Timeout--;
  } while (Timeout);
  return Status;
}

/**
  Updates the EEPROM checksum by reading/adding each word of the EEPROM
  up to the checksum.  Then calculates the EEPROM checksum and writes the
  value to the EEPROM. Next commit EEPROM data onto the Flash.

  @param[in]  Hw                        - Pointer to hardware instance

  @retval     IGC_SUCCESS               - Checksum update succeeded
  @retval     IGC_ERR_SWFW_SYNC         - a semaphore issue occurred
  @retval     IGC_ERR_NVM               - Some other error occurred

**/
IGC_STATUS
UpdateNvmChecksum (
  IN IGC_HW    *Hw
 )
{
  IGC_STATUS Status;
  UINT16     Checksum;
  UINT16     Index;
  UINT16     NvmData;

  Checksum = 0;

  /* Read the first word from the EEPROM. If this times out or fails, do
    * not continue or we could be in for a very long wait while every
    * EEPROM read fails
    */
  Status = ReadNvmEerd(Hw, 0, &NvmData, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "EEPROM read failed\n"));
    goto UpdateNvmChecksumExit;
  }

  if (AcquireSwFwSemaphore (Hw) == IGC_SUCCESS) {
    for (Index = 0; Index < NVM_CHECKSUM_REG; Index++) {
      Status = ReadNvmEerd (Hw, Index, &NvmData, FALSE);
      if (Status) {
        ReleaseSwFw (Hw);
        DEBUG ((DEBUG_ERROR, "NVM Read Error while updating checksum\n"));
        goto UpdateNvmChecksumExit;
      }
      Checksum += NvmData;
    }
    Checksum = (UINT16) NVM_SUM - Checksum;
    Status = WriteNvmSrwr (Hw, NVM_CHECKSUM_REG, &Checksum, FALSE);
    if (EFI_ERROR (Status)) {
      ReleaseSwFw (Hw);
      DEBUG ((DEBUG_ERROR, "NVM Write Error while updating checksum\n"));
      goto UpdateNvmChecksumExit;
    }

    ReleaseSwFw (Hw);

    Status = CommitEepromToFlash (Hw);
  }
  else {
    Status = IGC_ERR_SWFW_SYNC;
  }

UpdateNvmChecksumExit:
  return Status;
}

/**
  Get the size of flash.

  @param[in]  Hw  Pointer to hardware instance.

  @retval  0         Unrecognized the size of flash.
  @retval  SIZE_1MB  Size of the flash is 1MB.
  @retval  SIZE_2MB  Size of the flash is 2MB.

**/
UINT32
GetFlashSize (
  IN IGC_HW  *Hw
  )
{
  FOXVILLE_FLASH_DEVICE_SIZE  FlashDeviceSize;
  UINT32                      FlashSize;

  FlashDeviceSize.Uint16 = 0;
  FlashSize              = 0;

  ReadNvmEerd (Hw, IGC_FLASH_SIZE_WORD, &FlashDeviceSize.Uint16, TRUE);

  switch (FlashDeviceSize.Bits.FlashSize) {
    case B_FOXVILLE_FLASH_SIZE_1MB:
      FlashSize = SIZE_1MB;
      break;

    case B_FOXVILLE_FLASH_SIZE_2MB:
      FlashSize = SIZE_2MB;
      break;

    default:
      DEBUG ((DEBUG_ERROR, "Unrecognized the flash size = [0x%X]\n", FlashDeviceSize.Bits.FlashSize));
  }

  return FlashSize;
}

/**
  Returns the status of the programmed firmware by reading FW valid bit

  @param[in]  Hw                          - Pointer to hardware instance
  @param[out] FirmwareStatus              - Pointer that is filled when the firmware status is read

  @retval     IGC_SUCCESS                 - The firmware is valid
  @retval     IGC_ERR_NVM                 - The firmware is invalid

**/
IGC_STATUS
GetFirmwareStatus (
  IN IGC_HW                   *Hw,
  OUT IGC_FIRMWARE_STATUS     *FirmwareStatus
 )
{
  IGC_STATUS Status;
  UINT32     Fwsm;

  Status = IGC_SUCCESS;
  Fwsm   = 0;

  DEBUG ((DEBUG_INFO, "Enter GetFirmwareStatus\n"));

  do {
    if (FirmwareStatus == NULL) {
      break;
    }

    Fwsm = ReadMacReg (Hw, IGC_FWSM);
    /* Check if firmware is valid.*/
    if ((Fwsm & IGC_FWSM_FW_VALID) == IGC_FWSM_FW_VALID) {
      DEBUG ((DEBUG_INFO, "GetFirmwareStatus: Firmware is valid.\n"));
      *FirmwareStatus = IGC_FIRMWARE_VALID;
    }
    else {
      DEBUG ((DEBUG_ERROR, "GetFirmwareStatus: Firmware is corrupted.\n"));
      *FirmwareStatus = IGC_FIRMWARE_INVALID;
      Status = IGC_ERR_NVM;
      break;
    }
  } while (0);

  return Status;
}

/**
  Identifies Flash programming mode by reading EECD and FLA registers.

  @param[in]  Hw                          - Pointer to hardware instance

  @retval     FLASH MODE                  - According to the MAC registers

**/
FLASH_MODE
GetFlashProgrammingMode (
  IN  IGC_HW    *Hw
 )
{
  IGC_STATUS          Status;
  IGC_FIRMWARE_STATUS FwStatus;
  UINT32              FlashAccessReg;
  UINT32              Eec;

  FwStatus = IGC_FIRMWARE_INVALID;
  FlashAccessReg = 0;
  Eec = 0;

  /* By default we are working in the normal mode. */
  FLASH_MODE FlashMode = PROTECTED_FLASH_MODE;

  DEBUG ((DEBUG_INFO, "Enter GetFlashProgrammingMode\n"));
  /* Discover flash device presence. */
  Eec = ReadMacReg (Hw, IGC_EECD);
  if ((Eec & IGC_EECD_FLASH_DETECTED) != IGC_EECD_FLASH_DETECTED) {
    FlashMode = NO_FLASH_DETECTED;
  }
  else {
    FlashAccessReg = ReadMacReg (Hw, IGC_FLA);
    if ((FlashAccessReg & IGC_FLA_LOCKED) == 0) {
      Status = GetFirmwareStatus (Hw, &FwStatus);
      if (!EFI_ERROR (Status)) {
        FlashMode = UNPROTECTED_FLASH_MODE;
      } else {
        FlashMode = BLANK_FLASH_MODE;
      }
    }
  }

  return FlashMode;
}

/**
 Returns the flash module pointer for a given module.

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Module                      - The module to identify it's pointer
  @param[out] ModulePointer               - The module pointer that is being returned

  @retval     IGC_SUCCESS                 - The module pointer could be calculated
  @retval     IGC_DEVICE_ERROR            - OROM pointer couldn't be read
  @retval     IGC_ERR_INVALID_ARGUMENT    - The given module is unkown

**/
IGC_STATUS
GetFlashModulePointer (
  IN IGC_HW             *Hw,
  IN IGC_FLASH_MODULES  Module,
  OUT UINT16            *ModulePointer
 )
{
  UINT16      EepromWord;
  IGC_STATUS  Status;

  EepromWord = 0;
  Status = IGC_SUCCESS;

  DEBUG ((DEBUG_INFO, "Enter GetFlashModulePointer\n"));

  switch (Module) {
    case IGC_FLASH_MODULE_EMP_IMAGE:
      *ModulePointer = 0x10;
      break;
    case IGC_FLASH_MODULE_SECURED_SETTINGS:
      *ModulePointer = 0x4A;
      break;
    case IGC_FLASH_MODULE_1ST_FREE_AREA:
      *ModulePointer = 0x40;
      break;
    default:
      Status = IGC_ERR_INVALID_ARGUMENT;
      break;
  }

  if (*ModulePointer == 0x4A) {
    Status = ReadNvmEerd (Hw, IGC_OPTION_ROM_PTR, &EepromWord, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "GetFlashModulePointer: Could not read correct OROM pointer!\n"));
      Status = IGC_DEVICE_ERROR;
    }
    if ((EepromWord == 0xFFFF || EepromWord == 0x0000) && !EFI_ERROR (Status)) {
      *ModulePointer = 0x7F0;
    }
  }

  DEBUG ((DEBUG_INFO, "GetFlashModulePointer: Return Status = %d!\n", Status));
  return Status;
}

/**
  Calculates a given module size from the image

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Module                      - A module to calculate it's size
  @param[in]  Buffer                      - A pointer to the module buffer in the image
  @param[in]  BufferSize                  - The size of the image in bytes
  @param[out] ModuleSize                  - The size of the module buffer from the image

  @retval     IGC_SUCCESS                 - The module size could be calculated
  @retval     IGC_DEVICE_ERROR            - Either the image is corrupted or couldn't calculate
                                            the module pointer
  @retval     IGC_ERR_INVALID_ARGUMENT    - The given module is unsupported

**/
IGC_STATUS
GetFlashModuleSizeFromImage (
  IN IGC_HW               *Hw,
  IN IGC_FLASH_MODULES    Module,
  IN UINT8                *Buffer,
  IN UINTN                BufferSize,
  OUT UINT32              *ModuleSize
 )
{
  IGC_STATUS Status;
  UINT16     EepromWord;
  UINT16     FreeAreaSize;
  UINT16     OROMPointer;
  UINT16     ModulePointer;
  UINT8      ModuleSizeOffset;
  UINT8      OROMOffset;

  Status           = IGC_ERR_INVALID_ARGUMENT;
  EepromWord       = 0;
  FreeAreaSize     = 0;
  OROMPointer      = 0;
  ModulePointer    = 0;
  ModuleSizeOffset = 0;
  OROMOffset       = 0;

  DEBUG ((DEBUG_INFO, "Enter GetFlashModuleSizeFromImage\n"));

  switch (Module) {
    /* Modules with fixed sizes */
    case IGC_FLASH_MODULE_SHADOW_RAM:
      *ModuleSize = IGC_EEPROM_SIZE; /* 4KB */
      Status = IGC_SUCCESS;
      break;

    case IGC_FLASH_MODULE_OPTION_ROM:
      ModuleSizeOffset = IGC_OPTION_ROM_PTR * sizeof(UINT16);
      EepromWord = MAKE_WORD (Buffer[ModuleSizeOffset + 1], Buffer[ModuleSizeOffset]);

      if (EepromWord == 0xFFFF || EepromWord == 0x0000) {
        *ModuleSize = IGC_SE_FLASH_ROM_NULL;
        Status = IGC_SUCCESS;
        break;
      }
      Status = IGC_SUCCESS;
      *ModuleSize = IGC_SE_FLASH_ROM_SIZE;
      break;

    case IGC_FLASH_MODULE_SECURED_SETTINGS:
      /* Get FPA pointer */
      Status = GetFlashModulePointer (Hw, IGC_FLASH_MODULE_1ST_FREE_AREA, &ModulePointer);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "GetFlashModuleSizeFromImage: Could not read correct FW pointer!\n"));
        Status = IGC_DEVICE_ERROR;
        break;
      }
      /* Check if OROM Exists */
      ModuleSizeOffset = IGC_OPTION_ROM_PTR * sizeof(UINT16);
      EepromWord = MAKE_WORD (Buffer[ModuleSizeOffset + 1], Buffer[ModuleSizeOffset]);

      if ((EepromWord == 0xFFFF || EepromWord == 0x0000) && !EFI_ERROR (Status)) {
        *ModuleSize = IGC_SECURED_MODULE_SIZE_NO_OROM;
        break;
      }
      *ModuleSize = IGC_SECURED_MODULE_SIZE_WITH_OROM;
      break;

    case IGC_FLASH_MODULE_EMP_IMAGE:
      /* Get FW pointer */
      Status = GetFlashModulePointer(Hw, IGC_FLASH_MODULE_EMP_IMAGE, &ModulePointer);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "GetFlashModulePointerFromBuffer: Could not read correct FW pointer!\n"));
        Status = IGC_DEVICE_ERROR;
        break;
      }
      ModulePointer = ModulePointer * 2;
      if ((UINT32) ModulePointer + 1 > BufferSize) {
        Status = IGC_ERR_INVALID_ARGUMENT;
        break;
      }
      EepromWord = MAKE_WORD (Buffer[ModulePointer + 1], Buffer[ModulePointer]);

      if (EepromWord == 0xFFFF || EepromWord == 0x0000 || EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "GetFlashModulePointerFromBuffer: Could not read correct FW pointer!\n"));
        Status = IGC_DEVICE_ERROR;
        break;
      }

      /* Get FPA pointer */
      Status = GetFlashModulePointer(Hw, IGC_FLASH_MODULE_1ST_FREE_AREA, &ModulePointer);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "GetFlashModulePointerFromBuffer: Could not read correct FW pointer!\n"));
        Status = IGC_DEVICE_ERROR;
        break;
      }

      /* Check if OROM Exists */
      ModuleSizeOffset = IGC_OPTION_ROM_PTR * sizeof (UINT16);
      EepromWord = MAKE_WORD (Buffer[ModuleSizeOffset + 1], Buffer[ModuleSizeOffset]);

      if ((EepromWord == 0xFFFF || EepromWord == 0x0000) && !EFI_ERROR (Status)) {
        *ModuleSize = IGC_SECURED_MODULE_SIZE_NO_OROM;
        break;
      }
      *ModuleSize = IGC_SECURED_MODULE_SIZE_WITH_OROM;
      break;

    case IGC_FLASH_MODULE_1ST_FREE_AREA:
      ModuleSizeOffset = IGC_FPA_SIZE * sizeof(UINT16);
      FreeAreaSize = MAKE_WORD (Buffer[ModuleSizeOffset + 1], Buffer[ModuleSizeOffset]);

      if (FreeAreaSize == 0xFFFF || FreeAreaSize == 0x0000) {
        DEBUG ((DEBUG_ERROR, "GetFlashModulePointerFromBuffer: Shadow RAM FPA size offset corrupted!\n"));
        Status = IGC_DEVICE_ERROR;
      }
      else {
        /* Check if we have an OROM in the NVM */
        OROMOffset = IGC_OPTION_ROM_PTR * sizeof(UINT16);
        OROMPointer = MAKE_WORD (Buffer[OROMOffset + 1], Buffer[OROMOffset]);
        if (OROMPointer == 0xFFFF || OROMPointer == 0x0000) {
          *ModuleSize = IGC_SECURED_MODULE_SIZE_NO_OROM;
        }
        else {
          Status = IGC_SUCCESS;
          *ModuleSize = IGC_SECURED_MODULE_SIZE_WITH_OROM;
        }
      }
      break;

    default:
      break;
  }

  return Status;
}

/**
  Calculates the offset of a given module in the flash

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Module                      - A module to calculate it's size
  @param[out] ModuleOffset                - The offset of the module in the flash

  @retval     IGC_SUCCESS                 - The size could be calculated
  @retval     IGC_DEVICE_ERROR            - Either the image is corrupted or couldn't calculate
                                            the module pointer
  @retval     IGC_ERR_INVALID_ARGUMENT    - The given module is unsupported

**/
IGC_STATUS
GetFlashModuleOffset (
  IN IGC_HW               *Hw,
  IN IGC_FLASH_MODULES    Module,
  OUT UINT32              *ModuleOffset
 )
{
  UINT32 EecRegister;
  UINT32 FlashSize;
  UINT16 EepromWord;
  UINT16 ModulePointer;
  IGC_STATUS Status;

  EecRegister = 0;
  FlashSize = Hw->FlashInfo->FlashSize;
  EepromWord = 0;
  ModulePointer = 0;
  Status = IGC_ERR_INVALID_ARGUMENT;

  DEBUG ((DEBUG_INFO, "Enter GetFlashModuleOffset, Module = 0x%x, Module offset = 0x%x\n", Module, ModuleOffset));

  if (ModuleOffset != NULL) {
    if (Module == IGC_FLASH_MODULE_SHADOW_RAM) {
      EecRegister = ReadMacReg (Hw, IGC_EECD);
      if ((EecRegister & IGC_EECD_SEC1VAL_I225) == IGC_EECD_SEC1VAL_I225) {
        *ModuleOffset = IGC_EEPROM_SIZE;
      } else {
        *ModuleOffset = 0x00000000;
      }
      Status = IGC_SUCCESS;
    }
    else if (Module == IGC_FLASH_MODULE_EMP_IMAGE)
    {
      /* On flash parts larger than 1 MB, the space from byte address 0x002000 to 0x081FFF is
      always reserved to the Expansion/Option ROM module. */
      if (FlashSize > SIZE_1MB) {
        GetFlashModulePointer (Hw, Module, &ModulePointer);
        /* Read the module offset from Shadow RAM */
        Status = ReadNvmEerd (Hw, ModulePointer, (UINT16*) ModuleOffset, TRUE);
        if (*ModuleOffset == 0xFFFF) {
          DEBUG ((DEBUG_ERROR, "GetFlashModuleOffset: Shadow RAM FPA pointer corrupted!\n"));
          Status = IGC_DEVICE_ERROR;
        }

        /* Pointer is calculated in 4KB sector units. Calculate byte offset basing on that */
        if (!EFI_ERROR (Status) && (*ModuleOffset & (1 << 15))) {
          *ModuleOffset = (*ModuleOffset & 0x7FFF) * IGC_EEPROM_SIZE;
        }

        /* Check if OROM exists */
        Status = ReadNvmEerd (Hw, IGC_OPTION_ROM_PTR, &EepromWord, TRUE);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "GetFlashModuleOffset: Could not read correct OROM pointer!\n"));
          Status = IGC_DEVICE_ERROR;
          return Status;
        }
        if (EepromWord == 0xFFFF || EepromWord == 0x0000) {
          *ModuleOffset -= IGC_SE_FLASH_PHY_SIZE;
        } else {
          *ModuleOffset -= IGC_SE_FLASH_PHY_SIZE + IGC_SE_FLASH_ROM_SIZE;
        }
        Status = IGC_SUCCESS;
      }
      else {
        Status = IGC_ERR_NVM;
      }
    }
    else {
      GetFlashModulePointer (Hw, Module, &ModulePointer);
      /* Read the module offset from Shadow RAM */
      Status = ReadNvmEerd (Hw, ModulePointer, (UINT16*)ModuleOffset, TRUE);
      if (*ModuleOffset == 0xFFFF) {
        DEBUG ((DEBUG_ERROR, "GetFlashModuleOffset: Shadow RAM FPA pointer corrupted!\n"));
        Status = IGC_DEVICE_ERROR;
      }

      /* Pointer is calculated in 4KB sector units. Calculate byte offset basing on that */
      if (!EFI_ERROR (Status) && (*ModuleOffset & (1 << 15))) {
        *ModuleOffset = (*ModuleOffset & 0x7FFF) * IGC_EEPROM_SIZE;
      }
    }
  }
  DEBUG ((DEBUG_INFO, "GetFlashModuleOffset Returns - %d\n", Status));
  return Status;
}

/**
  Calculates the offset of a given module in the image

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Module                      - A module to calculate it's size
  @param[in]  ModuleOffset                - The offset of the module in the flash
  @param[in]  Buffer                      - Pointer to the image
  @param[out] BufferSize                  - The size of the image

  @retval     IGC_SUCCESS                 - The size could be calculated
  @retval     IGC_DEVICE_ERROR            - Either the image is corrupted or couldn't calculate
                                            the module pointer
  @retval     IGC_ERR_INVALID_ARGUMENT    - The given module is unsupported

**/
IGC_STATUS
GetFlashModuleOffsetFromImage (
  IN IGC_HW                *Hw,
  IN IGC_FLASH_MODULES     Module,
  IN UINT8                 *Buffer,
  IN UINTN                 BufferSize,
  OUT UINT32               *ModuleOffset
 )
{
  IGC_STATUS  Status;
  UINT32      FlashSize;
  UINT16      EepromWord;
  UINT16      ModulePointer;

  Status = IGC_ERR_INVALID_ARGUMENT;
  FlashSize = Hw->FlashInfo->FlashSize;
  EepromWord = 0;
  ModulePointer = 0;

  DEBUG ((DEBUG_INFO, "Enter GetFlashModuleOffsetFromBuffer\n"));

  if (ModuleOffset != NULL) {
    do {
      if (Module == IGC_FLASH_MODULE_SHADOW_RAM) {
        if ((Buffer[IGC_NVM_VALIDITY_OFFSET] & IGC_SHARED_FLASH_EEPROM_BANK_SIGNATURE_MASK) == IGC_SHARED_FLASH_VALID_EEPROM_BANK_SIGNATURE) {
          *ModuleOffset = 0;
          Status = IGC_SUCCESS;
        }
        else if ((Buffer[IGC_SHARED_FLASH_EEPROM_BANK_SIZE + IGC_NVM_VALIDITY_OFFSET] & IGC_SHARED_FLASH_EEPROM_BANK_SIGNATURE_MASK) == IGC_SHARED_FLASH_VALID_EEPROM_BANK_SIGNATURE) {
          *ModuleOffset = IGC_EEPROM_SIZE;
          Status = IGC_SUCCESS;
        }
        else {
          DEBUG ((DEBUG_ERROR, "GetFlashModuleOffsetFromBuffer: No valid Shadow RAM bank found in image!\n"));
          Status = IGC_ERR_INVALID_ARGUMENT;
        }
      }
      else if (Module == IGC_FLASH_MODULE_EMP_IMAGE)
      {
        /* On flash parts larger than 1 MB, the space from byte address 0x002000 to 0x081FFF is
            always reserved to the Expansion/Option ROM module.
          */
        if (FlashSize > SIZE_1MB) {
          Status = GetFlashModulePointer (Hw, Module, &ModulePointer);
          if (EFI_ERROR (Status)) {
            break;
          }
          ModulePointer = ModulePointer * 2;
          if ((UINT32) ModulePointer + 1 > BufferSize) {
            Status = IGC_ERR_INVALID_ARGUMENT;
            break;
          }
          *ModuleOffset = MAKE_WORD (Buffer[ModulePointer + 1], Buffer[ModulePointer]);
          if (*ModuleOffset == 0xFFFF || *ModuleOffset == 0x0000) {
            DEBUG ((DEBUG_ERROR, "GetFlashModuleOffsetFromBuffer: Pointer for module %d is empty\n", Module));
            Status = IGC_DEVICE_ERROR;
            break;
          }

          /* If 15th bit is set the pointer is in 4 kB sectors */
          if (*ModuleOffset & (1 << 15)) {
            *ModuleOffset = (*ModuleOffset & ~(1 << 15)) * IGC_FLASH_4K_SECTOR_SIZE;
          }

          /* Check if OROM exists */
          DEBUG ((DEBUG_INFO, "GetFlashModuleOffsetFromBuffer: Checking OROM existence\n"));
          EepromWord = MAKE_WORD (Buffer[IGC_OPTION_ROM_PTR + 1], Buffer[IGC_OPTION_ROM_PTR]);
          if (EepromWord == 0xFFFF || EepromWord == 0x0000) {
            *ModuleOffset -= IGC_SE_FLASH_PHY_SIZE;
          } else {
            *ModuleOffset -= IGC_SE_FLASH_PHY_SIZE + IGC_SE_FLASH_ROM_SIZE;
          }
        }
      }
      else {
        Status = GetFlashModulePointer (Hw, Module, &ModulePointer);
        if (EFI_ERROR (Status)) {
          break;
        }
        ModulePointer = ModulePointer * 2;
        if ((UINT32) ModulePointer + 1 > BufferSize) {
          Status = IGC_ERR_INVALID_ARGUMENT;
          break;
        }
        DEBUG ((DEBUG_INFO, "GetFlashModuleOffsetFromBuffer: Getting module offset for module %d\n", Module));
        *ModuleOffset = MAKE_WORD (Buffer[ModulePointer + 1], Buffer[ModulePointer]);
        if (*ModuleOffset == 0xFFFF || *ModuleOffset == 0x0000) {
          DEBUG ((DEBUG_ERROR, "GetFlashModuleOffsetFromBuffer: Pointer for module %d is empty\n", Module));
          Status = IGC_DEVICE_ERROR;
          break;
        }

        /* If 15th bit is set the pointer is in 4 kB sectors */
        if (*ModuleOffset & (1 << 15)) {
          *ModuleOffset = (*ModuleOffset & ~(1 << 15)) * IGC_FLASH_4K_SECTOR_SIZE;
        }
      }
    } while (0);
  }
  DEBUG ((DEBUG_INFO, "GetFlashModuleOffsetFromBuffer: Function returned: %d\n", Status));
  return Status;
}

/**
  Extract pointer to the module and its size from the combo image

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Module                      - A module to calculate it's size
  @param[in]  Buffer                      - Pointer to the image
  @param[out] BufferSize                  - The size of the image
  @param[out] ModuleSize                  - The calculated size of the module in the image

  @retval     IGC_SUCCESS                 - The size could be calculated
  @retval     IGC_DEVICE_ERROR            - Either the image is corrupted or couldn't calculate
                                            the module pointer
  @retval     IGC_ERR_INVALID_ARGUMENT    - The given module is unsupported

**/
IGC_STATUS
GetModuleFromImage (
  IN IGC_HW               *Hw,
  IN IGC_FLASH_MODULES    Module,
  IN UINT8                *Buffer,
  IN UINTN                BufferSize,
  OUT UINT8               **ModuleBuffer,
  OUT UINT32              *ModuleSize
 )
{
  IGC_STATUS  Status;
  UINT32      ModuleOffset;

  Status = IGC_ERR_INVALID_ARGUMENT;
  ModuleOffset = 0;

  DEBUG ((DEBUG_INFO, "Enter GetModuleFromImage\n"));

  do {
    if (Buffer == NULL) {
      Status = IGC_ERR_INVALID_ARGUMENT;
      break;
    }

    Status = GetFlashModuleSizeFromImage (Hw, Module, Buffer, BufferSize, ModuleSize);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (Module == IGC_FLASH_MODULE_SHADOW_RAM) {
      if (BufferSize < 2 * IGC_EEPROM_SIZE) {
        DEBUG ((DEBUG_ERROR, "GetModuleFromImage: Given image is too small!\n"));
        Status = IGC_ERR_INVALID_ARGUMENT;
        break;
      }

      Status = GetFlashModuleOffsetFromImage (Hw, Module, Buffer, BufferSize, &ModuleOffset);
      if (EFI_ERROR (Status)) {
        break;
      }
      DEBUG ((DEBUG_INFO, "Module offset = 0x%x\n", ModuleOffset));
    }
    else if (Module == IGC_FLASH_MODULE_EMP_IMAGE || Module == IGC_FLASH_MODULE_OPTION_ROM) {
      Status = GetFlashModuleOffset (Hw, Module, &ModuleOffset);
      if (MAKE_WORD(Buffer[0x21], Buffer[0x20]) == 0x8002) {
        Status = IGC_DEVICE_ERROR;
      }
      DEBUG ((DEBUG_INFO, "Module: EMP_IMAGE Module offset = 0x%x\n", ModuleOffset));
    }
    else {
      /* For other modules get the image pointer from image */
      Status = GetFlashModuleOffsetFromImage (Hw, Module, Buffer, BufferSize, &ModuleOffset);
      if (EFI_ERROR (Status)) {
        break;
      }
      DEBUG ((DEBUG_INFO, "Module offset = 0x%x\n", ModuleOffset));
    }

    DEBUG ((DEBUG_INFO, "Combo Buffer Size = 0x%x\n", BufferSize));
    if (ModuleOffset + *ModuleSize > BufferSize) {
      DEBUG ((DEBUG_ERROR, "Corrupted image detected!\n ModuleOffset + ModuleSize exceeds buffer size\n"));
      Status = IGC_ERR_INVALID_ARGUMENT;
      break;
    }

    *ModuleBuffer = &Buffer[ModuleOffset];
  } while (0);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: GetModuleFromImage returned %x\n", Status));
  }

  DEBUG ((DEBUG_INFO, "GetModuleImage: Returned %d\n", Status));
  return Status;
}

/**
  Extracts module size for a given mode in the flash

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Module                      - A module to calculate it's size
  @param[out] ModuleSize                  - The calculated size of the module in the flash

  @retval     IGC_SUCCESS                 - The size could be calculated
  @retval     IGC_DEVICE_ERROR            - Either the image is corrupted or couldn't calculate
                                            the module pointer
  @retval     IGC_ERR_INVALID_ARGUMENT    - The given module is unsupported
**/
IGC_STATUS
GetFlashModuleSize (
  IN IGC_HW               *Hw,
  IN IGC_FLASH_MODULES    Module,
  OUT UINT32              *ModuleSize
 )
{
  IGC_STATUS  Status;
  UINT16      EepromWord;
  UINT16      FreeAreaSize;
  UINT16      ModulePointer;

  Status        = IGC_ERR_INVALID_ARGUMENT;
  EepromWord    = 0;
  FreeAreaSize  = 0;
  ModulePointer = 0;

  DEBUG ((DEBUG_INFO, "Enter GetFlashModuleSize\n"));

  switch (Module) {
    /* Module with fixed sizes */
    case IGC_FLASH_MODULE_SHADOW_RAM:
      *ModuleSize = IGC_EEPROM_SIZE; /* 4KB */
      Status = IGC_SUCCESS;
      break;

    case IGC_FLASH_MODULE_OPTION_ROM:
      /* Check if OROM exists */
      Status = ReadNvmEerd (Hw, IGC_OPTION_ROM_PTR, &EepromWord, TRUE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Could not read correct OROM pointer!\n"));
        Status = IGC_DEVICE_ERROR;
        break;
      }
      if ((EepromWord == 0xFFFF || EepromWord == 0x0000) && !EFI_ERROR (Status)) {
        *ModuleSize = IGC_SE_FLASH_ROM_NULL;
        break;
      }
      Status = IGC_SUCCESS;
      *ModuleSize = IGC_SE_FLASH_ROM_SIZE;
      break;

    case IGC_FLASH_MODULE_EMP_IMAGE:
      /* Get FW pointer */
      Status = GetFlashModulePointer (Hw, IGC_FLASH_MODULE_EMP_IMAGE, &ModulePointer);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Could not read correct FW pointer!\n"));
        Status = IGC_DEVICE_ERROR;
        break;
      }

      Status = ReadNvmEerd (Hw, ModulePointer, &EepromWord, TRUE);
      if (EepromWord == 0xFFFF || EepromWord == 0x0000 || EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Could not read correct FW pointer!\n"));
        Status = IGC_DEVICE_ERROR;
        break;
      }

      /* Get FPA pointer */
      Status = GetFlashModulePointer (Hw, IGC_FLASH_MODULE_1ST_FREE_AREA, &ModulePointer);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Could not read correct FW pointer!\n"));
        Status = IGC_DEVICE_ERROR;
        break;
      }

      Status = ReadNvmEerd (Hw, ModulePointer, &EepromWord, TRUE);
      if (EepromWord == 0xFFFF || EepromWord == 0x0000 || EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Could not read correct Free Area pointer!\n"));
        Status = IGC_DEVICE_ERROR;
        break;
      }

      /* Check if OROM exists */
      Status = ReadNvmEerd (Hw, IGC_OPTION_ROM_PTR, &EepromWord, TRUE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Could not read correct OROM pointer!\n"));
        Status = IGC_DEVICE_ERROR;
        break;
      }
      if ((EepromWord == 0xFFFF || EepromWord == 0x0000) && !EFI_ERROR (Status)) {
        *ModuleSize = IGC_SECURED_MODULE_SIZE_NO_OROM;
        break;
      }
      *ModuleSize = IGC_SECURED_MODULE_SIZE_WITH_OROM;
      break;

    case IGC_FLASH_MODULE_1ST_FREE_AREA:
    case IGC_FLASH_MODULE_SECURED_SETTINGS:
      /* Read the size from EEPROM word 0x41
      The size is expressed in 4KB sectors */
      Status = ReadNvmEerd (Hw, IGC_FPA_SIZE, &FreeAreaSize, TRUE);
      if (FreeAreaSize == 0xFFFF || FreeAreaSize == 0x0000)
      {
        DEBUG ((DEBUG_ERROR, "Shadow RAM FPA size corrupted!\n"));
        Status = IGC_DEVICE_ERROR;
      }
      if (!EFI_ERROR (Status)) {
        /* Check if we have an OROM in the NVM */
        Status = ReadNvmEerd (Hw, IGC_OPTION_ROM_PTR, &EepromWord, TRUE);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Could not read correct OROM pointer!\n"));
          Status = IGC_DEVICE_ERROR;
          break;
        }
        if ((EepromWord == 0xFFFF || EepromWord == 0x0000) && !EFI_ERROR (Status)) {
          *ModuleSize = IGC_SECURED_MODULE_SIZE_NO_OROM;
          break;
        }
        *ModuleSize = IGC_SECURED_MODULE_SIZE_WITH_OROM;
      }
      break;

    default:
      break;
  }

  return Status;
}

/**
  Checks if a given region is protected

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  RegionStart                 - The offset of the beginning region in the offset
  @param[in]  RegionSize                  - The size of the given region

  @retval     TRUE                        - Flash region is protected
  @retval     FALSE                       - Flash region is unprotected

**/
BOOLEAN
IsFlashRegionProtected (
  IN IGC_HW    *Hw,
  IN UINT32    RegionStart,
  IN UINT32    RegionSize
 )
{
  UINT32      ModuleOffset;
  UINT32      ModuleEnd;
  UINT32      ModuleSize;
  UINT32      RegionEnd;
  IGC_STATUS  Status;
  BOOLEAN     RegionProtected;

  ModuleOffset    = 0;
  ModuleEnd       = 0;
  ModuleSize      = 0;
  RegionEnd       = 0;
  Status          = IGC_ERR_INVALID_ARGUMENT;
  RegionProtected = TRUE;

  DEBUG ((DEBUG_INFO, "Enter IsFlashRegionProtected\n"));

  do {
    /* FW does not allow us to upgrade any Shadow RAM words using FLASH functions */
    if (RegionStart < 2 * IGC_EEPROM_SIZE) {
      break;
    }

    /* Check for RO modules - currently it's only FW module */
    Status = GetFlashModuleOffset (Hw, IGC_FLASH_MODULE_EMP_IMAGE, &ModuleOffset);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "IsFlashRegionProtected: Error reading FLASH module offset\n"));
      break;
    }

    Status = GetFlashModuleSize (Hw, IGC_FLASH_MODULE_EMP_IMAGE, &ModuleSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "IsFlashRegionProtected: Error reading FLASH module size\n"));
      break;
    }

    ModuleEnd = ModuleOffset + ModuleSize - 1;
    RegionEnd = RegionStart + RegionSize - 1;

    DEBUG ((DEBUG_INFO, "IsFlashRegionProtected: Protected module Start 0x%x, End 0x%x\n", ModuleOffset, ModuleEnd));
    DEBUG ((DEBUG_INFO, "IsFlashRegionProtected: Tried to write from 0x%x to 0x%x\n", RegionStart, RegionEnd));

    /* check if requested area can be written */
    if (ModuleEnd < RegionStart || ModuleOffset > RegionEnd) {
      RegionProtected = FALSE;
    }
    else {
      DEBUG ((DEBUG_INFO, "IsFlashRegionProtected: Tried to write to protected region!\n"));
    }
  } while (0);

  return RegionProtected;
}

/**
  Writes protected region in the flash

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Image                       - Pointer to the new image
  @param[in]  ImageSize                   - The size of the new image
  @param[in]  WriteMacAddress             - Boolean value to determine whether saving the
                                            old MAC address is required

  @retval     IGC_SUCCESS                 - Writing the protected flash region was successful
  @retval     IGC_ERR_INVALID_ARGUMENT    - The offset/size parameters are incorrect

**/
IGC_STATUS
WriteProtectedFlashRegion (
  IN  IGC_HW      *Hw,
  IN  UINT8       *Image,
  IN  UINTN       ImageSize,
  IN  BOOLEAN     WriteMacAddress
 )
{
  UINT8       *FwBuffer;
  IGC_STATUS  Status;
  UINT32      FwBufferSize;
  UINT16      FwBufferSize_16;
  UINT32      FwOffsetInBuffer;
  UINT32      NewModuleOffset;
  UINT32      ModuleOffset;
  UINT32      ModuleSize;
  UINT32      FlashSize;
  UINT32      MdnsSize;
  UINT32      FpaOffsetInBuffer;
  UINT32      NewFpaOffset;
  UINT32      SecuredModuleSizeFromBuffer;
  UINT32      CurrentModuleSize;
  UINT16      ModulePointerValue;
  UINT16      ModulePointer;

  FwBuffer                    = NULL;
  Status                      = IGC_ERR_INVALID_ARGUMENT;
  FwBufferSize                = 0;
  FwOffsetInBuffer            = 0;
  NewModuleOffset             = 0;
  ModuleOffset                = 0;
  ModuleSize                  = 0;
  FlashSize                   = Hw->FlashInfo->FlashSize;
  MdnsSize                    = 0x10;
  FpaOffsetInBuffer           = 0;
  NewFpaOffset                = 0;
  SecuredModuleSizeFromBuffer = 0;
  CurrentModuleSize           = 0;
  ModulePointerValue          = 0;
  ModulePointer               = 0;

  DEBUG ((DEBUG_INFO, "Enter WriteProtectedFlashImageEx\n"));

  do {
    /* Get secured module size from image */
    Status = GetFlashModuleSizeFromImage (Hw, IGC_FLASH_MODULE_SECURED_SETTINGS, Image, IGC_SE_FLASH_MAX_IMAGE_SIZE, &SecuredModuleSizeFromBuffer);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Could not get secured module size from buffer.\n"));
      break;
    }

    /* Get secured module size from flash */
    Status = GetFlashModuleSize (Hw, IGC_FLASH_MODULE_SECURED_SETTINGS, &CurrentModuleSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: error: Could not get module size: %x\n", IGC_FLASH_MODULE_SECURED_SETTINGS));
      break;
    }

    /* Verify whether the secured module size to update matches current size, otherwise break  */
    if (SecuredModuleSizeFromBuffer != CurrentModuleSize) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Error: Secured module size from buffer does not match Current secured module size\n"));
      Status = IGC_ERR_INVALID_ARGUMENT;
      break;
    }

    /* Get the firmware module from combo image */
    Status = GetModuleFromImage (Hw, IGC_FLASH_MODULE_SECURED_SETTINGS, Image, ImageSize, &FwBuffer, &FwBufferSize);
    if (Status == IGC_DEVICE_ERROR) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Warning: Empty module pointer \n"));
      break;
    }
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: error: Problem getting module %x from combo image\n", IGC_FLASH_MODULE_EMP_IMAGE));
      break;
    }

    MemoryFence ();
    /* Calculate new free provisioning area */

    DEBUG ((DEBUG_INFO, "WriteProtectedFlashImageEx: Getting secured module offset.\n"));
    /* Get appropriate Free Provisioning Area for the requested module */
    Status = GetFlashModuleOffset (Hw, IGC_FLASH_MODULE_SECURED_SETTINGS, &ModuleOffset);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Could not FPA offset for Secured module.\n"));
      break;
    }

    DEBUG ((DEBUG_INFO, "WriteProtectedFlashImageEx: Getting secured module offset from image.\n"));
    /* Get FW module offset from file */
    Status = GetFlashModuleOffsetFromImage(Hw, IGC_FLASH_MODULE_SECURED_SETTINGS, Image, ImageSize, &FwOffsetInBuffer);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Could not get flash module offset from buffer.\n"));
      break;
    }

    DEBUG ((DEBUG_INFO, "WriteProtectedFlashImageEx: Getting Flash Module free area from image\n"));
    /* Get FPA offset from file */
    Status = GetFlashModuleOffsetFromImage(Hw, IGC_FLASH_MODULE_1ST_FREE_AREA, Image, ImageSize, &FpaOffsetInBuffer);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Could not get flash module offset from buffer.\n"));
      break;
    }

    DEBUG ((DEBUG_INFO, "WriteProtectedFlashImageEx: Calculating new module size\n"));
    NewModuleOffset = (FwOffsetInBuffer <= FpaOffsetInBuffer) ? FwOffsetInBuffer : FpaOffsetInBuffer;
    if (ModuleOffset - NewModuleOffset >= FwBufferSize) {
      ModuleOffset = NewModuleOffset;
    }
    else {
      Status = GetFlashModuleSize (Hw, IGC_FLASH_MODULE_SECURED_SETTINGS, &ModuleSize);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Could not get flash module size.\n"));
        break;
      }
      ModuleOffset = MAX ((ModuleOffset + ModuleSize), (NewModuleOffset + FwBufferSize));

      if (ModuleOffset + FwBufferSize + MdnsSize > FlashSize) {
        DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Flash device too small\n"));
        Status = IGC_ERR_NVM;
        break;
      }
    }

    DEBUG ((DEBUG_INFO, "WriteProtectedFlashImageEx: New FW pointer: %x\n", ModuleOffset));

    /* Write new FPA pointer value in eeprom */
    GetFlashModulePointer (Hw, IGC_FLASH_MODULE_1ST_FREE_AREA, &ModulePointer);

    /* Pointer is calculated in 4KB sector units. Calculate byte offset basing on that */
    ModulePointerValue = (UINT16) ((ModuleOffset / IGC_SECTOR_SIZE) | IGC_POINTER_4K_GRANULARITY);

    Status = WriteNvmSrwr (Hw, (UINT16)ModulePointer, &ModulePointerValue, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: New FPA pointer write failed.\n"));
      break;
    }

    /* Update Firmware module */
    Status = UpdateSecuredModule (Hw, 0, FwBuffer, FwBufferSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Error: Problem updating module\n"));
      break;
    }

    /* Need to write new FPA pointer as it could not be any more the previous FW pointer */
    if (ModuleOffset == NewModuleOffset) {
      NewFpaOffset = NewModuleOffset + FwBufferSize;
    } else {
      NewFpaOffset = NewModuleOffset;
    }

    DEBUG ((DEBUG_INFO, "WriteProtectedFlashImageEx: New FPA pointer: %x\n", NewFpaOffset));

    /* Pointer is calculated in 4KB sector units. Calculate byte offset basing on that */
    ModulePointerValue = (UINT16) ((NewFpaOffset / IGC_SECTOR_SIZE) | IGC_POINTER_4K_GRANULARITY);
    GetFlashModulePointer (Hw, IGC_FLASH_MODULE_1ST_FREE_AREA, &ModulePointer);

    Status = WriteNvmSrwr (Hw, (UINT32) ModulePointer, &ModulePointerValue, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Writing FPA pointer failed.\n"));
      break;
    }

    FwBufferSize /= IGC_SECTOR_SIZE;
    FwBufferSize_16 =(UINT16)FwBufferSize;

    /*Need to write new FPA size */
    Status = WriteNvmSrwr (Hw, IGC_FPA_SIZE, (UINT16*)&FwBufferSize_16, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteProtectedFlashImageEx: Writing new FPA size failed %x.\n", Status));
      break;
    }

  } while (0);
  return Status;
}

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
 )
{
  UINT16              *EepromBuffer;
  UINT8               *CurrentModuleBuffer;
  IGC_STATUS          Status;
  IGC_FLASH_MODULES   CurrentModule;
  UINT32              CurrentModuleSize;
  UINT32              Index;
  UINT32              CurrentModuleSizeInWords;

  EepromBuffer         = NULL;
  CurrentModuleBuffer  = NULL;
  Status               = IGC_ERR_INVALID_ARGUMENT;
  CurrentModule        = IGC_FLASH_MODULE_SHADOW_RAM;
  CurrentModuleSize    = 0;
  Index                = 0;
  CurrentModuleSizeInWords = 0;

  do {
    DEBUG ((DEBUG_INFO, "Enter WriteProtectedFlashImage\n"));
    Status = WriteProtectedFlashRegion (Hw, Image, ImageSize, WriteMacAddress);
    if (EFI_ERROR (Status)) {
      break;
    }

    /* Get the module image pointer inside the image */
    Status = GetModuleFromImage (Hw, CurrentModule, Image, ImageSize, &CurrentModuleBuffer, &CurrentModuleSize);
    if (Status == IGC_DEVICE_ERROR) {
      DEBUG ((DEBUG_ERROR, "WriteSharedFlashImageEx: Warning: Empty module pointer %x\n", CurrentModule));
      break;
    }
    else if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteSharedFlashImageEx: Error: Problem updating module %x\n", CurrentModule));
      break;
    }

    /* Update Shadow RAM using Shadow RAM functions */
    EepromBuffer = (UINT16*) AllocateZeroPool (CurrentModuleSize);
    if (EepromBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "Couldn't allocate Eeprom Buffer\n"));
      Status = IGC_DEVICE_ERROR;
      break;
    }

    /* Convert 4KB of flash image to EEPROM buffer format */
    if (CurrentModuleBuffer != NULL) {
      for (Index = 0; Index < CurrentModuleSize; Index += 2) {
        EepromBuffer[Index / 2] = MAKE_WORD (CurrentModuleBuffer[Index + 1], CurrentModuleBuffer[Index]);
      }
    }

    CurrentModuleSizeInWords = CurrentModuleSize / 2;

    /* Save FPA pointer and FPA size */
    ReadNvmEerd (Hw, IGC_FPA_PTR, &EepromBuffer[IGC_FPA_PTR], TRUE);
    ReadNvmEerd (Hw, IGC_FPA_SIZE, &EepromBuffer[IGC_FPA_SIZE], TRUE);

    DEBUG ((DEBUG_INFO, "WriteSharedFlashImageEx: Writing EEPROM portion of shared flash image.\n"));

    Status = WriteEepromImage (Hw, EepromBuffer, CurrentModuleSizeInWords, WriteMacAddress);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteSharedFlashImageEx: Error: Problem updating module %x\n", CurrentModule));
      break;
    }
  } while (0);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "WriteSharedFlashImageEx: Error: WriteFlashImage returned %x\n", Status));
  }

  FreePool (EepromBuffer);

  return Status;
}

/**
  Updates secured flash module

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  ModuleOffset                - The offset of the module in the flash
  @param[in]  Buffer                      - A struct to read data from
  @param[in]  BufferSize                  - The size of the data to update

  @retval     IGC_SUCCESS                 - Updating the secured module was successful
  @retval     IGC_DEVICE_ERROR            - Required memory allocation failed
  @retval     IGC_ERR_INVALID_ARGUMENT    - The offset parameters are incorrect

**/
IGC_STATUS
UpdateSecuredModule (
  IN IGC_HW              *Hw,
  IN UINT32              ModuleOffset,
  IN UINT8               *Buffer,
  IN UINT32              BufferSize
 )
{
  IGC_STATUS  Status;
  UINT32      Flfwupdate;
  UINT32      Eec;
  UINT32      Timeout;
  UINT32      Step;

  Status      = IGC_ERR_INVALID_ARGUMENT;
  Flfwupdate  = 0;
  Eec         = 0;
  Timeout     = 20; /* in milliseconds */
  Step        = 200;  /* in milliseconds */

  DEBUG ((DEBUG_INFO, "Enter UpdateSecuredModule\n"));

  do {
    /* Get appropriate Free Provisioning Area */
    Status = GetFlashModuleOffset (Hw, IGC_FLASH_MODULE_1ST_FREE_AREA, &ModuleOffset);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Could not get flash module offset.\n"));
      break;
    }

    /* Write the buffer - we don't need to erase first, as write takes care of that
      * If we've upgraded FLASH module - commit saved Shadow RAM before exiting
      * A special FW images with appended Shadow RAM images for updating RO words in
      * Shadow RAM. To support that we need to save current image to revert changes done in RW words
      */
    DEBUG ((DEBUG_INFO, "UpdateSecuredModule: Upgrading EMP Image - redirecting writes to 1st free area\n"));

    Flfwupdate = ReadMacReg (Hw, IGC_FLFWUPDATE);
    if (Flfwupdate & IGC_FLFWUPDATE_AUTHEN_DONE) {
      DEBUG ((DEBUG_INFO, "UpdateSecuredModule: Previous FW update succeeded.\n"));
    }
    else {
      DEBUG ((DEBUG_ERROR, "UpdateSecuredModule: Previous FW update not succeeded.\n"));
      Status = IGC_ERR_NVM;
      break;
    }

    Eec = ReadMacReg (Hw, IGC_EECD);
    if (Eec & IGC_EECD_FLUDONE_I225) {
      DEBUG ((DEBUG_INFO, "UpdateSecuredModule: Previous EEPROM dump succeeded.\n"));
    }
    else {
      DEBUG ((DEBUG_ERROR, "UpdateSecuredModule: Previous EEPROM dump not succeeded.\n"));
      Status = IGC_SUCCESS;
      break;
    }

    DEBUG ((DEBUG_INFO, "UpdateSecuredModule: Writing new image to free area\n"));

    /* Since WriteFlashRegion takes care of resource ownership we don't do that here
        And we can just write the region by calling the function */
    if ((Hw->FlashInfo->FlashMode != PROTECTED_FLASH_MODE) ||
        (Hw->FlashInfo->FlashMode == PROTECTED_FLASH_MODE && IsFlashRegionProtected(Hw, ModuleOffset, BufferSize) == FALSE)) {
          Status = WriteFlashRegion (Hw, Buffer, BufferSize, ModuleOffset);
    }
    else {
      DEBUG ((DEBUG_ERROR, "Trying to write protected Region\n"));
      Status = WRITE_PROTECTED_REGION;
    }

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "UpdateSecuredModule: Writing flash region failed.\n"));
        break;
    }

    /* Write FLFWUPDATE.Update bit to start authentication procedure */
    DEBUG ((DEBUG_INFO, "UpdateSecuredModule: Triggering the update\n"));
    Flfwupdate = ReadMacReg (Hw, IGC_FLFWUPDATE);
    Flfwupdate |= IGC_FLFWUPDATE_UPDATE;
    Flfwupdate &= ~IGC_FLFWUPDATE_AUTHEN_DONE;
    WriteMacReg (Hw, IGC_FLFWUPDATE, Flfwupdate);

    /* Poll for FLFWUPDATE.AUTHEN-DONE bit set */
    DEBUG ((DEBUG_INFO, "UpdateSecuredModule: Waiting for authentication\n"));
    while (Timeout) {
      /* Delay for 200 ms */
      gBS->Stall (1000 * Step);

      /* If FLFWUPDATE.AUTHEN-DONE bit is set - FW upgrade is finished */
      Flfwupdate = ReadMacReg (Hw, IGC_FLFWUPDATE);
      if (Flfwupdate & IGC_FLFWUPDATE_AUTHEN_DONE) {
        DEBUG ((DEBUG_INFO, "UpdateSecuredModule: FW Image authentication finished.\n"));
        break;
      }
      Timeout -= Step;
    }
    if (Timeout == 0) {
      DEBUG ((DEBUG_ERROR, "UpdateSecuredModule: FW authentication timeout.\n"));
      Status = IGC_FLASH_FW_AUTHENTICATION_TIMEOUT;
    }
    /* If FLFWUPDATE.AUTH_FAIL bit is set - FW upgrade has failed */
    else if (Flfwupdate & IGC_FLFWUPDATE_AUT_FAIL) {
      DEBUG ((DEBUG_ERROR, "UpdateSecuredModule: Bad FW image!\n"));
      Status = IGC_FLASH_AUTHENTICATION_FAILED;
    }
    else {
      DEBUG ((DEBUG_INFO, "UpdateSecuredModule: FW image authenticated.\n"));
      Status = IGC_SUCCESS;
    }

    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "UpdateSecuredModule: Reverting MAC Address\n"));
      Status = UpdateMngCrcs (Hw);
      DEBUG ((DEBUG_INFO, "MNGCrCs Updated!, Status = %r\n", Status));
    }
  } while (0);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "UpdateSecuredModule: ERROR: returned %x\n", Status));
  }

  return Status;
}

/**
  Calculates the CRC for a given section of the firmware

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Pointer                     - Address of the pointer to the section
  @param[out] AdditionalCrc               - Calculated CRC
  @param[out] AdditionalCrcLocation       - Location of the CRC in the flash

  @retval     IGC_SUCCESS                     - CRC calculated successfully
  @retval     IGC_ERR_INVALID_ARGUMENT        - The offset parameters are incorrect
  @retval     IGC_EEPROM_CALCULATION_FAILED   - Calculation of the CRC failed

**/
IGC_STATUS
CalculateFwSectionCrc (
  IN  IGC_HW      *Hw,
  IN  UINT32      Pointer,
  OUT UINT8       *AdditionalCrc,
  OUT UINT16      *AdditionalCrcLocation
 )
{
  IGC_STATUS  Status;
  UINT32      FlashSize;
  UINT32      EepromSize;
  UINT32      ImageSize;
  UINT16      Length;
  UINT16      StartPosition;
  UINT16      Index;
  UINT8       Crc;
  UINT8       Buffer[2] = { 0, 0 };

  Status      = IGC_SUCCESS;
  FlashSize   = Hw->FlashInfo->FlashSize;
  EepromSize  = IGC_EEPROM_SIZE;
  ImageSize   = 0;
  Length      = 0;
  StartPosition = 0;
  Index       = 0;
  Crc         = 0;

  DEBUG ((DEBUG_INFO, "Enter CalculateFwSectionCrc\n"));

  if (AdditionalCrc == NULL || AdditionalCrcLocation == NULL) {
    DEBUG ((DEBUG_ERROR, "Additional CRC or location is NULL\n"));
    Status = IGC_ERR_INVALID_ARGUMENT;
  }
  else {
    do {
      /* Read the location of the specified pointer */
      if (Pointer >= EepromSize) {
        /* Pointers to the Manageability sections are stored on the Flash
          *
          * Firmware Extension Area data will not fall into this code block because Pointer
          * will equal to 0 - pointing at the beginning of the data buffer. We want to
          * use pointers passed in the buffer for Firmware Extension Area data. */

        if (Pointer < FlashSize) {
          *AdditionalCrcLocation = (UINT16) ReadFlashReg (Hw, Pointer * 2);
          *(UINT16*) Buffer = *AdditionalCrcLocation;
          *AdditionalCrcLocation = MAKE_WORD (Buffer[Index + 1], Buffer[Index]);
        }
        else {
          DEBUG ((DEBUG_ERROR, "Pointer is higher than Size Pointer = %d\n", Pointer));
          Status = IGC_ERR_INVALID_ARGUMENT;
          break;
        }

        /* Only some manageability sections can be located in the legacy EEPROM. Skip
          * manageability sections located on the Flash's Firmware Extension Area when we
          * passed-in only EEPROM data */
        if (*AdditionalCrcLocation >= ImageSize) {
          *AdditionalCrcLocation = 0;
        }
      }
      else {
        Status = ReadNvmEerd (Hw, (UINT16) Pointer, AdditionalCrcLocation, TRUE);
      }

      if (EFI_ERROR (Status)) {
        Status = IGC_EEPROM_CALCULATION_FAILED;
        DEBUG ((DEBUG_ERROR, "Couldn't read offset 0x%x for addCRCLoc\n", Pointer));
        break;
      }

      if (*AdditionalCrcLocation == 0 || *AdditionalCrcLocation == 0xFFFF) {
        Status = IGC_SUCCESS;
        break;
      }

      /* Read the header of the additional section */
      Status = ReadNvmEerd (Hw, *AdditionalCrcLocation, (UINT16*)Buffer, TRUE);

      if (EFI_ERROR (Status)) {
        Status = IGC_EEPROM_CALCULATION_FAILED;
        DEBUG ((DEBUG_ERROR, "Couldn't read offset 0x%x for TradSec\n", *AdditionalCrcLocation));
        break;
      }

      /* Parse the first word in the section */
      StartPosition = *AdditionalCrcLocation;
      Length = Buffer[0] * sizeof(UINT16);

      for (Index = StartPosition; Index < (Length / 2) + StartPosition; Index++) {
        ReadNvmEerd (Hw, Index, (UINT16*)Buffer, TRUE);
        Crc = Global_CrcTable[Crc ^ Buffer[0]];

        /* We are supposed to presume that the top byte of first word holds 0.
          * This byte holds the current CRC we are updating.
          */
        if (Index != StartPosition) {
          Crc = Global_CrcTable[Crc ^ Buffer[1]];
        }
        else {
          Crc = Global_CrcTable[Crc ^ 0];
        }
      }

      /* Return the Crc for this section. */
      *AdditionalCrc = Crc;
      Status = IGC_SUCCESS;
    } while (0);
  }

  return Status;
}

/**
  Calculates and updates the CRC for a given section of the firmware

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  Pointer                     - Address of the pointer to the section

  @retval     IGC_SUCCESS                     - CRC calculated successfully
  @retval     IGC_EEPROM_CALCULATION_FAILED   - Calculation of the CRC failed
  @retval     IGC_DEVICE_ERROR                - Couldn't read/write the NVM word

**/
IGC_STATUS
EvaluateFwSectionCrc (
  IN IGC_HW    *Hw,
  IN UINT32    Pointer
 )
{
  IGC_STATUS  Status;
  UINT16      AdditionalCrcLocation;
  UINT16      SectionHeader;
  UINT8       AdditionalCrc;

  Status = IGC_SUCCESS;
  AdditionalCrcLocation = 0;
  SectionHeader = 0;
  AdditionalCrc = 0;

  DEBUG ((DEBUG_INFO, "Enter EvaluateFwSectionCrc\n"));

  do {
    Status = CalculateFwSectionCrc (Hw, Pointer, &AdditionalCrc, &AdditionalCrcLocation);

    if (EFI_ERROR (Status)) {
      Status = IGC_EEPROM_CALCULATION_FAILED;
      break;
    }

    /* No addition section in the firmware */
    if (AdditionalCrcLocation == 0) {
      break;
    }

    /* The value is illegal */
    if (AdditionalCrcLocation == 0xFFFF) {
      Status = IGC_ERR_NVM;
      break;
    }

    Status = ReadNvmEerd (Hw, AdditionalCrcLocation, &SectionHeader, TRUE);
    if (EFI_ERROR (Status)) {
      Status = IGC_DEVICE_ERROR;
      break;
    }

    /* We are updating the CRC */
    SectionHeader = MAKE_WORD (AdditionalCrc, LOBYTE (SectionHeader));

    Status = WriteNvmSrwr (Hw, LOWORD (AdditionalCrcLocation), &AdditionalCrcLocation, TRUE);

    if (EFI_ERROR (Status)) {
      Status = IGC_DEVICE_ERROR;
      break;
    }
  } while (0);

  return Status;
}

/**
  Updates all the MNG sections CRCs

  @param[in]  Hw                              - Pointer to hardware instance

  @retval     IGC_SUCCESS                     - CRCs were updated successfully
  @retval     IGC_EEPROM_CALCULATION_FAILED   - Calculation of the CRC failed
  @retval     IGC_DEVICE_ERROR                - Couldn't read/write the NVM word

**/
IGC_STATUS
UpdateMngCrcs (
  IN  IGC_HW    *Hw
 )
{
  IGC_STATUS  Status;
  IGC_STATUS  ExitStatus;
  UINT16      FWSectionPointer;
  UINT16      ManagementMode;

  Status           = IGC_SUCCESS;
  ExitStatus       = IGC_SUCCESS;
  FWSectionPointer = 0;
  ManagementMode   = 0;

  DEBUG ((DEBUG_INFO, "Enter UpdateMngCrcs\n"));

  do {
    /* Get the FW Section Pointer */
    Status = ReadNvmEerd (Hw, IGC_FW_MODULE_POINTER, &FWSectionPointer, TRUE);

    /* If the Pointer is all F's then there is nothing to do, so exit */
    if (EFI_ERROR (Status) || FWSectionPointer == 0xFFFF) {
      break;
    }

    /* Update the Common FW parameters CRC */
    Status = EvaluateFwSectionCrc(Hw, (FWSectionPointer + IGC_FW_COMMON_PARMS_OFFSET));
    if (EFI_ERROR (Status)) {
      ExitStatus = Status;
    }

    /* Update the Flexible TCO Filters Config CRC */
    Status = EvaluateFwSectionCrc(Hw, (FWSectionPointer + IGC_FW_FLEX_FILTERS_OFFSET));

    /* It is possible that this module is disabled and above function will return
      * IGC_EEPROM_READ_FAILED */
    if ((EFI_ERROR (Status)) && (Status != IGC_DEVICE_ERROR)) {
      ExitStatus = Status;
    }

    /* Update the Traffic Type Params CRC */
    Status = EvaluateFwSectionCrc(Hw, (FWSectionPointer + IGC_FW_TRAFFIC_TYPES_OFFSET));
    if (EFI_ERROR (Status)) {
      ExitStatus = Status;
    }

    /* Get the Common MNG parameters section pointer */
    Status = ReadNvmEerd (Hw, (FWSectionPointer + IGC_FW_COMMON_PARMS_OFFSET), &ManagementMode, TRUE);
    if (EFI_ERROR (Status)) {
      ExitStatus = Status;
    }
    else {
      Status = ReadNvmEerd (Hw, (ManagementMode + IGC_FW_MNG_MODE_OFFSET), &ManagementMode, TRUE);
      if (EFI_ERROR (Status)) {
        ExitStatus = Status;
      } else {
        ManagementMode &= IGC_MANAGEABILITY_MASK; /* bits 8-10 */
      }

      /* Now check SPT unique sections */
      if ((!EFI_ERROR (Status)) && (ManagementMode == IGC_SPT_CAPABLE)) {
        /* Update the SideBand CRC */
        Status = EvaluateFwSectionCrc (Hw, FWSectionPointer + IGC_FW_SIDEBAND_CONFIG_OFFSET);
        if (EFI_ERROR (Status)) {
          ExitStatus = Status;
        }

        /* Get the Port Configuration Pointer */
        Status = EvaluateFwSectionCrc(Hw, FWSectionPointer + IGC_PT_LAN0_POINTER);
        if (EFI_ERROR (Status)) {
          ExitStatus = Status;
        }
      }
    }
  } while (0);

  return ExitStatus;
}

/**
  Write DataSize bytes of data at a time from flash starting at given offset

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  StartOffset                 - Start offset in the flash to write
  @param[in]  DataSize                    - Size of data to read
  @param[in]  DataToWrite                 - Data values to be written

  @retval     IGC_SUCCESS                 - Writing the data succeeded
  @retval     IGC_ERR_INVALID_ARGUMENT    - Given parameters are incorrect

**/
IGC_STATUS
WriteFlashData (
  IN IGC_HW    *Hw,
  IN UINT32    StartOffset,
  IN UINT32    DataSize,
  IN UINT8*    DataToWrite
 )
{
  IGC_STATUS  Status;
  UINT32      *CurrentOffsetData;
  UINT32      DwordsToWrite;
  UINT32      BytesToWrite;
  UINT32      DwordsLeftToWrite;
  UINT32      CurrentOffset;
  UINT32      Index;
  UINT32      FlashSize;
  BOOLEAN     Continue;

  Status              = IGC_SUCCESS;
  CurrentOffsetData   = (UINT32*)DataToWrite;
  DwordsToWrite       = 0;
  BytesToWrite        = 0;
  DwordsLeftToWrite   = (DataSize & ~0x3) / 4;
  CurrentOffset       = StartOffset;
  Index               = 0;
  FlashSize           = Hw->FlashInfo->FlashSize;
  Continue            = FALSE;

  DEBUG ((DEBUG_INFO, "Entering WriteFlashData\n"));

  if (!EFI_ERROR (Status)) {
    /* Check if we don't try to write beyond the Flash Size */
    if (StartOffset + DataSize > FlashSize) {
      Status = IGC_ERR_INVALID_ARGUMENT;
    }
    else {
      /* Check if Flash is ready */
      Status = GetFlswFlashOperationDone (Hw);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Flash not ready!\n"));
      }
    }
  }

  if (!EFI_ERROR (Status) && DwordsLeftToWrite != 0) {
    do {
      /* We can only write MAX_FLASH_DWORDS at a time. */
      if (DwordsLeftToWrite > IGC_FLSW_MAX_FLASH_DWORDS_TO_WRITE) {
        DwordsToWrite = IGC_FLSW_MAX_FLASH_DWORDS_TO_WRITE;
        DwordsLeftToWrite -= DwordsToWrite;
        Continue = TRUE;
      }
      else {
        DwordsToWrite = DwordsLeftToWrite;
        Continue = FALSE;
      }

      BytesToWrite = DwordsToWrite * 4;

      /* Set burst write length */
      Status = SetFlswBurstCounter (Hw, BytesToWrite);
      if (EFI_ERROR (Status)) {
        break;
      }

      /* Set address and opcode */
      Status = SendFlashCommand (Hw, IGC_FLSWCTL_CMD_WRITE, CurrentOffset);
      if (EFI_ERROR (Status)) {
        break;
      }

      /* Wait till operation has finished */
      Status = GetFlswFlashOperationDone (Hw);
      if (EFI_ERROR (Status)) {
        break;
      }

      Status = AcquireSwFwSemaphore (Hw);
      if (EFI_ERROR (Status)) {
        break;
      }

      for (Index = 0; Index < DwordsToWrite; Index++) {
        /* Write data to FLSWDATA register */
        WriteMacReg (Hw, IGC_FLSWDATA, *CurrentOffsetData);
        CurrentOffsetData++;
        CurrentOffset += 4;
        /* Wait for write completed */
        Status = GetFlswFlashOperationDone (Hw);
        if (EFI_ERROR (Status)) {
          break;
        }
      }

      ReleaseSwFw (Hw);
    } while (Continue == TRUE);
  }

  return Status;
}

/**
  Erase a given region in the flash

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  RegionOffset                - Starting offset of the region in the flash
  @param[in]  RegionSize                  - Size of the given region

  @retval     IGC_SUCCESS                 - Region was erased successfully

**/
IGC_STATUS
EraseFlashRegion (
  IN IGC_HW    *Hw,
  IN UINT32    RegionOffset,
  IN UINT32    RegionSize
 )
{
  IGC_STATUS  Status;
  UINT32      FlashSize;
  UINT32      CurrentSector;
  UINT32      RegionStart;
  UINT32      RegionEnd;
  UINT32      BurstEnd;

  Status          = IGC_SUCCESS;
  FlashSize       = Hw->FlashInfo->FlashSize;
  CurrentSector   = 0;
  RegionStart     = 0;
  RegionEnd       = 0;
  BurstEnd        = 0;

  DEBUG ((DEBUG_INFO, "Entering EraseFlashRegion\n"));

  /* Align Region to sector size */
  /* If RegionSize is not a 4k multiple then round Region Size to next 4k boundary */
  RegionStart = RegionOffset & IGC_FLASH_4K_SECTOR_MASK;
  RegionEnd = RegionStart + ((RegionSize + (IGC_FLASH_4K_SECTOR_SIZE - 1)) & IGC_FLASH_4K_SECTOR_MASK);

  do {
    if (RegionEnd > FlashSize) {
      Status = IGC_ERR_INVALID_ARGUMENT;
      DEBUG ((DEBUG_ERROR, "EraseFlashRegion: Region offset: %08x + Region Size: %08x exceeded FlashSize: %08x\n", RegionOffset, RegionSize, FlashSize));
      break;
    }

    /* Wait for FLASH to be ready for next operation */
    Status = GetFlswFlashOperationDone (Hw);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "EraseFlashRegion: Flash not ready!\n"));
      break;
    }

    do {
      /* If possible erase 4 sectors at once */
      if (RegionStart + IGC_FLSW_MAX_SECTORS_TO_ERASE * IGC_FLASH_4K_SECTOR_SIZE < RegionEnd) {
        BurstEnd = RegionStart + IGC_FLSW_MAX_SECTORS_TO_ERASE * IGC_FLASH_4K_SECTOR_SIZE;
      }
      else {
        BurstEnd = RegionEnd;
      }

      Status = AcquireSwFwSemaphore (Hw);
      if (EFI_ERROR (Status)) {
        break;
      }

      /* Erase all sectors in burst */
      for (CurrentSector = RegionStart; CurrentSector < BurstEnd; CurrentSector += IGC_FLASH_4K_SECTOR_SIZE) {
        /* Issue Flash sector erase command on current sector */
        Status = SendFlashCommand (Hw, IGC_FLSWCTL_CMD_SECTOR_ERASE, CurrentSector);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "EraseFlashRegion: Cannot erase sector %08x\n", CurrentSector));
          break;
        }

        /* Wait for Flash to finish pending operation.*/
        Status = GetFlswFlashOperationDone (Hw);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "EraseFlashRegion: Sector erase timed out for sector %08x\n", CurrentSector));
          break;
        }
      }
      ReleaseSwFw (Hw);
      RegionStart = BurstEnd;

      /* Delay before reacquiring Flash ownership */
      if (BurstEnd != RegionEnd) {
        gBS->Stall (10 * 1000);
      }
    } while (RegionStart < RegionEnd && !EFI_ERROR (Status));
  } while (0);

  return Status;
}

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
  IN IGC_HW    *Hw,
  IN UINT8*    Image,
  IN UINT32    ImageSize,
  IN UINT32    RegionOffset
 )
{
  IGC_STATUS  Status;
  UINT32      FlashSize;

  Status      = IGC_SUCCESS;
  FlashSize   = Hw->FlashInfo->FlashSize;

  DEBUG ((DEBUG_INFO, "Entering WriteFlashRegion\n"));

  do {
    /* Check if the image given fills minimal criteria. */
    if (Image == NULL || (RegionOffset + ImageSize) > FlashSize || ImageSize == 0) {
      DEBUG ((DEBUG_ERROR, "WriteFlashRegion: Flash image doesn't fit flash device\n"));
      Status = IGC_ERR_INVALID_ARGUMENT;
      break;
    }

    Status = EraseFlashRegion (Hw, RegionOffset, ImageSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteFlashRegion: Flash region cannot be erased\n"));
      break;
    }

    Status = WriteFlashData (Hw, RegionOffset, ImageSize, Image);
    DEBUG ((DEBUG_INFO, "Finished Writing FlsWriteFlashData Status =  %d", Status));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WriteFlashRegion: Flash region cannot be written\n"));
      break;
    }
  } while (0);

  return Status;
}

/**
  Writes an image to the EEPROM

  @param[in]  Hw                          - Pointer to hardware instance
  @param[in]  ImageToWrite                - Pointer to the data to write
  @param[in]  ImageToWriteSize            - Size of the data to write
  @param[in]  WriteMacAddress             - Boolean value whether to update the MAC address

  @retval     IGC_SUCCESS                 - EEPROM was written successfully
  @retval     IGC_ERR_INVALID_ARGUMENT    - Given parameters are incorrect

**/
IGC_STATUS
WriteEepromImage (
  IN     IGC_HW             *Hw,
  IN     UINT16             *ImageToWrite,
  IN     UINT32             ImageToWriteSize,
  IN     BOOLEAN            WriteMacAddress
 )
{
  IGC_STATUS  Status;
  UINT32      Offset;
  UINT16      WriteStart;

  Status = IGC_SUCCESS;
  Offset = 0;
  WriteStart = 0;

  DEBUG ((DEBUG_INFO, "Enter WriteEepromImage\n"));

  do {
    if (ImageToWrite == NULL) {
      Status = IGC_ERR_INVALID_ARGUMENT;
      break;
    }

    if (WriteMacAddress == FALSE) {
      /* WriteMacAddress is FALSE so do not write words 0x0 - 0x2.
        * Start the write at word 0x3. */
      WriteStart = 3;
    }

    if (ImageToWriteSize > IGC_EEPROM_SIZE) {
      /* If the image to write is larger than the Eeprom target size, then only
          write up to the Eeprom target size */
      ImageToWriteSize = IGC_EEPROM_SIZE;
    }

    /* Write the rest of the Eeprom Image */
    for (Offset = WriteStart; Offset < ImageToWriteSize; Offset++)
    {
      /* Do not override some  of the offsets since
        * FW has already updated them */
      if ((Offset == IGC_FW_MODULE_POINTER_OFFSET) ||
          (Offset == IGC_FPA_PTR) || (Offset == IGC_EXP_ROM_BOOT_CODE_SEC_PTR) ||
          (Offset == IGC_PHY_POINTER) || (Offset == IGC_SW_AREA_POINTER))
      {
        continue;
      }

      Status = WriteNvmSrwr (Hw, Offset, &ImageToWrite[Offset], TRUE);
      if (EFI_ERROR (Status)) {
        break;
      }
    }

    if (!EFI_ERROR (Status)) {
      if (!EFI_ERROR (Status)) {
        Status = UpdateNvmChecksum (Hw);
      }
    }
  } while (0);

  return Status;
}

/**
  Read the current NVM version from the flash.

  @param[in]   Hw          Pointer to hardware instance.
  @param[out]  NvmVersion  Pointer to FOXVILLE_DEV_STARTER_VERSION struct.

  @retval  IGC_SUCCESS  Succeed to read the NVM version from flash.
  @retval  Others       Failed to read the NVM version from flash.

**/
IGC_STATUS
IgcGetNvmVersion (
  IN     IGC_HW                        *Hw,
     OUT FOXVILLE_DEV_STARTER_VERSION  *NvmVersion
  )
{
  IGC_STATUS  Status;

  DEBUG ((DEBUG_INFO, "Getting NVM Version\n"));

  Status = ReadNvmEerd (Hw, IGC_DEV_STARTER_WORD, &(NvmVersion->Uint16), TRUE);

  return Status;
}

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
  IN IGC_HW    *Hw
 )
{
  UINT16 Index;

  DEBUG ((DEBUG_INFO, "Enter IgcResetPHYFW\n"));

  for (Index = 0; Index < 2; Index++) {
    WriteMacReg (Hw, IGC_MDICNFG, 0);
    gBS->Stall (100);

    WritePhyMmdReg (Hw, IGC_MMD_ACC_CTRL, 0x1E);
    WritePhyMmdReg (Hw, IGC_MMD_ACC_DATA, 0x7);
    WritePhyMmdReg (Hw, IGC_MMD_ACC_CTRL, 0x401E);
    ReadPhyMmdReg (Hw, IGC_MMD_ACC_DATA, 0);
    WritePhyMmdReg (Hw, IGC_MMD_ACC_DATA, 0x600);
    ReadPhyMmdReg (Hw, IGC_MMD_ACC_DATA, 0);
  }

  WriteMacReg (Hw, IGC_MDICNFG, 0);
  gBS->Stall (100);

  WritePhyMmdReg (Hw, IGC_MMD_ACC_CTRL, 0);
  WritePhyMmdReg (Hw, IGC_MMD_ACC_DATA, 0);
  WritePhyMmdReg (Hw, IGC_MMD_ACC_CTRL, 0x4000);
  ReadPhyMmdReg (Hw, IGC_MMD_ACC_DATA, 0);
  WritePhyMmdReg (Hw, IGC_MMD_ACC_DATA, 0x8000);
  ReadPhyMmdReg (Hw, IGC_MMD_ACC_DATA, 0);

  return IGC_SUCCESS;
}
