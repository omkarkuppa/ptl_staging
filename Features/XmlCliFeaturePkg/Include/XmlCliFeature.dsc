## @file
# This is a build description for the XmlCli advanced feature.
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
##

[Packages]
  XmlCliFeaturePkg/XmlCliFeaturePkg.dec

################################################################################
#
# Library Class section - list of all Library Classes needed by this feature.
#
################################################################################
[LibraryClasses.X64]
  XmlCliCommonLib|XmlCliFeaturePkg/LibraryPrivate/XmlCliCommonLib/XmlCliCommonLib.inf


[LibraryClasses.X64.MM_STANDALONE]
!if gXmlCliFeaturePkgTokenSpaceGuid.PcdStandaloneMmXmlCliEnable == TRUE
  XmlCliCommonLib|HybridMmPkg/XmlCliFeaturePkg/LibraryPrivate/StandaloneMmXmlCliCommonLib/StandaloneMmXmlCliCommonLib.inf

!endif
################################################################################
#
# Component section - list of all components that need built for this feature.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
################################################################################
[Components.$(FSP_ARCH)]
  #####################################
  # XmlCli Feature Package
  #####################################
  XmlCliFeaturePkg/XmlCliCommon/Pei/XmlCliCommonPei.inf

[Components.X64]
  #####################################
  # XmlCli Feature Package
  #####################################
  XmlCliFeaturePkg/XmlCliCommon/Dxe/XmlCliCommonDxe.inf {
    <LibraryClasses>
      RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
    <Packages>
      BoardModulePkg/BoardModulePkg.dec
  }

!if gXmlCliFeaturePkgTokenSpaceGuid.PcdStandaloneMmXmlCliEnable == TRUE
    HybridMmPkg/XmlCliFeaturePkg/XmlCliCommon/StandaloneMm/XmlCliCommonDxe.inf
    HybridMmPkg/XmlCliFeaturePkg/XmlCliCommon/StandaloneMm/XmlCliCommonStandaloneMm.inf
!else
    XmlCliFeaturePkg/XmlCliCommon/Smm/XmlCliCommonSmm.inf
!endif



[PcdsFixedAtBuild]
  # XmlCli Feature Package Token Space defining Default Setup Values


  # PCD values for setting default values for setup knobs
  gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliSupport|0x0
  gXmlCliFeaturePkgTokenSpaceGuid.PcdEnableXmlCliLite|0x0
  gXmlCliFeaturePkgTokenSpaceGuid.PcdDramPublishToCmos|0xF0

  gXmlCliFeaturePkgTokenSpaceGuid.PcdLiteXmlReserved|0xC0000

!if gXmlCliFeaturePkgTokenSpaceGuid.PcdPlatformXmlCli == 0x2
  gXmlCliFeaturePkgTokenSpaceGuid.PcdPublishSetupPgPtr|0x0
  # PCD values for reserving memory size
  gXmlCliFeaturePkgTokenSpaceGuid.PcdKnobsEntriesSize|0x60000
  gXmlCliFeaturePkgTokenSpaceGuid.PcdKnobValueMapSize|0x60000
  gXmlCliFeaturePkgTokenSpaceGuid.PcdKnobPatchDataBufferSize|0x60000

  !if $(MAX_SOCKET) == 8
    # 22 MB reserved, 19 MB XML + 2.5 MB Dram Shared MB
    gXmlCliFeaturePkgTokenSpaceGuid.PcdGbtXmlReserved|0x1600000
  !elseif $(MAX_SOCKET) == 4
    # 18 MB reserved, 15 MB XML + 2.5 MB Dram Shared MB
    gXmlCliFeaturePkgTokenSpaceGuid.PcdGbtXmlReserved|0x1200000
  !else
    ## 16 MB reserved, 13 MB XML + 2.5 MB Dram Shared MB
    gXmlCliFeaturePkgTokenSpaceGuid.PcdGbtXmlReserved|0x1000000
  !endif

  !if $(TARGET) != RELEASE
    gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliSupport|0x1
  !endif
!else
  gXmlCliFeaturePkgTokenSpaceGuid.PcdPublishSetupPgPtr|0x1
!endif
