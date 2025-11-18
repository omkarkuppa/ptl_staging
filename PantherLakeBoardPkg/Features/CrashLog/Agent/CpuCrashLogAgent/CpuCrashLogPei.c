/** @file
  CPU CrashLog PEI driver

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

#include "CpuCrashLogPei.h"

CPU_CRASHLOG_AGENT_PRIVATE_DATA  *CpuAgent;

/**
  Initialization flow of crashlog agent.

  @retval EFI_SUCCESS  The operation succeeds.
  @retval others       The operation failed due to some error.

**/
EFI_STATUS
CrashlogAgentInit (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT32      DiscoveryTblOffset;

  //
  // Locate crashlog DVSEC
  //
  Status = GetCrashLogDvsec (&(CpuAgent->DvsecStruct));
  if(EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Crashlog DVSEC failed: %r\n", Status));
    return Status;
  }

  //
  // Program BAR address and enable command register memory space decoding
  //
  CpuAgent->Device10MmioBase = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
  Status = EnableDevice10Mmio (
             &(CpuAgent->Device10MmioBase),
             CpuAgent->DvsecStruct.Fields.TBir,
             &(CpuAgent->SavedPciCmdSts),
             &(CpuAgent->MmioInitByThisInstance));
  if(EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Enable device 10 MMIO failed: %r\n", Status));
    return Status;
  }

  //
  // Get the crashlog discovery table
  //
  DiscoveryTblOffset = CpuAgent->DvsecStruct.Fields.DiscoveryTableOffset;
  DiscoveryTblOffset = DiscoveryTblOffset << 3;
  CpuAgent->DiscoveryTblAddr = CpuAgent->Device10MmioBase + DiscoveryTblOffset;
  Status = AccessMmioDword (
             &(CpuAgent->DiscoveryTbl),
             CpuAgent->DiscoveryTblAddr,
             sizeof (DISCOVERY_STRUCT_VER_1)/ sizeof(UINT32),
             READ);
  if(EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get crashlog discovery table failed: %r\n", Status));
    return Status;
  }

  //
  // Print discovery table
  //
  PrintDiscoveryTbl ();

  //
  // Check if crash type is client
  //
  if (CpuAgent->DiscoveryTbl.Header.DW0.CrashType != CRASH_TYPE_CLIENT) {
    DEBUG ((DEBUG_ERROR, "error: crash type does not match.\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Check if crashlog was not supproted or disabled status was set
  //
  Status = AccessMmioDword (
             &(CpuAgent->CapSts),
             CpuAgent->Device10MmioBase + R_CDIE_CAPABILITY_OFFSET,
             sizeof (CAPABILITY_STATUS_PHASE_1)/ sizeof(UINT32),
             READ);
  if (EFI_ERROR (Status) ||
      (CpuAgent->CapSts.Avail == 0) ||
      (CpuAgent->CapSts.DisSts == 1) ||
      (CpuAgent->CapSts.Mech != CRASHLOG_MECH_DVSEC)) {
    DEBUG ((DEBUG_ERROR, "error: Crashlog was not supported.\n"));
    return EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Uninitialization flow of crashlog agent.

  @retval EFI_SUCCESS  The operation succeeds.
  @retval others       The operation failed due to some error.

**/
EFI_STATUS
CrashlogAgentUnInit (
  VOID
  )
{
  //
  // Restore Device 10 SRAM MMIO
  //
  if (CpuAgent->MmioInitByThisInstance) {
    RestoreDevice10Mmio (CpuAgent->DvsecStruct.Fields.TBir, CpuAgent->SavedPciCmdSts);
  }

  FreePool (CpuAgent);
  return EFI_SUCCESS;
}

/**
  Crash record extraction.

  @param[in]  CrashNode             Indicate which crash node record to be extracted
  @param[out] RecordCount           Count of records
  @param[out] CpuCrashEfiPageCount  Count of EFI Pages allocated for all Cpu Crash records
  @param[out] Records               Crashlog record

  @retval EFI_SUCCESS  The operation succeeds.
  @retval others       The operation failed.

**/
EFI_STATUS
CrashlogAgentExtract (
  IN  UINT32                     CrashNode,
  OUT UINT32                     *RecordCount,
  OUT UINT32                     *CpuCrashEfiPageCount,
  OUT CRASHLOG_HOB_RECORD_ENTRY  *Records OPTIONAL
  )
{
  EFI_STATUS                 Status;
  EFI_PHYSICAL_ADDRESS       Address;
  CAPABILITY_STATUS_PHASE_2  ClStatus;

  DEBUG ((DEBUG_INFO, "Cpu CrashLogExtraction - start CrashNode= %X\n", CrashNode));

  if (RecordCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if CrashLog Record already been consumed by BIOS
  //
  Status = AccessMmioDword (
             &(ClStatus),
             CpuAgent->DiscoveryTblAddr,
             sizeof (CAPABILITY_STATUS_PHASE_2)/ sizeof(UINT32),
             READ);
  if (EFI_ERROR (Status) || ClStatus.ClConsumed == 1) {
    DEBUG ((DEBUG_INFO, "CrashLog Record has already been consumed by BIOS\n"));
    return EFI_NOT_FOUND;
  }

  *RecordCount = 0;
  *CpuCrashEfiPageCount = 0;
  //
  // Get crash record information from SSRAM
  //
  Status = UpdateCrashRecordInfo ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *RecordCount = CpuAgent->ValidRecordCount;

  //
  // Copy crash record from SSRAM to memory
  //
  if (Records != NULL) {
    if (CpuAgent->CrashRecordInfo.Valid) {
      Status = PeiServicesAllocatePages (
                  EfiBootServicesData,
                  EFI_SIZE_TO_PAGES (CpuAgent->CrashRecordInfo.Size),
                  &Address
                  );
      if (!EFI_ERROR (Status)) {
        AccessMmioDword (
          (VOID *)(UINTN)Address,
          CpuAgent->CrashRecordInfo.Addr,
          CpuAgent->CrashRecordInfo.Size / sizeof (UINT32),
          READ);
        Records->Address = (UINTN)Address;
        Records->Size = CpuAgent->CrashRecordInfo.Size;
        Records ++;
        *CpuCrashEfiPageCount += EFI_SIZE_TO_PAGES (CpuAgent->CrashRecordInfo.Size);
      } else {
        DEBUG ((DEBUG_ERROR, "failed to allocate enough memory store crashlog data.\n"));
        return Status;
      }
    }
    CpuAgent->RecordCollected = TRUE;
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
  //
  // Check if set crashlog disabled
  if (CpuAgent->ControlConfig.Enable == 0) {
    if (CpuAgent->CapSts.Dis == 1) {
      CpuCrashLogMailboxCommand (V_CPU_CRASHLOG_CMD_DISABLE, 0);
    }

  } else {
    //
    // Check if BIOS collect Crashlog record
    //
    if (CpuAgent->RecordCollected) {
      CpuCrashLogMailboxCommand (V_CPU_CRASHLOG_CMD_CONSUMED, 0);
    }
    //
    // Check if re-arm trigger is selected
    //
    if ((CpuAgent->ControlConfig.ReArm == 1) && (CpuAgent->CapSts.ReArm == 1)) {
      CpuCrashLogMailboxCommand (V_CPU_CRASHLOG_CMD_REARM, 0);
    }
   //
    // Check if clear option is selected
    //
    if ((CpuAgent->ControlConfig.Clr == 1) && (CpuAgent->CapSts.Clr == 1)) {
      CpuCrashLogMailboxCommand (V_CPU_CRASHLOG_CMD_CLEAR, 0);
    }
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
CpuCrashLogPeimEntry (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                       Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  UINTN                            VariableSize;
  CRASHLOG_VARIABLE                Variable;
  CRASHLOG_RECORD_PAGES            CrashRecordEfiPagesVariable;
  EFI_BOOT_MODE                    BootMode;
  UINT32                           RecordCount;
  CRASHLOG_HOB_DATA                *CrashlogHob;
  CRASHLOG_HOB_RECORD_ENTRY        *CrashRecordEntry;
  CRASHLOG_CONTROL_VARIABLE        *CtlVariableHob;
  CRASHLOG_RECORD_PAGES            *CrashRecordPagesHob;
  EFI_HOB_GUID_TYPE                *GuidHob;
  UINT32                           CpuCrashEfiPageCount;
  EFI_PHYSICAL_ADDRESS             Address;

  DEBUG ((DEBUG_INFO, "%a() - enter\n", __FUNCTION__));

  CrashlogHob = NULL;
  RecordCount = 0;
  CpuCrashEfiPageCount = 0;
  //
  //  Skip the flow if boot mode is S3
  //
  Status = PeiServicesGetBootMode (&BootMode);
  if ((EFI_ERROR (Status)) || (BootMode == BOOT_ON_S3_RESUME)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Get crashlog variable and update local control config variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,  // GUID
             0,                                 // INSTANCE
             NULL,                              // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices        // PPI
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to locate variable ppi. (%r)\n", Status));
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
    DEBUG ((DEBUG_ERROR, "Fail to get setup variable. (%r)\n", Status));
    return Status;
  }

  //
  // Memory allocation for private data
  //
  CpuAgent = AllocateZeroPool (sizeof (CPU_CRASHLOG_AGENT_PRIVATE_DATA));
  if (CpuAgent == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to allocate private data structure.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  CpuAgent->ControlConfig.Enable        = Variable.EnableCrashLog;
  CpuAgent->ControlConfig.Clr           = Variable.CpuClear;
  CpuAgent->ControlConfig.ReArm         = Variable.CpuRearm;

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
    Status = CrashlogAgentExtract (CRASHLOG_NODE_ALL, &RecordCount, &CpuCrashEfiPageCount, NULL);
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
        CrashRecordEntry = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN)CrashlogHob + sizeof (CRASHLOG_HOB_DATA));
        Status = CrashlogAgentExtract (CRASHLOG_NODE_ALL, &RecordCount, &CpuCrashEfiPageCount, CrashRecordEntry);
        if (EFI_ERROR (Status)) {
          ZeroMem (CrashRecordEntry, sizeof(CRASHLOG_HOB_RECORD_ENTRY) * RecordCount);
          RecordCount = 0;
        }

        CrashlogHob->RecordCount = RecordCount;
        CrashlogHob->CrashlogAgent = CrashlogAgentCpu;
      }
    } else if (BootMode == BOOT_ON_S4_RESUME) {
      VariableSize = sizeof (CRASHLOG_RECORD_PAGES);
      Status = VariableServices->GetVariable (
                                   VariableServices,
                                   CRASHLOG_RECORDPAGES_NAME,
                                   &gCrashLogRecordPagesGuid,
                                   NULL,
                                   &VariableSize,
                                   &CrashRecordEfiPagesVariable
                                   );
      if (!EFI_ERROR (Status) && (CrashRecordEfiPagesVariable.CpuCrashEfiPageCount != 0)) {
        Status = PeiServicesAllocatePages (
                   EfiBootServicesData,
                   CrashRecordEfiPagesVariable.CpuCrashEfiPageCount,
                   &Address
                   );
        if (!EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Successfully allocate %d pages during S4 resume for earlier CPU crashlog data.\n", CrashRecordEfiPagesVariable.CpuCrashEfiPageCount));
        } else {
          DEBUG ((DEBUG_ERROR, "failed to allocate enough memory during S4 resume for earlier CPU crashlog data.\n"));
          return Status;
        }
        CpuCrashEfiPageCount = CrashRecordEfiPagesVariable.CpuCrashEfiPageCount;
      }
    } else {
      CpuCrashEfiPageCount = 0;
    }
  }

  //
  // Set command interface based on SETUP options and capability
  //
  CrashlogAgentSendCommand ();
  
  //
  // Set the cpu crash records efi page count
  //
  GuidHob = NULL;
  GuidHob = GetFirstGuidHob (&gCrashLogRecordPagesGuid);
  if (GuidHob != NULL) {
    CrashRecordPagesHob = (CRASHLOG_RECORD_PAGES *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    DEBUG ((DEBUG_INFO, "Create Crash Record Pages Hob\n"));
    CrashRecordPagesHob = (CRASHLOG_RECORD_PAGES *)BuildGuidHob (
                                         &gCrashLogRecordPagesGuid,
                                         sizeof (CRASHLOG_RECORD_PAGES)
                                         );
    ZeroMem (CrashRecordPagesHob, sizeof (CRASHLOG_RECORD_PAGES));
  }
  if (CrashRecordPagesHob != NULL){
    CrashRecordPagesHob->CpuCrashEfiPageCount = CpuCrashEfiPageCount;
  }

  GuidHob = NULL;
  //
  // Set the crashlog control variable
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
    CtlVariableHob->CpuClear = (CpuAgent->CapSts.Clr == 1) ? 1 : 0;
    CtlVariableHob->CpuRearm = (CpuAgent->CapSts.ReArm == 1) ? 1 : 0;
  }

Exit:
  CrashlogAgentUnInit ();

  DEBUG ((DEBUG_INFO, "%a() - exit\n", __FUNCTION__));

  return Status;
}
