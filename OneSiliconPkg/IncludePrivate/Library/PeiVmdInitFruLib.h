/** @file
  Header file for VMD Lib.

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
#ifndef _PEI_VMD_INIT_FRU_LIB_H_
#define _PEI_VMD_INIT_FRU_LIB_H_

#include <VmdInfoHob.h>
#include <Register/VmdRegs.h>

/**
  This function detects the mass storage devices attached to Peg60 and PCH Pcie Root ports and the Sata controller.
  @param[in]  *VmdInfoHob     pointer to VMD info Hob to store the information of detected devices.
  @retval     EFI_SUCCESS     Successfully initialized configurations.
**/

EFI_STATUS
VmdDetectPcieStorageDevices (
  VMD_INFO_HOB          *VmdInfoHob
);

/**
  This function detects whether Root Bus1 is supported and if so, programs the additional set of registers for Root Bus 1.
  @param[in]      *VmdInfoHob  pointer to VMD info Hob to read the information of detected devices.
  @param[in, out] *VmConfig    pointer to VmConfig register structure.
**/

VOID
VmdInitFruApi (
  IN     VMD_INFO_HOB          *VmdInfoHob,
  IN OUT VMCONFIG_IOC_VMD_STRUCT *VmConfig
);

#endif /* _PEI_VMD_INIT_FRU_LIB_H_ */
