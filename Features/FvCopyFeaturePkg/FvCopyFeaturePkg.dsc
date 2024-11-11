## @file
#  Description (DSC) file for advanced feature package - Firmware Volume Copy Feature Pkg.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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
#  @par Specification Reference:
#
##

[Defines]
  PLATFORM_NAME           = FvCopyFeaturePkg
  PLATFORM_GUID           = 3CC94172-4FE2-4D1B-A389-337D7CFCDACF
  PLATFORM_VERSION        = 0.1
  DSC_SPECIFICATION       = 0x0001001C
  OUTPUT_DIRECTORY        = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES = IA32|X64
  BUILD_TARGETS           = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER        = DEFAULT

[Packages]


[LibraryClasses]
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

[LibraryClasses.common]
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf

[LibraryClasses.common.PEIM]
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf

[PcdsFixedAtBuild]
  gFvCopyPkgTokenSpaceGuid.PcdFreeSpaceSkipFvCopyEnable|FALSE

  #
  # This package always builds the needed modules.
  #
  !include FvCopyFeaturePkg/Include/FvCopyFeature.dsc
