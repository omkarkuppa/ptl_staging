/** @file
  Initializes PCH Primary To Sideband Bridge (P2SB) Device

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

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/P2sbRegs.h>
#include <Register/Sai.h>
#include <Library/P2sbPrivateLib.h>

#define R_HPET_MEM_GEN_CFG                    0x10
#define B_HPET_MEM_GEN_CFG_ENABLE_CNF         BIT0

#define N_HPET_ADDR_ASEL                      12
#define N_IO_APIC_ASEL                        12

#define R_IO_APIC_MEM_INDEX_OFFSET            0x00000000U
#define R_IO_APIC_MEM_DATA_OFFSET             0x00000010U

/**
  The function check the P2SB hidden or not.

  @param[in] PciAccess             P2SB PCIe register access.

  @return TRUE                     P2SB device not hidden.
  @return FALSE                    P2SB device hidden.
**/
BOOLEAN
P2sbControllerNotExist (
  IN REGISTER_ACCESS *PciAccess
  )
{
  if (PciAccess->Read16 (PciAccess, 0) == V_PCH_INTEL_VENDOR_ID) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Hide P2SB device.

  @param[in] PciAccess                 P2SB PCIe Register Acccess
**/
VOID
P2sbHideDevice (
  IN REGISTER_ACCESS  *PciAccess
  )
{
  PciAccess->Or32 (PciAccess, R_P2SB_CFG_P2SBC, B_P2SB_CFG_P2SBC_HIDE);
}

/**
  Unhide P2SB device.

  @param[in] PciAccess                 P2SB PCIe Register Acccess
**/
VOID
P2sbUnhideDevice (
  IN REGISTER_ACCESS  *PciAccess
  )
{
  // B_P2SB_CFG_P2SBC_HIDE is on second 8 bit part of 32 bits R_P2SB_CFG_P2SBC register
  // With AndOr32 is not possible to set due PCI config space is hidden
  PciAccess->Write8 (PciAccess, R_P2SB_CFG_P2SBC + 1, 0);
}

/**
  The function configures P2SB power management

  @param[in] P2SbHandle            P2SB controller handle
**/
VOID
P2sbPowerGatingConfigure (
  IN  P2SB_HANDLE                       *P2SbHandle
  )
{
  UINT16                      Data16;

  // Set Hardware Autonomous Enable (HAE) and PMC Power Gating Enable (PMCPG_EN)
  if (P2SbHandle->PrivateConfig->LegacyIoLowLatency || !P2SbHandle->PrivateConfig->HaPowerGatingSupported) {
    Data16 = 0;
  } else {
    Data16 = B_P2SB_CFG_PCE_HAE;
  }
  P2SbHandle->PcieConfigAccess->AndThenOr16 (
        P2SbHandle->PcieConfigAccess,
        R_P2SB_CFG_PCE,
        (UINT16) ~(B_P2SB_CFG_PCE_HAE |
                   B_P2SB_CFG_PCE_D3HE |
                   B_P2SB_CFG_PCE_I3E |
                   B_P2SB_CFG_PCE_PMCPG_EN),
        Data16
     );
}

/**
  Check SBREG readiness.

  @param[in] PciAccess        P2SB PCIe Register Acccess

  @retval TRUE                SBREG is ready
  @retval FALSE               SBREG is not ready
**/
BOOLEAN
P2sbIsSbregReady (
  IN REGISTER_ACCESS  *PciAccess
  )
{
  if (PciAccess->Read32 (PciAccess, R_P2SB_CFG_SBREG_BAR) & BIT2) {
    if (((PciAccess->Read32 (PciAccess, R_P2SB_CFG_SBREG_BAR) & B_P2SB_CFG_SBREG_BAR_RBA) == 0) &&
         (PciAccess->Read32 (PciAccess, R_P2SB_CFG_SBREG_BAR + 4) == 0)) {
      return FALSE;
    }
  } else if ((PciAccess->Read32 (PciAccess, R_P2SB_CFG_SBREG_BAR) & B_P2SB_CFG_SBREG_BAR_RBA) == 0) {
    return FALSE;
  }
  return TRUE;
}

/**
  Internal function performing HPET init in early PEI phase

  @param[in] P2SbHandle            P2SB controller handle
  @param[in] HpetMmio              HPET MMIO
**/
VOID
P2sbHpetInit (
  IN  P2SB_HANDLE                       *P2SbHandle,
  IN  UINTN                             HpetMmio
  )
{
  if (P2sbControllerNotExist (P2SbHandle->PcieConfigAccess)) {
    return;
  }

  //
  // Initial and enable HPET High Precision Timer memory address for basic usage
  //
  P2SbHandle->PcieConfigAccess->AndThenOr8 (
    P2SbHandle->PcieConfigAccess,
    R_P2SB_CFG_HPTC,
    (UINT8) ~B_P2SB_CFG_HPTC_AS,
    (UINT8) ((((UINT32) HpetMmio >> N_HPET_ADDR_ASEL) & B_P2SB_CFG_HPTC_AS) | B_P2SB_CFG_HPTC_AE)
    );
  //
  // Read back for posted write to take effect
  //
  P2SbHandle->PcieConfigAccess->Read8 (
    P2SbHandle->PcieConfigAccess,
    R_P2SB_CFG_HPTC
  );
  //
  // Set HPET Timer enable to start counter spinning
  //
  MmioOr32 ((UINT32) HpetMmio + R_HPET_MEM_GEN_CFG, B_HPET_MEM_GEN_CFG_ENABLE_CNF);
}

/**
  Program to which endpoints P2SB can send posted writes when performing a sideband
  access via a P2SB BAR.

  @param[in] PciAccess                      P2SB PCIe Register Acccess
  @param[in] P2SbPostedWriteEndpointsTable  Table of port ids for endpoints which can handle posted writes
  @param[in] TableLength                    Length of the table
**/
VOID
P2SbProgramPostedWriteMask (
  IN REGISTER_ACCESS  *PciAccess,
  IN P2SB_PID         *P2SbPostedWriteEndpointsTable,
  IN UINT32           TableLength
  )
{
  UINT32  Index;
  UINT32  RegisterOff;
  UINT32  BitNo;

  if (P2sbControllerNotExist (PciAccess)) {
    return;
  }

  for (Index = 0; Index < TableLength; ++Index) {
    if (P2SbPostedWriteEndpointsTable[Index] <= 0xFF) {
      RegisterOff = (P2SbPostedWriteEndpointsTable[Index] / 32) * sizeof (UINT32); // Calculate which 32 bit mask register
      BitNo = P2SbPostedWriteEndpointsTable[Index] % 32;                           // Calculate the bit number to be set in the mask register
      PciAccess->Or32 (PciAccess, R_P2SB_CFG_SBREGPOSTED0 + RegisterOff, (UINT32) 1 << BitNo);
    } else {
      DEBUG ((DEBUG_WARN, "%a: Incorrect PID\n", __FUNCTION__));
      continue;
    }
  }
}

/**
  Early init P2SB configuration

  @param[in] PciAccess                 P2SB PCIe Register Acccess
**/
VOID
P2sbEarlyConfig (
  IN REGISTER_ACCESS  *PciAccess
  )
{
  if (P2sbControllerNotExist (PciAccess)) {
    return;
  }

  //
  // BIOS shall program the PCI Capability List to 0 for P2SB controller.
  //
  PciAccess->Write8 (PciAccess, + PCI_CAPBILITY_POINTER_OFFSET, 0);

  //
  // Set Error Reporting Enable to 1
  //
  PciAccess->Or16 (PciAccess, R_P2SB_CFG_UREC, B_P2SB_CFG_UREC_ERE);
}

/**
  The function performs P2SB initialization.

  @param[in] P2SbHandle            P2SB controller handle
**/
VOID
P2sbConfigure (
  IN  P2SB_HANDLE             *P2SbHandle
  )
{
  if (P2sbControllerNotExist (P2SbHandle->PcieConfigAccess)) {
    return;
  }

  if (!P2SbHandle->PrivateConfig->DisableHpetAndApicBdfProgramming) {
    //
    // HPET and APIC BDF programming
    // Assign 0:30:6 for HPET and 0:30:7 for APIC statically.
    //
    P2SbHandle->PcieConfigAccess->Write16 (
      P2SbHandle->PcieConfigAccess,
      R_P2SB_CFG_HBDF,
      (V_P2SB_CFG_HBDF_BUS << 8) | (V_P2SB_CFG_HBDF_DEV << 3) | V_P2SB_CFG_HBDF_FUNC
    );
    P2SbHandle->PcieConfigAccess->Write16 (
      P2SbHandle->PcieConfigAccess,
      R_P2SB_CFG_IBDF,
      (V_P2SB_CFG_IBDF_BUS << 8) | (V_P2SB_CFG_IBDF_DEV << 3) | V_P2SB_CFG_IBDF_FUNC
    );
  }

  P2SbHandle->PcieConfigAccess->Or32 (
    P2SbHandle->PcieConfigAccess,
    R_P2SB_CFG_P2SBC,
    B_P2SB_CFG_P2SBC_RCSPE
  );

  if (P2SbHandle->PrivateConfig->EnableParityCheck) {
    P2SbHandle->PcieConfigAccess->Or32 (
      P2SbHandle->PcieConfigAccess,
      R_P2SB_CFG_P2SBC,
      B_P2SB_CFG_P2SBC_DPEE | B_P2SB_CFG_P2SBC_CPEE | B_P2SB_CFG_P2SBC_DPPEE
    );
    P2SbHandle->PcieConfigAccess->Or32 (
      P2SbHandle->PcieConfigAccess,
      PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_PARITY_ERROR_RESPOND | EFI_PCI_COMMAND_SERR
    );
  }

  //
  // P2SB power management settings.
  //
  if (P2SbHandle->PrivateConfig->HsleWorkaround) {
    DEBUG ((DEBUG_INFO, "\nHSLE Workaround to avoid p2sb clock gating\n"));
  } else {
    P2sbPowerGatingConfigure (P2SbHandle);
  }

  //
  // If Legacy IO Low Latency is enabled
  //
  if (P2SbHandle->PrivateConfig->LegacyIoLowLatency) {
    P2SbHandle->PcieConfigAccess->Or16 (
      P2SbHandle->PcieConfigAccess,
      R_P2SB_CFG_PDOMAIN, B_P2SB_CFG_PDOMAIN_CGD | B_P2SB_CFG_PDOMAIN_CKREQD
    );
    P2SbHandle->PcieConfigAccess->Or16 (
      P2SbHandle->PcieConfigAccess,
      R_P2SB_CFG_SDOMAIN, B_P2SB_CFG_SDOMAIN_CGD | B_P2SB_CFG_SDOMAIN_CKREQD
    );
  }

  ///
  /// Set LFIORIEC to 0 for Integrated Eembedded Controller (IEC) not supported.
  ///
  if (P2SbHandle->PrivateConfig->IecSupportDisable) {
    P2SbHandle->PcieConfigAccess->Write8 (
      P2SbHandle->PcieConfigAccess,
      R_P2SB_CFG_LFIORIEC,
      0
    );
  }
}

/**
  The function performs P2SB lock programming.

  @param[in] P2SbHandle            P2SB controller handle
**/
VOID
P2sbLock (
  IN  P2SB_HANDLE             *P2SbHandle
  )
{
  if (P2sbControllerNotExist (P2SbHandle->PcieConfigAccess)) {
    return;
  }

  if (P2SbHandle->P2SbConfig->SbAccessUnlock == 0) {
    //
    // Set PGCB Clock Gating Enable
    //
    P2SbHandle->PcieConfigAccess->Or32 (
      P2SbHandle->PcieConfigAccess,
      R_P2SB_CFG_P2SBC,
      B_P2SB_CFG_P2SBC_PGCBCGE
    );
  }

  //
  // Hide P2SB controller in the end of PEI.
  //
  P2sbHideDevice (P2SbHandle->PcieConfigAccess);
}

/**
  The function configures P2SB Regbar.

  @param[in] PciAccess                 P2SB PCIe Register Acccess
  @param[in] P2sbMmio                  P2SB MMIO
**/
VOID
P2sbRegbarConfigure (
  IN REGISTER_ACCESS  *PciAccess,
  IN UINT64           P2sbMmio
  )
{
  if (P2sbControllerNotExist (PciAccess)) {
    return;
  }

  PciAccess->And16 (PciAccess, PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);

  PciAccess->Write32 (PciAccess, R_P2SB_CFG_SBREG_BAR, (UINT32) P2sbMmio);
  if (PciAccess->Read32 (PciAccess, R_P2SB_CFG_SBREG_BAR) & BIT2) {
    PciAccess->Write32 (PciAccess, R_P2SB_CFG_SBREG_BARH, (UINT32) RShiftU64 (P2sbMmio, 32));
  }

  PciAccess->Or16 (PciAccess, PCI_COMMAND_OFFSET, (UINT16) EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Configures the P2SB IoAPIC Control Register

  @param[in] PciAccess                 P2SB PCIe Register Acccess
  @param[in] IoApicAddress             IoApic address

  @retval      None
**/
VOID
P2SbConfigureIoApic (
  IN  REGISTER_ACCESS     *PciAccess,
  IN  UINT32              IoApicAddress
  )
{
  UINT8                 IoApicSelect;

  IoApicSelect = (IoApicAddress >> N_IO_APIC_ASEL) & 0xFF;
  if (IoApicSelect != PciAccess->Read8 (PciAccess, R_P2SB_CFG_IOAC)) {
    PciAccess->And16 (PciAccess, R_P2SB_CFG_IOAC, (UINT16) ~(B_P2SB_CFG_IOAC_AE));
    PciAccess->AndThenOr16 (PciAccess,
      R_P2SB_CFG_IOAC,
      (UINT16) ~(B_P2SB_CFG_IOAC_ASEL),
      (UINT16) IoApicSelect
      );
  }
  ///
  /// PCH BIOS Spec Section 6.6.2.1
  /// 1. Enable the IOAPIC by setting the APIC Enable bit, P2SB PCI offset 64h [8], if the
  ///    system needs to use the IOxAPIC. The APIC Enable bits needs read back after the bit is written.
  ///
  PciAccess->Or16 (PciAccess, R_P2SB_CFG_IOAC, B_P2SB_CFG_IOAC_AE);
  ///
  /// Reads back for posted write to take effect
  ///
  PciAccess->Read16 (PciAccess, R_P2SB_CFG_IOAC);
}

/**
  The function returns P2SB MMIO base for particular PID.

  Note: This function is used only for one specific case,
  should not be used for any other cases.

  @param[in]  P2SbController        Pointer to P2SB controller implementing the sideband
  @param[in]  P2SbPid               Port id

  @return                           P2SB MMIO base for particular PID.
**/
UINT64
P2sbGetMmioBase (
  IN P2SB_CONTROLLER                    *P2SbController,
  IN P2SB_PID                           P2SbPid
  )
{
  return P2SbController->Mmio | ((UINT8) (P2SbPid) << 16);
}
