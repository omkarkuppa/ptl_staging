/** @file
  Header file for PCI information of USB4 host router.

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

#ifndef _USB4_HR_PCI_H_
#define _USB4_HR_PCI_H_
#include <Uefi.h>

//
// PCI device bus/device/function
//
typedef struct _PCI_DEV_BDF {
  UINT8  Bus : 8;
  UINT8  Dev : 5;
  UINT8  Func: 3;
} PCI_DEV_BDF, *PPCI_DEV_BDF;

//
// PCI bridge information
//
typedef struct _PCI_BRIDGE_INFO {
  PCI_DEV_BDF    Bdf;
  UINT8          PriBus;
  UINT8          SecBus;
  UINT8          SubBus;
  UINT16         MemBase;
} PCI_BRIDGE_INFO, *PPCI_BRIDGE_INFO;

#endif
