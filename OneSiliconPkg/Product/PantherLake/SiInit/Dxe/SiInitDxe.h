/** @file
  Header file for Silicon Init DXE.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _SI_INIT_DXE_MODULE_H_
#define _SI_INIT_DXE_MODULE_H_
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/EventGroup.h>
#include <Protocol/Smbios.h>
#include <Protocol/SiPolicyProtocol.h>
#include <Library/SmbiosMemoryLib.h>
#include <Library/SiFviLib.h>
#include <Library/IGpuDxeOpRegionInitLib.h>
#include <Library/IGpuDxeInitLib.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Library/DxeHybridGraphicsInitLib.h>
#include <Library/DxeHostBridgeInitLib.h>
#include <Library/DxeTseLib.h>
#include <Library/DxeIpuInit.h>
#include <Library/DxeVtdInitLib.h>
#include <Library/DxeSaNvsInitLib.h>
#include <Library/PchInfoLib.h>
#if FixedPcdGetBool(PcdTcssSupport) == 1
#include <Library/DxeTcssInit.h>
#include <Register/CpuUsbRegs.h>
#endif
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/AslUpdateLib.h>
#include <HostBridgeDataHob.h>
#include <Library/DxeVmdInitLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/DxeSaInitFruLib.h>
#include <Library/PeiDxeSmmReserveMmio64SizeLib.h>
#include <CMemoryAddressDecodeEncodeLib.h>
#include <Register/Cpuid.h>

/**
  Entry point of the incompatible pci device support code. Setup an incompatible device list template
  and install EFI Incompatible PCI Device Support protocol.

  @param ImageHandle             A handle for the image that is initializing this driver.
  @param SystemTable             A pointer to the EFI system table.

  @retval EFI_SUCCESS            Installed EFI Incompatible PCI Device Support Protocol successfully.
  @retval others                 Failed to install protocol.

**/
EFI_STATUS
EFIAPI
IncompatiblePciDeviceSupport (
  VOID
  );

/**
  Install TopSwapControlSupport protocol to provide Set/Get TopSwap functions.

  It is caller's responsiblity to ensure the accessibility of TopSwap in DXE phase.
  This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)

  @param ImageHandle             A handle for the image that is initializing this driver.
  @param SystemTable             A pointer to the EFI system table.

  @retval EFI_SUCCESS            Installed EFI Incompatible PCI Device Support Protocol successfully.
  @retval others                 Failed to install protocol.

**/
EFI_STATUS
EFIAPI
TopSwapControlSupport (
  VOID
  );

#endif // _SI_INIT_DXE_MODULE_H_
