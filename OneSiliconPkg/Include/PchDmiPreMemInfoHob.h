/** @file
  The GUID definition for HOBs for PCH DMI PreMem Info

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

@par Specification
**/

#ifndef _PCH_DMI_PREMEM_INFO_HOB_H_
#define _PCH_DMI_PREMEM_INFO_HOB_H_


///
/// PCH DMI PreMem Info HOB
//
typedef struct {
  UINT8                 PchDmiAspm;
} PCH_DMI_PREMEM_INFO;

#endif
