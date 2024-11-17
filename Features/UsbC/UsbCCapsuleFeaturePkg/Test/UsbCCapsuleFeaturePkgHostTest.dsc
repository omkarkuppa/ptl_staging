## @file
#  UsbCCapsuleFeaturePkgHostTest DSC file used to build host-based unit tests.
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

[Defines]
  PLATFORM_NAME           = UsbCCapsuleFeaturePkgHostTest.dsc
  PLATFORM_GUID           = AF157F0B-FA23-4B40-A102-1CB73C5A5783
  PLATFORM_VERSION        = 0.1
  DSC_SPECIFICATION       = 0x0001001C
  OUTPUT_DIRECTORY        = Build/UsbCCapsuleFeaturePkg/HostTest
  SUPPORTED_ARCHITECTURES = IA32|X64
  BUILD_TARGETS           = NOOPT
  SKUID_IDENTIFIER        = DEFAULT

#
# Include UnitTestFrameworkPkgHost.dsc.inc
#
!include UnitTestFrameworkPkg/UnitTestFrameworkPkgHost.dsc.inc

[LibraryClasses]
  UefiRuntimeServicesTableLib|MdePkg/Test/Mock/Library/GoogleTest/MockUefiRuntimeServicesTableLib/MockUefiRuntimeServicesTableLib.inf
  TimerLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockTimerLib/MockTimerLib.inf
  UsbcCapsuleDebugLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockUsbcCapsuleDebugLib/MockUsbcCapsuleDebugLib.inf
  PcdLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePcdLib/MockBasePcdLib.inf
  Usb4DebugLib|Usb4FeaturePkg/Library/Usb4Cm/Usb4DebugLib/Usb4DebugLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  CmUtilsLib|Usb4FeaturePkg/LibraryPrivate/Usb4Cm/CmUtilsLib/CmUtilsLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  PostCodeLib|MdePkg/Library/BasePostCodeLibPort80/BasePostCodeLibPort80.inf
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf

[Components]
  #
  # Build UsbCCapsuleFeaturePkgHostTest HOST_APPLICATION Tests
  #

  UsbCCapsuleFeaturePkg/Library/FmpDeviceLib/DiscreteTbt/GoogleTest/GTestFmpDeviceLibDiscreteTbt.inf {
    <LibraryClasses>
      # TbtNvmRetimerUpdateLib package
      TbtNvmRetimerUpdateLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockTbtNvmRetimerUpdateLib/MockTbtNvmRetimerUpdateLib.inf
      # Target file for Unit Test
      NULL|UsbCCapsuleFeaturePkg/Library/FmpDeviceLib/DiscreteTbt/FmpDeviceLibDiscreteTbt.inf
      # Mock Library
      UefiBootServicesTableLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockUefiBootServicesTableLib/MockUefiBootServicesTableLib.inf
      UsbcRetimerProtocol|UsbCCapsuleFeaturePkg/Test/Mock/Include/GoogleTest/Private/MockUsbcRetimerProtocol/MockUsbcRetimerProtocol.inf
      FirmwareManagementLib|UsbCCapsuleFeaturePkg/Test/Mock/Include/GoogleTest/Private/MockFirmwareManagementLib/MockFirmwareManagementLib.inf
      PciIopProtocolLib|UsbCCapsuleFeaturePkg/Test/Mock/Include/GoogleTest/Private/MockPciIopProtocolLib/MockPciIopProtocolLib.inf
      MockUsbCProgressCodeProtocolLib|UsbCCapsuleFeaturePkg/Test/Mock/Include/GoogleTest/Private/MockUsbCProgressCodeProtocol/MockUsbCProgressCodeProtocol.inf
  }
  UsbCCapsuleFeaturePkg/Library/FmpDeviceLib/UsbCRetimer/GoogleTest/GTestFmpDeviceLibUsbCRetimer.inf {
    <LibraryClasses>
      # TbtNvmRetimerUpdateLib package
      TbtNvmRetimerUpdateLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockTbtNvmRetimerUpdateLib/MockTbtNvmRetimerUpdateLib.inf
      # Target file for Unit Test
      NULL|UsbCCapsuleFeaturePkg/Library/FmpDeviceLib/UsbCRetimer/FmpDeviceLibTbtRetimer.inf
      # Mock Library
      UefiBootServicesTableLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockUefiBootServicesTableLib/MockUefiBootServicesTableLib.inf
      UsbcRetimerProtocol|UsbCCapsuleFeaturePkg/Test/Mock/Include/GoogleTest/Private/MockUsbcRetimerProtocol/MockUsbcRetimerProtocol.inf
      FirmwareManagementLib|UsbCCapsuleFeaturePkg/Test/Mock/Include/GoogleTest/Private/MockFirmwareManagementLib/MockFirmwareManagementLib.inf
      MockUsbCProgressCodeProtocolLib|UsbCCapsuleFeaturePkg/Test/Mock/Include/GoogleTest/Private/MockUsbCProgressCodeProtocol/MockUsbCProgressCodeProtocol.inf
  }
