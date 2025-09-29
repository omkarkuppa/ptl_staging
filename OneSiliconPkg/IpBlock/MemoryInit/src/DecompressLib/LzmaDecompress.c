/** @file
  LZMA Decompress interfaces
  @copyright
  Copyright (c) 2024, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "DecompressLib.h"
#include "LzmaDecompress.h"

#define SCRATCH_BUFFER_REQUEST_SIZE 16384 // Round up to 16k, *not* chunk dependent!

/**
  Allocation routine used by LZMA decompression.

  @param P                Pointer to the ISzAlloc instance
  @param Size             The size in bytes to be allocated

  @return The allocated pointer address, or NULL on failure
**/
VOID *
SzAlloc (
  CONST ISzAlloc  *P,
  SizeT           Size
  )
{
  VOID              *Addr;
  ISzAllocWithData  *Private;

  Private = (ISzAllocWithData *)P;

  if (Private->BufferSize >= Size) {
    Addr                 = Private->Buffer;
    Private->Buffer      = (VOID *)((UINT8 *)Addr + Size);
    Private->BufferSize -= Size;
    return Addr;
  } else {
    return NULL;
  }
}

#define LZMA_HEADER_SIZE  (LZMA_PROPS_SIZE + 8) // 5 bytes props + 8 bytes uncompressed size (LZMA SDK 25.00)

/**
  Get the size of the uncompressed buffer by parsing EncodeData header.

  @param EncodedData  Pointer to the compressed data.

  @return The size of the uncompressed buffer.
**/
UINT64
GetDecodedSizeOfBuf (
  UINT8  *EncodedData
  )
{
  UINT32 LowPart = 0;
  UINT32 HighPart = 0;
  
  // LZMA SDK 25.00: uncompressed size is 8 bytes, little-endian, after props
  // Process all 8 bytes but avoid 64-bit arithmetic to prevent MSVC runtime helpers
  
  // Build low 32 bits from bytes 0-3 (little-endian)
  LowPart = (UINT32)EncodedData[LZMA_PROPS_SIZE] |
            ((UINT32)EncodedData[LZMA_PROPS_SIZE + 1] << 8) |
            ((UINT32)EncodedData[LZMA_PROPS_SIZE + 2] << 16) |
            ((UINT32)EncodedData[LZMA_PROPS_SIZE + 3] << 24);
  
  // Build high 32 bits from bytes 4-7 (little-endian)  
  HighPart = (UINT32)EncodedData[LZMA_PROPS_SIZE + 4] |
             ((UINT32)EncodedData[LZMA_PROPS_SIZE + 5] << 8) |
             ((UINT32)EncodedData[LZMA_PROPS_SIZE + 6] << 16) |
             ((UINT32)EncodedData[LZMA_PROPS_SIZE + 7] << 24);
  
  // Check for "unknown size" marker (all bits set to 1)
  if (LowPart == 0xFFFFFFFF && HighPart == 0xFFFFFFFF) {
    // Return unknown size marker using union to avoid 64-bit operations
    union {
      UINT64 Full;
      struct {
        UINT32 Low;
        UINT32 High;
      } Parts;
    } UnknownSize;
    
    UnknownSize.Parts.Low = 0xFFFFFFFF;
    UnknownSize.Parts.High = 0xFFFFFFFF;
    return UnknownSize.Full;
  }
  
  // For firmware use, sizes > 4GB are unrealistic, return only low part
  // This avoids 64-bit operations while maintaining compatibility
  return (UINT64)LowPart;
}

/**
  Decompresses a Lzma compressed source buffer.

  Extracts decompressed data to its original form.
  If the compressed source data specified by Source is successfully decompressed
  into Destination, then mrcSuccess is returned.  If the compressed source data
  specified by Source is not in a valid compressed data format,
  then mrcWrongInputParameter is returned.

  @param  Source      The source buffer containing the compressed data.
  @param  SourceSize  The size of source buffer.
  @param  Destination The destination buffer to store the decompressed data
  @param  DestinationSize The size of the destination buffer.
  @param  BytesWritten The number of bytes written into the destination buffer.

  @retval  mrcSuccess Decompression completed successfully, and
                          the uncompressed buffer is returned in Destination.
  @retval  mrcWrongInputParameter
                          The source buffer specified by Source is corrupted
                          (not in a valid compressed format).
**/
MrcStatus
MrcDecompress (
  IN CONST VOID  *Source,
  IN UINTN       SourceSize,
  IN OUT VOID    *Destination,
  IN UINTN       DestinationSize,
  OUT UINTN      *BytesWritten
  )
{
  SRes              LzmaResult;
  ELzmaStatus       Status;
  SizeT             DecodedBufSize;
  SizeT             EncodedDataSize;
  ISzAllocWithData  AllocFuncs;
  UINT8 Scratch[SCRATCH_BUFFER_REQUEST_SIZE];

  *BytesWritten = 0;
  AllocFuncs.Functions.Alloc = SzAlloc;
  AllocFuncs.Functions.Free  = NULL;
  AllocFuncs.Buffer          = Scratch;
  AllocFuncs.BufferSize      = SCRATCH_BUFFER_REQUEST_SIZE;

  UINT64 HeaderDecodedSize = GetDecodedSizeOfBuf ((UINT8 *)Source);
  // Check for unknown size using same union approach to avoid 64-bit constants
  union {
    UINT64 Full;
    struct {
      UINT32 Low;
      UINT32 High;
    } Parts;
  } UnknownSizeCheck;
  
  UnknownSizeCheck.Parts.Low = 0xFFFFFFFF;
  UnknownSizeCheck.Parts.High = 0xFFFFFFFF;
  
  if (HeaderDecodedSize == UnknownSizeCheck.Full) {
    // LZMA header signals unknown size, use caller's expected size
    DecodedBufSize = (SizeT)DestinationSize;
  } else {
    DecodedBufSize = (SizeT)HeaderDecodedSize;
    if (DecodedBufSize > DestinationSize) {
      return mrcWrongInputParameter; // Output buffer too small
    }
  }
  EncodedDataSize = (SizeT)(SourceSize - LZMA_HEADER_SIZE);

  LzmaResult = LzmaDecode (
                 Destination,
                 &DecodedBufSize,
                 (Byte *)((UINT8 *)Source + LZMA_HEADER_SIZE),
                 &EncodedDataSize,
                 Source,
                 LZMA_PROPS_SIZE,
                 LZMA_FINISH_END,
                 &Status,
                 &(AllocFuncs.Functions)
                 );

  if (LzmaResult == SZ_OK) {
    *BytesWritten = DecodedBufSize;
    return mrcSuccess;
  } else {
    return mrcWrongInputParameter;
  }
}