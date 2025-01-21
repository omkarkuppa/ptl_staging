/** @file
  Header for PeiDmiSipInitLib

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#ifndef _DMI_SIP_INIT_LIB_H_
#define _DMI_SIP_INIT_LIB_H_

#include <Library/PeiPcieSipInitLib.h>
#include <Library/PeiPchDmiLib.h>
#include <Library/PchDmiLib.h>
#include <ConfigBlock/PchGeneralConfig.h>

typedef struct {
  UINT8  PsfId;
  UINT8  PortGroupId;
  UINT8  PortId;
  UINT8  ChannelId;
} IOC_ROOT_PORT_DEST_ID;

/**
  Get PCH IOE default TC VC Mapping settings. This funciton returns the default PCH setting
  System Agent/IIO can update the settings according to polices.

  @param[in, out] PchDmiTcVcMap         Buffer for PCH_DMI_TC_VC_MAP instance.
  @param[in]      SipVersion            Programming version.

**/
VOID
DmiSipTcVcMapInit (
  IN OUT  PCH_DMI_TC_VC_MAP             *PchDmiTcVcMap,
  IN      PCIE_SIP_VERSION              SipVersion
  );

/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel
  until negotiation completion.

  @param[in]  PchDmiTcVcMap              Buffer for PCH_DMI_TC_VC_MAP instance.
  @param[in]  RpDev                      Pointer to the root port device.

  @retval EFI_SUCCESS                   The function completed successfully
  @retval Others                        All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
DmiSipTcVcProgPoll (
  IN   PCH_DMI_TC_VC_MAP          *PchDmiTcVcMap,
  IN   IN  PCIE_ROOT_PORT_DEV     *RpDev
  );

/**
  Get PCH DMI SIP17 Virtual Channel Control and Status registers

  @param[in]  Vc                      The virtual channel number for programing
  @param[out] PchDmiVcCtlAddress      DMI Virtual Channel Control register address
  @param[out] PchDmiVcStsAddress      DMI Virtual Channel Status register address
**/
VOID
DmiSip17VcRegs (
  IN   PCH_DMI_VC_TYPE  Vc,
  OUT  UINT16           *PchDmiVcCtlAddress,
  OUT  UINT16           *PchDmiVcStsAddress
  );

/**
  This function checks if DMI SIP15 Secured Register Lock (SRL) is set

  @param[in]  RpDev                Pointer to the root port device
  @retval SRL state
**/
BOOLEAN
DmiSipIsPchDmiLocked (
  IN   PCIE_ROOT_PORT_DEV                *RpDev
  );

/**
  Enable PCIe Relaxed Order for DMI SIP15

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipEnablePcieRelaxedOrder (
  IN   PCIE_ROOT_PORT_DEV                *RpDev
  );

/**
  This function will switch SAI value to be driven to IOSF Primary Fabric
  for cycles with Core BDF from HOSTIA_BOOT_SAI to HOSTIA_POSTBOOT_SAI.
  To be used when PCH is paired with CFL CPU.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipEnablePostBootSai (
  IN   PCIE_ROOT_PORT_DEV                *RpDev
  );

/**
  This function performs PCH-H DMI Thermal Throttling init.

  @param[in]  ThermalConfig        Pointer to the PCH Thermal Config block
  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureThermalThrottling (
  IN  THERMAL_CONFIG              *ThermalConfig,
  IN  PCIE_ROOT_PORT_DEV          *RpDev
  );

/**
  This function  Sets BIOS Lock-Down (BILD)

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipSetBiosLockDown (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  );

/**
  This function performs early PCH-H DMI SIP15 init.
  It must be called before any upstream bus host transactions are enabled

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
PchDmiEarlyInit (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  );

/**
  This function configures DMI SIP

  @param[in]  PchPcieConfig        Pointer to PCH PCIe config block
  @param[in]  ThermalConfig        Pointer to Thermal Throttling config block
  @param[in]  PchGeneralConfig     Pointer to PCH General config block
  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipInit (
  IN  PCH_PCIE_CONFIG             *PchPcieConfig,
  IN  THERMAL_CONFIG              *ThermalConfig,
  IN  PCH_GENERAL_CONFIG          *PchGeneralConfig,
  IN  PCIE_ROOT_PORT_DEV          *RpDev
  );

/**
  This function configures late DMI SIP
  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipLateInit (
  IN  PCIE_ROOT_PORT_DEV          *RpDev
  );

/**
  The function set the Target Link Speed in PCH to DMI GEN 3.

  @param[in]  TargetLinkSpeed       Target Link Speed
                                    2: GEN2
                                    3: GEN3
  @param[in]  RpDev                 Pointer to the root port device
**/
VOID
DmiSipSetTargetLinkSpeed (
  IN  UINT8                             TargetLinkSpeed,
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  );

/**
  This function reads Supported ASPM stats from Link Capabilities register.

  @param[in]  RpDev        Pointer to the root port device
  @param[out] PchDmiAspm   PCH DMI ASPM Supported
**/
VOID
DmiSipGetAspmSupported (
  IN  PCIE_ROOT_PORT_DEV          *RpDev,
  OUT UINT8                       *PchDmiAspm
  );

/**
  This function enables ASPM states in PCH DMI Link Control register.

  @param[in]  PchDmiAspm   Bitmap with ASPM states to enable
  @param[in]  RpDev        Pointer to the root port device
**/
VOID
DmiSipSetAspm (
  IN  UINT8                       PchDmiAspm,
  IN  PCIE_ROOT_PORT_DEV          *RpDev
  );

/**
  This function performs configuration of DMI Coalescing.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
  @return     A bitmap of supported link speeds (gens) is returned.
**/
UINT32
DmiSipGetSupportedLinkSpeedsVector (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  );

/**
  This function performs enabling of Thermal Throttling.
  From SIP 17 it should be performed afrer link training to max speed.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipEnableThermalThrottling (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  );

/**
  This function configures ASPM on DMI

  @param[in]  PchPcieConfig        Pointer to PCH PCIe Config block
  @param[in]  PchGeneralConfig     Pointer to PCH General Config block
  @param[in]  RpDev                Pointer to the root port device
**/

VOID
DmiSipConfigureAspm (
  IN  PCH_PCIE_CONFIG              *PchPcieConfig,
  IN  PCH_GENERAL_CONFIG           *PchGeneralConfig,
  IN  PCIE_ROOT_PORT_DEV           *RpDev
  );

/**
  The function configure the L1ss for DMI.

  @param[in]  RpDev                 Pointer to the root port device
**/
VOID
DmiConfigureL1ss (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  );

/**
  This function configures Port 80 decode settings.

  @param[in]  RpDev                Pointer to the root port device
  @param[in]  IocRpDestinationId   IOC Root Port destination ID
**/
VOID
DmiSipConfigurePort80Decode (
  IN  PCIE_ROOT_PORT_DEV     *RpDev,
  IN  IOC_ROOT_PORT_DEST_ID  IocRpDestinationId
  );

/**
  Dumps best EQ coefficients values that were selected during EQ.

  @param[in]  RpDev        Pointer to the root port device
  @param[in]  RpLaneIndex  Lane index
  @param[in]  Speed        Speed for which best Coefficients are required
**/
VOID
DmiSipDumpBestEqCoefficients (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                RpLaneIndex,
  IN UINT8                 Speed
  );
#endif
