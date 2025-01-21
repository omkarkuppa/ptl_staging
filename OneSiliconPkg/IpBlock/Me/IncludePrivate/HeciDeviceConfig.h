/** @file
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
**/

#ifndef _HECI_DEVICE_CONFIG_H_
#define _HECI_DEVICE_CONFIG_H_

#include <MeDefines.h>

typedef struct _HECI_DEVICE_LOCATION {
  SECURITY_ENGINE SecurityEngine;     ///<0x00 Csme
                                      ///<0x01 Sse
                                      ///<0x02 Pse

  UINTN           HeciDevFunction;    ///< Varies by SecurityEngine
                                      ///< Of type HECI_DEVICE for Csme
                                      ///< Of type PSE_HECI_DEVICE for Pse
} HECI_DEVICE_LOCATION;

/**
  HECI device configuration structure.
  This structure describes HECI device instance.
  It is used by the HECI driver for device initialization.
**/
typedef struct _HECI_DEVICE_CONFIG {
  SECURITY_ENGINE SecurityEngine;     ///< Engine device where HECI is located

  UINT64          PciSegment;         ///< PCI segment where this HECI device is located
  UINT64          PciBus;             ///< PCI bus where this HECI device is located
  UINT64          PciDevice;          ///< PCI device where this HECI device is located
  UINT64          PciFunction;        ///< PCI function number of this HECI device

  UINT32          HeciInterruptMode;  ///< HECI interrupt mode for this HECI device

  UINT64          TempMemoryBar;      ///< Temporary MMIO address space assigned to this device
} HECI_DEVICE_CONFIG;

#endif
