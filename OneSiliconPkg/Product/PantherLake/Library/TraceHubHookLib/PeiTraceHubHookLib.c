/** @file
  Pei TraceHub Hook Lib.

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
#include <Uefi/UefiBaseType.h>
#include <Register/TraceHubRegs.h>
#include <TraceHubDataHob.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Defines/PchReservedResources.h>

/**
  Return TraceHub Trace Address
  BIOS trace output to FW_BAR, Host number is 72. Channel number ranges from 1~127.

  @param [in] Host                      Host number
  @param [in] Channel                   Channel number
  @param [out] TraceAddress             Trace Address given by host and channel number

  @retval EFI_SUCCESS                   Retrieve correct Address.
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
  TRACEHUB_DATA_HOB                     *TraceHubDataHob = NULL;
  UINT32                                FwBase;

  FwBase = PCH_TRACE_HUB_FW_BASE_ADDRESS;
  TraceHubDataHob = (TRACEHUB_DATA_HOB *) GetFirstGuidHob (&gTraceHubDataHobGuid);

  if (TraceAddress != NULL) {
    if (TraceHubDataHob != NULL) {
      *TraceAddress = TraceHubDataHob->TraceHubFwBase + (UINTN) 0x40 * (V_TRACE_HUB_MEM_STHCAP1_CHLCNT * (Host - V_TRACE_HUB_MEM_STHCAP1_FW_MSTR) + Channel);
    } else {
      //
      // If HOB is not there yet, PTL-S returns PCH TraceHub FW Base and the others return SOC TraceHub FW Base by default.
      //
      *TraceAddress = FwBase + (UINTN) 0x40 * (V_TRACE_HUB_MEM_STHCAP1_CHLCNT * (Host - V_TRACE_HUB_MEM_STHCAP1_FW_MSTR) + Channel);
    }
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
  TRACEHUB_DATA_HOB                     *TraceHubDataHob;

  TraceHubDataHob = (TRACEHUB_DATA_HOB *) GetFirstGuidHob (&gTraceHubDataHobGuid);
  if (!TraceHubDataHob) {
    //
    // Early trace is actived prior to BIOS, before HOB exists
    // enable all verbosity to let traces pass thru.
    //
    *ScratchPad2RegValue = B_TRACE_HUB_MEM_SCRPD2_BIOS_TRACE_EN | (V_TRACE_HUB_MEM_SCRPD2_TRACE_ALL << N_TRACE_HUB_MEM_SCRPD2_TRACE_VERBO);
  } else {
    *ScratchPad2RegValue = TraceHubDataHob->TraceVerbosity;
  }
  return EFI_SUCCESS;
}
