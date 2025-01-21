/** @file
    The implementation for Block IO Test application.

  Copyright (c) 2022 - 2023, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SortLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Library/ShellLib.h>
#include <Library/ShellCommandLib.h>
#include <Protocol/ShellParameters.h>
#include <Protocol/BlockIo.h>
#include <Protocol/BlockIoCrypto.h>

#include "HexPi.h"

#define BLOCK_IO_TEST_APP_VER  L"0.2.1"

#define TSE_IO_16_BYTE_ALIGNMENT  0x10 // 16 Byte alignment

CONST CHAR16    gShellTestAppFileName[] = L"TestApp";
CONST CHAR16    gReadOperationType[] = L"read";
CONST CHAR16    gWriteOperationType[] = L"write";
CONST CHAR16    gBlockIoOperation[] = L"blockio";
CONST CHAR16    gBlockIoCryptoOperation[] = L"blockiocrypto";

EFI_HII_HANDLE  gBlockIoTestAppHiiHandle  = NULL;

EFI_GUID gKeyOwnerGuid = { 0x5f4ce6c1, 0xdd53, 0x43cd, { 0xa8, 0x80, 0x87, 0xb6, 0x94, 0x60, 0xa3, 0x60 } };
EFI_GUID gNilGuid      = { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
UINT64 * gCryptoKeyPtr;

/**
   An array of acpiview command line parameters.
**/
STATIC CONST SHELL_PARAM_ITEM  ParamList[] = {
  { L"-h",          TypeFlag  },
  { L"-showconfig", TypeFlag  },
  { L"-setconfig",  TypeFlag  },
  { L"-index",      TypeValue },
  { L"-key",        TypeValue },
  { L"-clear",      TypeFlag  },
  { L"-r",          TypeFlag  },
  { L"-w",          TypeFlag  },
  { L"-cr",         TypeFlag  },
  { L"-cw",         TypeFlag  },
  { L"-device",     TypeValue },
  { L"-tweak",      TypeValue },
  { L"-lba",        TypeValue },
  { L"-count",      TypeValue },
  { L"-align",      TypeValue },
  { L"-offset",     TypeValue },
  { L"-pattern",    TypeValue },
  { L"-zero",       TypeFlag  },
  { L"-debug",      TypeFlag  },
  { L"-encrypt",    TypeFlag  },
  { L"-slba",       TypeValue },
  { L"-elba",       TypeValue },
  { L"-clba",       TypeValue },
  { NULL,           TypeMax   }
};

typedef struct {
  /*
    Flag to indicate the provided index should be cleared from the UEFI crypto configuration.
  */
  BOOLEAN        ClearConfigFlag;
  /*
    Index to be configured in the UEFI crypto configuration.
   */
  UINT64         Index;
  /*
    Key to be configured in the UEFI crypto configuration.
   */
  UINT64         CryptoKey;
} CRYPTO_TABLE_CONFIG;

typedef struct {
  /*
    The Lba number to start from.
  */
  UINT64  Lba;
  /*
    How many blocks to read.
  */
  UINT64  BlockCount;
  /*
    If true, Align parameter determines the buffer alignment value override. Else,
    the alignment parameter is ignored and EFI_PAGE_SIZE is used for alignment.
  */
  BOOLEAN AlignOverrideFlag;
  /*
    If AlignOverrideFlag is true, this value describes the buffer alignment.
  */
  UINT64  Align;
  /*
    Offset value of the host buffer pointer, used for the NVMe IO operation.
  */
  UINT64  Offset;
  /*
    If true, the function will use the BlockIoCrypto protocol and parameters to perform
    the cryptographic IO operation. Else, it will use BlockIo protocol to perform a
    regular IO operation.
  */
  BOOLEAN CryptoOperationFlag;
  /*
    For cryptographic operations, this is the pointer to the index in the Crypto Configuration table
    to be used in the BlockIoCrypto protocol for encrypting and decrypting data.
  */
  UINT64 * IndexPtr;
  /*
    If true, the function will write into the NVMe block device.
  */
  BOOLEAN WriteOperationFlag;
  /*
    If true, will write zeros to all blocks.
  */
  BOOLEAN ZeroOutFlag;
  /*
     Custom 64-Bit pattern value
  */
  UINT64  PatternValue;
  /*
    Flag to signal hexdump of read bytes.
  */
  BOOLEAN DebugFlag;

  /*
    Flag to indicate the encryption drive request.
  */
  BOOLEAN EncryptionDriveFlag;

  /*
    The Start Lba number to start from encryption process.
  */
  UINT64 EncryptionStartLba;

  /*
    The End Lba number to start from encryption process.
  */
  UINT64 EncryptionEndLba;

  /*
    Flag to indicate full drive encryption
  */
  BOOLEAN EncryptionFullRequestFlag;

  /*
    Number of LBAs to be issued thru protocols at a time
  */
  UINT64 EncryptionLbasAtTime;

  /*
    CryptoIvInput to describe the custom tweak value
  */
  EFI_BLOCK_IO_CRYPTO_IV_INPUT_AES_XTS * CryptoIvInputPtr;
} TEST_IO_CONFIG;

