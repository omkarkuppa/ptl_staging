/** @file
  Null instance of Smbios Processor Information Library which produces Smbios type 4 and 7 tables.

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

#include <PiDxe.h>

/**
  Add Smbios Cache information (type 7) table and Processor information (type 4) table
  using the HOB info from Silicon.

  This is a Null instance.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
AddSmbiosProcessorAndCacheTables (
  VOID
  )
{
  return EFI_SUCCESS;
}
