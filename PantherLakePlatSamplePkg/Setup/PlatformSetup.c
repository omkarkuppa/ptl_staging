/** @file
  Platform Setup Routines

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#include "SetupPrivate.h"
#include <Library/PciSegmentLib.h>
#include <Guid/GlobalVariable.h>
#include <Protocol/Wdt.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/HiiString.h>
#include <Txt.h>
#include <TxtInfoHob.h>
#include <IndustryStandard/SmBios.h>
#include <IndustryStandard/FirmwareVersionInfo.h>
#include <Protocol/Smbios.h>
#include <PlatformBoardId.h>
#include <Protocol/I2cIo.h>
#include <Library/DxeFirmwareVersionInfoLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TbtNvmRetimerUpdateLib.h>
#include <Library/DebugLib.h>
#include <Protocol/UsbIo.h>
#include <IndustryStandard/Usb.h>
#include <Library/GbeLib.h>
#include <Library/PeiDxeBoardIdsLib.h>
#include <BootStateLib.h>
#include "MeSetup.h"
#include <Library/DxeMeLib.h>
#include <Register/PchRegs.h>
#include <Uefi/UefiSpec.h>
#include <Protocol/PlatformSpecificResetFilter.h>
#include <Library/UefiRuntimeLib.h>
#include <PchResetPlatformSpecific.h>
#include <Library/EcMiscLib.h>
#include <Library/PchPciBdfLib.h>
#include <UsbCRetimerSetup.h>
#include <UsbCPdSetup.h>
#include <Library/PmcLib.h>

extern BOOLEAN   mMeReset;

//
// BCD to Decimal Conversion
//
#define BCD_TO_DECIMAL(A)           (((((A) >> 4) & 0xF) * 10) + ((A) & 0xF))

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HII_HANDLE            gPsHiiHandle = INVALID_HII_HANDLE;

//
// LAN PHY Revision definitions
//
#define LAN_PHY_REV_TC 0xA0
#define LAN_PHY_REV_A0 0xA1
#define LAN_PHY_REV_A1 0xA2
#define LAN_PHY_REV_A2 0xA3
#define LAN_PHY_REV_A3 0xA4
#define LAN_PHY_REV_B1 0xA5
#define LAN_PHY_REV_B2 0xA6

typedef struct _LAN_PHY_INFO {
  UINT16 LanPhyRev;
  char *LanPhyString;
} LAN_PHY_INFO;

#pragma pack(push, 1)
typedef struct {
  UINT8       ReportId;
  struct
  {
    UINT16  Width;
    UINT16  Height;
  } Dimensions;
  UINT8       BitDepth;
  UINT8       Orientation : 2;
  UINT8       SupportsCS : 1;
  UINT8       : 5; // padding
  UINT8       NumberOfBuffers;
  UINT8       NumberOfButtons;
  UINT8       NumberOfFingers;
  UINT8       NumberOfFrontLights;
  UINT32      FirmwareVersion;
  UINT32      PipSizeInBytes;
} HID_FEATURE_REPORT_DISPLAY_ATTRIBUTES;
#pragma pack(pop)

LAN_PHY_INFO LanPhyInfoTable[] = {
  {LAN_PHY_REV_TC, "(TC Stepping)"},
  {LAN_PHY_REV_A0, "(A0 Stepping)"},
  {LAN_PHY_REV_A1, "(A1 Stepping)"},
  {LAN_PHY_REV_A2, "(A2 Stepping)"},
  {LAN_PHY_REV_A3, "(A3 Stepping)"},
  {LAN_PHY_REV_B1, "(B1 Stepping)"},
  {LAN_PHY_REV_B2, "(B2 Stepping)"}
};

/**
  Determine if TXT supported on a platform based on CPU and PCH TXT capability

  @retval True          if TXT  supported on the platform

**/

BOOLEAN
IsTxtSupportedOnPlatform(
  VOID
  )
{
  TXT_INFO_HOB                  *HobList;
  TXT_INFO_DATA                 *TxtInfoData;

  HobList = NULL;
  TxtInfoData = NULL;
  ///
  /// Find TXT HOB
  ///
  HobList = (TXT_INFO_HOB *) GetFirstGuidHob (&gTxtInfoHobGuid);
  if (HobList == NULL) {
    return FALSE;
  }
  TxtInfoData = &HobList->Data;
  if (TxtInfoData == NULL) {
    return FALSE;
  }
  if (TxtInfoData->ProcessorIsTxtCapable && TxtInfoData->ChipsetIsTxtCapable)
    return TRUE;
  else
    return FALSE;
}

VOID
EFIAPI
CallbackGetLanPhyRevision (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  );

/**
  Acquire the string associated with the Index from smbios structure and return it.
  The caller is responsible for free the string buffer.

  @param    OptionalStrStart  The start position to search the string
  @param    Index             The index of the string to extract
  @param    String            The string that is extracted

  @retval   EFI_SUCCESS       The function returns EFI_SUCCESS always.
**/
EFI_STATUS
GetSmBiosStringByIndex (
  IN   CHAR8                   *OptionalStrStart,
  IN   UINT8                   Index,
  OUT  CHAR8                   **String
  )
{
  UINTN          StrSize;

  if (Index == 0) {
    *String = AllocateZeroPool (sizeof (CHAR8));
    return EFI_SUCCESS;
  }

  StrSize = 0;
  do {
    Index--;
    OptionalStrStart += StrSize;
    StrSize           = AsciiStrSize (OptionalStrStart);
  } while (OptionalStrStart[StrSize] != 0 && Index != 0);

  *String = AllocatePool (StrSize);
  ASSERT (*String != NULL);
  if (*String != NULL) {
    CopyMem (*String, OptionalStrStart, StrSize);
  }
  return EFI_SUCCESS;
}

