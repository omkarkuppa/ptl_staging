## @file
#  Component description file for the PantherLake SOC DXE FRU drivers.
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

!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
!if gSiPkgTokenSpaceGuid.PcdTseEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Tse/AcpiTables/TseAcpiTables.inf
!endif
!endif

  $(PLATFORM_SI_PACKAGE)/IpBlock/Wdt/Dxe/WdtDxe.inf
!if gSiPkgTokenSpaceGuid.PcdSmmVariableEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Spi/Smm/SpiSmm.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Spi/Dxe/SpiSmmStub.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Spi/Dxe/SpiSmmDxe.inf
!else
  $(PLATFORM_SI_PACKAGE)/IpBlock/Spi/RuntimeDxe/SpiRuntime.inf
!endif
  $(PLATFORM_SI_PACKAGE)/IpBlock/Smbus/SmbDriver/SmbusSmm.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Smbus/SmbDriver/SmbusDxe.inf

#
# ME
#
  $(PLATFORM_SI_PACKAGE)/IpBlock/Me/HeciInit/Dxe/HeciInitDxe.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Me/HeciInit/Smm/HeciInitSmm.inf {
    <LibraryClasses>
      HeciAsyncSupportLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/LibraryPrivate/PeiDxeHeciAsyncSupportLibNull/PeiDxeHeciAsyncSupportLibNull.inf
  }
  $(PLATFORM_SI_PACKAGE)/IpBlock/Me/MeFwDowngrade/Dxe/MeFwDowngrade.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Me/MeSmbiosDxe/MeSmbiosDxe.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Me/Asf/Dxe/AsfDxe.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Me/HwAsset/Dxe/HwAssetDxe.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Me/ResetHandler/Dxe/ResetHandler.inf

!if gSiPkgTokenSpaceGuid.PcdAmtEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Amt/AmtInit/Dxe/AmtInitDxe.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Mebx/Dxe/Mebx.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Sol/Dxe/SerialOverLan.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdThcEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Thc/QuickSpiDriver/QuickSpi.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Thc/QuickI2cDriver/QuickI2c.inf
!endif

#
# GPIOv2
#
$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/SocInit/GpioV2ServicesInit/DxeSmm/GpioV2PtlPcdProtocolInitDxe.inf
$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/SocInit/GpioV2ServicesInit/DxeSmm/GpioV2PtlPcdProtocolInitSmm.inf