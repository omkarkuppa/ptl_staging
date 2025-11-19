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
#include <Library/BaseMemoryLib.h>
#include <I2cPlatformDxe.h>
#include <PrivateMock.h>

/**
  Mock SerialIoI2cBusFrequencySet.

  @param[in] This           Address of an EFI_I2C_MASTER_PROTOCOL
                            structure
  @param[in] BusClockHertz  New I2C bus clock frequency in Hertz

  @retval EFI_SUCCESS       The bus frequency was set successfully.
  @retval EFI_UNSUPPORTED   The controller does not support this frequency.
**/
EFI_STATUS
EFIAPI
MockSetBusFrequency (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This,
  IN UINTN                         *BusClockHertz
  )
{
  UINTN *Frequency;
  Frequency = mock_ptr_type (UINTN *);
  CopyMem (Frequency, BusClockHertz, sizeof (UINTN));

  return mock_type (EFI_STATUS);
}

/**
  Mock PciIoConfigRead.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory operations.
  @param  Offset                The offset within the PCI configuration space for the PCI controller.
  @param  Count                 The number of PCI configuration operations to perform.
  @param  Buffer                For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.


  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI configuration header of the PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER Buffer is NULL or Width is invalid.

**/
EFI_STATUS
EFIAPI
MockPciIoConfigRead (
  IN     EFI_PCI_IO_PROTOCOL        *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH  Width,
  IN     UINT32                     Offset,
  IN     UINTN                      Count,
  OUT    VOID                       *Buffer
  )
{
  UINT16     PciIoData;
  UINT32     DeviceId;

  check_expected (Offset);
  PciIoData = mock_type (UINT16);
  DeviceId = (UINT32) PciIoData << 16;
  CopyMem (Buffer, &DeviceId, sizeof (UINTN));
  return mock_type (EFI_STATUS);
}