/**
  Platform Setup Callback

  @param[in] Event         - Event based on EFI_SETUP_ENTER_GUID.
  @param[in] Context       - Pointer to the notification function's context.

**/
VOID
EFIAPI
PlatformSetupCallback (
  IN EFI_EVENT Event,
  IN VOID *Context
  )
{
  EFI_STATUS                      Status;
  UINT8                           StrIndex;
  CHAR8                           *NewString;
  BOOLEAN                         BoardInfo;
  BOOLEAN                         BiosInfo;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  EFI_SMBIOS_PROTOCOL             *Smbios;
  SMBIOS_TABLE_TYPE2              *Type2Record;
  INTEL_FIRMWARE_VERSION_INFO     *FviItem;
  UINTN                           FviCount;
  EFI_SMBIOS_TABLE_HEADER         *Record;
  UINTN                           Index;
  EFI_STRING                      BoardNameString;
  SETUP_DATA                      SetupData;
  UINT32                          VarAttributes;
  UINTN                           VarSize;
  CHAR8                           *Buffer;

  gBS->CloseEvent (Event);
  BoardInfo = FALSE;
  BiosInfo = FALSE;

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  do {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (Record->Type == EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION) {
      Type2Record = (SMBIOS_TABLE_TYPE2 *) Record;
      //Update Board Name
      StrIndex = Type2Record->ProductName;
      GetSmBiosStringByIndex ((CHAR8*)((UINT8*)Type2Record + Type2Record->Hdr.Length), StrIndex, &NewString);
      InitString (gPsHiiHandle,STRING_TOKEN (STR_BOARD_NAME_VALUE),L"%a%",NewString);
      FreePool (NewString);

      BoardNameString = HiiGetString (gPsHiiHandle, STRING_TOKEN (STR_BOARD_NAME_VALUE), NULL);

      //Update Board ID
      if (BoardNameString != NULL) {
        for (Index = 0; Index < mSizeOfmBoardIndex; Index++) {
          if (StrCmp((CHAR16*)BoardNameString, mBoardIdIndex[Index]) == 0) {
            InitString (gPsHiiHandle,STRING_TOKEN (STR_BOARD_ID_VALUE),L"0x%X",Index);
            break;
          }
        }
      }

      //Update Fab ID
      StrIndex = Type2Record->Version;
      GetSmBiosStringByIndex ((CHAR8*)((UINT8*)Type2Record + Type2Record->Hdr.Length), StrIndex, &NewString);
      InitString (gPsHiiHandle,STRING_TOKEN (STR_FAB_ID_VALUE),L"%a%",NewString);
      FreePool (NewString);
      BoardInfo = TRUE;
    }
    if (Record->Type == INTEL_FVI_SMBIOS_TYPE) {
      FviCount = ((SMBIOS_TABLE_TYPE_OEM_INTEL_FVI *) Record)->Count;
      FviItem  = &(((SMBIOS_TABLE_TYPE_OEM_INTEL_FVI *) Record)->Fvi[0]);
      Buffer = (CHAR8 *) Record + Record->Length;
      for (Index = 0; Index < FviCount; Index++) {
        if (AsciiStrCmp ((CHAR8 *) Buffer, EC_FVI_STRING) == 0) {
          break;
        }
        if (FviItem[Index].ComponentName != 0) {
          Buffer += AsciiStrSize (Buffer);
        }
        if (FviItem[Index].VersionString != 0) {
          Buffer += AsciiStrSize (Buffer);
        }
      }
      if (Index == FviCount) {
        continue;
      }

      InitString (
        gPsHiiHandle,
        STRING_TOKEN (STR_CHIP_EC_REV_VALUE),
        L"%02d.%02d.%02d.%04d",
        FviItem[Index].Version.MajorVersion,
        FviItem[Index].Version.MinorVersion,
        FviItem[Index].Version.Revision,
        FviItem[Index].Version.BuildNumber
        );

      BiosInfo = TRUE;
    }
  } while (!(BoardInfo && BiosInfo));

  //Update eSPI Flash Sharing Mode Info
  switch (PcdGet8 (PcdEcEspiFlashSharingMode)) {
    case 1:
      InitString (gPsHiiHandle,STRING_TOKEN (STR_ESPI_SHARING_MODE_VALUE),L"%s",L"SAF");
      break;
    case 2:
      InitString (gPsHiiHandle,STRING_TOKEN (STR_ESPI_SHARING_MODE_VALUE),L"%s",L"MAF");
      break;
    case 0:
    default:
      InitString (gPsHiiHandle,STRING_TOKEN (STR_ESPI_SHARING_MODE_VALUE),L"%s",L"G3");
  }

  //Update eSPI PECI Mode Info
  VarSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  &VarAttributes,
                  &VarSize,
                  &SetupData
                  );

  if (!EFI_ERROR (Status)) {
    switch (SetupData.EcPeciMode) {
      case 1:
        InitString (gPsHiiHandle,STRING_TOKEN (STR_READ_PECI_MODE_VALUE),L"%s",L"PECI over eSPI mode");
        break;
      case 0:
      default:
        InitString (gPsHiiHandle,STRING_TOKEN (STR_READ_PECI_MODE_VALUE),L"%s",L"Legacy PECI mode");
    }
  }
}

