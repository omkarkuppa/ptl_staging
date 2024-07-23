/** @file
  This file contains definitions of Si Schedule Reset HOB.

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
#ifndef _SI_SCHEDULE_RESET_HOB_H_
#define _SI_SCHEDULE_RESET_HOB_H_

/**
  This structure is used to provide information about PCH Resets
**/
typedef struct {
  EFI_RESET_TYPE             ResetType;
  PCH_RESET_DATA             ResetData;
} SI_SCHEDULE_RESET_HOB;

extern EFI_GUID gSiScheduleResetHobGuid;

#endif // _SI_SCHEDULE_RESET_HOB_H_

