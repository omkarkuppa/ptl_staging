/** @file
  System reset library services.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2007 Intel Corporation.

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
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/PmcLib.h>
#include <Library/BaseLib.h>
#include <Register/EspiRegs.h>
#include <Register/PmcRegs.h>

/**
  Calling this function causes a system-wide reset. This sets
  all circuitry within the system to its initial state. This type of reset
  is asynchronous to system operation and operates without regard to
  cycle boundaries.

  System reset should not return, if it returns, it means the system does
  not support cold reset.
**/
VOID
EFIAPI
ResetCold (
  VOID
  )
{
  IoWrite8 (R_PCH_IO_RST_CNT, V_PCH_IO_RST_CNT_FULLRESET);
  CpuDeadLoop ();
}

/**
  Calling this function causes a system-wide initialization. The processors
  are set to their initial state, and pending cycles are not corrupted.

  System reset should not return, if it returns, it means the system does
  not support warm reset.
**/
VOID
EFIAPI
ResetWarm (
  VOID
  )
{
  IoWrite8 (R_PCH_IO_RST_CNT, V_PCH_IO_RST_CNT_HARDRESET);
  CpuDeadLoop ();
}

/**
  Calling this function causes the system to enter a power state equivalent
  to the ACPI G2/S5 or G3 states.

  System shutdown should not return, if it returns, it means the system does
  not support shut down reset.
**/
VOID
EFIAPI
ResetShutdown (
  VOID
  )
{
  UINT16         ABase;
  UINT32         Data32;

  ABase = PmcGetAcpiBase ();

  ///
  /// Firstly, GPE0_EN should be disabled to avoid any GPI waking up the system from S5
  ///
  IoWrite32 (ABase + R_ACPI_IO_GPE0_EN_127_96, 0);
  IoWrite32 (ABase + R_ACPI_IO_GPE1_EN_31_0,   0);  // GPE1 is enabled in new chipsets
                                                    // GPE1 registers are not defined in older chipsets
                                                    // writing to GPE1 registers may cause conflicts in older systems

  ///
  /// Secondly, PwrSts register must be cleared
  ///
  /// Write a "1" to bit[8] of power button status register at
  /// (PM_BASE + PM1_STS_OFFSET) to clear this bit
  ///
  IoWrite16 (ABase + R_ACPI_IO_PM1_STS, B_ACPI_IO_PM1_STS_PWRBTN);

  ///
  /// Finally, transform system into S5 sleep state
  ///
  Data32 = IoRead32 (ABase + R_ACPI_IO_PM1_CNT);

  Data32 = (UINT32) ((Data32 &~(B_ACPI_IO_PM1_CNT_SLP_TYP + B_ACPI_IO_PM1_CNT_SLP_EN)) | V_ACPI_IO_PM1_CNT_S5);

  IoWrite32 (ABase + R_ACPI_IO_PM1_CNT, Data32);

  Data32 = Data32 | B_ACPI_IO_PM1_CNT_SLP_EN;

  IoWrite32 (ABase + R_ACPI_IO_PM1_CNT, Data32);
  CpuDeadLoop ();

  return;
}

/**
  Calling this function causes the system to enter a power state for platform specific.

  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.

**/
VOID
EFIAPI
ResetPlatformSpecific (
  IN UINTN            DataSize,
  IN VOID             *ResetData OPTIONAL
  )
{
  IoWrite8 (R_PCH_IO_RST_CNT, V_PCH_IO_RST_CNT_FULLRESET);
  CpuDeadLoop ();
}

/**
  Calling this function causes the system to enter a power state for capsule update.

  Reset update should not return, if it returns, it means the system does
  not support capsule update.

**/
VOID
EFIAPI
EnterS3WithImmediateWake (
  VOID
  )
{
  //
  // In case there are pending capsules to process, need to flush the cache.
  //
  AsmWbinvd ();

  ResetWarm ();
  ASSERT (FALSE);
}