VOID
UpdateSetUpVarRevisionString (
  EFI_HII_HANDLE HiiHandle
  )
{
  EFI_STATUS Status;
  SA_SETUP SaSetup;
  CPU_SETUP CpuSetup;
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  OC_SETUP  OcSetup;
#endif
  PCH_SETUP PchSetup;
  ME_SETUP MeSetup;
  SI_SETUP SiSetup;
  SETUP_DATA SetupData;
  UINTN VarSize;

  VarSize = sizeof (ME_SETUP);
  Status = gRT->GetVariable (
                  L"MeSetup",
                  &gMeSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &MeSetup
                  );

  if (!EFI_ERROR (Status)) {
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_ME_SETUP_REVISION_VALUE),
      L"%d",
      MeSetup.Revision
      );
  }

  VarSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &CpuSetup
                  );

  if (!EFI_ERROR (Status)) {
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_CPU_SETUP_REVISION_VALUE),
      L"%d",
      CpuSetup.Revision
      );
  }

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  VarSize = sizeof (OC_SETUP);
  Status = gRT->GetVariable (
                  L"OcSetup",
                  &gOcSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &OcSetup
                  );

  if (!EFI_ERROR (Status)) {
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_OC_SETUP_REVISION_VALUE),
      L"%d",
      OcSetup.Revision
      );
  }
#endif

  VarSize = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &SaSetup
                  );

  if (!EFI_ERROR (Status)) {
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_SA_SETUP_REVISION_VALUE),
      L"%d",
      SaSetup.Revision
      );
  }

  VarSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &PchSetup
                  );

  if (!EFI_ERROR (Status)) {
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_PCH_SETUP_REVISION_VALUE),
      L"%d",
      PchSetup.Revision
      );
  }

  VarSize = sizeof (SI_SETUP);
  Status = gRT->GetVariable (
                  L"SiSetup",
                  &gSiSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &SiSetup
                  );

  if (!EFI_ERROR (Status)) {
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_SI_SETUP_REVISION_VALUE),
      L"%d",
      SiSetup.Revision
      );
  }

  VarSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &SetupData
                  );

  if (!EFI_ERROR (Status)) {
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_SETUP_DATA_REVISION_VALUE),
      L"%d",
      SetupData.Revision
      );
  }
}

/**
  Update PD Information

  @param[in] HiiHandle    HiiHandle Handle to HII database

**/
VOID
UpdatePDInformation (
  IN EFI_HII_HANDLE         HiiHandle
  )
{
  EFI_STATUS            Status;
  UINT64                UsbCPdVersion;
  USBC_PD_SETUP         UsbCPdSetup;
  UINT32                VarAttributes;
  UINTN                 VarSize;
  UINT32                FwVersion;
  UINT32                SubFwVersion;
  UINT8                 PdNumber;
  UINT8                 Index;
  EFI_STRING_ID         TokenToUpdate;

  DEBUG ((DEBUG_INFO, "<UpdatePDInformation> - Start\n"));

  ZeroMem (&UsbCPdSetup, sizeof (USBC_PD_SETUP));
  VarSize = sizeof (USBC_PD_SETUP);
  Status = gRT->GetVariable (
                  L"UsbCPdSetup",
                  &gUsbCPdSetupGuid,
                  &VarAttributes,
                  &VarSize,
                  &UsbCPdSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UsbCPdSetup variable with return Status = (%r).\n", Status));
    goto Exit;
  }

  PdNumber = PcdGet8 (PcdUsbCPdNumber);
  if (PdNumber > MAX_PD_NUMBER) {
    DEBUG ((DEBUG_ERROR, "PcdUsbCPdNumber is invalid\n"));
    goto Exit;
  }

  for (Index = 0; Index < PdNumber; Index++) {
    switch (Index) {
      case 0:
        UsbCPdVersion = UsbCPdSetup.UsbCPd0Version;
        TokenToUpdate = STRING_TOKEN (STR_PD0_VERSION_VALUE);
        break;
      case 1:
        UsbCPdVersion = UsbCPdSetup.UsbCPd1Version;
        TokenToUpdate = STRING_TOKEN (STR_PD1_VERSION_VALUE);
        break;
      case 2:
        UsbCPdVersion = UsbCPdSetup.UsbCPd2Version;
        TokenToUpdate = STRING_TOKEN (STR_PD2_VERSION_VALUE);
        break;
    }
    //
    // if success then update string
    //
    if (UsbCPdVersion != 0) {
      FwVersion    = (UINT32) (UsbCPdVersion & 0xFFFFFFFF);
      SubFwVersion = (UINT32) ((UsbCPdVersion >> 32) & 0xFFFFFFFF);

      InitString (
        HiiHandle,
        TokenToUpdate,
        L"%08X.%08X",
        FwVersion,
        SubFwVersion
        );
    } else {
      DEBUG ((DEBUG_INFO, "[USBC] PD%d Version is not found.\n", Index));
    }
  }

Exit:
  DEBUG ((DEBUG_INFO, "<UpdatePDInformation> - End\n"));
}

