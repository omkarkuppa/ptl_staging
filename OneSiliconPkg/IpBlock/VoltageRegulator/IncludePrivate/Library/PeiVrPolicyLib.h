/** @file
  Header file for the VR Policy Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _PEI_VR_POLICY_LIB_H
#define _PEI_VR_POLICY_LIB_H

#include <Ppi/SiPolicy.h>

/**
  Print whole CPU Config blocks of SI_PREMEM_POLICY_PPI and serial out in PreMem.

  @param[in] SiPreMemPolicyPpi The SI Pre-Mem Policy PPI instance
**/
VOID
EFIAPI
CpuPowerMgmtVrConfigPrint (
  IN  SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  );

/**
  Load Config block default

  @param[in,out] ConfigBlockPointer  Pointer to config block
**/
VOID
EFIAPI
LoadCpuPowerMgmtVrConfigDefault (
  IN OUT VOID *ConfigBlockPointer
  );


/**
  Get VR config block total size.

  @retval Size of config block
**/
UINT16
EFIAPI
GetCpuPowerMgmtVrConfigBlockTotalSize (
  VOID
  );

/**
  Add VR ConfigBlock.

  @param[in] ConfigBlockTableAddress The pointer to config block table

  @retval EFI_SUCCESS                The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES       Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
AddCpuPowerMgmtVrConfigBlock (
  IN VOID *ConfigBlockTableAddress
  );

#endif //_PEI_VR_POLICY_LIB_H
