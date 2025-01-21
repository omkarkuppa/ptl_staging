/** @file
  Null version instance of the Register Address Translation library class.

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

#include <Library/DebugLib.h>
#include <Library/RegisterAddressTranslationLib.h>

/**
  Returns the base address of the requested endpoint instance in given register
  access space.

  @param[in] Socket     Socket ID or index
  @param[in] Id         Endpoint ID within SoC
  @param[in] Instance   Instance of endpoint given by Id
  @param[in] Type       Type of Register access space

  @retval               NULL if register address could not be determined,
                        valid address otherwise
**/
VOID*
EFIAPI
GetIpBaseAddress (
  IN UINT8                  Socket,
  IN UINT64                 Id,
  IN UINT16                 Instance,
  IN REGISTER_SPACE_TYPE    Type
  )
{
  ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
  return NULL;
}

/**
  Reads the value of the requested register within given endpoint instance.

  @param[in] Socket     Socket ID or index
  @param[in] Id         Endpoint ID within SoC
  @param[in] Instance   Instance of endpoint given by Id
  @param[in] Type       Register access space type
  @param[in] Register   Register to read from
  @param[in] Size       Byte width of register

  @retval             - Value read from register
**/
UINT64
EFIAPI
ReadIpRegister (
  IN UINT8                Socket,
  IN UINT64               Id,
  IN UINT16               Instance,
  IN REGISTER_SPACE_TYPE  Type,
  IN UINT32               Register,
  IN REGISTER_WIDTH_BYTES Size
  )
{
  ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
  return (UINT64)-1;
}

/**
  Writes a value to the requested register within given endpoint instance.

  @param[in] Socket     Socket ID or index
  @param[in] Id         Endpoint ID within SoC
  @param[in] Instance   Instance of endpoint given by Id
  @param[in] Type       Register access space type
  @param[in] Register   Register to write to
  @param[in] Value      Value to be written
  @param[in] Size       Byte width of register

  @retval               None
**/
VOID
EFIAPI
WriteIpRegister (
  IN UINT8                Socket,
  IN UINT64               Id,
  IN UINT16               Instance,
  IN REGISTER_SPACE_TYPE  Type,
  IN UINT32               Register,
  IN UINT64               Value,
  IN REGISTER_WIDTH_BYTES Size
  )
{
  ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
}

