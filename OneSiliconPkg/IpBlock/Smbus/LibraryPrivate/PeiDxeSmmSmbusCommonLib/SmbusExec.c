/** @file
  PCH Smbus Executive Code (common PEI/DXE/SMM code)

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/SmBus.h>
#include <Library/PciSegmentLib.h>
#include <Library/SmbusLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/SmbusRegs.h>
#include <Library/PchPciBdfLib.h>

/**
  Get SMBUS IO Base address

  @retval UINT32                  The SMBUS IO Base Address
**/
static
UINT16
SmbusGetIoBase (
  VOID
  )
{
  UINT16  SmbusIoBase;

  SmbusIoBase = (UINT16)PciSegmentRead32 (SmbusPciCfgBase () + R_SMBUS_CFG_SBA) & B_SMBUS_CFG_SBA_BA;

  ASSERT (SmbusIoBase != B_SMBUS_CFG_SBA_BA && SmbusIoBase != 0);

  return SmbusIoBase;
}

/**
  This function provides a standard way to read PCH Smbus IO registers.

  @param[in] Offset               Register offset from Smbus base IO address.

  @retval UINT8                   Returns data read from IO.
**/
UINT8
SmbusIoRead (
  IN      UINT8           Offset
  )
{
  return IoRead8 (SmbusGetIoBase () + Offset);
}

/**
  This function provides a standard way to write PCH Smbus IO registers.

  @param[in] Offset               Register offset from Smbus base IO address.
  @param[in] Data                 Data to write to register.

**/
VOID
SmbusIoWrite (
  IN      UINT8           Offset,
  IN      UINT8           Data
  )
{
  //
  // Write New Value
  //
  IoWrite8 (SmbusGetIoBase () + Offset, Data);
  return;
}

/**
  This function provides a standard way to execute Smbus protocols
  as defined in the SMBus Specification. The data can either be of
  the Length byte, word, or a block of data. The resulting transaction will be
  either the SMBus Device accepts this transaction or this function
  returns with an error

  @param[in] DeviceAddress        Smbus device the command is directed at
  @param[in] Command              Smbus Device dependent
  @param[in] Operation            Which SMBus protocol will be used
  @param[in] PecCheck             Defines if Packet Error Code Checking is to be used
  @param[in, out] Length          How many bytes to read. Must be 0 <= Length <= 32 depending on Operation
                                  It will contain the actual number of bytes read/written.
  @param[in, out] Buffer          Contain the data read/written.

  @retval EFI_SUCCESS             The operation completed successfully.
  @exception EFI_UNSUPPORTED      The operation is unsupported.

  @retval EFI_INVALID_PARAMETER   Length or Buffer is NULL for any operation besides
                                  quick read or quick write.
  @retval EFI_TIMEOUT             The transaction did not complete within an internally
                                  specified timeout period, or the controller is not
                                  available for use.
  @retval EFI_DEVICE_ERROR        There was an Smbus error (NACK) during the operation.
                                  This could indicate the SMBus device is not present
                                  or is in a hung condition.
**/
EFI_STATUS
SmbusExec (
  IN      EFI_SMBUS_DEVICE_ADDRESS  DeviceAddress,
  IN      EFI_SMBUS_DEVICE_COMMAND  Command,
  IN      EFI_SMBUS_OPERATION       Operation,
  IN      BOOLEAN                   PecCheck,
  IN OUT  UINTN                     *Length,
  IN OUT  VOID                      *Buffer
  )
{
  RETURN_STATUS ReturnStatus;
  UINT8         *ByteBuffer;
  UINT16        *WordBuffer;

  ReturnStatus = RETURN_SUCCESS;
  ByteBuffer   = (UINT8 *)Buffer;
  WordBuffer   = (UINT16 *)Buffer;

  //
  // For any operations besides quick read & write, the pointers to
  // Length and Buffer must not be NULL.
  //
  if ((Operation != EfiSmbusQuickRead) && (Operation != EfiSmbusQuickWrite)) {
    if ((Length == NULL) || (Buffer == NULL)) {
      return EFI_INVALID_PARAMETER;
    }
  }

  switch (Operation) {
    case EfiSmbusQuickRead:
      SmBusQuickRead (
        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
        &ReturnStatus
        );
      break;
    case EfiSmbusQuickWrite:
      SmBusQuickWrite (
        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
        &ReturnStatus
        );
      break;
    case EfiSmbusReceiveByte:
      if (*Length < 1) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 1;
      ByteBuffer[0] = SmBusReceiveByte (
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        &ReturnStatus
                        );
      break;
    case EfiSmbusSendByte:
      if (*Length < 1) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 1;
      ByteBuffer[0] = SmBusSendByte (
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        ByteBuffer[0],
                        &ReturnStatus
                        );
      break;
    case EfiSmbusReadByte:
      if (*Length < 1) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 1;
      ByteBuffer[0] = SmBusReadDataByte (
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        &ReturnStatus
                        );
      break;
    case EfiSmbusWriteByte:
      if (*Length < 1) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 1;
      ByteBuffer[0] = SmBusWriteDataByte (
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        ByteBuffer[0],
                        &ReturnStatus
                        );
      break;
    case EfiSmbusReadWord:
      if (*Length < 2) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 2;
      WordBuffer[0] = SmBusReadDataWord (
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        &ReturnStatus
                        );
      break;
    case EfiSmbusWriteWord:
      if (*Length < 2) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 2;
      WordBuffer[0] = SmBusWriteDataWord (
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        WordBuffer[0],
                        &ReturnStatus
                        );
      break;
    case EfiSmbusReadBlock:
      *Length = SmBusReadBlock (
                  SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                  Buffer,
                  &ReturnStatus
                  );
      break;
    case EfiSmbusWriteBlock:
      *Length = SmBusWriteBlock (
                  SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, *Length, PecCheck),
                  Buffer,
                  &ReturnStatus
                  );
      break;
    case EfiSmbusProcessCall:
      if (*Length < 2) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 2;
      WordBuffer[0] = SmBusProcessCall (
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        WordBuffer[0],
                        &ReturnStatus
                        );
      break;
    case EfiSmbusBWBRProcessCall:
      *Length = SmBusBlockProcessCall (
                  SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, *Length, PecCheck),
                  Buffer,
                  Buffer,
                  &ReturnStatus
                  );
      break;
    default:
      return EFI_INVALID_PARAMETER;
      break;
  }

  return (EFI_STATUS) ReturnStatus;
}

/**
  This function initializes the Smbus Registers.

**/
VOID
InitializeSmbusRegisters (
  VOID
  )
{
  UINT64 SmbusRegBase;

  SmbusRegBase = SmbusPciCfgBase ();
  //
  // Enable the Smbus I/O Enable
  //
  PciSegmentOr8 (SmbusRegBase + PCI_COMMAND_OFFSET, (UINT8) EFI_PCI_COMMAND_IO_SPACE);

  //
  // Enable the Smbus host controller
  //
  PciSegmentAndThenOr8 (
    SmbusRegBase + R_SMBUS_CFG_HCFG,
    (UINT8) (~(B_SMBUS_CFG_HCFG_SSEN | B_SMBUS_CFG_HCFG_I2CEN)),
    B_SMBUS_CFG_HCFG_HSTEN
    );

  SmbusIoWrite (R_SMBUS_IO_HSTS, V_SMBUS_IO_HSTS_ALL);
}