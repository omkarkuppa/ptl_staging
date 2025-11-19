/** @file
  Boot services mock for the Smbios misc dxe driver.
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
#include "BootServicesMock.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
/**
  Returns the first protocol instance that matches the given protocol.

  @param[in]   Protocol          Provides the protocol to search for.
  @param[in]   Registration      Optional registration key returned from
                                 RegisterProtocolNotify().
  @param[out]  Interface         On return, a pointer to the first interface that matches Protocol and
                                 Registration.

  @retval EFI_SUCCESS            A protocol instance matching Protocol was found and returned in
                                 Interface.
  @retval EFI_NOT_FOUND          No protocol instances were found that match Protocol and
                                 Registration.
  @retval EFI_INVALID_PARAMETER  Interface is NULL.
                                 Protocol is NULL.

**/
EFI_STATUS
EFIAPI
MockBootServiceLocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID      **Interface
  )
{
  check_expected (Protocol);

  *Interface = mock_type (VOID *);

  return mock_type (EFI_STATUS);
}

/**
  Closes an event and frees the event structure.

  @param  UserEvent              Event to close

  @retval EFI_INVALID_PARAMETER  Parameters are not valid.
  @retval EFI_SUCCESS            The event has been closed

**/
EFI_STATUS
EFIAPI
MockBootServiceCloseEvent(
  IN EFI_EVENT    UserEvent
 )
{
  return mock_type (EFI_STATUS);
}