/** @file
  The file implements interfaces defined in EFI_BLUETOOTH_CONFIG_PROTOCOL.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include "BluetoothBusDxe.h"
#include "uefi_app.h"

/**
  Initialize BluetoothLE host controller and local device.

  The Init() function initializes BluetoothLE host controller and local device.

  @param[in]  This              Pointer to the EFI_BLUETOOTH_LE_CONFIG_PROTOCOL instance.

  @retval EFI_SUCCESS           The BluetoothLE host controller and local device is initialized successfully.
  @retval EFI_DEVICE_ERROR      A hardware error occurred trying to initialize the BluetoothLE host controller
                                and local device.

**/
EFI_STATUS
EFIAPI
BluetoothLeConfigInit (
  IN EFI_BLUETOOTH_LE_CONFIG_PROTOCOL  *This
  )
{
  BTHC_DEV                      *BtHcDevice;

  if (EfiGetCurrentTpl() != TPL_APPLICATION) {
    return EFI_ACCESS_DENIED;
  }

  BtHcDevice = BTHC_DEV_FROM_BLUETOOTH_LE_CONFIG_PROTOCOL(This);

  // Proceed with Stack initialization
  stack_init(BtHcDevice);

  return EFI_SUCCESS;
}
