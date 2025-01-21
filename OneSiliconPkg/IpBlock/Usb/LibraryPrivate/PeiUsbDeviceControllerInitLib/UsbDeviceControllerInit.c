/**@file
  This is the code that initializes xDCI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include <Library/UsbDeviceControllerInitLib.h>

#include <IndustryStandard/Pci30.h>
#include <Register/UsbDeviceRegs.h>
#include <UsbController.h>
#include <Library/DebugLib.h>

#define XDCI_WAIT_FOR_D3_US      10000      ///< Timeout of 10000 * 10us = 100ms

/**
  Common entry point for PCH and CPU xDCI controller

  @param[in]  UsbHandle               USB Handle

  @retval     EFI_SUCCESS             Configuration ended successfully
              EFI_INVALID_PARAMETER   UsbHandle was NULL
              EFI_UNSUPPORTED         USB controller was not found
**/
EFI_STATUS
XdciConfigure (
  IN  USB_HANDLE      *UsbHandle
  )
{
  UINT32                Timeout;
  UINT64                MmioBar;
  UINT16                Data16;
  USB_CONFIG            *UsbConfig;
  USB_CALLBACK          *Callback;
  USB_PRIVATE_CONFIG    *PrivateConfig;
  REGISTER_ACCESS       *PciCfgAccess;
  REGISTER_ACCESS       *XdciMmio;

  DEBUG ((DEBUG_INFO, "XdciConfigure() - entry.\n"));

  if (UsbHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "Invalid parameters provided to %a.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  UsbConfig     = UsbHandle->UsbConfig;
  Callback      = UsbHandle->Callback;
  PrivateConfig = UsbHandle->PrivateConfig;
  PciCfgAccess  = UsbHandle->DeviceController->PcieConfigAccess;
  XdciMmio      = UsbHandle->DeviceController->MmioAccess;
  MmioBar       = UsbHandle->DeviceController->ReservedMmio;

  if (PciCfgAccess->Read16 (PciCfgAccess, PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "xDCI: PCI device NOT found.\n"));
    return EFI_UNSUPPORTED;
  }

  if (PrivateConfig->Location != Tcss) {
    //
    //  Enabling Trunk Clock Gating
    //  Set bit [5:0]=6'b111111
    //
    UsbHandle->DeviceController->SbMmioAccess->AndThenOr32 (
      UsbHandle->DeviceController->SbMmioAccess,
      R_XDCI_PCR_PMCTL,
      (UINT32) ~(0),
      (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
      );
  }

  //
  // Assign memory resources for xDCI
  //
  PciCfgAccess->And16 (PciCfgAccess, PCI_COMMAND_OFFSET, (UINT16) ~(EFI_PCI_COMMAND_MEMORY_SPACE));
  PciCfgAccess->Write32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET, (UINT32) MmioBar);
  if (PciCfgAccess->Read32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciCfgAccess->Write32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (MmioBar, 32));
  }
  PciCfgAccess->Or16 (PciCfgAccess, PCI_COMMAND_OFFSET, (UINT16) EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Set the CPGE D3-Hot to Enable either BIT1 or BIT2 can be set at the same time
  // If BIT2 is set then function will power gate when idle and the PMCSR[1:0] register in the function = 2'b11 (D3)
  // xDCI cfg space 0xA2 bit [2:1] = 2'b10
  //
  PciCfgAccess->AndThenOr32 (
    PciCfgAccess,
    R_XDCI_CFG_D0I3_MAX_POW_LAT_PG_CONFIG,
    (UINT32)~(BIT17),
    (UINT32)(BIT18)
    );

  //
  // Set the bits below to 1 to enable the USB2 PHY suspend if no XDCI driver is loaded
  // Set xDCIBAR + C200h [6] to 1b
  //
  XdciMmio->Or32 (
    XdciMmio,
    R_XDCI_MEM_GUSB2PHYCFG_0,
    B_XDCI_MEM_GUSB2PHYCFG_0_SUSPENDUSB20
    );

  //
  // Enabling this bit will allow USB3.0 PHY to enter Suspend mode under valid conditions
  // Set xDCIBAR + C2C0h [17] to 1b
  //
  XdciMmio->Or32 (
    XdciMmio,
    R_XDCI_MEM_GUSB3PIPECTL_0,
    B_XDCI_MEM_GUSB3PIPECTL_0_SUSPENDENABLE
    );

  if ((PrivateConfig->Location != Tcss) && UsbConfig->XdciConfig.Enable == 0) {
    DEBUG ((DEBUG_INFO, "xDCI: Device disabled\n"));

    //
    // Since xDCI function is disabled, the U2/U3 PME shall NOT be enabled
    //
    XdciMmio->And32 (
      XdciMmio,
      R_XDCI_MEM_APBFC_U3PMU_CFG5,
      (UINT32)~(BIT4 | BIT3)
      );

    //
    // 1. Set xDCIBAR + C110h [1] to 1b
    // It will allow hibernation through xDCI's MMIO register space
    //
    XdciMmio->Or32 (
      XdciMmio,
      R_XDCI_MEM_GCTL,
      B_XDCI_MEM_GCTL_GBLHIBERNATIONEN
      );

    //
    // 2. Put xDCI into D3 using xDCI MMIO space (APBFC_U3PMU_CFG4[1:0] = 2'b11)
    //
    XdciMmio->Or16 (
      XdciMmio,
      R_XDCI_MEM_APBFC_U3PMU_CFG4,
      (UINT16) (BIT1 | BIT0)
      );

    //
    // 3. Wait for D3 using the xDCI MMIO space (APBFC_U3PMU_CFG2[11:8] = 4'b1111)
    //
    Timeout = 0;
    Data16 = XdciMmio->Read16 (XdciMmio, R_XDCI_MEM_APBFC_U3PMU_CFG2);
    while (((Data16 & (BIT11 | BIT10 | BIT9 | BIT8)) != 0xF00) &&
           (Timeout < XDCI_WAIT_FOR_D3_US)) {
      if (Callback->DelayForUs != NULL) {
        Callback->DelayForUs (UsbHandle, 10);
      }
      Timeout++;
      Data16 = XdciMmio->Read16 (XdciMmio, R_XDCI_MEM_APBFC_U3PMU_CFG2);
    }

    //
    // 4. Set the xDCI PCI Config space PMCSR register to indicate D3 (XDCI cfg space 0x84 bit [1:0] = 2'b11)
    //
    PciCfgAccess->Or8 (
      PciCfgAccess,
      R_XDCI_CFG_PMECTRLSTATUS,
      (UINT8) (BIT1 | BIT0)
      );

    //
    // Clear memory resources for xDCI
    //
    PciCfgAccess->And16 (PciCfgAccess, PCI_COMMAND_OFFSET, (UINT16) (~EFI_PCI_COMMAND_MEMORY_SPACE));
    PciCfgAccess->Write32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET, 0);
    if (PciCfgAccess->Read32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
      PciCfgAccess->Write32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET + 4, 0);
    }

    if (Callback->UsbDeviceControllerDisable != NULL) {
      Callback->UsbDeviceControllerDisable (UsbHandle);
    }
  }

  //
  // Clear memory resources for xDCI
  //
  PciCfgAccess->And16 (PciCfgAccess, PCI_COMMAND_OFFSET, (UINT16) (~EFI_PCI_COMMAND_MEMORY_SPACE));
  PciCfgAccess->Write32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET, 0);
  PciCfgAccess->Write32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET + 4, 0);

  return EFI_SUCCESS;
}

