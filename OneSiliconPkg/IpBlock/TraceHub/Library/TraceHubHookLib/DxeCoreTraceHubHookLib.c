/** @file
  DXE Core TraceHub Hook Lib.

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
#include <Uefi/UefiBaseType.h>
#include <Register/TraceHubRegs.h>
#include <TraceHubDataHob.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>

UINT8        mTraceVerbosity = 0;
UINT32       mTraceHubFwBase = 0;

/**
  Return TraceHub Trace Address
  BIOS trace output to FW_BAR, Host number is 72. Channel number ranges from 1~127.

  @param [in] Host                      Host number
  @param [in] Channel                   Channel number
  @param [out] TraceAddress             Trace Address given by host and channel number

  @retval EFI_SUCCESS                   Retrieve Trace Address
          EFI_INVALID_PARAMETER         Invalid output buffer pointer
**/
EFI_STATUS
EFIAPI
TraceHubMmioTraceAddress (
  IN  UINT32                            Host,
  IN  UINT32                            Channel,
  OUT UINTN                             *TraceAddress
  )
{
  if (TraceAddress != NULL) {
    *TraceAddress = mTraceHubFwBase + (UINTN) 0x40 * (V_TRACE_HUB_MEM_STHCAP1_CHLCNT * (Host - V_TRACE_HUB_MEM_STHCAP1_FW_MSTR) + Channel);
    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
}

/**
  Return Scratch Pad2 Register Value

  @param [out] ScratchPad2RegValue Scratch Pad2 Register Value

  @retval EFI_SUCCESS              Retrieve Scratch Pad2 Register Value
**/
RETURN_STATUS
EFIAPI
TraceHubScratchPad2RegValue (
  OUT UINT8    *ScratchPad2RegValue
  )
{
  *ScratchPad2RegValue = mTraceVerbosity;
  return EFI_SUCCESS;
}

/**
  The DxeSmmTraceHubHookLib library constructor.

  This constructor depends upon gTraceHubDataHobGuid. It should be produced in RC the HOB producer phase.

  @param[in]  ImageHandle           The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable           A pointer to the EFI system table.

  @retval     EFI_SUCCESS           The constructor executed successfully.
**/
EFI_STATUS
EFIAPI
DxeCoreTraceHubHookLibInit (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  TRACEHUB_DATA_HOB                     *TraceHubDataHob;

  TraceHubDataHob = (TRACEHUB_DATA_HOB *) GetFirstGuidHob (&gTraceHubDataHobGuid);
  if (!TraceHubDataHob) {
    mTraceVerbosity = 0;
    mTraceHubFwBase = 0;
  } else {
    mTraceVerbosity = TraceHubDataHob->TraceVerbosity;
    mTraceHubFwBase = TraceHubDataHob->TraceHubFwBase;
  }
  return EFI_SUCCESS;
}