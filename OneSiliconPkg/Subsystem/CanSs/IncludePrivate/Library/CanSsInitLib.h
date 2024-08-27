/** @file
  Initialize CAN IP.

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

#ifndef _CAN_SS_INIT_LIB_H_
#define _CAN_SS_INIT_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <RegisterAccess.h>

typedef struct _CAN_CTRL CAN_CTRL;

struct _CAN_CTRL {
  /*
    Zero based CAN Controller index
  */
  UINT32      CtrlIndex;

  /*
    PCI Configuration and Control register offset
    in CAN Subsystem PCR space assigned for this controller
  */
  UINT32      PciCfgCtrlOffset;

  /*
    PCI_IRQ:
    IRQ number to be sent in the message with data field
    for PCI Assert_IRQ and PCI_Deassert_IRQ message
  */
  UINT8       IrqNumber;

  CAN_CTRL    *Next;
};

typedef struct {
  /*
    CAN Subsystem registers access protocol
    for Private Configuration Registers (PCR) space
   */
  REGISTER_ACCESS   *PcrAccess;

  /*
    Maximum posible controller number
  */
  UINT8             MaxCtrlNumber;

  /*
    List of controllers integrated in CAN Subsytem
  */
  CAN_CTRL          *Controllers;
} CAN_SUBSYSTEM;

/**
  Configures CAN Subsystem

  @param[in] CanSubsystem     The CAN Subsystem

  @retval    EFI_SUCCESS      Initialization completed successfully
**/
EFI_STATUS
CanSubsystemInit (
  IN CAN_SUBSYSTEM *CanSubsystem
  );

#endif //_CAN_SS_INIT_LIB_H_
