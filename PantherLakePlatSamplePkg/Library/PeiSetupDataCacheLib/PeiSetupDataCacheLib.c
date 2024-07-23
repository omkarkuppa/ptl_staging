/** @file
  Main implementation source file for the support of caching the "Setup" variable.

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

#include "PeiSetupDataCacheLib.h"


EFI_PEI_READ_ONLY_VARIABLE2_PPI mNewVariablePpi = {
  NewGetVariable,
  NewNextVariableName
};

EFI_PEI_PPI_DESCRIPTOR     mNewVariablePpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiReadOnlyVariable2PpiGuid,
  &mNewVariablePpi
};

/**
  This service retrieves a variable's value using its name and GUID.

  Read the specified variable from the UEFI variable store. If the Data
  buffer is too small to hold the contents of the variable, the error
  EFI_BUFFER_TOO_SMALL is returned and DataSize is set to the required buffer
  size to obtain the data.

  @param[in]      This                  A pointer to this instance of the EFI_PEI_READ_ONLY_VARIABLE2_PPI.
  @param[in]      VariableName          A pointer to a null-terminated string that is the variable's name.
  @param[in]      VariableGuid          A pointer to an EFI_GUID that is the variable's GUID. The combination of
                                        VariableGuid and VariableName must be unique.
  @param[out]     Attributes            If non-NULL, on return, points to the variable's attributes.
  @param[in, out] DataSize              On entry, points to the size in bytes of the Data buffer.
                                        On return, points to the size of the data returned in Data.
  @param[out]     Data                  Points to the buffer which will hold the returned variable value.

  @retval         EFI_SUCCESS           The variable was read successfully.
  @retval         EFI_NOT_FOUND         The variable could not be found.
  @retval         EFI_BUFFER_TOO_SMALL  The DataSize is too small for the resulting data.
                                        DataSize is updated with the size required for
                                        the specified variable.
  @retval         EFI_INVALID_PARAMETER VariableName, VariableGuid, DataSize or Data is NULL.
  @retval         EFI_DEVICE_ERROR      The variable could not be retrieved because of a device error.
**/
EFI_STATUS
EFIAPI
NewGetVariable (
  IN CONST  EFI_PEI_READ_ONLY_VARIABLE2_PPI *This,
  IN CONST  CHAR16                          *VariableName,
  IN CONST  EFI_GUID                        *VariableGuid,
  OUT       UINT32                          *Attributes,
  IN OUT    UINTN                           *DataSize,
  OUT       VOID                            *Data
  )
{
  SETUP_DATA_HOB              *SetupDataHob;

  if (VariableName == NULL || VariableGuid == NULL || DataSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SetupDataHob = (SETUP_DATA_HOB *)(VOID *)GetFirstGuidHob (&gSetupDataHobGuid);
  ASSERT (SetupDataHob != NULL);
  if (SetupDataHob == NULL) {
    return EFI_NOT_FOUND;
  }

  SetupDataHob->TotalCnt++;

  if ((StrCmp (VariableName, L"Setup") == 0) &&
       CompareGuid (VariableGuid, &gSetupVariableGuid)) {
    SetupDataHob->CacheCnt++;

    if (*DataSize < sizeof (SETUP_DATA)) {
      *DataSize = sizeof (SETUP_DATA);
      return EFI_BUFFER_TOO_SMALL;
    }

    if (Attributes != NULL) {
      *Attributes = SetupDataHob->SetupAttributes;
    }
    *DataSize = sizeof (SETUP_DATA);
    CopyMem (Data, &SetupDataHob->SetupData, sizeof(SETUP_DATA));
    return EFI_SUCCESS;
  }

  if ((StrCmp (VariableName, L"BoardInfoSetup") == 0) &&
       CompareGuid (VariableGuid, &gBoardInfoVariableGuid)) {
    SetupDataHob->CacheCnt++;

    if (*DataSize < sizeof (BOARD_INFO_SETUP)) {
      *DataSize = sizeof (BOARD_INFO_SETUP);
      return EFI_BUFFER_TOO_SMALL;
    }

    if (Attributes != NULL) {
      *Attributes = SetupDataHob->BoardInfoAttributes;
    }
    *DataSize = sizeof (BOARD_INFO_SETUP);
    CopyMem (Data, &SetupDataHob->BoardInfoSetup, sizeof(BOARD_INFO_SETUP));
    return EFI_SUCCESS;
  }

  if ((StrCmp (VariableName, L"SaSetup") == 0) &&
       CompareGuid (VariableGuid, &gSaSetupVariableGuid)) {
    SetupDataHob->CacheCnt++;

    if (*DataSize < sizeof (SA_SETUP)) {
      *DataSize = sizeof (SA_SETUP);
      return EFI_BUFFER_TOO_SMALL;
    }

    if (Attributes != NULL) {
      *Attributes = SetupDataHob->SaAttributes;
    }
    *DataSize = sizeof (SA_SETUP);
    CopyMem (Data, &SetupDataHob->SaSetup, sizeof(SA_SETUP));
    return EFI_SUCCESS;
  }

  if ((StrCmp (VariableName, L"MeSetup") == 0) &&
       CompareGuid (VariableGuid, &gMeSetupVariableGuid)) {
    SetupDataHob->CacheCnt++;

    if (*DataSize < sizeof (ME_SETUP)) {
      *DataSize = sizeof (ME_SETUP);
      return EFI_BUFFER_TOO_SMALL;
    }

    if (Attributes != NULL) {
      *Attributes = SetupDataHob->MeAttributes;
    }
    *DataSize = sizeof (ME_SETUP);
    CopyMem (Data, &SetupDataHob->MeSetup, sizeof(ME_SETUP));
    return EFI_SUCCESS;
  }

  if ((StrCmp (VariableName, L"CpuSetup") == 0) &&
       CompareGuid (VariableGuid, &gCpuSetupVariableGuid)) {
    SetupDataHob->CacheCnt++;

    if (*DataSize < sizeof (CPU_SETUP)) {
      *DataSize = sizeof (CPU_SETUP);
      return EFI_BUFFER_TOO_SMALL;
    }

    if (Attributes != NULL) {
      *Attributes = SetupDataHob->CpuAttributes;
    }
    *DataSize = sizeof (CPU_SETUP);
    CopyMem (Data, &SetupDataHob->CpuSetup, sizeof(CPU_SETUP));
    return EFI_SUCCESS;
  }

  if ((StrCmp (VariableName, L"PchSetup") == 0) &&
       CompareGuid (VariableGuid, &gPchSetupVariableGuid)) {
    SetupDataHob->CacheCnt++;

    if (*DataSize < sizeof (PCH_SETUP)) {
      *DataSize = sizeof (PCH_SETUP);
      return EFI_BUFFER_TOO_SMALL;
    }

    if (Attributes != NULL) {
      *Attributes = SetupDataHob->PchAttributes;
    }
    *DataSize = sizeof (PCH_SETUP);
    CopyMem (Data, &SetupDataHob->PchSetup, sizeof(PCH_SETUP));
    return EFI_SUCCESS;
  }

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  if ((StrCmp (VariableName, L"OcSetup") == 0) &&
       CompareGuid (VariableGuid, &gOcSetupVariableGuid)) {
    SetupDataHob->CacheCnt++;

    if (*DataSize < sizeof (OC_SETUP)) {
      *DataSize = sizeof (OC_SETUP);
      return EFI_BUFFER_TOO_SMALL;
    }

    if (Attributes != NULL) {
      *Attributes = SetupDataHob->OcAttributes;
    }
    *DataSize = sizeof (OC_SETUP);
    CopyMem (Data, &SetupDataHob->OcSetup, sizeof(OC_SETUP));
    return EFI_SUCCESS;
  }
#endif

  return SetupDataHob->VariablePpi->GetVariable (
                         This,
                         VariableName,
                         VariableGuid,
                         Attributes,
                         DataSize,
                         Data
                         );
}


/**
  Return the next variable name and GUID.

  This function is called multiple times to retrieve the VariableName
  and VariableGuid of all variables currently available in the system.
  On each call, the previous results are passed into the interface,
  and, on return, the interface returns the data for the next
  interface. When the entire variable list has been returned,
  EFI_NOT_FOUND is returned.

  @param[in]      This                  A pointer to this instance of the EFI_PEI_READ_ONLY_VARIABLE2_PPI.
  @param[in, out] VariableNameSize      On entry, points to the size of the buffer pointed to by VariableName.
                                        On return, the size of the variable name buffer.
  @param[in, out] VariableName          On entry, a pointer to a null-terminated string that is the variable's name.
                                        On return, points to the next variable's null-terminated name string.
  @param[in, out] VariableGuid          On entry, a pointer to an EFI_GUID that is the variable's GUID.
                                        On return, a pointer to the next variable's GUID.

  @retval         EFI_SUCCESS           The variable was read successfully.
  @retval         EFI_NOT_FOUND         The variable could not be found.
  @retval         EFI_BUFFER_TOO_SMALL  The VariableNameSize is too small for the resulting
                                        data. VariableNameSize is updated with the size
                                        required for the specified variable.
  @retval         EFI_INVALID_PARAMETER VariableName, VariableGuid or
                                        VariableNameSize is NULL.
  @retval         EFI_DEVICE_ERROR      The variable could not be retrieved because of a device error.
**/
EFI_STATUS
EFIAPI
NewNextVariableName (
  IN CONST  EFI_PEI_READ_ONLY_VARIABLE2_PPI *This,
  IN OUT UINTN                              *VariableNameSize,
  IN OUT CHAR16                             *VariableName,
  IN OUT EFI_GUID                           *VariableGuid
  )
{
  SETUP_DATA_HOB              *SetupDataHob;

  SetupDataHob = (SETUP_DATA_HOB *)(VOID *)GetFirstGuidHob (&gSetupDataHobGuid);
  ASSERT (SetupDataHob != NULL);
  if (SetupDataHob == NULL) {
    return EFI_NOT_FOUND;
  }

  return SetupDataHob->VariablePpi->NextVariableName (
                                      This,
                                      VariableNameSize,
                                      VariableName,
                                      VariableGuid
                                      );
}

/**
  Notification function of Read Only Variable2 PPI.

  @retval     EFI_SUCCESS      SetupDataHob already exists, or was successfully (re)installed
  @retval     Others           Error status return
**/
EFI_STATUS
EFIAPI
SetupDataCacheInit (
  VOID
  )
{
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariablePpi;
  EFI_PEI_PPI_DESCRIPTOR          *OldVariablePpiList;
  UINTN                           VarSize;
  UINTN                           SaVarSize;
  UINTN                           MeVarSize;
  UINTN                           CpuVarSize;
  #if FixedPcdGetBool(PcdOverclockEnable) == 1
  UINTN                           OcVarSize;
  #endif
  UINTN                           PchVarSize;
  UINTN                           BoardInfoVarSize;
  SETUP_DATA_HOB                  *SetupDataHob;
  EFI_STATUS                      Status;

  SetupDataHob = (SETUP_DATA_HOB *)(VOID *)GetFirstGuidHob (&gSetupDataHobGuid);
  if (SetupDataHob != NULL) {
    //
    // SetupDataHob already created
    //
    return EFI_SUCCESS;
  }

  Status = PeiServicesLocatePpi (
                         &gEfiPeiReadOnlyVariable2PpiGuid,
                         0,
                         &OldVariablePpiList,
                         (VOID **) &VariablePpi
                         );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR(Status);
    return Status;
  }

  Status = PeiServicesCreateHob (
                         EFI_HOB_TYPE_GUID_EXTENSION,
                         sizeof (SETUP_DATA_HOB),
                         (VOID **) &SetupDataHob
                         );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR(Status);
    return Status;
  }

  CopyMem(&SetupDataHob->Header.Name, &gSetupDataHobGuid, sizeof(EFI_GUID));
  SetMem ((VOID *)&SetupDataHob->SetupData, sizeof(SETUP_DATA), 0);
  SetMem ((VOID *)&SetupDataHob->BoardInfoSetup, sizeof(BOARD_INFO_SETUP), 0);
  SetMem ((VOID *)&SetupDataHob->SaSetup, sizeof(SA_SETUP), 0);
  SetMem ((VOID *)&SetupDataHob->MeSetup, sizeof(ME_SETUP), 0);
  SetMem ((VOID *)&SetupDataHob->CpuSetup, sizeof(CPU_SETUP), 0);
  SetMem ((VOID *)&SetupDataHob->PchSetup, sizeof(PCH_SETUP), 0);
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  SetMem ((VOID *)&SetupDataHob->OcSetup, sizeof(OC_SETUP), 0);
#endif
  SetupDataHob->VariablePpi = VariablePpi;
  SetupDataHob->TotalCnt = 0;
  SetupDataHob->CacheCnt = 0;

  VarSize = sizeof (SETUP_DATA);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"Setup",
                          &gSetupVariableGuid,
                          &SetupDataHob->SetupAttributes,
                          &VarSize,
                          &SetupDataHob->SetupData
                          );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  BoardInfoVarSize = sizeof (BOARD_INFO_SETUP);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"BoardInfoSetup",
                          &gBoardInfoVariableGuid,
                          &SetupDataHob->BoardInfoAttributes,
                          &BoardInfoVarSize,
                          &SetupDataHob->BoardInfoSetup
                          );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  SaVarSize = sizeof (SA_SETUP);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"SaSetup",
                          &gSaSetupVariableGuid,
                          &SetupDataHob->SaAttributes,
                          &SaVarSize,
                          &SetupDataHob->SaSetup
                          );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  MeVarSize = sizeof (ME_SETUP);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"MeSetup",
                          &gMeSetupVariableGuid,
                          &SetupDataHob->MeAttributes,
                          &MeVarSize,
                          &SetupDataHob->MeSetup
                          );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  CpuVarSize = sizeof (CPU_SETUP);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"CpuSetup",
                          &gCpuSetupVariableGuid,
                          &SetupDataHob->CpuAttributes,
                          &CpuVarSize,
                          &SetupDataHob->CpuSetup
                          );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  PchVarSize = sizeof (PCH_SETUP);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"PchSetup",
                          &gPchSetupVariableGuid,
                          &SetupDataHob->PchAttributes,
                          &PchVarSize,
                          &SetupDataHob->PchSetup
                          );
  if (EFI_ERROR(Status)) {
    return Status;
  }

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  OcVarSize = sizeof (OC_SETUP);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"OcSetup",
                          &gOcSetupVariableGuid,
                          &SetupDataHob->OcAttributes,
                          &OcVarSize,
                          &SetupDataHob->OcSetup
                          );
  if (EFI_ERROR(Status)) {
    return Status;
  }
