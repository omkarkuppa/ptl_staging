/** @file
  This file includes all definitions needed to load Green MRC payloads.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#ifndef _GreenMrcPayload_h_
#define _GreenMrcPayload_h_
#include "CMrcInterface.h"  // for MrcParameters

#ifndef MDEPKG_NDEBUG
#define MRC_GREEN_PAYLOAD_END 17
#else // Release
#define MRC_GREEN_PAYLOAD_END 17
#endif
#define MRC_FAST_PAYLOAD_START 0
#define MRC_FAST_PAYLOAD_END MRC_GREEN_PAYLOAD_END
#define MRC_UNCOMPRESSED_CHUNK_SIZE 16384
#if (MRC_UNCOMPRESSED_CHUNK_SIZE == 0)
#define DECOMP_BUFFER_SIZE 1
#else
#define DECOMP_BUFFER_SIZE MRC_UNCOMPRESSED_CHUNK_SIZE
#endif

typedef struct {
  UINT8 Tag[8];
  MrcVersion Version;
  UINT32 ChunkSize; // 0 = uncompressed
} MRC_PAYLOAD_HEADER;

typedef struct {
  UINT32 Offset;
  UINT32 Address;
  UINT32 Size;
} MRC_PAYLOAD_SECTION;

extern MRC_PAYLOAD_SECTION gUcPayloadSections[];
extern UINT32 gUcMrcDataAddress;
extern UINT32 gUcCommBufferAddress;
extern UINT32 gUcExtInputsAddress;

#endif // _GreenMrcPayload_h_
