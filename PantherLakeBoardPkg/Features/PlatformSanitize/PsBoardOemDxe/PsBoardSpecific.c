/** @file
  Platform Sanitize Dxe driver supports Platform specific sanitization.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "PsBoardOemDxe.h"

//
// List Setup related NVM variables in an Array.
// These variables would be deleted when "PS_BIOS_RELOAD_GOLDEN_CONFIG" is Triggered so that on next boot the same
// variables load with default configuration settings.
//
// Note: OEM can modify this list as per their platform setup to load wiht default config.
//
BIOS_SETUP_NVM_VARIABLE_LIST mSetupNvmVariableList[] = {
  { L"EnableCustomDefaultsState", &gOsVendorGlobalVariableGuid },
  { L"Setup"                    , &gSetupVariableGuid      },
  { L"MeInfoSetup"              , &gMeInfoSetupGuid        },
  { L"TcgSetup"                 , &gSetupVariableGuid      },
  { L"SaSetup"                  , &gSaSetupVariableGuid    },
  { L"CpuSetup"                 , &gCpuSetupVariableGuid   },
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  { L"OcSetup"                  , &gOcSetupVariableGuid    },
#endif
  { L"PchSetup"                 , &gPchSetupVariableGuid   },
  { L"MeSetup"                  , &gMeSetupVariableGuid    }
};

/**
  Platform Sanitize to perform Reload NVM Variable with Golden Configuration settings.
  Intel RVP-BIOS, Setup options are reloaded to default configuration.

  Note: OEM can modify to reload their own set of BIOS Configuration.

  @param[in] *This              - A pointer to the PS_BOARD_OEM_PROTOCOL instance

  @retval EFI_SUCCESS           - The operation succeeds.
  @retval Others                - Other status errors
**/
EFI_STATUS
EFIAPI
PsNvmGoldenConfig (
  IN PS_BOARD_OEM_PROTOCOL *This
  )
{
  EFI_STATUS      Status;
  UINTN           Index;
  BOOLEAN         AnyVarFailToDelete;

  DEBUG ((DEBUG_INFO, "Delete Setup User Data\n"));

  AnyVarFailToDelete = FALSE;
  for (Index = 0; Index < sizeof (mSetupNvmVariableList) / sizeof (mSetupNvmVariableList[0]); Index++) {
    Status = gRT->SetVariable (
                    mSetupNvmVariableList[Index].Name,
                    mSetupNvmVariableList[Index].Guid,
                    0,
                    0,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Failed to delete variable %s GUID = %r, Status = %r\n", mSetupNvmVariableList[Index].Name, mSetupNvmVariableList[Index].Guid, Status));
      if (Status != EFI_NOT_FOUND) {
        AnyVarFailToDelete = TRUE;
      }
    }
  }

  if (AnyVarFailToDelete) {
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}

/**
  This function invoked Post "PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION" action, to perform any action to take after formating the entire
  NVM variable region like adding Secure Boot, Keys, Certificates related NVM variables which might essential for UEFI Secure Boot.

  Note: Intel-RVP BIOS returns simply with EFI_SUCCESS.
        OEM can modify this function as per their need.

  @param[in] *This              - A pointer to the PS_BOARD_OEM_PROTOCOL instance

  @retval EFI_SUCCESS           - The operation succeeds.
  @retval Others                - Other status errors
**/
EFI_STATUS
EFIAPI
PsPostClearAllBiosNvmVariableRegionAction (
  IN PS_BOARD_OEM_PROTOCOL *This
  )
{
  DEBUG ((DEBUG_INFO, "Post Clear All BIOS NVM Variable Region.\n"));

  return EFI_SUCCESS;
}
