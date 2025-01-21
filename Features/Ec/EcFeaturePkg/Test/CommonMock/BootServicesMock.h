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

#ifndef UEFI_BOOT_SERVICES_TABLE_LIB_MOCK_H
#define UEFI_BOOT_SERVICES_TABLE_LIB_MOCK_H

#include <Uefi.h>
/**
  Mock of Boot Services for LocateProtocol.
  @param[in]   Protocol          The protocol to search for
  @param[in]   Registration      Optional Registration Key returned from
                                 RegisterProtocolNotify()
  @param[out]  Interface         Return the Protocol interface (instance).

  @retval EFI_SUCCESS            If a valid Interface is returned
  @retval EFI_INVALID_PARAMETER  Invalid parameter
  @retval EFI_NOT_FOUND          Protocol interface not found
**/
EFI_STATUS
EFIAPI
MockBootServiceLocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID      **Interface
  );

/**
  Mock of Boot Services for LocateHandleBuffer.
  @param[in]      SearchType       Specifies which handle(s) are to be returned.
  @param[in]      Protocol         Specifies the protocol to search by.
  @param[in]      SearchKey        Specifies the search key.
  @param[in,out]  NumberHandles    On input, the size in bytes of Buffer. On output, the size in bytes of
                                   the array returned in Buffer (if the buffer was large enough) or the
                                   size, in bytes, of the buffer needed to obtain the array (if the buffer was
                                   not large enough).
  @param[out]     Buffer           The buffer in which the array is returned.
  @retval EFI_SUCCESS              The result array of handles was returned.
  @retval EFI_NOT_FOUND            No handles match the search.
  @retval EFI_OUT_OF_RESOURCES     There is not enough pool memory to store the
                                   matching results.
  @retval EFI_INVALID_PARAMETER    One or more parameters are not valid.
**/
EFI_STATUS
EFIAPI
MockBootServiceLocateHandleBuffer (
  IN EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN EFI_GUID                *Protocol OPTIONAL,
  IN VOID                    *SearchKey OPTIONAL,
  IN OUT UINTN               *NumberHandles,
  OUT EFI_HANDLE             **Buffer
  );

/**
  Mock of Boot Services for HandleProtocol.
  @param[in]  UserHandle             The handle being queried.
  @param[in]  Protocol               The published unique identifier of the protocol.
  @param[out]  Interface             Supplies the address where a pointer to the
                                     corresponding Protocol Interface is returned.
  @retval EFI_SUCCESS                The interface information for the specified protocol was returned.
  @retval EFI_UNSUPPORTED            The device does not support the specified protocol.
  @retval EFI_INVALID_PARAMETER      Handle, Protocol, Interface is NULL.
**/
EFI_STATUS
EFIAPI
MockBootServiceHandleProtocol (
  IN EFI_HANDLE  UserHandle,
  IN EFI_GUID    *Protocol,
  OUT VOID       **Interface
  );

/**
  Mock of Boot Services for OpenProtocol.
  @param[in]      UserHandle         The handle to obtain the protocol interface on
  @param[in]      Protocol           The ID of the protocol
  @param[out]     Interface          The location to return the protocol interface
  @param[in]      ImageHandle        The handle of the Image that is opening the
                                     protocol interface specified by Protocol and
                                     Interface.
  @param[in]      ControllerHandle   The controller handle that is requiring this
                                     interface.
  @param[in]      Attributes         The open mode of the protocol interface
                                     specified by Handle and Protocol.
  @retval EFI_INVALID_PARAMETER      Protocol is NULL.
  @retval EFI_SUCCESS                Get the protocol interface.
**/
EFI_STATUS
EFIAPI
MockBootServiceOpenProtocol (
  IN  EFI_HANDLE  UserHandle,
  IN  EFI_GUID    *Protocol,
  OUT VOID        **Interface OPTIONAL,
  IN  EFI_HANDLE  ImageHandle,
  IN  EFI_HANDLE  ControllerHandle,
  IN  UINT32      Attributes
  );


/**
  Mock of Boot Services for InstallProtocol.

  @param  UserHandle             The handle to install the protocol handler on,
                                 or NULL if a new handle is to be allocated
  @param  Protocol               The protocol to add to the handle
  @param  InterfaceType          Indicates whether Interface is supplied in
                                 native form.
  @param  Interface              The interface for the protocol being added

  @return Status code

**/
EFI_STATUS
EFIAPI
MockBootServiceInstallProtocolInterface (
  IN OUT EFI_HANDLE      *UserHandle,
  IN EFI_GUID            *Protocol,
  IN EFI_INTERFACE_TYPE  InterfaceType,
  IN VOID                *Interface
  );

#endif
