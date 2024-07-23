/** @file
  Pcie Root Port info header

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _PCH_PCIERP_INFO_H_
#define _PCH_PCIERP_INFO_H_

//
// Number of PCIe ports per PCIe controller
//
#define PCH_PCIE_CONTROLLER_PORTS                     4u

#define PEG_PCIE_CONTROLLER_PORTS                     2u

//
// Number of PCIe lanes per PCIe port
//
#define PCH_PCIE_LANES_PER_PORT                       1

//
// Number of PCIe lanes per PCIe controller
//
#define PCH_PCIE_LANES_PER_CONTROLLER                 (PCH_PCIE_CONTROLLER_PORTS * PCH_PCIE_LANES_PER_PORT)

#endif
