/** @file
  Header file for the DxeSaNvsInitLib.

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

@par Specification Reference:
**/
#ifndef _DXE_SA_NVS_INIT_LIB_PTL_CDIE_H_
#define _DXE_SA_NVS_INIT_LIB_PTL_CDIE_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/DxeSaInitFruLib.h>
#include <Library/PeiDxeSmmReserveMmio64SizeLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Register/Intel/Cpuid.h>

/**
  Install SSDT Table

  @retval EFI_SUCCESS - SSDT Table load successful.
**/
EFI_STATUS
EFIAPI
InstallSsdtAcpiTable (
  IN GUID   SsdtTableGuid,
  IN UINT64 Signature
  );

#endif
