/** @file
  AmtReadyToBoot Protocol definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef _AMT_READY_TO_BOOT_PROTOCOL_H_
#define _AMT_READY_TO_BOOT_PROTOCOL_H_

///
/// AMT Ready to Boot Protocol
/// This protocol performs all Management Engine task
///

#define AMT_READY_TO_BOOT_PROTOCOL_REVISION  1

typedef struct _AMT_READY_TO_BOOT_PROTOCOL  AMT_READY_TO_BOOT_PROTOCOL;

/**
  Signal an event for Amt ready to boot.

  Any function registers to this protocol will be executed in ME ready to boot callback function.
**/
typedef
VOID
(EFIAPI *AMT_READY_TO_BOOT_PROTOCOL_SIGNAL) (
  VOID
  );

/**
  AMT Ready to Boot Protocol
  The interface functions are for Performing all Management Engine task
**/
struct _AMT_READY_TO_BOOT_PROTOCOL {
  UINT8                             Revision; ///< Revision for the protocol structure
  AMT_READY_TO_BOOT_PROTOCOL_SIGNAL Signal;   ///< Performs all Management Engine task
};

extern EFI_GUID gAmtReadyToBootProtocolGuid;

#endif
