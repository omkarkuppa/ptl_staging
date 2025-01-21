/** @file
  This code provides an instance of Pei PSMI Init Fru Lib.

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
#include <Ppi/SiPolicy.h>
#include <Library/PciSegmentLib.h>
#include <Library/HobLib.h>
#include <Register/TraceHubRegs.h>
#include <Library/PeiPsmiInitFruLib.h>
#include <Defines/PchReservedResources.h>
#include <Library/PchPciBdfLib.h>

/**
  Override policies relevant to PCH Trace Hub to enable PSMI debug in PreMem.

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
PsmiTraceHubPolicyOverride (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                           Status;
  TRACE_HUB_PREMEM_CONFIG              *TraceHubPreMemConfig;
  PSMI_DATA_HOB_VER1                   *PsmiDataHob;

  TraceHubPreMemConfig               = NULL;
  PsmiDataHob                        = NULL;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  PsmiDataHob = (PSMI_DATA_HOB_VER1 *)GetFirstGuidHob (&gPsmiDataHobGuid);
  if (PsmiDataHob != NULL) {
  //
  // PCH Trace Hub settings for PSMI (Force HostDebugger Mode)
  //
    if (PsmiDataHob->PsmiConfig.HandlerInfo.HandlerSize != 0) {

      DEBUG ((DEBUG_INFO, "PSMI Force SOC Trace Hub in HostDebugger mode \n"));
      DEBUG ((DEBUG_INFO, "Override SOC Trace Hub MemSize 0/1 to 0 \n"));

      TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size = 0;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size = 0;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode  = TraceHubModeEnabled;

      DEBUG ((DEBUG_INFO, "TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode = %x\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode));
      DEBUG ((DEBUG_INFO, "TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size = %lx\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size));
      DEBUG ((DEBUG_INFO, "TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size = %lx\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size));
    }
  }
}

/**
  Returns the Base address of PCH Trace Hub

  @retval PCH Trace Hub Base Address
**/
UINT32
GetTraceHubBaseAddress (
  VOID
  )
{
  return (UINT32) PchTraceHubPciCfgBase ();
}

/**
  Returns the Base address of Trace Hub MTB Bar

  @retval Tracehub MTB Bar base address
**/
UINT32
GetTraceHubMtbBarBaseAddress (
  VOID
  )
{
  return PCH_TRACE_HUB_MTB_BASE_ADDRESS;
}
