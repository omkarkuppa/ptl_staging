## @file
#  DSC file of core DXE driver that contain into UPL binary
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
# @par Specification
#
##

[Components.X64]

  MdeModulePkg/Core/Dxe/DxeMain.inf {
  <LibraryClasses>
  !if gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable == TRUE
    NULL|SecurityPkg/Library/DxeRsa2048Sha256GuidedSectionExtractLib/DxeRsa2048Sha256GuidedSectionExtractLib.inf
  !endif
  !if gPlatformModuleTokenSpaceGuid.PcdLzmaEnable == TRUE
    NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  !endif
  }

  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }

  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf {
    <LibraryClasses>
      SerialIoUartLib|$(PLATFORM_SI_PACKAGE)/IpBlock/SerialIo/Uart/Library/PeiDxeSmmSerialIoUartLib/DxeRuntimeSerialIoUartLib.inf
      !if $(TARGET) == DEBUG
        DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
      !else
        !if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
          DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
        !endif
      !endif
  }
  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  UefiCpuPkg/CpuIo2Dxe/CpuIo2Dxe.inf
  MdeModulePkg/Universal/Metronome/Metronome.inf
  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  PcAtChipsetPkg/PcatRealTimeClockRuntimeDxe/PcatRealTimeClockRuntimeDxe.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf

  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf

  UefiCpuPkg/CpuDxe/CpuDxe.inf

  PcAtChipsetPkg/HpetTimerDxe/HpetTimerDxe.inf

!if gTseFeaturePkgTokenSpaceGuid.PcdTseFeatureEnable == FALSE
  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
!endif

  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf

!if gSiPkgTokenSpaceGuid.PcdAtaEnable == TRUE
  MdeModulePkg/Bus/Pci/SataControllerDxe/SataControllerDxe.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
!endif

  MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  MdeModulePkg/Bus/Pci/UhciDxe/UhciDxe.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
!if gPlatformModuleTokenSpaceGuid.PcdUsbEnable == TRUE
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
!endif
  MdeModulePkg/Bus/Isa/Ps2KeyboardDxe/Ps2KeyboardDxe.inf
!if gPlatformModuleTokenSpaceGuid.PcdMouseEnable == TRUE
  MdeModulePkg/Bus/Isa/Ps2MouseDxe/Ps2MouseDxe.inf
!endif

!if gPlatformModuleTokenSpaceGuid.PcdEbcEnable == TRUE
  MdeModulePkg/Universal/EbcDxe/EbcDxe.inf
!endif

!if gPlatformModuleTokenSpaceGuid.PcdScsiEnable == TRUE
  MdeModulePkg/Bus/Scsi/ScsiBusDxe/ScsiBusDxe.inf
  MdeModulePkg/Bus/Scsi/ScsiDiskDxe/ScsiDiskDxe.inf
!endif

  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  FatPkg/EnhancedFatDxe/Fat.inf

  MdeModulePkg/Universal/Console/GraphicsOutputDxe/GraphicsOutputDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf {
    <LibraryClasses>
    !if $(TARGET) == DEBUG
      # It can't use PeiDxeDebugLibReportStatusCode as DebugLib, otherwise some FPDT log will not shown.
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
    !endif
    !if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
    !endif
  }
!endif

!if gBoardModuleTokenSpaceGuid.PcdTerminalEnable == TRUE
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
!endif
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

!if gPlatformModuleTokenSpaceGuid.PcdMemoryTestEnable == TRUE
  MdeModulePkg/Universal/MemoryTest/GenericMemoryTestDxe/GenericMemoryTestDxe.inf
!else
  MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf
!endif

  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf

  #
  # SMBIOS Support
  #
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf

  #
  # ACPI Support
  #
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  ShellPkg/DynamicCommand/DpDynamicCommand/DpDynamicCommand.inf
!endif

!if gBoardModuleTokenSpaceGuid.PcdUefiShellEnable == TRUE
  #
  #UEFI Shell
  #
  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf

    <PcdsFixedAtBuild>
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
  }
!endif