#endif

  ASSERT ((VarSize == sizeof(SETUP_DATA)) &&
          (SaVarSize == sizeof(SA_SETUP)) &&
          (MeVarSize == sizeof(ME_SETUP)) &&
          (CpuVarSize == sizeof(CPU_SETUP)) &&
          (PchVarSize == sizeof(PCH_SETUP)) &&
          (BoardInfoVarSize == sizeof(BOARD_INFO_SETUP))
        );
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  ASSERT ((OcVarSize == sizeof(OC_SETUP)));
#endif

  if ((VarSize == sizeof(SETUP_DATA)) &&
      (SaVarSize == sizeof(SA_SETUP)) &&
      (MeVarSize == sizeof(ME_SETUP)) &&
      (CpuVarSize == sizeof(CPU_SETUP)) &&
#if FixedPcdGetBool(PcdOverclockEnable) == 1
      (OcVarSize == sizeof(OC_SETUP)) &&
#endif
      (PchVarSize == sizeof(PCH_SETUP)) &&
      (BoardInfoVarSize == sizeof(BOARD_INFO_SETUP))) {
    Status = PeiServicesReInstallPpi (
                                (CONST EFI_PEI_PPI_DESCRIPTOR *)OldVariablePpiList,
                                (CONST EFI_PEI_PPI_DESCRIPTOR *)&mNewVariablePpiList
                                );
    ASSERT_EFI_ERROR(Status);
  }

  return Status;
}

