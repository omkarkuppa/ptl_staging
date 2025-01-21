/** @file
  Definitions for Shared Mailbox table

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

#ifndef SHARED_MAILBOX_H
#define SHARED_MAILBOX_H


#define OS_GETVAR_FAIL                  (0xCE)
#define SKIP_DEF_VAR_SNAPSHOT           (0x51)
#define BOOT_FLAG_EN                    (0xE9)
#define LEGACY_CLI_KNOBS_FORMAT         (0x00)
#define GANGES_XML                      (0x01)
#define FLAT_KNOBS_XML                  (0x02)
#define XMLCLI_DONE_POSTCODE            (0xFC)
#define XMLCLI_GEN2_DONE_POSTCODE       (0xDE)

#define OFF_SEND_KNOBS_SIG              (0x08)
#define OFF_GBT_ADDRESS                 (0x0C)
#define OFF_LT_RESET                    (0x10)
#define OFF_POST_DONE                   (0x12)
#define OFF_GBT_COMPRESSED_ADDRESS      (0x1C)
#define OFF_PRPOST                      (0x32)
#define LEG_KNOB_ADDRESS_OFF            (0x34)
#define LEG_KNOB_SIZE_OFF               (0x38)
#define SETUP_KNOB_ADDRESS_OFF          (0x3C)
#define SETUP_KNOB_SIZE_OFF             (0x40)
#define DRAM_SHARED_MB_OFF              (0x44)
#define KNOB_XML_ENTRY_OFF              (0x48)

#define OFF_GEN2_XML_CLI_ENABLED        (0x28)
#define OFF_GEN2_GBT_ADDRESS            (0x50)
#define OFF_GEN2_GBT_COMPR_ADDR         (0x5C)

#define OFF_XML_CLI_TEMP_ADDR           (0x60)
#define OFF_BIOS_KNOBS_DATA_BIN_ADDR    (0x68)
#define OFF_BIOS_KNOBS_DATA_BIN_SIZE    (0x6C)

#define ITP_XDP_VEND_DEV_ID             (0x14928086)  // custom define for ITP vendor Device ID
#define ITP_XDP_BAR_NO                  (0xD)         // custom define for ITP DAL interface

#define SHARED_MEM_MAX_ENTRY_NUMBER     (10)

#define READ_MB                         (0x4D)
#define WRITE_MB                        (0x91)
#define READ_MAILBOX(Offset, Size, BufferAddr, MailboxAddress, PcieAddr)  ReadWriteMB(Offset, Size, BufferAddr, MailboxAddress, PcieAddr, READ_MB)
#define WRITE_MAILBOX(Offset, Size, BufferAddr, MailboxAddress, PcieAddr)  ReadWriteMB(Offset, Size, BufferAddr, MailboxAddress, PcieAddr, WRITE_MB)

// header Signature
#define SHARED_MB_BIOS_SIG              (0xBA5EBA11)

// for Legacy mailbox KNOBS area
#define LEG_MAILBOX_SIG                 (0x5A7ECAFE)
#define LEG_MAILBOX_OFFSET              (0x200)
#define LEG_MAILBOX_SIZE                (0x3E00)

#define CLI_BUFFER_SIZE                 (0x100200)
#define CLI_LITE_BUFFER_SIZE            (0x30000)

// CLI Request block area
#define CLI_REQ_SIG                     (0xCA11AB1E)
#define CLI_REQ_BUFFER_OFFSET           (LEG_MAILBOX_OFFSET + LEG_MAILBOX_SIZE)
// CLI Response block area
#define CLI_RES_SIG                     (0xCA11B0B0)
#define CLI_RES_BUFFER_OFFSET           (CLI_REQ_BUFFER_OFFSET + CLI_BUFFER_SIZE)
#define CLI_LITE_RES_BUFFER_OFFSET      (CLI_REQ_BUFFER_OFFSET + CLI_LITE_BUFFER_SIZE)

#define XML_CLI_DISABLED_SIG            (0xCD15A1ED)

#define GET_SET_TEMP_DATA_BUFF_SIZE     (0x20000)
#define XMLCLI_DEBUG_LOG_BUFF_SIZE      (0x40000)

#define SHARED_MB_LAST_SIG              (0xDEADBEA7)

#define SHARED_MEMORY_FLAG_MEMORY_TYPE  (0)
#define SHARED_MEMORY_FLAG_MMIO_TYPE    (1)
#define SHARED_MEMORY_FLAG_IO_TYPE      (2)


#pragma pack(push, 1)

typedef struct _SHARED_MEMORY_HEADER_ENTRY {
  UINT32  BiosSignature;
  union {
    struct {
      UINT32  EntriesNumber    : 8;
      UINT32  FirstEntryOffset : 8;
      UINT32  EntrySize        : 8;
      UINT32  ReservedByte     : 7;
      UINT32  ValidHeader      : 1;
    } EntriesInfo;
    UINT32 RawEntriesInfo;
  } _u1;
  UINT32  BiosSignature2;
  UINT32  CheckSum;
  struct {
    UINT32  Reserved0 : 8;
    UINT32  DeviceId  : 16;
    UINT32  Bar       : 4;
    UINT32  Reserved1 : 4;
  } CommonFlags;
  struct {
    UINT32  MinorVersion   : 8;
    UINT32  MajorVersion   : 16;
    UINT32  ReleaseVersion : 4;
    UINT32  Reserved       : 4;
  } CliSpecVersion;
  UINT32  Reserved[2];
} SHARED_MEMORY_HEADER_ENTRY;

typedef struct _SHARED_MEMORY_ENTRY {
  UINT32  Signature;
  UINT32  Offset;
  UINT32  Size;
  union {
    struct {
      UINT32  AccessType    : 3;
      UINT32  ReservedType  : 5;
      UINT32  DeviceId      : 16;
      UINT32  Bar           : 4;
      UINT32  CapabilityBit : 4;
    } Flags;
    UINT32  RawFlags;
  } _u2;
} SHARED_MEMORY_ENTRY;


typedef struct _SHARED_MEMORY_TABLE {
  SHARED_MEMORY_HEADER_ENTRY  Header;

  ///
  /// One entry for terminator 0xDEADBEA7
  ///
  SHARED_MEMORY_ENTRY  Entry[SHARED_MEM_MAX_ENTRY_NUMBER + 1];
} SHARED_MEMORY_TABLE;

typedef struct {
  UINT32  Signature;
  UINT32  Offset;
  UINT32  Size;
  UINT32  AccessType;
} SHARED_MAILBOX_TYPE;


/**
  Initialize Shared Memory Table

  @param[in,out] SharedMemTable  Pointer address at which Shared Memory Table to be initialized
  @param[in]     DeviceId        Device Id
  @param[in]     BarNumber       Base Address Register Number

  @retval  VOID
**/
VOID
SharedMemInitTable (
  IN OUT SHARED_MEMORY_TABLE  *SharedMemTable,
  IN UINT16                   DeviceId,
  IN UINT8                    BarNumber
  );

