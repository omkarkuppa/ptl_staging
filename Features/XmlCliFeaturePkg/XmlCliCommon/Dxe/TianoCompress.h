/** @file
  Internal include file for Tiano Decompress Libary.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2006 Intel Corporation.

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

#ifndef __TIANO_COMPRESS_H__
#define __TIANO_COMPRESS_H__

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDecompressLib.h>

#define BITBUFSIZ 32
#define MAXMATCH  256
#define THRESHOLD 3
#define CODE_BIT  16
#define BAD_TABLE - 1

typedef INT32 NODE;

//
// C: Char&Len Set; P: Position Set; T: exTra Set
//
#define NC      (0xff + MAXMATCH + 2 - THRESHOLD)
#define CBIT    9
#define MAXPBIT 5
#define TBIT    5
#define MAXNP   ((1U << MAXPBIT) - 1)
#define NT      (CODE_BIT + 3)
#if NT > MAXNP
#define NPT NT
#else
#define NPT MAXNP
#endif

typedef struct {
  UINT8   *mSrcBase;  // Starting address of compressed data
  UINT8   *mDstBase;  // Starting address of decompressed data
  UINT32  mOutBuf;
  UINT32  mInBuf;

  UINT16  mBitCount;
  UINT32  mBitBuf;
  UINT32  mSubBitBuf;
  UINT16  mBlockSize;
  UINT32  mCompSize;
  UINT32  mOrigSize;

  UINT16  mBadTableFlag;

  UINT16  mLeft[2 * NC - 1];
  UINT16  mRight[2 * NC - 1];
  UINT8   mCLen[NC];
  UINT8   mPTLen[NPT];
  UINT16  mCTable[4096];
  UINT16  mPTTable[256];

  //
  // The length of the field 'Position Set Code Length Array Size' in Block Header.
  // For EFI 1.1 de/compression algorithm, mPBit = 4
  // For Tiano de/compression algorithm, mPBit = 5
  //
  UINT8   mPBit;
} SCRATCH_DATA;

//
// Function Prototypes
//

EFI_STATUS
GetFileContents (
  IN char    *InputFileName,
  OUT UINT8   *FileBuffer,
  OUT UINT32  *BufferLength
  );

STATIC
VOID
PutDword(
  IN UINT32 Data
  );

STATIC
EFI_STATUS
AllocateMemory (
  VOID
  );

STATIC
VOID
FreeMemory (
  VOID
  );

STATIC
VOID
InitSlide (
  VOID
  );

STATIC
NODE
Child (
  IN NODE   NodeQ,
  IN UINT8  CharC
  );

STATIC
VOID
MakeChild (
  IN NODE  NodeQ,
  IN UINT8 CharC,
  IN NODE  NodeR
  );

STATIC
VOID
Split (
  IN NODE Old
  );

STATIC
VOID
InsertNode (
  VOID
  );

STATIC
VOID
DeleteNode (
  VOID
  );

STATIC
VOID
GetNextMatch (
  VOID
  );

STATIC
EFI_STATUS
Encode (
  VOID
  );

STATIC
VOID
CountTFreq (
  VOID
  );

STATIC
VOID
WritePTLen (
  IN INT32 Number,
  IN INT32 nbit,
  IN INT32 Special
  );

STATIC
VOID
WriteCLen (
  VOID
  );

STATIC
VOID
EncodeC (
  IN INT32 Value
  );

STATIC
VOID
EncodeP (
  IN UINT32 Value
  );

STATIC
VOID
SendBlock (
  VOID
  );

STATIC
VOID
Output (
  IN UINT32 c,
  IN UINT32 p
  );

STATIC
VOID
HufEncodeStart (
  VOID
  );

STATIC
VOID
HufEncodeEnd (
  VOID
  );

STATIC
VOID
MakeCrcTable (
  VOID
  );


STATIC
VOID
PutBits (
  IN INT32  Number,
  IN UINT32 Value
  );

STATIC
INT32
FreadCrc (
  OUT UINT8 *Pointer,
  IN  INT32 Number
  );

STATIC
VOID
InitPutBits (
  VOID
  );

STATIC
VOID
CountLen (
  IN INT32 Index
  );

STATIC
VOID
MakeLen (
  IN INT32 Root
  );

STATIC
VOID
DownHeap (
  IN INT32 Index
  );

STATIC
VOID
MakeCode (
  IN  INT32       Number,
  IN  UINT8 Len[  ],
  OUT UINT16 Code[]
  );

STATIC
INT32
MakeTree (
  IN  INT32            NParm,
  IN  UINT16  FreqParm[],
  OUT UINT8   LenParm[ ],
  OUT UINT16  CodeParm[]
  );

/**
  Read NumOfBit of bits from source into mBitBuf

  Shift mBitBuf NumOfBits left. Read in NumOfBits of bits from source.

  @param  Sd        The global scratch data
  @param  NumOfBits The number of bits to shift and read.

**/
VOID
FillBuf (
  IN  SCRATCH_DATA  *Sd,
  IN  UINT16        NumOfBits
  );

/**
  Get NumOfBits of bits out from mBitBuf

  Get NumOfBits of bits out from mBitBuf. Fill mBitBuf with subsequent
  NumOfBits of bits from source. Returns NumOfBits of bits that are
  popped out.

  @param  Sd        The global scratch data.
  @param  NumOfBits The number of bits to pop and read.

  @return The bits that are popped out.

**/
UINT32
GetBits (
  IN  SCRATCH_DATA  *Sd,
  IN  UINT16        NumOfBits
  );

/**
  Creates Huffman Code mapping table according to code length array.

  Creates Huffman Code mapping table for Extra Set, Char&Len Set
  and Position Set according to code length array.

  @param  Sd        The global scratch data
  @param  NumOfChar Number of symbols in the symbol set
  @param  BitLen    Code length array
  @param  TableBits The width of the mapping table
  @param  Table     The table

  @retval  0 OK.
  @retval  BAD_TABLE The table is corrupted.

**/
UINT16
MakeTable (
  IN  SCRATCH_DATA  *Sd,
  IN  UINT16        NumOfChar,
  IN  UINT8         *BitLen,
  IN  UINT16        TableBits,
  OUT UINT16        *Table
  );

/**
  Decodes a position value.

  Get a position value according to Position Huffman Table.

  @param  Sd the global scratch data

  @return The position value decoded.

**/
UINT32
DecodeP (
  IN  SCRATCH_DATA  *Sd
  );

/**
  Reads code lengths for the Extra Set or the Position Set.

  Read in the Extra Set or Pointion Set Length Arrary, then
  generate the Huffman code mapping for them.

  @param  Sd      The global scratch data.
  @param  nn      Number of symbols.
  @param  nbit    Number of bits needed to represent nn.
  @param  Special The special symbol that needs to be taken care of.

  @retval  0 OK.
  @retval  BAD_TABLE Table is corrupted.

**/
UINT16
ReadPTLen (
  IN  SCRATCH_DATA  *Sd,
  IN  UINT16        nn,
  IN  UINT16        nbit,
  IN  UINT16        Special
  );

/**
  Reads code lengths for Char&Len Set.

  Read in and decode the Char&Len Set Code Length Array, then
  generate the Huffman Code mapping table for the Char&Len Set.

  @param  Sd the global scratch data

**/
VOID
ReadCLen (
  SCRATCH_DATA  *Sd
  );

RETURN_STATUS
UefiTianoDecompress (
  IN CONST VOID  *Source,
  IN OUT VOID    *Destination,
  IN OUT VOID    *Scratch,
  IN UINT32      Version
  );

#endif  // __TIANO_COMPRESS_H__
