/** @file
  Header file for PeiTelemetryPolicyLib.

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

#ifndef TELEMETRY_PEI_POLICY_LIB_H_
#define TELEMETRY_PEI_POLICY_LIB_H_

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TelemetryLoadPeiConfigDefault (
  IN VOID   *ConfigBlockPointer
  );

/**
  Get Telemetry PEI config block table total size.

  @retval     Size of Telemetry PEI config block table
**/
UINT16
EFIAPI
TelemetryGetPeiConfigBlockTotalSize (
  VOID
  );

/**
  TelemetryAddConfigBlocks add all Telemetry PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Ip config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
TelemetryAddPeiConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  This function prints the Telemetry PEI phase policy.

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
TelemetryPrintPeiPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TelemetryLoadPeiConfigPreMemDefault (
  IN VOID   *ConfigBlockPointer
  );

/**
  Get Telemetry PEI PreMem config block table total size.

  @retval     Size of Telemetry PEI PreMem config block table
**/
UINT16
EFIAPI
TelemetryGetPeiConfigBlockTotalSizePreMem (
  VOID
  );

/**
  TelemetryAddPeiConfigBlockPreMem add all Telemetry PreMem PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Ip config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
TelemetryAddPeiConfigBlockPreMem (
  IN VOID           *ConfigBlockTableAddress
  );
/**
  This function prints the Telemetry PreMem PEI phase policy.

  @param[in] SiPreMemPolicyPpi    Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
TelemetryPrintPeiPolicyPpiPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );
#endif
