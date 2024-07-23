/** @file
  Header file for public ITSS Library for PTL PCD.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _PTLPCD_ITSS_LIB_H_
#define _PTLPCD_ITSS_LIB_H_

/**
  This function sets the 8254CGE bit
  After BIOSDONE, the function only works inside SMM.

  @param[in]  ClockGateEnable    Enable or disable clock gate

  @retval  EFI_SUCCESS           Set 8254CGE status successfully.
           EFI_UNSUPPORTED       Doesn't support 8254CGE set.
**/
EFI_STATUS
PtlPcdItssSet8254ClockGateState (
  IN BOOLEAN          ClockGateEnable
  );

#endif // PTLPCD__ITSS_LIB_H_
