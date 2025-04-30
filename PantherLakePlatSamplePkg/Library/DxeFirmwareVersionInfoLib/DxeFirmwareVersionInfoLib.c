/** @file
  This file implements firmware version information.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/EventGroup.h>

#include <Protocol/Smbios.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/GopComponentName2.h>
#include <Protocol/ResetNotification.h>

#include <PlatformInfo.h>
#include <Library/PciSegmentLib.h>
#include <Library/SiFviLib.h>
#include <PlatformBoardConfig.h>
#include <Library/BoardConfigLib.h>
#include <Library/DxeFirmwareVersionInfoLib.h>
#include <Library/GbeLib.h>
#include <Library/PchPciBdfLib.h>
#include <Register/PchRegs.h>
#include <Library/EcTcssLib.h>
#include <Library/EcMiscLib.h>
#include <Library/PmcLib.h>
#include <Library/HobLib.h>
#include <EcCommon.h>
#include <PlatformBoardId.h>
#include <IndustryStandard/Pci30.h>
#include <IndustryStandard/FirmwareVersionInfo.h>
#include <Library/TimerLib.h>
#include <Protocol/UsbCRetimerProtocol.h>
#include <Protocol/UsbCProgressCodeProtocol.h>
#include <Library/TbtNvmRetimerUpdateLib.h>
#include <SetupVariable.h>
#include <UsbCRetimerSetup.h>
#include <UsbCPdSetup.h>

GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8 StrEnabled[] = PLATFORM_FVI_STATUS_ENABLED;

GLOBAL_REMOVE_IF_UNREFERENCED FVI_DATA mPlatformFviData[] = {
  FVI_VERSION (LAN_PHY_VERSION),
  FVI_VERSION (SENSOR_FW_VERSION),
  FVI_VERSION (DEBUG_MODE_VERSION),
  FVI_VERSION (PERFORMANCE_MODE_VERSION),
  FVI_VERSION (DEBUG_USE_USB_VERSION),
  FVI_VERSION (ICC_OVERLOCKING_VERSION),
  FVI_VERSION (UNDI_VERSION),
  FVI_VERSION (EC_VERSION),
  FVI_VERSION (GOP_VERSION),
  FVI_VERSION (ROYALPARK_VERSION),
  FVI_VERSION (PLATFORM_VERSION),
  FVI_VERSION (TCP0_PD_VERSION),
  FVI_VERSION (TCP1_PD_VERSION),
  FVI_VERSION (TCP2_PD_VERSION),
  FVI_VERSION (TCP3_PD_VERSION),
  FVI_VERSION (USBC_RETIMER0_VERSION),
  FVI_VERSION (USBC_RETIMER1_VERSION),
  FVI_VERSION (USBC_RETIMER2_VERSION),
  FVI_VERSION (USBC_RETIMER3_VERSION),
};

GLOBAL_REMOVE_IF_UNREFERENCED FVI_HEADER mPlatformFviHeader = DEFAULT_FVI_HEADER_DATA(mPlatformFviData);

GLOBAL_REMOVE_IF_UNREFERENCED FVI_STRINGS mPlatformFviStrings[] = {
  { LAN_PHY_FVI_STRING,          NULL, },
  { SENSOR_FW_FVI_STRING,        NULL, },
  { DEBUG_MODE_STATUS,           PLATFORM_FVI_STATUS_DISABLED, },
  { PERFORMANCE_MODE_STATUS,     PLATFORM_FVI_STATUS_DISABLED, },
  { DEBUG_USE_USB_STATUS,        PLATFORM_FVI_STATUS_DISABLED, },
  { ICC_OVERLOCKING_FVI_STRING,  NULL, },
  { UNDI_FVI_STRING,             NULL, },
  { EC_FVI_STRING,               NULL, },
  { GOP_FVI_STRING,              NULL, },
  { ROYALPARK_FVI_STRING,        NULL, },
  { PLATFORM_FVI_STRING,         NULL, },
  { TCP0_PD_FVI_STRING,          TCP0_PD_DEFAULT_VERSION_STRING, },
  { TCP1_PD_FVI_STRING,          TCP1_PD_DEFAULT_VERSION_STRING, },
  { TCP2_PD_FVI_STRING,          TCP2_PD_DEFAULT_VERSION_STRING, },
  { TCP3_PD_FVI_STRING,          TCP3_PD_DEFAULT_VERSION_STRING, },
  { USBC_RETIMER0_FVI_STRING,    NULL, },
  { USBC_RETIMER1_FVI_STRING,    NULL, },
  { USBC_RETIMER2_FVI_STRING,    NULL, },
  { USBC_RETIMER3_FVI_STRING,    NULL, },
};

/**
  Read USBC PD Version
**/
VOID
ReadUsbCPdVersion(
  VOID
  )
{
  EFI_STATUS               Status;
  UINT8                    DataBuffer [8];
  SETUP_DATA               SetupData;
  USBC_PD_SETUP            UsbCPdSetup;
  UINT32                   VarAttributes;
  UINTN                    VarSize;
  UINT8                    Index;
  UINT64                   TempPdVersion;

  DEBUG ((DEBUG_INFO, "ReadUsbCPdVersion - Start\n"));

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
  } else {
    DEBUG ((DEBUG_INFO, "The UsbCPdSetup variable has been successfully initialized.\n"));
    goto Exit;
  }

  for (Index = 0; Index < FixedPcdGet8 (PcdMaxUsbCPdNumber); Index++) {
    ZeroMem (DataBuffer, sizeof (DataBuffer));
    Status = GetPDFwVersion (Index, DataBuffer);
    DEBUG ((DEBUG_INFO, "Get PD FW version on TCP%d with status:%r\n", Index, Status));
    if (Status == EFI_SUCCESS) {
      TempPdVersion = LShiftU64 (DataBuffer[7], 56) + LShiftU64 (DataBuffer[6], 48) \
                    + LShiftU64 (DataBuffer[5], 40) + LShiftU64 (DataBuffer[4], 32) \
                    + LShiftU64 (DataBuffer[3], 24) + LShiftU64 (DataBuffer[2], 16) \
                    + LShiftU64 (DataBuffer[1], 8) + DataBuffer[0];
    } else {
      TempPdVersion = 0;
    }

    switch (Index) {
      case 0:
        UsbCPdSetup.Tcp0PdVersion = TempPdVersion;
        break;
      case 1:
        UsbCPdSetup.Tcp1PdVersion = TempPdVersion;
        break;
#if FixedPcdGet8 (PcdMaxUsbCPdNumber) > 2
      case 2:
        UsbCPdSetup.Tcp2PdVersion = TempPdVersion;
        break;
#endif
#if FixedPcdGet8 (PcdMaxUsbCPdNumber) > 3
      case 3:
        UsbCPdSetup.Tcp3PdVersion = TempPdVersion;
        break;
#endif
    }
  }

  VarAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE;
  Status = gRT->SetVariable (
                  USBC_PD_NAME,
                  &gUsbCPdSetupGuid,
                  VarAttributes,
                  VarSize,
                  &UsbCPdSetup
                  );
  ASSERT_EFI_ERROR (Status);

  ZeroMem (&SetupData, sizeof (SETUP_DATA));
  VarSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  &VarAttributes,
                  &VarSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get setup variable\n"));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "ReadUsbCPdVersion - End\n"));
}

