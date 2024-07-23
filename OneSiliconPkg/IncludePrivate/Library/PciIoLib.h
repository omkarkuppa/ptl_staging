/** @file
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

#ifndef _PCI_IO_LIB_H_
#define _PCI_IO_LIB_H_

#include <Uefi/UefiSpec.h>
#include <Ppi/PciDevice.h>
#include <Library/PciExpressHelpersLib.h>

typedef struct _PCI_DEVICE_PRIVATE_DATA PCI_DEVICE_PRIVATE_DATA;

struct _PCI_DEVICE_PRIVATE_DATA {
  EDKII_PCI_DEVICE_PPI     PciDevice;
  UINT64                   PciCfgBase;
  UINT32                   Signature;
  LIST_ENTRY               Link;
  SBDF                     Sbdf;
  UINT64                   Attributes;
  UINT64                   Supports;
  PCI_DEVICE_PRIVATE_DATA  *Parent;
};

typedef struct _HIDDEN_PCI_DEVICE_PRIVATE_DATA  HIDDEN_PCI_DEVICE_PRIVATE_DATA;

typedef enum {
  PciBarTypeUnknown = 0,
  PciBarTypeIo16,
  PciBarTypeIo32,
  PciBarTypeMem32,
  PciBarTypePMem32,
  PciBarTypeMem64,
  PciBarTypePMem64,
  PciBarTypeOpRom,
  PciBarTypeIo,
  PciBarTypeMem,
  PciBarTypeMaxType
} PCI_BAR_TYPE;

#define PCI_MAX_HIDDEN_BAR 6

struct _HIDDEN_PCI_DEVICE_PRIVATE_DATA {
  EDKII_PCI_DEVICE_PPI  PciDevice;
  UINT64                PciCfgAddress;
  UINT64                BarAddress[PCI_MAX_HIDDEN_BAR];
  PCI_BAR_TYPE          BarType[PCI_MAX_HIDDEN_BAR];
};

/**
  Creates a EDKII_PCI_DEVICE_PPI from the PCI bus address and
  a parent device path. This function allocetes memory necessary
  for the EDKII_PCI_DEVICE_PPI.

  @param[in]   Segment           PCI Segment
  @param[in]   Bus               PCI bus
  @param[in]   Device            PCI device
  @param[in]   Function          PCI function
  @param[in]   ParentDevicePath  Pointer to the device path of a parent
  @param[out]  PciDevicePpi      On output a pointer to the valid EDKII_PCI_DEVICE_PPI
**/
BOOLEAN
CreatePciDeviceIoFromSbdf (
  IN UINT8                     Segment,
  IN UINT8                     Bus,
  IN UINT8                     Device,
  IN UINT8                     Function,
  IN EFI_DEVICE_PATH_PROTOCOL  *ParentDevicePath,
  OUT EDKII_PCI_DEVICE_PPI           **PciDevicePpi
  );

BOOLEAN
CreateHiddenPciDeviceIo (
  IN UINT64                    PciCfgAddress,
  IN UINT64                    NoOfBars,
  IN UINT64                    *Bar,
  IN PCI_BAR_TYPE              *BarTypes,
  OUT EDKII_PCI_DEVICE_PPI     **PciDevicePpi
  );

/**
  Refreshes EDKII_PCI_DEVICE_PPI function pointers when callback is called

  @param  PciDevicePpi            Pointer to the EDKII_PCI_DEVICE_PPI

  @retval EFI_SUCCESS             Function pointers were refreshed successfully
  @retval EFI_INVALID_PARAMETER   PCI Device PPI is NULL
**/
EFI_STATUS
EFIAPI
RefreshPciDevicePointersPostMem (
  IN EDKII_PCI_DEVICE_PPI     **PciDevicePpi
  );

#endif