/**
  Used for programming interrupt pin and IRQ number in xDCI controller

  @param[in]  UsbHandle               USB Handle
  @param[in]  InterruptPin            Interrupt pin
  @param[in]  Irq                     IRQ number

  @return     EFI_STATUS              Programmed interrupt in xDCI controller
**/
EFI_STATUS
XdciConfigureInterrupt (
  IN  USB_HANDLE      *UsbHandle,
  IN  UINT8           InterruptPin,
  IN  UINT8           Irq
  )
{
  UINT32            Data32Or;
  UINT32            Data32And;
  REGISTER_ACCESS   *XdciSbAccess;

  XdciSbAccess = UsbHandle->DeviceController->SbMmioAccess;

  //
  // Set Interrupt Pin and IRQ number
  //
  Data32Or = (UINT32) ((InterruptPin << N_XDCI_PCR_PCICFGCTR1_IPIN1) |
                        (Irq << N_XDCI_PCR_PCICFGCTR1_PCI_IRQ1));
  Data32And =~(UINT32) (B_XDCI_PCR_PCICFGCTR1_PCI_IRQ1 | B_XDCI_PCR_PCICFGCTR1_ACPI_IRQ1
    | B_XDCI_PCR_PCICFGCTR1_IPIN1 | B_XDCI_PCR_PCICFGCTR1_ACPI_INTR_EN1);

  XdciSbAccess->AndThenOr32 (XdciSbAccess, R_XDCI_PCR_PCICFGCTR1, Data32And, Data32Or);

  return EFI_SUCCESS;
}

/**
  Prints USB Device Controller configuration

  @param[in]  UsbHandle               USB_HANDLE structure
**/
VOID
PrintUsbDeviceControllerInformation (
  IN  USB_HANDLE      *UsbHandle
  )
{
  USB_CONTROLLER    *UsbController;

  UsbController = UsbHandle->DeviceController;
  DEBUG ((DEBUG_INFO, "DeviceController->Usb2PortCount = 0x%08x;\n", UsbController->Usb2PortCount));
  DEBUG ((DEBUG_INFO, "DeviceController->UsbrPortCount = 0x%08x;\n", UsbController->UsbrPortCount));
  DEBUG ((DEBUG_INFO, "DeviceController->Usb3LanesCount = 0x%08x;\n", UsbController->Usb3LanesCount));
  DEBUG ((DEBUG_INFO, "DeviceController->ReservedMmio = 0x%08x;\n", UsbController->ReservedMmio));
}