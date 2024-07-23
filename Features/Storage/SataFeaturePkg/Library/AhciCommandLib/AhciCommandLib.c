/**@file
  AHCI Command library

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
**/

#include "AhciCommand.h"

/**
  Get identify data through Ata Pass Thru Protocol.

  @param[in] AtaPassThru         The pointer to the ATA_PASS_THRU protocol.
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.
  @param[in] IdentifyData        The buffer to store identify data.

  @retval EFI_SUCCESS            Successful to get identify data.
  @retval EFI_INVALID_PARAMETER  The parameter passed-in is invalid.
  @retval EFI_OUT_OF_RESOURCES   Not enough memory to get identify data.
  @retval EFI_DEVICE_ERROR       Can not get identify data.

**/
EFI_STATUS
EFIAPI
GetAtaIdentifyData (
  IN  EFI_ATA_PASS_THRU_PROTOCOL    *AtaPassThru,
  IN  UINT16                        Port,
  IN  UINT16                        PortMultiplierPort,
  IN  ATA_IDENTIFY_DATA             *IdentifyData
  )
{
  EFI_STATUS                        Status;
  EFI_ATA_COMMAND_BLOCK             Acb;
  EFI_ATA_STATUS_BLOCK              *Asb;
  EFI_ATA_PASS_THRU_COMMAND_PACKET  Packet;

  if ((AtaPassThru == NULL) || (IdentifyData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a (Port: 0x%x, PortMultiplier: 0x%x).\n", __FUNCTION__, Port, PortMultiplierPort));

  //
  // The 'Asb' field (a pointer to the EFI_ATA_STATUS_BLOCK structure) in
  // EFI_ATA_PASS_THRU_COMMAND_PACKET is required to be aligned specified by
  // the 'IoAlign' field in the EFI_ATA_PASS_THRU_MODE structure. Meanwhile,
  // the structure EFI_ATA_STATUS_BLOCK is composed of only UINT8 fields, so it
  // may not be aligned when allocated on stack for some compilers. Hence, we
  // use the API AllocateAlignedPages to ensure this structure is properly
  // aligned.
  //
  Asb = AllocateAlignedPages (
          EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)),
          AtaPassThru->Mode->IoAlign
          );
  if (Asb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Prepare for ATA command block.
  //
  ZeroMem (&Acb, sizeof (Acb));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));
  Acb.AtaCommand    = ATA_CMD_IDENTIFY_DRIVE;
  Acb.AtaDeviceHead = (UINT8) (BIT7 | BIT6 | BIT5 | (PortMultiplierPort == 0xFFFF ? 0 : (PortMultiplierPort << 4)));

  //
  // Prepare for ATA pass through packet.
  //
  ZeroMem (&Packet, sizeof (Packet));
  Packet.Protocol = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN;
  Packet.Length   = EFI_ATA_PASS_THRU_LENGTH_BYTES | EFI_ATA_PASS_THRU_LENGTH_SECTOR_COUNT;
  Packet.Asb      = Asb;
  Packet.Acb      = &Acb;
  Packet.InDataBuffer     = IdentifyData;
  Packet.InTransferLength = sizeof (ATA_IDENTIFY_DATA);
  Packet.Timeout          = ATA_TIMEOUT;

  Status = AtaPassThru->PassThru (
                          AtaPassThru,
                          Port,
                          PortMultiplierPort,
                          &Packet,
                          NULL
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: Identify Command Status=%r\n", __FUNCTION__, Status));
  }
  FreeAlignedPages (Asb, EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)));

  return Status;
}

