/** @file
  General ME Definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _ME_DEFINES_H_
#define _ME_DEFINES_H_

typedef enum {
  Csme,
  Sse,
  Pse
} SECURITY_ENGINE;

///
/// CSME PCI devices should be numbered according to their PCI function number
/// HECI1 = 0, HECI2 = 1, IDER = 2, SOL = 3, HECI3 = 4, HECI4 = 5
///
typedef enum {
  HECI1 = 0,
  HECI2 = 1,
  IDER  = 2,
  SOL   = 3,
  HECI3 = 4,
  HECI4 = 5
} HECI_DEVICE;

typedef enum {
  PseHeci1 = 0,
  PseHeci2,
  PseHeci3
} PSE_HECI_DEVICE;

typedef enum {
  Disabled = 0,
  Enabled,
} HECI_DEVICE_FUNC_CTRL;

///
/// Macro to convert HECI_DEVICE enum values to 0-based index values
///
#define HECI_DEVICE_TO_INDEX(Device) \
  ((Device) == HECI1 ? 0 : \
   (Device) == HECI2 ? 1 : \
   (Device) == HECI3 ? 2 : \
   (Device) == HECI4 ? 3 : \
   -1) // -1 for non-HECI devices: IDER and SOL

///
/// Macro to convert HECI_DEVICE enum values to HECI number
///
#define HECI_DEVICE_TO_NUMBER(Device) \
  ((Device) == HECI1 ? 1 : \
   (Device) == HECI2 ? 2 : \
   (Device) == HECI3 ? 3 : \
   (Device) == HECI4 ? 4 : \
   -1) // -1 for non-HECI devices: IDER and SOL

#endif
