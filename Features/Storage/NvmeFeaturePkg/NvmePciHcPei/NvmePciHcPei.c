/**@file
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

#include "NvmePciHcPei.h"

EDKII_NVM_EXPRESS_HOST_CONTROLLER_PPI  mNvmeHostControllerPpi = {
  GetNvmeHcMmioBar,
  GetNvmeHcDevicePath
};

EFI_PEI_PPI_DESCRIPTOR  mPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiPeiNvmExpressHostControllerPpiGuid,
  &mNvmeHostControllerPpi
};

EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  NvmeHcEndOfPei
};

UINT32                                   MmioBarBase;
UINT32                                   MmioBarLimit;

//
// Template for a NVM Express Host controller
//
NVME_HC_DEVICE_PATH  mNvmeHcDevicePathTemplate = {
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
  {  // PciBridge
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
  {  // NvmeHcEp
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
  )
{
  NVME_HC_PEI_PRIVATE_DATA    *Private;

  if ((This == NULL) || (MmioBar == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Private = NVME_HC_PEI_PRIVATE_DATA_FROM_THIS (This);

  if (ControllerId >= Private->TotalNvmeHcs) {
    return EFI_NOT_FOUND;
  }

  *MmioBar = (UINTN)Private->HcInfo[ControllerId].Bar;

  return EFI_SUCCESS;
}

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
  )
{
  NVME_HC_PEI_PRIVATE_DATA    *Private;

  if ((This == NULL) || (DevicePath == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Private = NVME_HC_PEI_PRIVATE_DATA_FROM_THIS (This);

  if (ControllerId >= Private->TotalNvmeHcs) {
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
  Program the PCI configuration space of a PCI-PCI bridge to enable it.

  @param[in, out] HcInfo    A pointer to the NVME_HC_INFO structure.

**/
VOID
NvmePciBridgesConfig (
  IN OUT NVME_HC_INFO    *HcInfo
  )
{
  UINTN    PpbBase;
  UINT8    SecondaryBus;

  PpbBase     = PCI_LIB_ADDRESS (
                  HcInfo->Bridge.Bus,
                  HcInfo->Bridge.Device,
                  HcInfo->Bridge.Function,
                  0
                  );
  SecondaryBus = HcInfo->ControllerEndPoint.Bus;

  DEBUG ((
    DEBUG_INFO, "%a: Bridge @ B-D-F(0x%x-0x%x-0x%x), SecondaryBus(0x%x)\n",
    __FUNCTION__,
    HcInfo->Bridge.Bus,
    HcInfo->Bridge.Device,
    HcInfo->Bridge.Function,
    SecondaryBus
    ));

  //
  // Save origin configuration space.
  //
  PciReadBuffer (PpbBase, PCI_BRIDGE_CONFIG_SPACE_STORE_SIZE, HcInfo->OriBridgeConfigSpace);

  //
  // Initialize the configuration space.
  //
  PciAnd16 (
    PpbBase + PCI_COMMAND_OFFSET,
    (UINT16)~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_IO_SPACE)
    );
  PciWrite8 (PpbBase + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, SecondaryBus);
  PciWrite8 (PpbBase + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, SecondaryBus);

  PciWrite8  (PpbBase + PCI_BRIDGE_IO_BASE,                    0xFF);
  PciWrite8  (PpbBase + PCI_BRIDGE_IO_LIMIT,                   0x00);
  PciWrite16 (PpbBase + PCI_BRIDGE_MEMORY_BASE,  (UINT16) RShiftU64 (HcInfo->Bar, 16));
  PciWrite16 (PpbBase + PCI_BRIDGE_MEMORY_LIMIT, (UINT16) RShiftU64 (HcInfo->Bar, 16));
  PciWrite16 (PpbBase + PCI_BRIDGE_PREFETCHABLE_MEMORY_BASE,   0xFFFF);
  PciWrite16 (PpbBase + PCI_BRIDGE_PREFETCHABLE_MEMORY_LIMIT,  0x0000);
  PciWrite32 (PpbBase + PCI_BRIDGE_PREFETCHABLE_BASE_UPPER32,  0xFFFFFFFF);
  PciWrite32 (PpbBase + PCI_BRIDGE_PREFETCHABLE_LIMIT_UPPER32, 0x00000000);
  PciOr16    (PpbBase + PCI_COMMAND_OFFSET, (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));

}

