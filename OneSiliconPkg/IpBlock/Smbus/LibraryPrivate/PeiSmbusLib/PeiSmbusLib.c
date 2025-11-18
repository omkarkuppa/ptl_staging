/** @file
  PCH Smbus Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#include "PeiSmbusLibInternal.h"
#include <Library/PeiSmbusLib.h>
#include <Register/PchRegs.h>
#include <Register/PmcRegs.h>
#include <Register/TcoRegs.h>
#include <Register/SmbusRegs.h>
#include <Ppi/MemoryDiscovered.h>


/**
  Configures interrupt number and enables interrupt generation for TCO.

  @param[in] SmbusHandle     SmbusHandle Instance
**/
VOID
PchTcoIrqSet (
  IN  SMBUS_HANDLE       *SmbusHandle
  )
{
  UINT32  Data32Or;

  if (SmbusHandle->SocConfig->TcoIrq == 0) {
    return;
  }

  if (SmbusHandle->SmbusSbAccessMmio == NULL) {
    DEBUG ((DEBUG_ERROR, "%a SMBUS Sideband access not supported\n", __FUNCTION__));
    return;
  }

  Data32Or = B_SMBUS_PCR_TCOCFG_IE;

  //
  // IRQx    Value to set in register
  // IRQ9    000b
  // IRQ10   001b
  // IRQ11   010b
  // IRQ20   100b
  // IRQ21   101b
  // IRQ22   110b
  // IRQ23   111b
  //
  switch (SmbusHandle->SocConfig->TcoIrq) {
    case 9:  Data32Or |= V_SMBUS_PCR_TCOCFG_IRQ_9;  break;
    case 10: Data32Or |= V_SMBUS_PCR_TCOCFG_IRQ_10; break;
    case 11: Data32Or |= V_SMBUS_PCR_TCOCFG_IRQ_11; break;
    case 20: Data32Or |= V_SMBUS_PCR_TCOCFG_IRQ_20; break;
    case 21: Data32Or |= V_SMBUS_PCR_TCOCFG_IRQ_21; break;
    case 22: Data32Or |= V_SMBUS_PCR_TCOCFG_IRQ_22; break;
    case 23: Data32Or |= V_SMBUS_PCR_TCOCFG_IRQ_23; break;
    default:
      //
      // Unsupported TCO IRQ
      //
      ASSERT (FALSE);
      return;
  }
  SmbusHandle->SmbusSbAccessMmio->AndThenOr32 (SmbusHandle->SmbusSbAccessMmio, R_SMBUS_PCR_TCOCFG, ~(UINT32)(B_SMBUS_PCR_TCOCFG_IRQSEL | B_SMBUS_PCR_TCOCFG_IE), Data32Or);
  return;
}

