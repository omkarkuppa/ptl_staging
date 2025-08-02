/** @file
   Updates Platform Information to Setup Data

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

#include <Library/DxeUpdatePlatformInfoLib.h>
#include <Library/PlatformUsbConfigLib.h>

/**

  This function update the UEFI variable "SetupVolatileData" to reflect current platform values,
  Return out if the variable is NOT found.

**/
VOID
UpdatePlatformInfo (
  VOID
  )
{

#if FixedPcdGetBool (PcdSetupEnable) == 1
  EFI_STATUS              Status;
  BOARD_INFO_SETUP        BoardInfoSetup;
  UINT32                  BoardInfoAttributes;
  SETUP_VOLATILE_DATA     SetupVolatileData;
  UINT32                  SetupVolAttributes;
  UINTN                   VariableSize;
  UINTN                   VolVariableSize;
  UINT64                  HbPciD0F0RegBase;

  DEBUG ((DEBUG_INFO, "UpdatePlatformInfo Start.\n"));

  SetMem (&SetupVolatileData, sizeof (SETUP_VOLATILE_DATA),0);

  VolVariableSize = sizeof (SETUP_VOLATILE_DATA);
  Status = gRT->GetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  &SetupVolAttributes,
                  &VolVariableSize,
                  &SetupVolatileData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UpdatePlatformInfo Get SetupVolatileData Variable Error.\n"));
    return;
  }

  SetMem (&BoardInfoSetup, sizeof (BOARD_INFO_SETUP), 0);

  VariableSize = sizeof (BOARD_INFO_SETUP);
  Status = gRT->GetVariable (
                  L"BoardInfoSetup",
                  &gBoardInfoVariableGuid,
                  &BoardInfoAttributes,
                  &VariableSize,
                  &BoardInfoSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UpdatePlatformInfo Get BoardInfoSetup Variable Error.\n"));
    return;
  }

  //
  // Get VTd status
  //
  HbPciD0F0RegBase = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
  SetupVolatileData.VTdAvailable = (PciSegmentRead32 (HbPciD0F0RegBase + 0xe4) & BIT23) ? 0 : 1;

  // @todo separate board information varibales from Setup variables.
  BoardInfoSetup.RevisionId              = BOARD_INFO_SETUP_REVISION;
  BoardInfoSetup.EcEnable                = FixedPcdGetBool (PcdEcEnable);
  BoardInfoSetup.EcEspiFlashSharingMode  = PcdGet8 (PcdEcEspiFlashSharingMode);
  BoardInfoSetup.EcMajorRevision         = PcdGet8 (PcdEcMajorRevision);
  BoardInfoSetup.EcMinorRevision         = PcdGet8 (PcdEcMinorRevision);
  BoardInfoSetup.EcPlatformMajorRevision = PcdGet8 (PcdEcPlatformMajorRevision);
  BoardInfoSetup.EcBuildRevision         = PcdGet8 (PcdEcBuildRevision);
  BoardInfoSetup.BoardId                 = PcdGet16 (PcdBoardId);
  BoardInfoSetup.SpdPresent              = PcdGetBool (PcdSpdPresent);
  BoardInfoSetup.PlatformGeneration      = PcdGet8 (PcdPlatformGeneration);
  SetupVolatileData.PlatformFlavor = PcdGet8 (PcdPlatformFlavor);
  BoardInfoSetup.FabId          = PcdGet16 (PcdBoardRev);
  BoardInfoSetup.BoardBomId     = PcdGet16 (PcdBoardBomId);
  BoardInfoSetup.DisplayId         = PcdGet16 (PcdDisplayId);
  SetupVolatileData.PlatformType   = PcdGet8 (PcdPlatformType);
  SetupVolatileData.BoardType      = PcdGet8 (PcdBoardType);
  SetupVolatileData.PlatId         = (UINT16) LibPcdGetSku ();
  SetupVolatileData.PchGeneration  = PchGeneration ();
  SetupVolatileData.DockAttached   = PcdGetBool (PcdDockAttached);
  SetupVolatileData.IsS3Supported  = 0;

  // Update No of Type-C Port Supported
  if (PcdGet8 (VpdPcdUsbCUcmMode)) {
    SetupVolatileData.TypeCPortsSupportedMap = GetConnectableUsbCConnectorsMap ();
  }

  DEBUG ((DEBUG_INFO, "SetupVolatileData.PchGeneration = %d\n", SetupVolatileData.PchGeneration));
  SetupVolAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS;
  Status = gRT->SetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  SetupVolAttributes,
                  sizeof (SETUP_VOLATILE_DATA),
                  &SetupVolatileData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UpdatePlatformInfo Set SetupVolatileData Variable Error.\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = gRT->SetVariable (
                  L"BoardInfoSetup",
                  &gBoardInfoVariableGuid,
                  BoardInfoAttributes,
                  sizeof (BOARD_INFO_SETUP),
                  &BoardInfoSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UpdatePlatformInfo Set BoardInfoSetup Variable Error.\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  DEBUG ((DEBUG_INFO, "UpdatePlatformInfo End.\n"));

#else
  //
  // No Setup Data to update
  //
  DEBUG ((DEBUG_INFO, "UpdatePlatformInfo Null Function.\n"));

#endif
  return;
}
