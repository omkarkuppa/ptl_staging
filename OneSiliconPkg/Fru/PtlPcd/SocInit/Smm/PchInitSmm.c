/** @file
  PCH Init Smm module for PCH specific SMI handlers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include "PchInitSmm.h"
#include <Register/PchRegs.h>
#include <Register/UsbHostRegs.h>
#include <Register/SmbusRegs.h>
#include <Register/PmcRegsFivr.h>
#include <Library/PchPciBdfLib.h>
#include <Library/DciPrivateLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL        *mPchIoTrap;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SMM_SX_DISPATCH2_PROTOCOL             *mSxDispatch;

GLOBAL_REMOVE_IF_UNREFERENCED PCH_NVS_AREA                              *mPchNvsArea;
GLOBAL_REMOVE_IF_UNREFERENCED UINT16                                    mAcpiBaseAddr;

//
// NOTE: The module variables of policy here are only valid in post time, but not runtime time.
//

// The reserved MMIO range to be used in Sx handler
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_PHYSICAL_ADDRESS                      mResvMmioBaseAddr;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                                     mResvMmioSize;

/**
  SMBUS Sx entry SMI handler.
**/
VOID
SmbusSxCallback (
  VOID
  )
{
  UINT64                      SmbusRegBase;
  UINT16                      SmbusIoBase;

  SmbusRegBase = SmbusPciCfgBase ();

  if (PciSegmentRead32 (SmbusRegBase) == 0xFFFFFFFF) {
    return;
  }

  SmbusIoBase = PciSegmentRead16 (SmbusRegBase + R_SMBUS_CFG_SBA) & B_SMBUS_CFG_SBA_BA;
  if (SmbusIoBase == 0) {
    return;
  }

  PciSegmentOr8 (SmbusRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_IO_SPACE);
  //
  // Clear SMBUS status and SMB_WAK_STS of GPE0
  //
  IoWrite8 (SmbusIoBase + R_SMBUS_IO_HSTS, B_SMBUS_IO_HSTS_SMSTS);
  IoWrite32 (mAcpiBaseAddr + R_ACPI_IO_GPE0_STS_127_96, B_ACPI_IO_GPE0_STS_127_96_SMB_WAK_STS);
}