/**
  Add Entry to Shared Memory Table

  @param[in,out] SharedMemTable  EFI Shared Memory Table
  @param[in]     Signature       Signature of the Entry
  @param[in]     Offset          Offset of the Entry
  @param[in]     Size            Size of the Entry

  @retval  EFI_SUCCESS             Operation Successful
  @retval  EFI_OUT_OF_RESOURCES    Resources are not available to add new entry
  @retval  !EFI_SUCCESS            Failure
**/
EFI_STATUS
SharedMemAddEntry (
  IN OUT SHARED_MEMORY_TABLE  *SharedMemTable,
  IN UINT32                   Signature,
  IN UINT32                   Offset,
  IN UINT32                   Size,
  IN UINT32                   AccessType
  );

/**
  Get Offset and Size of the Entry from Shared memory table by Signature

  @param[in]  SharedMemTable  EFI Shared Memory Table pointer to lookup entry
  @param[in]  Signature       Signature to look for offset and size
  @param[out] Offset          Entry's offset is returned if Signature found
  @param[out] Size            Entry's size is returned if Signature found

  @retval  EFI_SUCCESS             Operation Successful
  @retval  EFI_INVALID_PARAMETER   Parameter Offset or Size is not valid
  @retval  !EFI_SUCCESS            Failure
**/
EFI_STATUS
SharedMemGetEntryBySignature (
  IN SHARED_MEMORY_TABLE  *SharedMemTable,
  IN UINT32               Signature,
  OUT UINT32              *Offset,
  OUT UINT32              *Size
  );

/**
  Add Table to Shared Memory Table

  @param[in,out] SharedMemTable  Shared Memory table where entry is to be added
  @param[in]     XmlCliCommon    XmlCli Common Structure
  @param[in]     SharedMailBox   Pointer to shared mailbox
  @param[in]     MailboxAddress  Shared mailbox address

  @retval  EFI_SUCCESS   Entry added successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
SharedMemAddTable (
  IN OUT SHARED_MEMORY_TABLE*  SharedMemTable,
  IN VOID                      *XmlCliCommon,
  IN SHARED_MAILBOX_TYPE*      SharedMailBox,
  IN UINT32                    MailboxAddress
  );

/**
  Construct Shared Mail Box

  @param[in] XmlCliCom       XmlCli Common Structure
  @param[in] MailboxTable    Pointer to where Shared mailbox table to construct
  @param[in] BarNumber       BAR Number
  @param[in] VenDevId        Device Vendor Id
  @param[in] MailboxAddress  Shared mailbox address

  @retval  EFI_SUCCESS   Entry added successfully
  @retval  !EFI_SUCCESS  Failure
**/
VOID
SharedMemConstructSharedMB (
  IN VOID                 *XmlCliCom,
  IN SHARED_MAILBOX_TYPE  *MailboxTable,
  IN UINT8                BarNumber,
  IN UINT32               VenDevId,
  IN UINT32               MailboxAddress
  );

#pragma pack(pop)
#endif // SHARED_MAILBOX_H