/**
  Restore the PCI configuration space of the NVM Express host controller and its
  relating PCI-PCI bridge.

  @param[in] HcInfo    A pointer to the NVME_HC_INFO structure.

**/
VOID
NvmeRestoreHcPciConfigSpace (
  IN NVME_HC_INFO    *HcInfo
  )
{
  UINTN    PciHcBase;

  //
  // Restore for the Controller Endpoint.
  //
  PciHcBase = PCI_LIB_ADDRESS (
                HcInfo->ControllerEndPoint.Bus,
                HcInfo->ControllerEndPoint.Device,
                HcInfo->ControllerEndPoint.Function,
                0
                );
  PciWrite8  (PciHcBase + PCI_COMMAND_OFFSET, HcInfo->OriCmdRegLow);
  PciWrite32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET, HcInfo->OriBar0Reg);
  PciWrite32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET + 4, HcInfo->OriBar1Reg);

  //
  // Restore for the Pci-Pci Bridge.
  //
  PciHcBase   = PCI_LIB_ADDRESS (
                  HcInfo->Bridge.Bus,
                  HcInfo->Bridge.Device,
                  HcInfo->Bridge.Function,
                  0
                  );
  PciWriteBuffer (PciHcBase, PCI_BRIDGE_CONFIG_SPACE_STORE_SIZE, HcInfo->OriBridgeConfigSpace);

}

