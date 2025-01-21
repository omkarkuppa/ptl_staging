/** @file
  Force power on YFL router

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
#include <Library/TbtNvmDrvYflRouter.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  TbtNvmDrvYflForcePwrFunc function to perform force power on YFL router
  Operation:
    Read modify write PCI CFG space, byte offset - 0xfc[1] = 1
    Wait for PCI CFG space, byte offset - 0xC8[31] to be set.
**/
TBT_STATUS
TbtNvmDrvYflForcePwrFunc (
  IN       EFI_PCI_IO_PROTOCOL   *PciIoProto,
  IN       BOOLEAN               Enable,
  IN  OUT  BOOLEAN               *TBTControllerWasPowered
  )
{
  EFI_STATUS Status;
  UINT32     Reg;
  UINT32     Index;

  // Force power
  if (Enable) {
    DEBUG ((DEBUG_INFO, "TbtNvmDrvYflForcePwrFunc: Performing force power on TBT Controller\n"));
  } else {
    DEBUG ((DEBUG_INFO, "TbtNvmDrvYflForcePwrFunc: Disabling force power on TBT Controller\n"));
  }

  if (PciIoProto == NULL) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvYflForcePwrFunc: PciIoProto parameter is NULL\n"));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  if (TBTControllerWasPowered == NULL) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvYflForcePwrFunc: TBTControllerWasPowered parameter is NULL\n"));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  // Check if TBT Controller was already powered in SW CM mode.
  if (*TBTControllerWasPowered == TRUE) {
    return TBT_STATUS_SUCCESS;
  }

  Status = PciIoProto->Pci.Read (PciIoProto, EfiPciIoWidthUint32, FORCE_PWR_REG_OFFSET, 1, &Reg);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvYflForcePwrFunc: Couldn't read from PCIe register - 0x%x, Status - %r\n", FORCE_PWR_REG_OFFSET, Status));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  if (Enable) {
    // Check if TBT Controller was already powered
    if (Reg & MASK (FORCE_PWR_REG_BIT_INDEX, FORCE_PWR_REG_BIT_INDEX)) {
      *TBTControllerWasPowered = TRUE;
      return TBT_STATUS_SUCCESS;
    }
    Reg = Reg | BIT (FORCE_PWR_REG_BIT_INDEX);
  } else {
    Reg = Reg & ~BIT (FORCE_PWR_REG_BIT_INDEX);
  }
  Status = PciIoProto->Pci.Write (PciIoProto, EfiPciIoWidthUint32, FORCE_PWR_REG_OFFSET, 1, &Reg);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvYflForcePwrFunc: Couldn't write to PCIe register - 0x%x, Status - %r\n", FORCE_PWR_REG_OFFSET, Status));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  // In case the operation is disable, no need to verify FW is ready.
  if (!Enable) {
    return TBT_STATUS_SUCCESS;
  }

  // Wait for FW is ready
  for (Index = 0; Index < TOTAL_WAIT_FOR_PWR_ON_MS; Index++) {
    Status = PciIoProto->Pci.Read (PciIoProto, EfiPciIoWidthUint32, FW_READY_REG_OFFSET, 1, &Reg);
    if (EFI_ERROR (Status)) {
      continue;
    }
    Reg = Reg & MASK (FW_READY_REG_BIT_INDEX, FW_READY_REG_BIT_INDEX);
    if (Reg != 0) {
      DEBUG ((DEBUG_INFO, "FW of TBT controller is ready.\n"));
      *TBTControllerWasPowered = TRUE;
      return TBT_STATUS_SUCCESS;
    }
    DEBUG ((DEBUG_WARN, "FW of TBT controller is not yet ready.\n"));
    gBS->Stall (1000);   // Wait 1ms
  }
  DEBUG ((DEBUG_ERROR, "TbtNvmDrvYflForcePwrFunc: Waiting too much on power on. Exiting...\n"));
  return TBT_STATUS_NON_RECOVERABLE_ERROR;

}