/**
  Update USBC PD Version
**/
VOID
UpdateUsbCPdVersion (
  VOID
  )
{
  EFI_STATUS     Status;
  UINT64         UsbCPdVersion;
  USBC_PD_SETUP  UsbCPdSetup;
  UINT32         VarAttributes;
  UINTN          VarSize;
  UINT32         FwVersion;
  UINT32         SubFwVersion;
  UINT8          Index;
  CHAR8          Tcp0PdVersionString[18];
  CHAR8          Tcp1PdVersionString[18];
#if FixedPcdGet8 (PcdMaxUsbCPdNumber) > 2
  CHAR8          Tcp2PdVersionString[18];
#endif
#if FixedPcdGet8 (PcdMaxUsbCPdNumber) > 3
  CHAR8          Tcp3PdVersionString[18];
#endif

  DEBUG ((DEBUG_INFO, "UpdateUsbCPdVersion - Start\n"));

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

  for (Index = 0; Index < FixedPcdGet8 (PcdMaxUsbCPdNumber); Index++) {
    switch (Index) {
      case 0:
        UsbCPdVersion = UsbCPdSetup.Tcp0PdVersion;
        if (UsbCPdVersion != 0) {
          FwVersion    = (UINT32) (UsbCPdVersion & 0xFFFFFFFF);
          SubFwVersion = (UINT32) ((UsbCPdVersion >> 32) & 0xFFFFFFFF);
          AsciiSPrint (Tcp0PdVersionString, sizeof (Tcp0PdVersionString), "%08x.%08x", FwVersion, SubFwVersion);
          CopyMem (mPlatformFviStrings[TCP0_PD_VER].VersionString, Tcp0PdVersionString, sizeof (Tcp0PdVersionString));
        }
        break;
      case 1:
        UsbCPdVersion = UsbCPdSetup.Tcp1PdVersion;
        if (UsbCPdVersion != 0) {
          FwVersion    = (UINT32) (UsbCPdVersion & 0xFFFFFFFF);
          SubFwVersion = (UINT32) ((UsbCPdVersion >> 32) & 0xFFFFFFFF);
          AsciiSPrint (Tcp1PdVersionString, sizeof (Tcp1PdVersionString), "%08x.%08x", FwVersion, SubFwVersion);
          CopyMem (mPlatformFviStrings[TCP1_PD_VER].VersionString, Tcp1PdVersionString, sizeof (Tcp1PdVersionString));
        }
        break;
#if FixedPcdGet8 (PcdMaxUsbCPdNumber) > 2
      case 2:
        UsbCPdVersion = UsbCPdSetup.Tcp2PdVersion;
        if (UsbCPdVersion != 0) {
          FwVersion    = (UINT32) (UsbCPdVersion & 0xFFFFFFFF);
          SubFwVersion = (UINT32) ((UsbCPdVersion >> 32) & 0xFFFFFFFF);
          AsciiSPrint (Tcp2PdVersionString, sizeof (Tcp2PdVersionString), "%08x.%08x", FwVersion, SubFwVersion);
          CopyMem (mPlatformFviStrings[TCP2_PD_VER].VersionString, Tcp2PdVersionString, sizeof (Tcp2PdVersionString));
        }
        break;
#endif
#if FixedPcdGet8 (PcdMaxUsbCPdNumber) > 3
      case 3:
        UsbCPdVersion = UsbCPdSetup.Tcp3PdVersion;
        if (UsbCPdVersion != 0) {
          FwVersion    = (UINT32) (UsbCPdVersion & 0xFFFFFFFF);
          SubFwVersion = (UINT32) ((UsbCPdVersion >> 32) & 0xFFFFFFFF);
          AsciiSPrint (Tcp3PdVersionString, sizeof (Tcp3PdVersionString), "%08x.%08x", FwVersion, SubFwVersion);
          CopyMem (mPlatformFviStrings[TCP3_PD_VER].VersionString, Tcp3PdVersionString, sizeof (Tcp3PdVersionString));
        }
#endif
        break;
    }
  }

Exit:
  DEBUG ((DEBUG_INFO, "UpdateUsbCPdVersion - End\n"));
}

