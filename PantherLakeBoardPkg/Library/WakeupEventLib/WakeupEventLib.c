/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PmcLib.h>
#include <Library/IoLib.h>
#include <Register/PmcRegs.h>
#include <IndustryStandard/SmBios.h>

/**
  Parse the status registers for figuring out the wake-up event.

  @param[out]  WakeUpType       Updates the wakeuptype based on the status registers
**/
VOID
GetWakeupEvent (
  OUT  UINT8   *WakeUpType
  )
{
  UINT16             Pm1Sts;
  UINT32             Gpe0Sts;
  UINT32             Gpe1Sts;
  UINT32             Gpe0PmeSts;
  UINT32             Gpe0IshSts;
  UINT32             Gpe0LanSts;
  UINT32             Gpe1GbePmeB0Sts;
  UINT16             AcpiBase;

  DEBUG ((DEBUG_INFO, "GetWakeupEvent ()\n"));

  Gpe0Sts         = 0;
  Gpe1Sts         = 0;
  Gpe0IshSts      = 0;
  Gpe0PmeSts      = 0;
  Gpe0LanSts      = 0;
  Gpe1GbePmeB0Sts = 0;
  AcpiBase = PmcGetAcpiBase ();

  ///
  /// Read the ACPI registers
  ///
  Pm1Sts     = IoRead16 (AcpiBase + R_ACPI_IO_PM1_STS);
  Gpe0Sts    = IoRead32 (AcpiBase + R_ACPI_IO_GPE0_STS_127_96);
  Gpe1Sts    = IoRead32 (AcpiBase + R_ACPI_IO_GPE1_STS_31_0);

  Gpe0IshSts      = Gpe0Sts & B_ACPI_IO_GPE0_STS_127_96_ISH_STS;
  Gpe0PmeSts      = Gpe0Sts & B_ACPI_IO_GPE0_STS_127_96_PME_STS;
  Gpe0LanSts      = Gpe0Sts & B_ACPI_IO_GPE0_STS_127_96_LANWAKE_STS;
  Gpe1GbePmeB0Sts = Gpe1Sts & B_ACPI_IO_GPE1_STS_31_0_GBE_PME_B0_STS;

  DEBUG ((DEBUG_INFO, "Gpe0Sts is: %08x\n", Gpe0Sts));
  DEBUG ((DEBUG_INFO, "Gpe1Sts is: %08x\n", Gpe1Sts));
  DEBUG ((DEBUG_INFO, "ACPI Wake Status Register: %04x\n", Pm1Sts));

  ///
  /// Figure out the wake-up event
  ///
  if ((Pm1Sts & B_ACPI_IO_PM1_EN_PWRBTN) != 0) {
    *WakeUpType = SystemWakeupTypePowerSwitch;
  } else if ((Pm1Sts & B_ACPI_IO_PM1_STS_WAK) != 0) {
    if (Gpe0PmeSts != 0) {
      *WakeUpType = SystemWakeupTypePciPme;
    } else if (Gpe0IshSts != 0) {
      *WakeUpType = SystemWakeupTypeModemRing;
    } else if ((Gpe0LanSts != 0) || (Gpe1GbePmeB0Sts != 0)) {
      *WakeUpType = SystemWakeupTypeLanRemote;
    } else {
      *WakeUpType = SystemWakeupTypeOther;
    }
  } else if ((Pm1Sts & B_ACPI_IO_PM1_STS_RTC) != 0) {
    *WakeUpType = SystemWakeupTypeApmTimer;
  } else {
    *WakeUpType = SystemWakeupTypeOther;
  }

  DEBUG ((DEBUG_INFO, "GetWakeupEvent : WakeUpType %x\n", *WakeUpType));
}
