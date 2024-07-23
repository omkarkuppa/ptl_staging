## @file
#  Component description file for the SiPkg DXE libraries.
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
[LibraryClasses.X64]
#
# Silicon Init Dxe Library
#
#!include $(PLATFORM_SI_PACKAGE)/UniversalDxeLib.dsc
#
# Pch
#
  PlatformPchSmiRegisterLockLib         |$(PLATFORM_SI_PACKAGE)/Pch/Library/PlatformPchSmiRegisterLockLibNull/PlatformPchSmiRegisterLockLibNull.inf


#
# FRUs
#
#!include $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/DxeLib.dsc
#
# PCH
#
PtlPcdInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdInfoLib/PtlPcdDxeInfoLib.inf
#
#  GPIOV2
#
GpioV2AccessLib            |$(PLATFORM_SI_PACKAGE)/IpBlock/GpioV2/Library/GpioV2Services/GpioV2AccessDxeLib.inf

SiConfigBlockLib|$(PLATFORM_SI_PACKAGE)/Library/BaseSiConfigBlockLib/BaseSiConfigBlockLib.inf

#
# Cpu
#
 CpuCommonLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PeiDxeSmmCpuCommonLib/PeiDxeSmmCpuCommonLib.inf

#
# Pch
#
 SmmPchPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/SmmPchPrivateLib/SmmPchPrivateLib.inf