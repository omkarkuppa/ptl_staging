/** @file
  SMBUS Library that layers on top of the SMBUS SMM Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

**/

#include "InternalSmbusLib.h"

//
// Global variable to cache pointer to Smbus protocol.
//
EFI_SMBUS_HC_PROTOCOL  *mSmbus = NULL;

/**
  The constructor function caches the pointer to Smbus protocol.

  The constructor function locates Smbus protocol from protocol database.
  It will ASSERT() if that operation fails and it will always return EFI_SUCCESS.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SmbusLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gSmst->SmmLocateProtocol (&gEfiSmmSmbusProtocolGuid, NULL, (VOID **)&mSmbus);
  ASSERT_EFI_ERROR (Status);
  ASSERT (mSmbus != NULL);

  return Status;
}

/**
  Executes an SMBus operation to an SMBus controller.

  This function provides a standard way to execute Smbus script
  as defined in the SmBus Specification. The data can either be of
  the Length byte, word, or a block of data.

  @param  SmbusOperation  Signifies which particular SMBus hardware protocol instance
                          that it will use to execute the SMBus transactions.
  @param  SmBusAddress    The address that encodes the SMBUS Slave Address,
                          SMBUS Command, SMBUS Data Length, and PEC.
  @param  Length          Signifies the number of bytes that this operation will do.
                          The maximum number of bytes can be revision specific
                          and operation specific.
  @param  Buffer          Contains the value of data to execute to the SMBus slave
                          device. Not all operations require this argument. The
                          length of this buffer is identified by Length.
  @param  Status          Return status for the executed command.
                          This is an optional parameter and may be NULL.

  @return The actual number of bytes that are executed for this operation.

**/
UINTN
InternalSmBusExec (
  IN     EFI_SMBUS_OPERATION  SmbusOperation,
  IN     UINTN                SmBusAddress,
  IN     UINTN                Length,
  IN OUT VOID                 *Buffer,
  OUT RETURN_STATUS           *Status        OPTIONAL
  )
{
  RETURN_STATUS             ReturnStatus;
  EFI_SMBUS_DEVICE_ADDRESS  SmbusDeviceAddress;

  SmbusDeviceAddress.SmbusDeviceAddress = SMBUS_LIB_SLAVE_ADDRESS (SmBusAddress);

  ReturnStatus = mSmbus->Execute (
                           mSmbus,
                           SmbusDeviceAddress,
                           SMBUS_LIB_COMMAND (SmBusAddress),
                           SmbusOperation,
                           SMBUS_LIB_PEC (SmBusAddress),
                           &Length,
                           Buffer
                           );
  if (Status != NULL) {
    *Status = ReturnStatus;
  }

  return Length;
}
