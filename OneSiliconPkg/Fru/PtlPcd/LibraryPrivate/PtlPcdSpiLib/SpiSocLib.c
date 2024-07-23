/** @file
  SPI SOC library

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/SpiSocLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/SpiRegs.h>
#include <Defines/PchReservedResources.h>
#include <Library/HobLib.h>
#include <SpiConfigHob.h>
#include <PchConfigHobCommon.h>

/**
  Get SPI Flash controller instance

  @param[in, out] SpiHandle       SPI controller handle
  @param[in]      SiPolicy        The Silicon Policy PPI instance - OPTIONAL

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - SpiHandle NULL
              EFI_NOT_FOUND         - Didn't find SPI Config Block
**/
EFI_STATUS
GetSpiHandle (
  IN OUT  SPI_HANDLE       *SpiHandle,
  IN      SI_POLICY_PPI    *SiPolicy
  )
{
  EFI_STATUS                   Status;
  EFI_PEI_HOB_POINTERS         HobPtr;
  SPI_CONFIG_HOB               *SpiConfigHob;

  if (SpiHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (SpiHandle, sizeof (SPI_HANDLE));
  SpiHandle->MmioBase = PCH_SPI_BASE_ADDRESS;
  SpiHandle->PciCfgBase = SpiPciCfgBase ();

  //
  // We need to handle SiPolicy being NULL in cases where SPI_HANDLE is needed
  // in example in PreMem but SiPolicy is not available there. Because of
  // that we mark SiPolicy as optional parameter and act accordingly, still
  // allowing for grabbing SPI_HANDLE
  //
  if (SiPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gSpiConfigGuid, (VOID *) &SpiHandle->SpiConfig);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "%a - Cannot find SPI config block, Status %r\n",
        __FUNCTION__, Status
        ));

      SpiHandle->SpiConfig = NULL;

      return EFI_NOT_FOUND;
    }
  }

  HobPtr.Guid   = GetFirstGuidHob (&gPchSpiConfigHobGuid);
  if (HobPtr.Guid != NULL) {
    SpiConfigHob    = (SPI_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);
    SpiHandle->SpiConfigHob      = SpiConfigHob;
  }

  return EFI_SUCCESS;
}

/**
  Clears BIOS Write Protect Disable bit
**/
VOID
SocSpiClearBiosWriteProtectDisable (
  VOID
  )
{
  SPI_HANDLE    SpiHandle;
  EFI_STATUS    Status;

  Status = GetSpiHandle (&SpiHandle, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a - cannot get SPI controller handle, aborting\n",
      __FUNCTION__
      ));
    return;
  }

  //
  // Disable BIOSWE bit to protect BIOS
  //
  PciSegmentAnd8 (
    SpiHandle.PciCfgBase + R_SPI_CFG_BIOS_SPI_BC,
    (UINT8) ~B_SPI_CFG_BIOS_SPI_BC_WPD
    );
}
