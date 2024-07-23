/** @file
  PEI HDA Verb Table database declaration
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _PCH_HDA_VERB_TABLES_H_
#define _PCH_HDA_VERB_TABLES_H_

typedef struct {
  UINT8                Size;
  HDAUDIO_VERB_TABLE*  HdaVerbTable[];
} HDA_VERB_TABLE_DATABASE;

extern HDA_VERB_TABLE_DATABASE HdaVerbTableDataBase;
extern HDAUDIO_VERB_TABLE      HdaVerbTableAlc245NoDmic;
extern HDAUDIO_VERB_TABLE      HdaVerbTableAlc245X4;

#endif // _PCH_HDA_VERB_TABLES_H_
