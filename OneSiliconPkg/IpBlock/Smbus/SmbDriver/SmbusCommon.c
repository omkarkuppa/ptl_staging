/** @file
  Common SMBUS functions between PEI and DXE

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

#include "SmbusCommon.h"

#define SMBUS_NUM_RESERVED          38      ///< Number of device addresses that are reserved by the SMBus spec.
#define SMBUS_ADDRESS_ARP           0xC2 >> 1
#define SMBUS_DATA_PREPARE_TO_ARP   0x01
#define SMBUS_DATA_RESET_DEVICE     0x02
#define SMBUS_DATA_GET_UDID_GENERAL 0x03
#define SMBUS_DATA_ASSIGN_ADDRESS   0x04
#define SMBUS_GET_UDID_LENGTH       17      ///< 16 byte UDID + 1 byte address

#define SMBUS_DEFAULT_HOST_TIMING_VALUE 0x0A0A0000

///
/// These addresses are reserved by the SMBus 2.0 specification
///
static UINT8  mReservedAddress[SMBUS_NUM_RESERVED] = {
  0x00,
  0x02,
  0x04,
  0x06,
  0x08,
  0x0A,
  0x0C,
  0x0E,
  0x10,
  0x18,
  0x50,
  0x6E,
  0xC2,
  0xF0,
  0xF2,
  0xF4,
  0xF6,
  0xF8,
  0xFA,
  0xFC,
  0xFE,
  0x12,
  0x14,
  0x16,
  0x58,
  0x5A,
  0x80,
  0x82,
  0x84,
  0x86,
  0x88,
  0x90,
  0x92,
  0x94,
  0x96,
  0x1A,
  0x1C,
  0x1E
};

/**
  This function provides a standard way to read PCH Smbus IO registers.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in] Offset               Register offset from Smbus base IO address.

  @retval UINT8                   Returns data read from IO.
**/
UINT8
SmbusIoRead (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8               Offset
  )
{
  UINT8       Data;
  EFI_STATUS  Status;

  Status = PciIo->Io.Read (
            PciIo,
            EfiPciIoWidthUint8,
            4,
            Offset,
            1,
            &Data
            );
  if (EFI_ERROR (Status)) {
    return 0xFF;
  }

  return Data;
}

/**
  This function provides a standard way to write PCH Smbus IO registers.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in] Offset               Register offset from Smbus base IO address.
  @param[in] Data                 Data to write to register.

**/
VOID
SmbusIoWrite (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN      UINT8           Offset,
  IN      UINT8           Data
  )
{
  PciIo->Io.Write (
              PciIo,
              EfiPciIoWidthUint8,
              4,
              Offset,
              1,
              &Data
  );
}

/**
  Acquires the ownership of SMBUS.

  This internal function reads the host state register.
  If the SMBUS is not available, RETURN_TIMEOUT is returned;
  Otherwise, it performs some basic initializations and returns
  RETURN_SUCCESS.

  @param[in] PciIo                PCI_IO for SMB controller

  @retval     RETURN_SUCCESS    The SMBUS command was executed successfully.
  @retval     RETURN_TIMEOUT    A timeout occurred while executing the SMBUS command.

**/
RETURN_STATUS
InternalSmbusAcquire (
  IN EFI_PCI_IO_PROTOCOL  *PciIo
  )
{
  UINT8   HostStatus;

  HostStatus = SmbusIoRead (PciIo, R_SMBUS_IO_HSTS);
  if ((HostStatus & B_SMBUS_IO_HSTS_HBSY) != 0) {
    //
    // Clear host status register and exit.
    //
    SmbusIoWrite (PciIo, R_SMBUS_IO_HSTS, V_SMBUS_IO_HSTS_ALL);
    return RETURN_TIMEOUT;
  }
  //
  // Clear out any odd status information.
  //
  SmbusIoWrite (PciIo, R_SMBUS_IO_HSTS, HostStatus);

  return RETURN_SUCCESS;
}

/**
  Starts the SMBUS transaction and waits until the end.

  This internal function start the SMBUS transaction and waits until the transaction
  of SMBUS is over by polling the INTR bit of Host status register.
  If the SMBUS is not available, RETURN_TIMEOUT is returned;
  Otherwise, it performs some basic initializations and returns
  RETURN_SUCCESS.

  @param[in]  PciIo               PCI_IO for SMB controller
  @param[in]  HostControl         The Host control command to start SMBUS transaction.

  @retval     RETURN_SUCCESS      The SMBUS command was executed successfully.
  @retval     RETURN_CRC_ERROR    The checksum is not correct (PEC is incorrect).
  @retval     RETURN_DEVICE_ERROR The request was not completed because a failure reflected
                                  in the Host Status Register bit.  Device errors are
                                  a result of a transaction collision, illegal command field,
                                  unclaimed cycle (host initiated), or bus errors (collisions).

**/
RETURN_STATUS
InternalSmbusStart (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINT8              HostControl
  )
{
  UINT8   HostStatus;
  UINT8   AuxiliaryStatus;
  UINT32  Timeout;

  //
  // Set Host Control Register (Initiate Operation, Interrupt disabled).
  //
  SmbusIoWrite (PciIo, R_SMBUS_IO_HCTL, (UINT8)(HostControl | B_SMBUS_IO_HCTL_START));

  Timeout = 100;
  do {
    //
    // Poll INTR bit of Host Status Register.
    //
    HostStatus = SmbusIoRead (PciIo, R_SMBUS_IO_HSTS);
    MicroSecondDelay (1);
    Timeout--;
  } while (((HostStatus & (B_SMBUS_IO_HSTS_INTR | B_SMBUS_IO_HSTS_DERR | B_SMBUS_IO_HSTS_BERR | B_SMBUS_IO_HSTS_FAIL | B_SMBUS_IO_HSTS_BDS)) == 0) && Timeout != 0);

  if ((HostStatus & (B_SMBUS_IO_HSTS_DERR | B_SMBUS_IO_HSTS_BERR | B_SMBUS_IO_HSTS_FAIL)) == 0) {
    return RETURN_SUCCESS;
  }
  //
  // Clear error bits of Host Status Register.
  //
  SmbusIoWrite (PciIo, R_SMBUS_IO_HSTS, B_SMBUS_IO_HSTS_DERR | B_SMBUS_IO_HSTS_BERR | B_SMBUS_IO_HSTS_FAIL);
  //
  // Read Auxiliary Status Register to judge CRC error.
  //
  AuxiliaryStatus = SmbusIoRead (PciIo, R_SMBUS_IO_AUXS);
  if ((AuxiliaryStatus & B_SMBUS_IO_AUXS_CRCE) != 0) {
    return RETURN_CRC_ERROR;
  }

  return RETURN_DEVICE_ERROR;
}

