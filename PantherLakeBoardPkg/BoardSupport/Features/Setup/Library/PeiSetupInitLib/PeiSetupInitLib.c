/** @file
  Implementation of Setup variable initialization in PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <PiPei.h>
#include <SetupVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SetupInitLib.h>
#include <Library/UefiVariableDefaultHobLib.h>
#include <Library/UefiVariableHelperLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Setup.h>
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
#include <DptfConfig.h>
#endif
#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 1
#include <MipiCamConfig.h>
#endif

VARIABLE_ENTRY_CONSISTENCY mSetupVariableList[] = {
  // GUID                    Name               Data Size
  { &gSetupVariableGuid,     L"Setup",          sizeof(SETUP_DATA)       },
  { &gBoardInfoVariableGuid, L"BoardInfoSetup", sizeof(BOARD_INFO_SETUP) },
  { &gSaSetupVariableGuid,   L"SaSetup",        sizeof(SA_SETUP)         },
  { &gCpuSetupVariableGuid,  L"CpuSetup",       sizeof(CPU_SETUP)        },
  { &gPchSetupVariableGuid,  L"PchSetup",       sizeof(PCH_SETUP)        },
  { &gMeSetupVariableGuid,   L"MeSetup",        sizeof(ME_SETUP)         },
  { &gSiSetupVariableGuid,   L"SiSetup",        sizeof(SI_SETUP)         },
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  {&gDptfConfigVariableGuid,DPTF_CONFIG_VARIABLE_NAME,sizeof(DPTF_CONFIG)},
#endif
#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 1
  {&gMipiCamConfigVariableGuid,MIPICAM_CONFIG_VARIABLE_NAME,sizeof(MIPICAM_CONFIG)},
#endif
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  { &gOcSetupVariableGuid,   L"OcSetup",        sizeof(OC_SETUP)         },
#endif
};

typedef union {
  SETUP_DATA       SetupData;
  BOARD_INFO_SETUP BoardInfoSetup;
  CPU_SETUP        CpuSetup;
  ME_SETUP         MeSetup;
  SA_SETUP         SaSetup;
  PCH_SETUP        PchSetup;
  SI_SETUP         SiSetup;
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  DPTF_CONFIG      DptfConfig;
#endif
#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 1
  MIPICAM_CONFIG   MipiCamConfig;
#endif
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  OC_SETUP         OcSetup;
#endif
} SETUP_VAR_DATA;

/**
  Update variable revision field value.

  @param[in]      Offset         The field offset of the variable data structure.
  @param[in]      Revision       An unsigned integer represent the revision value.
  @param[in]      *VarName       A pointer to a unicode string represent the varaibel name.
  @param[in]      *VarGuid       A pointer to a GUID represnet the variable GUID.
  @param[in]      VarSize        A usnsigned integer that represent the variable size in byte.

  @retval         EFI_SUCCESS    the value is updated successfully.
**/
EFI_STATUS
EFIAPI
UpdateSetupVariableRevision (
  IN UINTN      Offset,
  IN UINT8      Revision,
  IN CHAR16     *VarName,
  IN EFI_GUID   *VarGuid,
  IN UINTN      VarSize
  )
{
  EFI_STATUS      Status;
  UINTN           VarDataSize;
  SETUP_VAR_DATA  VarData;
  UINT8           *RevisionPtr;

  VarDataSize = VarSize;

  Status = GetVariableFromHob (
                  VarName,
                  VarGuid,
                  NULL,
                  &VarDataSize,
                  &VarData
                  );

  if (Status == EFI_BUFFER_TOO_SMALL) {
    DEBUG ((DEBUG_ERROR, "VarDataSize =  : %x\n", VarDataSize));
    Status = GetVariableFromHob (
                  VarName,
                  VarGuid,
                  NULL,
                  &VarDataSize,
                  &VarData
                  );
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get variable from hob failed : %r\n", Status));
    DEBUG ((DEBUG_ERROR, "Variable Name : %s, GUID : %g\n", VarName, VarGuid));
    return Status;
  }

  RevisionPtr = (UINT8 *) (VOID *) &VarData + Offset;
  *RevisionPtr = Revision;

  Status = SetVariableToHob (
                  VarName,
                  VarGuid,
                  NULL,
                  VarDataSize,
                  &VarData
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Set variable to hob failed : %r\n", Status));
    DEBUG ((DEBUG_ERROR, "Variable Name : %s, GUID : %g\n", VarName, VarGuid));
  }

  return Status;
}

