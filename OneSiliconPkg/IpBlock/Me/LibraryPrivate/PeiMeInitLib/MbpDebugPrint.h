/** @file
  ME Payload Data Debug Print Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#ifndef _MBP_DEBUG_PRINT_H_
#define _MBP_DEBUG_PRINT_H_

#include <MeBiosPayloadHob.h>

/**
  Print MbpHob data

  @param[in] MbpHob             Pointer to Mbp Hob

**/
VOID
PrintMbpData (
  IN ME_BIOS_PAYLOAD_HOB *MbpHob
  );

#endif
