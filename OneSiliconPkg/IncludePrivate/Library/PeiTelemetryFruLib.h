/** @file
  Telemetry Initialization Fru Library header file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _TELEMETRY_INIT_FRU_LIB_PTL_CDIE_H_
#define _TELEMETRY_INIT_FRU_LIB_PTL_CDIE_H_

#include <Ppi/SiPolicy.h>

/**
  This function will program Stream Tracer memory buffers
  @param[in]  SiPreMemPolicy      The Silicon Pre Mem Policy PPI instance

**/
VOID
EFIAPI
SetStreamTracerMemoryBuffer (
  IN    SI_PREMEM_POLICY_PPI        *SiPreMemPolicy
  );

/**
  This function will check if Intel Telemetry is Enabled and Supported

  @retval TRUE          Telemetry Enabled and Supported
  @retval FALSE         Telemetry not Enabled or not Supported
**/
BOOLEAN
EFIAPI
IsTelemetrySupported (
  VOID
  );

/**
  Calculate Streamtracer size required to reserve memory

  @retval Streamtracer size
**/
UINT32
EFIAPI
GetStreamTracerSize (
  VOID
  );

/**
  Calculate Streamtracer size required for IMR

  @retval Size Streamtracer IMR size
**/
UINT32
EFIAPI
GetStreamTracerImrSize (
  VOID
  );
#endif // _TELEMETRY_INIT_FRU_LIB_H