/**
  Updates the Setup variable revision for the Setup structures.

  @retval         EFI_SUCCESS    The revision for the Setup structures was updated successfully.
**/
EFI_STATUS
EFIAPI
UpdateAllSetupVariableRevisions (
  VOID
  )
{
  EFI_STATUS Status;

  Status = UpdateSetupVariableRevision (
             OFFSET_OF (SETUP_DATA, Revision),
             SETUP_DATA_REVISION,
             PLATFORM_SETUP_VARIABLE_NAME,
             &gSetupVariableGuid,
             sizeof (SETUP_DATA)
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Set SETUP variable revision.
  //
  Status = UpdateSetupVariableRevision (
             OFFSET_OF (ME_SETUP, Revision),
             ME_SETUP_REVISION,
             L"MeSetup",
             &gMeSetupVariableGuid,
             sizeof (ME_SETUP)
             );
  ASSERT_EFI_ERROR (Status);

  Status = UpdateSetupVariableRevision (
             OFFSET_OF (CPU_SETUP, Revision),
             CPU_SETUP_REVISION,
             L"CpuSetup",
             &gCpuSetupVariableGuid,
             sizeof (CPU_SETUP)
             );
  ASSERT_EFI_ERROR (Status);

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  Status = UpdateSetupVariableRevision (
             OFFSET_OF (OC_SETUP, Revision),
             OC_SETUP_REVISION,
             L"OcSetup",
             &gOcSetupVariableGuid,
             sizeof (OC_SETUP)
             );
  ASSERT_EFI_ERROR (Status);
#endif

  Status = UpdateSetupVariableRevision (
             OFFSET_OF (SA_SETUP, Revision),
             SA_SETUP_REVISION,
             L"SaSetup",
             &gSaSetupVariableGuid,
             sizeof (SA_SETUP)
             );
  ASSERT_EFI_ERROR (Status);

  Status = UpdateSetupVariableRevision (
             OFFSET_OF (PCH_SETUP, Revision),
             PCH_SETUP_REVISION,
             L"PchSetup",
             &gPchSetupVariableGuid,
             sizeof (PCH_SETUP)
             );
  ASSERT_EFI_ERROR (Status);

  Status = UpdateSetupVariableRevision (
             OFFSET_OF (SI_SETUP, Revision),
             SI_SETUP_REVISION,
             L"SiSetup",
             &gSiSetupVariableGuid,
             sizeof (SI_SETUP)
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Checks if Setup UEFI variables are changed in the UEFI variable store.

  @retval BOOLEAN   Indicates whether the UEFI variable was changed, if changed, return false.

**/
BOOLEAN
EFIAPI
IsSetupStructuresUnChanged (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINTN                           Size;
  UINTN                           VariableIndex;
  SETUP_VAR_DATA                  Setup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiReadOnlyVariablePpi;
  UINT8                           InitSetupFlag;
#ifndef MDEPKG_NDEBUG
  DEBUG_CONFIG_DATA               DebugConfigData;
#endif

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &PeiReadOnlyVariablePpi
             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return FALSE;
  }

  for (
    VariableIndex = 0;
    VariableIndex < (sizeof (mSetupVariableList) / sizeof (mSetupVariableList[0]));
    VariableIndex++
    ) {
    Size = mSetupVariableList[VariableIndex].VariableSize;
    Status = PeiReadOnlyVariablePpi->GetVariable (
                                       PeiReadOnlyVariablePpi,
                                       mSetupVariableList[VariableIndex].Name,
                                       mSetupVariableList[VariableIndex].Guid,
                                       NULL,
                                       &Size,
                                       &Setup
                                       );
  if (Status == EFI_NOT_FOUND || (Size != mSetupVariableList[VariableIndex].VariableSize)) {
      DEBUG ((
        DEBUG_INFO,
        "Found setup variable changes %s - %g - %r - %d - %d.\n",
        mSetupVariableList[VariableIndex].Name,
        mSetupVariableList[VariableIndex].Guid,
        Status,
        Size,
        mSetupVariableList[VariableIndex].VariableSize
        ));
      return FALSE;
    } else if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "un-expected get variable error %s - %g -%r.\n",
        mSetupVariableList[VariableIndex].Name,
        mSetupVariableList[VariableIndex].Guid,
        Status
        ));
      return FALSE;
    }
  }

  Size = sizeof (InitSetupFlag);
  Status = PeiReadOnlyVariablePpi->GetVariable (PeiReadOnlyVariablePpi, L"InitSetupVariable", &gSetupVariableGuid, NULL, &Size, &InitSetupFlag);
  if (Status == EFI_NOT_FOUND || (Size != sizeof (InitSetupFlag))) {
    DEBUG((DEBUG_INFO, "SetupInitLib detects the first boot \n"));
    return FALSE;
  }

