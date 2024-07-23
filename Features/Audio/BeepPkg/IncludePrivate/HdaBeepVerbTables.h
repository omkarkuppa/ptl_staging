/** @file
  Header file with HD-Audio verb table definitions

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

@par Specification Reference:
**/

#ifndef __HDA_BEEP_VERB_TABLES_H__
#define __HDA_BEEP_VERB_TABLES_H__

#pragma pack (push,1)

#define HDAUDIO_VERB_TABLE_INIT(Vid,Did,Rid,Sdi,...) \
{ \
  { Vid, Did, Rid, Sdi, (sizeof((UINT32[]){__VA_ARGS__})/sizeof(UINT32)) }, \
  (UINT32*)(UINT32[]){ __VA_ARGS__ } \
}

#define HDA_BEEP_VERB_TABLES_INIT(InitVerbTable,BeepOnVerbTable,BeepOffVerbTable) \
{ \
  InitVerbTable,   \
  BeepOnVerbTable, \
  BeepOffVerbTable \
}

typedef struct {
  UINT16  VendorId;             ///< Codec Vendor ID
  UINT16  DeviceId;             ///< Codec Device ID
  UINT8   RevisionId;           ///< Revision ID of the codec. 0xFF matches any revision.
  UINT8   SdiNum;               ///< SDI number, 0xFF matches any SDI.
  UINT16  DataDwords;           ///< Number of data DWORDs following the header.
} HDA_VERB_TABLE_HEADER;

typedef struct  {
  HDA_VERB_TABLE_HEADER  Header;
  UINT32                 *Data;
} HDAUDIO_VERB_TABLE;


typedef struct {
  HDAUDIO_VERB_TABLE     *BeepEnable;
  HDAUDIO_VERB_TABLE     *BeepOn;
  HDAUDIO_VERB_TABLE     *BeepOff;
} HDA_BEEP_VERB_TABLES;

#pragma pack (pop)


extern HDA_BEEP_VERB_TABLES HdaBeepVerbTablesAlc256;

#endif /** __HDA_BEEP_VERB_TABLES_H__ **/
