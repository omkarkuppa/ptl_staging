## @file
#  Component description file for the SiPkg DXE libraries.
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
[LibraryClasses.X64]
#
# Silicon Init Dxe Library
#
!include $(PLATFORM_SI_PACKAGE)/UniversalDxeLib.dsc

#
# FRUs
#
!include $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/DxeLib.dsc
!include $(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/DxeLib.dsc

#
# Common
#
!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
 AslUpdateLib|IntelSiliconPkg/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf
!else
 AslUpdateLib|$(PLATFORM_SI_PACKAGE)/Library/DxeAslUpdateLibNull/DxeAslUpdateLibNull.inf
!endif
 SiFviLib|$(PLATFORM_SI_PACKAGE)/Library/DxeSiFviLib/DxeSiFviLib.inf
 SiFviInitLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/DxeSiFviInitLib/DxeSiFviInitLib.inf
 SiConfigBlockLib|$(PLATFORM_SI_PACKAGE)/Library/BaseSiConfigBlockLib/BaseSiConfigBlockLib.inf

#
# Cpu
#
 CpuCommonLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PeiDxeSmmCpuCommonLib/PeiDxeSmmCpuCommonLib.inf

#
# Pch
#
 DxePchPolicyLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/DxePchPolicyLib/DxePchPolicyLib.inf
 SmmPchPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/SmmPchPrivateLib/SmmPchPrivateLib.inf

#
# SystemAgent
#
#  DxeSaPolicyLib|$(PLATFORM_SI_PACKAGE)/SystemAgent/Library/DxeSaPolicyLib/DxeSaPolicyLib.inf
  DxeIGpuPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainIGpu/Library/DxeIGpuPolicyLib/DxeIGpuPolicyLib.inf


#
# SMBIOS
#
!if gSiPkgTokenSpaceGuid.PcdSmbiosEnable == TRUE
 SmbiosMemoryLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/DxeSmbiosMemoryLib/DxeSmbiosMemoryLib.inf
!else
 SmbiosMemoryLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/DxeSmbiosMemoryLibNull/DxeSmbiosMemoryLibNull.inf
!endif

#
# TBT
#
 DxeITbtPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tbt/Library/DxeITbtPolicyLib/DxeITbtPolicyLib.inf
 DxeTcssInitLib|$(PLATFORM_SI_PACKAGE)/Subsystem/Tcss/DxeTcssInitLib/DxeTcssInitLib.inf
 DxeTcssInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Tcss/LibraryPrivate/PtlPcdDxeTcssInitFruLib/PtlPcdDxeTcssInitFruLib.inf

#
# ME
#
 MeLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/DxeMeLib/DxeMeLib.inf
 MeUtilsLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/PeiDxeSmmMeUtilsLib/DxeSmmMeUtilsLib.inf
 DxeMePolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/DxeMePolicyLib/DxeMePolicyLib.inf
 DxeAsfLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/DxeAsfLib/DxeAsfLib.inf
 DxeAsfHeciLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/DxeAsfHeciLib/DxeAsfHeciLib.inf
 HeciAsyncSupportLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/LibraryPrivate/DxeHeciAsyncSupportLib/DxeHeciAsyncSupportLib.inf
 MeFruInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdMeInitLib/PtlPcdDxeMeInitLib.inf

#
# AMT
#
!if gSiPkgTokenSpaceGuid.PcdAmtEnable == TRUE
 DxeAmtHeciLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/DxeAmtHeciLib/DxeAmtHeciLib.inf
 DxeAmthiMsgsLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/LibraryPrivate/DxeAmthiMsgsLib/DxeAmthiMsgsLib.inf
 DxeMebxDisplayWrapperLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/DxeMebxDisplayWrapperLib/DxeMebxDisplayWrapperLib.inf
 DxeMebxWrapperLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/DxeMebxWrapperLib/DxeMebxWrapperLib.inf
 DxeAmtPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/DxeAmtPolicyLib/DxeAmtPolicyLib.inf
 DxeAmtSupportLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/DxeAmtSupportLib/DxeAmtSupportLib.inf
!else
 DxeAmtPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/DxeAmtPolicyLibNull/DxeAmtPolicyLibNull.inf
 DxeAmtHeciLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/DxeAmtHeciLibNull/DxeAmtHeciLibNull.inf
 DxeAmtSupportLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/DxeAmtSupportLibNull/DxeAmtSupportLibNull.inf
!endif

#
# RAT Libs
#
!if $(RAT_SUPPORT) == TRUE
 RegisterAddressTranslationLib|$(PLATFORM_SI_PACKAGE)/Library/BaseRegisterAddressTranslationLib/BaseRegisterAddressTranslationLib.inf
 RegisterAddressMapLib|$(PLATFORM_SI_PACKAGE)/Product/PantherLake/LibraryPrivate/RegisterAddressMapLib/DxeRuntimeAddressMapTableLib.inf
!else
 RegisterAddressTranslationLib|$(PLATFORM_SI_PACKAGE)/Library/RegisterAddressTranslationLibNull/RegisterAddressTranslationLibNull.inf
!endif

[LibraryClasses.common.DXE_SMM_DRIVER]
  SmbusLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Smbus/LibraryPrivate/SmmSmbusLib/SmmSmbusLib.inf

[LibraryClasses.common.DXE_DRIVER]
  SmbusLib|MdePkg/Library/DxeSmbusLib/DxeSmbusLib.inf