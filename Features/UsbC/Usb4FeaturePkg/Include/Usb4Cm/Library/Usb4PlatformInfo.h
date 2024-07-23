/** @file
  Header file for USB4 platform information.

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

#ifndef _USB4_PLATFORM_INFO_H_
#define _USB4_PLATFORM_INFO_H_

#include <Uefi.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4Control.h>
#include <Library/Usb4HrPci.h>

#define HR_MASK_IUSB4_BITS     0x0F
#define HR_MASK_DUSB4_BITS     0xF0

#define HR_MASK_IUSB4_0        0x01
#define HR_MASK_IUSB4_1        0x02
#define HR_MASK_IUSB4_2        0x04
#define HR_MASK_IUSB4_3        0x08
#define HR_MASK_DUSB4_0        0x10
#define HR_MASK_DUSB4_1        0x20
#define HR_MASK_DUSB4_2        0x40
#define HR_MASK_DUSB4_3        0x80

//
// USB4 Host Router Information
//
typedef struct _USB4_HR_INFO {
  BOOLEAN               IntegratedHr;
  // TBT index in platform
  UINT8                 TbtIndex;
  // SW CM version host router can support
  UINT8                 CmVer;
  // USB4 control supported : USB3/DP/PCIe/InterDomain
  USB4_CONTROL          Usb4Control;
  // Mailbox status register offset
  UINT32                MbStatusReg;
  // Mailbox command register offset
  UINT32                MbCmdReg;
  // BDF for Mailbox command
  PCI_DEV_BDF           MailboxBdf;
  // BDF for USB4 Host Interface
  PCI_DEV_BDF           NhiBdf;
  // Root port of USB4 host router
  PCI_BRIDGE_INFO       Rp;
  // Upstream port of USB4 host router
  PCI_BRIDGE_INFO       UsPort;
  // Downstream port of USB4 host router
  PCI_BRIDGE_INFO       DsPort;
  // PEI reserved DMA buffer address for USB4 host router
  PHYSICAL_ADDRESS      PeiDmaBufBase;
  // PEI reserved DMA buffer size for USB4 host router
  UINT32                PeiDmaBufSize;
} USB4_HR_INFO, *PUSB4_HR_INFO;

//
// Overall USB4 Host Router information in the platform.
//
typedef struct _USB4_PLATFORM_INFO {
  UINT8           Usb4HrCount;
  UINT8           Usb4HrMask;
  UINT8           CmMode;
  UINT8           CmModeOption;
  BOOLEAN         CmSwitchSupport;
  BOOLEAN         PcieOverUsb4En;
  UINT32          Usb4DebugLevel;
  // Indicate whether USB4 Ver2 support is enabled or not
  BOOLEAN         CmVer2Support;
  USB4_HR_INFO    Usb4Hr[USB4_HR_SUPPORT_MAX];
  UINT8           Usb4MaxTopologyDepth;
} USB4_PLATFORM_INFO, *PUSB4_PLATFORM_INFO;

#endif
