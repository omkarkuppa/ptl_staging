/** @file
  Log Event and Log String Definitions - FmpDeviceLibPdBridge

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef _USBC_CAPSULE_FMP_DEV_PD_BRIDGE_H_
#define _USBC_CAPSULE_FMP_DEV_PD_BRIDGE_H_

#include "UsbCCapsuleLogEventsId.h"

///
/// FmpDeviceCheckImageWithStatus - ImageUpdatable Pointer Parameter is NULL
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_CHECKIMAGE                         0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_CHECKIMAGE                         ((EVT_ID_FMP_DEV_PD_BRIDGE_CHECKIMAGE << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_CHECKIMAGE << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_CHECKIMAGE                           "CheckImage - ImageUpdatable Pointer Parameter is NULL.\n"
///
/// FmpDeviceCheckImageWithStatus - Image Pointer Parameter is NULL
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_CHECKIMAGE_POINTER_NULL            0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_CHECKIMAGE_POINTER_NULL            ((EVT_ID_FMP_DEV_PD_BRIDGE_CHECKIMAGE_POINTER_NULL << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_CHECKIMAGE_POINTER_NULL << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_CHECKIMAGE_POINTER_NULL              "CheckImage - Image Pointer Parameter is NULL.\n"
///
/// FmpDeviceCheckImageWithStatus - PD Bridge payload signature is not detected
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_SIGNATURE_NOT_DETECTED             0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_SIGNATURE_NOT_DETECTED             ((EVT_ID_FMP_DEV_PD_BRIDGE_SIGNATURE_NOT_DETECTED << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_SIGNATURE_NOT_DETECTED << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_SIGNATURE_NOT_DETECTED               "CheckImage - PD Bridge payload signature is not detected.\n"
///
/// FmpDeviceCheckImageWithStatus - PD Bridge in header is 0, nothing to update
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_ZERO                       0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_ZERO                       ((EVT_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_ZERO << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_ZERO << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_ZERO                         "CheckImage - PD Bridge in header is 0, nothing to update.\n"
///
/// FmpDeviceCheckImageWithStatus - PD Bridge payload size too small
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_SIZE_SMALL                 0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_SIZE_SMALL                 ((EVT_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_SIZE_SMALL << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_SIZE_SMALL << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_SIZE_SMALL                   "CheckImage - PD Bridge payload size too small\n"
///
/// FmpDeviceCheckImageWithStatus - PD Bridge payload is out of bounds
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS1                0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS1                ((EVT_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS1 << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS1 << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS1                  "CheckImage - PD Bridge payload is out of bounds\n"
///
/// UpdatePdBridgeVersion - Failed to get UsbCPdBridgeVersionGuid variable with return Status = (%r)
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_GET_VERSION_VARIABLE_FAILED        1
#define EVT_CODE_FMP_DEV_PD_BRIDGE_GET_VERSION_VARIABLE_FAILED        ((EVT_ID_FMP_DEV_PD_BRIDGE_GET_VERSION_VARIABLE_FAILED << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_GET_VERSION_VARIABLE_FAILED << EVT_CODE_ARG_NUM_OFFSET) | EVT_CODE_ARG0_STATUS)
#define STR_ID_FMP_DEV_PD_BRIDGE_GET_VERSION_VARIABLE_FAILED          "Failed to get UsbCPdBridgeVersionGuid variable with return Status = (%r).\n"
///
/// FmpDeviceSetImageWithStatus - FmpDeviceSetImageWithStatus (PD Bridge) - Start
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_SET_IMAGE_START                    0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_SET_IMAGE_START                    ((EVT_ID_FMP_DEV_PD_BRIDGE_SET_IMAGE_START << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_SET_IMAGE_START << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_SET_IMAGE_START                      "FmpDeviceSetImageWithStatus (PD Bridge) - Start\n"
///
/// FmpDeviceSetImageWithStatus - Image is NULL
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_IMAGE_NULL                         0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_IMAGE_NULL                         ((EVT_ID_FMP_DEV_PD_BRIDGE_IMAGE_NULL << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_IMAGE_NULL << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_IMAGE_NULL                           "FmpDeviceSetImageWithStatus - Image is NULL\n"
///
/// FmpDeviceSetImageWithStatus - Invalid progress callback
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_PROGRESS_IS_NULL                   0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_PROGRESS_IS_NULL                   ((EVT_ID_FMP_DEV_PD_BRIDGE_PROGRESS_IS_NULL << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_PROGRESS_IS_NULL << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_PROGRESS_IS_NULL                     "FmpDeviceSetImageWithStatus - Invalid progress callback\n"
///
/// FmpDeviceSetImageWithStatus - Progress Callback failed with Status %r
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_PROGRESS_CALLBACK_FAILED           1
#define EVT_CODE_FMP_DEV_PD_BRIDGE_PROGRESS_CALLBACK_FAILED           ((EVT_ID_FMP_DEV_PD_BRIDGE_PROGRESS_CALLBACK_FAILED << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_PROGRESS_CALLBACK_FAILED << EVT_CODE_ARG_NUM_OFFSET) | EVT_CODE_ARG0_STATUS)
#define STR_ID_FMP_DEV_PD_BRIDGE_PROGRESS_CALLBACK_FAILED             "FmpDeviceSetImageWithStatus - Progress Callback failed with Status %r.\n"
///
/// FmpDeviceSetImageWithStatus - Update PD Bridge - Start
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_UPDATE_START                       0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_UPDATE_START                       ((EVT_ID_FMP_DEV_PD_BRIDGE_UPDATE_START << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_UPDATE_START << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_UPDATE_START                         "Update PD Bridge - Start\n"
///
/// FmpDeviceSetImageWithStatus - Failed to locate PdBridgeProtocol (%r)
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_LOCATE_PROTOCOL_FAILED             1
#define EVT_CODE_FMP_DEV_PD_BRIDGE_LOCATE_PROTOCOL_FAILED             ((EVT_ID_FMP_DEV_PD_BRIDGE_LOCATE_PROTOCOL_FAILED << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_LOCATE_PROTOCOL_FAILED << EVT_CODE_ARG_NUM_OFFSET) | EVT_CODE_ARG0_STATUS)
#define STR_ID_FMP_DEV_PD_BRIDGE_LOCATE_PROTOCOL_FAILED               "FmpDeviceSetImageWithStatus - Failed to locate PdBridgeProtocol (%r).\n"
///
/// FmpDeviceSetImageWithStatus - Update PD Bridge payload(%d/%d)
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_COUNT                      2
#define EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_COUNT                      ((EVT_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_COUNT << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_COUNT << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_COUNT                        "Update PD Bridge payload(%d/%d).\n"
///
/// FmpDeviceSetImageWithStatus - ImageOffset=0x%x, size=0x%x
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_IMAGE_OFFSET_SIZE          2
#define EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_IMAGE_OFFSET_SIZE          ((EVT_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_IMAGE_OFFSET_SIZE << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_IMAGE_OFFSET_SIZE << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_IMAGE_OFFSET_SIZE            "ImageOffset=0x%x, size=0x%x\n"
///
/// FmpDeviceSetImageWithStatus - TcpIndex=0x%x, ShareFlashMode=0x%x
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_CNTRL_INDEX_SHARE_FLASH_MODE  2
#define EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_CNTRL_INDEX_SHARE_FLASH_MODE  ((EVT_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_CNTRL_INDEX_SHARE_FLASH_MODE << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_CNTRL_INDEX_SHARE_FLASH_MODE << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_CNTRL_INDEX_SHARE_FLASH_MODE    "TcpIndex=0x%x, ShareFlashMode=0x%x, "
///
/// FmpDeviceSetImageWithStatus - PdBridgeType=0x%x\n
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_PD_BRIDGE_TYPE             1
#define EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_PD_BRIDGE_TYPE             ((EVT_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_PD_BRIDGE_TYPE << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_PD_BRIDGE_TYPE << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_PD_BRIDGE_TYPE               "PdBridgeType=0x%x\n"
///
/// FmpDeviceSetImageWithStatus - PD Bridge payload is out of bounds
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS2                0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS2                ((EVT_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS2 << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS2 << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS2                  "PD Bridge payload is out of bounds\n"
///
/// FmpDeviceSetImageWithStatus - PayLoad - Update failed on un-support FirmwareType value %d
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_UNSUPPORT_FW_TYPE                  1
#define EVT_CODE_FMP_DEV_PD_BRIDGE_UNSUPPORT_FW_TYPE                  ((EVT_ID_FMP_DEV_PD_BRIDGE_UNSUPPORT_FW_TYPE << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_UNSUPPORT_FW_TYPE << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_UNSUPPORT_FW_TYPE                    "PayLoad - Update failed on un-support FirmwareType value %d\n"
///
/// FmpDeviceSetImageWithStatus - PayLoad - Update failed on invalid ShareFlashMode value %d
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_INVALID_SHARE_FLASH_MODE_VALUE     1
#define EVT_CODE_FMP_DEV_PD_BRIDGE_INVALID_SHARE_FLASH_MODE_VALUE     ((EVT_ID_FMP_DEV_PD_BRIDGE_INVALID_SHARE_FLASH_MODE_VALUE << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_INVALID_SHARE_FLASH_MODE_VALUE << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_INVALID_SHARE_FLASH_MODE_VALUE       "PayLoad - Update failed on invalid ShareFlashMode value %d\n"
///
/// FmpDeviceSetImageWithStatus - PayLoad - Update failed on invalid PdBridgeType value %d\n
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_INVALID_PD_BRIDGE_TYPE_VALUE       1
#define EVT_CODE_FMP_DEV_PD_BRIDGE_INVALID_PD_BRIDGE_TYPE_VALUE       ((EVT_ID_FMP_DEV_PD_BRIDGE_INVALID_PD_BRIDGE_TYPE_VALUE << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_INVALID_PD_BRIDGE_TYPE_VALUE << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_INVALID_PD_BRIDGE_TYPE_VALUE         "PayLoad - Update failed on invalid PdBridgeType value %d\n"
///
/// FmpDeviceSetImageWithStatus - InitialVendorCommandData failed with Status %r.\n
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_INIT_VENDOR_CMD_DATA_FAILED        1
#define EVT_CODE_FMP_DEV_PD_BRIDGE_INIT_VENDOR_CMD_DATA_FAILED        ((EVT_ID_FMP_DEV_PD_BRIDGE_INIT_VENDOR_CMD_DATA_FAILED << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_INIT_VENDOR_CMD_DATA_FAILED << EVT_CODE_ARG_NUM_OFFSET) | EVT_CODE_ARG0_STATUS)
#define STR_ID_FMP_DEV_PD_BRIDGE_INIT_VENDOR_CMD_DATA_FAILED          "InitialVendorCommandData failed with Status %r.\n"
///
/// FmpDeviceSetImageWithStatus - [USBC] PD%d
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_FW_VERSION_INDEX                   1
#define EVT_CODE_FMP_DEV_PD_BRIDGE_FW_VERSION_INDEX                   ((EVT_ID_FMP_DEV_PD_BRIDGE_FW_VERSION_INDEX << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_FW_VERSION_INDEX << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_FW_VERSION_INDEX                     "[USBC] TCP%d PD "
///
/// FmpDeviceSetImageWithStatus - Version is %08x.%08x
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_FW_VERSION                         2
#define EVT_CODE_FMP_DEV_PD_BRIDGE_FW_VERSION                         ((EVT_ID_FMP_DEV_PD_BRIDGE_FW_VERSION << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_FW_VERSION << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_FW_VERSION                           "Version is %08x.%08x\n"
///
/// FmpDeviceSetImageWithStatus - PD Bridge update failed (%r) at image index %d
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_UPDATE_FAILED_AT_INDEX             2
#define EVT_CODE_FMP_DEV_PD_BRIDGE_UPDATE_FAILED_AT_INDEX             ((EVT_ID_FMP_DEV_PD_BRIDGE_UPDATE_FAILED_AT_INDEX << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_UPDATE_FAILED_AT_INDEX << EVT_CODE_ARG_NUM_OFFSET) | EVT_CODE_ARG0_STATUS)
#define STR_ID_FMP_DEV_PD_BRIDGE_UPDATE_FAILED_AT_INDEX               "PD Bridge update failed (%r) at image index %d\n"
///
/// FmpDeviceSetImageWithStatus - FmpDeviceSetImageWithStatus (PD Bridge) - End
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_SET_IMAGE_END                      0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_SET_IMAGE_END                      ((EVT_ID_FMP_DEV_PD_BRIDGE_SET_IMAGE_END << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_SET_IMAGE_END << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_SET_IMAGE_END                        "FmpDeviceSetImageWithStatus (PD Bridge) - End\n"
///
/// FmpDeviceLock - FmpDeviceLib(PD Bridge): FmpDeviceLock() for system FLASH
///
#define EVT_ARGS_FMP_DEV_PD_BRIDGE_LOCK                               0
#define EVT_CODE_FMP_DEV_PD_BRIDGE_LOCK                               ((EVT_ID_FMP_DEV_PD_BRIDGE_LOCK << EVT_CODE_EVT_ID_OFFSET) | (EVT_ARGS_FMP_DEV_PD_BRIDGE_LOCK << EVT_CODE_ARG_NUM_OFFSET))
#define STR_ID_FMP_DEV_PD_BRIDGE_LOCK                                 "FmpDeviceLib(PD Bridge): FmpDeviceLock() for system FLASH\n"
#endif
