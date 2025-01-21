/** @file
  SMBUS SMM driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

**/

#include <PiSmm.h>
#include <Protocol/SmbusHc.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PciIoLib.h>
#include <Protocol/PciIo.h>
#include "SmbusCommon.h"
#include "SmbusDxeSmm.h"

#define SMM_SMBUS_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('s', 's', 'm', 'b')

typedef struct {
  UINTN                          Signature;
  EFI_HANDLE                     Handle;
  EFI_SMBUS_HC_PROTOCOL          SmbusController;
  EFI_PCI_IO_PROTOCOL            *PciIo;
} SMBUS_PRIVATE_DATA_SMM;

#define SMBUS_SMM_PRIVATE_DATA_FROM_THIS(a)  CR(a, SMBUS_PRIVATE_DATA_SMM, SmbusController, SMM_SMBUS_PRIVATE_DATA_SIGNATURE)

/**
  Set Device address for an Smbus device with a known UDID or perform a general
  ARP of all devices.

  @param[in] This                 Pointer to the instance of the EFI_SMBUS_HC_PROTOCOL.
  @param[in] ArpAll               If TRUE, do a full ARP. Otherwise, just ARP the specified UDID.
  @param[in] SmbusUdid            When doing a directed ARP, ARP the device with this UDID.
  @param[in, out] DeviceAddress   Buffer to store new SMBus Device Address during directed ARP. On output,If
                                  ArpAlll == TRUE, this will contain the newly assigned SMBus Device address.

  @exception EFI_UNSUPPORTED      This functionality is not supported
**/
EFI_STATUS
EFIAPI
SmbusArpDevice (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN      BOOLEAN                       ArpAll,
  IN      EFI_SMBUS_UDID                * SmbusUdid, OPTIONAL
  IN OUT  EFI_SMBUS_DEVICE_ADDRESS      * DeviceAddress OPTIONAL
  )
{
  ///
  /// ARP should be done in DXE SMBUS driver.
  /// Not needed here.
  ///
  return EFI_UNSUPPORTED;
}

/**
  Get a pointer to the assigned mappings of UDID's to SMbus Device Addresses.

  @param[in] This                 Pointer to the instance of the EFI_SMBUS_HC_PROTOCOL.
  @param[in, out] Length          Buffer to contain the lenght of the Device Map, it will be updated to
                                  contain the number of pairs of UDID's mapped to SMBus Device Addresses.
  @param[in, out] SmbusDeviceMap  Buffer to contian a pointer to the Device Map, it will be updated to
                                  point to the first pair in the Device Map

  @exception EFI_UNSUPPORTED      This functionality is not supported
**/
EFI_STATUS
EFIAPI
SmbusGetArpMap (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN OUT  UINTN                         *Length,
  IN OUT  EFI_SMBUS_DEVICE_MAP          **SmbusDeviceMap
  )
{
  ///
  /// ARP should be done in DXE SMBUS driver.
  /// Not needed here.
  ///
  return EFI_UNSUPPORTED;
}

/**
  Register a callback in the event of a Host Notify command being sent by a
  specified SMBus Device.

  @param[in] This                 Pointer to the instance of the EFI_SMBUS_HC_PROTOCOL.
  @param[in] DeviceAddress        Address of the device whose Host Notify command we want to
                                  trap.
  @param[in] Data                 Data of the Host Notify command we want to trap.
  @param[in] NotifyFunction       Function to be called in the event the desired Host Notify
                                  command occurs.

  @exception EFI_UNSUPPORTED      This functionality is not supported
**/
EFI_STATUS
EFIAPI
SmbusNotify (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN      EFI_SMBUS_DEVICE_ADDRESS      DeviceAddress,
  IN      UINTN                         Data,
  IN      EFI_SMBUS_NOTIFY_FUNCTION     NotifyFunction
  )
{
  ///
  /// Not needed for SMM.
  ///
  return EFI_UNSUPPORTED;
}

