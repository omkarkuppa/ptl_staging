/** @file
  PEI HostBridge policy library.

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
#ifndef _PEI_HOSTBRIDGE_POLICY_LIB_H_
#define _PEI_HOSTBRIDGE_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HostBridgeLoadPeiConfigPreMemDefault (
  IN VOID   *ConfigBlockPointer
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HostBridgeLoadPeiConfigDefault (
  IN VOID   *ConfigBlockPointer
  );

/**
  Get HostBridge PEI PreMem config block table total size.

  @retval     Size of HostBridge PEI PreMem config block table
**/
UINT16
EFIAPI
HostBridgeGetPeiConfigBlockTotalSizePreMem (
  VOID
  );

/**
  Get HostBridge PEI phase config block table total size.

  @retval     Size of HostBridge PEI config block table
**/
UINT16
EFIAPI
HostBridgeGetPeiConfigBlockTotalSize (
  VOID
  );

/**
  HostBridgeAddPeiConfigBlockPreMem add all HostBridge PreMem PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Ip config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
HostBridgeAddPeiConfigBlockPreMem (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  HostBridgeAddPeiConfigBlock add all HostBridge PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Ip config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
HostBridgeAddPeiConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Print HostBridge Config

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
HostBridgePreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi
  );

/**
  Print HostBridge PEI Config

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
HostBridgePeiPrintConfig (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  );

#endif // _PEI_HOSTBRIDGE_POLICY_LIB_H_
