## @file
#  Platform description.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2017 Intel Corporation.
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

  #
  # Generic EDKII Driver
  #
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }

  #
  # Network Support
  #
!if gPlatformModuleTokenSpaceGuid.PcdNetworkEnable == TRUE
  NetworkPkg/WifiConnectionManagerDxe/WifiConnectionManagerDxe.inf
  SecurityPkg/Hash2DxeCrypto/Hash2DxeCrypto.inf
!endif # gPlatformModuleTokenSpaceGuid.PcdNetworkEnable

!if gI2cFeaturePkgTokenSpaceGuid.PcdI2cFeatureEnable == TRUE
  MdeModulePkg/Bus/I2c/I2cDxe/I2cDxe.inf
!endif


  #
  # S3 Support
  #
!if gSiPkgTokenSpaceGuid.PcdS3Enable == TRUE
  UefiCpuPkg/CpuS3DataDxe/CpuS3DataDxe.inf
!endif
!if gSiPkgTokenSpaceGuid.PcdS3Enable == TRUE
  MdeModulePkg/Universal/LockBox/SmmLockBox/SmmLockBox.inf
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationSmm.inf
  MdeModulePkg/Universal/Acpi/S3SaveStateDxe/S3SaveStateDxe.inf
  MdeModulePkg/Universal/Acpi/SmmS3SaveState/SmmS3SaveState.inf
  MdeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf {
    <LibraryClasses>
    !if $(TARGET) == DEBUG
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
      TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/BaseTraceHubDebugSysTLib.inf
      TraceHubHookLib|$(SILICON_PRODUCT_PATH)/Library/TraceHubHookLib/BaseTraceHubHookLib.inf
    !endif
    !if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
      TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/BaseTraceHubDebugSysTLib.inf
      TraceHubHookLib|$(SILICON_PRODUCT_PATH)/Library/TraceHubHookLib/BaseTraceHubHookLib.inf
    !endif
  }
!endif # gSiPkgTokenSpaceGuid.PcdS3Enable

  #
  # SMM Support
  #
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  MdeModulePkg/Core/PiSmmCore/PiSmmIpl.inf
  MdeModulePkg/Core/PiSmmCore/PiSmmCore.inf

  MdeModulePkg/Universal/ReportStatusCodeRouter/Smm/ReportStatusCodeRouterSmm.inf
  MdeModulePkg/Universal/StatusCodeHandler/Smm/StatusCodeHandlerSmm.inf {
    <LibraryClasses>
    !if $(TARGET) == DEBUG
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
    !else
    !if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
    !endif
    !endif
    !if gPlatformModuleTokenSpaceGuid.PcdBeepStatusCodeEnable == TRUE
      NULL|BeepDebugFeaturePkg/Library/BeepStatusCodeHandlerLib/SmmBeepStatusCodeHandlerLib.inf
    !endif
    !if gPlatformModuleTokenSpaceGuid.PcdPostCodeStatusCodeEnable == TRUE
      NULL|PostCodeDebugFeaturePkg/Library/PostCodeStatusCodeHandlerLib/SmmPostCodeStatusCodeHandlerLib.inf
    !endif
      NULL|$(PLATFORM_SI_PACKAGE)/Library/TraceHubStatusCodeHandlerLib/SmmTraceHubStatusCodeHandlerLib.inf
  }

!if gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit == FALSE
  UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf {
    <LibraryClasses>
      DgrSmmLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dgr/LibraryPrivate/DgrSmmLib/DgrSmmLib.inf
      SmmCpuFeaturesLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/Library/SmmCpuFeaturesLib/SmmCpuFeaturesLib.inf
      MmSaveStateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/Library/MmSaveStateLib/DxeMmSaveStateLib.inf
      SmmCpuSyncLib|UefiCpuPkg/Library/SmmCpuSyncLib/SmmCpuSyncLib.inf
  }
!endif #PcdStopAfterMemInit

  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableSmm/FirmwarePerformanceSmm.inf {
    <LibraryClasses>
    !if $(TARGET) == DEBUG
      # It can't use PeiDxeDebugLibReportStatusCode as DebugLib, otherwise some FPDT log will not shown.
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
    !endif
    !if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
    !endif
  }

  UefiCpuPkg/CpuIo2Smm/CpuIo2Smm.inf
  MdeModulePkg/Universal/SmmCommunicationBufferDxe/SmmCommunicationBufferDxe.inf
!endif #PcdBootToShellOnly

  #
  # Variable Support
  #
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteSmm.inf

  !if gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable == TRUE
    ProtectedVariablePkg/Universal/ProtectedVariable/RuntimeDxe/VariableSmmRuntimeDxe.inf

    ProtectedVariablePkg/Universal/ProtectedVariable/RuntimeDxe/VariableSmm.inf {
      <LibraryClasses>
        VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf
        NULL|MdeModulePkg/Library/VarCheckPolicyLib/VarCheckPolicyLib.inf
        NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
        NULL|MdeModulePkg/Library/VarCheckHiiLib/VarCheckHiiLib.inf
        NULL|$(PLATFORM_FULL_PACKAGE)/Library/PlatformVarCheckLib/PlatformVarCheckLib.inf
        !if gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable == TRUE
          PlatformSecureLib|$(PLATFORM_FULL_PACKAGE)/Library/PlatformSecureLibTest/PlatformSecureLibTest.inf
          AuthVariableLib|SecurityPkg/Library/AuthVariableLib/AuthVariableLib.inf
        !else
          PlatformSecureLib|SecurityPkg/Library/PlatformSecureLibNull/PlatformSecureLibNull.inf
          AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
        !endif
    }
  !else
    MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmmRuntimeDxe.inf

    MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmm.inf {
      <LibraryClasses>
        VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf
        NULL|MdeModulePkg/Library/VarCheckPolicyLib/VarCheckPolicyLib.inf
        NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
        NULL|MdeModulePkg/Library/VarCheckHiiLib/VarCheckHiiLib.inf
        NULL|$(PLATFORM_FULL_PACKAGE)/Library/PlatformVarCheckLib/PlatformVarCheckLib.inf
        !if gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable == TRUE
          PlatformSecureLib|$(PLATFORM_FULL_PACKAGE)/Library/PlatformSecureLibTest/PlatformSecureLibTest.inf
          AuthVariableLib|SecurityPkg/Library/AuthVariableLib/AuthVariableLib.inf
        !else
          PlatformSecureLib|SecurityPkg/Library/PlatformSecureLibNull/PlatformSecureLibNull.inf
          AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
        !endif
    }
  !endif
