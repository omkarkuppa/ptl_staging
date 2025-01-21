/** @file
  VFR file of Bluetooth LE Connection Manager Driver.

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

#ifndef _BLUETOOTH_CONFIG_VFR_H_
#define _BLUETOOTH_CONFIG_VFR_H_

//
// Label Id
//
#define LABEL_BLUETOOTH_HOST_CONTROLLER_LIST           0x1100
#define LABEL_BLUETOOTH_END                            0x11FF
#define LABEL_BLUETOOTH_PAIRED_DEVICE_LIST             0x1200
#define LABEL_BLUETOOTH_PAIRED_END                     0x12FF
#define LABEL_BLUETOOTH_AVAILABLE_DEVICE_LIST          0x1300
#define LABEL_BLUETOOTH_AVAILABLE_END                  0x13FF
#define LABEL_BLUETOOTH_AVAILABLE_SERVICE_LIST         0x1400
#define LABEL_BLUETOOTH_AVAILABLE_SERVICE_END          0x14FF
#define LABEL_BLUETOOTH_SCAN                           0x1500
#define LABEL_BLUETOOTH_SCAN_END                       0x15FF

//
// Question Id
//
#define KEY_ENUM_DEVICE                                0x6000
#define KEY_GOTO_DEVICE_MANAGER                        0x6001
#define KEY_HOST_CONTROLLER_LOCAL_NAME                 0x6002
#define KEY_REFRESH_DEVICE_ID                          0x6003
#define KEY_SCAN_DEVICE_ID                             0x6004

#define HOST_CONTROLLER_LIST_OFFSET                    0x8000
#define DELETE_DEVICE_OFFSET                           0x8100
#define CONNECT_DEVICE_OFFSET                          0x8200
#define DISCONNECT_DEVICE_OFFSET                       0x8300
#define REMOTE_DEVICE_OFFSET                           0x8400

#define DEVICE_LIST_MAX                                0x0100



//
// Form Id
//
#define BLUETOOTH_HOST_CONTROLLER_FORM                 0x0001
#define BLUETOOTH_MANAGEMENT_FORM                      0x0002
#define BLUETOOTH_DEVICE_MANAGEMENT_FORM               0x0003
#define BLUETOOTH_REMOTE_DEVICE_FORM                   0x0004


//
// EFI Variable attributes
//
#define EFI_VARIABLE_NON_VOLATILE       0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS     0x00000004
#define EFI_VARIABLE_READ_ONLY          0x00000008

#endif

