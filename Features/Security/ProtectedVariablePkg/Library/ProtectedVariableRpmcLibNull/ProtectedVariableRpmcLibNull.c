/** @file
  NULL ProtectedVariableRpmcLib instance for build purpose.

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

#include <Library/DebugLib.h>
#include <Library/ProtectedVariableRpmcLib.h>

/**
  Requests count from monotonic counter 0.

 @param[out]   CounterValue            A pointer to a buffer to store the RPMC value.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
EFIAPI
ProtectedVariableRequestMonotonicCounter0 (
  OUT UINT32  *CounterValue
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

/**
  Requests count from monotonic counter 1.

 @param[out]   CounterValue            A pointer to a buffer to store the RPMC value.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
EFIAPI
ProtectedVariableRequestMonotonicCounter1 (
  OUT UINT32  *CounterValue
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

/**
  Increments monotonic counter 0 in the SPI flash device by 1.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
EFIAPI
ProtectedVariableIncrementMonotonicCounter0 (
  VOID
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

/**
  Increments monotonic counter 1 in the SPI flash device by 1.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
EFIAPI
ProtectedVariableIncrementMonotonicCounter1 (
  VOID
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}
