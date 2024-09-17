/** @file
 Intel PEI SA Policy update by board configuration

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

#include "PeiPolicyBoardConfig.h"
#include <TcssPeiPreMemConfig.h>
#include <TcssPeiConfig.h>
#include <GpioV2Config.h>
#include <PlatformBoardId.h>
#include <TcssInfo.h>
#include <PolicyUpdateMacro.h>
#include <Library/PeiLib.h>
#include <Library/BmpSupportLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Ppi/IGpuPlatformPolicyPpi.h>

EFI_STATUS
EFIAPI
GetPeiPlatformLidStatus (
  OUT LID_STATUS  *CurrentLidStatus
  );

EFI_STATUS
EFIAPI
GetVbtData (
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
  );

PEI_IGPU_PLATFORM_POLICY_PPI PeiIGpuPlatform = {
  PEI_IGPU_PLATFORM_POLICY_REVISION,
  GetPeiPlatformLidStatus,
  GetVbtData
};

EFI_PEI_PPI_DESCRIPTOR  mPeiIGpuPlatformPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiGraphicsPlatformPpiGuid,
  &PeiIGpuPlatform
};

EFI_STATUS
EFIAPI
GetVbtData (
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
  )
{
  EFI_GUID                        FileGuid;
  EFI_GUID                        BmpImageGuid;
  UINTN                           Size;

  Size    = 0;

  DEBUG((DEBUG_INFO, "GetVbtData Entry\n"));

  CopyMem (&BmpImageGuid, PcdGetPtr(PcdIntelGraphicsVbtFileGuid), sizeof(BmpImageGuid));

  CopyMem(&FileGuid, &BmpImageGuid, sizeof(FileGuid));
  PeiGetSectionFromAnyFv(&FileGuid, EFI_SECTION_RAW, 0, (VOID**)VbtAddress, &Size);
  if (Size == 0) {
    DEBUG((DEBUG_ERROR, "Could not locate VBT\n"));
  } else {
    DEBUG ((DEBUG_INFO, "GetVbtData VbtAddress is 0x%x\n", *VbtAddress));
    DEBUG ((DEBUG_INFO, "GetVbtData Size is 0x%x\n", Size));
    *VbtSize   = (UINT32) Size;
  }
  DEBUG((DEBUG_INFO, "GetVbtData exit\n"));

  return EFI_SUCCESS;
}

/**
  This function will return Lid Status in PEI phase.

  @param[out] CurrentLidStatus

  @retval     EFI_SUCCESS
  @retval     EFI_UNSUPPORTED
**/

EFI_STATUS
EFIAPI
GetPeiPlatformLidStatus (
  OUT LID_STATUS  *CurrentLidStatus
  )
{
  *CurrentLidStatus = LidOpen;
  return EFI_SUCCESS;
}



/**
  Performs FSP SA PEI Policy initialization.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
UpdateGraphics(
  IGPU_PEI_CONFIG *IGpuConfig
  )
{
  EFI_STATUS                      Status;
  VOID                            *Buffer;
  UINTN                           Size;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *Blt;
  UINTN                           BltSize;
  UINTN                           Height;
  UINTN                           Width;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd;
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#endif

  DEBUG((DEBUG_INFO, "\nUpdating SA Policy in Post Mem\n"));
  UPDATE_POLICY_V2 (((FSPS_UPD *) FspsUpd)->FspsConfig.PeiGraphicsPeimInit, IGpuConfig->PeiDisplayConfig.PeiGraphicsPeimInit, 1);

  Size = 0;
  Buffer = NULL;
  PeiGetSectionFromAnyFv(PcdGetPtr(PcdIntelGraphicsVbtFileGuid), EFI_SECTION_RAW, 0, &Buffer, &Size);
  DEBUG((DEBUG_INFO, "Vbt Pointer from PeiGetSectionFromAnyFv is 0x%x\n", Buffer));
  DEBUG((DEBUG_INFO, "Vbt Size from PeiGetSectionFromAnyFv is 0x%x\n", Size));

  IGpuConfig->PeiDisplayConfig.GraphicsConfigPtr = Buffer;

  Size = 0;
  Buffer = NULL;
  PeiGetSectionFromAnyFv(&gTianoLogoGuid, EFI_SECTION_RAW, 0, &Buffer, &Size);
  DEBUG((DEBUG_INFO, "Buffer from PeiGetSectionFromAnyFv is 0x%x\n", Buffer));
  DEBUG((DEBUG_INFO, "Size from PeiGetSectionFromAnyFv is 0x%x\n", Size));

  if (Buffer != NULL) {
    Blt = NULL;
    Status = TranslateBmpToGopBlt (
              Buffer,
              Size,
              &Blt,
              &BltSize,
              &Height,
              &Width
              );
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "TranslateBmpToGopBlt, Status = %r\n", Status));
        ASSERT_EFI_ERROR (Status);
        return Status;
    }

    UPDATE_POLICY_V2 (((FSPS_UPD *) FspsUpd)->FspsConfig.LogoPixelWidth, IGpuConfig->PeiDisplayConfig.LogoPixelWidth, (UINT32)Width);
    UPDATE_POLICY_V2 (((FSPS_UPD *) FspsUpd)->FspsConfig.LogoPixelHeight, IGpuConfig->PeiDisplayConfig.LogoPixelHeight, (UINT32)Height);

    IGpuConfig->PeiDisplayConfig.BltBufferAddress = (VOID *) Blt;

  }

  //
  // Install mPeiIGpuPlatformPpi
  //
  DEBUG ((DEBUG_INFO, "Install mPeiIGpuPlatformPpi\n"));
  Status = PeiServicesInstallPpi (&mPeiIGpuPlatformPpi);

  return EFI_SUCCESS;
}

/**
  This function performs PEI SA Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyBoardConfig (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  DEBUG ((DEBUG_INFO, "Updating SA Policy by board config in Post-Mem\n"));

#if FixedPcdGet8(PcdFspModeSelection) == 0
  EFI_STATUS          Status;
  IGPU_PEI_CONFIG     *IGpuConfig;

  Status           = EFI_SUCCESS;
  IGpuConfig         = NULL;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *) &IGpuConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Update UPD: VBT
  //
  UpdateGraphics(IGpuConfig);
#endif

  return EFI_SUCCESS;
}
