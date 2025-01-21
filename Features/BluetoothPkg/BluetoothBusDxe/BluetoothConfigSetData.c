/** @file
  The file implements some interfaces of EFI_BLUETOOTH_CONFIG_PROTOCOL.

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
  Set BluetoothLE configuration data.

  The SetData() function sets local BluetoothLE device configuration data. Not all DataType can be
  set.

  @param[in]  This              Pointer to the EFI_BLUETOOTH_LE_CONFIG_PROTOCOL instance.
  @param[in]  DataType          Configuration data type.
  @param[in]  DataSize          Indicates the size, in bytes, of the data buffer specified by Data.
  @param[in]  Data              A pointer to the buffer of data that will be set.

  @retval EFI_SUCCESS           The BluetoothLE configuration data is set successfully.
  @retval EFI_INVALID_PARAMETER One or more of the following conditions is TRUE:
                                - DataSize is 0.
                                - Data is NULL.
  @retval EFI_UNSUPPORTED       The DataType is unsupported.
  @retval EFI_WRITE_PROTECTED   Cannot set configuration data.

**/
EFI_STATUS
EFIAPI
BluetoothLeConfigSetData (
  IN EFI_BLUETOOTH_LE_CONFIG_PROTOCOL       *This,
  IN EFI_BLUETOOTH_CONFIG_DATA_TYPE         DataType,
  IN UINTN                                  DataSize,
  IN VOID                                   *Data
)
{
  switch (DataType) {
    case EfiBluetoothConfigDataTypeDeviceName:
    case EfiBluetoothConfigDataTypeIoCapability:
    case EfiBluetoothConfigDataTypeOOBDataFlag:
    case EfiBluetoothConfigDataTypeKeyType:
    case EfiBluetoothConfigDataTypeEncKeySize:
      return stack_set_local_data(DataType, DataSize, Data);
    default:
      return EFI_INVALID_PARAMETER;
  }
}

