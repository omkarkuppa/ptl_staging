/** @file
  Header file for Pci Express helps library implementation.

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
#ifndef _PCI_EXPRESS_HELPERS_LIBRARY_H_
#define _PCI_EXPRESS_HELPERS_LIBRARY_H_

#include <Library/PchInfoLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Register/PchPcieRpRegs.h>

#define LTR_VALUE_MASK (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7 + BIT8 + BIT9)
#define LTR_SCALE_MASK (BIT10 + BIT11 + BIT12)

  #define CONFIG_WRITE_LOOP_COUNT   100000

//
// LTR related macros
//
#define LTR_LATENCY_VALUE(x)           ((x) & LTR_VALUE_MASK)
#define LTR_SCALE_VALUE(x)             (((x) & LTR_SCALE_MASK) >> 10)
#define LTR_LATENCY_NS(x)              (LTR_LATENCY_VALUE(x) * (1 << (5 * LTR_SCALE_VALUE(x))))

#endif
