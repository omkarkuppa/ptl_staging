## @file
#  Platform description.
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

  #
  # Generic EDKII Driver
  #

  MdeModulePkg/Core/Pei/PeiMain.inf

  MdeModulePkg/Universal/PCD/Pei/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  MdeModulePkg/Universal/ReportStatusCodeRouter/Pei/ReportStatusCodeRouterPei.inf
  MdeModulePkg/Universal/StatusCodeHandler/Pei/StatusCodeHandlerPei.inf

  UefiCpuPkg/CpuIoPei/CpuIoPei.inf

  MdeModulePkg/Universal/FaultTolerantWritePei/FaultTolerantWritePei.inf

  !if gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable == TRUE
    ProtectedVariablePkg/Universal/ProtectedVariable/Pei/VariablePei.inf {
      <LibraryClasses>
        MbedTlsLib|CryptoPkg/Library/MbedTlsLib/MbedTlsLib.inf
        OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
        BaseCryptLib|CryptoPkg/Library/BaseCryptLibMbedTls/PeiCryptLib.inf
        TlsLib|CryptoPkg/Library/TlsLibNull/TlsLibNull.inf
        MemoryAllocationLib|ProtectedVariablePkg/Library/PeiMemoryAllocationLibProtectedVariable/PeiMemoryAllocationLibProtectedVariable.inf
    }
  !else
    MdeModulePkg/Universal/Variable/Pei/VariablePei.inf
  !endif

  MdeModulePkg/Universal/PcatSingleSegmentPciCfg2Pei/PcatSingleSegmentPciCfg2Pei.inf

  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  SecurityPkg/Tcg/Tcg2Config/Tcg2ConfigPei.inf
  SecurityPkg/Tcg/Tcg2Pei/Tcg2Pei.inf {
    <LibraryClasses>
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
  }
!endif

  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTablePei/FirmwarePerformancePei.inf
