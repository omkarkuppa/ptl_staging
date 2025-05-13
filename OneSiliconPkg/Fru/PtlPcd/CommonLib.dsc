## @file
#  Component description file for the PantherLake SOC Common FRU libraries.
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

HostBridgeDataLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/Library/HostBridgeDataLib/HostBridgeDataLib.inf
CpuPcieInfoFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/CpuPcieRp/Library/PtlPcdPeiDxeSmmCpuPcieInfoFruLibNull/PtlPcdPeiDxeSmmCpuPcieInfoFruLibNull.inf
BaseTraceHubInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/TraceHub/Library/BaseTraceHubInfoLib/BaseTraceHubInfoLib.inf
PsfLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Psf/LibraryPrivate/PsfLib/PeiDxeSmmPsfLib.inf
	PsfSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiDxeSmmPsfLib/PtlPcdPeiDxeSmmPsfLib.inf
P2sbPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/LibraryPrivate/P2sbPrivateLib/P2sbPrivateLib.inf
P2SbSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdP2SbLib/PtlPcdP2SbLib.inf
PchPcrLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/Library/PeiDxeSmmPchPcrLib/PeiDxeSmmPchPcrLib.inf
P2SbSidebandAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/P2sb/LibraryPrivate/PeiDxeSmmP2SbSidebandAccessLib/PeiDxeSmmP2SbSidebandAccessLib.inf
PcieConfigAccessLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PcieConfigAccessLib/PcieConfigAccessLib.inf
GbeMdiLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gbe/LibraryPrivate/PeiDxeSmmGbeMdiLib/PeiDxeSmmGbeMdiLib.inf
GbeLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gbe/Library/PeiDxeSmmGbeLib/PeiDxeSmmGbeLib.inf
TsnLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tsn/LibraryPrivate/PeiDxeSmmTsnLib/PeiDxeSmmTsnLib.inf
EspiPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Espi/LibraryPrivate/PeiDxeSmmEspiPrivateLib/PeiDxeSmmEspiPrivateLib.inf
DxeEspiPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Espi/LibraryPrivate/DxeEspiPrivateLib/DxeEspiPrivateLib.inf
EspiLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdPeiDxeSmmEspiLib/PtlPcdPeiDxeSmmEspiLib.inf
ReserveMmio64SizeFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdReserveMmio64SizeFruLib/PtlPcdReserveMmio64SizeFruLib.inf
MmioAccessLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/MmioAccessLib/MmioAccessLib.inf
BaseImrInfoLib|$(PLATFORM_SI_PACKAGE)/Library/BaseImrInfoLib/BaseImrInfoLib.inf
PchFiaLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Fia/LibraryPrivate/PeiPchFiaLib/PeiPchFia19Lib.inf

PmcLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Pmc/Library/PeiDxeSmmPmcLib/PeiDxeSmmPmcLib.inf
TcoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Smbus/Library/PeiDxeSmmTcoLib/PeiDxeSmmTcoLib.inf
PmcPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Pmc/LibraryPrivate/PeiDxeSmmPmcPrivateLib/PeiDxeSmmPmcPrivateLib.inf
PmcSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPeiDxeSmmPmcLib/PtlPcdPeiDxeSmmPmcLib.inf
PmcPrivateLibWithS3|$(PLATFORM_SI_PACKAGE)/IpBlock/Pmc/LibraryPrivate/PeiDxeSmmPmcPrivateLib/PeiDxeSmmPmcPrivateLibWithS3.inf
SmbusInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Smbus/Library/PeiDxeSmmSmbusInfoLib/SmbusInfoLib.inf
SpiCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/LibraryPrivate/BaseSpiCommonLib/BaseSpiCommonLib.inf
SmbusLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Smbus/LibraryPrivate/BaseSmbusLib/BaseSmbusLib.inf
SmbusCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Smbus/LibraryPrivate/PeiDxeSmmSmbusCommonLib/PeiDxeSmmSmbusCommonLib.inf
WdtCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Wdt/LibraryPrivate/PeiDxeSmmWdtCommonLib/PeiDxeSmmWdtCommonLib.inf
!if gSiPkgTokenSpaceGuid.PcdOcWdtEnable == TRUE
OcWdtLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Wdt/Library/PeiOcWdtLib/PeiOcWdtLib.inf
!else
OcWdtLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Wdt/Library/PeiOcWdtLibNull/PeiOcWdtLibNull.inf
!endif
PeiWdtLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Wdt/LibraryPrivate/PeiWdtLib/PeiWdtLib.inf
PchDmiAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/PchDmi/Library/PeiDxeSmmPchDmiAccessLib/PeiDxeSmmPchDmiAccessLib.inf
PchDmiLib|$(PLATFORM_SI_PACKAGE)/IpBlock/PchDmi/LibraryPrivate/PeiDxeSmmPchDmiLib/PeiDxeSmmPchDmiLib.inf
PchDmiWithS3Lib|$(PLATFORM_SI_PACKAGE)/IpBlock/PchDmi/LibraryPrivate/PeiDxeSmmPchDmiLib/PeiDxeSmmPchDmiWithS3Lib.inf
IocAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ioc/Library/PeiDxeSmmIocAccessLib/PeiDxeSmmIocAccessLib.inf
IocLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ioc/LibraryPrivate/IocLib/PeiDxeSmmIocLib.inf
IocRegAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Ioc/LibraryPrivate/IocRegAccessLib/IocRegAccessLib.inf

SataSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdSataLib/PtlPcdSataLib.inf

SpiAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/Library/PeiDxeSmmSpiAccessLib/PeiDxeSmmSpiAccessLib.inf
DciPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dci/LibraryPrivate/PeiDxeSmmDciPrivateLib/PeiDxeSmmDciPrivateLib.inf
CnviLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdCnviLib/PtlPcdCnviLib.inf
ItssPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Itss/LibraryPrivate/PeiDxeSmmItssPrivateLib/PeiDxeSmmItssPrivateLib.inf
ItssLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdPeiDxeSmmItssLib/PtlPcdPeiDxeSmmItssLib.inf
ThermalAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Thermal/Library/PeiDxeSmmThermalAccessLib/PeiDxeSmmThermalAccessLib.inf
RtcPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Rtc/LibraryPrivate/PeiDxeSmmRtcPrivateLib/PeiDxeSmmRtcPrivateLib.inf
RtcAccessLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdPeiDxeSmmRtcAccessLib/PtlPcdPeiDxeSmmRtcAccessLib.inf
TraceHubHookLib|$(SILICON_PRODUCT_PATH)/Library/TraceHubHookLib/BaseTraceHubHookLib.inf
PtlPcdPcieRpSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPcieRpLib/PtlPcdPcieRpLib.inf
PchPcieRpLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdPcieRpLib/PtlPcdPcieRpLib.inf
PciExpressHelpersLib|$(PLATFORM_SI_PACKAGE)/IpBlock/PcieRp/LibraryPrivate/PciExpressHelpersLibrary/PeiDxeSmmPciExpressHelpersLib.inf
BasePcieHelperLib|$(PLATFORM_SI_PACKAGE)/IpBlock/PcieRp/Library/BasePcieHelperLib/BasePcieHelperLib.inf
RstCrLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Rst/Library/PeiDxeSmmRstCrLib/PeiDxeSmmRstCrLibVer3.inf
NpuInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Npu/Library/PeiDxeSmmNpuInfoLib/NpuInfoLib.inf
PchInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdPeiDxeSmmPchInfoLib/PtlPcdPeiDxeSmmPchInfoLib.inf
SocInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdInfoLib/PtlPcdInfoLib.inf
IoTrapSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdIoTrapLib/PtlPcdIoTrapLib.inf
PchCycleDecodingLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdPeiDxeSmmPchCycleDecodingLib/PtlPcdPeiDxeSmmPchCycleDecodingLib.inf
HsioSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdHsioLib/PtlPcdHsioLib.inf
ChipsetInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Hsio/LibraryPrivate/ChipsetInitLib/ChipsetInitLib.inf
SerialIoAccessLib|$(PLATFORM_SI_PACKAGE)/IpBlock/SerialIo/Library/PeiDxeSmmSerialIoAccessLib/PeiDxeSmmSerialIoAccessLib.inf
SerialIoPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/SerialIo/LibraryPrivate/PeiDxeSmmSerialIoPrivateLib/PeiDxeSmmSerialIoPrivateLib.inf
SerialIoI2cMasterCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/SerialIo/I2c/LibraryPrivate/PeiDxeSerialIoI2cMasterCommonLib/PeiDxeSerialIoI2cMasterCommonLib.inf
SerialIoI2cLib|$(PLATFORM_SI_PACKAGE)/IpBlock/SerialIo/I2c/Library/BaseSerialIoI2cLib/BaseSerialIoI2cLib.inf
SerialIoI2cPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/SerialIo/I2c/LibraryPrivate/SerialIoI2cPrivateLib/SerialIoI2cPrivateLib.inf
SerialIoSpiLib|$(PLATFORM_SI_PACKAGE)/IpBlock/SerialIo/Spi/Library/BaseSerialIoSpiLib/BaseSerialIoSpiLib.inf
SpiSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSpiLib/PtlPcdSpiLib.inf
SpiInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/LibraryPrivate/SpiInitLib/SpiInitLib.inf

