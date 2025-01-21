/** @file
  The DXE TCSS Init Library Implements After Memory PEIM

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

#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <TcssDataHob.h>
#include <Ppi/SiPolicy.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Protocol/TcssNvsArea.h>
#include <Library/PcdLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/BaseMemoryLib.h>
#include <PcieRegs.h>
#include <Register/HostDmaRegs.h>
#include <Register/CpuUsbRegs.h>
#include <Register/ItbtPcieRegs.h>
#include <Register/UsbDeviceRegs.h>
#include <Library/DxeSaNvsInitLib.h>
#include <Library/DxeTcssInit.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/HostBridgeDataLib.h>
#include <Register/HostBridgeRegs.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TcssNvsDumpLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED TCSS_NVS_AREA_PROTOCOL         mTcssNvsAreaProtocol;


/**
  This function gets registered as a callback to perform TCSS PCIe initialization before EndOfDxe

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
TcssPciEnumCompleteCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS              Status;
  VOID                    *ProtocolPointer;
  ///
  /// Check if this is first time called by EfiCreateProtocolNotifyEvent() or not,
  /// if it is, we will skip it until real event is triggered
  ///
  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  gBS->CloseEvent (Event);
  TcssIbtPcieConfigPciEnumComplete ();

  return;
}


/**
  DXE TCSS Initialization
**/
VOID
DxeTcssInit (
  VOID
  )
{
  TCSS_DATA_HOB                   *TcssHob;
  BOOLEAN                         TcssXhciExisted;
  EFI_STATUS                      Status;
  VOID                            *Registration;

  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    DEBUG ((DEBUG_ERROR, "DxeTcssInit : Can't find gTcssHobGuid\n"));
    return;
  }

  if (TcssHob->TcssData.IOMReady == 0) {
    DEBUG ((DEBUG_ERROR, "DxeTcssInit : TCSS IOM not ready\n"));
    return;
  }

  TcssXhciExisted = FALSE;

  if (0xFFFF != PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (0, XHCI_NORTH_BUS_NUM, XHCI_NORTH_DEV_NUM, XHCI_NORTH_FUNC_NUM, PCI_VENDOR_ID_OFFSET))) {
    TcssXhciExisted = TRUE;
  }
  if (TcssXhciExisted == TRUE) {
    ///
    /// Get TcssHob HOB
    ///
    TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
    ///
    /// Install SEG SSDT table only if Multiple Segment is enabled
    ///
    if (TcssHob != NULL) {
      if (TcssHob->TcssData.PcieMultipleSegmentEnabled) {
        Status = InstallSsdtAcpiTable (gSegSsdtAcpiTableStorageGuid, SIGNATURE_64 ('S','e','g','S','s','d','t',0));
        ASSERT_EFI_ERROR (Status);
      }

      ///
      /// Install TCSS SSDT Table
      ///
      Status = InstallSsdtAcpiTable (gTcssSsdtAcpiTableStorageGuid, SIGNATURE_64 ('T','c','s','s','S','s','d','t'));
      ASSERT_EFI_ERROR (Status);

      ///
      /// Create PCI Enumeration Completed callback for CPU PCIe
      ///
      EfiCreateProtocolNotifyEvent (
        &gEfiPciEnumerationCompleteProtocolGuid,
        TPL_CALLBACK,
        TcssPciEnumCompleteCallback,
        NULL,
        &Registration
        );

      ///
      /// Update TCSS NVS AREA tables
      ///
      UpdateTcssNVS ();
    }
  }
}

