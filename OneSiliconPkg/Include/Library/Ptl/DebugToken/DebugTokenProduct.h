/** @file
  Header of PTL Debug Token Fields.

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

#ifndef __DEBUG_TOKEN_PRODUCT_H__
#define __DEBUG_TOKEN_PRODUCT_H__

typedef union {
  struct {
    /**
    @ToolOption: BEGIN
      start: 0
      name: Expose Debug Menu
      description: "Hide/Expose BIOS Debug Menu. Note: Intel Reference BIOS debug menu is always exposed."
      values:
        - value: 0
          name: No Impact
        - value: 1
          name: Expose if Hidden
    @ToolOption: END
    **/
    UINT32 ExposeDebugMenu    :  1;
    /**
    @ToolOption: BEGIN
      start: 1
      length: 4
      name: Platform Debug Option
      description: Set/Override the Platform Debug Option in BIOS.
      values:
        - value: 0
          name: Follow BIOS menu setting
        - value: 1
          name: Enabled - Trace active
          description: TraceHub is enabled and trace is active, blocks s0ix.
        - value: 2
          name: Enabled - Trace ready
          description: TraceHub is enabled and allowed S0ix.
        - value: 3
          name: Enabled - Trace power off
          description: TraceHub is power gated, provide setting close to functional low power state.
        - value: 4
          name: Force Disabled
          description: Disable Platform Debug
    @ToolOption: END
    **/
    UINT32 StreamingTraceSink :  4;
    /**
    @ToolOption: BEGIN
      start: 5
      length: 4
      name: Intel Reserved
      description: Reserved by Intel, for future use.
      values: []
    @ToolOption: END
    **/
    UINT32 IntelReserved1     : 4;
    /**
    @ToolOption: BEGIN
      start: 9
      length: 1
      name: Processor Trace Enable
      description: Enable processor traces by debug token.
      values:
        - value: 0
          name: Follow BIOS menu setting
        - value: 1
          name: Enabled - Processor traces
          description: Enabled processor traces with default settings (4K buffer per thread).
    @ToolOption: END
    **/
    UINT32 ProcessorTraceEnable : 1;
    /**
    @ToolOption: BEGIN
      start: 10
      length: 6
      name: Intel Reserved
      description: Reserved by Intel, for future use.
      values: []
    @ToolOption: END
    **/
    UINT32 IntelReserved2     : 6;
    /**
    @ToolOption: BEGIN
      start: 16
      length: 16
      name: OEM Reserved
      description: Available for OEM use.
      values: []
    @ToolOption: END
    **/
    UINT32 OemReserved        : 16;
  } Fields;
  UINT32  Data;
} DEBUG_TOKEN_KNOB_DATA;

#endif // __PTL_DEBUG_TOKEN_H__
