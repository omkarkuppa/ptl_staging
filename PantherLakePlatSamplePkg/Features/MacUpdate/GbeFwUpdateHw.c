/** @file
     Gbe Fw Update Hw Driver.

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

#include "GbeFwUpdateDefines.h"

/**
  This function calls the MemIo callback to read a DWord from the device's address space.

  @param[in]    Hw             Pointer to hardware instance.
  @param[in]    Reg            Which register to read from.

  @return                      The Data read from the register.
**/
UINT32
ReadMacReg (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT32           Reg
  )
{
  UINT32 Results;
  ADAPTER_INFO* Adapter;
  Adapter = Hw->Back;

  MemoryFence ();
  Adapter->PciIo->Mem.Read (
           Adapter->PciIo,
           EfiPciIoWidthUint32,
           0,
           Reg,
           1,
           (VOID*)(&Results)
           );
  MemoryFence ();
  return Results;
}

/**
  This function writes to a register in the flash space.

  @param[in]    Hw            Pointer to hardware instance.
  @param[in]    Data          Data to write to Port.
  @param[in]    Reg           Which register to write to.

  @return                     None.
**/
VOID
WriteFlashReg (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT32          Reg,
  IN UINT32          Data
  )
{
  MemoryFence ();
  UINT32* Addr = (UINT32*)(Hw->FlashAddr + Reg);
  *Addr = Data;
  MemoryFence();
  return;
}

/**
  This function writes to a register in the address space.

  @param[in]      Hw            Pointer to hardware instance.
  @param[in]      Reg           Which port to write to.
  @param[in]      Data          Data to write to Port.

  @return                       None.
**/
VOID
WriteMacReg (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT32           Reg,
  IN UINT32           Data
  )
{
  UINT32 Value;
  ADAPTER_INFO* Adapter;
  Adapter = Hw->Back;
  Value = Data;

  MemoryFence ();
  Adapter->PciIo->Mem.Write (
           Adapter->PciIo,
           EfiPciIoWidthUint32,
           0,
           Reg,
           1,
           (VOID*)(&Value)
           );
  MemoryFence ();
  return;
}

/**
  This function reads data from a register in the flash space.

  @param[in]    Hw            Pointer to hardware instance.
  @param[in]    Reg           Which port to read from.

  @return                     The Data read from the port.
**/
UINT32
ReadFlashReg (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT32 Reg
  )
{
  UINT32  Results;
  MemoryFence ();
  Results = (*(UINT32*)(Hw->FlashAddr + Reg));
  MemoryFence();
  return Results;
}

/**
  This function calls the MemIo callback to read a DWord from the device's
  address space
  Since UNDI3.0 uses the TmpMemIo function (instead of the callback routine)
  which also takes the UniqueId parameter (as in UNDI3.1 spec) we don't have
  to make undi3.0 a special case.

  @param[in]    Hw           Pointer to hardware instance.
  @param[in]    Reg          Which port to read from.

  @return                    The Data read from the port.
**/
UINT16
ReadFlashReg16 (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT32 Reg
  )
{
  UINT16  Results;
  MemoryFence ();
  Results = (*(UINT16*)(Hw->FlashAddr + Reg));
  MemoryFence();
  return Results;
}

