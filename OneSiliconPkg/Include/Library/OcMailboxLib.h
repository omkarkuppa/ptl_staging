/** @file
  Header file for Oc Mailbox Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _OC_MAILBOX_LIB_H_
#define _OC_MAILBOX_LIB_H_

//
//  OC Mailbox MSR
//
#define MSR_OC_MAILBOX                                  0x00000150

//
//  OC Mailbox commands
//
#define MAILBOX_OC_CMD_GET_OC_CAPABILITIES              0x01
#define MAILBOX_OC_CMD_GET_PER_CORE_RATIO_LIMIT         0x02
#define MAILBOX_OC_CMD_GET_DDR_CAPABILITIES             0x03
#define MAILBOX_OC_CMD_GET_VR_TOPOLOGY                  0x04
#define MAILBOX_OC_CMD_BCLK_FREQUENCY_CMD               0x05
#define MAILBOX_OC_CMD_GET_VOLTAGE_FREQUENCY            0x10
#define MAILBOX_OC_CMD_SET_VOLTAGE_FREQUENCY            0x11
#define MAILBOX_OC_CMD_GET_MISC_GLOBAL_CONFIG           0x14
#define MAILBOX_OC_CMD_SET_MISC_GLOBAL_CONFIG           0x15
#define MAILBOX_OC_CMD_GET_ICCMAX                       0x16
#define MAILBOX_OC_CMD_SET_ICCMAX                       0x17
#define MAILBOX_OC_CMD_GET_MISC_TURBO_CONTROL           0x18
#define MAILBOX_OC_CMD_SET_MISC_TURBO_CONTROL           0x19
#define MAILBOX_OC_CMD_GET_AVX_RATIO_OFFSET             0x1A
#define MAILBOX_OC_CMD_SET_AVX_RATIO_OFFSET             0x1B
#define MAILBOX_OC_CMD_READ_PER_CORE_RATIO_LIMITS_CMD   0x1C
#define MAILBOX_OC_CMD_WRITE_PER_CORE_RATIO_LIMITS_CMD  0x1D
#define MAILBOX_OC_CMD_GET_AVX_VOLTAGE_GUARDBAND        0x20
#define MAILBOX_OC_CMD_SET_AVX_VOLTAGE_GUARDBAND        0x21
#define MAILBOX_OC_CMD_GET_CPU_BCLK_FREQUENCY           0x22
#define MAILBOX_OC_CMD_SET_CPU_BCLK_FREQUENCY           0x23
#define MAILBOX_OC_CMD_GET_OC_TVB_CONFIG                0x24
#define MAILBOX_OC_CMD_SET_OC_TVB_CONFIG                0x25
#define MAILBOX_OC_CMD_READ_GRANULAR_RATIO_SUPPORT_CMD  0x26
#define MAILBOX_OC_CMD_WRITE_GRANULAR_RATIO_SUPPOR_CMD  0x27
#define MAILBOX_OC_CMD_READ_MAX_VOLTAGE_LIMIT           0x28
#define MAILBOX_OC_CMD_WRITE_MAX_VOLTAGE_LIMIT          0x29

//
// PCODE Mailbox OC Interface 0x37 command set
//
#define MAILBOX_OC_CMD_OC_INTERFACE                          0x00000037
#define MAILBOX_OC_SUBCMD_READ_OC_MISC_CONFIG                0x00000000
#define MAILBOX_OC_SUBCMD_WRITE_OC_MISC_CONFIG               0x00000001
#define MAILBOX_OC_SUBCMD_READ_OC_PERSISTENT_OVERRIDES       0x00000002
#define MAILBOX_OC_SUBCMD_WRITE_OC_PERSISTENT_OVERRIDES      0x00000003
#define MAILBOX_OC_SUBCMD_READ_TJ_MAX_OFFSET                 0x00000004
#define MAILBOX_OC_SUBCMD_WRITE_TJ_MAX_OFFSET                0x00000005
#define MAILBOX_OC_SUBCMD_READ_PLL_VCC_TRIM_OFFSET           0x00000006
#define MAILBOX_OC_SUBCMD_WRITE_PLL_VCC_TRIM_OFFSET          0x00000007
#define MAILBOX_OC_SUBCMD_READ_PVD_RATIO_THRESHOLD_OVERRIDE  0x00000008
#define MAILBOX_OC_SUBCMD_WRITE_PVD_RATIO_THRESHOLD_OVERRIDE 0x00000009
#define MAILBOX_OC_SUBCMD_READ_PERCORE_HT_DISABLE            0x0000000C
#define MAILBOX_OC_SUBCMD_WRITE_PERCORE_HT_DISABLE           0x0000000D
#define MAILBOX_OC_SUBCMD_READ_D2D_CONFIG                    0x00000012
#define MAILBOX_OC_SUBCMD_WRITE_D2D_CONFIG                   0x00000013
#define MAILBOX_OC_SUBCMD_READ_FLL_OVERRIDE                  0x00000014
#define MAILBOX_OC_SUBCMD_WRITE_FLL_OVERRIDE                 0x00000015
#define MAILBOX_OC_SUBCMD_READ_UNDERVOLT_PROTECTION          0x00000016
#define MAILBOX_OC_SUBCMD_WRITE_UNDERVOLT_PROTECTION         0x00000017
#define MAILBOX_OC_SUBCMD_READ_PLL_IREFTUNE                  0x0000001C
#define MAILBOX_OC_SUBCMD_WRITE_PLL_IREFTUNE                 0x0000001D
#define MAILBOX_OC_SUBCMD_READ_IA_MIN_RATIO                  0x0000001E
#define MAILBOX_OC_SUBCMD_WRITE_IA_MIN_RATIO                 0x0000001F

//
//  OC Mailbox completion codes
//
#define MAILBOX_OC_COMPLETION_CODE_SUCCESS              0x00
#define MAILBOX_OC_COMPLETION_CODE_OC_LOCKED            0x01
#define MAILBOX_OC_COMPLETION_CODE_INVALID_DOMAIN       0x02
#define MAILBOX_OC_COMPLETION_CODE_MAX_RATIO_EXCEEDED   0x03
#define MAILBOX_OC_COMPLETION_CODE_MAX_VOLTAGE_EXCEEDED 0x04
#define MAILBOX_OC_COMPLETION_CODE_OC_NOT_SUPPORTED     0x05
#define MAILBOX_OC_COMPLETION_CODE_ILLEGAL_VR_ADDRESS   0x06
#define MAILBOX_OC_COMPLETION_CODE_INVALID_ICCMAX       0x07
#define MAILBOX_OC_COMPLETION_CODE_OCVOLT_OVRD_DIS      0x08
#define MAILBOX_OC_COMPLETION_CODE_INVALID_AVX_LEVEL    0x09
#define MAILBOX_OC_COMPLETION_CODE_RUNTIME_DDR_OC_DIS   0x0A
#define MAILBOX_OC_COMPLETION_CODE_INVALID_MC_REG_ID    0x0B
#define MAILBOX_OC_COMPLETION_CODE_INVALID_PARAMETER    0x10
#define MAILBOX_OC_COMPLETION_CODE_UNRECOGNIZED_CMD     0x1F

//
// Common Domain ID definitions
//
// Domain  |   OCMB Cmd
// =====================
//      0  |     IA_CORE
//      1  |          GT
//      2  |        RING
//      3  |    Reserved
//      4  |          SA
//      5  |    Reserved
//      6  |       MemSS
//      7  | Compute SAF
//      8  |      CPU_SA
#define MAILBOX_OC_DOMAIN_ID_MAX                0x09

#define MAILBOX_OC_DOMAIN_ID_IA_CORE            0x00
#define MAILBOX_OC_DOMAIN_ID_GT                 0x01
#define MAILBOX_OC_DOMAIN_ID_RING               0x02
#define MAILBOX_OC_DOMAIN_ID_RESERVED           0x03
#define MAILBOX_OC_DOMAIN_ID_SYSTEM_AGENT       0x04
#define MAILBOX_OC_DOMAIN_ID_MEMORY_SUBSYSTEM   0x06
#define MAILBOX_OC_DOMAIN_ID_COMPUTE_SAF        0x07
#define MAILBOX_OC_DOMAIN_ID_NPU                0x08
//
// PLL Domain ID definitions
//
#define MAILBOX_OC_DOMAIN_ID_IA_ATOM            0x05  // PLL IA_ATOM domain also used to indicate ATOM(E-Core) for MAX_VOLTAGE_LIMIT (0x28,0x29)
#define MAILBOX_OC_DOMAIN_ID_CPU_SA             0x08

//
// BCLK Select
//
#define SOC_BCLK_SELECT                      0x0
#define CPU_BCLK_SELECT                      0x2

//
// Bit 10 is the S11.0.10V sign bit
//
#define FIXED_POINT_SIGN_BIT_MASK 0x0400
#define INT16_SIGN_BIT_MASK       0x8000

//
// Voltage Conversion defines
//
#define MILLIVOLTS_PER_VOLT  1000
#define MAX_TARGET_MV      4095
#define MAX_OFFSET_MV      500

#define CONVERT_TO_FIXED_POINT_VOLTS  0
#define CONVERT_TO_BINARY_MILLIVOLT   1

//
// Masks and offsets
//
#define BIT0_MASK             0x1
#define MAX_RATIO_MASK        0x000000FF
#define VOLTAGE_TARGET_MASK   0x000FFF00
#define VOLTAGE_TARGET_OFFSET 8
#define VOLTAGE_MODE_MASK     0x00100000
#define VOLTAGE_MODE_OFFSET   20
#define VOLTAGE_OFFSET_MASK   0xFFE00000
#define VOLTAGE_OFFSET_OFFSET 21
#define QCLK_RATIO_MASK         0x000000FF
#define MC_REF_CLK_MASK         0x00000100
#define MC_REF_CLK_OFFSET       8
#define NUM_DDR_CHANNELS_MASK   0x00000C00
#define NUM_DDR_CHANNELS_OFFSET 10

// VR Type
#define SVID_VR           0
#define NON_SVID_VR       1

#define OC_ICCMAX_MIN_VALUE                 4

// UnderVolt Protection definitions
#define UNDERVOLT_PROTECTION_DISABLED       0
#define UNDERVOLT_PROTECTION_ENABLED        1

// OC TVB defines
#define TVB_CONFIG_PER_CCP            0
#define TVB_CONFIG_PER_PCORE_GROUP    1
#define TVB_THRESHOLD_OFFSET          4

#define MAX_VOLTAGE_LIMIT_MASK 0xFFF


#ifndef MAILBOX_WAIT_TIMEOUT
#define MAILBOX_WAIT_TIMEOUT  1000  ///< 1 millisecond
#endif
#ifndef MAILBOX_WAIT_STALL
#define MAILBOX_WAIT_STALL    1     ///< 1 microsecond
#endif


///
/// Expanded Overclocking Mailbox interface defintion, contains command id/completion code,
/// input parameters and the run/busy bit
///
typedef union _OC_MAILBOX_INTERFACE {
  UINT32 InterfaceData;          ///< All bit fields as a 32-bit value.
  ///
  /// Individual bit fields.
  ///
  struct {
    UINT32 CommandCompletion : 8;   ///< [7:0]   Command ID and completion code
    UINT32 Param_4_0         : 5;   ///< [12:8]  Param_4_0, generally used to specify Domain_ID
    UINT32 InstanceId        : 13;  ///< [25:13] Param_17_5 used to specify InstanceId for command
    UINT32 Param_22_18       : 5;   ///< [30:26]
    UINT32 RunBusy           : 1;   ///< [31] Run/Busy bit. This bit is set by BIOS to indicate the mailbox buffer is ready. pcode will clear this bit after the message is consumed.
  } Fields;
} OC_MAILBOX_INTERFACE;


typedef union _OCTVB_MAILBOX_INTERFACE {
  UINT32 InterfaceData;          ///< All bit fields as a 32-bit value.
  ///
  /// Individual bit fields.
  ///
  struct {
    UINT32 CommandCompletion : 8;  ///< [7:0]   Command ID and completion code
    UINT32 Param_4_0         : 5;  ///< [12:8]  Param_4_0, core group for TRL
    UINT32 InstanceId        : 11; ///< [23:13] Param_17_5 used to specify InstanceId for command
    UINT32 InstanceId_Rsvd   : 2;  ///< [25:24] Reserved for future use
    UINT32 Param_19_18       : 2;  ///< [27:26] Limit select, used for OC TVB
    UINT32 Param_21_20       : 2;  ///< [29:28] Threshold select, used for OC TVB
    UINT32 Param_22          : 1;  ///< [30] Reserved
    UINT32 RunBusy           : 1; ///< [31] Run/Busy bit. This bit is set by BIOS to indicate the mailbox buffer is ready. pcode will clear this bit after the message is consumed.
  } Fields;
} OCTVB_MAILBOX_INTERFACE;

///
///  Consolidated OC mailbox command structure containing both data and interface information
///
typedef struct _OC_MAILBOX_FULL {
  UINT32               Data;      ///< OC Mailbox read/write data
  OC_MAILBOX_INTERFACE Interface; ///< OC mailbox interface
} OC_MAILBOX_FULL;


/**
  Generic Mailbox function for mailbox write commands. This function will
  poll the mailbox interface for control, issue the write request, poll
  for completion, and verify the write was succussful.

  @param[in]  MailboxCommand  Overclocking mailbox command data
  @param[in]  MailboxData     Overclocking mailbox interface data
  @param[out] *MailboxStatus  Pointer to the mailbox status returned from pcode. Possible mailbox status values are:
                              - SUCCESS (0)               Command succeeded.
                              - OC_LOCKED (1)             Overclocking is locked. Service is read-only.
                              - INVALID_DOMAIN (2)        Invalid Domain ID provided in command data.
                              - MAX_RATIO_EXCEEDED (3)    Ratio exceeds maximum overclocking limits.
                              - MAX_VOLTAGE_EXCEEDED (4)  Voltage exceeds input VR's max voltage.
                              - OC_NOT_SUPPORTED (5)      Domain does not support overclocking.

  @retval EFI_STATUS
          - EFI_SUCCESS           Command succeeded.
          - EFI_INVALID_PARAMETER Invalid read data detected from pcode.
          - EFI_UNSUPPORTED       Unsupported MailboxType parameter.
**/
EFI_STATUS
EFIAPI
OcMailboxWrite (
  IN UINT32  MailboxCommand,
  IN UINT32  MailboxData,
  OUT UINT32 *MailboxStatus
  );

