/** @file
  Mrc definition of supported features.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _MrcApi_h_
#define _MrcApi_h_

#include "CMrcTypes.h"

#define MRC_TIMER_1NS     (1)
#define MRC_TIMER_1US     (1000)
#define MRC_TIMER_1MS     (1000 * 1000)
#define MRC_TIMER_1S      (1000 * 1000 * 1000)

#define MRC_WAIT_TIMEOUT      10000  // 10 second wait timeout
#define MRC_DLL_DETERMINISTIC_WAIT_TIMEOUT      15000  // 15 second wait timeout

#define FEMTOSECONDS_PER_NANOSECOND 1000000 ///< Number of Femtoseconds in a Nanosecond

#define START_TEST_DELAY  (60)
#define IO_RESET_DELAY    (60)

// Max Possible Vrefs
#define MAX_RX_VREF_OFFSET           255     ///< Maximum possible RxVref Offset - in LNL comes from DATA0CH0_CR_DDRCRADC.VrefOffset
#define MAX_RX_VREF_OFFSET_NTC       63      ///< Maximum possible RxVref Offset - in NTC comes from DATA0CH0_CR_DDRCRDATAOFFSETTRAIN.VrefOffset

// Max Possible Timing
#define MAX_POSSIBLE_TIME            31      ///< Maximum possible margin for time
#define MAX_POSSIBLE_TIME_RCVENX     24      ///< Maximum possible margin for time for RcvEnable
#define MAX_POSSIBLE_TIME_RCVENX_RMT  8      ///< Maximum possible margin for time for RcvEnable during RMT

#define COMP_INT                    0x0A    ///< For 10ms
#define MID_INT_VREF_UY             191     ///< Middle of InternalVref Range CaVref
#define MID_INT_VREF_H              211     ///< Middle of InternalVref Range CaVref
#define TXEQFULLDRV                 (0x30)  ///< SKL - 15 Emphasize legs (not trained)
#define DIMMRON                     (ODIC_RZQ_6)

///
/// Compile time configuration parameters - START.
/// The user must set these values for the system.
///
#define DIMMSIZEMIN            512         ///< The minimum size of DIMM, in MBytes.
#define DIMMSIZEMAX            (32 * 1024) ///< The maximum size of DIMM, in MBytes.

#define FREQMIN                f1067       ///< The minimum valid frequency.

#define SUPPORT_SPD_CRC        UNSUPPORT   ///< SUPPORT means that the CRC of the DIMMs SPD must match.
#define SUPPORT_UDIMM          SUPPORT     ///< SUPPORT means that unbuffered DIMMs are supported, else UNSUPPORT.
#define SUPPORT_SODIMM         SUPPORT     ///< SUPPORT means that SO-DIMMs are supported, else UNSUPPORT.
#define SUPPORT_RDIMM          UNSUPPORT   ///< SUPPORT means that registered DIMMs are supported, else UNSUPPORT.
#define SUPPORT_PRIWIDTH_8     UNSUPPORT   ///< SUPPORT means that SDRAM primary bus width of 8 is supported by the system.
#define SUPPORT_PRIWIDTH_16    SUPPORT     ///< SUPPORT means that SDRAM primary bus width of 16 is supported by the system.
#define SUPPORT_PRIWIDTH_32    SUPPORT     ///< SUPPORT means that SDRAM primary bus width of 32 is supported by the system.
#define SUPPORT_PRIWIDTH_64    SUPPORT     ///< SUPPORT means that SDRAM primary bus width of 64 is supported by the system.
#define SUPPORT_DEVWIDTH_4     UNSUPPORT   ///< SUPPORT means that SDRAM device width of 4 is supported by the system.
#define SUPPORT_DEVWIDTH_8     SUPPORT     ///< SUPPORT means that SDRAM device width of 8 is supported by the system.
#define SUPPORT_DEVWIDTH_16    SUPPORT     ///< SUPPORT means that SDRAM device width of 16 is supported by the system.
#define SUPPORT_DEVWIDTH_32    SUPPORT     ///< SUPPORT means that SDRAM device width of 32 is supported by the system.

#define SUPPORT_ROW_12         SUPPORT     ///< SUPPORT means that 12 bit size is supported by the system.
#define SUPPORT_ROW_13         SUPPORT     ///< SUPPORT means that 13 bit size is supported by the system.
#define SUPPORT_ROW_14         SUPPORT     ///< SUPPORT means that 14 bit size is supported by the system.
#define SUPPORT_ROW_15         SUPPORT     ///< SUPPORT means that 15 bit size is supported by the system.
#define SUPPORT_ROW_16         SUPPORT     ///< SUPPORT means that 16 bit size is supported by the system.
#define SUPPORT_ROW_17         SUPPORT     ///< SUPPORT means that 17 bit size is supported by the system.
#define SUPPORT_ROW_18         SUPPORT     ///< SUPPORT means that 18 bit size is supported by the system.

#define SUPPORT_BANK_4         SUPPORT     ///< SUPPORT means that 4 banks is supported by the system.
#define SUPPORT_BANK_8         SUPPORT     ///< SUPPORT means that 8 banks is supported by the system.
#define SUPPORT_BANK_16        SUPPORT     ///< SUPPORT means that 16 banks is supported by the system.
#define SUPPORT_BANK_32        UNSUPPORT   ///< SUPPORT means that 32 banks is supported by the system.
#define SUPPORT_BANK_64        UNSUPPORT   ///< SUPPORT means that 64 banks is supported by the system.

#define CASLATMINPOSSIBLE      22          ///< CAS Latency possible range, in number of tCK cycles
#define CASLATMAXPOSSIBLE      90
#define TAAMINPOSSIBLE         4           ///< tAAmin possible range, in number of tCK cycles.
#define TAAMAXPOSSIBLE         24
#define TWRMINPOSSIBLE         5           ///< tWRmin possible range, in number of tCK cycles.
#define TWRMAXPOSSIBLE         16          ///< tWRmin values of 9, 11, 13 ,15 are not valid for DDR3.
#define TRCDMINPOSSIBLE        4           ///< tRCDmin possible range, in number of tCK cycles.
#define TRCDMAXPOSSIBLE        20
#define TRRDMINPOSSIBLE        4           ///< tRRDmin possible range, in number of tCK cycles.
#define TRRDSMINPOSSIBLE       4           ///< tRRD_Smin possible range, in number of tCK cycles.
#define TRRDSMAXPOSSIBLE       10
#define TRRDLMINPOSSIBLE       4           ///< tRRD_Lmin possible range, in number of tCK cycles.
#define TRRDLMAXPOSSIBLE       15
#define TRPMINPOSSIBLE         4           ///< tRPmin possible range, in number of tCK cycles.
#define TRPABMINPOSSIBLE       4           ///< tRPabmin possible range, in number of tCK cycles.
#define TRASMINPOSSIBLE        10          ///< tRASmin possible range, in number of tCK cycles.
#define TRASMAXPOSSIBLE        40
#define TRCMINPOSSIBLE         1           ///< tRCmin possible range, in number of tCK cycles.
#define TRCMAXPOSSIBLE         4095
#define TRFCMINPOSSIBLE        1           ///< tRFCmin possible range, in number of tCK cycles.
#define TRFCMAXPOSSIBLE        1023
#define TWTRMINPOSSIBLE        4           ///< tWTRmin possible range, in number of tCK cycles.
#define TWTRMAXPOSSIBLE        10
#define TWTRSMINPOSSIBLE       2           ///< tWTR_S possible range, in number of tCK cycles.
#define TWTRSMAXPOSSIBLE       4
#define TWTRLMINPOSSIBLE       4           ///< tWTR_L possible range, in number of tCK cycles.
#define TWTRLMAXPOSSIBLE       11
#define TRTPMINPOSSIBLE        12          ///< tRTPmin possible range, in number of tCK cycles.
#define TRTPMAXPOSSIBLE        33
#define TFAWMINPOSSIBLE        10          ///< tFAWmin possible range, in number of tCK cycles.
#define TFAWMAXPOSSIBLE        54
#define TCWLMINPOSSIBLE        5           ///< tCWLmin possible range, in number of tCK cycles.
#define TCWLMAXPOSSIBLE        12
#define TREFIMINPOSSIBLE       1           ///< tREFImin possible range, in number of tCK cycles.
#define TREFIMAXPOSSIBLE       65535
#define NMODEMINPOSSIBLE       1           ///< Command rate mode min possible range, in number of tCK cycles.
#define NMODEMAXPOSSIBLE       3

#define DDR5_TWRMINPOSSIBLE    48          ///< tWRmin possible range, in number of tCK cycles.
#define DDR5_TWRMAXPOSSIBLE    132         ///< tWRmin values for DDR5 are in 6 CK step size.

#define LPDDR5_TCWLMINPOSSIBLE  3           ///< tCWLmin possible range, in number of tCK cycles.
#define LPDDR_TWRMINPOSSIBLE    4           ///< tWRminpossible range, in number of tCK cycles.

#define HOST_BRIDGE_BUS        0           ///< The host bridge bus number.
#define HOST_BRIDGE_DEVICE     0           ///< The host bridge device number.
#define HOST_BRIDGE_FUNCTION   0           ///< The host bridge function number.
#define HOST_BRIDGE_DEVID      0           ///< The host bridge device id offset.
#define HOST_BRIDGE_REVID      8           ///< The host bridge revision id offset.

#define MEMORY_RATIO_MIN       0           ///< The minimum DDR ratio value that the hardware supports.
#define MEMORY_RATIO_MAX       255         ///< The maximum DDR ratio value that the hardware supports.

// CA NMode defines the number of tCK cycles for the channel DIMM's command rate mode
#define CA_1_NMODE             1
#define CA_2_NMODE             2

///
/// Compile time configuration parameters - END.
///
#define EYE_MARGIN_SUPPORT       0       ///< Include code to plot 2D eye diagrams
#define DATA_EYE_MARGIN_SUPPORT  0       ///<  plot 2D data eye diagrams in RMT
#define CMD_EYE_MARGIN_SUPPORT   0       ///<  plot 2D cmd eye diagrams in RMT

///
/// Exit mode
///
typedef enum {
  emSlow      = 0,
  emFast      = 1,
  emAuto      = 0xFF,
} MrcExitMode;


/// SafeModeOverride definitions
#define MRC_SAFE_OVERRIDE_DDR      (MRC_BIT0)
#define MRC_SAFE_OVERRIDE_MC       (MRC_BIT1)
#define MRC_SAFE_OVERRIDE_RESERVED (MRC_BIT2)
#define MRC_SAFE_OVERRIDE_TRAINING (MRC_BIT3)
#define MRC_SAFE_OVERRIDE_SAGV     (MRC_BIT4)

/// Mrc LpMode
#define MRC_LPMODE0P5     (MRC_BIT0)
#define MRC_LPMODE2       (MRC_BIT1)
#define MRC_LPMODE3       (MRC_BIT2)
#define MRC_LPMODE_ALL    (MRC_BIT0 | MRC_BIT1 | MRC_BIT2)

/// Mrc LpMode4

typedef enum {
  MrcLpMode4_Disable,
  MrcLpMode4_Enable,
  MrcLpMode4_DynamicThreshold2,
  MrcLpMode4_DynamicThreshold3
} MRC_LPMODE4_TYPES;

///
/// System definitions
///
///
/// Register default values
///
#define MRC_DIMM_RANK_INTERLEAVE      (1)
#define MRC_ENHANCED_INTERLEAVE_MODE  (1)

///
///  CPU Mailbox Related Definitions
///
#ifndef PCODE_MAILBOX_INTERFACE_OFFSET
#define PCODE_MAILBOX_INTERFACE_OFFSET  BIOS_MAILBOX_INTERFACE_PCU_REG
#endif
#ifndef PCODE_MAILBOX_DATA_OFFSET
#define PCODE_MAILBOX_DATA_OFFSET       BIOS_MAILBOX_DATA_PCU_REG
#endif
#define OC_MAILBOX_MSR                  0x00000150

//
// CPU Mailbox Commands
//
#define CPU_MAILBOX_CMD_SAGV_SET_POLICY         0x00000122
#define SET_EPG_BIOS_POWER_OVERHEAD_0_CMD       0x00000020
#define SET_EPG_BIOS_POWER_OVERHEAD_1_CMD       0x00000120
#define MAILBOX_BIOS_CMD_READ_BIOS_MC_REQ_ERROR 0x00000009  ///< Allows reading the error indication for DDR checks where the memory does not lock.
#define PMA_ERROR_NO_UPDATE                                   0xFF ///< No Update to Error Status
#define PMA_ERROR_DDR_CHECKS_PASSED                           0 ///< No error - memory should lock
#define PMA_ERROR_DDR_CHECKS_ILLEGAL_DDR_TYPE                 1 ///< DDR Type is not one of the fuse enabled technologies
#define PMA_ERROR_DDR_CHECKS_FAILED_LT_CONDITIONS             2 ///< LT conditions not met (Failing since this is LT platform, SMX not disabled and memory already locked)
#define PMA_ERROR_DDR_CHECKS_QCLK_FREQ_BELOW_MIN              3 ///< The frequency is too low
#define PMA_ERROR_DDR_CHECKS_QCLK_FREQ_ABOVE_OC_LIMIT         4 ///< The frequency is above allowed OC limit
#define PMA_ERROR_DDR_CHECKS_QCLK_FREQ_ABOVE_MAX_F_LIMIT      5 ///< The frequency is above the maximum allowed Qclk
#define PMA_ERROR_DDR_CHECKS_DDR_FREQ_ABOVE_MAX_F_TECH_LIMIT  6 ///< The frequency is above the maximum allowed Qclk for the requested DDR Type
#define PMA_ERROR_DDR_CHECKS_DDRPHY_ERROR                     10 ///< Error in DDR Phy

#define CPU_MAILBOX_BIOS_CMD_READ_DDRPHY_TEMPERATURE            0x2
// Result[9:0]   - 2s complement Temperature1 (diode 2)
// Result[10:10] - Diode 2 temperature valid bit
// Result[20:11] - 2s complement Temperature2 (diode 3)
// Result[21:21] - Diode 3 temperature valid bit
typedef union {
  struct {
    INT32   DdrphyTemperature1      : 10; // Bits 9:0
    UINT32  DdrphyTemperature1Valid : 1;  // Bit  10
    INT32   DdrphyTemperature2      : 10; // Bits 20:11
    UINT32  DdrphyTemperature2Valid : 1;  // Bit  21
  } Bits;
  UINT32 Data;
} DDRPHY_TEMPERATURE_RESULT;

#define DDRPHY_TEMPERATURE_INVALID 0x1FF

// PARAM1[15:8] - subcommand
// PARAM2[28:16] - MC0 or MC1
#define CPU_MAILBOX_BIOS_CMD_MRC_CR_INTERFACE                   0x3E
#define CPU_MRC_CR_INTERFACE_SUBCMD_READ_MC_CFI_FABRIC_CONTROL  0
#define CPU_MRC_CR_INTERFACE_SUBCMD_WRITE_MC_CFI_FABRIC_CONTROL 1
#define CPU_MRC_CR_INTERFACE_GET_MC_MCAERR_CTL                  2
#define CPU_MRC_CR_INTERFACE_SET_MC_MCAERR_CTL                  3

#define MC_MCAERR_CTL_ERR_ADDRESS_ACCESS 0x2    // MC_MCAERR_CTL.err_address_access is bit [1]


#define CPU_MAILBOX_BCLK_CONFIG_CMD                             0x0000003F
#define CPU_MAILBOX_BCLK_CONFIG_GET_SSC_CONTROL_SUBCOMMAND      3
#define CPU_MAILBOX_BCLK_CONFIG_SET_SSC_CONTROL_SUBCOMMAND      4

#define MAILBOX_VR_CMD_EARLY_VR_DONE     0x46

#define CPU_MAILBOX_CMD_PARAM_1_OFFSET   8
#define CPU_MAILBOX_CMD_PARAM_2_OFFSET   16

#define CPU_MAILBOX_CMD(Cmd, Param1, Param2) ((Cmd) | ((Param1) << CPU_MAILBOX_CMD_PARAM_1_OFFSET) | ((Param2) << CPU_MAILBOX_CMD_PARAM_2_OFFSET))

//
// CPU Mailbox Completion Codes
//
#ifndef PCODE_MAILBOX_CC_SUCCESS
#define PCODE_MAILBOX_CC_SUCCESS            0
#endif

//
// MSR definitions
//
#define PCU_CR_PLATFORM_INFO        0xCE
#define MRC_MSR_IA32_BIOS_SIGN_ID   0x8B
#define MRC_MSR_IA32_MC5_CTL        0x414
#define MRC_MSR_IA32_MC5_STATUS     0x415
#define MRC_MSR_IA32_MC5_MISC       0x417

#pragma pack (push, 1)
typedef union {
  struct {
  UINT32: 8;
    UINT32 MAX_NON_TURBO_LIM_RATIO : 8;
  UINT32: 16;
  UINT32: 32;
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PCU_CR_PLATFORM_INFO_STRUCT;
#pragma pack (pop)

#endif
