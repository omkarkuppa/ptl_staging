/** @file
  Implementation of PeiBoardConfigLib.

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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <IntelRcStatusCode.h>
#include <Library/TimerLib.h>
#include <Library/EcMiscLib.h>
#include <Library/EcTcssLib.h>
#include <Library/PeiDxeBoardIdsLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/BaseLib.h>
#include <Library/MeInfoLib.h>
#include <Library/PciSegmentLib.h>

#include <PlatformBoardType.h>
#include <PlatformBoardId.h>
#include <PlatformBoardConfig.h>
#include <OemSetup.h>
#include <SetupVariable.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Library/SiliconInitLib.h>
#include <Library/PmcLib.h>
#include <Library/BoardConfigLib.h>
#include <BootStateLib.h>
#include <TbtNvmRetimer.h>
#include <Library/PchInfoLib.h>
#include <Library/HobLib.h>
#include <Library/BiosIdLib.h>
#include "BoardInfo.h"
#include <Register/HeciRegs.h>
#include <Register/Msr.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Ppi/IGpuPlatformPolicyPpi.h>
#include <Library/PeiGetFvInfoLib.h>
#include <HsioBoardConfig.h>
#include <Library/BaseMemoryLib.h>
#include <CnvBoardConfigPcd.h>
#include <Ucsi.h>

#define STALL_TIME                          1000000    // 1,000,000 microseconds = 1 second

#include <Library/PlatformUsbConfigLib.h>
#include <Library/TcssInfoLib.h>
#include <TcssPeiPreMemConfig.h>

#if FixedPcdGetBool (PcdFspModeSelection) == 0x1
static EFI_PEI_NOTIFY_DESCRIPTOR  mPlatformDebugStateChecksNotifyList  = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gFspSiliconInitDonePpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) PlatformDebugStateChecksCallback
};
#else
static EFI_PEI_NOTIFY_DESCRIPTOR  mPlatformDebugStateChecksNotifyList  = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) PlatformDebugStateChecksCallback
};
#endif


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

/**
 * @brief  This function returns VBT address and size in the PEI Phase.
  @param[out] VbtAddress
  @param[out] VbtSize

  @retval     EFI_SUCCESS
  @retval     EFI_NOT_FOUND
**/

EFI_STATUS
EFIAPI
GetVbtData (
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
  )
{
  EFI_GUID                        FileGuid;
  EFI_GUID                        BmpImageGuid;
  UINT32                          Size;

  Size    = 0;

  DEBUG ((DEBUG_INFO, "GetVbtData Entry\n"));

  CopyMem (&BmpImageGuid, PcdGetPtr (PcdIntelGraphicsVbtFileGuid), sizeof (BmpImageGuid));

  CopyMem (&FileGuid, &BmpImageGuid, sizeof (FileGuid));
  PeiGetSectionFromFv (FileGuid, (VOID**) VbtAddress, &Size);
  if (Size == 0) {
    DEBUG ((DEBUG_ERROR, "Could not locate VBT\n"));
  } else {
    DEBUG ((DEBUG_INFO, "GetVbtData VbtAddress is 0x%x\n", *VbtAddress));
    DEBUG ((DEBUG_INFO, "GetVbtData Size is 0x%x\n", Size));
    *VbtSize   = Size;
  }
  DEBUG ((DEBUG_INFO, "GetVbtData exit\n"));

  return EFI_SUCCESS;
}

/**
 Install mPeiIGpuPlatformPpi.
**/
VOID
InstallPeiGfxPlatformPpi (
  VOID
  )
{
  EFI_STATUS              Status;
  DEBUG ((DEBUG_INFO, "Install mPeiIGpuPlatformPpi\n"));
  Status = PeiServicesInstallPpi (&mPeiIGpuPlatformPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallPeiGfxPLatformPpi: PeiServicesInstallPpi failed\n"));
  }
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
  EFI_STATUS              Status;
  GPIOV2_PAD_STATE        GpioLidStatus;
  VPD_GPIO_PAD            *GpioVpd;

  GpioLidStatus = GpioV2StateHigh;
  GpioVpd = NULL;

  DEBUG ((DEBUG_INFO, "LidStatus Entry\n"));
  //
  // If the platform does not support a lid, the function must return EFI_UNSUPPORTED
  //
  if (PcdGet8 (PcdPlatformType) == TypeTrad && PcdGet8 (PcdPlatformFlavor) == FlavorDesktop) {
    DEBUG ((DEBUG_INFO, "Returning Lid status as unsupported to GOP for DT/AIO board\n"));
    return EFI_UNSUPPORTED;
  } else {
    GpioVpd = PcdGetPtr(VpdPcdGpioLidStatus);
    if (GpioVpd->GpioPad != 0x0) {
      Status = GpioV2GetRx (GpioVpd->GpioPad, &GpioLidStatus);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Invalid Lid GPIO: %x\n", GpioVpd->GpioPad));
        return EFI_UNSUPPORTED;
      }
      if (GpioLidStatus == GpioV2StateHigh) {
        *CurrentLidStatus = LidOpen;
      } else {
        *CurrentLidStatus = LidClosed;
      }
      DEBUG ((DEBUG_INFO, "LidStatus Exit\n"));
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_INFO, "LidStatus UnSupported\n"));
  return EFI_UNSUPPORTED;
}

/**
  Parse the input BoardInfo data with BOARD_ID_INFO structure and
  initialize the PcdBoardId, PcdBoardBomId, PcdBoardRev, PcdSpdPresent
  and PcdDisplayId.

  @param[in]    BoardInfo     BoardInfo data from EC.
  @retval       EFI_SUCCESS   Update the pcds successfully.
**/
EFI_STATUS
EFIAPI
ParseBoardInfo (
  IN UINT16           BoardInfo
  )
{
  BOARD_ID_INFO       *BoardInfoStructure;

  BoardInfoStructure = (BOARD_ID_INFO *)&BoardInfo;
  PcdSet16S (PcdBoardId, (UINT16)BoardInfoStructure->RvpFields.BoardId);
  PcdSet16S (PcdBoardBomId, (UINT16)BoardInfoStructure->RvpFields.BomId);
  PcdSet16S (PcdBoardRev, (UINT16)BoardInfoStructure->RvpFields.FabId);
  if (BoardInfoStructure->RvpFields.SpdPresent == 1) {
    PcdSetBoolS (PcdSpdPresent, TRUE);
  } else {
    PcdSetBoolS (PcdSpdPresent, FALSE);
  }

  PcdSet16S (PcdDisplayId, (UINT16)((BoardInfoStructure->RvpFields.DdiAConfig) | (BoardInfoStructure->RvpFields.DdiBConfig << BIT1)));

  return EFI_SUCCESS;
}