/**
  Enable the NVM Express host controller specified by the PCI root port index.

  @param[in,out] Private      A pointer to the NVME_HC_PEI_PRIVATE_DATA structure.
  @param[in]     RpBus        Bus number of the PCI root port.
  @param[in]     RpDev        Device number of the PCI root port.
  @param[in]     RpFun        Function number of the PCI root port.

  @retval EFI_SUCCESS    Controller enabled successfully.
  @retval others         Error occurs during the enabling process.

**/
EFI_STATUS
NvmeEnableThisHc (
  IN OUT NVME_HC_PEI_PRIVATE_DATA    *Private,
  IN     UINT8                       RpBus,
  IN     UINT8                       RpDev,
  IN     UINT8                       RpFun
  )
{
  EFI_STATUS             Status;
  UINTN                  HcIndex;
  NVME_HC_INFO           *HcInfo;
  UINTN                  PciHcBase;
  UINT32                 Size;
  UINT64                 MmioSize;
  NVME_HC_DEVICE_PATH    *HcDevicePath;

  if (Private->TotalNvmeHcs >= MAX_NVME_HCS) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Check if the PCI NVM Express host controller has already been enabled.
  //
  for (HcIndex = 0; HcIndex < Private->TotalNvmeHcs; HcIndex++) {
    HcInfo = &(Private->HcInfo[HcIndex]);
    if ((HcInfo->Bridge.Device == RpDev) && (HcInfo->Bridge.Function == RpFun)
      && (HcInfo->Bridge.Bus== RpBus)) {
      //
      // Already started, return EFI_SUCCESS directly.
      //
      return EFI_SUCCESS;
    }
  }

  HcIndex = Private->TotalNvmeHcs;
  HcInfo  = &(Private->HcInfo[HcIndex]);

  HcInfo->Bridge.Bus                  = RpBus;
  HcInfo->Bridge.Device               = RpDev;
  HcInfo->Bridge.Function             = RpFun;
  HcInfo->ControllerEndPoint.Bus      = (UINT8) (ENDPOINT_PCI_BUS_NUMBER_START + Private->TotalNvmeHcs);
  HcInfo->ControllerEndPoint.Device   = (UINT8) 0;
  HcInfo->ControllerEndPoint.Function = (UINT8) 0;
  HcInfo->Bar                         = MmioBarBase;

  //
  // Configure the Pci-Pci Bridge PCI configuration space.
  //
  NvmePciBridgesConfig (HcInfo);

  //
  // Check endpoint device type
  //

  PciHcBase = PCI_LIB_ADDRESS (
                HcInfo->ControllerEndPoint.Bus,
                HcInfo->ControllerEndPoint.Device,
                HcInfo->ControllerEndPoint.Function,
                0
                );

  HcInfo->OriCmdRegLow                = PciRead8  (PciHcBase + PCI_COMMAND_OFFSET);
  HcInfo->OriBar0Reg                  = PciRead32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET);
  HcInfo->OriBar1Reg                  = PciRead32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET + 4);

  if (!IsPciNvmeHc (PciHcBase)) {
    Status = EFI_UNSUPPORTED;
    goto ErrorExit;
  }

  //
  // Get the address of endpoint BAR 0
  //

  PciAnd8 (PciHcBase + PCI_COMMAND_OFFSET, 0);
  PciWrite32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET, 0xFFFFFFFF);
  Size = PciRead32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET);
  if ((Size & 0x07) == 0x4) {
    //
    // According to Nvm Express 1.1 spec Section 2.1.10, the register is 64-bits wide.
    //
    MmioSize = Size & 0xFFFFFFF0;
    PciWrite32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET + 4, 0xFFFFFFFF);
    Size = PciRead32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET + 4);
    //
    // Fix the length to support some spefic 64 bit BAR
    //
    Size |= ((UINT32)(-1) << HighBitSet32 (Size));
    //
    // Calculate the size of 64bit bar
    //
    MmioSize  |= LShiftU64 ((UINT64) Size, 32);
    MmioSize  = (~(MmioSize)) + 1;
    //
    // Clean the high 32bits of this 64bit BAR to 0 as we only allow a 32bit BAR.
    //
    PciWrite32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET + 4, 0);
  } else {
    DEBUG ((DEBUG_ERROR, "%a: Unknown BAR type for an NVM Express controller!\n", __FUNCTION__));
    Status = EFI_UNSUPPORTED;
    goto ErrorExit;
  }
  //
  // Roundup the MMIO size for 1MB to meet the requirement by Pci-Pci Bridge.
  // Section 3.2.5.8, PCI-to-PCI Bridge Architeture Specification, Revision 1.2.
  //
  MmioSize = ALIGN_VALUE (MmioSize, SIZE_1MB);
  //
  // Check the BAR range upon the limit
  //
  if ((MmioSize > MmioBarLimit) ||
      (MmioBarBase > MmioBarLimit - MmioSize)) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorExit;
  }
  MmioBarBase += (UINT32) MmioSize;

  //
  // Construct the device path of the PCI NVM Express host controller.
  //
  HcInfo->DevicePathLength = sizeof (NVME_HC_DEVICE_PATH);
  HcInfo->DevicePath       = AllocateCopyPool (HcInfo->DevicePathLength, &mNvmeHcDevicePathTemplate);
  if (HcInfo->DevicePath == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorExit;
  }

  HcDevicePath = (NVME_HC_DEVICE_PATH *)HcInfo->DevicePath;
  HcDevicePath->PciBridge.Device   = HcInfo->Bridge.Device;
  HcDevicePath->PciBridge.Function = HcInfo->Bridge.Function;
  HcDevicePath->NvmeHcEp.Device    = HcInfo->ControllerEndPoint.Device;
  HcDevicePath->NvmeHcEp.Function  = HcInfo->ControllerEndPoint.Function;

  PciWrite32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET, (UINT32)HcInfo->Bar);
  PciWrite32 (PciHcBase + PCI_BASE_ADDRESSREG_OFFSET + 4, 0);
  PciWrite8  (PciHcBase + PCI_COMMAND_OFFSET, (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));
  Private->TotalNvmeHcs++;

  DEBUG ((
    DEBUG_INFO,
    "%a: NVM Express host controller found at - Bus(0x%x), Dev(0x%x), Func(0x%x)!\n",
    __FUNCTION__,
    HcInfo->ControllerEndPoint.Bus,
    HcInfo->ControllerEndPoint.Device,
    HcInfo->ControllerEndPoint.Function
    ));
  DEBUG ((
    DEBUG_INFO, "%a: Bar = 0x%x, MmioSize = 0x%x.\n",
    __FUNCTION__, HcInfo->Bar, MmioSize
    ));

  return EFI_SUCCESS;

ErrorExit:
  NvmeRestoreHcPciConfigSpace (HcInfo);
  ZeroMem (HcInfo, sizeof (NVME_HC_INFO));
  return Status;
}

