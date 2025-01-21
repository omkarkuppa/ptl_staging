/** @file
  This file contains the implementation of BpdtLib library.

  The library exposes an abstract interface for accessing boot data
  stored in the BPDT format on the Logical Boot Partition of the boot device.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include <Uefi.h>
#include <PiPei.h>
#include <Ppi/BlockIo.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BpdtLib.h>

#define UFS_BOOT_LUN          1

/**
  Print FV name based on GUID
**/
VOID
PrintFvName (
  EFI_GUID  *FvName
)
{
   if (CompareGuid (FvName, &gEfiAuthenticatedVariableGuid)) {
    DEBUG ((DEBUG_INFO, "gEfiAuthenticatedVariableGuid\n"));
  } else if (CompareGuid (FvName, &gFspMemoryFvGuid)) {
    DEBUG ((DEBUG_INFO, "gFspMemoryFvGuid\n"));
  } else if (CompareGuid (FvName, &gFspTempRamFvGuid)) {
    DEBUG ((DEBUG_INFO, "gFspTempRamFvGuid\n"));
  } else if (CompareGuid (FvName, &gFvPreMemoryFvGuid)) {
    DEBUG ((DEBUG_INFO, "gFvPreMemoryFvGuid\n"));
  } else if (CompareGuid (FvName, &gFvSecurityFvGuid)) {
    DEBUG ((DEBUG_INFO, "gFvSecurityFvGuid\n"));
  } else if (CompareGuid (FvName, &gFvPostMemoryFvGuid)) {
    DEBUG ((DEBUG_INFO, "gFvPostMemoryFvGuid\n"));
  } else if (CompareGuid (FvName, &gFvUefiBootFvGuid)) {
    DEBUG ((DEBUG_INFO, "gFvUefiBootFvGuid\n"));
  } else if (CompareGuid (FvName, &gFvOsBootFvGuid)) {
    DEBUG ((DEBUG_INFO, "gFvOsBootFvGuid\n"));
  } else if (CompareGuid (FvName, &gFirmwareBinariesFvGuid)) {
    DEBUG ((DEBUG_INFO, "gFirmwareBinariesFvGuid\n"));
  } else if (CompareGuid (FvName, &gFvAdvancedFvGuid)) {
    DEBUG ((DEBUG_INFO, "gFvAdvancedFvGuid\n"));
  } /*else if (CompareGuid (FvName, &gMicrocodeFvGuid)) {
    DEBUG ((DEBUG_INFO, "gMicrocodeFvGuid\n"));
  } else {
    DEBUG((DEBUG_INFO, "unnamed Guid %g \n", FvName));
  }
}

/**
  Copy Length bytes from Source to Destination using SSE4.

  @param[out] Dst           The target of the copy request.
  @param[in]  Src           The place to copy from.
  @param[in]  SizeInBytes   The number of bytes to copy.
**/
VOID
CopyMemSse4 (
  OUT VOID  *Dst,
  IN  VOID  *Src,
  IN  UINTN SizeInBytes
  )
{
#if defined __GNUC__  // GCC compiler
  __asm__ __volatile__ (
    // Initialize pointers to start of the USWC memory
    "\n\t mov  %1, %%esi"  //mov esi, Src
    "\n\t mov  %1, %%edx"  //mov edx, Src

    // Initialize pointer to end of the USWC memory
    "\n\t add  %2, %%edx"  //add edx, SizeInBytes

    // Initialize pointer to start of the cacheable WB buffer
    "\n\t mov  %0, %%edi"  //mov edi, Dst

    // save xmm0 ~ xmm3 to stack
    "\n\t sub  $0x40, %%esp"
    "\n\t movdqu  %%xmm0, 0x00(%%esp)"
    "\n\t movdqu  %%xmm1, 0x10(%%esp)"
    "\n\t movdqu  %%xmm2, 0x20(%%esp)"
    "\n\t movdqu  %%xmm3, 0x30(%%esp)"

    // Start of Bulk Load loop
    "\n\t inner_start:"
    // Load data from USWC Memory using Streaming Load
    "\n\t MOVNTDQA  0x00(%%esi), %%xmm0"
    "\n\t MOVNTDQA  0x10(%%esi), %%xmm1"
    "\n\t MOVNTDQA  0x20(%%esi), %%xmm2"
    "\n\t MOVNTDQA  0x30(%%esi), %%xmm3"

    // Copy data to buffer
    "\n\t MOVDQA  %%xmm0, 0x00(%%edi)"
    "\n\t MOVDQA  %%xmm1, 0x10(%%edi)"
    "\n\t MOVDQA  %%xmm2, 0x20(%%edi)"
    "\n\t MOVDQA  %%xmm3, 0x30(%%edi)"

    // Increment pointers by cache line size and test for end of loop
    "\n\t add  $0x40, %%esi"
    "\n\t add  $0x40, %%edi"
    "\n\t cmp  %%edx, %%esi"
    "\n\t jne inner_start"

    // restore xmm0 ~ xmm3
    "\n\t mfence"
    "\n\t movdqu  0x00(%%esp), %%xmm0"
    "\n\t movdqu  0x10(%%esp), %%xmm1"
    "\n\t movdqu  0x20(%%esp), %%xmm2"
    "\n\t movdqu  0x30(%%esp), %%xmm3"
    "\n\t add  $0x40, %%esp" // stack cleanup
    ::"a"(Dst),"b"(Src),"c"(SizeInBytes)
    :"%esi", "%edi", "%edx"
  );
#else //MSFT compiler
  _asm {
    // Initialize pointers to start of the USWC memory
    mov esi, Src
    mov edx, Src

    // Initialize pointer to end of the USWC memory
    add edx, SizeInBytes

    // Initialize pointer to start of the cacheable WB buffer
    mov edi, Dst

    // save xmm0 ~ xmm3 to stack
    sub     esp, 040h
    movdqu  [esp], xmm0
    movdqu  [esp + 16], xmm1
    movdqu  [esp + 32], xmm2
    movdqu  [esp + 48], xmm3

    // Start of Bulk Load loop
    inner_start:
    // Load data from USWC Memory using Streaming Load
    MOVNTDQA xmm0, xmmword ptr [esi]
    MOVNTDQA xmm1, xmmword ptr [esi + 16]
    MOVNTDQA xmm2, xmmword ptr [esi + 32]
    MOVNTDQA xmm3, xmmword ptr [esi + 48]

    // Copy data to buffer
    MOVDQA xmmword ptr [edi], xmm0
    MOVDQA xmmword ptr [edi + 16], xmm1
    MOVDQA xmmword ptr [edi + 32], xmm2
    MOVDQA xmmword ptr [edi + 48], xmm3

    // Increment pointers by cache line size and test for end of loop
    add esi, 040h
    add edi, 040h
    cmp esi, edx
    jne inner_start

    // restore xmm0 ~ xmm3
    mfence
    movdqu  xmm0, [esp]
    movdqu  xmm1, [esp + 16]
    movdqu  xmm2, [esp + 32]
    movdqu  xmm3, [esp + 48]
    add     esp, 040h // stack cleanup
  }
#endif
}

/**
  Read data from UFS into Memory.
  The caller is responsible for calling FreePages on DataBuffer if necessary.

  @param    DataBuffer              A pointer to the buffer address
  @param    Offset                  Offset of the data to read (4k aligned)
  @param    DataSize                Size of the data to read

  @retval   EFI_SUCCESS             The operation completed successfully
  @retval   EFI_INVALID_PARAMETER   DataBuffer was NULL
  @retval   EFI_NOT_FOUND           Problems reading from UFS
  @retval   EFI_OUT_OF_RESOURCES    Couldn't allocate the buffer
**/
EFI_STATUS
EFIAPI
ReadFromUfs (
  IN OUT  VOID                  **DataBuffer,
  IN      UINT32                Offset,
  IN      UINTN                 DataSize
  )
{
  EFI_STATUS                    Status;
  EFI_PEI_SERVICES              **PeiServices;
  EFI_PEI_RECOVERY_BLOCK_IO_PPI *BlockIoPpi;
  EFI_PEI_BLOCK_IO_MEDIA        Media;
  EFI_PEI_LBA                   StartBlock;
  UINTN                         UfsBufferSize;
  VOID                          *UfsBuffer;

  if (DataBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "DataBuffer pointer is NULL!\n"));
    return EFI_INVALID_PARAMETER;
  }
  if (Offset % SIZE_4KB) {
    DEBUG ((DEBUG_ERROR, "Offset is not 4k aligned!\n"));
    return EFI_INVALID_PARAMETER;
  }

  *DataBuffer = NULL;
  if (DataSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer();

  //Find UFS Block IO PPI
  Status = PeiServicesLocatePpi (
            &gEfiPeiVirtualBlockIoPpiGuid,
            0,
            NULL,
            (VOID **) &BlockIoPpi
            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Virtual Block IO PPI not found!\n"));
    ASSERT_EFI_ERROR (Status);
    return EFI_NOT_FOUND;
  }

  // Check if boot media is present
  ZeroMem (&Media, sizeof(Media));
  Status = BlockIoPpi->GetBlockDeviceMediaInfo (
                        PeiServices,
                        BlockIoPpi,
                        UFS_BOOT_LUN,
                        &Media
                        );
  if (EFI_ERROR (Status) || !Media.MediaPresent) {
    DEBUG ((DEBUG_ERROR, "Fail to get MediaInfo or boot media not present\n"));
    return EFI_NOT_FOUND;
  }

  //Execute the read operation
  StartBlock =         (EFI_PEI_LBA) EFI_SIZE_TO_PAGES (Offset);
  UfsBufferSize = EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (DataSize));
  UfsBuffer =         AllocatePages (EFI_SIZE_TO_PAGES (DataSize));
  if (UfsBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = BlockIoPpi->ReadBlocks (
                        PeiServices,
                        BlockIoPpi,
                        UFS_BOOT_LUN,
                        StartBlock,
                        UfsBufferSize,
                        UfsBuffer
                        );

  *DataBuffer = UfsBuffer;
  return Status;
}

