/** @file
  IoTrap SoC layer Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IocRegAccessLib.h>
#include <Library/S3BootScriptLib.h>
#include <IoTrapSocLib.h>
#include <Register/HostBridgeRegs.h>

/**
  Helper function that encapsulates SOC specific IoTrap low word register access.

  @param[in] TrapHandlerNum    trap number (0-3)
  @param[in] Value             value to be written
  @param[in] SaveToBootscript  if true, this register write will be saved to bootscript

**/
VOID
SocSetIoTrapLowDword (
  IN UINT8   TrapHandlerNum,
  IN UINT32  Value,
  IN BOOLEAN SaveToBootscript
  )
{
  UINT64  Address;

  Address = IOT1_IOC_MCHBAR_REG + TrapHandlerNum * 8;
  IocRegAccessWrite32 (Address, Value);
  IocRegAccessRead32 (Address);
  Address = Address + (UINTN) GetIocRegAccessBaseAddress ();

  if (SaveToBootscript) {
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, Address, 1, &Value);
  }
}

/**
  Helper function that encapsulates Soc specific IoTrap high word register access.

  @param[in] TrapHandlerNum    trap number (0-3)
  @param[in] Value             value to be written in both registers
  @param[in] SaveToBootscript  if true, this register write will be saved to bootscript

**/
VOID
SocSetIoTrapHighDword (
  IN UINT8   TrapHandlerNum,
  IN UINT32  Value,
  IN BOOLEAN SaveToBootscript
  )
{
  UINT64  Address;

  Address = IOT1_IOC_MCHBAR_REG + TrapHandlerNum * 8 + 4;
  IocRegAccessWrite32 (Address, Value);
  IocRegAccessRead32 (Address);
  Address = Address + (UINTN) GetIocRegAccessBaseAddress ();

  if (SaveToBootscript) {
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, Address, 1, &Value);
  }
}