/**
  Send set password cmd through Ata Pass Thru Protocol.

  @param[in] AtaPassThru                The pointer to the ATA_PASS_THRU protocol.
  @param[in] Port                       The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort         The port multiplier port number of the ATA device to send the command.
                                        If there is no port multiplier, then specify 0xFFFF.
  @param[in] Identifier                 The identifier to set user or master password.
  @param[in] SecurityLevel              The security level to be set to device.
  @param[in] MasterPasswordIdentifier   The master password identifier to be set to device.
  @param[in] Password                   The hdd password of attached ATA device.

  @retval EFI_SUCCESS            Successful to set hdd password cmd.
  @retval EFI_INVALID_PARAMETER  The parameter passed-in is invalid.
  @retval EFI_OUT_OF_RESOURCES   Not enough memory to set hdd password cmd.
  @retval EFI_DEVICE_ERROR       Can not set hdd password cmd.

**/
EFI_STATUS
EFIAPI
SetAtaPassword (
  IN EFI_ATA_PASS_THRU_PROTOCOL  *AtaPassThru,
  IN UINT16                      Port,
  IN UINT16                      PortMultiplierPort,
  IN CHAR8                       Identifier,
  IN CHAR8                       SecurityLevel,
  IN CHAR16                      MasterPasswordIdentifier,
  IN CHAR8                       Password[ATA_PASSWORD_MAX_LENGTH]
  )
{
  EFI_STATUS                        Status;
  EFI_ATA_COMMAND_BLOCK             Acb;
  EFI_ATA_STATUS_BLOCK              *Asb;
  EFI_ATA_PASS_THRU_COMMAND_PACKET  Packet;
  ATA_SET_PASSWORD_COMMAND_BLOCK    SecuritySetPassword;

  if ((AtaPassThru == NULL) || (Password == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // The 'Asb' field (a pointer to the EFI_ATA_STATUS_BLOCK structure) in
  // EFI_ATA_PASS_THRU_COMMAND_PACKET is required to be aligned specified by
  // the 'IoAlign' field in the EFI_ATA_PASS_THRU_MODE structure. Meanwhile,
  // the structure EFI_ATA_STATUS_BLOCK is composed of only UINT8 fields, so it
  // may not be aligned when allocated on stack for some compilers. Hence, we
  // use the API AllocateAlignedPages to ensure this structure is properly
  // aligned.
  //
  Asb = AllocateAlignedPages (
          EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)),
          AtaPassThru->Mode->IoAlign
          );
  if (Asb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Prepare for ATA command block.
  //
  ZeroMem (&Acb, sizeof (Acb));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));
  Acb.AtaCommand    = ATA_CMD_SECURITY_SET_PASSWORD;
  Acb.AtaDeviceHead = (UINT8)(PortMultiplierPort == 0xFFFF ? 0 : (PortMultiplierPort << 4));

  //
  // Prepare for ATA pass through packet.
  //
  ZeroMem (&Packet, sizeof (Packet));
  Packet.Protocol = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT;
  Packet.Length   = EFI_ATA_PASS_THRU_LENGTH_BYTES | EFI_ATA_PASS_THRU_LENGTH_SECTOR_COUNT;
  Packet.Asb      = Asb;
  Packet.Acb      = &Acb;

  ZeroMem (&SecuritySetPassword, sizeof (ATA_SET_PASSWORD_COMMAND_BLOCK));
  CopyMem (&SecuritySetPassword.Password, Password, ATA_PASSWORD_MAX_LENGTH);
  SecuritySetPassword.ControlData = Identifier & BIT0;
  SecuritySetPassword.ControlData |= (UINT16)(SecurityLevel << 8) & BIT8;
  if ((Identifier & BIT0) != 0) {
    SecuritySetPassword.MasterPasswordRevisionCode = MasterPasswordIdentifier;
  }

  Packet.OutDataBuffer     = &SecuritySetPassword;
  Packet.OutTransferLength = sizeof (ATA_SET_PASSWORD_COMMAND_BLOCK);
  Packet.Timeout           = ATA_TIMEOUT;

  Status = AtaPassThru->PassThru (
                          AtaPassThru,
                          Port,
                          PortMultiplierPort,
                          &Packet,
                          NULL
                          );
  if (!EFI_ERROR (Status) &&
    ((Asb->AtaStatus & ATA_STSREG_ERR) != 0) &&
    ((Asb->AtaError & ATA_ERRREG_ABRT) != 0))
  {
    DEBUG ((DEBUG_INFO, "AtaStatus = %02x, AtaError = %02x\n", Asb->AtaStatus, Asb->AtaError));
    Status = EFI_DEVICE_ERROR;
  }

  FreeAlignedPages (Asb, EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)));
  DEBUG ((DEBUG_INFO, "%a() - Port = 0x%04x, multiplier = 0x%04x, Status = %r\n", __FUNCTION__, Port, PortMultiplierPort, Status));
  return Status;
}

