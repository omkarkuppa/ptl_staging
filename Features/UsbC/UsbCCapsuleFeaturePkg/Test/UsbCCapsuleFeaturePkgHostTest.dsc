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
  MockUefiBootServicesTableLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockUefiBootServicesTableLib/MockUefiBootServicesTableLib.inf
  TbtNvmRetimerUpdateLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockTbtNvmRetimerUpdateLib/MockTbtNvmRetimerUpdateLib.inf
  TimerLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockTimerLib/MockTimerLib.inf
  
[Components]
  #
  # Build UsbCCapsuleFeaturePkgHostTest HOST_APPLICATION Tests
  #

  UsbCCapsuleFeaturePkg/Library/FmpDeviceLib/DiscreteTbt/GoogleTest/GTestFmpDeviceLibDiscreteTbt.inf {
    <LibraryClasses>
      NULL|UsbCCapsuleFeaturePkg/Library/FmpDeviceLib/DiscreteTbt/FmpDeviceLibDiscreteTbt.inf
  }
  UsbCCapsuleFeaturePkg/Library/FmpDeviceLib/UsbCRetimer/GoogleTest/GTestFmpDeviceLibUsbCRetimer.inf {
    <LibraryClasses>
      NULL|UsbCCapsuleFeaturePkg/Library/FmpDeviceLib/UsbCRetimer/FmpDeviceLibTbtRetimer.inf
  }

  UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/GoogleTest/TbtNvmDrvHr/GTestTbtNvmDrvHr.inf {
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2E
    <LibraryClasses>
      UefiBootServicesTableLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockUefiBootServicesTableLib/MockUefiBootServicesTableLib.inf
      MockHobLib|MdePkg/Test/Mock/Library/GoogleTest/MockHobLib/MockHobLib.inf
      PchPciBdfLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePchPciBdfLibNull/MockBasePchPciBdfLibNull.inf
      PciSegmentLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciSegmentLibPciNull/MockPciSegmentLibNull.inf
      PciLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciLibPciExpressNull/MockBasePciLibPciExpressNull.inf
      # Target file for Unit Test
      NULL|UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/TbtNvmRetimerUpdateLib.inf
  }

  UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/GoogleTest/TbtNvmDrvRetimerThruHr/GTestTbtNvmDrvRetimerThruHr.inf {
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2E
    <LibraryClasses>
      UefiBootServicesTableLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockUefiBootServicesTableLib/MockUefiBootServicesTableLib.inf
      PchPciBdfLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePchPciBdfLibNull/MockBasePchPciBdfLibNull.inf
      PciSegmentLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciSegmentLibPciNull/MockPciSegmentLibNull.inf
      PciLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciLibPciExpressNull/MockBasePciLibPciExpressNull.inf
      # Target file for Unit Test
      NULL|UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/TbtNvmRetimerUpdateLib.inf
  }

  UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/GoogleTest/TbtNvmDrvRetimerThruHrHelpers/GTestTbtNvmDrvRetimerThruHrHelpers.inf {
    <LibraryClasses>
      UefiBootServicesTableLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockUefiBootServicesTableLib/MockUefiBootServicesTableLib.inf
      PchPciBdfLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePchPciBdfLibNull/MockBasePchPciBdfLibNull.inf
      PciSegmentLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciSegmentLibPciNull/MockPciSegmentLibNull.inf
      PciLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciLibPciExpressNull/MockBasePciLibPciExpressNull.inf
      # Target file for Unit Test
      NULL|UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/TbtNvmRetimerUpdateLib.inf
  }

  UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/GoogleTest/TbtNvmDrvYflRouter/GTestTbtNvmDrvYflRouter.inf {
    <LibraryClasses>
      # Mock Library
      UefiBootServicesTableLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockUefiBootServicesTableLib/MockUefiBootServicesTableLib.inf
      PchPciBdfLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePchPciBdfLibNull/MockBasePchPciBdfLibNull.inf
      PciSegmentLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciSegmentLibPciNull/MockPciSegmentLibNull.inf
      PciLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciLibPciExpressNull/MockBasePciLibPciExpressNull.inf
      # Target file for Unit Test
      NULL|UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/TbtNvmRetimerUpdateLib.inf
  }

  UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/GoogleTest/TbtNvmRetimerDrvHelpers/GTestTbtNvmRetimerDrvHelpers.inf {
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2E
    <LibraryClasses>
      UefiBootServicesTableLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockUefiBootServicesTableLib/MockUefiBootServicesTableLib.inf
      PchPciBdfLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePchPciBdfLibNull/MockBasePchPciBdfLibNull.inf
      PciSegmentLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciSegmentLibPciNull/MockPciSegmentLibNull.inf
      PciLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciLibPciExpressNull/MockBasePciLibPciExpressNull.inf
      # Target file for Unit Test
      NULL|UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/TbtNvmRetimerUpdateLib.inf
  }

  UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/GoogleTest/TbtRetimerNvmUpdateLib/GTestTbtRetimerNvmUpdateLib.inf {
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2E
    <LibraryClasses>
      # Mock Library
      UefiBootServicesTableLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockUefiBootServicesTableLib/MockUefiBootServicesTableLib.inf
      PcdLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePcdLib/MockBasePcdLib.inf
      UefiRuntimeServicesTableLib|MdePkg/Test/Mock/Library/GoogleTest/MockUefiRuntimeServicesTableLib/MockUefiRuntimeServicesTableLib.inf
      PchPciBdfLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePchPciBdfLibNull/MockBasePchPciBdfLibNull.inf
      PciSegmentLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciSegmentLibPciNull/MockPciSegmentLibNull.inf
      PciLib|UsbCCapsuleFeaturePkg/Test/Mock/Library/GoogleTest/MockBasePciLibPciExpressNull/MockBasePciLibPciExpressNull.inf
      # Target file for Unit Test
      NULL|UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/TbtNvmRetimerUpdateLib.inf
  }