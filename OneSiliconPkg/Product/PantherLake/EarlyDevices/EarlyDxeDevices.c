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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PciIoLib.h>
#include <Library/PmcLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Defines/PchReservedResources.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/MeInitLib.h>
#include <Protocol/DevicePath.h>
#include <HiddenDevices.h>
#include <Defines/PcdPchBdfAssignment.h>

typedef struct {
  INTC_ACPI_EXTENDED_DEVICE_PATH    Intc;
  EFI_DEVICE_PATH_PROTOCOL          End;
} HIDDEN_DEV_PATH;

VOID
InstallHiddenPmcDevice (
  IN UINT64  Pwrm,
  IN UINT32  Acpi,
  IN UINT32  UID
  )
{
  UINT64                Bars[PCI_MAX_HIDDEN_BAR];
  PCI_BAR_TYPE          BarType[PCI_MAX_HIDDEN_BAR];
  HIDDEN_DEV_PATH       *DevPath;
  EDKII_PCI_DEVICE_PPI  *PmcPciIo;
  EFI_HANDLE            Handle;
  EFI_STATUS            Status;

  Handle = NULL;

  Bars[0] = Pwrm;
  BarType[0] = PciBarTypeMem;
  Bars[1] = 0; // BAR1 is currently not programed. Normally it provides access to PCI config space of a hidden device
  BarType[1] = PciBarTypeUnknown;
  Bars[2] = Acpi;
  BarType[2] = PciBarTypeIo;
  CreateHiddenPciDeviceIo (0, 3, Bars, BarType, &PmcPciIo);

  if (PmcPciIo != NULL) {
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gEfiPciIoProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    PmcPciIo
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to install PMC PCI IO\n"));
      return;
    }
    DevPath = AllocateZeroPool(sizeof(HIDDEN_DEV_PATH));
    DevPath->Intc.Acpi.Header.Type = ACPI_DEVICE_PATH;
    DevPath->Intc.Acpi.Header.SubType = ACPI_EXTENDED_DP;
    DevPath->Intc.Acpi.Header.Length[0] = sizeof(INTC_ACPI_EXTENDED_DEVICE_PATH);
    DevPath->Intc.Acpi.UID = UID;
    CopyMem (DevPath->Intc.HidStr, PMC_HID_STR, INTC_HID_STR_SIZE);

    DevPath->End.Type = END_DEVICE_PATH_TYPE;
    DevPath->End.SubType = END_ENTIRE_DEVICE_PATH_SUBTYPE;
    DevPath->End.Length[0] = sizeof(EFI_DEVICE_PATH_PROTOCOL);

    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gEfiDevicePathProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    DevPath
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to install PMC device path\n"));
      return;
    }
  }
}

VOID
InstallHiddenP2SbDevice (
  IN UINT64  P2SbBase,
  IN UINT32  UID
  )
{
  UINT64                Bars[PCI_MAX_HIDDEN_BAR];
  PCI_BAR_TYPE          BarType[PCI_MAX_HIDDEN_BAR];
  HIDDEN_DEV_PATH       *DevPath;
  EDKII_PCI_DEVICE_PPI  *P2SbPciIo;
  EFI_HANDLE            Handle;
  EFI_STATUS            Status;

  Handle = NULL;

  Bars[0] = P2SbBase;
  BarType[0] = PciBarTypeMem;
  CreateHiddenPciDeviceIo (0, 1, Bars, BarType, &P2SbPciIo);

  if (P2SbPciIo != NULL) {
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gEfiPciIoProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    P2SbPciIo
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to install P2Sb PCI IO\n"));
      return;
    }
    DevPath = AllocateZeroPool(sizeof(HIDDEN_DEV_PATH));
    DevPath->Intc.Acpi.Header.Type = ACPI_DEVICE_PATH;
    DevPath->Intc.Acpi.Header.SubType = ACPI_EXTENDED_DP;
    DevPath->Intc.Acpi.Header.Length[0] = sizeof(INTC_ACPI_EXTENDED_DEVICE_PATH);
    DevPath->Intc.Acpi.UID = UID;
    CopyMem (DevPath->Intc.HidStr, P2SB_HID_STR, INTC_HID_STR_SIZE);

    DevPath->End.Type = END_DEVICE_PATH_TYPE;
    DevPath->End.SubType = END_ENTIRE_DEVICE_PATH_SUBTYPE;
    DevPath->End.Length[0] = sizeof(EFI_DEVICE_PATH_PROTOCOL);

    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gEfiDevicePathProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    DevPath
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to install P2Sb device path\n"));
      return;
    }
  }
}

/**
  EarlyDevicesDxe

  @param[in]  ImageHandle       Handle of the file being invoked.
  @param[in]  SystemTable       Pointer to system table.

  @return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
EarlyDevicesEntryPoint (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EDKII_PCI_DEVICE_PPI    *SmbusPciIo;
  EFI_HANDLE              Handle;
  EFI_STATUS              Status;

  SmbusPciIo = NULL;
  Handle = NULL;

  CreatePciDeviceIoFromSbdf (
    0,
    0,
    PCI_DEVICE_NUMBER_PCH_SMBUS,
    PCI_FUNCTION_NUMBER_PCH_SMBUS,
    NULL,
    &SmbusPciIo
    );

  Status = EFI_UNSUPPORTED;
  if (SmbusPciIo != NULL) {
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gEfiPciIoProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    SmbusPciIo
                    );
    gBS->ConnectController (Handle, NULL, NULL, TRUE);
  }

  //
  // All PMC controllers access. PMC is hidden and not discoverable on PCI bus.
  //
  InstallHiddenPmcDevice (PmcGetPwrmBase (), PmcGetAcpiBase (), PMC_PCD_UID);
  InstallHiddenP2SbDevice (PCH_PCR_BASE_ADDRESS, P2SB_PCD_UID);

  InstallMeHandle ();

  return Status;
}