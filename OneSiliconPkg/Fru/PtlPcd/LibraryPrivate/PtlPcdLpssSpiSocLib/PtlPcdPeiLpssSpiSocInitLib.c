/** @file
  LPSS Private Spi Lib implementation PantherLake specific.
  All functions in this library are available in PEI,
  But do not support UEFI RUNTIME environment call.

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

@par Specification Reference:
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/Pci30.h>
#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiLpssSpiInitLib.h>
#include <Library/PeiItssLib.h>
#include <Library/LpssControllerInfoLib.h>
#include <LpssSpiConfig.h>
#include <LpssSpiHandle.h>

/**
  Build LPSS SPI Handle structures

  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in, out] SpiHandle        The Lpss SPI Handle instance
  @param[in]      SpiDeviceConfig  Lpss SPI Config Block
  @param[in]      SpiIndex         SPI Controller index

  @retval     EFI_SUCCESS           Completed successfully
              EFI_UNSUPPORTED       SOC is not supported
**/
EFI_STATUS
PtlPcdLpssSpiBuildHandle (
  IN     SI_POLICY_PPI              *SiPolicy,
  IN OUT LPSS_SPI_HANDLE            *SpiHandle,
  IN     LPSS_SPI_DEVICE_CONFIG     *SpiDeviceConfig,
  IN     UINT8                      SpiIndex
  )
{
  EFI_STATUS                       Status;
  PCI_CONFIG_SPACE_ACCESS   *CfgAccess;

  if (SpiIndex >= GetMaxSpiInterfacesNum ()) {
    return EFI_UNSUPPORTED;
  }

  SpiHandle->ControllerId = SpiIndex;

  Status = PtlPcdLpssSpiGetController (SpiIndex, &SpiHandle->Controller);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // SPI Config Block
  //
  SpiHandle->SpiConfig = SpiDeviceConfig;

  //
  // Private configuration options
  //
  CfgAccess = (PCI_CONFIG_SPACE_ACCESS *)SpiHandle->Controller->PcieCfgAccess;
  ItssGetDevIntConfig (
                    SiPolicy,
                    CfgAccess->Dev,
                    CfgAccess->Fun,
                    &SpiHandle->PrivateConfig.InterruptPin,
                    &SpiHandle->PrivateConfig.Irq);

  return EFI_SUCCESS;
}

/**
  Initialize the all SPI controllers integrated in the SOC's LPSS

  @param[in]      SiPolicy         The SI Policy PPI instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       The SOC is not supported
              EFI_NOT_FOUND         SPI Config Block not found
**/
EFI_STATUS
PtlPcdLpssSpiSocInit (
  IN  SI_POLICY_PPI         *SiPolicy
  )
{
  EFI_STATUS                Status;
  BOOLEAN                   ErrorFlag;
  UINT8                     SpiIndex;
  LPSS_SPI_CONFIG           *LpssSpiConfig;
  LPSS_SPI_HANDLE           LpssSpiHandle;

  DEBUG ((DEBUG_INFO, "%a: Start\n", __FUNCTION__));

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssSpiConfigGuid, (VOID *) &LpssSpiConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get LPSS SPI config block!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  ErrorFlag = FALSE;
  for (SpiIndex = 0; SpiIndex < GetMaxSpiInterfacesNum (); SpiIndex++) {
    Status = PtlPcdLpssSpiBuildHandle (SiPolicy, &LpssSpiHandle, &(LpssSpiConfig->SpiDeviceConfig[SpiIndex]), SpiIndex);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot build SPI handle structure for Ctrl index %d (Status %r)!\n", __FUNCTION__, SpiIndex, Status));
      DEBUG ((DEBUG_ERROR, "Skip SPI Initialization for controller index %d!\n", SpiIndex));
      ErrorFlag = TRUE;
      PtlPcdLpssSpiControllerFree (LpssSpiHandle.Controller);
      continue;
    }
    Status = LpssSpiInit (&LpssSpiHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: ERROR: Failed to initialize SPI controller #%d!\n", __FUNCTION__, SpiIndex));
      ErrorFlag = TRUE;
    }
    PtlPcdLpssSpiControllerFree (LpssSpiHandle.Controller);
  }

  DEBUG ((DEBUG_INFO, "%a: End\n", __FUNCTION__));

  if (ErrorFlag == TRUE) {
    if (EFI_ERROR (Status)) {
      return Status;
    } else {
      return EFI_UNSUPPORTED;
    }
  }

  return EFI_SUCCESS;
}
