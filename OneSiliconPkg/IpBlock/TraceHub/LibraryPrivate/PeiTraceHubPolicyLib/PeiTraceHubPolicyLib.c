/** @file
  This file provides services to configure Trace Hub PreMem policy

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>

/**
  Print PCH Trace Hub Config

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
PchTraceHubPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  TRACE_HUB_PREMEM_CONFIG     *TraceHubPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "------------------ PCH Trace Hub PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " EnableMode    : 0x%x\n", TraceHubPreMemConfig->TraceHub[PchTraceHub].EnableMode));
  DEBUG ((DEBUG_INFO, " MemReg0Size   : 0x%lx\n", TraceHubPreMemConfig->TraceHub[PchTraceHub].MemReg0Size));
  DEBUG ((DEBUG_INFO, " MemReg1Size   : 0x%lx\n", TraceHubPreMemConfig->TraceHub[PchTraceHub].MemReg1Size));
  DEBUG ((DEBUG_INFO, " AetEnabled    : 0x%x\n", TraceHubPreMemConfig->TraceHub[PchTraceHub].AetEnabled));
  DEBUG ((DEBUG_INFO, " BiosTraceSink : 0x%x\n", TraceHubPreMemConfig->TraceHub[PchTraceHub].BiosTraceSink));
}

/**
  Print SOC Trace Hub Config

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
SocTraceHubPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  TRACE_HUB_PREMEM_CONFIG     *TraceHubPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "EnableMode : 0x%x\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode));
  DEBUG ((DEBUG_INFO, "MemReg0Size : 0x%lx\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size));
  DEBUG ((DEBUG_INFO, "MemReg1Size : 0x%lx\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size));
  DEBUG ((DEBUG_INFO, "AetEnabled : 0x%x\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].AetEnabled));
  DEBUG ((DEBUG_INFO, "BiosTraceSink : 0x%x\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].BiosTraceSink));
}

/**
  Load Trace Hub Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TraceHubPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  TRACE_HUB_PREMEM_CONFIG  *TraceHubPreMemConfig;
  TraceHubPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "TraceHubPreMemConfig->Header.GuidHob.Name = %g\n", &TraceHubPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "TraceHubPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", TraceHubPreMemConfig->Header.GuidHob.Header.HobLength));

  TraceHubPreMemConfig->TraceHub[SocTraceHub].AetEnabled = TRUE;
  TraceHubPreMemConfig->TraceHub[SocTraceHub].BiosTraceSink = TRUE;
}

STATIC COMPONENT_BLOCK_ENTRY  mTraceHubPreMemBlocks = {
  &gTraceHubPreMemConfigGuid,
  sizeof (TRACE_HUB_PREMEM_CONFIG),
  TRACE_HUB_PREMEM_CONFIG_REVISION,
  TraceHubPreMemLoadConfigDefault
};

/**
  Get Trace Hub config block table size.

  @retval      Size of config block
**/
UINT16
TraceHubPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return mTraceHubPreMemBlocks.Size;
}

/**
  Add Trace Hub ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
TraceHubPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mTraceHubPreMemBlocks, 1);
}