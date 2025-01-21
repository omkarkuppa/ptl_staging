## @file
#  Definition of Foxville Non-Volatile Memory Map Common header.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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
#  @par Specification
#    - Foxville Ethernet Controller EAS, revision 1.0.4
#
##

from typing import Any, Dict, List

from CapsuleCommon.Format.HexadecimalLib import *
from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

#
# Define of Foxville firmware Structure Member.
#
FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_0                 : str = 'EthernetIndividualAddress0'
FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_1                 : str = 'EthernetIndividualAddress1'
FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_2                 : str = 'EthernetIndividualAddress2'
FXVL_FW_HDR_COMPATIBILITY                                 : str = 'Compatibility'
FXVL_FW_HDR_OEM_LED_CFG                                   : str = 'OemLedCfg'
FXVL_FW_HDR_DEV_STARTER_VER                               : str = 'DevStarterVer'
FXVL_FW_HDR_OEM_CFG_1                                     : str = 'OemCfg1'
FXVL_FW_HDR_OEM_CFG_2                                     : str = 'OemCfg2'
FXVL_FW_HDR_PBA_NUM_0                                     : str = 'PbaNum0'
FXVL_FW_HDR_PBA_NUM_1                                     : str = 'PbaNum1'
FXVL_FW_HDR_INIT_CTRL_WORD_1                              : str = 'InitCtrlWord1'
FXVL_FW_HDR_SUBSYSTEM_ID                                  : str = 'SubsystemId'
FXVL_FW_HDR_SUBSYSTEM_VENDOR_ID                           : str = 'SubsystemVendorId'
FXVL_FW_HDR_DEVICE_ID                                     : str = 'DeviceId'
FXVL_FW_HDR_VENDOR_ID                                     : str = 'VendorId'
FXVL_FW_HDR_INIT_CTRL_WORD_2                              : str = 'InitCtrlWord2'
FXVL_FW_HDR_FW_IMG_PTR                                    : str = 'FwImagePtr'
FXVL_FW_HDR_FLASH_DEVICE_SIZE                             : str = 'FlashDeviceSize'
FXVL_FW_HDR_EEPROM_SIZING_AND_PROTECTED                   : str = 'EepromSizingAndProtected'
FXVL_FW_HDR_INIT_CTRL_WORD_4                              : str = 'InitCtrlWord4'
FXVL_FW_HDR_PCIE_L1_EXIT_LATENCIES                        : str = 'PcieL1ExitLatencies'
FXVL_FW_HDR_PCIE_COMPLETION_TIMEOUT_CFG                   : str = 'PcieCompletionTimeoutCfg'
FXVL_FW_HDR_MSI_X_CFG                                     : str = 'MsiXCfg'
FXVL_FW_HDR_SW_RESET_CSR_AUTO_CFG_PTR                     : str = 'SwResetCsrAutoCfgPtr'
FXVL_FW_HDR_PCIE_INIT_CFG_1                               : str = 'PcieInitCfg1'
FXVL_FW_HDR_PCIE_INIT_CFG_2                               : str = 'PcieInitCfg2'
FXVL_FW_HDR_PCIE_INIT_CFG_3                               : str = 'PcieInitCfg3'
FXVL_FW_HDR_PCIE_CTRL_1                                   : str = 'PcieCtrl1'
FXVL_FW_HDR_LED_1_3_CFG_DEFAULT                           : str = 'Led13CfgDefault'
FXVL_FW_HDR_DUMMY_DEVICE_ID                               : str = 'DummyDeviceId'
FXVL_FW_HDR_DEVICE_REV_ID                                 : str = 'DeviceRevId'
FXVL_FW_HDR_LED_0_2_CFG_DEFAULT                           : str = 'Led02CfgDefault'
FXVL_FW_HDR_SW_DEFINED_PINS_CTRL                          : str = 'SwDefinedPinsCtrl'
FXVL_FW_HDR_FUNC_CTRL                                     : str = 'FuncCtrl'
FXVL_FW_HDR_LAN_PWR_CONSUMPTION                           : str = 'LanPwrConsumption'
FXVL_FW_HDR_PCIE_RESET_CSR_AUTO_CFG_PTR                   : str = 'PcieResetCsrAutoCfgPtr'
FXVL_FW_HDR_INIT_CTRL_WORD_3                              : str = 'InitCtrlWord3'
FXVL_FW_HDR_RSVD_1                                        : str = 'Rsvd1'
FXVL_FW_HDR_RSVD_2                                        : str = 'Rsvd2'
FXVL_FW_HDR_CSR_AUTO_CFG_PWR_UP_PTR                       : str = 'CsrAutoCfgPwrUpPtr'
FXVL_FW_HDR_PCIE_CTRL_2                                   : str = 'PcieCtrl2'
FXVL_FW_HDR_PCIE_CTRL_3                                   : str = 'PcieCtrl3'
FXVL_FW_HDR_CDQM_MEMORY_BASE_LOW                          : str = 'CdqmMemoryBaseLow'
FXVL_FW_HDR_CDQM_MEMORY_BASE_HIGH                         : str = 'CdqmMemoryBaseHigh'
FXVL_FW_HDR_END_OF_RO_AREA_PTR                            : str = 'EndOfRoAreaPtr'
FXVL_FW_HDR_START_OF_RO_AREA_PTR                          : str = 'StartOfRoAreaPtr'
FXVL_FW_HDR_WATCHDOG_CFG                                  : str = 'WatchdogCfg'
FXVL_FW_HDR_VPD_PTR                                       : str = 'VpdPtr'
FXVL_FW_HDR_SETUP_OPTS_PCI_FUNC                           : str = 'SetupOptsPciFunc'
FXVL_FW_HDR_CFG_CUSTOMIZATION_OPTS_PCI_FUNC               : str = 'CfgCustomizationOptsPciFunc'
FXVL_FW_HDR_PXE_VER                                       : str = 'PxeVer'
FXVL_FW_HDR_IBA_CAPS                                      : str = 'IbaCaps'
FXVL_FW_HDR_PCIE_PHY_CFG_0_LOW                            : str = 'PciePhyCfg0Low'
FXVL_FW_HDR_PCIE_PHY_CFG_0_HIGH                           : str = 'PciePhyCfg0High'
FXVL_FW_HDR_ISCSI_OPT_ROM_VER                             : str = 'IscsiOptRomVer'
FXVL_FW_HDR_ALTERNATE_MAC_ADDRESS_LOC                     : str = 'AlternateMacAddressLoc'
FXVL_FW_HDR_PCIE_PHY_CFG_1_LOW                            : str = 'PciePhyCfg1Low'
FXVL_FW_HDR_PCIE_PHY_CFG_1_HIGH                           : str = 'PciePhyCfg1High'
FXVL_FW_HDR_PCIE_PHY_CFG_2_LOW_AND_RESET_TO_PCIE_PHY_DELAY: str = 'PciePhyCfg2LowAndResetToPciePhyDelay'
FXVL_FW_HDR_RSVD_3                                        : str = 'Rsvd3'
FXVL_FW_HDR_PXE_VLAN_PTR                                  : str = 'PxeVlanPtr'
FXVL_FW_HDR_ISCSI_BOOT_CFG_PTR                            : str = 'IscsiBootCfgPtr'
FXVL_FW_HDR_RSVD_4                                        : str = 'Rsvd4'
FXVL_FW_HDR_CHECKSUM                                      : str = 'Checksum'
FXVL_FW_HDR_FREE_PROV_AREA_PTR                            : str = 'FreeProvAreaPtr'
FXVL_FW_HDR_FREE_PROV_AREA_SIZE                           : str = 'FreeProvAreaSize'
FXVL_FW_HDR_IMG_UNIQUE_ID_0                               : str = 'ImageUniqueId0'
FXVL_FW_HDR_IMG_UNIQUE_ID_1                               : str = 'ImageUniqueId1'
FXVL_FW_HDR_PCIE_L1_SUB_STATES_CAP_LOW                    : str = 'PcieL1SubStatesCapLow'
FXVL_FW_HDR_PCIE_L1_SUB_STATES_CAP_HIGH                   : str = 'PcieL1SubStatesCapHigh'
FXVL_FW_HDR_PCIE_L1_SUB_STATES_CTRL_1_LOW                 : str = 'PcieL1SubStatesCtrl1Low'
FXVL_FW_HDR_PCIE_L1_SUB_STATES_CTRL_1_HIGH                : str = 'PcieL1SubStatesCtrl1High'
FXVL_FW_HDR_PCIE_L1_SUB_STATES_CTRL_2                     : str = 'PcieL1SubStatesCtrl2'
FXVL_FW_HDR_PTM_SETTING                                   : str = 'PtmSetting'
FXVL_FW_HDR_EXP_ROM_BOOT_CODE_SEC_PTR                     : str = 'ExpRomBootCodeSecPtr'
FXVL_FW_HDR_ULP_CAP_ENABLE                                : str = 'UlpCapEnable'
FXVL_FW_HDR_RSVD_5                                        : str = 'Rsvd5'
FXVL_FW_HDR_RSVD_6                                        : str = 'Rsvd6'
FXVL_FW_HDR_QBV_MNG_WATCHDOG_LOW                          : str = 'QbvMngWatchdogLow'
FXVL_FW_HDR_QBV_MNG_WATCHDOG_HIGH                         : str = 'QbvMngWatchdogHigh'
FXVL_FW_HDR_RO_CMDS_VER                                   : str = 'RoCmdsVer'
FXVL_FW_HDR_FW_MODULE_CFG_PTR                             : str = 'FwModuleCfgPtr'
FXVL_FW_HDR_PCIE_PHY_CFG_3_LOW                            : str = 'PciePhyCfg3Low'
FXVL_FW_HDR_PCIE_PHY_CFG_3_HIGH                           : str = 'PciePhyCfg3High'
FXVL_FW_HDR_PCIE_PHY_CFG_4_LOW                            : str = 'PciePhyCfg4Low'
FXVL_FW_HDR_PCIE_PHY_CFG_4_HIGH                           : str = 'PciePhyCfg4High'
FXVL_FW_HDR_PCIE_PHY_CFG_5_LOW                            : str = 'PciePhyCfg5Low'
FXVL_FW_HDR_PCIE_PHY_CFG_5_HIGH                           : str = 'PciePhyCfg5High'

