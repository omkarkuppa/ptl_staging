/** @file
  Header file for Cpu Mailbox Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#ifndef _CPU_MAILBOX_LIB_H_
#define _CPU_MAILBOX_LIB_H_

//
//  Mailbox Related Definitions
//
#define PCODE_MAILBOX_INTERFACE_OFFSET        0x5DA4
#define PCODE_MAILBOX_DATA_OFFSET             0x5DA0
#define PCU_CR_BCLK_FREQ_MCHBAR               0x00005F60

#ifndef MAILBOX_WAIT_TIMEOUT
#define MAILBOX_WAIT_TIMEOUT  1000  ///< 1 millisecond
#endif
#ifndef MAILBOX_WAIT_STALL
#define MAILBOX_WAIT_STALL    1     ///< 1 microsecond
#endif


//
//  Pcode Mailbox completion codes
//
#define PCODE_MAILBOX_CC_SUCCESS                      0x0
#define PCODE_MAILBOX_CC_ILLEGAL_CMD                  0x1
#define PCODE_MAILBOX_CC_TIMEOUT                      0x2
#define PCODE_MAILBOX_CC_ILLEGAL_DATA                 0x3
#define PCODE_MAILBOX_CC_EDRAM_NOT_FUNCTIONAL         0x4
#define PCODE_MAILBOX_CC_ILLEGAL_VR_ID                0x5
#define PCODE_MAILBOX_CC_LOCKED                       0x6
#define PCODE_MAILBOX_CC_VR_ERROR                     0x7
#define PCODE_MAILBOX_CC_ILLEGAL_SUBCOMMAND           0x8
#define PCODE_MAILBOX_CC_EDRAM_CURRENTLY_UNAVAILABLE  0xA

///
///  Expanded Pcode Mailbox interface defintion, contains command id, param1, param2,
///  and the run/busy bit
///
typedef union _PCODE_MAILBOX_INTERFACE {
  UINT32 InterfaceData;   ///< All bit fields as a 32-bit value.
  ///
  /// Individual bit fields.
  ///
  struct {
    UINT32 Command  : 8;  ///< Pcode mailbox command
    UINT32 Param1   : 8;  ///< This field contains parameter 1, used by sub-commands.
    UINT32 Param2   : 13; ///< This field contains parameter 2.
    UINT32 Reserved : 2;  ///< Reserved for future use
    UINT32 RunBusy  : 1;  ///< Run/Busy bit. This bit is set by BIOS to indicate the mailbox buffer is ready. pcode will clear this bit after the message is consumed.
  } Fields;
} PCODE_MAILBOX_INTERFACE;

///
///  Consolidated Pcode mailbox command structure containing both data and interface information
///
typedef struct _PCODE_MAILBOX_FULL {
  PCODE_MAILBOX_INTERFACE Interface; ///< Pcode mailbox interface
  UINT32                  Data;      ///< Pcode mailbox read/write data
} PCODE_MAILBOX_FULL;

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
**/
EFI_STATUS
EFIAPI
MailboxWrite (
  IN UINT32  MailboxCommand,
  IN UINT32  MailboxData,
  OUT UINT32 *MailboxStatus
  );

/**
  Generic Mailbox function for mailbox read commands. This function will write
  the read request, and populate the read results in the MailboxDataPtr.

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
**/
EFI_STATUS
EFIAPI
MailboxRead (
  IN UINT32  MailboxCommand,
  OUT UINT32 *MailboxDataPtr,
  OUT UINT32 *MailboxStatus
  );

/**
  Generic Mailbox function for mailbox read commands. This function will write
  the read request, and populate the read results in the output data.
  Different with MailboxRead, the param MailboxDataPtr is IN&OUT type, which will
  be writen into mailbox at first, then be replaced by the read result.

  @param[in]  MailboxCommand
  @param[out] *MailboxDataPtr
  @param[out] *MailboxStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
MailboxReadWithInput (
  IN     UINT32  MailboxCommand,
  IN OUT UINT32  *MailboxDataPtr,
     OUT UINT32  *MailboxStatus
  );

#endif
