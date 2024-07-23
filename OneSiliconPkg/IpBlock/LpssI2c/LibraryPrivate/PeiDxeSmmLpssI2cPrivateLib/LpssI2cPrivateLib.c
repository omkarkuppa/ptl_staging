/** @file
  Common Lpss Private Lib implementation - I2C part
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/Pci.h>
#include <Register/LpssI2cRegs.h>
#include <LpssI2cController.h>

/**
  Configures Lpss Controller before control is passd to the OS

  @param[in] I2cController     Instace of I2C Controller
**/
VOID
LpssI2cBootHandler (
  IN  LPSS_I2C_CONTROLLER   *I2cController
  )
{
    //
    // Put Device in D3
    //
  I2cController->PcieCfgAccess->Or32 (I2cController->PcieCfgAccess, R_LPSS_I2C_PCR_PMECTRLSTATUS, 0x3);
    //
    // Reading back value after write to ensure bridge observes the BAR1 write access
    //
  I2cController->PcieCfgAccess->Read8 (I2cController->PcieCfgAccess, R_LPSS_I2C_PCR_PMECTRLSTATUS);

    //
    // Disable MMIO
    //
  I2cController->PcieCfgAccess->And16 (I2cController->PcieCfgAccess, PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  I2cController->PcieCfgAccess->Write32 (I2cController->PcieCfgAccess, R_LPSS_I2C_CFG_BAR, 0x0);
  if (I2cController->PcieCfgAccess->Read32 (I2cController->PcieCfgAccess, R_LPSS_I2C_CFG_BAR) & BIT2) {
    I2cController->PcieCfgAccess->Write32 (I2cController->PcieCfgAccess, R_LPSS_I2C_CFG_BAR_HIGH, 0x0);
  }
}

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     I2cController  Instace of I2C Controller
  @param[in]     TurnOff        If PCIe function is disabled
  @param[in/out] Command           Pci Command register data to save
  @param[in/out] Pme               Pci Pme Control register data to save

**/
VOID
LpssI2cS3Handler (
  IN     LPSS_I2C_CONTROLLER  *I2cController,
  IN     BOOLEAN              TurnOff,
  IN OUT UINT32                       *Command,
  IN OUT UINT32                       *Pme
  )
{
  *Pme = I2cController->PcieCfgAccess->Read32 (I2cController->PcieCfgAccess, R_LPSS_I2C_PCR_PMECTRLSTATUS);
  *Command = I2cController->PcieCfgAccess->Read32 (I2cController->PcieCfgAccess, PCI_COMMAND_OFFSET);

  if (TurnOff) {
    *Pme          = *Pme | BIT0 | BIT1;
    *Command      = *Command & (UINT32)~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  }
}
