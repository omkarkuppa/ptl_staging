/** @file
  Initializes TSN mGBE Controller (TSN).

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <PiPei.h>
#include <Ppi/SiPolicy.h>
#include <Register/PchRegs.h>
#include <Register/TsnRegs.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiItssLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci30.h>
#include <TsnHandle.h>


/**
  The function programs the Tc to DMA channel mapping
**/
STATIC
VOID
TsnTcDmaMapping (
  IN  REGISTER_ACCESS   *TsnPcrAccess
  )
{
  // Tc to DMA channel mapping
  TsnPcrAccess->AndThenOr32 (
    TsnPcrAccess,
    R_TSN_PCR_AXIID_TO_TC0_MAP_0,
    (UINT32) ~B_TSN_PCR_AXIID_TO_TC0_MAP_0,
    V_TSN_PCR_AXIID_TO_TC0_MAP_0
    );
  TsnPcrAccess->AndThenOr32 (
    TsnPcrAccess,
    R_TSN_PCR_AXIID_TO_TC1_MAP_0,
    (UINT32) ~B_TSN_PCR_AXIID_TO_TC1_MAP_0,
    V_TSN_PCR_AXIID_TO_TC1_MAP_0
    );

  //MSI Tc to DMA channel mapping
  TsnPcrAccess->AndThenOr32 (
    TsnPcrAccess,
    R_TSN_PCR_MSI_TO_TC0_MAP,
    (UINT32) ~B_TSN_PCR_MSI_TO_TC0_MAP,
    V_TSN_PCR_MSI_TO_TC0_MAP
    );
  TsnPcrAccess->AndThenOr32 (
    TsnPcrAccess,
    R_TSN_PCR_MSI_TO_TC1_MAP,
    (UINT32) ~B_TSN_PCR_MSI_TO_TC1_MAP,
    V_TSN_PCR_MSI_TO_TC1_MAP
    );
}