/**
  This function initializes the flash.
  This function does initial flash setup so that a new read/write/erase cycle
  can be started.

  @param[in]        Hw            Pointer to hardware instance.

  @return           Status        The return status for this function.
**/
STATIC
GBE_FW_UPDATE_STATUS
FlashCycleInit (
  IN struct GBE_FW_UPDATE_HW* Hw
  )
{
  union FLASH_STATUS FSts;
  GBE_FW_UPDATE_STATUS Status = -GBE_FW_UPDATE_INVALID_PARAMETER;
  FSts.RegVal = ReadFlashReg16 (Hw, ICH_FLASH_HSFSTS);
  // Check if the flash descriptor is valid

  if (!FSts.HSF_STATUS.FDescValid) {
    DEBUG ((DEBUG_INFO,"Flash descriptor invalid.  SW Sequencing must be used.\n"));
    return -GBE_FW_UPDATE_INVALID_PARAMETER;
  }
  // Clear FCERR and DAEL in Hw Status by writing 1

  FSts.HSF_STATUS.FCErr = 1;
  FSts.HSF_STATUS.DAEL = 1;
  WriteFlashReg (Hw, ICH_FLASH_HSFSTS,FSts.RegVal & 0xFFFF);
  // Either we should have a hardware SPI cycle in progress
  // bit to check against, in order to start a new cycle or
  // FDONE bit should be changed in the hardware so that it
  // is 1 after hardware reset, which can then be used as an
  // indication whether a cycle is in progress or has been
  // completed.

  if (!FSts.HSF_STATUS.FCInProg) {
    // There is no cycle running at present,
    // so we can start a cycle.
    // Begin by setting Flash Cycle Done.
    FSts.HSF_STATUS.FCDone = 1;
    WriteFlashReg (Hw, ICH_FLASH_HSFSTS, FSts.RegVal & 0xFFFF);
    Status = GBE_FW_UPDATE_SUCCESS;
  }
  else {
    INT32 i;
    // Otherwise poll for sometime so the current
    // cycle has a chance to end before giving up.
    for (i = 0; i < ICH_FLASH_READ_COMMAND_TIMEOUT; i++) {
      FSts.RegVal = ReadFlashReg16 (Hw, ICH_FLASH_HSFSTS);
      if (!FSts.HSF_STATUS.FCInProg) {
        Status = GBE_FW_UPDATE_SUCCESS;
        break;
      }
      gBS->Stall(1);
    }
    if (Status == GBE_FW_UPDATE_SUCCESS) {
      // Successful in waiting for previous cycle to Timeout,
      // now set the Flash Cycle Done.

      FSts.HSF_STATUS.FCDone = 1;
      WriteFlashReg (Hw, ICH_FLASH_HSFSTS,FSts.RegVal & 0xFFFF);
    }
  }
  return Status;
}

/**
  This function Starts flash cycle (read/write/erase).
  This function starts a flash cycle and waits for its completion.

  @param[in]    Hw             Pointer to hardware instance.
  @param[in]    Timeout        Maximum time to wait for completion.

  @return       Status         The return status for this function.
**/
STATIC GBE_FW_UPDATE_STATUS
StartFlashCycle (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT32 Timeout
  )
{
  union FLASH_CTRL FCtrl;
  union FLASH_STATUS FSts;
  UINT32 i = 0;

  // Start a cycle by writing 1 in Flash Cycle Go in Hw Flash Control
  FCtrl.RegVal = ReadFlashReg (Hw, ICH_FLASH_HSFSTS) >> 16;
  FCtrl.HSF_CTRL.FCGo = 1;
  WriteFlashReg (Hw, ICH_FLASH_HSFSTS,FCtrl.RegVal << 16);
  // wait till FDONE bit is set to 1
  do {
    FSts.RegVal = ReadFlashReg16 (Hw, ICH_FLASH_HSFSTS);
    if (FSts.HSF_STATUS.FCDone)
      break;
    gBS->Stall (1);
  } while (i++ < Timeout);
  if (FSts.HSF_STATUS.FCDone && !FSts.HSF_STATUS.FCErr){
    return GBE_FW_UPDATE_SUCCESS;
  }
  return -GBE_FW_UPDATE_INVALID_PARAMETER;
}

/**
  This funcion reads the word in the given offset.

  @param[in]    Hw            Pointer to hardware instance.
  @param[in]    Offset        The offset to the word to read.
  @param[out]   Data          The  data read from the word.

  @return       Status        The return status for this function.
**/
STATIC GBE_FW_UPDATE_STATUS
ReadFlashData (
  IN  struct GBE_FW_UPDATE_HW* Hw,
  IN  UINT32 Offset,
  OUT UINT32* Data
  )
{
  union FLASH_STATUS FSts;
  union FLASH_CTRL FCtrl;
  UINT32 FlashLinearAddr;
  GBE_FW_UPDATE_STATUS Status = GBE_FW_UPDATE_INVALID_PARAMETER;
  UINT8 Count = 0;

  // Must convert word Offset into bytes.
  Offset <<= 1;
  if (Offset > ICH_FLASH_LINEAR_ADDR_MASK) {
    return GBE_FW_UPDATE_INVALID_PARAMETER;
  }
  FlashLinearAddr = ((ICH_FLASH_LINEAR_ADDR_MASK & Offset));
  do {
    gBS->Stall(1);
    // Steps
    Status = FlashCycleInit (Hw);
    if (Status != GBE_FW_UPDATE_SUCCESS)
      break;
    // In SPT and above, This register is in Lan memory space, not flash.
    // Therefore, only 32 bit access is supported

    FCtrl.RegVal = ReadFlashReg (Hw, ICH_FLASH_HSFSTS) >> 16;
    // 0b/1b corresponds to 1 or 2 byte size, respectively.
    FCtrl.HSF_CTRL.FDBCount = sizeof(UINT32) - 1;
    FCtrl.HSF_CTRL.FCycle = ICH_CYCLE_READ;
    // In SPT and above, This register is in Lan memory space, not flash.
    // Therefore, only 32 bit access is supported

    WriteFlashReg (Hw, ICH_FLASH_HSFSTS,(UINT32)FCtrl.RegVal << 16);
    WriteFlashReg (Hw, ICH_FLASH_FADDR, FlashLinearAddr);
    Status = StartFlashCycle (Hw, ICH_FLASH_READ_COMMAND_TIMEOUT);
    // Check if FCERR is set to 1, if set to 1, clear it
    // and try the whole sequence a few more times, else
    // read in (shift in) the Flash Data0, the order is
    // least significant byte first msb to lsb
    if (Status == GBE_FW_UPDATE_SUCCESS) {
      *Data = ReadFlashReg (Hw, ICH_FLASH_FDATA0);
      break;
    }
    else {
      // If we've gotten here, then things are probably
      // completely hosed, but if the error condition is
      // detected, it won't hurt to give it another try.
      // ICH_FLASH_CYCLE_REPEAT_COUNT times.

      FSts.RegVal = ReadFlashReg16 (Hw,ICH_FLASH_HSFSTS);
      if (FSts.HSF_STATUS.FCErr) {
        // Repeat for some time before giving up.
        continue;
      }
      else if (!FSts.HSF_STATUS.FCDone) {
        DEBUG ((DEBUG_INFO,"Timeout error - flash cycle did not complete.\n"));
        break;
      }
    }
  } while (Count++ < ICH_FLASH_CYCLE_REPEAT_COUNT);
  return Status;
}

