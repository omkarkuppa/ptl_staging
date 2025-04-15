/** @file
  DXE Library for Initializing VMD FRU

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
#include <Library/DxeVmdInitLib.h>
#include <Protocol/VmdNvsArea.h>
#include <Library/PchPciBdfLib.h>
#include <Library/VmdInfoLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/AslUpdateLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED VMD_NVS_AREA_PROTOCOL  mVmdNvsAreaProtocol;

/**
  Update VMD NVS area.

  @param[in]  VmdInfoHob              Pointer to VMD_INFO_HOB
**/
VOID
UpdateVmdNvs (
  VMD_INFO_HOB                    *VmdInfoHob
  )
{
  UINT8                           VmdDevIndex;
  VMD_NVS_AREA_PROTOCOL           *VmdNvsAreaProtocol;
  EFI_STATUS                      Status;

  Status = gBS->LocateProtocol (&gVmdNvsAreaProtocolGuid, NULL, (VOID **) &VmdNvsAreaProtocol);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Locate VMD NVS Area failed.\n"));
    return;
  }

  if ((VmdNvsAreaProtocol->Area->VmdEnable = IsVmdEnabled ()) == 1) {
    VmdNvsAreaProtocol->Area->VmdSataPort0to7 = 0;
    VmdNvsAreaProtocol->Area->VmdSocPciePorts = 0;
    VmdNvsAreaProtocol->Area->VmdPchPcieRp    = 0;
    VmdNvsAreaProtocol->Area->PchRpBusNo      = 0;

    if (VmdInfoHob != NULL) {
      for (VmdDevIndex = 0; VmdDevIndex < VMD_MAX_DEVICES; ++VmdDevIndex) {
        if (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].PortEn) {
          if (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpBus == 0) {
            DEBUG ((DEBUG_INFO, "VMD NVS protocol - PCIe RP with B/D/F %d/%d/%d is remapped under VMD\n", \
                  VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpBus, \
                  VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev, \
                  VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc));
            ///
            /// SOC + IOE PCIe RPs mapped under VMD
            ///
            if ((VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9) && \
                (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc == PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1)) {
              // B0D6F0
              VmdNvsAreaProtocol->Area->VmdSocPciePorts |= 0x1;
            } else if ((VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_10) && \
                       (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc == PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2)) {
              // B0D6F1
              VmdNvsAreaProtocol->Area->VmdSocPciePorts |= (0x1 << 1);
            } else if ((VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_11) && \
                       (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc == PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3)) {
              /// B0D6F2
              VmdNvsAreaProtocol->Area->VmdSocPciePorts |= (0x1 << 2);
            } else if ((VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_10) && \
                       (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc == PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4)) {
              // B0D6F3
              VmdNvsAreaProtocol->Area->VmdSocPciePorts |= (0x1 << 3);
            } else if ((VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_10) && \
                       (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc == PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5)) {
              // B0D6F4
              VmdNvsAreaProtocol->Area->VmdSocPciePorts |= (0x1 << 4);
            } else if ((VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12) && \
                      (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc == PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_12)) {
              // B0D1F0
              VmdNvsAreaProtocol->Area->VmdSocPciePorts |= (0x1 << 5);
            }
          } else if (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpBus == VmdNvsAreaProtocol->Area->PchRpBusNo) {
            DEBUG ((DEBUG_INFO, "VMD NVS protocol - PCIe RP with B/D/F %d/%d/%d is remapped under VMD\n", \
                  VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpBus, \
                  VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev, \
                  VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc));

            if (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev == 28) {
              VmdNvsAreaProtocol->Area->VmdPchPcieRp |= (UINT32) (0x1 << (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc));
            } else if (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev == 29) {
              VmdNvsAreaProtocol->Area->VmdPchPcieRp |= (UINT32) (0x100 << (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc));
            } else if (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev == 27) {
              VmdNvsAreaProtocol->Area->VmdPchPcieRp |= (UINT32) (0x10000 << (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpFunc));
            }
          }

          ///
          /// SATA Ports
          ///
          if (VmdInfoHob->VmdPortInfo.PortInfo[VmdDevIndex].RpDev == PCI_DEVICE_NUMBER_PCH_SATA_1) {
            DEBUG ((DEBUG_INFO, "VMD NVS protocol - SATA is mapped under VMD\n"));
            VmdNvsAreaProtocol->Area->VmdSataPort0to7 = 0xFF;
          }
        }
      }
    }
  }
}

/**
  Initialize VMD Nvs Area operation region.

  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
EFIAPI
PatchVmdNvsAreaAddress (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT32                                Address;
  UINT16                                Length;
  VMD_NVS_AREA_PROTOCOL                 *VmdNvsAreaProtocol;

  Status = gBS->LocateProtocol (&gVmdNvsAreaProtocolGuid, NULL, (VOID **) &VmdNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);

  Address = (UINT32) (UINTN) VmdNvsAreaProtocol->Area;
  Length  = (UINT16) sizeof (VMD_NVS_AREA);
  DEBUG ((DEBUG_INFO, "PatchVmdNvsAreaAddress: VMD NVS Address %x Length %x\n", Address, Length));

  Status  = UpdateNameAslCode (SIGNATURE_32 ('V','M','N','B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status  = UpdateNameAslCode (SIGNATURE_32 ('V','M','N','L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install VMD Global NVS protocol

  @param[in]  ImageHandle  Image Handle.
**/
VOID
EFIAPI
InstallVmdNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  )
{
  EFI_STATUS                Status;

  ///
  /// Install VMD Global NVS protocol
  ///
  DEBUG ((DEBUG_INFO, "Install VMD NVS protocol\n"));
  Status = (gBS->AllocatePool) (EfiACPIMemoryNVS, sizeof (VMD_NVS_AREA), (VOID **) &mVmdNvsAreaProtocol.Area);
  ASSERT_EFI_ERROR (Status);

  ZeroMem ((VOID *) mVmdNvsAreaProtocol.Area, sizeof (VMD_NVS_AREA));

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gVmdNvsAreaProtocolGuid,
                  &mVmdNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}
