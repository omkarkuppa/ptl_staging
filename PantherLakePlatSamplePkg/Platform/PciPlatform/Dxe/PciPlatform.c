/** @file
  Registers onboard PCI ROMs with PCI.IO

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

#include "PciPlatform.h"
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/ConfigBlockLib.h>
#include <SetupVariable.h>
#include <Library/EcTcssLib.h>
#include <Library/TcssInfoLib.h>
#include <Library/PcdLib.h>
//
// Files PlatformOpRomTable.h and PlatformOpRomTable_Default.inc will be created at runtime
//
#include "MultiPlatform/PlatformOpRomTable.h"
#include "MultiPlatform/PlatformOptionRomTable_Default.h"
#include <Register/PchRegs.h>
#include <Library/SmbusInfoLib.h>
#include <Register/UsbHostRegs.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PchPciBdfLib.h>
#include <PcieRegs.h>
#include <IGpuConfig.h>
#include <Library/BrRouterSupportLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PCI_PLATFORM_PROTOCOL mPciPlatform = {
  PhaseNotify,
  PlatformPrepController,
  GetPlatformPolicy,
  GetPciRom
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HANDLE mPciPlatformHandle = NULL;

/**
  Enable the Host DMAx (x=0/1/2) PCI CMD Register bits

**/
VOID
EnableDMAxCommandReg (
  VOID
  )
{
  UINT32  DmaBar;
  UINT16  Command;
  UINT8   Func;
  UINT64  DmaBaseAddress;
  UINT8   TbtDma0FuncNumber;

  // Save TbtDma0FuncNumber
  TbtDma0FuncNumber = (UINT8) GetTbtDma0FuncNumber ();

  DEBUG((DEBUG_INFO, "Enabling  Host Dma PCI Command Register \n "));
  for (Func = TbtDma0FuncNumber; ((Func <= (TbtDma0FuncNumber + MAX_HOST_ITBT_DMA_NUMBER - 1)) && (Func <= PCI_MAX_FUNC)); Func++) {

    //
    // Step 1. Make sure the Host DMA device is enabled.
    //
    DmaBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, GetTbtDmaBusNumber (), GetTbtDmaDevNumber (), Func, 0);
    if (PciSegmentRead16 (DmaBaseAddress + PCI_DEVICE_ID_OFFSET) == V_SA_DEVICE_ID_INVALID)  {
      continue;
    }

    //
    // Step 2. Make sure the Host DMA BAR is initialized.
    //         Check if the lower 32 bits of the 64-bit BAR are configured.
    //
    DmaBar = PciSegmentRead32 (DmaBaseAddress + PCI_BASE_ADDRESSREG_OFFSET) & ~(0xF);
    if ((DmaBar & 0xFFFF0000) == 0) {
      //
      // If the lower 32 bits are not configured, check the upper 32 bits.
      //
      DmaBar = PciSegmentRead32 (DmaBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX1 * 4));
      if (DmaBar == 0) {
        continue;
      }
    }

    //
    // Step 3. If Host DMA MSE (Memory Space Enable) or BME (Bus Main Enable)
    //         bits are cleared, set it.
    Command = PciSegmentRead16 (DmaBaseAddress + PCI_COMMAND_OFFSET);
    if ((Command & (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) != (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) {
      PciSegmentOr16 (DmaBaseAddress + PCI_COMMAND_OFFSET, (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER));
    }
  }
  return;
}

