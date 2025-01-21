/** @file
  Header file for IoTrapSoc Libraray.

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

#ifndef _IOTRAP_SOC_LIB_H_
#define _IOTRAP_SOC_LIB_H_

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
  );

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
  );

#endif
