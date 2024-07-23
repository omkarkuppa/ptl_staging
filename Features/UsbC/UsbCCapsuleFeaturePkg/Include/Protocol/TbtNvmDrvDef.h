/** @file
  Device communication interface definition.

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

#ifndef _TBT_RETIMER_NVM_DEFINE_
#define _TBT_RETIMER_NVM_DEFINE_

//
// TBT_HOST_ROUTER communication  (TbtNvmRetimerDrvHelpers.c)
//
// TBT CIO device config space RegNum
#define ROUTER_CMD_ADDR                26   //Opcode
#define ROUTER_METADATA_ADDR           25   //Metadata
#define ROUTER_DATA_ADDR                9   //Data

// TBT CIO device config space sends command
#define OPCODE_NVM_WRITE_CMD         0x20
#define OPCODE_NVM_AUTH_CMD          0x21
#define OPCODE_NVM_READ_CMD          0x22
#define OPCODE_NVM_SETOFFSET_CMD     0x23
#define OPCODE_NVM_GETSECTORSIZE_CMD 0x25

// TBT CIO device config space data bit check
#define TBT_CMD_VALID              BIT(31)
#define TBT_CMD_NOTSUPPORTED       BIT(30)
#define TBT_METADATA_VALID         BIT(24)

//
// TBT_RETIMER communication   (TbtNvmRetimerDrvHelpers.c)
//
// IECS RegNum
#define IECS_CMD_ADDR                   8
#define IECS_METADATA_ADDR              9
#define IECS_MSG_OUT_RDATA             18

// IECS command
// The commands below are 4cc encoded
#define IECS_BOPS_CMD          0x53504f42 /* NVM_SET_OFFSET */
#define IECS_BLKW_CMD          0x574b4c42 /* NVM_BLOCK_WRITE */
#define IECS_AUTH_CMD          0x48545541 /* NVM_AUTH_WRITE */
#define IECS_PCYC_CMD          0x43594350 /* ? Not in CIO spec */
#define IECS_AFRR_CMD          0x52524641 /* NVM_READ */


// IECS read data check
// 4cc error indications
#define IECS_USB4_CMD_NOTSUPPORTED 0x444D4321
#define IECS_USB4_CMD_ERR          0x20525245

// This is number of accesses to remote IECS CMD before declaring timeout
#define TBT_TOTAL_OF_ACCESSES_TO_CMD_UNTIL_TIMEOUT 2000

//
// TBT IECS command                     (TbtNvmDrvRetimerThruHr.c)
#define TBT_IECS_CMD_LSUP      0x5055534c /* SET_INBOUND_LSTX */
#define TBT_IECS_CMD_USUP      0x50555355 /* UNSET_INBOUND_LSTX */ /* WA to mitigate no response issue */
#define TBT_IECS_CMD_ENUM      0x4d554e45 /* ENUMERATE_RETIMERS */
#define TBT_IECS_CMD_LSEN      0x4e45534c /* RETIMER_OFFLINE_MODE */

#define TBT_IECS_MAILBOX_LEGTH_IN_DW 16

// TBT LC data status check.
#define TBT_LC_CMD_SUCCESS 0x0

// TBT IECS command                   (TbtNvmDrvRetimerThruHr.c)
#define TBT_TOTAL_ENUM_ACCESSES 4
#define TBT_TOTAL_LSUP_ACCESSES 3
#define TBT_WAIT_TIME_BETWEEN_ENUM_ACCESSES   10000   // time in us, 10ms
#define TBT_WAIT_TIME_BEFORE_NEXT_IECS_ACCESS 100     // time in us
#define TBT_WAIT_TIME_BEFORE_NEXT_MSG_OUT_ACCESS 50   // time in us
#define TBT_TOTAL_WAIT_TIME_UNTIL_TIMEOUT 200000      // 200ms in us

