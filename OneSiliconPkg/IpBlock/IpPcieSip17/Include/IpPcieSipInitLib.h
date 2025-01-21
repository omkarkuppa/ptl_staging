/** @file
  Header file for PCIe SIP init library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _IP_PCIE_SIP_INIT_LIB_H_
#define _IP_PCIE_SIP_INIT_LIB_H_

#include <IpCsi.h>
#include <IpCpcie.h>

#define MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP17  14
#define MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP16  11
#define MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP14  5
#define MAX_PTM_STAGE_DELAY_CONFIG_REGS        MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP17

typedef struct {
  UINT16  Segment;
  UINT16  Bus;
  UINT16  Device;
  UINT16  Function;
} IP_PCIE_SBDF;

/**
  Performs PCIe link equalization according to platform settings.
  Please make sure that link is in gen1 or gen2 before calling this
  procedure.

  @param[in] pInst                   Pointer to the root port device
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link equalization settings to be used during EQ
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link equalization settings to be used during EQ
  @param[in] PcieGen5LinkEqSettings  PCIe Gen5 link equalization settings to be used during EQ
  @param[in] RootPortPcieSpeed       PCIe speed of Root Port
**/
void
SipLinkEqualize (
  IP_PCIE_INST             *pInst,
  IP_PCIE_LINK_EQ_SETTINGS *PcieGen3LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS *PcieGen4LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS *PcieGen5LinkEqSettings,
  IP_PCIE_SPEED            RootPortPcieSpeed
  );

/**
  Get max link width.

  @param[in] pInst  *pInst
  @retval            Max link width
**/
UINT8
SipGetMaxLinkWidth (
  IP_PCIE_INST      *pInst
  );

/**
  Checks if lane reversal is enabled on a given Root Port

  @param[in] pInst  *pInst

  @retval TRUE if lane reversal is enbabled, FALSE otherwise
**/
BOOLEAN
IpPcieSipIsLaneReversalEnabled (
  IP_PCIE_INST  *pInst
  );

/**
  Get PCIe port number for enabled port.
  @param[in] pInst  *pInst

  @retval Root Port number (1 based)
**/
UINT32
SipGetPortNum (
  IP_PCIE_INST      *pInst
  );

/**
  Get max PCIe link speed supported by the root port.

  @param[in]  pInst  *pInst

  @retval Max link speed
**/
UINT32
SipGetMaxLinkSpeed (
  IP_PCIE_INST      *pInst
  );

/**
  Dump PCIe HW EQ results. For internal use only.

  @param[in] pInst  *pInst
**/
void
SipDumpHwEqResults (
  IP_PCIE_INST  *pInst
  );

/**
  This function creates SIP17 Capability and Extended Capability List

  @param[in]  IP_PCIE_INST       *pInst
**/
void
SipInitCapabilityList (
  IP_PCIE_INST  *pInst
  );

/**
  Configure Pcie Squelch Power Management.

  @param[in] pInst                *pInst
**/
void
SipConfigureSquelchPowerManagement (
  IP_PCIE_INST  *pInst
  );

/**
  Configure Pcie Phy Configuration.
  This function has to be executed before
  start of equalization.

  @param[in] pInst  *pInst
**/
void
SipConfigurePhyInit (
  IP_PCIE_INST      *pInst
  );

/**
  Reset and enable Recovery Entry and Idle Framing Error Count

  @param[in] pInst                *pInst
**/
void
SipResetErrorCounts (
  IP_PCIE_INST      *pInst
  );

/**
  Sets required transmitter presets for the root port.
  Those presets will be used during phase 1 of the equalization process
  to establish initial communication at higher speed.

  @param[in] IP_PCIE_INST            *pInst
  @param[in] PcieGen3LinkEqSettings  PCIe link EQ settings
  @param[in] PcieGen4LinkEqSettings  PCIe link EQ settings
  @param[in] PcieGen5LinkEqSettings  PCIe link EQ settings
  @param[in] PcieSpeed               PCIe speed to be trained to
**/
void
SipSetTransmitterPresets (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings,
  IP_PCIE_SPEED               PcieSpeed
  );

/**
  If local transmitter override is enabled this function will
  program all of the required coefficients overrides.

  @param[in] IP_PCIE_INST           *pInst
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link EQ settings
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link EQ settings
  @param[in] PcieGen5LinkEqSettings  PCIe Gen5 link EQ settings
  @param[in] TargetPcieSpeed         PCIe speed to train to
**/
void
SipSetupEqPh2 (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings,
  IP_PCIE_SPEED                  TargetPcieSpeed
  );

/**
  Program registers to Setup EQTS2 recovery

  @param[in] pInst                *pInst
**/
void
SipSetupEQTS2Recovery (
  IP_PCIE_INST  *pInst
  );

/**
  Program registers to Setup Gen5 Hardware Equalization.

  @param[in] pInst               *pInst
  @param[in] PcieLinkEqSettings  PCIe link EQ settings
**/
void
SipSetupGen5HardwareEq (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieLinkEqSettings
  );

/**
  Start Equalization process

  @param[in] IP_PCIE_INST         *pInst
  @param[in] TargetPcieSpeed      PCIe speed to be trained to
**/
void
SipStartEqualization (
  IP_PCIE_INST          *pInst,
  IP_PCIE_SPEED         TargetPcieSpeed
  );