/**
  Update Retimer Information

  @param[in] HiiHandle    HiiHandle Handle to HII database

**/
VOID
UpdateRetimerInformation (
  IN EFI_HII_HANDLE         HiiHandle
  )
{
  EFI_STATUS              Status;
  USBC_RETIMER_SETUP      UsbCRetimerSetup;
  UINTN                   VarSize;
  UINT32                  TempRetimerVersion;
  UINT8                   UsbCRetimerNumber;
  UINT8                   Index;
  UINT16                  InitStringRetimerValue;

  DEBUG ((DEBUG_INFO, "<UpdateRetimerInformation> - Start\n"));

  DEBUG ((DEBUG_INFO, "UpdateRetimerInformation - HiiHandle 0x%x\n", HiiHandle));
  ZeroMem (&UsbCRetimerSetup, sizeof (USBC_RETIMER_SETUP));
  VarSize = sizeof (USBC_RETIMER_SETUP);
  Status = gRT->GetVariable (
                  USBC_RETIMER_NAME,
                  &gUsbCRetimerSetupGuid,
                  NULL,
                  &VarSize,
                  &UsbCRetimerSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UsbCRetimer variable with return Status = (%r).\n", Status));
    return;
  }

  UsbCRetimerNumber = PcdGet8 (PcdUsbCRetimerFlashNumber);
  for (Index = 0; Index < UsbCRetimerNumber; Index++) {
    switch (Index) {
      case 0:
        InitStringRetimerValue = STR_RETIMER0_VERSION_VALUE;
        TempRetimerVersion     = UsbCRetimerSetup.UsbCRetimer0Version;
        break;
      case 1:
        InitStringRetimerValue = STR_RETIMER1_VERSION_VALUE;
        TempRetimerVersion     = UsbCRetimerSetup.UsbCRetimer1Version;
        break;
      case 2:
        InitStringRetimerValue = STR_RETIMER2_VERSION_VALUE;
        TempRetimerVersion     = UsbCRetimerSetup.UsbCRetimer2Version;
        break;
      default:
        DEBUG ((DEBUG_ERROR, "UsbCRetimerNumber is more than setting Index = %x.\n", Index));
        break;
    }
    DEBUG ((DEBUG_INFO, "UsbC Retimer%x version is 0x%x.\n", Index , TempRetimerVersion));
    if ((TempRetimerVersion == 0xFFFFFFFF) || (TempRetimerVersion == 0x0)) {
      DEBUG ((DEBUG_ERROR, "UsbC Retimer%x failed in GetUsbCRetimerVersion.\n", Index));
      continue;
    }
    //
    // Update Retimer information
    //
    InitString (
      HiiHandle,
      STRING_TOKEN (InitStringRetimerValue),
      L"%02X.%02X.%02X",
      ((TempRetimerVersion >> 16) & 0xFF),
      ((TempRetimerVersion >> 8) & 0x3F),
      (TempRetimerVersion & 0xFF)
      );
  }
  DEBUG ((DEBUG_INFO, "<UpdateRetimerInformation> - End\n"));
}

/**
  Initialize Platform Strings

  @param[in] HiiHandle    HiiHandle Handle to HII database
  @param[in] Class        Indicates the setup class

**/
VOID
InitPlatformStrings (
  EFI_HII_HANDLE HiiHandle,
  UINT16         Class
  )
{
  EFI_STATUS            Status;
  UINTN                 VariableSize;
  EFI_BOOT_MODE         BootMode;
  ME_INFO_SETUP_DATA    MeInfoSetupData;
  UINT8                 *FwMeSku[] = {
     (UINT8 *) ME_FW_IMAGE_TYPE_CONSUMER_STRING,
     (UINT8 *) ME_FW_IMAGE_TYPE_CORPORATE_STRING,
     (UINT8 *) ME_FW_IMAGE_TYPE_UNIDENTIFIED_STRING,
  };
  UINT8                 *MeFwSkuValue;
  EFI_EVENT             SetupEnterEvent;
  VOID                  *pSetupRegistration;
  EFI_EVENT             PlatformSetupEvent;
  VOID                  *Registration;
  BOOLEAN               IsTriggeredAtSetupMenu;
  FLASH_PARTITION_DATA  PartitionIdData;
  VOID                  *TempProtocolPtr=NULL;
  EFI_GUID              XmlCliCommonGuid = {
    0xbf030b10, 0x2d9b, 0x4e71, { 0xa0, 0xc4, 0xbc, 0x99, 0x10, 0x57, 0x9d, 0x40
  }};  // This GUID is similar to definition in XmlCliFeaturePkg
  CHAR8 QdfInfo[5];

  if (Class == MAIN_FORM_SET_CLASS) {

    DEBUG ((DEBUG_INFO, "<InitPlatformStrings>"));

    //
    // Preserve HiiHandle to access Strings in callbacks
    //
    if (gPsHiiHandle == INVALID_HII_HANDLE) {
      gPsHiiHandle = HiiHandle;
    }

    VariableSize = sizeof (ME_INFO_SETUP_DATA);
    Status = gRT->GetVariable (
                    L"MeInfoSetup",
                    &gMeInfoSetupGuid,
                    NULL,
                    &VariableSize,
                    &MeInfoSetupData
                    );
    if (!EFI_ERROR (Status)) {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_ME_FW_REV_VALUE),
        L"%d.%d.%d.%d",
        MeInfoSetupData.MeMajor,
        MeInfoSetupData.MeMinor,
        MeInfoSetupData.MeHotFix,
        MeInfoSetupData.MeBuildNo
        );
    }

    //
    // Check if InitPlatformStrings is triggered via mSetupEnterGuid
    //
    IsTriggeredAtSetupMenu = FALSE;
    VariableSize = sizeof(SETUP_DATA);
    Status = gRT->GetVariable (
                    L"Setup",
                    &gSetupVariableGuid,
                    NULL,
                    &VariableSize,
                    &mSetupData
                    );
    ASSERT_EFI_ERROR(Status);

    if (!EFI_ERROR (Status) && mSetupData.FastBoot) {
      BootMode = GetBootModeHob();
      if ((mSetupData.SkipHIIUpdate) &&
          ((BootMode == BOOT_WITH_MINIMAL_CONFIGURATION) ||
           (BootMode == BOOT_ON_S4_RESUME) ||
           (BootMode == BOOT_ASSUMING_NO_CONFIGURATION_CHANGES))) {
        IsTriggeredAtSetupMenu = TRUE;
      }
    }

    //
    // Call routine to calculate LAN PHY Revision when entering Setup UI
    //
    if (IsTriggeredAtSetupMenu == TRUE) {
      PlatformSetupCallback (NULL, NULL);
      CallbackGetLanPhyRevision (NULL, NULL);
    } else {
      Status = gBS->CreateEvent (
                      EVT_NOTIFY_SIGNAL,
                      TPL_CALLBACK,
                      PlatformSetupCallback,
                      NULL,
                      &PlatformSetupEvent
                      );
      ASSERT_EFI_ERROR (Status);

      Status = gBS->RegisterProtocolNotify (
                      &gSetupEnterGuid,
                      PlatformSetupEvent,
                      &Registration
                      );
      ASSERT_EFI_ERROR (Status);

      Status = gBS->CreateEvent (
                      EVT_NOTIFY_SIGNAL,
                      TPL_CALLBACK,
                      CallbackGetLanPhyRevision,
                      NULL,
                      &SetupEnterEvent
                      );
      ASSERT_EFI_ERROR(Status);

      Status = gBS->RegisterProtocolNotify (
                      &gSetupEnterGuid,
                      SetupEnterEvent,
                      &pSetupRegistration
                      );
      ASSERT_EFI_ERROR(Status);
    }

    if (mMeSetup.MeImageType == ME_IMAGE_CONSUMER_SKU_FW) {
      MeFwSkuValue = FwMeSku[0];
    } else if (mMeSetup.MeImageType == ME_IMAGE_CORPORATE_SKU_FW) {
      MeFwSkuValue = FwMeSku[1];
    } else  {
      MeFwSkuValue = FwMeSku[2];
    }
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_ME_FW_SKU_VALUE),
      L"%a",
      MeFwSkuValue
      );

    Status = HeciGetImageFwVersionMsg (FPT_PARTITION_NAME_EFWP, &PartitionIdData);
    if (!EFI_ERROR (Status)) {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_SSE_FW_REV_VALUE),
        L"%d.%d.%d.%d",
        PartitionIdData.Version.Major,
        PartitionIdData.Version.Minor,
        PartitionIdData.Version.Hotfix,
        PartitionIdData.Version.Build
        );
    }

    Status = HeciGetImageFwVersionMsg (FTP_PARTITION_NAME_PMCP, &PartitionIdData);
    if (!EFI_ERROR (Status)) {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_PMC_FW_REV_VALUE),
        L"%d.%d.%d.%d",
        PartitionIdData.Version.Major,
        PartitionIdData.Version.Minor,
        PartitionIdData.Version.Hotfix,
        PartitionIdData.Version.Build
        );
    }

    //
    // Platform/PCH TXT capability
    //
    if (IsTxtSupportedOnPlatform() == TRUE) {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_PLATFORM_TXT_VALUE),
        L"%a",
        "Supported"
        );
    } else {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_PLATFORM_TXT_VALUE),
        L"%a",
        "Unsupported"
        );
    }

    //
    // Production Type
    //
    if (MmioRead32(TXT_PUBLIC_BASE + 0x200) & BIT31) {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_PRODUCTION_TYPE_VALUE),
        L"%a",
        "Production"
        );
    } else {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_PRODUCTION_TYPE_VALUE),
        L"%a",
        "Pre-Production"
        );
    }

    Status = PmcReadQdf(QdfInfo);

    if ( !EFI_ERROR (Status) ) {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_QDF_INFO_VALUE),
        L"%a",
        QdfInfo
        );
    } else {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_QDF_INFO_VALUE),
        L"%a",
        "N/A"
        );
    }

    //
    // if XmlCli Support is present bios will update the BoardID, FabID, KSC version and LanPhyversion without entering into setup
    //
    Status = gBS->LocateProtocol(&XmlCliCommonGuid, NULL, (VOID **)&TempProtocolPtr);
    if ( EFI_ERROR (Status) ) { // XmlCliCommon not published.
      DEBUG ((EFI_D_INFO, "XML_CLI: XmlCli Support Status: Disabled. \n"));
    } else {
      DEBUG ((EFI_D_INFO, "XML_CLI: XmlCli Support Status: Enabled. \n"));
      PlatformSetupCallback (NULL, NULL);
      CallbackGetLanPhyRevision (NULL, NULL);
    }

  } else if (Class == ADVANCED_FORM_SET_CLASS) {
    DEBUG ((DEBUG_INFO, "<InitPlatformStrings> - Advanced Form\n"));
    if (PcdGetBool (PcdUsbCEcSupportPdInfoPresent)) {
      UpdatePDInformation (HiiHandle);
    }
    if (PcdGetBool (PcdUsbCEcSupportRetimerInfoPresent)) {
      UpdateRetimerInformation (HiiHandle);
    }
  }

  UpdateSetUpVarRevisionString (HiiHandle);
}