/**
  Send disable password cmd through Ata Pass Thru Protocol.

  @param[in] AtaPassThru         The pointer to the ATA_PASS_THRU protocol.
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.
  @param[in] Identifier          The identifier to set user or master password.
  @param[in] Password            The hdd password of attached ATA device.

  @retval EFI_SUCCESS            Successful to disable hdd password cmd.
  @retval EFI_INVALID_PARAMETER  The parameter passed-in is invalid.
  @retval EFI_OUT_OF_RESOURCES   Not enough memory to disable hdd password cmd.
  @retval EFI_DEVICE_ERROR       Can not disable hdd password cmd.

**/
EFI_STATUS
EFIAPI
DisableAtaPassword (
  IN EFI_ATA_PASS_THRU_PROTOCOL     *AtaPassThru,
  IN UINT16                         Port,
  IN UINT16                         PortMultiplierPort,
  IN CHAR8                          Identifier,
  IN CHAR8                          Password[ATA_PASSWORD_MAX_LENGTH]
  )
{
  EFI_STATUS                         Status;
  EFI_ATA_COMMAND_BLOCK              Acb;
  EFI_ATA_STATUS_BLOCK               *Asb;
  EFI_ATA_PASS_THRU_COMMAND_PACKET   Packet;
  ATA_SECURE_COMMAND_BLOCK           SecurityDisablePassword;

  if ((AtaPassThru == NULL) || (Password == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // The 'Asb' field (a pointer to the EFI_ATA_STATUS_BLOCK structure) in
  // EFI_ATA_PASS_THRU_COMMAND_PACKET is required to be aligned specified by
  // the 'IoAlign' field in the EFI_ATA_PASS_THRU_MODE structure. Meanwhile,
  // the structure EFI_ATA_STATUS_BLOCK is composed of only UINT8 fields, so it
  // may not be aligned when allocated on stack for some compilers. Hence, we
  // use the API AllocateAlignedPages to ensure this structure is properly
  // aligned.
  //
  Asb = AllocateAlignedPages (
          EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)),
          AtaPassThru->Mode->IoAlign
          );
  if (Asb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Prepare for ATA command block.
  //
  ZeroMem (&Acb, sizeof (Acb));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));
  Acb.AtaCommand    = ATA_CMD_SECURITY_DISABLE_PASSWORD;
  Acb.AtaDeviceHead = (UINT8) (PortMultiplierPort == 0xFFFF ? 0 : (PortMultiplierPort << 4));

  //
  // Prepare for ATA pass through packet.
  //
  ZeroMem (&Packet, sizeof (Packet));
  Packet.Protocol = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT;
  Packet.Length   = EFI_ATA_PASS_THRU_LENGTH_BYTES;
  Packet.Asb      = Asb;
  Packet.Acb      = &Acb;

  ZeroMem (&SecurityDisablePassword, sizeof (ATA_SECURE_COMMAND_BLOCK));
  CopyMem (&SecurityDisablePassword.Password, Password, ATA_PASSWORD_MAX_LENGTH);
  SecurityDisablePassword.ControlData = Identifier & BIT0;

  Packet.OutDataBuffer     = &SecurityDisablePassword;
  Packet.OutTransferLength = sizeof (ATA_SECURE_COMMAND_BLOCK);
  Packet.Timeout           = ATA_TIMEOUT;

  Status = AtaPassThru->PassThru (
                          AtaPassThru,
                          Port,
                          PortMultiplierPort,
                          &Packet,
                          NULL
                          );
  if (!EFI_ERROR (Status) &&
    ((Asb->AtaStatus & ATA_STSREG_ERR) != 0) &&
    ((Asb->AtaError & ATA_ERRREG_ABRT) != 0)) {
    DEBUG ((DEBUG_INFO, "AtaStatus = %02x, AtaError = %02x\n", Asb->AtaStatus, Asb->AtaError));
    Status = EFI_DEVICE_ERROR;
  }

  FreeAlignedPages (Asb, EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)));
  DEBUG ((DEBUG_INFO, "%a() - Port = 0x%04x, multiplier = 0x%04x, Status = %r\n", __FUNCTION__, Port, PortMultiplierPort, Status));
  return Status;
}

