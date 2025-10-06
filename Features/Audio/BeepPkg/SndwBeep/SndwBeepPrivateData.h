/** @file
  Header file that contains private data definitions for SoundWire Beep

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par specification reference:
**/

#ifndef __SNDW_BEEP_PRIVATE_DATA__
#define __SNDW_BEEP_PRIVATE_DATA__

#include <Beep.h>
#include <SndwAccess.h>

#define SNDW_BEEP_SIGNATURE SIGNATURE_32 ('s', 'd', 'w', 'b')

typedef struct {
  UINT32                          Signature;
  BEEP                            BeepApi;
  SNDW_ACCESS                     *SndwAccessApi;
  SNDW_CODEC_INFO                 SndwCodecInfo;
} SNDW_BEEP_CONTEXT;

#define SNDW_BEEP_CONTEXT_FROM_SNDW_BEEP_PPI_PROTOCOL(a)  CR (a, SNDW_BEEP_CONTEXT, BeepApi, SNDW_BEEP_SIGNATURE)

typedef enum {
  RltkAlc1308PartId = 0x1308,
  RltkAlc722PartId  = 0x722,
  SndwCodecPartIdInvalid
} SNDW_BEEP_CODEC_PART_ID;

#endif /** __SNDW_BEEP_PRIVATE_DATA__ **/