/**
  Retrieves LAN PHY Revision

  @param[in] Event         - Event based on EFI_SETUP_ENTER_GUID.
  @param[in] Context       - Pointer to the notification function's context.

**/
VOID
EFIAPI
CallbackGetLanPhyRevision (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
{
  EFI_STATUS      Status;
  UINT16          LanPhyRev;
  UINTN           Index;
  BOOLEAN         LanPhyRevisionUpdated;

  LanPhyRev = 0;
  LanPhyRevisionUpdated = FALSE;

  //
  // Get Phy Version
  //
  Status = GbeGetLanPhyRevision (GbePciCfgBase (), &LanPhyRev);
  if (Status == EFI_NOT_FOUND) {
    return;
  }
  //
  // Close event, so it will not be invoked again.
  //
  gBS->CloseEvent (Event);

  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // LAN PHY Revision
  //
  if (LanPhyRev) {
    for (Index = 0; Index < (sizeof (LanPhyInfoTable) / sizeof (LAN_PHY_INFO)); Index++) {
      if ((UINT8) LanPhyRev == LanPhyInfoTable[Index].LanPhyRev) {
        InitString (
          gPsHiiHandle,
          STRING_TOKEN (STR_LAN_PHY_REV_VALUE),
          L"%02x %a",
          (UINT8) LanPhyRev,
          LanPhyInfoTable[Index].LanPhyString
          );
        LanPhyRevisionUpdated = TRUE;
        break;
      }
    }

    if (LanPhyRevisionUpdated == FALSE) {
      InitString (
        gPsHiiHandle,
        STRING_TOKEN (STR_LAN_PHY_REV_VALUE),
        L"%02x",
        (UINT8) LanPhyRev
        );
    }
  }

  return;
}

/**
  Determines if SusPwrDnAck needs to be set.
  For IbexPeak Platform with EC support, SusPwrDnAck should be to set.

  @retval TRUE        if SusPwrDnAck needs to be set

**/
BOOLEAN
CheckIfSetSusPwrDnAck (
  VOID
  )
{
  EFI_STATUS          Status;
  UINTN               VariableSize;
  SETUP_VOLATILE_DATA SetupVolatileData;
  BOOLEAN             SusPwrDnAck;

  SusPwrDnAck  = FALSE;

  VariableSize = sizeof (SETUP_VOLATILE_DATA);
  Status = gRT->GetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SetupVolatileData
                  );
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    if ((SetupVolatileData.PlatformFlavor == FlavorMobile) ||
        (SetupVolatileData.PlatformFlavor == FlavorEmbedded)) {
      SusPwrDnAck = TRUE;
    }
  }

  return SusPwrDnAck;
}

