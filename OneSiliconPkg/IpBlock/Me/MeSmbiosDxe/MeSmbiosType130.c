/**@file
  Type 130: AMT-Specific SMBIOS Structure.

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
#include <Base.h>

//
// Structure for AMT-specific SMBIOS type 130 filled with default data.
//
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TABLE_TYPE130 mSmbiosTableType130Data = {
  {
    MESMBIOS_TYPE_130,              // Smbios type
    sizeof (SMBIOS_TABLE_TYPE130),  // Data Length
    0x00                            // handle value is dummy and is filled by Smbios Core driver
  },
  { '$','A','M','T' },              // AMT Signature
  0,                                // AMT Supported?
  0,                                // AMT Enabled?
  0,                                // SRoU Enabled?
  0,                                // Serial Over Lan Enabled?
  0,                                // Network interface of the Intel AMT part is enabled?
  EXTENDED_DATA_RESERVED_BITMASK,   // 0xA5 (for backward compatibility)
  OEM_CAPS_1_RESERVED_BITMASK,      // 1st Byte of OEM capabilities - bits 0-1 reserved (must be 0x01), 2 BIOS Reflash, 3 BIOS Setup, 4 BIOS Pause, 5-7 reserved (must be 0x01)
  0,                                // 2nd Byte of OEM capabilities (Terminal Emulation), 0x02 = VT100+
  0,                                // 3rd Byte of OEM capabilities - bits 0-6 reserved (must be 0x00), 7 BIOS Secure Boot Capability,
  0,                                // 4th Byte of OEM capabilities - bits 0 AMT over 2.5GbE, 1 OCR HTTPS Boot, 2 OCR PBA Boot, 3 OCR WinRe Boot, 4 disable secure boot for AMT, 5 OCR Wifi Profile, 6-7 reserved
  0,                                // Kvm Enabled?
  0,                                // Reserved
  0                                 // RPE Capabilities Dword - bits 0-31 Remote Platform Erase Capabilities.
};


/**
  This function makes creates the ME SMBIOS table 130 which is filled with default values.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.
**/
EFI_STATUS
CreateSmbiosTableType130 (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = AddSmbiosRecord ((VOID*)&mSmbiosTableType130Data, sizeof (SMBIOS_TABLE_TYPE130));
  DEBUG ((DEBUG_INFO, "%a() exit, status = %r\n", __FUNCTION__, Status));

  return Status;
}