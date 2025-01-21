/** @file
  Header file for IGpu Dxe Init Lib.

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

@par Specification Reference:
**/
#ifndef _IGPU_DXE_INIT_LIB_H_
#define _IGPU_DXE_INIT_LIB_H_

#include <Uefi.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/IGpuPolicy.h>
#include <Protocol/GopComponentName2.h>
#include <SiConfigHob.h>
#include <Defines/HostBridgeDefines.h>
#include <Defines/IgdDefines.h>

/**
  Initialize GT ACPI tables

  @param[in] IGpuPolicy    - IGPU DXE Policy protocol

  @retval EFI_SUCCESS          - GT ACPI initialization complete
  @retval EFI_NOT_FOUND        - Dxe System Table not found.
  @retval EFI_OUT_OF_RESOURCES - Mmio not allocated successfully.
**/
EFI_STATUS
IGpuDxeInit (
  IN IGPU_POLICY_PROTOCOL     *IGpuPolicy
  );

/**
  IGPU initialization on PCIe Enum complete like GopDriver version init etc.

  @retval EFI_SUCCESS          Either IGD is not present or function executed successfully.
**/
EFI_STATUS
IGpuInitOnPciEnumComplete (
  VOID
  );

/**
Save IGPU BAR for S3

**/
VOID
IGpuSaveBarForS3 (
  VOID
  );

/**
  Update Igd NVS Area
  @param[in] IGpuPolicy -        IGPU DXE Policy protocol
**/
VOID
IGpuUpdateNvsArea (
  IN IGPU_POLICY_PROTOCOL      *IGpuPolicy
  );

/**
  Install IGPU NVS protocol
**/
VOID
IGpuInstallNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  );

/**
  This function initializes SA Nvs Area operation region.
  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
EFIAPI
IGpuPatchNvsAreaAddress (
  VOID
  );

#endif
