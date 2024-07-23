/** @file
  This PEIM driver initialize Nvm Express host contoller and
  produce EdkiiPeiNvmExpressHostControllerPpi instance for other driver.

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

#ifndef _NVME_PCI_HC_PEI_H_
#define _NVME_PCI_HC_PEI_H_

#include <PiPei.h>
#include <IndustryStandard/Pci.h>

#include <Ppi/MasterBootMode.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/NvmExpressHostController.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/NvmePcieRpLib.h>

//
// NVME HC PEI driver implementation related definitions
//
#define MAX_NVME_HCS                             8
#define ENDPOINT_PCI_BUS_NUMBER_START            0x10
#define PCI_BRIDGE_CONFIG_SPACE_STORE_SIZE       0x30

//
// PCIE configuration space offset for Pci-Pci Bridge
//
#define PCI_BRIDGE_IO_BASE                       0x1C
#define PCI_BRIDGE_IO_LIMIT                      0x1D
#define PCI_BRIDGE_MEMORY_BASE                   0x20
#define PCI_BRIDGE_MEMORY_LIMIT                  0x22
#define PCI_BRIDGE_PREFETCHABLE_MEMORY_BASE      0x24
#define PCI_BRIDGE_PREFETCHABLE_MEMORY_LIMIT     0x26
#define PCI_BRIDGE_PREFETCHABLE_BASE_UPPER32     0x28
#define PCI_BRIDGE_PREFETCHABLE_LIMIT_UPPER32    0x2C

#pragma pack(1)

//
// Device path for NVM Express host controller
//
typedef struct {
  ACPI_HID_DEVICE_PATH                     PciRoot;
  PCI_DEVICE_PATH                          PciBridge;
  PCI_DEVICE_PATH                          NvmeHcEp;
  EFI_DEVICE_PATH_PROTOCOL                 End;
} NVME_HC_DEVICE_PATH;

#pragma pack()

typedef struct {
  UINT8                                    Bus;
  UINT8                                    Device;
  UINT8                                    Function;
} PCI_NODE_INFO;

typedef struct {
  PCI_NODE_INFO                            Bridge;
  PCI_NODE_INFO                            ControllerEndPoint;

  UINTN                                    Bar;
  UINTN                                    DevicePathLength;
  EFI_DEVICE_PATH_PROTOCOL                 *DevicePath;

  UINT8                                    OriBridgeConfigSpace[PCI_BRIDGE_CONFIG_SPACE_STORE_SIZE];
  UINT8                                    OriCmdRegLow;
  UINT32                                   OriBar0Reg;
  UINT32                                   OriBar1Reg;
} NVME_HC_INFO;

#define NVME_HC_PEI_SIGNATURE                    SIGNATURE_32 ('N', 'H', 'C', 'P')

typedef struct {
  UINTN                                    Signature;
  EDKII_NVM_EXPRESS_HOST_CONTROLLER_PPI    NvmeHostControllerPpi;
  EFI_PEI_PPI_DESCRIPTOR                   PpiList;
  EFI_PEI_NOTIFY_DESCRIPTOR                EndOfPeiNotifyList;

  UINTN                                    TotalNvmeHcs;
  NVME_HC_INFO                             HcInfo[MAX_NVME_HCS];
} NVME_HC_PEI_PRIVATE_DATA;

#define NVME_HC_PEI_PRIVATE_DATA_FROM_THIS(a)           \
  CR (a, NVME_HC_PEI_PRIVATE_DATA, NvmeHostControllerPpi, NVME_HC_PEI_SIGNATURE)
#define NVME_HC_PEI_PRIVATE_DATA_FROM_THIS_NOTIFY(a)    \
  CR (a, NVME_HC_PEI_PRIVATE_DATA, EndOfPeiNotifyList, NVME_HC_PEI_SIGNATURE)


//
// EDKII_NVM_EXPRESS_HOST_CONTROLLER_PPI services
//

/**
  Get the MMIO base address of NVM Express host controller.

  @param[in]  This                 The PPI instance pointer.
  @param[in]  ControllerId         The ID of the NVM Express host controller.
  @param[out] MmioBar              The MMIO base address of the controller.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    The parameters are invalid.
  @retval EFI_NOT_FOUND            The specified NVM Express host controller not
                                   found.

**/
EFI_STATUS
EFIAPI
GetNvmeHcMmioBar (
  IN  EDKII_NVM_EXPRESS_HOST_CONTROLLER_PPI    *This,
  IN  UINT8                                    ControllerId,
  OUT UINTN                                    *MmioBar
  );