/**
  Get EC Version
**/
VOID
GetEcVersion(
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_SMBIOS_HANDLE       SmbiosHandle;
  EFI_SMBIOS_PROTOCOL     *mSmbios;
  SMBIOS_TABLE_TYPE0      *Type0Record;
  EFI_SMBIOS_TABLE_HEADER *Record;

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **)&mSmbios
                  );
  if (!EFI_ERROR (Status)) {
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    do {
      Status = mSmbios->GetNext (mSmbios, &SmbiosHandle, NULL, &Record, NULL);
      if (EFI_ERROR(Status)) {
        break;
      }
      if (Record->Type == EFI_SMBIOS_TYPE_BIOS_INFORMATION) {
        Type0Record = (SMBIOS_TABLE_TYPE0 *) Record;
        mPlatformFviData[EC_VER].Version.MajorVersion = Type0Record->EmbeddedControllerFirmwareMajorRelease;
        mPlatformFviData[EC_VER].Version.MinorVersion = Type0Record->EmbeddedControllerFirmwareMinorRelease;
        mPlatformFviData[EC_VER].Version.Revision     = PcdGet8 (PcdEcPlatformMajorRevision);
        mPlatformFviData[EC_VER].Version.BuildNum     = PcdGet8 (PcdEcBuildRevision);
        break;
      }
    } while (Record->Type != EFI_SMBIOS_TYPE_BIOS_INFORMATION);
  }
}

/**
  Retrieves LAN PHY Revision by reading register at the first boot time
  after flashing FW image or updating capsule and set into NV variable.
  Then get the revision from NV varialbe during POST at the other time.
**/
VOID
GetLanPhyVersion(
  VOID
  )
{
  EFI_STATUS         Status;
  UINT16             LanPhyRev;
  PLATFORM_FVI_SETUP PlatformFviSetup = {0};
  UINTN              VariableSize;

  LanPhyRev = 0;

  VariableSize = sizeof (PLATFORM_FVI_SETUP);
  Status = gRT->GetVariable (
                  PLATFORM_FVI_SETUP_NAME,
                  &gPlatformFviSetupGuid,
                  NULL,
                  &VariableSize,
                  &PlatformFviSetup
                  );

  if (!EFI_ERROR(Status) && (PlatformFviSetup.LanPhyVersion != 0)) {
    //
    // The other time, to get Lan Phy revision from NV.
    //
     LanPhyRev = (UINT16) PlatformFviSetup.LanPhyVersion;
  } else if (Status == EFI_NOT_FOUND) {

    Status = GbeGetLanPhyRevision (GbePciCfgBase (), &LanPhyRev);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "GetLanPhyVersion failed to obtain Phy Revision. Status: %r\n", Status));
      LanPhyRev = 0;
    }

    PlatformFviSetup.LanPhyVersion = (UINT8) LanPhyRev;
    gRT->SetVariable (
           PLATFORM_FVI_SETUP_NAME,
           &gPlatformFviSetupGuid,
           EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
           sizeof(PLATFORM_FVI_SETUP),
           &PlatformFviSetup
           );
  }

  //
  // LAN PHY Revision
  //
  mPlatformFviData[LAN_PHY_VER].Version.MajorVersion = 0;
  mPlatformFviData[LAN_PHY_VER].Version.MinorVersion = 0;
  mPlatformFviData[LAN_PHY_VER].Version.BuildNum = LanPhyRev;
  mPlatformFviData[LAN_PHY_VER].Version.Revision = 0;

  return;
}


/**
  Update the Debug Mode Status, Performance Mode Status and Debug Use USB Status.

**/
VOID
UpdateDebugInformation(
  VOID
  )
{
  //
  // Default debug mode is disabled and update when MDEPKG_NDEBUG is False
  //
  DEBUG_CODE(mPlatformFviStrings[DEBUG_MODE_VER].VersionString = (CHAR8 *) &StrEnabled;);

  //
  // Default Performance mode is disabled and update it when PcdPerformanceEnable is TRUE.
  //
  if (FeaturePcdGet (PcdPerformanceEnable )) {
    mPlatformFviStrings[PERFORMANCE_MODE_VER].VersionString = (CHAR8 *) &StrEnabled;
  }

  //
  // Default Debug usage is Serial port and update it when PcdSourceDebugUseUsb is TRUE.
  //
#if FixedPcdGetBool(PcdSourceDebugUseUsb) == 1
  mPlatformFviStrings[DEBUB_USE_USB_VER].VersionString = (CHAR8 *) &StrEnabled;
#endif
}