/**
  Platform Reset function

  @param[in] PlatformResetType          Reset type aligned with EFI_RESET_TYPE

  @retval EFI_RESET_TYPE                final reset type aligned with EFI_RESET_TYPE

**/
EFI_RESET_TYPE
PlatformReset (
  IN  EFI_RESET_TYPE       PlatformResetType
  )
{
  EFI_STATUS               Status;
  UINTN                    VariableSize;

  //
  // Reset Flag
  //
  BOOLEAN                  GlobalResetRequired = FALSE;
  BOOLEAN                  PowerCycleResetRequired = FALSE;

  SETUP_DATA               CurrentData;
  SETUP_DATA               SetupData;
  SA_SETUP                 SaCurrentData;
  SA_SETUP                 SaSetup;
  CPU_SETUP                CpuCurrentData;
  CPU_SETUP                CpuSetup;
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  OC_SETUP                 OcCurrentData;
  OC_SETUP                 OcSetup;
#endif
  PCH_SETUP                PchCurrentData;
  PCH_SETUP                PchSetup;
  SI_SETUP                 SiSetup;
  SI_SETUP                 SiCurrentData;
  EFI_HANDLE               Handle;

  VariableSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SetupData
                  );
  //
  // ASSERT_EFI_ERROR (Status);
  //
  if (EFI_ERROR (Status)) {
    return PlatformResetType;
  }

  VariableSize = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SaSetup
                  );
  if (EFI_ERROR (Status)) {
    return PlatformResetType;
  }

  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &CpuSetup
                  );
  if (EFI_ERROR (Status)) {
    return PlatformResetType;
  }

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  VariableSize = sizeof (OC_SETUP);
  Status = gRT->GetVariable (
                  L"OcSetup",
                  &gOcSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &OcSetup
                  );
  if (EFI_ERROR (Status)) {
    return PlatformResetType;
  }
#endif

  VariableSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &PchSetup
                  );
  if (EFI_ERROR (Status)) {
    return PlatformResetType;
  }

  VariableSize = sizeof (SI_SETUP);
  Status = gRT->GetVariable(
                  L"SiSetup",
                  &gSiSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SiSetup
                  );
  if (EFI_ERROR(Status)) {
    return PlatformResetType;
  }

  VariableSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"ColdReset",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &CurrentData
                  );
  if (EFI_ERROR (Status)) {
    return PlatformResetType;
  }

  VariableSize = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaColdReset",
                  &gSaSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SaCurrentData
                  );
  if (EFI_ERROR (Status)) {
    return PlatformResetType;
  }

  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuColdReset",
                  &gCpuSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &CpuCurrentData
                  );
  if (EFI_ERROR (Status)) {
    return PlatformResetType;
  }

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  VariableSize = sizeof (OC_SETUP);
  Status = gRT->GetVariable (
                  L"OcColdReset",
                  &gOcSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &OcCurrentData
                  );
  if (EFI_ERROR (Status)) {
    return PlatformResetType;
  }
#endif

  VariableSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchColdReset",
                  &gPchSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &PchCurrentData
                  );
  if (EFI_ERROR (Status)) {
    return PlatformResetType;
  }

  VariableSize = sizeof (SI_SETUP);
  Status = gRT->GetVariable(
                  L"SiColdReset",
                  &gSiSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SiCurrentData
                  );
  if (EFI_ERROR(Status)) {
    return PlatformResetType;
  }

  //
  // Checks if silicon setup variables has been modified
  //
  if (CompareMem(&PchSetup, &PchCurrentData, sizeof(PchSetup)) != 0 ||
      CompareMem(&CpuSetup, &CpuCurrentData, sizeof(CpuSetup)) != 0 ||
#if FixedPcdGetBool(PcdOverclockEnable) == 1
      CompareMem(&OcSetup,  &OcCurrentData,  sizeof(OcSetup))  != 0 ||
#endif
      CompareMem(&SaSetup, &SaCurrentData, sizeof(SaSetup)) != 0 ||
      CompareMem(&SiSetup, &SiCurrentData, sizeof(SiSetup)) != 0
     ) {
    //
    // Install gPlatformConfigChangeProtocolGuid
    //
    Handle = NULL;
    gBS->InstallProtocolInterface(
      &Handle,
      &gPlatformConfigChangeProtocolGuid,
      EFI_NATIVE_INTERFACE,
      NULL
      );
  }

  //
  // Clear PdtExist non-volatile variable to update PDT, when MPdtSupport(SensorBomId) option change in BIOS setup menu.
  //
  if (SetupData.MPdtSupport != CurrentData.MPdtSupport) {
    gRT->SetVariable (
           L"PdtExist",
           &gPdtExistGuid,
           0,
           0,
           NULL
           );
  }

  //
  // Change boot mode for critical setup questions.
  //


  if ((SaSetup.EnableVtd != SaCurrentData.EnableVtd) ||
      (PchSetup.PchHdAudio != PchCurrentData.PchHdAudio) ||
      (CpuSetup.Txt != CpuCurrentData.Txt) ||
      (CpuSetup.DfdEnable != CpuCurrentData.DfdEnable) ||
      (SaSetup.IgdDvmt50PreAlloc != SaCurrentData.IgdDvmt50PreAlloc) ||
      (SaSetup.MaxTolud != SaCurrentData.MaxTolud) ||
      (SaSetup.InternalGraphics != SaCurrentData.InternalGraphics) ||
      (SaSetup.PrimaryDisplay != SaCurrentData.PrimaryDisplay) ||
      (SaSetup.SpdProfileSelected != SaCurrentData.SpdProfileSelected)
      ) {
    PowerCycleResetRequired = TRUE;
  }

  if (mMeReset) {
    GlobalResetRequired = TRUE;
  }

  if (SetupData.EnablePcieTunnelingOverUsb4 != CurrentData.EnablePcieTunnelingOverUsb4) {
    DEBUG ((DEBUG_INFO, "GlobalResetRequired = TRUE\n"));
    GlobalResetRequired = TRUE;
  }

  if ((CpuSetup.ConfigTdpLevel != CpuCurrentData.ConfigTdpLevel) ||
     (CpuSetup.ConfigTdpLock != CpuCurrentData.ConfigTdpLock) ||
     (CpuSetup.CustomTurboActivationRatio != CpuCurrentData.CustomTurboActivationRatio)
    ) {
    PowerCycleResetRequired = TRUE;
  }

  //
  // Doesn't need to do platform special reset
  //
  if (GlobalResetRequired) {
    return EfiResetPlatformSpecific;
  } else if (PowerCycleResetRequired) {
    return EfiResetCold;
  }

  return PlatformResetType;
}



