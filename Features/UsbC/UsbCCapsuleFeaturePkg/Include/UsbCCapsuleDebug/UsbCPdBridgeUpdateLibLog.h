/** @file
  Log Event and Log String Definitions - UsbCPdBridgeUpdateLib

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

#ifndef _USBC_CAPSULE_PD_BRIDGE_H_
#define _USBC_CAPSULE_PD_BRIDGE_H_

#include "UsbCCapsuleLogEventsId.h"

///
/// NvmRead - Length is Invalid
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_READ_LENGTH_INVALID                1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_READ_LENGTH_INVALID                ((EVT_ID_USBC_PD_BRIDGE_NVM_READ_LENGTH_INVALID << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_READ_LENGTH_INVALID))
#define STR_ID_USBC_PD_BRIDGE_NVM_READ_LENGTH_INVALID                  "NvmRead: Invalid Length %d.\n"
///
/// NvmRead - OutputData or OutputDataSize is NULL
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_READ_OUTPUT_DATA_NULL              0
#define EVT_CODE_USBC_PD_BRIDGE_NVM_READ_OUTPUT_DATA_NULL              ((EVT_ID_USBC_PD_BRIDGE_NVM_READ_OUTPUT_DATA_NULL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_READ_OUTPUT_DATA_NULL))
#define STR_ID_USBC_PD_BRIDGE_NVM_READ_OUTPUT_DATA_NULL                "NvmRead: OutputData or OutputDataSize is NULL\n"
///
/// NvmRead - Read data Start at Offset:0x%x Length:0x%x
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_READ_OFFSET_LENGTH                 2
#define EVT_CODE_USBC_PD_BRIDGE_NVM_READ_OFFSET_LENGTH                 ((EVT_ID_USBC_PD_BRIDGE_NVM_READ_OFFSET_LENGTH << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_READ_OFFSET_LENGTH))
#define STR_ID_USBC_PD_BRIDGE_NVM_READ_OFFSET_LENGTH                   "NvmRead: Read data Start at Offset:0x%x Length:0x%x "
///
/// NvmRead - for PdBridge on TCP%d
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_READ_DATA_INDEX                    1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_READ_DATA_INDEX                    ((EVT_ID_USBC_PD_BRIDGE_NVM_READ_DATA_INDEX << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_READ_DATA_INDEX))
#define STR_ID_USBC_PD_BRIDGE_NVM_READ_DATA_INDEX                      "for PdBridge on TCP%d\n"
///
/// NvmRead - Execute Vendor command code :0x%x failed, Status:%r
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_VENDOR_CMD_FAIL                    2
#define EVT_CODE_USBC_PD_BRIDGE_NVM_VENDOR_CMD_FAIL                    ((EVT_ID_USBC_PD_BRIDGE_NVM_VENDOR_CMD_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_VENDOR_CMD_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_VENDOR_CMD_FAIL                      "NvmRead: Execute Vendor command code :0x%x failed, Status:%r\n"
///
/// NvmSetOffset - SetOffset to 0x%x for PdBridge on TCP%d
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_SET_OFFSET_INDEX                   2
#define EVT_CODE_USBC_PD_BRIDGE_NVM_SET_OFFSET_INDEX                   ((EVT_ID_USBC_PD_BRIDGE_NVM_SET_OFFSET_INDEX << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_SET_OFFSET_INDEX))
#define STR_ID_USBC_PD_BRIDGE_NVM_SET_OFFSET_INDEX                     "NvmSetOffset: SetOffset to 0x%x for PdBridge on TCP%d\n"
///
/// NvmSetOffset - Execute Vendor command code :%x failed, Status:%r
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_SET_OFFSET_VENDOR_CMD_FAIL         2
#define EVT_CODE_USBC_PD_BRIDGE_NVM_SET_OFFSET_VENDOR_CMD_FAIL         ((EVT_ID_USBC_PD_BRIDGE_NVM_SET_OFFSET_VENDOR_CMD_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_SET_OFFSET_VENDOR_CMD_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_SET_OFFSET_VENDOR_CMD_FAIL           "NvmSetOffset: Execute Vendor command code :0x%x failed, Status:%r\n"
///
/// NvmSetOffset - SetOffset to 0x%x for PdBridge on TCP%d successfully
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_SET_OFFSET_SUCCESS                 2
#define EVT_CODE_USBC_PD_BRIDGE_NVM_SET_OFFSET_SUCCESS                 ((EVT_ID_USBC_PD_BRIDGE_NVM_SET_OFFSET_SUCCESS << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_SET_OFFSET_SUCCESS))
#define STR_ID_USBC_PD_BRIDGE_NVM_SET_OFFSET_SUCCESS                   "NvmSetOffset: SetOffset to 0x%x for PdBridge on TCP%d successfully\n"
///
/// NvmWrite - Buffer is NULL
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_WRITE_NULL                         0
#define EVT_CODE_USBC_PD_BRIDGE_NVM_WRITE_NULL                         ((EVT_ID_USBC_PD_BRIDGE_NVM_WRITE_NULL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_WRITE_NULL))
#define STR_ID_USBC_PD_BRIDGE_NVM_WRITE_NULL                           "NvmWrite: Buffer is NULL\n"
///
/// NvmWrite - Invalid BufferSize %d
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_WRITE_INVALID_BUFFER_SIZE          1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_WRITE_INVALID_BUFFER_SIZE          ((EVT_ID_USBC_PD_BRIDGE_NVM_WRITE_INVALID_BUFFER_SIZE << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_WRITE_INVALID_BUFFER_SIZE))
#define STR_ID_USBC_PD_BRIDGE_NVM_WRITE_INVALID_BUFFER_SIZE            "NvmWrite: Invalid BufferSize %d\n"
///
/// NvmWrite - Execute Vendor command code :0x%x failed, Status:%r
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_WRITE_VENDOR_CMD_FAIL              2
#define EVT_CODE_USBC_PD_BRIDGE_NVM_WRITE_VENDOR_CMD_FAIL              ((EVT_ID_USBC_PD_BRIDGE_NVM_WRITE_VENDOR_CMD_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_WRITE_VENDOR_CMD_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_WRITE_VENDOR_CMD_FAIL                "NvmWrite: Execute Vendor command code :0x%x failed, Status:%r\n"
///
/// NvmAuthenticateWrite - Send NVM Authenticate Write command to PdBridge on TCP%d
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_AUTH_CMD_INDEX                     1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_AUTH_CMD_INDEX                     ((EVT_ID_USBC_PD_BRIDGE_NVM_AUTH_CMD_INDEX << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_AUTH_CMD_INDEX))
#define STR_ID_USBC_PD_BRIDGE_NVM_AUTH_CMD_INDEX                       "NvmAuthenticateWrite: Send NVM Authenticate Write command to PdBridge on TCP:%d\n"
///
/// NvmAuthenticateWrite - Execute Vendor command code :0x%x failed, Status:%r
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_AUTH_VENDOR_CMD_FAIL               2
#define EVT_CODE_USBC_PD_BRIDGE_NVM_AUTH_VENDOR_CMD_FAIL               ((EVT_ID_USBC_PD_BRIDGE_NVM_AUTH_VENDOR_CMD_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_AUTH_VENDOR_CMD_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_AUTH_VENDOR_CMD_FAIL                 "NvmAuthenticateWrite: Execute Vendor command code :0x%x failed, Status:%r\n"
///
/// NvmAuthenticateWrite - Send NVM Authenticate Write command to PdBridge on TCP%d successfully
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_AUTH_WRITE_CMD_SUCCESS             1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_AUTH_WRITE_CMD_SUCCESS             ((EVT_ID_USBC_PD_BRIDGE_NVM_AUTH_WRITE_CMD_SUCCESS << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_AUTH_WRITE_CMD_SUCCESS))
#define STR_ID_USBC_PD_BRIDGE_NVM_AUTH_WRITE_CMD_SUCCESS               "NvmAuthenticateWrite: Send NVM Authenticate Write command to PdBridge on TCP%d successfully\n"
///
/// NvmStallNvmAccess - Send Stall NVM Access command to PdBridge on TCP%d
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_STALL_INDEX                        1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_STALL_INDEX                        ((EVT_ID_USBC_PD_BRIDGE_NVM_STALL_INDEX << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_STALL_INDEX))
#define STR_ID_USBC_PD_BRIDGE_NVM_STALL_INDEX                          "NvmStallNvmAccess: Send Stall NVM Access command to PdBridge on TCP%d\n"
///
/// NvmStallNvmAccess - Execute Vendor command code :0x%x failed, Status:%r
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_STALL_VENDOR_CMD_FAIL              1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_STALL_VENDOR_CMD_FAIL              ((EVT_ID_USBC_PD_BRIDGE_NVM_STALL_VENDOR_CMD_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_STALL_VENDOR_CMD_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_STALL_VENDOR_CMD_FAIL                "NvmStallNvmAccess: Execute Vendor command code :0x%x failed, Status:%r\n"
///
/// NvmStallNvmAccess - Send Stall NVM Access command to PdBridge on TCP%d successfully
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_STALL_CMD_SUCCESS                  1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_STALL_CMD_SUCCESS                  ((EVT_ID_USBC_PD_BRIDGE_NVM_STALL_CMD_SUCCESS << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_STALL_CMD_SUCCESS))
#define STR_ID_USBC_PD_BRIDGE_NVM_STALL_CMD_SUCCESS                    "NvmStallNvmAccess: Send Stall NVM Access command to PdBridge on TCP%d successfully\n"
///
/// NvmReset - Send NVM Reset command to PdBridge on TCP%d
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_RESET_INDEX                        1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_RESET_INDEX                        ((EVT_ID_USBC_PD_BRIDGE_NVM_RESET_INDEX << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_RESET_INDEX))
#define STR_ID_USBC_PD_BRIDGE_NVM_RESET_INDEX                          "NvmReset: Send NVM Reset command to PdBridge on TCP%d\n"
///
/// NvmReset - Execute Vendor command code :0x%x failed, Status:%r
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_RESET_VENDOR_CMD_FAIL              2
#define EVT_CODE_USBC_PD_BRIDGE_NVM_RESET_VENDOR_CMD_FAIL              ((EVT_ID_USBC_PD_BRIDGE_NVM_RESET_VENDOR_CMD_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_RESET_VENDOR_CMD_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_RESET_VENDOR_CMD_FAIL                "NvmReset: Execute Vendor command code :0x%x failed, Status:%r\n"
///
/// NvmReset - Send NVM Reset command to PdBridge on TCP%d successfully
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_RESET_CMD_SUCCESS                  1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_RESET_CMD_SUCCESS                  ((EVT_ID_USBC_PD_BRIDGE_NVM_RESET_CMD_SUCCESS << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_RESET_CMD_SUCCESS))
#define STR_ID_USBC_PD_BRIDGE_NVM_RESET_CMD_SUCCESS                    "NvmReset: Send NVM Reset command to PdBridge on TCP%d successfully\n"
///
/// UpdatePdBridgeNvmFirmware - UpdatePdBridgeNvmFirmware START (0x%06x bytes to write):
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_START                    1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_START                    ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_START << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_START))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_START                      "UpdatePdBridgeNvmFirmware START (0x%06x bytes to write):\n"
///
/// UpdatePdBridgeNvmFirmware - NvmSetOffset failed, Status:%r
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_SET_OFFSET_FAIL          1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_SET_OFFSET_FAIL          ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_SET_OFFSET_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_SET_OFFSET_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_SET_OFFSET_FAIL            "UpdatePdBridgeNvmFirmware: NvmSetOffset failed, Status:%r\n"
///
/// UpdatePdBridgeNvmFirmware - The total data size to write is 0x%6x
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_TOTAL_SIZE               1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_TOTAL_SIZE               ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_TOTAL_SIZE << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_TOTAL_SIZE))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_TOTAL_SIZE                 "The total data size to write is 0x%6x\n"
///
/// UpdatePdBridgeNvmFirmware - Written so far : 0x%05x bytes
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITTEN_SO_FAR           1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITTEN_SO_FAR           ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITTEN_SO_FAR << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITTEN_SO_FAR))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITTEN_SO_FAR             "Written so far : 0x%05x bytes\n"
///
/// UpdatePdBridgeNvmFirmware - NvmWrite failed, Status:%r
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMWRITE_FAIL            1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMWRITE_FAIL            ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMWRITE_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMWRITE_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMWRITE_FAIL              "UpdatePdBridgeNvmFirmware: NvmWrite failed, Status:%r\n"
///
/// UpdatePdBridgeNvmFirmware - Image write finished
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITE_FINISH             0
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITE_FINISH             ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITE_FINISH << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITE_FINISH))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITE_FINISH               "Image write finished.\n"
///
/// UpdatePdBridgeNvmFirmware - Got an error while writing the image. As a recovery, starting again
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_GOT_ERROR_RECOVERY       0
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_GOT_ERROR_RECOVERY       ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_GOT_ERROR_RECOVERY << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_GOT_ERROR_RECOVERY))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_GOT_ERROR_RECOVERY         "Got an error while writing the image. As a recovery, starting again\n"
///
/// UpdatePdBridgeNvmFirmware - Image write wasn't successful due to a device error
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_DEV_ERROR                0
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_DEV_ERROR                ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_DEV_ERROR << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_DEV_ERROR))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_DEV_ERROR                  "Image write wasn't successful due to a device error\n"
///
/// UpdatePdBridgeNvmFirmware - NvmStallNvmAccess failed, Status:%r
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_STALLNVM_FAIL            1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_STALLNVM_FAIL            ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_STALLNVM_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_STALLNVM_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_STALLNVM_FAIL              "UpdatePdBridgeNvmFirmware: NvmStallNvmAccess failed, Status:%r\n"
///
/// UpdatePdBridgeNvmFirmware -
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_AUTHWRITE_FAIL           1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_AUTHWRITE_FAIL           ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_AUTHWRITE_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_AUTHWRITE_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_AUTHWRITE_FAIL             "UpdatePdBridgeNvmFirmware: NvmAuthenticateWrite failed, Status:%r\n"
///
/// UpdatePdBridgeNvmFirmware - NvmReset failed, Status:%r
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMRESET_FAIL            1
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMRESET_FAIL            ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMRESET_FAIL << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMRESET_FAIL))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMRESET_FAIL              "UpdatePdBridgeNvmFirmware: NvmReset failed, Status:%r\n"
///
/// UpdatePdBridgeNvmFirmware - PD Bridge NVM firmware update and authentication success
///
#define EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_SUCCESS                  0
#define EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_SUCCESS                  ((EVT_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_SUCCESS << 2) | (EVT_ARGS_USBC_PD_BRIDGE_NVM_FW_UPDATE_SUCCESS))
#define STR_ID_USBC_PD_BRIDGE_NVM_FW_UPDATE_SUCCESS                    "UpdatePdBridgeNvmFirmware: PD Bridge NVM firmware update and authentication success\n"

#endif
