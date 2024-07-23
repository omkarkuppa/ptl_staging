## @file
#  CnvFeaturePkgPchInit.dsc
#  Default Interface PCD files of CNV Feature Package
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2022 Intel Corporation.
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
# @par Specification
#
##

#
# Include CNV Board Configuration related PCD Init file.
#
[PcdsDynamicExDefault.common.DEFAULT]
  #
  # Table for Structural PCD Of CNV Board Configuration
  #
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvBoardConfig|{CODE(
  {
    {
      0,                     // Revision
      0,                     // WlanRootPortNumber
      0,                     // DiscreteUsbBtPortNumber
      0,                     // IntegratedUsbBtPortNumber
      0,                     // WlanRfKillGpio
      0,                     // WlanWakeGpio
      0,                     // WlanRstGpio
      0,                     // BtRfKillGpio
      0                      // BtIrqGpio
    }
  })}

[PcdsFixedAtBuild]
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelWifiDsmFunc1    |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelWifiDsmFunc2    |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelWifiDsmFunc4    |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelWifiDsmFunc5    |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdOemWifiDsmFunc11     |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelBtDsmFunc1      |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelBtDsmFunc2      |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelBtDsmFunc3      |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelBtDsmFunc4      |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelBtDsmFunc5      |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelBtDsmFunc6      |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelBtDsmFunc7      |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdIntelBtTileDsmFunc1  |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdSupportedWifi1       |0x25268086
  gCnvFeaturePkgTokenSpaceGuid.PcdSupportedWifi2       |0x271B8086
  gCnvFeaturePkgTokenSpaceGuid.PcdSupportedWifi3       |0x27238086
  gCnvFeaturePkgTokenSpaceGuid.PcdSupportedWifi4       |0x27258086
  gCnvFeaturePkgTokenSpaceGuid.PcdSupportedWifi5       |0x272B8086
  gCnvFeaturePkgTokenSpaceGuid.PcdSupportedWifi6       |0
  gCnvFeaturePkgTokenSpaceGuid.PcdSupportedWifi7       |0
  gCnvFeaturePkgTokenSpaceGuid.PcdSupportedWifi8       |0
  gCnvFeaturePkgTokenSpaceGuid.PcdSupportedWifi9       |0
  gCnvFeaturePkgTokenSpaceGuid.PcdSupportedWifi10      |0
  gCnvFeaturePkgTokenSpaceGuid.PcdMaxRootPortNumber    |6
  gCnvFeaturePkgTokenSpaceGuid.PcdMaxUsb2PortNumber    |14
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvSetupMenu         |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvAcpiTables        |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvUefiVariables     |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvUefiVarVersion    |0x0002