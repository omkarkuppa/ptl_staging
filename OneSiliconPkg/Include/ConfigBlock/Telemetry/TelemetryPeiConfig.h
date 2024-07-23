/** @file
  Configurations for Telemetry

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _TELEMETRY_CONFIG_H_
#define _TELEMETRY_CONFIG_H_

#include <ConfigBlock.h>

#pragma pack (push,1)

#define TELEMETRY_PEI_CONFIG_REVISION 1
#define TELEMETRY_PEI_PREMEM_CONFIG_REVISION 1

#define STREAMTRACER_DISABLED   0
#define STREAMTRACER_USER_INPUT 3

extern EFI_GUID gTelemetryPeiConfigGuid;
extern EFI_GUID gTelemetryPeiPreMemConfigGuid;

typedef enum {
  StreamTracerDisabled      = STREAMTRACER_DISABLED,
  StreamTracerUserInputSize = STREAMTRACER_USER_INPUT,
  StreamTracerAuto,
  StreamTracerAdvancedTrace
} STREAMTRACER_MODE;

/**
  This configuration block describes Telemetry settings in PreMem.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER        Header;              /// Config Block Header
  UINT32                     CpuCrashLogDevice;   /// Enable or Disable CrashLog/Telemetry Device  0- Disable, <b>1- Enable</b>
  UINT32                     StreamTracerSize;    /// StreamTracer size in KB, Range 64KB - 512MB
  UINT64                     StreamTracerBase;    /// StreamTracer physical address
  UINT32                     StreamTracerMode;    /// 0 - Disable StreamTracer, 3 - Allow user to enter size, 8192 - Auto mode with size 8MB, 524288 - Advanced trace mode with size 512MB
} TELEMETRY_PEI_PREMEM_CONFIG;

/**
  This configuration block describes Telemetry settings in PostMem.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER        Header;              /// Config Block Header
  UINT32                     CpuCrashLogEnable;   /// Enable or Disable CrashLog feature  0- Disable, <b>1- Enable</b>
} TELEMETRY_PEI_CONFIG;

#pragma pack (pop)

#endif