#define TBT_TOTAL_ACCESSES_WHILE_WAIT_FOR_IECS  \
 TBT_TOTAL_WAIT_TIME_UNTIL_TIMEOUT / TBT_WAIT_TIME_BEFORE_NEXT_IECS_ACCESS
#define TBT_TOTAL_ACCESSES_WHILE_WAIT_FOR_MSG_OUT \
 TBT_TOTAL_WAIT_TIME_UNTIL_TIMEOUT / TBT_WAIT_TIME_BEFORE_NEXT_MSG_OUT_ACCESS

#define TBT_TOTAL_RETRIES_ON_MSG_OUT 10

// TBT CIO adapter config space       (TbtNvmDrvRetimerThruHr.c)
#define TBT_MSG_OUT_CMD_VALID       BIT(31)
#define TBT_MSG_OUT_ATCT1_LT0       BIT(30)
#define TBT_MSG_OUT_WR_RD_OFFSET    23
#define TBT_MSG_OUT_TIMEOUT         BIT(25)
#define TBT_MSG_OUT_INVALID         BIT(26)
#define TBT_MSG_OUT_LENGTH_OFFSET   16
#define TBT_MSG_OUT_REG_ADDR_OFFSET 8
#define TBT_MSG_OUT_START_CT        BIT(6)
#define TBT_MSG_OUT_RETIMER_INDEX_OFFSET 1
#define TBT_MSG_OUT_CMD             BIT(0)

// TBT CIO adapter config space offset  (TbtNvmDrvRetimerThruHr.c)
#define TBT_USB4_PORT_CAPABILITY_LENGTH_OFFSET          8
#define TBT_USB4_PORT_CAPABILITY_TARGET_OFFSET         16
#define TBT_USB4_PORT_CAPABILITY_RETIMER_INDEX_OFFSET  20
#define TBT_USB4_PORT_CAPABILITY_WnR_OFFSET            24
#define TBT_USB4_PORT_CAPABILITY_TIMEOUT_OFFSET        25
#define TBT_USB4_PORT_CAPABILITY_RC_OFFSET             26
#define TBT_USB4_PORT_CAPABILITY_PND_OFFSET            31

#define PORT_CS_1 1
#define PORT_CS_2 2

#define ROUTER_CS_26 26

#define TBT_MSG_OUT_DEFAULT_VALS TBT_MSG_OUT_CMD_VALID | \
                                 TBT_MSG_OUT_ATCT1_LT0 | \
                                 TBT_MSG_OUT_START_CT  | \
                                 TBT_MSG_OUT_CMD

//
// Update NVM Firmware define
//
#define TBT_NVM_MAX_DWS_TO_WRITE 16

#define TBT_TOTAL_NUM_OF_RECOVERIES_DURING_IMAGE_WRITE 1

//
// Authenticate define
//
// Success status from LC
#define TBT_AUTH_SUCCESS 1

#define TBT_TIME_TO_WAIT_FOR_AUTH_US 1000 * 1000 * 5              // 5s

//
// Read NVM version define
//
#define TBT_NVM_VERSION_OFFSET_DW 2

#define TBT_NVM_VERSION_MASK MASK(23, 0)

// Offset in DROM to Model Revision fields
#define TBT_TBT3_DROM_MODEL_REVISION_OFFSET 0x14
#define TBT_SIMPLIFIED_DROM_MODEL_REVISION_OFFSET 0x4 // It is actually at offset 0x5 but we need to read from DWORD aligned address.

//
// Proxy define (Unused)
//
#define GET_NVM_SECTOR_SIZE    0x53534E47 /* GET_NVM_SECTOR_SIZE */
#define NVM_SET_OFFSET         0x53504f42 /* NVM_SET_OFFSET */
#define NVM_BLOCK_WRITE        0x574b4c42 /* NVM_BLOCK_WRITE */
#define NVM_AUTH_WRITE         0x48545541 /* NVM_AUTH_WRITE */
#define NVM_READ               0x52524641 /* NVM_READ */

#endif  // _TBT_RETIMER_NVM_DEFINE_
