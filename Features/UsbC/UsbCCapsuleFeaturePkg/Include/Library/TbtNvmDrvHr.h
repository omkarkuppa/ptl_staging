/** @file
  Declares the interface for TbtNvmDrvHr class.
  This class represents TBT HR abstraction.

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

@par Specification Reference:

**/
#ifndef TBT_NVM_DRV_HR_
#define TBT_NVM_DRV_HR_

#include <Protocol/PciIo.h>
#include <TbtNvmRetimer.h>

#include <Library/TbtNvmDrvDma.h>
#include <Protocol/TbtNvmDrvHrProtocol.h>

#define TBT_EE_ARC_EN_BIT_OFFSET 0
#define TBT_EE_ARC_EN_XOR_BIT_OFFSET 1
#define TBT_DEVICE_CS_LC_PER_CONTROLLER_VSEC0_BASE 0x113
#define TBT_DEVICE_CS_LC_PER_CONTROLLER_VSEC1_BASE 0x213
#define TBT_DEVICE_CS_LC_SX_CTRL_OFFSET TBT_DEVICE_CS_LC_PER_CONTROLLER_VSEC0_BASE + 0x96
#define INTEL_CM_BIT    (1<<28)

#define USB4_CLASS_CODE 0x0C0340

#define USB_VENDOR_ID_1 0x8086
#define USB_VENDOR_ID_2 0x8087
//
// CM mode in Pre-boot - bit 2:0
//
#define USB4_CM_MODE_FW_CM                  0x00
#define USB4_CM_MODE_SW_CM                  0x01
#define USB4_CM_MODE_IN_PRE_BOOT            (BIT2 | BIT1 | BIT0) ///< CM mode in Pre-boot - 3 bits

//
// Interface of PCI class code for USB4 host router.
// Defined in Usb4Reg.h
// Redefined here to avoid static code analysis failure.
//
#define PCI_IF_USB4    0x40

#define MAX_HOST_ROUTERS 10

typedef struct {
  UINT32 reqCode;
  UINT32 crc;
} DRV_RDY_CMD;

/*
* SW->FW commands, supported by this implementation
* CC = Command Code
*/
enum {
  CC_DRV_READY = 3
};

/*
* FW->SW responses, supported by this implementation
* RC = response code
*/
enum {
  RC_DRV_READY = 3
};

typedef struct {
  UINT16 VendorId;
  UINT16 DeviceId;
} PCI_ID;

TBT_HOST_ROUTER*
TbtNvmDrvHrCtor (
  IN UINT8           FirmwareType,
  IN PCIE_RP_CONFIG  *PcieRpConfig,
  IN PCIE_BDF        *TbtDmaPcieBdf, OPTIONAL
  IN FORCE_PWR_HR    ForcePwrFunc OPTIONAL
  );

#endif /* TBT_NVM_DRV_HR_ */