/**
  Configures Timed Sensitive Network (TSN) Controller Registers and MAC Programming

  @param[in] TsnHandle   Pointer to TSN Handle
**/
STATIC
VOID
TsnConfigureRegisters (
  IN  TSN_HANDLE   *TsnHandle
  )
{
  UINT64               PchTsnBase;
  UINTN                TsnMemBar;
  REGISTER_ACCESS      *TsnPcrAccess;
  TSN_CONFIG           *TsnConfig;
  TSN_PRIVATE_CONFIG   *TsnPrivate;
  UINT32               Data32;
  UINT32               MacLow;
  UINT32               MacHigh;

  DEBUG ((DEBUG_VERBOSE, "%a () - Start.\n", __FUNCTION__));

  PchTsnBase   = TsnHandle->Controller->PciCfgBaseAddr;
  TsnPcrAccess = TsnHandle->Controller->TsnPcrAccess;
  TsnMemBar    = TsnHandle->Controller->Mmio;
  TsnConfig    = TsnHandle->Config;
  TsnPrivate   = TsnHandle->PrivateConfig;

  // Configure Multi-Vc via Tc to DMA channel mapping
  if (TsnConfig->MultiVcEnable == 1) {
    TsnTcDmaMapping (TsnPcrAccess);
  }

  // Program Temp BARs
  PciSegmentAnd8 (PchTsnBase + PCI_COMMAND_OFFSET, (UINT8) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentWrite32 (PchTsnBase + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) TsnMemBar);
  if (PciSegmentRead32 (PchTsnBase + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (PchTsnBase + PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (TsnMemBar, 32));
  }
  PciSegmentWrite32 (PchTsnBase + PCI_BASE_ADDRESSREG_OFFSET + 8,  0x0);
  if (PciSegmentRead32 (PchTsnBase + PCI_BASE_ADDRESSREG_OFFSET + 8) & BIT2) {
    PciSegmentWrite32 (PchTsnBase + PCI_BASE_ADDRESSREG_OFFSET + 12, 0x0);
  }
  DEBUG ((DEBUG_INFO, "TsnMemBar: 0x%xll\n", TsnMemBar));

  // Enable Memory Space encoding
  PciSegmentOr32 (PchTsnBase + PCI_COMMAND_OFFSET, (EFI_PCI_COMMAND_MEMORY_SPACE));

  // Program MAC address for TSN on PCH
  DEBUG ((DEBUG_INFO, "Start MAC address programming\n"));

  // Program MA0IE bit at MAC extended configuration register
  Data32 = MmioRead32 (TsnMemBar + R_TSN_MEM_MAC_EXTENDED_CONFIGURATION);
  Data32 &= ~(B_TSN_MEM_MAC_EXTENDED_CONFIGURATION_MA0IE);
  MmioWrite32 (TsnMemBar + R_TSN_MEM_MAC_EXTENDED_CONFIGURATION, Data32);

  // Program MacAddressLow[31:0]
  MmioWrite32 (TsnMemBar + R_TSN_MEM_MAC_ADDRESS0_LOW, TsnPrivate->Port->MacAddr[0]);

  // Program MacAddressHigh[15:0]
  // MacAddressHigh[31] Address Enable Bit
  MmioWrite32 (TsnMemBar + R_TSN_MEM_MAC_ADDRESS0_HIGH, ((TsnPrivate->Port->MacAddr[1] & B_TSN_MEM_MAC_ADDRESS_HIGH_MASK) | B_TSN_MEM_ADDRESS_ENABLE));

  MacLow = MmioRead32 (TsnMemBar + R_TSN_MEM_MAC_ADDRESS0_LOW);
  MacHigh = MmioRead32 (TsnMemBar + R_TSN_MEM_MAC_ADDRESS0_HIGH);
  DEBUG ((DEBUG_INFO, "Mac Address High: 0x%x Mac Address Low: 0x%x \n", MacHigh, MacLow));

  // Disable Memory Space encoding
  PciSegmentAnd16 (PchTsnBase + PCI_COMMAND_OFFSET, (UINT16) (~EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER));
  PciSegmentWrite32 (PchTsnBase + PCI_BASE_ADDRESSREG_OFFSET, 0);
  if (PciSegmentRead32 (PchTsnBase + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (PchTsnBase + PCI_BASE_ADDRESSREG_OFFSET + 4, 0);
  }

  DEBUG ((DEBUG_VERBOSE, "%a () - End.\n", __FUNCTION__));
}

/**
  Configures Timed Sensitive Network (TSN) Controller Interrupt

  @param[in] TsnHandle   Pointer to TSN Handle
**/
STATIC
VOID
TsnConfigureInterrupt (
  IN  TSN_HANDLE    *TsnHandle
  )
{
  TSN_PRIVATE_CONFIG   *TsnPrivate;
  REGISTER_ACCESS      *TsnPcrAccess;
  UINT32               Data32Or;
  UINT32               Data32And;

  TsnPrivate = TsnHandle->PrivateConfig;
  TsnPcrAccess = TsnHandle->Controller->TsnPcrAccess;

  DEBUG ((DEBUG_VERBOSE, "%a () - Start.\n", __FUNCTION__));

  Data32Or =  (UINT32) ((TsnPrivate->InterruptPin << N_TSN_PCR_PCICFGCTR_IPIN1) |
                        (TsnPrivate->Irq << N_TSN_PCR_PCICFGCTR_PCI_IRQ));
  Data32And =~(UINT32) (B_TSN_PCR_PCICFGCTR_PCI_IRQ | B_TSN_PCR_PCICFGCTR_ACPI_IRQ | B_TSN_PCR_PCICFGCTR_IPIN1);

  TsnPcrAccess->AndThenOr32 (TsnPcrAccess, R_TSN_PCR_PCICFGCTRL, Data32And, Data32Or);

  DEBUG ((DEBUG_VERBOSE, "%a () - End.\n", __FUNCTION__));
}

/**
  Initialize the Intel TSN Controller

  @param[in] TsnHandle   Pointer to TSN Handle
**/
VOID
TsnInit (
  IN  TSN_HANDLE   *TsnHandle
  )
{
  UINT64   PchTsnBase;

  DEBUG ((DEBUG_VERBOSE, "%a () - Start.\n", __FUNCTION__));

  PchTsnBase = TsnHandle->Controller->PciCfgBaseAddr;

  // Check if TSN is available
  if ((PciSegmentRead16 (PchTsnBase + PCI_VENDOR_ID_OFFSET)) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "TSN config space not accessible! TSN not available\n"));
    return;
  }

  TsnConfigureRegisters (TsnHandle);

  TsnConfigureInterrupt (TsnHandle);

  DEBUG ((DEBUG_VERBOSE, "%a () - End.\n", __FUNCTION__));
}