/** @file
  Definitions for HECI Clients' Addresses.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2008 Intel Corporation.

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
#ifndef HECI_ADDRESSES_H
#define HECI_ADDRESSES_H

///
/// Host Address of Fixed Address MEI Header -
/// This is the logical address of the Host client of the message. This address
/// is assigned when the host client registers itself with the Host MEI driver.
///

/**
  Address 0 is statically allocated for MEI bus message protocol that assists in MEI bus
  enumeration, power management, error and other bus control functionality.
**/
#define BIOS_FIXED_HOST_ADDR                0
#define BIOS_ASF_HOST_ADDR                  1 ///< Statically allocated for host AMT/ASF message

///
/// ME Address of Fixed Address MEI Header -
/// This is the logical address of the Intel(R) ME client
/// of the message. This address is assigned during ME firmware initialization.
///
#define HECI_HBM_MESSAGE_ADDR               0x00
#define HECI_ASF_MESSAGE_ADDR               0x02
#define HECI_ISH_SERVICE_MESSAGE_ADDR       0x03
#define HECI_PSR_MESSAGE_ADDR               0x04
#define HECI_MKHI_MESSAGE_ADDR              0x07
#define HECI_ICC_MESSAGE_ADDR               0x08
#define HECI_MCHI_MESSAGE_ADDR              0x09
#define HECI_HWA_MESSAGE_ADDR               0x0B
#define HECI_UPID_MESSAGE_ADDR              0x0D
#define HECI_BIOS_STORAGE_MESSAGE_ADDR      0x0F
#define HECI_MAX_ME_FIXED_ADDR              0x1F  ///< Max ME address for fixed clients

#endif
