/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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
#include <Uefi.h>
#include <IndustryStandard/Pci22.h>
#include <Protocol/PciIo.h>
#include <Library/DebugLib.h>

VOID
EFIAPI
OpromPatchPciIo (
  IN EFI_PCI_IO_PROTOCOL           *PciIo
  )
{
  PCI_DEVICE_INDEPENDENT_REGION  PciHeader;
  EFI_PCI_ROM_HEADER             RomHeader;
  PCI_DATA_STRUCTURE             *Pcir;
  UINTN                          Segment;
  UINTN                          Bus;
  UINTN                          Device;
  UINTN                          Function;

  if ((PciIo->RomSize == 0) || (PciIo->RomImage == NULL)) {
    return ;
  }
  PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
  PciIo->Pci.Read (
               PciIo,
               EfiPciIoWidthUint32,
               0,
               (sizeof (PciHeader.VendorId) + sizeof (PciHeader.DeviceId)) / sizeof (UINT32),
               &PciHeader
               );
  DEBUG ((DEBUG_INFO, "OpromPatchEntrypoint - %02x, %02x, %02x\n", Bus, Device, Function));
  DEBUG ((DEBUG_INFO, "OpromPatchEntrypoint - %04x:%04x\n", (UINTN)PciHeader.VendorId, (UINTN)PciHeader.DeviceId));
  DEBUG ((DEBUG_INFO, "OpromPatchEntrypoint - Rom: %08x:%08x\n", (UINTN)PciIo->RomImage, (UINTN)PciIo->RomSize));
  RomHeader.Raw = PciIo->RomImage;
  Pcir = (PCI_DATA_STRUCTURE *) (RomHeader.Raw + RomHeader.Generic->PcirOffset);
  DEBUG ((DEBUG_INFO, "OpromPatchEntrypoint - Pcir: %04x:%04x\n", (UINTN)Pcir->VendorId, (UINTN)Pcir->DeviceId));

  //
  // Fix MS-HD5770 video adapter can not work:
  // This device is not a OPROM 3.0 and does not have device id list as well.
  // It only have 1 device id in OPROM.
  // Device Id in OpROM is not same with the value in PCI configuration space
  // it will cause VBIOS fails to start
  //
  if ((PciHeader.VendorId == 0x1002) && (PciHeader.DeviceId == 0x68BE)) {
    DEBUG ((DEBUG_INFO, "MS-HD5770 video adapter\n"));
    if ((Pcir->VendorId == 0x1002) &&(Pcir->DeviceId ==0x68B8)) {
      //
      // Assign same device id in PCI configuration space
      //
      Pcir->DeviceId = PciHeader.DeviceId;
    }
  }

  return ;
}

