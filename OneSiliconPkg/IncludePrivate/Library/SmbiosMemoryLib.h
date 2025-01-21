/** @file
  Prototype of the SmbiosMemory library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
  System Management BIOS (SMBIOS) Reference Specification v2.8.0
  dated 2013-Mar-28 (DSP0134)
  http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.8.0.pdf
**/
#ifndef _SMBIOS_MEMORY_LIB_H_
#define _SMBIOS_MEMORY_LIB_H_

/**
  <b> SMBIOS Memory Information </b>
  This function will determine memory configuration information from the chipset and memory and install SMBIOS table types 16, 17, and 19.
  @note Type 20 is optional as per SMBIOS specification v2.5 and above.
  - <b>Introduction</b> \n
    This module publishes the SMBIOS information.  This module reads the System Agent configuration registers to determine memory configuration, creates the SMBIOS structures and (Native Mode) adds them using the SMBIOS protocol, or (ECP Mode) logs them to the data hub. The data hub structures are later retrieved by the SMBIOS thunk driver for publishing the SMBIOS tables in memory.
    This module is required for systems supporting SMBIOS tables.

  - @pre
    - EFI_SMBUS_HC_PROTOCOL
    - EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL (See UEFI Spec, v2.0)
    - MEM_INFO_PROTOCOL
    - EFI_SMBIOS_PROTOCOL

  - @result
    Publishes SMBIOS data structures related to the System Agent.
     - Publishes using EFI_SMBIOS_PROTOCOL.

  - @warning
    Any modification of this module should ensure properly formatted structures are produced.
    Properly formatted SMBIOS table structures
    It is recommended that all SMBIOS information produced for your platform is reviewed for accuracy.
    No recommendation is provided on how to address issues if any are encountered in the memory structures provided by this module.

  @param[in] SmbiosProtocol     - Instance of Smbios Protocol

  @retval EFI_SUCCESS           - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES  - if not able to get resources.
  @retval EFI_INVALID_PARAMETER - if a required parameter in a subfunction is NULL.
**/
EFI_STATUS
EFIAPI
SmbiosMemory (
  IN  EFI_SMBIOS_PROTOCOL *SmbiosProtocol
  );
#endif // _DXE_SMBIOS_MEMORY_LIB_H_