/**
  Generic Mailbox function for mailbox read commands. This function will write
  the read request from MailboxType, and populate the read results in the MailboxDataPtr.

  @param[in]  MailboxCommand  Overclocking mailbox command data
  @param[out] *MailboxDataPtr Pointer to the overclocking mailbox interface data
  @param[out] *MailboxStatus  Pointer to the mailbox status returned from pcode. Possible mailbox status are
                              - SUCCESS (0)               Command succeeded.
                              - OC_LOCKED (1)             Overclocking is locked. Service is read-only.
                              - INVALID_DOMAIN (2)        Invalid Domain ID provided in command data.
                              - MAX_RATIO_EXCEEDED (3)    Ratio exceeds maximum overclocking limits.
                              - MAX_VOLTAGE_EXCEEDED (4)  Voltage exceeds input VR's max voltage.
                              - OC_NOT_SUPPORTED (5)      Domain does not support overclocking.

  @retval EFI_STATUS
          - EFI_SUCCESS           Command succeeded.
          - EFI_INVALID_PARAMETER Invalid read data detected from pcode.
          - EFI_UNSUPPORTED       Unsupported MailboxType parameter.

**/
EFI_STATUS
EFIAPI
OcMailboxRead (
  IN UINT32  MailboxCommand,
  OUT UINT32 *MailboxDataPtr,
  OUT UINT32 *MailboxStatus
  );
#endif