/**
  Send prepare secure erase ATA command

  @param[in] AtaPassThru                The pointer to the EFI_ATA_PASS_THRU_PROTOCOL.
  @param[in] Port                       Port number on the ATA controller
  @param[in] PortMultiplierPort         Port multiplier port number on the ATA controller

  @retval    EFI_SUCCESS
  @retval    EFI_BAD_BUFFER_SIZE        The ATA command was not executed. The number of bytes that could be transferred
                                        is returned in InTransferLength. For write and bi-directional commands,
                                        OutTransferLength bytes were transferred by OutDataBuffer.
  @retval    EFI_NOT_READY              The ATA command could not be sent because there are too many ATA commands
                                        already queued. The caller may retry again later.
  @retval    EFI_DEVICE_ERROR           A device error occurred while attempting to send the ATA command.
  @retval    EFI_INVALID_PARAMETER      Port, PortMultiplierPort, or the contents of Acb are invalid. The ATA
                                        command was not sent, so no additional status information is available.

**/
EFI_STATUS
EFIAPI
AtaPrepareSecureErase (
  IN EFI_ATA_PASS_THRU_PROTOCOL     *AtaPassThru,
  IN UINT16                         Port,
  IN UINT16                         PortMultiplierPort
  )
{
  EFI_STATUS                        Status;
  EFI_ATA_PASS_THRU_COMMAND_PACKET  Packet;
  EFI_ATA_COMMAND_BLOCK             Acb;
  EFI_ATA_STATUS_BLOCK              *Asb;

  if ((AtaPassThru == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // The 'Asb' field (a pointer to the EFI_ATA_STATUS_BLOCK structure) in
  // EFI_ATA_PASS_THRU_COMMAND_PACKET is required to be aligned specified by
  // the 'IoAlign' field in the EFI_ATA_PASS_THRU_MODE structure. Meanwhile,
  // the structure EFI_ATA_STATUS_BLOCK is composed of only UINT8 fields, so it
  // may not be aligned when allocated on stack for some compilers. Hence, we
  // use the API AllocateAlignedPages to ensure this structure is properly
  // aligned.
  //
  Asb = AllocateAlignedPages (
          EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)),
          AtaPassThru->Mode->IoAlign
          );
  if (Asb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&Acb, sizeof (EFI_ATA_COMMAND_BLOCK));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));
  Acb.AtaCommand    = ATA_CMD_SECURITY_ERASE_PREPARE;
  Acb.AtaDeviceHead = (UINT8) (PortMultiplierPort == 0xFFFF ? 0 : (PortMultiplierPort << 4));

  ZeroMem (&Packet, sizeof (EFI_ATA_PASS_THRU_COMMAND_PACKET));
  Packet.Acb             = &Acb;
  Packet.Asb             = Asb;
  Packet.Protocol        = EFI_ATA_PASS_THRU_PROTOCOL_ATA_NON_DATA;
  Packet.Length          = EFI_ATA_PASS_THRU_LENGTH_BYTES | EFI_ATA_PASS_THRU_LENGTH_SECTOR_COUNT;
  Packet.Timeout         = ATA_TIMEOUT;

  Status = AtaPassThru->PassThru (
                          AtaPassThru,
                          Port,
                          PortMultiplierPort,
                          &Packet,
                          NULL
                          );
  if (!EFI_ERROR (Status) &&
    ((Asb->AtaStatus & ATA_STSREG_ERR) != 0) &&
    ((Asb->AtaError & ATA_ERRREG_ABRT) != 0)) {
    DEBUG ((DEBUG_INFO, "AtaStatus = %02x, AtaError = %02x\n", Asb->AtaStatus, Asb->AtaError));
    Status = EFI_DEVICE_ERROR;
  }

  FreeAlignedPages (Asb, EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)));
  DEBUG ((DEBUG_INFO, "%a() - Port = 0x%04x, multiplier = 0x%04x, Status = %r\n", __FUNCTION__, Port, PortMultiplierPort, Status));
  return Status;
}