VOID
EFIAPI
InternalUpdateRvpBoardConfig (
  IN OUT UINT16         BoardId
  )
{
  UINTN         Size;
  UINT8         BoardType;
  UINT8         PlatformType;
  UINT8         PlatformFlavor;

  if (BoardId < mSizeOfmBoardIndex) {
    Size = StrSize (mBoardIdIndex[BoardId]);
    PcdSetPtrS (PcdBoardName, &Size, mBoardIdIndex[BoardId]);
  }

  //
  // Update Board Type/Platform Type/Platform Flavor
  //
  switch (BoardId) {
    case BoardIdPtlSimics:
    case BoardIdPtlUHLp5Rvp1:
    case BoardIdPtlUHHmpRvp:
    case BoardIdPtlUHLp5Rvp3:
    case BoardIdPtlUHDdr5Rvp4:
    case BoardIdPtlUHCammDTbTRvp2:
    case BoardIdPtlUHCammBep:
    case BoardIdPtlHLp5Gcs1:
    case BoardIdPtlHLp5Gcs2:
    case BoardIdPtlUHLp5Aep:
    case BoardIdPtlUHLp5MemSktmRvp:
    case BoardIdPtlUHLp5Adk1:
    case BoardIdPtlUHLp5Adk2:
    case BoardIdPtlUHLp5DerCore:
      BoardType = BoardTypeRvp;
      PlatformType = TypeUltUlx;
      PlatformFlavor = FlavorMobile;
      if(PcdSet64S(PcdAcpiDefaultOemTableId, ACPI_OEM_TABLE_ID_PTL) != EFI_SUCCESS)
      {
        DEBUG ((DEBUG_INFO, "Set PcdAcpiDefaultOemTableId error!!!\n"));
      }
      break;

    default:
      BoardType = BoardTypeRvp;
      PlatformType = TypeUnknown;
      PlatformFlavor = FlavorUnknown;
      break;
  }

  PcdSet8S (PcdBoardType, BoardType);
  PcdSet8S (PcdPlatformType, PlatformType);
  PcdSet8S (PcdPlatformFlavor, PlatformFlavor);

  DEBUG ((DEBUG_INFO, "PcdAcpiDefaultOemTableId is 0x%llX\n", PcdGet64 (PcdAcpiDefaultOemTableId)));
}

/**
  This function call checks if valid Board information is available in the stored region
  on the previous boot and restores them.

  @retval    TRUE    Valid Board information is available and restored successfully.
  @retval    FALSE   No valid Board information found.
**/
BOOLEAN
IsValidBoardInformationStored (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariablePpi;
  UINTN                             Size;
  BOARD_INFO_SETUP                  BoardInfoSetup;
  SETUP_DATA                        Setup;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **)&VariablePpi
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "IsValidBoardInformationStored : Read Only Variable PPI is not found.\n"));
    return FALSE;
  }

  //
  // 1. Check if Board information variables are available in NV storage.
  //    @todo decouple Board information variables from BOARD_INFO_SETUP.
  Size = sizeof (BOARD_INFO_SETUP);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"BoardInfoSetup",
                          &gBoardInfoVariableGuid,
                          NULL,
                          &Size,
                          &BoardInfoSetup
                          );
  if (!EFI_ERROR (Status) && (BoardInfoSetup.BoardId != BoardIdUnknown1) && (BoardInfoSetup.BoardId != 0)) {
    DEBUG ((DEBUG_INFO, "IsValidBoardInformationStored : Found Board information variables in NV storage.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "IsValidBoardInformationStored : Board information variables are not stored in NV storage.\n"));
    return FALSE;
  }

  Size = sizeof (SETUP_DATA);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"Setup",
                          &gSetupVariableGuid,
                          NULL,
                          &Size,
                          &Setup
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Setup variables are not stored in NV storage.\n"));
    return FALSE;
  }

  //
  // 2. Check if this is NOT the first boot after BIOS/EC update by Capsule update.
  //
#if FixedPcdGetBool(PcdCapsuleEnable) == 1
  if (!IsBootStatePresent ()) {
    DEBUG ((DEBUG_INFO, "IsValidBoardInformationStored : BootStateAfterCapsule variable is not present or TRUE.\n"));
    DEBUG ((DEBUG_INFO, "This is the very first boot after BIOS flash or Capsule update for EC/BIOS.\n"));
    return FALSE;
  }
#endif

  //
  // 3. Check if this is NOT the first boot after RTC clear.
  //    Technically Board information variables are not impacted by RTC clear, but check for fail safe to force to read EC.
  if (!PmcIsRtcBatteryGood ()) {
    DEBUG ((DEBUG_INFO, "IsValidBoardInformationStored : This is the very first boot after RTC clear.\n"));
    return FALSE;
  }

  /*
    Note : The check logic #1-#3 do not capture the following scenario. Recommendation is to do RTC clear to support those.
     - EC is updated by flash programming tool.
     - HW rework is performed on the board for BomId, FabId or BoardID update.
  */

  DEBUG ((DEBUG_INFO, "IsValidBoardInformationStored : Valid Board information is available.\n"));

  //
  // Restore the valid Board information to PCDs
  //
  DEBUG ((DEBUG_INFO, "IsValidBoardInformationStored : Restoring Board information.\n"));
  PcdSetBoolS (PcdEcPresent, BoardInfoSetup.EcPresent);
  PcdSet8S (PcdEcEspiFlashSharingMode, BoardInfoSetup.EcEspiFlashSharingMode);
  PcdSet8S (PcdEcPeciMode, Setup.EcPeciMode);
  PcdSet8S (PcdEcMajorRevision, BoardInfoSetup.EcMajorRevision);
  PcdSet8S (PcdEcMinorRevision, BoardInfoSetup.EcMinorRevision);
  PcdSet8S (PcdEcPlatformMajorRevision, BoardInfoSetup.EcPlatformMajorRevision);
  PcdSet8S (PcdEcBuildRevision, BoardInfoSetup.EcBuildRevision);

  DEBUG ((DEBUG_INFO, "EcEnable: %x\n", BoardInfoSetup.EcEnable));
  DEBUG ((DEBUG_INFO, "EcPresent: %x\n", BoardInfoSetup.EcPresent));
  DEBUG ((DEBUG_INFO, "  +==============================================+\n"));
  DEBUG ((DEBUG_INFO, "  | EC Major Revision:          %02X         |\n", BoardInfoSetup.EcMajorRevision));
  DEBUG ((DEBUG_INFO, "  | EC Minor Revision:          %02X         |\n", BoardInfoSetup.EcMinorRevision));
  DEBUG ((DEBUG_INFO, "  | EC Platform Major Revision: %02X         |\n", BoardInfoSetup.EcPlatformMajorRevision));
  DEBUG ((DEBUG_INFO, "  | EC Build Revision:          %02X         |\n", BoardInfoSetup.EcBuildRevision));
  DEBUG ((DEBUG_INFO, "  +==============================================+\n"));

  PcdSet16S (PcdBoardId, BoardInfoSetup.BoardId);
  PcdSet16S (PcdBoardBomId, BoardInfoSetup.BoardBomId);
  PcdSet16S (PcdBoardRev, BoardInfoSetup.FabId);
  PcdSetBoolS (PcdSpdPresent, BoardInfoSetup.SpdPresent);
  PcdSet8S (PcdPlatformGeneration, BoardInfoSetup.PlatformGeneration);
  PcdSet16S (PcdDisplayId, BoardInfoSetup.DisplayId);

  DEBUG ((DEBUG_INFO, "  BoardId is 0x%x\n", PcdGet16 (PcdBoardId)));
  DEBUG ((DEBUG_INFO, "  BomId is 0x%x\n", PcdGet16 (PcdBoardBomId)));
  DEBUG ((DEBUG_INFO, "  FabId is 0x%x\n", PcdGet16 (PcdBoardRev)));
  DEBUG ((DEBUG_INFO, "  SpdPresent = %x\n", PcdGetBool (PcdSpdPresent)));
  DEBUG ((DEBUG_INFO, "  DisplayId = %x\n" , PcdGet16 (PcdDisplayId)));

  return TRUE;
}

