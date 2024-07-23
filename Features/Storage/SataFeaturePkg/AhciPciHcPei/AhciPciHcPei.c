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

#include "AhciPciHcPei.h"

EDKII_ATA_AHCI_HOST_CONTROLLER_PPI  mAhciHostControllerPpi = {
  GetAhciHcMmioBar,
  GetAhciHcDevicePath
};

EFI_PEI_PPI_DESCRIPTOR  mPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiPeiAtaAhciHostControllerPpiGuid,
  &mAhciHostControllerPpi
};

EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  AhciHcEndOfPei
};

UINT32                                MmioBarBase;
UINT32                                MmioBarLimit;

//
// Template for an ATA AHCI host controller
//
ATA_AHCI_DEVICE_PATH  mAtaAhciDevicePathTemplate = {
  {  // PciRoot
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    0x0A0341D0,
    0
  },
  {  // AhciEp
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    0,
    0
  },
  {  // End
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (sizeof (EFI_DEVICE_PATH_PROTOCOL)),
      (UINT8) ((sizeof (EFI_DEVICE_PATH_PROTOCOL)) >> 8)
    }
  }
};


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
  )
{
  AHCI_HC_PEI_PRIVATE_DATA    *Private;

  if ((This == NULL) || (MmioBar == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Private = AHCI_HC_PEI_PRIVATE_DATA_FROM_THIS (This);

  if (ControllerId >= Private->TotalAhciHcs) {
    return EFI_NOT_FOUND;
  }

  *MmioBar = Private->HcInfo[ControllerId].Bar;

  return EFI_SUCCESS;
}

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
  )
{
  AHCI_HC_PEI_PRIVATE_DATA    *Private;

  if ((This == NULL) || (DevicePathLength == NULL) || (DevicePath == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Private = AHCI_HC_PEI_PRIVATE_DATA_FROM_THIS (This);

  if (ControllerId >= Private->TotalAhciHcs) {
    return EFI_NOT_FOUND;
  }

  *DevicePathLength = Private->HcInfo[ControllerId].DevicePathLength;
  *DevicePath       = AllocateCopyPool (*DevicePathLength, Private->HcInfo[ControllerId].DevicePath);
  if (*DevicePath == NULL) {
    *DevicePathLength = 0;
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  Restore the PCI configuration space of the ATA AHCI host controller.

  @param[in] HcInfo    A pointer to the AHCI_HC_INFO structure.

**/
VOID
AhciHcConfigSpaceRestore (
  IN AHCI_HC_INFO    *HcInfo
  )
{
  UINTN    PciHcBase;

  PciHcBase = PCI_LIB_ADDRESS (
              HcInfo->PciInfo.Bus,
              HcInfo->PciInfo.Device,
              HcInfo->PciInfo.Function,
              0
              );

  PciWrite8  (PciHcBase + PCI_COMMAND_OFFSET, HcInfo->OriCmdRegLow);
  PciWrite32 (PciHcBase + PCI_AHCI_BASE_ADDRESSREG_OFFSET, HcInfo->OriABarReg);

}

/**
  Enable the ATA AHCI host controller specified by the PCI information.

  @param[in,out] Private        A pointer to the AHCI_HC_PEI_PRIVATE_DATA structure.
  @param[in]     PciNodeInfo    Data structure that holds the PCI information of
                                the ATA AHCI host controller.

  @retval EFI_SUCCESS    Controller enabled successfully.
  @retval others         Error occurs during the enabling process.

**/
EFI_STATUS
AhciEnableThisHc (
  IN OUT AHCI_HC_PEI_PRIVATE_DATA    *Private,
  IN     UINT8                       RpBus,
  IN     UINT8                       RpDev,
  IN     UINT8                       RpFun
  )
{
  EFI_STATUS              Status;
  UINTN                   HcIndex;
  AHCI_HC_INFO            *HcInfo;
  UINTN                   PciHcBase;
  UINT32                  Size;
  UINT32                  MmioSize;
  ATA_AHCI_DEVICE_PATH    *HcDevicePath;

  if (Private->TotalAhciHcs >= MAX_ATA_AHCI_HCS) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Check if the PCI AHCI host controller has already been enabled.
  //
  for (HcIndex = 0; HcIndex < Private->TotalAhciHcs; HcIndex++) {
    HcInfo = &(Private->HcInfo[HcIndex]);
    if (RpDev == HcInfo->PciInfo.Device &&
        RpFun == HcInfo->PciInfo.Function) {
      //
      // Already started, return EFI_SUCCESS directly.
      //
      return EFI_SUCCESS;
    }
  }

  //
  // Enable this PCI AHCI host controller by setting BME and MSE bits of PCI_CMD register.
  //
  HcIndex = Private->TotalAhciHcs;
  HcInfo  = &(Private->HcInfo[HcIndex]);

  PciHcBase = PCI_LIB_ADDRESS (
                RpBus,
                RpDev,
                RpFun,
                0
                );

  //
  // check if device conntected behind SATA controller
  //
  if (!IsPciAhciHc (PciHcBase)) {
    Status = EFI_UNSUPPORTED;
    goto ErrorExit;
  }

  HcInfo->PciInfo.Bus      = RpBus;
  HcInfo->PciInfo.Device   = RpDev;
  HcInfo->PciInfo.Function = RpFun;
  HcInfo->OriCmdRegLow     = PciRead8 (PciHcBase + PCI_COMMAND_OFFSET);
  HcInfo->OriABarReg       = PciRead32 (PciHcBase + PCI_AHCI_BASE_ADDRESSREG_OFFSET);

  HcInfo->Bar          = MmioBarBase;
  PciAnd8 (PciHcBase + PCI_COMMAND_OFFSET, 0);
  PciWrite32 (PciHcBase + PCI_AHCI_BASE_ADDRESSREG_OFFSET, 0xFFFFFFFF);
  Size = PciRead32 (PciHcBase + PCI_AHCI_BASE_ADDRESSREG_OFFSET);
  if ((Size & 0x07) == 0x0) {
    //
    // According to Serial ATA Advanced Host Controller Interface (AHCI) 1.3.1
    // spec Section 2.1.11, the register is 32-bits wide.
    //
    MmioSize = (~(Size & 0xFFFFFFF0)) + 1;
  } else {
    DEBUG ((DEBUG_ERROR, "%a: Unknown BAR type for an AHCI controller!\n", __FUNCTION__));
    Status = EFI_UNSUPPORTED;
    goto ErrorExit;
  }
  //
  // Roundup the MMIO size to 8KB according to Serial ATA Advanced Host Controller
  // Interface (AHCI) 1.3.1 spec Section 2.1.11.
  //
  MmioSize = ALIGN_VALUE (MmioSize, SIZE_8KB);
  //
  // Check the BAR range upon the limit
  //
  if ((MmioSize > MmioBarLimit) ||
      (MmioBarBase  > MmioBarLimit - MmioSize)) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorExit;
  }
  MmioBarBase += MmioSize;

  //
  // Construct the device path of the AHCI controller.
  //
  HcInfo->DevicePathLength = sizeof (ATA_AHCI_DEVICE_PATH);
  HcInfo->DevicePath       = AllocatePool (HcInfo->DevicePathLength);
  if (HcInfo->DevicePath == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorExit;
  }

  HcDevicePath = (ATA_AHCI_DEVICE_PATH *)HcInfo->DevicePath;
  CopyMem (
    HcDevicePath,
    &mAtaAhciDevicePathTemplate,
    sizeof (ATA_AHCI_DEVICE_PATH)
    );
  HcDevicePath->AhciEp.Device   = RpDev;
  HcDevicePath->AhciEp.Function = RpFun;

  PciWrite32 (PciHcBase + PCI_AHCI_BASE_ADDRESSREG_OFFSET, (UINT32) HcInfo->Bar);
  PciWrite8  (PciHcBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE);
  Private->TotalAhciHcs++;

  DEBUG ((
    DEBUG_INFO,
    "%a: ATA AHCI host controller found at - Bus(0x0), Dev(0x%x), Func(0x%x)!\n",
    __FUNCTION__, HcInfo->PciInfo.Device, HcInfo->PciInfo.Function
    ));
  DEBUG ((
    DEBUG_INFO, "%a: Bar = 0x%x, MmioSize = 0x%x.\n",
    __FUNCTION__, HcInfo->Bar, MmioSize
    ));

  return EFI_SUCCESS;

ErrorExit:
  AhciHcConfigSpaceRestore (HcInfo);
  ZeroMem (HcInfo, sizeof (AHCI_HC_INFO));
  return Status;
}

/**
  Enable the Ahci host controllers on certian PCI bus

  @param[in,out] Private     A pointer to the NVME_HC_PEI_PRIVATE_DATA structure.
  @param[in] RpComplex       Root port complex infomation

  @retval EFI_SUCCESS        The function completes successfully.
  @retval others             The function fails.

**/
EFI_STATUS
AhciEnableHcOnPciBus (
  IN OUT AHCI_HC_PEI_PRIVATE_DATA    *Private,
  IN PCI_ROOT_PORT_COMPLEX           *RpComplex
  )
{
  UINT8                   Bus;
  UINT8                   Dev;
  UINT8                   Fun;

  //
  // Get MMIO base of this root port complex.
  //
  MmioBarBase = RpComplex->MmioBase;
  MmioBarLimit = RpComplex->MmioLimit;
  DEBUG ((DEBUG_INFO, "%a: BarBase = 0x%x, BarLimit = 0x%x.\n", __FUNCTION__, MmioBarBase, MmioBarLimit));

  Bus = RpComplex->Bus;

  //
  // Scan PCI root port on certain PCI bus
  //
  for ( Dev = 0; Dev <= PCI_MAX_DEVICE; Dev++) {
    for ( Fun = 0; Fun <= PCI_MAX_FUNC; Fun++) {

      if( !IsValidPci (Bus, Dev, Fun)) {
        if (Fun == 0) {
          //
          // If function 0 is not implemented, do not scan other functions.
          //
          break;
        }
        continue;
      }

      //
      // Found Sata device, then enable MMIO if Sata connected.
      //
      AhciEnableThisHc (Private, Bus, Dev, Fun);

      if (Fun == 0) {
        if ( !IsPciMultiFunc(Bus, Dev, Fun)) {
          //
          // It is not a multi-function device, do not scan other functions.
          //
          break;
        }
      }

    } // Fun
  } // Dev

  return EFI_SUCCESS;
}

/**
  Enable the ATA AHCI host controllers by programing the PCI configuration space.

  @param[in,out] Private     A pointer to the AHCI_HC_PEI_PRIVATE_DATA structure.

  @retval EFI_SUCCESS        The function completes successfully.
  @retval others             The function fails.

**/
EFI_STATUS
AhciEnableHcs (
  IN OUT AHCI_HC_PEI_PRIVATE_DATA    *Private
  )
{
  UINT16                       RpBus;
  EFI_STATUS                   Status;
  PCI_ROOT_PORT_COMPLEX        RpComplex;

  Status = EFI_SUCCESS;

  //
  // Scan PCIE root port on each PCI bus. By default, we always scan bus 0.
  //
  RpBus = (UINT16) DEFAULT_BUS_NUM;
  while (TRUE) {
    Status = GetNextRootPortComplex (RpBus, &RpComplex);
    if (EFI_ERROR (Status)) {
      break;
    } else {
      AhciEnableHcOnPciBus (Private, &RpComplex);
    }
    RpBus = RpComplex.Bus;
  }

  DEBUG ((
    DEBUG_INFO, "%a: A total of %d AHCI host controller found\n",
    __FUNCTION__, Private->TotalAhciHcs
  ));

  if (Private->TotalAhciHcs == 0) {
    return EFI_NOT_FOUND;
  }

  Private->Signature   = AHCI_HC_PEI_SIGNATURE;
  CopyMem (&Private->AhciControllerPpi, &mAhciHostControllerPpi, sizeof (EDKII_ATA_AHCI_HOST_CONTROLLER_PPI));
  CopyMem (&Private->PpiList, &mPpiList, sizeof (EFI_PEI_PPI_DESCRIPTOR));

  CopyMem (&Private->EndOfPeiNotifyList, &mEndOfPeiNotifyList, sizeof (EFI_PEI_NOTIFY_DESCRIPTOR));
  Private->PpiList.Ppi = &Private->AhciControllerPpi;

  PeiServicesInstallPpi (&Private->PpiList);
  PeiServicesNotifyPpi (&Private->EndOfPeiNotifyList);

  return EFI_SUCCESS;
}

/**
  Initialize ATA AHCI host controller.

  @retval EFI_SUCCESS    The function completes successfully.
  @retval others         The function fails.

**/
EFI_STATUS
AhciPciConfigSpaceProgram (
  VOID
  )
{
  AHCI_HC_PEI_PRIVATE_DATA    *Private;

  //
  // Initialize AHCI controllers
  //
  Private = (AHCI_HC_PEI_PRIVATE_DATA *) AllocateZeroPool (sizeof (AHCI_HC_PEI_PRIVATE_DATA));
  if (Private == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to allocate memory for AHCI_HC_PEI_PRIVATE_DATA.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  AhciEnableHcs (Private);

  return EFI_SUCCESS;
}

/**
  One notified function to restore the controller PCI configuration space at
  EndOfPei.

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
  )
{
  AHCI_HC_PEI_PRIVATE_DATA    *Private;
  UINTN                       HcIndex;

  Private = AHCI_HC_PEI_PRIVATE_DATA_FROM_THIS_NOTIFY (NotifyDescriptor);
  ASSERT (Private->TotalAhciHcs <= MAX_ATA_AHCI_HCS);

  for (HcIndex = 0; HcIndex < Private->TotalAhciHcs; HcIndex++) {
    AhciHcConfigSpaceRestore (&(Private->HcInfo[HcIndex]));
  }

  return EFI_SUCCESS;
}

/**
  Entry point of the PEIM.

  @param[in] FileHandle     Handle of the file being invoked.
  @param[in] PeiServices    Describes the list of possible PEI Services.

  @retval EFI_SUCCESS             Operation performed successfully.
  @retval EFI_OUT_OF_RESOURCES    Not enough memory to allocate.

**/
EFI_STATUS
EFIAPI
AhciPciHcPeimEntry (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS       Status;
  EFI_BOOT_MODE    BootMode;

  Status = PeiServicesGetBootMode (&BootMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to get the current boot mode.\n", __FUNCTION__));
    return Status;
  }

  if ((BootMode == BOOT_ON_S3_RESUME) ||
      (BootMode == BOOT_IN_RECOVERY_MODE) ||
      (BootMode == BOOT_ON_FLASH_UPDATE)) {
    Status = AhciPciConfigSpaceProgram ();
  }

  return Status;
}