/**
  Send ATA secure erase unit command and wait for completion for a specified timeout

  @param[in] AtaPassThru           The pointer to the EFI_ATA_PASS_THRU_PROTOCOL.
  @param[in] Port                  Port number on the ATA controller
  @param[in] PortMultiplierPort    Port multiplier port number on the ATA controller
  @param[in] Identifier            The identifier to set user or master password.
  @param[in] EraseMode             Erase mode: normal or enhanced.
  @param[in] Password              Disk password
  @param[in] Timeout               Time to wait in 100ns units

  @retval    EFI_SUCCESS
  @retval    EFI_TIMEOUT           Secure erase command takes longer to complete
                                   then the specified timeout
  @retval    EFI_DEVICE_ERROR      The device has failed to complete the command
  @retval    EFI_OUT_OF_RESOURCES  There was not enough resources to complete
                                   the call
**/
EFI_STATUS
EFIAPI
SendAtaEraseUnit (
  IN EFI_ATA_PASS_THRU_PROTOCOL        *AtaPassThru,
  IN UINT16                            Port,
  IN UINT16                            PortMultiplierPort,
  IN CHAR8                             Identifier,
  IN CHAR8                             EraseMode,
  IN CHAR8                             Password[ATA_PASSWORD_MAX_LENGTH],
  IN UINT64                            Timeout
  )
{
  EFI_STATUS                           Status;
  EFI_ATA_PASS_THRU_COMMAND_PACKET     Packet;
  EFI_ATA_COMMAND_BLOCK                Acb;
  EFI_ATA_STATUS_BLOCK                 *Asb;
  ATA_SECURE_COMMAND_BLOCK             SecurityEraseUnit;

  if ((AtaPassThru == NULL) || (Password == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // The 'Asb' field (a pointer to the EFI_ATA_STATUS_BLOCK structure) in
  // EFI_ATA_PASS_THRU_COMMAND_PACKET is required to be aligned specified by
  // the 'IoAlign' field in the EFI_ATA_PASS_THRU_MODE structure. Meanwhile,
  // the structure EFI_ATA_STATUS_BLOCK is composed of only UINT8 fields, so it
  // may not be aligned when allocated on stack for some compilers. Hence, we
  // use the API AllocateAlignedPages to ensure this structure is properly
  // aligned.
  //
  Asb = AllocateAlignedPages (
          EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)),
          AtaPassThru->Mode->IoAlign
          );
  if (Asb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&Acb, sizeof (EFI_ATA_COMMAND_BLOCK));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));
  Acb.AtaCommand    = ATA_CMD_SECURITY_ERASE_UNIT;
  Acb.AtaDeviceHead = (UINT8)(PortMultiplierPort == 0xFFFF ? 0 : (PortMultiplierPort << 4));

  ZeroMem (&Packet, sizeof (EFI_ATA_PASS_THRU_COMMAND_PACKET));
  Packet.Protocol  = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT;
  Packet.Length    = EFI_ATA_PASS_THRU_LENGTH_BYTES | EFI_ATA_PASS_THRU_LENGTH_SECTOR_COUNT;
  Packet.Acb       = &Acb;
  Packet.Asb       = Asb;

  ZeroMem (&SecurityEraseUnit, sizeof (ATA_SECURE_COMMAND_BLOCK));
  SecurityEraseUnit.ControlData = Identifier & BIT0;
  SecurityEraseUnit.ControlData |= (EraseMode << 1) & BIT1;
  CopyMem (SecurityEraseUnit.Password, Password, ATA_PASSWORD_MAX_LENGTH);

  Packet.OutDataBuffer     = &SecurityEraseUnit;
  Packet.OutTransferLength = sizeof (ATA_SECURE_COMMAND_BLOCK);
  Packet.Timeout           = Timeout;

  Status = AtaPassThru->PassThru (
                          AtaPassThru,
                          Port,
                          PortMultiplierPort,
                          &Packet,
                          NULL
                          );
  if (!EFI_ERROR (Status) &&
    ((Asb->AtaStatus & ATA_STSREG_ERR) != 0) &&
    ((Asb->AtaError & ATA_ERRREG_ABRT) != 0))
  {
    DEBUG ((DEBUG_INFO, "AtaStatus = %02x, AtaError = %02x\n", Asb->AtaStatus, Asb->AtaError));
    Status = EFI_DEVICE_ERROR;
  }

  FreeAlignedPages (Asb, EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)));
  DEBUG ((DEBUG_INFO, "%a() - Port = 0x%04x, multiplier = 0x%04x, Status = %r\n", __FUNCTION__, Port, PortMultiplierPort, Status));
  return Status;
}

