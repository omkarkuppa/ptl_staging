/**@file
  Header file for Pcie Domain library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef __DOMAIN_PCIE_H__
#define __DOMAIN_PCIE_H__

/**
  DomainInit for Initilizing IP Instances

  @param[in] MaxRootPortNum       Pcie Max RootPort number
  @param[in] PcieType             PCIe Integration Type

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    pInst configuration has bad size
**/
EFI_STATUS
DomainPcieInit (
  IN  UINT32  MaxRootPortNum,
  IN BOOLEAN  PcieType
  );

/**
  Configure root port function number mapping.

**/
VOID
IpPcieSipConfigureRpfnMapping (
  VOID
  );

/**
  Hides root ports that have been disabled by straps
  or by the controller config.

  @param[in] PcieType             PCIe Integration Type

**/
VOID
SipHideDisableRootPorts (
  IN BOOLEAN  PcieType
  );

#endif