#ifndef MDEPKG_NDEBUG
  Size = sizeof (DEBUG_CONFIG_DATA);
  Status = PeiReadOnlyVariablePpi->GetVariable (PeiReadOnlyVariablePpi, L"DebugConfigData", &gDebugConfigVariableGuid, NULL, &Size, &DebugConfigData);
  if (Status == EFI_NOT_FOUND || (Size != sizeof (DEBUG_CONFIG_DATA))) {
    DEBUG((DEBUG_INFO, "debug config data check changed in SetupInitLib: %d\n", Size));
    return FALSE;
  }
#endif

  return TRUE;
}

/**
  Prints all Setup UEFI variables located in the UEFI variable store.

  @retval EFI_SUCCESS   An error did not occur located the UEFI variables and printing the contents.
  @retval Others        An error occurred locating the UEFI variables.

**/
EFI_STATUS
EFIAPI
PrintSetupStructures (
  VOID
  )
{
  DEBUG_CODE_BEGIN();
    EFI_STATUS                      Status;
    UINTN                           Size;
    UINTN                           VariableIndex;
    SETUP_VAR_DATA                  Setup;
    EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiReadOnlyVariablePpi;

    Status = PeiServicesLocatePpi (
               &gEfiPeiReadOnlyVariable2PpiGuid,
               0,
               NULL,
               (VOID **) &PeiReadOnlyVariablePpi
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    for (
      VariableIndex = 0;
      VariableIndex < sizeof (mSetupVariableList) / sizeof (mSetupVariableList[0]);
      VariableIndex++
      ) {
      Size = sizeof (SETUP_VAR_DATA);
      Status = PeiReadOnlyVariablePpi->GetVariable (
                                         PeiReadOnlyVariablePpi,
                                         mSetupVariableList[VariableIndex].Name,
                                         mSetupVariableList[VariableIndex].Guid,
                                         NULL,
                                         &Size,
                                         &Setup
                                         );
      if (EFI_ERROR (Status)) {
        return Status;
      }
      DEBUG ((DEBUG_INFO, "\nDumping %s Structure, Size = 0x%X, Guid = %g", mSetupVariableList[VariableIndex].Name, Size, mSetupVariableList[VariableIndex].Guid));
      PrintVariableData ((UINT8 *) &Setup, Size);
    }
  DEBUG_CODE_END();

  return EFI_SUCCESS;
}
