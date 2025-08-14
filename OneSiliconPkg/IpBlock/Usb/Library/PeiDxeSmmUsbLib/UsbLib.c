/** @file
  Implementation of functions available in general USB Library

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

#include <Library/UsbLib.h>
#include <Library/DebugLib.h>
#include <Register/UsbHostRegs.h>
#include <Library/MmioAccessLib.h>

/**
  Function for direct programming in xHCI's USB2 and USB3 PDO registers
  @param[in]  MmioAccess          xHCI Memory Mapped IO access
  @param[in]  XhciUsb2Pdo         USB2 lane bitmap that will be written to USB2PDO register
  @param[in]  XhciUsb2PdoMask     USB2 PDO port count bitmask
  @param[in]  XhciUsb3Pdo         USB3 lane bitmap that will be written to USB3PDO register
  @param[in]  XhciUsb3PdoMask     USB3 PDO port count bitmask
  @retval     EFI_SUCCESS         Programming ended successfully and no errors occured
              EFI_ACCESS_DENIED   Port Disable Override register was locked and write
                                  didn't go through. Platform may require restart to unlock.
**/
STATIC
EFI_STATUS
ProgramUsbPdo (
  IN REGISTER_ACCESS  *MmioAccess,
  IN UINT32           XhciUsb2Pdo,
  IN UINT32           XhciUsb2PdoMask,
  IN UINT32           XhciUsb3Pdo,
  IN UINT32           XhciUsb3PdoMask
  )
{
  UINT32   XhciUsb2PdoRd;
  UINT32   XhciUsb3PdoRd;

  DEBUG ((DEBUG_INFO, "XhciUsb2Pdo 0x%8X, XhciUsb3Pdo 0x%8X\n", XhciUsb2Pdo, XhciUsb3Pdo));

  //
  // USB2PDO and USB3PDO are Write-Once registers and bits in them are in the SUS Well.
  //
  MmioAccess->Write32 (MmioAccess, R_XHCI_MEM_USB2PDO, XhciUsb2Pdo);
  MmioAccess->Write32 (MmioAccess, R_XHCI_MEM_USB3PDO, XhciUsb3Pdo);

  XhciUsb2PdoRd = MmioAccess->Read32 (MmioAccess, R_XHCI_MEM_USB2PDO);
  XhciUsb3PdoRd = MmioAccess->Read32 (MmioAccess, R_XHCI_MEM_USB3PDO);

  //
  // If USB2PDO and USB3PDO are not updated successfully perform Warm Reset to unlock RWO bits.
  //
  if (((XhciUsb2Pdo & XhciUsb2PdoMask) != (XhciUsb2PdoRd & XhciUsb2PdoMask)) ||
      ((XhciUsb3Pdo & XhciUsb3PdoMask) != (XhciUsb3PdoRd & XhciUsb3PdoMask))) {
    DEBUG ((DEBUG_INFO, "USB2PDO - Read: 0x%8X, Expected: 0x%8X\n", XhciUsb2PdoRd, XhciUsb2Pdo));
    DEBUG ((DEBUG_INFO, "USB3PDO - Read: 0x%8X, Expected: 0x%8X\n", XhciUsb3PdoRd, XhciUsb3Pdo));
    DEBUG ((DEBUG_WARN, "PDO register value doesn't match expected value. PDO register might be locked.\n"));
    return EFI_ACCESS_DENIED;
  }

  return EFI_SUCCESS;
}