/**
  Procedure to detect current board HW configuration.

**/
VOID
EFIAPI
GetBoardConfig (
  VOID
  )
{
  UINT16          BoardId;
  RETURN_STATUS   Status;
#if FixedPcdGetBool (PcdEcEnable) == 1
  UINT8           DataBuffer[4];
  UINT8           EcData;
  UINT16          BoardInfo;
  UINT8           Retry;
  Retry           = 0;
#endif

  //
  // Get Platform Info and fill the PCD
  //
  BoardId   = BoardIdUnknown1;
  PcdSet8S (PcdBoardType, BoardTypeMax);

  EarlyLpcIoDecode();
  Status = EFI_UNSUPPORTED;
#if FixedPcdGetBool (PcdEcEnable) == 1
  Status = WakeUpEc ();
#endif
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Wake Up EC Fail. Status = %r \n", Status));
    PcdSetBoolS (PcdEcPresent, FALSE);
  } else {
    PcdSetBoolS (PcdEcPresent, TRUE);
  }

  if ( (PcdGetBool (PcdEcPresent) == TRUE) &&
       (!IsValidBoardInformationStored ()) ) {
#if FixedPcdGetBool (PcdEcEnable) == 1
    //
    // Detect EC Revision
    //
    DEBUG((DEBUG_INFO, "Reading EC to set Board information to PCDs \n"));
    Retry = 5;
    do {
      Status = DetectEcSupport ((UINT8 *)DataBuffer);
      Retry--;
      if ((RETURN_ERROR (Status)) && (Retry != 0)) {
        MicroSecondDelay (STALL_TIME);
      }
    } while ((RETURN_ERROR (Status)) && (Retry != 0));
    if (!RETURN_ERROR (Status)) {
      // Byte3[4:3] 0 = G3
      //            1 = SAF
      //            2 = MAF
      PcdSet8S (PcdEcEspiFlashSharingMode, (DataBuffer[3]>>3)&0x3);

      // Byte3[1] 0 = Legacy PECI mode        // default
      //          1 = PECI over eSPI mode
      PcdSet8S (PcdEcPeciMode, (DataBuffer[3]>>1)&0x1);

      Status = DetectEcRevision ((UINT8 *)DataBuffer);
      if (IsSimicsEnvironment () || IsHfpgaEnvironment ()) {
        if (Status == EFI_TIMEOUT) {
          Status = EFI_SUCCESS;
        }
      }

      if (!EFI_ERROR (Status)) {
        PcdSet8S (PcdEcMajorRevision,         DataBuffer[0]);
        PcdSet8S (PcdEcMinorRevision,         DataBuffer[1]);
        PcdSet8S (PcdEcPlatformMajorRevision, DataBuffer[2]);
        PcdSet8S (PcdEcBuildRevision,         DataBuffer[3]);

        DEBUG ((DEBUG_INFO, "+==============================================+\n"));
        DEBUG ((DEBUG_INFO, "  | EC Major Revision:          %02X         |\n", DataBuffer[0]));
        DEBUG ((DEBUG_INFO, "  | EC Minor Revision:          %02X         |\n", DataBuffer[1]));
        DEBUG ((DEBUG_INFO, "  | EC Platform Major Revision: %02X         |\n", DataBuffer[2]));
        DEBUG ((DEBUG_INFO, "  | EC Build Revision:          %02X         |\n", DataBuffer[3]));
        DEBUG ((DEBUG_INFO, "+==============================================+\n"));
      }
    }
    //
    // get RVP board ID
    //

    //
    // Get BoardInfo from EC
    //
    DEBUG ((DEBUG_INFO, "Getting BoardInfo From EC ...\n"));
    do {
      Status = GetBoardInfo (&BoardInfo);
    } while (Status != EFI_SUCCESS);
    DEBUG ((DEBUG_INFO, "BoardInfo from EC: 0x%x \n", BoardInfo));
    ParseBoardInfo (BoardInfo);
#else
    // todo: get board ID on an EC less board. hard code BoardIdUnknown1 for temp use
    PcdSet16S (PcdBoardId, BoardIdUnknown1);
#endif
  }

  BoardId = PcdGet16 (PcdBoardId);
  //
  // update RVP board config
  //
  InternalUpdateRvpBoardConfig (BoardId);

