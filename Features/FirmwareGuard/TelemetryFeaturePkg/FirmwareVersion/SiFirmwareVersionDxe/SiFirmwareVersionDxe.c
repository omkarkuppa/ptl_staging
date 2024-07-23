/** @file
  Silicon related firmware version for telemetry driver

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TelemetryFviLib.h>
#include <Library/HobLib.h>
#include <Library/FspInfoLib.h>
#include <Protocol/Smbios.h>

#include <IndustryStandard/SmBios.h>
#include <IndustryStandard/FirmwareVersionInfo.h>
#include <SiFvi.h>
#include <Guid/ComponentId.h>
#include <TelemetryCommon.h>
#include "SiFirmwareVersionDxe.h"
#include <Library/UefiRuntimeServicesTableLib.h>

//
// @todo: Push below define/struct to be moved to SiFvi.h
//
#define  BIOS_GUARD_FVI_STRING       "BIOS Guard"

//
// The order of this array must match TELEMETRY_PCH_FVI_INDEX
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_6_5_PHAT_VERSION_ELEMENT mPchFviTelemetryData[] = {

  { PCH_COMPONENT_ID,                  DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { PCH_CRID_ORIGINAL_COMPONENT_ID,    DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { PCH_CRID_NEW_COMPONENT_ID,         DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { PCH_RAID_VERSION_COMPONENT_ID,     DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
};

//
// The order of this array must match TELEMETRY_SA_FVI_INDEX
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_6_5_PHAT_VERSION_ELEMENT mSaFviTelemetryData[] = {

  { SA_MEM_RC_VERSION_COMPONENT_ID,    DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { SA_CRID_ORIGINAL_COMPONENT_ID,     DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { SA_CRID_NEW_COMPONENT_ID,          DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { SA_VBIOS_VERSION_COMPONENT_ID,     DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { SA_IOM_COMPONENT_ID,               DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { SA_PHY_VERSION_COMPONENT_ID,       DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { SA_TBT_VERSION_COMPONENT_ID,       DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { SA_SAM_FW_VERSION_COMPONENT_ID,    DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
};

//
// The order of this array must match TELEMETRY_CPU_FVI_INDEX
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_6_5_PHAT_VERSION_ELEMENT mCpuFviTelemetryData[] = {

  { CPU_UCODE_VERSION_COMPONENT_ID,    DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
  { CPU_TXT_ACM_VERSION_COMPONENT_ID,  DEFAULT_FVI_TELEMETRY_VERSION,   FVI_INTEL_SIGNATURE,  },
};

//
// The order of this array must match TELEMETRY_ME_FVI_INDEX
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_6_5_PHAT_VERSION_ELEMENT mMeFviTelemetryData[] = {

  { ME_FIRMWARE_VERSION_COMPONENT_ID,  DEFAULT_FVI_TELEMETRY_VERSION,  FVI_INTEL_SIGNATURE,  },
  { ME_SSE_VERSION_COMPONENT_ID,       DEFAULT_FVI_TELEMETRY_VERSION,  FVI_INTEL_SIGNATURE,  },
};

/**
  Covert INTEL_FIRMWARE_VERSION to UINT64 Telemetry Version

  @param[in]       A pointer to INTEL_FIRMWARE_VERSION buffer
  @retval          A 64-bit value

**/
UINT64
IntelFwVerToUint64 (
  IN INTEL_FIRMWARE_VERSION *IntelFwVersion
  )
{
  return (((UINT64) LShiftU64 (IntelFwVersion->MajorVersion, VERSION_MAJOR_VERSION_SHIFT)) | \
          ((UINT64) IntelFwVersion->MinorVersion << VERSION_MINOR_VERSION_SHIFT) | \
          ((UINT64) IntelFwVersion->Revision     << VERSION_REVISION_SHIFT) | \
          ((UINT16) IntelFwVersion->BuildNumber));
}

/**
  Publish FSP binary version via Telemetry Version

  @param[in]     None

  @retval        EFI_SUCCESS
  @retval        EFI_NOT_FOUND.
  @retval        EFI_OUT_OF_RESOURCES.

**/
EFI_STATUS
InitializeFspRcFviData (
  VOID
  )
{
  EFI_STATUS        Status;
  UINT64            FspTelemetryVersionVal;
  UINT64            RcTelemetryVersionVal;
  FSP_INFO_HOB      *FspInfo;
  EFI_HOB_GUID_TYPE *GuidHob;

  //
  // Locate FSP_INFO_HOB
  //
  GuidHob = GetFirstGuidHob (&gFspInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }

  FspInfo = (FSP_INFO_HOB *) GET_GUID_HOB_DATA (GuidHob);

  //
  // Init FSP binary Fvi Telemetry record
  //

  FspTelemetryVersionVal = (UINT64)(LShiftU64 ((UINT64)(FspInfo->VersionMajor), VERSION_MAJOR_VERSION_SHIFT)
                         | LShiftU64 ((UINT64)(FspInfo->VersionMinor), VERSION_MINOR_VERSION_SHIFT)
                         | LShiftU64 ((UINT64)(FspInfo->VersionRevision), VERSION_REVISION_SHIFT)
                         | LShiftU64 ((UINT64)(FspInfo->VersionBuild) , VERSION_BUILD_NUM_SHIFT));
  Status = AppendTelemetryFviBlock (gFspBinVersionComponentId, FspTelemetryVersionVal, FVI_INTEL_SIGNATURE);
  DEBUG ((DEBUG_INFO, "FSP Fvi: AppendTelemetryFviBlock returns %r.\n", Status));

  // Need to set the SKU bit fields to ZERO for RC version.
  RcTelemetryVersionVal = (FspTelemetryVersionVal & ~(0xF << RC_BITFIELD_ZERO_SHIFT));
  Status = AppendTelemetryFviBlock (gRcVersionComponentId, RcTelemetryVersionVal, FVI_INTEL_SIGNATURE);
  DEBUG ((DEBUG_INFO, "RC Fvi: AppendTelemetryFviBlock returns %r.\n", Status));
  return Status;
}

/**
  Publish PCH Fvi in AIP protocol.

  @param[in] PchFviData        A pointer to the INTEL_FIRMWARE_VERSION_INFO
  @param[in] PchFviStrings     A pointer to the Fvi Strings information

  @retval EFI_SUCCESS          The Fvi data is published in AIP protocol.
  @retval Others               Failed to publish Fvi data in AIP protocol.

**/
EFI_STATUS
InitializePchFviData (
  IN INTEL_FIRMWARE_VERSION_INFO *PchFviData,
  IN CHAR8                       **PchFviStrings
  )
{
  EFI_STATUS  Status;
  UINT16      RecordCount;
  UINT16      Index;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  Index  = 0;
  Status = EFI_SUCCESS;

  //
  // PCH CRID
  //
  mPchFviTelemetryData[PchCridOriginal].VersionValue = IntelFwVerToUint64 (&PchFviData[PCH_CRID_ORIGINAL].Version);
  mPchFviTelemetryData[PchCridNew].VersionValue      = IntelFwVerToUint64 (&PchFviData[PCH_CRID_NEW].Version);

  //
  // PCH RST
  //
  mPchFviTelemetryData[RaidVer].VersionValue = IntelFwVerToUint64 (&PchFviData[RAID_VER].Version);

  //
  // Append Fvi block
  //
  RecordCount = sizeof (mPchFviTelemetryData) / sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT);
  for (Index = 0; Index < RecordCount; Index++) {
    Status = AppendTelemetryFviBlock (mPchFviTelemetryData[Index].ComponentId, mPchFviTelemetryData[Index].VersionValue,
                                      mPchFviTelemetryData[Index].ProducerId);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PCH Fvi: AppendTelemetryFviBlock returns %r.\n", Status));
      return Status;
    }
  }

  return Status;
}

/**
  Publish SA Fvi in AIP protocol.

  @param[in] SaFviData         A pointer to the INTEL_FIRMWARE_VERSION_INFO
  @param[in] SaFviStrings      A pointer to the Fvi Strings information

  @retval EFI_SUCCESS          The Fvi data is published in AIP protocol.
  @retval Others               Failed to publish Fvi data in AIP protocol.

**/
EFI_STATUS
InitializeSaFviData (
  IN INTEL_FIRMWARE_VERSION_INFO *SaFviData,
  IN CHAR8                       **SaFviStrings
  )
{
  EFI_STATUS  Status;
  UINT16      RecordCount;
  UINT16      Index;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  Index  = 0;
  Status = EFI_SUCCESS;

  //
  // RC version
  //
  mSaFviTelemetryData[MemRcVer].VersionValue = IntelFwVerToUint64 (&SaFviData[MEM_RC_VER].Version);

  //
  // CRID
  //
  mSaFviTelemetryData[CridOriginal].VersionValue = IntelFwVerToUint64 (&SaFviData[CRID_ORIGINAL].Version);
  mSaFviTelemetryData[Cridnew].VersionValue      = IntelFwVerToUint64 (&SaFviData[CRID_NEW].Version);

  //
  // VBIOS
  //
  mSaFviTelemetryData[VbiosVer].VersionValue = IntelFwVerToUint64 (&SaFviData[VBIOS_VER].Version);

  //
  // TCSS
  //
  mSaFviTelemetryData[IomFwVer].VersionValue = IntelFwVerToUint64 (&SaFviData[IOM_FW_VER].Version);
  mSaFviTelemetryData[PhyVer].VersionValue   = IntelFwVerToUint64 (&SaFviData[PHY_VER].Version);
  mSaFviTelemetryData[TbtFwVer].VersionValue = IntelFwVerToUint64 (&SaFviData[PHY_VER].Version);

  //
  // SAM
  //
  mSaFviTelemetryData[SamFwVer].VersionValue = IntelFwVerToUint64 (&SaFviData[SAM_FW_VER].Version);

  //
  // Append Fvi block
  //
  RecordCount = sizeof (mSaFviTelemetryData) / sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT);
  for (Index = 0; Index < RecordCount; Index++) {
    Status = AppendTelemetryFviBlock (mSaFviTelemetryData[Index].ComponentId, mSaFviTelemetryData[Index].VersionValue,
                                      mSaFviTelemetryData[Index].ProducerId);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "SA Fvi: AppendTelemetryFviBlock returns %r.\n", Status));
      return Status;
    }
  }

  return Status;
}

