/** @file
  NvmExpressDxe driver is used to manage non-volatile memory subsystem which follows
  NVM Express specification.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include "NvmExpress.h"

/**
  Reset the block device hardware.

  The Reset() function resets the block device hardware.

  As part of the initialization process, the firmware/device will make a quick but
  reasonable attempt to verify that the device is functioning.

  If the ExtendedVerificationflag is TRUE the firmware may take an extended amount
  of time to verify the device is operating on reset. Otherwise the reset operation
  is to occur as quickly as possible.

  The hardware verification process is not defined by this specification and is left
  up to the platform firmware or driver to implement.

  @param[in]  This                 Pointer to the EFI_BLOCK_IO_CRYPTO_PROTOCOL instance.
  @param[in]  ExtendedVerification Indicates that the driver may perform a more exhausive
                                   verification operation of the device during reset.

  @retval EFI_SUCCESS              The block device was reset.
  @retval EFI_DEVICE_ERROR         The block device is not functioning correctly and could
                                   not be reset.
  @retval EFI_INVALID_PARAMETER    This is NULL.

**/
EFI_STATUS
EFIAPI
NvmeBlockIoCryptoReset (
  IN EFI_BLOCK_IO_CRYPTO_PROTOCOL  *This,
  IN BOOLEAN                       ExtendedVerification
  )
{
  EFI_TPL                       OldTpl;
  NVME_CONTROLLER_PRIVATE_DATA  *Private;
  NVME_DEVICE_PRIVATE_DATA      *Device;
  EFI_STATUS                    Status;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // For Nvm Express subsystem, reset block device means reset controller.
  //
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);

  Device = NVME_DEVICE_PRIVATE_DATA_FROM_BLOCK_IO_CRYPTO (This);

  Private = Device->Controller;

  Status = NvmeControllerInit (Private);

  if (EFI_ERROR (Status)) {
    Status = EFI_DEVICE_ERROR;
  }

  gBS->RestoreTPL (OldTpl);

  return Status;
}

/**
  Get the capabilities of the underlying inline cryptographic interface.

  The GetCapabilities() function determines whether pre-OS controllable inline crypto
  is supported by the system for the current disk and, if so, returns the capabilities
  of the crypto engine.

  The caller is responsible for providing the Capabilities structure with a sufficient
  number of entries.

  If the structure is too small, the EFI_BUFFER_TOO_SMALL error code is returned and the
  CapabilityCount field contains the number of entries needed to contain the capabilities.

  @param[in]  This              Pointer to the EFI_BLOCK_IO_CRYPTO_PROTOCOL instance.
  @param[out] Capabilities      Pointer to the EFI_BLOCK_IO_CRYPTO_CAPABILITIES structure.

  @retval EFI_SUCCESS           The ICI is ready for use.
  @retval EFI_BUFFER_TOO_SMALL  The Capabilities structure was too small. The number of
                                entries needed is returned in the CapabilityCount field
                                of the structure.

  @retval EFI_INVALID_PARAMETER This is NULL.
  @retval EFI_INVALID_PARAMETER Capabilities is NULL.

**/
EFI_STATUS
EFIAPI
NvmeBlockIoCryptoGetCapabilities (
  IN  EFI_BLOCK_IO_CRYPTO_PROTOCOL           *This,
  OUT EFI_BLOCK_IO_CRYPTO_CAPABILITIES       *Capabilities
  )
{
  NVME_DEVICE_PRIVATE_DATA      *Device;
  NVME_CONTROLLER_PRIVATE_DATA  *Private;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_VERBOSE, "%a()\n", __FUNCTION__));

  Status = EFI_SUCCESS;

  while (TRUE) {
    if (This == NULL) {
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    if ((NULL == (Device = NVME_DEVICE_PRIVATE_DATA_FROM_BLOCK_IO_CRYPTO (This))) ||
        (NULL == (Private = Device->Controller))) {
      Status = EFI_DEVICE_ERROR;
      break;
    }

    if (NULL == (Private->TseProtocol)) {
      Status = EFI_NOT_FOUND;
      break;
    }

    Status = Private->TseProtocol->GetCapabilities (Capabilities);
    break;
  }

  return Status;
}

/**
  Set the configuration of the underlying inline cryptographic interface.

  The SetConfiguration() function allows the user to set the current configuration of the
  inline cryptographic interface and should be called before attempting any crypto operations.

  This configures the configuration table entries with algorithms, key sizes and keys. Each
  configured entry can later be referred to by index at the time of storage transaction.

  The configuration table index will refer to the combination of KeyOwnerGuid, Algorithm, and
  CryptoKey.

  KeyOwnerGuid identifies the component taking ownership of the entry. It helps components to
  identify their own entries, cooperate with other owner components, and avoid conflicts. This
  Guid identifier is there to help coordination between cooperating components and not a security
  or synchronization feature. The Nil GUID can be used by a component to release use of entry
  owned. It is also used to identify potentially available entries (see GetConfiguration).

  CryptoKey specifies algorithm-specific key material to use within parameters of selected crypto
  capability.

  This function is called infrequently typically once, on device start, before IO starts. It
  can be called at later times in cases the number of keysused on the drive is higher than what
  can be configured at a time or a new key has to be added.

  Components setting or changing an entry or entries for a given index or indices must ensure
  that IO referencing affected indices is temporarily blocked (run-down) at the time of change.

  Indices parameters in each parameter table entry allow to set only a portion of the available
  table entries in the crypto module anywhere from single entry to entire table supported.

  If corresponding table entry or entries being set are already in use by another owner the call
  should be failed and none of the entries should be modified. The interface implementation must
  enforce atomicity of this operation (should either succeed fully or fail completely without
  modifying state).

  Note that components using GetConfiguration command to discover available entries should be
  prepared that by the time of calling SetConfiguration the previously available entry may have
  become occupied. Such components should be prepared to re-try the sequence of operations.

  Alternatively EFI_BLOCK_IO_CRYPTO_INDEX_ANY can be used to have the implementation discover
  and allocate available,if any, indices atomically.

  An optional ResultingTable pointer can be provided by the caller to receive the newly configured
  entries. The array provided by the caller must have at least ConfigurationCount of entries.

  @param[in]  This                Pointer to the EFI_BLOCK_IO_CRYPTO_PROTOCOL instance.
  @param[in]  ConfigurationCount  Number of entries being configured with this call.
  @param[in]  ConfigurationTable  Pointer to a table used to populate the configuration table.
  @param[out] ResultingTable      Optional pointer to a table that receives the newly configured
                                  entries.

  @retval EFI_SUCCESS             The ICI is ready for use.
  @retval EFI_NO_RESPONSE         No response was received from the ICI.
  @retval EFI_DEVICE_ERROR        An error occurred when attempting to access the ICI.
  @retval EFI_INVALID_PARAMETER   This is NULL.
  @retval EFI_INVALID_PARAMETER   ConfigurationTable is NULL.
  @retval EFI_INVALID_PARAMETER   ConfigurationCount is 0.
  @retval EFI_OUT_OF_RESOURCES    Could not find the requested number of available entries in the
                                  configuration table.

**/
EFI_STATUS
EFIAPI
NvmeBlockIoCryptoSetConfiguration (
  IN     EFI_BLOCK_IO_CRYPTO_PROTOCOL                     *This,
  IN     UINT64                                           ConfigurationCount,
  IN     EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *ConfigurationTable,
  OUT    EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ResultingTable OPTIONAL
  )
{
  NVME_DEVICE_PRIVATE_DATA      *Device;
  NVME_CONTROLLER_PRIVATE_DATA  *Private;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_VERBOSE, "%a()\n", __FUNCTION__));

  Status = EFI_SUCCESS;

  while (TRUE) {
    if ((This == NULL) ||
        (ConfigurationCount == 0) ||
        (ConfigurationTable == NULL)) {
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    if ((NULL == (Device = NVME_DEVICE_PRIVATE_DATA_FROM_BLOCK_IO_CRYPTO (This))) ||
        (NULL == (Private = Device->Controller))) {
      Status = EFI_DEVICE_ERROR;
      break;
    }

    if (NULL == (Private->TseProtocol)) {
      Status = EFI_NOT_FOUND;
      break;
    }

    Status = Private->TseProtocol->SubmitUefiTableConfigurationEntries (
                                     ConfigurationCount,
                                     ConfigurationTable,
                                     ResultingTable
                                     );
    break;
  }

  return Status;
}

/**
  Get the configuration of the underlying inline cryptographic interface.

  The GetConfiguration() function allows the user to get the configuration of the inline
  cryptographic interface.

  Retrieves, entirely or partially, the currently configured key table. Note that the keys
  themselves are not retrieved, but rather just indices, owner GUIDs and capabilities.

  If fewer entries than specified by ConfigurationCount are returned, the Index field of the
  unused entries is set to EFI_BLOCK_IO_CRYPTO_INDEX_ANY.

  @param[in]  This                Pointer to the EFI_BLOCK_IO_CRYPTO_PROTOCOL instance.
  @param[in]  StartIndex          Configuration table index at which to start the configuration
                                  query.
  @param[in]  ConfigurationCount  Number of entries to return in the response table.
  @param[in]  KeyOwnerGuid        Optional parameter to filter response down to entries with a
                                  given owner. A pointer to the Nil value can be used to return
                                  available entries. Set to NULL when no owner filtering is required.
  @param[out] ConfigurationTable  Table of configured configuration table entries (with no CryptoKey
                                  returned): configuration table index, KeyOwnerGuid, Capability.
                                  Should have sufficient space to store up to ConfigurationCount
                                  entries.

  @retval EFI_SUCCESS             The ICI is ready for use.
  @retval EFI_NO_RESPONSE         No response was received from the ICI.
  @retval EFI_DEVICE_ERROR        An error occurred when attempting to access the ICI.
  @retval EFI_INVALID_PARAMETER   This is NULL.
  @retval EFI_INVALID_PARAMETER   Configuration table is NULL.
  @retval EFI_INVALID_PARAMETER   StartIndex is out of bounds.

**/
EFI_STATUS
EFIAPI
NvmeBlockIoCryptoGetConfiguration (
  IN     EFI_BLOCK_IO_CRYPTO_PROTOCOL                     *This,
  IN     UINT64                                           StartIndex,
  IN     UINT64                                           ConfigurationCount,
  IN     EFI_GUID                                         *KeyOwnerGuid OPTIONAL,
  OUT    EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ConfigurationTable
  )
{
  NVME_DEVICE_PRIVATE_DATA         *Device;
  NVME_CONTROLLER_PRIVATE_DATA     *Private;
  EFI_STATUS                       Status;

  DEBUG ((DEBUG_VERBOSE, "%a()\n", __FUNCTION__));

  Status = EFI_SUCCESS;

  while (TRUE) {
    if ((This == NULL) ||
        (ConfigurationCount == 0) ||
        (ConfigurationTable == NULL)) {
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    if ((NULL == (Device = NVME_DEVICE_PRIVATE_DATA_FROM_BLOCK_IO_CRYPTO (This))) ||
        (NULL == (Private = Device->Controller))) {
      Status = EFI_DEVICE_ERROR;
      break;
    }

    if (NULL == (Private->TseProtocol)) {
      Status = EFI_NOT_FOUND;
      break;
    }

    Status = Private->TseProtocol->GetUefiTableConfigurationEntries (
                                     StartIndex,
                                     ConfigurationCount,
                                     KeyOwnerGuid,
                                     ConfigurationTable
                                     );
    break;
  }
  return Status;
}

/**
  Read some sectors from the device.

  @param  Device                 The pointer to the NVME_DEVICE_PRIVATE_DATA data structure.
  @param  Buffer                 The buffer used to store the data read from the device.
  @param  Lba                    The start block number.
  @param  Blocks                 Total block number to be read.

  @retval EFI_SUCCESS            Datum are read from the device.
  @retval Others                 Fail to read all the datum.

**/
EFI_STATUS
NvmeBlockIoCryptoReadSectors (
  IN NVME_DEVICE_PRIVATE_DATA  *Device,
  IN UINT64                    Buffer,
  IN UINT64                    Lba,
  IN UINT32                    Blocks
  )
{
  NVME_CONTROLLER_PRIVATE_DATA              *Private;
  UINT32                                    Bytes;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;
  EFI_STATUS                                Status;
  UINT32                                    BlockSize;
  UINT32                                    CryptoIoAlign;

  Private       = Device->Controller;
  BlockSize     = Device->Media.BlockSize;
  Bytes         = Blocks * BlockSize;

  CryptoIoAlign = Device->CryptoMedia.IoAlign;
  //
  // Buffer alignment requirement check
  //
  if ((CryptoIoAlign > 0) && (((UINTN)Buffer & (CryptoIoAlign - 1)) != 0)) {
    DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] Buffer does not satisfy crypto alignment requirements!\nCryptoIoAlign: 0x%x, TransferBuffer: 0x%016llx\n", CryptoIoAlign, Buffer));
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;

  CommandPacket.NvmeCmd->Cdw0.Opcode = NVME_IO_READ_OPC;
  CommandPacket.NvmeCmd->Nsid        = Device->NamespaceId;
  CommandPacket.TransferBuffer       = (VOID *)(UINTN)Buffer;

  CommandPacket.TransferLength = Bytes;
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_IO_QUEUE;

  CommandPacket.NvmeCmd->Cdw10 = (UINT32)Lba;
  CommandPacket.NvmeCmd->Cdw11 = (UINT32)RShiftU64 (Lba, 32);
  CommandPacket.NvmeCmd->Cdw12 = (Blocks - 1) & 0xFFFF;

  CommandPacket.NvmeCmd->Flags = CDW10_VALID | CDW11_VALID | CDW12_VALID;

  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] READ SECTORS...\n"));
  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] TransferBuffer: 0x%x\n", CommandPacket.TransferBuffer));
  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] TransferLength: 0x%x\n", CommandPacket.TransferLength));
  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] Cdw10: 0x%x\n", CommandPacket.NvmeCmd->Cdw10));
  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] Cdw11: 0x%x\n", CommandPacket.NvmeCmd->Cdw11));
  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] Cdw12: 0x%x\n", CommandPacket.NvmeCmd->Cdw12));

  Status = Private->CryptoPassthru.PassThru (
                               &Private->Passthru,
                               Device->NamespaceId,
                               &CommandPacket,
                               NULL
                               );

  return Status;
}

/**
  Read some blocks from the device.

  @param  Device                 The pointer to the NVME_DEVICE_PRIVATE_DATA data structure.
  @param  Buffer                 The buffer used to store the data read from the device.
  @param  Lba                    The start block number.
  @param  Blocks                 Total block number to be read.

  @retval EFI_SUCCESS            Datum are read from the device.
  @retval Others                 Fail to read all the datum.

**/
EFI_STATUS
NvmeBlockIoCryptoRead (
  IN     NVME_DEVICE_PRIVATE_DATA  *Device,
  OUT VOID                         *Buffer,
  IN     UINT64                    Lba,
  IN     UINTN                     Blocks
  )
{
  EFI_STATUS                    Status;
  UINT32                        BlockSize;
  NVME_CONTROLLER_PRIVATE_DATA  *Private;
  UINT32                        MaxTransferBlocks;
  UINTN                         OrginalBlocks;
  BOOLEAN                       IsEmpty;
  EFI_TPL                       OldTpl;

  //
  // Wait for the device's asynchronous I/O queue to become empty.
  //
  while (TRUE) {
    OldTpl  = gBS->RaiseTPL (TPL_NOTIFY);
    IsEmpty = IsListEmpty (&Device->AsyncQueue);
    gBS->RestoreTPL (OldTpl);

    if (IsEmpty) {
      break;
    }

    gBS->Stall (100);
  }

  Status        = EFI_SUCCESS;
  Private       = Device->Controller;
  BlockSize     = Device->Media.BlockSize;
  OrginalBlocks = Blocks;

  if (Private->ControllerData->Mdts != 0) {
    MaxTransferBlocks = (1 << (Private->ControllerData->Mdts)) * (1 << (Private->Cap.Mpsmin + 12)) / BlockSize;
  } else {
    MaxTransferBlocks = 1024;
  }

  while (Blocks > 0) {
    if (Blocks > MaxTransferBlocks) {
      Status = NvmeBlockIoCryptoReadSectors (Device, (UINT64)(UINTN)Buffer, Lba, MaxTransferBlocks);

      Blocks -= MaxTransferBlocks;
      Buffer  = (VOID *)(UINTN)((UINT64)(UINTN)Buffer + MaxTransferBlocks * BlockSize);
      Lba    += MaxTransferBlocks;

      //
      // The CryptoBlockNumber used in the AES XTS IV needs to be incrementing for each consecutive
      // crypto block in the IO operation.
      //
      // When Blocks > MaxTransferBlocks, the IO request gets sliced into multiple NVMe requests.
      // If previous operation completed without error, increment TseProtocol CryptoIvInput context
      // to match the corresponding block of the subsequent slice.
      //
      if (!EFI_ERROR (Status)) {
        Status = Private->TseProtocol->CryptoBlockIncrement (MaxTransferBlocks);
      }
    } else {
      Status = NvmeBlockIoCryptoReadSectors (Device, (UINT64)(UINTN)Buffer, Lba, (UINT32)Blocks);
      Blocks = 0;
    }

    if (EFI_ERROR (Status)) {
      break;
    }
  }

  DEBUG ((
    DEBUG_BLKIO,
    "%a: Lba = 0x%08Lx, Original = 0x%08Lx, "
    "Remaining = 0x%08Lx, BlockSize = 0x%x, Status = %r\n",
    __FUNCTION__,
    Lba,
    (UINT64)OrginalBlocks,
    (UINT64)Blocks,
    BlockSize,
    Status
    ));

  return Status;
}

/**
  Reads the requested number of blocks from the device and optionally decrypts
  them inline.

  TheReadExtended() function allows the caller to perform a storage device read
  operation. The function reads the requested number of blocks from the device
  and then if Index is specified decrypts them inline. All the blocks are read
  and decrypted (if decryption requested),  or an error is returned.

  If there is no media in the device, the function returns EFI_NO_MEDIA. If the
  MediaId is not the ID for the current media in the device, the function returns
  EFI_MEDIA_CHANGED.

  If EFI_DEVICE_ERROR, EFI_NO_MEDIA, or EFI_MEDIA_CHANGED is returned and nonblocking
  I/O is being used, the Event associated with this request will not be signaled.

  In addition to standard storage transaction parameters (LBA, IO size, and buffer),
  this command will also specify a configuration table Index and CryptoIvInput
  when data has  to be decrypted inline by the controller after being read from
  the storage device. If an Index parameter is not specified, no decryption is
  performed.

  @param[in]      This          Pointer to the EFI_BLOCK_IO_CRYPTO_PROTOCOL instance.
  @param[in]      MediaId       The media ID that the read request is for.
  @param[in]      LBA           The starting logical block address to read from on
                                the device.
  @param[in, out] Token         A pointer to the token associated with the transaction.
  @param[in]      BufferSize    The size of the Buffer in bytes. This must be a multiple
                                of the intrinsic block size of the device.
  @param[out]     Buffer        A pointer to the destination buffer for the data. The
                                caller is responsible for either having implicit or
                                explicit ownership of the buffer.
  @param[in]      Index         A pointer to the configuration table index. This is
                                optional.
  @param[in]      CryptoIvInput A pointer to a buffer that contains additional
                                cryptographic parameters as required by the capability
                                referenced by the configuration table index, such as
                                cryptographic initialization vector.

  @retval EFI_SUCCESS           The read request was queued if Token-> Event is not NULL.
                                The data was read correctly from the device if the
                                Token->Event is NULL.
  @retval EFI_DEVICE_ERROR      The device reported an error while attempting to perform
                                the read operation and/or decryption operation.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHANGED     The MediaId is not for the current media.
  @retval EFI_BAD_BUFFER_SIZE   The BufferSize parameter is not a multiple of the intrinsic
                                block size of the device.
  @retval EFI_INVALID_PARAMETER This is NULL, or the read request contains LBAs that are
                                not valid, or the buffer is not on proper alignment.
  @retval EFI_INVALID_PARAMETER CryptoIvInput is incorrect.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of
                                resources.

**/
EFI_STATUS
EFIAPI
NvmeBlockIoCryptoReadBlocksEx (
  IN     EFI_BLOCK_IO_CRYPTO_PROTOCOL  *This,
  IN     UINT32                        MediaId,
  IN     EFI_LBA                       Lba,
  IN OUT EFI_BLOCK_IO_CRYPTO_TOKEN     *Token,
  IN     UINT64                        BufferSize,
  OUT VOID                             *Buffer,
  IN     UINT64                        *Index OPTIONAL,
  IN     VOID                          *CryptoIvInput OPTIONAL
  )
{
  NVME_CONTROLLER_PRIVATE_DATA *Private;
  NVME_DEVICE_PRIVATE_DATA  *Device;
  EFI_STATUS                Status;
  EFI_BLOCK_IO_MEDIA        *Media;
  UINTN                     BlockSize;
  UINTN                     NumberOfBlocks;
  UINTN                     IoAlign;
  UINT64                    CryptoBlockNumber;
  EFI_TPL                   OldTpl;

  //
  // Check parameters.
  //
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Media = This->Media;

  if (MediaId != Media->MediaId) {
    return EFI_MEDIA_CHANGED;
  }

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize == 0) {
    return EFI_SUCCESS;
  }

  BlockSize = Media->BlockSize;
  if ((BufferSize % BlockSize) != 0) {
    return EFI_BAD_BUFFER_SIZE;
  }

  NumberOfBlocks = BufferSize / BlockSize;
  if ((Lba + NumberOfBlocks - 1) > Media->LastBlock) {
    return EFI_INVALID_PARAMETER;
  }

  IoAlign = Media->IoAlign;
  if ((IoAlign > 0) && (((UINTN)Buffer & (IoAlign - 1)) != 0)) {
    return EFI_INVALID_PARAMETER;
  }

  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);

  Device = NVME_DEVICE_PRIVATE_DATA_FROM_BLOCK_IO_CRYPTO (This);

  if (Device == NULL) {
    DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] NVMe Private Data Device is NULL\n"));
    gBS->RestoreTPL (OldTpl);
    return EFI_NOT_READY;
  }

  Private = Device->Controller;

  DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] Lba: 0x%016llx, BufferPtr: 0x%016llx, IndexPtr: 0x%016llx, CryptoIvInputPtr: 0x%016llx\n", Lba, Buffer, Index, CryptoIvInput));

  //
  // If Index parameter is NULL, perform non cryptographic IO operation
  // Wrap around Block IO2 protocol ReadBlockEx API
  //
  if (Index == NULL) {
    if (Device->BlockIo2.ReadBlocksEx == NULL) {
      DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] Device's BlockIo2 protocol not available!\n"));
      gBS->RestoreTPL (OldTpl);
      return EFI_PROTOCOL_ERROR;
    }

    Status = Device->BlockIo2.ReadBlocksEx (
                                &Device->BlockIo2,
                                Device->Media.MediaId,
                                Lba,
                                (EFI_BLOCK_IO2_TOKEN *) Token,
                                BufferSize,
                                Buffer
                                );

    gBS->RestoreTPL (OldTpl);
    return Status;
  }

  //
  // Sanity check CryptoIvInput parameter.
  // If NULL, return return unsupported error.
  //
  if (CryptoIvInput == NULL) {
    DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] CryptoIvInput is NULL!\n"));
    gBS->RestoreTPL (OldTpl);
    return EFI_UNSUPPORTED;
  }

  //
  // CryptoIvInput overflow check
  //
  CryptoBlockNumber = (UINT64) ((EFI_BLOCK_IO_CRYPTO_IV_INPUT_AES_XTS *) CryptoIvInput)->CryptoBlockNumber;

  if ((CryptoBlockNumber > CRYPTO_IV_INPUT_MAX_VALUE) ||
      ((UINT64) NumberOfBlocks > (CRYPTO_IV_INPUT_MAX_VALUE - CryptoBlockNumber) + 1)) {
    DEBUG ((DEBUG_ERROR,
      "[BlockIoCrypto] Invalid parameters:\n\tCryptoBlockNumber: 0x%016llx\n\tNumberOfBlocks: 0x%016llx\n",
      CryptoBlockNumber,
      (UINT64) NumberOfBlocks));

    gBS->RestoreTPL (OldTpl);
    return EFI_INVALID_PARAMETER;
  }

  //
  // If CryptoBlockByteSize is different to the device's blocksize, return unsupported error.
  //
  if (BlockSize != (((EFI_BLOCK_IO_CRYPTO_IV_INPUT_AES_XTS *) CryptoIvInput)->CryptoBlockByteSize)) {
    DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] Media->BlockSize & CryptoBlockByteSize mismatch! 0x%x != 0x%x\n", BlockSize, (((EFI_BLOCK_IO_CRYPTO_IV_INPUT_AES_XTS *) CryptoIvInput)->CryptoBlockByteSize)));
    gBS->RestoreTPL (OldTpl);
    return EFI_UNSUPPORTED;
  }

  //
  // Set TSE protocol IO context
  //
  Private->TseProtocol->SetBlockIoMedia (Media);
  Private->TseProtocol->SetCryptoConfig (Index, CryptoIvInput);

  Status = NvmeBlockIoCryptoRead (Device, Buffer, Lba, NumberOfBlocks);

  //
  // Clear TSE protocol IO context
  //
  Private->TseProtocol->SetBlockIoMedia (NULL);
  Private->TseProtocol->SetCryptoConfig (NULL, NULL);

  gBS->RestoreTPL (OldTpl);
  return Status;
}

/**
  Write some sectors to the device.

  @param  Device                 The pointer to the NVME_DEVICE_PRIVATE_DATA data structure.
  @param  Buffer                 The buffer to be written into the device.
  @param  Lba                    The start block number.
  @param  Blocks                 Total block number to be written.

  @retval EFI_SUCCESS            Datum are written into the buffer.
  @retval Others                 Fail to write all the datum.

**/
EFI_STATUS
NvmeBlockIoCryptoWriteSectors (
  IN NVME_DEVICE_PRIVATE_DATA  *Device,
  IN UINT64                    Buffer,
  IN UINT64                    Lba,
  IN UINT32                    Blocks
  )
{
  NVME_CONTROLLER_PRIVATE_DATA              *Private;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;
  EFI_STATUS                                Status;
  UINT32                                    Bytes;
  UINT32                                    BlockSize;
  UINT32                                    CryptoIoAlign;

  Private   = Device->Controller;
  BlockSize = Device->Media.BlockSize;
  Bytes     = Blocks * BlockSize;

  CryptoIoAlign = Device->CryptoMedia.IoAlign;
  //
  // Buffer alignment requirement check
  //
  if ((CryptoIoAlign > 0) && (((UINTN)Buffer & (CryptoIoAlign - 1)) != 0)) {
    DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] Buffer does not satisfy crypto alignment requirements!\nCryptoIoAlign: 0x%x, TransferBuffer: 0x%016llx\n", CryptoIoAlign, Buffer));
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;

  CommandPacket.NvmeCmd->Cdw0.Opcode = NVME_IO_WRITE_OPC;
  CommandPacket.NvmeCmd->Nsid        = Device->NamespaceId;
  CommandPacket.TransferBuffer       = (VOID *)(UINTN)Buffer;

  CommandPacket.TransferLength = Bytes;
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_IO_QUEUE;

  CommandPacket.NvmeCmd->Cdw10 = (UINT32)Lba;
  CommandPacket.NvmeCmd->Cdw11 = (UINT32)RShiftU64 (Lba, 32);
  //
  // Set Force Unit Access bit (bit 30) to use write-through behaviour
  //
  CommandPacket.NvmeCmd->Cdw12 = ((Blocks - 1) & 0xFFFF) | BIT30;

  CommandPacket.MetadataBuffer = NULL;
  CommandPacket.MetadataLength = 0;

  CommandPacket.NvmeCmd->Flags = CDW10_VALID | CDW11_VALID | CDW12_VALID;

  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] Write SECTORS...\n"));
  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] TransferBuffer: 0x%x\n", CommandPacket.TransferBuffer));
  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] TransferLength: 0x%x\n", CommandPacket.TransferLength));
  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] Cdw10: 0x%x\n", CommandPacket.NvmeCmd->Cdw10));
  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] Cdw11: 0x%x\n", CommandPacket.NvmeCmd->Cdw11));
  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] Cdw12: 0x%x\n", CommandPacket.NvmeCmd->Cdw12));

  Status = Private->CryptoPassthru.PassThru (
                                     &Private->Passthru,
                                     Device->NamespaceId,
                                     &CommandPacket,
                                     NULL
                                     );

  return Status;
}


/**
  Write some blocks to the device.

  @param  Device                 The pointer to the NVME_DEVICE_PRIVATE_DATA data structure.
  @param  Buffer                 The buffer to be written into the device.
  @param  Lba                    The start block number.
  @param  Blocks                 Total block number to be written.

  @retval EFI_SUCCESS            Datum are written into the buffer.
  @retval Others                 Fail to write all the datum.

**/
EFI_STATUS
NvmeBlockIoCryptoWrite (
  IN NVME_DEVICE_PRIVATE_DATA  *Device,
  IN VOID                      *Buffer,
  IN UINT64                    Lba,
  IN UINTN                     Blocks
  )
{
  EFI_STATUS                    Status;
  UINT32                        BlockSize;
  NVME_CONTROLLER_PRIVATE_DATA  *Private;
  UINT32                        MaxTransferBlocks;
  UINTN                         OrginalBlocks;
  BOOLEAN                       IsEmpty;
  EFI_TPL                       OldTpl;

  //
  // Wait for the device's asynchronous I/O queue to become empty.
  //
  while (TRUE) {
    OldTpl  = gBS->RaiseTPL (TPL_NOTIFY);
    IsEmpty = IsListEmpty (&Device->AsyncQueue);
    gBS->RestoreTPL (OldTpl);

    if (IsEmpty) {
      break;
    }

    gBS->Stall (100);
  }

  Status        = EFI_SUCCESS;
  Private       = Device->Controller;
  BlockSize     = Device->Media.BlockSize;
  OrginalBlocks = Blocks;

  if (Private->ControllerData->Mdts != 0) {
    MaxTransferBlocks = (1 << (Private->ControllerData->Mdts)) * (1 << (Private->Cap.Mpsmin + 12)) / BlockSize;
  } else {
    MaxTransferBlocks = 1024;
  }

  while (Blocks > 0) {
    if (Blocks > MaxTransferBlocks) {
      Status = NvmeBlockIoCryptoWriteSectors (Device, (UINT64)(UINTN)Buffer, Lba, MaxTransferBlocks);

      Blocks -= MaxTransferBlocks;
      Buffer  = (VOID *)(UINTN)((UINT64)(UINTN)Buffer + MaxTransferBlocks * BlockSize);
      Lba    += MaxTransferBlocks;
      //
      // The CryptoBlockNumber used in the AES XTS IV needs to be incrementing for each consecutive
      // crypto block in the IO operation.
      //
      // When Blocks > MaxTransferBlocks, the IO request gets sliced into multiple NVMe requests.
      // If previous operation completed without error, increment TseProtocol CryptoIvInput context
      // to match the corresponding block of the subsequent slice.
      //
      if (!EFI_ERROR (Status)) {
        Status = Private->TseProtocol->CryptoBlockIncrement (MaxTransferBlocks);
      }
    } else {
      Status = NvmeBlockIoCryptoWriteSectors (Device, (UINT64)(UINTN)Buffer, Lba, (UINT32)Blocks);
      Blocks = 0;
    }

    if (EFI_ERROR (Status)) {
      break;
    }
  }

  DEBUG ((
    DEBUG_BLKIO,
    "%a: Lba = 0x%08Lx, Original = 0x%08Lx, "
    "Remaining = 0x%08Lx, BlockSize = 0x%x, Status = %r\n",
    __FUNCTION__,
    Lba,
    (UINT64)OrginalBlocks,
    (UINT64)Blocks,
    BlockSize,
    Status
    ));

  return Status;
}


