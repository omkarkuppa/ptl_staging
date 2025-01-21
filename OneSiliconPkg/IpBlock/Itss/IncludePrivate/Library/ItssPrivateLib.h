/** @file
  Header file for private ITSS Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _ITSS_PRIVATE_LIB_H_
#define _ITSS_PRIVATE_LIB_H_

#include <RegisterAccess.h>

/**
  This function gets the 8254CGE bit
  After BIOSDONE, the function only works inside SMM.

  @param[in]   ItssPcrAccess     Instance of LPC sideband Register Access Protocol
  @param[out]  ClockGateState    TRUE : 8254CGE is set, FALSE: 8254CGE is clear.

  @retval  EFI_SUCCESS           Get 8254CGE status successfully.
           EFI_UNSUPPORTED       Doesn't support 8254CGE get.
**/
EFI_STATUS
ItssGet8254ClockGateState (
  IN  REGISTER_ACCESS *ItssPcrAccess,
  OUT BOOLEAN         *ClockGeteState
  );

/**
  This function sets the 8254CGE bit
  After BIOSDONE, the function only works inside SMM.

  @param[in]  ClockGateEnable    Enable or disable clock gate
  @param[in]  ItssPcrAccess      Instance of LPC sideband Register Access Protocol

  @retval  EFI_SUCCESS           Set 8254CGE status successfully.
           EFI_UNSUPPORTED       Doesn't support 8254CGE set.
**/
EFI_STATUS
ItssSet8254ClockGateState (
  IN BOOLEAN          ClockGateEnable,
  IN REGISTER_ACCESS  *ItssPcrAccess
  );

#endif // _ITSS_PRIVATE_LIB_H_
