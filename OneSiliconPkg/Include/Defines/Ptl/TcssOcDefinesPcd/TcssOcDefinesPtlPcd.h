/** @file
  Register names for TCSS USB devices

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

#ifndef TCSS_OC_DEFINES_PTL_PCD_H_
#define TCSS_OC_DEFINES_PTL_PCD_H_

//Mapping from USB OC pin to USB OCM register index
#define TCSS_USB3_OCM_INDEX(a) ((a >= PtlMTcssUsb3VwOcPin0) ? (a - PtlMTcssUsb3VwOcPin0) : a)

//
// The enumerations below are OC pin definitions
//

typedef enum {
  // OC pin                           Index
  PtlMTcssUsb2PhysicalOcPin0        = 0,
  PtlMTcssUsb2PhysicalOcPin1        = 1,
  PtlMTcssUsb2PhysicalOcPin2        = 2,
  PtlMTcssUsb2PhysicalOcPin3        = 3,
  PtlMTcssUsb2VwOcPin0              = 4,
  PtlMTcssUsb2VwOcPin1              = 5,
  PtlMTcssUsb2VwOcPin2              = 6,
  PtlMTcssUsb2VwOcPin3              = 7
} PTL_M_TCSS_USB2_OC;

typedef enum {
  // OC pin                           Index
  PtlMTcssUsb3PhysicalOcPin0        = 0,
  PtlMTcssUsb3PhysicalOcPin1        = 1,
  PtlMTcssUsb3PhysicalOcPin2        = 2,
  PtlMTcssUsb3PhysicalOcPin3        = 3,
  PtlMTcssUsb3VwOcPin0              = 4,
  PtlMTcssUsb3VwOcPin1              = 5,
  PtlMTcssUsb3VwOcPin2              = 6,
  PtlMTcssUsb3VwOcPin3              = 7
} PTL_M_TCSS_USB3_OC;

#endif
