/** @file
  EFI SMM Variable Protocol is related to EDK II-specific implementation of variables
  and intended for use as a means to store data in the EFI SMM environment.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#ifndef __SMM_VARIABLE_H__
#define __SMM_VARIABLE_H__

#define EFI_SMM_VARIABLE_PROTOCOL_GUID \
  { \
    0xed32d533, 0x99e6, 0x4209, { 0x9c, 0xc0, 0x2d, 0x72, 0xcd, 0xd9, 0x98, 0xa7 } \
  }

typedef struct _EFI_SMM_VARIABLE_PROTOCOL  EFI_SMM_VARIABLE_PROTOCOL;

///
/// EFI SMM Variable Protocol is intended for use as a means
/// to store data in the EFI SMM environment.
///
struct _EFI_SMM_VARIABLE_PROTOCOL {
  EFI_GET_VARIABLE            SmmGetVariable;
  EFI_GET_NEXT_VARIABLE_NAME  SmmGetNextVariableName;
  EFI_SET_VARIABLE            SmmSetVariable;
  EFI_QUERY_VARIABLE_INFO     SmmQueryVariableInfo;
};

extern EFI_GUID gEfiSmmVariableProtocolGuid;

#endif
