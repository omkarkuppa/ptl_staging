/** @file
  Common SMBUS header for DXE and SMM

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _SMBUS_DXE_SMM_H_
#define _SMBUS_DXE_SMM_H_

#include <Base.h>
#include <Protocol/SmbusHc.h>
#include "SmbusCommon.h"

#define PCH_SMBUS_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('p', 's', 'm', 'b')

typedef struct _SMBUS_PRIVATE_DATA_DXE {
  UINTN                          Signature;
  EFI_HANDLE                     Handle;
  EFI_SMBUS_HC_PROTOCOL          SmbusController;
  SMBUS_DEVICE_ENUMERATION_DATA  DeviceEnumData;
  LIST_ENTRY                     NotifyFunctionList;
  EFI_EVENT                      NotificationEvent;
  EFI_PCI_IO_PROTOCOL            *PciIo;
} SMBUS_PRIVATE_DATA_DXE;

#define SMBUS_PRIVATE_DATA_FROM_THIS(a)  CR(a, SMBUS_PRIVATE_DATA_DXE, SmbusController, PCH_SMBUS_PRIVATE_DATA_SIGNATURE)

#endif // _SMBUS_DXE_SMM_H_