/**
  Get UNDI version.

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
GetUndiVersion (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                  Status;
  UINTN                       Index;
  UINTN                       DriverImageHandleCount;
  EFI_HANDLE                  *DriverImageHandleBuffer;
  EFI_LOADED_IMAGE_PROTOCOL   *LoadedImage;
  EFI_GUID                    *NameGuid;
  EFI_DEVICE_PATH_PROTOCOL    *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    *LastDeviceNode;
  EFI_DRIVER_BINDING_PROTOCOL *DriverBinding;
  UINT32                      RetVal;
  VOID                        *ProtocolPointer;

  RetVal = (UINT32)-1;
  DriverImageHandleCount = 0;

  Status = gBS->LocateProtocol (&gEfiPxeBaseCodeProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  //
  // Close event, so it will not be invoked again.
  //
  gBS->CloseEvent (Event);

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiLoadedImageProtocolGuid,
                  NULL,
                  &DriverImageHandleCount,
                  &DriverImageHandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  for (Index = 0; Index < DriverImageHandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    DriverImageHandleBuffer[Index],
                    &gEfiLoadedImageProtocolGuid,
                    (VOID *) &LoadedImage
                    );
    if (LoadedImage->FilePath == NULL) {
      continue;
    }

    TempDevicePath = LoadedImage->FilePath;
    LastDeviceNode = TempDevicePath;
    while (!IsDevicePathEnd (TempDevicePath)) {
      LastDeviceNode = TempDevicePath;
      TempDevicePath = NextDevicePathNode (TempDevicePath);
    }
    NameGuid = EfiGetNameGuidFromFwVolDevicePathNode (
                 (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *) LastDeviceNode
                 );
    if ((NameGuid != NULL) && (CompareGuid (NameGuid, &gUndiDriverBinGuid))) {
      Status = gBS->HandleProtocol(
                      DriverImageHandleBuffer[Index],
                      &gEfiDriverBindingProtocolGuid,
                      (VOID*)&DriverBinding
                      );
      if (!EFI_ERROR(Status)) {
        RetVal = DriverBinding->Version;
        break;
      }
    }
  }
  gBS->FreePool (DriverImageHandleBuffer);
  mPlatformFviData[UNDI_VER].Version.MajorVersion = (RetVal >> 24) & 0x000000FF;
  mPlatformFviData[UNDI_VER].Version.MinorVersion = (RetVal >> 16) & 0x000000FF;
  mPlatformFviData[UNDI_VER].Version.Revision     = (RetVal >>  8) & 0x000000FF;
  mPlatformFviData[UNDI_VER].Version.BuildNum     = RetVal & 0x000000FF;
}


/**
  Get GOP Version - To get the GOP version depends on the
  GOP_COMPONENT_NAME2_PROTOCOL_GUID and transfer the Revision the CHAR16 to UINT.

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
GetGopVersion (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                    Status;
  UINT32                        Version[3] = {0};
  UINT32                        Index = 0;
  UINT32                        VersionIndex = 0;
  CHAR16                        TempStr;
  CHAR16                        *DriverVersion;
  GOP_COMPONENT_NAME2_PROTOCOL  *GopComponentName2Protocol = NULL;
  VOID                          *ProtocolPointer;

  Status = gBS->LocateProtocol (&gGopComponentName2ProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol (&gGopComponentName2ProtocolGuid, NULL, (VOID **) &GopComponentName2Protocol);
  if (!EFI_ERROR (Status)) {
    Status = GopComponentName2Protocol->GetDriverVersion (
                                          GopComponentName2Protocol,
                                          "en-US",
                                          &DriverVersion
                                          );
    if (!EFI_ERROR (Status)) {
      for (Index = 0; Index <= StrSize(DriverVersion); Index++) {
        TempStr = DriverVersion[Index];
        if (TempStr == '.') {
          VersionIndex++;
        } else if ((TempStr <= '9') && (TempStr >= '0')) {
          Version[VersionIndex] *= 0x10;
          Version[VersionIndex] += ((UINT32)(TempStr) - '0');
        }
      }

      mPlatformFviData[GOP_VER].Version.MajorVersion = 0;
      mPlatformFviData[GOP_VER].Version.MinorVersion = (UINT8) Version[0];
      mPlatformFviData[GOP_VER].Version.Revision     = (UINT8) Version[1];
      mPlatformFviData[GOP_VER].Version.BuildNum     = (UINT16)Version[2];
    }
  }
}

/**
  Get Platform Version
**/
VOID
GetPlatformVersion (
  VOID
  )
{
  mPlatformFviData[PLATFORM_VER].Version.MajorVersion = (UINT8)  PcdGet8 (PcdPlatformVersionMajor);
  mPlatformFviData[PLATFORM_VER].Version.MinorVersion = (UINT8)  PcdGet8 (PcdPlatformVersionMinor);
  mPlatformFviData[PLATFORM_VER].Version.Revision     = (UINT8)  PcdGet8 (PcdPlatformVersionRevision);
  mPlatformFviData[PLATFORM_VER].Version.BuildNum     = (UINT16) PcdGet8 (PcdPlatformVersionBuild);
}