#if FixedPcdGetBool (PcdEcEnable) == 1
  if ((PcdGetBool (PcdEcPresent) == TRUE) &&
      (((PcdGet8 (PcdPlatformFlavor) == FlavorMobile) ||
        (PcdGet8 (PcdPlatformFlavor) == FlavorMobileWorkstation)))
     ) {
    //
    // check dock status if support
    //
    Status = GetVirtualDockStatus (&EcData);
    if ((Status == EFI_SUCCESS) && (EcData & 1)) {
      PcdSetBoolS (PcdDockAttached, TRUE);
    }
    //
    // Notify EC to set PECI mode
    //
    Status = EcSetPeciMode (PcdGet8 (PcdEcPeciMode));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Fail to set PECI mode with EC Data %02X\n", PcdGet8 (PcdEcPeciMode)));
      Status = DetectEcSupport ((UINT8 *)DataBuffer);
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Set PcdEcPeciMode back to the original value\n"));
        PcdSet8S (PcdEcPeciMode, (DataBuffer[3] >> 1) & 0x1);
      } else {
        DEBUG ((DEBUG_ERROR, "Fail to detect EcSupport!!\n"));
      }
    }
  }
#endif // endif (PcdEcEnable) == 1


  DEBUG ((DEBUG_INFO, "Platform Information:\n"));
  DEBUG ((DEBUG_INFO, "PlatformType: %x\n", PcdGet8 (PcdPlatformType)));
  DEBUG ((DEBUG_INFO, "BoardName: %s\n", PcdGetPtr (PcdBoardName)));
  DEBUG ((DEBUG_INFO, "PlatformFlavor: %x\n", PcdGet8 (PcdPlatformFlavor)));
  DEBUG ((DEBUG_INFO, "BoardID: 0x%x\n", BoardId));
  DEBUG ((DEBUG_INFO, "BoardRev: %x\n", PcdGet16 (PcdBoardRev)));
  DEBUG ((DEBUG_INFO, "BoardBomId: %x\n", PcdGet16 (PcdBoardBomId)));
  DEBUG ((DEBUG_INFO, "BoardType: %x\n", PcdGet8 (PcdBoardType)));
  DEBUG ((DEBUG_INFO, "PlatformGeneration: %x\n", PcdGet8 (PcdPlatformGeneration)));
  DEBUG ((DEBUG_INFO, "PcdEcEnable: %d\n", FixedPcdGetBool (PcdEcEnable)));
  DEBUG ((DEBUG_INFO, "EcPresent: %d\n", PcdGetBool (PcdEcPresent)));
  DEBUG ((DEBUG_INFO, "SpdPresent: %d\n", PcdGetBool (PcdSpdPresent)));
  DEBUG ((DEBUG_INFO, "DisplayId %x\n", PcdGet16 (PcdDisplayId)));

  //
  // Halt the system if BoardID unknown
  //
  if (BoardId == BoardIdUnknown1) {
    UINT32 code = 0;

    switch (PcdGet8 (PcdBoardType)) {
      case BoardTypeRvp:
        code = 0xBD10;
        break;
      case BoardTypeSv:
        code = 0xBD20;
        break;
      default:
        code = 0xBD30;
        break;
    }
    DEBUG ((DEBUG_ERROR, "Platform BoardID unknown, BOARD ID =0x%x, halting the system !\n", code));

    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_INIT_BOARD_ID_UNKNOWN); // PostCode (code);
    CpuDeadLoop ();
  }

}

/**
  Count the number of GPIO settings in the Table.

  @param[in]  GpioTable   The pointer of GPIO config table
  @param[out] GpioCount   The number of GPIO config entries
**/
VOID
GetGpioTableSize (
  GPIOV2_INIT_CONFIG   *GpioTable,
  OUT UINT16           *GpioCount
  )
{
  *GpioCount = 0;
  if(GpioTable != NULL) {
    while (GpioTable[*GpioCount].GpioPad != 0 && *GpioCount < MAX_GPIO_PINS) {
      DEBUG ((DEBUG_INFO, "GpioTable[%d]->GpioPad = %x \n", *GpioCount, GpioTable[*GpioCount].GpioPad));
      (*GpioCount) ++;
    }
  } else {
    DEBUG ((DEBUG_INFO, "GpioTable is NULL\n"));
  }
  DEBUG ((DEBUG_INFO, "GetGpioTableSize() GpioCount = %d\n", *GpioCount));
}

