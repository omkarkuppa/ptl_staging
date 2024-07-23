/** @file
  DXE eSPI private library

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

#include <Library/S3PciSegmentLib.h>
#include <Register/EspiRegs.h>
#include <Uefi/UefiBaseType.h>

/**
  Configure BiosLockEnable bit and BiosInterfaceLock bit according to policy setting.

  @param[in] PciCfgBase         eSPI controller address in pcicfg space
  @param[in] BiosLockEnable     Policy for BiosLockEnable bit programming
  @param[in] BiosInterfaceLock  Policy for BiosInterfaceLock bit programming
  @param[in] EissEnable         Policy for EISS bit programming

**/
VOID
EspiBiosLockEnableAndBiosInterfaceLockWithS3BootScript (
  IN UINT64   PciCfgBase,
  IN BOOLEAN  BiosLockEnable,
  IN BOOLEAN  BiosInterfaceLock,
  IN BOOLEAN  EissEnable
  )
{
  UINT8                    Data8;

  if (!BiosLockEnable && !BiosInterfaceLock) {
    return;
  }


  ///
  /// PCH BIOS Spec Flash Security Recommendation
  ///
  /// BIOS needs to enable the BIOS Lock Enable (BLE) feature of the PCH by setting
  /// SPI/eSPI/LPC PCI offset DCh[1] = 1b.
  /// When this bit is set, attempts to write the Write Protect Disable (WPD) bit
  /// in PCH will cause a SMI which will allow the BIOS to verify that the write is
  /// from a valid source.
  /// Remember that BIOS needs to set SPI/LPC/eSPI PCI Offset DC [0] = 0b to enable
  /// BIOS region protection before exiting the SMI handler.
  /// Also, TCO_EN bit needs to be set (SMI_EN Register, ABASE + 30h[13] = 1b) to keep
  /// BLE feature enabled after booting to the OS.
  /// Intel requires that BIOS enables the Lock Enable (LE) feature of the PCH to
  /// ensure SMM protection of flash.
  /// RC installs a default SMI handler that clears WPD.
  /// There could be additional SMI handler to log such attempt if desired.
  ///
  /// BIOS needs to enable the "Enable in SMM.STS" (EISS) feature of the PCH by setting
  /// SPI PCI offset DCh[5] = 1b for SPI or setting eSPI PCI offset DCh[5] = 1b for eSPI.
  /// When this bit is set, the BIOS region is not writable until SMM sets the InSMM.STS bit,
  /// to ensure BIOS can only be modified from SMM. Please refer to CPU BWG for more details
  /// on InSMM.STS bit.
  /// Intel requires that BIOS enables the Lock Enable (LE) feature of the PCH to ensure
  /// SMM protection of flash.
  /// SPI PCI offset DCh[1] = 1b for SPI or setting eSPI PCI offset DCh[1] = 1b for eSPI.
  /// When this bit is set, EISS is locked down.
  ///
  Data8 = 0;
  if (BiosLockEnable) {
    Data8 |= B_ESPI_CFG_ESPI_BC_LE;
    if (EissEnable) {
      Data8 |= B_ESPI_CFG_ESPI_BC_EISS;
    }
  }
  ///
  /// BIOS also needs to set the BIOS Interface Lock Down bit in multiple locations
  /// (PCR[DMI] + 274Ch[0], LPC/eSPI PCI offset DCh[7] and SPI PCI offset DCh[7]).
  /// Setting these bits will prevent writes to the Top Swap bit (under their respective locations)
  /// and the Boot BIOS Straps. Enabling this bit will mitigate malicious software
  /// attempts to replace the system BIOS option ROM with its own code.
  ///
  if (BiosInterfaceLock) {
    Data8 |= B_ESPI_CFG_ESPI_BC_BILD;
  }

  S3PciSegmentOr8 (PciCfgBase + R_ESPI_CFG_ESPI_BC, Data8);
  //
  // Reads back for posted write to take effect
  //
  Data8 = S3PciSegmentRead8 (PciCfgBase + R_ESPI_CFG_ESPI_BC);
}

/**
  Lock SPI BIOS decode enable range from change. Saving lock configuration with use of S3BootScript.

  @param[in] PciCfgBase          eSPI controller address in pcicfg space

**/
VOID
EspiBiosDecodeEnableLockWithS3BootScript (
  IN UINT64 PciCfgBase
  )
{
  S3PciSegmentOr32 (PciCfgBase + R_ESPI_CFG_ESPI_BDE, B_ESPI_CFG_ESPI_BDE_BLE);
}