/**
  Executes an SMBUS quick, byte or word command.

  This internal function executes an SMBUS quick, byte or word commond.
  If Status is not NULL, then the status of the executed command is returned in Status.

  @param[in] PciIo            PCI_IO for SMB controller
  @param[in]  HostControl     The value of Host Control Register to set.
  @param[in]  DeviceAddress   Address that encodes the SMBUS Device Address,
                              SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  Value           The byte/word write to the SMBUS.
  @param[out] Status          Return status for the executed command.
                              This is an optional parameter and may be NULL.

  @retval The byte/word read from the SMBUS.

**/
UINT16
InternalSmbusNonBlock (
  IN EFI_PCI_IO_PROTOCOL        *PciIo,
  IN  UINT8                     HostControl,
  IN  UINTN                     SmbusAddress,
  IN  UINT16                    Value,
  OUT RETURN_STATUS             *Status
  )
{
  RETURN_STATUS                 ReturnStatus;
  UINT8                         AuxiliaryControl;

  //
  // Try to acquire the ownership of SMBUS.
  //
  ReturnStatus = InternalSmbusAcquire (PciIo);
  if (RETURN_ERROR (ReturnStatus)) {
    goto Done;
  }
  //
  // Set the appropriate Host Control Register and auxiliary Control Register.
  //
  AuxiliaryControl = 0;
  if (SMBUS_LIB_PEC (SmbusAddress)) {
    AuxiliaryControl |= B_SMBUS_IO_AUXC_AAC;
  }

  //
  // We do not need Data Register for SendByte Command
  //
  if ((HostControl == V_SMBUS_IO_SMB_CMD_BYTE) && ((SmbusAddress & BIT0) == V_SMBUS_IO_WRITE)) {
    //
    // Set Host Commond Register.
    //
    SmbusIoWrite (PciIo, R_SMBUS_IO_HCMD, (UINT8)Value);
  } else {
    //
    // Set Host Commond Register.
    //
    SmbusIoWrite (PciIo, R_SMBUS_IO_HCMD, (UINT8)SMBUS_LIB_COMMAND (SmbusAddress));
    //
    // Write value to Host Data 0 and Host Data 1 Registers.
    //
    SmbusIoWrite (PciIo, R_SMBUS_IO_HD0, (UINT8)Value);
    SmbusIoWrite (PciIo, R_SMBUS_IO_HD1, (UINT8)(Value >> 8));
  }
  //
  // Set Auxiliary Control Regiester.
  //
  SmbusIoWrite (PciIo, R_SMBUS_IO_AUXC, AuxiliaryControl);
  //
  // Set SMBUS Device address for the device to send/receive from.
  //
  SmbusIoWrite (PciIo, R_SMBUS_IO_TSA, (UINT8) SmbusAddress);
  //
  // Start the SMBUS transaction and wait for the end.
  //
  ReturnStatus = InternalSmbusStart (PciIo, HostControl);
  //
  // Read value from Host Data 0 and Host Data 1 Registers.
  //
  Value = (UINT16)(SmbusIoRead (PciIo, R_SMBUS_IO_HD1) << 8);
  Value = (UINT16)(Value | SmbusIoRead (PciIo, R_SMBUS_IO_HD0));
  //
  // Clear Host Status Register and Auxiliary Status Register.
  //
  SmbusIoWrite (PciIo, R_SMBUS_IO_HSTS, V_SMBUS_IO_HSTS_ALL);
  SmbusIoWrite (PciIo, R_SMBUS_IO_AUXS, B_SMBUS_IO_AUXS_CRCE);

Done:
  if (Status != NULL) {
    *Status = ReturnStatus;
  }

  return Value;
}

