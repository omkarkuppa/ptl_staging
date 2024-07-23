/** @file
  Definitions for HECI driver

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

@par Specification Reference:
**/
#ifndef _HECI_CORE_H
#define _HECI_CORE_H

//
// Timeout values based on HPET
//
#define HECI_WAIT_DELAY             1000       ///< 1ms timeout for IO delay
#define HECI_INIT_TIMEOUT           15000000   ///< 15sec timeout in microseconds
#define HECI_READ_TIMEOUT           5000000    ///< 5sec timeout in microseconds
#define HECI_SEND_TIMEOUT           5000000    ///< 5sec timeout in microseconds
#define HECI_CB_OVERFLOW            0xFFFFFFFF ///< Circular buffer overflow

///
/// Header for all HECI message message placed in an Intel MEI circular buffer
///
typedef union {
  UINT32  Data;
  struct {
    /**
      This is the logical address of the Intel ME client of the message. This address is assigned
      during ME firmware initialization.
    **/
    UINT32  MeAddress       : 8;
    /**
      This is the logical address of the Host client of the message. This address is assigned
      when the host client registers itself with the Host MEI driver.
    **/
    UINT32  HostAddress     : 8;
    /**
      This is the message length in bytes, not including the MEI_MESSAGE_HEADER. A value of 0
      indicates no additional bytes are part of the message.
    **/
    UINT32  Length          : 9;
    UINT32  Reserved        : 6;
    UINT32  MessageComplete : 1; ///< Indicates the last message of the list
  } Fields;
} HECI_MESSAGE_HEADER;

#endif // _HECI_CORE_H
