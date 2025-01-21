## @file
#  Component description file for the PantherLake SOC PEI FRU ibraries.
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
# Host Bridge
#
  PeiHostBridgeInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/LibraryPrivate/PeiHostBridgeInitLib/PeiHostBridgeInitLib.inf
  PeiHostBridgePolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/LibraryPrivate/PeiHostBridgePolicyLib/PeiHostBridgePolicyLibVer3.inf

  UsbDeviceControllerInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Usb/LibraryPrivate/PeiUsbDeviceControllerInitLib/PeiUsbDeviceControllerInitLib.inf

#
# TCSS Fru Library
#
  PtlPcdTcssInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Tcss/LibraryPrivate/PtlPcdTcssInitLib/PtlPcdTcssInitLib.inf

  GbeSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdGbeLib/PtlPcdGbeLib.inf
  FiaSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdFiaLib/PtlPcdFiaLib.inf
  PeiBpkPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Bpk/LibraryPrivate/PeiBpkPrivateLib/PeiBpkPrivateLib.inf
  SerialIoUartSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PtlPcdPeiSerialIoUartLib.inf
  SerialIoI2cSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PtlPcdPeiSerialIoI2cLib.inf
  PeiLpssUartInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssUart/LibraryPrivate/PeiLpssUartInitLib/PeiLpssUartInitLib.inf
  PeiLpssSpiInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssSpi/LibraryPrivate/PeiLpssSpiInitLib/PeiLpssSpiInitLib.inf
  LpssI2cSocInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdLpssI2cSocLib/PtlPcdPeiLpssI2cSocInitLib.inf
  LpssI3cSocInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdLpssI3cSocLib/PtlPcdPeiLpssI3cSocInitLib.inf
  LpssSpiSocInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdLpssSpiSocLib/PtlPcdPeiLpssSpiSocInitLib.inf
  LpssUartSocInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdLpssUartSocLib/PtlPcdPeiLpssUartSocInitLib.inf

  #
  # Following NULL library entry is added to make sure build works, this will be removed once SerialIO
  # division into 4 IPs is complete.
  #
  SerialIoI3cSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PeiDxeSmmSerialIoI3cSocNullLib.inf
  SerialIoSpiSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PtlPcdPeiSerialIoSpiLib.inf
  PeiLpssI3cInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssI3c/LibraryPrivate/PeiI3cInitLib/PeiI3cInitLib.inf
  PeiLpssI2cInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssI2c/LibraryPrivate/PeiI2cInitLib/PeiI2cInitLib.inf

  PeiThermalLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Thermal/LibraryPrivate/PeiThermalLib/PeiThermalLib.inf
  PeiThermalPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Thermal/LibraryPrivate/PeiThermalPolicyLib/PeiThermalPolicyLib.inf
  PeiP2sbPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/LibraryPrivate/PeiP2sbPolicyLib/PeiP2sbPolicyLib.inf
  PeiP2sbPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/LibraryPrivate/P2sbPrivateLib/PeiP2sbPrivateLib.inf
  PeiEspiInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Espi/LibraryPrivate/PeiEspiInitLib/PeiEspiInitLib.inf
  PeiEspiPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Espi/LibraryPrivate/PeiEspiPolicyLib/PeiEspiPolicyLib.inf
  PeiIshInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ish/LibraryPrivate/PeiIshInitLib/PeiIshInitLib.inf
  PeiIshInitSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiIshInitLib/PtlPcdPeiIshInitLib.inf
  PeiIshPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ish/LibraryPrivate/PeiIshPolicyLib/PeiIshPolicyLib.inf
  PtlPcdPeiIshPolicyLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiIshPolicyLib/PtlPcdPeiIshPolicyLib.inf
  PeiPsfPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Psf/LibraryPrivate/PeiPsfPolicyLib/PeiPsfPolicyLib.inf
  PeiRtcLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Rtc/LibraryPrivate/PeiRtcLib/PeiRtcLib.inf
  PeiRtcPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Rtc/LibraryPrivate/PeiRtcPolicyLib/PeiRtcPolicyLib.inf
  PeiPmcPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Pmc/LibraryPrivate/PeiPmcPrivateLib2/PeiPmcPrivateLib.inf
  PeiPmcPolicyCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Pmc/LibraryPrivate/PeiPmcPolicyLib/PeiPmcPolicyCommonLib.inf
  PeiPmcPolicyLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiPmcPolicyLib/PtlPcdPeiPmcPolicyLib.inf
  PeiSmbusLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Smbus/LibraryPrivate/PeiSmbusLib/PeiSmbusLib.inf
  PeiSmbusPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Smbus/LibraryPrivate/PeiSmbusPolicyLib/PeiSmbusPolicyLib.inf
  PeiSpiLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/Library/PeiSpiLib/PeiSpiLib.inf
  !if gSiPkgTokenSpaceGuid.PcdSpiDmaEnable == TRUE
    PeiSpiDmaLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/LibraryPrivate/PeiSpiDmaLib/PeiSpiDmaLib.inf
  !else
    PeiSpiDmaLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/LibraryPrivate/PeiSpiDmaLibNull/PeiSpiDmaLibNull.inf
  !endif
  PeiItssPolicyCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Itss/LibraryPrivate/PeiItssPolicyLib/PeiItssPolicyCommonLib.inf
  PeiItssPolicyLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiItssPolicyLib/PtlPcdPeiItssPolicyLib.inf
  PeiItssLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Itss/LibraryPrivate/PeiItssLib/PeiItssLib.inf
  PeiScsLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Scs/LibraryPrivate/PeiScsLib/PeiScsLib.inf
  PeiScsPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Scs/LibraryPrivate/PeiScsPolicyLib/PeiScsPolicyLib.inf
  PeiAceInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Hda/LibraryPrivate/PeiAceInitLib/PeiAceInitLib.inf
  PeiHdaLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Hda/LibraryPrivate/PeiHdaLib/PeiHdaLib.inf
  PeiHdaPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Hda/LibraryPrivate/PeiHdaPolicyLib/PeiHdaPolicyLib.inf
  PtlPcdAceLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdHdaLib/PtlPcdAceLib.inf
  PeiPchDmiLib|$(PLATFORM_SI_PACKAGE)/IpBlock/PchDmi/LibraryPrivate/PeiDxeSmmPchDmiLib/PeiPchDmiLibNull.inf
  PeiPchDmiPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/PchDmi/LibraryPrivate/PeiPchDmiPolicyLib/PeiPchDmiPolicyLib.inf
  PeiIehInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ieh/LibraryPrivate/PeiIehInitLib/PeiIehInitLib.inf
  PeiIehPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ieh/LibraryPrivate/PeiIehPolicyLib/PeiIehPolicyLib.inf
  PeiPsfLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Psf/LibraryPrivate/PeiPsfLib/PeiPsfLib.inf
  HeciControlFilterLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciControl/HeciControlFilterLib/HeciControlFilterLib.inf
  MeUmaLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/LibraryPrivate/PeiMeUmaLib/PeiMeUmaLib.inf
  MeInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/LibraryPrivate/PeiMeInitLib/PeiMeInitLib.inf
  MeLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/PeiMeLib/PeiMeLib.inf
  MeUtilsLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/PeiDxeSmmMeUtilsLib/PeiMeUtilsLib.inf
  PeiMePolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/PeiMePolicyLib/PeiMePolicyLib.inf
  HeciAsyncSupportLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/LibraryPrivate/PeiDxeHeciAsyncSupportLibNull/PeiDxeHeciAsyncSupportLibNull.inf
  MeFruInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdMeInitLib/PtlPcdPeiMeInitLib.inf
  GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gpio/LibraryPrivate/PeiGpioHelpersLib/PeiGpioHelpersLib.inf
  PeiDciSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiDciLib/PtlPcdPeiDciLib.inf
  PeiDciInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dci/LibraryPrivate/PeiDciInitLib/PeiDciInitLib.inf
  PeiDciPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dci/LibraryPrivate/PeiDciPolicyLib/PeiDciPolicyLib.inf
  PeiTraceHubSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiTraceHubLib/PtlPcdPeiTraceHubLib.inf
  PtlPcdTcssPmcLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdTcssPmcLib/PtlPcdTcssPmcLib.inf
  PeiTraceHubInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/TraceHub/LibraryPrivate/PeiTraceHubInitLib/PeiTraceHubInitLib.inf
  TraceHubHookLib|$(SILICON_PRODUCT_PATH)/Library/TraceHubHookLib/PeiTraceHubHookLib.inf
  PeiCnviInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Cnvi/LibraryPrivate/PeiCnviInitLib/PeiCnviInitLib.inf
    PeiCnviPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Cnvi/LibraryPrivate/PeiCnviPolicyLib/PeiCnviPolicyLib.inf
  PeiGbeInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gbe/LibraryPrivate/PeiGbeInitLib/PeiGbeInitLib.inf
  PeiGbePolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gbe/LibraryPrivate/PeiGbePolicyLib/PeiGbePolicyLib.inf
  PeiTraceHubPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/TraceHub/LibraryPrivate/PeiTraceHubPolicyLib/PeiTraceHubPolicyLib.inf
  PeiTsnInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tsn/LibraryPrivate/PeiTsnInitLib/PeiTsnInitLib.inf
  PeiTsnPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tsn/LibraryPrivate/PeiTsnPolicyLib/PeiTsnPolicyLib.inf
  !if gSiPkgTokenSpaceGuid.PcdAmtEnable == TRUE
    PeiAmtPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/PeiAmtPolicyLib/PeiAmtPolicyLib.inf
  !else
    PeiAmtPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/PeiAmtPolicyLibNull/PeiAmtPolicyLibNull.inf
  !endif
  PeiUsbPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Usb/LibraryPrivate/PeiUsbPolicyLib/PeiUsbPolicyLib.inf
  PeiWdtPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Wdt/LibraryPrivate/PeiWdtPolicyLib/PeiWdtPolicyLib.inf
  ThcSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdThcLib/PtlPcdThcLib.inf
  PeiThcInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Thc/LibraryPrivate/PeiThcInitLib/PeiThcInitLib.inf
  PeiThcPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Thc/LibraryPrivate/PeiThcPolicyLib/PeiThcPolicyLib.inf
  PeiThcInitSsidLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiThcSsidInitLib/PtlPcdPeiThcInitSsidLib.inf
  PeiPchPcieClocksLibCommon|$(PLATFORM_SI_PACKAGE)/IpBlock/PcieClocks/LibraryPrivate/PeiPchPcieClocksLib/PeiPchPcieClocksLibCommon.inf
  PeiPchPcieClocksLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiPchPcieClocksLib/PtlPcdPeiPchPcieClocksLib.inf
  PeiFusaLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Fusa/LibraryPrivate/PeiFusaLibNull/PeiFusaLibNull.inf
  PeiFusaPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Fusa/LibraryPrivate/PeiFusaPolicyLibNull/PeiFusaPolicyLibNull.inf
  PchInitPreMemFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiPchInitPreMemFruLib/PtlPcdPeiPchInitPreMemFruLib.inf
  PeiLpssI3cPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssI3c/LibraryPrivate/PeiI3cPolicyLib/PeiI3cPolicyLib.inf
  PeiLpssSpiPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssSpi/LibraryPrivate/PeiLpssSpiPolicyLib/PeiLpssSpiPolicyLib.inf
  PeiLpssUartPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssUart/LibraryPrivate/PeiLpssUartPolicyLib/PeiLpssUartPolicyLib.inf
  PeiLpssI2cPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssI2c/LibraryPrivate/PeiI2cPolicyLib/PeiI2cPolicyLib.inf
  Usb2PhyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Usb/LibraryPrivate/PeiExternalUsb2PhyLib/PeiExternalUsb2PhyLib.inf
  PeiPcieSipInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/PcieRp/LibraryPrivate/PeiPcieSipInitLib/PeiPcieSipInitLib.inf
  PeiLpcLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Espi/LibraryPrivate/PeiLpcLib/PeiLpcLib.inf
  PtlPcdSerialIoPolicyLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PeiSerialIoPolicyLib/PeiSerialIoPolicyLibPtl.inf
  PtlPcdHeciDeviceConfigLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdHeciDeviceConfigLib/PtlPcdHeciDeviceConfigLib.inf
  CanSsInitLib|$(PLATFORM_SI_PACKAGE)/Subsystem/CanSs/CanSsInitLib/CanSsInitLib.inf