/**
  Executes an SMBUS quick read command.

  Executes an SMBUS quick read command on the SMBUS device specified by SmbusAddress.
  Only the SMBUS Device address field of SmbusAddress is required.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If PEC is set in SmbusAddress, then ASSERT().
  If Command in SmbusAddress is not zero, then ASSERT().
  If Length in SmbusAddress is not zero, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

**/
VOID
SmbusQuickRead (
  IN EFI_PCI_IO_PROTOCOL        *PciIo,
  IN  UINTN                     SmbusAddress,
  OUT RETURN_STATUS             *Status       OPTIONAL
  )
{
  if (SMBUS_LIB_PEC (SmbusAddress)           ||
    (SMBUS_LIB_COMMAND (SmbusAddress)  != 0) ||
    (SMBUS_LIB_LENGTH (SmbusAddress)   != 0) ||
    (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return;
  }

  InternalSmbusNonBlock (
    PciIo,
    V_SMBUS_IO_SMB_CMD_QUICK,
    SmbusAddress | V_SMBUS_IO_READ,
    0,
    Status
    );
}

/**
  Executes an SMBUS quick write command.

  Executes an SMBUS quick write command on the SMBUS device specified by SmbusAddress.
  Only the SMBUS Device address field of SmbusAddress is required.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If PEC is set in SmbusAddress, then ASSERT().
  If Command in SmbusAddress is not zero, then ASSERT().
  If Length in SmbusAddress is not zero, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in] PciIo          PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

**/
VOID
SmbusQuickWrite (
  IN EFI_PCI_IO_PROTOCOL        *PciIo,
  IN  UINTN                     SmbusAddress,
  OUT RETURN_STATUS             *Status       OPTIONAL
  )
{
  if (SMBUS_LIB_PEC (SmbusAddress)           ||
    (SMBUS_LIB_COMMAND (SmbusAddress)  != 0) ||
    (SMBUS_LIB_LENGTH (SmbusAddress)   != 0) ||
    (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return;
  }

  InternalSmbusNonBlock (
    PciIo,
    V_SMBUS_IO_SMB_CMD_QUICK,
    SmbusAddress | V_SMBUS_IO_WRITE,
    0,
    Status
    );
}

/**
  Executes an SMBUS receive byte command.

  Executes an SMBUS receive byte command on the SMBUS device specified by SmbusAddress.
  Only the SMBUS Device address field of SmbusAddress is required.
  The byte received from the SMBUS is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Command in SmbusAddress is not zero, then ASSERT().
  If Length in SmbusAddress is not zero, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The byte received from the SMBUS.

**/
UINT8
SmbusReceiveByte (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINTN               SmbusAddress,
  OUT RETURN_STATUS       *Status        OPTIONAL
  )
{
  if ((SMBUS_LIB_COMMAND (SmbusAddress)  != 0) ||
      (SMBUS_LIB_LENGTH (SmbusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return (UINT8) InternalSmbusNonBlock (
                   PciIo,
                   V_SMBUS_IO_SMB_CMD_BYTE,
                   SmbusAddress | V_SMBUS_IO_READ,
                   0,
                   Status
                   );
}

/**
  Executes an SMBUS send byte command.

  Executes an SMBUS send byte command on the SMBUS device specified by SmbusAddress.
  The byte specified by Value is sent.
  Only the SMBUS Device address field of SmbusAddress is required.  Value is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Command in SmbusAddress is not zero, then ASSERT().
  If Length in SmbusAddress is not zero, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  Value         The 8-bit value to send.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The parameter of Value.

**/
UINT8
SmbusSendByte (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINTN               SmbusAddress,
  IN  UINT8               Value,
  OUT RETURN_STATUS       *Status        OPTIONAL
  )
{
  if ((SMBUS_LIB_COMMAND (SmbusAddress)  != 0) ||
      (SMBUS_LIB_LENGTH (SmbusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return (UINT8) InternalSmbusNonBlock (
                   PciIo,
                   V_SMBUS_IO_SMB_CMD_BYTE,
                   SmbusAddress | V_SMBUS_IO_WRITE,
                   Value,
                   Status
                   );
}

/**
  Executes an SMBUS read data byte command.

  Executes an SMBUS read data byte command on the SMBUS device specified by SmbusAddress.
  Only the SMBUS Device address and SMBUS command fields of SmbusAddress are required.
  The 8-bit value read from the SMBUS is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmbusAddress is not zero, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo           PCI_IO for SMB controller
  @param[in]  SmbusAddress    Address that encodes the SMBUS Device Address,
                              SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Status          Return status for the executed command.
                              This is an optional parameter and may be NULL.

  @retval The byte read from the SMBUS.

**/
UINT8
SmbusReadDataByte (
  IN  EFI_PCI_IO_PROTOCOL *PciIo,
  IN  UINTN               SmbusAddress,
  OUT RETURN_STATUS       *Status        OPTIONAL
  )
{
  if ((SMBUS_LIB_LENGTH (SmbusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return (UINT8) InternalSmbusNonBlock (
                   PciIo,
                   V_SMBUS_IO_SMB_CMD_BYTE_DATA,
                   SmbusAddress | V_SMBUS_IO_READ,
                   0,
                   Status
                   );
}

/**
  Executes an SMBUS write data byte command.

  Executes an SMBUS write data byte command on the SMBUS device specified by SmbusAddress.
  The 8-bit value specified by Value is written.
  Only the SMBUS Device address and SMBUS command fields of SmbusAddress are required.
  Value is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmbusAddress is not zero, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  Value         The 8-bit value to write.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The parameter of Value.

**/
UINT8
SmbusWriteDataByte (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINTN               SmbusAddress,
  IN  UINT8               Value,
  OUT RETURN_STATUS       *Status        OPTIONAL
  )
{
  if ((SMBUS_LIB_LENGTH (SmbusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return (UINT8) InternalSmbusNonBlock (
                   PciIo,
                   V_SMBUS_IO_SMB_CMD_BYTE_DATA,
                   SmbusAddress | V_SMBUS_IO_WRITE,
                   Value,
                   Status
                   );
}

/**
  Executes an SMBUS read data word command.

  Executes an SMBUS read data word command on the SMBUS device specified by SmbusAddress.
  Only the SMBUS Device address and SMBUS command fields of SmbusAddress are required.
  The 16-bit value read from the SMBUS is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmbusAddress is not zero, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The byte read from the SMBUS.

**/
UINT16
SmbusReadDataWord (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINTN               SmbusAddress,
  OUT RETURN_STATUS       *Status        OPTIONAL
  )
{
  if ((SMBUS_LIB_LENGTH (SmbusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmbusNonBlock (
           PciIo,
           V_SMBUS_IO_SMB_CMD_WORD_DATA,
           SmbusAddress | V_SMBUS_IO_READ,
           0,
           Status
           );
}

/**
  Executes an SMBUS write data word command.

  Executes an SMBUS write data word command on the SMBUS device specified by SmbusAddress.
  The 16-bit value specified by Value is written.
  Only the SMBUS Device address and SMBUS command fields of SmbusAddress are required.
  Value is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmbusAddress is not zero, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  Value         The 16-bit value to write.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The parameter of Value.

**/
UINT16
SmbusWriteDataWord (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINTN               SmbusAddress,
  IN  UINT16              Value,
  OUT RETURN_STATUS       *Status        OPTIONAL
  )
{
  if ((SMBUS_LIB_LENGTH (SmbusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmbusNonBlock (
           PciIo,
           V_SMBUS_IO_SMB_CMD_WORD_DATA,
           SmbusAddress | V_SMBUS_IO_WRITE,
           Value,
           Status
           );
}

/**
  Executes an SMBUS process call command.

  Executes an SMBUS process call command on the SMBUS device specified by SmbusAddress.
  The 16-bit value specified by Value is written.
  Only the SMBUS Device address and SMBUS command fields of SmbusAddress are required.
  The 16-bit value returned by the process call command is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmbusAddress is not zero, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  Value         The 16-bit value to write.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The 16-bit value returned by the process call command.

**/
UINT16
SmbusProcessCall (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINTN               SmbusAddress,
  IN  UINT16              Value,
  OUT RETURN_STATUS       *Status        OPTIONAL
  )
{
  if ((SMBUS_LIB_LENGTH (SmbusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmbusNonBlock (
           PciIo,
           V_SMBUS_IO_SMB_CMD_PROCESS_CALL,
           SmbusAddress | V_SMBUS_IO_WRITE,
           Value,
           Status
           );
}

/**
  Executes an SMBUS block command.

  Executes an SMBUS block read, block write and block write-block read command
  on the SMBUS device specified by SmbusAddress.
  Bytes are read from the SMBUS and stored in Buffer.
  The number of bytes read is returned, and will never return a value larger than 32-bytes.
  If Status is not NULL, then the status of the executed command is returned in Status.
  It is the caller's responsibility to make sure Buffer is large enough for the total number of bytes read.
  SMBUS supports a maximum transfer size of 32 bytes, so Buffer does not need to be any larger than 32 bytes.

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  HostControl     The value of Host Control Register to set.
  @param[in]  SmbusAddress    Address that encodes the SMBUS Device Address,
                              SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  WriteBuffer     Pointer to the buffer of bytes to write to the SMBUS.
  @param[out] ReadBuffer      Pointer to the buffer of bytes to read from the SMBUS.
  @param[out] Status          Return status for the executed command.
                              This is an optional parameter and may be NULL.

  @retval The number of bytes read from the SMBUS.

**/
UINTN
InternalSmbusBlock (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINT8               HostControl,
  IN  UINTN               SmbusAddress,
  IN  UINT8               *WriteBuffer,
  OUT UINT8               *ReadBuffer,
  OUT RETURN_STATUS       *Status
  )
{
  RETURN_STATUS                 ReturnStatus;
  UINTN                         Index;
  UINTN                         BytesCount;
  UINT8                         AuxiliaryControl;

  BytesCount = SMBUS_LIB_LENGTH (SmbusAddress);

  //
  // Try to acquire the ownership of SMBUS.
  //
  ReturnStatus = InternalSmbusAcquire (PciIo);
  if (RETURN_ERROR (ReturnStatus)) {
    goto Done;
  }
  //
  // Set the appropriate Host Control Register and auxiliary Control Register.
  //
  AuxiliaryControl = B_SMBUS_IO_AUXC_E32B;
  if (SMBUS_LIB_PEC (SmbusAddress)) {
    AuxiliaryControl |= B_SMBUS_IO_AUXC_AAC;
  }
  //
  // Set Host Command Register.
  //
  SmbusIoWrite (PciIo, R_SMBUS_IO_HCMD, (UINT8) SMBUS_LIB_COMMAND (SmbusAddress));
  //
  // Set Auxiliary Control Regiester.
  //
  SmbusIoWrite (PciIo, R_SMBUS_IO_AUXC, AuxiliaryControl);
  //
  // Clear byte pointer of 32-byte buffer.
  //
  SmbusIoRead (PciIo, R_SMBUS_IO_HCTL);

  if (WriteBuffer != NULL) {
    //
    // Write the number of block to Host Block Data Byte Register.
    //
    SmbusIoWrite (PciIo, R_SMBUS_IO_HD0, (UINT8) BytesCount);
    //
    // Write data block to Host Block Data Register.
    //
    for (Index = 0; Index < BytesCount; Index++) {
      SmbusIoWrite (PciIo, R_SMBUS_IO_HBD, WriteBuffer[Index]);
    }
  }
  //
  // Set SMBUS Device address for the device to send/receive from.
  //
  SmbusIoWrite (PciIo, R_SMBUS_IO_TSA, (UINT8) SmbusAddress);
  //
  // Start the SMBUS transaction and wait for the end.
  //
  ReturnStatus = InternalSmbusStart (PciIo, HostControl);
  if (RETURN_ERROR (ReturnStatus)) {
    goto Done;
  }

  if (ReadBuffer != NULL) {
    //
    // Read the number of block from host block data byte register.
    //
    BytesCount = SmbusIoRead (PciIo, R_SMBUS_IO_HD0);
    //
    // Write data block from Host Block Data Register.
    //
    for (Index = 0; Index < BytesCount; Index++) {
      ReadBuffer[Index] = SmbusIoRead (PciIo, R_SMBUS_IO_HBD);
    }
  }

Done:
  //
  // Clear Host Status Register and Auxiliary Status Register.
  //
  SmbusIoWrite (PciIo, R_SMBUS_IO_HSTS, V_SMBUS_IO_HSTS_ALL);
  SmbusIoWrite (PciIo, R_SMBUS_IO_AUXS, B_SMBUS_IO_AUXS_CRCE);

  if (Status != NULL) {
    *Status = ReturnStatus;
  }

  return BytesCount;
}

/**
  Executes an SMBUS read block command.

  Executes an SMBUS read block command on the SMBUS device specified by SmbusAddress.
  Only the SMBUS Device address and SMBUS command fields of SmbusAddress are required.
  Bytes are read from the SMBUS and stored in Buffer.
  The number of bytes read is returned, and will never return a value larger than 32-bytes.
  If Status is not NULL, then the status of the executed command is returned in Status.
  It is the caller's responsibility to make sure Buffer is large enough for the total number of bytes read.
  SMBUS supports a maximum transfer size of 32 bytes, so Buffer does not need to be any larger than 32 bytes.
  If Length in SmbusAddress is not zero, then ASSERT().
  If Buffer is NULL, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Buffer        Pointer to the buffer to store the bytes read from the SMBUS.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The number of bytes read.

**/
UINTN
EFIAPI
SmbusReadBlock (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINTN               SmbusAddress,
  OUT VOID                *Buffer,
  OUT RETURN_STATUS       *Status        OPTIONAL
  )
{
  if ((Buffer == NULL)                         ||
      (SMBUS_LIB_LENGTH (SmbusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmbusBlock (
           PciIo,
           V_SMBUS_IO_SMB_CMD_BLOCK,
           SmbusAddress | V_SMBUS_IO_READ,
           NULL,
           Buffer,
           Status
           );
}

/**
  Executes an SMBUS write block command.

  Executes an SMBUS write block command on the SMBUS device specified by SmbusAddress.
  The SMBUS Device address, SMBUS command, and SMBUS length fields of SmbusAddress are required.
  Bytes are written to the SMBUS from Buffer.
  The number of bytes written is returned, and will never return a value larger than 32-bytes.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmbusAddress is zero or greater than 32, then ASSERT().
  If Buffer is NULL, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Buffer        Pointer to the buffer to store the bytes read from the SMBUS.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The number of bytes written.

**/
UINTN
EFIAPI
SmbusWriteBlock (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINTN               SmbusAddress,
  OUT VOID                *Buffer,
  OUT RETURN_STATUS       *Status        OPTIONAL
  )
{
  if ((Buffer == NULL)                         ||
      (SMBUS_LIB_LENGTH (SmbusAddress)   == 0) ||
      (SMBUS_LIB_LENGTH (SmbusAddress)   > 32) ||
      (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmbusBlock (
           PciIo,
           V_SMBUS_IO_SMB_CMD_BLOCK,
           SmbusAddress | V_SMBUS_IO_WRITE,
           Buffer,
           NULL,
           Status
           );
}

/**
  Executes an SMBUS block process call command.

  Executes an SMBUS block process call command on the SMBUS device specified by SmbusAddress.
  The SMBUS Device address, SMBUS command, and SMBUS length fields of SmbusAddress are required.
  Bytes are written to the SMBUS from WriteBuffer.  Bytes are then read from the SMBUS into ReadBuffer.
  If Status is not NULL, then the status of the executed command is returned in Status.
  It is the caller's responsibility to make sure ReadBuffer is large enough for the total number of bytes read.
  SMBUS supports a maximum transfer size of 32 bytes, so Buffer does not need to be any larger than 32 bytes.
  If Length in SmbusAddress is zero or greater than 32, then ASSERT().
  If WriteBuffer is NULL, then ASSERT().
  If ReadBuffer is NULL, then ASSERT().
  If any reserved bits of SmbusAddress are set, then ASSERT().

  @param[in]  PciIo         PCI_IO for SMB controller
  @param[in]  SmbusAddress  Address that encodes the SMBUS Device Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  WriteBuffer   Pointer to the buffer of bytes to write to the SMBUS.
  @param[out] ReadBuffer    Pointer to the buffer of bytes to read from the SMBUS.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The number of bytes written.

**/
UINTN
EFIAPI
SmbusBlockProcessCall (
  IN  EFI_PCI_IO_PROTOCOL *PciIo,
  IN  UINTN               SmbusAddress,
  IN  VOID                *WriteBuffer,
  OUT VOID                *ReadBuffer,
  OUT RETURN_STATUS       *Status        OPTIONAL
  )
{
  if ((WriteBuffer == NULL)                    ||
      (ReadBuffer  == NULL)                    ||
      (SMBUS_LIB_LENGTH (SmbusAddress)   == 0) ||
      (SMBUS_LIB_LENGTH (SmbusAddress)   > 32) ||
      (SMBUS_LIB_RESERVED (SmbusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmbusBlock (
           PciIo,
           V_SMBUS_IO_SMB_CMD_BLOCK_PROCESS,
           SmbusAddress | V_SMBUS_IO_WRITE,
           WriteBuffer,
           ReadBuffer,
           Status
           );
}

/**
  Dispatches SMBUS  command based on the input parameters.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in] DeviceAddress        Smbus device address
  @param[in] Command              Command to be sent
  @param[in] Operation            Which SMBus PPI will be used
  @param[in] PecCheck             Defines if Packet Error Code Checking is to be used
  @param[in, out] Length          How many bytes to read/write. Must be 1 <= Length <= 32 depending on the Operation
  @param[in, out] Buffer          Data buffer

  @return Status of the operation
**/
EFI_STATUS
SmbusDispatchCommand (
  IN EFI_PCI_IO_PROTOCOL     *PciIo,
  IN EFI_SMBUS_DEVICE_ADDRESS DeviceAddress,
  IN EFI_SMBUS_DEVICE_COMMAND Command,
  IN EFI_SMBUS_OPERATION      Operation,
  IN BOOLEAN                  PecCheck,
  IN OUT UINTN                *Length,
  IN OUT VOID                 *Buffer
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
      SmbusQuickRead (
        PciIo,
        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
        &ReturnStatus
        );
      break;
    case EfiSmbusQuickWrite:
      SmbusQuickWrite (
        PciIo,
        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
        &ReturnStatus
        );
      break;
    case EfiSmbusReceiveByte:
      if (*Length < 1) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 1;
      ByteBuffer[0] = SmbusReceiveByte (
                        PciIo,
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        &ReturnStatus
                        );
      break;
    case EfiSmbusSendByte:
      if (*Length < 1) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 1;
      ByteBuffer[0] = SmbusSendByte (
                        PciIo,
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
      ByteBuffer[0] = SmbusReadDataByte (
                        PciIo,
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        &ReturnStatus
                        );
      break;
    case EfiSmbusWriteByte:
      if (*Length < 1) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 1;
      ByteBuffer[0] = SmbusWriteDataByte (
                        PciIo,
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
      WordBuffer[0] = SmbusReadDataWord (
                        PciIo,
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        &ReturnStatus
                        );
      break;
    case EfiSmbusWriteWord:
      if (*Length < 2) {
        return EFI_BUFFER_TOO_SMALL;
      }
      *Length = 2;
      WordBuffer[0] = SmbusWriteDataWord (
                        PciIo,
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        WordBuffer[0],
                        &ReturnStatus
                        );
      break;
    case EfiSmbusReadBlock:
      *Length = SmbusReadBlock (
                  PciIo,
                  SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                  Buffer,
                  &ReturnStatus
                  );
      break;
    case EfiSmbusWriteBlock:
      *Length = SmbusWriteBlock (
                  PciIo,
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
      WordBuffer[0] = SmbusProcessCall (
                        PciIo,
                        SMBUS_LIB_ADDRESS (DeviceAddress.SmbusDeviceAddress, Command, 0, PecCheck),
                        WordBuffer[0],
                        &ReturnStatus
                        );
      break;
    case EfiSmbusBWBRProcessCall:
      *Length = SmbusBlockProcessCall (
                  PciIo,
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
  Issue a prepare ARP command to informs all devices that the ARP Smbus Host Controller is starting the ARP process

  @param[in] PciIo                PCI_IO for SMB controller

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SmbusPrepareToArp (
  IN EFI_PCI_IO_PROTOCOL  *PciIo
  )
{
  EFI_SMBUS_DEVICE_ADDRESS  DeviceAddress;
  EFI_STATUS                Status;
  UINTN                     Length;
  UINT8                     Buffer;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  DeviceAddress.SmbusDeviceAddress = SMBUS_ADDRESS_ARP;
  Length                          = 1;
  Buffer                          = SMBUS_DATA_PREPARE_TO_ARP;

  Status = SmbusDispatchCommand (
    PciIo,
    DeviceAddress,
    0,
    EfiSmbusSendByte,
    TRUE,
    &Length,
    &Buffer
    );

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return Status;
}

/**
  Issue a Get UDID (general) command to requests ARP-capable and/or Discoverable devices to
  return their SMBus Device address along with their UDID.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in, out] DeviceMap       Pointer to SMBUS device map table that SMBus device return

  @retval EFI_SUCCESS             The function completed successfully
**/
STATIC
EFI_STATUS
SmbusGetUdidGeneral (
  IN      EFI_PCI_IO_PROTOCOL   *PciIo,
  IN OUT  EFI_SMBUS_DEVICE_MAP  *DeviceMap
  )
{
  EFI_SMBUS_DEVICE_ADDRESS  DeviceAddress;
  EFI_STATUS                Status;
  UINTN                     Length;
  UINT8                     Buffer[SMBUS_GET_UDID_LENGTH];

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  DeviceAddress.SmbusDeviceAddress = SMBUS_ADDRESS_ARP;
  Length                          = SMBUS_GET_UDID_LENGTH;

  Status = SmbusDispatchCommand (
    PciIo,
    DeviceAddress,
    SMBUS_DATA_GET_UDID_GENERAL,
    EfiSmbusReadBlock,
    TRUE,
    &Length,
    &Buffer
    );

  if (!EFI_ERROR (Status)) {
    if (Length == SMBUS_GET_UDID_LENGTH) {
      DeviceMap->SmbusDeviceUdid.DeviceCapabilities = Buffer[0];
      DeviceMap->SmbusDeviceUdid.VendorRevision     = Buffer[1];
      DeviceMap->SmbusDeviceUdid.VendorId           = (UINT16) ((Buffer[2] << 8) + Buffer[3]);
      DeviceMap->SmbusDeviceUdid.DeviceId           = (UINT16) ((Buffer[4] << 8) + Buffer[5]);
      DeviceMap->SmbusDeviceUdid.Interface          = (UINT16) ((Buffer[6] << 8) + Buffer[7]);
      DeviceMap->SmbusDeviceUdid.SubsystemVendorId  = (UINT16) ((Buffer[8] << 8) + Buffer[9]);
      DeviceMap->SmbusDeviceUdid.SubsystemDeviceId  = (UINT16) ((Buffer[10] << 8) + Buffer[11]);
      DeviceMap->SmbusDeviceUdid.VendorSpecificId = (UINT32) ((Buffer[12] << 24) + (Buffer[13] << 16) + (Buffer[14] << 8) + Buffer[15]);
      DeviceMap->SmbusDeviceAddress.SmbusDeviceAddress = (UINT8) (Buffer[16] >> 1);
    } else {
      Status = EFI_DEVICE_ERROR;
    }
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return Status;
}

/**
  Issue a Assign address command to assigns an address to a specific SMBus device.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in, out] DeviceMap       Pointer to SMBUS device map table that send to SMBus device

  @retval EFI_SUCCESS             The function completed successfully
**/
STATIC
EFI_STATUS
SmbusAssignAddress (
  IN      EFI_PCI_IO_PROTOCOL    *PciIo,
  IN OUT  EFI_SMBUS_DEVICE_MAP  *DeviceMap
  )
{
  EFI_SMBUS_DEVICE_ADDRESS  DeviceAddress;
  EFI_STATUS                Status;
  UINTN                     Length;
  UINT8                     Buffer[SMBUS_GET_UDID_LENGTH];

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Buffer[0]                       = DeviceMap->SmbusDeviceUdid.DeviceCapabilities;
  Buffer[1]                       = DeviceMap->SmbusDeviceUdid.VendorRevision;
  Buffer[2]                       = (UINT8) (DeviceMap->SmbusDeviceUdid.VendorId >> 8);
  Buffer[3]                       = (UINT8) (DeviceMap->SmbusDeviceUdid.VendorId);
  Buffer[4]                       = (UINT8) (DeviceMap->SmbusDeviceUdid.DeviceId >> 8);
  Buffer[5]                       = (UINT8) (DeviceMap->SmbusDeviceUdid.DeviceId);
  Buffer[6]                       = (UINT8) (DeviceMap->SmbusDeviceUdid.Interface >> 8);
  Buffer[7]                       = (UINT8) (DeviceMap->SmbusDeviceUdid.Interface);
  Buffer[8]                       = (UINT8) (DeviceMap->SmbusDeviceUdid.SubsystemVendorId >> 8);
  Buffer[9]                       = (UINT8) (DeviceMap->SmbusDeviceUdid.SubsystemVendorId);
  Buffer[10]                      = (UINT8) (DeviceMap->SmbusDeviceUdid.SubsystemDeviceId >> 8);
  Buffer[11]                      = (UINT8) (DeviceMap->SmbusDeviceUdid.SubsystemDeviceId);
  Buffer[12]                      = (UINT8) (DeviceMap->SmbusDeviceUdid.VendorSpecificId >> 24);
  Buffer[13]                      = (UINT8) (DeviceMap->SmbusDeviceUdid.VendorSpecificId >> 16);
  Buffer[14]                      = (UINT8) (DeviceMap->SmbusDeviceUdid.VendorSpecificId >> 8);
  Buffer[15]                      = (UINT8) (DeviceMap->SmbusDeviceUdid.VendorSpecificId);
  Buffer[16]                      = (UINT8) (DeviceMap->SmbusDeviceAddress.SmbusDeviceAddress << 1);

  DeviceAddress.SmbusDeviceAddress = SMBUS_ADDRESS_ARP;
  Length                          = SMBUS_GET_UDID_LENGTH;

  Status = SmbusDispatchCommand (
    PciIo,
    DeviceAddress,
    SMBUS_DATA_ASSIGN_ADDRESS,
    EfiSmbusWriteBlock,
    TRUE,
    &Length,
    &Buffer
    );

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return Status;
}

/**
  Check whether the address is assignable.

  @param[in] DeviceEnumData       Pointer to device enumeartion data
  @param[in] DeviceAddress        The SMBus Device Address for checking

  @retval TRUE                    The address is assignable
  @retval FALSE                   The address is not assignable
**/
STATIC
BOOLEAN
IsAddressAvailable (
  IN SMBUS_DEVICE_ENUMERATION_DATA  *DeviceEnumData,
  IN EFI_SMBUS_DEVICE_ADDRESS       DeviceAddress
  )
{
  UINT8 Index;

  ///
  /// See if we have already assigned this address to a device
  ///
  for (Index = 0; Index < DeviceEnumData->DeviceMapEntries; Index++) {
    if (DeviceAddress.SmbusDeviceAddress == DeviceEnumData->DeviceMap[Index].SmbusDeviceAddress.SmbusDeviceAddress) {
      return FALSE;
    }
  }
  ///
  /// See if this address is claimed by a platform non-ARP-capable device
  ///
  for (Index = 0; Index < DeviceEnumData->PlatformNumRsvd; Index++) {
    if ((DeviceAddress.SmbusDeviceAddress << 1) == DeviceEnumData->PlatformRsvdAddr[Index]) {
      return FALSE;
    }
  }
  ///
  /// See if this is a reserved address
  ///
  for (Index = 0; Index < SMBUS_NUM_RESERVED; Index++) {
    if ((DeviceAddress.SmbusDeviceAddress << 1) == (UINTN) mReservedAddress[Index]) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Find an available address to assign

  @param[in] DeviceEnumData       Pointer to device enumeartion data
  @param[in] DeviceAddress        Buffer to store new SMBus Device Address during directed ARP.

  @retval EFI_OUT_OF_RESOURCES    There is no available address to assign
  @retval EFI_SUCCESS             The function completed successfully
**/
STATIC
EFI_STATUS
GetNextAvailableAddress (
  IN SMBUS_DEVICE_ENUMERATION_DATA  *DeviceEnumData,
  IN EFI_SMBUS_DEVICE_ADDRESS       *DeviceAddress
  )
{
  for (DeviceAddress->SmbusDeviceAddress = 0x03;
       DeviceAddress->SmbusDeviceAddress < 0x7F;
       DeviceAddress->SmbusDeviceAddress++
       ) {
    if (IsAddressAvailable (DeviceEnumData, *DeviceAddress)) {
      return EFI_SUCCESS;
    }
  }

  return EFI_OUT_OF_RESOURCES;
}

/**
  Do a fully (general) Arp procress to assign the SMBus Device address of all ARP-capable device.
  This function will issue issue the "Prepare to ARP", "Get UDID" and "Assign Address" commands.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in] DeviceEnumData       Pointer to device enumeartion data

  @retval EFI_OUT_OF_RESOURCES    No available address to assign
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SmbusFullArp (
  IN EFI_PCI_IO_PROTOCOL            *PciIo,
  IN SMBUS_DEVICE_ENUMERATION_DATA  *DeviceEnumData
  )
{
  EFI_STATUS            Status;
  EFI_SMBUS_DEVICE_MAP  *CurrentDeviceMap;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Status = SmbusPrepareToArp (PciIo);
  if (EFI_ERROR (Status)) {
    if (Status == EFI_DEVICE_ERROR) {
      ///
      ///  ARP is complete
      ///
      return EFI_SUCCESS;
    } else {
      return Status;
    }
  }
  ///
  ///  Main loop to ARP all ARP-capable devices
  ///
  do {
    CurrentDeviceMap  = &DeviceEnumData->DeviceMap[DeviceEnumData->DeviceMapEntries];
    Status            = SmbusGetUdidGeneral (PciIo, CurrentDeviceMap);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (CurrentDeviceMap->SmbusDeviceAddress.SmbusDeviceAddress == (0xFF >> 1)) {
      ///
      /// If address is unassigned, assign it
      ///
      Status = GetNextAvailableAddress (
                 DeviceEnumData,
                 &CurrentDeviceMap->SmbusDeviceAddress
                 );
      if (EFI_ERROR (Status)) {
        return EFI_OUT_OF_RESOURCES;
      }
    } else if (((CurrentDeviceMap->SmbusDeviceUdid.DeviceCapabilities) & 0xC0) != 0) {
      ///
      /// if address is not fixed, check if the current address is available
      ///
      if (!IsAddressAvailable (
             DeviceEnumData,
             CurrentDeviceMap->SmbusDeviceAddress
             )) {
        ///
        /// if currently assigned address is already used, get a new one
        ///
        Status = GetNextAvailableAddress (
                   DeviceEnumData,
                   &CurrentDeviceMap->SmbusDeviceAddress
                   );
        if (EFI_ERROR (Status)) {
          return EFI_OUT_OF_RESOURCES;
        }
      }
    }

    Status = SmbusAssignAddress (PciIo, CurrentDeviceMap);
    if (EFI_ERROR (Status)) {
      ///
      /// If there was a device error, just continue on and try again.
      /// Other errors should be reported.
      ///
      if (Status != EFI_DEVICE_ERROR) {
        return Status;
      }
    } else {
      ///
      /// If there was no error, the address was assigned and we must update our
      /// records.
      ///
      DeviceEnumData->DeviceMapEntries++;
    }

  } while (DeviceEnumData->DeviceMapEntries < MAX_SMBUS_DEVICES);

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Do a directed Arp procress to assign the SMBus Device address of a single ARP-capable device.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in] DeviceEnumData       Pointer to device enumeartion data
  @param[in] SmbusUdid            When doing a directed ARP, ARP the device with this UDID.
  @param[in, out] DeviceAddress   Buffer to store new SMBus Device Address during directed ARP.

  @retval EFI_OUT_OF_RESOURCES    DeviceMapEntries is more than Max number of SMBus devices.
                                  Or there is no available address to assign
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SmbusDirectedArp (
  IN     EFI_PCI_IO_PROTOCOL            *PciIo,
  IN     SMBUS_DEVICE_ENUMERATION_DATA  *DeviceEnumData,
  IN     EFI_SMBUS_UDID                 *SmbusUdid,
  IN OUT EFI_SMBUS_DEVICE_ADDRESS       *DeviceAddress
  )
{
  EFI_STATUS            Status;
  EFI_SMBUS_DEVICE_MAP  *CurrentDeviceMap;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  if (DeviceEnumData->DeviceMapEntries >= MAX_SMBUS_DEVICES) {
    return EFI_OUT_OF_RESOURCES;
  }

  CurrentDeviceMap = &DeviceEnumData->DeviceMap[DeviceEnumData->DeviceMapEntries];

  ///
  /// Find an available address to assign
  ///
  Status = GetNextAvailableAddress (
             DeviceEnumData,
             &CurrentDeviceMap->SmbusDeviceAddress
             );
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  CurrentDeviceMap->SmbusDeviceUdid.DeviceCapabilities  = SmbusUdid->DeviceCapabilities;
  CurrentDeviceMap->SmbusDeviceUdid.DeviceId            = SmbusUdid->DeviceId;
  CurrentDeviceMap->SmbusDeviceUdid.Interface           = SmbusUdid->Interface;
  CurrentDeviceMap->SmbusDeviceUdid.SubsystemDeviceId   = SmbusUdid->SubsystemDeviceId;
  CurrentDeviceMap->SmbusDeviceUdid.SubsystemVendorId   = SmbusUdid->SubsystemVendorId;
  CurrentDeviceMap->SmbusDeviceUdid.VendorId            = SmbusUdid->VendorId;
  CurrentDeviceMap->SmbusDeviceUdid.VendorRevision      = SmbusUdid->VendorRevision;
  CurrentDeviceMap->SmbusDeviceUdid.VendorSpecificId    = SmbusUdid->VendorSpecificId;

  Status = SmbusAssignAddress (PciIo, CurrentDeviceMap);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DeviceEnumData->DeviceMapEntries++;
  DeviceAddress->SmbusDeviceAddress = CurrentDeviceMap->SmbusDeviceAddress.SmbusDeviceAddress;

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

#define INTEL_VENDOR_ID  0x8086

/**
  Checks if PciIo protocol is connected with a supported
  PCI_IO_PROTOCOL.

  @param[in] PciIo  Pointer to a PCI_IO_PROTOCOL to check

  @retval EFI_SUCCESS      Supported SMB device present
  @retval EFI_UNSUPPORTED  Unsupported device
**/
EFI_STATUS
SmbusSupported (
  IN EFI_PCI_IO_PROTOCOL  *PciIo
  )
{
  PCI_TYPE00  PciData;
  EFI_STATUS  Status;

  Status = PciIo->Pci.Read (
              PciIo,
              EfiPciIoWidthUint8,
              PCI_CLASSCODE_OFFSET,
              sizeof (PciData.Hdr.ClassCode),
              &PciData.Hdr.ClassCode
              );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  if (PciData.Hdr.ClassCode[2] !=  PCI_CLASS_SERIAL || PciData.Hdr.ClassCode[1] != PCI_CLASS_SERIAL_SMB) {
    return EFI_UNSUPPORTED;
  }

  Status = PciIo->Pci.Read (
            PciIo,
            EfiPciIoWidthUint8,
            PCI_VENDOR_ID_OFFSET,
            sizeof (PciData.Hdr.VendorId),
            &PciData.Hdr.VendorId
            );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  if (PciData.Hdr.VendorId != INTEL_VENDOR_ID) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Enables host controler to make it ready to process commands.

  @param[in] PciIo  Pointer to a PCI_IO_PROTOCOL

  @retval EFI_SUCCESS  Controller initialized
  @retval Other        Failed to initialize controller
**/
EFI_STATUS
SmbusEnableHc (
  IN EFI_PCI_IO_PROTOCOL  *PciIo
  )
{
  EFI_STATUS  Status;
  UINT8       Hsten;
  UINT64      EnabledPciAttributes;
  UINT32      HostTiming;

  Status = PciIo->Attributes (
                  PciIo,
                  EfiPciIoAttributeOperationSupported,
                  0,
                  &EnabledPciAttributes
                  );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  } else {
    EnabledPciAttributes &= (UINT64)EFI_PCI_IO_ATTRIBUTE_IO;
    Status                = PciIo->Attributes (
                                    PciIo,
                                    EfiPciIoAttributeOperationEnable,
                                    EnabledPciAttributes,
                                    NULL
                                    );
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  }

  PciIo->Pci.Read (
    PciIo,
    EfiPciIoWidthUint8,
    R_SMBUS_CFG_HCFG,
    sizeof (Hsten),
    &Hsten
    );
  Hsten |= B_SMBUS_CFG_HCFG_SSRESET;
  PciIo->Pci.Write (
    PciIo,
    EfiPciIoWidthUint8,
    R_SMBUS_CFG_HCFG,
    sizeof (Hsten),
    &Hsten
  );

  HostTiming = SMBUS_DEFAULT_HOST_TIMING_VALUE;
  PciIo->Pci.Write (
    PciIo,
    EfiPciIoWidthUint32,
    R_SMBUS_CFG_HTIM,
    sizeof (HostTiming),
    &HostTiming
  );

  PciIo->Pci.Read (
    PciIo,
    EfiPciIoWidthUint8,
    R_SMBUS_CFG_HCFG,
    sizeof (Hsten),
    &Hsten
  );
  Hsten &= ~(B_SMBUS_CFG_HCFG_SSEN | B_SMBUS_CFG_HCFG_I2CEN);
  Hsten |= B_SMBUS_CFG_HCFG_HSTEN;
  PciIo->Pci.Write (
    PciIo,
    EfiPciIoWidthUint8,
    R_SMBUS_CFG_HCFG,
    sizeof (Hsten),
    &Hsten
  );

  SmbusIoWrite (PciIo, R_SMBUS_IO_HSTS, V_SMBUS_IO_HSTS_ALL);

  return Status;
}