/**
  Optionally encrypts a specified number of blocks inline and then writes to the
  device.

  The WriteExtended() function allows the caller to perform a storage device write
  operation. The function encrypts the requested number of blocks inline if Index
  is specified  and then writes them to the device. All the blocks are encrypted
  (if encryption requested) and  written, or an error is returned.

  If there is no media in the device, the function returns EFI_NO_MEDIA. If the
  MediaId is not the ID for the current media in the device, the function returns
  EFI_MEDIA_CHANGED.

  If EFI_DEVICE_ERROR, EFI_NO_MEDIA, or EFI_MEDIA_CHANGED is returned and nonblocking
  I/O is being used, the Event associated with this request will not be signaled.

  In addition to standard storage transaction parameters (LBA, IO size, and buffer),
  this command will also specify a configuration table Index and a CryptoIvInput
  when data has to be decrypted inline by the controller before being written to
  the storage device. If no Index parameter is specified, no encryption is performed.

  @param[in]      This          Pointer to the EFI_BLOCK_IO_CRYPTO_PROTOCOL instance.
  @param[in]      MediaId       The media ID that the read request is for.
  @param[in]      LBA           The starting logical block address to read from on
                                the device.
  @param[in, out] Token         A pointer to the token associated with the transaction.
  @param[in]      BufferSize    The size of the Buffer in bytes. This must be a multiple
                                of the intrinsic block size of the device.
  @param[in]      Buffer        A pointer to the source buffer for the data.
  @param[in]      Index         A pointer to the configuration table index. This is
                                optional.
  @param[in]      CryptoIvInput A pointer to a buffer that contains additional
                                cryptographic parameters as required by the capability
                                referenced by the configuration table index, such as
                                cryptographic initialization vector.

  @retval EFI_SUCCESS           The request to encrypt (optionally) and write was queued
                                if Event is not NULL. The data was encrypted (optionally)
                                and written correctly to the device if the Event is NULL.
  @retval EFI_WRITE_PROTECTED   The device cannot be written to.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHANGED     The MediaId is not for the current media.
  @retval EFI_DEVICE_ERROR      The device reported an error while attempting to encrypt
                                blocks or to perform the write operation.
  @retval EFI_BAD_BUFFER_SIZE   The BufferSize parameter is not a multiple of the intrinsic
                                block size of the device.
  @retval EFI_INVALID_PARAMETER This is NULL, or the write request contains LBAs that are
                                not valid, or the buffer is not on proper alignment.
  @retval EFI_INVALID_PARAMETER CryptoIvInput is incorrect.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of
                                resources.

**/
EFI_STATUS
EFIAPI
NvmeBlockIoCryptoWriteBlocksEx (
  IN     EFI_BLOCK_IO_CRYPTO_PROTOCOL  *This,
  IN     UINT32                        MediaId,
  IN     EFI_LBA                       Lba,
  IN OUT EFI_BLOCK_IO_CRYPTO_TOKEN     *Token,
  IN     UINT64                        BufferSize,
  IN     VOID                          *Buffer,
  IN     UINT64                        *Index OPTIONAL,
  IN     VOID                          *CryptoIvInput OPTIONAL
  )
{
  NVME_DEVICE_PRIVATE_DATA  *Device;
  NVME_CONTROLLER_PRIVATE_DATA              *Private;
  EFI_STATUS                Status;
  EFI_BLOCK_IO_MEDIA        *Media;
  UINTN                     BlockSize;
  UINTN                     NumberOfBlocks;
  UINTN                     IoAlign;
  EFI_TPL                   OldTpl;
  UINT64                    CryptoBlockNumber;

  //
  // Check parameters.
  //
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Media = This->Media;

  if (MediaId != Media->MediaId) {
    return EFI_MEDIA_CHANGED;
  }

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize == 0) {
    return EFI_SUCCESS;
  }

  BlockSize = Media->BlockSize;
  if ((BufferSize % BlockSize) != 0) {
    return EFI_BAD_BUFFER_SIZE;
  }

  NumberOfBlocks = BufferSize / BlockSize;
  if ((Lba + NumberOfBlocks - 1) > Media->LastBlock) {
    return EFI_INVALID_PARAMETER;
  }

  IoAlign = Media->IoAlign;
  if ((IoAlign > 0) && (((UINTN)Buffer & (IoAlign - 1)) != 0)) {
    return EFI_INVALID_PARAMETER;
  }

  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);

  Device = NVME_DEVICE_PRIVATE_DATA_FROM_BLOCK_IO_CRYPTO (This);

  if (Device == NULL) {
    DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] NVMe Private Data Device is NULL\n"));
    return EFI_NOT_READY;
  }

  Private = Device->Controller;

  DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] Lba: 0x%016llx, BufferPtr: 0x%016llx, IndexPtr: 0x%016llx, CryptoIvInputPtr: 0x%016llx\n", Lba, Buffer, Index, CryptoIvInput));

  //
  // If Index parameter is NULL, perform non cryptographic IO operation
  // Wrap around Block IO2 protocol WriteBlockEx API
  //
  if (Index == NULL) {
    if (Device->BlockIo2.WriteBlocksEx == NULL) {
      DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] Device's BlockIo2 protocol not available!\n"));
      gBS->RestoreTPL (OldTpl);
      return EFI_PROTOCOL_ERROR;
    }

    Status = Device->BlockIo2.WriteBlocksEx (
                                &Device->BlockIo2,
                                Device->Media.MediaId,
                                Lba,
                                (EFI_BLOCK_IO2_TOKEN *) Token,
                                BufferSize,
                                Buffer
                                );

    gBS->RestoreTPL (OldTpl);
    return Status;
  }

  //
  // Sanity check CryptoIvInput parameter.
  // If NULL, return return unsupported error.
  //
  if (CryptoIvInput == NULL) {
    DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] CryptoIvInput is NULL!\n"));
    gBS->RestoreTPL (OldTpl);
    return EFI_UNSUPPORTED;
  }

  //
  // CryptoIvInput overflow check
  //
  CryptoBlockNumber = (UINT64) ((EFI_BLOCK_IO_CRYPTO_IV_INPUT_AES_XTS *) CryptoIvInput)->CryptoBlockNumber;

  if ((CryptoBlockNumber > CRYPTO_IV_INPUT_MAX_VALUE) ||
      ((UINT64) NumberOfBlocks > (CRYPTO_IV_INPUT_MAX_VALUE - CryptoBlockNumber) + 1)) {
    DEBUG ((DEBUG_ERROR,
      "[BlockIoCrypto] Invalid parameters:\n\tCryptoBlockNumber: 0x%016llx\n\tNumberOfBlocks: 0x%016llx\n",
      CryptoBlockNumber,
      (UINT64) NumberOfBlocks));

    gBS->RestoreTPL (OldTpl);
    return EFI_INVALID_PARAMETER;
  }

  //
  // If CryptoBlockByteSize is different to the device's blocksize, return unsupported error.
  //
  if (BlockSize != (((EFI_BLOCK_IO_CRYPTO_IV_INPUT_AES_XTS *) CryptoIvInput)->CryptoBlockByteSize)) {
    DEBUG ((DEBUG_ERROR, "[BlockIoCrypto] Media->BlockSize & CryptoBlockByteSize mismatch! 0x%x != 0x%x\n", BlockSize, (((EFI_BLOCK_IO_CRYPTO_IV_INPUT_AES_XTS *) CryptoIvInput)->CryptoBlockByteSize)));
    gBS->RestoreTPL (OldTpl);
    return EFI_UNSUPPORTED;
  }

  //
  // Set TSE protocol IO context
  //
  Private->TseProtocol->SetBlockIoMedia (Media);
  Private->TseProtocol->SetCryptoConfig (Index, CryptoIvInput);

  Status = NvmeBlockIoCryptoWrite (Device, Buffer, Lba, NumberOfBlocks);

  //
  // Clear TSE protocol IO context
  //
  Private->TseProtocol->SetBlockIoMedia (NULL);
  Private->TseProtocol->SetCryptoConfig (NULL, NULL);

  gBS->RestoreTPL (OldTpl);
  return Status;
}