/**
  This function detecs whether the valid bank is 0 or 1.
  This function starts a flash cycle and waits for its completion.

  @param[in]    Hw            Pointer to hardware instance.
  @param[out]   Bank          Pointer to the variable that returns the active Bank.

  @return       Status        The return status for this function.
  @retval       GBE_FW_UPDATE_SUCCESS
  @retval       GBE_FW_UPDATE_INVALID_PARAMETER
**/
STATIC GBE_FW_UPDATE_STATUS
DetectValidNVMBank (
  IN struct GBE_FW_UPDATE_HW* Hw,
  OUT UINT32* Bank
  )
{
  UINT32 Bank1Offset = Hw->FlashBankSize * sizeof (UINT16);
  UINT32 ActOffset = GBE_FW_UPDATE_ICH_NVM_SIG_WORD * 2 + 1;
  UINT32 NVMDWord = 0;
  UINT8 SigByte = 0;
  GBE_FW_UPDATE_STATUS Status;

  Bank1Offset = Hw->FlashBankSize;
  ActOffset = GBE_FW_UPDATE_ICH_NVM_SIG_WORD;
  // set Bank to 0 in case flash read fails
  *Bank = 0;
  // Check Bank 0
  Status = ReadFlashData (Hw, ActOffset, &NVMDWord);
  if (Status) {
    return Status;
  }
  SigByte = (UINT8)((NVMDWord & 0xFF00) >> 8);
  if ((SigByte & GBE_FW_UPDATE_ICH_NVM_VALID_SIG_MASK) == GBE_FW_UPDATE_ICH_NVM_SIG_VALUE) {
    *Bank = 0;
    return GBE_FW_UPDATE_SUCCESS;
  }
  // Check Bank 1
  Status = ReadFlashData (Hw, ActOffset + Bank1Offset, &NVMDWord);
  if (Status) {
    return Status;
  }
  SigByte = (UINT8)((NVMDWord & 0xFF00) >> 8);
  if ((SigByte & GBE_FW_UPDATE_ICH_NVM_VALID_SIG_MASK) == GBE_FW_UPDATE_ICH_NVM_SIG_VALUE) {
    *Bank = 1;
    return GBE_FW_UPDATE_SUCCESS;
  }
  return GBE_FW_UPDATE_INVALID_PARAMETER;
}

