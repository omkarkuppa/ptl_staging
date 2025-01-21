/** @file
  Update setup formset to Advanced menu.

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

#ifndef SETUP_UPDATE_H
#define SETUP_UPDATE_H

#include "SetupPrivate.h"

typedef struct _HII_CONFIG_PCD {
  UINT32    Formid : 16; /* The id of form to display formset under advanced menu. */
  UINT32    Order  : 8;  /* Formset display order. */
  UINT32    Rsvd1  : 7;  /* Reserved */
  UINT32    Show   : 1;  /* Whether to show formset, 1 - show, 0 - hide. */
} HII_CONFIG_PCD;

typedef struct _HII_GOTO_RECORD {
  EFI_STRING_ID    Token;       /* Hii entry formset title. */
  EFI_STRING_ID    TokenHelp;   /* Hii entry formset help. */
  EFI_GUID         FormsetGuid; /* Hii entry formset guid. */
  UINT8            Order;       /* Hii entry display order.*/
  LIST_ENTRY       GotoEntry;
} HII_GOTO_RECORD;

typedef struct _HII_FORM_RECORD {
  UINT16             Formid;      /* The id of form to display Hii entry under advanced menu. */
  HII_GOTO_RECORD    *GotoRecord;
  LIST_ENTRY         HiiFormEntry;
} HII_FORM_RECORD;

#define HII_GOTO_RECORD_FROM_LINK(link)  BASE_CR (link, HII_GOTO_RECORD, GotoEntry)
#define HII_FORM_RECORD_FROM_LINK(link)  BASE_CR (link, HII_FORM_RECORD, HiiFormEntry)

EFI_GUID  AdvancedGuid = ADVANCED_FORM_SET_GUID;

#define ADVANCED_QUESTION_OFFSET 0xF000
#endif
