/** @file
  Header file for RstCrLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _RST_CR_LIB_H_
#define _RST_CR_LIB_H_

#define RST_PCIE_STORAGE_CR_1                       0
#define RST_PCIE_STORAGE_CR_2                       1
#define RST_PCIE_STORAGE_CR_3                       2
#define RST_PCIE_STORAGE_CR_INVALID                 99

/**
  Check the RST PCIe Storage Cycle Router number according to the root port number and PCH type

  @param[in] RootPortNum  Root Port Number

  @return  The RST PCIe Storage Cycle Router Number
**/
UINT8
RstGetCycleRouterNumber (
  IN  UINT32                   RootPortNum
  );
#endif
