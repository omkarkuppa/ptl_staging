## @file
#  Component description file for the PantherLake CPU Common FRU libraries.
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

  MsrFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/BaseMsrFruLib/BaseMsrFruLib.inf
  CpuInfoFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/BaseCpuInfoFruLib/BaseCpuInfoFruLib.inf

!if gSiPkgTokenSpaceGuid.PcdTseEnable == TRUE
  TseInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tse/Library/PeiDxeSmmTseInfoLib/PeiDxeSmmTseInfoLib.inf
!else
  TseInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tse/Library/PeiDxeSmmTseInfoNullLib/PeiDxeSmmTseInfoNullLib.inf
!endif # gSiPkgTokenSpaceGuid.PcdTseEnable == TRUE

!if gSiPkgTokenSpaceGuid.PcdTdxEnable == TRUE
  TdxInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tdx/Library/TdxInfoLib/TdxInfoLib.inf
!else
  TdxInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tdx/Library/TdxInfoLibNull/TdxInfoLibNull.inf
!endif # gSiPkgTokenSpaceGuid.PcdTdxEnable == TRUE

  TmeInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tme/Library/TmeInfoLib.inf
  DgrInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dgr/Library/PeiDxeSmmDgrInfoLib/DgrInfoLib.inf
  CceInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Cce/Library/CceInfoLib.inf

#################################################################
# Uncore Start #
#################################################################

  #
  # Telemetry
  #
  TelemetryLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Telemetry/Library/PeiDxeTelemetryLib/PeiDxeTelemetryLib.inf
  TelemetryPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Telemetry/LibraryPrivate/PeiDxeTelemetryPrivateLib/PeiDxeTelemetryPrivateLib.inf

  #
  # VMD IpBlock
  #
  VmdInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/Library/VmdInfoLib/VmdInfoLib.inf

  #
  # VTD IpBlock
  #
  VtdInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vtd/Library/PeiDxeSmmVtdInfoLib/PeiDxeSmmVtdInfoLib.inf
  VtdInfoFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Vtd/Library/PeiDxeSmmVtdInfoFruLib/VtdInfoFruLib.inf

  #
  # TDX IpBlock
  #
  TdxFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/TdxFruLib/TdxFruLib.inf

  #
  # Host Bridge
  #
  HostBridgeInfoFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/HostBridge/Library/PeiDxeSmmHostBridgeInfoFruLib/HostBridgeInfoFruLib.inf

  #
  # IPU IpBlock
  #
  IpuLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ipu/Library/PeiDxeSmmIpuLib/IpuLib.inf

  #
  # Hybrid Grapics
  #
  HybridGraphicsInfoFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/HybridGraphics/Library/PeiDxeSmmHybridGraphicsInfoFruLib/PeiDxeSmmHybridGraphicsInfoFruLib.inf

  #
  # System Agent
  #
  CpuRegbarAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/Library/PeiDxeSmmCpuRegbarAccessLib/PeiDxeSmmCpuRegbarAccessLib.inf
  CpuRegbarAccessFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Library/PeiDxeSmmCpuRegbarAccessFruLib/PeiDxeSmmCpuRegbarAccessFruLib.inf
  CpuSbiAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/Library/PeiDxeSmmCpuSbiAccessLib/PeiDxeSmmCpuSbiAccessLibNull.inf
  PeiHostBridgeIpStatusLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Library/PeiHostBridgeIpStatusLib/PeiHostBridgeIpStatusLib.inf
  IaxInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Iax/Library/PeiDxeSmmIaxInfoLib/IaxInfoLib.inf

  #
  # PMON
  #
  PmonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Pmon/LibraryPrivate/PmonLib/PmonLib.inf
  PmonDataLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PmonDataLib/PmonDataLib.inf

  #
  # NOC
  #
  NguInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Library/NguInfoLib/NguInfoLib.inf

  #
  # Overclocking
  #
!if gSiPkgTokenSpaceGuid.PcdOverclockEnable == TRUE
  OcFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Overclocking/LibraryPrivate/BaseOcFruLib/BaseOcFruLib.inf
  OcCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiDxeSmmOcCommonLib/PeiDxeSmmOcCommonLib.inf
  OcPlatformLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/Library/DxeOcPlatformLib/DxeOcPlatformLib.inf
!else
  OcCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiDxeSmmOcCommonLibNull/PeiDxeSmmOcCommonLibNull.inf
  OcPlatformLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/Library/DxeOcPlatformLibNull/DxeOcPlatformLibNull.inf
!endif
#################################################################
# Uncore End #
#################################################################
