/** @file
  Dxe Host Bridge Init Library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <Library/DxeHostBridgeInitLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/DxeSaInitFruLib.h>
#include <Protocol/HostBridgeNvsArea.h>
#include <Register/SncuRegs.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiDxeSmmReserveMmio64SizeLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/HostBridgeInfoLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED HOST_BRIDGE_NVS_AREA_PROTOCOL  mHostBridgeNvsAreaProtocol;

/**
  A protocol callback which updates 64bits MMIO Base and Length in Host Bridge GNVS area
**/
VOID
EFIAPI
UpdateHostBridgeNvs (
  VOID
  )
{
  EFI_PHYSICAL_ADDRESS              PciBaseAddress;
  UINT64                            Length;
  UINTN                             ResMemLimit1;
  BOOLEAN                           EnableAbove4GBMmioBiosAssignemnt;
  UINT64                            PhysicalAddressLimit;
  EFI_STATUS                        Status;
  HOST_BRIDGE_NVS_AREA_PROTOCOL     *HostBridgeNvsAreaProtocol;

  PciBaseAddress                   = 0;
  Length                           = 0;
  ResMemLimit1                     = 0;
  EnableAbove4GBMmioBiosAssignemnt = IsAbove4GBMmioEnabled ();
  PhysicalAddressLimit             = 0;
  //
  // Read memory map registers
  //
  PciBaseAddress = (EFI_PHYSICAL_ADDRESS) GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase);
  ResMemLimit1 = (UINTN) PcdGet64 (PcdSiPciExpressBaseAddress);
  Length = ResMemLimit1 - PciBaseAddress;
  PhysicalAddressLimit = LShiftU64 (1, GetMaxPhysicalAddressSize ());

  Status = gBS->LocateProtocol (&gHostBridgeNvsAreaProtocolGuid, NULL, (VOID **) &HostBridgeNvsAreaProtocol);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Locate Host Bridge NVS Area failed.\n"));
    return;
  }
  //
  // Check Enable Above 4GB MMIO or not
  //
  DEBUG ((DEBUG_INFO, "Update Host Bridge GNVS Area.\n"));
  HostBridgeNvsAreaProtocol->Area->Mmio32Base   = (UINT32) PciBaseAddress;
  HostBridgeNvsAreaProtocol->Area->Mmio32Length = (UINT32) Length;
  if (EnableAbove4GBMmioBiosAssignemnt == TRUE) {
    if ((PhysicalAddressLimit % PcdGet64(PcdMmio64Base)) == 0) { // Checking Memory Size multiples of PcdMmio64Base.
      HostBridgeNvsAreaProtocol->Area->Mmio64Base   = PcdGet64(PcdMmio64Base);
      //
      // Some platforms need to reserve MMIO space from PhysicalAddressLimit for P2SB usage.
      //
      HostBridgeNvsAreaProtocol->Area->Mmio64Length = PhysicalAddressLimit - HostBridgeNvsAreaProtocol->Area->Mmio64Base - ReserveMmio64Size ();
    } else {
      HostBridgeNvsAreaProtocol->Area->Mmio64Base   = PcdGet64(PcdMmio64Base);
      HostBridgeNvsAreaProtocol->Area->Mmio64Length = PcdGet64(PcdMmio64Length);
    }
  }
  DEBUG ((DEBUG_INFO, "HostBridgeNvsAreaProtocol.Area->Mmio64Base = %lx\n", HostBridgeNvsAreaProtocol->Area->Mmio64Base));
  DEBUG ((DEBUG_INFO, "HostBridgeNvsAreaProtocol.Area->Mmio64Length = %lx\n", HostBridgeNvsAreaProtocol->Area->Mmio64Length));
  DEBUG ((DEBUG_INFO, "HostBridgeNvsAreaProtocol.Area->Mmio32Base = %lx\n", HostBridgeNvsAreaProtocol->Area->Mmio32Base));
  DEBUG ((DEBUG_INFO, "HostBridgeNvsAreaProtocol.Area->Mmio32Length = %lx\n", HostBridgeNvsAreaProtocol->Area->Mmio32Length));
}


/**
  Initialize Host Bridge Nvs Area operation region.

  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
EFIAPI
PatchHostBridgeNvsAreaAddress (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT32                                Address;
  UINT16                                Length;
  HOST_BRIDGE_NVS_AREA_PROTOCOL         *HostBridgeNvsAreaProtocol;

  Status = gBS->LocateProtocol (&gHostBridgeNvsAreaProtocolGuid, NULL, (VOID **) &HostBridgeNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);

  Address = (UINT32) (UINTN) HostBridgeNvsAreaProtocol->Area;
  Length  = (UINT16) sizeof (HOST_BRIDGE_NVS_AREA);
  DEBUG ((DEBUG_INFO, "PatchHostBridgeNvsAreaAddress: HostBridge NVS Address %x Length %x\n", Address, Length));

  Status  = UpdateNameAslCode (SIGNATURE_32 ('H','B','N','B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status  = UpdateNameAslCode (SIGNATURE_32 ('H','B','N','L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install Host Bridge Global NVS protocol

  @param[in]  ImageHandle  Image Handle.
**/
VOID
EFIAPI
InstallHostBridgeNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  )
{
  EFI_STATUS                Status;

  ///
  /// Install Host Bridge Global NVS protocol
  ///
  DEBUG ((DEBUG_INFO, "Install Host Bridge GNVS protocol\n"));
  Status = (gBS->AllocatePool) (EfiACPIMemoryNVS, sizeof (HOST_BRIDGE_NVS_AREA), (VOID **) &mHostBridgeNvsAreaProtocol.Area);
  ASSERT_EFI_ERROR (Status);

  ZeroMem ((VOID *) mHostBridgeNvsAreaProtocol.Area, sizeof (HOST_BRIDGE_NVS_AREA));
  mHostBridgeNvsAreaProtocol.Area->XPcieCfgBaseAddress  = (UINT32) (PcdGet64 (PcdSiPciExpressBaseAddress));
  mHostBridgeNvsAreaProtocol.Area->SimicsEnvironment = (UINT8) IsSimicsEnvironment ();

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gHostBridgeNvsAreaProtocolGuid,
                  &mHostBridgeNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}