!else
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf {
    <LibraryClasses>
      !if gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable == TRUE
        PlatformSecureLib|$(PLATFORM_FULL_PACKAGE)/Library/PlatformSecureLibTest/PlatformSecureLibTest.inf
        AuthVariableLib|SecurityPkg/Library/AuthVariableLib/AuthVariableLib.inf
      !else
        PlatformSecureLib|SecurityPkg/Library/PlatformSecureLibNull/PlatformSecureLibNull.inf
        AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
      !endif
    <PcdsFixedAtBuild>
      gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvModeEnable|TRUE
  }
!endif
  MdeModulePkg/Bus/Pci/PciSioSerialDxe/PciSioSerialDxe.inf
  MdeModulePkg/Universal/DriverHealthManagerDxe/DriverHealthManagerDxe.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf {
    <LibraryClasses>
    !if gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable == TRUE
      NULL|SecurityPkg/Library/DxeImageVerificationLib/DxeImageVerificationLib.inf
    !endif
    !if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
      NULL|SecurityPkg/Library/DxeTpm2MeasureBootLib/DxeTpm2MeasureBootLib.inf
    !endif
  }

  MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf


!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable == TRUE
  SecurityPkg/VariableAuthenticated/SecureBootConfigDxe/SecureBootConfigDxe.inf {
    <LibraryClasses>
      PlatformSecureLib|$(PLATFORM_FULL_PACKAGE)/Library/PlatformSecureLibTest/PlatformSecureLibTest.inf
      FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
    <BuildOptions>
      #
      # Specify GUID gEfiIfrBootMaintenanceGuid, to install Secure Boot Configuration menu
      # into Boot Maintenance Manager menu
      #
      *_*_*_VFR_FLAGS   = -g b2dedc91-d59f-48d2-898a-12490c74a4e0
  }
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  SecurityPkg/Tcg/MemoryOverwriteControl/TcgMor.inf
  SecurityPkg/Tcg/Tcg2Dxe/Tcg2Dxe.inf {
    <LibraryClasses>
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterDxe.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
  }
  SecurityPkg/Tcg/Tcg2Smm/Tcg2Smm.inf
  SecurityPkg/Tcg/Tcg2Acpi/Tcg2Acpi.inf
  SecurityPkg/Tcg/Tcg2Config/Tcg2ConfigDxe.inf
!endif

!if (gSiPkgTokenSpaceGuid.PcdAtaEnable == TRUE) OR (gNvmeFeaturePkgTokenSpaceGuid.PcdNvmeFeatureEnable == TRUE)
!if gPlatformModuleTokenSpaceGuid.PcdOpalPasswordEnable == TRUE
  SecurityPkg/Tcg/Opal/OpalPassword/OpalPasswordDxe.inf
!endif
!endif

!if gPlatformModuleTokenSpaceGuid.PcdCryptoEnable == TRUE
  SecurityPkg/RandomNumberGenerator/RngDxe/RngDxe.inf {
    <LibraryClasses>
      RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  }
!endif

!if gPlatformModuleTokenSpaceGuid.PcdTpmEnable == TRUE
!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  SecurityPkg/Tcg/Tcg2Dxe/Tcg2Dxe.inf {
    <LibraryClasses>
      Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterDxe.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      !if gPlatformModuleTokenSpaceGuid.PcdDeprecatedCryptoRemove == FALSE
        NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      !endif
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha384/HashInstanceLibSha384.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha512/HashInstanceLibSha512.inf
      NULL|SecurityPkg/Library/HashInstanceLibSm3/HashInstanceLibSm3.inf
      !if $(TARGET) == DEBUG
        DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
        SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/DxeSmmSerialIoUartDebugPropertyLib.inf
      !endif
      !if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
        DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
      !endif
  }
  SecurityPkg/Tcg/Tcg2Config/Tcg2ConfigDxe.inf {
    <LibraryClasses>
      Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterDxe.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
    <BuildOptions>
  }
!endif # gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable

  MdeModulePkg/Universal/SmbiosMeasurementDxe/SmbiosMeasurementDxe.inf
  UefiCpuPkg/MicrocodeMeasurementDxe/MicrocodeMeasurementDxe.inf
!endif # gPlatformModuleTokenSpaceGuid.PcdTpmEnable

!if gSiPkgTokenSpaceGuid.PcdSmmVariableEnable == TRUE
!if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf
!endif
!else
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf
!endif

!if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
  # capsule related drivers and Application.
  MdeModulePkg/Universal/EsrtFmpDxe/EsrtFmpDxe.inf
  MdeModulePkg/Application/CapsuleApp/CapsuleApp.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
!endif

!if gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdOneClickRecoveryEnable == TRUE
  MdeModulePkg/Universal/Disk/RamDiskDxe/RamDiskDxe.inf
!endif