/**
   This function dumps the current contents of the passed configuration table

   @retval EFI_SUCCESS     If the configuration table is not NULL
**/
EFI_STATUS
DumpResponseConfigurationTable (
  IN CONST EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ResponseTable,
  IN CONST UINT64 EntriesNo
  )
{
  UINT64  ResultEntryIdx;
  ResultEntryIdx = 0;
  ShellPrintEx (-1, -1, L"Response configuration:\n", ResultEntryIdx);

  if (ResponseTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (ResultEntryIdx = 0; ResultEntryIdx < EntriesNo; ResultEntryIdx++) {
    if (CompareGuid (&(ResponseTable[ResultEntryIdx].KeyOwnerGuid), &gNilGuid)) {
      // Skip entries with NilGuid owner
      continue;
    }

    ShellPrintEx (-1, -1, L"EntryIdx[%d]\n", ResultEntryIdx);
    ShellPrintEx (-1, -1, L"  Index: %d\n", ResponseTable[ResultEntryIdx].Index);
    ShellPrintEx (-1, -1, L"  KeyOwnerGuid: %g\n", &(ResponseTable[ResultEntryIdx].KeyOwnerGuid));
    ShellPrintEx (-1, -1, L"  Capability:\n");
                  ShellPrintEx (-1, -1, L"    Algorithm: %g\n", &(ResponseTable[ResultEntryIdx].Capability.Algorithm));
    ShellPrintEx (-1, -1, L"    KeySize: 0x%016llx\n", ResponseTable[ResultEntryIdx].Capability.KeySize);
    ShellPrintEx (-1, -1, L"    CryptoBlockSizeBitMask: 0x%016llx\n", ResponseTable[ResultEntryIdx].Capability.CryptoBlockSizeBitMask);
  }

  return EFI_SUCCESS;
}

/**
   This function constructs a configuration table entry based on the passed parameters
   and calls the BlockIoCrypto SetConfiguration API to register the new entry.

   @param[in] CryptoTableConfig  Parameters for configuring a key into the UEFI configuration table

   @retval EFI_SUCCESS     If the configuration was registered successfully
   @retval other           If an error occured during configuration.
**/
EFI_STATUS
SetUefiTableCryptoConfiguration (
  IN CONST CRYPTO_TABLE_CONFIG *CryptoTableConfig
  )
{
  EFI_STATUS  Status;
  EFI_BLOCK_IO_CRYPTO_PROTOCOL  *BlockIoCrypto;
  EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *NewConfiguration;
  EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ResultingTable;

  Status = gBS->LocateProtocol (&gEfiBlockIoCryptoProtocolGuid, NULL, (VOID **) &BlockIoCrypto);

  if (EFI_ERROR (Status)) {
    ShellPrintEx (-1, -1, L"Unable to open BlockIoCrypto protocol!\n");
    return Status;
  }

  //
  // Allocate configuration entries
  //
  NewConfiguration = AllocateZeroPool (sizeof (EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY) * 1);
  ResultingTable = AllocateZeroPool (sizeof (EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY) * 1);
  gCryptoKeyPtr = AllocateZeroPool (sizeof (UINT64) * 1);
  *gCryptoKeyPtr = CryptoTableConfig->CryptoKey;

  NewConfiguration[0].Index = (UINT64) CryptoTableConfig->Index;

  if (CryptoTableConfig->ClearConfigFlag == FALSE) {
    NewConfiguration[0].KeyOwnerGuid = gKeyOwnerGuid;
  } else {
    NewConfiguration[0].KeyOwnerGuid = gNilGuid;
  }

  NewConfiguration[0].CryptoKey = gCryptoKeyPtr;

  NewConfiguration[0].Capability.Algorithm = gTseBlockIoCryptoAlgoAesXtsGuid;
  NewConfiguration[0].Capability.KeySize = BIT8;
  NewConfiguration[0].Capability.CryptoBlockSizeBitMask = BIT9 | BIT12;

  Status = BlockIoCrypto->SetConfiguration (BlockIoCrypto, 1, NewConfiguration, ResultingTable);

  DumpResponseConfigurationTable (ResultingTable, 1);
  FreePool (ResultingTable);
  return Status;
}

/**
   This function retrieves up to 16 entries from the current configuration table

   @retval EFI_SUCCESS     If the configuration was registered successfully
   @retval other           If an error occured during configuration.
**/
EFI_STATUS
GetUefiTableCryptoConfiguration (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_BLOCK_IO_CRYPTO_PROTOCOL  *BlockIoCrypto;
  EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ResultingTable;

  Status = gBS->LocateProtocol (&gEfiBlockIoCryptoProtocolGuid, NULL, (VOID **) &BlockIoCrypto);

  if (EFI_ERROR (Status)) {
    ShellPrintEx (-1, -1, L"Unable to open BlockIoCrypto protocol!\n");
    return Status;
  }

  //
  // Allocate configuration entries
  //
  ResultingTable = AllocateZeroPool (sizeof (EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY) * 16);
  Status = BlockIoCrypto->GetConfiguration (BlockIoCrypto, 0, 16, &gKeyOwnerGuid, ResultingTable);
  DumpResponseConfigurationTable (ResultingTable, 16);
  FreePool (ResultingTable);
  return Status;
}

/**
   Helper function to print progress bar.
   @param[in] current            Parameter which points to current progress value.
   @param[in] max                Parameter which points to maximum progrees value.
   @param[in] frequencyDisplay   Parameter which points to how often refresh progress bar.
   @param[in] colPos             Parameter which points to column position to print progress bar.
   @param[in] rowPos             Parameter which points to row position to print progress bar.
   @param[in] debug              Parameter which points to debug flow.
**/
VOID
DisplayProgressBar(
  UINTN current,
  UINTN max,
  UINTN frequencyDisplay,
  INT32 colPos,
  INT32 rowPos,
  BOOLEAN debug
)
{
  if (current % frequencyDisplay == 0) {
    if(debug) {
      ShellPrintEx((INT32)colPos + 5, (INT32)rowPos, L"(%d / %d)", current, max);
    } else {
      ShellPrintEx((INT32)colPos, (INT32)rowPos, L"%d %%", (current)*100/max);
    }
  }
}

/**
   Perform encryption operations on NVMe drive using the BlockIo and BlockIoCrypto protocols.

   @param[in] TestIoConfig         Nvme IO operation parameters.

   @retval EFI_SUCCESS             The read operation completed successfully
**/
EFI_STATUS
PerformEncodingOnBlockDevice(
  IN TEST_IO_CONFIG *TestIoConfig,
  IN BOOLEAN encryptWholeDriveFlag
)
{
  EFI_STATUS                    Status;
  EFI_BLOCK_IO_PROTOCOL         *BlockIo;
  EFI_BLOCK_IO_CRYPTO_PROTOCOL  *BlockIoCrypto;
  EFI_HANDLE                    BlockIoCryptoHandle;
  EFI_HANDLE                    *CryptoHandleBuffer;

  UINTN       Alignment  = EFI_PAGE_SIZE;   // by default set page aligned size
  UINTN       Index      = 0;               // by default use crypto key index from MOCK functionality
  UINTN       BlockCount = 1;               // by default perform read/write per 1 block
  UINT8       *OriginalBuffer;
  UINT8       *AlignedBuffer;
  UINTN       BufferSize;
  UINTN       numHandles;                   // stores number of handles of BLOCK_IO_CRYPTO_PROTOCOL
  UINTN       startLba;                     // stores start LBA value for encryption process
  UINTN       endLba;                       // stores end LBA value for encryption process
  UINTN       totalLbaNum;                  // stores total number of LBA to be encrypted
  UINTN       progressBarDivider;           // used as a divider for progress bar purposes
  INT32       colPos;                       // used to store current column position of the cursor
  INT32       rowPos;                       // used to store current row position of the cursor
  UINTN       lba;                          // helper variable used to store current LBA value for encryption process
  UINTN       lbaLeft;                      // helper variable used to store number of LBAs which are left for encryption

  EFI_BLOCK_IO_CRYPTO_IV_INPUT_AES_XTS CryptoIvInput = { 0 };

  // get customized block read if specified
  if(TestIoConfig->EncryptionLbasAtTime != 0) {
    BlockCount = TestIoConfig->EncryptionLbasAtTime;
  }

  // override alignment if specified
  if(TestIoConfig->Align != 0) {
    Alignment = TestIoConfig->Align;
  }

  // Locate BLOCK IO CRYPTO on NVMe CONTROLLER
  Status = gBS->LocateHandleBuffer(
      ByProtocol,
      &gEfiBlockIoCryptoProtocolGuid,
      NULL,
      &numHandles,
      &CryptoHandleBuffer);

  if (EFI_ERROR(Status)) {
    ShellPrintEx(-1, -1, L"%EERROR: Unable to locate BLOCK IO CRYPTO PROTOCOL ON SYSTEM!\n");
    goto EXIT;
  }

  ShellPrintEx(-1, -1, L"[PASS] BLOCK IO CRYPTO detected.\r\n");

  BlockIoCryptoHandle = CryptoHandleBuffer[0];

  // OPEN BLOCK IO CRYPTO PROTOCOL
  Status = gBS->OpenProtocol(BlockIoCryptoHandle, &gEfiBlockIoCryptoProtocolGuid, (VOID **)&BlockIoCrypto, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR(Status)) {
    ShellPrintEx(-1, -1, L"%EERROR: Unable to open OUTPUT BlockIoCrypto protocol!\n");
    goto EXIT;
  }

  if (BlockIoCrypto->Media->IoAlign == 0) {
    BlockIoCrypto->Media->IoAlign = 1;
  }

  CryptoIvInput.CryptoBlockByteSize = BlockIoCrypto->Media->BlockSize;

  ShellPrintEx(-1, -1, L"[PASS] BLOCK IO CRYPTO opened.\r\n");

  // Open BLOCK IO PROTOCOL
  Status = gBS->OpenProtocol(
          CryptoHandleBuffer[0],
          &gEfiBlockIoProtocolGuid,
          (VOID **)&BlockIo,
          gImageHandle,
          NULL,
          EFI_OPEN_PROTOCOL_GET_PROTOCOL
          );

  if (EFI_ERROR(Status)) {
    ShellPrintEx(-1, -1, L"%EERROR: Unable to open BlockIo protocol!\n");
    goto EXIT;
  }

  ShellPrintEx(-1, -1, L"[PASS] BLOCK IO protocol opened.\r\n");
  ShellPrintEx(-1, -1, L"\r\n");

  // override encrypting range when specified
  if(encryptWholeDriveFlag) {
    startLba = 0;
    endLba   = BlockIo->Media->LastBlock;
  } else {
    startLba = TestIoConfig->EncryptionStartLba;
    endLba   = TestIoConfig->EncryptionEndLba;
  }

  // calculate total LBA number for encryption
  totalLbaNum = (endLba - startLba) + 1;

  // sanity check for chunked LBA value
  if(BlockCount > totalLbaNum) {
    BlockCount = totalLbaNum;
    ShellPrintEx(-1, -1, L"WARNING: Provided chunked LBA value exceeds the LBA range size. Setting chunk LBA to 0x%X.\r\n", BlockCount);
  }

  // sanity check for max LBA range
  if(totalLbaNum > BlockIo->Media->LastBlock + 1) {
    totalLbaNum = BlockIo->Media->LastBlock;
    ShellPrintEx(-1, -1, L"WARNING: Provided end LBA value exceeds the drive size. Setting max LBA range to 0x%X.\r\n", totalLbaNum);
  }

  // calculate buffer size
  BufferSize = BlockIo->Media->BlockSize * (UINTN)BlockCount;
  if (BlockIo->Media->IoAlign == 0) {
    BlockIo->Media->IoAlign = 1;
  }

  OriginalBuffer = AllocateZeroPool(BufferSize + Alignment);

  if(OriginalBuffer == NULL) {
    ShellPrintEx(-1, -1, L"%EERROR: Unable to allocate enough memory for buffer (requested %d bytes)!\n", BufferSize);
    Status = EFI_OUT_OF_RESOURCES;
    goto EXIT;
  }

  AlignedBuffer = ALIGN_POINTER(OriginalBuffer, Alignment);

  // calculate the correct value of progress frequency divider
  progressBarDivider = 1;
  if(encryptWholeDriveFlag) {
    progressBarDivider = 20000;
  } else if (totalLbaNum > 1000) {
    progressBarDivider = totalLbaNum / 1000;
  } else {
      progressBarDivider = TestIoConfig->EncryptionLbasAtTime;
  }

  colPos = gST->ConOut->Mode->CursorColumn;
  rowPos = gST->ConOut->Mode->CursorRow;

  ShellPrintEx(-1, -1, L"Encrypting in progress (startLba: 0x%X endLba: 0x%X total LBA to convert: 0x%X)\r\n", startLba, endLba, totalLbaNum);

  lba = startLba;
  while (lba <= endLba)
  {
    CryptoIvInput.CryptoBlockNumber = lba;

    // READ thru BLOCK IO
    Status = BlockIo->ReadBlocks(
        BlockIo,
        BlockIo->Media->MediaId,
        lba,
        BufferSize,
        AlignedBuffer);

    if (EFI_ERROR(Status)) {
      ShellPrintEx(-1, -1, L"%EERROR: Cannot read BLOCK! (lba 0x%08x status %r)\r\n", lba, Status);
      goto EXIT;
    }

    // WRITE ENCRYPTED DATA THRU BLOCK IO CRYPTO
    Status = BlockIoCrypto->WriteExtended(
        BlockIoCrypto,
        BlockIoCrypto->Media->MediaId,
        lba,
        NULL, // Token
        BufferSize,
        AlignedBuffer,
        &Index,
        &CryptoIvInput
    );

    if (EFI_ERROR(Status)) {
      ShellPrintEx(-1, -1, L"%EERROR: Cannot Write BLOCK IO CRYPTO DATA (lba 0x%08x status = %r)\r\n", lba, Status);
      goto EXIT;
    }

    // clear buffer
    ZeroMem(AlignedBuffer, BufferSize);

    lba += BlockCount;

    // display progress bar
    DisplayProgressBar(lba-startLba, totalLbaNum, progressBarDivider, colPos, rowPos, TestIoConfig->DebugFlag);

    // in case of lower granularity than pointed by chunked LBA, change the buffer size to fill the last request
    lbaLeft = (totalLbaNum - (lba-startLba));
    if(lbaLeft < BlockCount) {
      BufferSize = lbaLeft * BlockIo->Media->BlockSize;
    }
  }

  DisplayProgressBar(100, 100, 100, colPos, rowPos, TestIoConfig->DebugFlag);
  ShellPrintEx(-1, -1, L"\r\nAlignment = 0x%x index = 0x%x LBA range <0x%X;0x%X> \r\n", Alignment, Index, startLba, endLba);
  ShellPrintEx(-1, -1, L"%VSUCCESS: Finished to encode drive!\r\n");

EXIT:
  //
  // Close corresponding protocols
  //
  gBS->CloseProtocol (BlockIoCryptoHandle, &gEfiBlockIoCryptoProtocolGuid, gImageHandle, NULL);

  if (OriginalBuffer != NULL) {
    FreePool (OriginalBuffer);
  }

  if (CryptoHandleBuffer != NULL) {
    FreePool (CryptoHandleBuffer);
  }

  return Status;
}


/**
   Perform IO operations on NVMe drive using the corresponding protocols.

   @param[in] DeviceName           The block device name to get the blocks from.
   @param[in] TestIoConfig         Nvme IO operation parameters.

   @retval EFI_SUCCESS             The read operation completed successfully
**/
EFI_STATUS
NvmeIoOperation (
  IN CONST CHAR16 *DeviceName,
  IN TEST_IO_CONFIG *TestIoConfig
)
{
  EFI_STATUS                    Status;
  EFI_DEVICE_PATH_PROTOCOL      *DevPath;
  EFI_BLOCK_IO_PROTOCOL         *BlockIo;
  EFI_HANDLE                    BlockIoHandle;
  EFI_BLOCK_IO_CRYPTO_PROTOCOL  *BlockIoCrypto;
  EFI_HANDLE                    BlockIoCryptoHandle;

  UINTN   BufferNaturalAlignment;
  UINT8   *OriginalBuffer;
  UINT8   *AlignedBuffer;
  UINTN   BufferSize;
  UINT32  *PatternAddr;

  UINTN   CopyIndex;
  UINTN   PatternSize;
  UINTN   CopySize;
  UINT64  RemainderSize;

  Status = EFI_SUCCESS;
  DevPath = NULL;
  BufferSize = 0;

  if (TestIoConfig->PatternValue == 0) {
    PatternSize = sizeof (HexPi);
    PatternAddr = HexPi;
  } else {
    PatternSize = sizeof (UINT64);
    PatternAddr = (UINT32 *) &(TestIoConfig->PatternValue);
  }

  if (TestIoConfig->AlignOverrideFlag == FALSE) {
    BufferNaturalAlignment = EFI_PAGE_SIZE;
  } else {
    BufferNaturalAlignment = (UINTN) TestIoConfig->Align;
  }

  //
  // Locate corresponding protocols, calculate buffer size based on block device block size.
  //
  DevPath = (EFI_DEVICE_PATH_PROTOCOL *)gEfiShellProtocol->GetDevicePathFromMap (DeviceName);
  if (DevPath == NULL) {
    ShellPrintEx (-1, -1, L"Invalid blockname!\n");
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PARAM_INV), gBlockIoTestAppHiiHandle, L"BlockIoTestApp", DeviceName);
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GET_USAGE_BLOCKIOTESTAPP), gBlockIoTestAppHiiHandle, L"BlockIoTestApp");
    return EFI_INVALID_PARAMETER;
  }

  if (TestIoConfig->CryptoOperationFlag == FALSE) {
    //
    // Block IO
    //
    Status = gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid, (EFI_DEVICE_PATH_PROTOCOL **)&DevPath, &BlockIoHandle);
    if (EFI_ERROR (Status)) {
      ShellPrintEx (-1, -1, L"Unable to locate device!\n");
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_MAP_PROTOCOL), gBlockIoTestAppHiiHandle, L"BlockIoTestApp", DeviceName);
      return Status;
    }

    Status = gBS->OpenProtocol (BlockIoHandle, &gEfiBlockIoProtocolGuid, (VOID **)&BlockIo, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR (Status)) {
      ShellPrintEx (-1, -1, L"Unable to open BlockIo protocol!\n");
      return Status;
    }

    BufferSize = BlockIo->Media->BlockSize * (UINTN) TestIoConfig->BlockCount;
    if (BlockIo->Media->IoAlign == 0) {
      BlockIo->Media->IoAlign = 1;
    }

    ShellPrintEx (-1, -1, L"  BlockIo->IoAlign: 0x%08x\r\n", BlockIo->Media->IoAlign);
    ShellPrintEx (-1, -1, L"  BlockSize: 0x%08x, BlockCount: 0x%08x\r\n", BlockIo->Media->BlockSize, TestIoConfig->BlockCount);
  } else {
    //
    // Block IO Crypto
    //
    Status = gBS->LocateDevicePath (&gEfiBlockIoCryptoProtocolGuid, (EFI_DEVICE_PATH_PROTOCOL **)&DevPath, &BlockIoCryptoHandle);
    if (EFI_ERROR (Status)) {
      ShellPrintEx (-1, -1, L"Unable to locate device!\n");
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_MAP_PROTOCOL), gBlockIoTestAppHiiHandle, L"BlockIoCryptoTestApp", DeviceName);

      return Status;
    }

    Status = gBS->OpenProtocol (BlockIoCryptoHandle, &gEfiBlockIoCryptoProtocolGuid, (VOID **)&BlockIoCrypto, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR (Status)) {
      ShellPrintEx (-1, -1, L"Unable to open BlockIoCrypto protocol!\n");
      return Status;
    }

    //
    // Configure CryptoIvInput pointer
    //
    if ((TestIoConfig->IndexPtr != NULL) && (TestIoConfig->CryptoIvInputPtr != NULL)) {
      TestIoConfig->CryptoIvInputPtr->CryptoBlockByteSize = BlockIoCrypto->Media->BlockSize;
      ShellPrintEx (-1, -1, L"  CryptoIvInput->CryptoBlockByteSize: 0x%016llx, CryptoIvInput->CryptoBlockNumber: 0x%016llx\r\n", TestIoConfig->CryptoIvInputPtr->CryptoBlockByteSize, TestIoConfig->CryptoIvInputPtr->CryptoBlockNumber);
    } else {
      ShellPrintEx (-1, -1, L"WARNING: No index provided. Performing non-cryptographic IO operation.\n");
    }

    BufferSize = BlockIoCrypto->Media->BlockSize * (UINTN) TestIoConfig->BlockCount;
    if (BlockIoCrypto->Media->IoAlign == 0) {
      BlockIoCrypto->Media->IoAlign = 1;
    }

    ShellPrintEx (-1, -1, L"  BlockIoCrypto->IoAlign: 0x%08x\r\n", BlockIoCrypto->Media->IoAlign);
    ShellPrintEx (-1, -1, L"  BlockIoCrypto->Media->BlockSize: 0x%08x, BlockCount: 0x%08x\r\n", BlockIoCrypto->Media->BlockSize, TestIoConfig->BlockCount);
  }

  OriginalBuffer  = AllocateZeroPool (BufferSize + BufferNaturalAlignment + (UINTN) TestIoConfig->Offset);
  AlignedBuffer   = ALIGN_POINTER (OriginalBuffer, BufferNaturalAlignment);
  AlignedBuffer   += (UINTN) TestIoConfig->Offset;

  ShellPrintEx (-1, -1, L"  Host Buffer @ 0x%08x\r\n", AlignedBuffer);

  if (TestIoConfig->WriteOperationFlag == FALSE) {
    if (TestIoConfig->CryptoOperationFlag == FALSE) {
      //
      // Block IO Read
      //
      ShellPrintHiiEx (
        -1,
        -1,
        NULL,
        STRING_TOKEN (STR_BLOCK_IO_TEST_APP_HEADER),
        gBlockIoTestAppHiiHandle,
        TestIoConfig->Lba,
        BufferSize
        );

      Status = BlockIo->ReadBlocks (
                          BlockIo,
                          BlockIo->Media->MediaId,
                          TestIoConfig->Lba,
                          BufferSize,
                          AlignedBuffer);
    } else {
      //
      // Block IO Crypto Read
      //
      ShellPrintHiiEx (
        -1,
        -1,
        NULL,
        STRING_TOKEN (STR_BLOCK_IO_TEST_APP_HEADER),
        gBlockIoTestAppHiiHandle,
        TestIoConfig->Lba,
        BufferSize
        );

      Status = BlockIoCrypto->ReadExtended (
                                BlockIoCrypto,
                                BlockIoCrypto->Media->MediaId,
                                TestIoConfig->Lba,
                                NULL, // Token
                                BufferSize,
                                AlignedBuffer,
                                TestIoConfig->IndexPtr,
                                TestIoConfig->CryptoIvInputPtr
                                );
    }

    if (!EFI_ERROR (Status) && (AlignedBuffer != NULL) && (TestIoConfig->DebugFlag == TRUE)) {
      ShellPrintEx (-1, -1, L"Dumping read blocks...\n");
      DumpHex (2, 0, BufferSize, AlignedBuffer);
    } else if (EFI_ERROR (Status)) {
      ShellPrintEx (-1, -1, L"ReadBlocks failed! Status = %r\n", Status);
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_FILE_READ_FAIL), gBlockIoTestAppHiiHandle, L"BlockIoTestApp");
      Status = EFI_DEVICE_ERROR;
    }
  } else {
    if (TestIoConfig->ZeroOutFlag == TRUE) {
      ShellPrintEx (-1, -1, L"WARNING: Zero out write operation!\n");
    } else if (BufferSize <= PatternSize) {
      //
      // Copy hardcoded pattern into host buffer
      //
      PatternSize = BufferSize;
      //
      // Fill buffer with pattern
      //
      CopyMem (AlignedBuffer, PatternAddr, PatternSize);
    } else {
      //
      // Fill buffer with multiple copies of the pattern
      //
      CopySize = (UINTN)DivU64x64Remainder ((UINT64)BufferSize, PatternSize, &RemainderSize);

      CopyIndex = 0;
      while ((CopySize != 0) && (CopyIndex < CopySize)) {
        CopyMem (AlignedBuffer + (CopyIndex * PatternSize), PatternAddr, PatternSize);
        CopyIndex++;
      }
      CopyMem (AlignedBuffer + (CopyIndex * PatternSize), PatternAddr, (UINTN) RemainderSize);
    }
    if (TestIoConfig->CryptoOperationFlag == FALSE) {
      //
      // Block IO Write
      //
      ShellPrintHiiEx (
        -1,
        -1,
        NULL,
        STRING_TOKEN (STR_BLOCK_IO_TEST_APP_HEADER),
        gBlockIoTestAppHiiHandle,
        TestIoConfig->Lba,
        BufferSize
        );

      Status = BlockIo->WriteBlocks (
                          BlockIo,
                          BlockIo->Media->MediaId,
                          TestIoConfig->Lba,
                          BufferSize,
                          AlignedBuffer);
    } else {
      //
      // Block IO Crypto Write
      //
      ShellPrintHiiEx (
        -1,
        -1,
        NULL,
        STRING_TOKEN (STR_BLOCK_IO_TEST_APP_HEADER),
        gBlockIoTestAppHiiHandle,
        TestIoConfig->Lba,
        BufferSize
        );

      Status = BlockIoCrypto->WriteExtended (
                                BlockIoCrypto,
                                BlockIoCrypto->Media->MediaId,
                                TestIoConfig->Lba,
                                NULL, // Token
                                BufferSize,
                                AlignedBuffer,
                                TestIoConfig->IndexPtr,
                                TestIoConfig->CryptoIvInputPtr
                                );
    }

    if (!EFI_ERROR (Status) && (AlignedBuffer != NULL) && (TestIoConfig->DebugFlag == TRUE)) {
      ShellPrintEx (-1, -1, L"Dumping written blocks...\n");
      ShellPrintHiiEx (
        -1,
        -1,
        NULL,
        STRING_TOKEN (STR_BLOCK_IO_TEST_APP_HEADER),
        gBlockIoTestAppHiiHandle,
        TestIoConfig->Lba,
        BufferSize
        );

      DumpHex (2, 0, BufferSize, AlignedBuffer);
    } else if (EFI_ERROR (Status)) {
      ShellPrintEx (-1, -1, L"Write failed! Status = %r\n", Status);
      Status = EFI_DEVICE_ERROR;
    }
  }

  //
  // Close corresponding protocols
  //
  if (TestIoConfig->CryptoOperationFlag == FALSE) {
    gBS->CloseProtocol (BlockIoHandle, &gEfiBlockIoProtocolGuid, gImageHandle, NULL);
  } else {
    gBS->CloseProtocol (BlockIoCryptoHandle, &gEfiBlockIoCryptoProtocolGuid, gImageHandle, NULL);
  }

  if (OriginalBuffer != NULL) {
    FreePool (OriginalBuffer);
  }

  return Status;
}

