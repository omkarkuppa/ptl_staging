/** @file
  USB4 Log Event and Log String Definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _USB4_LOG_EVENTS_H_
#define _USB4_LOG_EVENTS_H_

#include <Uefi.h>

//
// Usb4CmDxe Entry
//
#define USB4_EVT_ID_CM_DXE_ENTRY              0
#define USB4_EVT_ARGS_CM_DXE_ENTRY            0
#define USB4_EVT_CODE_CM_DXE_ENTRY            ((USB4_EVT_ID_CM_DXE_ENTRY << 2) | (USB4_EVT_ARGS_CM_DXE_ENTRY))
#define USB4_EVT_STR_CM_DXE_ENTRY             "Usb4CmDxe Entry\n"

//
// Usb4CmDxe Exit
//
#define USB4_EVT_ID_CM_DXE_EXIT               1
#define USB4_EVT_ARGS_CM_DXE_EXIT             0
#define USB4_EVT_CODE_CM_DXE_EXIT             ((USB4_EVT_ID_CM_DXE_EXIT << 2) | (USB4_EVT_ARGS_CM_DXE_EXIT))
#define USB4_EVT_STR_CM_DXE_EXIT              "Usb4CmDxe Exit\n"

//
// Usb4CmExecute Entry
//
#define USB4_EVT_ID_CM_EXECUTE_ENTRY          2
#define USB4_EVT_ARGS_CM_EXECUTE_ENTRY        0
#define USB4_EVT_CODE_CM_EXECUTE_ENTRY        ((USB4_EVT_ID_CM_EXECUTE_ENTRY << 2) | (USB4_EVT_ARGS_CM_EXECUTE_ENTRY))
#define USB4_EVT_STR_CM_EXECUTE_ENTRY         "Usb4CmExecute Entry\n"

//
// Usb4CmExecute Exit
// Arg0 - Returned Status
//
#define USB4_EVT_ID_CM_EXECUTE_EXIT           3
#define USB4_EVT_ARGS_CM_EXECUTE_EXIT         1
#define USB4_EVT_CODE_CM_EXECUTE_EXIT         ((USB4_EVT_ID_CM_EXECUTE_EXIT << 2) | (USB4_EVT_ARGS_CM_EXECUTE_EXIT))
#define USB4_EVT_STR_CM_EXECUTE_EXIT          "Usb4CmExecute Exit - Status = %r\n"

//
// Usb4HrCreate Entry
//
#define USB4_EVT_ID_HR_CREATE_ENTRY           4
#define USB4_EVT_ARGS_HR_CREATE_ENTRY         0
#define USB4_EVT_CODE_HR_CREATE_ENTRY         ((USB4_EVT_ID_HR_CREATE_ENTRY << 2) | (USB4_EVT_ARGS_HR_CREATE_ENTRY))
#define USB4_EVT_STR_HR_CREATE_ENTRY          "Usb4HrCreate Entry\n"

//
// Usb4HrCreate Exit
// Arg0 - Returned Status
//
#define USB4_EVT_ID_HR_CREATE_EXIT            5
#define USB4_EVT_ARGS_HR_CREATE_EXIT          1
#define USB4_EVT_CODE_HR_CREATE_EXIT          ((USB4_EVT_ID_HR_CREATE_EXIT << 2) | (USB4_EVT_ARGS_HR_CREATE_EXIT))
#define USB4_EVT_STR_HR_CREATE_EXIT           "Usb4HrCreate Exit - Status = %r\n"

//
// Usb4HrBuildTopology Entry
//
#define USB4_EVT_ID_BLD_TOPOLOGY_ENTRY        6
#define USB4_EVT_ARGS_BLD_TOPOLOGY_ENTRY      0
#define USB4_EVT_CODE_BLD_TOPOLOGY_ENTRY      ((USB4_EVT_ID_BLD_TOPOLOGY_ENTRY << 2) | (USB4_EVT_ARGS_BLD_TOPOLOGY_ENTRY))
#define USB4_EVT_STR_BLD_TOPOLOGY_ENTRY       "Usb4HrBuildTopology Entry\n"

//
// Usb4HrBuildTopology Exit
// Arg0 - Returned Status
//
#define USB4_EVT_ID_BLD_TOPOLOGY_EXIT         7
#define USB4_EVT_ARGS_BLD_TOPOLOGY_EXIT       1
#define USB4_EVT_CODE_BLD_TOPOLOGY_EXIT       ((USB4_EVT_ID_BLD_TOPOLOGY_EXIT << 2) | (USB4_EVT_ARGS_BLD_TOPOLOGY_EXIT))
#define USB4_EVT_STR_BLD_TOPOLOGY_EXIT        "Usb4HrBuildTopology Exit - Status = %r\n"

//
// Usb4HrCreateDomainRoot Entry
//
#define USB4_EVT_ID_ROOTRT_CREATE_ENTRY       8
#define USB4_EVT_ARGS_ROOTRT_CREATE_ENTRY     0
#define USB4_EVT_CODE_ROOTRT_CREATE_ENTRY     ((USB4_EVT_ID_ROOTRT_CREATE_ENTRY << 2) | (USB4_EVT_ARGS_ROOTRT_CREATE_ENTRY))
#define USB4_EVT_STR_ROOTRT_CREATE_ENTRY      "Usb4HrCreateDomainRoot Entry\n"

//
// Usb4HrCreateDomainRoot Exit
// Arg0 - Returned Status
//
#define USB4_EVT_ID_ROOTRT_CREATE_EXIT        9
#define USB4_EVT_ARGS_ROOTRT_CREATE_EXIT      1
#define USB4_EVT_CODE_ROOTRT_CREATE_EXIT      ((USB4_EVT_ID_ROOTRT_CREATE_EXIT << 2) | (USB4_EVT_ARGS_ROOTRT_CREATE_EXIT))
#define USB4_EVT_STR_ROOTRT_CREATE_EXIT       "Usb4HrCreateDomainRoot Exit - Status = %r\n"

//
// Usb4RtEnumAllDsRouters Entry
//
#define USB4_EVT_ID_ENUM_ALLDSRT_ENTRY        10
#define USB4_EVT_ARGS_ENUM_ALLDSRT_ENTRY      0
#define USB4_EVT_CODE_ENUM_ALLDSRT_ENTRY      ((USB4_EVT_ID_ENUM_ALLDSRT_ENTRY << 2) | (USB4_EVT_ARGS_ENUM_ALLDSRT_ENTRY))
#define USB4_EVT_STR_ENUM_ALLDSRT_ENTRY       "Usb4RtEnumAllDsRouters Entry\n"

//
// Usb4RtEnumAllDsRouters Exit
// Arg0 - Returned Status
//
#define USB4_EVT_ID_ENUM_ALLDSRT_EXIT         11
#define USB4_EVT_ARGS_ENUM_ALLDSRT_EXIT       1
#define USB4_EVT_CODE_ENUM_ALLDSRT_EXIT       ((USB4_EVT_ID_ENUM_ALLDSRT_EXIT << 2) | (USB4_EVT_ARGS_ENUM_ALLDSRT_EXIT))
#define USB4_EVT_STR_ENUM_ALLDSRT_EXIT        "Usb4RtEnumAllDsRouters Exit - Status = %r\n"

//
// Usb4RtEnumDsRouter Entry
//
#define USB4_EVT_ID_ENUM_DSRT_ENTRY           12
#define USB4_EVT_ARGS_ENUM_DSRT_ENTRY         0
#define USB4_EVT_CODE_ENUM_DSRT_ENTRY         ((USB4_EVT_ID_ENUM_DSRT_ENTRY << 2) | (USB4_EVT_ARGS_ENUM_DSRT_ENTRY))
#define USB4_EVT_STR_ENUM_DSRT_ENTRY          "Usb4RtEnumDsRouter Entry\n"

//
// Usb4RtEnumDsRouter Exit
// Arg0 - Returned Status
//
#define USB4_EVT_ID_ENUM_DSRT_EXIT            13
#define USB4_EVT_ARGS_ENUM_DSRT_EXIT          1
#define USB4_EVT_CODE_ENUM_DSRT_EXIT          ((USB4_EVT_ID_ENUM_DSRT_EXIT << 2) | (USB4_EVT_ARGS_ENUM_DSRT_EXIT))
#define USB4_EVT_STR_ENUM_DSRT_EXIT           "Usb4RtEnumDsRouter Exit - Status = %r\n"

//
// Usb4DevRtInit Entry
//
#define USB4_EVT_ID_DEVRT_INIT_ENTRY          14
#define USB4_EVT_ARGS_DEVRT_INIT_ENTRY        0
#define USB4_EVT_CODE_DEVRT_INIT_ENTRY        ((USB4_EVT_ID_DEVRT_INIT_ENTRY << 2) | (USB4_EVT_ARGS_DEVRT_INIT_ENTRY))
#define USB4_EVT_STR_DEVRT_INIT_ENTRY         "Usb4DevRtInit Entry\n"

//
// Usb4DevRtInit Exit
// Arg0 - Returned Status
//
#define USB4_EVT_ID_DEVRT_INIT_EXIT           15
#define USB4_EVT_ARGS_DEVRT_INIT_EXIT         1
#define USB4_EVT_CODE_DEVRT_INIT_EXIT         ((USB4_EVT_ID_DEVRT_INIT_EXIT << 2) | (USB4_EVT_ARGS_DEVRT_INIT_EXIT))
#define USB4_EVT_STR_DEVRT_INIT_EXIT          "Usb4DevRtInit Exit - Status = %r\n"

//
// Usb4CmEndOfDxe Entry
//
#define USB4_EVT_ID_EOF_DXE_ENTRY             16
#define USB4_EVT_ARGS_EOF_DXE_ENTRY           0
#define USB4_EVT_CODE_EOF_DXE_ENTRY           ((USB4_EVT_ID_EOF_DXE_ENTRY << 2) | (USB4_EVT_ARGS_EOF_DXE_ENTRY))
#define USB4_EVT_STR_EOF_DXE_ENTRY            "Usb4CmEndOfDxe Entry\n"

//
// Usb4CmEndOfDxe Exit
//
#define USB4_EVT_ID_EOF_DXE_EXIT              17
#define USB4_EVT_ARGS_EOF_DXE_EXIT            0
#define USB4_EVT_CODE_EOF_DXE_EXIT            ((USB4_EVT_ID_EOF_DXE_EXIT << 2) | (USB4_EVT_ARGS_EOF_DXE_EXIT))
#define USB4_EVT_STR_EOF_DXE_EXIT             "Usb4CmEndOfDxe Exit\n"

//
// USB4 Host Router Count
// Arg0 - Count
//
#define USB4_EVT_ID_HR_COUNT                  18
#define USB4_EVT_ARGS_HR_COUNT                1
#define USB4_EVT_CODE_HR_COUNT                ((USB4_EVT_ID_HR_COUNT << 2) | (USB4_EVT_ARGS_HR_COUNT))
#define USB4_EVT_STR_HR_COUNT                 "USB4 Host Router Count = %d\n"

//
// USB4 CM mode information
// Arg0 - CM mode option
// Arg1 - Applied CM mode
//
#define USB4_EVT_ID_CM_MODE_INFO              19
#define USB4_EVT_ARGS_CM_MODE_INFO            2
#define USB4_EVT_CODE_CM_MODE_INFO            ((USB4_EVT_ID_CM_MODE_INFO << 2) | (USB4_EVT_ARGS_CM_MODE_INFO))
#define USB4_EVT_STR_CM_MODE_INFO             "USB4 CM option = 0x%x, CM mode = %d\n"

//
// Router Topology ID
// Arg0 - Topology ID High DWORD
// Arg1 - Topology ID Low DWORD
//
#define USB4_EVT_ID_RT_TOPOLOGY               20
#define USB4_EVT_ARGS_RT_TOPOLOGY             2
#define USB4_EVT_CODE_RT_TOPOLOGY             ((USB4_EVT_ID_RT_TOPOLOGY << 2) | (USB4_EVT_ARGS_RT_TOPOLOGY))
#define USB4_EVT_STR_RT_TOPOLOGY              "RT 0x%08X%08X\n"

//
// Lane adapter state
// Arg0 - Adapter number
// Arg1 - Adapter state
//
#define USB4_EVT_ID_ADP_STATE                 21
#define USB4_EVT_ARGS_ADP_STATE               2
#define USB4_EVT_CODE_ADP_STATE               ((USB4_EVT_ID_ADP_STATE << 2) | (USB4_EVT_ARGS_ADP_STATE))
#define USB4_EVT_STR_ADP_STATE                "Lane Adp %d State = %d\n"

//
// Router connection fount at downstream Lane adapter
// Arg0 - Downstream adapter number
//
#define USB4_EVT_ID_DSRT_FOUND                22
#define USB4_EVT_ARGS_DSRT_FOUND              1
#define USB4_EVT_CODE_DSRT_FOUND              ((USB4_EVT_ID_DSRT_FOUND << 2) | (USB4_EVT_ARGS_DSRT_FOUND))
#define USB4_EVT_STR_DSRT_FOUND               "Router is detected at Adp %d\n"

//
// Hotplug event from Router
// Arg0 - Router Topology ID High DWORD
// Arg1 - Router Topology ID Low DWORD
//
#define USB4_EVT_ID_HOTPLUG_RT                23
#define USB4_EVT_ARGS_HOTPLUG_RT              2
#define USB4_EVT_CODE_HOTPLUG_RT              ((USB4_EVT_ID_HOTPLUG_RT << 2) | (USB4_EVT_ARGS_HOTPLUG_RT))
#define USB4_EVT_STR_HOTPLUG_RT               "Hotplug event from RT 0x%08X%08X\n"

//
// Adapter/Unplug information of Hotplug event
// Arg0 - Adapter number
// Arg1 - Unplug indicator (0 = Hotplug, 1 = Unplug)
//
#define USB4_EVT_ID_HOTPLUG_ADP               24
#define USB4_EVT_ARGS_HOTPLUG_ADP             2
#define USB4_EVT_CODE_HOTPLUG_ADP             ((USB4_EVT_ID_HOTPLUG_ADP << 2) | (USB4_EVT_ARGS_HOTPLUG_ADP))
#define USB4_EVT_STR_HOTPLUG_ADP              "Adp %d - Unplug = %d\n"

//
// Notification event from Router
// Arg0 - Router Topology ID High DWORD
// Arg1 - Router Topology ID Low DWORD
//
#define USB4_EVT_ID_NOTIFICATION_RT           25
#define USB4_EVT_ARGS_NOTIFICATION_RT         2
#define USB4_EVT_CODE_NOTIFICATION_RT         ((USB4_EVT_ID_NOTIFICATION_RT << 2) | (USB4_EVT_ARGS_NOTIFICATION_RT))
#define USB4_EVT_STR_NOTIFICATION_RT          "Notification from RT 0x%08X%08X\n"

//
// Adapter/EventCode indication of Notification packet
// Arg0 - Adapter number indicated in Notification packet
// Arg1 - Event code indicated in Notification package
//
#define USB4_EVT_ID_NOTIFICATION_ADP          26
#define USB4_EVT_ARGS_NOTIFICATION_ADP        2
#define USB4_EVT_CODE_NOTIFICATION_ADP        ((USB4_EVT_ID_NOTIFICATION_ADP << 2) | (USB4_EVT_ARGS_NOTIFICATION_ADP))
#define USB4_EVT_STR_NOTIFICATION_ADP         "Adp %d - EvtCode = %d\n"

//
// Packet with unexpected PDF is received
// Arg0 - Packet PDF
//
#define USB4_EVT_ID_UNEXPECTED_PDF            27
#define USB4_EVT_ARGS_UNEXPECTED_PDF          1
#define USB4_EVT_CODE_UNEXPECTED_PDF          ((USB4_EVT_ID_UNEXPECTED_PDF << 2) | (USB4_EVT_ARGS_UNEXPECTED_PDF))
#define USB4_EVT_STR_UNEXPECTED_PDF           "Packet with unexpected Pdf %d is received\n"

//
// Router disconnect
// Arg0 - Router Topology ID High DWORD
// Arg1 - Router Topology ID Low DWORD
//
#define USB4_EVT_ID_RT_DISCONNECT             28
#define USB4_EVT_ARGS_RT_DISCONNECT           2
#define USB4_EVT_CODE_RT_DISCONNECT           ((USB4_EVT_ID_RT_DISCONNECT << 2) | (USB4_EVT_ARGS_RT_DISCONNECT))
#define USB4_EVT_STR_RT_DISCONNECT            "Disconnect RT 0x%08X%08X\n"

//
// Router operation status of DP resource query
// Arg0 - DP resources query status
// Arg1 - Router Operation status
//
#define USB4_EVT_ID_DPRES_QUERY               29
#define USB4_EVT_ARGS_DPRES_QUERY             2
#define USB4_EVT_CODE_DPRES_QUERY             ((USB4_EVT_ID_DPRES_QUERY << 2) | (USB4_EVT_ARGS_DPRES_QUERY))
#define USB4_EVT_STR_DPRES_QUERY              "DP resources query - Status %r, OpStatus 0x%x\n"

//
// Router operation status of DP resource allocation
// Arg0 - DP resources allocate status
// Arg1 - Router Operation status
//
#define USB4_EVT_ID_DPRES_ALLOC               30
#define USB4_EVT_ARGS_DPRES_ALLOC             2
#define USB4_EVT_CODE_DPRES_ALLOC             ((USB4_EVT_ID_DPRES_ALLOC << 2) | (USB4_EVT_ARGS_DPRES_ALLOC))
#define USB4_EVT_STR_DPRES_ALLOC              "DP resources allocate - Status %r, OpStatus 0x%x\n"

//
// Router operation status of DP resource de-allocation
// Arg0 - DP resources de-allocate status
// Arg1 - Router Operation status
//
#define USB4_EVT_ID_DPRES_DEALLOC             31
#define USB4_EVT_ARGS_DPRES_DEALLOC           2
#define USB4_EVT_CODE_DPRES_DEALLOC           ((USB4_EVT_ID_DPRES_DEALLOC << 2) | (USB4_EVT_ARGS_DPRES_DEALLOC))
#define USB4_EVT_STR_DPRES_DEALLOC            "DP resources de-allocate - Status %r, OpStatus 0x%x\n"

//
// DP stream enabled
// Arg0 - DP stream index
//
#define USB4_EVT_ID_DPSTREAM_ENABLED          32
#define USB4_EVT_ARGS_DPSTREAM_ENABLED        1
#define USB4_EVT_CODE_DPSTREAM_ENABLED        ((USB4_EVT_ID_DPSTREAM_ENABLED << 2) | (USB4_EVT_ARGS_DPSTREAM_ENABLED))
#define USB4_EVT_STR_DPSTREAM_ENABLED         "DP stream %d is enabled\n"

//
// DP stream disabled
// Arg0 - DP stream index
//
#define USB4_EVT_ID_DPSTREAM_DISABLED         33
#define USB4_EVT_ARGS_DPSTREAM_DISABLED       1
#define USB4_EVT_CODE_DPSTREAM_DISABLED       ((USB4_EVT_ID_DPSTREAM_DISABLED << 2) | (USB4_EVT_ARGS_DPSTREAM_DISABLED))
#define USB4_EVT_STR_DPSTREAM_DISABLED        "DP stream %d is disabled\n"

//
// DP Source added
// Arg0 - DP Adapter number
//
#define USB4_EVT_ID_DPSRC_ADD                 34
#define USB4_EVT_ARGS_DPSRC_ADD               1
#define USB4_EVT_CODE_DPSRC_ADD               ((USB4_EVT_ID_DPSRC_ADD << 2) | (USB4_EVT_ARGS_DPSRC_ADD))
#define USB4_EVT_STR_DPSRC_ADD                "DP Source added - Adp %d\n"

//
// DP Source removed
// Arg0 - DP Adapter number
//
#define USB4_EVT_ID_DPSRC_REMOVE              35
#define USB4_EVT_ARGS_DPSRC_REMOVE            1
#define USB4_EVT_CODE_DPSRC_REMOVE            ((USB4_EVT_ID_DPSRC_REMOVE << 2) | (USB4_EVT_ARGS_DPSRC_REMOVE))
#define USB4_EVT_STR_DPSRC_REMOVE             "DP Source Removed - Adp %d\n"

//
// DP Sink added
// Arg0 - DP Adapter number
//
#define USB4_EVT_ID_DPSINK_ADD                36
#define USB4_EVT_ARGS_DPSINK_ADD              1
#define USB4_EVT_CODE_DPSINK_ADD              ((USB4_EVT_ID_DPSINK_ADD << 2) | (USB4_EVT_ARGS_DPSINK_ADD))
#define USB4_EVT_STR_DPSINK_ADD               "DP Sink added - Adp %d\n"

//
// DP Sink removed
// Arg0 - DP Adapter number
//
#define USB4_EVT_ID_DPSINK_REMOVE             37
#define USB4_EVT_ARGS_DPSINK_REMOVE           1
#define USB4_EVT_CODE_DPSINK_REMOVE           ((USB4_EVT_ID_DPSINK_REMOVE << 2) | (USB4_EVT_ARGS_DPSINK_REMOVE))
#define USB4_EVT_STR_DPSINK_REMOVE            "DP Sink Removed - Adp %d\n"

//
// Notification event received in Rx ring
// Arg0 - Status return for error event
//
#define USB4_EVT_ID_NOTIFICATION_ERROR        38
#define USB4_EVT_ARGS_NOTIFICATION_ERROR      1
#define USB4_EVT_CODE_NOTIFICATION_ERROR      ((USB4_EVT_ID_NOTIFICATION_ERROR << 2) | (USB4_EVT_ARGS_NOTIFICATION_ERROR))
#define USB4_EVT_STR_NOTIFICATION_ERROR       "Error Notification event - Status = %r\n"

//
// Usb4Cm Destroy
//
#define USB4_EVT_ID_CM_DESTROY_ERROR          39
#define USB4_EVT_ARGS_CM_DESTROY_ERROR        1
#define USB4_EVT_CODE_CM_DESTROY_ERROR        ((USB4_EVT_ID_CM_DESTROY_ERROR << 2) | (USB4_EVT_ARGS_CM_DESTROY_ERROR))
#define USB4_EVT_STR_CM_DESTROY_ERROR         "Usb4Cm Destroy - Status = %r\n"

//
// Usb4Hr Destroy
//
#define USB4_EVT_ID_HR_DESTROY_ERROR          40
#define USB4_EVT_ARGS_HR_DESTROY_ERROR        1
#define USB4_EVT_CODE_HR_DESTROY_ERROR        ((USB4_EVT_ID_HR_DESTROY_ERROR << 2) | (USB4_EVT_ARGS_HR_DESTROY_ERROR))
#define USB4_EVT_STR_HR_DESTROY_ERROR         "Usb4Hr Destroy - Status = %r\n"

//
// Usb4Domain Destroy
//
#define USB4_EVT_ID_DOMAIN_DESTROY_ERROR      41
#define USB4_EVT_ARGS_DOMAIN_DESTROY_ERROR    1
#define USB4_EVT_CODE_DOMAIN_DESTROY_ERROR    ((USB4_EVT_ID_DOMAIN_DESTROY_ERROR << 2) | (USB4_EVT_ARGS_DOMAIN_DESTROY_ERROR))
#define USB4_EVT_STR_DOMAIN_DESTROY_ERROR     "Usb4Domain Destroy - Status = %r\n"

//
// Usb4Rt Destroy
//
#define USB4_EVT_ID_RT_DESTROY_ERROR          42
#define USB4_EVT_ARGS_RT_DESTROY_ERROR        1
#define USB4_EVT_CODE_RT_DESTROY_ERROR        ((USB4_EVT_ID_RT_DESTROY_ERROR << 2) | (USB4_EVT_ARGS_RT_DESTROY_ERROR))
#define USB4_EVT_STR_RT_DESTROY_ERROR         "Usb4Rt Destroy - Status = %r\n"

//
// Usb4Rt Destroy
//
#define USB4_EVT_ID_RT_DESTROY_ERROR          42
#define USB4_EVT_ARGS_RT_DESTROY_ERROR        1
#define USB4_EVT_CODE_RT_DESTROY_ERROR        ((USB4_EVT_ID_RT_DESTROY_ERROR << 2) | (USB4_EVT_ARGS_RT_DESTROY_ERROR))
#define USB4_EVT_STR_RT_DESTROY_ERROR         "Usb4Rt Destroy - Status = %r\n"

//
// Usb4HiCore Destroy
//
#define USB4_EVT_ID_HI_CORE_DESTROY_ERROR     43
#define USB4_EVT_ARGS_HI_CORE_DESTROY_ERROR   1
#define USB4_EVT_CODE_HI_CORE_DESTROY_ERROR   ((USB4_EVT_ID_HI_CORE_DESTROY_ERROR << 2) | (USB4_EVT_ARGS_HI_CORE_DESTROY_ERROR))
#define USB4_EVT_STR_HI_CORE_DESTROY_ERROR    "Usb4HiCore Destroy - Status = %r\n"

//
// HiCoreRing0 Destroy
//
#define USB4_EVT_ID_RING_ZERO_DESTROY_ERROR   44
#define USB4_EVT_ARGS_RING_ZERO_DESTROY_ERROR 1
#define USB4_EVT_CODE_RING_ZERO_DESTROY_ERROR ((USB4_EVT_ID_RING_ZERO_DESTROY_ERROR << 2) | (USB4_EVT_ARGS_RING_ZERO_DESTROY_ERROR))
#define USB4_EVT_STR_RING_ZERO_DESTROY_ERROR  "HiCoreRing0 Destroy - Status = %r\n"

#endif
