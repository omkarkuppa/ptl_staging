/** @file
  Header file for Tx/Rx ring configuration of USB4 host interface.

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

@par Specification
**/

#ifndef _USB4_HI_RING_H_
#define _USB4_HI_RING_H_

#define USB4_TX_RING_FULL(prod, cons, size)  ((((prod) + 1) % (size)) == (cons))
#define USB4_TX_RING_EMPTY(prod, cons)       ((prod) == (cons))
#define USB4_RX_RING_FULL(prod, cons)        ((prod) == (cons))
#define USB4_RX_RING_EMPTY(prod, cons, size) ((((cons) + 1) % (size)) == (prod))

#define USB4_TX_RING_POLL_US                     10
#define USB4_TX_RING_TOTAL_WAITS                 200
#define USB4_RX_RING_POLL_US                     50         // 50 us
#define USB4_RX_RING_TOTAL_WAIT_US               10000000   // 10s
#define USB4_RX_RING_TOTAL_WAITS                 (USB4_RX_RING_TOTAL_WAIT_US / USB4_RX_RING_POLL_US)
#define MAX_PACKETS_TO_RX_WHILE_WAIT_FOR_RESP    1000

//
// Register definition of Host Interface Programming interface
//
#define REG_TX_RING_BASE                      0x00000
#define REG_RX_RING_BASE                      0x08000
#define REG_RING_PHYS_LO_OFFSET               0
#define REG_RING_PHYS_HI_OFFSET               4
#define REG_RING_CONS_PROD_OFFSET             8
#define REG_RING_CONS_SHIFT                   0
#define REG_RING_CONS_MASK                    0x0000FFFF     // Bit[15:0]
#define REG_RING_PROD_SHIFT                   16
#define REG_RING_PROD_MASK                    0xFFFF0000     // Bit[31:16]
#define REG_RING_SIZE_OFFSET                  12
#define REG_RING_SIZE_SHIFT                   0
#define REG_RING_SIZE_MASK                    0x0000FFFF     // Bit[15:0]
#define REG_RING_BUF_SIZE_SHIFT               16
#define REG_RING_BUF_SIZE_MASK                0x0FFF0000     // Bit[27:16]

#define REG_TX_OPTIONS_BASE                   0x19800
#define REG_RX_OPTIONS_BASE                   0x29800
#define REG_RX_OPTS_MASK_OFFSET               4
#define REG_RX_OPTS_MASK_EOF_SHIFT            0
#define REG_RX_OPTS_MASK_EOF_MASK             0x0000FFFF     // Bit[15:0]
#define REG_RX_OPTS_MASK_SOF_SHIFT            16
#define REG_RX_OPTS_MASK_SOF_MASK             0xFFFF0000     // Bit[31:16]
#define REG_OPTS_RAW                          BIT30
#define REG_OPTS_VALID                        BIT31

#define DESC_ATTR_LEN_SHIFT                   0
#define DESC_ATTR_LEN_MASK                    0x00000FFF     // Bit[11:0]
#define DESC_ATTR_EOF_SHIFT                   12
#define DESC_ATTR_EOF_MASK                    0x0000F000     // Bit[15:12]
#define DESC_ATTR_SOF_SHIFT                   16
#define DESC_ATTR_SOF_MASK                    0x000F0000     // Bit[19:16]
#define DESC_ATTR_TX_ISOCH_DMA_EN             BIT20  // TX
#define DESC_ATTR_RX_CRC_ERR                  BIT20  // RX after use
#define DESC_ATTR_DESC_DONE                   BIT21
#define DESC_ATTR_REQ_STS                     BIT22  // TX and RX before use
#define DESC_ATTR_RX_BUF_OVRN_ERR             BIT22  // RX after use
#define DESC_ATTR_INT_EN                      BIT23
#define DESC_ATTR_OFFSET_SHIFT                24
#define DESC_ATTR_OFFSET_MASK                 0xFF000000     // Bit[31:24]

#endif