#
# IAX
#
  PeiIaxPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Iax/LibraryPrivate/PeiIaxPolicyLib/PeiIaxPolicyLib.inf
  IaxInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Iax/LibraryPrivate/PeiIaxInitLib/PeiIaxInitLib.inf

#
# GPIOV2
#
  GpioV2AccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/GpioV2/Library/GpioV2Services/GpioV2AccessPeiLib.inf
  PtlPcdSerialIoGpioAccessLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PtlPcdPeiSerialIoGpioAccessLib.inf

#
# Pch
#
  PtlPcdInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdInfoLib/PtlPcdPeiInfoLib.inf

#
#  VMD IpBlock
#
!if gSiPkgTokenSpaceGuid.PcdVmdEnable == TRUE
 PeiVmdInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/LibraryPrivate/PeiVmdInitLib/PeiVmdInitLib.inf
 PeiVmdPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/LibraryPrivate/PeiVmdPolicyLib/PeiVmdPolicyLib.inf
 PeiVmdInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Vmd/LibraryPrivate/PeiVmdInitFruLib/PeiVmdInitFruLib.inf
!else
 PeiVmdInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/LibraryPrivate/PeiVmdInitLibNull/PeiVmdInitLibNull.inf
 PeiVmdPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/LibraryPrivate/PeiVmdPolicyLibNull/PeiVmdPolicyLibNull.inf
 PeiVmdInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Vmd/LibraryPrivate/PeiVmdInitFruLibNull/PeiVmdInitFruLibNull.inf
!endif
