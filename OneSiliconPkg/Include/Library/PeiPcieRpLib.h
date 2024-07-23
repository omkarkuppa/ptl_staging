/** @file
  Header file for PEI PCIe functionality

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
#ifndef _PEI_PCIE_LIB_H_
#define _PEI_PCIE_LIB_H_

extern EFI_GUID gPciImrHobGuid;

typedef struct {
  EFI_HOB_GUID_TYPE Header;
  UINT64            PciImrBase;
} PCI_IMR_HOB;


/**
  Read the PCIE policy to check port Location for PCIe IMR

  @retval   RootPort Location
**/
UINT8
GetPcieImrPortLocation (
  VOID
  );
#endif
