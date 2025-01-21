/** @file
  Register Definitions for PSE

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

#ifndef _PSE_REGS_H_
#define _PSE_REGS_H_

#pragma pack(1)
///
/// HECI1_HFS, offset 40h
/// HECI1_H_GS, offset 4Ch
///
typedef union {
  UINT32  ul;
  struct {
    UINT32  EomTriggered       : 1;  ///< 0 - 1: EOM is triggered, 0: No trigger.
    UINT32  EomFirstBoot       : 1;  ///< 1 - 1: EOM on first boot, 0: EOM triggered by a command.
    UINT32  EomHwBindingEnable : 1;  ///< 2 - 1: HW Binding enabled, 0: HW Binding disabled.
    UINT32  PseSocCl           : 1;  ///< 3 - PSE_SOC_CL is set
    UINT32  EomStatus          : 4;  ///< 4:7 - 0: EOM Not Started
                                     ///        1: EOM Skipped
                                     ///        2: EOM Pending
                                     ///        3: EOM Completed Successfully
                                     ///        4: Failed to program Soft ROM Hash
                                     ///        5: Failed to program Security Keys
                                     ///        6: Failed to program fuse list
                                     ///        7: General Error
                                     ///        8-15: Reserved
    UINT32  Reserved           :24;  ///< 8:31 - This bit is set when firmware is not able to load BRINGUP from the fault tolerant (FT) code
  } r;
} PSE_HFS, PSE_H_GS;

///
/// Bit definition of R_ME_CFG_H_GS
///
#define B_ME_CFG_H_GS_PSE_EOM_TRIGGER    BIT0 ///< PSE Trigger EOM

///
/// PSE EOM Status
///
#define PSE_EOM_STATUS_NOT_STARTED   0x00
#define PSE_EOM_STATUS_SKIPPED       0x01
#define PSE_EOM_STATUS_PENDING       0x02
#define PSE_EOM_STATUS_SUCCESS       0x03
#define PSE_EOM_STATUS_FAIL_HASH     0x04
#define PSE_EOM_STATUS_FAIL_KEYS     0x05
#define PSE_EOM_STATUS_FAIL_FUSE     0x06
#define PSE_EOM_STATUS_GENERAL_ERROR 0x07

#pragma pack()

#endif // _PSE_REGS_H_
