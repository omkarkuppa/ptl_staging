## @file
#  Component description file for the SiPkg both Pei and Dxe libraries DSC file.
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

#!include $(PLATFORM_SI_PACKAGE)/UniversalCommonLib.dsc
#
# Silicon Init Common Library
#
  FspErrorInfoLib|$(PLATFORM_SI_PACKAGE)/Library/FspErrorInfoLib/PeiDxeFspErrorInfoLib.inf

#
# Common
#
!if $(SEAMLESS_SDU_SUPPORT) == TRUE && $(CPU_GNRD_SUPPORT) != TRUE
  MmRuntimeUpdateLib|SmmRuntimeUpdateFeaturePkg/Library/MmRuntimeUpdateLib/MmRuntimeUpdateLib.inf
!else
  MmRuntimeUpdateLib|$(PLATFORM_SI_PACKAGE)/Library/MmRuntimeUpdateLibNull/MmRuntimeUpdateLibNull.inf
!endif

#
# FRUs
#
# Maintain the strict order to avoid overriding Libraries.
CpuPcieInfoFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/CpuPcieRp/Library/PtlPcdPeiDxeSmmCpuPcieInfoFruLibNull/PtlPcdPeiDxeSmmCpuPcieInfoFruLibNull.inf
PsfLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Psf/LibraryPrivate/PsfLib/PeiDxeSmmPsfLib.inf
PsfSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiDxeSmmPsfLib/PtlPcdPeiDxeSmmPsfLib.inf
P2SbSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdP2SbLib/PtlPcdP2SbLib.inf
PchPcrLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/Library/PeiDxeSmmPchPcrLib/PeiDxeSmmPchPcrLib.inf
P2SbSidebandAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/LibraryPrivate/PeiDxeSmmP2SbSidebandAccessLib/PeiDxeSmmP2SbSidebandAccessLib.inf
EspiPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Espi/LibraryPrivate/PeiDxeSmmEspiPrivateLib/PeiDxeSmmEspiPrivateLib.inf

PmcLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Pmc/Library/PeiDxeSmmPmcLib/PeiDxeSmmPmcLib.inf
PmcPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Pmc/LibraryPrivate/PeiDxeSmmPmcPrivateLib/PeiDxeSmmPmcPrivateLib.inf
PmcSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiDxeSmmPmcLib/PtlPcdPeiDxeSmmPmcLib.inf
PmcPrivateLibWithS3|$(PLATFORM_SI_PACKAGE)/IpBlock/Pmc/LibraryPrivate/PeiDxeSmmPmcPrivateLib/PeiDxeSmmPmcPrivateLibWithS3.inf
SpiCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/LibraryPrivate/BaseSpiCommonLib/BaseSpiCommonLib.inf
OcWdtLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Wdt/Library/PeiOcWdtLibNull/PeiOcWdtLibNull.inf
PchDmiLib|$(PLATFORM_SI_PACKAGE)/IpBlock/PchDmi/LibraryPrivate/PeiDxeSmmPchDmiLib/PeiDxeSmmPchDmiLib.inf
IocLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ioc/LibraryPrivate/IocLib/PeiDxeSmmIocLib.inf
IocRegAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ioc/LibraryPrivate/IocRegAccessLib/IocRegAccessLib.inf

SataSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdSataLib/PtlPcdSataLib.inf

P2sbPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/LibraryPrivate/P2sbPrivateLib/P2sbPrivateLib.inf
SpiAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/Library/PeiDxeSmmSpiAccessLib/PeiDxeSmmSpiAccessLib.inf
PtlPcdPcieRpSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPcieRpLib/PtlPcdPcieRpLib.inf
PchPcieRpLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPcieRpLib/PtlPcdPcieRpLib.inf
PchInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdPeiDxeSmmPchInfoLib/PtlPcdPeiDxeSmmPchInfoLib.inf
SocInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdInfoLib/PtlPcdInfoLib.inf
IoTrapSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdIoTrapLib/PtlPcdIoTrapLib.inf
PchCycleDecodingLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdPeiDxeSmmPchCycleDecodingLib/PtlPcdPeiDxeSmmPchCycleDecodingLib.inf
SpiSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSpiLib/PtlPcdSpiLib.inf
SpiInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/LibraryPrivate/SpiInitLib/SpiInitLib.inf

