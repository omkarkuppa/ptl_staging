/** @file
  Prototype of the Cpu Powermanagement related Policy library.

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

@par Specification
**/
#ifndef _CPU_POWER_MANAGEMENT_POLICY_LIB_H_
#define _CPU_POWER_MANAGEMENT_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>
#include <CpuPowerDeliveryConfig.h>

/**
  Print whole CPU config blocks of SiPolicyPpi and serial out.

  @param[in] SiPolicyPpi             The SI Policy PPI instance
**/
VOID
EFIAPI
CpuPowerManagementPrintPolicy (
  IN  SI_POLICY_PPI                 *SiPolicyPpi
  );

/**
  Print CPU_POWER_DELIVERY_CONFIG and serial out

  @param[in] CpuPowerDeliveryConfig   Pointer to a CPU_POWER_DELIVERY_CONFIG
**/
VOID
EFIAPI
CpuPowerDeliveryConfigPrint (
  IN CONST CPU_POWER_DELIVERY_CONFIG  *CpuPowerDeliveryConfig
  );

/**
  Get CPU config block table total size.

  @retval Size of CPU config block table
**/
UINT16
EFIAPI
CpuPowerManagementGetConfigBlockTotalSize (
  VOID
  );

/**
  CpuAddConfigBlocks add all Cpu config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add CPU config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CpuPowerManagementAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  Get Cpu Power Delivery related config block table total size.

  @retval Size of CPU Power Delivery block table
**/
UINT16
EFIAPI
CpuPowerDeliveryConfigBlockTotalSize (
  VOID
  );

/**
  AddCpuPowerDeliveryConfigBlocks add all Cpu Power Delivery config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add CPU config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
AddCpuPowerDeliveryConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

#endif // _CPU_POWER_MANAGEMENT_POLICY_LIB_H_
