/** @file
  Common Lpss Private Lib implementation - I3C part
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
#include <Register/LpssI3cRegs.h>
#include <LpssI3cController.h>

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     I3cController  Instace of I3C Controller
  @param[in]     TurnOff        If PCIe function is disabled
  @param[in/out] Command           Pci Command register data to save
  @param[in/out] Pme               Pci Pme Control register data to save

**/
VOID
LpssI3cS3Handler (
  IN     LPSS_I3C_CONTROLLER  *I3cController,
  IN     BOOLEAN              TurnOff,
  IN OUT UINT32               *Command,
  IN OUT UINT32               *Pme
  )
{
  *Pme = I3cController->PcieCfgAccess->Read32 (I3cController->PcieCfgAccess, R_LPSS_I3C_PCR_PMECTRLSTATUS);
  *Command = I3cController->PcieCfgAccess->Read32 (I3cController->PcieCfgAccess, PCI_COMMAND_OFFSET);

  if (TurnOff) {
    *Pme          = *Pme | BIT0 | BIT1;
    *Command      = *Command & (UINT32)~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  }
}
