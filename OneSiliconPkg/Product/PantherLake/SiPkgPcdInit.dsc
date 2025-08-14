## @file
#  Component description file for the SiPkg DSC file.
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
#@par Specification Reference:
#
##s

[PcdsFixedAtBuild]
  gSiPkgTokenSpaceGuid.PcdWclSupport                   |FALSE
  gSiPkgTokenSpaceGuid.PcdWhPSupport                   |FALSE

  gSiPkgTokenSpaceGuid.PcdFclwPtlSiSupport             |FALSE

  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress    |0xE0000000

  gSiPkgTokenSpaceGuid.PcdSmmVariableEnable            |TRUE
  gSiPkgTokenSpaceGuid.PcdHgEnable                     |TRUE
  gSiPkgTokenSpaceGuid.PcdAcpiEnable                   |TRUE
  gSiPkgTokenSpaceGuid.PcdSourceDebugEnable            |FALSE
  gSiPkgTokenSpaceGuid.PcdPpmEnable                    |TRUE
  gSiPkgTokenSpaceGuid.PcdTxtEnable                    |FALSE
  gSiPkgTokenSpaceGuid.PcdTseEnable                    |TRUE
  gSiPkgTokenSpaceGuid.PcdAmtEnable                    |FALSE
  gSiPkgTokenSpaceGuid.PcdSmbiosEnable                 |TRUE
  gSiPkgTokenSpaceGuid.PcdS3Enable                     |TRUE
  gSiPkgTokenSpaceGuid.PcdOverclockEnable              |FALSE
  gSiPkgTokenSpaceGuid.PcdCpuPowerOnConfigEnable       |TRUE
  gSiPkgTokenSpaceGuid.PcdBdatEnable                   |TRUE
  gSiPkgTokenSpaceGuid.PcdIgdEnable                    |TRUE
  gSiPkgTokenSpaceGuid.PcdDmiEnable                    |FALSE
  gSiPkgTokenSpaceGuid.PcdIpuEnable                    |TRUE
  gSiPkgTokenSpaceGuid.PcdVtdEnable                    |TRUE
  gSiPkgTokenSpaceGuid.PcdBiosGuardEnable              |TRUE  #BiosGuardModule.bin
  gSiPkgTokenSpaceGuid.PcdBiosGuardLegacyAcpi          |TRUE
  gSiPkgTokenSpaceGuid.PcdTdxEnable                    |FALSE
  gSiPkgTokenSpaceGuid.PcdOptimizeCompilerEnable       |TRUE
  gSiPkgTokenSpaceGuid.PcdPeiDisplayEnable             |TRUE
  gSiPkgTokenSpaceGuid.PcdNpuEnable                    |TRUE
  gSiPkgTokenSpaceGuid.PcdOcWdtEnable                  |TRUE
  gSiPkgTokenSpaceGuid.PcdBootGuardEnable              |TRUE
  gSiPkgTokenSpaceGuid.PcdTcssSupport                  |TRUE
  gSiPkgTokenSpaceGuid.PcdThcEnable                    |FALSE
  gSiPkgTokenSpaceGuid.PcdSpecialPoolEnable            |FALSE
  gSiPkgTokenSpaceGuid.PcdPsmiEnable                   |TRUE
  gSiPkgTokenSpaceGuid.PcdEmbeddedEnable               |0x0
!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1
  gSiPkgTokenSpaceGuid.PcdTsnSupport                   |0x0
  gSiPkgTokenSpaceGuid.PcdFusaSupport                  |0x1
!else
  gSiPkgTokenSpaceGuid.PcdTsnSupport                   |0x0
  gSiPkgTokenSpaceGuid.PcdFusaSupport                  |0x0
!endif
  gSiPkgTokenSpaceGuid.PcdCanSupport                   |0x0
  gSiPkgTokenSpaceGuid.PcdTccSupport                   |0x0
!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1
  gSiPkgTokenSpaceGuid.PcdTccSupport                   |0x1
!else
  gSiPkgTokenSpaceGuid.PcdTccSupport                   |0x0
!endif
  gSiPkgTokenSpaceGuid.PcdSiPciExpressBaseAddress      |gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress
  gSiPkgTokenSpaceGuid.PcdSocCoreBootEnable            |FALSE
  gSiPkgTokenSpaceGuid.PcdFspVEnable                   |FALSE
  gSiPkgTokenSpaceGuid.PcdDtprEnable                   |TRUE
  gSiPkgTokenSpaceGuid.PcdFspBinaryEnable              |FALSE
  gSiPkgTokenSpaceGuid.PcdVmdEnable                    |TRUE
  gSiPkgTokenSpaceGuid.PcdPcieIpsdEnabled              |TRUE
  gSiPkgTokenSpaceGuid.PcdFamilyBranch                 |0x02
  gSiPkgTokenSpaceGuid.PcdProgBranch                   |0x1
  gSiPkgTokenSpaceGuid.PcdBranchNestedLevel            |0x1
  gSiPkgTokenSpaceGuid.PcdSiliconSegment               |0x0
  gSiPkgTokenSpaceGuid.PcdFspSku                       |0x0
  gSiPkgTokenSpaceGuid.PcdFspYear                      |0x25
  gSiPkgTokenSpaceGuid.PcdFspBuildVer                  |0x3210
  gSiPkgTokenSpaceGuid.PcdSmbusBaseAddress             |0x0000EFA0
  gSiPkgTokenSpaceGuid.PcdSpiDmaEnable                 |TRUE
  gIntelSiliconPkgTokenSpaceGuid.PcdVTdPeiDmaBufferSize|0x00600000
  gIntelSiliconPkgTokenSpaceGuid.PcdVTdPeiDmaBufferSizeS3|0x00600000
  gSiPkgTokenSpaceGuid.PcdSafBarBaseAddress            |0xFA000000

  ## Specifies delay value in microseconds after sending out an INIT IPI.
  # @Prompt Configure delay value after send an INIT IPI
  gUefiCpuPkgTokenSpaceGuid.PcdCpuInitIpiDelayInMicroSeconds | 100

[PcdsDynamicExDefault]
  gSiPkgTokenSpaceGuid.PcdPciExpressRegionLength       |0x10000000
  ## Specifies the AP wait loop state during POST phase.
  #  The value is defined as below.
  #  1: Place AP in the Hlt-Loop state.
  #  2: Place AP in the Mwait-Loop state.
  #  3: Place AP in the Run-Loop state.
  # @Prompt The AP wait loop state.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApLoopMode           |2
  ## Specifies the AP target C-state for Mwait during POST phase.
  #  The default value 0 means C1 state.
  #  The value is defined as below.<BR><BR>
  # @Prompt The specified AP target C-state for Mwait.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApTargetCstate       |0

  gEfiMdeModulePkgTokenSpaceGuid.PcdPcieResizableBarSupport|FALSE

  ## Indicates processor trace is enabled on BSP only or not when CPU processor trace is enabled.<BR><BR>
  #  TRUE  - CPU processor trace is enabled on BSP only.<BR>
  #  FASLE - CPU processor trace is enabled on all CPU.<BR>
  # @Prompt Enable CPU processor trace only on BSP or all CPU.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuProcTraceBspOnly|FALSE

  ## Indicates if enable performance collecting when CPU processor trace is enabled.<BR><BR>
  #  CYC/TSC timing packets will be generated to collect performance data if this PCD is TRUE.<BR><BR>
  # @Prompt Enable performance collecting when processor trace is enabled.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuProcTracePerformanceCollecting|FALSE
