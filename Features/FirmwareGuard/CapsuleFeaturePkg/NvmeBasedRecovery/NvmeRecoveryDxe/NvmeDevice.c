/**@file
  The file includes major RPMB access and Boot partion read/write commands
  based on NVMe1.4 spec.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <NvmeDevice.h>

/**
  Nvme map one DMA buffer.

  @param PciIo          Pointer to PciIo for Nvme device.
  @param Address        DMA buffer address to be mapped.
  @param NumberOfBytes  Number of bytes to be mapped.

**/
VOID
NvmeMapOneDmaBuffer (
  IN EFI_PCI_IO_PROTOCOL        *PciIo,
  IN EFI_PHYSICAL_ADDRESS       Address,
  IN UINTN                      NumberOfBytes
  )
{
  EFI_STATUS                    Status;
  VOID                          *HostAddress;
  EFI_PHYSICAL_ADDRESS          DeviceAddress;
  VOID                          *Mapping;

  HostAddress = (VOID *) (UINTN) Address;
  Status = PciIo->Map (
                    PciIo,
                    EfiPciIoOperationBusMasterCommonBuffer,
                    HostAddress,
                    &NumberOfBytes,
                    &DeviceAddress,
                    &Mapping
                    );
  ASSERT_EFI_ERROR (Status);
  ASSERT (DeviceAddress == ((EFI_PHYSICAL_ADDRESS) (UINTN) HostAddress));
}

/**
  Allocates pages that are suitable for an OperationBusMasterCommonBuffer or
  OperationBusMasterCommonBuffer64 mapping.

  @param PciIo                  Pointer to PciIo for Nvme device.
  @param Pages                  The number of pages to allocate.
  @param Address                A pointer to store the base system memory address of the
                                allocated range.

  @retval EFI_SUCCESS           The requested memory pages were allocated.
  @retval EFI_UNSUPPORTED       Attributes is unsupported. The only legal attribute bits are
                                MEMORY_WRITE_COMBINE and MEMORY_CACHED.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The memory pages could not be allocated.

**/
EFI_STATUS
NvmeAllocateDmaBuffer (
  IN EFI_PCI_IO_PROTOCOL    *PciIo,
  IN UINTN                  BufferLength,
  OUT VOID                  **Address
  )
{
  EFI_STATUS            Status;

  *Address = NULL;
  Status = PciIo->AllocateBuffer (
                    PciIo,
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES ((UINTN) BufferLength),
                    Address,
                    0
                    );
  if (!EFI_ERROR (Status)) {
    NvmeMapOneDmaBuffer (
      PciIo,
      (EFI_PHYSICAL_ADDRESS) (UINTN) *Address,
      BufferLength
      );
  }
  return Status;
}

