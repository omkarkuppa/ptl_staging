/** @file
  PCH BIOS Write Protect Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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
#include "PchInitSmm.h"
#include <Register/PchRegs.h>
#include <Register/SpiRegs.h>
#include <Register/EspiRegs.h>
#include <Library/SpiInitLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/SpiSocLib.h>
#include <Library/PcdEspiLib.h>
#include <PchConfigHobCommon.h>

//
// Global variables
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_TCO_SMI_DISPATCH_PROTOCOL     *mPchTcoSmiDispatchProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_ESPI_SMI_DISPATCH_PROTOCOL    *mEspiSmmDispatchProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED UINT64                            mLpcRegBase;

/**
  This hardware SMI handler will be run every time the BIOS Write Enable bit is set.

  @param[in] DispatchHandle       Not used
**/
VOID
EFIAPI
PchSpiBiosWpCallback (
  IN  EFI_HANDLE                              DispatchHandle
  )
{
  SocSpiClearBiosWriteProtectDisable ();
}

/**
  This hardware SMI handler will be run every time the BIOS Write Enable bit is set.

  @param[in] DispatchHandle       Not used

**/
VOID
EFIAPI
PchLpcBiosWpCallback (
  IN  EFI_HANDLE                              DispatchHandle
  )
{
  //
  // Disable BIOSWE bit to protect BIOS
  //
  PciSegmentAnd8 ((UINTN) (mLpcRegBase + R_ESPI_CFG_ESPI_BC), (UINT8) ~B_ESPI_CFG_ESPI_BC_WPD);
}

/**
  Entry point for Pch Bios Write Protect driver.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
**/
EFI_STATUS
EFIAPI
InstallPchBiosWriteProtect (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle;
  EFI_PEI_HOB_POINTERS    HobPtr;
  LOCK_DOWN_CONFIG_HOB    *LockDownConfigHob;

  DEBUG ((DEBUG_INFO, "InstallPchBiosWriteProtect()\n"));

  //
  // Get PchLockDownHob
  //
  HobPtr.Guid   = GetFirstGuidHob (&gPchLockDownConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  LockDownConfigHob = (LOCK_DOWN_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  if (LockDownConfigHob->BiosLock != TRUE) {
    return EFI_SUCCESS;
  }

  mLpcRegBase = LpcPciCfgBase ();

  DEBUG ((DEBUG_INFO, "Installing BIOS Write Protect SMI handler\n"));
  //
  // Get the PCH TCO SMM dispatch protocol
  //
  mPchTcoSmiDispatchProtocol = NULL;
  Status = gSmst->SmmLocateProtocol (&gPchTcoSmiDispatchProtocolGuid, NULL, (VOID **) &mPchTcoSmiDispatchProtocol);
  ASSERT_EFI_ERROR (Status);
  //
  // Always register an SPI BiosWp callback function to handle TCO BIOSWR SMI
  // NOTE: No matter the BIOS resides behind SPI or not, it needs to handle the SPI BIOS WP SMI
  //       to avoid SMI deadloop on SPI WPD write.
  //
  Handle = NULL;
  Status = mPchTcoSmiDispatchProtocol->SpiBiosWpRegister (
                                         mPchTcoSmiDispatchProtocol,
                                         PchSpiBiosWpCallback,
                                         &Handle
                                         );
  ASSERT_EFI_ERROR (Status);

  //
  // Always register an LPC/eSPI BiosWp callback function to handle TCO BIOSWR SMI
  // NOTE: No matter the BIOS resides behind LPC/eSPI or not, it needs to handle the BIOS WP SMI
  //       to avoid SMI deadloop on LPC/eSPI WPD write.
  //
  if (PtlPcdIsEspiEnabled ()) {
    //
    // Get the PCH ESPI SMM dispatch protocol
    //
    mEspiSmmDispatchProtocol = NULL;
    Status = gSmst->SmmLocateProtocol (&gPchEspiSmiDispatchProtocolGuid, NULL, (VOID **) &mEspiSmmDispatchProtocol);
    ASSERT_EFI_ERROR (Status);

    //
    // Register an ESpiBiosWp callback function to handle BIOSWR SMI
    //
    Handle = NULL;
    Status = mEspiSmmDispatchProtocol->BiosWrProtectRegister (
                                         mEspiSmmDispatchProtocol,
                                         PchLpcBiosWpCallback,
                                         &Handle
                                         );
    ASSERT_EFI_ERROR (Status);
  } else {
    //
    // Register an LPC BiosWp callback function to handle TCO BIOSWR SMI
    //
    Handle = NULL;
    Status = mPchTcoSmiDispatchProtocol->LpcBiosWpRegister (
                                           mPchTcoSmiDispatchProtocol,
                                           PchLpcBiosWpCallback,
                                           &Handle
                                           );
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

