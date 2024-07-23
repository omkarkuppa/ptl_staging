/** @file
  DXE PcieRp policy library.

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
#ifndef _DXE_PCH_PCIERP_POLICY_LIB_H_
#define _DXE_PCH_PCIERP_POLICY_LIB_H_

#include <Protocol/PchPolicy.h>

/**
  Load DXE Config block default for Pch PCIE RP

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchPcieRpDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Print PCIE_RP_DXE_CONFIG.

  @param[in] PchPolicy            Pointer to a PCH_POLICY_PROTOCOL
**/
VOID
PchPcieRpDxePrintConfig (
  IN PCH_POLICY_PROTOCOL    *PchPolicy
  );

/**
  Get PchPcieRp config block table size.

  @retval      Size of config block
**/
UINT16
PchPcieRpDxeGetConfigBlockTotalSize (
  VOID
  );

/**
  Add PchPcieRp ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PchPcieRpDxeAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif
