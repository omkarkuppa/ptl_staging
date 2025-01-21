/** @file
  Definitions for the TraceHubHook Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#ifndef __TRACE_HUB_HOOK_LIB_H__
#define __TRACE_HUB_HOOK_LIB_H__

/**
  Return TraceHub Trace Address
  BIOS trace output to FW_BAR, Host number is 72. Channel number ranges from 1~127.

  @param [in] Host                      Host number
  @param [in] Channel                   Channel number
  @param [out] TraceAddress             Trace Address given by host and channel number

  @retval EFI_SUCCESS                   Retrieve Trace Address
          EFI_INVALID_PARAMETER         Invalid output buffer pointer
**/
RETURN_STATUS
EFIAPI
TraceHubMmioTraceAddress (
  IN  UINT32    Host,
  IN  UINT32    Channel,
  OUT UINTN     *TraceAddress
  );

/**
  Return Scratch Pad2 Register Value

  @param [out] ScratchPad2RegValue Scratch Pad2 Register Value

  @retval EFI_SUCCESS              Retrieve Scratch Pad2 Register Value
**/
RETURN_STATUS
EFIAPI
TraceHubScratchPad2RegValue (
  OUT UINT8    *ScratchPad2RegValue
  );

#endif // _TRACE_HUB_LIB_H_
