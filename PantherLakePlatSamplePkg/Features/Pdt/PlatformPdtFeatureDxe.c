/** @file
  This driver collects platform features to manipulation ISH PDT (Platform Description Table).

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

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeMeLib.h>
#include <Uefi/UefiBaseType.h>
#include <SetupVariable.h>
#include <Library/DxeServicesLib.h>
#include <Library/PdtUpdateLib.h>
#include <Library/HobLib.h>

/*
  The function will send HeciSensorControlMsg command for sensor control.

  Sensor control data is expected to set in variable. When detecting the variable,
  BIOS sends Pdt Sensor Control Message to ISH, then delete variable.

*/
VOID
PdtSensorControl (
  VOID
  )
{
  EFI_STATUS                                Status;
  UINTN                                     Size;
  VOID                                      *SensorControlVariableBuffer;
  static CONST CHAR8                        FileName[] = "bios2ish";

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  Size                          = 0;
  SensorControlVariableBuffer   = NULL;
  Status = gRT->GetVariable (
                  L"SensorControlInfo",
                  &gPdtSensorControlGuid,
                  NULL,
                  &Size,
                  SensorControlVariableBuffer
                  );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    DEBUG ((DEBUG_ERROR, "Get Variable MultiPdtDxeInfo failed, Status = %r\n", Status));
  } else {
    SensorControlVariableBuffer = AllocateZeroPool (Size);
    if (SensorControlVariableBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "Allocate memory for Variable SensorControlInfo failed\n"));
    } else {
      Status = gRT->GetVariable (
                      L"SensorControlInfo",
                      &gPdtSensorControlGuid,
                      NULL,
                      &Size,
                      SensorControlVariableBuffer
                      );
      if (Status == EFI_SUCCESS) {
        DEBUG ((DEBUG_INFO, "Get SensorControlInfo Variable success\n"));
        Status = HeciSensorControlMsg (FileName, (UINT8 *) SensorControlVariableBuffer, (UINT16) Size);
        DEBUG ((DEBUG_INFO, "Send SensorControl Msg, Status = %r\n", Status));

        //
        // delete Variable SensorControlInfo
        //
        Status = gRT->SetVariable (L"SensorControlInfo", &gPdtSensorControlGuid, 0, 0, NULL);
        DEBUG ((DEBUG_INFO, "Delete Variable SensorControlInfo Status = %r\n", Status));
      } else {
        DEBUG ((DEBUG_ERROR, "Get Variable SensorControlInfo failed, Status = %r\n", Status));
      }
    }
  }
}

/*
  The function checks M-PDT binary resides in BIOS that contiains one or more PDT data.
  Then finds out the right PDT data with matched BomId and udpate PDT on the platform.

  @retval EFI_SUCCESS   PdtUpdate happened successfully or there is no request detected.

*/
EFI_STATUS
MultiPdtUpdate (
  VOID
  )
{
  EFI_STATUS                                Status;
  EFI_BOOT_MODE                             BootMode;
  SETUP_DATA                                SetupData;
  UINTN                                     Size;
  UINT32                                    BomId;
  UINT8                                     *MPdtFile;
  UINTN                                     MPdtFileSize;
  UINT32                                    MPdtBufferSize;
  BOOLEAN                                   PdtExist;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  //
  // Get boot mode.
  //
  BootMode = GetBootModeHob ();

  //
  // Skip if boot mode is BOOT_ON_FLASH_UPDATE
  //
  if (BootMode == BOOT_ON_FLASH_UPDATE) {
    DEBUG ((DEBUG_INFO, "Skip PDT update, BootMode is BOOT_ON_FLASH_UPDATE\n"));
    return EFI_SUCCESS;
  }

  Size = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &Size,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SetupData : Get variable failure.\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Skip MultiPdt update if MPdtSupport is disabled
  //
  if (!SetupData.MPdtSupport) {
    DEBUG ((DEBUG_INFO, "MultiPdtDxe MPdtSupport is Disabled\n"));
    return Status;
  }

  //
  // Get BomId value from Setup option
  //
  BomId = SetupData.MPdtSupport;

  //
  // Skip MultiPdt update if PdtExist non-volatile variable exist
  //
  Size = sizeof (PdtExist);
  Status = gRT->GetVariable (
                  L"PdtExist",
                  &gPdtExistGuid,
                  NULL,
                  &Size,
                  &PdtExist
                  );
  if ((Status != EFI_NOT_FOUND) && EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PdtExist : Get variable failure, status= %r\n", Status));
    return Status;
  }
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "PdtExist non-volatile variable exist\n"));
    return Status;
  }

  //
  // Extract M-PDT buffer in BIOS
  //
  Status = GetSectionFromFv (
             &gMPdtBinGuid,
             EFI_SECTION_RAW,
             0,
             (VOID **) &MPdtFile,
             &MPdtFileSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "MPdtFile buffer : GetSectionFromFv failure, status= %r\n", Status));
    return Status;
  }

  //
  // Get correct PDT file from M-PDT buffer based on BomId value,
  // then Update PDT image into ME data region through Heci interface
  //
  MPdtBufferSize = MPdtFileSize & 0xFFFFFFFF;
  Status = PdtUpdate (BomId, MPdtFile, MPdtBufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PdtUpdate failure, status= %r\n", Status));
    return Status;
  }

  //
  // Set PdtExist NV variable after updating PDT to avoid repetitive update.
  //
  PdtExist = TRUE;
  gRT->SetVariable (
         L"PdtExist",
         &gPdtExistGuid,
         EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
         sizeof (PdtExist),
         &PdtExist
         );

  DEBUG ((DEBUG_INFO, "PdtUpdate succeeded\n"));

  return Status;

}
/*
  Entry point for PlatformPdtFeature driver

  @param[in]  ImageHandle  Image Handle.
  @param[in]  SystemTable  EFI System Table.

  @retval     EFI_SUCCESS  Function has completed successfully.
  @retval     Others       All other error conditions encountered result in an ASSERT.
*/
EFI_STATUS
EFIAPI
PlatformPdtFeatureEntryPoint (
  IN EFI_HANDLE                             ImageHandle,
  IN EFI_SYSTEM_TABLE                       *SystemTable
  )
{

#if FixedPcdGet8(PcdMultiPdtUpdateEnable) == 0x1
  MultiPdtUpdate ();
#endif

#if FixedPcdGet8(PcdPdtSensorControlEnable) == 0x1
  PdtSensorControl ();
#endif

  return EFI_SUCCESS;
}
