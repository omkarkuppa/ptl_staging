/** @file
  Header file for Discrete Thunderbolt (TBT) Software Connection Manager

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

#ifndef _DTBT_SWCM_WRAPPER_H
#define _DTBT_SWCM_WRAPPER_H

//
// Max NHI device index value
//
#define MAX_TBT_DTID    1

//
// Windows USB4 host router driver requirements
// NHI ACPI device name ends with digit 0 - 7 according to TBT ID
// To distinguish iTBT and dTBT in digit, iTBT:0-3, dTBT:4-7
// Make sure _DSD of PCIe DS1/DS3 and XHCI SS ports reflect the correct NHI device whenever ACPI device name changes
//
#define NHI0_DEV        NHI4
#define NHI1_DEV        NHI5

#endif
