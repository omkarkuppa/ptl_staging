/** @file
  CrashLog DXE driver

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


#include "CrashLogDxe.h"


CRASHLOG_DXE_PRIVATE_DATA  mCrashlogDxe;

/**
  Get records from HobDataPtr and insert into BertTable.

**/
VOID
InsertRecordToBert (
  VOID
  )
{
  EFI_STATUS                 Status;
  UINT32                     HobIndex;
  UINT32                     Index;
  CRASHLOG_HOB_RECORD_ENTRY  *CrashRecord;

  for (HobIndex = 0; HobIndex < mCrashlogDxe.HobCount; HobIndex ++) {
    CrashRecord = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN)mCrashlogDxe.HobDataPtr[HobIndex] + sizeof (CRASHLOG_HOB_DATA));
    for (Index = 0; Index < mCrashlogDxe.HobDataPtr[HobIndex]->RecordCount; Index ++) {
      Status = InsertGenericErrorData (mCrashlogDxe.Bert, CrashRecord[Index].Address, CrashRecord[Index].Size);
      DEBUG ((DEBUG_INFO, " Wrtie %d bytes data from HOB[%d].Record[%d] to BERT table, status= %r\n", CrashRecord[Index].Size, HobIndex, Index, Status));
    }
  }

}

/**
  Initialize the APEI BERT table

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
InitBertTbl (
  VOID
  )
{
  EFI_STATUS                 Status;

  DEBUG ((DEBUG_INFO, "BERT Init\n"));

  //
  // Init the Bert Table
  //
  Status = InitBertTblHeader (&(mCrashlogDxe.Bert), mCrashlogDxe.PayloadSize, mCrashlogDxe.PayloadCount);
  if(EFI_ERROR(Status) || (mCrashlogDxe.Bert == NULL)) {
    return Status;
  }

  InsertRecordToBert ();

  return EFI_SUCCESS;
}

/**
  Calculate payload size in crash record.

  @param[in] HobData  Hob data of payload.

  @retval PayloadSize  Payload size.

**/
UINT32
GetPayloadSize (
  IN CRASHLOG_HOB_DATA  *HobData
  )
{
  UINT32                     Index;
  UINT32                     PayloadSize;
  CRASHLOG_HOB_RECORD_ENTRY  *CrashRecord;

  PayloadSize = 0;
  CrashRecord = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN)HobData + sizeof (CRASHLOG_HOB_DATA));

  for (Index = 0; Index < HobData->RecordCount; Index ++) {
    PayloadSize += CrashRecord[Index].Size;
  }

  return PayloadSize;
}

/**
  Get data from HOB and update private data structure

**/
VOID
GetPayloadInfoFromHob (
  VOID
  )
{
  UINT32             HobCount;
  UINT32             PayloadCount;
  UINT32             PayloadSize;
  EFI_HOB_GUID_TYPE  *GuidHob;
  CRASHLOG_HOB_DATA  *HobData;

  HobCount     = 0;
  PayloadCount = 0;
  PayloadSize  = 0;

  GuidHob = GetFirstGuidHob (&gCrashLogRecordHobGuid);

  while (GuidHob != NULL && HobCount < CrashlogAgentTypeMax) {
    HobData = (CRASHLOG_HOB_DATA *)GET_GUID_HOB_DATA (GuidHob);
    mCrashlogDxe.HobDataPtr[HobCount] = HobData;

    PayloadCount += HobData->RecordCount;
    PayloadSize  += GetPayloadSize (HobData);
    DEBUG ((DEBUG_INFO, " Find HOB %d, agent type= %d, current total payload count= %d, current total payload size= %d\n",
            HobCount, HobData->CrashlogAgent, PayloadCount, PayloadSize));

    GuidHob = GetNextGuidHob (&gCrashLogRecordHobGuid, GET_NEXT_HOB (GuidHob));
    HobCount ++;
  }

  mCrashlogDxe.HobCount     = HobCount;
  mCrashlogDxe.PayloadCount = PayloadCount;
  mCrashlogDxe.PayloadSize  = PayloadSize;
}

/**
  Locate AcpiTbl Protocol.

  @retval  AcpiTblProtocol  Pointer of EFI_ACPI_TABLE_PROTOCOL instance.

**/
EFI_ACPI_TABLE_PROTOCOL *
LocateAcpiProtocol (
  VOID
  )
{
  EFI_STATUS               Status;
  EFI_ACPI_TABLE_PROTOCOL  *AcpiTblProtocol;

  AcpiTblProtocol = NULL;

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTblProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate ACPI table protocol (%r)\n", Status));
    return NULL;
  }

  return AcpiTblProtocol;
}

/**
  Initialize private data structure and allocate memory for HobDataPtr.

  @retval  EFI_SUCCESS           mCrashlogDxe initialized successfully.
           EFI_OUT_OF_RESOURCES  Could not allocate needed resources.
           EFI_NOT_FOUND         Could not find AcpiProtocol.

**/
EFI_STATUS
InitializePrivateData (
  VOID
  )
{
  ZeroMem (&mCrashlogDxe, sizeof (CRASHLOG_DXE_PRIVATE_DATA));

  mCrashlogDxe.HobDataPtr = AllocateZeroPool (sizeof (CRASHLOG_HOB_DATA *) * CrashlogAgentTypeMax);
  if (mCrashlogDxe.HobDataPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate space for HOB data.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  mCrashlogDxe.AcpiTblProtocol = LocateAcpiProtocol ();
  if (mCrashlogDxe.AcpiTblProtocol == NULL) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  Entry point of the CrashLog Dxe driver.

  @param[in] ImageHandle  EFI_HANDLE: A handle for the image that is initializing this driver
  @param[in] SystemTable  EFI_SYSTEM_TABLE: A pointer to the EFI system table

  @retval  EFI_SUCCESS:           Driver initialized successfully
           EFI_LOAD_ERROR:        Failed to Initialize or to Load the driver
           EFI_OUT_OF_RESOURCES:  Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
CrashLogDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a() - enter\n", __FUNCTION__));

  Status = InitializePrivateData ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GetPayloadInfoFromHob ();

  if (mCrashlogDxe.PayloadSize > 0) {
    Status = InitBertTbl ();

    if (!EFI_ERROR (Status)) {
      InstallApeiBertTables (mCrashlogDxe.AcpiTblProtocol, mCrashlogDxe.Bert);
    }
  }

  DEBUG ((DEBUG_INFO, "%a() - exit\n", __FUNCTION__));

  return EFI_SUCCESS;
}
