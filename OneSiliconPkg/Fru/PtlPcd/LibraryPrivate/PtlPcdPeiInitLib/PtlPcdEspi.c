/** @file
  The ESPI initialization After Memory.

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

#include "PtlPcdInitPei.h"

#include <ConfigBlock.h>
#include <EspiConfig.h>
#include <EspiHandle.h>
#include <Library/PeiEspiInitLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>

/**
  Build a ESPI handle for PTL SOC DIE

  @param[in]  SiPolicy            SiPolicyPpi instance
  @param[in]  P2SbAccess          P2SB Register Access structure
  @param[in]  P2SbCtrl            P2SB Controller structure
  @param[out] EspiHandle          On output pointer to initialized ESPI handle
**/
VOID
PchEspiGetHandle (
  IN  SI_POLICY_PPI                 *SiPolicy,
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *P2SbAccess,
  IN  P2SB_CONTROLLER               *P2SbCtrl,
  OUT ESPI_HANDLE                   *EspiHandle
  )
{
  EFI_STATUS        Status;
  PCH_ESPI_CONFIG   *EspiConfig;
  P2SB_PORT_16_ID   P2SBPid;

  //
  // PCIe Config Space Base Address
  //
  EspiHandle->PciCfgBase = EspiPciCfgBase();

  //
  // eSPI public configuration
  //
  Status = GetConfigBlock ((VOID *) SiPolicy, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);
  EspiHandle->EspiConfig = EspiConfig;

  //
  // eSPI private configuration
  //
  ZeroMem (&EspiHandle->EspiPrivateConfig, sizeof (ESPI_PRIVATE_CONFIG));
  EspiHandle->EspiPrivateConfig.PmSupport          = TRUE;
  EspiHandle->EspiPrivateConfig.EspiErrorReporting = FALSE;
  EspiHandle->EspiPrivateConfig.BmeControl         = TRUE;

  //
  // Register Access - PCR
  //
  P2SBPid.Pid16bit = PTL_SID_F2_PID_SPBC;
  PtlPcdGetP2SbController (P2SbCtrl, P2SBPid);
  BuildP2SbSidebandAccess (
    P2SbCtrl,
    P2SBPid.PortId.LocalPid,
    0,
    P2SbPrivateConfig,
    P2SbMmioAccess,
    FALSE,
    P2SbAccess
    );
  EspiHandle->EspiPcrAccess = &P2SbAccess->Access;
}

/**
  Initialize ESPI controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
PtlPcdEspiInit (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  ESPI_HANDLE                     EspiHandle;
  P2SB_SIDEBAND_REGISTER_ACCESS   P2SbAccess;
  P2SB_CONTROLLER                 P2SbCtrl;

  PchEspiGetHandle (SiPolicy, &P2SbAccess, &P2SbCtrl, &EspiHandle);

  if (EspiHandle.EspiConfig->LgmrEnable) {
    PchLpcMemRangeSet (PCH_ESPI_LGMR_BASE_ADDRESS);
  }

  EspiInit (&EspiHandle);

  EspiSetTimeoutValues (&EspiHandle, 0);
}

