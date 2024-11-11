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
  size_t          Size
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

#define LZMA_HEADER_SIZE  (LZMA_PROPS_SIZE + 8)

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
  UINT32  DecodedSize;
  INTN    Index;

  /* Parse header */
  DecodedSize = 0;
  // 32 bit is plenty, ignore the other 4 bytes of size data
  for (Index = LZMA_PROPS_SIZE + 3; Index >= LZMA_PROPS_SIZE; Index--) {
    //DecodedSize = LShiftU64 (DecodedSize, 8) + EncodedData[Index];
    DecodedSize = (DecodedSize << 8) + EncodedData[Index];
  }

  return DecodedSize;
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

  DecodedBufSize  = (SizeT)GetDecodedSizeOfBuf ((UINT8 *)Source);
  if (DecodedBufSize > DestinationSize) {
    return mrcWrongInputParameter; // Output buffer too small
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
