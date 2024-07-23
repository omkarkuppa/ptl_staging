/** @file
  Header file for DXE VTD Init Lib.

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
#ifndef _DXE_VTD_INIT_LIB_H_
#define _DXE_VTD_INIT_LIB_H_

#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/CpuPlatformLib.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Pci.h>
#include <VtdDataHob.h>
#include <Library/PchInfoHob.h>
#include <CpuRegs.h>
#include <Register/P2sbRegs.h>
#include <DmaRemappingTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>

/**
  Locate the VT-d ACPI tables data file and read ACPI SSDT tables.
  Publish the appropriate SSDT based on current configuration and capabilities.

  @retval EFI_SUCCESS - Vtd initialization complete
  @retval Other       - No Vtd function initiated
**/
EFI_STATUS
VtdInit (
  );

/**
  EndOfPcieEnum routine for update DMAR
**/
VOID
UpdateDmarEndOfPcieEnum (
  VOID
  );

/**
  This functions installs VT-d topology protocol.
    @see OneSiliconPkg\Include\Protocol\VtdTopologyProtocol.h
**/
VOID
InstallVtdTopologyProtocol (
  VOID
  );
#endif
