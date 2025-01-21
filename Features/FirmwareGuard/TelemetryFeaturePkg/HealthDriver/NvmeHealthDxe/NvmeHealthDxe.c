/** @file
  Nvme Dxe driver, produces Nvme Health Aip protocol.

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

#include "NvmeHealthDxe.h"

/**
  Check current device is an VMD device or not.

  @param[in]  PciIoProtocol        A pointer to pciio protocol

  @retval TRUE      vmd enabled on this device.
  @retval FALSE     vmd disabled on this device

**/
BOOLEAN
EFIAPI
IsVmdDevice (
  IN EFI_PCI_IO_PROTOCOL       *PciIoProtocol
  )
{
  BOOLEAN              IsVmd = FALSE;
  UINTN                Segment;
  UINTN                Bus;
  UINTN                Device;
  UINTN                Function;

  if (PciIoProtocol == NULL) {
    return IsVmd;
  }

  //
  // Now further check the PCI header
  //
  PciIoProtocol->GetLocation (PciIoProtocol, &Segment, &Bus, &Device, &Function);
  DEBUG ((DEBUG_INFO, "Bus Device and Function are: %x %x %x. \n", Bus, Device, Function));
  if ((GetVmdBusNumber() == Bus) && (GetVmdDevNumber() == Device) && (GetVmdFuncNumber() == Function)) {
    IsVmd = TRUE;
  }
  else {
    IsVmd = FALSE;
  }

  return IsVmd;
}

/**
  get current device's namespace ID

  @param[in]  DevicePath        Current device's path

  @retval NamespaceId       Current device's namespace ID

**/
UINT32
EFIAPI
GetNamespaceId (
  IN  EFI_DEVICE_PATH_PROTOCOL                    *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL         *TempDevicePath;
  NVME_NAMESPACE_DEVICE_PATH       *Node;

  TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) DevicePath;
  while (!IsDevicePathEndType (TempDevicePath)) {
    if (DevicePathType (TempDevicePath) == MESSAGING_DEVICE_PATH &&
          DevicePathSubType (TempDevicePath) == MSG_NVME_NAMESPACE_DP) {
      Node  = (NVME_NAMESPACE_DEVICE_PATH *)TempDevicePath;
      return Node->NamespaceId;
    }
    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }
  return NVME_ALL_NAMESPACES;
}