#
# LPSS
#
LpssUartDebugPropertyPcdLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssUart/Library/LpssUartDebugPropertyPcdLib/LpssUartDebugPropertyPcdLib.inf

#
# GPIOV2
#
GpioV2ServicesCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/GpioV2/Library/GpioV2Services/Common/GpioV2ServicesCommonLib.inf
PtlPcdGpioTopologyLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdGpioTopology/PtlPcdGpioTopology.inf
PtlPcdSecGpioTopology|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdGpioTopology/PtlPcdSecGpioTopology.inf

#!include $(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/CommonLib.dsc
CpuInfoFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/BaseCpuInfoFruLib/BaseCpuInfoFruLib.inf

#
# ME
#
MeChipsetLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/PeiDxeMeChipsetLib/PeiDxeMeChipsetLib.inf
MeInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdMeLib/PtlPcdMeLib.inf
HeciInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/LibraryPrivate/PeiDxeHeciInitLib/PeiDxeHeciInitLib.inf
MeFwStsLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/BaseMeFwStsLib/BaseMeFwStsLib.inf
MeShowBufferLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/PeiDxeMeShowBufferLib/PeiDxeMeShowBufferLib.inf

# #################################################################
# # Uncore Start #
# #################################################################
  #
  # System Agent
  #
  CpuRegbarAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/Library/PeiDxeSmmCpuRegbarAccessLib/PeiDxeSmmCpuRegbarAccessLib.inf
  CpuRegbarAccessFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Library/PeiDxeSmmCpuRegbarAccessFruLib/PeiDxeSmmCpuRegbarAccessFruLib.inf
  PeiHostBridgeIpStatusLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Library/PeiHostBridgeIpStatusLib/PeiHostBridgeIpStatusLib.inf

#################################################################
# Uncore End #
#################################################################

#
# Common
#
 PreSiliconEnvDetectLib|$(PLATFORM_SI_PACKAGE)/Library/BasePreSiliconEnvDetectLib/BasePreSiliconEnvDetectLib.inf
 MmPciLib|$(PLATFORM_SI_PACKAGE)/Library/PeiDxeSmmMmPciLib/PeiDxeSmmMmPciLib.inf
 IpWrapperLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/BaseIpWrapperLib/BaseIpWrapperLib.inf
 IpWrapper64BitLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/BaseIpWrapper64BitLib/BaseIpWrapper64BitLib.inf

#
# IGPU
#
BaseIGpuInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/DomainIGpu/Library/BaseIGpuInfoLib/BaseIGpuInfoLib.inf
IpIGpuLib|$(PLATFORM_SI_PACKAGE)/CsiIpBlock/IpIGpu/IpIGpuLib.inf

#
# NOC
#
NguInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Library/NguInfoLib/NguInfoLib.inf

#
# Cpu
#
 CpuPlatformLib|$(PLATFORM_SI_PACKAGE)/Library/PeiDxeSmmCpuPlatformLib/PeiDxeSmmCpuPlatformLib.inf

#
# BTG
#
 BootGuardLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BootGuard/LibraryPrivate/PeiDxeSmmBootGuardLibNull/PeiDxeSmmBootGuardLibNull.inf

 CpuMailboxLib|$(PLATFORM_SI_PACKAGE)/Library/PeiDxeSmmCpuMailboxLib/PeiDxeSmmCpuMailboxLib.inf
 ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseResetSystemLib/BaseResetSystemLib.inf
 PchPciBdfLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BasePchPciBdfLib/BasePchPciBdfLib.inf