/**
  The function performs SMBUS specific programming.

  @param[in] SmbusHandle       SmbusHandle instance

**/
VOID
SmbusPostMemConfigure (
  IN  SMBUS_HANDLE       *SmbusHandle
  )
{
  UINT32                      Data32And;
  UINT32                      Data32Or;
  UINT16                      SmbusIoBase;
  UINT16                      Tco1Sts;
  UINT32                      SmiSts;

  if (SmbusHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return;
  }

  if (SmbusHandle->SocConfig->PowerManagementSupport) {

    if (SmbusHandle->SmbusSbAccessMmio == NULL) {
      DEBUG ((DEBUG_ERROR, "%a SMBUS Sideband access not supported\n", __FUNCTION__));
      return;
    }

    ///
    /// Enable SMBus dynamic clock gating by setting Smbus PCI offset 80h [16,14,12,10,8] = 0b and [5] = 1b respectively
    /// Configure SMBus dynamic clock gating of PGCB (80h[18]) and ROSC TCG (80h[20]) based on policy
    ///
    Data32And = (UINT32) ~(BIT16 | BIT14 | BIT12 | BIT10 | BIT8);
    Data32Or  = BIT5;
    if (SmbusHandle->Config->DynamicPowerGating) {
      Data32And &= (UINT32)~(BIT20 | BIT18);
    } else {
      Data32Or |= (BIT20 | BIT18);
    }
    PciSegmentAndThenOr32 (
      SmbusHandle->Controller->PciCfgBase + R_SMBUS_CFG_SMBSM,
      Data32And,
      Data32Or
      );

    ///
    ///Enable PMCRE bit and Set SMBUS PCR 0x10h [5] = 1 and clear [2,1] = 0
    ///
    Data32And = ~(UINT32) (BIT2 | BIT1);
    Data32Or = 0;
    if (SmbusHandle->Config->DynamicPowerGating) {
      Data32Or = (UINT32) (B_SMBUS_PCR_PCE_HAE | B_SMBUS_PCR_PCE_PMCRE);
    } else {
      Data32And &= ~((UINT32) (B_SMBUS_PCR_PCE_HAE | B_SMBUS_PCR_PCE_PMCRE));
    }
    SmbusHandle->SmbusSbAccessMmio->AndThenOr32 (SmbusHandle->SmbusSbAccessMmio, R_SMBUS_PCR_PCE, Data32And, Data32Or);
  }
  SmbusIoBase = PciSegmentRead16 (SmbusHandle->Controller->PciCfgBase + R_SMBUS_CFG_SBA) & B_SMBUS_CFG_SBA_BA;
  PciSegmentOr8 (SmbusHandle->Controller->PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_IO_SPACE);
  ASSERT (SmbusIoBase != 0);

  ///
  /// Clear SMBUS Alert status in case it got set after enabling SMBALERT# pin
  ///
  IoWrite8 (SmbusIoBase + R_SMBUS_IO_HSTS, B_SMBUS_IO_HSTS_SMSTS);

  if (SmbusHandle->SocConfig->HostNotifyWakeSupport) {
    ///
    /// Enable reception of a Host Notify command as a wake event
    ///
    IoOr8 (SmbusIoBase + R_SMBUS_IO_SCMD, B_SMBUS_IO_SCMD_HNW);
  }

  if (SmbusHandle->Config->SpdWriteDisable) {
    ///
    /// PCH BIOS Spec section 5.16 Security Recommendations - Set SPD write disable bit, Smbus PCI offset 40h[4] = 1b
    ///
    PciSegmentOr8 (SmbusHandle->Controller->PciCfgBase + R_SMBUS_CFG_HCFG, B_SMBUS_CFG_HCFG_SPDWD);
  }

  ///
  /// Check to disable Smbus controller
  ///
  if (SmbusHandle->Config->Enable == FALSE) {

    if (SmbusHandle->SmbusSbAccessMmio == NULL) {
      DEBUG ((DEBUG_ERROR, "%a SMBUS Sideband access not supported\n", __FUNCTION__));
      return;
    }

    ///
    /// Function disable Smbus Controller
    ///
    SmbusHandle->SmbusSbAccessMmio->AndThenOr32 (SmbusHandle->SmbusSbAccessMmio, R_SMBUS_PCR_GC, 0xFFFFFFFF, B_SMBUS_PCR_GC_FD);
    ///
    /// Disable and hide SMBUS in PSF
    ///
    if (SmbusHandle->Callbacks->SmbusDisable != NULL) {
      SmbusHandle->Callbacks->SmbusDisable (SmbusHandle);
    }
    return;
  }
  ///
  /// Configure SMBUS interrupt
  ///
  PciSegmentWrite8 (SmbusHandle->Controller->PciCfgBase + PCI_INT_PIN_OFFSET, SmbusHandle->SocConfig->IntPin);

  ///
  /// Configure TCO IRQ
  ///
  PchTcoIrqSet (SmbusHandle);

  ///
  /// Clear Timeout bit and TCO_STS
  ///
  if (SmbusHandle->SocConfig->TcoSmiTimeoutSupport) {
    Tco1Sts  = IoRead16 (SmbusHandle->SocConfig->TcoBase + R_TCO_IO_TSTS1);
    Tco1Sts |= B_TCO_IO_TSTS1_TIMEOUT;
    IoWrite16 ((SmbusHandle->SocConfig->TcoBase + R_TCO_IO_TSTS1), Tco1Sts);
  }

  SmiSts   = IoRead32 (SmbusHandle->Controller->AcpiBase + R_ACPI_IO_SMI_STS);
  SmiSts  |= B_ACPI_IO_SMI_STS_TCO_STS;
  IoWrite32 ((SmbusHandle->Controller->AcpiBase + R_ACPI_IO_SMI_STS), SmiSts);
}