/**
  Return the result of RPMB Data Frame based on Figure 465: RPMB Operation Result in Nvme 1.4.

  @param  RpmbDataFrame          The pointer to the GetNvmeRPMBOperationResult data structure.

  @retval EFI_SUCCESS            Successfully read/write the RPMB Configuration Block.
  @retval EFI_DEVICE_ERROR       Fail to read/write the RPMB Configuration Block.
  @retval EFI_INVALID_PARAMETER  the input pointer is invalid.

**/
EFI_STATUS
GetNvmeRpmbOperationResult (
  IN NVME_RPMB_DATA_FRAME                     *RpmbDataFrame
  )
{
  if (RpmbDataFrame == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (RpmbDataFrame->Result == NVME_RPMB_RESULT_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Operation successful ! \n" ));
    return EFI_SUCCESS;
  } else if (RpmbDataFrame->Result == NVME_RPMB_RESULT_GENERAL_FAILURE) {
    DEBUG ((DEBUG_ERROR, "General failure ! \n" ));
    return EFI_DEVICE_ERROR;
  } else if (RpmbDataFrame->Result == NVME_RPMB_RESULT_AHTHENTICATION_FAILURE) {
    DEBUG ((DEBUG_ERROR, "Authentication failure ! \n" ));
    return EFI_DEVICE_ERROR;
  } else if (RpmbDataFrame->Result == NVME_RPMB_RESULT_COUNTER_FAILURE) {
    DEBUG ((DEBUG_ERROR, "Counter failure ! \n" ));
    return EFI_DEVICE_ERROR;
  } else if (RpmbDataFrame->Result == NVME_RPMB_RESULT_ADDRESS_FAILURE) {
    DEBUG ((DEBUG_ERROR, "Address failure ! \n" ));
    return EFI_DEVICE_ERROR;
  } else if (RpmbDataFrame->Result == NVME_RPMB_RESULT_WRITE_FAILURE) {
    DEBUG ((DEBUG_ERROR, "Write failure(data/counter/result write failure) ! \n" ));
    return EFI_DEVICE_ERROR;
  } else if (RpmbDataFrame->Result == NVME_RPMB_RESULT_READ_FAILURE) {
    DEBUG ((DEBUG_ERROR, "Read failure(data/counter/result write failure) ! \n" ));
    return EFI_DEVICE_ERROR;
  } else if (RpmbDataFrame->Result == NVME_RPMB_RESULT_AUTHKEY_NOT_PROGRAMMED) {
    DEBUG ((DEBUG_ERROR, "Authentication Key not yet programmed ! \n" ));
    return EFI_DEVICE_ERROR;
  } else if (RpmbDataFrame->Result == NVME_RPMB_RESULT_INVALID_DCB) {
    DEBUG ((DEBUG_ERROR, "Invalid RPMB Device Configuration Block ! \n" ));
    return EFI_DEVICE_ERROR;
  } else {
    DEBUG ((DEBUG_ERROR, "Unknow Operation Result ! \n" ));
    return EFI_DEVICE_ERROR;
  }
}

/**
  The Firmware Image Download command is used to download all or a portion of an image for a future
  update to the controller.

  @param  Passthru      The pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL protocol.
  @param  NamespaceId   The NamespaceId for current device.
  @param  Buffer        The buffer used to store value read from an external image.
  @param  Length        The size of image.
  @param  DataOffset    Current dataoffset of the buffer in a loop.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
EFIAPI
NvmeStorageFirmwareDownload (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL       *Passthru,
  IN UINT32                                   NamespaceId,
  IN VOID                                     *Buffer,
  IN UINTN                                    Length,
  IN UINTN                                    DataOffset
  )
{
  EFI_STATUS                               Status;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                  Command;
  EFI_NVM_EXPRESS_COMPLETION               Completion;

  Status  = EFI_SUCCESS;

  if (Passthru == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));

  Command.Nsid                 = NVME_CONTROLLER_ID;
  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;

  Command.Cdw0.Opcode          = NVME_ADMIN_FW_IAMGE_DOWNLOAD_CMD;
  CommandPacket.TransferBuffer = Buffer;
  CommandPacket.TransferLength = (UINT32) Length;
  CommandPacket.NvmeCmd->Cdw10 = (UINT32) NUMD (Length);
  CommandPacket.NvmeCmd->Cdw11 = (UINT32) DWORD (DataOffset);

  CommandPacket.NvmeCmd->Flags = CDW10_VALID | CDW11_VALID;
  CommandPacket.NvmeCmd->Nsid  = 0;
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;

  Status = Passthru->PassThru (
                       Passthru,
                       NamespaceId,
                       &CommandPacket,
                       NULL
                       );

  return Status;
}

/**
  The Firmware Commit command is used to modify the firmware image or Boot Partitions.

  @param  Passthru      The pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL protocol.
  @param  NamespaceId   The NamespaceId for current device.
  @param  CommitAction  This field specifies the action that is taken on the image downloaded with the Firmware Image
                        Download command or on a previously downloaded and placed image. (refer to Nvme1.4, Figure 175)

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
EFIAPI
NvmeStorageFirmwareCommit (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL     *Passthru,
  IN UINT32                                 NamespaceId,
  IN UINT32                                 CommitAction
  )
{
  EFI_STATUS                               Status;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                  Command;
  EFI_NVM_EXPRESS_COMPLETION               Completion;

  if (Passthru == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status  = EFI_SUCCESS;
  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));

  Command.Nsid                 = NVME_CONTROLLER_ID;
  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;

  Command.Cdw0.Opcode          = NVME_ADMIN_FW_COMMIT_CMD;
  CommitAction = CommitAction << 3;
  CommandPacket.NvmeCmd->Cdw10 = 0 | CommitAction;

  CommandPacket.NvmeCmd->Flags = CDW10_VALID;
  CommandPacket.NvmeCmd->Nsid  = 0;
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;

  Status = Passthru->PassThru (
                       Passthru,
                       NamespaceId,
                       &CommandPacket,
                       NULL
                       );

  return Status;
}

/**
  Read data from Nvm Express Boot Partition and save it to local memory.

  @param  PciIo             The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  BPRSEL            The configuration value of BPRSEL register.
  @param  CfgSpaceAddress   Current device's config space address.
  @param  IsVmd             VMD enabled flag.
  @param  HostMemoryBuffer  The buffer which is allocated by Host used to store value read from controller.

  @retval EFI_SUCCESS      Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR Fail to read the controller capability register.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex. Or BarIndex not valid for
                                this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_TIMEOUT           The operation is time out.

**/
EFI_STATUS
EFIAPI
NvmeBootPartitionReadToHostByPcie (
  IN EFI_PCI_IO_PROTOCOL                  *PciIo,
  IN NVME_BPRSEL                          BPRSEL,
  IN EFI_PHYSICAL_ADDRESS                 CfgSpaceAddress,
  IN BOOLEAN                              IsVmd,
  IN OUT VOID                             *HostMemoryBuffer
  )
{
  EFI_STATUS                              Status;
  NVME_BPINFO                             BPINFO;
  EFI_PHYSICAL_ADDRESS                    MemSpaceAddress;
  NVME_BPMBL                              BPMBL;
  UINT64                                  Delay;
  UINT64                                  Temp64Data;
  UINT32                                  Temp32Data;
  UINT32                                  Bar0Address;
  UINT32                                  Bar1Address;

  Delay  = 1000;
  Status = EFI_SUCCESS;

  ZeroMem (&BPINFO, REG_BPINFO_SIZE);
  ZeroMem (&BPMBL, REG_BPMBL_SIZE);

  if (IsVmd) {
    Bar0Address = MmioRead32 (CfgSpaceAddress + CFG_SPACE_BAR0_OFFSET);
    Bar1Address = MmioRead32 (CfgSpaceAddress + CFG_SPACE_BAR1_OFFSET);

    MemSpaceAddress = (Bar0Address & 0xFFFFFFF0) + LShiftU64 (Bar1Address, 32);
    BPMBL.Bmbba     = (UINT64) HostMemoryBuffer >> 12;

    //
    // configure BPMBL/BPRSEL when VMD is enabled.
    //
    CopyMem (&Temp64Data, &BPMBL, REG_BPMBL_SIZE);
    MmioWrite64 (MemSpaceAddress + NVME_BPMBL_OFFSET, Temp64Data);
    CopyMem (&Temp32Data, &BPRSEL, REG_BPRSEL_SIZE);
    MmioWrite32 (MemSpaceAddress + NVME_BPRSEL_OFFSET, Temp32Data);

    do {
      MicroSecondDelay (50);
      Temp32Data = MmioRead32 (MemSpaceAddress + NVME_BPINFO_OFFSET);
      CopyMem (&BPINFO, &Temp32Data, REG_BPINFO_SIZE);
      Delay--;
    } while ((BPINFO.Brs != 2) && (Delay != 0));
    MicroSecondDelay (WDC_SN530_PCIE_TIMEOUT);          // It should be judged by different storage vendors
                                                        // The timeout value could be available for WDC SN530,
                                                        // FW version: 405FEE1F_GO

    if (Delay == 0) {
      DEBUG ((DEBUG_ERROR, "Boot Partition PCIE access(VMD mode) time out. \n"));
      Status = EFI_TIMEOUT;
    }
  } else {
    //
    // configure BPMBL/BPRSEL in tranditional PCI mode.
    //
    BPMBL.Bmbba = (UINT64) HostMemoryBuffer >> 12;
    Status = WriteNvmeController64 (PciIo, &BPMBL, NVME_BPMBL_OFFSET);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,"NvmeBootPartitionReadToHostByPcie : writing BPMBL failed. \n"));
    }
    Status = WriteNvmeController32 (PciIo, &BPRSEL, NVME_BPRSEL_OFFSET);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,"NvmeBootPartitionReadToHostByPcie : writing BPRSEL failed. \n"));
    }

    do {
      MicroSecondDelay (50);
      Status = ReadNvmeController32 (PciIo, &BPINFO, NVME_BPINFO_OFFSET);
      Delay--;
    } while ((BPINFO.Brs != 2) && (Delay != 0));
    MicroSecondDelay (WDC_SN530_PCIE_TIMEOUT);          // It should be judged by different storage vendors
                                                        // The timeout value could be available for WDC SN530,
                                                        // FW version: 405FEE1F_GO

    if (Delay == 0) {
      Print (L"  Boot Partition read time out ! \n");
      Status = EFI_TIMEOUT;
    }

  }

  return Status;
}

