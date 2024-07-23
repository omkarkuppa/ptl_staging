## @file
# I2cFeaturePkgHostTest DSC include file for host based test DSC
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2022 Intel Corporation.
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
#@par Specification Reference:
#
##

[Defines]
  PLATFORM_NAME                  = I2cFeaturePkgHostTest.dsc
  PLATFORM_GUID                  = bb59119d-ddd3-4f71-b638-c7ff1855a5c2
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x0001001C
  OUTPUT_DIRECTORY               = Build/I2cFeaturePkg/HostTest
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = NOOPT
  SKUID_IDENTIFIER               = DEFAULT

  #
  # Include UnitTestFrameworkPkgHost.dsc.inc
  #
!include UnitTestFrameworkPkg/UnitTestFrameworkPkgHost.dsc.inc

[LibraryClasses]
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/UnitTestHostBaseCryptLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf

[Components]
  #
  # Build Unit test Components with HOST_APPLICATION.
  #
  I2cFeaturePkg/I2cPlatformDxe/UnitTest/I2cPlatformDxeHost.inf {
    <LibraryClasses>
    PcdLib|I2cFeaturePkg/Test/CommonMock/PcdLibMock.inf
  }
  I2cFeaturePkg/I2cPssMonzaDxe/UnitTest/I2cMonzaDxeHost.inf
  I2cFeaturePkg/I2cTouchPanelDxe/UnitTest/I2cTouchPanelDxeHost.inf
  I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchCommon/UnitTest/I2cCommonHost.inf {
    <LibraryClasses>
    PcdLib|I2cFeaturePkg/Test/CommonMock/PcdLibMock.inf
  }
  I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchDevAcpi/UnitTest/I2cTouchDevHost.inf {
    <LibraryClasses>
    PcdLib|I2cFeaturePkg/Test/CommonMock/PcdLibMock.inf
  }
  I2cFeaturePkg/I2cSensorPei/UnitTest/I2cSensorPeiHost.inf{
    <LibraryClasses>
    I2cSensorPeiLib|I2cFeaturePkg/Library/I2cSensorPeiLib/I2cSensorPeiLib.inf
    PcdLib|I2cFeaturePkg/Test/CommonMock/PcdLibMock.inf
  }

  #
  # Generate aml file
  #
  I2cFeaturePkg/I2cPowerMeter/I2cPowerMeterAcpiTable.inf
  #
  # Due to this testcase made CI random failure, so disabled PowerMeter testcase for short term solution. 
  #
  # I2cFeaturePkg/I2cPowerMeter/UnitTest/I2cPowerMeterHost.inf