/**
  Publish CPU Fvi in AIP protocol.

  @param[in] CpuFviData        A pointer to the INTEL_FIRMWARE_VERSION_INFO
  @param[in] CpuFviStrings     A pointer to the Fvi Strings information

  @retval EFI_SUCCESS          The Fvi data is published in AIP protocol.
  @retval Others               Failed to publish Fvi data in AIP protocol.

**/
EFI_STATUS
EFIAPI
InitializeCpuFviData (
  IN INTEL_FIRMWARE_VERSION_INFO *CpuFviData,
  IN CHAR8                       **CpuFviStrings
  )
{
  EFI_STATUS  Status;
  UINT16      RecordCount;
  UINT16      Index;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  Index  = 0;
  Status = EFI_SUCCESS;

  //
  // Microcode
  //
  mCpuFviTelemetryData[UcodeVer].VersionValue = IntelFwVerToUint64 (&CpuFviData[UCODE_VER].Version);


  //
  // TXT
  //
  mCpuFviTelemetryData[TxtVer].VersionValue = IntelFwVerToUint64 (&CpuFviData[TXT_VER].Version);

  //
  // Append Fvi block
  //
  RecordCount = sizeof (mCpuFviTelemetryData) / sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT);
  for (Index = 0; Index < RecordCount; Index++) {
    Status = AppendTelemetryFviBlock (mCpuFviTelemetryData[Index].ComponentId, mCpuFviTelemetryData[Index].VersionValue,
                                      mCpuFviTelemetryData[Index].ProducerId);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "CPU Fvi: AppendTelemetryFviBlock returns %r.\n", Status));
      return Status;
    }
  }

  return Status;
}

/**
  Publish ME Fvi in AIP protocol.

  @param[in] MeFviData         A pointer to the INTEL_FIRMWARE_VERSION_INFO
  @param[in] MeFviStrings      A pointer to the Fvi Strings information

  @retval EFI_SUCCESS          The Fvi data is published in AIP protocol.
  @retval Others               Failed to publish Fvi data in AIP protocol.

**/
EFI_STATUS
EFIAPI
InitializeMeFviData (
  IN INTEL_FIRMWARE_VERSION_INFO    *MeFviData,
  IN CHAR8                          **MeFviStrings
  )
{
  EFI_STATUS  Status;
  UINT16      RecordCount;
  UINT16      Index;

  Index  = 0;
  Status = EFI_SUCCESS;

  //
  // ME
  //
  mMeFviTelemetryData[MeFwVer].VersionValue = IntelFwVerToUint64 (&MeFviData[EnumMeFw].Version);

  //
  // SSE
  //
  mMeFviTelemetryData[MeSseFwVer].VersionValue = IntelFwVerToUint64 (&MeFviData[EnumMeSse].Version);

  RecordCount = sizeof (mMeFviTelemetryData) / sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT);
  for (Index = 0; Index < RecordCount; Index++) {
    DEBUG ((EFI_D_INFO, "mMeFviTelemetryData[%d].VersionValue is %d\n", Index, mMeFviTelemetryData[Index].VersionValue));
    Status = AppendTelemetryFviBlock (mMeFviTelemetryData[Index].ComponentId, mMeFviTelemetryData[Index].VersionValue,
                                      mMeFviTelemetryData[Index].ProducerId);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ME Fvi: AppendTelemetryFviBlock returns %r.\n", Status));
      return Status;
    }
  }

  return Status;
}