//
// String token ID of help message text.
// Shell supports to find help message in the resource section of an application image if
// .MAN file is not found. This global variable is added to make build tool recognizes
// that the help string is consumed by user and then build tool will add the string into
// the resource section. Thus the application can use '-?' option to show help message in
// Shell.
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID  mStringHelpTokenId = STRING_TOKEN (STR_GET_HELP_BLOCKIOTESTAPP);

/**
   Application Entry Point wrapper around the shell command

   @param[in] ImageHandle  Handle to the Image (NULL if internal).
   @param[in] SystemTable  Pointer to the System Table (NULL if internal).
**/
EFI_STATUS
EFIAPI
BlockIoTestAppMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS     Status;
  LIST_ENTRY     *Package;
  TEST_IO_CONFIG TestIoConfig;
  CRYPTO_TABLE_CONFIG CryptoTableConfig;

  UINT64  Index;
  EFI_BLOCK_IO_CRYPTO_IV_INPUT_AES_XTS CryptoIvInput;

  //
  // Input parameter strings
  //
  CONST CHAR16   *InputAlignString;
  CONST CHAR16   *InputOffsetString;
  CONST CHAR16   *InputIndexString;
  CONST CHAR16   *InputCryptoKeyString;
  CONST CHAR16   *InputBlockDeviceName;
  CONST CHAR16   *InputLbaString;
  CONST CHAR16   *InputBlockCountString;
  CONST CHAR16   *InputTweakString;
  CONST CHAR16   *InputPatternString;

  CONST CHAR16   *InputStartLbaString;
  CONST CHAR16   *InputEndLbaString;
  CONST CHAR16   *InputChunkLbaString;

  Status  = EFI_SUCCESS;
  Package = NULL;

  ZeroMem (&TestIoConfig, sizeof (TEST_IO_CONFIG));
  ZeroMem (&CryptoTableConfig, sizeof (CRYPTO_TABLE_CONFIG));

  //
  // Now initialize the shell library (it requires Shell Parameters protocol)
  //
  Status = ShellInitialize ();
  ASSERT_EFI_ERROR (Status);

  Status = CommandInit ();
  ASSERT_EFI_ERROR (Status);

  gBlockIoTestAppHiiHandle = HiiAddPackages (&gBlockIoTestAppHiiGuid, gImageHandle, BlockIoTestAppStrings, NULL);

  if (gBlockIoTestAppHiiHandle == NULL) {
    ShellPrintEx (-1, -1, L"ERROR: Unable to allocate HII handle!\n");
    return EFI_DEVICE_ERROR;
  }

  ShellPrintEx (-1, -1, L"=========================================================\n");
  ShellPrintEx (-1, -1, L"        BlockIo & BlockIoCrypto Test Application\n");
  ShellPrintEx (-1, -1, L"        Version: %s\n", BLOCK_IO_TEST_APP_VER);
  ShellPrintEx (-1, -1, L"=========================================================\n");

  Status = ShellCommandLineParse (ParamList, &Package, NULL, TRUE);

  if (EFI_ERROR (Status)) {
    ShellPrintEx (-1, -1, L"Error parsing parameters! Status = %r\n", Status);
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM), gBlockIoTestAppHiiHandle);
    Status = EFI_INVALID_PARAMETER;
    goto ON_EXIT;
  }

  //
  // If help flag is passed, always display help string without executing any operation
  //
  if (ShellCommandLineGetFlag (Package, L"-h")) {
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GET_HELP_BLOCKIOTESTAPP), gBlockIoTestAppHiiHandle);
    Status = EFI_SUCCESS;
    goto ON_EXIT;
  }

  if (ShellCommandLineGetFlag (Package, L"-setconfig")) {
    //
    // If set configuration flag is passed, parse key index, key value and clear flag
    //
    if (ShellCommandLineGetFlag (Package, L"-index")) {
      InputIndexString = ShellCommandLineGetValue (Package, L"-index");
      if (InputIndexString != NULL) {
        ShellConvertStringToUint64 (InputIndexString, &(CryptoTableConfig.Index), FALSE, FALSE);
      }
    }

    if (ShellCommandLineGetFlag (Package, L"-key")) {
      InputCryptoKeyString = ShellCommandLineGetValue (Package, L"-key");
      if (InputCryptoKeyString != NULL) {
        ShellConvertStringToUint64 (InputCryptoKeyString, &(CryptoTableConfig.CryptoKey), FALSE, FALSE);
      }
    }

    if (ShellCommandLineGetFlag (Package, L"-clear")) {
      CryptoTableConfig.ClearConfigFlag = TRUE;
    }

    //
    // If configuration parameters are correct, attempt to set new entry
    // into UEFI configuration table.
    //
    if ((InputIndexString != NULL) &&
        ((InputCryptoKeyString != NULL) ||
         (CryptoTableConfig.ClearConfigFlag == TRUE))) {
      ShellPrintEx (-1, -1, L"Setting UEFI crypto table entry configuration...\n");
      Status = SetUefiTableCryptoConfiguration (&CryptoTableConfig);
      goto ON_EXIT;
    } else {
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_TOO_FEW), gBlockIoTestAppHiiHandle);
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GET_USAGE_BLOCKIOTESTAPP), gBlockIoTestAppHiiHandle);
      Status = EFI_INVALID_PARAMETER;
      goto ON_EXIT;
    }
  } else if (ShellCommandLineGetFlag (Package, L"-showconfig")) {
    ShellPrintEx (-1, -1, L"Dumping UEFI crypto table configuration...\n");
    //
    // If show configuration flag is passed, show the current UEFI configuration table.
    //
    Status = GetUefiTableCryptoConfiguration ();
    goto ON_EXIT;
  } else if (ShellCommandLineGetFlag (Package, L"-w")) {
    //
    // Regular Block IO write operation
    //
    ShellPrintEx (-1, -1, L"Write operation...\n");
    TestIoConfig.CryptoOperationFlag = FALSE;
    TestIoConfig.WriteOperationFlag = TRUE;
    if (ShellCommandLineGetFlag (Package, L"-zero")) {
      TestIoConfig.ZeroOutFlag = TRUE;
    }
  } else if (ShellCommandLineGetFlag (Package, L"-r")) {
    //
    // Regular Block IO read operation
    //
    ShellPrintEx (-1, -1, L"Read operation...\n");
    TestIoConfig.CryptoOperationFlag = FALSE;
    TestIoConfig.WriteOperationFlag = FALSE;
  } else if (ShellCommandLineGetFlag (Package, L"-cw")) {
    //
    // Block IO Crypto write operation
    //
    ShellPrintEx (-1, -1, L"Crypto write operation...\n");
    TestIoConfig.CryptoOperationFlag = TRUE;
    TestIoConfig.WriteOperationFlag = TRUE;
    if (ShellCommandLineGetFlag (Package, L"-zero")) {
      TestIoConfig.ZeroOutFlag = TRUE;
    }
  } else if (ShellCommandLineGetFlag (Package, L"-cr")) {
    //
    // Block IO Crypto read operation
    //
    ShellPrintEx (-1, -1, L"Crypto read operation...\n");
    TestIoConfig.CryptoOperationFlag = TRUE;
    TestIoConfig.WriteOperationFlag = FALSE;
  } else if (ShellCommandLineGetFlag (Package, L"-encrypt")) {
    //
    // ENCRYPT GIVEN BLOCK DEVICE
    //
    ShellPrintEx (-1, -1, L"Drive encryption operation...\n");
    ShellPrintEx (-1, -1, L"Note about limitation: Only one drive shall be connected to the system for encryption process.\n");
    TestIoConfig.EncryptionDriveFlag = TRUE;
  }
  else {
    ShellPrintEx (-1, -1, L"ERROR: Incorrect parameter usage!\n");
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GET_USAGE_BLOCKIOTESTAPP), gBlockIoTestAppHiiHandle);
    Status = EFI_INVALID_PARAMETER;
    goto ON_EXIT;
  }

  //
  // Setup IO Operation
  //
  if (ShellCommandLineGetFlag (Package, L"-debug")) {
    TestIoConfig.DebugFlag = TRUE;
  }

  if (TestIoConfig.CryptoOperationFlag == TRUE) {
    InputIndexString = ShellCommandLineGetValue (Package, L"-index");
    if (InputIndexString == NULL) {
      ShellPrintEx (-1, -1, L"WARNING: No index provided for crypto operation.\n");
      TestIoConfig.CryptoIvInputPtr = NULL;
      TestIoConfig.IndexPtr = NULL;
    } else {
      TestIoConfig.CryptoIvInputPtr = &CryptoIvInput;
      TestIoConfig.IndexPtr = &Index;
      ShellConvertStringToUint64 (InputIndexString, TestIoConfig.IndexPtr, TRUE, FALSE);

      InputTweakString = ShellCommandLineGetValue (Package, L"-tweak");
      if (InputTweakString == NULL) {
        ShellPrintEx (-1, -1, L"ERROR: No tweak value specified.\n");
        Status = EFI_INVALID_PARAMETER;
        goto ON_EXIT;
      } else {
        ShellConvertStringToUint64 (InputTweakString, &(TestIoConfig.CryptoIvInputPtr->CryptoBlockNumber), TRUE, FALSE);
      }
    }
  }

  InputLbaString = ShellCommandLineGetValue (Package, L"-lba");
  if (InputLbaString == NULL) {
    TestIoConfig.Lba = 0;
  } else {
    ShellConvertStringToUint64 (InputLbaString, &(TestIoConfig.Lba), TRUE, FALSE);
  }



  InputBlockCountString = ShellCommandLineGetValue (Package, L"-count");
  if (InputBlockCountString == NULL) {
    TestIoConfig.BlockCount = 1;
  } else {
    ShellConvertStringToUint64 (InputBlockCountString, &(TestIoConfig.BlockCount), TRUE, FALSE);
  }

  InputAlignString = ShellCommandLineGetValue (Package, L"-align");
  if (InputAlignString != NULL) {
    TestIoConfig.AlignOverrideFlag = TRUE;
    ShellConvertStringToUint64 (InputAlignString, &(TestIoConfig.Align), TRUE, FALSE);
  } else {
    TestIoConfig.Align = 0;
  }

  InputOffsetString = ShellCommandLineGetValue (Package, L"-offset");
  if (InputOffsetString == NULL) {
    TestIoConfig.Offset = 0;
  } else {
    ShellConvertStringToUint64 (InputOffsetString, &(TestIoConfig.Offset), TRUE, FALSE);
  }

  InputPatternString = ShellCommandLineGetValue (Package, L"-pattern");
  if (InputPatternString == NULL) {
    TestIoConfig.PatternValue = 0;
  } else {
    ShellConvertStringToUint64 (InputPatternString, &(TestIoConfig.PatternValue), TRUE, FALSE);
  }

  if(TestIoConfig.EncryptionDriveFlag == TRUE) {
    InputStartLbaString = ShellCommandLineGetValue(Package, L"-slba");
    if (InputStartLbaString == NULL) {
        ShellPrintEx(-1, -1, L"Encrypt whole Block Device...\n");
        TestIoConfig.EncryptionFullRequestFlag = TRUE;
    } else  {
      ShellConvertStringToUint64(InputStartLbaString, &(TestIoConfig.EncryptionStartLba), TRUE, FALSE);
      InputEndLbaString = ShellCommandLineGetValue(Package, L"-elba");
      if (InputEndLbaString == NULL) {
        ShellPrintEx(-1, -1, L"ERROR: No end lba specified!\n");
        Status = EFI_INVALID_PARAMETER;
        goto ON_EXIT;
      } else {
        ShellConvertStringToUint64(InputEndLbaString, &(TestIoConfig.EncryptionEndLba), TRUE, FALSE);
      }
    }

    InputChunkLbaString = ShellCommandLineGetValue(Package, L"-clba");
    if (InputChunkLbaString == NULL) {
      TestIoConfig.EncryptionLbasAtTime = 1;
    } else  {
      ShellConvertStringToUint64(InputChunkLbaString, &(TestIoConfig.EncryptionLbasAtTime), TRUE, FALSE);
    }

    PerformEncodingOnBlockDevice(&TestIoConfig, TestIoConfig.EncryptionFullRequestFlag);
    goto ON_EXIT;
  }

  InputBlockDeviceName = ShellCommandLineGetValue(Package, L"-device");
  if (InputBlockDeviceName == NULL)
  {
    ShellPrintEx(-1, -1, L"ERROR: No block device specified!\n");
    ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN(STR_GET_USAGE_BLOCKIOTESTAPP), gBlockIoTestAppHiiHandle);

    HiiRemovePackages(gBlockIoTestAppHiiHandle);
    Status = EFI_INVALID_PARAMETER;
    goto ON_EXIT;
   }

  NvmeIoOperation (InputBlockDeviceName, &TestIoConfig);

ON_EXIT:
  ShellCommandLineFreeVarList (Package);
  HiiRemovePackages (gBlockIoTestAppHiiHandle);
  return Status;
}
