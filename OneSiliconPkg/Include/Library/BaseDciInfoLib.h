/** @file
  Base DCI Library header file

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

#ifndef _BASE_DCI_INFO_LIB_H_
#define _BASE_DCI_INFO_LIB_H_

#include <DciConfig.h>
#include <Register/DciRegs.h>

#define EXI_MAILBOX_REG_OFFSET(DCI_MAILBOX_REG)     R_DCI_PCR_EMB0 + (DCI_MAILBOX_REG * 4)

/**
  Get Dci Mailbox Address

  @param[in] Index    Indicate which DCI mailbox

  @retval Address of DCI mailbox
**/
UINT64
GetDciMailboxAddr (
  IN DCI_MAILBOX_REG              Index
  );

/**
  Return DCI host connection status

  @retval TRUE   DCI host is connected
  @retval FALSE  DCI host is not connected
**/
BOOLEAN
IsHostConnected (
  VOID
  );

#endif // _BASE_DCI_INFO_LIB_H_
