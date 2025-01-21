/** @file
  Library for AHCI Command

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

#ifndef _AHCI_COMMAND_LIB_H_
#define _AHCI_COMMAND_LIB_H_

#include <Uefi.h>
#include <IndustryStandard/Atapi.h>
#include <Protocol/AtaPassThru.h>

///
/// Ata IDENTIFY DEVICE data, word 128, security related flags
///
#define ATA_SECURITY_STATUS_LEVEL                       ((UINT16)BIT8)
#define ATA_SECURITY_STATUS_ERASE_BIT                   ((UINT16)BIT5)
#define ATA_SECURITY_STATUS_COUNT_EXPIRED_BIT           ((UINT16)BIT4)
#define ATA_SECURITY_STATUS_FROZEN_BIT                  ((UINT16)BIT3)
#define ATA_SECURITY_STATUS_LOCKED_BIT                  ((UINT16)BIT2)
#define ATA_SECURITY_STATUS_ENABLED_BIT                 ((UINT16)BIT1)
#define ATA_SECURITY_STATUS_FEATURE_SET_BIT             ((UINT16)BIT0)
///
/// Ata IDENTIFY DEVICE data, word 59, Sanitize related flags
///
#define ATA_SANITIZE_FEATURE_SET_BIT                    ((UINT16)BIT12)
#define ATA_SANITIZE_CRYPTO_SUPPORT_BIT                 ((UINT16)BIT13)
#define ATA_SANITIZE_OVERWRITE_SUPPORT_BIT              ((UINT16)BIT14)
#define ATA_SANITIZE_BLOCKERASE_SUPPORT_BIT             ((UINT16)BIT15)
///
/// Ata SECURITY Feature output data
///
#define ATA_SECURITY_ERASE_IDENTIFY_USER_PASSWORD       0
#define ATA_SECURITY_ERASE_IDENTIFY_MASTER_PASSWORD     1
#define ATA_SECURITY_ERASE_MODE_NORMAL                  0
#define ATA_SECURITY_ERASE_MODE_ENHANCED                1
#define ATA_SECURITY_ERASE_LEVEL_HIGH                   0
#define ATA_SECURITY_ERASE_LEVEL_MAXIMUM                1
///
/// Ata SANITIZE Feature Command input
///
#define ATA_SANITIZE_CMD                                0xB4
#define ATA_SANITIZE_FEATURE_BLOCKERASE                 0x12
#define ATA_SANITIZE_FEATURE_CRYPTO                     0x11
#define ATA_SANITIZE_FEATURE_OVERWRITE                  0x14
#define ATA_SANITIZE_LBA_BLOCKERASE                     0x426B4572
#define ATA_SANITIZE_LBA_CRYPTO                         0x43727970
#define ATA_SANITIZE_OVERWRITE_PATTER                   0x4F57

#define ATA_PASSWORD_MAX_LENGTH        32

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
  );

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
  );

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
  );

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
  );

/**
  Send ATA secure erase command and wait for completion for a specified timeout

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
  );

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
  );

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
  );

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
  );
#endif  // _AHCI_COMMAND_LIB_H_
