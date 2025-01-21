/** @file
  GPIO device policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _GPIO_DEV_CONFIG_H_
#define _GPIO_DEV_CONFIG_H_

extern EFI_GUID gGpioDxeConfigGuid;

#define GPIO_DXE_CONFIG_REVISION 2

#pragma pack (push,1)

/**
  This structure contains the DXE policies which are related to GPIO device.

  <b>Revision 1:</b>
  - Inital version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;          ///< Config Block Header
  /**
    If GPIO ACPI device is not used by OS it can be hidden. In such case
    no other device exposed to the system can reference GPIO device in one
    of its resources through GpioIo(..) or GpioInt(..) ACPI descriptors.
    <b>0: Disable</b>; 1: Enable
  **/
  UINT32  HideGpioAcpiDevice    :  1;
  /**
    Only hide Hard GPIOs ACPI device
    'HideGpioAcpiDevice' set to 1 can override this flag.
    <b>0: Disable</b>; 1: Enable
  **/
  UINT32  HideHardGpioAcpiDevice:  1;
  /**
    Only hide Virtual GPIOs ACPI device
    'HideGpioAcpiDevice' set to 1 can override this flag.
    <b>0: Disable</b>; 1: Enable
  **/
  UINT32  HideVirtGpioAcpiDevice:  1;
  UINT32  RsvdBits              : 29;    ///< Reserved bits

} GPIO_DXE_CONFIG;

#pragma pack (pop)

#endif // _GPIO_DEV_CONFIG_H_
