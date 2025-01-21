/** @file
  Definitions used by this library implementation.

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

#ifndef PROTECTED_VARIABLE_ENCRYPTION_H_
#define PROTECTED_VARIABLE_ENCRYPTION_H_

#include <Library/ProtectedVariableLib.h>

#define ENC_KEY_SEP        L":"
#define ENC_KEY_SEP_SIZE   2
#define ENC_KEY_NAME       L"VAR_ENC_KEY"
#define ENC_KEY_NAME_SIZE  22

#define ENC_KEY_SIZE    (256/8)
#define ENC_BLOCK_SIZE  AES_BLOCK_SIZE
#define ENC_IVEC_SIZE   ENC_BLOCK_SIZE
#define ENC_SALT_SIZE   64

#define ENC_PADDING_BYTE  0x0F

//
// PKCS#5 padding
//
// #define AES_CIPHER_DATA_SIZE(PlainDataSize)
//  (AES_BLOCK_SIZE + (PlainDataSize)) & (~(AES_BLOCK_SIZE - 1))
//
#define AES_CIPHER_DATA_SIZE(PlainDataSize)  ALIGN_VALUE (PlainDataSize, AES_BLOCK_SIZE)

#define FREE_POOL(Address)      \
    if ((Address) != NULL) {    \
      FreePool (Address);       \
      (Address) = NULL;         \
    }

#pragma pack(1)

typedef struct {
  UINT32    DataType;         // SYM_TYPE_AES
  UINT32    HeaderSize;       // sizeof(VARIABLE_ENCRYPTION_HEADER)
  UINT32    PlainDataSize;    // Plain data size
  UINT32    CipherDataSize;   // Cipher data size
  UINT8     KeyIvec[ENC_IVEC_SIZE];
  UINT8     Salt[ENC_SALT_SIZE];
} VARIABLE_ENCRYPTION_HEADER;

/**
  Encrypt protected variable data.

  If VarEncInfo->PlainData is not NULL, VarEncInfo->PlainData holds the plain
  data. Otherwise, VarEncInfo->Headr.Data is supposed to be the plain data.

  If VarEncInfo->CipherData is not NULL, The encrypted data is stored in
  VarEncInfo->CipherData. Otherwise, the encrypted data is stored directly
  in variable data buffer, i.e. VarEncInfo->Headr.Data.

  @param[in, out]   VarEncInfo   Pointer to structure containing detailed
                                 information about a variable.

  @retval EFI_SUCCESS             Variable was encrypted successfully.
  @retval EFI_INVALID_PARAMETER   Variable information in VarEncInfo is invalid.
  @retval EFI_BUFFER_TOO_SMALL    VarEncInfo->CipherData is not NULL but
                                  VarEncInfo->CipherDataSize is too small.
  @retval EFI_ABORTED             Uknown error occurred during encrypting.
  @retval EFI_OUT_OF_RESOURCES    Fail to allocate enough resource.
  @retval EFI_UNSUPPORTED         Unsupported to encrypt variable.

**/
EFI_STATUS
EFIAPI
EncryptProtectedVariable (
  IN OUT VARIABLE_ENCRYPTION_INFO  *VarEncInfo
  );

/**
  Decrypt protected variable data.

  If VarEncInfo->CipherData is not NULL, it must holds the cipher data to be
  decrypted. Otherwise, assume the cipher data from variable data buffer, i.e.
  VarEncInfo->Header.Data.

  If VarEncInfo->Flags.DecryptInPlace is TRUE, the decrypted data will be put
  back in the same buffer as cipher buffer got above, after encryption header,
  which helps to identify later if the data in buffer is decrypted or not. This
  can avoid repeat decryption when accessing the same variable more than once.

  If VarEncInfo->Flags.DecryptInPlace is FALSE, VarEncInfo->PlainData must be
  passed in with a valid buffer with VarEncInfo->PlainDataSize set correctly
  with its size.

  Note the VarEncInfo->PlainData is always pointing to the buffer address with
  decrypted data without encryption header, and VarEncInfo->PlainDataSize is
  always the size of original variable data, if this function returned
  successfully.

  @param[in, out]   VarEncInfo   Pointer to structure containing detailed
                                 information about a variable.

  @retval EFI_SUCCESS             Variable was decrypted successfully.
  @retval EFI_INVALID_PARAMETER   Variable information in VarEncInfo is invalid.
  @retval EFI_BUFFER_TOO_SMALL    VarEncInfo->PlainData is not NULL but
                                  VarEncInfo->PlainDataSize is too small.
  @retval EFI_ABORTED             Uknown error occurred during decrypting.
  @retval EFI_OUT_OF_RESOURCES    Fail to allocate enough resource.
  @retval EFI_COMPROMISED_DATA    The cipher header is not valid.
  @retval EFI_UNSUPPORTED         Unsupported to encrypt variable.

**/
EFI_STATUS
EFIAPI
DecryptProtectedVariable (
  IN OUT VARIABLE_ENCRYPTION_INFO  *VarEncInfo
  );

/**
  Get cipher information about a variable, including plaindata size,
  cipher algorithm type, etc.

  For data passed in with VarEncInfo,

    VarEncInfo->Header.Data
      - The variable data in normal variable structure.
    VarEncInfo->Header.DataSize
      - The size of variable data.

  For data passed out with VarEncInfo (valid only if EFI_SUCCESS is returned),

    VarEncInfo->CipherDataType
      - ENC_TYPE_NULL, if the variable is not encrypted or has been decrypted;
      - ENC_TYPE_AES, if the variable is encrypted.
    VarEncInfo->CipherHeaderSize
      - Size of cipher header put before encrypted or decrypted data.
    VarEncInfo->PlainData
      - NULL, if the variable is encrypted; Or
      - pointer to original variable data, if the variable has been decrypted.
    VarEncInfo->PlainDataSize
      - The size of original variable data
    VarEncInfo->CipherData
      - NULL, if the variable is decrypted; Or
      - pointer to start of encrypted variable data, including encryption header;
    VarEncInfo->CipherDataSize
      - The size of encrypted variable data, including encryption header.

  @param[in]   VarEncInfo   Pointer to structure containing detailed
                            information about a variable.

  @retval EFI_SUCCESS             The information was retrieved successfully.
  @retval EFI_INVALID_PARAMETER   Variable information in VarEncInfo is invalid.
  @retval EFI_NOT_FOUND           No cipher information recognized.
  @retval EFI_UNSUPPORTED         Unsupported interface.

**/
EFI_STATUS
EFIAPI
GetCipherDataInfo (
  IN VARIABLE_ENCRYPTION_INFO  *VarEncInfo
  );

/**
  Force set cipher information for a variable, like plaindata size,
  cipher algorithm type, cipher data etc.

  The destination buffer must be passed via VarEncInfo->Header.Data.

  This method is only used to update and/or change plain data information.

  @param[in]   VarEncInfo   Pointer to structure containing detailed
                            information about a variable.

  @retval EFI_SUCCESS             The information was updated successfully.
  @retval EFI_INVALID_PARAMETER   Variable information in VarEncInfo is invalid.
  @retval EFI_UNSUPPORTED         If this method is not supported.

**/
EFI_STATUS
EFIAPI
SetCipherDataInfo (
  IN VARIABLE_ENCRYPTION_INFO  *VarEncInfo
  );
#pragma pack()

#endif // _PROTECTED_VARIABLE_ENCRYPTION_H_