/**
  Publish BiosGuard Fvi in AIP protocol.

  @param[in] BiosGuardFviData         A pointer to the INTEL_FIRMWARE_VERSION_INFO
  @param[in] BiosGuardFviStrings      A pointer to the Fvi Strings information

  @retval EFI_SUCCESS          The Fvi data is published in AIP protocol.
  @retval Others               Failed to publish Fvi data in AIP protocol.

**/
EFI_STATUS
EFIAPI
InitializeBiosGuardFviData (
  IN INTEL_FIRMWARE_VERSION_INFO    *BiosGuardFviData,
  IN CHAR8                          **BiosGuardFviStrings
  )
{
  return AppendTelemetryFviBlock (
           gBiosGuardComponentId,
           IntelFwVerToUint64 (&BiosGuardFviData->Version),
           FVI_INTEL_SIGNATURE
           );

}

/**
  Call Publish Fvi Telemetry Info table in AIP protocol

  @param[in]  Event
  @param[in]  *Context
**/
VOID
EFIAPI
InitFviTelemetryAip (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  VOID                        *Interface;
  EFI_STATUS                  Status;
  EFI_SMBIOS_PROTOCOL         *Smbios;
  EFI_SMBIOS_HANDLE           SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER     *Record;
  INTEL_FIRMWARE_VERSION_INFO *FviData;
  FVI_STRINGS                 *FviString;

  DEBUG ((DEBUG_INFO, "%a - Start\n", __FUNCTION__));

  Status = gBS->LocateProtocol (&gEfiDxeSmmReadyToLockProtocolGuid, NULL, &Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  gBS->CloseEvent (Event);

  InitializeFspRcFviData ();

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (Smbios == NULL || EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Error locating gEfiSmbiosProtocolGuid failure. Status = %r\n", Status));
    return;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  do {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR (Status)) {
      break;
    }
    if (Record->Type == INTEL_FVI_SMBIOS_TYPE) {

      FviData   = (INTEL_FIRMWARE_VERSION_INFO *)((UINT8 *)Record + sizeof (EFI_SMBIOS_TABLE_HEADER) + sizeof (UINT8));
      FviString = (FVI_STRINGS *)((UINT8 *)Record + Record->Length);

      //
      // SI Fvi
      //
      if (AsciiStrnCmp ((CHAR8 *) &FviString->ComponentName, PCH_CRID_STATUS_STRING, AsciiStrLen (PCH_CRID_STATUS_STRING)) == 0) {
        InitializePchFviData (FviData, (CHAR8 **) &FviString);
      }

      if (AsciiStrnCmp ((CHAR8 *) &FviString->ComponentName, MEM_FVI_STRING, AsciiStrLen (MEM_FVI_STRING)) == 0) {
        InitializeSaFviData (FviData, (CHAR8 **) &FviString);
      }

      if (AsciiStrnCmp ((CHAR8 *) &FviString->ComponentName, UCODE_FVI_STRING, AsciiStrLen (UCODE_FVI_STRING)) == 0) {
        InitializeCpuFviData (FviData, (CHAR8 **) &FviString);
      }

      if (AsciiStrnCmp ((CHAR8 *) &FviString->ComponentName, ME_FW_FVI_STRING, AsciiStrLen (ME_FW_FVI_STRING)) == 0) {
        InitializeMeFviData (FviData, (CHAR8 **) &FviString);
      }

      //
      // BiosGuard Fvi
      //
      if (AsciiStrnCmp ((CHAR8 *) &FviString->ComponentName, BIOS_GUARD_FVI_STRING, AsciiStrLen (BIOS_GUARD_FVI_STRING)) == 0) {
        InitializeBiosGuardFviData (FviData, (CHAR8 **) &FviString);
      }

    }
  } while (Status == EFI_SUCCESS);

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));

  return;
}


/**
  This is driver entry point to install AIP protocol.

  @param[in] ImageHandle  A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The initialization finished successfully.
**/
EFI_STATUS
EFIAPI
SiFirmwareVersionDxe (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS          Status;
  EFI_EVENT           GetSiFviEvent;
  VOID                *Registration;

  GetSiFviEvent = NULL;
  Status = EFI_SUCCESS;

  GetSiFviEvent = EfiCreateProtocolNotifyEvent (
                    &gEfiDxeSmmReadyToLockProtocolGuid,
                    TPL_CALLBACK,
                    InitFviTelemetryAip,
                    NULL,
                    &Registration
                    );

  if (GetSiFviEvent == NULL) {
    ASSERT (FALSE);
    Status = EFI_NOT_READY;
  }

  return Status;
}
