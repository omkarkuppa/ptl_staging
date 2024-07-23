/** @file
  SOC PMC CrashLog PEI driver

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

#include "SocCrashLogPei.h"


SOC_CRASHLOG_AGENT_PRIVATE_DATA          *SocAgent;

/**
  Initialization flow of crashlog agent.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval others                   The operation failed due to some error.

**/
EFI_STATUS
CrashlogAgentInit (
  VOID
  )
{
  EFI_STATUS  Status;

  //
  // Program BAR address and enable command register memory space decoding
  //
  SocAgent->PmcSsramMmioBase = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
  Status = EnablePmcSsramMmio (&(SocAgent->PmcSsramMmioBase), &(SocAgent->PmcSavedPciCmdSts), &(SocAgent->PmcMmioInitByThisInstance));
  if(EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, " %a - Enable PMC SSRAM failed: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Get the crashlog discovery table and update descriptor table
  //
  Status = GetPmcDiscoveryTbl ();
  if(EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, " %a - Get crashlog discovery table failed: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Print discovery table
  //
  PrintDiscoveryTbl ();

  //
  // Check if crashlog disabled status set
  //
  if ((SocAgent->CapStsPtr->Avail == 0) || (SocAgent->CapStsPtr->DisSts == 1)) {
    DEBUG ((DEBUG_ERROR, " %a - Crashlog is unsupported(%d) or disabled(%d).\n", __FUNCTION__, SocAgent->CapStsPtr->Avail, SocAgent->CapStsPtr->DisSts));
    return EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Uninitialization flow of crashlog agent.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval others                   The operation failed due to some error.

**/
EFI_STATUS
CrashlogAgentUnInit (
  VOID
  )
{
  //
  // Restore PMC SSRAM MMIO
  //
  if (SocAgent->PmcMmioInitByThisInstance) {
    RestorePmcSsramMmio (SocAgent->PmcSavedPciCmdSts);
  }
  //
  // Restore IOE SSRAM MMIO
  //
  if (SocAgent->IoeMmioInitByThisInstance) {
    RestoreIoeSsramMmio ();
  }
  //
  // Reset private data
  //
  if (SocAgent->DescTbl != NULL) {
    FreePool (SocAgent->DescTbl);
  }
  if (SocAgent->CrashRecordInfo != NULL) {
    FreePool (SocAgent->CrashRecordInfo);
  }

  FreePool (SocAgent);

  return EFI_SUCCESS;
}

/**
  Get Crash record information.
  if Records is not NULL, extract the data from SSRAM.

  @param[in] CrashNode     Indicate which crash node record to be extracted
  @param[out] RecordCount  Count of records
  @param[out] Records      Crashlog record

  @retval EFI_SUCCESS  The operation succeeds.
  @retval others       The operation failed dur to some error.

**/
EFI_STATUS
CrashlogAgentExtract (
  IN  UINT32                     CrashNode,
  OUT UINT32                     *RecordCount,
  OUT CRASHLOG_HOB_RECORD_ENTRY  *Records OPTIONAL
  )
{
  EFI_STATUS            Status;
  UINT32                Index;
  EFI_PHYSICAL_ADDRESS  Address;

  DEBUG ((DEBUG_INFO, "Soc CrashLogExtraction - start CrashNode=%X\n", CrashNode));

  if (RecordCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *RecordCount = 0;

  //
  // Get crash record information from SSRAM
  //
  if (SocAgent->CrashRecordInfo == NULL) {
    Status = InitCrashRecordInfo ();
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (SocAgent->CrashRecordInfo == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  *RecordCount = SocAgent->ValidRecordCount;

  //
  // Copy crash record from SSRAM to memory
  //
  if (Records != NULL) {
    for (Index = 0; Index < SocAgent->DescTbl->NumOfRegions; Index++) {
      if (SocAgent->CrashRecordInfo[Index].Valid) {
        Status = PeiServicesAllocatePages (
                   EfiBootServicesData,
                   EFI_SIZE_TO_PAGES (SocAgent->CrashRecordInfo[Index].Size),
                   &Address
                   );
        if (!EFI_ERROR (Status)) {
          AccessMmioDword ((VOID *)(UINTN)Address, SocAgent->CrashRecordInfo[Index].Addr, SocAgent->CrashRecordInfo[Index].Size / sizeof (UINT32), READ);
          Records->Address = (UINTN)Address;
          Records->Size = SocAgent->CrashRecordInfo[Index].Size;
          Records ++;
        } else {
          DEBUG ((DEBUG_ERROR, "failed to allocate enough memory store crashlog data.\n"));
          return Status;
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Crash agent set control interface.

  @retval EFI_SUCCESS  The operation succeeds.
  @retval others       The operation failed due to some error.

**/
EFI_STATUS
CrashlogAgentSendCommand (
  VOID
  )
{
  //
  // Send disable command to PMC if disable command is supported, skip other flow
  //
  if (SocAgent->ControlConfig.Enable == 0) {
    if (SocAgent->CapStsPtr->Dis == 1) {
      DEBUG ((DEBUG_INFO, " send PMC crashlog disable command\n"));
      PmcCrashLogDisable ();
    }
    return EFI_SUCCESS;
  }

  //
  // Re-arm command and clear command should only send if bios find valid crashlog
  //
  if (SocAgent->ValidRecordCount > 0) {
    if ((SocAgent->ControlConfig.ReArm == 1) && (SocAgent->CapStsPtr->ReArm == 1)) {
      DEBUG ((DEBUG_INFO, " send PMC re-arm command\n"));
      PmcCrashLogReArm ();
    }

    if ((SocAgent->ControlConfig.Clr == 1) && (SocAgent->CapStsPtr->Clr == 1)) {
      DEBUG ((DEBUG_INFO, " send PMC clear command\n"));
      PmcCrashLogClear ();
    }
  }

  //
  // Send crahslog on all reset command to PMC
  //
  if((SocAgent->ControlConfig.AllReset == 1) && (SocAgent->CapStsPtr->TriAllRst == 1)) {
    DEBUG ((DEBUG_INFO, " send PMC trigger on all reset command\n"));
    PmcCrashLogOnAllReset ();
  }

  //
  // Send crahslog command to PMC to program new global reset trigger mask value
  //
  if(SocAgent->CapStsPtr->GlbRstTriMsk == 1) {
    PmcCrashlogSetGblRstMask ();
  }

  return EFI_SUCCESS;
}

/**
  Entry point of the PEIM.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @retval EFI_SUCCESS  PPI successfully installed.

**/
EFI_STATUS
EFIAPI
SocCrashLogPeimEntry (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                       Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  UINTN                            VariableSize;
  CRASHLOG_VARIABLE                Variable;
  EFI_BOOT_MODE                    BootMode;

  UINT32                           RecordCount;
  CRASHLOG_HOB_DATA                *CrashlogHob;
  CRASHLOG_HOB_RECORD_ENTRY        *CrashRecordEntry;
  CRASHLOG_CONTROL_VARIABLE        *CtlVariableHob;
  CRASHLOG_VARIABLE                *VariableHob;
  EFI_HOB_GUID_TYPE                *GuidHob;

  CrashlogHob = NULL;
  RecordCount = 0;

  DEBUG ((DEBUG_INFO, "%a() - enter\n", __FUNCTION__));

  //
  //  Skip the flow if boot mode is S3
  //
  Status = PeiServicesGetBootMode (&BootMode);
  if (EFI_ERROR (Status) || (BootMode == BOOT_ON_S3_RESUME)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Get crashlog variable and update local control config variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,  // GUID
             0,                                 // INSTANCE
             NULL,                              // EFI_PEI_PPI_DESCRIPTOR
             (VOID **)&VariableServices         // PPI
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate variable ppi. (%r)\n", Status));
    return Status;
  }

  VariableSize = sizeof (CRASHLOG_VARIABLE);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               CRASHLOG_VARIABLE_NAME,
                               &gCrashLogVariableGuid,
                               NULL,
                               &VariableSize,
                               &Variable
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get setup variable. (%r)\n",Status));
    return Status;
  }

  //
  // Memory allocation for private data
  //
  SocAgent = AllocateZeroPool (sizeof (SOC_CRASHLOG_AGENT_PRIVATE_DATA));
  if (SocAgent == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate private data structure.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  SocAgent->ControlConfig.Enable   = Variable.EnableCrashLog;
  SocAgent->ControlConfig.Clr      = Variable.SiliconClear;
  SocAgent->ControlConfig.ReArm    = Variable.SiliconRearm;
  SocAgent->ControlConfig.AllReset = Variable.SiliconOnAllReset;
  SocAgent->ControlConfig.GblRst   = Variable.SiliconGblRst;
  SocAgent->GblRstMask0            = Variable.SiliconGblRstMask0;
  SocAgent->GblRstMask1            = Variable.SiliconGblRstMask1;

  //
  // Crash Log collection flow start
  //
  Status = CrashlogAgentInit ();
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Get crashlog record if crashlog option is enabled
  //
  if (Variable.EnableCrashLog) {
    //
    // Get crash record count from crashlog agent
    //
    Status = CrashlogAgentExtract (CRASHLOG_NODE_ALL, &RecordCount, NULL);
    //
    // Build of location for Crash log in Hob
    //
    if (!EFI_ERROR (Status) && (RecordCount != 0)) {
      //
      // Build of location for Crash log in Hob
      //
      CrashlogHob = (CRASHLOG_HOB_DATA *)BuildGuidHob (
                                           &gCrashLogRecordHobGuid,
                                           sizeof (CRASHLOG_HOB_DATA) + sizeof (CRASHLOG_HOB_RECORD_ENTRY) * RecordCount
                                           );
      if (CrashlogHob != NULL) {
        //
        // Extract crash record to HOB
        //
        CrashRecordEntry = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN) CrashlogHob + sizeof (CRASHLOG_HOB_DATA));
        Status = CrashlogAgentExtract (CRASHLOG_NODE_ALL, &RecordCount, CrashRecordEntry);
        if (EFI_ERROR (Status)) {
          ZeroMem (CrashRecordEntry, sizeof(CRASHLOG_HOB_RECORD_ENTRY) * RecordCount);
          RecordCount = 0;
        }

        CrashlogHob->RecordCount = RecordCount;
        CrashlogHob->CrashlogAgent = CrashlogAgentSoc;
      }

    }
  }

  //
  // Set command interface based on SETUP options and capability
  //
  CrashlogAgentSendCommand ();

  //
  // Set the variable
  //
  GuidHob = NULL;
  GuidHob = GetFirstGuidHob (&gCrashLogVariableGuid);
  if (GuidHob != NULL) {
    VariableHob = (CRASHLOG_VARIABLE *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    DEBUG ((DEBUG_INFO, "Create VariableHob\n"));
    VariableHob = (CRASHLOG_VARIABLE *)BuildGuidHob (
                                         &gCrashLogVariableGuid,
                                         sizeof (CRASHLOG_VARIABLE)
                                         );
    ZeroMem (VariableHob, sizeof (CRASHLOG_VARIABLE));
  }
  if (VariableHob != NULL) {
    CopyMem (VariableHob, &Variable, sizeof (CRASHLOG_VARIABLE));

    VariableHob->SiliconGblRstMask0 = SocAgent->GblRstMask0;
    VariableHob->SiliconGblRstMask1 = SocAgent->GblRstMask1;
    if (SocAgent->ControlConfig.GblRst == 1) {
      if ((SocAgent->GblRstMask0 == 0xFFFFFFFF) && (SocAgent->GblRstMask1 == 0xFFFFFFFF)) {
        VariableHob->SiliconGblRst = 0;
      }
    }
  }

  //
  // Set the control variable
  //
  GuidHob = GetFirstGuidHob (&gCrashLogCtlVariableGuid);
  if (GuidHob != NULL) {
    CtlVariableHob = (CRASHLOG_CONTROL_VARIABLE *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    DEBUG ((DEBUG_INFO, "Create CtlVariableHob\n"));
    CtlVariableHob = (CRASHLOG_CONTROL_VARIABLE *)BuildGuidHob (
                                                    &gCrashLogCtlVariableGuid,
                                                    sizeof (CRASHLOG_CONTROL_VARIABLE)
                                                    );
    ZeroMem (CtlVariableHob, sizeof (CRASHLOG_CONTROL_VARIABLE));
  }
  if (CtlVariableHob != NULL) {
    CtlVariableHob->SiliconClear      = (SocAgent->CapStsPtr->Clr == 1) ? 1 : 0;
    CtlVariableHob->SiliconOnAllReset = (SocAgent->CapStsPtr->TriAllRst == 1) ? 1 : 0;
    CtlVariableHob->SiliconRearm      = (SocAgent->CapStsPtr->ReArm == 1) ? 1 : 0;
    CtlVariableHob->SiliconGblRst     = (SocAgent->CapStsPtr->GlbRstTriMsk == 1) ? 1 : 0;
  }

Exit:
  CrashlogAgentUnInit ();
  DEBUG ((DEBUG_INFO, "%a() - exit\n", __FUNCTION__));

  return Status;
}

