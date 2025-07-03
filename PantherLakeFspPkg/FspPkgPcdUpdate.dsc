## @file
#  FSP description file updates configuration (PCD) settings by if-statement for this project.
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
# @par Specification
##

#
# Override some PCDs for specific build requirements.
#
[PcdsFixedAtBuild]


!if gSiPkgTokenSpaceGuid.PcdSmmVariableEnable == FALSE
  gSiPkgTokenSpaceGuid.PcdBiosGuardEnable|FALSE
!endif
!if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
  gSiPkgTokenSpaceGuid.PcdMrcTraceMessageSupported|FALSE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x02
!else
  gSiPkgTokenSpaceGuid.PcdMrcTraceMessageSupported|TRUE
!endif
!if $(TARGET) == DEBUG
  gSiPkgTokenSpaceGuid.PcdLpssUartDebugEnable|1
  gSiPkgTokenSpaceGuid.PcdLpssUartNumber|0
  gPantherLakeFspPkgTokenSpaceGuid.PcdMonoStatusCode|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|7
!else
  gSiPkgTokenSpaceGuid.PcdLpssUartDebugEnable|0
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|3
  gSiPkgTokenSpaceGuid.PcdTraceHubCatalogEnable|TRUE
!endif
!if gPantherLakeFspPkgTokenSpaceGuid.PcdFspPerformanceEnable == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|140
!endif
!if $(FSP64_BUILD) == TRUE
  gPantherLakeFspPkgTokenSpaceGuid.PcdFspImageAttributes|0x0017
!else
  gPantherLakeFspPkgTokenSpaceGuid.PcdFspImageAttributes|0x0013
!endif
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel|0x80000000      # This PCD gets overriden in prep.bat based on 'CATALOG_DEBUG_PRINT_LEVEL' ENV variable.
  gSiPkgTokenSpaceGuid.PcdTraceHubCatalogEnable|TRUE
!endif
  gSiPkgTokenSpaceGuid.PcdDmiEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdOverclockEnable|FALSE

#
# Fusa Ip Blocks Enabled
#
!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1
  gSiPkgTokenSpaceGuid.PcdFusaSupport|TRUE
!endif

#
# 14 MB BIOS Flash Base Address and Size
#
  gSiPkgTokenSpaceGuid.PcdBiosAreaBaseAddress|0xFF200000
  gSiPkgTokenSpaceGuid.PcdBiosSize|0x0E00000


[PcdsPatchableInModule]
!if $(TARGET) == DEBUG
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000042
!else
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000000
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x00000001
!endif
!endif
