/** @file
  Definitions for HECI SMM driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _HECI_SMM_INIT_H
#define _HECI_SMM_INIT_H

#include <Protocol/HeciProtocol.h>

#define HECI_PRIVATE_DATA_SIGNATURE         SIGNATURE_32 ('H', 'e', 'c', 'i')
#define HECI_ROUND_UP_BUFFER_LENGTH(Length) ((UINT32) ((((Length) + 3) / 4) * 4))

///
/// HECI private data structure
///
typedef struct {
  EFI_HANDLE              Handle;
  HECI_PROTOCOL           HeciCtlr;
} HECI_INSTANCE;

#endif