/**
  Construct the Retimer instances through the Retimer Config Table and number.
  Also construct the Host Router instances for sending LSEN.

  @param[out] RetimerDeviceInstances        A pointer to the Retimer device instances base on RETIMER_DEV_INSTANCE.
  @param[out] RetimerDeviceInstancesCount   A pointer to count the number of Retimer device instances.
  @param[out] HrDeviceInstances             A pointer to the Host Router device instances base on HR_DEV_INSTANCE.
  @param[out] HrDeviceInstancesCount        A pointer to count the number of Host Router device instances.

  @retval EFI_SUCCESS             The retimer instances were constructed successfully through the retimer
                                  Config Table and number
  @retval EFI_ABORTED             The operation is aborted.  Additional details
                                  are provided in AbortReason.
  @retval EFI_UNSUPPORTED         RetimerConfigTable is NULL or UsbCRetimerNumber is 0.
**/
STATIC
EFI_STATUS
ConstructRetimerInstances (
  OUT RETIMER_DEV_INSTANCE  *RetimerDeviceInstances,
  OUT UINT32                *RetimerDeviceInstancesCount,
  OUT HR_DEV_INSTANCE       *HrDeviceInstances,
  OUT UINT32                *HrDeviceInstancesCount
  )
{
  EFI_STATUS            Status;
  RETIMER_CONFIG        *RetimerConfigTable;
  UINT8                 UsbCRetimerNumber;
  UINT8                 Index;
  RETIMER_DEV_INSTANCE  RetimerDevice;

  //
  // Get RetimerConfigTable address through PcdUsbCRetimerConfigTable and verify it.
  //
  RetimerConfigTable = (RETIMER_CONFIG *) PcdGetPtr (PcdUsbCRetimerConfigTable);
  if (RetimerConfigTable == NULL) {
    DEBUG ((DEBUG_ERROR, "RetimerConfigTable: retimer config table is empty.\n"));
    return EFI_UNSUPPORTED;
  }
  UsbCRetimerNumber = PcdGet8 (PcdUsbCRetimerFlashNumber);
  //
  // Check PcdUsbCRetimerFlashNumber value.
  //
  if (UsbCRetimerNumber == 0) {
    DEBUG ((DEBUG_ERROR, "WARNING!! The PcdUsbCRetimerFlashNumber value is zero.\n"));
    return EFI_UNSUPPORTED;
  }

  for (Index = 0; Index < UsbCRetimerNumber; Index++) {
    RetimerDevice = NULL;
    DEBUG ((DEBUG_INFO, "CreateRetimerDevInstance: Retimer Instance %x\n", Index));
    Status = CreateRetimerDevInstance (
               RetimerConfigTable[Index].FirmwareType,
               RetimerConfigTable[Index].PcieRpType,
               RetimerConfigTable[Index].PcieRootPort,
               &RetimerConfigTable[Index].RetimerDevAddress,
               &RetimerDevice
               );

    if (EFI_ERROR (Status) || (RetimerDevice == NULL)) {
      DEBUG ((DEBUG_ERROR, "CreateRetimerDevInstance failed (%r) at instance index %d\n", Status, Index));
      continue;
    }
    RetimerDeviceInstances[*RetimerDeviceInstancesCount] = RetimerDevice;
    ++(*RetimerDeviceInstancesCount);
  }

  //
  // Create Host Router device instances for sending OFFLINE_MODE(EXIT) to the specified HR and specified port
  //
  Status = CreateHrDevInstance (
             RetimerDeviceInstances,
             RetimerDeviceInstancesCount,
             HrDeviceInstances,
             HrDeviceInstancesCount
             );

  return Status;
}

