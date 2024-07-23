/** @file
  Header file for private PeiPcieRpInitLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _PEI_PCIE_RP_INIT_LIB_H_
#define _PEI_PCIE_RP_INIT_LIB_H_

#include <Library/PeiPcieSipInitLib.h>
#include <Ppi/SiPolicy.h>
#include <PchPcieRpConfig.h>

/**
  Hide rootports disabled by policy. This needs to be done in premem,
  because graphics init from SystemAgent code depends on those ports
  being already hidden

  @param[in] PcieRpPreMemConfig   Platform policy
**/
VOID
PcieRpEarlyDisabling (
  IN PCH_PCIE_RP_PREMEM_CONFIG *PcieRpPreMemConfig
  );

/**
  Configure root port function number mapping
**/
VOID
PcieRpConfigureRpfnMapping (
  VOID
  );

/*
  Get PCIE Port configuration and disable value for each RP
  and perform function disable.
*/
VOID
GetConfigAndDisablePcieRootPort (
  VOID
  );

/*
  Perform some early Pcie Root port configuration to enable RCRB and program decoder control registers
*/
VOID
EarlyPcieRootPortConfig (
  VOID
  );

/**
  Perform Initialization of the Downstream Root Ports.

  @param[in] SiPolicy             The SI Policy PPI
**/
VOID
PcieRpInit (
  IN CONST SI_POLICY_PPI     *SiPolicy
  );

/**
  Configure PCIe Grant Counts
**/
VOID
PcieRpConfigureGrantCounts (
  VOID
  );

/**
  Get information about the endpoint

  @param[in]  RpBase      Root port pci segment base address
  @param[in]  TempPciBus  Temporary bus number
  @param[out] DeviceInfo  Device information structure

  @raturn TRUE if device was found, FALSE otherwise
**/
BOOLEAN
GetDeviceInfo (
  IN  UINT64            RpBase,
  IN  UINT8             TempPciBus,
  OUT PCIE_DEVICE_INFO  *DeviceInfo
  );

/**
  Returns the SIP version of the PCIe root port.

  @param[in] RpIndex  Index of the root port in PCH

  @return Version of the SIP
**/
PCIE_SIP_VERSION
PcieRpGetSipVer (
  IN UINT32  RpIndex
  );

/**
  Enables PCIe controller trunk clock gating.

  @param[in] RpIndex  Index of the controller on which to enable trunk clock gating.
**/
VOID
PcieEnableTrunkClockGate (
  IN UINT32 ControllerIndex
  );

/**
  Translate PCIe Port/Lane pair to 0-based PCIe lane number.

  @param[in] RpIndex    Root Port index
  @param[in] RpLane     Root Port Lane

  @retval PCIe lane number (0-based)
**/
UINT32
PchPciePhysicalLane (
  UINT32  RpIndex,
  UINT32  RpLane
  );

/**
  This function determines whether root port is configured in non-common clock mode.
  Result is based on the NCC soft-strap setting.

  @param[in] RpBase      Root Port pci segment base address

  @retval TRUE           Port in NCC SSC mode.
  @retval FALSE          Port not in NCC SSC mode.
**/
BOOLEAN
IsPcieNcc (
  IN     UINT64  RpBase
  );

/**
  Get max link width.

  @param[in] RpBase    Root Port base address

  @retval Max link width
**/
UINT8
GetMaxLinkWidth (
  UINT64  RpBase
  );

#endif
