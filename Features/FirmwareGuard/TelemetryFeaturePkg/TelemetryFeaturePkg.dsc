## @file
# This is a build description for the Telemetry feature.
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
##

[Defines]
  PLATFORM_NAME                  = TelemetryFeaturePkg
  PLATFORM_GUID                  = FBB879B5-7678-46E3-942B-EAB6CD92AC54
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = $(FSP_ARCH)|X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  PEI_ARCH                       = $(FSP_ARCH)
  DXE_ARCH                       = X64

[LibraryClasses]
  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf

  #
  # Entry point
  #
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf

  #
  # Basic
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf

  #
  # Misc
  #
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
  PostCodeLib|MdePkg/Library/BasePostCodeLibDebug/BasePostCodeLibDebug.inf
  SmbusLib|MdePkg/Library/BaseSmbusLibNull/BaseSmbusLibNull.inf
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxDxeLib.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf

  #
  # Generic Modules
  #
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf

  #
  # Silicon
  #
  MeInfoLib|OneSiliconPkg/IpBlock/Me/Library/MeInfoLibNull/MeInfoLibNull.inf
  VmdInfoLib|TelemetryFeaturePkg/Library/VmdInfoLibNull/VmdInfoLibNull.inf
  PmcLib|OneSiliconPkg/IpBlock/Pmc/Library/PeiDxeSmmPmcLib/PeiDxeSmmPmcLib.inf

  #
  # Telemetry
  #
  TelemetryFirmwareVersionHookLib|TelemetryFeaturePkg/Library/TelemetryFirmwareVersionHookLibNull/TelemetryFirmwareVersionHookLibNull.inf

[LibraryClasses.common]
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  AslUpdateLib|MinPlatformPkg/Acpi/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf

[LibraryClasses.common.PEIM]
  #
  # PEI phase common
  #
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf

[Components]
  !include Include/TelemetryFeature.dsc