/**
  This function detecs whether the valid bank is 0 or 1.
  This function starts a flash cycle and waits for its completion.

  @param[in]    Hw                  Pointer to hardware instance.
  @param[in]    Offset              The Offset (in bytes) of the word(s) to read.
  @param[in]    Words               Size of Data to read in Words.
  @param[out]   Data                Pointer to the word(s) to read at Offset.

  @return       Status              The return status for this function.
  @retval       GBE_FW_UPDATE_SUCCESS
  @retval       GBE_FW_UPDATE_INVALID_PARAMETER
**/
GBE_FW_UPDATE_STATUS
ReadNVM (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT16 Offset,
  IN UINT16 Words,
  OUT UINT16* Data
  )
{
  UINT32 ActOffset;
  INT32 Status = GBE_FW_UPDATE_SUCCESS;
  UINT32 Bank = 0;
  UINT32 DWord = 0;
  UINT16 offset_to_read;
  UINT16 i = 0;

  if ((Offset >= GBE_FW_UPDATE_SHADOW_RAM_WORDS) || (Words > GBE_FW_UPDATE_SHADOW_RAM_WORDS - Offset) ||(Words == 0)) {
    Status = GBE_FW_UPDATE_INVALID_PARAMETER;
    goto out;
  }
  Status = DetectValidNVMBank (Hw, &Bank);
  if (Status != GBE_FW_UPDATE_SUCCESS) {
    DEBUG ((DEBUG_INFO,"Could not detect valid Bank, assuming Bank 0\n"));
    Bank = 0;
  }
  ActOffset = (Bank) ? Hw->FlashBankSize : 0;
  ActOffset += Offset;
  Status = GBE_FW_UPDATE_SUCCESS;
  for (i = 0; i < Words; i += 2) {
    if (Words - i == 1) {
      if (Hw->ShadowRam[Offset + i].Modified) {
        Data[i] = Hw->ShadowRam[Offset + i].Value;
      }
      else {
        offset_to_read = (UINT16)ActOffset + i - (((UINT16)ActOffset + i) % 2);
        Status = ReadFlashData (Hw, offset_to_read, &DWord);
        if (Status){
          break;
        }
        if ((ActOffset + i) % 2 == 0){
          Data[i] = (UINT16)(DWord & 0xFFFF);
        }
        else {
          Data[i] = (UINT16)((DWord >> 16) & 0xFFFF);
        }
      }
    }
    else {
      offset_to_read = (UINT16)ActOffset + i;
      if (!(Hw->ShadowRam[Offset + i].Modified) ||
      !(Hw->ShadowRam[Offset + i + 1].Modified)) {
        Status = ReadFlashData (Hw, offset_to_read, &DWord);
        if (Status) {
          break;
        }
      }
      if (Hw->ShadowRam[Offset + i].Modified){
        Data[i] = Hw->ShadowRam[Offset + i].Value;
      }
      else {
        Data[i] = (UINT16)(DWord & 0xFFFF);
      }
      if (Hw->ShadowRam[Offset + i + 1].Modified) {
        Data[i + 1] = Hw->ShadowRam[Offset + i + 1].Value;
      }
      else {
        Data[i + 1] = (UINT16)(DWord >> 16 & 0xFFFF);
      }
    }
  }
out:
if (Status){
  DEBUG ((DEBUG_INFO,"NVM read error: %d\n", Status));
}
return Status;
}

/**
  This function saves the NVM data to ShadowRam struct for later
  use when wrting the data to the flash.

  @param[in]    Hw                 Pointer to hardware instance.
  @param[in]    Offset             The Offset (in bytes) of the word(s) to read.
  @param[in]    Word               Size of Data to read in Words.
  @param[out]   Data               Pointer to the word(s) to read at Offset.

  @return       Status             The return status for this function.
  @retval       GBE_FW_UPDATE_SUCCES
  @retval       GBE_FW_UPDATE_INVALID_PARAMETER
**/
GBE_FW_UPDATE_STATUS
SaveNVMData (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT16 Offset,
  IN UINT16 Words,
  OUT UINT16* Data
  )
{
  UINT16 i;
  if ((Offset >= GBE_FW_UPDATE_SHADOW_RAM_WORDS) || (Words > GBE_FW_UPDATE_SHADOW_RAM_WORDS - Offset) || (Words == 0)) {
    return GBE_FW_UPDATE_INVALID_PARAMETER;
  }
  for (i = 0; i < Words; i++) {
    Hw->ShadowRam[Offset + i].Modified = TRUE;
    Hw->ShadowRam[Offset + i].Value = Data[i];
  }
  return GBE_FW_UPDATE_SUCCESS;
}

/**
  This function updates the EEPROM checksum by reading/adding
  each word of the EEPROM up to the checksum.  Then calculates
  the EEPROM checksum and writes the value to the EEPROM.

  @param[in]    Hw            Pointer to hardware instance.

  @retval       Status        The return status for this function.
**/
STATIC
GBE_FW_UPDATE_STATUS
UpdateNVMChecksum (
  IN struct GBE_FW_UPDATE_HW* Hw
  )
{
  GBE_FW_UPDATE_STATUS Status;
  UINT16 Checksum = 0;
  UINT16 i;
  UINT16 NvmData [NVM_CHECKSUM_REG];

  ZeroMem (NvmData, NVM_CHECKSUM_REG);
  for (i = 0; i < NVM_CHECKSUM_REG; i++) {
    Status = ReadNVM (Hw, i, 1, &NvmData[i]);
    if (Status) {
      DEBUG ((DEBUG_INFO,"NVM Read Error while updating checksum.\n"));
      return Status;
    }
    Checksum += NvmData[i];
  }
  Checksum = (UINT16)NVM_SUM - Checksum;
  Status = SaveNVMData (Hw, NVM_CHECKSUM_REG, 1, &Checksum);
  if (Status) {
    DEBUG ((DEBUG_INFO,"NVM Write Error while updating checksum.\n"));
  }
  return Status;
}

