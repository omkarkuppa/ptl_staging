## @file
#  platform build option configuration file.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2020 Intel Corporation.
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
#  @par Specification
##

[LibraryClasses.X64]

  DxeAcpiGnvsInitLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeAcpiGnvsInitLib/DxeAcpiGnvsInitLib.inf
  DxeFirmwareVersionInfoLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeFirmwareVersionInfoLib/DxeFirmwareVersionInfoLib.inf
  DxeOpromPatchLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeOpromPatchLib/DxeOpromPatchLib.inf
  DxePolicyUpdateLib|$(PLATFORM_FULL_PACKAGE)/Library/DxePolicyUpdateLib/DxePolicyUpdateLib.inf
  SerialPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeSmmSerialPortParameterLib/DxeSmmSerialPortParameterLib.inf
  SpiFlashCommonLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeSpiFlashCommonLib/DxeSpiFlashCommonLib.inf


  FspWrapperPlatformLib|$(PLATFORM_FULL_PACKAGE)/FspWrapper/Library/DxeFspWrapperPlatformLib/DxeFspWrapperPlatformLib.inf

[LibraryClasses.X64.PEIM]

  SerialPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiSerialPortParameterLib/PeiSerialPortParameterLib.inf

[LibraryClasses.X64.DXE_CORE]

  SerialPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeCoreSerialPortParameterLib/DxeCoreSerialPortParameterLib.inf

!if $(TARGET) == DEBUG
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/DxeCoreSerialIoUartDebugPropertyLib.inf
!endif

[LibraryClasses.X64.DXE_SMM_DRIVER]

  DxeSmmScriptLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeSmmScriptLib/DxeSmmScriptLib.inf
  SpiFlashCommonLib|$(PLATFORM_FULL_PACKAGE)/Library/SmmSpiFlashCommonLib/SmmSpiFlashCommonLib.inf

!if $(TARGET) == DEBUG
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/DxeSmmSerialIoUartDebugPropertyLib.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdBiosGuardEnable == TRUE
  SpiFlashCommonLib|$(PLATFORM_FULL_PACKAGE)/Library/SmmSpiFlashCommonLib/SmmSpiFlashCommonLibBiosGuard.inf
!else
  SpiFlashCommonLib|$(PLATFORM_FULL_PACKAGE)/Library/SmmSpiFlashCommonLib/SmmSpiFlashCommonLib.inf
!endif

[LibraryClasses.X64.SMM_CORE]

!if $(TARGET) == DEBUG
  DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/DxeSmmSerialIoUartDebugPropertyLib.inf
!endif

[LibraryClasses.X64.DXE_RUNTIME_DRIVER]

  DebugPrintErrorLevelLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiDxeSmmDebugPrintErrorLevelLib/DxeSmmDebugPrintErrorLevelLib.inf

!if $(TARGET) == DEBUG
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/DxeSmmSerialIoUartDebugPropertyLib.inf
!endif

[LibraryClasses.X64.UEFI_DRIVER]

[LibraryClasses.X64.UEFI_APPLICATION]


