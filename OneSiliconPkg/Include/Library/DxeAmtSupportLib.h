/** @file
  Prototype of the DxeAmtSupport library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _DXE_AMT_SUPPORT_LIB_H_
#define _DXE_AMT_SUPPORT_LIB_H_

#include <Protocol/DevicePath.h>

/**
  Checks if Manageability is supported.

  @retval True           Manageability is supported.
  @retval False          Manageability is not supported.
**/
BOOLEAN
AmtIsManageabilitySupportEnabled (
  VOID
  );

/**
  Checks if KVM session is active

  @retval True                    KVM session is active.
  @retval False                   No KVM session in progress.
**/
BOOLEAN
AmtIsKvmSessionEnabled (
  VOID
  );

/**
  Checks if there is any remote session active

  @retval True                    Remote session active.
  @retval False                   No remote session in progress.
**/
BOOLEAN
AmtIsRemoteSessionEnabled (
  VOID
  );

/**
  Get Storage Redirection Device Path

  @retval DevicePath   Pointer to Storage Redirection Device Path
  @retval NULL         Storage Redirection Device Path Not Found
**/
VOID *
AmtGetStorageRedirectionDevicePath (
  VOID
  );

/**
  Get Serial Over Lan Device Path

  @retval DevicePath   Pointer to Serial Over Lan Device Path
  @retval NULL         Serial Over Lan Device Path Not Found
**/
VOID *
AmtGetSolDevicePath (
  VOID
  );

#endif // _DXE_AMT_SUPPORT_LIB_H_
