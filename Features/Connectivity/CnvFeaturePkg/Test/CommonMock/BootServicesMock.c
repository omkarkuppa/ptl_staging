/**@file
  Boot Services mock.

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
#include <Library/MemoryAllocationLib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/UnitTestLib.h>

/**
  Mock of Allocates memory pages.

  @param[in]       Type         The type of allocation to perform.
  @param[in]       MemoryType   The type of memory to allocate.
                                MemoryType values in the range 0x70000000..0x7FFFFFFF
                                are reserved for OEM use. MemoryType values in the range
                                0x80000000..0xFFFFFFFF are reserved for use by UEFI OS loaders
                                that are provided by operating system vendors.
  @param[in]       Pages        The number of contiguous 4 KB pages to allocate.
  @param[in, out]  Memory       The pointer to a physical address. On input, the way in which the address is
                                used depends on the value of Type.

  @retval EFI_SUCCESS           The requested pages were allocated.
  @retval EFI_INVALID_PARAMETER 1) Type is not AllocateAnyPages or
                                AllocateMaxAddress or AllocateAddress.
                                2) MemoryType is in the range
                                EfiMaxMemoryType..0x6FFFFFFF.
                                3) Memory is NULL.
                                4) MemoryType is EfiPersistentMemory.
  @retval EFI_OUT_OF_RESOURCES  The pages could not be allocated.
  @retval EFI_NOT_FOUND         The requested pages could not be found.

**/
EFI_STATUS
EFIAPI
MockBootServiceAllocatePages (
  IN EFI_ALLOCATE_TYPE         Type,
  IN EFI_MEMORY_TYPE           MemoryType,
  IN UINTN                     Pages,
  IN OUT EFI_PHYSICAL_ADDRESS  *Memory
  )
{
  EFI_STATUS  Status;

  *Memory = mock_type (EFI_PHYSICAL_ADDRESS);
  Pages   = mock_type (UINTN);
  Status  = mock_type (EFI_STATUS);

  if ((Status == EFI_SUCCESS) && (Pages > 0) && (Memory != NULL)) {
    Memory = AllocatePool (sizeof (EFI_PHYSICAL_ADDRESS));
    if (Memory == NULL) {
      return EFI_INVALID_PARAMETER;
    }
  }

  return Status;
}

/**
  Mock of InstallMultipleProtocolInterfaces.

  @param[in, out]  Handle       The pointer to a handle to install the new protocol interfaces on,
                                or a pointer to NULL if a new handle is to be allocated.
  @param  ...                   A variable argument list containing pairs of protocol GUIDs and protocol
                                interfaces.

  @retval EFI_SUCCESS           All the protocol interface was installed.
  @retval EFI_OUT_OF_RESOURCES  There was not enough memory in pool to install all the protocols.
  @retval EFI_ALREADY_STARTED   A Device Path Protocol instance was passed in that is already present in
                                the handle database.
  @retval EFI_INVALID_PARAMETER Handle is NULL.
  @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle.

**/
EFI_STATUS
EFIAPI
MockInstallMultipleProtocolInterfaces (
  IN OUT EFI_HANDLE  *Handle,
  ...
  )
{
  return mock_type (EFI_STATUS);
}

/**
  Mock LocateProtocol: returns the first protocol instance that matches the given protocol.

  @param[in]  Protocol          Provides the protocol to search for.
  @param[in]  Registration      Optional registration key returned from
                                RegisterProtocolNotify().
  @param[out]  Interface        On return, a pointer to the first interface that matches Protocol and
                                Registration.

  @retval EFI_SUCCESS           A protocol instance matching Protocol was found and returned in
                                Interface.
  @retval EFI_NOT_FOUND         No protocol instances were found that match Protocol and
                                Registration.
  @retval EFI_INVALID_PARAMETER Interface is NULL.
                                Protocol is NULL.

**/
EFI_STATUS
EFIAPI
MockLocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID      **Interface
  )
{
  check_expected (Protocol);
  *Interface = mock_type (VOID *);

  return mock_type (EFI_STATUS);
}