/**
Configure GPIO Table setting to PcdBoardGpioTablePreMem && PcdBoardGpioTable

@param[in]  GpioTable   The pointer of GPIO config table
@param[in]  IsPostMem   Is call from PostMem/PreMem
                        True - PostMem, False - PreMem
**/
VOID
ConfigureGpioTabletoPCD(
  IN GPIOV2_INIT_CONFIG   *GpioTable,
  IN UINT16               IsPostMem
  )
{
  UINT16        GpioCount = 0;
  UINTN         Size = 0;
  EFI_STATUS    Status = EFI_SUCCESS;
  BOOLEAN       DisableVpdGpioTable = FALSE;

  DEBUG ((DEBUG_INFO, "ConfigureGpioTabletoPCD() Start\n"));

  DisableVpdGpioTable = (BOOLEAN) PcdGetBool (PcdDisableVpdGpioTable);
  DEBUG((DEBUG_INFO, "PcdDisableVpdGpioTable = %d\n", DisableVpdGpioTable));
  if (!DisableVpdGpioTable) {
    if (GpioTable != NULL) {
      GetGpioTableSize (GpioTable, &GpioCount);
      if (GpioCount) {
        if (IsPostMem == POST_MEM) {    // Post Mem GPIO Configuration
          if (GpioCount >= MAX_GPIO_PINS) {
            DEBUG ((DEBUG_ERROR, "GpioTable entries exceeds limit, Configure only MAX_GPIO_PINS Pins.\n"));
            GpioCount = MAX_GPIO_PINS;
          }
          DEBUG ((DEBUG_INFO, "GpioTable Count = %d\n", GpioCount));
          Size = (UINTN) (GpioCount * sizeof (GPIOV2_INIT_CONFIG));
          Status = PcdSetPtrS (PcdBoardGpioTable, &Size, GpioTable);
        } else if (IsPostMem == PRE_MEM) {    // Pre Mem GPIO Configuration
          if (GpioCount >= MAX_PRE_MEM_GPIO_PINS) {
            DEBUG ((DEBUG_ERROR, "PreMem GpioTable entries exceeds limit, Configure only MAX_PRE_MEM_GPIO_PINS Pins.\n"));
            GpioCount = MAX_PRE_MEM_GPIO_PINS;
          }
          DEBUG ((DEBUG_INFO, "GpioTable Count = %d\n", GpioCount));
          Size = (UINTN) (GpioCount * sizeof (GPIOV2_INIT_CONFIG));
          Status = PcdSetPtrS (PcdBoardGpioTablePreMem, &Size, GpioTable);
        } else if (IsPostMem == EARLY_PRE_MEM) {    // Pre Mem GPIO Configuration
          if (GpioCount >= MAX_PRE_MEM_GPIO_PINS) {
            DEBUG((DEBUG_ERROR, "EarlyPreMem GpioTable entries exceeds limit, Configure only MAX_PRE_MEM_GPIO_PINS Pins.\n"));
            GpioCount = MAX_PRE_MEM_GPIO_PINS;
          }
          DEBUG((DEBUG_INFO, "EarlyPreMem GpioTable Count = %d\n", GpioCount));
          Size = (UINTN)(GpioCount * sizeof(GPIOV2_INIT_CONFIG));
          Status = PcdSetPtrS(PcdBoardGpioTableEarlyPreMem, &Size, GpioTable);
        }
        ASSERT_EFI_ERROR (Status);
      } else {
        DEBUG ((DEBUG_INFO, "GpioTable is Empty\n"));
      }
    } else {
      DEBUG ((DEBUG_INFO, "GpioTable is NULL\n"));
    }
  } else {
    DEBUG ((DEBUG_INFO, "PcdDisableVpdGpioTable is TRUE, GPIO Tables will be updated by PCT PEIM \n"));
  }

  DEBUG ((DEBUG_INFO, "ConfigureGpioTabletoPCD() End\n"));
}

/**
  Configures GPIO

  @param[in]  GpioTable       Point to Platform Gpio table
  @param[in]  GpioTableCount  Number of Gpio table entries
**/
STATIC
VOID
ConfigureGpio (
  IN GPIOV2_INIT_CONFIG                 *GpioTable,
  IN UINT16                              GpioTableCount
  )
{
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "ConfigureGpio() Start\n"));

  Status = GpioV2ConfigurePads (GpioTable, GpioTableCount);
  ASSERT_EFI_ERROR (Status);

  Status = GpioV2SetConflictCheckData (GpioTable, GpioTableCount);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "ConfigureGpio() End\n"));
}

/**
  Configure GPIO Before Memory is initialized.

  @param[in]  GpioTable  Pointer to Gpio table
**/
VOID
GpioInit (
  IN GPIOV2_INIT_CONFIG *GpioTable
  )
{
  UINT16             GpioCount;

  if (GpioTable != 0) {
    GpioCount = 0;
    GetGpioTableSize (GpioTable, &GpioCount);
    if (GpioCount != 0) {
      ConfigureGpio ((VOID *) GpioTable, (UINTN) GpioCount);
    }
  }
}

/**
  Configure GPIO group GPE tier.

**/
VOID
GpioGroupTierInit (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "GpioGroupTierInit Start\n"));

  if (PcdGet32 (PcdGpioGroupToGpeDw0)) {
    GpioV2SetGroupToGpeMapping (PcdGet32 (PcdGpioGroupToGpeDw0),
                                PcdGet32 (PcdGpioGroupToGpeDw1),
                                PcdGet32 (PcdGpioGroupToGpeDw2));
  }

  DEBUG ((DEBUG_INFO, "GpioGroupTierInit End\n"));
}

/**
  Update Pch Type in BIOS ID string based on PchSeries.

**/
VOID
UpdatePchTypeInBoardId (
  VOID
  )
{
  EFI_STATUS              Status;
  BIOS_ID_IMAGE           *BiosIdImage;
  EFI_PEI_HOB_POINTERS    HobList;
  UINT8                   PchType;

  DEBUG ((DEBUG_INFO, "UpdatePchTypeInBoardId Start\n"));

  Status = GetBiosId (NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Status: %r\n",Status));
  }

  HobList.Guid = GetFirstGuidHob (&gBiosIdGuid);
  ASSERT(HobList.Guid != NULL);
  BiosIdImage = (BIOS_ID_IMAGE *) GET_GUID_HOB_DATA (HobList.Guid);

  if (BiosIdImage != NULL) {
    PchType = PchSeries ();

    switch (PchType) {
      case PCH_M:
        BiosIdImage->BiosIdString.BoardId[3] = 'M';
        break;
      case PCH_N:
        BiosIdImage->BiosIdString.BoardId[3] = 'N';
        break;
    }
  }
}

