/** @file
  This is an implementation of the Advanced ACPI driver.

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

#ifndef _ACPI_PLATFORM_H_
#define _ACPI_PLATFORM_H_

//
// Statements that include other header files
//

#include <IndustryStandard/Acpi.h>
#include <Guid/EventGroup.h>
#include <Guid/GlobalVariable.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/MpService.h>
#include <Register/Cpuid.h>
#include <Protocol/PciIo.h>
#include <Library/PchCycleDecodingLib.h>

#include <IndustryStandard/HighPrecisionEventTimerTable.h>
#include <Library/DxeAcpiGnvsInitLib.h>

#include <Register/PchRegs.h>
#include <Defines/HostBridgeDefines.h>

//
// Produced protocols
//
#include <Protocol/PlatformNvsArea.h>

#include <Platform.h>
#include <Library/AslUpdateLib.h>
#include <SetupVariable.h>
#include <Library/EcMiscLib.h>

#include <Acpi/AcpiTables/Fadt/BoardFadtConfig.h>


//
// ACPI table information used to initialize tables.
//

#define KBC_DATA_PORT             0x60
#define KBC_CMD_STS_PORT          0x64

#define ENABLE_CMD                0xf4
#define WRITE_AUX_DEV             0xd4

#define PS2_ACK                   0xfa
#define PS2_RESEND                0xfe

#define KBC_INPB                  0x02
#define KBC_OUTB                  0x01
#define KBC_AUXB                  0x20

#define TIMEOUT                   50000
#define BAT_TIMEOUT               5000

#define IO_EXPANDER_DISABLED      0xFF

//
// Function definitions
//
EFI_STATUS
LocateSupportProtocol (
  IN     EFI_GUID                      *Protocol,
  IN     EFI_GUID                      gEfiAcpiMultiTableStorageGuid,
  OUT    VOID                          **Instance,
  IN     BOOLEAN                       Type
  );

/**
  Initilize the EC base on Setup, SaSetup, CpuSetup value.
  And update platform Setup base on the data report from EC.

  @param[in]  CpuSetupDataPtr     Pointer point to CpuSetup.

  @retval     EFI_SUCCESS         Initialize EC successfully.
  @retval     others              Fail to do EC initialization.
**/
EFI_STATUS
EFIAPI
EcInitialize (
  IN  CPU_SETUP           *CpuSetupDataPtr
  );

/**
  Entry point of the ACPI platform driver.

  @param[in] ImageHandle  A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The driver installed without error.
  @retval    EFI_ABORTED  The driver encountered an error and could not complete
                          installation of the ACPI tables.
**/
EFI_STATUS
EFIAPI
InstallAcpiPlatform (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

BOOLEAN
IsPs2MouseConnected (
  VOID
  );

#endif
