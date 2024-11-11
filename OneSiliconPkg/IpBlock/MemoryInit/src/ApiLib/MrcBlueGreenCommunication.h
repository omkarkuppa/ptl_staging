/** @file
  This file includes all the data structures that facilitate Blue and Green
  MRC communication.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _MrcBlueGreenCommunication_h_
#define _MrcBlueGreenCommunication_h_

#include "CMrcTypes.h"
#ifdef XTENSA
#include "CGreenMrcCommonTypes.h"
#else
#include "CBlueMrcCommonTypes.h"
#endif

#define MRC_GREEN_LOG_SIZE                  0x100
#define MRC_BLUE_GREEN_COMM_CONTROLS_SIZE   20
#define MRC_BLUE_GREEN_COMM_LOG_SIZE_OFFSET 0
#define MRC_BLUE_GREEN_COMM_LOG_SIZE_SIZE   4

// the following defines are common to the Blue/Green interpreter functions
#define INVALID_TASK_INDEX     (0x7FFFFFFF) ///< Interpreter value to assign for invalid PostCodes in Blue/Green.
#define POST_CODE_IS_GREEN_IDX (0xFFFF)
#define GREEN_CONTINUE         (0x8FFF)
#define GREEN_EXECUTE_TO       (0x7FFF)
#define GREEN_LAST_TASK        (0x6FFF)
#define GREEN_NO_PREV_TASK     (0x5FFF)
#define GREEN_PRINT_CALL_TABLE (0x4FFF) // print the Green call table upon entry and return to Blue (used for interpreter)
#define GREEN_RUN_CUSTOM_RMT   (0x3FFF) // run custom RMT in Green and return to Blue
#define ENTER_INTERPRETER_NO_POST_CODE_SPECIFIED (0x0)

// Located in dram0 sram region
#define BlueGreenMailBox  0x1ffb7eec
#define GREEN_STACK_TOP   (BlueGreenMailBox - 1)

#pragma pack (push, 1)
typedef struct {
  UINT32 LogSize; // Set by Green, cleared by Blue. NOTE: must be the first entry in this struct
  UINT32 GreenStatus;
  union {
    struct {
      UINT16 Start;
      UINT16 Stop;
    } PostCodes;
    UINT32   Data;
  } TaskControl;  // IN/OUT, control green task flow and pass Green POST codes back to Blue
  union {
    struct {
      UINT32 ExecutingCommand       : 1;  // [0:0] Symmetrical IN/OUT to denote Green is to execute an interpreter command
      UINT32 Halt                   : 1;  // [1:1] Set/cleared by blue, checked by Green
      UINT32 GreenDone              : 1;  // [2:2] Set by Green, cleared by Blue
      UINT32 DoToggleVoltage        : 1;  // [3:3] Set by Green, cleared by Blue
      UINT32 DoHitEnter             : 1;  // [4:4] Set by Green, cleared by Blue
      UINT32 DoDvfscEnInputOvrd     : 1;  // [5:5] Set by Green, cleared by Blue
      UINT32 DoReadTemperature      : 1;  // [6:6] Set by Green, cleared by Blue
      INT32  PhyTemperature         : 10; // [16:7] Written by Blue, consumed by Green
      UINT32 Reserved               : 15; // [31:17]
    } Bits;
    UINT32   Data;
  } Misc;
  union {
    struct {
      UINT32 Address       : 16; // [15:0] Smbus device address
      UINT32 Value         : 8;  // [23:16] Data byte read from / sent to a Smbus device
      UINT32 Status        : 1;  // [24:24] Operation status
      UINT32 RequestType   : 2;  // [26:25] Smbus request type: 0 - No pending request, 1 - Write request, 2 - Read request
      UINT32 Reserved      : 5;  // [31:27]
    } Bits;
    UINT32   Data;
  } SmbusTransaction;
  UINT8 LogBuffer[MRC_GREEN_LOG_SIZE];
} MrcBlueGreenCommunication;

extern volatile MrcBlueGreenCommunication gCommBuffer;

// CR-based communication details
#define MRC_CR_BASED_MRC_DONE   0xFF
#define MRC_CR_BASED_HIT_ENTER  0xFE

typedef union {
  struct {
    UINT32 Byte0 : 8;  // [7:0]
    UINT32 Byte1 : 8;  // [15:8]
    UINT32 Byte2 : 8;  // [23:16]
    UINT32 Size  : 8;  // [31:24] - Print size (1..3 bytes), or a custom message for Blue/Green communication
  } Bits;
  UINT32 Data;
} MrcPrintBuf;

#pragma pack (pop)

//
// This block is payload data that should not be #defines
//

#define MRC_GREEN_COMMON_PAYLOAD_ADDR 0x1000
#define MRC_GREEN_COMMON_PAYLOAD_SIZE 0x500
#define MRC_GREEN_TASK_PAYLOAD_ADDR   0x2000
#define MRC_GREEN_TASK_PAYLOAD_SIZE   0x500

//
// Xtensa SRAM regions in UCSS address space
//

#define MRC_UCSS_DRAM_START   0x00000
#define MRC_UCSS_DRAM_END     0x17FFF
#define MRC_UCSS_DRAM_SIZE    (MRC_UCSS_DRAM_END - MRC_UCSS_DRAM_START + 1)

#define MRC_UCSS_IRAM_START   0x18000
#define MRC_UCSS_IRAM_END     0x4BFFF
#define MRC_UCSS_IRAM_SIZE    (MRC_UCSS_IRAM_END - MRC_UCSS_IRAM_START + 1)

#define MRC_UCSS_FESRAM_START 0x50000
#define MRC_UCSS_FESRAM_END   0x11FFFF
#define MRC_UCSS_FESRAM_SIZE  (MRC_UCSS_FESRAM_END - MRC_UCSS_FESRAM_START + 1)

//
// Xtensa SRAM regions in Xtensa address space
//
#define XTENSA_DRAM_BASE    0x1ffa0000
#define XTENSA_IRAM_BASE    0x1ffc0000
#define XTENSA_FESRAM_BASE  0x20002000

#define XTENSA_LOOPBACK_RESULT_BASE (XTENSA_DRAM_BASE + 0x100)  // 0x1ffa0100

// Address is relative to UCSS sram region start
#define MRC_BLUE_GREEN_COMM_DWORD1_ADDR (MRC_UCSS_DRAM_START + (BlueGreenMailBox - XTENSA_DRAM_BASE))
#define MRC_BLUE_GREEN_COMM_DWORD2_ADDR (MRC_BLUE_GREEN_COMM_DWORD1_ADDR + 4)
#define MRC_BLUE_GREEN_COMM_DWORD3_ADDR (MRC_BLUE_GREEN_COMM_DWORD2_ADDR + 4)
#define MRC_BLUE_GREEN_LOG_BUFFER_ADDR  (MRC_BLUE_GREEN_COMM_DWORD3_ADDR + 4)

//
// These are named flags/values to avoid confusion
//

#define MRC_XTENSA_SW_HALT    TRUE
#define MRC_XTENSA_SW_UNHALT  FALSE

#define MRC_XTENSA_HW_HALT    TRUE
#define MRC_XTENSA_HW_UNHALT  FALSE

#define MRC_XTENSA_SOFT_RESET           TRUE
#define MRC_XTENSA_FW_DOWNLOAD_COMPLETE TRUE

#define MRC_XTENSA_BUSY_WAIT 0x7FFF

#define MRC_GREEN_DONE 0x12345678

#define MRC_GREEN_TIMEOUT 1000000 // 1000 seconds

#define MRC_BLUE_WAIT_CYCLE 100

//
// On/off flags for behavioral switches
//

#define MRC_DEBUG_VIA_BLUE

#endif // _MrcBlueGreenCommunication_h_