/**
  Set TCSS Port Capability PCD

**/
VOID
EFIAPI
SetTcssPortCapPcd (
  VOID
  )
{
  UINT8                            ConnectorIndex;
  UINT8                            *TcssPortCap;
  UINT32                           TcssPortCapMap;
  USB_CONNECTOR_HOB_DATA           *UsbConnectorHobDataPtr;
  USBC_CONNECTOR_HOB_DATA          *UsbCConnectorHobDataPtr;
  USB_CONNECTOR_BOARD_CONFIG       *UsbConnectorBoardConfig;
  USBC_CONNECTOR_BOARD_CONFIG      *UsbCConnectorBoardConfig;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  SA_SETUP                         SaSetup;
  UINTN                            DataSize;
  EFI_STATUS                       Status;
  UINT8                            PortNum;
  BOOLEAN                          IsUsbConnectorConnectableMapChanged;

  TcssPortCapMap = 0;
  TcssPortCap = (UINT8 *) &TcssPortCapMap;
  IsUsbConnectorConnectableMapChanged = FALSE;

  UsbConnectorHobDataPtr  = GetUsbConnectorHobData ();
  UsbCConnectorHobDataPtr = GetUsbCConnectorHobData ();
  if (UsbConnectorHobDataPtr == NULL || UsbCConnectorHobDataPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorHobDataPtr or UsbCConnectorHobDataPtr is not available!!\n", __FUNCTION__));
    return;
  }

  UsbConnectorBoardConfig = UsbConnectorHobDataPtr->UsbConnectorBoardConfig;
  if (UsbConnectorBoardConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorBoardConfig is not available!!\n", __FUNCTION__));
    return;
  }

  UsbCConnectorBoardConfig = UsbCConnectorHobDataPtr->UsbCConnectorBoardConfig;
  if (UsbCConnectorBoardConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: UsbCConnectorBoardConfig is not available!!\n", __FUNCTION__));
    return;
  }

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (!EFI_ERROR (Status)) {
    DataSize = sizeof (SA_SETUP);
    Status = VariableServices->GetVariable (
                                 VariableServices,
                                 L"SaSetup",
                                 &gSaSetupVariableGuid,
                                 NULL,
                                 &DataSize,
                                 &SaSetup
                                 );
    ASSERT_EFI_ERROR (Status);
  } else {
    DEBUG ((DEBUG_ERROR, "%a: Failed to get SaSetup!!\n", __FUNCTION__));
    return;
  }

  for (ConnectorIndex = 0; ConnectorIndex < UsbCConnectorHobDataPtr->NumberOfUsbCConnectors; ConnectorIndex++) {
    //
    // Look for TCSS USB3 port and update port capability
    //
    if (UsbConnectorBoardConfig[ConnectorIndex].Usb3Controller == TCSS_USB3 &&
        UsbConnectorBoardConfig[ConnectorIndex].Usb3PortNum < MAX_TCSS_USB3_PORTS) {
      PortNum = (UINT8) UsbConnectorBoardConfig[ConnectorIndex].Usb3PortNum;
      if (UsbConnectorBoardConfig[ConnectorIndex].ConnectorConnectable == CONNECTABLE) {
        if (UsbCConnectorBoardConfig[ConnectorIndex].Usb4Supported == 1) {
          TcssPortCap[PortNum] = (UsbCConnectorBoardConfig[ConnectorIndex].PcieSupported == 1) ? FullFunction : NoPcie;
        } else {
          TcssPortCap[PortNum] = NoThunderbolt;
        }
        if (TcssPortCap[PortNum] == FullFunction) {
          switch (SaSetup.TcssPortConf[PortNum]) {
            case NoThunderbolt:
              UsbCConnectorBoardConfig[ConnectorIndex].Usb4Supported = 0;
              UsbCConnectorBoardConfig[ConnectorIndex].Tbt3Supported = 0;
              UsbCConnectorBoardConfig[ConnectorIndex].PcieSupported = 0;
              break;
            case NoPcie:
            case DpOnly:
              UsbCConnectorBoardConfig[ConnectorIndex].Tbt3Supported = 0;
              UsbCConnectorBoardConfig[ConnectorIndex].PcieSupported = 0;
              break;
          }
        }
        if (SaSetup.TcssPortConf[PortNum] == UsbCDisable) {
          UsbConnectorBoardConfig[ConnectorIndex].ConnectorConnectable = UNCONNECTABLE;
          IsUsbConnectorConnectableMapChanged = TRUE;
        }
      } else {
        TcssPortCap[PortNum] = UsbCDisable;
      }
    }
  }

  if (IsUsbConnectorConnectableMapChanged) {
    // Update the number of connectable USBC connectors and map
    Status = UpdateNumOfConnectableUsbCConnectorsAndMap ();
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "TcssPortCapMap = 0x%08x\n", TcssPortCapMap));
  PcdSet32S (PcdTcssPortCapMap, TcssPortCapMap);
}

/**
  Update UcxxDriverType PCD based on TcssUcmDevice setup menu and
  PortUcxxDriverSupport PCD based on
  TestDisableTcssPortUcxxSupport[x] setup menu.

**/
VOID
EFIAPI
UpdateUcxxDriverTypePcd (
  VOID
  )
{

  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  EFI_STATUS                        Status;
  SETUP_DATA                        Setup;
  UINTN                             DataSize;
  UINTN                             Size;
  UINT8                             UsbCHostFlags;
  EFI_BOOT_MODE                     BootMode;

  Status = EFI_SUCCESS;
  UsbCHostFlags = 0x0;

  DEBUG ((DEBUG_INFO, "[TCSS] UpdateUcxxDriverTypePcd: Start\n"));

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (!EFI_ERROR (Status)) {
    DataSize = sizeof (SETUP_DATA);
    Status = VariableServices->GetVariable (
                                 VariableServices,
                                 L"Setup",
                                 &gSetupVariableGuid,
                                 NULL,
                                 &DataSize,
                                 &Setup
                                 );
    if (!EFI_ERROR (Status)) {
      PcdSet8S (PcdUcmSelection, Setup.TcssUcmDevice);
      SetUcsiRevisionPcd ();
      if (BootMode != BOOT_ON_S3_RESUME) {
        UsbCHostFlags = (Setup.TcssUcmDevice > 0) ? HOST_USBC_UCSI_STATUS : 0;
        if (PcdGetBool (PcdEcPresent) == TRUE) {
          UpdateUsbCHostFlagsToEc (&UsbCHostFlags);
        }
      }

      Size = sizeof (UINT8) * 10;
      PcdSetPtrS (PcdPortUcxxDriverSupport, &Size, &Setup.TestDisableTcssPortUcxxSupport[0]);
   }
  }

  DEBUG ((DEBUG_INFO, "UpdateUcxxDriverTypePcd() End\n"));

  return;
}

