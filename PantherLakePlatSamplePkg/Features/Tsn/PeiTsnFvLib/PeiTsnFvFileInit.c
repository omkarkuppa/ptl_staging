/**@file
  TSN FV Installation

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

@par Specification
**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiTsnFvLib.h>
#include <Library/PeiSubRegionLib.h>

/**
  Get Tsn FV base address

  Base address will vary between SPI and UFS boot.

  @return FvBase associated with FvBinary FV.
**/
UINT32
GetFlashTsnFvBase (
  VOID
  )
{
  UINT32 FvBase;

  FvBase = FixedPcdGet32 (PcdFlashFvTsnMacAddressBase);

  DEBUG ((DEBUG_INFO, "GetFvTsnBase - 0x%x  ......\n", FvBase));
  return FvBase;
}

/**
  Install Base and Size Info Ppi for Siip Firmware Volume.

  @retval     EFI_SUCCESS  Always returns success.

**/
EFI_STATUS
EFIAPI
InstallTsnFv (
  VOID
)
{
  UINT32 FlashTsnFvBase;

  FlashTsnFvBase = GetFlashTsnFvBase ();
  // Report and install FV Info Ppi to Pei Core for TSN FV
  PeiServicesInstallFvInfoPpi (
    &((EFI_FIRMWARE_VOLUME_HEADER *)(UINTN)FlashTsnFvBase)->FileSystemGuid,
    (VOID *) FlashTsnFvBase,
    PcdGet32 (PcdFlashFvTsnMacAddressSize),
    NULL,
    NULL
    );
  return EFI_SUCCESS;
}

/**
  Build FV Hob for TSN Sub Region so it can be found in DXE

  @retval     EFI_SUCCESS  Always returns success.

**/
EFI_STATUS
EFIAPI
LoadFlashTsnFv (
  VOID
)
{
  UINT32 FlashTsnFvBase;
  UINT32 TsnFvBaseOrig;

  FlashTsnFvBase = GetFlashTsnFvBase ();
  TsnFvBaseOrig = FixedPcdGet32 (PcdFlashFvTsnMacAddressBase);

  BuildFvHob (
    (UINTN) FlashTsnFvBase,
    (UINTN) FixedPcdGet32 (PcdFlashFvTsnMacAddressSize)
  );

  return EFI_SUCCESS;
}

/*
  Reads TSN Sub region.

  Reads TSN sub region data from FV. Returns pointer to data.

  @param[out]  SubRegion Pointer to sub region data.

  @retval     EFI_SUCCESS  Function has completed successfully.
  @retval     Others       TSN region was not found
*/
EFI_STATUS
GetTsnSubRegion (
  OUT TSN_SUB_REGION** SubRegion
  )
{
  EFI_STATUS Status;
  VOID  *TsnSubRegionStart;
  UINTN TsnSubRegionSize;
  UINT32 CRC;

  DEBUG ((DEBUG_INFO, "GetTsnSubRegion() Start\n"));

  // Read Spi Flash
  Status = ReadSectionInformationFromFv (&gIntelFvTsnMacAddrBinGuid, EFI_SECTION_RAW, &TsnSubRegionStart, &TsnSubRegionSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Failed to get TSN Sub Region FV\n"));
    return Status;
  }

  *SubRegion = (TSN_SUB_REGION*) TsnSubRegionStart;

  CRC = CalculateCrc32(*SubRegion, sizeof(PCH_TSN_SUB_REGION_PORT));
  if (CompareMem (&(*SubRegion)->Signature, &CRC, sizeof(UINT32)) != 0) {
    DEBUG ((DEBUG_ERROR, "TSN sub region image loaded is corrupted.\n"));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "GetTsnSubRegion() End\n"));
  return Status;
}