/**
  Provides a pointer to the CSE Layout Pointers structure, which is stored in a HOB.
  If the HOB does not exist, the HOB is created by reading from UFS.

  @param[out]  CseLayoutPointers      Pointer to the CSE Layout Pointers HOB data.

  @retval      EFI_SUCCESS            The CSE Layout Pointers structure was retrieved.
  @retval      EFI_OUT_OF_RESOURCES   Could not create a HOB to store the data.
  @retval      Others                 An error occurred reading from UFS.
**/
EFI_STATUS
EFIAPI
GetCseLayoutPointers (
  OUT CSE_LAYOUT_POINTERS   **CseLayoutPointers
  )
{
  EFI_STATUS            Status;
  VOID                  *UfsBuffer;
  EFI_HOB_GUID_TYPE     *GuidHobPtr;

  //Default to NULL to allow failure-checking from caller
  *CseLayoutPointers = NULL;
  Status = EFI_SUCCESS;

  //Check if data has already been retrieved
  GuidHobPtr = GetFirstGuidHob (&gIfwiCseLayoutPointersHobGuid);

  if (GuidHobPtr != NULL) {
    //Retrieve a pointer to the data
    *CseLayoutPointers = (CSE_LAYOUT_POINTERS *) GET_GUID_HOB_DATA (GuidHobPtr);
  } else {
   //Get the data from UFS
    Status = ReadFromUfs (&UfsBuffer, 0, sizeof (CSE_LAYOUT_POINTERS));
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //Store the data in a HOB
    *CseLayoutPointers = (CSE_LAYOUT_POINTERS *) BuildGuidDataHob (
                                                  &gIfwiCseLayoutPointersHobGuid,
                                                  UfsBuffer,
                                                  sizeof (CSE_LAYOUT_POINTERS)
                                                  );
    //Cleanup
    FreePages(UfsBuffer, EFI_SIZE_TO_PAGES (sizeof (CSE_LAYOUT_POINTERS)));
    if (*CseLayoutPointers == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  return Status;
}

/**
  Provides a pointer to the BIOS region of the IFWI (BPDT 4).
  The BIOS region is stored in memory.
  The pointer is stored in a HOB.
  If the HOB does not exist, the HOB is created by reading from UFS.

  @param[out]  BiosRegion             Pointer to the BIOS region stored in memory.

  @retval      EFI_SUCCESS            The BIOS region was retrieved.
  @retval      EFI_OUT_OF_RESOURCES   Could not create a HOB to store the pointer.
  @retval      Others                 An error occurred retrieving CSE Layout pointers,
                                      or reading from UFS.
**/
EFI_STATUS
EFIAPI
GetBiosRegion (
  IN OUT VOID           **BiosRegion
  )
{
  EFI_STATUS            Status;
  VOID                  *GuidHobPtr;
  CSE_LAYOUT_POINTERS   *CseLayoutPointers;

  if (BiosRegion == NULL) {
    DEBUG ((DEBUG_ERROR, "BiosRegion pointer is NULL!\n"));
    return EFI_INVALID_PARAMETER;
  }

  //Default to NULL to allow failure-checking from caller
  *BiosRegion = NULL;
  Status = EFI_SUCCESS;

  //Check if data has already been retrieved
  GuidHobPtr = GetFirstGuidHob (&gIfwiBiosRegionPointerHobGuid);

  if (GuidHobPtr != NULL) {
    //Retrieve a pointer to the data
    *BiosRegion = *((VOID **) GET_GUID_HOB_DATA (GuidHobPtr));
  } else {
    //Retrieve CSE Layout Pointers to find BIOS region location
    Status = GetCseLayoutPointers(&CseLayoutPointers);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //Read the BIOS region from UFS
    DEBUG ((DEBUG_INFO, "Lbp4Offset = 0x%08X\n", CseLayoutPointers->Lbp4Offset));
    DEBUG ((DEBUG_INFO, "Lbp4Size   = 0x%08X\n", CseLayoutPointers->Lbp4Size));
    Status = ReadFromUfs (
              BiosRegion,
              CseLayoutPointers->Lbp4Offset,
              CseLayoutPointers->Lbp4Size
              );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //Store the pointer in a HOB
    GuidHobPtr = BuildGuidDataHob (
                  &gIfwiBiosRegionPointerHobGuid,
                  BiosRegion,
                  sizeof (VOID *)
                  );
    if (GuidHobPtr == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    if (*BiosRegion != *((VOID **) GuidHobPtr)) {
      DEBUG ((DEBUG_ERROR, "BIOS REGION POINTER MISMATCH!\n"));
    }
  }

  return Status;
}

/**
  Returns an Entry of a specific Type from the provided BPDT.

  @param[in]   Type                   Entry type to return.
  @param[in]   Bpdt                   Pointer to a BPDT.
  @param[out]  BpdtEntry              Pointer to a BPDT Entry.

  @retval      EFI_SUCCESS            The BPDT Entry was found.
  @retval      EFI_NOT_FOUND          The BPDT Entry was not found.
  @retval      Others                 An error occurred.
**/
EFI_STATUS
EFIAPI
GetEntryFromBpdt (
  IN  BPDT_ENTRY_TYPE   Type,
  IN  BPDT_HEADER       *Bpdt,
  OUT BPDT_ENTRY        **Entry
  )
{
  UINTN                 Index;
  BPDT_ENTRY            *EntryList;

  if (Type >= BpdtMaxType) {
    DEBUG((DEBUG_ERROR, "BPDT Entry Type %d is greater than max type %d!\n", Type, BpdtMaxType-1));
    return EFI_INVALID_PARAMETER;
  }

  if ((Bpdt == NULL) || (Entry == NULL)) {
    DEBUG((DEBUG_ERROR, "Parameter is NULL!\n"));
    return EFI_INVALID_PARAMETER;
  }

  //Default to NULL to allow failure-checking from caller
  *Entry = NULL;

  if (Bpdt->Signature != BPDT_SIGN_GREEN) {
    DEBUG ((DEBUG_INFO, "Signature of BPDT Header is invalid - stop parsing\n"));
    return EFI_INVALID_PARAMETER;
  }

  //Walk BPDT Entry List
  EntryList = (BPDT_ENTRY *) (Bpdt + 1);
  for (Index = 0; Index < Bpdt->DescriptorCount; Index++) {
    *Entry = &EntryList[Index];
    if ((BPDT_ENTRY_TYPE) (*Entry)->Type == Type) {
      DEBUG ((DEBUG_INFO, "Entry Type %d Offset = 0x%08X\n", Type, (*Entry)->SubPartitionOffset));
      DEBUG ((DEBUG_INFO, "Entry Type %d Size   = 0x%08X\n", Type, (*Entry)->SubPartitionSize));
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Populates a SUB_PARTITION_DATA struct from the provided BPDT & EntryType.

  @param[in]      Type                Entry type to return.
  @param[in]      Bpdt                Pointer to a BPDT.
  @param[in, out] PayloadData         Pointer to SUB_PARTITION_DATA struct.

  @retval      EFI_SUCCESS            The BPDT Entry was found.
  @retval      EFI_NOT_FOUND          The BPDT Entry was not found.
  @retval      Others                 An error occurred.
**/
EFI_STATUS
EFIAPI
GetSubPartitionData (
  IN      BPDT_ENTRY_TYPE     Type,
  IN      BPDT_HEADER         *Bpdt,
  IN OUT  SUB_PARTITION_DATA  *PayloadData
  )
{
  EFI_STATUS            Status;
  BPDT_ENTRY            *BpdtEntry;

  if ((Bpdt == NULL) || (PayloadData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetEntryFromBpdt (Type, Bpdt, &BpdtEntry);
  if (EFI_ERROR (Status) || BpdtEntry == NULL) {
    return EFI_NOT_FOUND;
  }
  PayloadData->Address = (VOID *)(UINTN) ((UINTN) Bpdt + BpdtEntry->SubPartitionOffset);
  PayloadData->Size = BpdtEntry->SubPartitionSize;

  return Status;
}

