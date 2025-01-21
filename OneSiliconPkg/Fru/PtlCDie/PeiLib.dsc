## @file
#  Component description file for the PantherLake CPU PEI FRU ibraries.
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
#  Tse IpBlock
#
!if gSiPkgTokenSpaceGuid.PcdTseEnable == TRUE
  PeiTseLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tse/LibraryPrivate/PeiTseLib/PeiTseLib.inf
!else
  PeiTseLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tse/LibraryPrivate/PeiTseLibNull/PeiTseLibNull.inf
!endif # gSiPkgTokenSpaceGuid.PcdTseEnable == TRUE

#
#  MkTme IpBlock
#
TmeLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tme/LibraryPrivate/MkTmeLib/MkTmeLib.inf

#
#  Cce IpBlock
#
CceLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Cce/LibraryPrivate/CceLibVer1/CceLibVer1.inf

#
#  TDx IpBlock
#
PeiTdxLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tdx/LibraryPrivate/PeiTdxLib/PeiTdxLib.inf

#
# Intel VR IpBlock and Fru Library
#
  PeiVrPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/VoltageRegulator/LibraryPrivate/PeiVrPolicyLib/PeiVrPolicyLib.inf
  PeiVrDomainLib|$(PLATFORM_SI_PACKAGE)/IpBlock/VoltageRegulator/Library/PeiVrDomainLib/PeiVrDomainLib.inf
  PeiVrLib|$(PLATFORM_SI_PACKAGE)/IpBlock/VoltageRegulator/LibraryPrivate/PeiVrLib/PeiVrLib.inf
  PeiVrFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/VoltageRegulator/LibraryPrivate/PeiVrFruLib/PeiVrFruLib.inf

#
# CPU init Fru Library
#
  PeiCpuInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiCpuInitFruLib/PeiCpuInitFruLib.inf

#################################################################
# Uncore Start #
#################################################################

#
# Graphics
#
  PeiGraphicsPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainIGpu/LibraryPrivate/IGpuPeiPolicyLibGen13/IGpuPeiPolicyLib.inf
  GraphicsInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Graphics/LibraryPrivate/PeiGraphicsInitFruLib/PeiGraphicsInitFruLib.inf

#
#  VTD IpBlock
#
!if gSiPkgTokenSpaceGuid.PcdVtdEnable == TRUE
  PeiVtdPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vtd/LibraryPrivate/PeiVtdPolicyLib/PeiVtdPolicyLibVer1.inf
  VtdInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vtd/LibraryPrivate/PeiVtdInitLib/PeiVtdInitLibVer3.inf
  PeiVtdInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Vtd/LibraryPrivate/PeiVtdInitLib/PeiVtdInitFruLib.inf
!else
  PeiVtdPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vtd/LibraryPrivate/PeiVtdPolicyLibNull/PeiVtdPolicyLibNull.inf
  VtdInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vtd/LibraryPrivate/PeiVtdInitLibNull/PeiVtdInitLibNull.inf
!endif

#
# Telemetry
#
  PeiTelemetryPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Telemetry/LibraryPrivate/PeiTelemetryPolicyLib/PeiTelemetryPolicyLib.inf
  PeiTelemetryFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Telemetry/LibraryPrivate/PeiTelemetryFruLib/PeiTelemetryFruLib.inf

#
# Hybrid Grapics IpBlock
#
!if gSiPkgTokenSpaceGuid.PcdHgEnable == TRUE
  PeiHybridGraphicsPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HybridGraphics/LibraryPrivate/PeiHybridGraphicsPolicyLib/PeiHybridGraphicsPolicyLib.inf
!else
  PeiHybridGraphicsPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HybridGraphics/LibraryPrivate/PeiHybridGraphicsPolicyLibNull/PeiHybridGraphicsPolicyLibNull.inf
!endif

#
# IPU
#
!if gSiPkgTokenSpaceGuid.PcdIpuEnable == TRUE
  PeiIpuPolicyPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ipu/LibraryPrivate/PeiIpuPolicyPrivateLib/PeiIpuPolicyPrivateLib.inf
  IpuInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ipu/LibraryPrivate/PeiIpuInitPrivateLib/PeiIpuInitLib.inf
  IpuInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Ipu/LibraryPrivate/PeiIpuInitFruLib/PeiIpuInitFruLib.inf