/**
  Setting the number of retimer from the configuration table.

  @param[in]  RetimerFlashConfig      The pointer of retimer flash configuration table
**/
VOID
SetRetimerFlashNumber (
  USBC_RETIMER_FLASH_BOARD_CONFIG       *RetimerFlashConfig
  )
{
  UINT8 RetimerFlashCount;

  RetimerFlashCount = 0;

  if(RetimerFlashConfig != NULL) {
    while (RetimerFlashCount < MAX_RETIMER_FLASH_PART && (RetimerFlashConfig[RetimerFlashCount].CascadedRetimerFlashIndex != 0)) {
      DEBUG ((DEBUG_INFO, "RetimerFlashConfig[%d] : First supported USBC connector index is %x \n", RetimerFlashCount, RetimerFlashConfig[RetimerFlashCount].FirstUsbCConnectorIndex));
      RetimerFlashCount++;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "RetimerFlashConfig is NULL\n"));
  }
  DEBUG ((DEBUG_INFO, "SetRetimerNumberFromCfgTable(): RetimerFlashCount = %d\n", RetimerFlashCount));
  PcdSet8S (PcdUsbCRetimerFlashNumber, RetimerFlashCount);
}

/**
  Setting PcdUsbCRetimerConfigTable from UsbC connector/Retimer configuration table.

  @param[in]  RetimerFlashConfig       The pointer of retimer flash configuration table

**/
VOID
SetRetimerCfgTablePcd (
  IN USBC_RETIMER_FLASH_BOARD_CONFIG   *RetimerFlashConfig
  )
{
  UINTN                           Size;
  UINT8                           Index;
  UINT8                           RetimerCount;
  UINT16                          UsbCConnectorIndex;
  UINT16                          HOST_DMA_BUS_NUM;
  UINT16                          HOST_DMA_DEV_NUM;
  UINT16                          HOST_DMA0_FUN_NUM;
  UINT16                          HOST_DMA1_FUN_NUM;
  RETIMER_CONFIG_ARRAY            *RetimerCfgTable;
  USBC_CONNECTOR_HOB_DATA         *UsbCConnectorHobDataPtr;
  USBC_CONNECTOR_BOARD_CONFIG     *UsbCConnectorBoardConfig;
  EFI_STATUS                      Status;

  DEBUG ((DEBUG_INFO, "SetRetimerCfgTablePCD() Start\n"));

  Size = 0;
  UsbCConnectorIndex = 0;
  Status = EFI_SUCCESS;
  RetimerCount = PcdGet8 (PcdUsbCRetimerFlashNumber);
  HOST_DMA_BUS_NUM = (UINT16) GetTbtDmaBusNumber();
  HOST_DMA_DEV_NUM = (UINT16) GetTbtDmaDevNumber();
  HOST_DMA0_FUN_NUM = (UINT16) GetTbtDma0FuncNumber();
  HOST_DMA1_FUN_NUM = (UINT16) GetTbtDma1FuncNumber();

  Status = PeiServicesAllocatePool (sizeof (RETIMER_CONFIG_ARRAY), (VOID **) &RetimerCfgTable);
  ASSERT_EFI_ERROR(Status);

  UsbCConnectorHobDataPtr = GetUsbCConnectorHobData ();
  UsbCConnectorBoardConfig = UsbCConnectorHobDataPtr->UsbCConnectorBoardConfig;

  if (RetimerFlashConfig != NULL && RetimerCfgTable != NULL) {
    //
    // Generate the RetimerCfgTable from UsbCConnectorBoardConfig and RetimerFlashConfig.
    //
    for (Index = 0; Index < RetimerCount; Index++) {
      //
      // FirstUsbCConnectorIndex in RetimerFlashConfig is the index number of UsbCConnectorBoardConfig.
      //
      UsbCConnectorIndex = (RetimerFlashConfig[Index].FirstUsbCConnectorIndex);

      //
      // Check if iTBT controller supported or not. Assign iTBT Retimer configuration.
      //
      if (UsbCConnectorBoardConfig[UsbCConnectorIndex].TbtControllerType == ITBT_DMA) {
        if (UsbCConnectorBoardConfig[UsbCConnectorIndex].TbtControllerNum == TBT_CONTROLLER1) {
          // Assign RETIMER_CONFIG information under TBT controller 1.
          RetimerCfgTable->RetimerConfig[Index].RetimerDevAddress.Function = HOST_DMA0_FUN_NUM;
        } else if (UsbCConnectorBoardConfig[UsbCConnectorIndex].TbtControllerNum == TBT_CONTROLLER2) {
          // Assign RETIMER_CONFIG information under TBT controller 2.
          RetimerCfgTable->RetimerConfig[Index].RetimerDevAddress.Function = HOST_DMA1_FUN_NUM;
        }
        // Assign others RETIMER_CONFIG information.
        RetimerCfgTable->RetimerConfig[Index].FirmwareType = (UINT8) ((UsbCConnectorBoardConfig[UsbCConnectorIndex].TbtControllerType) - 1);
        RetimerCfgTable->RetimerConfig[Index].PcieRpType = 0;
        RetimerCfgTable->RetimerConfig[Index].PcieRootPort = 0;
        RetimerCfgTable->RetimerConfig[Index].RetimerDevAddress.Bus = HOST_DMA_BUS_NUM;
        RetimerCfgTable->RetimerConfig[Index].RetimerDevAddress.Device = HOST_DMA_DEV_NUM;
        RetimerCfgTable->RetimerConfig[Index].RetimerDevAddress.Port = (UINT8) UsbCConnectorBoardConfig[UsbCConnectorIndex].DmaPortNum;
        RetimerCfgTable->RetimerConfig[Index].RetimerDevAddress.CascadedRetimerIndex = (UINT8) RetimerFlashConfig[Index].CascadedRetimerFlashIndex;
      } else {
        DEBUG ((DEBUG_ERROR, "SetRetimerCfgTablePCD(): Connector doesn't support TBT.\n"));
      }
    }
    DEBUG ((DEBUG_INFO, "SetRetimerCfgTablePCD(): RetimerCfgTable Ptr: 0x%p\n", RetimerCfgTable));
    Size = RetimerCount * sizeof (RETIMER_CONFIG);
    Status = PcdSetPtrS (PcdUsbCRetimerConfigTable, &Size, RetimerCfgTable);
    ASSERT_EFI_ERROR (Status);
  } else {
    DEBUG ((DEBUG_ERROR, "SetRetimerCfgTablePCD(): RetimerFlashConfig is NULL\n"));
  }

  DEBUG ((DEBUG_INFO, "SetRetimerCfgTablePCD() End\n"));
}

