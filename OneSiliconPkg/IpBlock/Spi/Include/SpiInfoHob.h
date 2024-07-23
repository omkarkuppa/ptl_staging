/** @file
  This file contains definitions of SPI Info HOB.

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
#ifndef _SPI_INFO_HOB_H_
#define _SPI_INFO_HOB_H_

#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/HobLib.h>

extern EFI_GUID gSpiInfoHobGuid;

#pragma pack (push,1)
/**
  This structure is used to provide the information of SPI Info Hob.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  EFI_HOB_GUID_TYPE EfiHobGuidType;
  /**
    This member specifies the revision of the SPI Info HOB. This field is used
    to indicate backwards compatible changes to the protocol. Platform code that
    consumes this protocol must read the correct revision value to correctly interpret
    the content of the protocol fields.
  **/
  BOOLEAN   ExtendedBiosDecodeRangeEnable;
  UINT8     Rsvd[3];
} SPI_INFO_HOB;

#pragma pack (pop)

#endif // _SPI_INFO_HOB_H_

