/** @file
  Resources initialization values for Thunderbolt(TM).

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

@par Specification
**/

#ifndef _DISCRETE_TBT_RESOURCES_INIT_VALUES_H_
#define _DISCRETE_TBT_RESOURCES_INIT_VALUES_H_
//
//As per the Bios implementation Guide
//(Table 2.3: Host Router resource allocation/reservation scheme) ,
//we need to allocate enough  resources based on Host router type
//one port or two port
//

//
//  One port dTBT default init values for Barlow Ridge
//
#define DTBT_PCIE_EXTRA_BUS_RSVD_ONE_PORT_DEFAULT     42
#define DTBT_PCIE_MEM_RSVD_ONE_PORT_DEFAULT           128
#define DTBT_PCIE_MEM_ADDRRNGMAX_ONE_PORT_DEFAULT     26
#define DTBT_PCIE_PMEM_RSVD_ONE_PORT_DEFAULT          98304
#define DTBT_PCIE_PMEM_ADDRRNGMAX_ONE_PORT_DEFAULT    28

//
//  Two ports dTBT default init values for Barlow Ridge
//
#define DTBT_PCIE_EXTRA_BUS_RSVD_TWO_PORT_DEFAULT     81
#define DTBT_PCIE_MEM_RSVD_TWO_PORT_DEFAULT           256
#define DTBT_PCIE_MEM_ADDRRNGMAX_TWO_PORT_DEFAULT     26
#define DTBT_PCIE_PMEM_RSVD_TWO_PORT_DEFAULT          196608
#define DTBT_PCIE_PMEM_ADDRRNGMAX_TWO_PORT_DEFAULT    28

#endif
