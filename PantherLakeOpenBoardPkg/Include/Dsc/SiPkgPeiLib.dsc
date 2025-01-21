## @file
#  Component description file for the SiPkg PEI libraries.
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
[LibraryClasses.IA32.SEC, LibraryClasses.X64.SEC]
  SecGetFsptApiParameterLib|$(PLATFORM_SI_PACKAGE)/Library/SecGetFsptApiParameterLib/SecGetFsptApiParameterLib.inf

[LibraryClasses.IA32.SEC, LibraryClasses.IA32.PEIM, LibraryClasses.IA32.PEI_CORE, LibraryClasses.X64.SEC, LibraryClasses.X64.PEIM, LibraryClasses.X64.PEI_CORE]
#
# Silicon Init Pei Library
#

#
# FRUs
#
#
#!include $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/PeiLib.dsc
#

  PeiSpiDmaLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/LibraryPrivate/PeiSpiDmaLibNull/PeiSpiDmaLibNull.inf
  GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gpio/LibraryPrivate/PeiGpioHelpersLib/PeiGpioHelpersLib.inf
  PtlPcdHeciDeviceConfigLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdHeciDeviceConfigLib/PtlPcdHeciDeviceConfigLib.inf
  MeInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/LibraryPrivate/PeiMeInitLib/PeiMeInitLib.inf
  MeFruInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdMeInitLib/PtlPcdPeiMeInitLib.inf
  MeLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/PeiMeLib/PeiMeLib.inf
  PeiItssLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Itss/LibraryPrivate/PeiItssLib/PeiItssLib.inf
  PeiSpiLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/Library/PeiSpiLib/PeiSpiLib.inf
  HeciAsyncSupportLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/LibraryPrivate/PeiDxeHeciAsyncSupportLibNull/PeiDxeHeciAsyncSupportLibNull.inf
  MeUtilsLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/PeiDxeSmmMeUtilsLib/PeiMeUtilsLib.inf

#
# GPIOV2
#
  GpioV2AccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/GpioV2/Library/GpioV2Services/GpioV2AccessPeiLib.inf

#
# Pch
#
  PtlPcdInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdInfoLib/PtlPcdPeiInfoLib.inf

#
# RAT Libs
#
!if $(RAT_SUPPORT) == TRUE
  RegisterAddressTranslationLib|$(PLATFORM_SI_PACKAGE)/Library/BaseRegisterAddressTranslationLib/BaseRegisterAddressTranslationLib.inf
  RegisterAddressMapLib|$(PLATFORM_SI_PACKAGE)/Product/PantherLake/LibraryPrivate/RegisterAddressMapLib/PeiAddressMapTableLib.inf
!else
  RegisterAddressTranslationLib|$(PLATFORM_SI_PACKAGE)/Library/RegisterAddressTranslationLibNull/RegisterAddressTranslationLibNull.inf
!endif