/**
  Execute an SMBUS operation

  @param[in] This                 The protocol instance
  @param[in] DeviceAddress        The address of the SMBUS device
  @param[in] Command              The SMBUS command
  @param[in] Operation            Which SMBus protocol will be issued
  @param[in] PecCheck             If Packet Error Code Checking is to be used
  @param[in, out] Length          Length of data
  @param[in, out] Buffer          Data buffer

  @retval EFI_SUCCESS             The SMBUS operation is successful
  @retval Other Values            Something error occurred
**/
EFI_STATUS
EFIAPI
SmbusExecute (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN      EFI_SMBUS_DEVICE_ADDRESS      DeviceAddress,
  IN      EFI_SMBUS_DEVICE_COMMAND      Command,
  IN      EFI_SMBUS_OPERATION           Operation,
  IN      BOOLEAN                       PecCheck,
  IN OUT  UINTN                         *Length,
  IN OUT  VOID                          *Buffer
  )
{
  SMBUS_PRIVATE_DATA_SMM  *Private;

  DEBUG ((DEBUG_VERBOSE, "SmbusExecute() Start, SmbusDeviceAddress=%x, Command=%x, Operation=%x\n", (DeviceAddress.SmbusDeviceAddress << 1), Command, Operation));

  Private = SMBUS_SMM_PRIVATE_DATA_FROM_THIS (This);

  return SmbusDispatchCommand (
           Private->PciIo,
           DeviceAddress,
           Command,
           Operation,
           PecCheck,
           Length,
           Buffer
           );
}

/**
  <b>SMBus DXE Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The SMBus module is a DXE driver that provides a standard way to execute an SMBus command.

  - @pre
    - @link _PCH_POLICY PCH_POLICY_HOB @endlink
      - This module uses the parameters in SmbusConfig for platform reserved (non ARP capable) addresses.

  - @result
    The SMBus driver produces EFI_SMBUS_HC_PROTOCOL which is documented in the SMBus Host Controller
    Protocol Specification.

  @param[in] ImageHandle          ImageHandle of this module
  @param[in] SystemTable          EFI System Table

  @retval EFI_SUCCESS             Driver initializes successfully
  @retval Other values            Some error occurred
**/
EFI_STATUS
EFIAPI
SmbusSmmEntryPoint (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS               Status;
  UINTN         HandleCount;
  EFI_HANDLE    *HandleBuffer;
  UINTN         HandleIndex;
  EFI_SMBUS_HC_PROTOCOL  *SmbusHcProtocol;
  SMBUS_PRIVATE_DATA_DXE *DxePrivateData;
  SMBUS_PRIVATE_DATA_SMM *SmmPrivateData;
  UINTN Segment;
  UINTN Bus;
  UINTN Device;
  UINTN Function;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSmbusHcProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || (HandleCount == 0)) {
    return EFI_NOT_FOUND;
  }

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiSmbusHcProtocolGuid,
                    (VOID **)&SmbusHcProtocol
                    );

    if (!EFI_ERROR (Status)) {
      DxePrivateData = SMBUS_PRIVATE_DATA_FROM_THIS (SmbusHcProtocol);
      Status = DxePrivateData->PciIo->GetLocation (DxePrivateData->PciIo, &Segment, &Bus, &Device, &Function);
      if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
      }

      SmmPrivateData = AllocateZeroPool (sizeof (SMBUS_PRIVATE_DATA_SMM));
      if (SmmPrivateData == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CreatePciDeviceIoFromSbdf ((UINT8) Segment, (UINT8) Bus, (UINT8) Device, (UINT8) Function, NULL, (EDKII_PCI_DEVICE_PPI **) &SmmPrivateData->PciIo);

      SmmPrivateData->Signature                  = SMM_SMBUS_PRIVATE_DATA_SIGNATURE;
      SmmPrivateData->SmbusController.Execute    = SmbusExecute;
      SmmPrivateData->SmbusController.ArpDevice  = SmbusArpDevice;
      SmmPrivateData->SmbusController.GetArpMap  = SmbusGetArpMap;
      SmmPrivateData->SmbusController.Notify     = SmbusNotify;

      Status = gSmst->SmmInstallProtocolInterface (
                        &SmmPrivateData->Handle,
                        &gEfiSmmSmbusProtocolGuid,
                        EFI_NATIVE_INTERFACE,
                        &SmmPrivateData->SmbusController
                        );
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }
  }

  gBS->FreePool (HandleBuffer);

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return Status;
}
