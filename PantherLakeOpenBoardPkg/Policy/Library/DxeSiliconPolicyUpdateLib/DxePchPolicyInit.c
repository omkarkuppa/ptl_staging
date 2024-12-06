/**@file
  This file initialises and Installs GopPolicy Protocol.

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

@par Specification
**/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

typedef struct {
  UINT8    Reserved;
} PCH_NVS_AREA;

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gPchNvsAreaProtocolGuid;

/**
  This protocol is used to sync PCH information from POST to runtime ASL.
  This protocol exposes the pointer of PCH NVS Area only. Please refer to
  ASL definition for PCH NVS AREA.
**/
typedef struct {
  PCH_NVS_AREA                          *Area;
} PCH_NVS_AREA_PROTOCOL;

GLOBAL_REMOVE_IF_UNREFERENCED PCH_NVS_AREA_PROTOCOL    mPchNvsAreaProtocol;

EFI_STATUS
EFIAPI
CreatePchDxeConfigBlocks (
  IN OUT  VOID      **PchPolicy
  );

EFI_STATUS
EFIAPI
PchInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  VOID                        *PchPolicy
  );

/**
  Initialize PCH DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
PchPolicyInitDxe (
  IN EFI_HANDLE           ImageHandle
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE    PchHandle;
  VOID          *PchPolicy;

  //
  // Call CreatePchDxeConfigBlocks to create & initialize platform policy structure
  // and get all Intel default policy settings.
  //
  Status = CreatePchDxeConfigBlocks (&PchPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Install PchInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  PchHandle = NULL;
  Status = PchInstallPolicyProtocol (PchHandle, PchPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Install PchNvsAreaProtocolGuid
  //

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gPchNvsAreaProtocolGuid,
                  &mPchNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