/**
  This function erases the given bank from the NVM.

  @param[in]    Hw               Pointer to hardware instance.
  @param[in]    Bank             The bank to erase (0 or 1).

  @return       Status           The return status for this function.
  @retval       GBE_FW_UPDATE_SUCCESS
  @retval       GBE_FW_UPDATE_INVALID_PARAMETER
**/
STATIC GBE_FW_UPDATE_STATUS
EraseFlashBank (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT32 Bank
  )
{
  union FLASH_STATUS FSts;
  union FLASH_CTRL FCtrl;
  UINT32 FlashLinearAddr;
  // Bank size is in 16bit Words - adjust to bytes
  UINT32 FlashBankSize = Hw->FlashBankSize * 2;
  GBE_FW_UPDATE_STATUS Status;
  INT32 Count = 0;
  INT32 j, Iteration, SectorSize;

  FSts.RegVal = ReadFlashReg16 (Hw, ICH_FLASH_HSFSTS);
  // Determine HW Sector size: Read BERASE bits of Hw flash Status
  // register
  // 00: The Hw sector is 256 bytes, hence we need to erase 16
  //     consecutive sectors.  The start index for the nth Hw sector
  //     can be calculated as = Bank * 4096 + n * 256
  // 01: The Hw sector is 4K bytes, hence we need to erase 1 sector.
  //     The start index for the nth Hw sector can be calculated
  //     as = Bank * 4096
  // 10: The Hw sector is 8K bytes, nth sector = Bank * 8192
  //     (ich9 only, otherwise error condition)
  // 11: The Hw sector is 64K bytes, nth sector = Bank * 65536

  switch (FSts.HSF_STATUS.berasesz) {
  case 0:
    // Hw sector size 256
    SectorSize = ICH_FLASH_SEG_SIZE_256;
    Iteration = FlashBankSize / ICH_FLASH_SEG_SIZE_256;
    break;
  case 1:
    SectorSize = ICH_FLASH_SEG_SIZE_4K;
    Iteration = 1;
    break;
  case 2:
    SectorSize = ICH_FLASH_SEG_SIZE_8K;
    Iteration = 1;
    break;
  case 3:
    SectorSize = ICH_FLASH_SEG_SIZE_64K;
    Iteration = 1;
    break;
  default:
    return -GBE_FW_UPDATE_INVALID_PARAMETER;
  }
  // Start with the base address, then add the sector Offset.
  FlashLinearAddr = 0;
  FlashLinearAddr += (Bank) ? FlashBankSize : 0;
  for (j = 0; j < Iteration; j++) {
    do {
      UINT32 Timeout = ICH_FLASH_ERASE_COMMAND_TIMEOUT;
      // Steps
      Status = FlashCycleInit(Hw);
      if (Status) {
        return Status;
      }
      // Write a value 11 (block Erase) in Flash.
      // Cycle field in Hw flash control

      FCtrl.RegVal = ReadFlashReg (Hw, ICH_FLASH_HSFSTS) >> 16;
      FCtrl.HSF_CTRL.FCycle = ICH_CYCLE_ERASE;
      WriteFlashReg (Hw, ICH_FLASH_HSFSTS, FCtrl.RegVal << 16);
      // Write the last 24 bits of an index within the
      // block into Flash Linear address field in Flash
      // Address.
      FlashLinearAddr += (j * SectorSize);
      WriteFlashReg (Hw, ICH_FLASH_FADDR,FlashLinearAddr);
      Status = StartFlashCycle (Hw, Timeout);
      if (Status == GBE_FW_UPDATE_SUCCESS) {
        break;
      }
      // Check if FCERR is set to 1.  If 1,
      // clear it and try the whole sequence
      // a few more times else Done

      FSts.RegVal = ReadFlashReg16 (Hw, ICH_FLASH_HSFSTS);
      if (FSts.HSF_STATUS.FCErr) {
        // repeat for some time before giving up
        continue;
      }
      else if (!FSts.HSF_STATUS.FCDone) {
        return Status;
      }
    } while (++Count < ICH_FLASH_CYCLE_REPEAT_COUNT);
  }
  return GBE_FW_UPDATE_SUCCESS;
}

