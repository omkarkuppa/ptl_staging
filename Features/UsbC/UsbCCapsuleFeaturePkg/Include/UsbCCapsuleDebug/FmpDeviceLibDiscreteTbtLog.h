/** @file
  Log Event and Log String Definitions - FmpDeviceLibDiscreteTbt

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

#ifndef _USBC_CAPSULE_FMP_DISCRETE_TBT_H_
#define _USBC_CAPSULE_FMP_DISCRETE_TBT_H_

#include "UsbCCapsuleLogEventsId.h"

///
/// FmpDeviceCheckImageWithStatus: ImageUpdatable is NULL
///
#define EVT_ARGS_DTBT_CHECKIMAGE_UPDATABLE_NULL                0
#define EVT_CODE_DTBT_CHECKIMAGE_UPDATABLE_NULL                ((EVT_ID_DTBT_CHECKIMAGE_UPDATABLE_NULL << 2) | (EVT_ARGS_DTBT_CHECKIMAGE_UPDATABLE_NULL))
#define STR_ID_DTBT_CHECKIMAGE_UPDATABLE_NULL                  "CheckImage - ImageUpdatable Pointer Parameter is NULL.\n"
///
/// FmpDeviceCheckImageWithStatus: Image pointer is NULL
///
#define EVT_ARGS_DTBT_CHECKIMAGE_POINTER_NULL                  0
#define EVT_CODE_DTBT_CHECKIMAGE_POINTER_NULL                  ((EVT_ID_DTBT_CHECKIMAGE_POINTER_NULL << 2) | (EVT_ARGS_DTBT_CHECKIMAGE_POINTER_NULL))
#define STR_ID_DTBT_CHECKIMAGE_POINTER_NULL                    "CheckImage - Image Pointer Parameter is NULL.\n"
///
/// FmpDeviceCheckImageWithStatus: Discrete TBT payload signature is not detected
///
#define EVT_ARGS_DTBT_CHECKIMAGE_PAYLOAD_SIGNATURE_UNDETECTED  0
#define EVT_CODE_DTBT_CHECKIMAGE_PAYLOAD_SIGNATURE_UNDETECTED  ((EVT_ID_DTBT_CHECKIMAGE_PAYLOAD_SIGNATURE_UNDETECTED << 2) | (EVT_ARGS_DTBT_CHECKIMAGE_PAYLOAD_SIGNATURE_UNDETECTED))
#define STR_ID_DTBT_CHECKIMAGE_PAYLOAD_SIGNATURE_UNDETECTED    "CheckImage - Discrete TBT payload signature is not detected.\n"
///
/// FmpDeviceCheckImageWithStatus: DiscreteTbtCount in header is 0
///
#define EVT_ARGS_DTBT_CHECKIMAGE_PAYLOAD_HEADER_ZERO           0
#define EVT_CODE_DTBT_CHECKIMAGE_PAYLOAD_HEADER_ZERO           ((EVT_ID_DTBT_CHECKIMAGE_PAYLOAD_HEADER_ZERO << 2) | (EVT_ARGS_DTBT_CHECKIMAGE_PAYLOAD_HEADER_ZERO))
#define STR_ID_DTBT_CHECKIMAGE_PAYLOAD_HEADER_ZERO             "CheckImage - DiscreteTbtCount in header is 0, nothing to update.\n"
///
/// FmpDeviceCheckImageWithStatus: Current BIOS only supports a single Discrete TBT update
///
#define EVT_ARGS_DTBT_CHECKIMAGE_PAYLOAD_COUNT_INVALID         0
#define EVT_CODE_DTBT_CHECKIMAGE_PAYLOAD_COUNT_INVALID         ((EVT_ID_DTBT_CHECKIMAGE_CHECKIMAGE_PAYLOAD_COUNT_INVALID << 2) | (EVT_ARGS_DTBT_CHECKIMAGE_PAYLOAD_COUNT_INVALID))
#define STR_ID_DTBT_CHECKIMAGE_PAYLOAD_COUNT_INVALID           "Current BIOS only supports a single Discrete TBT update.\n"
///
/// FmpDeviceCheckImageWithStatus: DiscreteTbt payload size too small
///
#define EVT_ARGS_DTBT_CHECKIMAGE_PAYLOAD_SIZE_TOO_SMALL        0
#define EVT_CODE_DTBT_CHECKIMAGE_PAYLOAD_SIZE_TOO_SMALL        ((EVT_ID_DTBT_CHECKIMAGE_CHECKIMAGE_PAYLOAD_SIZE_TOO_SMALL << 2) | (EVT_ARGS_DTBT_CHECKIMAGE_PAYLOAD_SIZE_TOO_SMALL))
#define STR_ID_DTBT_CHECKIMAGE_PAYLOAD_SIZE_TOO_SMALL          "CheckImage - DiscreteTbt payload size too small.\n"
///
/// FmpDeviceCheckImageWithStatus: DiscreteTbt payload size too small
///
#define EVT_ARGS_DTBT_CHECKIMAGE_PAYLOAD_OUT_BOUNDS            0
#define EVT_CODE_DTBT_CHECKIMAGE_PAYLOAD_OUT_BOUNDS            ((EVT_ID_DTBT_CHECKIMAGE_CHECKIMAGE_PAYLOAD_OUT_BOUNDS << 2) | (EVT_ARGS_DTBT_CHECKIMAGE_PAYLOAD_OUT_BOUNDS))
#define STR_ID_DTBT_CHECKIMAGE_PAYLOAD_OUT_BOUNDS              "CheckImage - DiscreteTbtCount payload is out of bounds.\n"
///
/// FmpDeviceSetImageWithStatus: FmpDeviceSetImageWithStatus (TCSS Discrete TBT) - Start
///
#define EVT_ARGS_DTBT_FMP_UPDATE_START                         0
#define EVT_CODE_DTBT_FMP_UPDATE_START                         ((EVT_ID_DTBT_FMP_UPDATE_START << 2) | (EVT_ARGS_DTBT_FMP_UPDATE_START))
#define STR_ID_DTBT_FMP_UPDATE_START                           "FmpDeviceSetImageWithStatus (Discrete TBT) - Start.\n"
///
/// FmpDeviceSetImageWithStatus: Invalid progress callback
///
#define EVT_ARGS_DTBT_FMP_UPDATE_PROGRESS_NULL                 0
#define EVT_CODE_DTBT_FMP_UPDATE_PROGRESS_NULL                 ((EVT_ID_DTBT_FMP_UPDATE_PROGRESS_NULL << 2) | (EVT_ARGS_DTBT_FMP_UPDATE_PROGRESS_NULL))
#define STR_ID_DTBT_FMP_UPDATE_PROGRESS_NULL                   "UsbC dTBT Capsule - Invalid progress callback.\n"
///
/// FmpDeviceSetImageWithStatus: Progress Callback failed with Status
///
#define EVT_ARGS_DTBT_FMP_UPDATE_PROGRESS_FAIL                 1
#define EVT_CODE_DTBT_FMP_UPDATE_PROGRESS_FAIL                 ((EVT_ID_DTBT_FMP_UPDATE_PROGRESS_FAIL << 2) | (EVT_ARGS_DTBT_FMP_UPDATE_PROGRESS_FAIL))
#define STR_ID_DTBT_FMP_UPDATE_PROGRESS_FAIL                   "UsbC dTBT Capsule - Progress Callback failed with Status %r.\n"
///
/// FmpDeviceSetImageWithStatus: Update Discrete TBT  - Start
///
#define EVT_ARGS_DTBT_FMP_UPDATE_DTBT_START                    0
#define EVT_CODE_DTBT_FMP_UPDATE_DTBT_START                    ((EVT_ID_DTBT_FMP_UPDATE_DTBT_START << 2) | (EVT_ARGS_DTBT_FMP_UPDATE_DTBT_START))
#define STR_ID_DTBT_FMP_UPDATE_DTBT_START                      "UsbC dTBT Capsule - Update Start.\n"
///
/// FmpDeviceSetImageWithStatus: Discrete TBT payload is out of bounds
///
#define EVT_ARGS_DTBT_PAYLOAD_OUT_BOUNDS                       0
#define EVT_CODE_DTBT_PAYLOAD_OUT_BOUNDS                       ((EVT_ID_DTBT_PAYLOAD_OUT_BOUNDS << 2) | (EVT_ARGS_DTBT_PAYLOAD_OUT_BOUNDS))
#define STR_ID_DTBT_PAYLOAD_OUT_BOUNDS                         "UsbC dTBT Capsule - Payload is out of bounds.\n"
///
/// FmpDeviceSetImageWithStatus: Update DiscreteTbt (%d/%d)
///
#define EVT_ARGS_DTBT_UPDATE                                   2
#define EVT_CODE_DTBT_UPDATE                                   ((EVT_ID_DTBT_UPDATE << 2) | (EVT_ARGS_DTBT_UPDATE))
#define STR_ID_DTBT_UPDATE                                     "UsbC dTBT Capsule - Update DiscreteTbt (%d/%d).\n"
///
/// FmpDeviceSetImageWithStatus: ImageOffset=0x%x, size=0x%x
///
#define EVT_ARGS_DTBT_PAYLOAD_OFFSET_SIZE                      2
#define EVT_CODE_DTBT_PAYLOAD_OFFSET_SIZE                      ((EVT_ID_DTBT_PAYLOAD_OFFSET_SIZE << 2) | (EVT_ARGS_DTBT_PAYLOAD_OFFSET_SIZE))
#define STR_ID_DTBT_PAYLOAD_OFFSET_SIZE                        "UsbC dTBT Capsule - ImageOffset=0x%x, size=0x%x\n"
///
/// FmpDeviceSetImageWithStatus: DisconnectController gEfiUsb2HcProtocolGuid
///
#define EVT_ARGS_DTBT_DISCONNECT_CONTROLLER                    0
#define EVT_CODE_DTBT_DISCONNECT_CONTROLLER                    ((EVT_ID_DTBT_DISCONNECT_CONTROLLER << 2) | (EVT_ARGS_DTBT_DISCONNECT_CONTROLLER))
#define STR_ID_DTBT_DISCONNECT_CONTROLLER                      "UsbC dTBT Capsule - DisconnectController gEfiUsb2HcProtocolGuid.\n"
///
/// FmpDeviceSetImageWithStatus: PayLoad - Discrete TBT's Pcie root port type value %x, root port number %d
///
#define EVT_ARGS_DTBT_PCIE_TYPE_RP                             2
#define EVT_CODE_DTBT_PCIE_TYPE_RP                             ((EVT_ID_DTBT_PCIE_TYPE_RP << 2) | (EVT_ARGS_DTBT_PCIE_TYPE_RP))
#define STR_ID_DTBT_PCIE_TYPE_RP                               "UsbC dTBT Capsule - Discrete TBT's Pcie root port type value %x, root port number %d.\n"
///
/// FmpDeviceSetImageWithStatus: PayLoad - Update failed on un-support FirmwareType value %d
///
#define EVT_ARGS_DTBT_FIRMWARETYPE_ERROR                       1
#define EVT_CODE_DTBT_FIRMWARETYPE_ERROR                       ((EVT_ID_DTBT_FIRMWARETYPE_ERROR << 2) | (EVT_ARGS_DTBT_FIRMWARETYPE_ERROR))
#define STR_ID_DTBT_FIRMWARETYPE_ERROR                         "UsbC dTBT Capsule - Update failed on un-support FirmwareType value %d.\n"
///
/// UsbC dTBT Capsule - Create dTBT device instance
///
#define EVT_ARGS_DTBT_CREATE_DEV_INST                          0
#define EVT_CODE_DTBT_CREATE_DEV_INST                          ((EVT_ID_DTBT_CREATE_DEV_INST << 2) | (EVT_ARGS_DTBT_CREATE_DEV_INST))
#define STR_ID_DTBT_CREATE_DEV_INST                            "UsbC dTBT Capsule - Create dTBT device instance\n"
///
/// FmpDeviceSetImageWithStatus: Discrete TBT initialization failed (%r) at image index %d
///
#define EVT_ARGS_DTBT_INITIALIZE_FAIL                          2
#define EVT_CODE_DTBT_INITIALIZE_FAIL                          ((EVT_ID_DTBT_INITIALIZE_FAIL << 2) | (EVT_ARGS_DTBT_INITIALIZE_FAIL))
#define STR_ID_DTBT_INITIALIZE_FAIL                            "UsbC dTBT Capsule - Initialization failed (%r) at image index %d.\n"
///
/// UsbC dTBT Capsule - Update dTBT NVM firmware
///
#define EVT_ARGS_DTBT_UPDATE_NVM_FW                            0
#define EVT_CODE_DTBT_UPDATE_NVM_FW                            ((EVT_ID_DTBT_UPDATE_NVM_FW << 2) | (EVT_ARGS_DTBT_UPDATE_NVM_FW))
#define STR_ID_DTBT_UPDATE_NVM_FW                              "UsbC dTBT Capsule - Update dTBT NVM Firmware\n"
///
/// FmpDeviceSetImageWithStatus: Discrete TBT update failed (%r) at image index %d
///
#define EVT_ARGS_DTBT_INSTANCE_UPDATE_FAIL                     2
#define EVT_CODE_DTBT_INSTANCE_UPDATE_FAIL                     ((EVT_ID_DTBT_INSTANCE_UPDATE_FAIL << 2) | (EVT_ARGS_DTBT_INSTANCE_UPDATE_FAIL))
#define STR_ID_DTBT_INSTANCE_UPDATE_FAIL                       "UsbC dTBT Capsule - Update failed (%r) at image index %d.\n"
///
/// FmpDeviceSetImageWithStatus: FmpDeviceSetImageWithStatus (Discrete TBT) - %r
///
#define EVT_ARGS_DTBT_FMP_UPDATE_END                           0
#define EVT_CODE_DTBT_FMP_UPDATE_END                           ((EVT_ID_DTBT_FMP_UPDATE_END << 2) | (EVT_ARGS_DTBT_FMP_UPDATE_END))
#define STR_ID_DTBT_FMP_UPDATE_END                             "FmpDeviceSetImageWithStatus (Discrete TBT) - End\n"
///
/// FmpDeviceLock: FmpDeviceLib(Discrete TBT): FmpDeviceLock() for system FLASH
///
#define EVT_ARGS_DTBT_FMP_DEVICE_LOCK                          0
#define EVT_CODE_DTBT_FMP_DEVICE_LOCK                          ((EVT_ID_DTBT_FMP_DEVICE_LOCK  << 2) | (EVT_ARGS_DTBT_FMP_DEVICE_LOCK))
#define STR_ID_DTBT_FMP_DEVICE_LOCK                            "FmpDeviceLib(Discrete TBT): FmpDeviceLock() for system FLASH.\n"

#endif
