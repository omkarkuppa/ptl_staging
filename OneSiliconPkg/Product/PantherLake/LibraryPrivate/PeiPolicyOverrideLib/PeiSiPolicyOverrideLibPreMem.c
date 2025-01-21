/** @file
  Provide polciy override functions in PreMem to enable specific silicon feature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiSiPolicyOverrideLib.h>
#include <Library/PeiPsmiInitFruLib.h>
#include <Library/PeiMeLib.h>
#include <Library/HobLib.h>
#include <DebugTokenDataHob.h>
#include <Library/PeiTraceHubInitLib.h>
#include <Defines/PchReservedResources.h>
#include <Library/CpuPlatformLib.h>
#include <DciConfig.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <Library/DebugToken.h>

/*
  Get Debug Token Data and save it in the HOB

  @param[out]  DebugToken       Pointer to Token data

  @retval      TRUE             Debug Token is existed
  @retval      FALSE            Debug Token is not existed
*/
STATIC
BOOLEAN
GetDebugTokenData (
  OUT DEBUG_TOKEN_KNOB_DATA     *DebugToken
  )
{
  EFI_STATUS                     Status;
  DEBUG_TOKEN_DATA_HOB           *DebugTokenDataHob;

  Status = PeiHeciGetDebugTokenData ((UINT32 *)DebugToken);
  if (!EFI_ERROR (Status)) {
    //
    // check if StreamingTraceSink is valid or roll it back to setup menu default
    //
    if ((DebugToken->Fields.StreamingTraceSink != SetupMenuDefault) && (DebugToken->Fields.StreamingTraceSink > ForceDisabled)) {
      DEBUG ((DEBUG_INFO, "Invalid Debug Token setting : StreamingTraceSink = %x, PlatformDebugOption will not be overriden but comply to setup menu\n", DebugToken->Fields.StreamingTraceSink));
      DebugToken->Fields.StreamingTraceSink = SetupMenuDefault;
    }
    ///
    /// Create HOB for Debug Token Data
    ///
    DebugTokenDataHob = BuildGuidHob (&gDebugTokenDataHobGuid, sizeof (DEBUG_TOKEN_DATA_HOB));
    ASSERT (DebugTokenDataHob != 0);
    if (DebugTokenDataHob != NULL) {
      ///
      /// Initialize default HOB data
      ///
      DebugTokenDataHob->EfiHobGuidType.Name = gDebugTokenDataHobGuid;
      DebugTokenDataHob->DebugTokenData = DebugToken->Data;
    } else {
      DEBUG ((DEBUG_INFO, "Failed to create Debug Token Data Hob\n"));
    }
    DEBUG ((DEBUG_INFO, "DebugToken Fields\n"));
    DEBUG ((DEBUG_INFO, "ExposeDebugMenu = %x\n", DebugToken->Fields.ExposeDebugMenu));
    DEBUG ((DEBUG_INFO, "StreamingTraceSink = %x\n", DebugToken->Fields.StreamingTraceSink));
    DEBUG ((DEBUG_INFO, "ProcessorTraceEnable = %x\n", DebugToken->Fields.ProcessorTraceEnable));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, "No BIOS payload present in system\n"));
    return FALSE;
  }
}

