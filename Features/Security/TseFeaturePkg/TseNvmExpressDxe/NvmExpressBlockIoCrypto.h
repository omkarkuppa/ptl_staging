/** @file
  Header file for EFI_BLOCK_IO_CRYPTO_PROTOCOL interface.

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

@par Specification
**/

#ifndef _EFI_NVME_BLOCKIO_CRYPTO_H_
#define _EFI_NVME_BLOCKIO_CRYPTO_H_

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
  IN  EFI_BLOCK_IO_CRYPTO_PROTOCOL  *This,
  IN  BOOLEAN                ExtendedVerification
  );

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
  @retval EFI_NO_RESPONSE       No response was received from the ICI.
  @retval EFI_DEVICE_ERROR      An error occurred when attempting to access the ICI.
  @retval EFI_INVALID_PARAMETER This is NULL.
  @retval EFI_INVALID_PARAMETER Capabilities is NULL.

**/
EFI_STATUS
EFIAPI
NvmeBlockIoCryptoGetCapabilities (
  IN  EFI_BLOCK_IO_CRYPTO_PROTOCOL           *This,
  OUT EFI_BLOCK_IO_CRYPTO_CAPABILITIES       *Capabilities
  );

/**
  Set the configuration of the underlying inline cryptographic interface.

  The SetConfiguration() function allows the user to set the current configuration of the
  inline cryptographic interface and should be called before attempting any crypto operations.

  This configures the configuration table entries with algorithms, key sizes and keys. Each
  configured entry can later be referred to by index at the time of storage transaction.

  The configuration table index will refer to the combination ofKeyOwnerGuid, Algorithm, and
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
  OUT EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ResultingTable OPTIONAL
  );

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
  OUT EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ConfigurationTable
  );

/**
  Read BufferSize bytes from Lba into Buffer.

  This function reads the requested number of blocks from the device. All the
  blocks are read, or an error is returned.
  If EFI_DEVICE_ERROR, EFI_NO_MEDIA,_or EFI_MEDIA_CHANGED is returned and
  non-blocking I/O is being used, the Event associated with this request will
  not be signaled.

  @param[in]       This       Indicates a pointer to the calling context.
  @param[in]       MediaId    Id of the media, changes every time the media is
                              replaced.
  @param[in]       Lba        The starting Logical Block Address to read from.
  @param[in, out]  Token      A pointer to the token associated with the
                              transaction.
  @param[in]       BufferSize Size of Buffer, must be a multiple of device
                              block size.
  @param[out]      Buffer     A pointer to the destination buffer for the data.
                              The caller is responsible for either having
                              implicit or explicit ownership of the buffer.
  @param[in]      Index       A pointer to the configuration table index. This is
  @param[in]      CryptoIvInput A pointer to a buffer that contains additional

  @retval EFI_SUCCESS           The read request was queued if Token->Event is
                                not NULL.The data was read correctly from the
                                device if the Token->Event is NULL.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing
                                the read.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHANGED     The MediaId is not for the current media.
  @retval EFI_BAD_BUFFER_SIZE   The BufferSize parameter is not a multiple of
                                the intrinsic block size of the device.
  @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not
                                valid, or the buffer is not on proper
                                alignment.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a
                                lack of resources.

**/
EFI_STATUS
EFIAPI
NvmeBlockIoCryptoReadBlocksEx (
  IN     EFI_BLOCK_IO_CRYPTO_PROTOCOL  *This,
  IN     UINT32                        MediaId,
  IN     EFI_LBA                       Lba,
  IN OUT EFI_BLOCK_IO_CRYPTO_TOKEN     *Token,
  IN     UINT64                        BufferSize,
  OUT    VOID                          *Buffer,
  IN     UINT64                        *Index OPTIONAL,
  IN     VOID                          *CryptoIvInput OPTIONAL
  );

/**
  Write BufferSize bytes from Lba into Buffer.

  This function writes the requested number of blocks to the device. All blocks
  are written, or an error is returned.If EFI_DEVICE_ERROR, EFI_NO_MEDIA,
  EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED is returned and non-blocking I/O is
  being used, the Event associated with this request will not be signaled.

  @param[in]       This       Indicates a pointer to the calling context.
  @param[in]       MediaId    The media ID that the write request is for.
  @param[in]       Lba        The starting logical block address to be written.
                              The caller is responsible for writing to only
                              legitimate locations.
  @param[in, out]  Token      A pointer to the token associated with the
                              transaction.
  @param[in]       BufferSize Size of Buffer, must be a multiple of device
                              block size.
  @param[in]       Buffer     A pointer to the source buffer for the data.
  @param[in]      Index         A pointer to the configuration table index. This is
  optional.
  @param[in]      CryptoIvInput A pointer to a buffer that contains additional


  @retval EFI_SUCCESS           The write request was queued if Event is not
                                NULL.
                                The data was written correctly to the device if
                                the Event is NULL.
  @retval EFI_WRITE_PROTECTED   The device can not be written to.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHNAGED     The MediaId does not matched the current
                                device.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing
                                the write.
  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size
                                of the device.
  @retval EFI_INVALID_PARAMETER The write request contains LBAs that are not
                                valid, or the buffer is not on proper
                                alignment.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a
                                lack of resources.

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
  );

/**
   Flush the Block Device.

   @param  This              Indicates a pointer to the calling context.
   @param  Token         A pointer to the token associated with the transaction.

   @retval EFI_SUCCESS       All outstanding data was written to the device
   @retval EFI_DEVICE_ERROR  The device reported an error while writing back the data
   @retval EFI_NO_MEDIA      There is no media in the device.

**/
EFI_STATUS
EFIAPI
NvmeBlockIoCryptoFlushBlocks (
  IN     EFI_BLOCK_IO_CRYPTO_PROTOCOL  *This,
  IN OUT EFI_BLOCK_IO_CRYPTO_TOKEN     *Token
  );

#endif
