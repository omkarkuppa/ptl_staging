/** @file
  LZMA Decompress interfaces
  @copyright
  Copyright (c) 2024, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef DECOMPRESS_LIB_H
#define DECOMPRESS_LIB_H
#include "CMrcTypes.h"

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
  );

#endif // DECOMPRESS_LIB_H
