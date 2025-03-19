/** @file
 Source code for the board SA configuration Pcd init functions in Pre-Memory init phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include "BoardSaConfigPreMem.h"
#include <Library/CpuPlatformLib.h>
#include <PlatformBoardId.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <PlatformBoardConfig.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>

/**
  MRC configuration init function for PEI pre-memory phase.

  @param[in]  VOID

  @retval VOID
**/
VOID
PtlSaMiscConfigInit (
  VOID
  )
{
  return;
}

/**
  Board Memory Init related configuration init function for PEI pre-memory phase.

  @param[in]  VOID

  @retval VOID
**/
VOID
PtlMrcConfigInit (
  VOID
  )
{
  return;
}

/**
  Board SA related GPIO configuration init function for PEI pre-memory phase.

  @param[in]  VOID

  @retval VOID
**/
VOID
PtlSaGpioConfigInit (
  VOID
  )
{
  return;
}

/**
  SA Display DDI configuration init function for PEI pre-memory phase.

  @param[in]  VOID

  @retval     VOID
**/
VOID
PtlSaDisplayConfigInit (
  VOID
  )
{
  VPD_DISPLAY_DDI_CONFIG *SaDisplayDdiConfigTable;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  SaDisplayDdiConfigTable = PcdGetPtr(VpdPcdDisplayDdiConfigTable);

  PcdSet64S (PcdSaDisplayConfigTable, (UINTN) SaDisplayDdiConfigTable->DdiConfigTable);
  PcdSet16S (PcdSaDisplayConfigTableSize, SaDisplayDdiConfigTable->Size);
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return;
}

/**
  Build the GopConfig Driver Hob to modify the VBT Runtime based on the SKU ID
**/
VOID
BuildGopConfigDriver (
  VOID
  )
{
  GOP_CONFIG_DRIVER_HOB  *GopConfigDriverHob;

  GopConfigDriverHob = BuildGuidHob (&gGopConfigDriverHobGuid, sizeof (GOP_CONFIG_DRIVER_HOB));
  DEBUG ((DEBUG_INFO, "GopConfigDriverHob = 0x%X\n", GopConfigDriverHob));
  ASSERT (GopConfigDriverHob != NULL);
  if (GopConfigDriverHob != NULL) {
    ZeroMem (GopConfigDriverHob, sizeof (GOP_CONFIG_DRIVER_HOB));
    GopConfigDriverHob->SkuId = LibPcdGetSku ();
    GopConfigDriverHob->SoftwareId = PcdGet8 (VpdPcdGopConfigSoftwareId);
    GopConfigDriverHob->DisplayId = (UINT8) PcdGet16 (PcdDisplayId);
    GopConfigDriverHob->PlatformId = 0;
    DEBUG ((DEBUG_INFO, "GopConfig SkuId = 0x%X\n", GopConfigDriverHob->SkuId));
    DEBUG ((DEBUG_INFO, "GopConfig SoftwareId = 0x%X\n", GopConfigDriverHob->SoftwareId));
    DEBUG ((DEBUG_INFO, "GopConfig DisplayId = 0x%X\n", GopConfigDriverHob->DisplayId));
    DEBUG ((DEBUG_INFO, "GopConfig PlatformId = 0x%X\n", GopConfigDriverHob->PlatformId));
  }
}

/**
  Board USB related configuration init function for PEI pre-memory phase.

  @param[in]  VOID

  @retval VOID
**/
VOID
PtlSaUsbConfigInit (
  VOID
  )
{
  //
  // Set TCSS Port Capability PCD based on USB and USBC Connector HOBs
  //
  SetTcssPortCapPcd ();

  PcdSet8S (PcdUsbCUcmMode, PcdGet8 (VpdPcdUsbCUcmMode));

  return;
}
