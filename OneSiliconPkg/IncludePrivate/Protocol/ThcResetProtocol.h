/** @file
  Touch Host Controller Reset API

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

#ifndef _THC_RESET_PROTOCOL_H_
#define _THC_RESET_PROTOCOL_H_

///
/// THC Reset Protocol
/// The interface function is to control THC reset pin
///

#define THC_RESET_PROTOCOL_REVISION  1
typedef struct _THC_RESET_PROTOCOL THC_RESET_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *THC_RESET_SET_OUTPUT_VALUE) (
  IN THC_RESET_PROTOCOL  *This,
  IN UINT8               InstanceId,
  IN UINT32              Value
  );

/**
  THC reset Protocol
  The interface function is to control THC reset pin

  <b>Revision 1:</b>
  - Initial version.
**/
struct _THC_RESET_PROTOCOL {
  UINT8                         Revision;
  THC_RESET_SET_OUTPUT_VALUE    SetOutputValue;
};


#endif // _THC_RESET_PROTOCOL_H_
