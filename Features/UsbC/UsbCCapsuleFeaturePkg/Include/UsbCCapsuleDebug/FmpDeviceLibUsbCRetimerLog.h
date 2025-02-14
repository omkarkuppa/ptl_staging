/** @file
  Log Event and Log String Definitions - FmpDeviceLibTbtRetimer

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

@par Specification Reference:
**/

#ifndef _USBC_CAPSULE_FMP_TBT_RETIMER_H_
#define _USBC_CAPSULE_FMP_TBT_RETIMER_H_

#include "UsbCCapsuleLogEventsId.h"

///
/// UsbC Retimer Capsule - FmpDeviceGetVersion: Fail to get TcssTbtRetimer variable
///
#define EVT_ARGS_GETVERSION_FAIL_GET_SETUP_VARIABLE            1
#define EVT_CODE_GETVERSION_FAIL_GET_SETUP_VARIABLE            ((EVT_ID_GETVERSION_FAIL_GET_SETUP_VARIABLE << 2) | (EVT_ARGS_GETVERSION_FAIL_GET_SETUP_VARIABLE))
#define STR_ID_GETVERSION_FAIL_GET_SETUP_VARIABLE              "Failed to get UsbCRetimer variable with return Status = (%r).\n"
///
/// UsbC Retimer Capsule - FmpDeviceCheckImageWithStatus: ImageUpdatable is NULL
///
#define EVT_ARGS_CHECKIMAGE_UPDATABLE_NULL                     0
#define EVT_CODE_CHECKIMAGE_UPDATABLE_NULL                     ((EVT_ID_CHECKIMAGE_UPDATABLE_NULL << 2) | (EVT_ARGS_CHECKIMAGE_UPDATABLE_NULL))
#define STR_ID_CHECKIMAGE_UPDATABLE_NULL                       "CheckImage - ImageUpdatable Pointer Parameter is NULL.\n"
///
/// UsbC Retimer Capsule - FmpDeviceCheckImageWithStatus: Image pointer is NULL
///
#define EVT_ARGS_CHECKIMAGE_POINTER_NULL                       0
#define EVT_CODE_CHECKIMAGE_POINTER_NULL                       ((EVT_ID_CHECKIMAGE_POINTER_NULL << 2) | (EVT_ARGS_CHECKIMAGE_POINTER_NULL))
#define STR_ID_CHECKIMAGE_POINTER_NULL                         "CheckImage - Image Pointer Parameter is NULL.\n"
///
/// UsbC Retimer Capsule - FmpDeviceCheckImageWithStatus: Retimer payload signature is not detected
///
#define EVT_ARGS_CHECKIMAGE_PAYLOAD_SIGNATURE_NOT_DETECTED     0
#define EVT_CODE_CHECKIMAGE_PAYLOAD_SIGNATURE_NOT_DETECTED     ((EVT_ID_CHECKIMAGE_PAYLOAD_SIGNATURE_NOT_DETECTED << 2) | (EVT_ARGS_CHECKIMAGE_PAYLOAD_SIGNATURE_NOT_DETECTED))
#define STR_ID_CHECKIMAGE_PAYLOAD_SIGNATURE_NOT_DETECTED       "CheckImage - Retimer payload signature is not detected.\n"
///
/// UsbC Retimer Capsule - FmpDeviceCheckImageWithStatus: Retimer payload header is 0
///
#define EVT_ARGS_CHECKIMAGE_PAYLOAD_HEADER_ZERO                0
#define EVT_CODE_CHECKIMAGE_PAYLOAD_HEADER_ZERO                ((EVT_ID_CHECKIMAGE_PAYLOAD_HEADER_ZERO << 2) | (EVT_ARGS_CHECKIMAGE_PAYLOAD_HEADER_ZERO))
#define STR_ID_CHECKIMAGE_PAYLOAD_HEADER_ZERO                  "CheckImage - RetimerCount in header is 0, nothing to update.\n"
///
/// UsbC Retimer Capsule - FmpDeviceCheckImageWithStatus: Retimer payload size too small
///
#define EVT_ARGS_CHECKIMAGE_PAYLOAD_SIZE_TOO_SMALL             0
#define EVT_CODE_CHECKIMAGE_PAYLOAD_SIZE_TOO_SMALL             ((EVT_ID_CHECKIMAGE_PAYLOAD_SIZE_TOO_SMALL << 2) | (EVT_ARGS_CHECKIMAGE_PAYLOAD_SIZE_TOO_SMALL))
#define STR_ID_CHECKIMAGE_PAYLOAD_SIZE_TOO_SMALL               "CheckImage - Retimer payload size too small.\n"
///
/// UsbC Retimer Capsule - FmpDeviceCheckImageWithStatus: Retimer payload out of bounds
///
#define EVT_ARGS_CHECKIMAGE_PAYLOAD_OUT_BOUNDS                 0
#define EVT_CODE_CHECKIMAGE_PAYLOAD_OUT_BOUNDS                 ((EVT_ID_CHECKIMAGE_PAYLOAD_OUT_BOUNDS << 2) | (EVT_ARGS_CHECKIMAGE_PAYLOAD_OUT_BOUNDS))
#define STR_ID_CHECKIMAGE_PAYLOAD_OUT_BOUNDS                   "CheckImage - Retimer payload is out of bounds.\n"
///
/// UsbC Retimer Capsule - ConstructRetimerInstances: Total Retimer Payload
///
#define EVT_ARGS_CONSTRUCT_INSTANCE_TOTAL_PAYLOAD              1
#define EVT_CODE_CONSTRUCT_INSTANCE_TOTAL_PAYLOAD              ((EVT_ID_CONSTRUCT_INSTANCE_TOTAL_PAYLOAD << 2) | (EVT_ARGS_CONSTRUCT_INSTANCE_TOTAL_PAYLOAD))
#define STR_ID_CONSTRUCT_INSTANCE_TOTAL_PAYLOAD                "Total of Retimer Payload = %d\n"
///
/// UsbC Retimer Capsule - ConstructRetimerInstances: MAX_RETIMER_INSTANCES too small
///
#define EVT_ARGS_CONSTRUCT_INSTANCE_MAX_INSTANCE_TOO_SMALL     1
#define EVT_CODE_CONSTRUCT_INSTANCE_MAX_INSTANCE_TOO_SMALL     ((EVT_ID_CONSTRUCT_INSTANCE_MAX_INSTANCE_TOO_SMALL << 2) | (EVT_ARGS_CONSTRUCT_INSTANCE_MAX_INSTANCE_TOO_SMALL))
#define STR_ID_CONSTRUCT_INSTANCE_MAX_INSTANCE_TOO_SMALL       "MAX_RETIMER_INSTANCES(%d) too small\n"
///
/// UsbC Retimer Capsule - ConstructRetimerInstances: Retimer Device Address - Bus/Device
///
#define EVT_ARGS_CONSTRUCT_INSTANCE_ADDRESS_BUS_DEVICE         2
#define EVT_CODE_CONSTRUCT_INSTANCE_ADDRESS_BUS_DEVICE         ((EVT_ID_CONSTRUCT_INSTANCE_ADDRESS_BUS_DEVICE << 2) | (EVT_ARGS_CONSTRUCT_INSTANCE_ADDRESS_BUS_DEVICE))
#define STR_ID_CONSTRUCT_INSTANCE_ADDRESS_BUS_DEVICE           "PayLoad - Integrated TBT retimer address B%d D%d"
///
/// UsbC Retimer Capsule - ConstructRetimerInstances: Retimer Device Address - Function/Port
///
#define EVT_ARGS_CONSTRUCT_INSTANCE_ADDRESS_FUNC               1
#define EVT_CODE_CONSTRUCT_INSTANCE_ADDRESS_FUNC               ((EVT_ID_CONSTRUCT_INSTANCE_ADDRESS_FUNC << 2) | (EVT_ARGS_CONSTRUCT_INSTANCE_ADDRESS_FUNC))
#define STR_ID_CONSTRUCT_INSTANCE_ADDRESS_FUNC                 " F%d,"
///
/// UsbC Retimer Capsule - ConstructRetimerInstances: Retimer Device Address - PORT/Index
///
#define EVT_ARGS_CONSTRUCT_INSTANCE_ADDRESS_PORT_INDEX         2
#define EVT_CODE_CONSTRUCT_INSTANCE_ADDRESS_PORT_INDEX         ((EVT_ID_CONSTRUCT_INSTANCE_ADDRESS_PORT_INDEX << 2) | (EVT_ARGS_CONSTRUCT_INSTANCE_ADDRESS_PORT_INDEX))
#define STR_ID_CONSTRUCT_INSTANCE_ADDRESS_PORT_INDEX           " Tbt Port %d, Retimer Index %d\n"
///
/// UsbC Retimer Capsule - ConstructRetimerInstances: PCIE RP Type and RP number
///
#define EVT_ARGS_CONSTRUCT_INSTANCE_PCIE_TYPE_RP               2
#define EVT_CODE_CONSTRUCT_INSTANCE_PCIE_TYPE_RP               ((EVT_ID_CONSTRUCT_INSTANCE_PCIE_TYPE_RP << 2) | (EVT_ARGS_CONSTRUCT_INSTANCE_PCIE_TYPE_RP))
#define STR_ID_CONSTRUCT_INSTANCE_PCIE_TYPE_RP                 "PayLoad - Discrete TBT retimer's Pcie root port type value %x, root port number %d,"
///
/// UsbC Retimer Capsule - ConstructRetimerInstances: Un-Support Retimer FirmwareType
///
#define EVT_ARGS_CONSTRUCT_INSTANCE_FIRMWARETYPE_ERROR         1
#define EVT_CODE_CONSTRUCT_INSTANCE_FIRMWARETYPE_ERROR         ((EVT_ID_CONSTRUCT_INSTANCE_FIRMWARETYPE_ERROR << 2) | (EVT_ARGS_CONSTRUCT_INSTANCE_FIRMWARETYPE_ERROR))
#define STR_ID_CONSTRUCT_INSTANCE_FIRMWARETYPE_ERROR           "PayLoad - Update failed on Un-Support FirmwareType value %d\n"
///
/// UsbC Retimer Capsule - ConstructRetimerInstances: Create Retimer Instance fail
///
#define EVT_ARGS_CONSTRUCT_INSTANCE_CREATE_RETIMER_FAIL        2
#define EVT_CODE_CONSTRUCT_INSTANCE_CREATE_RETIMER_FAIL        ((EVT_ID_CONSTRUCT_INSTANCE_CREATE_RETIMER_FAIL << 2) | (EVT_ARGS_CONSTRUCT_INSTANCE_CREATE_RETIMER_FAIL))
#define STR_ID_CONSTRUCT_INSTANCE_CREATE_RETIMER_FAIL          "CreateRetimerDevInstance failed (%r) at instance index %d\n"
///
/// UsbC Retimer Capsule - ConstructRetimerInstances: CreateHrDevInstance failed (%r) at Host Router instance index %d
///
#define EVT_ARGS_CONSTRUCT_INSTANCE_CREATE_HR_FAIL             2
#define EVT_CODE_CONSTRUCT_INSTANCE_CREATE_HR_FAIL             ((EVT_ID_CONSTRUCT_INSTANCE_CREATE_HR_FAIL << 2) | (EVT_ARGS_CONSTRUCT_INSTANCE_CREATE_HR_FAIL))
#define STR_ID_CONSTRUCT_INSTANCE_CREATE_HR_FAIL               "CreateHrDevInstance failed (%r) at Host Router instance index %d\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: FmpDeviceSetImageWithStatus (UsbC Retimer) - Start
///
#define EVT_ARGS_FMP_UPDATE_START                              0
#define EVT_CODE_FMP_UPDATE_START                              ((EVT_ID_FMP_UPDATE_START << 2) | (EVT_ARGS_FMP_UPDATE_START))
#define STR_ID_FMP_UPDATE_START                                "FmpDeviceSetImageWithStatus (UsbC Retimer) - Start\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Invalid progress callback
///
#define EVT_ARGS_FMP_UPDATE_PROGRESS_NULL                      0
#define EVT_CODE_FMP_UPDATE_PROGRESS_NULL                      ((EVT_ID_FMP_UPDATE_PROGRESS_NULL << 2) | (EVT_ARGS_FMP_UPDATE_PROGRESS_NULL))
#define STR_ID_FMP_UPDATE_PROGRESS_NULL                        "UsbC Retimer Capsule - Invalid progress callback\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Progress Callback failed with Status %r
///
#define EVT_ARGS_FMP_UPDATE_PROGRESS_FAIL                      1
#define EVT_CODE_FMP_UPDATE_PROGRESS_FAIL                      ((EVT_ID_FMP_UPDATE_PROGRESS_FAIL << 2) | (EVT_ARGS_FMP_UPDATE_PROGRESS_FAIL))
#define STR_ID_FMP_UPDATE_PROGRESS_FAIL                        "UsbC Retimer Capsule - Progress Callback failed with Status %r.\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Failed to locate TcssRetimerProtocol
///
#define EVT_ARGS_FMP_UPDATE_LOCATE_USBCRETIMER_PROTOCOL_FAIL   1
#define EVT_CODE_FMP_UPDATE_LOCATE_USBCRETIMER_PROTOCOL_FAIL   ((EVT_ID_FMP_UPDATE_LOCATE_USBCRETIMER_PROTOCOL_FAIL << 2) | (EVT_ARGS_FMP_UPDATE_LOCATE_USBCRETIMER_PROTOCOL_FAIL))
#define STR_ID_FMP_UPDATE_LOCATE_USBCRETIMER_PROTOCOL_FAIL     "UsbC Retimer Capsule - Failed to locate UsbCRetimerProtocol (%r).\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Failed to locate UsbCProgressCodeProtocol
///
#define EVT_ARGS_FMP_UPDATE_LOCATE_USBCPROGRESS_PROTOCOL_FAIL  1
#define EVT_CODE_FMP_UPDATE_LOCATE_USBCPROGRESS_PROTOCOL_FAIL  ((EVT_ID_FMP_UPDATE_LOCATE_USBCPROGRESS_PROTOCOL_FAIL << 2) | (EVT_ARGS_FMP_UPDATE_LOCATE_USBCPROGRESS_PROTOCOL_FAIL))
#define STR_ID_FMP_UPDATE_LOCATE_USBCPROGRESS_PROTOCOL_FAIL    "UsbC Retimer Capsule - Failed to locate UsbCProgressCodeProtocol (%r).\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Failed to ConstructRetimerInstances(%r). LastAttemptStatus(%d)
///
#define EVT_ARGS_FMP_UPDATE_CONSTRUCT_RETIMER_INSTANCE_FAILED  2
#define EVT_CODE_FMP_UPDATE_CONSTRUCT_RETIMER_INSTANCE_FAILED  ((EVT_ID_FMP_UPDATE_CONSTRUCT_RETIMER_INSTANCE_FAILED << 2) | (EVT_ARGS_FMP_UPDATE_CONSTRUCT_RETIMER_INSTANCE_FAILED))
#define STR_ID_FMP_UPDATE_CONSTRUCT_RETIMER_INSTANCE_FAILED    "UsbC Retimer Capsule - Failed to ConstructRetimerInstances(%r). LastAttemptStatus(%d)\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: DisconnectController gEfiUsb2HcProtocolGuid
///
#define EVT_ARGS_FMP_UPDATE_DISCONNECT_CONTROLLER_START        0
#define EVT_CODE_FMP_UPDATE_DISCONNECT_CONTROLLER_START        ((EVT_ID_FMP_UPDATE_DISCONNECT_CONTROLLER_START << 2) | (EVT_ARGS_FMP_UPDATE_DISCONNECT_CONTROLLER_START))
#define STR_ID_FMP_UPDATE_DISCONNECT_CONTROLLER_START          "UsbC Retimer Capsule - DisconnectController gEfiUsb2HcProtocolGuid\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: DisconnectController gEfiUsb2HcProtocolGuid HandleIndex
///
#define EVT_ARGS_FMP_UPDATE_DISCONNECT_CONTROLLER_HANDLEINDEX   1
#define EVT_CODE_FMP_UPDATE_DISCONNECT_CONTROLLER_HANDLEINDEX   ((EVT_ID_FMP_UPDATE_DISCONNECT_CONTROLLER_HANDLEINDEX << 2) | (EVT_ARGS_FMP_UPDATE_DISCONNECT_CONTROLLER_HANDLEINDEX))
#define STR_ID_FMP_UPDATE_DISCONNECT_CONTROLLER_HANDLEINDEX     "UsbC Retimer Capsule - DisconnectController gEfiUsb2HcProtocolGuid HandleIndex %d\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: TbtSendOfflineMode failed
///
#define EVT_ARGS_FMP_UPDATE_SEND_OFFLINEMODE_FAIL               2
#define EVT_CODE_FMP_UPDATE_SEND_OFFLINEMODE_FAIL               ((EVT_ID_FMP_UPDATE_SEND_OFFLINEMODE_FAIL << 2) | (EVT_ARGS_FMP_UPDATE_SEND_OFFLINEMODE_FAIL))
#define STR_ID_FMP_UPDATE_SEND_OFFLINEMODE_FAIL                 "UsbC Retimer Capsule - TbtSendOfflineMode failed (%r) on HR instance index %d\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Drive all TBT ports to TBT mode...
///
#define EVT_ARGS_FMP_UPDATE_DRIVE_TBTMODE_START                 0
#define EVT_CODE_FMP_UPDATE_DRIVE_TBTMODE_START                 ((EVT_ID_FMP_UPDATE_DRIVE_TBTMODE_START << 2) | (EVT_ARGS_FMP_UPDATE_DRIVE_TBTMODE_START))
#define STR_ID_FMP_UPDATE_DRIVE_TBTMODE_START                   "UsbC Retimer Capsule - Drive all TBT ports to TBT mode...\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Failed to drive ports to TBT mode
///
#define EVT_ARGS_FMP_UPDATE_DRIVE_TBTMODE_FAIL                  1
#define EVT_CODE_FMP_UPDATE_DRIVE_TBTMODE_FAIL                  ((EVT_ID_FMP_UPDATE_DRIVE_TBTMODE_FAIL << 2) | (EVT_ARGS_FMP_UPDATE_DRIVE_TBTMODE_FAIL))
#define STR_ID_FMP_UPDATE_DRIVE_TBTMODE_FAIL                    "UsbC Retimer Capsule - Failed to drive ports to TBT mode (%r).\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Time delay for Retimer device ready
///
#define EVT_ARGS_FMP_UPDATE_WAIT_FOR_RETIMER_DEVICE             1
#define EVT_CODE_FMP_UPDATE_WAIT_FOR_RETIMER_DEVICE             ((EVT_ID_FMP_UPDATE_WAIT_FOR_RETIMER_DEVICE << 2) | (EVT_ARGS_FMP_UPDATE_WAIT_FOR_RETIMER_DEVICE))
#define STR_ID_FMP_UPDATE_WAIT_FOR_RETIMER_DEVICE               "Time delay for Retimer device ready = %d(MicroSecond)\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Assert Retimer ForcePower GPIO
///
#define EVT_ARGS_FMP_UPDATE_ASSERT_RETIMER_FP_GPIO              0
#define EVT_CODE_FMP_UPDATE_ASSERT_RETIMER_FP_GPIO              ((EVT_ID_FMP_UPDATE_ASSERT_RETIMER_FP_GPIO  << 2) | (EVT_ARGS_FMP_UPDATE_ASSERT_RETIMER_FP_GPIO))
#define STR_ID_FMP_UPDATE_ASSERT_RETIMER_FP_GPIO                "UsbC Retimer Capsule - Assert Retimer ForcePower GPIO\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Failed to assert Retimer Force Power GPIO
///
#define EVT_ARGS_FMP_UPDATE_ASSERT_RETIMER_FP_GPIO_FAILED       1
#define EVT_CODE_FMP_UPDATE_ASSERT_RETIMER_FP_GPIO_FAILED       ((EVT_ID_FMP_UPDATE_ASSERT_RETIMER_FP_GPIO_FAILED   << 2) | (EVT_ARGS_FMP_UPDATE_ASSERT_RETIMER_FP_GPIO_FAILED ))
#define STR_ID_FMP_UPDATE_ASSERT_RETIMER_FP_GPIO_FAILED         "UsbC Retimer Capsule - Failed to assert Retimer Force Power GPIO (%d).\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Update TBT Retimer - Start
///
#define EVT_ARGS_FMP_UPDATE_RETIMER_START                       0
#define EVT_CODE_FMP_UPDATE_RETIMER_START                       ((EVT_ID_FMP_UPDATE_RETIMER_START << 2) | (EVT_ARGS_FMP_UPDATE_RETIMER_START))
#define STR_ID_FMP_UPDATE_RETIMER_START                         "UsbC Retimer Capsule - Update TBT Retimer - Start\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Total of Retimer payload
///
#define EVT_ARGS_FMP_UPDATE_TOTAL_RETIMER_PAYLOAD               1
#define EVT_CODE_FMP_UPDATE_TOTAL_RETIMER_PAYLOAD               ((EVT_ID_FMP_UPDATE_TOTAL_RETIMER_PAYLOAD << 2) | (EVT_ARGS_FMP_UPDATE_TOTAL_RETIMER_PAYLOAD))
#define STR_ID_FMP_UPDATE_TOTAL_RETIMER_PAYLOAD                 "UsbC Retimer Capsule - Total of Retimer Payload = %d\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Update Retimer payload
///
#define EVT_ARGS_FMP_UPDATE_UPDATE_RETIMER_PAYLOAD              1
#define EVT_CODE_FMP_UPDATE_UPDATE_RETIMER_PAYLOAD              ((EVT_ID_FMP_UPDATE_UPDATE_RETIMER_PAYLOAD << 2) | (EVT_ARGS_FMP_UPDATE_UPDATE_RETIMER_PAYLOAD))
#define STR_ID_FMP_UPDATE_UPDATE_RETIMER_PAYLOAD                "UsbC Retimer Capsule - Update Retimer Payload = %d\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: ImageOffset=0x%x, size=0x%x
///
#define EVT_ARGS_FMP_UPDATE_PAYLOAD_OFFSET_SIZE                 2
#define EVT_CODE_FMP_UPDATE_PAYLOAD_OFFSET_SIZE                 ((EVT_ID_FMP_UPDATE_PAYLOAD_OFFSET_SIZE << 2) | (EVT_ARGS_FMP_UPDATE_PAYLOAD_OFFSET_SIZE))
#define STR_ID_FMP_UPDATE_PAYLOAD_OFFSET_SIZE                   "UsbC Retimer Capsule - ImageOffset=0x%x, size=0x%x\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: InitRetimerHW Fail
///
#define EVT_ARGS_FMP_UPDATE_INIT_RETIMER_HW_FAIL                1
#define EVT_CODE_FMP_UPDATE_INIT_RETIMER_HW_FAIL                ((EVT_ID_FMP_UPDATE_INITRETIMERHW_FAIL << 2) | (EVT_ARGS_FMP_UPDATE_INIT_RETIMER_HW_FAIL))
#define STR_ID_FMP_UPDATE_INIT_RETIMER_HW_FAIL                  "UsbC Retimer Capsule - InitRetimerHW Fail (%r)\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Retimer version after the update is 0x%x (%r)
///
#define EVT_ARGS_FMP_UPDATE_NVM_VERSION                         2
#define EVT_CODE_FMP_UPDATE_NVM_VERSION                         ((EVT_ID_FMP_UPDATE_NVM_VERSION << 2) | (EVT_ARGS_FMP_UPDATE_NVM_VERSION))
#define STR_ID_FMP_UPDATE_NVM_VERSION                           "UsbC Retimer Capsule - Retimer version after the update is 0x%x (%r) "
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: at image index %d
///
#define EVT_ARGS_FMP_UPDATE_NVM_VERSION1                        1
#define EVT_CODE_FMP_UPDATE_NVM_VERSION1                        ((EVT_ID_FMP_UPDATE_NVM_VERSION1 << 2) | (EVT_ARGS_FMP_UPDATE_NVM_VERSION1))
#define STR_ID_FMP_UPDATE_NVM_VERSION1                          "at image index %d\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: TerminateRetimerHW fail :%r
///
#define EVT_ARGS_FMP_UPDATE_TERMINATE_RETIMER_HW_FAIL           1
#define EVT_CODE_FMP_UPDATE_TERMINATE_RETIMER_HW_FAIL           ((EVT_ID_FMP_UPDATE_TERMINATE_RETIMER_HW_FAIL << 2) | (EVT_ARGS_FMP_UPDATE_TERMINATE_RETIMER_HW_FAIL))
#define STR_ID_FMP_UPDATE_TERMINATE_RETIMER_HW_FAIL             "UsbC Retimer Capsule - TerminateRetimerHW fail :%r\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: De-Assert Retimer ForcePower GPIO
///
#define EVT_ARGS_FMP_UPDATE_DEASSERT_RETIMER_FP_GPIO            0
#define EVT_CODE_FMP_UPDATE_DEASSERT_RETIMER_FP_GPIO            ((EVT_ID_FMP_UPDATE_DEASSERT_RETIMER_FP_GPIO  << 2) | (EVT_ARGS_FMP_UPDATE_DEASSERT_RETIMER_FP_GPIO))
#define STR_ID_FMP_UPDATE_DEASSERT_RETIMER_FP_GPIO              "UsbC Retimer Capsule - De-assert Retimer ForcePower GPIO\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Failed to de-assert Retimer Force Power GPIO
///
#define EVT_ARGS_FMP_UPDATE_DEASSERT_RETIMER_FP_GPIO_FAILED     1
#define EVT_CODE_FMP_UPDATE_DEASSERT_RETIMER_FP_GPIO_FAILED     ((EVT_ID_FMP_UPDATE_DEASSERT_RETIMER_FP_GPIO_FAILED   << 2) | (EVT_ARGS_FMP_UPDATE_DEASSERT_RETIMER_FP_GPIO_FAILED ))
#define STR_ID_FMP_UPDATE_DEASSERT_RETIMER_FP_GPIO_FAILED       "UsbC Retimer Capsule - Failed to de-assert Retimer Force Power GPIO (%r).\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: Restore to TBT ports to original mode (%r)
///
#define EVT_ARGS_FMP_UPDATE_RESTORE_TBTMODE_FAIL                1
#define EVT_CODE_FMP_UPDATE_RESTORE_TBTMODE_FAIL                ((EVT_ID_FMP_UPDATE_RESTORE_TBTMODE_FAIL   << 2) | (EVT_ARGS_FMP_UPDATE_RESTORE_TBTMODE_FAIL ))
#define STR_ID_FMP_UPDATE_RESTORE_TBTMODE_FAIL                  "UsbC Retimer Capsule - Restore to TBT ports to original mode (%r).\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: connect Controller gEfiUsb2HcProtocolGuid
///
#define EVT_ARGS_FMP_UPDATE_CONNECT_CONTROLLER_USB2HC           0
#define EVT_CODE_FMP_UPDATE_CONNECT_CONTROLLER_USB2HC           ((EVT_ID_FMP_UPDATE_CONNECT_CONTROLLER_USB2HC << 2) | (EVT_ARGS_FMP_UPDATE_CONNECT_CONTROLLER_USB2HC))
#define STR_ID_FMP_UPDATE_CONNECT_CONTROLLER_USB2HC             "UsbC Retimer Capsule - Connect Controller gEfiUsb2HcProtocolGuid\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: DestroyRetimerDevInstance: Retimer Instance %x
///
#define EVT_ARGS_FMP_UPDATE_DESTORY_RETIMER_INSTANCE            1
#define EVT_CODE_FMP_UPDATE_DESTORY_RETIMER_INSTANCE            ((EVT_ID_FMP_UPDATE_DESTORY_RETIMER_INSTANCE  << 2) | (EVT_ARGS_FMP_UPDATE_DESTORY_RETIMER_INSTANCE))
#define STR_ID_FMP_UPDATE_DESTORY_RETIMER_INSTANCE              "UsbC Retimer Capsule - DestroyRetimerDevInstance: Retimer Instance %x\n"
///
/// UsbC Retimer Capsule - FmpDeviceSetImageWithStatus: FmpDeviceSetImageWithStatus (UsbC Retimer) - End
///
#define EVT_ARGS_FMP_UPDATE_END                                 0
#define EVT_CODE_FMP_UPDATE_END                                 ((EVT_ID_FMP_UPDATE_END << 2) | (EVT_ARGS_FMP_UPDATE_END))
#define STR_ID_FMP_UPDATE_END                                   "FmpDeviceSetImageWithStatus (UsbC Retimer) - End\n"
///
/// UsbC Retimer Capsule - FmpDeviceLock() for system FLASH
///
#define EVT_ARGS_FMP_DEVICE_LOCK                                0
#define EVT_CODE_FMP_DEVICE_LOCK                                ((EVT_ID_FMP_DEVICE_LOCK << 2) | (EVT_ARGS_FMP_DEVICE_LOCK))
#define STR_ID_FMP_DEVICE_LOCK                                  "FmpDeviceLib(TBT Retimer): FmpDeviceLock() for system FLASH\n"
#endif
