/** @file
  Header file for PeiTelemetryInitLib.h

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

#ifndef _PEI_TELEMETRY_INIT_LIB_H_
#define _PEI_TELEMETRY_INIT_LIB_H_

/**
  Reserve Memory for Telemetry

  @param[in]  TopUseableMemAddr         Top usable memory address
  @param[in]  TopMemSize                Top of memory size
  @param[in]  MemoryMapData             Memory Map Info
  @param[out] StreamTracerMemSize       StreamTracerMemSize in MB
  @param[out] StreamTracerBaseAddr      StreamTracerBaseAddr in MB
**/
VOID
PeiTelemetryMemoryAllocation (
  IN OUT EFI_PHYSICAL_ADDRESS         *TopUseableMemAddr,
  IN OUT UINT64                       *Touud,
  IN OUT EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested,
  OUT UINT32                          *StreamTracerMemSize,
  OUT UINT64                          *StreamTracerBaseAddr
);
#endif
