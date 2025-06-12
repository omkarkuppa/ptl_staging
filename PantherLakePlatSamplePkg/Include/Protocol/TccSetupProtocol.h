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

#ifndef _TCC_SETUP_PROTOCOL_H_
#define _TCC_SETUP_PROTOCOL_H_

extern EFI_GUID  gTccSetupProtocolGuid;

///
/// Revision
///
#define TCC_SETUP_PROTOCOL_VERSION  1

typedef struct _TCC_SETUP_PROTOCOL TCC_SETUP_PROTOCOL;

/**
 Reset TCC DSO setup options to their default setting

 @retval EFI_SUCCESS  Successfully reset DSO setup options to their deafult setting
 @retval Other        Failed to reset DSO setup options ther deafult setting
**/
typedef
EFI_STATUS
(EFIAPI *TCC_DSO_RESET_DEFAULT_SETUP)(
  VOID
  );

///
/// TCC Setup Protocol
///
struct _TCC_SETUP_PROTOCOL {
  TCC_DSO_RESET_DEFAULT_SETUP      ResetTccDsoSettings;
};

#endif
