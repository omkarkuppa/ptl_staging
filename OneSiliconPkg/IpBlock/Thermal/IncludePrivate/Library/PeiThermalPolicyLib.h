/** @file
  PEI Thermal policy library.

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

@par Specification Reference:
**/
#ifndef _PEI_THERMAL_POLICY_LIB_H_
#define _PEI_THERMAL_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print thermal config block settings.

  @param[in] SiPolicyPpi    Instance of SI_POLICY_PPI
**/
VOID
ThermalPrintConfig (
  IN  SI_POLICY_PPI         *SiPolicyPpi
  );

/**
  Print IOE thermal config block settings.

  @param[in] SiPolicyPpi    Instance of SI_POLICY_PPI
**/
VOID
ThermalPrintIoeConfig (
  IN  SI_POLICY_PPI         *SiPolicyPpi
  );

/**
  Load thermal config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
ThermalLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Load IOE thermal config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
ThermalLoadIoeConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get Thermal config block table size.

  @retval      Size of config block
**/
UINT16
ThermalGetConfigBlockTotalSize (
  VOID
  );

/**
  Get Thermal IOE config block table size.

  @retval      Size of config block
**/
UINT16
ThermalGetIoeConfigBlockTotalSize (
  VOID
  );

/**
  Add Thermal ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
ThermalAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Add Thermal IOE ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
ThermalAddIoeConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _PEI_THERMAL_POLICY_LIB_H_
