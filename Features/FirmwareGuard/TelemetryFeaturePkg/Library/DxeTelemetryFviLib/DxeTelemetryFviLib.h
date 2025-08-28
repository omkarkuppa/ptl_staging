/** @file
  Dxe Telemetry Lib implementation.

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
#ifndef _DXE_TELEMETRY_FVI_LIB_H_
#define _DXE_TELEMETRY_FVI_LIB_H_

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TelemetryFviLib.h>
#include <Protocol/TelemetryAdapterInformation.h>
#include <Protocol/AdapterInformation.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TelemetryFviHookLib.h>
#include <Guid/ComponentId.h>
#include "FviPartitionId.h"

//
// Version field bit shift constants (from TelemetryCommon.h)
//

#define VERSION_MAJOR_VERSION_SHIFT   48
#define VERSION_MINOR_VERSION_SHIFT   32
#define VERSION_REVISION_SHIFT        16

//
// HECI FLASH_PARTITION_DATA structures (from CSE firmware interface specification)
//

typedef struct {
  UINT16  Major;
  UINT16  Minor;
  UINT16  Hotfix;
  UINT16  Build;
} FW_VERSION;

typedef struct {
  UINT32      PartitionId;       // FPT_PARTITION_ID
  UINT8       Reserved1[8];
  FW_VERSION  Version;
  UINT32      VendorId;
  UINT32      TcbSvn;
  UINT32      ArbSvn;
  UINT32      Vcn;
  UINT32      Reserved2[13];
} FLASH_PARTITION_DATA;

#endif