/**
   The function update USBC Retimer information before system reset.

   @param[in] ResetType   - Type of reset to perform.
   @param[in] ResetStatus - Status code of the reset.
   @param[in] DataSize    - Size of ResetData, in bytes.
   @param[in] ResetData   - Optional Null-terminated string which can be used to introduce a platform specific reset.
**/
VOID
EFIAPI
UpdateUsbCRetimerInfoResetCallback (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID            *ResetData OPTIONAL
  )
{
  EFI_STATUS                         Status;
  USBC_RETIMER_PROTOCOL              *UsbCRetimerProtocol;
  UINT32                             RetimerVersion;
  UINT8                              Index;
  USBC_RETIMER_SETUP                 UsbCRetimerSetup;
  UINT32                             VarAttributes;
  UINTN                              VarSize;
  RETIMER_DEV_INSTANCE               RetimerDevice;
  RETIMER_DEV_INSTANCE               RetimerDeviceInstances[MAX_RETIMER_INSTANCES];
  UINT32                             RetimerDeviceInstancesCount;
  HR_DEV_INSTANCE                    HrDeviceInstances[MAX_RETIMER_INSTANCES];
  UINT32                             HrDeviceInstancesCount;
  UINTN                              WaitForRetimerReadyToUpdate;
  USBC_PROGRESS_CODE_PROTOCOL        *UsbCProgressCodeProtocol;

  DEBUG ((DEBUG_INFO, "UpdateUsbCRetimerInfoResetCallback - Start\n"));

  VarSize = sizeof (USBC_RETIMER_SETUP);
  Status = gRT->GetVariable (
                  USBC_RETIMER_NAME,
                  &gUsbCRetimerSetupGuid,
                  &VarAttributes,
                  &VarSize,
                  &UsbCRetimerSetup
                  );
  if (!EFI_ERROR (Status)) {
    //
    // The UsbCRetimer variable has been successfully initialized.
    //
    DEBUG ((DEBUG_INFO, "The UsbCRetimer variable has been successfully initialized.\n"));
  } else if (Status == EFI_BUFFER_TOO_SMALL) {
    DEBUG ((DEBUG_ERROR, "Failed to GetVariable, EFI_ERROR (%r)\n", Status));
    goto Exit;
  } else {
    RetimerDevice               = NULL;
    RetimerDeviceInstancesCount = 0;
    HrDeviceInstancesCount      = 0;
    ZeroMem (&UsbCRetimerSetup, sizeof (USBC_RETIMER_SETUP));

    //
    // Locate UsbC Capsule Debug Progress Code Protocol
    //
    Status = gBS->LocateProtocol (&gUsbCCapsuleDebugProgressCodeProtocolGuid, NULL, (VOID**) &UsbCProgressCodeProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Failed to locate UsbCProgressCodeProtocol (%r).\n", Status));
      UsbCProgressCodeProtocol->ShowProgressCode = UsbCCapsuleShowProgressCodeDefault;
    }
    UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_READ_RETIMER_VERSION_START);

    Status = ConstructRetimerInstances (
               RetimerDeviceInstances,
               &RetimerDeviceInstancesCount,
               HrDeviceInstances,
               &HrDeviceInstancesCount
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to ConstructRetimerInstances(%r).\n", Status));
      goto FreeInstances;
    }

    //
    // Send OFFLINE_MODE_ENTRY to the specified HR and specified port
    //
    UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_OFFLINE_MODE_ENTRY);
    for (Index = 0; Index < HrDeviceInstancesCount; Index++) {
      Status = TbtSendOfflineMode (HrDeviceInstances[Index], OFFLINE_MODE_ENTRY);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "TbtSendOfflineMode failed (%r) on HR instance index %d\n", Status, Index));
        goto OfflineModeExit;
      }
    }

    //
    // Drive PD into force power mode to make sure the retimer is ready
    // to communicate for get retimer version and save them to the variable.
    //
    // Enforce all TBT ports into TBT mode
    //
    Status = gBS->LocateProtocol (&gUsbCRetimerProtocolGuid, NULL, (VOID**) &UsbCRetimerProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to locate UsbCRetimerProtocol (%r).\n", Status));
      goto OfflineModeExit;
    }

    UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_PD_DRIVE);
    Status = DriveToFwUpdateMode (UsbCRetimerProtocol, gAllTbtRetimerDeviceGuid);
    WaitForRetimerReadyToUpdate = (UINTN) PcdGet32 (PcdTcssWaitRetimerDeviceReady);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UpdateUsbCRetimerInfoResetCallback - Failed to drive ports to TBT mode (%r).\n", Status));
      goto RestorePdPowerMode;
    } else {
      //
      // Delay for Retimer device ready.
      //
      DEBUG ((DEBUG_INFO, "Time delay for Retimer device ready = %d(MicroSecond)\n", WaitForRetimerReadyToUpdate));
      MicroSecondDelay (WaitForRetimerReadyToUpdate);
    }

    //
    // BIOS should assert retimer ForcePower GPIO after completing BIOS EC-PD command to avoid retimer entering sleep mode
    //
    UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_RETIMER_FP_GPIO_ASSERT);
    if (UsbCRetimerProtocol->RetimerFP != NULL) {
      DEBUG ((DEBUG_INFO, "Assert Retimer ForcePower GPIO\n"));
      Status = UsbCRetimerProtocol->RetimerFP (RETIMER_FORCE_POWER_GPIO_HIGH);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "UpdateUsbCRetimerInfoResetCallback - Failed to assert Retimer Force Power GPIO (%r).\n", Status));
        goto DeAssertRetimerForcePowerGpio;
      }
    }

    DEBUG ((DEBUG_INFO, "Read firmware version on the first system reboot.\n"));

    for (Index = 0; Index < RetimerDeviceInstancesCount; Index++) {
      RetimerVersion = 0;
      RetimerDevice  = RetimerDeviceInstances[Index];
      Status = InitRetimerHW (RetimerDevice);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "InitRetimerHW fail :%r\n", Status));
        continue;
      }

      if (RetimerDevice != NULL) {
        //
        // Read Retimer NVM FW version on given RetimerDevice
        //
        Status = ReadRetimerNvmVersion (RetimerDevice, &RetimerVersion);
        DEBUG ((DEBUG_INFO, "Retimer%x version after the update is %02x.%02x.%02x (%r).\n", \
          Index, \
          ((RetimerVersion >> 16) & 0xFF), \
          ((RetimerVersion >> 8) & 0x3F), \
          (RetimerVersion & 0xFF), \
          Status));
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "ReadRetimerNvmVersion fail :%r\n", Status));
        }
      }

      //
      // Update USBC Retimer version
      //
      switch (Index) {
        case 0:
          UsbCRetimerSetup.UsbCRetimer0Version = RetimerVersion;
          break;
        case 1:
          UsbCRetimerSetup.UsbCRetimer1Version = RetimerVersion;
          break;
        case 2:
          UsbCRetimerSetup.UsbCRetimer2Version = RetimerVersion;
          break;
        case 3:
          UsbCRetimerSetup.UsbCRetimer3Version = RetimerVersion;
          break;
        default:
          DEBUG ((DEBUG_ERROR, "UsbCRetimerNumber is more than setting Index = %x.\n", Index));
          break;
      }

      //
      // Send LSUP disable command (USUP) to the target retimer
      //
      Status = TerminateRetimerHW (RetimerDeviceInstances[Index]);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "TerminateRetimerHW fail on retimer I:%r\n", Status));
      }
    }

  DeAssertRetimerForcePowerGpio:
    //
    // De-assert Retimer ForcePower GPIO before sending restore command to EC
    //
    UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_RETIMER_FP_GPIO_DEASSERT);
    if (UsbCRetimerProtocol->RetimerFP != NULL) {
      DEBUG ((DEBUG_INFO, "De-assert Retimer ForcePower GPIO\n"));
      Status = UsbCRetimerProtocol->RetimerFP (RETIMER_FORCE_POWER_GPIO_LOW);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Failed to de-assert Retimer Force Power GPIO (%r).\n", Status));
      }
    }

  RestorePdPowerMode:
    //
    // Restore all TBT ports back to its original mode after communicating with Retimer.
    //
    UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_PD_RESTORE);
    Status = RestoreToOriginalMode (UsbCRetimerProtocol, gAllTbtRetimerDeviceGuid);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to restore to TBT ports to original mode (%r).\n", Status));
    }

  OfflineModeExit:
    //
    //  Send OFFLINE_MODE_EXIT to the specified HR and specified port
    //
    UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_OFFLINE_MODE_EXIT);
    for (Index = 0; Index < HrDeviceInstancesCount; Index++) {
      Status = TbtSendOfflineMode (HrDeviceInstances[Index], OFFLINE_MODE_EXIT);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "TbtSendOfflineMode failed (%r) on HR instance index %d\n", Status, Index));
      }
    }

  FreeInstances:
    //
    // Release Retimer device data.
    //
    for (Index = 0; Index < RetimerDeviceInstancesCount; Index++) {
      DEBUG ((DEBUG_INFO, "DestroyRetimerDevInstance: Retimer Instance %x\n", Index));
      DestroyRetimerDevInstance (RetimerDeviceInstances[Index]);
    }

    VarAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE;
    Status = gRT->SetVariable (
                    USBC_RETIMER_NAME,
                    &gUsbCRetimerSetupGuid,
                    VarAttributes,
                    VarSize,
                    &UsbCRetimerSetup
                    );
    ASSERT_EFI_ERROR (Status);
  }
