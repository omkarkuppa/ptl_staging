/** @file
  Header file for DciPrivateLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _DCI_PRIVATE_LIB_H_
#define _DCI_PRIVATE_LIB_H_

#include <RegisterAccess.h>

/**
  Return HDCI Debug Enabled status.

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE   HDCI/HEEN Debug is enabled
  @retval FALSE  HDCI/HEEN Debug is disabled
**/
BOOLEAN
IsHdciDebugEnabled (
  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  Return DCI Debug Enabled status.

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE   DCI Debug is enabled
  @retval FALSE  DCI Debug is disabled
**/
BOOLEAN
IsDciDebugEnabled (
  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  Return USB2 DbC enable status.

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE   USB2 DbC is enabled
  @retval FALSE  USB2 DbC is disabled
**/
BOOLEAN
IsUsb2DbcDebugEnabled (
  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  Return PCH USB DbC connection status

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE   PCH USB DbC is connected
  @retval FALSE  PCH USB DbC is not connected
**/
BOOLEAN
IsPchUsbDbcConnected (
  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  Return DCI host connection status

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE   DCI host is connected
  @retval FALSE  DCI host is not connected
**/
BOOLEAN
IsDciHostConnected (
  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  Disable DCI clock request override.
**/
VOID
DciDisClkReq (
  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  Enable DCI clock request override.
**/
VOID
DciEnClkReq (
  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  DCI keep early trace
**/
VOID
DciKeepEarlyTrace (
  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  DCI disable early trace
**/
VOID
DciDisableEarlyTrace (
  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  Return status of keep early trace

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE    Keep early trace
  @retval FALSE   Stop early trace
**/
BOOLEAN
IsKeepEarlyTrace (
  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  Returns status if any DbC options are enabled

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE    Either USB2DbC or USB3DbC is enabled
  @retval FALSE   Both USB2DbC and USB3DbC are disabled
**/
BOOLEAN
IsAnyDbcEnabled (
  REGISTER_ACCESS            *DciPcrAccess
  );
#endif // _DCI_PRIVATE_LIB_H_
