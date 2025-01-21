/** @file
  Header file for PCI Express helpers library implementation.

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
#ifndef _PCIE_HELPERS_LIBRARY_H_
#define _PCIE_HELPERS_LIBRARY_H_

#include <PcieConfig.h>
#include <IpCpcie.h>
#include <Library/PciExpressHelpersLib.h>

/**
   Configure Pcie configure the downstream bridges and end point

  @param[in] pInst  *pInst

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
PcieRpDownstreamConfiguration (
  IP_PCIE_INST   *pInst
  );

/**
  Multi VC configuration.

  @param[in]  Sbdf     SBDF
  @param[in]  BusMin   minimum bus
  @param[in]  BusMax   maximum bus
  @param[in]  TcVcMap  TC/VC map
**/
VOID
PcieRpMultiVcConfiguration (
  IP_PCIE_INST   *pInst,
  TC_VC_MAP      *TcVcMap
  );

/**
  Sets Common Clock Configuration bit for devices that share common clock across link
  Devices on both sides of a PCIE link share common clock if both upstream component
  and function 0 of downstream component report Slot Clock Configuration bit = 1.
  When this function executes on upstream component, it checks SCC of both sides of the link
  If they both support it, sets CCC for both sides (this means all functions of downstream component)
  When this function executes on downstream component, it only returns SCC capability

  @param[in] *pInst               Point of pInst
  @retval  IpCsiStsSuccess        The function completes successfully
**/
IP_CSI_STATUS
PcieRpDownStreamCcConfiguration (
  IP_PCIE_INST   *pInst
  );

VOID
IpPcieSpecCompliantLtrOverride (
  UINT64           RpBase
  );

/**
  Configures LTR override in Root Port's proprietary registers.
  @param[in] RpBase                        Root Port base address
  @param[in] DevNum                        Currently visited device number
  @param[in] TreeLtr                       Snoop Latency Override settings
  @param[in] LtrConfig                     Root Port LTR configuration
**/
VOID
IpPcieConfigureRpLtrOverride (
  UINT64              RpBase,
  UINT32              DevNum,
  LTR_OVERRIDE        *TreeLtr,
  IP_PCIE_LTR_CONFIG  *LtrConfig
  );

/**
  Configure Downstream PM configuration

  @param[in] pInst  *pInst
  @param[in] AspmOverrideTableSize    size of override array
  @param[in] AspmOverrideTable        array of device that need exceptions in configuration

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
IpPcieRpDownstreamPmConfiguration (
  IP_PCIE_INST                        *pInst,
  UINT32                              AspmOverrideTableSize,
  PCIE_DEVICE_OVERRIDE                *AspmOverrideTable
  );
#endif