/**
  Setting PcdUcmSelection

  @param[in]  UcsiVersion   UCSI Version

**/
VOID
UpdatePcdUcmSelection (
  IN  UINT16  UcsiVersion
  )
{
  switch (UcsiVersion) {
    case UCSI_VER_1_2:
      PcdSet8S (PcdUcmSelection, UCM_UCSI_1_2);
      break;
    case UCSI_VER_2_0:
      PcdSet8S (PcdUcmSelection, UCM_UCSI_2_0);
      break;
    case UCSI_VER_2_1:
      PcdSet8S (PcdUcmSelection, UCM_UCSI_2_1);
      break;
    default:
      PcdSet8S (PcdUcmSelection, UCXX_DISABLE);
      break;
  }
}

/**
  Setting PcdUcsiRevision

**/
VOID
SetUcsiRevisionPcd (
  VOID
  )
{
  UINT16  Version;

  DEBUG ((DEBUG_INFO, "SetUcsiRevisionPcd - Start\n"));

  switch (PcdGet8 (PcdUcmSelection)) {
    case UCM_UCSI_AUTO:
      if (PcdGetBool (PcdEcPresent) == TRUE) {
        Version = GetUcsiRev ();
      } else {
        Version = UCXX_DISABLE;
      }
      UpdatePcdUcmSelection (Version);
      break;
    default:
      Version = 0;
      break;
  }

  PcdSet16S (PcdUcsiRevision, Version);

  DEBUG ((DEBUG_INFO, "SetUcsiRevisionPcd - End\n"));
}

VOID
EFIAPI
CnvPreMemPcdInitBeforeSiliconInit (
  VOID
  )
{
  UINTN                 DataSize;
  CNV_BOARD_CONFIG_PCD  *CnvBoardConfigTable;

  DEBUG ((DEBUG_INFO, "CnvPreMemPcdInitBeforeSiliconInit\n"));
  CnvBoardConfigTable = (CNV_BOARD_CONFIG_PCD *) PcdGetPtr (VpdPcdCnvBoardConfig);
  DataSize = sizeof (CNV_BOARD_CONFIG);
  PcdSetPtrS (PcdCnvBoardConfig, &DataSize, CnvBoardConfigTable);
}

/**
  Returns whether the platform is in a debug state, by checking if any of the
  following conditions is true:

    - Using a sample part
    - Feature Control MSR lock is not set
    - Debug interface is enabled
    - SOC Configuration lock is not set (manufacturing mode)

  @retval  TRUE    Platform is in a debug state.
  @retval  FALSE   Platform is NOT in a debug state.
**/
BOOLEAN
PlatformDebugStateChecks (
  VOID
  )
{
  MSR_IA32_FEATURE_CONTROL_REGISTER  Ia32FeatureControlMsr;
  MSR_IA32_DEBUG_INTERFACE_REGISTER  DebugInterfaceMsr;
  HECI_FW_STS6_REGISTER              MeFirmwareStatus;

  BOOLEAN DebugState = FALSE;

  DEBUG ((DEBUG_INFO, "PlatformDebugStateChecks\n"));

  //
  // Check for sample part
  //
  if (IsSamplePart () == TRUE) {
    DEBUG ((DEBUG_WARN, "  Sample part detected!\n"));
    DebugState = TRUE;
  }

  //
  // Check for MSR Feature Control lock
  //
  Ia32FeatureControlMsr.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);
  DEBUG ((DEBUG_INFO, "  Ia32FeatureControlMsr.Uint64 = 0x%x\n", Ia32FeatureControlMsr.Uint64));
  if (Ia32FeatureControlMsr.Bits.Lock == 0) {
    DEBUG ((DEBUG_WARN, "  MSR Feature Control MSR lock is not set!\n"));
    DebugState = TRUE;
  }

  //
  // Check for Debug interface
  //
  DebugInterfaceMsr.Uint64 = AsmReadMsr64 (MSR_IA32_DEBUG_INTERFACE);
  DEBUG ((DEBUG_INFO, "  DebugInterfaceMsr.Uint64 = 0x%x\n", DebugInterfaceMsr.Uint64));
  if (DebugInterfaceMsr.Bits.Enable) {
    DEBUG ((DEBUG_WARN, "  Debug interface is enabled!\n"));
    DebugState = TRUE;
  }

  //
  // Check for Manufacturing Mode
  //
  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS_6);
  DEBUG ((DEBUG_INFO, "  MeFirmwareStatus.ul = 0x%x\n", MeFirmwareStatus.ul));
  if (MeFirmwareStatus.r.FpfSocConfigLock == 0) {
    DEBUG ((DEBUG_WARN, "  SOC Configuration Lock is not set!\n"));
    DebugState = TRUE;
  }

  return DebugState;
}

/**
   This callback function checks the debug state of the platform and
   sets the PcdFirmwareDebuggerInitialized value accordingly.

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
PlatformDebugStateChecksCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  //
  // Check Platform debug state
  //
  if (PlatformDebugStateChecks () == TRUE) {
    //
    // Extend platform state into TPM PCR[7]
    //
    PcdSetBoolS (PcdFirmwareDebuggerInitialized, TRUE);
  }
  DEBUG ((DEBUG_INFO, "PcdFirmwareDebuggerInitialized = %x\n", PcdGetBool (PcdFirmwareDebuggerInitialized)));
  return EFI_SUCCESS;
}

/**
  Register a notify event to check debug state and update
  PcdFirmwareDebuggerInitialized.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
PlatformDebugStatePcdInit (
  VOID
  )
{
  EFI_STATUS                        Status;

  Status = PeiServicesNotifyPpi (&mPlatformDebugStateChecksNotifyList);
  return Status;
}
