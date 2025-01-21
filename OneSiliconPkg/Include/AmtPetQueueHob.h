/** @file
  Definition of AmtPetQueueHob data structures

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#ifndef _AMT_PET_QUEUE_HOB_H_
#define _AMT_PET_QUEUE_HOB_H_


///
/// Queue Hob.
/// This hob is used by PEI Status Code generic driver before Intel AMT PEI status code driver
/// starts.
/// 1) If AMT driver is not ready yet, StatusCode driver can save information
///    to QueueHob.
/// 2) If after AMT driver start, but ME is not ready, the AMT
///    driver can also save information to QueueHob.
/// Later, when ME is ready, AMT driver will send all the
/// message in the QueueHob.
///
typedef struct {
  EFI_HOB_GUID_TYPE     EfiHobGuidType; ///< The GUID type hob header
  EFI_STATUS_CODE_VALUE Value;          ///< Status code value
  EFI_STATUS_CODE_TYPE  Type;           ///< Status code type
} AMT_PET_QUEUE_HOB;

extern EFI_GUID gAmtPetQueueHobGuid;

#endif
