/** @file
  Initializes UFS host controller located on SCS Iosf2Ocp bridge.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Register/ScsUfsRegs.h>
#include "PeiScsInitInternal.h"

/**
  Configure Ufs controller

  @param[in] PciBaseAddress  PCI config base address of the controller
  @param[in] MmioBase        MMIO base address
**/
STATIC
VOID
ScsUfsInitMmioRegisters (
  IN UINT64  PciBaseAddress,
  IN UINTN   MmioBase
  )
{
  ScsControllerEnableMmio (PciBaseAddress, MmioBase);
  //
  // Configure UFS REF_CLK
  //
  MmioWrite32 (MmioBase + R_UFS_MEM_REF_CLK, V_UFS_MEM_REF_CLK);
  ScsControllerDisableMmio (PciBaseAddress);
}

/**
  Configure Ufs MMIO Register AUTO_LTR_VALUE (Offset 0x808)

  @param[in] PciBaseAddress  PCI config base address of the controller
  @param[in] MmioBase        MMIO base address
**/
STATIC
VOID
ScsUfsSetMmioAutoLtrRegister (
  IN UINT64  PciBaseAddress,
  IN UINTN   MmioBase
  )
{
  ScsControllerEnableMmio (PciBaseAddress, MmioBase);
  //
  // Configure UFS AUTO_LTR_VALUE (Offset 0x808) register with the below values -
  //  auto_ltr_val.reserved0=0x0
  //  auto_ltr_val.snoop_requirment=0x1
  //  auto_ltr_val.reserved_low_2=0x0
  //  auto_ltr_val.snoop_latency_scale=0x2
  //  auto_ltr_val.snoop_value=0x28A (650usec)
  //
  MmioWrite32 (MmioBase + R_SCS_MEM_UFS_AUTO_LTR_VALUE, 0x8A8A);
  ScsControllerDisableMmio (PciBaseAddress);
}

/**
  Enables UFS controller.

  @param[in] ScsUfsHandle  Handle.
**/
STATIC
VOID
ScsUfsEnable (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  DEBUG ((DEBUG_INFO, "Enabling UFS\n"));

  Iosf2OcpDisableBar1 (ScsUfsHandle->Controller.Iosf2OcpPort);
  ScsUfsHandle->Callbacks.Bar1Disable (ScsUfsHandle);
  if (ScsUfsHandle->Controller.Mmp) {
    MmpInit (ScsUfsHandle->Controller.Mmp);
  }
  if (!ScsUfsHandle->SocConfig.IsBootMedium) {
    Iosf2OcpEnableUfs (ScsUfsHandle->Controller.Iosf2OcpPort, ScsUfsHandle->SocConfig.NumOfLanes);
    ScsUfsInitMmioRegisters (ScsUfsHandle->Controller.PciCfgBase, (UINTN) ScsUfsHandle->Controller.MmioBase);
  }
  if (ScsUfsHandle->Config->InlineEncryption) {
    Iosf2OcpEnableUfsInlineEncryption (ScsUfsHandle->Controller.Iosf2OcpPort);
  }
  ScsUfsSetMmioAutoLtrRegister (ScsUfsHandle->Controller.PciCfgBase, (UINTN) ScsUfsHandle->Controller.MmioBase);
  Iosf2OcpConfigureInterrupts (
    ScsUfsHandle->Controller.Iosf2OcpPort,
    ScsUfsHandle->SocConfig.IntPin,
    ScsUfsHandle->SocConfig.Irq
    );
}

/**
  Disable UFS controller.

  @param[in] ScsUfsHandle  Handle.
**/
STATIC
VOID
ScsUfsDisable (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  DEBUG ((DEBUG_INFO, "Disabling UFS\n"));

  Iosf2OcpDisableBar1 (ScsUfsHandle->Controller.Iosf2OcpPort);
  ScsUfsHandle->Callbacks.Bar1Disable (ScsUfsHandle);
  PciSegmentOr16 (
    ScsUfsHandle->Controller.PciCfgBase + R_SCS_CFG_PG_CONFIG,
    (B_SCS_CFG_PG_CONFIG_SE| B_SCS_CFG_PG_CONFIG_PGE | B_SCS_CFG_PG_CONFIG_I3E)
    );
  ScsControllerPutToD3 (ScsUfsHandle->Controller.PciCfgBase);
  if (ScsUfsHandle->Controller.Mmp) {
    MmpDisable (ScsUfsHandle->Controller.Mmp);
  }
  Iosf2OcpDisableUfs (ScsUfsHandle->Controller.Iosf2OcpPort);
  ScsUfsHandle->Callbacks.Disable (ScsUfsHandle);
}

/**
  Prints UFS SoC config into debug port.

  @param[in] ScsUfsHandle  UFS handle
**/
VOID
ScsUfsPrintSocConfig (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  DEBUG ((DEBUG_INFO, "Printing UFS SoC config\n"));
  DEBUG ((DEBUG_INFO, "IsBootMedium: %d\n", ScsUfsHandle->SocConfig.IsBootMedium));
  DEBUG ((DEBUG_INFO, "NumOfLanes: %d\n", ScsUfsHandle->SocConfig.NumOfLanes));
  DEBUG ((DEBUG_INFO, "IntPin: %d\n", ScsUfsHandle->SocConfig.IntPin));
  DEBUG ((DEBUG_INFO, "Irq: %d\n", ScsUfsHandle->SocConfig.Irq));
}

/**
  Configure UFS controller in SCS.

  @param[in] ScsUfsHandle  Handle.
**/
VOID
ScsUfsInit (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  DEBUG ((DEBUG_INFO, "UFS init start\n"));

  ScsUfsPrintSocConfig (ScsUfsHandle);

  if (ScsUfsHandle->SocConfig.NumOfLanes == 0) {
    ScsUfsHandle->UfsInfo->Supported = FALSE;
  } else {
    ScsUfsHandle->UfsInfo->Supported = TRUE;
  }

  if ((ScsUfsHandle->Config->Enable || ScsUfsHandle->SocConfig.IsBootMedium) &&
      (ScsUfsHandle->SocConfig.NumOfLanes != 0) && ScsUfsHandle->Config->UfsDeviceConnected) {
    ScsUfsEnable (ScsUfsHandle);
  } else {
    ScsUfsDisable (ScsUfsHandle);
  }
  DEBUG ((DEBUG_INFO, "UFS init finished\n"));
}