VOID
InitBoardStrings (
  EFI_HII_HANDLE  HiiHandle,
  UINT16          Class
  )
{
  return;
}

/**
  ThermalFunctionCallback() is called when we enter Thermal Configuration Form
  When "Enable All Thermal Functions" setup option value is changed then it updated
  as below:
   "Enable All Thermal Functions" is enabled it enables:
     1. Active Trip Points
     2. Critical Trip Points
   "Enable All Thermal Functions" option is disabled then "Memory Thermal Management",
   Active ,Critical Trip Points option is disabled
**/

EFI_STATUS
EFIAPI
ThermalFunctionCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  SETUP_DATA  SetupData;
  UINTN       VarSize;
  EFI_STATUS  Status;
  EFI_STRING  RequestString;

  RequestString = NULL;

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  VarSize = sizeof(SETUP_DATA);
  Status = HiiGetBrowserData(&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) &SetupData);
  ASSERT_EFI_ERROR(Status);

  //
  //   "Enable All Thermal Functions" option is enabled  then  "Active Trip Points", "Critical Trip Points"  option is enabled
  //   "Enable All Thermal Functions" option is disabled then  "Active Trip Points", "Critical Trip Points" option is disabled
  //
  if (!EFI_ERROR (Status)) {
    if(!(SetupData.DisableActiveTripPoints == Value->u8) ) {
      SetupData.DisableActiveTripPoints = Value->u8;
      RequestString = NULL;
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DisableActiveTripPoints),sizeof(SetupData.DisableActiveTripPoints));

      if (RequestString != NULL) {
        Status = HiiSetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) &SetupData, RequestString);
        ASSERT_EFI_ERROR (Status);
        FreePool (RequestString);
      }
    }

    if(!(SetupData.DisableCriticalTripPoints == Value->u8) ) {
      SetupData.DisableCriticalTripPoints = Value->u8;
      RequestString = NULL;
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, DisableCriticalTripPoints),sizeof(SetupData.DisableCriticalTripPoints));
    }
    if (RequestString != NULL) {
      Status = HiiSetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) &SetupData, RequestString);
      ASSERT_EFI_ERROR (Status);
      FreePool (RequestString);
    }
  }
  return Status;
}



/**
  setup reset call back function

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
PlatformSetupResetCallback (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  static BOOLEAN              IsExecuted;
  EFI_RESET_TYPE              OrgResetType;
  PCH_RESET_DATA              NewResetData;


  if (IsExecuted) return;
  IsExecuted = TRUE;

  DEBUG ((DEBUG_INFO,"PlatformSetupResetCallback() Executed\n"));


  OrgResetType = ResetType;
  ResetType = PlatformReset (ResetType);
  if(OrgResetType == ResetType) return;

  DEBUG((DEBUG_INFO,"new ResetType is %d\n",ResetType));

  if(ResetType != EfiResetPlatformSpecific) {
    gRT->ResetSystem (ResetType, EFI_SUCCESS, 0, NULL);
  } else {
    DataSize = sizeof (PCH_RESET_DATA);
    CopyMem (&NewResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
    StrCpyS (NewResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
    gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, DataSize, &NewResetData);
  }
}

/**
  Hook the system reset to properly reset function for platform setup reset

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OnResetFilterInstall (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{
  EFI_STATUS                      Status;
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetNotify;

  Status = gBS->LocateProtocol (&gEdkiiPlatformSpecificResetFilterProtocolGuid, NULL, (VOID **) &ResetNotify);
  if (!EFI_ERROR (Status)) {
    Status = ResetNotify->RegisterResetNotify (ResetNotify, PlatformSetupResetCallback);
    if (Event) gBS->CloseEvent (Event);
  }

}

/**
  add a reset filter protocol call back for setup exit funciton

**/
VOID
InstallPlatfromReset (
  VOID
  )
{
  VOID   *Registration;

  EfiCreateProtocolNotifyEvent (
    &gEdkiiPlatformSpecificResetFilterProtocolGuid,
    TPL_CALLBACK,
    OnResetFilterInstall,
    NULL,
    &Registration
    );
}

