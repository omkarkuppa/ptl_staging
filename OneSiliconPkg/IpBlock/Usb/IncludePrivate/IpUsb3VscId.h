/** @file
  UBS3 IP Version Specific Configuration IDs

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

@par Specification
**/

#ifndef IP_USB3_VSCID_H_
#define IP_USB3_VSCID_H_

//
// Version specific configuration ID definitions
//
typedef enum {
  IpUsb3VscIdUnknown = 0,           // Unknown configuration
  IpUsb3VscIdSocCreditExchangeTime, // SOC Credit Exchange Time
  IpUsb3VscIdUsb2RfPowerGating,     // USB2 RF Power Gating
  IpUsb3VscIdBackToBackWrSupport,   // xHCI USB2 Debug Mode Back to Back WR support
  IpUsb3VscIdCameraFlicker,         // USB Camera Flicker
  IpUsb3VscIdC20PhyElasticBuffer,   // SNPS C20 Elastic Buffer mode
  IpUsb3VscIdLast
} IP_USB3_VSC_ID;

#endif