!else
  PeiIpuPolicyPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ipu/LibraryPrivate/PeiIpuPolicyPrivateLibNull/PeiIpuPolicyPrivateLibNull.inf
  IpuInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ipu/LibraryPrivate/PeiIpuInitPrivateLibNull/PeiIpuInitLibNull.inf
!endif

#
# PSMI
#
!if gSiPkgTokenSpaceGuid.PcdPsmiEnable == TRUE
  PeiPsmiInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Psmi/LibraryPrivate/PeiPsmiInitLib/PeiPsmiInitLib.inf
  PeiPsmiInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiPsmiInitFruLib/PeiPsmiInitFruLib.inf
!else
  PeiPsmiInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Psmi/LibraryPrivate/PeiPsmiInitLibNull/PeiPsmiInitLibNull.inf
  PeiPsmiInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiPsmiInitFruLibNull/PeiPsmiInitFruLibNull.inf
!endif

#
# NPU
#
!if gSiPkgTokenSpaceGuid.PcdNpuEnable == TRUE
  PeiNpuPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Npu/LibraryPrivate/PeiNpuPolicyLib/PeiNpuPolicyLib.inf
  NpuInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Npu/LibraryPrivate/PeiNpuInitLib/PeiNpuInitLib.inf
!else
  PeiNpuPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Npu/LibraryPrivate/PeiNpuPolicyLibNull/PeiNpuPolicyLibNull.inf
  NpuInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Npu/LibraryPrivate/PeiNpuInitLibNull/PeiNpuInitLibNull.inf
!endif

#
# NOC
#
  NocInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Noc/LibraryPrivate/PeiNocInitLib/PeiNocInitLib.inf
  ImrInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiImrInitLib/PeiImrInitLib.inf
  TprInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Noc/LibraryPrivate/PeiTprInitLib/PeiTprInitLib.inf

#
# Host Bridge
#
  PeiHostBridgeInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/LibraryPrivate/PeiHostBridgeInitLib/PeiHostBridgeInitLib.inf
  PeiHostBridgePolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/LibraryPrivate/PeiHostBridgePolicyLib/PeiHostBridgePolicyLibVer3.inf
  PeiHostBridgeInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiHostBridgeInitFruLib/PeiHostBridgeInitFruLib.inf
  PeiMemoryMapPrintLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiMemoryMapPrintLib/PeiMemoryMapPrintLib.inf

#
# System Agent
#
  PeiSaInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiSaInitFruLib/PeiSaInitFruLib.inf
  PeiMemPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/MemoryInit/Library/PeiMemPolicyLib/PeiMemPolicyLibFspPtl.inf

#################################################################
# Uncore End #
#################################################################

#
#  BiosGuard IpBlock
#
!if gSiPkgTokenSpaceGuid.PcdBiosGuardEnable == TRUE
  BiosGuardLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BiosGuard/LibraryPrivate/PeiBiosGuardLib/PeiBiosGuardLib.inf
  PeiBiosGuardPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BiosGuard/LibraryPrivate/PeiBiosGuardPolicyLib/PeiBiosGuardPolicyLib.inf
  PeiBiosGuardInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiBiosGuardInitFruLib/PeiBiosGuardInitFruLib.inf
!else
  BiosGuardLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BiosGuard/LibraryPrivate/PeiBiosGuardLibNull/PeiBiosGuardLibNull.inf
  PeiBiosGuardPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BiosGuard/LibraryPrivate/PeiBiosGuardPolicyLibNull/PeiBiosGuardPolicyLibNull.inf
!endif

#
#  Intel Overclocking IpBlock and Fru Library
#
  PeiOcPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiOcPolicyLib/PeiOcPolicyLib.inf
!if gSiPkgTokenSpaceGuid.PcdOverclockEnable == TRUE
  PeiOcLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiOcLib/PeiOcLib.inf
  PeiOcInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiOcInitLib/PeiOcInitLib.inf
!else
  PeiOcLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiOcLibNull/PeiOcLibNull.inf
  PeiOcInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiOcInitLibNull/PeiOcInitLibNull.inf
!endif
