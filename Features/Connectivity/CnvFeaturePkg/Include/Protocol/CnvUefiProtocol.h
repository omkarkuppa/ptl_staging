/** @file
  CNV UEFI Variable Configuration Protocol.

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

@par Specification
**/

#ifndef _CNV_UEFI_VARIABLE_CONFIG_PROTOCOL_H_
#define _CNV_UEFI_VARIABLE_CONFIG_PROTOCOL_H_

typedef struct _CNV_UEFI_VARIABLE_CONFIG_PROTOCOL CNV_UEFI_VARIABLE_CONFIG_PROTOCOL;

//
// Extern the GUID for protocol users.
//
extern EFI_GUID  gCnvUefiVariableConfigProtocolGuid;

/**
  Protocol revision number
**/
#define CNV_UEFI_CONFIG_PROTOCOL_REVISION  1

struct _CNV_UEFI_VARIABLE_CONFIG_PROTOCOL {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT8              Revision;
  UEFI_CNV_CONFIG    UefiCnvConfig;
};

#endif
