## @file
#  Component description file for the SiPkg DXE drivers.
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
# Silicon
#
$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/Smm/SpiSmm.inf
$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/SocInit/GpioV2ServicesInit/DxeSmm/GpioV2PtlPcdProtocolInitDxe.inf
$(PLATFORM_SI_PACKAGE)/Pch/SmmControl/RuntimeDxe/SmmControl.inf
$(PLATFORM_SI_PACKAGE)/Pch/PchSmiDispatcher/Smm/PchSmiDispatcher.inf
$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/SocInit/GpioV2ServicesInit/Pei/GpioV2PtlPcdPpiInitPei.inf

!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/DomainIGpu/AcpiTables/IGpuSsdt.inf
!endif
