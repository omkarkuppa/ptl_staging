/** @file
  This file contains definitions of ME BIOS Payload HOB.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _ME_BIOS_PAYLOAD_HOB_H_
#define _ME_BIOS_PAYLOAD_HOB_H_

#include <MeBiosPayloadData.h>

typedef struct {
  EFI_HOB_GUID_TYPE Header;
  UINT8             Revision;
  UINT8             Reserved[3];
  ME_BIOS_PAYLOAD   MeBiosPayload;
} ME_BIOS_PAYLOAD_HOB;

extern EFI_GUID gMeBiosPayloadHobGuid;

#endif // _ME_BIOS_PAYLOAD_HOB_H_
