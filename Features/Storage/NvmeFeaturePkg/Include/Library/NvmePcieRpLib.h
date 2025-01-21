/** @file
  NvmePcieRpLib provide the interface to get the PCI bridge.

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

#ifndef _NVME_PCIE_RP_LIB_H_
#define _NVME_PCIE_RP_LIB_H_

#define DEFAULT_BUS_NUM  0xFFFF

//
// Pci root port complex
//
typedef struct {
  UINT8    Bus;
  UINT32   MmioBase;
  UINT32   MmioLimit;
} PCI_ROOT_PORT_COMPLEX;


/**
  Get the next root port complex information.

  @param[in]  Bus            current bus number. This function shall return first root port complex if Bus number is DEFAULT_BUS_NUM.
  @param[out] RpComplex      root port complex information

  @retval EFI_SUCCESS    The function completes successfully.
  @retval others         The function fails.

**/
EFI_STATUS
GetNextRootPortComplex (
  IN  UINT16   Bus,
  OUT PCI_ROOT_PORT_COMPLEX *RpComplex
  );

/**
  Check if support the nvme host controller.

  @param[out] Support    Support the nvme host controller

  @retval EFI_SUCCESS    The function completes successfully.
  @retval others         The function fails.

**/
EFI_STATUS
EFIAPI
NvmeHcSupport (
  BOOLEAN *Support
  );
#endif  // _NVME_PCIE_RP_LIB_H_