/*
  PCH xHCI Sx entry SMI handler
*/
VOID
PchXhciSxCallback (
  VOID
  )
{
  UINT64                          XhciPciBase;
  UINT64                          XhciMmioBase;
  UINT32                          XhciBar0Low;
  UINT32                          XhciBar0High;
  UINT32                          Pm1Cnt;
  UINT16                          XhciCmd;
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   DciPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_EXI;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &DciPcrAccess);

  XhciPciBase = PchXhciPciCfgBase ();

  //
  // Safety check for xHCI existense
  //
  if (PciSegmentRead32 (XhciPciBase) == 0xFFFFFFFF) {
    return;
  }

  XhciMmioBase = (UINT64) mResvMmioBaseAddr;
  if (XhciMmioBase == 0) {
    ASSERT (FALSE);
    return;
  }

  ///
  /// Get Power Management 1 Control Register Value
  ///
  Pm1Cnt = IoRead32 ((UINTN)mAcpiBaseAddr + R_ACPI_IO_PM1_CNT);

  // if SLP TYP is S4 or S5
  // if ECTRL.USB2DBCEN =1 & ECTRL.DBGENABLE = 1
  if (((Pm1Cnt & B_ACPI_IO_PM1_CNT_SLP_TYP) == V_ACPI_IO_PM1_CNT_S5 ||
       (Pm1Cnt & B_ACPI_IO_PM1_CNT_SLP_TYP) == V_ACPI_IO_PM1_CNT_S4) &&
       (IsUsb2DbcDebugEnabled ((REGISTER_ACCESS *) &DciPcrAccess))) {
    // MMIO BAR and MSE values
    XhciBar0Low = PciSegmentRead32 (XhciPciBase + R_XHCI_CFG_MBAR_N0);
    if (PciSegmentRead32 (XhciPciBase + R_XHCI_CFG_MBAR_N0) & BIT2) {
      XhciBar0High = PciSegmentRead32 (XhciPciBase + R_XHCI_CFG_MBAR_N0 + 4);
    }
    XhciCmd = PciSegmentRead16 (XhciPciBase + PCI_COMMAND_OFFSET);

    // MMIO decode using reserved range.
    PciSegmentAnd16 (XhciPciBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
    PciSegmentWrite32 (XhciPciBase + R_XHCI_CFG_MBAR_N0, (UINT32) XhciMmioBase);
    if (PciSegmentRead32 (XhciPciBase + R_XHCI_CFG_MBAR_N0) & BIT2) {
      PciSegmentWrite32 (XhciPciBase + R_XHCI_CFG_MBAR_N0 + 4, (UINT32) RShiftU64 (XhciMmioBase, 32));
    }

    // Take XHCI to D0
    PciSegmentAnd8 (XhciPciBase + R_XHCI_CFG_PM_CS, (UINT8)~B_XHCI_CFG_PM_CS_POWERSTATE);
    // Enable MSE
    PciSegmentOr16 (XhciPciBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

    // Set chicken bit cmd_ssv
    MmioOr32 (XhciMmioBase + R_XHCI_MEM_SOCHWSTSAVE_REG1, B_XHCI_MEM_SOCHWSTSAVE_REG1_CMD_SSV);

    // Restore BAR and MSE
    PciSegmentAnd16 (XhciPciBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
    PciSegmentWrite32 (XhciPciBase + R_XHCI_CFG_MBAR_N0, XhciBar0Low);
    if (PciSegmentRead32 (XhciPciBase + R_XHCI_CFG_MBAR_N0) & BIT2) {
      PciSegmentWrite32 (XhciPciBase + R_XHCI_CFG_MBAR_N0 + 4, XhciBar0High);
    }
    PciSegmentWrite32 (XhciPciBase + PCI_COMMAND_OFFSET, XhciCmd);

    // Put XHCI back in D3
    PciSegmentOr8 (XhciPciBase + R_XHCI_CFG_PM_CS, (UINT8)B_XHCI_CFG_PM_CS_POWERSTATE);
  }

  //
  // If xHCI is in D0 that means OS didn't put it to D3 during Sx entry i.e. USB kernel debug is enabled.
  // Unless it is put manually to D3, USB wake functionality will not work.
  //
  if ((PciSegmentRead8 (XhciPciBase + R_XHCI_CFG_PM_CS) & V_XHCI_CFG_PM_CS_POWERSTATE_D3) == 0) {
    //
    // Put device to D3 to enable wake functionality for USB devices
    //
    PciSegmentOr8 (XhciPciBase + R_XHCI_CFG_PM_CS, (UINT8)(B_XHCI_CFG_PM_CS_POWERSTATE));
  }
}

/**
  PCH Sx entry SMI handler.

  @param[in]     Handle          Handle of the callback
  @param[in]     Context         The dispatch context
  @param[in,out] CommBuffer      A pointer to a collection of data in memory that will
                                 be conveyed from a non-SMM environment into an SMM environment.
  @param[in,out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
PchSxHandler (
  IN  EFI_HANDLE                   Handle,
  IN CONST VOID                    *Context OPTIONAL,
  IN OUT VOID                      *CommBuffer OPTIONAL,
  IN OUT UINTN                     *CommBufferSize OPTIONAL
  )
{
  PchLanSxCallback ();
  PchHdaSxCallback ();
  PchXhciSxCallback ();

  SmbusSxCallback ();

  return EFI_SUCCESS;
}

/**
  Initialize PCH Sx entry SMI handler.

  @param[in] ImageHandle - Handle for the image of this driver
**/
VOID
InitializeSxHandler (
  IN EFI_HANDLE        ImageHandle
  )
{
  EFI_SMM_SX_REGISTER_CONTEXT               SxDispatchContext;
  EFI_HANDLE                                SxDispatchHandle;
  EFI_SLEEP_TYPE                            SxType;
  EFI_STATUS                                Status;

  DEBUG ((DEBUG_INFO, "InitializeSxHandler() Start\n"));

  //
  // Register the callback for S3/S4/S5 entry
  //
  SxDispatchContext.Phase = SxEntry;
  for (SxType = SxS3; SxType <= SxS5; SxType++) {
    SxDispatchContext.Type  = SxType;
    Status = mSxDispatch->Register (
                            mSxDispatch,
                            PchSxHandler,
                            &SxDispatchContext,
                            &SxDispatchHandle
                            );
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "InitializeSxHandler() End\n"));
}

/**
  Allocates reserved MMIO for Sx SMI handler use.
**/
VOID
AllocateReservedMmio (
  VOID
  )
{
  mResvMmioBaseAddr = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
  mResvMmioSize     = PcdGet32 (PcdSiliconInitTempMemSize);
  DEBUG ((DEBUG_INFO, "mResvMmioBaseAddr %x, mResvMmioSize %x\n", mResvMmioBaseAddr, mResvMmioSize));
}

/**
  Initializes the PCH SMM handler for for PCIE hot plug support
  <b>PchInit SMM Module Entry Point</b>\n
  - <b>Introduction</b>\n
      The PchInitSmm module is a SMM driver that initializes the Intel Platform Controller Hub
      SMM requirements and services. It consumes the PCH_POLICY_HOB and SI_POLICY_HOB for expected
      configurations per policy.

  - <b>Details</b>\n
    This module provides SMI handlers to services PCIE HotPlug SMI, LinkActive SMI, and LinkEq SMI.
    And also provides port 0x61 emulation support, registers BIOS WP handler to process BIOSWP status,
    and registers SPI Async SMI handler to handler SPI Async SMI.
    This module also registers Sx SMI callback function to detail with GPIO Sx Isolation and LAN requirement.

  - @pre
    - PCH PCR base address configured
    - EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
      - This is to ensure that PCI MMIO and IO resource has been prepared and available for this driver to allocate.
    - EFI_SMM_BASE2_PROTOCOL
    - EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL
    - EFI_SMM_SX_DISPATCH2_PROTOCOL
    - EFI_SMM_CPU_PROTOCOL
    - @link _PCH_SMM_IO_TRAP_CONTROL_PROTOCOL PCH_SMM_IO_TRAP_CONTROL_PROTOCOL @endlink
    - @link _PCH_SMI_DISPATCH_PROTOCOL PCH_SMI_DISPATCH_PROTOCOL @endlink
    - @link _PCH_PCIE_SMI_DISPATCH_PROTOCOL PCH_PCIE_SMI_DISPATCH_PROTOCOL @endlink
    - @link _PCH_TCO_SMI_DISPATCH_PROTOCOL PCH_TCO_SMI_DISPATCH_PROTOCOL @endlink
    - @link _PCH_ESPI_SMI_DISPATCH_PROTOCOL PCH_ESPI_SMI_DISPATCH_PROTOCOL @endlink

  - <b>References</b>\n
    - @link _PCH_POLICY PCH_POLICY_HOB @endlink.
    - @link _SI_POLICY_STRUCT SI_POLICY_HOB @endlink.

  - <b>Integration Checklists</b>\n
    - Verify prerequisites are met. Porting Recommendations.
    - No modification of this module should be necessary
    - Any modification of this module should follow the PCH BIOS Specification and EDS

  @param[in] ImageHandle - Handle for the image of this driver
  @param[in] SystemTable - Pointer to the EFI System Table

  @retval EFI_SUCCESS    - PCH SMM handler was installed
**/
EFI_STATUS
EFIAPI
PchInitSmmEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                Status;
  PCH_NVS_AREA_PROTOCOL                     *PchNvsAreaProtocol;

  DEBUG ((DEBUG_INFO, "PchInitSmmEntryPoint()\n"));

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmIoTrapDispatch2ProtocolGuid,
                    NULL,
                    (VOID **) &mPchIoTrap
                    );
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSxDispatch2ProtocolGuid,
                    NULL,
                    (VOID**) &mSxDispatch
                    );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gPchNvsAreaProtocolGuid, NULL, (VOID **) &PchNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);
  mPchNvsArea = PchNvsAreaProtocol->Area;

  mAcpiBaseAddr = PmcGetAcpiBase ();

  AllocateReservedMmio ();

  InitializeSxHandler (ImageHandle);

  Status = InitializePchPcieSmm (ImageHandle, SystemTable);
  ASSERT_EFI_ERROR (Status);

  Status = InstallPchBiosWriteProtect (ImageHandle, SystemTable);
  ASSERT_EFI_ERROR (Status);

  Status = InstallPchSpiAsyncSmiHandler ();
  ASSERT_EFI_ERROR (Status);

  UpdateHdaCodecSxWakeCapabilitySetting ();


  Status = UpdateWOLFastStartupSupportSetting();
  ASSERT_EFI_ERROR(Status);

  return EFI_SUCCESS;
}
