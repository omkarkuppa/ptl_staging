/** @file
 Definitions for HECI Control Filter functionality driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _HECI_CONTROL_FILTER_LIB_H_
#define _HECI_CONTROL_FILTER_LIB_H_

/**
  HECI Control filter procedure

  This function returns HECI function number that message has been provided to

  @param[in]     SecurityEngine   Engine to communicate with
  @param[in]     HeciDevice       HECI device to communicate through
  @param[in]     HostAddress      Host address
  @param[in]     EngineAddress    Engine address
  @param[in]     MessageBody      Body of the message to send

  @retval EFI_SUCCESS             Success
  @retval EFI_INVALID_PARAMETER   Bad input parameters
  @retval EFI_ABORTED             Message is not allowed for given Engine's HECI device
**/
EFI_STATUS
FilterHeciMessage (
  IN  SECURITY_ENGINE     SecurityEngine,
  IN  HECI_DEVICE         HeciDevice,
  IN  UINT8               HostAddress,
  IN  UINT8               EngineAddress,
  IN  UINT32              *MessageBody
  );

#endif // _HECI_CONTROL_FILTER_LIB_H_
