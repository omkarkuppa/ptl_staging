/** @file
  TBT Policy Common definition.

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

#ifndef _DTBT_POLICY_GENERIC_H_
#define _DTBT_POLICY_GENERIC_H_

#include <IndustryStandard/Pci22.h>

#pragma pack(push, 1)

//
// dTBT Controller Data Structure
// Note: Add Reserved fields for DWORD alignment if necessary
//
typedef struct _DTBT_CONTROLLER_CONFIG {
  UINT8            DTbtControllerEn;   ///< Enable/Disable DTbtController.
  UINT8            RpType;             ///< 02 - PCIE_RP_TYPE_PCH, 04 - PCIE_RP_TYPE_CPU
  UINT8            PcieRpNumber;       ///< RP Number/ Root Port (0,1,2) that connect to dTBT controller. <Specific according to Board Design>
  UINT8            RpAcpiNumber;       ///< Number within RP ACPI device name. Not always same with PcieRpNumber.
  UINT8            PcieRpBus;          ///< RP number of Bus
  UINT8            PcieRpDev;          ///< RP number of Device
  UINT8            PcieRpFunc;         ///< RP number of Function
  UINT32           WakeGpioPin;        ///< Discrete Thunderbolt Controller Wake GPIO Pin.
  UINT8            PcieRpMps;          ///< The Max Payload Size of DTbt root port used, 0 : 128B, 1 : 256B.
  UINT8            Reserved[4];        ///< Reserved for DWORD alignment
} DTBT_CONTROLLER_CONFIG;

//
// dTBT Controller Data Structure
// Note: Add Reserved fields for DWORD alignment if necessary
//
typedef struct _DTBT_GENERIC_CONFIG {
  UINT8            TbtRtd3PepEnable;   ///< TBT RTD3 PEP enable
  UINT8            Usb4CmMode;         ///< USB4 CM mode
  UINT8            Usb4ClassOption;    ///< USB4 class code option
  UINT8            Reserved[1];        ///< Reserved for DWORD alignment
} DTBT_GENERIC_CONFIG;

#pragma pack(pop)

#define IS_DTBT_RP_NUM_VALID(DTbtControllerConfig, Index)  \
        DTbtControllerConfig[Index].PcieRpBus != 0 &&  \
        DTbtControllerConfig[Index].PcieRpDev != 0 &&  \
        DTbtControllerConfig[Index].PcieRpFunc != 0

#endif