/**
  Update TCSS NVS AREA tables
**/
VOID
UpdateTcssNVS (
  VOID
  )
{
  TCSS_DATA_HOB                    *TcssHob;
  UINT32                           Index = 0;
  TCSS_NVS_AREA_PROTOCOL           *TcssNvsAreaProtocol;
  EFI_STATUS                       Status;

  ///
  /// Update TCSS Device Enable status in Nvs from HOB values
  ///

  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  Status = gBS->LocateProtocol (&gTcssNvsAreaProtocolGuid, NULL, (VOID **) &TcssNvsAreaProtocol);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "[TCSS] Could not update TCSS NVS due to failed to locate TcssNvsAreaProtocol.\n"));
    return;
  }
  DEBUG ((DEBUG_INFO, "[TCSS] Update TCSS GNVS Area.\n"));

  //
  // Initialize default values for TCSS NVS
  //

  //
  // xHCI
  //
  TcssNvsAreaProtocol->Area->ItbtXhciEn = TcssHob->TcssData.ItbtXhciEn;

  //
  // IOM Ready
  //
  TcssNvsAreaProtocol->Area->IomReady = (UINT8) TcssHob->TcssData.IOMReady;

  //
  // IOM VccSt
  //
  TcssNvsAreaProtocol->Area->TcssIomVccSt = TcssHob->TcssData.TcssIomVccSt;

  //
  // TCSS RTD3
  //
  TcssNvsAreaProtocol->Area->TcssRtd3 = TcssHob->TcssData.TcssRtd3;

  //
  // iTBT PCIE rootports
  //
  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    TcssNvsAreaProtocol->Area->ItbtPcieRpEn[Index] = TcssHob->TcssData.ItbtPcieRpEn[Index];
  }
  //
  // Update ASL iTBT PCIE port address according to root port device and function
  //
  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    TcssNvsAreaProtocol->Area->ItbtPcieRpAddress[Index] = ((UINT8) ITBT_PCIE_DEV_NUM << 16) | (UINT8) ((TcssHob->TcssData.ItbtPcieFuncMap >> (3 * Index)) & 0x7);;
    TcssNvsAreaProtocol->Area->LtrEnable[Index] = TcssHob->PcieItbtLtrData[Index].LtrEnable;
    TcssNvsAreaProtocol->Area->PcieLtrMaxSnoopLatency[Index]   = (UINT16) (TcssHob->PcieItbtLtrData[Index].LtrMaxSnoopLatency);
    TcssNvsAreaProtocol->Area->PcieLtrMaxNoSnoopLatency[Index] = (UINT16) (TcssHob->PcieItbtLtrData[Index].LtrMaxNoSnoopLatency);
  }

  //
  // TBT DMA
  //
  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index++) {
    TcssNvsAreaProtocol->Area->ItbtDmaEn[Index] = TcssHob->TcssData.ItbtDmaEn[Index];
  }

  //
  // MCHBAR REGBAR Offset
  //
  TcssNvsAreaProtocol->Area->RegBarOffset = GetRegBar ();

  //
  // Retimer mapping
  //
  TcssNvsAreaProtocol->Area->RetimerMap = TcssHob->TcssData.RetimerMap;
  DEBUG ((DEBUG_INFO, "TCSS RetimerMap: %X\n", TcssHob->TcssData.RetimerMap));

  //
  // Iom Reg Bar base
  //

  TcssNvsAreaProtocol->Area->IomBase = TcssHob->TcssData.IomBase;

  //
  // Dump ACPI TCSS variable
  //
  DumpTcssVariable ();

}

/**
  Initialize TCSS Nvs Area operation region.
  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
EFIAPI
PatchTcssNvsAreaAddress (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT32                                Address;
  UINT16                                Length;
  TCSS_NVS_AREA_PROTOCOL                *TcssNvsAreaProtocol;

  Status = gBS->LocateProtocol (&gTcssNvsAreaProtocolGuid, NULL, (VOID **) &TcssNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);

  Address = (UINT32) (UINTN) TcssNvsAreaProtocol->Area;
  Length  = (UINT16) sizeof (TCSS_NVS_AREA);
  DEBUG ((DEBUG_INFO, "Patch TCSS NvsAreaAddress: TCSS NVS Address %x Length %x\n", Address, Length));

  Status  = UpdateNameAslCode (SIGNATURE_32 ('T','C','N','B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status  = UpdateNameAslCode (SIGNATURE_32 ('T','C','N','L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);

  return Status;
}


/**
  Install TCSS NVS protocol
  @param[in]  ImageHandle  Image Handle.
**/
VOID
InstallTcssNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  )
{

  EFI_STATUS                Status;

  ///
  /// Install TCSS Global NVS protocol
  ///
  DEBUG ((DEBUG_INFO, "Install TCSS GNVS protocol\n"));
  Status = (gBS->AllocatePool) (EfiACPIMemoryNVS, sizeof (TCSS_NVS_AREA), (VOID **) &mTcssNvsAreaProtocol.Area);
  ASSERT_EFI_ERROR (Status);

  ZeroMem ((VOID *) mTcssNvsAreaProtocol.Area, sizeof (TCSS_NVS_AREA));
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gTcssNvsAreaProtocolGuid,
                  &mTcssNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}
