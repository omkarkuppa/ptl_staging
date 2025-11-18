## @file
#  This is a build description for the MemoryTelemetryFeaturePkg
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
##

[Defines]
  PLATFORM_NAME                  = MemoryTelemetryFeaturePkg
  PLATFORM_GUID                  = 31BAF7FE-AF81-413F-BE83-F703CEA924C2
  PLATFORM_VERSION               = 0.2
  DSC_SPECIFICATION              = 0x00010001
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  DXE_ARCH                       = X64

[PcdsFixedAtBuild]
  gMemoryTelemetryFeaturePkgTokenSpaceGuid.PcdMemoryTelemetryEnabled|TRUE
  gMemoryTelemetryFeaturePkgTokenSpaceGuid.PcdPprCapability|TRUE

[PcdsDynamicHii.common.DEFAULT.STANDARD]
  gMemoryTelemetryFeaturePkgTokenSpaceGuid.PcdAmtPprEnable|L"AmtPprEnable"|gMemTelAmtPprVariableGuid|0x00||NV, BS, RT

[LibraryClasses]
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_DRIVER]
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf

!include MemoryTelemetryFeaturePkg/Include/MemoryTelemetryFeaturePkg.dsc