/**
  Disable PCIe root port in controller.

  @param[in] pInst                *pInst
**/
void
SipDisableRootPortInController (
  IP_PCIE_INST       *pInst
  );

/**
  Performs the initialization for hardware equalization to Gen3.
  Please make sure this function is called before programming any
  power management register in PCIe Root Port

  @param[in] pInst                   *pInst
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link equalization settings to be used during EQ
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link equalization settings to be used during EQ
  @param[in] PcieGen5LinkEqSettings  PCIe Gen5 link equalization settings to be used during EQ
**/
void
SipLinkEqualizeInit (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings
  );

/**
  Clears Gen3 Gen4 EQ related registers to protect against EQ reruns and bad defaults.

  @param[in]   @param[in] pInst          *pInst
**/
void
SipClearEqRegisters (
  IP_PCIE_INST        *pInst
  );

/**
  Programs presets-to-coefficients mapping for hardware equalization.
  It should not be performed for ports without Gen3 capability

  @param[in] IP_PCIE_INST             *pInst
  @param[in] PcieGen3LinkEqSettings   Pointer to PCIe link EQ settings
  @param[in] PcieGen4LinkEqSettings   Pointer to PCIe link EQ settings
  @param[in] PcieGen5LinkEqSettings   Pointer to PCIe link EQ settings
  @param[in] TargetPcieSpeed          PCIe speed to train to
**/
void
SipConfigurePresetToCoefficientMapping (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings,
  IP_PCIE_SPEED               TargetPcieSpeed
  );

/**
  Configure PCIe power down mapping

  @param[in] pInst                *pInst
**/
void
SipConfigurePowerDownMapping (
  IP_PCIE_INST            *pInst
  );

/**
  Perform reset prep programming

  @param[in] pInst                *pInst
**/
void
SipConfigureResetPrep (
   IP_PCIE_INST            *pInst
  );

/**
  Perform some early Pcie Root port configuration to enable RCRB and program decoder control registers

  @param[in] pInst  Pointer to IP_PCIE_INST
**/
void
SipEarlyDecodeEnable (
   IP_PCIE_INST            *pInst
  );

/**
  Program Rx primary Cycle Decode Registers for SIP17

  @param[in] pInst   Pointer to IP_PCIE_INST
**/
void
SipRxMasterCycleDecode (
   IP_PCIE_INST            *pInst
  );

/**
  Configure power management settings whcih are applicable to both enabled and disabled ports.
  This settings are relevant to all ports including disabled ports.
  Use sideband access since primary may not be available.

  @param[in]  pInst               *pInst
**/
void
SipConfigurePortBasePowerManagement (
   IP_PCIE_INST        *pInst
  );

/**
  Configure ASPM settings which are applicable to both enabled and disabled ports.
  These settings are relevant to all ports including disabled ports.
  Use sideband access since primary may not be available.

  @param[in] pInst                *pInst
**/
void
SipConfigureASPM (
  IP_PCIE_INST        *pInst
  );

/**
  Configures the dynamic clock gating

  @param[in] pInst  *pInst
**/
void
SipConfigureDynamicClockGating (
   IP_PCIE_INST  *pInst
  );

/**
  Program controller power management settings.
  This settings are relevant to all ports including disabled ports.
  All registers are located in the first port of the controller.
  Use sideband access since primary may not be available.

  @param[in]  pInst             *pInst

**/
void
SipConfigureControllerBasePowerManagement (
   IP_PCIE_INST         *pInst
  );

/**
  Configures PTM settings on the PCIe controller.

  @param[in] pInst      *pInst
  @param[in] PtmConfig  Pointer to the PTM configuration structure
**/
void
SipConfigurePtm (
  IP_PCIE_INST           *pInst,
  IP_PTM_CONFIGURATION   *PtmConfig
  );

/**
  Configure all LTR subtraction registers according to configuration

  @param[in] pInst             *pInst
  @param[in] L1StandardConfig  Pointer to L1Standard LTR subtraction config
  @param[in] L1p1Config        Pointer to L1.1 LTR subtraction config
  @param[in] L1p2Config        Pointer to L1.2 LTR subtraction config
  @param[in] LtrSubL11Npg      Pointer to L1.1 No Power Gate LTR subtraction config
**/
void
SipConfigureLtrSubstraction (
  IP_PCIE_INST               *pInst,
  IP_LTR_SUBTRACTION_CONFIG  *L1StandardConfig,
  IP_LTR_SUBTRACTION_CONFIG  *L1p1Config,
  IP_LTR_SUBTRACTION_CONFIG  *L1p2Config,
  IP_LTR_SUBTRACTION_CONFIG  *LtrSubL11Npg
  );

/**
  Performs mandatory Root Port Initialization.
  This function is silicon-specific and configures proprietary registers.

  @param[in] pInst                   *pInst
**/
void
SipInitRootPort (
  IP_PCIE_INST    *pInst
  );

/**
  Determines whether PCIe link is active

  @param[in] pInst  *pInst

  @retval Link Active state
**/
BOOLEAN
SipIsLinkActive (
  IP_PCIE_INST      *pInst
  );

/**
  Checks if given rootport has an endpoint connected

  @param[in]  IP_PCIE_INST       *pInst

  @retval TRUE   if endpoint is connected
  @retval FALSE  if no endpoint was detected
**/
BOOLEAN
SipIsEndpointConnected (
  IP_PCIE_INST  *pInst
  );

#endif