/**
  Function to be called when SMBus.Execute happens. This will check if
  the SMBus Host Controller has received a Host Notify command. If so, it will
  see if a notification has been reqested on that event and make any callbacks
  that may be necessary.

  @param[in] Private              Pointer to the SMBUS_INSTANCE

**/
STATIC
VOID
CheckNotification (
  IN SMBUS_INSTANCE *Private
  )
{
  EFI_SMBUS_DEVICE_ADDRESS  DeviceAddress;
  UINT8                     SstsReg;
  UINTN                     Data;
  UINTN                     Index;

  if (Private->ArpEnable == FALSE) {
    return;
  }

  DEBUG ((DEBUG_INFO, "PEI CheckNotification() Start\n"));

  if (Private->NotifyFunctionNum == 0) {
    ///
    /// Since no one register it, not need to check.
    ///
    return;
  }

  SstsReg = SmbusIoRead (R_SMBUS_IO_SSTS);
  if (!(SstsReg & B_SMBUS_IO_SSTS_HNS)) {
    ///
    /// Host Notify has not been received
    ///
    return;
  }
  ///
  /// There was a Host Notify, see if any one wants to know about it
  ///
  DeviceAddress.SmbusDeviceAddress = (SmbusIoRead (R_SMBUS_IO_NDA)) >> 1;

  for (Index = 0; Index < Private->NotifyFunctionNum; Index++) {

    if (Private->NotifyFunctionList[Index].DeviceAddress.SmbusDeviceAddress == DeviceAddress.SmbusDeviceAddress) {
      Data = (SmbusIoRead (R_SMBUS_IO_NDHB) << 8) + (SmbusIoRead (R_SMBUS_IO_NDLB));
      if ((UINT16) Private->NotifyFunctionList[Index].Data == (UINT16) Data) {
        ///
        /// We have a match, notify the requested function
        ///
        Private->NotifyFunctionList[Index].NotifyFunction (
                                             &Private->SmbusPpi,
                                             DeviceAddress,
                                             Data
                                             );
      }
    }
  }
  ///
  /// Clear the Notify Status bit and exit.
  ///
  SmbusIoWrite (R_SMBUS_IO_SSTS, B_SMBUS_IO_SSTS_HNS);

  DEBUG ((DEBUG_INFO, "PEI CheckNotification() End\n"));
}

/**
  This function provides a standard way to execute an SMBUS command
  PPI as defined in the SMBus Specification. The data can either be of
  the length byte, word, or a block of data (1 to 32 bytes long).
  The resulting transaction will be either the SMBus Device accepts
  this transaction or this function returns with an error

  @param[in] This                 PEI_SMBUS_PPI instance
  @param[in] DeviceAddress        Smbus device address
  @param[in] Command              Command to be sent
  @param[in] Operation            Which SMBus PPI will be used
  @param[in] PecCheck             Defines if Packet Error Code Checking is to be used
  @param[in, out] Length          How many bytes to read/write. Must be 1 <= Length <= 32 depending on the Operation
  @param[in, out] Buffer          Data buffer

  @retval EFI_SUCCESS             Operation success.
                                  Length will contain the actual number of bytes read.
                                  Buffer will contain the data read.
  @retval Otherwise               Operation failed.
**/
EFI_STATUS
EFIAPI
SmbusExecute (
  IN CONST EFI_PEI_SMBUS2_PPI *This,
  IN EFI_SMBUS_DEVICE_ADDRESS DeviceAddress,
  IN EFI_SMBUS_DEVICE_COMMAND Command,
  IN EFI_SMBUS_OPERATION      Operation,
  IN BOOLEAN                  PecCheck,
  IN OUT UINTN                *Length,
  IN OUT VOID                 *Buffer
  )
{
  EFI_STATUS     Status;
  SMBUS_INSTANCE *Private;
  DEBUG ((DEBUG_EVENT, "PEI SmbusExecute() Start, SmbusDeviceAddress=%x, Command=%x, Operation=%x\n", (DeviceAddress.SmbusDeviceAddress << 1), Command, Operation));
  Private = SMBUS_PRIVATE_DATA_FROM_PPI_THIS (This);

  Status = SmbusExec (
             DeviceAddress,
             Command,
             Operation,
             PecCheck,
             Length,
             Buffer
             );
  ///
  /// Last step, check notification
  ///
  CheckNotification (Private);
  DEBUG ((DEBUG_EVENT, "PEI SmbusExecute() End\n"));
  return Status;
}

/**
  Initializes the SMBus PPI and its PPI descriptor.

  @param[in] Private              Pointer to SMBUS private data structure
**/
VOID
InitializePeiPpi (
  OUT SMBUS_INSTANCE          *Private
  )
{
  Private->PpiDescriptor.Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  Private->PpiDescriptor.Guid  = &gEfiPeiSmbus2PpiGuid;
  Private->PpiDescriptor.Ppi   = &Private->SmbusPpi;

  Private->SmbusPpi.Execute    = SmbusExecute;
  Private->SmbusPpi.ArpDevice  = SmbusArpDevice;
  Private->SmbusPpi.GetArpMap  = SmbusGetArpMap;
  Private->SmbusPpi.Notify     = SmbusNotify;
}

