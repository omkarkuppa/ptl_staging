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

/**
  Register link connect complete callback function.

  The RegisterLinkConnectCompleteCallback() function registers Bluetooth link connect
  complete callback function. The Bluetooth Configuration driver may call
  RegisterLinkConnectCompleteCallback() to register a callback function. During pairing,
  Bluetooth bus driver must trigger this callback function to report device state, if it is registered.
  Then Bluetooth Configuration driver will get information on device connection, according to
  CallbackType defined by EFI_BLUETOOTH_CONNECT_COMPLETE_CALLBACK_TYPE

  @param[in]  This            Pointer to the EFI_BLUETOOTH_LE_CONFIG_PROTOCOL instance.
  @param[in]  Callback        The callback function. NULL means unregister.
  @param[in]  Context         Data passed into Callback function. This is optional parameter and may be NULL.

  @retval EFI_SUCCESS         The link connect complete callback function is registered successfully.
  @retval EFI_ALREADY_STARTED A callback function is already registered on the same attribute
                              opcode and attribute handle, when the Callback is not NULL.
  @retval EFI_NOT_STARTED     A callback function is not registered on the same attribute opcode
                              and attribute handle, when the Callback is NULL
**/
EFI_STATUS
EFIAPI
BluetoothLeConfigRegisterConnectCompleteCallBack (
  IN EFI_BLUETOOTH_LE_CONFIG_PROTOCOL                                     *This,
  IN EFI_BLUETOOTH_LE_CONFIG_CONNECT_COMPLETE_CALLBACK                    Callback,
  IN VOID                                                                 *Context
  )
{
  BTHC_DEV            *BtHcDevice;
  BtHcDevice = BTHC_DEV_FROM_BLUETOOTH_LE_CONFIG_PROTOCOL(This);
  if (Callback == NULL) {
    BtHcDevice->LeConnCompCallBack = NULL;
    BtHcDevice->LeConnCompCallBackContext = NULL;
    return EFI_SUCCESS;
  }
  BtHcDevice->LeConnCompCallBack = Callback;
  BtHcDevice->LeConnCompCallBackContext = Context;

  return EFI_SUCCESS;
}

/**
  Register Security Manager Protocol callback function for user authentication/authorization.

  The RegisterSmpAuthCallback() function register Security Manager Protocol callback
  function for user authentication/authorization.

  @param[in]  This            Pointer to the EFI_BLUETOOTH_LE_CONFIG_PROTOCOL instance.
  @param[in]  Callback        Callback function for user authentication/authorization.
  @param[in]  Context         Data passed into Callback function. This is optional parameter and may be NULL.

  @retval EFI_SUCCESS         The SMP callback function is registered successfully.
  @retval EFI_ALREADY_STARTED A callback function is already registered on the same attribute
                              opcode and attribute handle, when the Callback is not NULL.
  @retval EFI_NOT_STARTED     A callback function is not registered on the same attribute opcode
                              and attribute handle, when the Callback is NULL.

**/
EFI_STATUS
EFIAPI
BluetoothLeRegisterSmpAuthCallback (
  IN EFI_BLUETOOTH_LE_CONFIG_PROTOCOL  *This,
  IN EFI_BLUETOOTH_LE_SMP_CALLBACK     Callback,
  IN VOID                              *Context
  )
{
  BTHC_DEV            *BtHcDevice;
  BtHcDevice = BTHC_DEV_FROM_BLUETOOTH_LE_CONFIG_PROTOCOL(This);
  if (Callback == NULL) {
    BtHcDevice->LeSmpCallBack = NULL;
    BtHcDevice->LeSmpCallBackContext = NULL;
    return EFI_SUCCESS;
  }
  BtHcDevice->LeSmpCallBack = Callback;
  BtHcDevice->LeSmpCallBackContext = Context;

  return EFI_SUCCESS;
}

/**
  Register a callback function to get SMP related data.

  The RegisterSmpGetDataCallback() function registers a callback function to get SMP related data.

  @param[in]  This            Pointer to the EFI_BLUETOOTH_LE_CONFIG_PROTOCOL instance.
  @param[in]  Callback        Callback function for SMP get data.
  @param[in]  Context         Data passed into Callback function. This is optional parameter and may be NULL.

  @retval EFI_SUCCESS         The SMP get data callback function is registered successfully.
  @retval EFI_ALREADY_STARTED A callback function is already registered on the same attribute
                              opcode and attribute handle, when the Callback is not NULL.
  @retval EFI_NOT_STARTED     A callback function is not registered on the same attribute opcode
                              and attribute handle, when the Callback is NULL
**/
EFI_STATUS
EFIAPI
BluetoothLeConfigRegisterSmpGetDataCallback (
  IN EFI_BLUETOOTH_LE_CONFIG_PROTOCOL                                 *This,
  IN EFI_BLUETOOTH_LE_CONFIG_SMP_GET_DATA_CALLBACK                    Callback,
  IN VOID                                                             *Context
  )
{
  BTHC_DEV            *BtHcDevice;
  BtHcDevice = BTHC_DEV_FROM_BLUETOOTH_LE_CONFIG_PROTOCOL(This);
  if (Callback == NULL) {
    BtHcDevice->LeSmpGetDataCallBack = NULL;
    BtHcDevice->LeSmpGetDataCallBackContext = NULL;
    return EFI_SUCCESS;
  }
  BtHcDevice->LeSmpGetDataCallBack = Callback;
  BtHcDevice->LeSmpGetDataCallBackContext = Context;

  return EFI_SUCCESS;
}

/**
  Register a callback function to set SMP related data.

  The RegisterSmpSetDataCallback() function registers a callback function to set SMP related data.

  @param[in]  This            Pointer to the EFI_BLUETOOTH_LE_CONFIG_PROTOCOL instance.
  @param[in]  Callback        Callback function for SMP set data.
  @param[in]  Context         Data passed into Callback function. This is optional parameter and may be NULL.

  @retval EFI_SUCCESS         The SMP set data callback function is registered successfully.
  @retval EFI_ALREADY_STARTED A callback function is already registered on the same attribute
                              opcode and attribute handle, when the Callback is not NULL.
  @retval EFI_NOT_STARTED     A callback function is not registered on the same attribute opcode
                              and attribute handle, when the Callback is NULL
**/
EFI_STATUS
EFIAPI
BluetoothLeConfigRegisterSmpSetDataCallback (
  IN EFI_BLUETOOTH_LE_CONFIG_PROTOCOL                                    *This,
  IN EFI_BLUETOOTH_LE_CONFIG_SMP_SET_DATA_CALLBACK                       Callback,
  IN VOID                                                                *Context
  )
{
  BTHC_DEV            *BtHcDevice;
  BtHcDevice = BTHC_DEV_FROM_BLUETOOTH_LE_CONFIG_PROTOCOL(This);
  if (Callback == NULL) {
    BtHcDevice->LeSmpSetDataCallBack = NULL;
    BtHcDevice->LeSmpSetDataCallBackContext = NULL;
    return EFI_SUCCESS;
  }
  BtHcDevice->LeSmpSetDataCallBack = Callback;
  BtHcDevice->LeSmpSetDataCallBackContext = Context;

  return EFI_SUCCESS;
}

