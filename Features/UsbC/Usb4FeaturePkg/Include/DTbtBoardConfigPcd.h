/** @file
  dTBT Board Config Structural PCD definition..

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

@par Specification
**/

#ifndef _DTBT_BOARD_PCD_H_
#define _DTBT_BOARD_PCD_H_

#include <TbtInit/DTbtDefinition.h>

#pragma pack(push, 1)

//
// dTBT Controller Board Configuration Structure. Data are Specific according to Board Design.
//
typedef struct _DTBT_BOARD_CONFIG {
  //
  // Discrete Thunderbolt Controller Support Is available from board side or not.
  // < TRUE- dTBT HW is connected and Board Support is Available, FALSE- Not Supported.>
  //
  BOOLEAN                         DTbtBoardSupport;

  //
  // Discrete Thunderbolt Controller PCIE RP Type.
  // < 01-Pcie RP, 02- PEG, 0xFF - Invalid RP Type. Reserved.>
  //
  UINT8                           DTbtPcieRpType;

  //
  // PCIE RP Socket Number that connect to dTBT controller.
  //
  UINT8                           DTbtPcieRpSocketNumber;

  //
  // PCIE RP SEG Number that connect to dTBT controller.
  //
  UINT8                           DTbtPcieRpSegNumber;

  //
  // PCIE RP BUS Number that connect to dTBT controller.
  //
  UINT8                           DTbtPcieRpBusNumber;

  //
  // Discrete Thunderbolt Controller PCIE RP Index.
  // < PCH PCIE RP Number (0-23) / CPU PCIE(PEG) Port (0,1,2,3) that connect to dTBT controller.>
  //
  UINT8                           DTbtPcieRpNumber;

  //
  // Boolean indicate maximum payload size of DTbt root port be used
  //   0 = 128 Byte, 1 = 256 Byte
  //
  UINT8                           DTbtPcieRpMps;

  //
  // Reserved for DWORD alignment
  //
  UINT8                           Reserved;

  //
  // Discrete Thunderbolt Controller Wake GPIO Pin.
  //
  UINT32                          DTbtWakeGpioPin;
} DTBT_BOARD_CONFIG;

//
// dTBT Controller Board Configuration Structure PCD Interface
//
typedef struct _DTBT_BOARD_CONFIG_PCD {
  DTBT_BOARD_CONFIG             DTbtBoardConfig[MAX_DTBT_CONTROLLER_NUMBER];
} DTBT_BOARD_CONFIG_PCD;
#pragma pack(pop)

#endif
