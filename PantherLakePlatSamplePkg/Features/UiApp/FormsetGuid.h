/** @file
  FrontPage routines to handle the callbacks and browser calls

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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


//**/
#ifndef _FORMSET_GUID_H_
#define _FORMSET_GUID_H_

#include "BootMaint/FormGuid.h"

#define FRONT_PAGE_FORMSET_GUID  { 0x9e0c30bc, 0x3f06, 0x4ba6, 0x82, 0x88, 0x9, 0x17, 0x9b, 0x85, 0x5d, 0xbe }

// Used by Boot manager form
#define BOOT_MANAGER_FORMSET_GUID  { 0x847bc3fe, 0xb974, 0x446d, 0x94, 0x49, 0x5a, 0xd5, 0x41, 0x2e, 0x99, 0x3b }

#define BOOT_MANAGER_FORM_ID     0x1000


// Used by Device manager form.
#define DEVICE_MANAGER_FORMSET_GUID  { 0x3ebfa8e6, 0x511d, 0x4b5b, 0xa9, 0x5f, 0xfb, 0x38, 0x26, 0xf, 0x1c, 0x27 }


#define DEVICE_KEY_OFFSET                  0x2000
#define NETWORK_DEVICE_LIST_KEY_OFFSET     0x2100
#define MAX_KEY_SECTION_LEN                0x0100

#define QUESTION_NETWORK_DEVICE_ID         0x2FFF

#define LABEL_END                          0xffff
#define LABEL_FORM_ID_OFFSET               0x0100
#define LABEL_VBIOS                        0x0040


#endif
