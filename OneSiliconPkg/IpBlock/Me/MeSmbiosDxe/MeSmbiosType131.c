/**@file
  Type 131: ME SMBIOS Structure.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include "MeSmbiosInternal.h"

//
// Structure for ME SMBIOS type 131 filled with default data.
//
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TABLE_TYPE131 mSmbiosTableType131Data = {
  {
    MESMBIOS_TYPE_131,            // Smbios type
    sizeof (SMBIOS_TABLE_TYPE131),  // Data Length
    0x00                          // handle value is dummy and is filled by Smbios Core driver
  },
  { 0 },                          // Cpu Capabilities
  { 0 },                          // Reserved
  { 0 },                          // Pch Capabilities
  { 0 },                          // Me Capabilities
  { 0 },                          // Me Fw Configured
  { 0 },                          // Network Device
  { 0 },                          // Bios Capabilities
  { 'v','P','r','o' },            // Structure Identifier
  0                               // Reserved
};

/**
  This function makes creates the ME SMBIOS table 131 which is filled with default values.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.
**/
EFI_STATUS
CreateSmbiosTableType131 (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = AddSmbiosRecord ((VOID*)&mSmbiosTableType131Data, sizeof (SMBIOS_TABLE_TYPE131));
  DEBUG ((DEBUG_INFO, "%a() exit, status = %r\n", __FUNCTION__, Status));

  return Status;
}