/**
  This function writes 4 bytes to the NVM.

  @param[in]    Hw              Pointer to hardware instance.
  @param[in]    Offset          The Offset (in bytes) of the dWords to read.
  @param[in]    Data            The 4 bytes to write to the NVM.

  @return       Status          The return status for this function.
  @retval       GBE_FW_UPDATE_INVALID_PARAMETER
**/
STATIC GBE_FW_UPDATE_STATUS
WriteFlashData (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT32 Offset,
  IN UINT32 Data
  )
{
  union FLASH_STATUS FSts;
  union FLASH_CTRL FCtrl;
  UINT32 FlashLinearAddr;
  GBE_FW_UPDATE_STATUS Status;
  UINT8 Count = 0;
  if (Offset > ICH_FLASH_LINEAR_ADDR_MASK) {
    return -GBE_FW_UPDATE_INVALID_PARAMETER;
  }
  FlashLinearAddr = ((ICH_FLASH_LINEAR_ADDR_MASK & Offset));
  do {
    gBS->Stall(1);
    // Steps
    Status = FlashCycleInit (Hw);
    if (Status != GBE_FW_UPDATE_SUCCESS) {
      break;
    }
    // In SPT and above, This register is in Lan memory space, not
    // flash.  Therefore, only 32 bit access is supported

    FCtrl.RegVal = ReadFlashReg (Hw, ICH_FLASH_HSFSTS) >> 16;
    FCtrl.HSF_CTRL.FDBCount = sizeof(UINT32) - 1;
    FCtrl.HSF_CTRL.FCycle = ICH_CYCLE_WRITE;
    // In SPT, This register is in Lan memory space,
    // not flash.  Therefore, only 32 bit access is
    // supported

    WriteFlashReg (Hw, ICH_FLASH_HSFSTS, FCtrl.RegVal << 16);
    WriteFlashReg (Hw, ICH_FLASH_FADDR, FlashLinearAddr);
    WriteFlashReg (Hw, ICH_FLASH_FDATA0, Data);
    // check if FCERR is set to 1 , if set to 1, clear it
    // and try the whole sequence a few more times else done
    Status = StartFlashCycle (Hw, ICH_FLASH_WRITE_COMMAND_TIMEOUT);
    if (Status == GBE_FW_UPDATE_SUCCESS) {
      break;
    }
    // If we're here, then things are most likely
    // completely hosed, but if the error condition
    // is detected, it won't hurt to give it another
    // try...ICH_FLASH_CYCLE_REPEAT_COUNT times.
    FSts.RegVal = ReadFlashReg16 (Hw, ICH_FLASH_HSFSTS);
    if (FSts.HSF_STATUS.FCErr) {
      // Repeat for some time before giving up.
      continue;
    }
    if (!FSts.HSF_STATUS.FCDone) {
      DEBUG ((DEBUG_INFO,"Timeout error - flash cycle did not complete.\n"));
      break;
    }
  } while (Count++ < ICH_FLASH_CYCLE_REPEAT_COUNT);
  return Status;
}

/**
  This function writes a dWord to the NVM and
  goes through a retry algorithm before giving up.

  @param[in]    Hw              Pointer to hardware instance.
  @param[in]    Offset           The Offset (in bytes) of the dWords to read.
  @param[in]    DWord           The dWord to write to the NVM.

  @return       Status          The return status for this function.
  @retval       GBE_FW_UPDATE_SUCCESS
  @retval       GBE_FW_UPDATE_INVALID_PARAMETER
**/
STATIC GBE_FW_UPDATE_STATUS
RetryWriteFlashDWord (
  IN struct GBE_FW_UPDATE_HW* Hw,
  IN UINT32 Offset,
  IN UINT32 DWord
)
{
  GBE_FW_UPDATE_STATUS Status;
  UINT16 ProgramRetries;
  // Must convert word Offset into bytes.

  Offset <<= 1;
  Status = WriteFlashData (Hw, Offset, DWord);
  if (!Status) {
    return Status;
  }
  for (ProgramRetries = 0; ProgramRetries < 100; ProgramRetries++) {
    gBS->Stall (100);
    Status = WriteFlashData (Hw, Offset, DWord);
    if (Status == GBE_FW_UPDATE_SUCCESS) {
      break;
    }
  }
  if (ProgramRetries == 100) {
    return -GBE_FW_UPDATE_INVALID_PARAMETER;
  }
  return GBE_FW_UPDATE_SUCCESS;
}

