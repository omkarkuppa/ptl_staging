/** @file
  Definitions of registers for Sndw Cadence controllers

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

#ifndef __SNDW_CADENCE_REGS_H__
#define __SNDW_CADENCE_REGS_H__

#define R_SNDW_MEM_CONFIG                              0x4100
#define B_SNDW_MEM_CONFIG_MODE_AHB                     BIT3
#define B_SNDW_MEM_CONFIG_OM_NORMAL                    (BIT2 | BIT1 | BIT0)
#define V_SNDW_MEM_CONFIG_OM_RESET_VALUE               (BIT2 | BIT0)
#define R_SNDW_MEM_CTRL                                0x4104
#define B_SNDW_MEM_CTRL_BLOCKWAKEUP                    BIT0
#define B_SNDW_MEM_CTRL_CLOCKSTOPCLEAR                 BIT2
#define R_SNDW_MEM_CMDCTRL                             0x4108
#define R_SNDW_MEM_PHYCTRL                             0x411C
#define R_SNDW_MEM_INTSET                              0x414C

#define R_SNDW_MEM_SSPSTAT                             0x10C
#define R_SNDW_MEM_FRAMESHAPE                          0x110
#define R_SNDW_MEM_FRAMESHAPEINIT                      0x114
#define R_SNDW_MEM_CONFIGUPDATE                        0x118
#define B_SNDW_MEM_CONFIGUPDATE_UPDATE_DONE            BIT0

#define R_SNDW_MEM_STAT                                0x140
#define B_SNDW_MEM_STAT_TXE                            BIT1
#define B_SNDW_MEM_STAT_TXE_FIFO_EMPTY                 BIT1
#define B_SNDW_MEM_STAT_RXNE                           BIT3
#define B_SNDW_MEM_STAT_RXNE_FIFO_EMPTY                BIT3
#define R_SNDW_MEM_INTSTAT                             0x144
#define R_SNDW_MEM_INTMASK                             0x148
#define B_SNDW_MEM_INTMASK_RXNE                        BIT3
#define B_SNDW_MEM_INTMASK_RXNE_FIFO_EMPTY             BIT3

#define R_SNDW_MEM_PERIPHERALSTAT                      0x150
#define B_SNDW_MEM_PERIPHERALSTAT_STATUS(x)            ((BIT1 | BIT0) << (x * 2))
#define R_SNDW_MEM_PERIPHERALINTSTAT                   0x154
#define B_SNDW_MEM_PERIPHERALINTSTAT_STATUS(x)         ((BIT1 | BIT0) << (x * 2))

#define R_SNDW_MEM_FIFOSTAT                            0x17C
#define B_SNDW_MEM_FIFOSTAT_AVAIL                      (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define N_SNDW_MEM_FIFOSTAT_AVAIL                      0
#define B_SNDW_MEM_FIFOSTAT_FREE                       (BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_SNDW_MEM_FIFOSTAT_FREE                       8
#define R_SNDW_MEM_COMMAND                             0x4180

#define SNDW_MAX_PERIPHERAL_NUMBER                     11

#define R_PCP_DevNumber                                0x46
#define R_PCP_DevId_0                                  0x50
#define R_PCP_DevId_1                                  0x51
#define R_PCP_DevId_2                                  0x52
#define R_PCP_DevId_3                                  0x53
#define R_PCP_DevId_4                                  0x54
#define R_PCP_DevId_5                                  0x55

enum SNDW_PERIPHERAL_STATUS {
  SndwPeripheralNotPresent   = 0,
  SndwPeripheralAttachedOk   = 1,
  SndwPeripheralAlert        = 2,
  SndwPeripheralReserved     = 3
};

#endif /** __SNDW_CADENCE_REGS_H__ **/
