/** @file
  Mock instance of MtrrLib library class.

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

@par Specification
**/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/MtrrLib.h>
#include <Library/BaseMemoryLib.h>

/**
  Returns the variable MTRR count for the CPU.

  @return Variable MTRR count
**/
UINT32
EFIAPI
GetVariableMtrrCount (
  VOID
  )
{
  return mock_type (UINT32);
}

/**
  This function will get the memory cache type of the specific address.
  This function is mainly for debug purpose.

  @param[in]  Address  The specific address

  @return Memory cache type of the specific address
**/
MTRR_MEMORY_CACHE_TYPE
EFIAPI
MtrrGetMemoryAttribute (
  IN PHYSICAL_ADDRESS  Address
  )
{
  check_expected (Address);

  return mock_type (MTRR_MEMORY_CACHE_TYPE);
}

/**
  This function attempts to set the attributes into MTRR setting buffer for a memory range.

  @param[in, out]  MtrrSetting  MTRR setting buffer to be set.
  @param[in]       BaseAddress  The physical address that is the start address of a memory range.
  @param[in]       Length       The size in bytes of the memory range.
  @param[in]       Attribute    The bit mask of attributes to set for the memory range.

  @retval RETURN_SUCCESS            - The attributes were set for the memory range.
  @retval RETURN_INVALID_PARAMETER  - Length is zero.
  @retval RETURN_UNSUPPORTED        - The processor does not support one or more bytes of the
                                      memory resource range specified by BaseAddress and Length.
  @retval RETURN_UNSUPPORTED        - The bit mask of attributes is not support for the memory resource
                                      range specified by BaseAddress and Length.
  @retval RETURN_ACCESS_DENIED      - The attributes for the memory resource range specified by
                                      BaseAddress and Length cannot be modified.
  @retval RETURN_OUT_OF_RESOURCES   - There are not enough system resources to modify the attributes of
                                      the memory resource range.
                                      Multiple memory range attributes setting by calling this API multiple
                                      times may fail with status RETURN_OUT_OF_RESOURCES. It may not mean
                                      the number of CPU MTRRs are too small to set such memory attributes.
                                      Pass the multiple memory range attributes to one call of
                                      MtrrSetMemoryAttributesInMtrrSettings() may succeed.
  @retval RETURN_BUFFER_TOO_SMALL   - The fixed internal scratch buffer is too small for MTRR calculation.
                                      Caller should use MtrrSetMemoryAttributesInMtrrSettings() to specify
                                      external scratch buffer.
**/
RETURN_STATUS
EFIAPI
MtrrSetMemoryAttributeInMtrrSettings (
  IN OUT MTRR_SETTINGS       *MtrrSetting,
  IN PHYSICAL_ADDRESS        BaseAddress,
  IN UINT64                  Length,
  IN MTRR_MEMORY_CACHE_TYPE  Attribute
  )
{
  check_expected_ptr (MtrrSetting);
  check_expected (BaseAddress);
  check_expected (Length);
  check_expected (Attribute);

  CopyMem (MtrrSetting, mock_ptr_type (MTRR_SETTINGS*), sizeof (MTRR_SETTINGS));

  return mock_type (RETURN_STATUS);
}

/**
  This function gets the content in all MTRRs (variable and fixed)

  @param[out]  MtrrSetting  A buffer to hold all MTRRs content.

  @retval the pointer of MtrrSetting
**/
MTRR_SETTINGS *
EFIAPI
MtrrGetAllMtrrs (
  OUT MTRR_SETTINGS  *MtrrSetting
  )
{
  CopyMem (MtrrSetting, mock_ptr_type (MTRR_SETTINGS*), sizeof (MTRR_SETTINGS));

  return MtrrSetting;
}