/**
  This function updates the NVM by calling the UpdateNVMChecksum,
  which writes the checksum to the shadow ram.  The changes in the shadow
  ram are then committed to the EEPROM by processing each Bank at a time
  checking for the modified bit and writing only the pending changes.
  After a successful commit, the shadow ram is cleared and is ready for
  future writes.

  @param[in]     Hw         Pointer to hardware instance.

  @return        Status     The return status for this function.
**/
GBE_FW_UPDATE_STATUS
UpdateNVM (
  IN struct GBE_FW_UPDATE_HW* Hw
  )
{
  UINT32 i, NewBankOffset, OldBankOffset, Bank;
  UINT32 ActOffset;
  GBE_FW_UPDATE_STATUS Status;
  UINT32 DWord = 0;

  Status = UpdateNVMChecksum (Hw);
  if (Status) {
    goto out;
  }
  // We're writing to the opposite Bank so if we're on Bank 1,
  // write to Bank 0 etc.  We also need to erase the segment that
  // is going to be written

  Status = DetectValidNVMBank (Hw, &Bank);
  if (Status != GBE_FW_UPDATE_SUCCESS) {
    Bank = 0;
  }
  if (Bank == 0) {
    NewBankOffset = Hw->FlashBankSize;
    OldBankOffset = 0;
    Status = EraseFlashBank (Hw, 1);
    if (Status){
      goto release;
    }
  }
  else {
    OldBankOffset = Hw->FlashBankSize;
    NewBankOffset = 0;
    Status = EraseFlashBank (Hw, 0);
    if (Status) {
      goto release;
    }
  }
  for (i = 0; i < GBE_FW_UPDATE_SHADOW_RAM_WORDS; i += 2) {
    // Determine whether to write the value stored
    // in the other NVM Bank or a modified value stored
    // in the shadow RAM
    Status = ReadFlashData (Hw, i + OldBankOffset, &DWord);
    if (Hw->ShadowRam[i].Modified) {
      DWord &= 0xffff0000;
      DWord |= (Hw->ShadowRam[i].Value & 0xffff);
    }
    if (Hw->ShadowRam[i + 1].Modified) {
      DWord &= 0x0000ffff;
      DWord |= ((Hw->ShadowRam[i + 1].Value & 0xffff) << 16);
    }
    if (Status) {
      break;
    }
    // If the word is 0x13, then make sure the signature bits
    // (15:14) are 11b until the commit has completed.
    // This will allow us to write 10b which indicates the
    // signature is valid.  We want to do this after the write
    // has completed so that we don't mark the segment valid
    // while the write is still in progress

    if (i == GBE_FW_UPDATE_ICH_NVM_SIG_WORD - 1) {
      DWord |= GBE_FW_UPDATE_ICH_NVM_SIG_MASK << 16;
    }
    // Convert Offset to bytes.
    ActOffset = (i + NewBankOffset) << 1;
    gBS->Stall (100);
    // Write the Data to the new Bank. Offset in Words
    ActOffset = i + NewBankOffset;
    Status = RetryWriteFlashDWord (Hw, ActOffset, DWord);
    if (Status) {
      break;
    }
  }
  // Don't bother writing the segment valid bits if sector
  // programming failed.
  if (Status) {
    DEBUG ((DEBUG_INFO,"Flash commit failed.\n"));
    goto release;
  }
  // Finally validate the new segment by setting bit 15:14
  // to 10b in word 0x13 , this can be done without an
  // erase as well since these bits are 11 to start with
  // and we need to change bit 14 to 0b

  ActOffset = NewBankOffset + GBE_FW_UPDATE_ICH_NVM_SIG_WORD;
  // Offset in Words but we read DWord*/
  --ActOffset;
  Status = ReadFlashData (Hw, ActOffset, &DWord);
  if (Status) {
    goto release;
  }
  DWord &= 0xBFFFFFFF;
  Status = RetryWriteFlashDWord (Hw, ActOffset, DWord);
  if (Status) {
    goto release;
  }

  // Offset in Words but we read DWord*/
  ActOffset = OldBankOffset + GBE_FW_UPDATE_ICH_NVM_SIG_WORD - 1;
  Status = ReadFlashData (Hw, ActOffset, &DWord);
  if (Status) {
    goto release;
  }
  DWord &= 0x00FFFFFF;
  Status = RetryWriteFlashDWord (Hw, ActOffset, DWord);
  if (Status) {
    goto release;
  }
  // Great!  Everything worked, we can now clear the cached entries.
  for (i = 0; i < GBE_FW_UPDATE_SHADOW_RAM_WORDS; i++) {
    Hw->ShadowRam[i].Modified = FALSE;
    Hw->ShadowRam[i].Value = 0xFFFF;
  }

release:
  // Reload the EEPROM, or else modifications will not appear
  // until after the next Adapter reset.

  if (!Status) {
    ReloadEeprom (Hw);
    gBS->Stall (10000);
  }

out:
  if (Status) {
    DEBUG ((DEBUG_INFO,"NVM update error: %d\n", Status));
  }
  return Status;
}