/**
  Update the Rtd3 setting base on the board type.

  @param[IN]      UINT8                   Rtd3Support,
  @param[IN][OUT] SETUP_DATA              *SetupData

  @retval EFI_SUCCESS             Successfully completed updates.
  @retval Others                  Fail to update.
**/
EFI_STATUS
EFIAPI
UpdateRtd3Setting (
  IN      UINT8               Rtd3Support,
  IN OUT  SETUP_DATA          *SetupData
  )
{
  if (SetupData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Rtd3Support == 0) {
    if (SetupData->StorageRtd3Support == 2) {
      SetupData->StorageRtd3Support = 1;
    }
  }

  return EFI_SUCCESS;
}

/**
  Rtd3SupportCallBackFunction() is called when we want to control the Storage D3 control by "StorageRtd3Support" setup option.
  Please refer behavior as below.
  For Mobile platform:
    Option value + Option string + Option meaning
    | 0          | Disable       | D3 is disabled
    | 1          | D3Hot         | D3Hot setting
    | 2          | D3Cold        | D3Cold setting (hide if RTD3 disable)

  @param[IN]       EFI_FORM_CALLBACK_PROTOCOL   *This
  @param[IN]       EFI_BROWSER_ACTION           Action,
  @param[IN]       EFI_QUESTION_ID              KeyValue
  @param[IN]       UINT8                        Type,
  @param[IN]       EFI_IFR_TYPE_VALUE           *Value,
  @param[IN]       EFI_BROWSER_ACTION_REQUEST   *ActionRequest

  @retval EFI_SUCCESS            Successfully completed updates.
  @retval EFI_UNSUPPORTED        Unsupported browser action.
  @retval EFI_DEVICE_ERROR       The variable could not be read or saved.
**/
EFI_STATUS
EFIAPI
Rtd3SupportCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  SETUP_DATA            SetupData;
  UINTN                 VarSize;
  EFI_STRING            RequestString;

  DEBUG ((DEBUG_INFO, "%a() start\n", __FUNCTION__));

  if ((Action != EFI_BROWSER_ACTION_CHANGING) && (Action != EFI_BROWSER_ACTION_CHANGED)) {
    return EFI_UNSUPPORTED;
  }

  VarSize = sizeof (SETUP_DATA);
  if (!HiiGetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) &SetupData)) {
    DEBUG ((DEBUG_ERROR, "Fail to get Setup Browser Data!\n"));
    return EFI_DEVICE_ERROR;
  }

  if (Value->u8 == 0) {
    //
    // When RTD3 is disable, if STD3 is 2 (D3Cold), it would force be changed to 1 (D3Hot).
    //
    if (SetupData.StorageRtd3Support == 2) {
      SetupData.StorageRtd3Support = 1;
    }
  }

  RequestString = NULL;
  RequestString = HiiConstructRequestString (
                    RequestString,
                    OFFSET_OF (SETUP_DATA, StorageRtd3Support),
                    sizeof (SetupData.StorageRtd3Support)
                    );
  if (RequestString != NULL) {
    VarSize = sizeof (SETUP_DATA);
    if (!HiiSetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) &SetupData,RequestString)) {
      DEBUG ((DEBUG_ERROR, "Fail to set STD3 value to browser!\n"));
      return EFI_DEVICE_ERROR;
    }
  }

  DEBUG ((DEBUG_INFO, "%a() end\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  DeepestUsbSleepWakeCapability CallBack function

  @param[IN]       EFI_FORM_CALLBACK_PROTOCOL   *This
  @param[IN]       EFI_BROWSER_ACTION           Action,
  @param[IN]       EFI_QUESTION_ID              KeyValue
  @param[IN]       UINT8                        Type,
  @param[IN]       EFI_IFR_TYPE_VALUE           *Value,
  @param[IN]       EFI_BROWSER_ACTION_REQUEST   *ActionRequest

  @retval EFI_SUCCESS            Successfully completed updates.
  @retval EFI_UNSUPPORTED        Unsupported browser action.
  @retval EFI_DEVICE_ERROR       The variable could not be read or saved.
**/
EFI_STATUS
EFIAPI
DeepestUsbSleepWakeCapabilityCallBack (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  SA_SETUP        SaSetup;
  SETUP_DATA      SetupData;
  UINTN           VarSize;
  EFI_STATUS      Status;
  EFI_STRING      RequestString;

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "DeepestUsbSleepWakeCapabilityCallBack Start\n"));

  VarSize = sizeof (SA_SETUP);
  if (!HiiGetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VarSize, (UINT8 *) &SaSetup)) {
    DEBUG ((DEBUG_ERROR, "The SaSetup variable could not be read\n"));
    return EFI_DEVICE_ERROR;
  }

  VarSize = sizeof (SETUP_DATA);
  if (!HiiGetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) &SetupData)) {
    DEBUG ((DEBUG_ERROR, "The Setup variable could not be read\n"));
    return EFI_DEVICE_ERROR;
  }

  RequestString = NULL;

  //
  // Wake Capability is S3
  //
  if (SetupData.DeepestUSBSleepWakeCapability == 3) {
    SaSetup.TcssVccstStatus = 0;
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, TcssVccstStatus), sizeof (SaSetup.TcssVccstStatus));
  }

  if (RequestString != NULL) {
    VarSize = sizeof (SA_SETUP);
    if (!HiiSetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VarSize, (UINT8 *) &SaSetup, RequestString)) {
      DEBUG ((DEBUG_ERROR, "The SaSetup variable could not be saved\n"));
      Status = EFI_DEVICE_ERROR;
    }
    ASSERT_EFI_ERROR (Status);
    FreePool (RequestString);
  }

  DEBUG ((DEBUG_INFO, "DeepestUsbSleepWakeCapabilityCallBack End\n"));

  return Status;
}
