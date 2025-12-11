## @file
#  Component description file for the SiPkg DXE drivers.
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

!include $(PLATFORM_SI_PACKAGE)/UniversalDxe.dsc
#
# FRUs
#
!include $(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Dxe.dsc
!include $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Dxe.dsc

#
# Common
#
  $(PLATFORM_SI_PACKAGE)/Product/PantherLake/SiInit/Dxe/SiInitDxe.inf

#
# ME
#
!include $(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciDriverDxe.dsc
!include $(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciDriverSmm.dsc

#
# Pch
#
!if gSiPkgTokenSpaceGuid.PcdThcEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Thc/ThcDriver/Thc.inf
!endif

  $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/SocInit/Dxe/PtlPcdPchInitDxe.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/LpssI2c/I2cDriver/LpssI2cDriver.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/LpssUart/LpssUartDriver/LpssUartDriver.inf
  $(PLATFORM_SI_PACKAGE)/Pch/SmmControl/RuntimeDxe/SmmControl.inf

  $(PLATFORM_SI_PACKAGE)/Pch/PchSmiDispatcher/Smm/PchSmiDispatcher.inf{
    <LibraryClasses>
      #SmiHandlerProfileLib|MdeModulePkg/Library/SmmSmiHandlerProfileLib/SmmSmiHandlerProfileLib.inf
      SmiHandlerProfileLib|MdePkg/Library/SmiHandlerProfileLibNull/SmiHandlerProfileLibNull.inf
  }
  $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/SocInit/Smm/PtlPcdPchInitSmm.inf


#
# SystemAgent
#

  $(PLATFORM_SI_PACKAGE)/Bdat/Dxe/BdatAccessHandler.inf

!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/DomainIGpu/AcpiTables/IGpuSsdt.inf
!if gSiPkgTokenSpaceGuid.PcdIpuEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Ipu/AcpiTables/IpuSsdt.inf
!endif
  $(PLATFORM_SI_PACKAGE)/Subsystem/Tcss/AcpiTables/TcssSsdt/Ver1/TcssSsdt.inf


#
# Security
#
!if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Txt/AcpiTables/TxtSsdt.inf
!endif
!if gSiPkgTokenSpaceGuid.PcdTseEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Tse/AcpiTables/TseAcpiTables.inf
!endif

!endif

#
# Cpu
#
  $(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/Dxe/CpuInitDxe.inf {
    <BuildOptions>
!if gSiPkgTokenSpaceGuid.PcdSourceDebugEnable == TRUE
      *_*_*_CC_FLAGS       = -DSOURCE_DEBUG_ENABLE
!endif
  }

!if gSiPkgTokenSpaceGuid.PcdBiosGuardEnable == TRUE
!if gSiPkgTokenSpaceGuid.PcdBiosGuardLegacyAcpi == FALSE
  $(PLATFORM_SI_PACKAGE)/IpBlock/BiosGuard/Dxe/BiosGuardAcpiInit.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/BiosGuard/AcpiTables/BiosGuardAcpiTables.inf
!endif
  $(PLATFORM_SI_PACKAGE)/IpBlock/BiosGuard/Smm/BiosGuardServices.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdPpamEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Dgr/Dxe/DgrDxeInit.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/CpuPowerManagement/AcpiTables/CpuAcpiTables.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdPpmEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/CpuPowerManagement/Dxe/DxeCpuPowerManagement.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Txt/Dxe/TxtDxe.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdTseEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Tse/TseDxe/TseDxe.inf
!endif

#
# TBT
#
!if gSiPkgTokenSpaceGuid.PcdTcssSupport == TRUE
    $(PLATFORM_SI_PACKAGE)/IpBlock/Tbt/ITbtInit/Dxe/ITbtDxe.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Include/AcpiTables/Soc/PtlPcdGpeSsdt.inf
  $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Include/AcpiTables/Soc/PtlPcdCommonSsdt.inf
!endif
