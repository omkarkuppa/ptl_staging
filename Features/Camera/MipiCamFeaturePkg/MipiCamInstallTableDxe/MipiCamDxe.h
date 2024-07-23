/** @file
  Header file for the MIPI Camera driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _MIPICAM_H_
#define _MIPICAM_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/AcpiTable.h>
#include <Library/MemoryAllocationLib.h>
#include <PiDxe.h>
#include <Protocol/FirmwareVolume2.h>

EFI_STATUS
EFIAPI
InitializeMipiCam (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  );

EFI_STATUS
EFIAPI
LoadAcpiTables(
  VOID
  );
#endif
