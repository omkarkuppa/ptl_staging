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
#include <Library/UsbcCapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>

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
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_YFL_FORCE_PWR_PERF, 0, 0);
  } else {
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_YFL_FORCE_PWR_DISABLE, 0, 0);
  }

  if (PciIoProto == NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_YFL_FORCE_PWR_PCI_IO_NULL, 0, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  if (TBTControllerWasPowered == NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_YFL_FORCE_PWR_PARAMETER_NULL, 0, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  // Check if TBT Controller was already powered in SW CM mode.
  if (*TBTControllerWasPowered == TRUE) {
    return TBT_STATUS_SUCCESS;
  }

  Status = PciIoProto->Pci.Read (PciIoProto, EfiPciIoWidthUint32, FORCE_PWR_REG_OFFSET, 1, &Reg);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_YFL_FORCE_PWR_READ_PCIE_REG_ERROR, (UINT32) FORCE_PWR_REG_OFFSET, (UINT32) Status);
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
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_YFL_FORCE_PWR_READ_PCIE_REG_ERROR, (UINT32) FORCE_PWR_REG_OFFSET, (UINT32) Status);
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
      CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_YFL_TBT_CONTROLLER_READY, 0, 0);
      *TBTControllerWasPowered = TRUE;
      return TBT_STATUS_SUCCESS;
    }
    CapsuleLogWrite (USBC_CAPSULE_DBG_WARN, EVT_CODE_YFL_TBT_CONTROLLER_NOT_READY, 0, 0);
    gBS->Stall (1000);   // Wait 1ms
  }
  CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_YFL_FORCE_PWR_WAIT_ERROR, 0, 0);
  return TBT_STATUS_NON_RECOVERABLE_ERROR;
}