#
# LPSS
#
LpssInitLib|$(PLATFORM_SI_PACKAGE)/Subsystem/Lpss/LpssLib/LpssInitLib.inf
LpssUartLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssUart/Library/LpssUartLib/PeiLpssUartLib.inf
LpssI3cPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssI3c/LibraryPrivate/PeiDxeSmmI3cPrivateLib/PeiDxeSmmI3cPrivateLib.inf
LpssControllerInfoLib | $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/LpssControllerInfoLib/PtlPcdLpssControllerInfoLib.inf
LpssSpiBaseLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssSpi/Library/LpssSpiBaseLib/LpssSpiBaseLib.inf
LpssSpiPrivatelib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssSpi/LibraryPrivate/LpssSpiPrivateLib/LpssSpiPrivateLib.inf
LpssUartDebugPropertyPcdLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssUart/Library/LpssUartDebugPropertyPcdLib/LpssUartDebugPropertyPcdLib.inf
LpssI2cPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssI2c/LibraryPrivate/PeiDxeSmmLpssI2cPrivateLib/PeiDxeSmmLpssI2cPrivateLib.inf

#
# GPIOV2
#
GpioV2ServicesCommonLib|$(PLATFORM_SI_PACKAGE)/IpBlock/GpioV2/Library/GpioV2Services/Common/GpioV2ServicesCommonLib.inf
PtlPcdGpioTopologyLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdGpioTopology/PtlPcdGpioTopology.inf
PtlPcdGpioNativeLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdGpioNativeLib/PtlPcdGpioNativeLib.inf
PtlPcdSecGpioTopology|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdGpioTopology/PtlPcdSecGpioTopology.inf
#
# ME
#
HeciAccessCoreLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciAccess/HeciAccessCoreLib/HeciAccessCoreLib.inf
HeciTraceLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciTransport/HeciTraceLib/HeciTraceLib.inf
HeciControlCoreLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciControl/HeciControlCoreLib/HeciControlCoreLib.inf

MeInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdMeLib/PtlPcdMeLib.inf
MeChipsetLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/PeiDxeMeChipsetLib/PeiDxeMeChipsetLib.inf
MeShowBufferLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/PeiDxeMeShowBufferLib/PeiDxeMeShowBufferLib.inf
MeFwStsLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/BaseMeFwStsLib/BaseMeFwStsLib.inf
HeciInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/LibraryPrivate/PeiDxeHeciInitLib/PeiDxeHeciInitLib.inf

IpUsb3Lib|$(PLATFORM_SI_PACKAGE)/IpBlock/Usb/LibraryPrivate/IpUsb3/IpUsb3.inf
IpWrapperLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/BaseIpWrapperLib/BaseIpWrapperLib.inf