/**
  Performs every action required to securely erase a given device

  @param[in] AtaDevice                  The pointer to the EFI_ATA_PASS_THRU_PROTOCOL.
  @param[in] Port                       Port number on the ATA controller
  @param[in] PortMultiplierPort         Port multiplier port number on the ATA controller
  @param[in] Identifier                 The identifier to set user or master password.
  @param[in] EraseMode                  Erase mode: normal or enhanced.
  @param[in] Timeout                    Time to wait in 100ns units
  @param[in] Password                   Drive password

  @retval    EFI_SUCCESS
  @retval    EFI_DEVICE_ERROR           The command has failed
  @retval    EFI_INVALID_PARAMETER      Port, PortMultiplierPort, or the contents of Acb are invalid. The ATA
                                        command was not sent, so no additional status information is available.
  @retval    EFI_NOT_READY              The ATA command could not be sent because there are too many ATA commands
                                        already queued. The caller may retry again later.
  @retval    EFI_TIMEOUT                Secure erase command takes longer to complete
                                        then the specified timeout
**/
EFI_STATUS
EFIAPI
SendAtaSecureErase (
  IN EFI_ATA_PASS_THRU_PROTOCOL        *AtaPassThru,
  IN UINT16                            Port,
  IN UINT16                            PortMultiplierPort,
  IN CHAR8                             Identifier,
  IN UINT8                             EraseMode,
  IN UINT64                            Timeout,
  IN CHAR8                             Password[ATA_PASSWORD_MAX_LENGTH]
  )
{
  EFI_STATUS  Status;

  Status = AtaPrepareSecureErase (AtaPassThru, Port, PortMultiplierPort);
  if (!EFI_ERROR (Status)) {
    Status = SendAtaEraseUnit (AtaPassThru, Port, PortMultiplierPort, Identifier, EraseMode, Password, Timeout);
  }

  return Status;
}