/**
  Read Nvm Express Boot Partition data by Get Log Page command.

  @param  Passthru          The pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL data structure.
  @param  NamespaceId       The NamespaceId for current device.
  @param  HostMemoryBuffer  The buffer which is allocated by Host used to store value read from controller.
  @param  DataByWords       The data number for each read.
  @param  DataOffset        The current data offset for whole block read.

  @retval EFI_SUCCESS      Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR Fail to read the controller capability register.

**/
EFI_STATUS
EFIAPI
NvmeBootPartitionReadToHostByGetLogPage (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL   *Passthru,
  IN UINT32                               NamespaceId,
  IN OUT VOID                             *HostMemoryBuffer,
  IN UINTN                                DataByWords,
  IN UINTN                                DataOffset
  )
{
  EFI_STATUS                                Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL        *NvmeDevice;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;

  if (Passthru == NULL || HostMemoryBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status      = EFI_SUCCESS;
  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));
  NvmeDevice = Passthru;

  Command.Cdw0.Opcode          = NVME_ADMIN_GET_LOG_PAGE_CMD;
  Command.Nsid                 = NVME_CONTROLLER_ID;

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;
  CommandPacket.TransferBuffer = HostMemoryBuffer;
  CommandPacket.TransferLength = sizeof (NVME_BOOT_PARTITION_INFO_LOG);
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  //
  // Set Number of Dwords Lower (NUMDL) to the size of NVME_SMART_HEALTH_INFO_LOG,
  // Log Page Identifier (LID) to LID_SMART_INFO
  //
  Command.Cdw10                = (((UINT32) NUMD (DataByWords)) << 16) | LID_BP_INFO;     // Bit 8 is 0, means parition 0;
  Command.Cdw11                = 0;                                                             // Hight value for GetlogPage, 0x100 0000;
  Command.Cdw12                = (UINT32) DataOffset + sizeof (NVME_BOOT_PARTITION_HEADER);
  Command.Cdw13                = 0;
  Command.Flags                = CDW10_VALID | CDW11_VALID | CDW12_VALID | CDW13_VALID;

  Status = NvmeDevice->PassThru (
                         NvmeDevice,
                         NamespaceId,
                         &CommandPacket,
                         NULL
                         );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NvmeBootPartitionReadToHost2 : Sending Get Log Page Command, Status=[%r]. \n", Status));
  }

  return Status;
}

