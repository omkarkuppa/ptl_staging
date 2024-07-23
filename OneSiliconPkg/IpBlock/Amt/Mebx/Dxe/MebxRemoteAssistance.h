/** @file
  Remote Assistance support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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

#ifndef _MEBX_REMOTE_ASSISTANCE_H_
#define _MEBX_REMOTE_ASSISTANCE_H_

/**
  Perform all steps necessary  for connection and execution of CIRA/CILA flow.

  @retval EFI_SUCCESS             Remote Assistance process completed
  @retval EFI_UNSUPPORTED         Remote Assistance unsupported for current AMT network connection
  @retval Others                  Other error has occured
**/
EFI_STATUS
ActivateRemoteAssistance (
  VOID
  );

#endif // _MEBX_REMOTE_ASSISTANCE_H_
