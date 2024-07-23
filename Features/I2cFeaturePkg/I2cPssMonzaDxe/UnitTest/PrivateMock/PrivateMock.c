/**@file

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Uefi.h>
#include <PrivateMock.h>

/**
  Mock of I2cBusQueueRequest.

  @param[in] This               Pointer to an EFI_I2C_IO_PROTOCOL structure.
  @param[in] SlaveAddressIndex  Index value into an array of slave addresses
                                for the I2C device.  The values in the array
                                are specified by the board designer, with the
                                third party I2C device driver writer providing
                                the slave address order.

                                For devices that have a single slave address,
                                this value must be zero.  If the I2C device
                                uses more than one slave address then the
                                third party (upper level) I2C driver writer
                                needs to specify the order of entries in the
                                slave address array.

                                \ref ThirdPartyI2cDrivers "Third Party I2C
                                Drivers" section in I2cMaster.h.
  @param[in] Event              Event to signal for asynchronous transactions,
                                NULL for synchronous transactions
  @param[in] RequestPacket      Pointer to an EFI_I2C_REQUEST_PACKET structure
                                describing the I2C transaction
  @param[out] I2cStatus         Optional buffer to receive the I2C transaction
                                completion status

  @retval EFI_SUCCESS           The asynchronous transaction was successfully
                                queued when Event is not NULL.
  @retval EFI_SUCCESS           The transaction completed successfully when
                                Event is NULL.
  @retval EFI_BAD_BUFFER_SIZE   The RequestPacket->LengthInBytes value is too
                                large.
  @retval EFI_DEVICE_ERROR      There was an I2C error (NACK) during the
                                transaction.
  @retval EFI_INVALID_PARAMETER RequestPacket is NULL
  @retval EFI_NO_MAPPING        The EFI_I2C_HOST_PROTOCOL could not set the
                                bus configuration required to access this I2C
                                device.
  @retval EFI_NO_RESPONSE       The I2C device is not responding to the slave
                                address selected by SlaveAddressIndex.
                                EFI_DEVICE_ERROR will be returned if the
                                controller cannot distinguish when the NACK
                                occurred.
  @retval EFI_OUT_OF_RESOURCES  Insufficient memory for I2C transaction
  @retval EFI_UNSUPPORTED       The controller does not support the requested
                                transaction.

**/
EFI_STATUS
EFIAPI
MockI2cBusQueueRequest (
  IN CONST EFI_I2C_IO_PROTOCOL  *This,
  IN UINTN                      SlaveAddressIndex,
  IN EFI_EVENT                  Event               OPTIONAL,
  IN EFI_I2C_REQUEST_PACKET     *RequestPacket,
  OUT EFI_STATUS                *I2cStatus          OPTIONAL
  )
{
  UINT8                    BufferSize;
  UINT8                    Index;

  BufferSize = mock_type (UINT8);
  for (Index = 0; Index < BufferSize; Index++) {
    RequestPacket->Operation[1].Buffer[Index] = mock_type (UINT8);
  }
  return mock_type (EFI_STATUS);
}
/**
  Mock of FreeUnicodeStringTable.

  If UnicodeStringTable is NULL, then EFI_SUCCESS is returned.
  Otherwise, each language code, and each Unicode string in the Unicode string
  table are freed, and EFI_SUCCESS is returned.

  @param  UnicodeStringTable  A pointer to the table of Unicode strings.

  @retval EFI_SUCCESS         The Unicode string table was freed.

**/
EFI_STATUS
EFIAPI
FreeUnicodeStringTable (
  IN EFI_UNICODE_STRING_TABLE  *UnicodeStringTable
  )
{
  return mock_type (EFI_STATUS);
}