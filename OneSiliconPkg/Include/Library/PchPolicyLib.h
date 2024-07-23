/** @file
  Prototype of the PeiPchPolicy library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef _PEI_PCH_POLICY_LIB_H_
#define _PEI_PCH_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>
#include <SataConfig.h>
#include <RstConfig.h>
#include <HsioSataConfig.h>

/**
  Print whole PCH_PREMEM_POLICY_PPI and serial out.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
**/
VOID
EFIAPI
PchPreMemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi
  );

/**
  Print whole PCH_PREMEM_POLICY_PPI and serial out.

  @param[in] PchPreMemPolicyPpi       The RC PREMEM Policy PPI instance
**/
VOID
EFIAPI
AttachedPchPreMemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI          *PchPreMemPolicyPpi
  );

/**
  Print whole SI_POLICY_PPI and serial out.

  @param[in] SiPolicyPpi               The RC Policy PPI instance
**/
VOID
EFIAPI
PchPrintPolicyPpi (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  );

/**
  Get PCH PREMEM config block table total size.

  @retval                               Size of PCH PREMEM config block table
**/
UINT16
EFIAPI
PchGetPreMemConfigBlockTotalSize (
  VOID
  );

/**
  Get PCH config block table total size.

  @retval                               Size of PCH config block table
**/
UINT16
EFIAPI
PchGetConfigBlockTotalSize (
  VOID
  );

/**
  PchAddPreMemConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchAddPreMemConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  PchAddConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  Get SOC-S PREMEM config block table total size.

  @retval                               Size of PCH PREMEM config block table
**/
UINT16
EFIAPI
SocSGetPreMemConfigBlockTotalSize (
  VOID
  );

/**
  Get PCH PREMEM config block table total size.

  @retval                               Size of PCH PREMEM config block table
**/
UINT16
EFIAPI
AttachedPchGetPreMemConfigBlockTotalSize (
  VOID
  );

/**
  SocSAddPreMemConfigBlocks add all SOC-S PREMEM config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add SOC-S PREMEM config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SocSAddPreMemConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  AttachedPchAddPreMemConfigBlocks add all PCH PREMEM config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH PREMEM config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
AttachedPchAddPreMemConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  Get PCH config block table total size.

  @retval                               Size of PCH config block table
**/
UINT16
EFIAPI
SocSGetConfigBlockTotalSize (
  VOID
  );

/**
  Get PCH config block table total size.

  @retval                               Size of PCH config block table
**/
UINT16
EFIAPI
AttachedPchGetConfigBlockTotalSize (
  VOID
  );

/**
  SocSAddConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SocSAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  AttachedPchAddConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
AttachedPchAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  Get Sata Config Policy

  @param[in]  SiPolicy            The RC Policy PPI instance
  @param[in]  SataCtrlIndex       SATA controller index

  @retval     SataConfig          Pointer to Sata Config Policy
**/
SATA_CONFIG *
GetPchSataConfig (
  IN SI_POLICY_PPI      *SiPolicy,
  IN UINT32             SataCtrlIndex
  );

/**
  Get Rst Config Policy

  @param[in]  SiPolicy            The RC Policy PPI instance
  @param[in]  SataCtrlIndex       SATA controller index

  @retval     RstConfig          Pointer to Rst Config Policy
**/
RST_CONFIG *
GetPchRstConfig (
  IN SI_POLICY_PPI      *SiPolicy,
  IN UINT32             SataCtrlIndex
  );

/**
  Get Hsio Sata Pre Mem Config Policy

  @param[in]  SiPolicy            The RC Policy PPI instance
  @param[in]  SataCtrlIndex       SATA controller index

  @retval     Pointer to Hsio Sata Pre Mem Config Policy
**/
PCH_HSIO_SATA_PREMEM_CONFIG *
GetPchHsioSataPreMemConfig (
  IN SI_PREMEM_POLICY_PPI *SiPreMemPolicy,
  IN UINT32               SataCtrlIndex
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchPcieConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHsioConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

#endif // _PEI_PCH_POLICY_LIB_H_
