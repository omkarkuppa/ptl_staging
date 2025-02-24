/** @file
  Initializes CAN Controllers.

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

// Note: Uncomment below line for IpSim testing
//#define IPSIM_PRINT
#include <PrintService.h>
#include <Library/CanSsInitLib.h>
#include <Register/CanRegs.h>

/**
  Print CAN Device configuration data

  @param[in] CanCtrl   Pointer to CAN controller structure
**/
STATIC
VOID
CanSsPrintCtrlSettings (
  IN CAN_CTRL   *CanCtrl
  )
{
  DEBUG ((DEBUG_INFO, "\tController Index: %d\n",     CanCtrl->CtrlIndex));
  DEBUG ((DEBUG_INFO, "\tPciCfgCtrlOffset: 0x%04X\n", CanCtrl->PciCfgCtrlOffset));
  DEBUG ((DEBUG_INFO, "\tIrq:              %d\n",     CanCtrl->IrqNumber));
  DEBUG ((DEBUG_INFO, "\tIPin:             %d\n",     CanCtrl->IPin));
}

/**
  Configures CAN interrupts

  @param[in] PcrAccess  Registers Access protocol for CAN SS Registers
  @param[in] CanCtrl    CAN controller
**/
VOID
CanConfigureInterrupts (
  IN  REGISTER_ACCESS   *PcrAccess,
  IN  CAN_CTRL          *CanCtrl
  )
{
  UINT32    Data32Or;
  UINT32    Data32And;

  Data32Or =  (UINT32) ((CanCtrl->IrqNumber << N_CAN_PCR_PCICFGCTR_PCI_IRQ) |
                        (CanCtrl->IPin << N_CAN_PCR_PCICFGCTR_PCI_IPIN));
  Data32And = ~(UINT32) (B_CAN_PCR_PCICFGCTR_PCI_IRQ | B_CAN_PCR_PCICFGCTR_PCI_IPIN);

  PcrAccess->AndThenOr32 (PcrAccess, CanCtrl->PciCfgCtrlOffset, Data32And, Data32Or);
}

/**
  Configures CAN Subsystem

  @param[in] CanSubsystem     The CAN Subsystem

  @retval    EFI_SUCCESS            Initialization completed successfully
  @retval    EFI_INVALID_PARAMETER  Null pointer passed to the function
  @retval    EFI_ABORTED            Controller number out of the range
**/
EFI_STATUS
CanSubsystemInit (
  IN CAN_SUBSYSTEM *CanSubsystem
  )
{
  CAN_CTRL  *Ctrl;
  UINT32    Index;

  DEBUG ((DEBUG_INFO, "CanSubsystemInit: Start\n"));

  if (CanSubsystem == NULL) {
    DEBUG ((DEBUG_ERROR, "CanSubsystemInit: CanSubsystem is NULL pointer!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (CanSubsystem->PcrAccess == NULL) {
    DEBUG ((DEBUG_ERROR, "CanSubsystemInit: No Register Access protocol provided for CAN Subsystem.\n"));
    return EFI_ABORTED;
  }

  Ctrl = CanSubsystem->Controllers;
  if (Ctrl == NULL) {
    DEBUG ((DEBUG_ERROR, "CanSubsystemInit: No controllers defined for CAN Subsystem.\n"));
    return EFI_NOT_FOUND;
  }
  Index = 0;
  while (Ctrl != NULL) {
    DEBUG ((DEBUG_INFO, "Initilize CAN Device %d/%d:\n", Index, Ctrl->CtrlIndex));
    if (Index >= CanSubsystem->MaxCtrlNumber) {
      DEBUG ((DEBUG_ERROR, "CanSubsystemInit: Controller number out of the range.\n"));
      return EFI_ABORTED;
    }
    CanSsPrintCtrlSettings (Ctrl);
    CanConfigureInterrupts (CanSubsystem->PcrAccess, Ctrl);
    Index++;
    Ctrl = Ctrl->Next;
  }

  DEBUG ((DEBUG_INFO, "CanSubsystemInit: End\n"));

  return EFI_SUCCESS;
}
