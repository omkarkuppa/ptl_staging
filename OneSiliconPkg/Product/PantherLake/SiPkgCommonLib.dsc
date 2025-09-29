## @file
#  Component description file for the SiPkg both Pei and Dxe libraries DSC file.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2016 Intel Corporation.
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
# Silicon Init Common Library
#

#
# Set PCH generation according PCD.
# The DEFINE will be used to select PCH library INF file corresponding to PCH generation
#

!include $(PLATFORM_SI_PACKAGE)/UniversalCommonLib.dsc
#
# FRUs
#
!include $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/CommonLib.dsc
!include $(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/CommonLib.dsc

#
# Common
#
 PreSiliconEnvDetectLib|$(PLATFORM_SI_PACKAGE)/Library/BasePreSiliconEnvDetectLib/BasePreSiliconEnvDetectLib.inf
 MmPciLib|$(PLATFORM_SI_PACKAGE)/Library/PeiDxeSmmMmPciLib/PeiDxeSmmMmPciLib.inf
 PciExpressLib|$(PLATFORM_SI_PACKAGE)/Library/BasePciExpressMultiSegLib/BasePciExpressMultiSegLib.inf
 PciSegmentLib|$(PLATFORM_SI_PACKAGE)/Library/BasePciSegmentMultiSegLibPci/BasePciSegmentMultiSegLibPci.inf
 SystemTimeLib|$(PLATFORM_SI_PACKAGE)/Library/BaseSystemTimeLib/BaseSystemTimeLib.inf
 SiMtrrLib|$(PLATFORM_SI_PACKAGE)/Library/SiMtrrLib/SiMtrrLib.inf
 PeiDxeSmmReserveMmio64SizeLib|$(PLATFORM_SI_PACKAGE)/Library/PeiDxeSmmReserveMmio64SizeLib/PeiDxeSmmReserveMmio64SizeLib.inf

 IpWrapperLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/BaseIpWrapperLib/BaseIpWrapperLib.inf
 IpWrapper64BitLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/BaseIpWrapper64BitLib/BaseIpWrapper64BitLib.inf

#
# Cpu
#
CpuPmDataLib|OneSiliconPkg/LibraryPrivate/CpuPmDataLib/CpuPmDataLib.inf
SecCpuLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/SecCpuLib/SecCpuLib.inf

 CpuPlatformLib|$(PLATFORM_SI_PACKAGE)/Library/PeiDxeSmmCpuPlatformLib/PeiDxeSmmCpuPlatformLib.inf
 PeiVrLib|$(PLATFORM_SI_PACKAGE)/IpBlock/VoltageRegulator/LibraryPrivate/PeiVrLib/PeiVrLib.inf
 CpuUnitTestFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/CpuUnitTestFruLib/CpuUnitTestFruLib.inf
 CpuPerThreadInitPreMpLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/Library/CpuPerThreadInitPreMpLib/CpuPerThreadInitPreMpLib.inf

#
# TXT/BTG
#
#TxtLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Txt/LibraryPrivate/PeiDxeTxtLib/PeiDxeTxtLib.inf
 TxtLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Txt/Library/PeiDxeTxtLib/PeiDxeTxtLib.inf
!if gSiPkgTokenSpaceGuid.PcdBootGuardEnable == TRUE
  BootGuardLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BootGuard/LibraryPrivate/PeiDxeSmmBootGuardLib/PeiDxeSmmBootGuardLib.inf
!else
  BootGuardLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BootGuard/LibraryPrivate/PeiDxeSmmBootGuardLibNull/PeiDxeSmmBootGuardLibNull.inf
!endif

 CpuMailboxLib|$(PLATFORM_SI_PACKAGE)/Library/PeiDxeSmmCpuMailboxLib/PeiDxeSmmCpuMailboxLib.inf
 OcMailboxLib|$(PLATFORM_SI_PACKAGE)/Library/OcMailboxLib/OcMailboxLib.inf
 SecSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdSecLib/PtlPcdSecLib.inf

 CpuSbiAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/Library/PeiDxeSmmCpuSbiAccessLib/PeiDxeSmmCpuSbiAccessLib.inf
 ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseResetSystemLib/BaseResetSystemLib.inf

 SiScheduleResetLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/BaseSiScheduleResetLib/BaseSiScheduleResetLib.inf
 PchPciBdfLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BasePchPciBdfLib/BasePchPciBdfLib.inf

#
# NEM
#
 NemConfigLib|$(PLATFORM_SI_PACKAGE)/Library/BaseNemConfigLib/BaseNemConfigLib.inf


#
# SA
#
 ItbtPcieRpLib|$(PLATFORM_SI_PACKAGE)/Subsystem/Tcss/PeiDxeSmmItbtPcieRpLib/PeiDxeSmmItbtPcieRpLib.inf
 CpuRegbarAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/Library/PeiDxeSmmCpuRegbarAccessLib/PeiDxeSmmCpuRegbarAccessLib.inf
 TcssInfoLib|$(SILICON_PRODUCT_PATH)/Library/TcssInfoLib/TcssInfoLib.inf

 SecHostBridgeLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/Library/SecHostBridgeLib/SecHostBridgeLib.inf
 HostBridgeDataLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/Library/HostBridgeDataLib/HostBridgeDataLib.inf
 HostBridgeInfoFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/HostBridge/Library/PeiDxeSmmHostBridgeInfoFruLib/HostBridgeInfoFruLib.inf

#
# IGPU
#
  BaseIGpuInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainIGpu/Library/BaseIGpuInfoLib/BaseIGpuInfoLib.inf
  IpIGpuLib|$(PLATFORM_SI_PACKAGE)/CsiIpBlock/IpIGpu/IpIGpuLib.inf

#
# TBT
#

  TbtCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tbt/Library/PeiDxeSmmTbtCommonLib/TbtCommonLib.inf
  DxeTbtDisBmeLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tbt/Library/DxeTbtDisBmeLib/DxeTbtDisBmeLib.inf

#
# Overclocking
#
!if gSiPkgTokenSpaceGuid.PcdOverclockEnable == TRUE
  OcCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiDxeSmmOcCommonLib/PeiDxeSmmOcCommonLib.inf
!else
  OcCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiDxeSmmOcCommonLibNull/PeiDxeSmmOcCommonLibNull.inf
!endif

#
# Memory
#
MemoryAddressDecodeEncodeLib|$(PLATFORM_SI_PACKAGE)/IpBlock/MemoryInit/Library/PeiDxeSmmMemAddrEncodeLib/MemAddrDecodeEncodeLib.inf

#
# PEIM Memory Telemetry AMT PPR variable update library
#
PeiMemTelAmtPprLib|$(PLATFORM_SI_PACKAGE)/IpBlock/PeiMemTelAmtPprLib/Library/PeiMemTelAmtPprLib/PeiMemTelAmtPprLib.inf