/**
  Authentication Key programming is initiated by a Security Send command to program the Authentication
  Key to the specified RPMB target, followed by a subsequent Security Send command to request the result,
  and lastly, the host issues a Security Receive command to retrieve the result.

  @param  Ssp        The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId    Id of the media, changes every time the media is replaced.
  @param  Key        The Authentication key should be updated to the specified RPMB target.
  @param  KeySize    Size of the key.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeAuthenticatedKeyDataProgramming (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  IN  CONST VOID                              *Key,
  IN  UINTN                                   KeySize
  )
{
  EFI_STATUS                               Status;
  NVME_RPMB_DATA_FRAME                     RpmbDataFrame;
  UINT8                                    *ReceiveBuffer;
  UINTN                                    BufferSize;

  BufferSize    = 0;
  ReceiveBuffer = AllocateZeroPool (sizeof (NVME_RPMB_DATA_FRAME));
  if (ReceiveBuffer == NULL) {
     return EFI_OUT_OF_RESOURCES;
  }

  if (KeySize >= RPMB_FRAME_STUFF_BYTES) {
    DEBUG ((DEBUG_ERROR, "Warning: The size of key is [%d], it is a invalidate key. \n", KeySize));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  ZeroMem (&RpmbDataFrame, sizeof (NVME_RPMB_DATA_FRAME));
  //
  // Security Send 1
  //
  CopyMem (&(RpmbDataFrame.Sbakamc[RPMB_FRAME_STUFF_BYTES - KeySize]), Key, KeySize);
  RpmbDataFrame.Rpmessage = NVME_RPMB_AUTHKEY_PROGRAM;
  Status = Ssp->SendData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  &RpmbDataFrame
                  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "RPMB Key Programming : Authenticated Key Data Send 1 Command operation Successfully. \n"));
  } else {
    DEBUG ((DEBUG_ERROR, "RPMB Key Programming : Authenticated Key Data send 1 Command operation failed, Status:[%x]. \n", Status));
  }

  Status = GetNvmeRpmbOperationResult (&RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Key Programming : Operation Result 1 is abnormal. \n"));
    goto Exit;
  }

  //
  // Security Send 2
  //
  ZeroMem (&RpmbDataFrame, NVME_RPMB_DATA_FRAME_SIZE);
  RpmbDataFrame.Rpmessage = NVME_RPMB_RESULT_READ;
  Status = Ssp->SendData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  &RpmbDataFrame
                  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "RPMB Key Programming : Authenticated Key Data Send 2 Command operation succeeded.\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "RPMB Key Programming : Authenticated Key Data send 2 Command operation failed, Status:[%x].\n", Status));
  }

  Status = GetNvmeRpmbOperationResult (&RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Key Programming : Operation Result 2 is abnormal. \n"));
    goto Exit;
  }

  //
  // Security Receive 1
  //
  Status = Ssp->ReceiveData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  (VOID *)ReceiveBuffer,
                  &BufferSize
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Key Programming : Data Receive failed, status [%x]. \n", Status));
    goto Exit;
  }

  Status = GetNvmeRpmbOperationResult ((NVME_RPMB_DATA_FRAME *)ReceiveBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Key Programming: Operation Result 3 is abnormal. \n"));
    goto Exit;
  }

Exit:

  if (ReceiveBuffer != NULL) {
    FreePool (ReceiveBuffer);
  }
  return Status;
}

/**
  The Read Write Counter Value sequence is initiated by a Security Send command to request the Write
  Counter value, followed by a Security Receive command to retrieve the Write Counter result.

  @param  Ssp            The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId        Id of the media, changes every time the media is replaced.
  @param  RpmbTarget     Rpmb target to access.
  @param  ReceiveBuffer  The buffer used to store RPMB data frame value read from controller.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeRpmbReadWriteCounterValue (
  IN     EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN     UINT32                                  MediaId,
  IN     UINT8                                   RpmbTarget,
  IN OUT NVME_RPMB_DATA_FRAME                    *ReceiveBuffer
  )
{
  EFI_STATUS                               Status;
  NVME_RPMB_DATA_FRAME                     RpmbDataFrame;
  UINTN                                    BufferSize;
  UINT8                                    Mac[SHA256_DIGEST_SIZE];
  UINTN                                    MacSize;
  UINT8                                    *Bufferhead;

  if (ReceiveBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  BufferSize = 0;
  MacSize    = SHA256_DIGEST_SIZE;

  ZeroMem (&RpmbDataFrame, sizeof (NVME_RPMB_DATA_FRAME));
  if (GetRandomNumber128 (RpmbDataFrame.Nonce) != TRUE) {
    ASSERT(FALSE);
    return EFI_UNSUPPORTED;
  }

  RpmbDataFrame.Rpmbt = RpmbTarget;
  RpmbDataFrame.Rpmessage = NVME_RPMB_COUNTER_READ;
  Status = Ssp->SendData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  &RpmbDataFrame
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB RWCounter : send Command failed, Status:[%x]. \n", Status));
    goto Exit;
  } else {
    DEBUG ((DEBUG_INFO, "RPMB RWCounter : Send Command succeeded. \n"));
  }

  Status = GetNvmeRpmbOperationResult (&RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB ReadWriteCounterValue : Operation Result 1 is abnormal. \n"));
    goto Exit;
  }

  //
  // Read full supported security protocol list from device.
  //
  ZeroMem (ReceiveBuffer, sizeof (NVME_RPMB_DATA_FRAME));
  Status = Ssp->ReceiveData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  (VOID *)ReceiveBuffer,
                  &BufferSize
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Nvme RWCounter: receive Command failed, Status:[%x]. \n", Status));
    goto Exit;
  }

  Bufferhead = (UINT8 *) ReceiveBuffer;
  CopyMem (&RpmbDataFrame, ReceiveBuffer, sizeof (NVME_RPMB_DATA_FRAME));
  CopyMem (Mac, &(RpmbDataFrame.Sbakamc[RPMB_FRAME_STUFF_BYTES - SHA256_DIGEST_SIZE]), MacSize);

  if (HmacSha256Request (Mac, Bufferhead + RPMB_FRAME_STUFF_BYTES, NVME_RPMB_DATA_FRAME_SIZE - RPMB_FRAME_STUFF_BYTES, TRUE) == FALSE) {
    Status = EFI_DEVICE_ERROR;
  }

  Status = GetNvmeRpmbOperationResult ((NVME_RPMB_DATA_FRAME *)ReceiveBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB ReadWriteCounterValue : Operation Result 2 is abnormal. \n"));
    goto Exit;
  }

Exit:

  return Status;
}

/**
  The Authenticated Data Read implementation initiated by a Security Send command.

  @param  Ssp                The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId            Id of the media, changes every time the media is replaced.
  @param  RpmbTarget         Rpmb target to access.
  @param  ReceiveBuffer      The pointer to the The data Number of 512B blocks.
  @param  ReceiveBufferSize  The size of RPMB configuration block data.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeAuthenticatedDataRead (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  IN  UINT8                                   RpmbTarget,
  OUT VOID                                    *ReceiveBuffer,
  IN  UINTN                                   ReceiveBufferSize
  )
{
  EFI_STATUS                               Status;
  NVME_RPMB_DATA_FRAME                     RpmbDataFrame;
  UINTN                                    BufferSize;
  UINT8                                    *PayloadBuffer;
  UINT8                                    SectorCount;

  BufferSize    = 0;
  Status        = EFI_SUCCESS;
  PayloadBuffer = NULL;

  SectorCount = ReceiveBufferSize % NVME_RPMB_TARGET_MIN_UNIT == 0 ?
                (UINT8)(ReceiveBufferSize / NVME_RPMB_TARGET_MIN_UNIT) :
                (UINT8)((ReceiveBufferSize / NVME_RPMB_TARGET_MIN_UNIT) + 1);

  //
  // Security Send, Fill RPMB Send Data Frame
  //
  ZeroMem (&RpmbDataFrame, NVME_RPMB_DATA_FRAME_SIZE);
  if (GetRandomNumber128 (RpmbDataFrame.Nonce) != TRUE) {
    ASSERT(FALSE);
    return EFI_UNSUPPORTED;
  }

  RpmbDataFrame.Rpmbt      = RpmbTarget;
  RpmbDataFrame.Scount     = SectorCount;
  RpmbDataFrame.Address    = 0;
  RpmbDataFrame.Rpmessage  = NVME_RPMB_AUTHDATA_READ;
  Status = Ssp->SendData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  &RpmbDataFrame
                  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "RPMB Data Block Read : Send Command operation succeeded. \n"));
  } else {
    DEBUG ((DEBUG_ERROR, "RPMB Data Block Read : Send Command operation failed, Status:[%x]. \n", Status));
  }

  Status = GetNvmeRpmbOperationResult (&RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Data Block Read : Operation Result is abnormal. \n"));
    goto Exit;
  }

  PayloadBuffer = AllocateZeroPool (NVME_RPMB_DATA_FRAME_SIZE + SectorCount * NVME_RPMB_TARGET_MIN_UNIT);
  if (PayloadBuffer == NULL) {
     return EFI_OUT_OF_RESOURCES;
  }

  Status = Ssp->ReceiveData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE + SectorCount * NVME_RPMB_TARGET_MIN_UNIT,
                  (VOID *)PayloadBuffer,
                  &BufferSize
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Data Block Read : receive Command failed, Status:[%x]. \n", Status));
    goto Exit;
  }

  CopyMem (&RpmbDataFrame, PayloadBuffer, NVME_RPMB_DATA_FRAME_SIZE);
  Status = GetNvmeRpmbOperationResult (&RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Data Block Read : Operation Result is abnormal. \n"));
    goto Exit;
  }

  CopyMem (ReceiveBuffer, PayloadBuffer + NVME_RPMB_DATA_FRAME_SIZE, ReceiveBufferSize);

Exit:
  if (PayloadBuffer != NULL) {
    FreePool(PayloadBuffer);
  }

  return Status;
}

/**
  The Authenticated Data Write implementation initiated by a Security Send command.

  @param  Ssp                          The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId                      Id of the media, changes every time the media is replaced.
  @param  RpmbTarget                   Rpmb target to access.
  @param  RpmbTargetData               The pointer to the The data Number of 512B blocks.
  @param  ReceiveBufferSize            The size of RPMB configuration block data.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeAuthenticatedDataWrite (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  IN  UINT8                                   RpmbTarget,
  IN  VOID                                    *RpmbTargetData,
  IN  UINT32                                  RpmbTargetDataSize
  )
{
  EFI_STATUS                               Status;
  NVME_RPMB_DATA_FRAME                     RpmbDataFrame;
  NVME_RPMB_DATA_FRAME                     RpmbDataFrameCounter;
  UINT8                                    *RpmbDataFramBlock;
  UINT8                                    *PayloadBuffer;
  UINTN                                    PayloadSize;
  UINTN                                    BufferSize;
  UINT8                                    Mac[SHA256_DIGEST_SIZE];
  UINT8                                    SectorCount;

  PayloadSize   = 0;
  BufferSize    = 0;
  PayloadBuffer = NULL;

  SectorCount = RpmbTargetDataSize % NVME_RPMB_TARGET_MIN_UNIT == 0 ?
                (UINT8)(RpmbTargetDataSize / NVME_RPMB_TARGET_MIN_UNIT) :
                (UINT8)((RpmbTargetDataSize / NVME_RPMB_TARGET_MIN_UNIT) + 1);

  PayloadSize = NVME_RPMB_DATA_FRAME_SIZE + NVME_RPMB_TARGET_MIN_UNIT * SectorCount;
  RpmbDataFramBlock = AllocateZeroPool (PayloadSize);
  if (RpmbDataFramBlock == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = NvmeRpmbReadWriteCounterValue (Ssp, MediaId, RpmbTarget, &RpmbDataFrameCounter);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Read/Write Counter Value failed, Status:[%x]. \n", Status));
    goto Exit;
  }

  ZeroMem (&RpmbDataFrame, NVME_RPMB_DATA_FRAME_SIZE);
  RpmbDataFrame.Wcounter  = RpmbDataFrameCounter.Wcounter;
  RpmbDataFrame.Rpmbt     = RpmbTarget;
  RpmbDataFrame.Scount    = SectorCount;
  RpmbDataFrame.Address   = 0;
  RpmbDataFrame.Rpmessage = NVME_RPMB_AUTHDATA_WRITE;

  CopyMem (RpmbDataFramBlock, &RpmbDataFrame, NVME_RPMB_DATA_FRAME_SIZE);
  CopyMem (RpmbDataFramBlock + NVME_RPMB_DATA_FRAME_SIZE, RpmbTargetData, RpmbTargetDataSize);

  //
  // Generate Mac value by RPMB configuration data
  //
  if (HmacSha256Request (Mac, RpmbDataFramBlock + RPMB_FRAME_STUFF_BYTES, (NVME_RPMB_DATA_TOTAL_SIZE - RPMB_FRAME_STUFF_BYTES), FALSE) == FALSE) {
    DEBUG ((DEBUG_ERROR, "Warning : HmacSha256Request data digest failed. \n"));
    goto Exit;
  }

  CopyMem (&(RpmbDataFrame.Sbakamc[RPMB_FRAME_STUFF_BYTES - SHA256_DIGEST_SIZE]), Mac, SHA256_DIGEST_SIZE);
  CopyMem (RpmbDataFramBlock, &RpmbDataFrame, NVME_RPMB_DATA_FRAME_SIZE);

  //
  // Security Send 1
  //
  Status = Ssp->SendData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  PayloadSize,
                  RpmbDataFramBlock
                  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "RPMB Data Block Write : Send 1 Command operation succeeded. \n"));
  } else {
    DEBUG ((DEBUG_ERROR, "RPMB Data Block Write : Send 1 Command operation failed, Status:[%x]. \n", Status));
  }

  CopyMem (&RpmbDataFrame, RpmbDataFramBlock, NVME_RPMB_DATA_FRAME_SIZE);
  Status = GetNvmeRpmbOperationResult (&RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Data Block Write : Operation Result is abnormal. \n"));
    goto Exit;
  }

  //
  // Security Send 2
  //
  ZeroMem (&RpmbDataFrame, NVME_RPMB_DATA_FRAME_SIZE);
  RpmbDataFrame.Rpmbt     = RpmbTarget;
  RpmbDataFrame.Rpmessage = NVME_RPMB_RESULT_READ;

  Status = Ssp->SendData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  &RpmbDataFrame
                  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "RPMB Data Block Write : Send 2 Command operation succeeded. \n"));
  } else {
    DEBUG ((DEBUG_ERROR, "RPMB Data Block Write : Send 2 Command operation failed, Status:[%x]. \n", Status));
  }

  PayloadBuffer = AllocateZeroPool (NVME_RPMB_DATA_FRAME_SIZE);
  if (PayloadBuffer == NULL) {
     Status = EFI_OUT_OF_RESOURCES;
     goto Exit;
  }

  // Security Receive
  Status = Ssp->ReceiveData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  PayloadBuffer,
                  &BufferSize
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Data Block Write : receive Command failed, Status:[%x]. \n", Status));
    goto Exit;
  }

  ZeroMem (&RpmbDataFrame, NVME_RPMB_DATA_FRAME_SIZE);
  CopyMem (&RpmbDataFrame, PayloadBuffer, NVME_RPMB_DATA_FRAME_SIZE);
  Status = GetNvmeRpmbOperationResult (&RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Data Block Write : Operation Result is abnormal. \n"));
    goto Exit;
  }

  CopyMem (Mac, &(RpmbDataFrame.Sbakamc[RPMB_FRAME_STUFF_BYTES - SHA256_DIGEST_SIZE]), SHA256_DIGEST_SIZE);
  if (HmacSha256Request (Mac, PayloadBuffer + RPMB_FRAME_STUFF_BYTES, NVME_RPMB_DATA_FRAME_SIZE - RPMB_FRAME_STUFF_BYTES, TRUE) == FALSE) {
    DEBUG ((DEBUG_ERROR, "RPMB Nvme Authenticated Data Write : HmacSha256Request data digest failed. \n"));
    Status = EFI_DEVICE_ERROR;
  }

Exit:

  if (RpmbDataFramBlock != NULL) {
    FreePool (RpmbDataFramBlock);
  }

  if (PayloadBuffer != NULL) {
    FreePool(PayloadBuffer);
  }

  return Status;
}

/**
  The Authenticated Device Configuration Block Read implementation initiated by a Security Send command.

  @param  Ssp                The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId            Id of the media, changes every time the media is replaced.
  @param  ReceiveBuffer      The buffer used to store RPMB configuration block data read from controller.
  @param  ReceiveBufferSize  The size of RPMB configuration block data.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeAuthenticatedDeviceConfigurationBlockRead (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  OUT VOID                                    *ReceiveBuffer,
  IN  UINT32                                  ReceiveBufferSize
  )
{
  EFI_STATUS                               Status;
  NVME_RPMB_DATA_FRAME                     RpmbDataFrame;
  UINTN                                    BufferSize;
  UINT8                                    *PayloadBuffer;
  UINT8                                    Mac[SHA256_DIGEST_SIZE];

  BufferSize    = 0;
  Status        = EFI_SUCCESS;
  PayloadBuffer = NULL;
  //
  // Security Send, Fill RPMB Send Data Frame
  //
  ZeroMem (&RpmbDataFrame, NVME_RPMB_DATA_FRAME_SIZE);
  if (GetRandomNumber128 (RpmbDataFrame.Nonce) != TRUE) {
    ASSERT(FALSE);
    return EFI_UNSUPPORTED;
  }

  RpmbDataFrame.Scount    = 1;
  RpmbDataFrame.Rpmessage = NVME_RPMB_DCB_READ;
  Status = Ssp->SendData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  &RpmbDataFrame
                  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "RPMB Configuration Block Read : Send Command operation succeeded. \n"));
  } else {
    DEBUG ((DEBUG_ERROR, "RPMB Configuration Block Read : Send Command operation failed, Status:[%x]. \n", Status));
  }

  Status = GetNvmeRpmbOperationResult (&RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Configuration Block Read : Operation Result is abnormal. \n"));
    goto Exit;
  }

  PayloadBuffer = AllocateZeroPool (ReceiveBufferSize);
  if (PayloadBuffer == NULL) {
     return EFI_OUT_OF_RESOURCES;
  }

  Status = Ssp->ReceiveData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  ReceiveBufferSize,
                  (VOID *)PayloadBuffer,
                  &BufferSize
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Configuration Block Read : receive Command failed, Status:[%x]. \n", Status));
    goto Exit;
  }

  CopyMem (&RpmbDataFrame, PayloadBuffer, NVME_RPMB_DATA_FRAME_SIZE);
  Status = GetNvmeRpmbOperationResult (&RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Configuration Block Read : Operation Result is abnormal. \n"));
    goto Exit;
  }

  CopyMem (Mac, &(RpmbDataFrame.Sbakamc[RPMB_FRAME_STUFF_BYTES - SHA256_DIGEST_SIZE]), SHA256_DIGEST_SIZE);
  if (HmacSha256Request (Mac, PayloadBuffer + RPMB_FRAME_STUFF_BYTES, NVME_RPMB_DATA_FRAME_SIZE - RPMB_FRAME_STUFF_BYTES + NVME_RPMB_CONTENTS_SIZE, TRUE) == FALSE) {
    DEBUG ((DEBUG_ERROR, "RPMB Configuration Block Read : HmacSha256Request data digest failed. \n"));
    Status = EFI_DEVICE_ERROR;
  }

  CopyMem (ReceiveBuffer, PayloadBuffer, NVME_RPMB_DATA_TOTAL_SIZE);

Exit:
  if (PayloadBuffer != NULL) {
    FreePool(PayloadBuffer);
  }

  return Status;
}

/**
  The Authenticated Device Configuration Block Write implementation initiated by a Security Send command.

  @param  Ssp                          The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId                      Id of the media, changes every time the media is replaced.
  @param  RPMBDeviceConfigurationData  The buffer used to store RPMB configuration block data is ready to write
                                       to the configuration register of controller.
  @param  ReceiveBufferSize            The size of RPMB configuration block data.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeAuthenticatedDeviceConfigurationBlockWrite (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  IN  VOID                                    *RpmbDeviceConfigurationData,
  IN  UINT32                                  ReceiveBufferSize
  )
{
  EFI_STATUS                               Status;
  NVME_RPMB_DATA_FRAME                     *RpmbDataFrame;
  UINT8                                    *RpmbDeviceConfigurationBlock;
  UINTN                                    PayloadSize;
  UINTN                                    BufferSize;
  UINT32                                   Wcounter;

  PayloadSize   = 0;
  BufferSize    = 0;

  RpmbDeviceConfigurationBlock = AllocateZeroPool (NVME_RPMB_DATA_TOTAL_SIZE);
  if (RpmbDeviceConfigurationBlock == NULL) {
     return EFI_OUT_OF_RESOURCES;
  }

  Status = NvmeAuthenticatedDeviceConfigurationBlockRead (Ssp, MediaId, RpmbDeviceConfigurationBlock, NVME_RPMB_DATA_TOTAL_SIZE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Read/Write Counter Value failed, Status:[%x]. \n", Status));
    goto Exit;
  }

  //
  // Just need to revice the write counter and all other fields clear to 0
  //
  RpmbDataFrame = (NVME_RPMB_DATA_FRAME *)RpmbDeviceConfigurationBlock;
  Wcounter = RpmbDataFrame->Wcounter;
  ZeroMem (RpmbDataFrame, NVME_RPMB_DATA_FRAME_SIZE);
  RpmbDataFrame->Wcounter  = Wcounter;
  RpmbDataFrame->Scount    = 1;
  RpmbDataFrame->Rpmessage = NVME_RPMB_DCB_WRITE;

  CopyMem (RpmbDeviceConfigurationBlock + NVME_RPMB_DATA_FRAME_SIZE, RpmbDeviceConfigurationData, NVME_RPMB_CONTENTS_SIZE);
  PayloadSize = NVME_RPMB_DATA_TOTAL_SIZE;

  //
  // Generate Mac value by RPMB configuration data
  //
  if (HmacSha256Request (&(RpmbDataFrame->Sbakamc[RPMB_FRAME_STUFF_BYTES - SHA256_DIGEST_SIZE]), RpmbDeviceConfigurationBlock + RPMB_FRAME_STUFF_BYTES, (NVME_RPMB_DATA_TOTAL_SIZE - RPMB_FRAME_STUFF_BYTES), FALSE) == FALSE) {
    DEBUG ((DEBUG_ERROR, "Warning : HmacSha256Request data digest failed. \n"));
    goto Exit;
  }

  //
  // Security Send 1
  //
  Status = Ssp->SendData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  PayloadSize,
                  RpmbDeviceConfigurationBlock
                  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "RPMB Configuration Block Write : Send 1 Command operation succeeded. \n"));
  } else {
    DEBUG ((DEBUG_ERROR, "RPMB Configuration Block Write : Send 1 Command operation failed, Status:[%x]. \n", Status));
  }

  Status = GetNvmeRpmbOperationResult (RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Configuration Block Write : Operation Result is abnormal. \n"));
    goto Exit;
  }

  //
  // Security Send 2
  //
  ZeroMem (RpmbDataFrame, NVME_RPMB_DATA_FRAME_SIZE);
  RpmbDataFrame->Rpmessage = NVME_RPMB_RESULT_READ;

  Status = Ssp->SendData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  RpmbDataFrame
                  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "RPMB Configuration Block Write : Send 2 Command operation succeeded. \n"));
  } else {
    DEBUG ((DEBUG_ERROR, "RPMB Configuration Block Write : Send 2 Command operation failed, Status:[%x]. \n", Status));
  }

  // Security Receive
  Status = Ssp->ReceiveData (
                  Ssp,
                  MediaId,
                  NVME_SSP_TIMEOUT,
                  SECURITY_PROTOCOL_NVME,
                  SwapBytes16 (NSSF_RPMB_TARGET),
                  NVME_RPMB_DATA_FRAME_SIZE,
                  RpmbDataFrame,
                  &BufferSize
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Configuration Block Write : receive Command failed, Status:[%x]. \n", Status));
    goto Exit;
  }

  Status = GetNvmeRpmbOperationResult (RpmbDataFrame);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB Configuration Block Write : Operation Result is abnormal. \n"));
    goto Exit;
  }

Exit:

  if (RpmbDeviceConfigurationBlock != NULL) {
    FreePool (RpmbDeviceConfigurationBlock);
  }

  return Status;
}

/**
  Read Nvm Express Controller Registers data with 32-bit width.

  @param  PciIo            The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  NVMeReg          The buffer used to store value read from according the offset of registers.
  @param  RegOffset        The controller register offset.

  @retval EFI_SUCCESS           Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR      Fail to read the controller capability register.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex. Or BarIndex not valid for
                                this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/

EFI_STATUS
ReadNvmeController32 (
  IN EFI_PCI_IO_PROTOCOL                  *PciIo,
  IN OUT VOID                             *NvmeReg,
  IN UINT32                               RegOffset
  )
{
  EFI_STATUS            Status;
  UINT32                Data;
  if(PciIo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PciIo->Mem.Read (
                        PciIo,
                        EfiPciIoWidthUint32,
                        NVME_BAR,
                        RegOffset,
                        1,
                        &Data
                        );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  WriteUnaligned32 ((UINT32*)NvmeReg, Data);
  return EFI_SUCCESS;
}

/**
  Write Nvm Express Controller Registers data with 32-bit width.

  @param  PciIo            The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  NVMeReg          The buffer used to store value read from according the offset of registers.
  @param  RegOffset        The controller register offset.

  @retval EFI_SUCCESS           Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR      Fail to read the controller capability register.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex. Or BarIndex not valid for
                                this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/
EFI_STATUS
WriteNvmeController32(
  IN EFI_PCI_IO_PROTOCOL                  *PciIo,
  IN VOID                                 *NvmeReg,
  IN UINT32                               RegOffset
  )
{
  EFI_STATUS            Status;
  UINT32                Data;

  if(PciIo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Data   = ReadUnaligned32 ((UINT32*)NvmeReg);
  Status = PciIo->Mem.Write (
                        PciIo,
                        EfiPciIoWidthUint32,
                        NVME_BAR,
                        RegOffset,
                        1,
                        &Data
                        );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  WriteUnaligned32 ((UINT32*)NvmeReg, Data);
  return EFI_SUCCESS;
}

/**
  Read Nvm Express Controller Registers data with 64-bit width.

  @param  PciIo            The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  NvmeReg          The buffer used to store value read from according the offset of registers.
  @param  RegOffset        The controller register offset.

  @retval EFI_SUCCESS           Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR      Fail to read the controller capability register.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex. Or BarIndex not valid for
                                this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/
EFI_STATUS
ReadNvmeController64 (
  IN EFI_PCI_IO_PROTOCOL                  *PciIo,
  IN OUT VOID                             *NvmeReg,
  IN UINT32                               RegOffset
  )
{
  EFI_STATUS            Status;
  UINT64                Data;

  if(PciIo == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Data   = 0;
  Status = PciIo->Mem.Read (
                        PciIo,
                        EfiPciIoWidthUint64,
                        NVME_BAR,
                        RegOffset,
                        1,
                        &Data
                        );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  WriteUnaligned64 ((UINT64*)NvmeReg, Data);

  return EFI_SUCCESS;
}

/**
  Write Nvm Express Controller Registers data with 64-bit width.

  @param  PciIo            The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  NVMeReg          The buffer used to store value read from according the offset of registers.
  @param  RegOffset        The controller register offset.

  @retval EFI_SUCCESS           Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR      Fail to read the controller capability register.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex. Or BarIndex not valid for
                                this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/
EFI_STATUS
WriteNvmeController64(
  IN EFI_PCI_IO_PROTOCOL                  *PciIo,
  IN VOID                                 *NvmeReg,
  IN UINT32                               RegOffset
  )
{
  EFI_STATUS            Status;
  UINT64                Data;

  if(PciIo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Data   = ReadUnaligned64 ((UINT64*)NvmeReg);
  Status = PciIo->Mem.Write (
                        PciIo,
                        EfiPciIoWidthUint32,
                        NVME_BAR,
                        RegOffset,
                        2,
                        &Data
                        );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Write data to Boot partition after partition is unlocked.

  @param  Passthru                   A pointer to EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
  @param  NamespaceId                Current device NameSpaceId.
  @param  FirmwareImageBuffer        A pointer to the data should be saved to BP.
  @param  NvmeRecBPSize              The size of image in memory.
  @param  WriteLength                Size of per chunk.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeBootPartitionWrite (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL      *Passthru,
  IN  UINT32                                  NamespaceId,
  IN  UINT8                                   *FirmwareImageBuffer,
  IN  UINTN                                   NvmeRecBPSize,
  IN  UINTN                                   WriteLength
  )
{
  EFI_STATUS                               Status;
  EFI_STATUS                               DLStatus;
  UINTN                                    WriteCount;
  UINTN                                    WriteLoop;

  DLStatus   = EFI_SUCCESS;
  WriteCount = 0;

  WriteCount = ((NvmeRecBPSize % WriteLength) == 0 ?
               (NvmeRecBPSize / WriteLength) :((NvmeRecBPSize / WriteLength) + 1));
  DEBUG ((DEBUG_ERROR, "WriteCount = %d, WriteLength = %d.\n", WriteCount, WriteLength));
  for (WriteLoop = 0; WriteLoop < WriteCount; WriteLoop++) {
    Status = NvmeStorageFirmwareDownload (Passthru, NamespaceId, FirmwareImageBuffer + WriteLoop * WriteLength,
                                          WriteLength, WriteLoop * WriteLength);
    DLStatus = DLStatus | Status;
  }

  if (EFI_ERROR (DLStatus)) {
    DEBUG ((DEBUG_ERROR, "Firmware download command failed, Status:[%x].\n", Status));
    return DLStatus;
  }

  //
  // start commit image
  //
  Status = NvmeStorageFirmwareCommit (Passthru, NamespaceId, 6);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Firmware Commit command failed, Status:[%x].\n", Status));
    return Status;
  }

  return Status;
}

/**
  Read data from Boot partition thru Get Log Page command, caller should free the memory.

  @param  Passthru              A pointer to EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
  @param  NamespaceId           Current device NameSpaceId.
  @param  HostMemoryBuffer      A pointer to memory can be saved the data read from BP.
  @param  NvmeRecBPSize         The size of image in memory.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeBootPartitionGetLogPageRead (
  IN     EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL      *Passthru,
  IN     UINT32                                  NamespaceId,
  IN OUT VOID                                    **HostMemoryBuffer,
  IN OUT UINTN                                   NvmeRecBPSize
  )
{
  EFI_STATUS                               Status;
  EFI_STATUS                               DLStatus;
  UINTN                                    ReadLoop;
  UINTN                                    ReadCount;

  DLStatus  = EFI_SUCCESS;
  ReadCount = ((NvmeRecBPSize % NVME_16K_PORTION_READ_UNIT) == 0 ?
              (NvmeRecBPSize / NVME_16K_PORTION_READ_UNIT) :
              ((NvmeRecBPSize / NVME_16K_PORTION_READ_UNIT) + 1));

  *HostMemoryBuffer = AllocateZeroPool (ReadCount * NVME_16K_PORTION_READ_UNIT);
  if (*HostMemoryBuffer == NULL) {
     return EFI_OUT_OF_RESOURCES;
  }

  for (ReadLoop = 0; ReadLoop < ReadCount; ReadLoop++) {
   Status = NvmeBootPartitionReadToHostByGetLogPage (Passthru, NamespaceId, (UINT8 *)*HostMemoryBuffer + ReadLoop * NVME_16K_PORTION_READ_UNIT,
                                                     NVME_16K_PORTION_READ_UNIT, ReadLoop * NVME_16K_PORTION_READ_UNIT);
   DLStatus = DLStatus | Status;
  }

  return DLStatus;
}

/**
  Read data from Boot partition thru PCIE interface, caller should free the memory.

  @param  PciIo             The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  CfgSpaceAddress   Current device's config space address.
  @param  IsVmd             VMD enabled flag.
  @param  HostMemoryBuffer  A pointer to memory can be saved the data read from BP.
  @param  NvmeRecBPSize     The size of image in memory.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeBootPartitionPcieRead (
  IN     EFI_PCI_IO_PROTOCOL                       *PciIo,
  IN     EFI_PHYSICAL_ADDRESS                      CfgSpaceAddress,
  IN     BOOLEAN                                   IsVmd,
  IN OUT VOID                                      **HostMemoryBuffer,
  IN OUT UINTN                                     NvmeRecBPSize
  )
{
  EFI_STATUS                               Status;
  EFI_STATUS                               DLStatus;
  UINTN                                    ReadLoop;
  UINTN                                    ReadCount;
  NVME_BPRSEL                              BPRSEL;
  DLStatus   = EFI_SUCCESS;

  ReadCount = ((NvmeRecBPSize % NVME_2MB_PORTION_READ_UNIT) == 0 ?
              (NvmeRecBPSize / NVME_2MB_PORTION_READ_UNIT) :
              ((NvmeRecBPSize / NVME_2MB_PORTION_READ_UNIT) + 1));

  Status = NvmeAllocateDmaBuffer (PciIo, ReadCount * NVME_2MB_PORTION_READ_UNIT, (VOID **)HostMemoryBuffer);
  if (*HostMemoryBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  BPRSEL.Bprsz = NVME_2MB_PORTION_READ_UNIT / 0x1000;
  BPRSEL.Bpid  = 0;
  for (ReadLoop = 0; ReadLoop < ReadCount; ReadLoop++) {
    BPRSEL.Bprof = (UINT32) ReadLoop * BPRSEL.Bprsz;
    Status = NvmeBootPartitionReadToHostByPcie (PciIo, BPRSEL, CfgSpaceAddress, IsVmd,
                                                (UINT8 *)*HostMemoryBuffer + ReadLoop * NVME_2MB_PORTION_READ_UNIT);
    DLStatus = DLStatus | Status;
  }

  return DLStatus;
}

/**
  Get identify controller data.

  @param  Passthru         The pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL data structure.
  @param  NamespaceId      Current device NameSpaceId.
  @param  Buffer           The buffer used to store the identify controller data.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeIdentifyController2 (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *Passthru,
  IN UINT32                             NamespaceId,
  IN VOID                               *Buffer
  )
{
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                  Command;
  EFI_NVM_EXPRESS_COMPLETION               Completion;
  EFI_STATUS                               Status;

  ZeroMem (&CommandPacket, sizeof(EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof(EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof(EFI_NVM_EXPRESS_COMPLETION));

  Command.Cdw0.Opcode = NVME_ADMIN_IDENTIFY_CMD;
  //
  // According to Nvm Express 1.1 spec Figure 38, When not used, the field shall be cleared to 0h.
  // For the Identify command, the Namespace Identifier is only used for the Namespace data structure.
  //
  Command.Nsid        = NVME_CONTROLLER_ID;

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;
  CommandPacket.TransferBuffer = Buffer;
  CommandPacket.TransferLength = sizeof (NVME_ADMIN_CONTROLLER_DATA);
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  //
  // Set bit 0 (Cns bit) to 1 to identify a controller
  //
  Command.Cdw10                = 1;
  Command.Flags                = CDW10_VALID;

  Status = Passthru->PassThru (
                       Passthru,
                       NamespaceId,
                       &CommandPacket,
                       NULL
                       );

  return Status;
}

/**
  Dump NVMe identify information.

  @param  Passthru          The pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL data structure.
  @param  NamespaceId       Current device NameSpaceId.
  @param  ControllerData    output for current device's controller data.

  @retval EFI_SUCCESS      Successfully get the identify controller data.
  @retval EFI_NOT_FOUND    Fail to find NVMe controller.

**/
EFI_STATUS
DumpNvmeIdentifyControllerData (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *Passthru,
  IN UINT32                              NamespaceId,
  OUT NVME_ADMIN_CONTROLLER_DATA         *ControllerData
  )
{
  EFI_STATUS                      Status;
  UINT8                           Sn[21];
  UINT8                           Mn[41];
  //
  // Get current Identify Controller Data
  //
  if((Passthru == NULL) || (ControllerData == NULL))
    return RETURN_INVALID_PARAMETER;

  Status = NvmeIdentifyController2 (Passthru, NamespaceId, ControllerData);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Can't find NVMe Controller! \n"));
    return EFI_NOT_FOUND;
  }

  //
  // Dump NvmExpress Identify Controller Data
  //
  CopyMem (Sn, ControllerData->Sn, sizeof (ControllerData->Sn));
  Sn[20] = 0;
  CopyMem (Mn, ControllerData->Mn, sizeof (ControllerData->Mn));
  Mn[40] = 0;
  DEBUG ((DEBUG_INFO, " == NVME IDENTIFY CONTROLLER DATA ==\n"));
  DEBUG ((DEBUG_INFO, "    PCI VID   : 0x%x\n", ControllerData->Vid));
  DEBUG ((DEBUG_INFO, "    PCI SSVID : 0x%x\n", ControllerData->Ssvid));
  DEBUG ((DEBUG_INFO, "    SN        : %a\n",   Sn));
  DEBUG ((DEBUG_INFO, "    MN        : %a\n",   Mn));
  DEBUG ((DEBUG_INFO, "   FR        : 0x%x\n", *((UINT64*)ControllerData->Fr)));
  DEBUG ((DEBUG_INFO, "    RAB       : 0x%x\n", ControllerData->Rab));
  DEBUG ((DEBUG_INFO, "    IEEE      : 0x%x\n", *(UINT32*)ControllerData->Ieee_oui));
  DEBUG ((DEBUG_INFO, "    AERL      : 0x%x\n", ControllerData->Aerl));
  DEBUG ((DEBUG_INFO, "    SQES      : 0x%x\n", ControllerData->Sqes));
  DEBUG ((DEBUG_INFO, "    CQES      : 0x%x\n", ControllerData->Cqes));
  DEBUG ((DEBUG_INFO, "    NN        : 0x%x\n", ControllerData->Nn));

  DEBUG ((DEBUG_INFO, "    Mtfa      : 0x%x\n", ControllerData->Mtfa));
  DEBUG ((DEBUG_INFO, "    Hmpre     : 0x%x\n", ControllerData->Hmpre));
  DEBUG ((DEBUG_INFO, "    Hmmin     : 0x%x\n", ControllerData->Hmmin));
  DEBUG ((DEBUG_INFO, "    Rpmbs     : 0x%x\n", ControllerData->Rpmbs));
  DEBUG ((DEBUG_INFO, "    Fwug      : 0x%x\n", ControllerData->Fwug));

  return EFI_SUCCESS;
}