/**
  Send unlock ATA password cmd through Ata Pass Thru Protocol.

  @param[in] AtaPassThru         The pointer to the ATA_PASS_THRU protocol.
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.
  @param[in] Identifier          The identifier to set user or master password.
  @param[in] Password            The hdd password of attached ATA device.

  @retval EFI_SUCCESS            Successful to disable hdd password cmd.
  @retval EFI_INVALID_PARAMETER  The parameter passed-in is invalid.
  @retval EFI_OUT_OF_RESOURCES   Not enough memory to disable hdd password cmd.
  @retval EFI_DEVICE_ERROR       Can not disable hdd password cmd.

**/
EFI_STATUS
EFIAPI
UnlockAtaPassword (
  IN EFI_ATA_PASS_THRU_PROTOCOL     *AtaPassThru,
  IN UINT16                         Port,
  IN UINT16                         PortMultiplierPort,
  IN CHAR8                          Identifier,
  IN CHAR8                          Password[ATA_PASSWORD_MAX_LENGTH]
  )
{
  EFI_STATUS                        Status;
  EFI_ATA_COMMAND_BLOCK             Acb;
  EFI_ATA_STATUS_BLOCK              *Asb;
  EFI_ATA_PASS_THRU_COMMAND_PACKET  Packet;
  ATA_SECURE_COMMAND_BLOCK          SecurityUnlock;

  if ((AtaPassThru == NULL) || (Password == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // The 'Asb' field (a pointer to the EFI_ATA_STATUS_BLOCK structure) in
  // EFI_ATA_PASS_THRU_COMMAND_PACKET is required to be aligned specified by
  // the 'IoAlign' field in the EFI_ATA_PASS_THRU_MODE structure. Meanwhile,
  // the structure EFI_ATA_STATUS_BLOCK is composed of only UINT8 fields, so it
  // may not be aligned when allocated on stack for some compilers. Hence, we
  // use the API AllocateAlignedPages to ensure this structure is properly
  // aligned.
  //
  Asb = AllocateAlignedPages (
          EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)),
          AtaPassThru->Mode->IoAlign
          );
  if (Asb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Prepare for ATA command block.
  //
  ZeroMem (&Acb, sizeof (Acb));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));
  Acb.AtaCommand    = ATA_CMD_SECURITY_UNLOCK;
  Acb.AtaDeviceHead = (UINT8) (PortMultiplierPort == 0xFFFF ? 0 : (PortMultiplierPort << 4));

  //
  // Prepare for ATA pass through packet.
  //
  ZeroMem (&Packet, sizeof (Packet));
  Packet.Protocol = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT;
  Packet.Length   = EFI_ATA_PASS_THRU_LENGTH_BYTES;
  Packet.Asb      = Asb;
  Packet.Acb      = &Acb;

  ZeroMem (&SecurityUnlock, sizeof (ATA_SECURE_COMMAND_BLOCK));
  CopyMem (&SecurityUnlock.Password, Password, ATA_PASSWORD_MAX_LENGTH);
  SecurityUnlock.ControlData = Identifier & BIT0;

  Packet.OutDataBuffer     = &SecurityUnlock;
  Packet.OutTransferLength = sizeof (ATA_SECURE_COMMAND_BLOCK);
  Packet.Timeout           = ATA_TIMEOUT;

  Status = AtaPassThru->PassThru (
                          AtaPassThru,
                          Port,
                          PortMultiplierPort,
                          &Packet,
                          NULL
                          );
  if (!EFI_ERROR (Status) &&
    ((Asb->AtaStatus & ATA_STSREG_ERR) != 0) &&
    ((Asb->AtaError & ATA_ERRREG_ABRT) != 0))
  {
    DEBUG ((DEBUG_INFO, "AtaStatus = %02x, AtaError = %02x\n", Asb->AtaStatus, Asb->AtaError));
    Status = EFI_DEVICE_ERROR;
  }

  FreeAlignedPages (Asb, EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)));
  DEBUG ((DEBUG_INFO, "%a() - Port = 0x%04x, multiplier = 0x%04x, Status = %r\n", __FUNCTION__, Port, PortMultiplierPort, Status));
  return Status;
}