/**
  Attempt to set the XHCI controller's PCI CMD.MSE bit to enable OS kernel debugging over XHCI.

**/
VOID
AttemptToSetXhciMse (
  IN  UINT8   KernelDebugPatch
  )
{
  UINT32  XhciBar;
  UINT32  TcssXhciBar;
  UINT16  Command;

  //
  // Step 1. Make sure the XHCI controller is enabled.
  //
// @todo : Replace with XHCI Enable/Disable code
//  if ((MmioRead32 (RC + R_PCH_RCRB_FUNC_DIS) & (UINT32) B_PCH_RCRB_FUNC_DIS_XHCI) != 0) {
//    return;
//  }

  //
  // Step 2. Make sure the XHCI BAR is initialized.
  //         Check if the lower 32 bits of the 64-bit BAR are configured.
  //
  XhciBar = PciSegmentRead32 (PchXhciPciCfgBase () + R_XHCI_CFG_MBAR_N0);
  XhciBar &= ~(0xF);
  if ((XhciBar & 0xFFFF0000) == 0) {
    //
    // If the lower 32 bits are not configured, check the upper 32 bits.
    //
    XhciBar = PciSegmentRead32 (PchXhciPciCfgBase () + R_XHCI_CFG_MBAR_N0 + 4);
  }
  // TCSS XHCI BAR
  TcssXhciBar = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (
                                SA_SEG_NUM,
                                GetTcssXhciBusNumber (),
                                GetTcssXhciDevNumber (),
                                GetTcssXhciFuncNumber (),
                                R_XHCI_CFG_MBAR_N0
                                ));
  TcssXhciBar &= ~(0xF);
  if ((TcssXhciBar & 0xFFFF0000) == 0) {
    //
    // If the lower 32 bits are not configured, check the upper 32 bits.
    //
    TcssXhciBar = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (
                                  SA_SEG_NUM,
                                  GetTcssXhciBusNumber (),
                                  GetTcssXhciDevNumber (),
                                  GetTcssXhciFuncNumber (),
                                  R_XHCI_CFG_MBAR_N0
                                  ) + 4);
  }
  //
  // Step 3. If XHCI's MSE (Memory Space Enable) or BME (Bus Main Enable) bits are cleared, set it.
  //
  if (XhciBar != 0) {
    Command = PciSegmentRead16 (PchXhciPciCfgBase () + PCI_COMMAND_OFFSET);
    if ((Command & (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) != (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) {
      PciSegmentOr16 (PchXhciPciCfgBase () + PCI_COMMAND_OFFSET,
                      (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)
                      );
    }
  }
  // TCSS XHCI BAR
  if ((TcssXhciBar != 0) && KernelDebugPatch) {
    Command = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (
                                  SA_SEG_NUM,
                                  GetTcssXhciBusNumber (),
                                  GetTcssXhciDevNumber (),
                                  GetTcssXhciFuncNumber (),
                                  PCI_COMMAND_OFFSET
                                  ));
    if ((Command & (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) != (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) {
      PciSegmentOr16 (PCI_SEGMENT_LIB_ADDRESS (
                        SA_SEG_NUM,
                        GetTcssXhciBusNumber (),
                        GetTcssXhciDevNumber (),
                        GetTcssXhciFuncNumber (),
                        PCI_COMMAND_OFFSET
                        ),
                      (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)
                      );
    }
  }
  return;
}

EFI_STATUS
EFIAPI
PhaseNotify (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PHASE  Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
{
  EFI_STATUS                    Status;
  UINTN                         DataSize;
  SETUP_DATA                    SetupData;
  UINT8                         EcDataBuffer;
  UINT32                        Timeout;
  UINT8                         KernelDebugPatch;

  //
  // This is the flow to override the PCI resources allocation
  // For Thunderbolt boot Support
  // [1] Enumerate the Thunderbolt boot devices and allocate resources based on standard enumeration algorithm
  // [2] After PCI enumeration is done, invoke Thunderbolt SMI handler to specifically allocate resources, which can't be done by standard PCI enumeration algorithm. This step is performed by here when EfiPciHostBridgeBeginResourceAllocation phase is notified.
  // [3] Update BAR info already initialized by PCI enumeration using the new BAR address overridden by the Thunderbolt SMI handler; otherwise PciIo protocol can't provide the real BAR info.
  //
  if (Phase == EfiPciHostBridgeEndResourceAllocation) {
    DataSize = sizeof (SETUP_DATA);
    Status = gRT->GetVariable (
                    L"Setup",
                    &gSetupVariableGuid,
                    NULL,
                    &DataSize,
                    &SetupData
                    );
    if (Status == EFI_SUCCESS) {
      KernelDebugPatch = SetupData.KernelDebugPatch;
    } else {
      KernelDebugPatch = 0;
      DEBUG ((DEBUG_ERROR, "PhaseNotify: Failed to get Setup Variable.\n"));
    }

    PciSegmentWrite32 (SmbusPciCfgBase () + R_PCI_BAR4_OFFSET, PcdGet16 (PcdSmbusBaseAddress));

    //
    // Set XHCI's MSE (Memory Space Enable) bit in the PCI command register.
    // This is required to support Windows Kernel Debugger through the USB3 debug cable.
    // Like EHCI, the XHCI bit should always be set (i.e., even if XHCI kernel debugging isn't expected).
    // Set the MSE bit here for 2 reasons:
    // 1. This must be done before the EFI XHCI driver runs.  As a standard PCI device driver, the XHCI driver
    // saves/restores the Command register before/after it runs.
    // 2. The BAR must already be initialized so that we can set the MMIO Enable bit.
    //
    AttemptToSetXhciMse (KernelDebugPatch);

    //
    // This must be initialized after HOST DMA device is enumerated
    //
    EnableDMAxCommandReg();
  } else if (Phase == EfiPciHostBridgeEndEnumeration && ChipsetPhase == ChipsetEntry) {
    //
    // Send Get USB Conn Status command to EC
    //
    DataSize = sizeof (SETUP_DATA);
    Status = gRT->GetVariable (
                    L"Setup",
                    &gSetupVariableGuid,
                    NULL,
                    &DataSize,
                    &SetupData
                    );
    // Send UsbcGetUsbConnStatus only if EC Handshake is enabled in BIOS setup
    if (!EFI_ERROR(Status) && (SetupData.UsbcBiosTcssHandshake == 1) && (!PcdGetBool(VpdPcdTcssPmcPdEnable))) {
      EcDataBuffer = 0x00;
      Timeout = SetupData.UsbConnStatusTimeout;
      Status = UsbcGetUsbConnStatus(&EcDataBuffer, Timeout);
      // Do some error checking here to handle failure
    }
  } else if (Phase == EfiPciHostBridgeSetResources) {
    //
    // Enable BME on BR tree for PXE boot
    //
    EnableBrBmeCallBack ();
  }

  return EFI_SUCCESS;
}

/**
  Platform Prep Controller

  This          - GC_TODO: add argument description
  HostBridge    - GC_TODO: add argument description
  RootBridge    - GC_TODO: add argument description
  PciAddress    - GC_TODO: add argument description
  Phase         - GC_TODO: add argument description
  ChipsetPhase  - GC_TODO: add argument description

  EFI_UNSUPPORTED - GC_TODO: Add description for return value
**/
EFI_STATUS
EFIAPI
PlatformPrepController (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_HANDLE                                     RootBridge,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS    PciAddress,
  IN  EFI_PCI_CONTROLLER_RESOURCE_ALLOCATION_PHASE   Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get Platform Policy

  This      - GC_TODO: add argument description
  PciPolicy - GC_TODO: add argument description

  EFI_UNSUPPORTED - GC_TODO: Add description for return value
**/
EFI_STATUS
EFIAPI
GetPlatformPolicy (
  IN CONST EFI_PCI_PLATFORM_PROTOCOL               *This,
  OUT EFI_PCI_PLATFORM_POLICY                      *PciPolicy
  )
{
  if (IsSimicsEnvironment () || IsHfpgaEnvironment() ) {
    *PciPolicy = EFI_RESERVE_ISA_IO_NO_ALIAS | EFI_RESERVE_VGA_IO_ALIAS;
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

/**
  Return a PCI ROM image for the onboard device represented by PciHandle

  This      - Protocol instance pointer.
  PciHandle - PCI device to return the ROM image for.
  RomImage  - PCI Rom Image for onboard device
  RomSize   - Size of RomImage in bytes

  EFI_SUCCESS   - RomImage is valid
  EFI_NOT_FOUND - No RomImage
**/
EFI_STATUS
EFIAPI
GetPciRom (
  IN  CONST EFI_PCI_PLATFORM_PROTOCOL             *This,
  IN  EFI_HANDLE                                  PciHandle,
  OUT VOID                                        **RomImage,
  OUT UINTN                                       *RomSize
  )
{
  EFI_STATUS                    Status;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINTN                         Segment;
  UINTN                         Bus;
  UINTN                         Device;
  UINTN                         Function;
  UINT16                        VendorId;
  UINT16                        DeviceId;
  UINTN                         HandleCount;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         TableIndex;
  UINTN                         Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FirmwareVolume;
  UINT32                        AuthenticationStatus;
  VOID                          *LocalRomImage;
  UINTN                         LocalRomSize;
  UINT8                         ClassCode[3];
  UINTN                         SaSetupSize;
  SA_SETUP                      SaSetup;

  FirmwareVolume = NULL;

  Status = gBS->HandleProtocol (
                  PciHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo
                  );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  PciIo->GetLocation (
           PciIo,
           &Segment,
           &Bus,
           &Device,
           &Function
           );

  PciIo->Pci.Read (
              PciIo,
              EfiPciIoWidthUint16,
              0,
              1,
              &VendorId
              );

  PciIo->Pci.Read (
              PciIo,
              EfiPciIoWidthUint16,
              2,
              1,
              &DeviceId
              );

  PciIo->Pci.Read (
              PciIo,
              EfiPciIoWidthUint8,
              PCI_CLASSCODE_OFFSET,
              sizeof(ClassCode),
              &ClassCode[0]
              );

  // Work around to skip the dGPU's EFI Option Rom which is getting started by connectcontroller in
  // core code by LegacyBootManagerLib.c.
  if ((ClassCode[2] == PCI_CLASS_DISPLAY) && (VendorId != INTEL_VENDOR_ID)) {
    SaSetupSize = sizeof (SA_SETUP);
    Status = gRT->GetVariable(
                    L"SaSetup",
                    &gSaSetupVariableGuid,
                    NULL,
                    &SaSetupSize,
                    &SaSetup
                    );
    if (!EFI_ERROR(Status)) {
      if (SaSetup.PrimaryDisplay == DISPLAY_IGD || SaSetup.PrimaryDisplay == DISPLAY_HG) {
        //
        // When Primary Display is HG/IGD we need to skip VGA dGPU option rom, both EFI & Legacy option ROM.
        //
        PciIo->RomImage = NULL;
        PciIo->RomSize = 0;
        DEBUG((DEBUG_INFO, "Skip Discrete Option ROM, Both EFI (GOP) & Legacy option ROM for Device VendorId : %x\n", VendorId));
        return EFI_NOT_FOUND;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "GetRom  Bus  0x%X\n", Bus));
  DEBUG ((DEBUG_INFO, "GetRom  Device  0x%X\n", Device));
  DEBUG ((DEBUG_INFO, "GetRom  Function  0x%X\n", Function));
  DEBUG ((DEBUG_INFO, "GetRom  VID  0x%X\n", VendorId));
  DEBUG ((DEBUG_INFO, "GetRom  DID  0x%X\n", DeviceId));

  OpromPatchPciIo (PciIo);

  //
  // Get the list of available firmware volumes
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || HandleCount == 0) {
    return EFI_NOT_FOUND;
  }
  //
  // Loop through table of video option rom descriptions
  //
  for (TableIndex = 0; mPciOptionRomTable_Default[TableIndex].VendorId != 0xffff; TableIndex++) {
    //
    // See if the PCI device specified by PciHandle matches at device in mPciOptionRomTable_Default
    //
    if (VendorId != mPciOptionRomTable_Default[TableIndex].VendorId) {
      continue;
    }

    if (DeviceId != mPciOptionRomTable_Default[TableIndex].DeviceId) {
      continue;
    }

    if (Segment != mPciOptionRomTable_Default[TableIndex].Segment) {
      continue;
    }

    if (Bus != mPciOptionRomTable_Default[TableIndex].Bus) {
      continue;
    }

    if (Device != mPciOptionRomTable_Default[TableIndex].Device) {
      continue;
    }

    if (Function != mPciOptionRomTable_Default[TableIndex].Function) {
      continue;
    }
    //
    // Loop through the Firmware Volumes looking for the Easton 815 Video ROM image
    //
    for (Index = 0; Index < HandleCount; Index++) {
      //
      // Get the Firmware Volume Protocol
      //
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gEfiFirmwareVolume2ProtocolGuid,
                      (VOID **) &FirmwareVolume
                      );
      if (EFI_ERROR (Status)) {
        continue;
      }
      //
      // Get the 16-bit BIOS code from the same Firmware Volume as this driver
      //
      LocalRomImage = NULL;
      LocalRomSize  = 0;
      Status = FirmwareVolume->ReadSection (
                                 FirmwareVolume,
                                 &mPciOptionRomTable_Default[TableIndex].FileName,
                                 EFI_SECTION_RAW,
                                 0,
                                 &LocalRomImage,
                                 &LocalRomSize,
                                 &AuthenticationStatus
                                 );
      if (EFI_ERROR (Status)) {
        continue;
      }

      *RomImage = LocalRomImage;
      *RomSize  = LocalRomSize;

      FreePool (HandleBuffer);

      return EFI_SUCCESS;
    }
  }

  FreePool (HandleBuffer);

  return EFI_NOT_FOUND;
}


/**
  PciPlatformDriverEntry

  EFI_STATUS
**/
EFI_STATUS
EFIAPI
PciPlatformDriverEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                Status;

  //
  // Install on a new handle
  //
  Status = gBS->InstallProtocolInterface (
                  &mPciPlatformHandle,
                  &gEfiPciPlatformProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPciPlatform
                  );

  return Status;
}
