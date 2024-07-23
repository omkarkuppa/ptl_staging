## @file
#  Component description file for the SiPkg PEI libraries.
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

[LibraryClasses.IA32.SEC, LibraryClasses.IA32.PEIM, LibraryClasses.IA32.PEI_CORE, LibraryClasses.X64.SEC, LibraryClasses.X64.PEIM, LibraryClasses.X64.PEI_CORE, LibraryClasses.X64.HOST_APPLICATION]
#
# Silicon Init Pei Library
#
!include $(PLATFORM_SI_PACKAGE)/UniversalPeiLib.dsc

#
# FRUs
#
!include $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/PeiLib.dsc
!include $(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/PeiLib.dsc

#
# IpBlock Memory Init
#
MemoryInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/MemoryInit/PeiMemoryInitLibPtl.inf
SiPolicyLib|$(PLATFORM_SI_PACKAGE)/Product/PantherLake/Library/PeiSiPolicyLib/PeiSiPolicyLib.inf
SiConfigBlockLib|$(PLATFORM_SI_PACKAGE)/Library/BaseSiConfigBlockLib/BaseSiConfigBlockLib.inf
SiFviInitLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PeiSiFviInitLib/PeiSiFviInitLib.inf
StallPpiLib|$(PLATFORM_SI_PACKAGE)/Library/PeiInstallStallPpiLib/PeiStallPpiLib.inf
SiPolicyOverrideLib|$(PLATFORM_SI_PACKAGE)/Product/PantherLake/LibraryPrivate/PeiPolicyOverrideLib/PeiSiPolicyOverrideLib.inf
PeiSiSsidLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PeiSiSsidLib/PeiSiSsidLib.inf
SiMtrrLib|$(PLATFORM_SI_PACKAGE)/Library/SiMtrrLib/SiMtrrLib.inf
CacheAsRamLib|$(PLATFORM_SI_PACKAGE)/Library/BaseCacheAsRamLib/BaseCacheAsRamLib.inf
NemConfigLib|$(PLATFORM_SI_PACKAGE)/Library/BaseNemConfigLib/BaseNemConfigLib.inf

!if gSiPkgTokenSpaceGuid.PcdPcieIpsdEnabled == TRUE
 PcieHelpersLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainPcie/Ver1/PcieHelpersLib/PcieBaseHelpersLib.inf
 DomainPcieInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainPcie/Ver1/Library/DomainPcieInitLib/DomainPcieLib.inf
#
# PCIE IPFW
#
 IpPcieLib|$(PLATFORM_SI_PACKAGE)/IpBlock/IpPcieSip17/IpPcieLib/IpPcieLib.inf
!else
  DomainPcieInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainPcie/Ver1/Library/DomainPcieInitNullLib/DomainPcieNullLib.inf
!endif

#
# Domain IGPU
#
 IGpuInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainIGpu/LibraryPrivate/IGpuInit/IGpuInit.inf

#
# Pch
#

PeiPtlPcdInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiInitLib/PtlPcdPeiInitLib.inf
PtlPcdPolicyLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdPolicyLib/PtlPcdPolicyLib.inf

!if gSiPkgTokenSpaceGuid.PcdS3Enable == TRUE
 PchSmmControlLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiPchSmmControlLib/PeiPchSmmControlLib.inf
!else
 PchSmmControlLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiPchSmmControlLibNull/PeiPchSmmControlLibNull.inf
!endif

#
# Cpu
#
 CpuCommonLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PeiDxeSmmCpuCommonLib/PeiDxeSmmCpuCommonLib.inf
 CpuInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiCpuInitLib/PeiCpuInitLib.inf
 SmmRelocationLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/SmmRelocationLib/SmmRelocationLib.inf
!if gSiPkgTokenSpaceGuid.PcdBiosGuardEnable == TRUE
 BiosGuardLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BiosGuard/LibraryPrivate/PeiBiosGuardLib/PeiBiosGuardLib.inf
 PeiBiosGuardInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiBiosGuardInitFruLib/PeiBiosGuardInitFruLib.inf
!else
 BiosGuardLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BiosGuard/LibraryPrivate/PeiBiosGuardLibNull/PeiBiosGuardLibNull.inf
!endif
 CpuPowerManagementLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuPowerManagement/LibraryPrivate/PeiCpuPowerManagementLib/PeiCpuPowerManagementLib.inf

!if gSiPkgTokenSpaceGuid.PcdCpuPowerOnConfigEnable == TRUE
 CpuPowerOnConfigLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiCpuPowerOnConfigLib/PeiCpuPowerOnConfigLib.inf
!else
 CpuPowerOnConfigLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiCpuPowerOnConfigLibDisable/PeiCpuPowerOnConfigLibDisable.inf
!endif
!if gSiPkgTokenSpaceGuid.PcdSmbiosEnable == TRUE
 SmbiosCpuLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiSmbiosCpuLib/PeiSmbiosCpuLib.inf
!else
 SmbiosCpuLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiSmbiosCpuLibNull/PeiSmbiosCpuLibNull.inf
!endif
#
# Txt
#
!if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE
 PeiTxtLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Txt/LibraryPrivate/PeiTxtLib/PeiTxtLib.inf
!else
 PeiTxtLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Txt/LibraryPrivate/PeiTxtLibNull/PeiTxtLibNull.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE || gSiPkgTokenSpaceGuid.PcdBiosGuardEnable == TRUE
 PeiDprInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dpr/LibraryPrivate/PeiDprInfoLib/PeiDprInfoLib.inf
!else
 PeiDprInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dpr/LibraryPrivate/PeiDprInfoLibNull/PeiDprInfoLibNull.inf
!endif

 PeiExtGfxCardDetectLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PeiExtGfxCardDetectLib/PeiExtGfxCardDetectLib.inf
 TxtPolicyLibPreMem|$(PLATFORM_SI_PACKAGE)/IpBlock/Txt/LibraryPrivate/PeiTxtPolicyLib/PeiTxtPolicyLibPreMem.inf

#
#  Tse IpBlock
#
!if gSiPkgTokenSpaceGuid.PcdTseEnable == TRUE
 PeiTseLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tse/LibraryPrivate/PeiTseLib/PeiTseLib.inf
!else
 PeiTseLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tse/LibraryPrivate/PeiTseLibNull/PeiTseLibNull.inf
!endif

#
# Host Bridge
#
 PeiHostBridgeInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/LibraryPrivate/PeiHostBridgeInitLib/PeiHostBridgeInitLib.inf
 PeiHostBridgePolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/LibraryPrivate/PeiHostBridgePolicyLib/PeiHostBridgePolicyLibVer3.inf
 PeiHostBridgeInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiHostBridgeInitFruLib/PeiHostBridgeInitFruLib.inf
 PeiMemoryMapPrintLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiMemoryMapPrintLib/PeiMemoryMapPrintLib.inf

#
# DMI
#
!if gSiPkgTokenSpaceGuid.PcdDmiEnable == TRUE
  PeiDmiTaskDispatchLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dmi/Library/PeiDmiTaskDispatchLib/PeiDmiTaskDispatchLib.inf
!else
  PeiDmiTaskDispatchLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dmi/Library/PeiDmiTaskDispatchLibNull/PeiDmiTaskDispatchLibNull.inf
!endif

 PeiTcssPolicyLib|$(PLATFORM_SI_PACKAGE)/Subsystem/Tcss/PeiTcssPolicyLib/PeiTcssPolicyLibVer1.inf

 IpTcPcieRpLib|$(PLATFORM_SI_PACKAGE)/IpBlock/TcPcieRp/IpTcPcieRpLib/IpTcPcieRpLib.inf
 IpUsb4Lib|$(PLATFORM_SI_PACKAGE)/IpBlock/Usb4/IpUsb4Lib/IpUsb4Lib.inf
 IpIomLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Iom/IpIomLib/IpIomLib.inf
 IpTcssLib|$(PLATFORM_SI_PACKAGE)/Subsystem/Tcss/TcssLib/IpTcssLib.inf

!if gSiPkgTokenSpaceGuid.PcdFspBinaryEnable == TRUE
  PeiMemPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/MemoryInit/Library/PeiMemPolicyLib/PeiMemPolicyLibFspPtl.inf
!else
  PeiMemPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/MemoryInit/Library/PeiMemPolicyLib/PeiMemPolicyLibPtl.inf
!endif
 MemorySubSystemPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/MemorySubSystem/LibraryPrivate/MemorySubSystemPolicyLib/MemorySubSystemPolicyLib.inf

#
# TBT
#
  PeiITbtPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Usb4/PeiITbtPolicyLib/PeiITbtPolicyLib.inf

#
# Cpu
#
  CpuPowerManagementPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuPowerManagement/LibraryPrivate/PeiCpuPowerManagementPolicyLib/PeiCpuPowerManagementPolicyLib.inf
  CpuInitPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiCpuInitPolicyLib/PeiCpuInitPolicyLib.inf
  PeiBootGuardPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BootGuard/LibraryPrivate/PeiBootGuardPolicyLib/PeiBootGuardPolicyLib.inf

#
# Telemetry
#
PeiTelemetryInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Telemetry/LibraryPrivate/PeiTelemetryInitLib/PeiTelemetryInitLib.inf

#
# RAT Libs
#
!if $(RAT_SUPPORT) == TRUE
  RegisterAddressTranslationLib|$(PLATFORM_SI_PACKAGE)/Library/BaseRegisterAddressTranslationLib/BaseRegisterAddressTranslationLib.inf
  RegisterAddressMapLib|$(PLATFORM_SI_PACKAGE)/Product/PantherLake/LibraryPrivate/RegisterAddressMapLib/PeiAddressMapTableLib.inf
!else
  RegisterAddressTranslationLib|$(PLATFORM_SI_PACKAGE)/Library/RegisterAddressTranslationLibNull/RegisterAddressTranslationLibNull.inf
!endif

#
# Capsule update support
#
  CapsulePpiProcessLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/CapsulePpiProcessLib/CapsulePpiProcessLib.inf
