## @file
#  Component description file for the PantherLake CPU DXE FRU libraries.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2021 Intel Corporation.
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

#
# CPU
#
PtlCpuInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Library/PtlCpuInfoLib/DxePtlCpuInfoLib.inf

#################################################################
# Uncore Start #
#################################################################

#
# Graphics
#
DxeGraphicsPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainIGpu/LibraryPrivate/IGpuDxePolicyLib/IGpuDxePolicyLib.inf
DxeGraphicsInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainIGpu/LibraryPrivate/IGpuDxeInit/IGpuDxeInitLib.inf
DxeIgdOpRegionInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainIGpu/LibraryPrivate/IGpuDxeOpRegionInitLib/IGpuDxeOpRegionInit.inf

#
# VT-D
#
DxeVtdInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vtd/LibraryPrivate/DxeVtdInitLib/DxeVtdInitLibVer3.inf
DxeVtdInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Vtd/LibraryPrivate/DxeVtdInitLib/DxeVtdInitFruLib.inf

#
# Hybrid Graphics
#
!if gSiPkgTokenSpaceGuid.PcdHgEnable == TRUE
  DxeHybridGraphicsInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HybridGraphics/LibraryPrivate/DxeHybridGraphicsInitLib/DxeHybridGraphicsInitLib.inf
!else
  DxeHybridGraphicsInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HybridGraphics/LibraryPrivate/DxeHybridGraphicsInitLibNull/DxeHybridGraphicsInitLibNull.inf
!endif

#
#IPU
#
!if gSiPkgTokenSpaceGuid.PcdIpuEnable == TRUE
  DxeIpuInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ipu/LibraryPrivate/DxeIpuInitLib/DxeIpuInitLib.inf
!else
  DxeIpuInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ipu/LibraryPrivate/DxeIpuInitLibNull/DxeIpuInitLibNull.inf
!endif

#
# System Agent
#
  DxeSaInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/DxeSaInitFruLib/DxeSaInitFruLib.inf
  DxeSaNvsInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/DxeSaNvsInitLib/DxeSaNvsInitLib.inf
  DxeHostBridgeInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/LibraryPrivate/DxeHostBridgeInitLib/DxeHostBridgeInitLib.inf

#################################################################
# Uncore End #
#################################################################