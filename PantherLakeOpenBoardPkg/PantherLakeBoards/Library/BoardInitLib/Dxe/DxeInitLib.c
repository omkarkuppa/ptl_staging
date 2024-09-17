/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Uefi.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/PmcLib.h>
#include <PcieRegs.h>
#include <Register/PchRegs.h>
#include <Register/PmcRegs.h>
#include <Register/RtcRegs.h>
#include <Library/DebugLib.h>

#define RTC_ADDRESS_REGISTER_D                       0x0D


/**
  Handler to enable ACPI mode

  Clear Port 80h

  Disables the SW SMI Timer.
  ACPI events are disabled and ACPI event status is cleared.
  SCI mode is then enabled.

  Disable PM sources except power button
  Clear status bits

  Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)

  Enable SCI

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
EnableAcpiCallback (
  VOID
  )
{
  UINT32                              OutputValue;
  UINT32                              Pm1Cnt;
  UINT16                              mAcpiBaseAddr;

  //
  // Get the ACPI Base Address
  //
  mAcpiBaseAddr = PmcGetAcpiBase ();
  //
  // Disable PM sources except power button
  //
  OutputValue = B_ACPI_IO_PM1_EN_PWRBTN;
  IoWrite16 (mAcpiBaseAddr + R_ACPI_IO_PM1_EN, (UINT16)OutputValue);

  //
  // Clear PM status except Power Button status for RapidStart Resume
  //
  OutputValue = 0xFEFF;
  IoWrite16 (mAcpiBaseAddr + R_ACPI_IO_PM1_STS, (UINT16)OutputValue);

  //
  // Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)
  //
  OutputValue = RTC_ADDRESS_REGISTER_D;
  IoWrite8 (R_RTC_IO_INDEX_ALT, (UINT8)OutputValue);
  OutputValue = 0x0;
  IoWrite8 (R_RTC_IO_TARGET_ALT, (UINT8)OutputValue);

  //
  // Enable SCI
  //
  Pm1Cnt = IoRead32 (mAcpiBaseAddr + R_ACPI_IO_PM1_CNT);
  Pm1Cnt |= B_ACPI_IO_PM1_CNT_SCI_EN;
  IoWrite32 (mAcpiBaseAddr + R_ACPI_IO_PM1_CNT, Pm1Cnt);

  DEBUG ((DEBUG_INFO, "EnableAcpiCallback exit\n"));

  return EFI_SUCCESS;
}

/**
  A hook for board-specific functionality for the ExitBootServices event.

  @retval EFI_SUCCESS   The board initialization was successful.
**/
EFI_STATUS
EFIAPI
PtlBoardInitEndOfFirmware (
  VOID
  )
{
  EFI_STATUS Status;
  DEBUG ((DEBUG_INFO, "PtlBoardInitEndOfFirmware\n"));
  Status = EnableAcpiCallback();

  return Status;
}