Exit:
  DEBUG ((DEBUG_INFO, "UpdateUsbCRetimerInfoResetCallback - End\n"));
}

/**
  Hook to reset notification protocol to properly reset function for update USBC Retimer Information.

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
UsbCRetimerInfoResetNotify (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                            Status;
  USBC_RETIMER_SETUP                    UsbCRetimerSetup;
  SETUP_DATA                            SetupData;
  UINT32                                VarAttributes;
  UINTN                                 VarSize;
  EFI_RESET_NOTIFICATION_PROTOCOL       *ResetNotify;

  DEBUG ((DEBUG_INFO, "UsbCRetimerInfoResetNotify - Start.\n"));

  ZeroMem (&SetupData, sizeof (SETUP_DATA));
  VarSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  &VarAttributes,
                  &VarSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get setup variable\n"));
    goto Exit;
  }
  if (SetupData.UsbCRetimerNumber == 0) {
    //
    // Update UsbCRetimerNumber for show number of retimers.
    //
    SetupData.UsbCRetimerNumber = PcdGet8 (PcdUsbCRetimerFlashNumber);
    Status = gRT->SetVariable (
                    L"Setup",
                    &gSetupVariableGuid,
                    VarAttributes,
                    VarSize,
                    &SetupData
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to set setup variable\n"));
      goto Exit;
    }
  }
  //
  // Check whether UsbCRetimer variable is create.
  //
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
    DEBUG ((DEBUG_ERROR, "The variable of USBC Retimer with return Status = (%r).\n", Status));
    //
    // At first system reboot.
    // Register reset notify callback function for update USBC Retimer Information.
    //
    Status = gBS->LocateProtocol (
                    &gEdkiiPlatformSpecificResetFilterProtocolGuid,
                    NULL,
                    (VOID **) &ResetNotify
                    );
    if (!EFI_ERROR (Status)) {
      ResetNotify->RegisterResetNotify (ResetNotify, UpdateUsbCRetimerInfoResetCallback);
      DEBUG ((DEBUG_INFO, "UpdateUsbCRetimerInfoResetCallback() registered\n"));
    }
  } else {
    DEBUG ((DEBUG_INFO, "The UsbCRetimer variable has been successfully initialized.\n"));
  }
Exit:
  gBS->CloseEvent (Event);
  DEBUG ((DEBUG_INFO, "UsbCRetimerInfoResetNotify - End\n"));
  return;
}

/**
  Get USBC Retimer Version
**/
VOID
GetUsbCRetimerVersion (
  VOID
  )
{
  EFI_STATUS              Status;
  USBC_RETIMER_SETUP      UsbCRetimerSetup;
  UINTN                   VarSize;
  UINT8                   UsbCRetimerNumber;
  UINT8                   Index;
  UINT32                  TempRetimerVersion;

  DEBUG ((DEBUG_INFO, "GetUsbCRetimerVersion \n"));

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
        TempRetimerVersion = UsbCRetimerSetup.UsbCRetimer0Version;
        mPlatformFviData[USBC_RETIMER0_VER].Version.MajorVersion = (UINT8) (TempRetimerVersion >> 16 & 0xFF);
        mPlatformFviData[USBC_RETIMER0_VER].Version.MinorVersion = (UINT8) (TempRetimerVersion >> 8 & 0x3F);
        mPlatformFviData[USBC_RETIMER0_VER].Version.Revision     = (UINT8) (TempRetimerVersion & 0xFF);
        mPlatformFviData[USBC_RETIMER0_VER].Version.BuildNum     = 0;
        break;
      case 1:
        TempRetimerVersion = UsbCRetimerSetup.UsbCRetimer1Version;
        mPlatformFviData[USBC_RETIMER1_VER].Version.MajorVersion = (UINT8) (TempRetimerVersion >> 16 & 0xFF);
        mPlatformFviData[USBC_RETIMER1_VER].Version.MinorVersion = (UINT8) (TempRetimerVersion >> 8 & 0x3F);
        mPlatformFviData[USBC_RETIMER1_VER].Version.Revision     = (UINT8) (TempRetimerVersion & 0xFF);
        mPlatformFviData[USBC_RETIMER1_VER].Version.BuildNum     = 0;
        break;
      case 2:
        TempRetimerVersion = UsbCRetimerSetup.UsbCRetimer2Version;
        mPlatformFviData[USBC_RETIMER2_VER].Version.MajorVersion = (UINT8) (TempRetimerVersion >> 16 & 0xFF);
        mPlatformFviData[USBC_RETIMER2_VER].Version.MinorVersion = (UINT8) (TempRetimerVersion >> 8 & 0x3F);
        mPlatformFviData[USBC_RETIMER2_VER].Version.Revision     = (UINT8) (TempRetimerVersion & 0xFF);
        mPlatformFviData[USBC_RETIMER2_VER].Version.BuildNum     = 0;
        break;
      case 3:
        TempRetimerVersion = UsbCRetimerSetup.UsbCRetimer3Version;
        mPlatformFviData[USBC_RETIMER3_VER].Version.MajorVersion = (UINT8) (TempRetimerVersion >> 16 & 0xFF);
        mPlatformFviData[USBC_RETIMER3_VER].Version.MinorVersion = (UINT8) (TempRetimerVersion >> 8 & 0x3F);
        mPlatformFviData[USBC_RETIMER3_VER].Version.Revision     = (UINT8) (TempRetimerVersion & 0xFF);
        mPlatformFviData[USBC_RETIMER3_VER].Version.BuildNum     = 0;
        break;
      default:
        DEBUG ((DEBUG_ERROR, "UsbCRetimerNumber is more than setting Index = %x.\n", Index));
        break;
    }
  }
}

