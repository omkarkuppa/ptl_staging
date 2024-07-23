/** @file
  This file contains DEBUG macro definition implementation for IP Block usage.

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

#ifndef _PRINT_SERVICE_H_
#define _PRINT_SERVICE_H_

#ifdef IPSIM_PRINT
//
// If IPSIM_PRINT is defined it means this header will be included in for IpSim testing.
//
#include <stdio.h>

#define DEBUG_ERROR 0x80
#define DEBUG_WARN  0x10
#define DEBUG_INFO  0x00

#define _DEBUG_PRINT(PrintLevel, ...) \
  do {                                \
    printf (__VA_ARGS__);             \
  } while (0)

#define DEBUG(Expression)   _DEBUG_PRINT Expression

#else
//
// By default use EDK2 Debug library API for BIOS logging.
//
#include <Library/DebugLib.h>

#endif // IPSIM_PRINT

#endif // _PRINT_SERVICE_H_