#
# Pre-defined string.
#
DEFAULT_FXVL_FW_ETHERNET_INDIVIDUAL_ADDRESS: int = 0x0000_00C9_A000
DEFAULT_FXVL_FW_SUBSYSTEM_ID               : int = 0x0000
DEFAULT_FXVL_FW_SUBSYSTEM_VENDOR_ID        : int = 0x8086

FXVL_FW_HDR_FORMAT_DICT: Dict[str, Any] = {
    FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_0                 : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_1                 : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_2                 : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_COMPATIBILITY                                 : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_OEM_LED_CFG                                   : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_DEV_STARTER_VER                               : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_OEM_CFG_1                                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_OEM_CFG_2                                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PBA_NUM_0                                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PBA_NUM_1                                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_INIT_CTRL_WORD_1                              : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_SUBSYSTEM_ID                                  : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_SUBSYSTEM_VENDOR_ID                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_DEVICE_ID                                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_VENDOR_ID                                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_INIT_CTRL_WORD_2                              : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_FW_IMG_PTR                                    : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_FLASH_DEVICE_SIZE                             : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_EEPROM_SIZING_AND_PROTECTED                   : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_INIT_CTRL_WORD_4                              : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_L1_EXIT_LATENCIES                        : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_COMPLETION_TIMEOUT_CFG                   : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_MSI_X_CFG                                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_SW_RESET_CSR_AUTO_CFG_PTR                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_INIT_CFG_1                               : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_INIT_CFG_2                               : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_INIT_CFG_3                               : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_CTRL_1                                   : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_LED_1_3_CFG_DEFAULT                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_DUMMY_DEVICE_ID                               : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_DEVICE_REV_ID                                 : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_LED_0_2_CFG_DEFAULT                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_SW_DEFINED_PINS_CTRL                          : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_FUNC_CTRL                                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_LAN_PWR_CONSUMPTION                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_RESET_CSR_AUTO_CFG_PTR                   : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_INIT_CTRL_WORD_3                              : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_RSVD_1                                        : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_RSVD_2                                        : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_CSR_AUTO_CFG_PWR_UP_PTR                       : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_CTRL_2                                   : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_CTRL_3                                   : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_CDQM_MEMORY_BASE_LOW                          : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_CDQM_MEMORY_BASE_HIGH                         : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_END_OF_RO_AREA_PTR                            : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_START_OF_RO_AREA_PTR                          : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_WATCHDOG_CFG                                  : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_VPD_PTR                                       : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_SETUP_OPTS_PCI_FUNC                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_CFG_CUSTOMIZATION_OPTS_PCI_FUNC               : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PXE_VER                                       : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_IBA_CAPS                                      : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_0_LOW                            : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_0_HIGH                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_ISCSI_OPT_ROM_VER                             : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_ALTERNATE_MAC_ADDRESS_LOC                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_1_LOW                            : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_1_HIGH                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_2_LOW_AND_RESET_TO_PCIE_PHY_DELAY: { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_RSVD_3                                        : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PXE_VLAN_PTR                                  : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_ISCSI_BOOT_CFG_PTR                            : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_RSVD_4                                        : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_CHECKSUM                                      : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_FREE_PROV_AREA_PTR                            : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_FREE_PROV_AREA_SIZE                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_IMG_UNIQUE_ID_0                               : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_IMG_UNIQUE_ID_1                               : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_L1_SUB_STATES_CAP_LOW                    : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_L1_SUB_STATES_CAP_HIGH                   : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_L1_SUB_STATES_CTRL_1_LOW                 : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_L1_SUB_STATES_CTRL_1_HIGH                : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_L1_SUB_STATES_CTRL_2                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PTM_SETTING                                   : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_EXP_ROM_BOOT_CODE_SEC_PTR                     : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_ULP_CAP_ENABLE                                : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_RSVD_5                                        : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_RSVD_6                                        : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_QBV_MNG_WATCHDOG_LOW                          : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_QBV_MNG_WATCHDOG_HIGH                         : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_RO_CMDS_VER                                   : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_FW_MODULE_CFG_PTR                             : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_3_LOW                            : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_3_HIGH                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_4_LOW                            : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_4_HIGH                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_5_LOW                            : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    FXVL_FW_HDR_PCIE_PHY_CFG_5_HIGH                           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    }

FXVL_FW_HDR_STRUCT   : str = str ()
FXVL_FW_HDR_BYTE_SIZE: int = int ()
FXVL_FW_HDR_STRUCT, FXVL_FW_HDR_BYTE_SIZE = \
    GetStructFormatAndByteSize (FXVL_FW_HDR_FORMAT_DICT)

class FoxvilleFwHdr (object):
    DefaultEthernetIndividualAddress: int = DEFAULT_FXVL_FW_ETHERNET_INDIVIDUAL_ADDRESS
    DefaultSubsystemId              : int = DEFAULT_FXVL_FW_SUBSYSTEM_ID
    DefaultSubsystemVendorId        : int = DEFAULT_FXVL_FW_SUBSYSTEM_VENDOR_ID
    #
    # List of items need to check for input key within dict.
    #
    FOXVILLE_FW_HDR_MANDATORY: List[str] = [
        FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_0,
        FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_1,
        FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_2,
        FXVL_FW_HDR_COMPATIBILITY,
        FXVL_FW_HDR_OEM_LED_CFG,
        FXVL_FW_HDR_DEV_STARTER_VER,
        FXVL_FW_HDR_OEM_CFG_1,
        FXVL_FW_HDR_OEM_CFG_2,
        FXVL_FW_HDR_PBA_NUM_0,
        FXVL_FW_HDR_PBA_NUM_1,
        FXVL_FW_HDR_INIT_CTRL_WORD_1,
        FXVL_FW_HDR_SUBSYSTEM_ID,
        FXVL_FW_HDR_SUBSYSTEM_VENDOR_ID,
        FXVL_FW_HDR_DEVICE_ID,
        FXVL_FW_HDR_VENDOR_ID,
        FXVL_FW_HDR_INIT_CTRL_WORD_2,
        FXVL_FW_HDR_FW_IMG_PTR,
        FXVL_FW_HDR_FLASH_DEVICE_SIZE,
        FXVL_FW_HDR_EEPROM_SIZING_AND_PROTECTED,
        FXVL_FW_HDR_INIT_CTRL_WORD_4,
        FXVL_FW_HDR_PCIE_L1_EXIT_LATENCIES,
        FXVL_FW_HDR_PCIE_COMPLETION_TIMEOUT_CFG,
        FXVL_FW_HDR_MSI_X_CFG,
        FXVL_FW_HDR_SW_RESET_CSR_AUTO_CFG_PTR,
        FXVL_FW_HDR_PCIE_INIT_CFG_1,
        FXVL_FW_HDR_PCIE_INIT_CFG_2,
        FXVL_FW_HDR_PCIE_INIT_CFG_3,
        FXVL_FW_HDR_PCIE_CTRL_1,
        FXVL_FW_HDR_LED_1_3_CFG_DEFAULT,
        FXVL_FW_HDR_DUMMY_DEVICE_ID,
        FXVL_FW_HDR_DEVICE_REV_ID,
        FXVL_FW_HDR_LED_0_2_CFG_DEFAULT,
        FXVL_FW_HDR_SW_DEFINED_PINS_CTRL,
        FXVL_FW_HDR_FUNC_CTRL,
        FXVL_FW_HDR_LAN_PWR_CONSUMPTION,
        FXVL_FW_HDR_PCIE_RESET_CSR_AUTO_CFG_PTR,
        FXVL_FW_HDR_INIT_CTRL_WORD_3,
        FXVL_FW_HDR_RSVD_1,
        FXVL_FW_HDR_RSVD_2,
        FXVL_FW_HDR_CSR_AUTO_CFG_PWR_UP_PTR,
        FXVL_FW_HDR_PCIE_CTRL_2,
        FXVL_FW_HDR_PCIE_CTRL_3,
        FXVL_FW_HDR_CDQM_MEMORY_BASE_LOW,
        FXVL_FW_HDR_CDQM_MEMORY_BASE_HIGH,
        FXVL_FW_HDR_END_OF_RO_AREA_PTR,
        FXVL_FW_HDR_START_OF_RO_AREA_PTR,
        FXVL_FW_HDR_WATCHDOG_CFG,
        FXVL_FW_HDR_VPD_PTR,
        FXVL_FW_HDR_SETUP_OPTS_PCI_FUNC,
        FXVL_FW_HDR_CFG_CUSTOMIZATION_OPTS_PCI_FUNC,
        FXVL_FW_HDR_PXE_VER,
        FXVL_FW_HDR_IBA_CAPS,
        FXVL_FW_HDR_PCIE_PHY_CFG_0_LOW,
        FXVL_FW_HDR_PCIE_PHY_CFG_0_HIGH,
        FXVL_FW_HDR_ISCSI_OPT_ROM_VER,
        FXVL_FW_HDR_ALTERNATE_MAC_ADDRESS_LOC,
        FXVL_FW_HDR_PCIE_PHY_CFG_1_LOW,
        FXVL_FW_HDR_PCIE_PHY_CFG_1_HIGH,
        FXVL_FW_HDR_PCIE_PHY_CFG_2_LOW_AND_RESET_TO_PCIE_PHY_DELAY,
        FXVL_FW_HDR_RSVD_3,
        FXVL_FW_HDR_PXE_VLAN_PTR,
        FXVL_FW_HDR_ISCSI_BOOT_CFG_PTR,
        FXVL_FW_HDR_RSVD_4,
        FXVL_FW_HDR_CHECKSUM,
        FXVL_FW_HDR_FREE_PROV_AREA_PTR,
        FXVL_FW_HDR_FREE_PROV_AREA_SIZE,
        FXVL_FW_HDR_IMG_UNIQUE_ID_0,
        FXVL_FW_HDR_IMG_UNIQUE_ID_1,
        FXVL_FW_HDR_PCIE_L1_SUB_STATES_CAP_LOW,
        FXVL_FW_HDR_PCIE_L1_SUB_STATES_CAP_HIGH,
        FXVL_FW_HDR_PCIE_L1_SUB_STATES_CTRL_1_LOW,
        FXVL_FW_HDR_PCIE_L1_SUB_STATES_CTRL_1_HIGH,
        FXVL_FW_HDR_PCIE_L1_SUB_STATES_CTRL_2,
        FXVL_FW_HDR_PTM_SETTING,
        FXVL_FW_HDR_EXP_ROM_BOOT_CODE_SEC_PTR,
        FXVL_FW_HDR_ULP_CAP_ENABLE,
        FXVL_FW_HDR_RSVD_5,
        FXVL_FW_HDR_RSVD_6,
        FXVL_FW_HDR_QBV_MNG_WATCHDOG_LOW,
        FXVL_FW_HDR_QBV_MNG_WATCHDOG_HIGH,
        FXVL_FW_HDR_RO_CMDS_VER,
        FXVL_FW_HDR_FW_MODULE_CFG_PTR,
        FXVL_FW_HDR_PCIE_PHY_CFG_3_LOW,
        FXVL_FW_HDR_PCIE_PHY_CFG_3_HIGH,
        FXVL_FW_HDR_PCIE_PHY_CFG_4_LOW,
        FXVL_FW_HDR_PCIE_PHY_CFG_4_HIGH,
        FXVL_FW_HDR_PCIE_PHY_CFG_5_LOW,
        FXVL_FW_HDR_PCIE_PHY_CFG_5_HIGH
        ]

    def __init__ (self, **Kwargs) -> None:
        """ Class used to represent the Foxville NVM header.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of Foxville NVM header structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__EthernetIndividualAddress0: str = \
            self.__Kwargs[FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_0]
        self.__EthernetIndividualAddress1: str = \
            self.__Kwargs[FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_1]
        self.__EthernetIndividualAddress2: str = \
            self.__Kwargs[FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_2]

        self.__SubsystemId      : str = self.__Kwargs[FXVL_FW_HDR_SUBSYSTEM_ID]
        self.__SubsystemVendorId: str = self.__Kwargs[FXVL_FW_HDR_SUBSYSTEM_VENDOR_ID]
        self.__DevStarterVer    : str = self.__Kwargs[FXVL_FW_HDR_DEV_STARTER_VER]

    @property
    def EthernetIndividualAddress (self) -> str:
        """ Get the ethernet individual address of Foxville firmware. (6 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The ethernet individual address of Foxville firmware.
        """
        return ''.join ([
            self.__EthernetIndividualAddress2,
            self.__EthernetIndividualAddress1,
            self.__EthernetIndividualAddress0,
            ])

    @property
    def SubsystemId (self) -> str:
        """ Get the subsystem ID of Foxville firmware. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The subsystem ID of Foxville firmware.
        """
        return self.__SubsystemId

    @property
    def SubsystemVendorId (self) -> str:
        """ Get the subsystem vendor ID of Foxville firmware. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The subsystem vendor ID of Foxville firmware.
        """
        return self.__SubsystemVendorId

    @property
    def DevStarterVer (self) -> str:
        """ Get the DEV starter version of Foxville firmware. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The DEV starter version of Foxville firmware.
        """
        return self.__DevStarterVer

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) Foxville NVM header structure member is missing.
                (2) Foxville NVM ethernet individual address is not
                    default one.
                (3) Foxville NVM subsystem ID is not default one.
                (4) Foxville NVM subsystem vendor ID is not default one.

        Returns:
            None.
        """
        EthernetIndividualAddress: str = str ()
        SubsystemId              : str = str ()
        SubsystemVendorId        : str = str ()

        #
        # To check header information is meet the mandatory list.
        #
        for Member in FoxvilleFwHdr.FOXVILLE_FW_HDR_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )

        #
        # To check the parsed ethernet individual address is correct.
        #
        EthernetIndividualAddress = \
            ''.join ([
                 self.__Kwargs[FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_2],
                 self.__Kwargs[FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_1],
                 self.__Kwargs[FXVL_FW_HDR_ETHERNET_INDIVIDUAL_ADDRESS_0],
                 ])

        if (HexToDec (EthernetIndividualAddress)) != \
            FoxvilleFwHdr.DefaultEthernetIndividualAddress:
            raise ValueError (
                    f'Input ethernet individual address '
                    f'({EthernetIndividualAddress}) is not'
                    f'({DecToHex (FoxvilleFwHdr.DefaultEthernetIndividualAddress)})'
                    )

        #
        # To check the parsed subsystem ID is correct.
        #
        SubsystemId = self.__Kwargs[FXVL_FW_HDR_SUBSYSTEM_ID]
        if (HexToDec (SubsystemId)) != \
            FoxvilleFwHdr.DefaultSubsystemId:
            raise ValueError (
                    f'Input subsystem ID ({SubsystemId}) is not'
                    f'({DecToHex (FoxvilleFwHdr.DefaultSubsystemId)})'
                    )

        #
        # To check the parsed subsystem ID is correct.
        #
        SubsystemVendorId = self.__Kwargs[FXVL_FW_HDR_SUBSYSTEM_VENDOR_ID]
        if (HexToDec (SubsystemVendorId)) != \
            FoxvilleFwHdr.DefaultSubsystemVendorId:
            raise ValueError (
                    f'Input subsystem ID ({SubsystemVendorId}) is not'
                    f'({DecToHex (FoxvilleFwHdr.DefaultSubsystemVendorId)})'
                    )
