/** @file
  CPU Test Config Block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _CPU_TEST_CONFIG_H_
#define _CPU_TEST_CONFIG_H_

#define CPU_TEST_CONFIG_REVISION 1

extern EFI_GUID gCpuTestConfigGuid;

#pragma pack (push,1)

/**
  CPU Test Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                                  ///< Config Block Header
  UINT32                MlcStreamerPrefetcher           : 1;     ///< Enable or Disable MLC Streamer Prefetcher; 0: Disable; <b>1: Enable</b>.
  UINT32                MlcSpatialPrefetcher            : 1;     ///< Enable or Disable MLC Spatial Prefetcher; 0: Disable; <b>1: Enable</b>.
  UINT32                MonitorMwaitEnable              : 1;     ///< Enable or Disable Monitor /MWAIT instructions; 0: Disable; <b>1: Enable</b>.
  UINT32                MachineCheckEnable              : 1;     ///< Enable or Disable initialization of machine check registers; 0: Disable; <b>1: Enable</b>.
  UINT32                ProcessorTraceOutputScheme      : 1;     ///< Control on Processor Trace output scheme; <b>0: Single Range Output</b>; 1: ToPA Output.
  UINT32                ProcessorTraceEnable            : 1;     ///< Enable or Disable Processor Trace feature; <b>0: Disable</b>; 1: Enable.
  UINT32                ProcessorTraceBspOnly           : 1;     ///< Processor Trace for all cores or BSP only; <b>0: all core</b>; 1: BSP only.
  UINT32                ProcessorTraceTimingPacket      : 1;     ///< Enable or Disable Processor Trace timing packet (CYC, TSC); <b>0: Disable</b>; 1: Enable.
  UINT32                ThreeStrikeCounter              : 1;     ///< Enable or Disable Three strike counter; 0: Disable; <b>1: Enable</b>.
  UINT32                RsvdBits                        : 23;    ///< Reserved for future use
  /**
     Memory size per thread for Processor Trace.
     Processor Trace requires 2^N alignment and size in bytes per thread, from 4KB to 128MB.
     - <b>0xFF: Disabled</b>, 0:4k, 0x1:8k, 0x2:16k, 0x3:32k, 0x4:64k, 0x5:128k, 0x6:256k,
       0x7:512k, 0x8:1M, 0x9:2M, 0xa:4M. 0xb:8M, 0xc:16M, 0xd:32M, 0xe:64M, 0xf:128M
  **/
  UINT32                ProcessorTraceMemSize;
  UINT8                 Reserved0[4];
} CPU_TEST_CONFIG;

#pragma pack (pop)

#endif // _CPU_TEST_CONFIG_H_