/**
  get current device's protocol's interface

  @param  DevicePath        Current device's path
  @param  Protocol          Current protocol GUID

  @retval Interface       Current protocol GUID's content return as a pointer

**/
EFI_STATUS
EFIAPI
GetProtocolInterface (
  IN EFI_DEVICE_PATH_PROTOCOL      *DevicePath,
  IN EFI_GUID                      *Protocol,
  OUT VOID                         **Interface
  )
{
  EFI_STATUS                       Status;
  EFI_HANDLE                       Handle;
  EFI_DEVICE_PATH_PROTOCOL         *DevicePathTemp;

  if((DevicePath == NULL) || (Protocol == NULL) || (Interface == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  DevicePathTemp = DevicePath;
  Status = gBS->LocateDevicePath (
                  Protocol,
                  &DevicePathTemp,
                  &Handle
                  );
  if (!EFI_ERROR (Status)) {
    Status = gBS->HandleProtocol (
                  Handle,
                  Protocol,
                  (VOID**)Interface
                  );
    if(EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: HandleProtocol for interface is %r. \n", __FUNCTION__, Status));
      return EFI_NOT_FOUND;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a: LocateDevicePath for handle is %r. \n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

/**
  Get namespace ID is in VMD mode or not.

  @param[in]  IsVmd                 VMD enabled or not.
  @param[in] DevicePath             Current device path.
  @param[out] NameSpace             Current device name space ID.

**/
VOID
GetNameSpaceIdWithVMD (
  IN  BOOLEAN                                 IsVmd,
  IN  EFI_DEVICE_PATH_PROTOCOL                *DevicePath,
  OUT UINT32                                  *NameSpace
  )
{
  UINT32                                    PortId;
  UINT32                                    NamespaceId;


  NamespaceId         = NVME_ALL_NAMESPACES;
  //
  // call rst nvme passthrough protocol to get cfg space, namespace and BDF data
  //
  if (IsVmd) {
    //
    // get namespaceID for current device with VMD enabled.
    //
    NamespaceId = GetNamespaceId (DevicePath);
    DEBUG ((DEBUG_INFO, "%a: NamespaceId is 0x%x. \n", __FUNCTION__, NamespaceId));
    NamespaceId = NamespaceId & 0x7FFFFFFF;
    PortId = 0;
    while (PortId < 31) {
      if ((NamespaceId & (1 << PortId)) != 0) {
        break;
      }
      PortId++;
    }
    NamespaceId = PortId + 1;
    *NameSpace  = NamespaceId;
  } else {
    *NameSpace = NVME_CONTROLLER_ID;
  }
}

/**
  Implement 'Get Log Page' 02h(SMART / Health Information) Command.

  @param[in]   NvmeDevice   The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[out]  NamespaceId  NamespaceId for an NVM Express namespace present on the
                            NVM Express controller
  @param[out]  Buffer       The buffer used to store the log data.

  @return EFI_SUCCESS           Successfully get the identify controller data.
  @return EFI_DEVICE_ERROR      Fail to get the identify controller data.
  @return EFI_OUT_OF_RESOURCES  Unable to allocate required resources.

**/
EFI_STATUS
NvmeGetLogPageSmartInfo (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmeDevice,
  IN  UINT32                                NamespaceId,
  OUT VOID                                  *Buffer
  )
{
  EFI_STATUS                                Status;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;

  Status = EFI_SUCCESS;
  if (NvmeDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));

  DEBUG ((DEBUG_INFO, "OptaneSetup: Sending Get Log Page Command\n"));

  Command.Cdw0.Opcode          = NVME_ADMIN_GET_LOG_PAGE_CMD;
  Command.Nsid                 = NVME_CONTROLLER_ID;

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;
  CommandPacket.TransferBuffer = Buffer;
  CommandPacket.TransferLength = sizeof (NVME_SMART_HEALTH_INFO_LOG);
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  //
  // Set Number of Dwords Lower (NUMDL) to the size of NVME_SMART_HEALTH_INFO_LOG,
  // Log Page Identifier (LID) to LID_SMART_INFO
  //
  Command.Cdw10                = (sizeof (NVME_SMART_HEALTH_INFO_LOG) >> 2) << 16 | LID_SMART_INFO;
  Command.Flags                = CDW10_VALID;

  Status = NvmeDevice->PassThru (
                         NvmeDevice,
                         NamespaceId,
                         &CommandPacket,
                         NULL
                         );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NvmePassthru->PassThru : Status=%r\n", Status));
  }

  return Status;
}

/**
  Get a list of supported information types for this instance of the protocol.

  This function returns a list of InformationType GUIDs that are supported on an
  adapter with this instance of EFI_ADAPTER_INFORMATION_PROTOCOL. The list is returned
  in InfoTypesBuffer, and the number of GUID pointers in InfoTypesBuffer is returned in
  InfoTypesBufferCount.

  @param[in]  This                  A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[out] InfoTypesBuffer       A pointer to the array of InformationType GUIDs that are supported
                                    by This.
  @param[out] InfoTypesBufferCount  A pointer to the number of GUIDs present in InfoTypesBuffer.

  @retval EFI_SUCCESS               The list of information type GUIDs that are supported on this adapter was
                                    returned in InfoTypesBuffer. The number of information type GUIDs was
                                    returned in InfoTypesBufferCount.
  @retval EFI_INVALID_PARAMETER     This is NULL.
  @retval EFI_INVALID_PARAMETER     InfoTypesBuffer is NULL.
  @retval EFI_INVALID_PARAMETER     InfoTypesBufferCount is NULL.
  @retval EFI_OUT_OF_RESOURCES      There is not enough pool memory to store the results.

**/
EFI_STATUS
EFIAPI
NvmeHealthAipGetSupportedTypes (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  OUT EFI_GUID                          **InfoTypesBuffer,
  OUT UINTN                             *InfoTypesBufferCount
  )
{
  if ((This == NULL) || (InfoTypesBuffer == NULL) || (InfoTypesBufferCount == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *InfoTypesBuffer = AllocateCopyPool (sizeof (gAdapterInfoNvmeHealthGuid), &gAdapterInfoNvmeHealthGuid);
  if (*InfoTypesBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *InfoTypesBufferCount = 1;

  return EFI_SUCCESS;
}

/**
  Returns the current state information for the adapter.

  This function returns information of type InformationType from the adapter.
  If an adapter does not support the requested informational type, then
  EFI_UNSUPPORTED is returned.

  @param[in]  This                   A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in]  InformationType        A pointer to an EFI_GUID that defines the contents of InformationBlock.
  @param[out] InformationBlock       The service returns a pointer to the buffer with the InformationBlock
                                     structure which contains details about the data specific to InformationType.
  @param[out] InformationBlockSize   The driver returns the size of the InformationBlock in bytes.

  @retval EFI_SUCCESS                The InformationType information was retrieved.
  @retval EFI_UNSUPPORTED            The InformationType is not known.
  @retval EFI_DEVICE_ERROR           The device reported an error.
  @retval EFI_OUT_OF_RESOURCES       The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER      This is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlock is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlockSize is NULL.

**/
EFI_STATUS
EFIAPI
NvmeHealthAipGetInfo (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  OUT VOID                              **InformationBlock,
  OUT UINTN                             *InformationBlockSize
  )
{
  EFI_STATUS                                           Status;
  EFI_AIP_NVME_HEALTH_STATE                            *NvmeAipPtr;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL                   *NvmePassthru;
  EFI_DEVICE_PATH_PROTOCOL                             *DevicePath;
  EFI_PCI_IO_PROTOCOL                                  *PciIo;
  BOOLEAN                                              IsVmd;
  UINT32                                               NamespaceId;
  UINT8                                                *LogDataBuffer;
  UINT8                                                *LogDataBufferHead;
  UINT32                                               NvmeLogSize;
  UINT32                                               AppendNvmeLogSize;
  UINT32                                               DataLogSize;
  UINT32                                               TotalLogDataStructSize;
  NVME_LOG_DATA                                        DataLogStructHeader;
  UINT32                                               DataLogStructHeaderSize;
  UINT8                                                SmartHealthInfoLog[NVME_SMART_HEALTH_LOG_MAX_SIZE];
  UINT8                                                Index2;
  UINT32                                               TempSum;
  NVME_HEALTH_DEV                                      *NvmeHealthPrivate;
  EFI_TPL                                              OldTpl;

  Status                  = EFI_SUCCESS;
  NvmeLogSize             = EFI_PAGE_SIZE;
  AppendNvmeLogSize       = EFI_PAGE_SIZE;
  DataLogSize             = sizeof (NVME_SMART_HEALTH_INFO_LOG);
  TotalLogDataStructSize  = 0;
  DataLogStructHeaderSize = OFFSET_OF (NVME_LOG_DATA, LogData);
  Index2                  = 0;
  TempSum                 = 0;
  IsVmd                   = FALSE;
  NamespaceId             = NVME_ALL_NAMESPACES;

  if ((This == NULL) || (InformationBlock == NULL) || (InformationBlockSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *InformationBlockSize = 0;
  NvmeHealthPrivate = NVME_HEALTH_INSTANCE_FROM_THIS (This);

  if (!CompareGuid (InformationType, &gAdapterInfoNvmeHealthGuid)) {
    return EFI_UNSUPPORTED;
  }

  *InformationBlock = AllocateZeroPool (NvmeLogSize);
  if (*InformationBlock == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  LogDataBuffer = AllocateZeroPool (NVME_LOG_MAX_SIZE);
  if (LogDataBuffer == NULL) {
    FreePool (*InformationBlock);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Data Log Header (02h Command)
  //
  ZeroMem (&DataLogStructHeader, sizeof (NVME_LOG_DATA));
  DataLogStructHeader.LogIdentifier = SMARTHealth;
  DataLogStructHeader.NameSpaceId   = NVME_ALL_NAMESPACES;
  DataLogStructHeader.DataLength    = DataLogSize;

  NvmeAipPtr = (EFI_AIP_NVME_HEALTH_STATE *) (* InformationBlock);
  NvmeAipPtr->Header.Length            = 0;
  NvmeAipPtr->Header.Revision          = NVME_AIP_HEADER_REVISION;
  NvmeAipPtr->Header.Healthy           = (UINT8) TelemetryHealthy;
  CopyGuid (&NvmeAipPtr->Header.Signature, &gAdapterInfoNvmeHealthGuid);
  NvmeAipPtr->Revision                 = NVME_AIP_DATA_REVISION;
  NvmeAipPtr->LogCount                 = 0;
  LogDataBufferHead                    = (UINT8 *)(&NvmeAipPtr->LogData[0]);

  //
  // raise TPL to block potential NVME protocol uninstallation during this AIP GetInfo() function call
  //
  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
  Status = gBS->HandleProtocol (NvmeHealthPrivate->Handle, &gEfiNvmExpressPassThruProtocolGuid, (VOID **) &NvmePassthru);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gBS->HandleProtocol for gEfiNvmExpressPassThruProtocolGuid: Status=%r\n", Status));
    FreePool (*InformationBlock);
    goto Exit;
  }
  Status = gBS->HandleProtocol (NvmeHealthPrivate->Handle, &gEfiDevicePathProtocolGuid, (VOID **) &DevicePath);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gBS->HandleProtocol for gEfiDevicePathProtocolGuid: Status=%r\n", Status));
    FreePool (*InformationBlock);
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = GetProtocolInterface (
           DevicePath,
           &gEfiPciIoProtocolGuid,
           (VOID**)&PciIo
           );
  if(EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HandleProtocol for PciIo is %r. \n", Status));
    PciIo = NULL;
  } else {
    IsVmd = IsVmdDevice (PciIo);
  }

  GetNameSpaceIdWithVMD (IsVmd, DevicePath, &NamespaceId);

  //
  // Set get log page 02h command to NVME device, 512 bytes data will be returned
  //
  Status = NvmeGetLogPageSmartInfo (NvmePassthru, NamespaceId, LogDataBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NvmeGetLogPageSmartInfo is failed: Status=%r\n", Status));
    FreePool (*InformationBlock);
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // check Nvme unhealthy indicators -- start
  //
  CopyMem (SmartHealthInfoLog, LogDataBuffer, NVME_SMART_HEALTH_LOG_MAX_SIZE);
  if (SmartHealthInfoLog[NVME_CRITICALL_WARNING] != 0) {
    NvmeAipPtr->Header.Healthy = (UINT8) TelemetryUnhealthy;
    DEBUG ((DEBUG_ERROR, "NvmeHealth: [Byte0]'Critical Warning' is Non-Zero value [%x]\n", SmartHealthInfoLog[NVME_CRITICALL_WARNING]));
  }

  if (SmartHealthInfoLog[NVME_ENDURANCE_GROUP_CRITICALL_WARNING] != 0) {
    NvmeAipPtr->Header.Healthy = (UINT8) TelemetryUnhealthy;
    DEBUG ((DEBUG_ERROR, "NvmeHealth: [Byte6]'Endurance Group Critical Warning Summary' is Non-Zero value [%x]\n", SmartHealthInfoLog[NVME_ENDURANCE_GROUP_CRITICALL_WARNING]));
  }

  for (Index2 = NVME_MEDIA_DATA_INTEGRITY_ERRORS_START; Index2 < NVME_MEDIA_DATA_INTEGRITY_ERRORS_END + 1; Index2++) {
    TempSum = TempSum + SmartHealthInfoLog[Index2];
    DEBUG ((DEBUG_ERROR, "[Byte%x] = %x\n", Index2, SmartHealthInfoLog[Index2]));
  }
  if (TempSum != 0) {
    NvmeAipPtr->Header.Healthy = (UINT8) TelemetryUnhealthy;
    DEBUG ((DEBUG_ERROR, "NvmeHealth: [Byte160~175]'Media and Data Integrity Errors' is Non-Zero values.\n"));
  }

  //
  // Reallocate memory if the buffer preallocated is not enough.
  //
  if ((OFFSET_OF (EFI_AIP_NVME_HEALTH_STATE, LogData) + DataLogStructHeaderSize + DataLogSize) > NvmeLogSize) {
    *InformationBlock = ReallocatePool (NvmeLogSize, NvmeLogSize + AppendNvmeLogSize, *InformationBlock);
    if (*InformationBlock == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }
    NvmeAipPtr = (EFI_AIP_NVME_HEALTH_STATE *) (* InformationBlock);
    LogDataBufferHead = (UINT8 *)(&NvmeAipPtr->LogData[0]);
    NvmeLogSize = NvmeLogSize + AppendNvmeLogSize;
  }

  //
  // copy new log header
  //
  CopyMem (LogDataBufferHead, &DataLogStructHeader, DataLogStructHeaderSize);
  LogDataBufferHead = LogDataBufferHead + DataLogStructHeaderSize;
  //
  // copy new log data
  //
  CopyMem (LogDataBufferHead, LogDataBuffer, DataLogSize);

  LogDataBufferHead = LogDataBufferHead + DataLogSize;
  TotalLogDataStructSize = TotalLogDataStructSize + DataLogStructHeaderSize + DataLogSize;
  NvmeAipPtr->LogCount ++;

  NvmeAipPtr->Header.Length = OFFSET_OF (EFI_AIP_NVME_HEALTH_STATE, LogData) + (UINT16) TotalLogDataStructSize;
  *InformationBlockSize = NvmeAipPtr->Header.Length;

Exit:
  if (LogDataBuffer != NULL) {
    FreePool (LogDataBuffer);
  }
  gBS->RestoreTPL (OldTpl);

  return Status;
}

/**
  Sets state information for an adapter.

  This function sends information of type InformationType for an adapter.
  If an adapter does not support the requested information type, then EFI_UNSUPPORTED
  is returned.

  @param[in]  This                   A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in]  InformationType        A pointer to an EFI_GUID that defines the contents of InformationBlock.
  @param[in]  InformationBlock       A pointer to the InformationBlock structure which contains details
                                     about the data specific to InformationType.
  @param[in]  InformationBlockSize   The size of the InformationBlock in bytes.

  @retval EFI_SUCCESS                The information was received and interpreted successfully.
  @retval EFI_UNSUPPORTED            The InformationType is not known.
  @retval EFI_DEVICE_ERROR           The device reported an error.
  @retval EFI_INVALID_PARAMETER      This is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlock is NULL.
  @retval EFI_WRITE_PROTECTED        The InformationType cannot be modified using EFI_ADAPTER_INFO_SET_INFO().

**/
EFI_STATUS
EFIAPI
NvmeHealthAipSetInfo (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  IN  VOID                              *InformationBlock,
  IN  UINTN                             InformationBlockSize
  )
{
  if ((This == NULL) || (InformationBlock == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (!CompareGuid (InformationType, &gAdapterInfoNvmeHealthGuid)) {
    return EFI_UNSUPPORTED;
  }
  return EFI_WRITE_PROTECTED;
}

/**
  Install Nvme Health Telemetry AIP protocol at the end of DXE

  @param[in]  Event
  @param[in]  *Context
**/
VOID
EFIAPI
InitNvmeHealthTelemetryAip (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                       Status;
  NVME_HEALTH_DEV                  *NVMEHealthPrivate;
  UINTN                            HandleCount;
  EFI_HANDLE                       *HandleBuffer;
  UINTN                            Index;
  EFI_ADAPTER_INFORMATION_PROTOCOL *Aip;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiNvmExpressPassThruProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "NvmeHealth : gBS->LocateHandleBuffer : Status=%r\n", Status));
    return;
  }

  for (Index = 0; Index < HandleCount ; Index++) {
    //
    // If AIP already exist, then skip.
    //
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiAdapterInformationProtocolGuid, (VOID **) &Aip);
    if (Status == EFI_SUCCESS) {
      DEBUG ((EFI_D_INFO, "NvmeHealth : AIP protocol already Installed.\n"));
      continue;
    } else if (EFI_ERROR (Status) && Status != EFI_UNSUPPORTED) {
      DEBUG ((EFI_D_ERROR, "NvmeHealth : gBS->HandleProtocol : Status=%r\n", Status));
      goto Exit;
    }

    //
    // Allocate the private device structure for Nvme health
    //
    NVMEHealthPrivate = (NVME_HEALTH_DEV *) AllocateZeroPool (sizeof (NVME_HEALTH_DEV));
    if (NVMEHealthPrivate == NULL) {
      DEBUG ((EFI_D_ERROR, "EFI_OUT_OF_RESOURCES to Allocate EFI_ADAPTER_INFORMATION_PROTOCOL. \n"));
      goto Exit;
    }
    NVMEHealthPrivate->Signature = NVME_HEALTH_INSTANCE_DATA_SIGNATURE;
    NVMEHealthPrivate->Handle = HandleBuffer[Index];

    NVMEHealthPrivate->AdapterInfo.GetInformation    = NvmeHealthAipGetInfo;
    NVMEHealthPrivate->AdapterInfo.SetInformation    = NvmeHealthAipSetInfo;
    NVMEHealthPrivate->AdapterInfo.GetSupportedTypes = NvmeHealthAipGetSupportedTypes;

    Status = gBS->InstallMultipleProtocolInterfaces (
                    &NVMEHealthPrivate->Handle,
                    &gEfiAdapterInformationProtocolGuid,
                    &NVMEHealthPrivate->AdapterInfo,
                    NULL
                    );

    if (EFI_ERROR (Status)) {
      //
      // On error, clean the Aip service context data, and free the memory allocated.
      //
      DEBUG ((EFI_D_ERROR, "NvmeHealth : faild to install AIP protocol, %r.\n", Status));
      FreePool (NVMEHealthPrivate);
      continue;
    }
  }

Exit:
  FreePool (HandleBuffer);
  return;
}

/**
  The user Entry Point for Nvme driver health module.

  This is the entry point for Nvme Driver Health DXE Driver. It
  installs NotifyEvent on gEfiNvmExpressPassThruProtocolGuid.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
NvmeHealthDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  VOID                *Registration;

  EfiCreateProtocolNotifyEvent (
    &gEfiNvmExpressPassThruProtocolGuid,
    TPL_CALLBACK,
    InitNvmeHealthTelemetryAip,
    NULL,
    &Registration
    );

  return EFI_SUCCESS;
}