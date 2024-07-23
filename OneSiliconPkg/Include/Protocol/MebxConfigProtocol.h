/** @file
  MebxConfigProtocol definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _MEBX_CONFIG_PROTOCOL_H_
#define _MEBX_CONFIG_PROTOCOL_H_

///
/// Mebx Config Protocol
/// The interface function is to set/get MEBX settings.
///

#define MEBX_CONFIG_PROTOCOL_REVISION  1
typedef struct _MEBX_CONFIG_PROTOCOL  MEBX_CONFIG_PROTOCOL;

/**
  This function is used to set MEBx settings.

  @param[in]  DataSize           The size in bytes of the MEBx Data buffer.
  @param[in]  Data               The contents for the MEBx Data.

  @retval EFI_SUCCESS            MEBx config saved.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold MEBx data.
  @retval EFI_DEVICE_ERROR       MEBx config could not be retrieved due to a hardware error.
**/
typedef
EFI_STATUS
(EFIAPI *SET_MEBX_DATA) (
  IN  UINTN                        DataSize,
  IN  VOID                         *Data
  );

/**
  This function is used to retrieve MEBx settings.

  @param[in,out] DataSize        The size in bytes of the MEBx data buffer.
  @param[out]    Data            The pointer to MEBx configuration data buffer.

  @retval EFI_SUCCESS            MEBx config was successfully retrieved.
  @retval EFI_INVALID_PARAMETER  Parameter Data or Datasize is NULL.
  @retval EFI_DEVICE_ERROR       MEBx config could not be retrieved due to a hardware error.
**/
typedef
EFI_STATUS
(EFIAPI *GET_MEBX_DATA) (
  IN OUT  UINTN                    *DataSize,
     OUT  VOID                     *Data
  );

/**
  AMT Mebx Config Protocol
  The interface function is to set/get MEBX settings.
  It is platform vendor's responsibility to implement the function instances.

  <b>Revision 1:</b>
  - Initial version.

**/
struct _MEBX_CONFIG_PROTOCOL {
  UINT8                         Revision;
  SET_MEBX_DATA                 SetMebxConfig;
  GET_MEBX_DATA                 GetMebxConfig;
};

extern EFI_GUID gMebxConfigProtocolGuid;

#endif // _MEBX_CONFIG_PROTOCOL_H_
