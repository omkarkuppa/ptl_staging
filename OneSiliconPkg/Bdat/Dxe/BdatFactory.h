/** @file
  Copies the memory related timing and configuration information into the
  Compatible BIOS data (BDAT) table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef _BdatRmtHandler_h_
#define _BdatRmtHandler_h_

#include <Library/MemoryAllocationLib.h>

#ifndef MCHBAR_OFFSET
#define MCHBAR_OFFSET                   0x48
#endif

#define ACPI_BDAT_OEM_REV         0x00001000
#define EFI_BDAT_ACPI_POINTER     0x0

#define BDAT_PRIMARY_VERSION      4  ///< The BDAT structure that is currently supported.
#define BDAT_SECONDARY_VERSION    0  ///< The BDAT structure that is currently supported.
#define OEM_OFFSET                0  ///< The current offset to the OEM data in the BDAT structure.

#endif /// _BdatRmtHandler_h_