/**
  Disables requested ports through Port Disable Override register programming.
  This is extended function that allows for defining how many lanes are available
  since PDO constructed on per lane basis.
  @param[in]  MmioAccess          xHCI Memory Mapped IO access
  @param[in]  Usb2DisabledPorts   Disabled ports bitmap with a bit for each USB2 port
                                  i.e. BIT0 is Port 0, BIT1 is Port 1 etc
  @param[in]  Usb3DisabledPorts   Disabled ports bitmap with a bit for each USB3 port
                                  i.e. BIT0 is Port 0, BIT1 is Port 1 etc
  @param[in]  Usb2LanesCount      Number of USB2 lanes
  @param[in]  Usb3LanesCount      Number of USB3 lanes
  @retval EFI_SUCCESS             Programming ended successfully and no errors occured
          EFI_ACCESS_DENIED       Port Disable Override register was locked and write
                                  didn't go through. Platform may require restart to unlock.
**/
STATIC
EFI_STATUS
IpUsbDisablePortsEx (
  IN REGISTER_ACCESS  *MmioAccess,
  IN UINT32           Usb2DisabledPorts,
  IN UINT32           Usb3DisabledPorts,
  IN UINT32           Usb2LanesCount,
  IN UINT32           Usb3LanesCount
  )
{
  UINT32   XhciUsb2Pdo;
  UINT32   XhciUsb2PdoMask;
  UINT32   XhciUsb3Pdo;
  UINT32   XhciUsb3PdoMask;

  DEBUG ((DEBUG_INFO, "%a - Usb2DisabledPorts = 0x%x\n", __FUNCTION__, Usb2DisabledPorts));
  DEBUG ((DEBUG_INFO, "%a - Usb3DisabledPorts = 0x%x\n", __FUNCTION__, Usb3DisabledPorts));
  DEBUG ((DEBUG_INFO, "%a - Usb2LanesCount    = 0x%x\n", __FUNCTION__, Usb2LanesCount));
  DEBUG ((DEBUG_INFO, "%a - Usb3LanesCount    = 0x%x\n", __FUNCTION__, Usb3LanesCount));

  //
  // Masks for PDO registers based on supported number of ports, with a bit set for each supported port
  //
  XhciUsb2PdoMask = (UINT32)((1 << Usb2LanesCount) - 1);
  XhciUsb3PdoMask = (UINT32)((1 << Usb3LanesCount) - 1);

  DEBUG ((DEBUG_INFO, "XhciUsb2PdoMask 0x%8X, XhciUsb3PdoMask 0x%8X\n", XhciUsb2PdoMask, XhciUsb3PdoMask));

  XhciUsb2Pdo = Usb2DisabledPorts & XhciUsb2PdoMask;  // XHCI PDO for HS
  XhciUsb3Pdo = Usb3DisabledPorts & XhciUsb3PdoMask;  // XHCI PDO for SS

  return ProgramUsbPdo (
           MmioAccess,
           XhciUsb2Pdo,
           XhciUsb2PdoMask,
           XhciUsb3Pdo,
           XhciUsb3PdoMask
           );
}

/**
  Disables requested ports through Port Disable Override register programming.
  This is extended function that allows for defining how many lanes are available
  since PDO constructed on per lane basis.

  @param[in]  XhciMmioBase        xHCI Memory BAR0 address
  @param[in]  Usb2DisabledPorts   Disabled ports bitmap with a bit for each USB2 port
                                  i.e. BIT0 is Port 0, BIT1 is Port 1 etc
  @param[in]  Usb3DisabledPorts   Disabled ports bitmap with a bit for each USB3 port
                                  i.e. BIT0 is Port 0, BIT1 is Port 1 etc
  @param[in]  Usb2LanesCount      Number of USB2 lanes
  @param[in]  Usb3LanesCount      Number of USB3 lanes

  @retval EFI_SUCCESS             Programming ended successfully and no errors occured
          EFI_ACCESS_DENIED       Port Disable Override register was locked and write
                                  didn't go through. Platform may require restart to unlock.
**/
EFI_STATUS
UsbDisablePortsEx (
  IN  UINTN   XhciMmioBase,
  IN  UINT32  Usb2DisabledPorts,
  IN  UINT32  Usb3DisabledPorts,
  IN  UINT32  Usb2LanesCount,
  IN  UINT32  Usb3LanesCount
  )
{
  MMIO_ACCESS   MmioAccess;

  MmioAccessInit (XhciMmioBase, &MmioAccess);

  return IpUsbDisablePortsEx (
           (REGISTER_ACCESS *) &MmioAccess,
           Usb2DisabledPorts,
           Usb3DisabledPorts,
           Usb2LanesCount,
           Usb3LanesCount
           );
}
