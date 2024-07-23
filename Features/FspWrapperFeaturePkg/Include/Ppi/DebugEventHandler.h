/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef __FSPWRAPPER_DEBUG_EVENT_HANDLER_PPI_H__
#define __FSPWRAPPER_DEBUG_EVENT_HANDLER_PPI_H__

#include <Pi/PiStatusCode.h>

//
// Global ID for the DEBUG_EVENT_HANDLER_PPI
//
#define DEBUG_EVENT_HANDLER_PPI_GUID \
  { \
    0x6e6f5daa, 0x77cb, 0x48ff, { 0xb3, 0x18, 0x2d, 0x5e, 0x62, 0x0, 0xd0, 0x1e } \
  }

///
/// Forward declaration for the DEBUG_EVENT_HANDLER_PPI
///
typedef struct _DEBUG_EVENT_HANDLER_PPI DEBUG_EVENT_HANDLER_PPI;

/*
  FSP Wrapper debug event handler

  @param[in] Type                   Indicates the type of event being reported.
                                    See MdePkg/Include/Pi/PiStatusCode.h for the definition of EFI_STATUS_CODE_TYPE.
  @param[in] Value                  Describes the current status of a hardware or software entity.
                                    This includes information about the class and subclass that is used to classify the entity as well as an operation.
                                    For progress events, the operation is the current activity. For error events, it is the exception.
                                    For debug events, it is not defined at this time.
                                    See MdePkg/Include/Pi/PiStatusCode.h for the definition of EFI_STATUS_CODE_VALUE.
  @param[in] Instance               The enumeration of a hardware or software entity within the system.
                                    A system may contain multiple entities that match a class/subclass pairing. The instance differentiates between them.
                                    An instance of 0 indicates that instance information is unavailable, not meaningful, or not relevant.
                                    Valid instance numbers start with 1.
  @param[in] *CallerId              This parameter can be used to identify the sub-module within the FSP generating the event.
                                    This parameter may be NULL.
  @param[in] *Data                  This optional parameter may be used to pass additional data. The contents can have event-specific data.
                                    For example, the FSP provides a EFI_STATUS_CODE_STRING_DATA instance to this parameter when sending debug messages.
                                    This parameter is NULL when no additional data is provided.

  @retval EFI_SUCCESS               The event was handled successfully.
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid.
  @retval EFI_DEVICE_ERROR          The event handler failed.
*/
typedef
EFI_STATUS
(EFIAPI *FSP_EVENT_HANDLER) (
  IN          EFI_STATUS_CODE_TYPE   Type,
  IN          EFI_STATUS_CODE_VALUE  Value,
  IN          UINT32                 Instance,
  IN OPTIONAL EFI_GUID               *CallerId,
  IN OPTIONAL EFI_STATUS_CODE_DATA   *Data
  );

///
/// This PPI contains a set of services to print message to debug output device
///
struct _DEBUG_EVENT_HANDLER_PPI {
  FSP_EVENT_HANDLER                DebugEventHandler;
};

extern EFI_GUID gDebugEventHandlerPpiGuid;

#endif