/**
  This function causes an EEPROM reload.

  @param[in]       HW               Pointer to hardware instance.

  @return          Status           The return status for this function.
  @retval          GBE_FW_UPDATE_SUCCESS
**/
GBE_FW_UPDATE_STATUS
ReloadEeprom (
  IN  struct GBE_FW_UPDATE_HW* Hw
  )
{
  UINT32  CtrlExt = 0;
  CtrlExt = ReadMacReg (Hw, GBE_FW_UPDATE_CTRL_EXT);
  WriteMacReg (Hw, GBE_FW_UPDATE_CTRL_EXT, CtrlExt | GBE_FW_UPDATE_CTRL_EXT_EE_RST);
  gBS->Stall (100);
  return GBE_FW_UPDATE_SUCCESS;
}

/**
  This function writes the given image to the EEPROM.

  @param[in]    Adapter                  Pointer to the ADAPTER_INFO instance.
  @param[in]    ImageToWrite             Pointer to the image to write.
  @param[in]    ImageToWriteSize         The size of the image to write.

  @return       Status                   The return status for this function.
**/
EFI_STATUS
WriteEepromImage (
  IN     ADAPTER_INFO* Adapter,
  IN     CONST UINT16* ImageToWrite,
  IN     UINTN            ImageToWriteSize,
  IN     BOOLEAN          WriteMacAddress
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT16 Offset = 0;
  UINT16 Data [PSS_CHIP_MAC_ADDRESS_LENGTH / 2];
  UINT16 EepromSizeData [PSS_CHIP_MAC_ADDRESS_LENGTH / 2];

  do {
    // If WriteMacAddress is FALSE, we need to keep the existing MAC address.
    // Which is in offset 0 - 2
    if (WriteMacAddress == FALSE){
      Offset = 3;
    }
    if (ImageToWriteSize > GBE_FW_UPDATE_SHADOW_RAM_WORDS) {
      // If the image to write is larger than the Eeprom target size, then only
      // write up to the Eeprom target size
      ImageToWriteSize = GBE_FW_UPDATE_SHADOW_RAM_WORDS;
    }
    // Read the current value of the Eeprom Size word to see if it needs to be updated
    ReadNVM (&Adapter->Hw,
            0x00,//offset 0 has value 8888, offset 1 has 8888 and offset 2 has 8887
            3,
            EepromSizeData
            );
    //
    // Byte swapping
    //
    DEBUG((DEBUG_INFO, "Read before update (Default MAC in GBE Region) : %x %x %x\n", EepromSizeData[0],  EepromSizeData[1], EepromSizeData[2]));
    gBS->Stall (100);
    for (; Offset < ImageToWriteSize; Offset++) {
      Data[0] = ImageToWrite[Offset];

      // Save NVM Data to the ShadowRam struct
      Status = SaveNVMData (&Adapter->Hw,
                           Offset,
                           1,
                           Data
                           );
      gBS->Stall (100);
      if (Status != GBE_FW_UPDATE_SUCCESS) {
        DEBUG ((DEBUG_INFO,"Failed to save NVM offset 0x%x\n", Offset));
        break;
      }
      if (Status == GBE_FW_UPDATE_SUCCESS) {
        // Write the NVM data to the flash
        Status = UpdateNVM (&Adapter->Hw);
        // Perform a read - on some cards such as 82543
        // where the 1st read after a write fails.

        ReadNVM (&Adapter->Hw, 0x00, 3, Data);
        DEBUG ((DEBUG_INFO, "Read after update in GBE region order: %x %x %x\n",Data[0], Data[1], Data[2]));
      }
    }
  } while (0);
  gBS->Stall (1000000);
  return Status;
}

/**
  This function gets the size of the NVM by reading STRAP register bits 0-4.
  NVM Space size is indicated in multiples of 4KB.
  NVM size may very from 4KB to 128KB while 0 value means 4KB.

  @param[in]    Hw     Pointer to hardware instance.
**/
UINT32
GetNVMSize (
  IN struct GBE_FW_UPDATE_HW* Hw
  )
{
  return 8192;
}