/**
  Get the device path of NVM Express host controller.

  @param[in]  This                 The PPI instance pointer.
  @param[in]  ControllerId         The ID of the NVM Express host controller.
  @param[out] DevicePathLength     The length of the device path in bytes specified
                                   by DevicePath.
  @param[out] DevicePath           The device path of NVM Express host controller.
                                   This field re-uses EFI Device Path Protocol as
                                   defined by Section 10.2 EFI Device Path Protocol
                                   of UEFI 2.7 Specification.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    The parameters are invalid.
  @retval EFI_NOT_FOUND            The specified NVM Express host controller not
                                   found.
  @retval EFI_OUT_OF_RESOURCES     The operation fails due to lack of resources.

**/
EFI_STATUS
EFIAPI
GetNvmeHcDevicePath (
  IN  EDKII_NVM_EXPRESS_HOST_CONTROLLER_PPI    *This,
  IN  UINT8                                    ControllerId,
  OUT UINTN                                    *DevicePathLength,
  OUT EFI_DEVICE_PATH_PROTOCOL                 **DevicePath
  );

//
// Internal functions
//

/**
  One notified function to cleanup the allocated resources at the end of PEI.

  @param[in] PeiServices         Pointer to PEI Services Table.
  @param[in] NotifyDescriptor    Pointer to the descriptor for the Notification
                                 event that caused this function to execute.
  @param[in] Ppi                 Pointer to the PPI data associated with this function.

  @retval EFI_SUCCESS            The function completes successfully.

**/
EFI_STATUS
EFIAPI
NvmeHcEndOfPei (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  The callback function for SiInitDone.
  It is to make sure this driver get BDF after rootport function mapping.

  @param[in] PeiServices         Pointer to PEI Services Table.
  @param[in] NotifyDescriptor    Pointer to the descriptor for the Notification
                                 event that caused this function to execute.
  @param[in] Ppi                 Pointer to the PPI data associated with this function.

  @retval EFI_SUCCESS            The function completes successfully.
**/
EFI_STATUS
EFIAPI
SiInitCallBack (
  IN EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Check whether the controller is a PCI NVM Express host controller.

  @param[in] PciHcBase    PCI address of a specified controller.

  @retval TRUE     The specified controller is an NVM Express host controller.
  @retval FALSE    The specified controller is not an NVM Express host controller.

**/
BOOLEAN
IsPciNvmeHc (
  IN UINTN    PciHcBase
  );

/**
  Check if the PCI device valid.

  @param[in]  Bus            Bus Number.
  @param[in]  Dev            Device Number.
  @param[in]  Fun            Function Number.

  @retval TRUE               There is a bridge on that bus.
  @retval FALSE              There is a no bridge on that bus.

**/
BOOLEAN
IsValidPci(
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Fun
  );

/**
  Check if there is a PCI bridge on the Bus.

  @param[in]  Bus            Bus Number.
  @param[in]  Dev            Device Number.
  @param[in]  Fun            Function Number.

  @retval TRUE               There is a PCI bridge on that bus.
  @retval FALSE              There is a no PCI bridge on that bus.

**/
BOOLEAN
IsPciBridge(
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Fun
  );

/**
  Check if the PCI bridge support multi function

  @param[in]  Bus            Bus Number.
  @param[in]  Dev            Device Number.
  @param[in]  Fun            Function Number.

  @retval TRUE               It support multi function.
  @retval FALSE              It dose not support multi function.

**/
BOOLEAN
IsPciMultiFunc (
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Fun
  );

#endif  // _NVME_PCI_HC_PEI_H_
