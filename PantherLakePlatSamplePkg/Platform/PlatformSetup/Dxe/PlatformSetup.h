/** @file
  Header file for Platform Initialization Driver.

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

#ifndef _PLATFORM_SETUP_DRIVER_H_
#define _PLATFORM_SETUP_DRIVER_H_

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Protocol/MpService.h>
#include <IndustryStandard/Pci22.h>
#include <Protocol/Ps2Policy.h>
#include <Protocol/MemInfo.h>

#include <Protocol/PciIo.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/IdeControllerInit.h>

typedef UINT16  STRING_REF;

#include <Guid/GlobalVariable.h>
#include <Guid/HobList.h>

#include <Protocol/VariableWrite.h>
#include <Library/CpuPlatformLib.h>

#include <Protocol/DxePolicyUpdateProtocol.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <Platform.h>
#include <Configuration.h>
#include <SetupVariable.h>
#include <PlatformBoardId.h>
#include <Library/CpuMailboxLib.h>

//
// Prototypes
//
EFI_STATUS
EFIAPI
PlatformSetupEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

EFI_STATUS
EFIAPI
Ps2InitHardware (
  IN  EFI_HANDLE  Handle
  );

EFI_STATUS
GetStringFromToken (
  IN  EFI_GUID     *ProducerGuid,
  IN  STRING_REF   Token,
  OUT CHAR16       **String
  );

CHAR16 *
GetStringById (
  IN  STRING_REF   Id,
  EFI_HII_HANDLE   StringPackHandle
  );

VOID
SwapEntries (
  IN  CHAR8 *Data
  );

VOID
AsciiToUnicode (
  IN    CHAR8     *AsciiString,
  IN    CHAR16    *UnicodeString
  );

VOID
SetupPlatformPolicies (
  IN OUT  MEM_INFO_PROTOCOL    *MemoryInfoProtocol
  );

//
// Global externs
//
extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *mPciRootBridgeIo;

#endif