/**
  Enable the NVM Express host controllers on certian PCI bus

  @param[in,out] Private     A pointer to the NVME_HC_PEI_PRIVATE_DATA structure.
  @param[in] RpComplex       Root port complex infomation

  @retval EFI_SUCCESS        The function completes successfully.
  @retval others             The function fails.

**/
EFI_STATUS
NvmeEnableHcOnPciBus (
  IN OUT NVME_HC_PEI_PRIVATE_DATA    *Private,
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
      // Found PCI bridge, then enable MMIO if Nvme connected.
      //
      if (IsPciBridge (Bus, Dev, Fun)) {
        NvmeEnableThisHc (Private, Bus, Dev, Fun);
      }

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
  Enable the NVM Express host controllers by programing the PCI configuration space.

  @param[in,out] Private     A pointer to the NVME_HC_PEI_PRIVATE_DATA structure.

  @retval EFI_SUCCESS        The function completes successfully.
  @retval others             The function fails.

**/
EFI_STATUS
NvmeEnableHcs (
  IN OUT NVME_HC_PEI_PRIVATE_DATA    *Private
  )
{
  UINT16                       RpBus;
  EFI_STATUS                  Status;
  PCI_ROOT_PORT_COMPLEX       RpComplex;

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
      NvmeEnableHcOnPciBus (Private, &RpComplex);
    }
    RpBus = RpComplex.Bus;
  }

  DEBUG ((
    DEBUG_INFO, "%a: A total of %d NVM Express host controller found.\n",
    __FUNCTION__, Private->TotalNvmeHcs
  ));

  if (Private->TotalNvmeHcs == 0) {
    return EFI_NOT_FOUND;
  }

  Private->Signature = NVME_HC_PEI_SIGNATURE;
  CopyMem (&Private->NvmeHostControllerPpi, &mNvmeHostControllerPpi, sizeof (EDKII_NVM_EXPRESS_HOST_CONTROLLER_PPI));
  CopyMem (&Private->PpiList, &mPpiList, sizeof (EFI_PEI_PPI_DESCRIPTOR));

  CopyMem (&Private->EndOfPeiNotifyList, &mEndOfPeiNotifyList, sizeof (EFI_PEI_NOTIFY_DESCRIPTOR));
  Private->PpiList.Ppi = &Private->NvmeHostControllerPpi;

  PeiServicesInstallPpi (&Private->PpiList);
  PeiServicesNotifyPpi (&Private->EndOfPeiNotifyList);

  return EFI_SUCCESS;
}

/**
  Initialize NVM Express host controller.

  @retval EFI_SUCCESS    The function completes successfully.
  @retval others         The function fails.

**/
EFI_STATUS
NvmePciConfigSpaceProgram (
  VOID
  )
{
  EFI_STATUS                  Status;
  NVME_HC_PEI_PRIVATE_DATA    *Private;

  Status = EFI_SUCCESS;

  //
  // Initialize controllers of NVME devices
  //
  Private = (NVME_HC_PEI_PRIVATE_DATA *) AllocateZeroPool (sizeof (NVME_HC_PEI_PRIVATE_DATA));
  if (Private == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to allocate memory for NVME_HC_PEI_PRIVATE_DATA.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  NvmeEnableHcs (Private);
  return Status;
}

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
  )
{
  NVME_HC_PEI_PRIVATE_DATA    *Private;
  UINTN                       HcIndex;

  Private = NVME_HC_PEI_PRIVATE_DATA_FROM_THIS_NOTIFY (NotifyDescriptor);
  ASSERT (Private->TotalNvmeHcs <= MAX_NVME_HCS);

  for (HcIndex = 0; HcIndex < Private->TotalNvmeHcs; HcIndex++) {
    NvmeRestoreHcPciConfigSpace (&(Private->HcInfo[HcIndex]));
  }

  return EFI_SUCCESS;
}

/**
  The user code starts with this function.

  @param  FileHandle             Handle of the file being invoked.
  @param  PeiServices            Describes the list of possible PEI Services.

  @retval EFI_SUCCESS            The driver is successfully initialized.
  @retval Others                 Can't initialize the driver.

**/
EFI_STATUS
EFIAPI
NvmePciHcPeimEntry (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS       Status;
  BOOLEAN          Support;

  Status = NvmeHcSupport(&Support);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: %r.\n", __FUNCTION__, Status));
    return Status;
  }

  if (Support == TRUE) {
    Status = NvmePciConfigSpaceProgram ();
  } else {
    DEBUG ((DEBUG_INFO, "%a: not support.\n", __FUNCTION__));
  }

  return Status;
}
