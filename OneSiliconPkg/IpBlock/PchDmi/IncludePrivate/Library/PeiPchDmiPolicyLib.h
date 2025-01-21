/** @file
  PEI PchDmi policy library.

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
#ifndef _PEI_PCH_DMI_POLICY_LIB_H_
#define _PEI_PCH_DMI_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print PCH_DMI_PREMEM_CONFIG and serial out.

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
PchDmiPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PchDmiPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get PchDmi config block table size.

  @retval      Size of config block
**/
UINT16
PchDmiPreMemGetConfigBlockTotalSize (
  VOID
  );

/**
  Add PchDmi ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PchDmiPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Print PCH_DMI_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
PchDmiPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
PchDmiLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get PchDmi config block table size.

  @retval      Size of config block
**/
UINT16
PchDmiGetConfigBlockTotalSize (
  VOID
  );

/**
  Add PchDmi ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PchDmiAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Load Config block default for UpDmi

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
DmiUpPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get UpDmi config block table size.

  @retval      Size of config block
**/
UINT16
DmiUpPreMemGetConfigBlockTotalSize (
  VOID
  );

/**
  Add UpDmi ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
DmiUpPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Load Config block default for UpDmi

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
DmiUpLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get UpDmi config block table size.

  @retval      Size of config block
**/
UINT16
DmiUpGetConfigBlockTotalSize (
  VOID
  );

/**
  Add UpDmi ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
DmiUpAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _PEI_PCH_DMI_POLICY_LIB_H_