/**
  Override policies relevant to PlatformDebugOption to enable debug in PreMem.

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
STATIC
VOID
PlatformDebugOptionEnablePreMem (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                           Status;
  SI_PREMEM_CONFIG                     *SiPreMemConfig;
  PCH_DCI_PREMEM_CONFIG                *DciPreMemConfig;
  TRACE_HUB_PREMEM_CONFIG              *TraceHubPreMemConfig;
  CPU_INIT_PREMEM_CONFIG               *CpuInitPreMemConfig;
  PCH_GENERAL_PREMEM_CONFIG            *PchGeneralPreMemConfig;
  MEMORY_CONFIG_NO_CRC                 *MemConfigNoCrc;
  DEBUG_TOKEN_KNOB_DATA                DebugToken;
  BOOLEAN                              DebugTokenExist;
  UINT64                               ProcessorTraceTotalMemSize;
  UINT32                               ThreadCount;
  BOOLEAN                              SocThInUse;
  BOOLEAN                              PchThInUse;

  SiPreMemConfig         = NULL;
  DciPreMemConfig        = NULL;
  TraceHubPreMemConfig   = NULL;
  CpuInitPreMemConfig    = NULL;
  PchGeneralPreMemConfig = NULL;
  MemConfigNoCrc         = NULL;
  DebugTokenExist        = FALSE;
  ThreadCount            = 0;
  ProcessorTraceTotalMemSize = 0;
  PchThInUse             = FALSE;
  DebugToken.Data        = 0;

  SocThInUse = DebuggerInUseSoc ();

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSiPreMemConfigGuid, (VOID *) &SiPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gDciPreMemConfigGuid, (VOID *) &DciPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR(Status);
  //
  // Get Debug Token
  //
  DebugTokenExist = GetDebugTokenData (&DebugToken);
  if (DebugTokenExist) {
    if (DebugToken.Fields.StreamingTraceSink != SetupMenuDefault) {
      switch (DebugToken.Fields.StreamingTraceSink) {
        case TraceActive:
          SiPreMemConfig->PlatformDebugOption = EnabledTraceActive;
          break;

        case TraceReady:
          SiPreMemConfig->PlatformDebugOption = EnabledTraceReady;
          break;

        case TracePowerOff:
          SiPreMemConfig->PlatformDebugOption = EnabledTracePowerOff;
          break;

        case ForceDisabled:
          SiPreMemConfig->PlatformDebugOption = DisableDebug;
          break;
      }
      DEBUG ((DEBUG_INFO, "Token overrides PlatformDebugOption to %x\n", SiPreMemConfig->PlatformDebugOption));
    }
  }

  switch (SiPreMemConfig->PlatformDebugOption) {
    case DisableDebug:
      DciPreMemConfig->DciEn                                   = FALSE;
      DciPreMemConfig->DciClkEnable                            = TRUE;
      DciPreMemConfig->DciDbcMode                              = DciDbcUsb2;
      DciPreMemConfig->KeepEarlyTrace                          = KetDisabled;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode   = TraceHubModeDisabled;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size  = 0;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size  = 0;

      PchGeneralPreMemConfig->IoeDebugEn                       = FALSE;
      PchGeneralPreMemConfig->PmodeClkEn                       = FALSE;
      CpuInitPreMemConfig->DfdEnable                           = FALSE;
      break;

    case EnabledTracePowerOff:
      DciPreMemConfig->DciEn                                   = TRUE;
      DciPreMemConfig->DciClkEnable                            = FALSE;
      DciPreMemConfig->DciDbcMode                              = DciDbcUsb2;
      DciPreMemConfig->KeepEarlyTrace                          = KetDisabled;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode   = TraceHubModeDisabled;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size  = 0;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size  = 0;

      PchGeneralPreMemConfig->IoeDebugEn                       = FALSE;
      PchGeneralPreMemConfig->PmodeClkEn                       = FALSE;
      CpuInitPreMemConfig->DfdEnable                           = TRUE;
      break;

    case EnabledTraceReady:
      DciPreMemConfig->DciEn                                   = TRUE;
      DciPreMemConfig->DciClkEnable                            = TRUE;
      DciPreMemConfig->DciDbcMode                              = DciDbcUsb2;
      DciPreMemConfig->KeepEarlyTrace                          = KetDisabled;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode   = TraceHubModeEnabled;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size  = TRACEHUB_MEM_SIZE (TraceBuffer8M);
      TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size  = TRACEHUB_MEM_SIZE (TraceBuffer8M);
      PchGeneralPreMemConfig->IoeDebugEn                       = TRUE;
      PchGeneralPreMemConfig->PmodeClkEn                       = TRUE;
      CpuInitPreMemConfig->DfdEnable                           = TRUE;
      break;

    case EnabledTraceActive:
      DciPreMemConfig->DciEn                                   = TRUE;
      DciPreMemConfig->DciClkEnable                            = TRUE;
      DciPreMemConfig->DciDbcMode                              = DciDbcBoth;
      DciPreMemConfig->KeepEarlyTrace                          = KetEnabled;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode   = TraceHubModeEnabled;
      TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size  = TRACEHUB_MEM_SIZE (TraceBuffer8M);
      TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size  = TRACEHUB_MEM_SIZE (TraceBuffer8M);
      PchGeneralPreMemConfig->IoeDebugEn                       = TRUE;
      PchGeneralPreMemConfig->PmodeClkEn                       = TRUE;
      CpuInitPreMemConfig->DfdEnable                           = TRUE;
      break;

    default:
      break;
  }

  if (SocThInUse) {
    TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode     = TraceHubModeEnabled;
    TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size    = TRACEHUB_MEM_SIZE (TraceBuffer8M);
    TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size    = TRACEHUB_MEM_SIZE (TraceBuffer8M);
  }

  //
  // When ITH tool connected (trace activated), BIOS does not configure KET, which is done by DTS tool.
  //
  if (SocThInUse || PchThInUse) {
    DciPreMemConfig->KeepEarlyTrace = KetNoChange;
  }

  if ((TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode == TraceHubModeDisabled) && !SocThInUse) {
    DciPreMemConfig->KeepEarlyTrace = KetDisabled;
  }


  if (DebugTokenExist && DebugToken.Fields.ProcessorTraceEnable != 0) {
    if (IsIntelProcessorTraceSupported ()) {
      ThreadCount = GetEnabledThreadCount ();
      //
      // memory size for each thread is 4K for debug token
      // ProcessorTraceTotalMemSize is mutiplied by 2 for adding aligned memory (overhead) for natural alignment.
      // please refer to InternalAllocateAlignedPages (), the overhead will be free when creating memory map, it won't waste
      //
      ProcessorTraceTotalMemSize = 2 * SIZE_4KB * ThreadCount;
      MemConfigNoCrc->PlatformMemorySize += ProcessorTraceTotalMemSize;
      DEBUG ((DEBUG_INFO, "PlatformMemorySize = %x, ProcessorTraceTotalMemSize = %x\n", MemConfigNoCrc->PlatformMemorySize, ProcessorTraceTotalMemSize));
    }
  }

  DEBUG ((DEBUG_INFO, "SiPreMemConfig->PlatformDebugOption = %x\n", SiPreMemConfig->PlatformDebugOption));
  DEBUG ((DEBUG_INFO, "DciPreMemConfig->DciEn = %x\n", DciPreMemConfig->DciEn));
  DEBUG ((DEBUG_INFO, "DciPreMemConfig->DciClkEnable = %x\n", DciPreMemConfig->DciClkEnable));
  DEBUG ((DEBUG_INFO, "DciPreMemConfig->DciDbcMode = %x\n", DciPreMemConfig->DciDbcMode));
  DEBUG ((DEBUG_INFO, "DciPreMemConfig->KeepEarlyTrace = %x\n", DciPreMemConfig->KeepEarlyTrace));
  DEBUG ((DEBUG_INFO, "TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode = %x\n",  TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode));
  DEBUG ((DEBUG_INFO, "TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size = %lx\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size));
  DEBUG ((DEBUG_INFO, "TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size = %lx\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size));
  DEBUG ((DEBUG_INFO, "TraceHubPreMemConfig->TraceHub[SocTraceHub].BiosTraceSink = %x\n", TraceHubPreMemConfig->TraceHub[SocTraceHub].BiosTraceSink));
  DEBUG ((DEBUG_INFO, "CpuInitPreMemConfig->DfdEnable = %x\n", CpuInitPreMemConfig->DfdEnable));
  DEBUG ((DEBUG_INFO, "PchGeneralPreMemConfig->IoeDebugEn = %x\n", PchGeneralPreMemConfig->IoeDebugEn));
  DEBUG ((DEBUG_INFO, "PchGeneralPreMemConfig->PmodeClkEn = %x\n", PchGeneralPreMemConfig->PmodeClkEn));
}

/*
  Initialize TraceHub Data HOB

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
*/
VOID
BuildTraceHubDataHob (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  )
{
  UINT32                               FwBase;

  FwBase = PCH_TRACE_HUB_FW_BASE_ADDRESS;
  TraceHubDataHobInit (SiPreMemPolicyPpi, FwBase);
}

/**
  Policy override in PreMem phase

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
PeiSiPolicyOverridePreMem (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  )
{
  DEBUG ((DEBUG_INFO, "PeiSiPolicyOverridePreMem ()\n"));
  PlatformDebugOptionEnablePreMem (SiPreMemPolicyPpi);
  PsmiTraceHubPolicyOverride (SiPreMemPolicyPpi);
  BuildTraceHubDataHob (SiPreMemPolicyPpi);
}