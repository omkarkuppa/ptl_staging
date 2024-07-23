/** @file
  Memory Mapped IO (MMIO) space access lib - produces MMIO access protocol

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

@par Specification Reference:
**/

#ifndef _MMIO_ACCESS_LIB_H_
#define _MMIO_ACCESS_LIB_H_

#include <RegisterAccess.h>

typedef struct {
  REGISTER_ACCESS   RegAccess;
  UINT64            MmioBar;
} MMIO_ACCESS;

/**
  Initializes REGISTER_ACCESS protocol.

  @param[in]    MmioBar           MMIO base address
  @param[out]   MmioSpaceAccess   Structure to be initialized

  @retval TRUE   Initialization successful
  @retval FALSE  Failed to initialize
**/
BOOLEAN
MmioAccessInit (
  IN  UINT64        MmioBar,
  OUT MMIO_ACCESS   *MmioSpaceAccess
  );

#endif // _MMIO_ACCESS_LIB_H_
