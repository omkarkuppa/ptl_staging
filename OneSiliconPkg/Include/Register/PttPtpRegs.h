/** @file
  Register definitions for PTT HCI (Platform Trust Technology - Host Controller Interface).

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values of bits within the registers
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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
#ifndef _PTT_HCI_REGS_H_
#define _PTT_HCI_REGS_H_

///
/// LT public space registers
///
#define R_LT_UCS                           0xFED30880
#define R_LT_EXISTS                        0xFED30010

///
/// FTPM HCI register base address
///
#define R_PTT_HCI_BASE_ADDRESS             0xFED40000
#define R_PTT_TXT_STS_FTIF                 0xFED30800

///
/// FTIF PTT present value
///
#define V_FTIF_FTPM_PRESENT                0x00070000
///
/// FTIF SPI dTPM present value
///
#define V_FTIF_SPI_DTPM_PRESENT            0x00050000
///
/// FTIF PSE present value
///
#define V_FTIF_PSE_PRESENT                 0x00040000
///
/// FTIF TPM Location Mask
///
#define FTIF_FT_LOC_MASK                   (BIT18 | BIT17 | BIT16)

//
// FTPM HCI Control Area
//
#define R_PTT_LOCALITY_STATE                0x00
#define R_TPM_LOCALITY_CONTROL              0X08
#define R_TPM_LOCALITY_STATUS               0x0C
#define R_TPM_INTERFACE_ID                  0x30
#define R_CRB_CONTROL_EXT                   0x38
#define R_CRB_CONTROL_REQ                   0x40
#define R_CRB_CONTROL_STS                   0x44
#define R_CRB_CONTROL_CANCEL                0x48
#define R_CRB_CONTROL_START                 0x4C
#define R_CRB_CONTROL_INT                   0x50
#define R_CRB_CONTROL_CMD_SIZE              0x58
#define R_CRB_CONTROL_CMD_LOW               0x5C
#define R_CRB_CONTROL_CMD_HIGH              0x60
#define R_CRB_CONTROL_RESPONSE_SIZE         0x64
#define R_CRB_CONTROL_RESPONSE_ADDR         0x68
#define R_CRB_DATA_BUFFER                   0x80

//
// R_CRB_CONTROL_STS Bits
//
#define B_CRB_CONTROL_STS_TPM_STATUS             0x00000001 ///< BIT0
#define B_CRB_CONTROL_STS_TPM_IDLE               0x00000002 ///< BIT1

//
// R_CRB_CONTROL_REQ Bits
//
#define B_CRB_CONTROL_REQ_COMMAND_READY          0x00000001 ///< BIT0
#define B_CRB_CONTROL_REQ_GO_IDLE                0x00000002 ///< BIT1

//
// R_CRB_CONTROL_START Bits
//
#define B_CRB_CONTROL_START                      0x00000001 ///< BIT0

//
// R_TPM_LOCALITY_STATUS Bits
//
#define B_CRB_LOCALITY_STS_GRANTED               0x00000001 ///< BIT0
#define B_CRB_LOCALITY_STS_BEEN_SEIZED           0x00000002 ///< BIT1

//
// R_TPM_LOCALITY_CONTROL Bits
//
#define B_CRB_LOCALITY_CTL_REQUEST_ACCESS       0x00000001 ///< BIT0
#define B_CRB_LOCALITY_CTL_RELINQUISH           0x00000002 ///< BIT1
#define B_CRB_LOCALITY_CTL_SEIZE                0x00000004 ///< BIT2

//
// R_PTT_LOCALITY_STATE Bits
//
#define B_CRB_LOCALITY_STATE_TPM_ESTABLISHED    0x00000001 ///< BIT0
#define B_CRB_LOCALITY_STATE_LOCALITY_ASSIGNED  0x00000002 ///< BIT1
#define B_CRB_LOCALITY_STATE_REGISTER_VALID     0x00000080 ///< BIT7

//
// R_PTT_LOCALITY_STATE Mask Values
//
#define V_CRB_LOCALITY_STATE_ACTIVE_LOC_MASK    0x0000001C /// Bits [4:2]

//
// R_PTT_RESERVED
//
#define B_PTT_RESERVED_BIT    0x00080000 /// Bit [19]

//
// Value written to R_PTT_HCI_CMD and CA_START
// to indicate that a command is available for processing
//
#define V_PTT_HCI_COMMAND_AVAILABLE_START  0x00000001
#define V_PTT_HCI_COMMAND_AVAILABLE_CMD    0x00000000
#define V_PTT_HCI_BUFFER_ADDRESS_RDY       0x00000003

///
/// Ignore bit setting mask for WaitRegisterBits
///
#define V_PTT_HCI_IGNORE_BITS              0x00000000

///
/// All bits clear mask for WaitRegisterBits
///
#define V_PTT_HCI_ALL_BITS_CLEAR           0xFFFFFFFF
#define V_PTT_HCI_START_CLEAR              0x00000001

///
/// Max FTPM command/response buffer length
///
#define S_PTT_HCI_CRB_LENGTH               3968 ///< 0xFED40080:0xFED40FFF = 3968 Bytes

//
// TIS 1.3 style Interface
//
#define TIS_13_TPM_INTERFACE           0xFF

#endif
