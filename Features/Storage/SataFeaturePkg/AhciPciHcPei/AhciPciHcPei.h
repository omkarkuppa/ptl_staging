/** @file
  This PEIM driver initialize Ahci host contoller and
  produce gEdkiiPeiAtaAhciHostControllerPpi instance for other driver.

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

@par Specification
**/

#ifndef _AHCI_PCI_HC_PEI_H_
#define _AHCI_PCI_HC_PEI_H_

#include <PiPei.h>

#include <Ppi/MasterBootMode.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/AtaAhciController.h>

#include <IndustryStandard/Pci.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/AhciPcieRpLib.h>

//
// AHCI PCI HC PEIM driver implementation related definitions
//
#define MAX_ATA_AHCI_HCS                   1
#define ATA_AHCI_HC_PCI_BUS_NUM            0x00

//
// PCI related definitions
//
#define PCI_IF_AHCI                        0x01
#define PCI_AHCI_BASE_ADDRESSREG_OFFSET    0x24

#pragma pack(1)

//
// Device path for ATA AHCI host controller
//
typedef struct {
  ACPI_HID_DEVICE_PATH                  PciRoot;
  PCI_DEVICE_PATH                       AhciEp;
  EFI_DEVICE_PATH_PROTOCOL              End;
} ATA_AHCI_DEVICE_PATH;

#pragma pack()

//
// PCI Class Code structure
//
typedef struct {
  UINT8 PI;
  UINT8 SubClassCode;
  UINT8 BaseCode;
} PCI_CLASSC;

typedef struct {
  UINT8                                 Bus;
  UINT8                                 Device;
  UINT8                                 Function;
} PCI_NODE_INFO;

typedef struct {
  PCI_NODE_INFO                         PciInfo;

  UINTN                                 Bar;
  UINTN                                 DevicePathLength;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;

  UINT8                                 OriCmdRegLow;
  UINT32                                OriABarReg;

} AHCI_HC_INFO;

#define AHCI_HC_PEI_SIGNATURE              SIGNATURE_32 ('A', 'H', 'C', 'P')

typedef struct {
  UINTN                                 Signature;
  EDKII_ATA_AHCI_HOST_CONTROLLER_PPI    AhciControllerPpi;
  EFI_PEI_PPI_DESCRIPTOR                PpiList;
  EFI_PEI_NOTIFY_DESCRIPTOR             EndOfPeiNotifyList;

  BOOLEAN                               IsRaid;
  UINTN                                 TotalAhciHcs;
  AHCI_HC_INFO                          HcInfo[MAX_ATA_AHCI_HCS];
} AHCI_HC_PEI_PRIVATE_DATA;

#define AHCI_HC_PEI_PRIVATE_DATA_FROM_THIS(a)           \
  CR (a, AHCI_HC_PEI_PRIVATE_DATA, AhciControllerPpi, AHCI_HC_PEI_SIGNATURE)
#define AHCI_HC_PEI_PRIVATE_DATA_FROM_THIS_NOTIFY(a)    \
  CR (a, AHCI_HC_PEI_PRIVATE_DATA, EndOfPeiNotifyList, AHCI_HC_PEI_SIGNATURE)


//
// EDKII_ATA_AHCI_HOST_CONTROLLER_PPI services
//

/**
  Get the MMIO base address of ATA AHCI host controller.

  @param[in]  This                 The PPI instance pointer.
  @param[in]  ControllerId         The ID of the ATA AHCI host controller.
  @param[out] MmioBar              The MMIO base address of the controller.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    The parameters are invalid.
  @retval EFI_NOT_FOUND            The specified ATA AHCI host controller not found.

**/
EFI_STATUS
EFIAPI
GetAhciHcMmioBar (
  IN  EDKII_ATA_AHCI_HOST_CONTROLLER_PPI    *This,
  IN  UINT8                                 ControllerId,
  OUT UINTN                                 *MmioBar
  );

/**
  Get the device path of ATA AHCI host controller.

  @param[in]  This                 The PPI instance pointer.
  @param[in]  ControllerId         The ID of the ATA AHCI host controller.
  @param[out] DevicePathLength     The length of the device path in bytes specified
                                   by DevicePath.
  @param[out] DevicePath           The device path of ATA AHCI host controller.
                                   This field re-uses EFI Device Path Protocol as
                                   defined by Section 10.2 EFI Device Path Protocol
                                   of UEFI 2.7 Specification.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    The parameters are invalid.
  @retval EFI_NOT_FOUND            The specified ATA AHCI host controller not found.
  @retval EFI_OUT_OF_RESOURCES     The operation fails due to lack of resources.

**/
EFI_STATUS
EFIAPI
GetAhciHcDevicePath (
  IN  EDKII_ATA_AHCI_HOST_CONTROLLER_PPI    *This,
  IN  UINT8                                 ControllerId,
  OUT UINTN                                 *DevicePathLength,
  OUT EFI_DEVICE_PATH_PROTOCOL              **DevicePath
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
AhciHcEndOfPei (
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
  Check whether the controller is a Ahci host controller.

  @param[in] PciHcBase    PCI address of a specified controller.

  @retval TRUE     The specified controller is an NVM Express host controller.
  @retval FALSE    The specified controller is not an NVM Express host controller.

**/
BOOLEAN
IsPciAhciHc (
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
#endif  // _AHCI_PCI_HC_PEI_H_