/**
  Send Sanitize Block Erase command

  @param[in]       AtaPassThru                The pointer to the EFI_ATA_PASS_THRU_PROTOCOL.
  @param[in]       Port                       Port number on the ATA controller
  @param[in]       PortMultiplierPort         Port multiplier port number on the ATA controller
  @param[in]       Feature                    The feature code of sanitize command
  @param[in]       Lba                        The Lba of the sanitize command

  @retval          EFI_SUCCESS
  @retval          EFI_BAD_BUFFER_SIZE        The ATA command was not executed. The number of bytes that could be transferred
                                              is returned in InTransferLength. For write and bi-directional commands,
                                              OutTransferLength bytes were transferred by OutDataBuffer.
  @retval          EFI_NOT_READY              The ATA command could not be sent because there are too many ATA commands
                                              already queued. The caller may retry again later.
  @retval          EFI_DEVICE_ERROR           A device error occurred while attempting to send the ATA command.
  @retval          EFI_INVALID_PARAMETER      Port, PortMultiplierPort, or the contents of Acb are invalid. The ATA
                                              command was not sent, so no additional status information is available.

**/
EFI_STATUS
EFIAPI
AtaSanitizeCommand (
  IN EFI_ATA_PASS_THRU_PROTOCOL       *AtaPassThru,
  IN UINT16                           Port,
  IN UINT16                           PortMultiplierPort,
  IN UINT16                           Feature,
  IN UINT64                           Lba
  )
{
  EFI_STATUS                          Status;
  EFI_ATA_PASS_THRU_COMMAND_PACKET    Packet;
  EFI_ATA_COMMAND_BLOCK               Acb;
  EFI_ATA_STATUS_BLOCK                *Asb;

  if (AtaPassThru == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // The 'Asb' field (a pointer to the EFI_ATA_STATUS_BLOCK structure) in
  // EFI_ATA_PASS_THRU_COMMAND_PACKET is required to be aligned specified by
  // the 'IoAlign' field in the EFI_ATA_PASS_THRU_MODE structure. Meanwhile,
  // the structure EFI_ATA_STATUS_BLOCK is composed of only UINT8 fields, so it
  // may not be aligned when allocated on stack for some compilers. Hence, we
  // use the API AllocateAlignedPages to ensure this structure is properly
  // aligned.
  //
  Asb = AllocateAlignedPages (
          EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)),
          AtaPassThru->Mode->IoAlign
          );
  if (Asb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "%a: Feature:%4x, Lba:%016lx\n", __FUNCTION__, Feature, Lba));
  ZeroMem (&Acb, sizeof (EFI_ATA_COMMAND_BLOCK));
  ZeroMem (Asb, sizeof (EFI_ATA_STATUS_BLOCK));
  Acb.AtaCommand         = ATA_SANITIZE_CMD;
  Acb.AtaFeatures        = (UINT8) Feature;
  Acb.AtaFeaturesExp     = (UINT8) (Feature >> 8);
  Acb.AtaSectorNumber    = (UINT8) Lba;
  Acb.AtaCylinderLow     = (UINT8) RShiftU64 (Lba, 8);
  Acb.AtaCylinderHigh    = (UINT8) RShiftU64 (Lba, 16);
  Acb.AtaSectorNumberExp = (UINT8) RShiftU64 (Lba, 24);
  Acb.AtaCylinderLowExp  = (UINT8) RShiftU64 (Lba, 32);
  Acb.AtaCylinderHighExp = (UINT8) RShiftU64 (Lba, 40);

  ZeroMem (&Packet, sizeof (EFI_ATA_PASS_THRU_COMMAND_PACKET));
  Packet.Acb             = &Acb;
  Packet.Asb             = Asb;
  Packet.Protocol        = EFI_ATA_PASS_THRU_PROTOCOL_ATA_NON_DATA;
  Packet.Length          = EFI_ATA_PASS_THRU_LENGTH_BYTES | EFI_ATA_PASS_THRU_LENGTH_SECTOR_COUNT;
  Packet.Timeout         = ATA_TIMEOUT;

  Status = AtaPassThru->PassThru (
                          AtaPassThru,
                          Port,
                          PortMultiplierPort,
                          &Packet,
                          NULL
                          );
  if (!EFI_ERROR (Status) &&
      ((Asb->AtaStatus & ATA_STSREG_ERR) != 0) &&
      ((Asb->AtaError & ATA_ERRREG_ABRT) != 0))
  {
    DEBUG ((DEBUG_INFO, "AtaStatus = %02x, AtaError = %02x\n", Asb->AtaStatus, Asb->AtaError));
    Status = EFI_DEVICE_ERROR;
  }

  FreeAlignedPages (Asb, EFI_SIZE_TO_PAGES (sizeof (EFI_ATA_STATUS_BLOCK)));
  DEBUG ((DEBUG_INFO, "%a() - Port = 0x%04x, multiplier = 0x%04x, Status = %r\n", __FUNCTION__, Port, PortMultiplierPort, Status));
  return Status;
}

