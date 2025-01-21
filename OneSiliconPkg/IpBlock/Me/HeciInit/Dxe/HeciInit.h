/** @file
  Definitions for HECI driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2006 Intel Corporation.

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
#ifndef _HECI_INIT_H
#define _HECI_INIT_H

#include <Protocol/HeciProtocol.h>

#define HECI_MOCK_RECV_BUFFER_SIZE 512

///
/// HECI private data structure
///
typedef struct {
  UINTN                   Signature;
  EFI_HANDLE              Handle;
  UINT32                  RevisionInfo;
  HECI_PROTOCOL           HeciCtlr;
  LIST_ENTRY              Queue;
  EFI_EVENT               AsyncMsgTimer;
} HECI_INSTANCE;

/*
  Installs HECI_FLOW_PROTOCOL and registers a callback to uninstall it on exit boot services

  @retval EFI_SUCCESS       Protocol was installed, error otherwise
*/
EFI_STATUS
EFIAPI
InitializeHeciFlow (
  VOID
  );

#endif
