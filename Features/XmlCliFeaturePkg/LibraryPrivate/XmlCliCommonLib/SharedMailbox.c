/** @file
  Code implementing Shared Mailbox table generation in System
  memory.

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

#include <XmlCliComLib.h>


/**
  Min: Returns minimum of 2 numbers

  @param[in]     XmlCliCommon

  @retval  UINT32  Minimum value between two input numbers
**/
UINT32
Min (
  IN UINT32  X,
  IN UINT32  Y
  )
{
  if (X < Y)
  { return X; }
  else
  { return Y; }
}


/**
  Reads/Writes HIF Mailbox

  @param[in]  Offset          Mailbox offset
  @param[in]  Size            Size in bytes
  @param[out] Buffer          Output buffer
  @param[in]  MailboxAddress  Device Private Data
  @param[in]  OpRdOrWr        Operation type

  @retval  EFI_SUCCESS   Operation Successfull
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
ReadWriteHIF (
  IN  UINT32  Offset,
  IN  UINT32  Size,
  OUT VOID    *Buffer,
  IN  UINT32  MailboxAddress,
  IN  UINT8   OpRdOrWr
  )
{
  UINT32           i;
  UINT32           tempOffset;
  UINT32           noOfAlignedIo;
  UINT32           tempData;
  volatile UINT32  *mailboxAddr;

  // Our HIF card supports only dword RW and dword aligned offsets
  // Determine aligned offset and number of IOs to perform
  tempOffset  = (Offset/4) * 4;    // Get aligned offset

  // Determine start address
  mailboxAddr  = (UINT32*)(UINTN)((UINTN)MailboxAddress + tempOffset);

  // Perform as many IOs as required
  if (Offset & 0x3)  {  // Mis Aligned Offset?
    UINT32  lenOfValidData = Min(Size, 4-(Offset-tempOffset));
    UINT32  offsetWithinUnalignedDword = (Offset-tempOffset);
    // If mis-aligned offset, do first IO and extract only specific bytes
    if (OpRdOrWr == READ_MB) {
      tempData = *mailboxAddr++;
      CopyMem(Buffer, (VOID*) (((UINT8*)&tempData) + offsetWithinUnalignedDword), lenOfValidData);
    } else if (OpRdOrWr == WRITE_MB) {
      tempData = *mailboxAddr;
      CopyMem((VOID*) (((UINT8*)&tempData) + offsetWithinUnalignedDword), Buffer, lenOfValidData);
      *mailboxAddr++ = tempData;
    }
    Buffer = ((UINT8*)Buffer) + lenOfValidData;
    Size -= lenOfValidData;
  }

  noOfAlignedIo = Size/4;   // No. of Dword aligned IOs
  for (i=0; (noOfAlignedIo!=0) && (i < noOfAlignedIo); i++)  {  // Do Dword Aligned IOs
    if (OpRdOrWr == READ_MB) {
      *((UINT32*)Buffer) = * mailboxAddr++;
    } else if (OpRdOrWr == WRITE_MB) {
      * mailboxAddr++ =  *((UINT32*)Buffer);
    }
    Buffer  = ((UINT32*)Buffer)+1;
  }

  if (Size & 0x3)  {  // Mis aligned size?
    // Mis-aligned size, Do Last IO and extract only specific bytes
    tempData = *mailboxAddr;
    if (OpRdOrWr == READ_MB) {
      CopyMem(Buffer, (VOID*) &tempData, Size-noOfAlignedIo*4);
    } else if (OpRdOrWr == WRITE_MB) {
      CopyMem((VOID*) &tempData,  Buffer, Size-noOfAlignedIo*4);
      *mailboxAddr  = tempData;
    }
  }
  return EFI_SUCCESS;
}

/**
  Reads/Writes Mailbox

  @param[in]  Offset          Mailbox offset
  @param[in]  Size            Size in bytes
  @param[out] Buffer          Output buffer
  @param[in]  MailboxAddress  Device Private Data and should not exceed to PCIe Address
  @param[in]  PcieAddr        PCIe Address
  @param[in]  OpRdOrWr        Operation type

  @retval  EFI_SUCCESS   Operation Successfull
  @retval  !EFI_SUCCESS  Failure
**/
VOID ReadWriteMB (
  IN  UINT32  Offset,
  IN  UINT32  Size,
  OUT VOID    *Buffer,
  IN  UINT32  MailboxAddress,
  IN  UINT32  PcieAddr,
  IN  UINT8   OpRdOrWr
  )
{
  if (MailboxAddress == 0) { // either in SOFT SDV flow or MB address is 0
//    DEBUG ((DEBUG_INFO, "XML_CLI: SoftSDV Flow or MB address is 0, hence returning. \n"));
    return;
  }
  Offset = Offset + LEG_MAILBOX_OFFSET;

  if (MailboxAddress < PcieAddr) // if mailbox is below MM CFG base then its sitting in DRAM
  { // Write to the DRAM Mailbox
    if (OpRdOrWr == READ_MB) {
      CopyMem (Buffer, (VOID*)(UINTN)((UINTN)MailboxAddress + Offset), Size);
    } else if (OpRdOrWr == WRITE_MB) {
      CopyMem ((VOID*)(UINTN)((UINTN)MailboxAddress + Offset), Buffer, Size);
    }
    return;
  }

  ReadWriteHIF(Offset, Size, Buffer, MailboxAddress, OpRdOrWr);
}

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
  )
{
  UINT32  entry;

  if (Offset == NULL || Size == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (entry = 0; entry < SharedMemTable->Header._u1.EntriesInfo.EntriesNumber; entry++) {
    if (SharedMemTable->Entry[entry].Signature == Signature) {
      *Offset = SharedMemTable->Entry[entry].Offset;
      *Size   = SharedMemTable->Entry[entry].Size;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Calculate and store checksum of the Shared Memory Table

  @param[in,out] SharedMemTable  EFI Shared Memory Table, update
                                  header checksum

  @retval  VOID
**/
VOID
SharedMemCalculateChecksum (
  IN OUT SHARED_MEMORY_TABLE  *SharedMemTable
  )
{
  UINT8   i;
  UINT32  checkSum;

  checkSum = 0;
  for (i = 0; i < sizeof (SHARED_MEMORY_TABLE) / 4; i++) { // alignment to DWORD is must!
    checkSum += *((UINT32 *) (SharedMemTable) + i);
  }
  SharedMemTable->Header.CheckSum = checkSum;
}

/**
  Add Entry to SHared Memory Table for last entry with
  last entry signature

  @param[in,out] SharedMemTable  EFI Shared Memory Table

  @retval  VOID
**/
VOID
SharedMemSetLastEntry (
  IN OUT SHARED_MEMORY_TABLE  *SharedMemTable
  )
{
  UINT32  entry;  // update the last entry

  entry = SharedMemTable->Header._u1.EntriesInfo.EntriesNumber;

  SharedMemTable->Entry[entry].Signature      = SHARED_MB_LAST_SIG;
  SharedMemTable->Entry[entry].Offset         = SHARED_MB_LAST_SIG;
  SharedMemTable->Entry[entry].Size           = SHARED_MB_LAST_SIG;
  SharedMemTable->Entry[entry]._u2.RawFlags  = SHARED_MB_LAST_SIG;
}

/**
  Add Entry to Shared Memory Table

  @param[in,out] SharedMemTable  EFI Shared Memory Table
  @param[in]     Signature       Signature of the Entry
  @param[in]     Offset          Offset of the Entry
  @param[in]     Size            Size of the Entry

  @retval  EFI_SUCCESS             Operation Successfull
  @retval  EFI_OUT_OF_RESOURCES    Resources are not available to add new entry
  @retval  !EFI_SUCCESS            Failure
**/
EFI_STATUS
SharedMemAddEntry (
  IN OUT SHARED_MEMORY_TABLE  *SharedMemTable,
  IN UINT32                 Signature,
  IN UINT32                 Offset,
  IN UINT32                 Size,
  IN UINT32                 AccessType
  )
{
  UINT32  entryNum;

  if (SharedMemTable->Header._u1.EntriesInfo.EntriesNumber >= SHARED_MEM_MAX_ENTRY_NUMBER) {
    return EFI_OUT_OF_RESOURCES;
  }

  entryNum  = SharedMemTable->Header._u1.EntriesInfo.EntriesNumber;

  SharedMemTable->Header._u1.EntriesInfo.ValidHeader    = 0;
  SharedMemTable->Entry[entryNum].Signature             = Signature;
  SharedMemTable->Entry[entryNum].Offset                = Offset;
  SharedMemTable->Entry[entryNum].Size                  = Size;
  SharedMemTable->Entry[entryNum]._u2.Flags.AccessType  = AccessType;

  SharedMemTable->Entry[entryNum]._u2.Flags.DeviceId      = 0;
  SharedMemTable->Entry[entryNum]._u2.Flags.Bar           = 0;
  SharedMemTable->Entry[entryNum]._u2.Flags.ReservedType  = 0;
  SharedMemTable->Entry[entryNum]._u2.Flags.CapabilityBit = 0;

  if (AccessType == SHARED_MEMORY_FLAG_MMIO_TYPE) {
    SharedMemTable->Entry[entryNum]._u2.Flags.DeviceId  = SharedMemTable->Header.CommonFlags.DeviceId;
    SharedMemTable->Entry[entryNum]._u2.Flags.Bar       = SharedMemTable->Header.CommonFlags.Bar;
  }

  SharedMemTable->Header._u1.EntriesInfo.EntriesNumber++;
  SharedMemSetLastEntry (SharedMemTable);

  SharedMemTable->Header._u1.EntriesInfo.ValidHeader = 1;
  SharedMemCalculateChecksum (SharedMemTable);
  return EFI_SUCCESS;
}

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
  )
{
  ZeroMem ((VOID *) SharedMemTable, sizeof (SHARED_MEMORY_TABLE));

  SharedMemTable->Header.BiosSignature                   = SHARED_MB_BIOS_SIG;
  SharedMemTable->Header.BiosSignature2                  = SHARED_MB_BIOS_SIG;
  SharedMemTable->Header._u1.EntriesInfo.EntrySize        = sizeof (SHARED_MEMORY_ENTRY);
  SharedMemTable->Header._u1.EntriesInfo.FirstEntryOffset = VARIABLE_OFFSET (SHARED_MEMORY_TABLE, Entry);
  SharedMemTable->Header.CommonFlags.DeviceId             = DeviceId;
  SharedMemTable->Header.CommonFlags.Bar                  = BarNumber;
  SharedMemTable->Header.CliSpecVersion.MinorVersion      = 0;
  SharedMemTable->Header.CliSpecVersion.MajorVersion      = 6;
  SharedMemTable->Header.CliSpecVersion.ReleaseVersion    = 0;

  SharedMemTable->Header._u1.EntriesInfo.EntriesNumber    = 0;
  SharedMemSetLastEntry (SharedMemTable);

  // set the header as valid
  SharedMemTable->Header._u1.EntriesInfo.ValidHeader = 1;
  SharedMemCalculateChecksum (SharedMemTable);
}

/**
  Add Table to Shared Memory Table

  @param[in,out] SharedMemTable  Shared Memory table where entry is to be added
  @param[in]     XmlCliCommon    XmlCli Common Structure
  @param[in]     SharedMailBox   Pointer to shared mailbox
  @param[in]     MailboxAddress  Shared mailbox address

  @retval  EFI_SUCCESS   Entry added successfully
  @retval  !EFI_SUCCESS  Failure**/
EFI_STATUS
SharedMemAddTable (
  IN OUT SHARED_MEMORY_TABLE*  SharedMemTable,
  IN VOID                    *XmlCliCommon,
  IN SHARED_MAILBOX_TYPE*    SharedMailBox,
  IN UINT32                  MailboxAddress
  )
{
  UINT32      Signature;
  UINT32      Offset;
  UINT32      Size;
  UINTN       index;
  EFI_STATUS  Status;

  index = 0;

  while (SharedMailBox[ index ].Signature != SHARED_MB_LAST_SIG) {
    Signature = SharedMailBox[ index ].Signature;
    Offset = SharedMailBox[ index ].Offset;
    Size = SharedMailBox[ index ].Size;
    if (MailboxAddress < ((XML_CLI_COMMON*)XmlCliCommon)->PcieAddr) { // this is for Dram Shared Mailbox.
      Offset = ((XML_CLI_COMMON*)XmlCliCommon)->DramSharedMBAddr + Offset;
    } else {
      if ((Signature == CLI_REQ_SIG) || (Signature == CLI_RES_SIG)) {
        Offset = ((XML_CLI_COMMON*)XmlCliCommon)->DramSharedMBAddr + Offset;
      }
    }
    Status = SharedMemAddEntry(SharedMemTable, Signature, Offset, Size, SharedMailBox[ index ].AccessType);

    if (EFI_ERROR(Status)) {
      return (Status);
    }
    index++;
  }
  return EFI_SUCCESS;
}

/**
  Construct and Publish Dram Shared Mail Box

  @param[in] XmlCliCom       XmlCli Common Structure
  @param[in] MailboxTable    Pointer to where Shared mailbox table to construct
  @param[in] BarNumber       Base Address Register Number
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
  )
{
  UINT8                PublishShareMB;
  UINT32               Data32;
  EFI_STATUS           Status;
  SHARED_MEMORY_TABLE  SharedMemTable;
  SHARED_MEMORY_TABLE  *SharedMemTablePtr;
  XML_CLI_COMMON       *XmlCliCommon;

  SharedMemTablePtr = (SHARED_MEMORY_TABLE*)(UINTN)MailboxAddress;
  XmlCliCommon = (XML_CLI_COMMON*)XmlCliCom;

  PublishShareMB = 0;
  if ((SharedMemTablePtr->Header.BiosSignature != SHARED_MB_BIOS_SIG) ||
      (SharedMemTablePtr->Header.BiosSignature2 != SHARED_MB_BIOS_SIG)) {
    PublishShareMB = 1;
  }

  READ_MAILBOX(SETUP_KNOB_ADDRESS_OFF, 4, &Data32, MailboxAddress, XmlCliCommon->PcieAddr);
  if ((PublishShareMB == 0) && (Data32 != 0) && (Data32 != 0xAFAFAFAF))
  {
    DEBUG ((DEBUG_INFO, "XML_CLI:  DRAM Shared Mailbox Already Published, overwriting.\n"));
    //return;  // already published.
  }
  DEBUG ((DEBUG_INFO, "XML_CLI:  Constructing DRAM Shared Mailbox .\n"));
  ZeroMem((void*)(UINTN)MailboxAddress, 0x100);  // Zero out Shared MB header

  SharedMemInitTable(&SharedMemTable, (UINT16)(VenDevId >> 16) , BarNumber);
  if (XmlCliCommon->CliGen2Enable) {
    SharedMemTable.Header.CliSpecVersion.MinorVersion   = 0;
    SharedMemTable.Header.CliSpecVersion.MajorVersion   = 8;
    SharedMemTable.Header.CliSpecVersion.ReleaseVersion = 0;
  }
  Status = SharedMemAddTable(&SharedMemTable, (VOID*)XmlCliCommon, MailboxTable, MailboxAddress);
  ASSERT_EFI_ERROR(Status);

  ReadWriteHIF(0, sizeof(SHARED_MEMORY_TABLE), &SharedMemTable, MailboxAddress, WRITE_MB);  // copy shared mailbox table.

  if (XmlCliCommon->CliGen2Enable) {
    ///
    /// Indicate that XML CLI is enabled by setting BIT1 in target Status Flags offset
    ///
    Data32 = 1 << 1;
    WRITE_MAILBOX(OFF_GEN2_XML_CLI_ENABLED,    4, &Data32,                           MailboxAddress, XmlCliCommon->PcieAddr);
    Data32 = XMLCLI_GEN2_DONE_POSTCODE;
  } else {
    Data32 = XMLCLI_DONE_POSTCODE;
  }

  if ((SharedMemTable.Header.CliSpecVersion.MajorVersion >= 7) || (SharedMemTable.Header.CliSpecVersion.ReleaseVersion > 0)) {
    WRITE_MAILBOX(OFF_GEN2_GBT_ADDRESS,       4, &XmlCliCommon->GbtXmlAddress,      MailboxAddress, XmlCliCommon->PcieAddr);
  } else {
    WRITE_MAILBOX(OFF_GBT_ADDRESS,            4, &XmlCliCommon->GbtXmlAddress,      MailboxAddress, XmlCliCommon->PcieAddr);
  }
  WRITE_MAILBOX(DRAM_SHARED_MB_OFF,           4, &XmlCliCommon->DramSharedMBAddr,   MailboxAddress, XmlCliCommon->PcieAddr);
  WRITE_MAILBOX(OFF_BIOS_KNOBS_DATA_BIN_ADDR, 4, &XmlCliCommon->BiosKnobsDataAddr,  MailboxAddress, XmlCliCommon->PcieAddr);
  WRITE_MAILBOX(OFF_BIOS_KNOBS_DATA_BIN_SIZE, 4, &XmlCliCommon->BiosKnobsDataSize,  MailboxAddress, XmlCliCommon->PcieAddr);

  if (XmlCliCommon->SetupKnobsAddress != 0) {
    WRITE_MAILBOX(OFF_POST_DONE,              1, &Data32,                           MailboxAddress, XmlCliCommon->PcieAddr);
    Data32 = 0x5E9D90B5;
    WRITE_MAILBOX(OFF_SEND_KNOBS_SIG,         4, &Data32,                           MailboxAddress, XmlCliCommon->PcieAddr);
    WRITE_MAILBOX(SETUP_KNOB_ADDRESS_OFF,     4, &XmlCliCommon->SetupKnobsAddress,  MailboxAddress, XmlCliCommon->PcieAddr);
    WRITE_MAILBOX(SETUP_KNOB_SIZE_OFF,        4, &XmlCliCommon->SetupKnobsSize,     MailboxAddress, XmlCliCommon->PcieAddr);
    WRITE_MAILBOX(KNOB_XML_ENTRY_OFF,         4, &XmlCliCommon->KnobXmlEntryAddr,   MailboxAddress, XmlCliCommon->PcieAddr);
  }
}