/**
  Create SMBIOS Table type

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
InitializeFviDataCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS Status;
  VOID       *ProtocolPointer;

  Status = gBS->LocateProtocol (&gBdsAllDriversConnectedProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  gBS->CloseEvent (Event);

  //
  // Initialize and report Platform FVI information to Smbios
  //
  mPlatformFviHeader.SmbiosHeader.Type = INTEL_FVI_SMBIOS_TYPE;

  GetEcVersion();
  if (IsGbePresent (PmcGetPwrmBase ())) {
    GetLanPhyVersion ();
  }
  UpdateDebugInformation ();

  GetPlatformVersion ();

  if (PcdGetBool (PcdUsbCEcSupportPdInfoPresent)) {
    ReadUsbCPdVersion ();
    UpdateUsbCPdVersion ();
  }
  if (PcdGetBool (PcdUsbCEcSupportRetimerInfoPresent)) {
    GetUsbCRetimerVersion ();
  }

  Status = AddFviEntry (mPlatformFviHeader, mPlatformFviData, mPlatformFviStrings);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AddFviEntry failed. Status = %r\n", Status));
  }
  return;
}


/**
  This is driver entry point to register the notification event.

  @param[in] ImageHandle  A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The initialization finished successfully.
**/
EFI_STATUS
EFIAPI
DxeFirmwareVersionInfoLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  VOID               *Registration1;
  VOID               *Registration2;
  VOID               *Registration3;
  VOID               *Registration4;

  //
  // Register InitializeFviDataCallback
  //
  EfiCreateProtocolNotifyEvent (
          &gBdsAllDriversConnectedProtocolGuid,
          TPL_NOTIFY, // Give other callback functions a chance to consume the FviData.
          InitializeFviDataCallback,
          NULL,
          &Registration1
          );

  //
  // Get the UNDI driver version.
  //
  EfiCreateProtocolNotifyEvent (
          &gEfiPxeBaseCodeProtocolGuid,
          TPL_CALLBACK,
          GetUndiVersion,
          NULL,
          &Registration2
          );

  //
  // Get the GOP driver version.
  //
  EfiCreateProtocolNotifyEvent (
          &gGopComponentName2ProtocolGuid,
          TPL_CALLBACK,
          GetGopVersion,
          NULL,
          &Registration3
          );

  if (PcdGetBool (PcdUsbCEcSupportRetimerInfoPresent) && PcdGet8 (PcdUsbCRetimerFlashNumber)) {
    //
    // Register Notify call back function.
    //
    EfiCreateProtocolNotifyEvent (
      &gEdkiiPlatformSpecificResetFilterProtocolGuid,
      TPL_NOTIFY,
      UsbCRetimerInfoResetNotify,
      NULL,
      &Registration4
      );
  }
  return EFI_SUCCESS;
}
