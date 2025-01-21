/** @file
  This file contains the tests for the BootFirmwareMediaProtection BIT

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include "HstiIhvDxe.h"
#include <MeState.h>

#define V_SPI_BIOS_REGION_FLASH_WRITE_ACCESS_BIOS_GBE                        0x00A00000
#define V_SPI_BIOS_REGION_FLASH_WRITE_ACCESS_BIOS_GBE_PDR                    0x01A00000
#define V_SPI_BIOS_REGION_FLASH_WRITE_ACCESS_BIOS_GBE_EC                     0x10A00000
#define V_SPI_BIOS_REGION_FLASH_WRITE_ACCESS_BIOS_GBE_PDR_EC                 0x11A00000
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE           0x00000F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_PDR       0x00001F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_EC        0x00010F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_PDR_EC    0x00011F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI       0x00020F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_PDR   0x00021F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_EC    0x00030F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_EC_PDR     0x00031F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_PSE        0x00060F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_PSE_PDR    0x00061F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_EC_PSE     0x00070F00
#define V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_EC_PSE_PDR 0x00071F00
#define V_SPI_GBE_REGION_WRITE_READ_ACCESS                                   0x00800900
#define V_SPI_ME_REGION_WRITE_READ_ACCESS                                    0x00400D00
#define V_SPI_EC_REGION_WRITE_READ_ACCESS1                                   0x10010300
#define V_SPI_EC_REGION_WRITE_READ_ACCESS2                                   0x10010100


/**
  Run tests for BootFirmwareMediaProtection bit
**/
VOID
CheckBootFirmwareMediaProtection (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         Result;
  UINT32          Cpptv;
  UINT32          Data32;
  BOOLEAN         BildInDmiOrIoc;
  BOOLEAN         SpiBild;
  UINT32          MeStatus;
  UINT64          CodeReportedBitmap = 0;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION) == 0) {
    return;
  }

  Result = TRUE;

  DEBUG ((DEBUG_INFO, "  SPI Flash and BIOS Security Configuration\n"));
  DEBUG ((DEBUG_INFO, "      BIOS write-protection Test\n"));

  if (SpiIsWriteProtectionEnabled () == FALSE) {
    DEBUG((DEBUG_INFO, "        Unexpected Status: One or more bits misconfigured for BIOS Write-Protection\n"));
    DEBUG((DEBUG_INFO, "        Values expected: LE = 1; WPD = 0; EISS = 1\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_1,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_1
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      SPI flash descriptor security override pin-strap Test\n"));
  if (SpiIsFlashDescriptorOverrideEnabled ()) {
    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_2,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_2
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      SPI controller configuration is locked Test\n"));

  if (SpiIsFlashConfigurationLockDownEnabled () == FALSE) {
    DEBUG((DEBUG_INFO, "        Unexpected Status: SPI controller configuration Registers Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_3,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_3
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      BIOS Interface Lock Test\n"));
  BildInDmiOrIoc = IocIsBiosInterfaceLock ();
  SpiBild = SpiIsBiosInterfaceLockdownEnabled ();
  if ((BildInDmiOrIoc == FALSE) ||
      (SpiBild == FALSE)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: BildInDmiOrIoc = %x and/or BILD[7] = %x are not set\n", BildInDmiOrIoc, SpiBild));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_4,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_4
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      BIOS Top Swap Mode Test\n"));

  if (SpiIsTopSwapEnabled () == TRUE) {
    DEBUG((DEBUG_INFO, "        Unexpected Status: BIOS Top Swap Mode Enabled\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_5,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_5
      );
    Result = FALSE;
  }

  if (SpiIsSafModeActive() == FALSE) {
    DEBUG ((DEBUG_INFO, "      Component Property Parameter Table Valid Test\n"));

    SpiGetComponentPropertyParameterTable (0, &Cpptv);
    if (SpiIsCppValidBitSet (Cpptv) == FALSE) {
      DEBUG((DEBUG_INFO, "        Unexpected Status: Component Property Parameter Table Invalid.\n"));
      BuildAndAppendHstiUniqueStatusString (
        HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_6,
        HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_6,
        &CodeReportedBitmap,
        BIT6
        );
      Result = FALSE;
    }
    
    DEBUG ((DEBUG_INFO, "      Component Property Parameter Table Valid, Secondary Flash Part Test\n"));

    //
    // If Second part is present the verify Cpp Valid bit is set, otherwise if not present verify that it's clear
    //
    if (SpiGetComponentPropertyParameterTable (1, &Cpptv) == TRUE) {
      if (SpiIsCppValidBitSet (Cpptv) == FALSE) {
        DEBUG((DEBUG_INFO, "        Unexpected Status: Second SPI Flash part present, Cpp Valid Bit is set to 0\n"));

        BuildAndAppendHstiUniqueStatusString (
          HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_6,
          HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_6,
          &CodeReportedBitmap,
          BIT6
          );
        Result = FALSE;
      }
    }
  }
  DEBUG ((DEBUG_INFO, "      ME Firmware Status Test\n"));

  Status = MeBiosGetMeStatus (&MeStatus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "MeBiosGetMeStatus  = %r\n", Status));
  }

  if (ME_STATUS_ME_STATE_ONLY (MeStatus) != ME_READY)  {
    DEBUG((DEBUG_INFO, "        Unexpected Status: ME Firmware not in Normal Working Status = %x\n", (ME_STATUS_ME_STATE_ONLY (MeStatus))));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_8,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_8
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      BIOS Flash Descriptor Valid Test\n"));

  if (SpiIsFlashDescriptorValid () == FALSE) {
    DEBUG((DEBUG_INFO, "        Unexpected Status: BIOS Flash Descriptor Invalid\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_9,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_9
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      BIOS Region Flash Write Access Test\n"));
  ///
  /// Descriptor Main Access Read:  BIT 8
  /// Bios Main Access Read:        BIT 9
  /// CSE Main Access Read:         BIT 10
  /// GBE Main Access Read:         BIT 11
  /// PDR Main Access Read:         BIT 12
  /// EC  Main Access Read:         BIT 16
  /// Descriptor Main Access Write: BIT 20
  /// Bios Main Access Write:       BIT 21
  /// CSE Main Access Write:        BIT 22
  /// GBE Main Access Write:        BIT 23
  /// PDR Main Access Write:        BIT 24
  /// EC Main Access Write:         BIT 28
  /// Recommended Flash Region Read/Write access permissions as per SPI Programming Guide.
  ///
  Data32 = SpiGetBiosRegionFlashWriteAccess ();

  if ((Data32 != V_SPI_BIOS_REGION_FLASH_WRITE_ACCESS_BIOS_GBE) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_WRITE_ACCESS_BIOS_GBE_PDR) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_WRITE_ACCESS_BIOS_GBE_EC) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_WRITE_ACCESS_BIOS_GBE_PDR_EC)) {

    DEBUG ((DEBUG_INFO, "        Unexpected Status: Bios Region Main Write Access Invalid Production Permissions\n"));
    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_13,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_13
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      BIOS Region Flash Read Access Test\n"));
  Data32 = SpiGetBiosRegionFlashReadAccess ();

  if ((Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_PDR) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_EC) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_PDR_EC) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_PDR) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_EC) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_EC_PDR) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_PSE) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_PSE_PDR) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_EC_PSE) &&
      (Data32 != V_SPI_BIOS_REGION_FLASH_READ_ACCESS_DESCRIPTOR_BIOS_ME_GBE_SPI_EC_PSE_PDR)) {

    DEBUG ((DEBUG_INFO, "        Unexpected Status: Bios Region Main Read Access Invalid Production Permissions\n"));
    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_14,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_14
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      BIOS Main Read Access Grant Test\n"));

  if (SpiGetHostReadAccess () != 0) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: BMRAG Not Equal To 0x00\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_15,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_15
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      BIOS Main Write Access Grant Test\n"));

  if (SpiGetHostWriteAccess () != 0) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: BMWAG Not Equal To 0x00\n"));
    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_16,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_16
      );
    Result = FALSE;
  }


  DEBUG ((DEBUG_INFO, "      GbE Region Access Test\n"));
  if (SpiIsGbeRegionValid () == TRUE) {
    Data32 = SpiGetGbeRegionAccess ();
    if (Data32 != V_SPI_GBE_REGION_WRITE_READ_ACCESS) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: Gbe Region Access Flash Descriptor\n"));
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_17,
        HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_17
        );
      Result = FALSE;
    }
  }

  DEBUG ((DEBUG_INFO, "      ME Region Access Test\n"));

  Data32 = SpiGetCsmeRegionAccess ();
  if (Data32 != V_SPI_ME_REGION_WRITE_READ_ACCESS) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: ME Region Access Flash Descriptor\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_18,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_18
      );
    Result = FALSE;
  }
  DEBUG ((DEBUG_INFO, "      EC Region Access Test\n"));

  Data32 = SpiGetEcRegionAccess ();

  if ((Data32 != V_SPI_EC_REGION_WRITE_READ_ACCESS1) &&
      (Data32 != V_SPI_EC_REGION_WRITE_READ_ACCESS2)) {
      DEBUG((DEBUG_INFO, "        Unexpected Status: EC Region Access Flash Descriptor\n"));
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_19,
        HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_19
        );

    Result = FALSE;
  }

  DEBUG((DEBUG_INFO, "      Disallow WriteStatus SPI OPCODE Test\n"));

  if (SpiIsWriteStatusDisable () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: WriteStatus SPI OPCODE is Allowed\n"));

    BuildAndAppendHstiStatusString(
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_F,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_F
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "        Extended BIOS Decode Region Limit Locked Test\n"));
  if (IocIsExtendedBiosRegionLimitLocked () == FALSE) {
    DEBUG ((DEBUG_ERROR, "        Unexpected Status: Extended BIOS Decode Region limit is unlocked\n"));
    BuildAndAppendHstiStatusString(
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_11,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_11
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "        Extended BIOS Decode Region Base Locked Test\n"));
  if (SpiIsExtendedBiosRegionBaseLocked () == FALSE) {
    DEBUG ((DEBUG_ERROR, "        Unexpected Status: Extended BIOS Decode Region base is unlocked\n"));
    BuildAndAppendHstiStatusString(
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_12,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_12
      );
    Result = FALSE;
  }


  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               0,
               HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION
               );
    CheckStatusForHstiLibSet (Status);
  }

  return;
}
