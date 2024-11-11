/** @file
  LZMA Decompress interfaces
  @copyright
  Copyright (c) 2024, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef LZMA_DECOMPRESS_H
#define LZMA_DECOMPRESS_H

#include "7zTypes.h"
#include "LzmaDec.h"

typedef struct {
  ISzAlloc    Functions;
  VOID        *Buffer;
  UINTN       BufferSize;
} ISzAllocWithData;

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
  );

#define LZMA_HEADER_SIZE  (LZMA_PROPS_SIZE + 8)

/**
  Get the size of the uncompressed buffer by parsing EncodeData header.

  @param EncodedData  Pointer to the compressed data.

  @return The size of the uncompressed buffer.
**/
UINT64
GetDecodedSizeOfBuf (
  UINT8  *EncodedData
  );

#endif // LZMA_DECOMPRESS_H
