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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Library/UefiBootServicesTableLib.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleRegs.h"
#include "Misc/FoxvillePhy.h"
#include "Misc/FoxvilleRegAccess.h"

/**
  Write the MDICNFG register for PHY reset.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to write MDICNFG register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write MDICNFG register.

**/
EFI_STATUS
WriteMdicnfgForPhyReset (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMdicnfg (PciIoProtocolPtr, 0);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  gBS->Stall (FOXVILLE_MDICNFG_FOR_PHY_RESET_TIMEOUT);

  return EFI_SUCCESS;
}

/**
  Write the MDIC register with READ operation code for PHY reset.

  @note This function is used to support the PHY reset.
        The returned value of the read operation is not used.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to write MDIC register with READ operation code.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write MDIC register with READ operation code.

**/
EFI_STATUS
WriteMdicOpReadForPhyReset (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT16               Address
  )
{
  EFI_STATUS              Status;
  FOXVILLE_MDIC_REGISTER  Request;

  Request.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Request.Bits.Data       = 0;
  Request.Bits.RegAddress = Address;
  Request.Bits.OpCode     = V_MDIC_OP_CODE_READ;

  Status = WriteMdic (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  gBS->Stall (FOXVILLE_MDIC_FOR_PHY_RESET_TIMEOUT);

  return EFI_SUCCESS;
}

/**
  Write the MDIC register with WRITE operation code for PHY reset.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to write MDIC register with WRITE operation code.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write MDIC register with WRITE operation code.

**/
EFI_STATUS
WriteMdicOpWriteForPhyReset (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT16               Address,
  IN UINT16               Data
  )
{
  EFI_STATUS              Status;
  FOXVILLE_MDIC_REGISTER  Request;

  Request.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Request.Bits.Data       = Data;
  Request.Bits.RegAddress = Address;
  Request.Bits.OpCode     = V_MDIC_OP_CODE_WRITE;

  Status = WriteMdic (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  gBS->Stall (FOXVILLE_MDIC_FOR_PHY_RESET_TIMEOUT);

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;
  UINTN       Index;

  Index = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < 2; Index++) {
    Status = WriteMdicnfgForPhyReset (PciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    Status = WriteMdicOpWriteForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_CTRL, 0x001E);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    Status = WriteMdicOpWriteForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_DATA, 0x0007);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    Status = WriteMdicOpWriteForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_CTRL, 0x401E);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    Status = WriteMdicOpReadForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_DATA);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    Status = WriteMdicOpWriteForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_DATA, 0x0600);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    Status = WriteMdicOpReadForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_DATA);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  }

  Status = WriteMdicnfgForPhyReset (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = WriteMdicOpWriteForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_CTRL, 0x0000);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = WriteMdicOpWriteForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_DATA, 0x0000);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = WriteMdicOpWriteForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_CTRL, 0x4000);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = WriteMdicOpReadForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_DATA);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = WriteMdicOpWriteForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_DATA, 0x8000);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = WriteMdicOpReadForPhyReset (PciIoProtocolPtr, V_MDIC_REG_STD_MMD_DATA);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}
