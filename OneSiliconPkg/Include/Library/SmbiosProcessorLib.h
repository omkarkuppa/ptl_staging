/** @file
  Header file for SmbiosProcessorLib implementation

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

#ifndef _SMBIOS_PROCESSOR_LIB_H_
#define _SMBIOS_PROCESSOR_LIB_H_

/**
  Add Smbios Cache information (type 7) table and Processor information (type 4) table
  using the HOB info from Silicon.

  @retval EFI_SUCCESS          - Successfully installed SMBIOS cache information and processor information.
**/
EFI_STATUS
EFIAPI
AddSmbiosProcessorAndCacheTables (
  VOID
  );

#endif // _SMBIOS_PROCESSOR_LIB_H_
