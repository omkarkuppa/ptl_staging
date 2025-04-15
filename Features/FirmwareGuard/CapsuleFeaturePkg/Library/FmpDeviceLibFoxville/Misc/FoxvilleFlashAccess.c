/** @file
  Internal file to access Foxville flash.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include "Protocol/PciIo.h"
#include "Library/DebugLib.h"
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleRegs.h"
#include "Misc/FoxvilleFlashAccess.h"
#include "Misc/FoxvilleFwSts.h"
#include "Misc/FoxvilleRegAccess.h"

/**
  Set the number of bytes into the flash burst counter.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  NumOfByte         Indicate number of bytes to set into the counter.

  @retval  EFI_SUCCESS            Succeed to set the flash burst counter.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set the flash burst counter.

**/
EFI_STATUS
EFIAPI
SetFlashBurstCounter (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               NumOfByte
  )
{
  EFI_STATUS                 Status;
  FOXVILLE_FLSWCNT_REGISTER  Request;

  Request.Uint32 = NumOfByte;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteFlashSwCnt (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Polling to check the flash is free.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Device is in free state.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Device is in busy state.

**/
EFI_STATUS
EFIAPI
PollFlashFree (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PollFlashSwCtrlDoneAndFlashBusy (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Set the flash sector erase command via FLSWCTL register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Address used to perform the sector erase.

  @retval  EFI_SUCCESS            Succeed to erase the sector in assigned address.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to erase the sector in assigned address.

**/
EFI_STATUS
EFIAPI
SetFlashSwCtlSectorEraseCmd (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Address
  )
{
  EFI_STATUS                 Status;
  FOXVILLE_FLSWCTL_REGISTER  Request;
  FOXVILLE_FLSWCTL_REGISTER  Response;

  Request.Uint32  = 0;
  Response.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PollFlashSwCtrlDone (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Request.Bits.Cmd     = V_FLSWCTL_CMD_FLASH_SECTOR_4K_ERASE;
  Request.Bits.Address = Address;

  Status = WriteFlashSwCtrl (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = ReadFlashSwCtrl (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.CmdValid != V_FLSWCTL_CMD_VALID_SET) {
    return EFI_DEVICE_ERROR;
  }

  Status = PollFlashSwCtrlDoneAndFlashBusy (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Set the flash write command with assigned address via FLSWCTL register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Address used to perform the write.

  @retval  EFI_SUCCESS            Succeed to set the address to be written.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set the address to be written.

**/
EFI_STATUS
EFIAPI
SetFlashSwCtlWriteCmd (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Address
  )
{
  EFI_STATUS                 Status;
  FOXVILLE_FLSWCTL_REGISTER  Request;
  FOXVILLE_FLSWCTL_REGISTER  Response;

  Request.Uint32  = 0;
  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (Address > V_FLSWCTL_ADDRESS_MAX)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PollFlashSwCtrlDone (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Request.Bits.Cmd     = V_FLSWCTL_CMD_WRITE;
  Request.Bits.Address = Address;

  Status = WriteFlashSwCtrl (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = ReadFlashSwCtrl (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.CmdValid != V_FLSWCTL_CMD_VALID_SET) {
    return EFI_DEVICE_ERROR;
  }

  Status = PollFlashSwCtrlDoneAndFlashBusy (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Set the flash data by sequence via FLSWDATA register.

  @note It is caller responsibility to provide the bursted buffer
        aligned with 4 bytes.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  BufferPtr         Pointer to the buffer to be used.
  @param[in]  BufferSize        Size of the buffer to be used.

  @retval  EFI_SUCCESS            Succeed to set flash data by sequence.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set flash data by sequence.

**/
EFI_STATUS
EFIAPI
SetFlashSwDataInBurst (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN VOID                 *BufferPtr,
  IN UINTN                BufferSize
  )
{
  EFI_STATUS                  Status;
  FOXVILLE_FLSWDATA_REGISTER  Request;
  EFI_PHYSICAL_ADDRESS        DataAddress;
  UINTN                       Offset;

  Request.Uint32 = 0;
  DataAddress    = 0;
  Offset         = 0;

  if ((PciIoProtocolPtr == NULL) || (BufferPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((BufferSize == 0) || ((BufferSize % sizeof (UINT32)) != 0)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Set the base address for the data.
  //
  DataAddress = (EFI_PHYSICAL_ADDRESS)BufferPtr;

  //
  // Looping to write the register in a multiple of 4 bytes.
  //
  for (Offset = 0; Offset < BufferSize; Offset += sizeof (UINT32)) {
    Request.Uint32 = *((UINT32 *)DataAddress);

    Status = WriteFlashSwData (PciIoProtocolPtr, Request.Uint32);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to write @ [0x%08X]\n", DataAddress));
      return EFI_DEVICE_ERROR;
    }

    Status = PollFlashSwCtrlDoneAndFlashBusy (PciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    //
    // Move to next data location.
    //
    DataAddress += sizeof (UINT32);
  }

  return EFI_SUCCESS;
}

/**
  Commit the EEPROM content into flash via EEC register.

  @note This function should be used when the FW is active.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to commit the EEPROM content.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to commit the EEPROM content.

**/
EFI_STATUS
EFIAPI
CommitEepromToFlashEec (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "Commit the EEPROM into flash via EEC register.\n"));

  Status = PollEecFlashUpdateDone (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Previous Flash update is timeout.\n"));
    return EFI_DEVICE_ERROR;
  }

  Status = SetEecFlashUpdate (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set the flash update bit in EEC register.\n"));
    return EFI_DEVICE_ERROR;
  }

  Status = PollEecFlashUpdateDone (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Flash update is timeout.\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Succeed to commit the EEPROM into flash via EEC register.\n"));

  return EFI_SUCCESS;
}
