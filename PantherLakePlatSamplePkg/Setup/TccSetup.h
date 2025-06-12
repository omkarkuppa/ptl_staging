/** @file

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

#ifndef _TCC_SETUP_H_
#define _TCC_SETUP_H_

/**
 Reset TCC DSO setup options to their default setting

 @retval EFI_SUCCESS  Successfully reset DSO setup options to their deafult setting
 @retval Other        Failed to reset DSO setup options ther deafult setting
**/
EFI_STATUS
EFIAPI
ResetTccDsoSettings (
  VOID
  );

/**
  Install TCC Setup Protocol

  @param[in] ImageHandle          Handle for this drivers loaded image protocol.
**/
VOID
InstallTccSetupProtocol (
  IN EFI_HANDLE         *ImageHandle
  );

#endif
