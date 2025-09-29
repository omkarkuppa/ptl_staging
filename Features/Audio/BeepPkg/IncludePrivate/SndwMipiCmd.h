/** @file
  Header file that contains definitions for the MIPI standard commands

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

@par specification reference:
**/

#ifndef _SNDW_MIPI_CMD_H_
#define _SNDW_MIPI_CMD_H_

enum SNDW_COMMAND_TYPE {
  SndwCmdPing      = 0,
  SndwCmdReserved  = 1,
  SndwCmdRead      = 2,
  SndwCmdWrite     = 3,
  SndwCmdInvalid   = 4
};

typedef union {
  UINT32 Command;
  struct {
    UINT32 Reserved0     :  8;
    UINT32 SSync         :  8;
    UINT32 Reserved1     :  1;
    UINT32 DSync         :  4;
    UINT32 Reserved2     :  5;
    UINT32 Ssp           :  1;
    UINT32 Reserved3     :  1;
    UINT32 OpCode        :  3;
    UINT32 SspTag        :  1;
  } TxPing;
  struct {
    UINT32 RegData       :  8;
    UINT32 RegAddr       :  16;
    UINT32 DeviceAddress :  4;
    UINT32 OpCode        :  3;
    UINT32 SspTag        :  1;
  } TxWrite;
  struct {
    UINT32 Reserved0     :  8;
    UINT32 RegAddr       :  16;
    UINT32 DeviceAddress :  4;
    UINT32 OpCode        :  3;
    UINT32 SspTag        :  1;
  } TxRead;
  struct {
    UINT32 Reserved0     :  24;
    UINT32 DeviceAddress :  4;
    UINT32 OpCode        :  3;
    UINT32 SspTag        :  1;
  } Tx;
  struct {
    UINT32 Ack           :  1;
    UINT32 Nak           :  1;
    UINT32 Reserved0     :  2;
    UINT32 OpCode        :  3;
    UINT32 Reserved1     :  1;
    UINT32 Data          :  8;
    UINT32 Reserved2     : 16;
  } Rx;
} SNDW_COMMAND;

typedef struct {
  UINT32    LinearAddress;  // 32-bits linear address
  UINT8     Data;           // 8-bits data
} SDCA_COMMAND;

#endif
