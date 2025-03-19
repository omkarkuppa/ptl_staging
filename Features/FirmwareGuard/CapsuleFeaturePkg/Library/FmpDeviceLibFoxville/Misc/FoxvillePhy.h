/** @file
  Internal file for the PHY related operation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef __FOXVILLE_PHY_H__
#define __FOXVILLE_PHY_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>

#define FOXVILLE_MDICNFG_FOR_PHY_RESET_TIMEOUT  100
#define FOXVILLE_MDIC_FOR_PHY_RESET_TIMEOUT     100

/**
  Reset the PHY.

  @note A series of writes and reads via MDIO interface to PHY register.
        This would cause the GPHY FW to reset and reload.
        (Treat the written values as black box.)

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to reset the PHY.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to reset the PHY.

**/
EFI_STATUS
EFIAPI
PhyReset (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

#endif
