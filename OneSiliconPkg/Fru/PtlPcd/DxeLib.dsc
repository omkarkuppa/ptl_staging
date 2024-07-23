## @file
#  Component description file for the PantherLake SOC DXE FRU libraries.
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
#@par Specification Reference:
#
##


#
# TCSS
#
DxeTcssInitLib|$(PLATFORM_SI_PACKAGE)/Subsystem/Tcss/DxeTcssInitLib/DxeTcssInitLib.inf
DxeTcssInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Tcss/LibraryPrivate/PtlPcdDxeTcssInitFruLib/PtlPcdDxeTcssInitFruLib.inf

#
# TraceHub and IPU
#
  TraceHubHookLib|$(PLATFORM_SI_PACKAGE)/IpBlock/TraceHub/Library/TraceHubHookLib/DxeSmmTraceHubHookLib.inf

  DxeHdaPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Hda/LibraryPrivate/DxeHdaPolicyLib/DxeHdaPolicyLib.inf

  #
  # PCH
  #
  GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gpio/LibraryPrivate/BaseGpioHelpersLibNull/BaseGpioHelpersLibNull.inf
  DxeGpioPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gpio/LibraryPrivate/DxeGpioPolicyLib/DxeGpioPolicyLib.inf
  DxePchPcieRpPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/PcieRp/LibraryPrivate/DxePchPcieRpPolicyLib/DxePchPcieRpPolicyLib.inf
  SerialIoUartSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PtlPcdDxeSmmSerialIoUartLib.inf
  LpssI2cSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdLpssI2cSocLib/PtlPcdLpssI2cSocLib.inf
  SerialIoI2cSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PtlPcdDxeSmmSerialIoI2cLib.inf
  PtlPcdInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdInfoLib/PtlPcdDxeInfoLib.inf
  DxeThcSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdThcLib/PtlPcdDxeThcLib.inf
  ThcConfigurePrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Thc/LibraryPrivate/PeiDxeSmmThcPrivateLib/PeiDxeSmmThcPrivateLib.inf

#
# Vmd
#
!if gSiPkgTokenSpaceGuid.PcdVmdEnable == TRUE
  DxeVmdInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/LibraryPrivate/DxeVmdInitLib/DxeVmdInitLib.inf
  DxeVmdInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Vmd/LibraryPrivate/DxeVmdInitFruLib/DxeVmdInitFruLib.inf
!else
  DxeVmdInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/LibraryPrivate/DxeVmdInitLibNull/DxeVmdInitLibNull.inf
  DxeVmdInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Vmd/LibraryPrivate/DxeVmdInitFruLib/DxeVmdInitFruLibNull.inf
!endif

  #
  #  Tse IpBlock
  #
  !if gSiPkgTokenSpaceGuid.PcdTseEnable == TRUE
    DxeTseLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tse/LibraryPrivate/DxeTseLib/DxeTseLib.inf
  !else
    DxeTseLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tse/LibraryPrivate/DxeTseLibNull/DxeTseLibNull.inf
  !endif

#
#  GPIOV2
#
GpioV2AccessLib            |$(PLATFORM_SI_PACKAGE)/IpBlock/GpioV2/Library/GpioV2Services/GpioV2AccessDxeLib.inf
GpioV2SmmAccessLib         |$(PLATFORM_SI_PACKAGE)/IpBlock/GpioV2/Library/GpioV2Services/GpioV2AccessSmmLib.inf