/**
  This function initializes the SmBus driver in PEI.

  @param[in] Private              SMBUS private data structure
  @param[in] SmbusPreMemConfig    Smbus PREMEM config block

**/
VOID
InitializePeiPrivate (
  IN SMBUS_INSTANCE               *Private,
  IN PCH_SMBUS_PREMEM_CONFIG      *SmbusPreMemConfig
  )
{
  if (Private != NULL) {
    Private->Signature    = PCH_SMBUS_PRIVATE_DATA_SIGNATURE;

    if (SmbusPreMemConfig != NULL) {
      Private->PlatformNumRsvd          = SmbusPreMemConfig->NumRsvdSmbusAddresses;
      CopyMem (
        Private->PlatformRsvdAddr,
        SmbusPreMemConfig->RsvdSmbusAddressTable,
        sizeof (SmbusPreMemConfig->RsvdSmbusAddressTable)
        );
      Private->ArpEnable                = (BOOLEAN) SmbusPreMemConfig->ArpEnable;
    }

    InitializePeiPpi (Private);

    Private->DeviceMapEntries         = 0;
    Private->NotifyFunctionNum        = 0;
  }
}

/**
  ReInitialize the SMBus PPI after memory is discovered.

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database
**/
EFI_STATUS
SmbusReInit (
  VOID
  )
{
  EFI_STATUS               Status;
  SMBUS_INSTANCE           *Private;
  SMBUS_INSTANCE           *PrivateOld;
  EFI_PEI_PPI_DESCRIPTOR   *PpiDescriptorOld;
  EFI_PEI_SMBUS2_PPI       *SmbusPpiOld;

  Status = PeiServicesLocatePpi (
             &gEfiPeiSmbus2PpiGuid,
             0,
             &PpiDescriptorOld,
             (void **) &SmbusPpiOld
             );
  ASSERT_EFI_ERROR (Status);

  PrivateOld = SMBUS_PRIVATE_DATA_FROM_PPI_THIS (SmbusPpiOld);

  Private = (SMBUS_INSTANCE *) AllocatePool (sizeof (SMBUS_INSTANCE));
  if (Private == NULL) {
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (Private, PrivateOld, sizeof (SMBUS_INSTANCE));

  InitializePeiPpi (Private);

  Status = PeiServicesReInstallPpi (PpiDescriptorOld, &Private->PpiDescriptor);
  if (EFI_ERROR (Status)) {
    FreePool (Private);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

/**
  Callback of gEfiPeiMemoryDiscoveredPpiGuid.

  @param  PeiServices      Indirect reference to the PEI Services Table.
  @param  NotifyDescriptor Address of the notification descriptor data structure.
  @param  Ppi              Address of the PPI that was installed.

  @return Status of the notification.
          The status code returned from this function is ignored.
**/
EFI_STATUS
EFIAPI
MemoryDiscoveryPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  //
  // Fix pointers in SMBus PPI at post-memory to avoid accessing flash.
  //
  SmbusReInit ();

  return EFI_SUCCESS;
}

// Notify Descriptor used to register a callback
static EFI_PEI_NOTIFY_DESCRIPTOR mReInitializePchSmbusPpiPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  MemoryDiscoveryPpiNotifyCallback
  };

/**
  Initialize the Smbus PPI and program the Smbus BAR

  @param[in]  SMBUS_HANDLE        SmbusHandle instance

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database
**/
EFI_STATUS
SmbusPreMemInit (
  IN  SMBUS_HANDLE       *SmbusHandle
  )
{
  EFI_STATUS                  Status;
  SMBUS_INSTANCE              *Private;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  if (SmbusHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (SmbusHandle->SocConfig->HostTiming) {
    ///
    /// Set SMBus PCI offset 0x64
    ///
    PciSegmentWrite32 (SmbusHandle->Controller->PciCfgBase + R_SMBUS_CFG_HTIM, SmbusHandle->SocConfig->HostTiming);
  }
  if (SmbusHandle->SocConfig->InstallPpi) {

    Private = (SMBUS_INSTANCE *) AllocatePool (sizeof (SMBUS_INSTANCE));
    if (Private == NULL) {
      DEBUG ((DEBUG_ERROR, "Failed to allocate memory for Private! \n"));
      return EFI_OUT_OF_RESOURCES;
    }

    InitializePeiPrivate (Private, SmbusHandle->Config);
    ///
    /// Since PEI has no PCI enumerator, set the BAR & I/O space enable ourselves
    ///
    PciSegmentAndThenOr32 (SmbusHandle->Controller->PciCfgBase + R_SMBUS_CFG_SBA, B_SMBUS_CFG_SBA_BA, SmbusHandle->Config->SmbusIoBase);

    PciSegmentOr8 (SmbusHandle->Controller->PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_IO_SPACE);
    ///
    /// Reset the SMBus host controller
    ///
    PciSegmentOr8 (SmbusHandle->Controller->PciCfgBase + R_SMBUS_CFG_HCFG, B_SMBUS_CFG_HCFG_SSRESET);

    ///
    /// Enable the SMBus host controller
    ///
    PciSegmentAndThenOr8 (
      SmbusHandle->Controller->PciCfgBase + R_SMBUS_CFG_HCFG,
      (UINT8) (~(B_SMBUS_CFG_HCFG_SSEN | B_SMBUS_CFG_HCFG_I2CEN)),
      B_SMBUS_CFG_HCFG_HSTEN
      );

    ///
    /// Clear Status Register before anyone uses the interfaces
    ///
    SmbusIoWrite (R_SMBUS_IO_HSTS, V_SMBUS_IO_HSTS_ALL);

    Status = PeiServicesInstallPpi (&Private->PpiDescriptor);
    ASSERT_EFI_ERROR (Status);

    Status = PeiServicesNotifyPpi (&mReInitializePchSmbusPpiPeiNotifyList);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Enable SMBUS interface pins
  // NOTE: Given that SMB is used very early in the boot this step is either
  // not needed or should be executed earlier.
  //
  if (SmbusHandle->Config->Enable) {
    if (SmbusHandle->Callbacks->SmbusGpioEnable != NULL) {
      SmbusHandle->Callbacks->SmbusGpioEnable (SmbusHandle);
    }
    if (SmbusHandle->Config->SmbAlertEnable) {
      if (SmbusHandle->Callbacks->SmbusAlertEnable != NULL) {
        SmbusHandle->Callbacks->SmbusAlertEnable (SmbusHandle);
      }
    }
  }

  DEBUG((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return EFI_SUCCESS;
}
/**
  This function install a data HOB if any devices were ARPed during PEI.
  It describes what devices were ARPed and  passes these info to DXE.

  @retval EFI_SUCCESS             The function completed successfully.
**/
STATIC
EFI_STATUS
PchSmbusArpEnableInstallDataHob (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_PEI_PPI_DESCRIPTOR  *SmbusDescriptor;
  EFI_PEI_SMBUS2_PPI      *SmbusPpi;
  SMBUS_INSTANCE          *Private;
  UINTN                   BufferSize;
  VOID                    *Hob;

  Status = PeiServicesLocatePpi (
             &gEfiPeiSmbus2PpiGuid, /// GUID
             0,                     /// INSTANCE
             &SmbusDescriptor,      /// PEI_PPI_DESCRIPTOR
             (VOID **) &SmbusPpi    /// PPI
             );
  ASSERT_EFI_ERROR (Status);

  Private     = SMBUS_PRIVATE_DATA_FROM_DESCRIPTOR_THIS (SmbusDescriptor);
  BufferSize  = sizeof (EFI_SMBUS_DEVICE_MAP) * Private->DeviceMapEntries;

  Hob = BuildGuidDataHob (
          &gEfiSmbusArpMapGuid,
          Private->DeviceMap,
          BufferSize
          );
  ASSERT (Hob != NULL);

  return EFI_SUCCESS;
}


/**
  Set SMBus Device address for an Smbus device with a known UDID or perform a general
  ARP of all devices.

  @param[in] This                 Pointer to the instance of the PEI_SMBUS_PPI.
  @param[in] ArpAll               If TRUE, do a full ARP. Otherwise, just ARP the specified UDID.
  @param[in] SmbusUdid            When doing a directed ARP, ARP the device with this UDID.
  @param[in, out] DeviceAddress   Buffer to store new SMBus Device Address during directed ARP.

  @exception EFI_UNSUPPORTED      This functionality is not supported
  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
SmbusArpDevice (
  IN CONST EFI_PEI_SMBUS2_PPI       *This,
  IN      BOOLEAN                   ArpAll,
  IN      EFI_SMBUS_UDID            *SmbusUdid OPTIONAL,
  IN OUT  EFI_SMBUS_DEVICE_ADDRESS  *DeviceAddress OPTIONAL
  )
{
  SMBUS_INSTANCE *Private;
  EFI_STATUS     Status;
  UINT8          OldMapEntries;

  DEBUG ((DEBUG_INFO, "PEI SmbusArpDevice() Start\n"));

  Private       = SMBUS_PRIVATE_DATA_FROM_PPI_THIS (This);

  if (Private->ArpEnable == FALSE) {
    return EFI_UNSUPPORTED;
  }

  OldMapEntries = Private->DeviceMapEntries;

  if (ArpAll) {
    Status = SmbusFullArp (Private);
  } else {
    if ((SmbusUdid == NULL) || (DeviceAddress == NULL)) {
      return EFI_INVALID_PARAMETER;
    }

    Status = SmbusDirectedArp (Private, SmbusUdid, DeviceAddress);
  }

  if (EFI_ERROR (Status)) {
    return Status;
  }
  ///
  /// If we just added the first entry in the device map, set up a HOB so
  /// we can pass the map to DXE.
  ///
  if ((OldMapEntries == 0) && (Private->DeviceMapEntries > 0)) {
    Status = PchSmbusArpEnableInstallDataHob ();
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "PEI SmbusArpDevice() End\n"));

  return EFI_SUCCESS;
}

/**
  Get a pointer to the assigned mappings of UDID's to SMBus Device Addresses.

  @param[in] This                 Pointer to the instance of the PEI_SMBUS_PPI.
  @param[in, out] Length          Buffer to contain the lenght of the Device Map.
  @param[in, out] SmbusDeviceMap  Buffer to contian a pointer to the Device Map.

  @exception EFI_UNSUPPORTED      This functionality is not supported
  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
SmbusGetArpMap (
  IN CONST EFI_PEI_SMBUS2_PPI   *This,
  IN OUT   UINTN                *Length,
  IN OUT   EFI_SMBUS_DEVICE_MAP **SmbusDeviceMap
  )
{
  SMBUS_INSTANCE          *Private;

  Private         = SMBUS_PRIVATE_DATA_FROM_PPI_THIS (This);

  if (Private->ArpEnable == FALSE) {
    return EFI_UNSUPPORTED;
  }

  *Length         = Private->DeviceMapEntries * sizeof (EFI_SMBUS_DEVICE_MAP);
  *SmbusDeviceMap = Private->DeviceMap;
  return EFI_SUCCESS;
}

/**
  Register a callback in the event of a Host Notify command being sent by a
  specified SMBus Device.

  @param[in] This                 The PPI instance
  @param[in] DeviceAddress        Address of the device whose Host Notify command we want to trap.
  @param[in] Data                 Data of the Host Notify command we want to trap.
  @param[in] NotifyFunction       Function to be called in the event the desired Host Notify command occurs.

  @exception EFI_UNSUPPORTED      This functionality is not supported
  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
SmbusNotify (
  IN CONST EFI_PEI_SMBUS2_PPI             *This,
  IN       EFI_SMBUS_DEVICE_ADDRESS       DeviceAddress,
  IN       UINTN                          Data,
  IN       EFI_PEI_SMBUS_NOTIFY2_FUNCTION NotifyFunction
  )
{
  SMBUS_INSTANCE          *Private;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Private = SMBUS_PRIVATE_DATA_FROM_PPI_THIS (This);

  if (Private->ArpEnable == FALSE) {
    return EFI_UNSUPPORTED;
  }

  if (NotifyFunction == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ///
  /// NOTE: Currently there is no periodic event in PEI.
  /// So we just check the Notification at the end of in each
  /// Smbus.Execute function.
  ///
  if (Private->NotifyFunctionNum >= MAX_SMBUS_NOTIFICATION) {
    return EFI_OUT_OF_RESOURCES;
  }

  Private->NotifyFunctionList[Private->NotifyFunctionNum].DeviceAddress.SmbusDeviceAddress = DeviceAddress.SmbusDeviceAddress;
  Private->NotifyFunctionList[Private->NotifyFunctionNum].Data                            = Data;
  Private->NotifyFunctionList[Private->NotifyFunctionNum].NotifyFunction                  = NotifyFunction;
  Private->NotifyFunctionNum++;

  ///
  /// Last step, check notification
  ///
  CheckNotification (Private);

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  return EFI_SUCCESS;
}
