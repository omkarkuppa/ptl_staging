/** @file
  PCH Smbus DXE Driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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
#include "SmbusDxe.h"

extern EFI_GUID gEfiSmbusArpMapGuid;
SMBUS_INSTANCE  *mSmbusContext;

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
  DEBUG ((DEBUG_INFO, "SmbusExecute() Start, SmbusDeviceAddress=%x, Command=%x, Operation=%x\n", (DeviceAddress.SmbusDeviceAddress << 1), Command, Operation));
  InitializeSmbusRegisters ();

  return SmbusExec (
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
InitializeSmbus (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS               Status;
  UINTN                    DataSize;
  VOID                     *Data;
  EFI_PEI_HOB_POINTERS     HobList;
  UINT64                   SmbusBaseAddress;
  SMBUS_CONFIG_HOB         *SmbusConfigHob;

  DEBUG ((DEBUG_INFO, "InitializeSmbus() Start\n"));

  SmbusBaseAddress = SmbusPciCfgBase ();
  if (PciSegmentRead16 (SmbusBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "Smbus is disabled.\n"));
    return EFI_SUCCESS;
  }

  //
  // Get SMBUS CONFIG HOB.
  //
  HobList.Guid = GetFirstGuidHob (&gPchSmbusConfigHobGuid);
  ASSERT (HobList.Guid != NULL);
  SmbusConfigHob = (SMBUS_CONFIG_HOB *) GET_GUID_HOB_DATA (HobList.Guid);

  mSmbusContext = AllocateZeroPool (sizeof (SMBUS_INSTANCE));
  if (mSmbusContext == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  mSmbusContext->Signature                  = PCH_SMBUS_PRIVATE_DATA_SIGNATURE;
  mSmbusContext->SmbusController.Execute    = SmbusExecute;
  mSmbusContext->SmbusController.ArpDevice  = SmbusArpDevice;
  mSmbusContext->SmbusController.GetArpMap  = SmbusGetArpMap;
  mSmbusContext->SmbusController.Notify     = SmbusNotify;
  mSmbusContext->PlatformNumRsvd            = SmbusConfigHob->NumRsvdSmbusAddresses;
  mSmbusContext->PlatformRsvdAddr           = SmbusConfigHob->RsvdSmbusAddressTable;
  ASSERT (mSmbusContext->PlatformNumRsvd <= PCH_MAX_SMBUS_RESERVED_ADDRESS);

  ///
  /// See if PEI already ARPed any devices, and if so, update our device map.
  ///
  /// Get Hob list
  ///
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, (VOID **) &HobList.Raw);
  ASSERT_EFI_ERROR (Status);

  HobList.Raw = GetNextGuidHob (&gEfiSmbusArpMapGuid, HobList.Raw);
  ///
  /// If we found the right hob, store the information. Otherwise, continue.
  ///
  if (HobList.Raw != NULL) {
    Data      = (VOID *) ((UINT8 *) (&HobList.Guid->Name) + sizeof (EFI_GUID));
    DataSize  = HobList.Header->HobLength - sizeof (EFI_HOB_GUID_TYPE);
    CopyMem (mSmbusContext->DeviceMap, Data, DataSize);
    mSmbusContext->DeviceMapEntries = (UINT8) (DataSize / sizeof (EFI_SMBUS_DEVICE_MAP));
  }
  ///
  /// Initialize the NotifyFunctionList
  ///
  InitializeListHead (&mSmbusContext->NotifyFunctionList);

  ///
  /// Install the SMBUS interface
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mSmbusContext->Handle,
                  &gEfiSmbusHcProtocolGuid,
                  &mSmbusContext->SmbusController,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "InitializeSmbus() End\n"));

  return EFI_SUCCESS;
}
