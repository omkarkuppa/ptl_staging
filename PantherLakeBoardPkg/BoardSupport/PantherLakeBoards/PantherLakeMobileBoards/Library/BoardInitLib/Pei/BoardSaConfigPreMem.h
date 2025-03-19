/** @file
  PEI Boards Configurations for PreMem phase.

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

#ifndef _BOARD_SA_CONFIG_PRE_MEM_H_
#define _BOARD_SA_CONFIG_PRE_MEM_H_

#include <Ppi/SiPolicy.h>
#include <Library/BoardConfigLib.h>

//
// LPDDR3 / LPDDR4 RVP boards (memory soldered down)
// Single SPD EEPROM at 0xA2 serves both C0D0 and C1D0 (LPDDR3/4 is 1DPC only)
//
#define DIMM_SMB_SPD_P0C0D0_STP 0xA2
#define DIMM_SMB_SPD_P0C0D1_STP 0xA0
#define DIMM_SMB_SPD_P0C1D0_STP 0xA2
#define DIMM_SMB_SPD_P0C1D1_STP 0xA0
#define SA_MRC_MAX_RCOMP_TARGETS  (5)

//
// GopConfig Driver will use this Hob to override the VBT
//
typedef struct {
  UINT64         SkuId;
/*
  ModuleTypeCId: There are total 16Bits are available. Remaining Upper 16bits are unused/rsvd.
  0000    0000    0000    0000
  TCP4    TCP3    TCP2    TCP1

  Each 4Bit will be decoded as below: other combinations are not supported
  --------------------------------------------
  | BIT3 | BIT2 | BIT1 | BIT0 | Defination   |
  --------------------------------------------
  |   1  |   0  |   0  |   0  | eDP over TCP |
  |   1  |   0  |   0  |   1  | Module DP    |
  |   1  |   0  |   1  |   0  | Module HDMI  |
  --------------------------------------------
*/
  UINT32         ModuleTypeCId;   // Used to identify the Modular Type C Type.
  UINT8          DisplayId;       // Used to define Fixed Ports (non-Type C).
  UINT8          SoftwareId;      // extensibility for un-defined reworks.
  UINT16         PlatformId;      // Used to identify platform
  BOOLEAN        GopConfigEnable; // GopConfig Enable to check if the VBT modification is needed or not
  UINT8          Reserved[7];
} GOP_CONFIG_DRIVER_HOB;

#endif // _BOARD_SA_CONFIG_PRE_MEM_H_
