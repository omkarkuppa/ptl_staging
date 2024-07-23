/** @file
 Entry point of DTPR DXE driver.

 This driver will install the DTPR ACPI table.

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

#include "DtprDxe.h"

/**
  Main entry for this driver.

  @param ImageHandle     Image handle this driver.
  @param SystemTable     Pointer to SystemTable.

  @retval EFI_SUCESS     This function always complete successfully.
**/
EFI_STATUS
EFIAPI
DtprDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTable = NULL;
  UINTN                           TableKey;
  TPR_INFO_HOB                   *TprInfoHob = NULL;
  DTPR_TABLE_STRUCT               DtprTable;
  EFI_STATUS                      Status;
  UINT32                          Length;
  UINT32                          Index = 0;
  UINT32                          Count = 0;
  UINT8                          *TprBuffer = NULL;
  UINT8                          *Buffer = NULL;
  UINT64                         *DrainCtrlArrayPtr = NULL;

  DEBUG ((DEBUG_INFO, "DtprDxeEntryPoint\n"));

  // Get entries from TPR_INFO_HOB
  TprInfoHob = GetFirstGuidHob (&gTprInfoHobGuid);

  if (TprInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "TprInfoHobGuid not found!"));
    return EFI_NOT_FOUND;
  }

  ZeroMem(&DtprTable, sizeof(DTPR_TABLE_STRUCT));

  DtprTable.Header.Signature = DTPR_ACPI_TABLE_SIGNATURE;

  DtprTable.Header.Revision = 1;

  DtprTable.Flags = 0;

  if (TprInfoHob->InstanceCount > MAX_INS_COUNT) {
    DEBUG ((DEBUG_ERROR, "DtprDxe: Tpr Instance count is greater than MAX_INS_COUNT. Found = %d\n", \
                          TprInfoHob->InstanceCount));
    return EFI_INVALID_PARAMETER;
  }

  DtprTable.InsCnt = TprInfoHob->InstanceCount;

  for (Index = 0; Index < TprInfoHob->InstanceCount; Index++) {
    DtprTable.TprIns[Index].TprCnt = TprInfoHob->InstanceArray[Index].TprCount;
    DtprTable.TprIns[Index].Flags = 0;
    DtprTable.TprIns[Index].TprArr[0].TprBase = TprInfoHob->InstanceArray[Index].Tpr0BaseOffset;
    DtprTable.TprIns[Index].TprArr[1].TprBase = TprInfoHob->InstanceArray[Index].Tpr1BaseOffset;
  }

  //Check valid drain control register entries in Hob
  DrainCtrlArrayPtr = (UINT64 *)&(TprInfoHob->DrainControlArray);

  // Check valid drain control register entries in tprInfoHob
  for (Index = 0; Index < sizeof(DRAIN_CTL_REG)/sizeof(UINT64); Index++) {
    if (*DrainCtrlArrayPtr != 0) {
      DtprTable.SrlRegArr[Count] = *DrainCtrlArrayPtr;
      Count++;
    }
    DrainCtrlArrayPtr++;
  }

  // Check count of valid entries is same as published by Hob
  if (Count != TprInfoHob->DrainControlCount) {
    DEBUG ((DEBUG_ERROR, "DtprDxe: Valid entries in Hob does not match with count. Found = %d, expected = %d\n", \
                          Count, TprInfoHob->DrainControlCount));
    return EFI_ABORTED;
  }

  DtprTable.SrlCnt = Count;

  // Calculate Length of ACPI table
  Length = (sizeof(EFI_ACPI_DESCRIPTION_HEADER) + sizeof(UINT32) + sizeof(UINT32) + \
           sizeof(TPR_INS_STRUCT)*DtprTable.InsCnt + sizeof(UINT32) + \
           sizeof(UINT64)*Count);

  DtprTable.Header.Length = Length;

  // Allocate memory for ACPI table
  TprBuffer = AllocatePool (Length);

  Buffer = TprBuffer;
  if (TprBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Copying header
  CopyMem(Buffer, &DtprTable, sizeof(EFI_ACPI_DESCRIPTION_HEADER));
  Buffer += sizeof(EFI_ACPI_DESCRIPTION_HEADER);

  // Copying flags + TPRcount + TPR_INS_STRUCT
  CopyMem(Buffer, &(DtprTable.Flags), (sizeof(UINT32) + sizeof(UINT32) + sizeof(TPR_INS_STRUCT)*DtprTable.InsCnt));
  Buffer += sizeof(UINT32) + sizeof(UINT32) + sizeof(TPR_INS_STRUCT)*DtprTable.InsCnt;

  // Copying srlcount + srlregarr
  CopyMem(Buffer, &(DtprTable.SrlCnt), sizeof(UINT32) + sizeof(UINT64)*Count);

  /// Construct ACPI table
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);

   DEBUG ((DEBUG_INFO, "TprInfoHob in DXE %x\n", TprInfoHob));
   DEBUG ((DEBUG_INFO, "GtDrainControl in DXE %x\n", TprInfoHob->DrainControlArray.GtDrainControl));
   DEBUG ((DEBUG_INFO, "MediaDrainControl in DXE %x\n", TprInfoHob->DrainControlArray.MediaDrainControl));
   DEBUG ((DEBUG_INFO, "DisplayDrainControl in DXE %x\n", TprInfoHob->DrainControlArray.DisplayDrainControl));
   DEBUG ((DEBUG_INFO, "IaxDrainControl in DXE %x\n", TprInfoHob->DrainControlArray.IaxDrainControl));
   DEBUG ((DEBUG_INFO, "IocDrainControl in DXE %x\n", TprInfoHob->DrainControlArray.IocDrainControl));
   DEBUG ((DEBUG_INFO, "IvtuDrainControl in DXE %x\n", TprInfoHob->DrainControlArray.IvtuDrainControl));
   DEBUG ((DEBUG_INFO, "SvtuDrainControl in DXE %x\n", TprInfoHob->DrainControlArray.SvtuDrainControl));
   DEBUG ((DEBUG_INFO, "IpuDrainControl in DXE %x\n", TprInfoHob->DrainControlArray.IpuDrainControl));
   DEBUG ((DEBUG_INFO, "NpuDrainControl in DXE %x\n", TprInfoHob->DrainControlArray.NpuDrainControl));
   DEBUG ((DEBUG_INFO, "PunitDrainControl in DXE %x\n", TprInfoHob->DrainControlArray.PunitDrainControl));

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (AcpiTable == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        TprBuffer,
                        ((EFI_ACPI_DESCRIPTION_HEADER *)TprBuffer)->Length,
                        &TableKey
                        );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (TprBuffer != NULL) {
    gBS->FreePool (TprBuffer);
  }

  return Status;
}