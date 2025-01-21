/** @file

  I2C Platform Secure Storage Protocol.

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

@par Specification Reference:
**/

#ifndef _I2C_PSS_PROTOCOL_H_
#define _I2C_PSS_PROTOCOL_H_

typedef struct _I2C_PLATFORM_SECURE_STORAGE_PROTOCOL I2C_PLATFORM_SECURE_STORAGE_PROTOCOL;
//
// Extern the GUID for protocol users.
//
extern EFI_GUID gI2cPssProtocolGuid;

/**
  I2c Pss Protocol revision number
**/
#define I2C_PLATFORM_SECURE_STORAGE_PROTOCOL_REVISION 1
#define PSS_SN_LENGTH                                 16

/**
  Get 16 bytes Serial Number from PSS chip.

  @param[in]       *This              Pointer of I2C_PLATFORM_SECURE_STORAGE_PROTOCOL.
  @param[in, out]  Buffer             Data to store Serial Number from PSS chip

  @retval  EFI_SUCCESS                Get Serial Number from PSS chip successfully.
  @retval  EFI_INVALID_PARAMETER      Buffer or Size exists unexpected value.
  @retval  Other                      PSS chip detect failure.
**/
typedef
EFI_STATUS
(EFIAPI *I2C_GET_PLATFORM_SECURE_STORAGE_SERIAL_NUMBER) (
  IN     I2C_PLATFORM_SECURE_STORAGE_PROTOCOL *This,
  IN OUT UINT8                                *Buffer
  );

/**
  Get 16 bytes Mac Address from PSS chip.
  @param[in]       *This              Pointer of I2C_PLATFORM_SECURE_STORAGE_PROTOCOL.
  @param[in, out]  Buffer             Data to store Mac Address from PSS chip
  @retval  EFI_SUCCESS                Get Mac Address from PSS chip successfully.
  @retval  EFI_INVALID_PARAMETER      Buffer or Size exists unexpected value.
  @retval  Other                      PSS chip detect failure.
**/
typedef
EFI_STATUS
(EFIAPI *I2C_GET_PLATFORM_PSS_MAC_ADDRESS) (
  IN     I2C_PLATFORM_SECURE_STORAGE_PROTOCOL *This,
  IN OUT UINT16                                *Buffer
  );

typedef struct _I2C_PLATFORM_SECURE_STORAGE_PROTOCOL {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT32                                            Revision;
  I2C_GET_PLATFORM_SECURE_STORAGE_SERIAL_NUMBER     I2cPssGetSerialNumber;
  I2C_GET_PLATFORM_PSS_MAC_ADDRESS                  PssGetMacAddress;
} I2C_PLATFORM_SECURE_STORAGE_PROTOCOL;

#endif
