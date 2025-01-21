/** @file
  Struct and GUID definitions for CpuPowerManagementDataHob

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

@par Specification Reference:
**/
#ifndef _CPU_POWER_MANAGEMENT_DATA_HOB_H_
#define _CPU_POWER_MANAGEMENT_DATA_HOB_H_

extern EFI_GUID gCpuPowerManagementDataHobGuid;

/**
  Revision for the Cpu Power Management data hob.
  Any backwards compatible changes to this protocol will result in an update in the revision number.
  Major changes will require publication of a new protocol

  <b>Revision 1</b>: - Initial version
**/

#define CPU_POWER_MANAGEMENT_DATA_HOB_REVISION 1

///
/// This HOB is used to pass only the required information from PEI for DXE consumption.
///
typedef struct {
  UINT8                         Revision;
  UINT8                         EnableRp;                                         /// Enable/Disable Resource Priority Feature
} CPU_POWER_MANAGEMENT_DATA_HOB;
#endif