/**
  Flushes all modified data to the device.

  @param  Device                 The pointer to the NVME_DEVICE_PRIVATE_DATA data structure.

  @retval EFI_SUCCESS            Datum are written into the buffer.
  @retval Others                 Fail to write all the datum.

**/
EFI_STATUS
NvmeCryptoFlush (
  IN NVME_DEVICE_PRIVATE_DATA  *Device
  )
{
  NVME_CONTROLLER_PRIVATE_DATA              *Private;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;
  EFI_STATUS                                Status;

  Private = Device->Controller;

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;

  CommandPacket.NvmeCmd->Cdw0.Opcode = NVME_IO_FLUSH_OPC;
  CommandPacket.NvmeCmd->Nsid        = Device->NamespaceId;
  CommandPacket.CommandTimeout       = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType            = NVME_IO_QUEUE;

  DEBUG ((DEBUG_VERBOSE, "[NvmExpressBlockIoCrypto] Nvme Crypto Flush...\n"));

  Status = Private->CryptoPassthru.PassThru (
                                     &Private->Passthru,
                                     Device->NamespaceId,
                                     &CommandPacket,
                                     NULL
                                     );

  return Status;
}


/**
  Flushes all modified data to a physical block device.

  The FlushBlocks() function flushes all modified data to the physical block device.
  Any modified data that has to be encrypted must have been already encrypted as a
  part of WriteExtended() operation - inline crypto operation cannot be a part of
  flush operation.

  All data written to the device prior to the flush must be physically written before
  returning EFI_SUCCESS from this function. This would include any cached data the
  driver may have cached, and cached data the device may have cached. A flush may
  cause a read request following the flush to force a device access.

  If EFI_DEVICE_ERROR, EFI_NO_MEDIA, EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED is
  returned and non-blocking I/O is being used, the Event associated with this request
  will not be signaled.

  @param[in]      This          Pointer to the EFI_BLOCK_IO_CRYPTO_PROTOCOL instance.
  @param[in, out] Token         A pointer to the token associated with the transaction.

  @retval EFI_SUCCESS           The flush request was queued if Event is not NULL. All
                                outstanding data was written correctly to the device if
                                the Event is NULL.
  @retval EFI_DEVICE_ERROR      The device reported an error while attempting to write data.
  @retval EFI_WRITE_PROTECTED   The device cannot be written to.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHANGED     The MediaId is not for the current media.
  @retval EFI_INVALID_PARAMETER This is NULL.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of
                                resources.

**/
EFI_STATUS
EFIAPI
NvmeBlockIoCryptoFlushBlocks (
  IN     EFI_BLOCK_IO_CRYPTO_PROTOCOL  *This,
  IN OUT EFI_BLOCK_IO_CRYPTO_TOKEN     *Token
  )
{
  NVME_DEVICE_PRIVATE_DATA  *Device;
  EFI_STATUS                Status;
  EFI_TPL                   OldTpl;

  //
  // Check parameters.
  //
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);

  Device = NVME_DEVICE_PRIVATE_DATA_FROM_BLOCK_IO_CRYPTO (This);

  Status = NvmeCryptoFlush (Device);

  gBS->RestoreTPL (OldTpl);

  return Status;
}
