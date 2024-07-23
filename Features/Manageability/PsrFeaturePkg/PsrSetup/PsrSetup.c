/** @file
  This file contains source for PSR Setup initialization and support.

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

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DevicePathLib.h>

#include <Protocol/DevicePath.h>
#include <Protocol/HiiPopup.h>
#include <Protocol/HiiString.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/PsrProtocol.h>

#include <Guid/MdeModuleHii.h>

#include <MeBiosPayloadHob.h>
#include <PsrSettings.h>

EFI_HII_STRING_PROTOCOL           *mIfrLibHiiString               = NULL;
PLATFORM_SERVICE_RECORD_PROTOCOL  *mPlatformServiceRecordProtocol = NULL;
PSR_CONFIGURATION                 mPsrConfiguration               = {0};
EFI_GUID                          mPsrFormSetGuid                 = PSR_FORMSET_GUID;
EFI_HII_HANDLE                    mPsrHiiHandle;
EFI_HII_CONFIG_ROUTING_PROTOCOL   *mHiiConfigRouting;
EFI_HII_CONFIG_ACCESS_PROTOCOL    mPsrConfigAccess;
PSR_EXPORT_INFO_LINK              *mExportInfoLinkHead            = NULL;
UINT16                            mUsbDeviceCount                 = 0;
BOOLEAN                           mFirstEnterPsr                  = TRUE;

typedef struct {
  VENDOR_DEVICE_PATH        VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  End;
} HII_VENDOR_DEVICE_PATH;

HII_VENDOR_DEVICE_PATH mHiiVendorDevicePath = {
                           {
                             {
                               HARDWARE_DEVICE_PATH,
                               HW_VENDOR_DP,
                               {
                                 (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
                                 (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8)
                               }
                             },
                             PSR_FORMSET_GUID
                           },
                           {
                             END_DEVICE_PATH_TYPE,
                             END_ENTIRE_DEVICE_PATH_SUBTYPE,
                             {
                               (UINT8)(END_DEVICE_PATH_LENGTH),
                               (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
                             }
                           }
                         };

/**
  Convert a Value to a string in the format specified

  @param[in] HiiHandle     Handle to Hii database.
  @param[in] StrRef        Reference to string.
  @param[in] Format        Format specifier.

**/
VOID
InitString (
  EFI_HII_HANDLE    HiiHandle,
  EFI_STRING_ID     StrRef,
  CHAR16            *sFormat,
  ...
  )
{
  CHAR16      StringBuffer[1024];
  VA_LIST     ArgList;
  CHAR8       *SupportedLanguages;
  EFI_STATUS  Status;

  SupportedLanguages = HiiGetSupportedLanguages (HiiHandle);
  if (SupportedLanguages == NULL) {
    //
    // No supported language.
    //
    return;
  }

  //
  // Construct string value.
  //
  VA_START (ArgList, sFormat);
  UnicodeVSPrint (StringBuffer, sizeof (StringBuffer), sFormat, ArgList);
  VA_END (ArgList);

  //
  // Set string
  //
  if (mIfrLibHiiString == NULL) {
    Status = gBS->LocateProtocol (
                    &gEfiHiiStringProtocolGuid,
                    NULL,
                    (VOID **) &mIfrLibHiiString
                    );
    if (EFI_ERROR (Status)) {
      FreePool (SupportedLanguages);
      return;
    }
  }

  mIfrLibHiiString->SetString (
                      mIfrLibHiiString,
                      HiiHandle,
                      StrRef,
                      SupportedLanguages,
                      StringBuffer,
                      NULL
                      );

  FreePool (SupportedLanguages);
}

/**
  Convert PSR log state to its corresponding description
  @param[in]       PsrLogState        Psr Log State.
  @param[in, out]  PsrLogStateName    Psr Log State Description.
**/
VOID
GetPsrLogState (
  IN     UINT8      PsrLogState,
  IN OUT CHAR16     *PsrLogStateName
  )
{
  CHAR16            *PsrLogStateStr;
  UINTN             Index;

  switch (PsrLogState) {
    case PsrLogStateNotStarted:
      PsrLogStateStr = L"Not Started";
      break;

    case PsrLogStateStarted:
      PsrLogStateStr = L"Started";
      break;

    case PsrLogStateStopped:
      PsrLogStateStr = L"Stopped";
      break;

    default:
      PsrLogStateStr = L"Unknown";
      break;
  }

  UnicodeSPrint (PsrLogStateName, sizeof (CHAR16) * PSR_LOG_STATE_NAME_SIZE, PsrLogStateStr);
  //
  // Padding tail space to prevent random characters when scroll up/down
  //
  for (Index = StrLen (PsrLogStateStr); Index < (PSR_LOG_STATE_NAME_SIZE - 1); Index++) {
    UnicodeSPrint (PsrLogStateName + Index, sizeof (CHAR16) * PSR_LOG_STATE_NAME_SIZE, L" ");
  }
}

/**
  Convert CapabilityId to its corresponding description

  @param[in]  HiiHandle     Handle to Hii database.

**/
VOID
UpdatePsrCapabilitiesInfo (
  IN EFI_HII_HANDLE HiiHandle
  )
{
  UINT8               Index;
  EFI_STRING_ID       StringId;

  for (Index = 0; Index < PSR_MAX_SUPPORTED_CAPABILITIES; Index++) {
    switch (Index) {
      case PsrChassisIntrusion:
        StringId = STRING_TOKEN (STR_OPTION_PSR_CAP_CHASSIS_INTRUSION_VALUE);
        break;

      case PsrCriticalTemp:
        StringId = STRING_TOKEN (STR_OPTION_PSR_CAP_EXCESSIVE_OPERATIONAL_TEMPERATURE_VALUE);
        break;

      case PsrShockDetection:
        StringId = STRING_TOKEN (STR_OPTION_PSR_CAP_EXCESSIVE_SHOCK_VALUE);
        break;

      case PsrRpe:
        StringId = STRING_TOKEN (STR_OPTION_PSR_CAP_REMOTE_PLATFORM_ERASE_VALUE);
        break;

      case PsrLpe:
        StringId = STRING_TOKEN (STR_OPTION_PSR_CAP_LOCAL_PLATFORM_ERASE_VALUE);
        break;

      case PsrFwRecovery:
        StringId = STRING_TOKEN (STR_OPTION_PSR_CAP_FW_RECOVERY_VALUE);
        break;

      case PsrFwUpdate:
        StringId = STRING_TOKEN (STR_OPTION_PSR_CAP_FW_UPDATE_VALUE);
        break;

      case PsrSysHang:
        StringId = STRING_TOKEN (STR_OPTION_PSR_CAP_SYS_HANG_VALUE);
        break;

      case PsrPwrDrop:
        StringId = STRING_TOKEN (STR_OPTION_PSR_CAP_POWER_DROP_VALUE);
        break;

      default:
        StringId = 0;
    }

    if (StringId != 0) {
      InitString (
        HiiHandle,
        StringId,
        mPsrConfiguration.Capabilities[Index] ? PSR_CAPABILITIES_SUPPORTED : PSR_CAPABILITIES_NOT_SUPPORTED
        );
    }
  }
}

/**
  Convert EventSource to its corresponding description

  @param[in]       EventSource         Event Source.
  @param[in, out]  EventSourceName     Event Source Description.
**/
VOID
GetPsrEventSource (
  IN     UINT8      EventSource,
  IN OUT CHAR16     *EventSourceName
  )
{
  CHAR16            *EventSourceStr;
  UINTN             Index;

  if (EventSource == PsrEvtSrcRemote) {
    EventSourceStr = L"Remote Erase";
  } else if (EventSource == PsrEvtSrcLocal) {
    EventSourceStr = L"Local Erase";
  } else {
    EventSourceStr = L"Other";
  }

  UnicodeSPrint (EventSourceName, sizeof (CHAR16) * PSR_MAX_EVENT_NAME_SIZE, EventSourceStr);
  //
  // Padding tail space to prevent random characters when scroll up/down
  //
  for (Index = StrLen (EventSourceStr); Index < PSR_MAX_EVENT_NAME_SIZE; Index++) {
    UnicodeSPrint (EventSourceName + Index, sizeof (CHAR16) * PSR_MAX_EVENT_NAME_SIZE, L" ");
  }
}

/**
  Convert EventStatus to its corresponding description

  @param[in]       EventStatus         Event Status.
  @param[in, out]  EventStatusName     Event Status Description.
**/
VOID
GetPsrEventStatus (
  IN     UINT8      EventStatus,
  IN OUT CHAR16     *EventStatusName
  )
{
  CHAR16            *EventStatusStr;
  UINTN             Index;

  EventStatusStr = EventStatus? L"Success" : L"Fail";
  UnicodeSPrint (EventStatusName, sizeof (CHAR16) * PSR_MAX_EVENT_NAME_SIZE, EventStatusStr);
  //
  // Padding tail space to prevent random characters when scroll up/down
  //
  for (Index = StrLen (EventStatusStr); Index < PSR_MAX_EVENT_NAME_SIZE; Index++) {
    UnicodeSPrint (EventStatusName + Index, sizeof (CHAR16) * PSR_MAX_EVENT_NAME_SIZE, L" ");
  }
}

/**
  Convert EventActionId to its corresponding description

  @param[in]       EventActionId       Event Action Id.
  @param[in, out]  EventActionName     Event Action Description.
**/
VOID
GetPsrEventAction (
  IN     UINT8      EventActionId,
  IN OUT CHAR16     *EventActionName
  )
{
  CHAR16            *EventActionStr;
  UINTN             Index;

  switch (EventActionId) {
    case 2:
      EventActionStr = L"Secure Erase All SSD";
      break;

    case 3:
      EventActionStr = L"Verify Storage Erase";
      break;

    case 6:
      EventActionStr = L"TPM Clear";
      break;

    case 16:
      EventActionStr = L"OEM Custom Action";
      break;

    case 25:
      EventActionStr = L"Clear BIOS NVM Variables";
      break;

    case 26:
      EventActionStr = L"BIOS reload of Golden Config";
      break;

    case 31:
      EventActionStr = L"ME Unconfigure";
      break;

    default:
      EventActionStr = L"Unknown";
      break;
  }

  UnicodeSPrint (EventActionName, sizeof (CHAR16) * PSR_MAX_EVENT_NAME_SIZE, EventActionStr);

  //
  // Padding tail space to prevent random characters when scroll up/down
  //
  for (Index = StrLen (EventActionStr); Index < PSR_MAX_EVENT_NAME_SIZE; Index++) {
    UnicodeSPrint (EventActionName + Index, sizeof (CHAR16) * PSR_MAX_EVENT_NAME_SIZE, L" ");
  }
}

/**
  Convert EventId to its corresponding description

  @param[in]       EventId       Event Id.
  @param[in, out]  EventName     Event Description.
**/
VOID
GetPsrEventName (
  IN     UINT8      EventId,
  IN OUT CHAR16     *EventName
  )
{
  CHAR16            *EventNameStr;
  UINTN             Index;

  switch (EventId) {
    case PsrStartLogEvent:
      EventNameStr = L"Log Started";
      break;

    case PsrEndLogEvent:
      EventNameStr = L"Log Ended";
      break;

    case PsrMaxEventNumberReachedEvent:
      EventNameStr = L"Log Full";
      break;

    case PsrReplayProtectionInfraFailureEvent:
      EventNameStr = L"Replay Protection Infrastructure Failure";
      break;

    case PsrMissingEvent:
      EventNameStr = L"Log Missing";
      break;

    case PsrInvalidEvent:
      EventNameStr = L"Log Integrity Compromised";
      break;

    case PsrPrtcFailureEvent:
      EventNameStr = L"PRTC Reset";
      break;

    case PsrCsmeRecoveryEvent:
      EventNameStr = L"Log in Recovery State";
      break;

    case PsrCsmeEnteredDamStateEvent:
      EventNameStr = L"DAM State Entered";
      break;

    case PsrCsmeEnteredUnlockStateEvent:
      EventNameStr = L"Unlocked State Entered";
      break;

    case PsrSvnIncreaseEvent:
      EventNameStr = L"PSR SVN Incremented";
      break;

    case PsrChassisIntrusionEvent:
      EventNameStr = L"Chassis Intrusion Detected";
      break;

    case PsrExcessiveShockEvent:
      EventNameStr = L"Excessive Shock";
      break;

    case PsrExcessiveOperTemp:
      EventNameStr = L"Excessive Operational Temperature";
      break;

    case PsrErase:
      EventNameStr = L"Platform Erase";
      break;

    case PsrFwRecoveryEvent:
      EventNameStr = L"Firmware Recovery";
      break;

    case PsrFwUpdateEvent:
      EventNameStr = L"Firmware Update";
      break;

    case PsrSysHangEvent:
      EventNameStr = L"System Hang";
      break;

    case PsrPwrDropEvent:
      EventNameStr = L"Power Drop";
      break;

    default:
      EventNameStr = L"Unknown Event";
      break;
  }

  UnicodeSPrint (EventName, sizeof (CHAR16) * PSR_MAX_EVENT_NAME_SIZE, EventNameStr);
  //
  // Padding tail space to prevent random characters when scroll up/down
  //
  for (Index = StrLen (EventNameStr); Index < PSR_MAX_EVENT_NAME_SIZE; Index++) {
    UnicodeSPrint (EventName + Index, sizeof (CHAR16) * PSR_MAX_EVENT_NAME_SIZE, L" ");
  }
}

/**
  Calculate number of leap years in a given year range - between (EPOCH, Year).

  Difference of the number of Leap years in range (1, EPOCH) with the number of leap
  years in range (1, Year) will be the desired output

  @param[in] Year      Upperbound of year range

  @retval    Integer   Count of leap years between EPOCH (1970) and Year.
**/
UINT32
CountOfLeapYearsInRange (
  UINT32 Year
  )
{
  UINT32 LeapCount;

  // Number of Leap years in range (1, Year)
  LeapCount = (Year / 4) - (Year / 100) + (Year / 400);

  // Count of Leap years in range (EPOCH, Year)
  return (LeapCount - LEAP_YEARS_FROM_ONE_TO_EPOCH);
}

/**
  Convert Unix Timestamp to user readable YYYY/MM/DD HH:MM:SS format
  The Unix epoch is 00:00:00 UTC on 1 January 1970

  @param[in]       Timestamp     Unix timestamp in sec.
  @param[in] [out] LocalTime     Time Structure having converted value.

**/
VOID
UnixTimeStampToDate (
  IN     UINT32     Timestamp,
  IN OUT EFI_TIME   *LocalTime
  )
{
  UINT32  NumOfYears;
  UINT32  Leap;
  UINT32  Days;
  UINT32  Secs;
  UINT32  Months;
  UINT32  RemainingDays;
  UINT32  RemainingSecs;
  UINT32  CurrentYear;

  // Days in a month from Jan - Dec
  UINT32   DaysInMonth [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  Secs = Timestamp;

  // No. of days from the given timestamp
  Days = Secs / SEC_IN_A_DAY;

  RemainingSecs = Secs % SEC_IN_A_DAY;

  // Count of years - assuming all non-leap years
  NumOfYears = Days / 365;

  // Count of remaining days
  RemainingDays =  Days % 365;

  CurrentYear = (NumOfYears + EPOCH);

  // Count of leap years between (EPOCH , CurrentYear - 1) including both the bounds
  Leap = CountOfLeapYearsInRange (CurrentYear - 1);

  // If RemainingDays is lower than Leap, decrement CurrentYear by 1,
  // increment RemainingDays by 365 days and count Leap again.
  if (RemainingDays < Leap) {
    CurrentYear--;
    RemainingDays += 365;
    Leap = CountOfLeapYearsInRange (CurrentYear - 1);
  }

  // Decrement the RemainingDays by the count of leap years to get days passed
  // in current year
  RemainingDays -= Leap;

  // If current year is leap year, increment DaysInMonth of Feb.
  if ((((CurrentYear % 4 == 0) && (CurrentYear % 100 != 0) ) || (CurrentYear % 400 == 0))) {
    DaysInMonth[1]++;
  }

  // Decrement RemainingDays by each month to get returned month and day.
  for (Months = 0; DaysInMonth [Months] <= RemainingDays; Months++) {
    RemainingDays -= DaysInMonth [Months];
  }

  LocalTime->Year   =  (UINT16) CurrentYear;
  LocalTime->Month  =  (UINT8)  (Months + 1);
  LocalTime->Day    =  (UINT8)  (RemainingDays + 1);

  LocalTime->Hour   =  (UINT8)  (RemainingSecs / 3600);
  LocalTime->Minute =  (UINT8)  ((RemainingSecs / 60) % 60);
  LocalTime->Second =  (UINT8)  (RemainingSecs % 60);
}

/**
  Convert a Value to a string with Comma.

  @param[in] Value           The value to convert.
  @param[in] [out] String    Buffer with formatted string.

**/
VOID
ConvertToString (
  IN  UINT32    Value,
  IN OUT CHAR8  String[MAX_POWER_SEC_DIGITS]
  )
{
  UINT32  ModVal;
  UINT32  NextVal;
  UINT8   Digit;
  UINT8   RealDigit;
  UINT8   TotalDigit;
  UINT8   Check;
  CHAR8   TransferStr[MAX_POWER_SEC_DIGITS];

  ModVal    = 0;
  Digit     = 0;
  RealDigit = 0;
  NextVal   = Value;
  Check     = 0;

  if (NextVal > 0) {
    do {
      ModVal  = NextVal % 10;
      NextVal = NextVal / 10;
      TransferStr[Digit] = (CHAR8) (ModVal | 0x30);
      // Add ',' only if the input parameter to function is a 4 digit number (> 999) or greater
      if ((((Digit + 1) % 3) == Check) && Value > 999) {
        Check += 1;
        Digit += 1;
        TransferStr[Digit] = L',';
      }
      Digit += 1;
    } while (NextVal != 0);

    TotalDigit = Digit - 1;
    if (',' == TransferStr[TotalDigit]) {
      TotalDigit--;
      Digit--;
    }

    for (RealDigit = 0; RealDigit < Digit; RealDigit++) {
      String[RealDigit] = (CHAR8) TransferStr[TotalDigit];
      TotalDigit-=1;
    }
  } else {
    String[Digit] = L'0';
    Digit++;
  }
  String[Digit] = L'\0';

}

/**
  Removes (trims) specified leading and trailing characters from a string.

  @param[in][out]  Str      Pointer to the null-terminated string to be trimmed.
                            On return, Str will hold the trimmed string.

  @param[in]       CharC    Character will be trimmed from str.
**/
VOID
EFIAPI
StrTrim (
  IN OUT CHAR16    *Str,
  IN     CHAR16    CharC
  )
{
  CHAR16  *Pointer1;
  CHAR16  *Pointer2;

  if (*Str == 0) {
    return;
  }

  //
  // Trim off the leading and trailing characters c
  //
  for (Pointer1 = Str; (*Pointer1 != 0) && (*Pointer1 == CharC); Pointer1++) {
    ;
  }

  Pointer2 = Str;
  if (Pointer2 == Pointer1) {
    while (*Pointer1 != 0) {
      Pointer2++;
      Pointer1++;
    }
  } else {
    while (*Pointer1 != 0) {
    *Pointer2 = *Pointer1;
    Pointer1++;
    Pointer2++;
    }
    *Pointer2 = 0;
  }

  for (Pointer1 = Str + StrLen(Str) - 1; Pointer1 >= Str && *Pointer1 == CharC; Pointer1--) {
    ;
  }
  if  (Pointer1 !=  Str + StrLen(Str) - 1) {
    *(Pointer1 + 1) = 0;
  }
}

/**
  Show current PSR configuration.
**/
VOID
ShowConfig (
  VOID
  )
{
  UINT32       Index;

  DEBUG ((DEBUG_INFO, "[%a] PSR configuration dump...\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "PsrState   = %d\n"       , mPsrConfiguration.PsrLogState));
  DEBUG ((DEBUG_INFO, "PsrVersion = %02d.%02d\n", mPsrConfiguration.PsrVersion.Major, mPsrConfiguration.PsrVersion.Minor));
  DEBUG ((DEBUG_INFO, "Psrid      = %g\n"       , mPsrConfiguration.Psrid));
  DEBUG ((DEBUG_INFO, "Intel Upid = %016llx-%016llx-%016llx-%016llx-%016llx-%016llx-%016llx-%016llx\n",
    mPsrConfiguration.Upid.Name1, mPsrConfiguration.Upid.Name2,
    mPsrConfiguration.Upid.Name3, mPsrConfiguration.Upid.Name4,
    mPsrConfiguration.Upid.Name5, mPsrConfiguration.Upid.Name6,
    mPsrConfiguration.Upid.Name7, mPsrConfiguration.Upid.Name8
    ));

  DEBUG ((DEBUG_INFO, "Psr Cap: \n"));
  for (Index = 0; Index < PSR_MAX_SUPPORTED_CAPABILITIES ; Index++) {
    DEBUG ((DEBUG_INFO, "%02x ", mPsrConfiguration.Capabilities[Index]));
    if ((Index + 1) % 16 == 0) {
      DEBUG ((DEBUG_INFO, "\n"));
    }
  }

  DEBUG ((DEBUG_INFO, "OemInfo                = %a\n"  , mPsrConfiguration.OemInfo));
  DEBUG ((DEBUG_INFO, "OemMakeInfo            = %a\n"  , mPsrConfiguration.OemMakeInfo));
  DEBUG ((DEBUG_INFO, "OemModelInfo           = %a\n"  , mPsrConfiguration.OemModelInfo));
  DEBUG ((DEBUG_INFO, "ManufCOE               = %a\n"  , mPsrConfiguration.ManufCOE));
  DEBUG ((DEBUG_INFO, "S0 Runtime Counter     = %10d\n", mPsrConfiguration.S0RuntimeCtr));
  DEBUG ((DEBUG_INFO, "S0 to S5 Counter       = %10d\n", mPsrConfiguration.S0toS5Ctr));
  DEBUG ((DEBUG_INFO, "S0 to S4 Counter       = %10d\n", mPsrConfiguration.S0toS4Ctr));
  DEBUG ((DEBUG_INFO, "S0 to S3 Counter       = %10d\n", mPsrConfiguration.S0toS3Ctr));
  DEBUG ((DEBUG_INFO, "WarmReset Counter      = %10d\n", mPsrConfiguration.WarmResetCtr));
  DEBUG ((DEBUG_INFO, "IshConnection Counter  = %10d\n", mPsrConfiguration.IshConnectionCtr));
  DEBUG ((DEBUG_INFO, "CsmeReset Counter      = %10d\n", mPsrConfiguration.CsmeResetCtr));
  DEBUG ((DEBUG_INFO, "PrtcReset Counter      = %10d\n", mPsrConfiguration.PrtcFailureCtr));
  DEBUG ((DEBUG_INFO, "RecoveryState Counter  = %10d\n", mPsrConfiguration.CsmeInvalidStateCtr));
  DEBUG ((DEBUG_INFO, "DAMStateEnt Counter    = %10d\n", mPsrConfiguration.CsmeDamCtr));
  DEBUG ((DEBUG_INFO, "UnlockStateEnt Counter = %10d\n", mPsrConfiguration.CsmeUnlockedCtr));
  DEBUG ((DEBUG_INFO, "PsrSvnIncre Counter    = %10d\n", mPsrConfiguration.SvnIncreaseCtr));
  DEBUG ((DEBUG_INFO, "ExcessiveShock Counter = %10d\n", mPsrConfiguration.ExcessiveShockCtr));
  DEBUG ((DEBUG_INFO, "ExcessiveTemp Counter  = %10d\n", mPsrConfiguration.ExcessiveOperationalTempCtr));
  DEBUG ((DEBUG_INFO, "FwRecovery Counter     = %10d\n", mPsrConfiguration.FwRecoveryCtr));
  DEBUG ((DEBUG_INFO, "FwUpdate Counter       = %10d\n", mPsrConfiguration.FwUpdateCtr));
  DEBUG ((DEBUG_INFO, "SystemHang Counter     = %10d\n", mPsrConfiguration.SysHangCtr));
  DEBUG ((DEBUG_INFO, "PowerDrop Counter      = %10d\n", mPsrConfiguration.PwrDropCtr));

  DEBUG ((DEBUG_INFO, "TotalEventCount        = %10d\n", mPsrConfiguration.TotalEventCount));
  for (Index = 0; Index < mPsrConfiguration.TotalEventCount; Index++) {
    DEBUG ((DEBUG_INFO, "EventIndex [%02d] - Id = 0x%02x, Time = 0x%0llx, Sta = 0x%x, Act = 0x%x, Src = 0x%x\n",
      Index,
      mPsrConfiguration.Events[Index].EventId,
      mPsrConfiguration.Events[Index].EventTime,
      mPsrConfiguration.Events[Index].EventStatus,
      mPsrConfiguration.Events[Index].EventAction,
      mPsrConfiguration.Events[Index].EventSource
      ));
  }
  DEBUG ((DEBUG_INFO, "CsmeVersion = %d. %d. %d. %d\n",
    mPsrConfiguration.FwVersion.Major,
    mPsrConfiguration.FwVersion.Minor,
    mPsrConfiguration.FwVersion.Hotfix,
    mPsrConfiguration.FwVersion.Build
    ));
}

/**
  Show the content of current connected USB export devices.
**/
VOID
DumpPsrUsbExportOption (
  VOID
  )
{
  PSR_EXPORT_INFO_LINK  *Dump;
  LIST_ENTRY            *Link;
  UINT16                Index = 0;
  CHAR16                *TempString;
  CHAR16                *TempString1;

  if ((mExportInfoLinkHead == NULL) || (IsListEmpty (&mExportInfoLinkHead->Link))) {
    return;
  }

  Dump        = NULL;
  TempString  = NULL;
  TempString1 = NULL;

  DEBUG ((DEBUG_INFO, "[%a] Show USB export device content...\n", __FUNCTION__));

  Link = GetFirstNode (&mExportInfoLinkHead->Link);
  while (!IsNull (&mExportInfoLinkHead->Link, Link)) {
    Dump = PSR_EXPORT_INFO_FROM_LINK (Link);
    DEBUG ((DEBUG_INFO, "Dump USB Mass Storage [%d] Data Content\n", Index));
    DEBUG ((DEBUG_INFO, "  PsrExportInfoNode     = %x\n", Dump));
    DEBUG ((DEBUG_INFO, "  TargetblkHandle       = %x\n", Dump->TargetblkHandle));
    TempString = ConvertDevicePathToText (Dump->TargetDevicePath, TRUE, TRUE);
    DEBUG ((DEBUG_INFO, "  TargetDevicePath      = %s\n", TempString));
    TempString1 = AllocateCopyPool (StrSize (Dump->TargetUsbDescription), Dump->TargetUsbDescription);
    DEBUG ((DEBUG_INFO, "  TargetUsbDescription  = %s\n", TempString1));
    Link = GetNextNode (&mExportInfoLinkHead->Link, Link);
    Index++;
  }

  FreePool (TempString);
  FreePool (TempString1);

  return;
}

/**
  Free Psr USB Export Info Link.
**/
VOID
FreePsrUsbInfoLink (
  VOID
  )
{
  LIST_ENTRY            *FreeLink;
  PSR_EXPORT_INFO_LINK  *FreeRecord;

  if (mExportInfoLinkHead == NULL) {
    return;
  }

  if (!IsListEmpty (&mExportInfoLinkHead->Link)) {
    FreeLink  = GetFirstNode (&mExportInfoLinkHead->Link);
    while (!IsNull (&mExportInfoLinkHead->Link, FreeLink)) {
      FreeRecord = PSR_EXPORT_INFO_FROM_LINK (FreeLink);
      FreeLink = RemoveEntryList (&FreeRecord->Link);
      FreePool (FreeRecord);
    }
  }

  mExportInfoLinkHead = NULL;
}

/**
  Pop up check box for usb export status.

  @param[in] ExportStatus    Status of usb export action.

  @retval    Status          Status of creating pop up action.
**/
EFI_STATUS
EFIAPI
PsrCreatePopUp (
  IN EFI_STATUS    ExportStatus
  )
{
  EFI_HII_POPUP_PROTOCOL   *HiiPopUp;
  EFI_HII_HANDLE           HiiHandle;
  EFI_HII_POPUP_STYLE      PopUpStyle;
  EFI_HII_POPUP_TYPE       PopUpType;
  EFI_HII_POPUP_SELECTION  PopUpSelect;
  EFI_STATUS               Status;

  HiiHandle = mPsrHiiHandle;

  Status = gBS->LocateProtocol (&gEfiHiiPopupProtocolGuid, NULL, (VOID**) &HiiPopUp);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  switch (ExportStatus) {
    case EFI_SUCCESS:
      PopUpStyle = EfiHiiPopupStyleInfo;
      PopUpType = EfiHiiPopupTypeOk;
      HiiSetString (HiiHandle, STRING_TOKEN (STR_PSR_RECORD_EXPORT_STATUS_RETURN), L"Export File Success.", NULL);
      break;

    case EFI_UNSUPPORTED:
      PopUpStyle = EfiHiiPopupStyleError;
      PopUpType = EfiHiiPopupTypeOk;
      HiiSetString (HiiHandle, STRING_TOKEN (STR_PSR_RECORD_EXPORT_STATUS_RETURN), L"Export File Fail.", NULL);
      break;

    case EFI_NOT_FOUND:
      PopUpStyle = EfiHiiPopupStyleWarning;
      PopUpType = EfiHiiPopupTypeOk;
      HiiSetString (HiiHandle, STRING_TOKEN (STR_PSR_RECORD_EXPORT_STATUS_RETURN), L"Cannot Find The USB", NULL);
      break;

    default:
      PopUpStyle = EfiHiiPopupStyleInfo;
      PopUpType = EfiHiiPopupTypeOk;
      HiiSetString (HiiHandle, STRING_TOKEN (STR_PSR_RECORD_EXPORT_STATUS_RETURN), L"To Be Defined", NULL);
      break;
  }

  Status = HiiPopUp->CreatePopup (
                       HiiPopUp,
                       PopUpStyle,
                       PopUpType,
                       HiiHandle,
                       STRING_TOKEN (STR_PSR_RECORD_EXPORT_STATUS_RETURN),
                       &PopUpSelect
                       );

  DEBUG ((DEBUG_INFO, "Create PopUp Message Box Status: %r\n", Status));

  return Status;
}

/**
  See UEFI spec for the general meaning of Callback function.
  This function mostly just calls other, specific callback functions depending on question ID
  For Remote Assistance it also toggles between two strings to make impression that code didn't hang yet while
  it waits for remote connection

  @param[in]  This                    Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Action                  Specifies the type of action taken by the browser.
  @param[in]  QuestionId              A unique value which is sent to the original exporting driver
                                      so that it can identify the type of data to expect.
  @param[in]  Type                    The type of value for the question.
  @param[in]  Value                   A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest           On return, points to the action requested by the callback function.

  @retval     EFI_SUCCESS             The callback successfully handled the action.
  @retval     EFI_OUT_OF_RESOURCES    Not enough storage is available to hold the variable and its data.
  @retval     EFI_DEVICE_ERROR        The variable could not be saved.
  @retval     EFI_UNSUPPORTED         The specified Action is not supported by the callback.
**/
EFI_STATUS
EFIAPI
PsrDriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN        EFI_BROWSER_ACTION                Action,
  IN        EFI_QUESTION_ID                   QuestionId,
  IN        UINT8                             Type,
  IN        EFI_IFR_TYPE_VALUE                *Value,
  OUT       EFI_BROWSER_ACTION_REQUEST        *ActionRequest
  )
{
  EFI_STATUS            Status;
  EFI_HII_HANDLE        HiiHandle;
  VOID                  *StartOpCodeHandle;
  VOID                  *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL    *StartLabel;
  EFI_IFR_GUID_LABEL    *EndLabel;
  CHAR16                *UsbOptionDesc;
  CHAR16                *TempString;
  CHAR16                *HelpString;
  CHAR8                 StrBuf[MAX_POWER_SEC_DIGITS];
  EFI_STRING_ID         Token;
  EFI_STRING_ID         HelpToken;
  EFI_STRING_ID         EventList;
  EFI_HANDLE            *ExportHandle;
  UINT16                SearchNum;
  UINT16                TargetDevice;
  UINT32                EventIndex;
  EVENT_DATE_FORMAT     *EventDisplay;
  LIST_ENTRY            *PsrLink;
  PSR_EXPORT_INFO_LINK  *TargetDev;
  UINT32                TimeInUnixFormat;
  EFI_TIME              LocTime;
  EFI_TIME              LocEventTime;
  CHAR16                EventName[PSR_MAX_EVENT_NAME_SIZE];
  CHAR16                EventStatus[PSR_MAX_EVENT_NAME_SIZE];
  CHAR16                EventAction[PSR_MAX_EVENT_NAME_SIZE];
  CHAR16                EventSource[PSR_MAX_EVENT_NAME_SIZE];
  CHAR16                PsrLogCurState[PSR_LOG_STATE_NAME_SIZE];
  BOOLEAN               IsRtcClearEvent;

  StartOpCodeHandle = NULL;
  EndOpCodeHandle   = NULL;
  UsbOptionDesc     = NULL;
  HelpString        = NULL;
  TempString        = NULL;
  IsRtcClearEvent   = FALSE;

  HiiHandle = mPsrHiiHandle;

  if (Action == EFI_BROWSER_ACTION_FORM_OPEN) {
    //
    // Allocate space for creation of UpdateData Buffer
    //
    StartOpCodeHandle = HiiAllocateOpCodeHandle ();
    ASSERT (StartOpCodeHandle != NULL);
    if (StartOpCodeHandle == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    EndOpCodeHandle = HiiAllocateOpCodeHandle ();
    ASSERT (EndOpCodeHandle != NULL);
    if (EndOpCodeHandle == NULL) {
      HiiFreeOpCodeHandle (StartOpCodeHandle);
      return EFI_OUT_OF_RESOURCES;
    }
    //
    // Create Hii Extend Label OpCode as the start opcode
    //
    StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
    StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    //
    // Create Hii Extend Label OpCode as the end opcode
    //
    EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
    EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

    if (QuestionId == FORM_ID_PSR) {
      //
      // When user opens FORM_ID_PSR main menu,
      // Parse PSR Data and display the content.
      //
      StartLabel->Number = PSR_LABEL_START;
      EndLabel->Number   = PSR_LABEL_END;

      //
      // Only get PSR data blob at the first time enter the PSR menu.
      // To prevent sending HECI cmd in setup so many times.
      //
      if (mFirstEnterPsr) {
        mFirstEnterPsr = FALSE;
        PsrGetInfo (&mPsrConfiguration);
      }
      ShowConfig ();

      InitString (
        HiiHandle,
        STRING_TOKEN (STR_OPTION_PSR_GDL_INTEL_UPID_VALUE1),
        L"%016llx-%016llx",
        mPsrConfiguration.Upid.Name1,
        mPsrConfiguration.Upid.Name2);

      InitString (
        HiiHandle,
        STRING_TOKEN (STR_OPTION_PSR_GDL_INTEL_UPID_VALUE2),
        L"%016llx-%016llx",
        mPsrConfiguration.Upid.Name3,
        mPsrConfiguration.Upid.Name4);

      InitString (
        HiiHandle,
        STRING_TOKEN (STR_OPTION_PSR_GDL_INTEL_UPID_VALUE3),
        L"%016llx-%016llx",
        mPsrConfiguration.Upid.Name5,
        mPsrConfiguration.Upid.Name6);

      InitString (
        HiiHandle,
        STRING_TOKEN (STR_OPTION_PSR_GDL_INTEL_UPID_VALUE4),
        L"%016llx-%016llx",
        mPsrConfiguration.Upid.Name7,
        mPsrConfiguration.Upid.Name8);

      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_GDL_PSR_ID_VALUE)  , L"%g"       , mPsrConfiguration.Psrid     );
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_GDL_PSR_VER_VALUE) , L"%02d.%02d", mPsrConfiguration.PsrVersion.Major, mPsrConfiguration.PsrVersion.Minor);

      GetPsrLogState (mPsrConfiguration.PsrLogState, PsrLogCurState);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_LOG_STATE_VALUE)   , L"%s", PsrLogCurState);

      // PSR Status

      TimeInUnixFormat = mPsrConfiguration.GenesisDate;
      UnixTimeStampToDate (TimeInUnixFormat, &LocTime);
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_OPTION_PSR_GDL_GENESIS_DATE_VALUE),
        L"%04d/%02d/%02d %02d:%02d:%02d",
        LocTime.Year,
        LocTime.Month,
        LocTime.Day,
        LocTime.Hour,
        LocTime.Minute,
        LocTime.Second
        );

      UpdatePsrCapabilitiesInfo (HiiHandle);

      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_GDL_OEM_INFO_VALUE)      , L"%a", mPsrConfiguration.OemInfo      );
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_GDL_OEM_MAKE_INFO_VALUE) , L"%a", mPsrConfiguration.OemMakeInfo  );
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_GDL_OEM_MODEL_INFO_VALUE), L"%a", mPsrConfiguration.OemModelInfo );
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_GDL_MANUF_COE_VALUE)     , L"%a", mPsrConfiguration.ManufCOE     );

      ConvertToString (mPsrConfiguration.S0RuntimeCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_SO_RUNTIME_VALUE)  , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.S0toS5Ctr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_S0_TO_S5_CTR_VALUE), L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.S0toS4Ctr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_S0_TO_S4_CTR_VALUE), L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.S0toS3Ctr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_S0_TO_S3_CTR_VALUE), L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.WarmResetCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_PLAT_RST_CTR_VALUE), L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.IshConnectionCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_ISH_CONNECTION_CTR_VALUE)     , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.CsmeResetCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_CSME_RESET_CTR_VALUE)         , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.PrtcFailureCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_PRTC_FAILURE_CTR_VALUE)       , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.CsmeInvalidStateCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_CSME_INVALID_STATE_CTR_VALUE) , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.CsmeDamCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_CSME_DAM_STATE_CTR_VALUE)     , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.CsmeUnlockedCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_CSME_UNLOCKED_STATE_CTR_VALUE), L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.SvnIncreaseCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_SVN_INCREASE_CTR_VALUE)       , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.ExcessiveShockCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_EXCESSIVE_SHOCK_CTR_VALUE)    , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.ExcessiveOperationalTempCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_EXCESSIVE_OPER_TEMP_CTR_VALUE), L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.FwRecoveryCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_FW_RECOVERY_CTR_VALUE)        , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.FwUpdateCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_FW_UPDATE_CTR_VALUE)          , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.SysHangCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_SYS_HANG_CTR_VALUE)           , L"%a", StrBuf);

      ConvertToString (mPsrConfiguration.PwrDropCtr, StrBuf);
      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_PSHC_PWR_DROP_CTR_VALUE)           , L"%a", StrBuf);

      InitString (HiiHandle, STRING_TOKEN (STR_OPTION_PSR_GDL_CSME_VER_VALUE)                , L"%d. %d. %d. %d", mPsrConfiguration.FwVersion.Major, mPsrConfiguration.FwVersion.Minor, mPsrConfiguration.FwVersion.Hotfix, mPsrConfiguration.FwVersion.Build);

      HiiUpdateForm (
        HiiHandle,
        &mPsrFormSetGuid,
        FORM_ID_PSR,
        StartOpCodeHandle,
        EndOpCodeHandle
        );
    } else if (QuestionId == FORM_ID_PSR_EDL) {
      //
      // When user opens FORM_ID_PSR_EDL submenu,
      // Parse Event Data and display the content.
      //
      StartLabel->Number = PSR_EVENT_LABEL;
      EndLabel->Number   = PSR_EVENT_LABEL_END;

      EventDisplay = AllocateZeroPool (sizeof (EVENT_DATE_FORMAT));
      if (EventDisplay != NULL) {

        for (EventIndex = 0; EventIndex < mPsrConfiguration.TotalEventCount; EventIndex++) {
          EventList = HiiSetString (HiiHandle, 0, L" ", NULL);
          HiiCreateTextOpCode (
            StartOpCodeHandle,
            STRING_TOKEN (EventList),
            0,
            0
            );

          GetPsrEventName (mPsrConfiguration.Events[EventIndex].EventId, EventName);

          if ((PsrPrtcFailureEvent == mPsrConfiguration.Events[EventIndex].EventId) && (!IsRtcClearEvent)) {
            IsRtcClearEvent = TRUE;
          }
          if (!IsRtcClearEvent) {
            TimeInUnixFormat = mPsrConfiguration.Events[EventIndex].EventTime + mPsrConfiguration.GenesisDate;
            UnixTimeStampToDate (TimeInUnixFormat, &LocEventTime);

            EventDisplay->Year    =  LocEventTime.Year;
            EventDisplay->Month   =  LocEventTime.Month;
            EventDisplay->Day     =  LocEventTime.Day;
            EventDisplay->Hour    =  LocEventTime.Hour;
            EventDisplay->Minute  =  LocEventTime.Minute;
            EventDisplay->Second  =  LocEventTime.Second;

            if (mPsrConfiguration.Events[EventIndex].EventId == PsrErase) {
              GetPsrEventAction (mPsrConfiguration.Events[EventIndex].EventAction, EventAction);
              GetPsrEventStatus (mPsrConfiguration.Events[EventIndex].EventStatus, EventStatus);
              GetPsrEventSource (mPsrConfiguration.Events[EventIndex].EventSource, EventSource);

              InitString (
                HiiHandle,
                STRING_TOKEN (EventList),
                L"\nEvent %d : %s \n Timestamp :  %4d/%02d/%02d %02d:%02d:%02d \n Event Status: %s Event Action: %s Event Source: %s",
                EventIndex,
                EventName,
                EventDisplay->Year,
                EventDisplay->Month,
                EventDisplay->Day,
                EventDisplay->Hour,
                EventDisplay->Minute,
                EventDisplay->Second,
                EventStatus,
                EventAction,
                EventSource
                );
            } else {
              InitString (
                HiiHandle,
                STRING_TOKEN (EventList),
                L"\nEvent %d : %s \n Timestamp : %4d/%02d/%02d %02d:%02d:%02d\n",
                EventIndex,
                EventName,
                EventDisplay->Year,
                EventDisplay->Month,
                EventDisplay->Day,
                EventDisplay->Hour,
                EventDisplay->Minute,
                EventDisplay->Second
                );
            }
          } else {
            if ((PsrPrtcFailureEvent == mPsrConfiguration.Events[EventIndex].EventId)) {
              InitString (
                HiiHandle,
                STRING_TOKEN (EventList),
                L"\nEvent %d : %s \n Timestamp : Unknown \n",
                EventIndex,
                EventName
                );
            } else if (mPsrConfiguration.Events[EventIndex].EventId == PsrErase) {
              TimeInUnixFormat = mPsrConfiguration.Events[EventIndex].EventTime;
              GetPsrEventAction (mPsrConfiguration.Events[EventIndex].EventAction, EventAction);
              GetPsrEventStatus (mPsrConfiguration.Events[EventIndex].EventStatus, EventStatus);
              GetPsrEventSource (mPsrConfiguration.Events[EventIndex].EventSource, EventSource);

              InitString (
                HiiHandle,
                STRING_TOKEN (EventList),
                L"\nEvent %d : %s \n Timestamp :  %d s since last RTC event \n Event Status: %s Event Action: %s Event Source: %s",
                EventIndex,
                EventName,
                TimeInUnixFormat,
                EventStatus,
                EventAction,
                EventSource
                );
            } else {
              TimeInUnixFormat = mPsrConfiguration.Events[EventIndex].EventTime;
              InitString (
                HiiHandle,
                STRING_TOKEN (EventList),
                L"\nEvent %d : %s \n Timestamp : %d s since last RTC event \n",
                EventIndex,
                EventName,
                TimeInUnixFormat
                );
            }
          }
        }
        HiiUpdateForm (
          HiiHandle,
          &mPsrFormSetGuid,
          FORM_ID_PSR_EDL,
          StartOpCodeHandle,
          EndOpCodeHandle
          );
        FreePool (EventDisplay);
        IsRtcClearEvent = FALSE;
      }
    } else if (QuestionId == FORM_ID_PSR_RECORD_EXPORT) {
      //
      // When user opens FORM_ID_PSR_RECORD_EXPORT submenu, need to call the Locate PSR protocol
      // and list all flash drives available for data export
      //
      mExportInfoLinkHead = DetectAndListAvailableFlashDrives ();
      DumpPsrUsbExportOption ();

      StartLabel->Number     = PSR_EXPORT_OPTION_LABEL;
      EndLabel->Number       = PSR_EXPORT_OPTION_LABEL_END;

      if ((mExportInfoLinkHead != NULL) && (!IsListEmpty (&mExportInfoLinkHead->Link))) {
        PsrLink = GetFirstNode (&mExportInfoLinkHead->Link);
        while (!IsNull (&mExportInfoLinkHead->Link, PsrLink)) {
          TargetDev = PSR_EXPORT_INFO_FROM_LINK (PsrLink);
          UsbOptionDesc = AllocateCopyPool (StrSize (TargetDev->TargetUsbDescription), TargetDev->TargetUsbDescription);
          ASSERT (UsbOptionDesc != NULL);
          if (UsbOptionDesc == NULL) {
            HiiFreeOpCodeHandle (StartOpCodeHandle);
            HiiFreeOpCodeHandle (EndOpCodeHandle);
            return EFI_NOT_FOUND;
          }
          StrTrim (UsbOptionDesc, L' ');
          Token = HiiSetString (HiiHandle, 0, UsbOptionDesc, NULL);
          FreePool (UsbOptionDesc);

          TempString = ConvertDevicePathToText (TargetDev->TargetDevicePath, TRUE, TRUE);
          if (TempString != NULL) {
            HelpString = AllocateCopyPool (StrSize (TempString), TempString);
          }
          ASSERT (HelpString != NULL);
          if (HelpString == NULL) {
            HiiFreeOpCodeHandle (StartOpCodeHandle);
            HiiFreeOpCodeHandle (EndOpCodeHandle);
            return EFI_NOT_FOUND;
          }
          HelpToken = HiiSetString (HiiHandle, 0, HelpString, NULL);
          FreePool (HelpString);

          HiiCreateGotoOpCode (
            StartOpCodeHandle,
            FORM_ID_PSR_RECORD_EXPORT,
            Token,
            HelpToken,
            EFI_IFR_FLAG_CALLBACK,
            (EFI_QUESTION_ID) (KEY_PSR_EXPORT_BASE + mUsbDeviceCount)
            );

          mUsbDeviceCount++;
          PsrLink = GetNextNode (&mExportInfoLinkHead->Link, PsrLink);
        }

        HiiUpdateForm (
          HiiHandle,
          &mPsrFormSetGuid,
          FORM_ID_PSR_RECORD_EXPORT,
          StartOpCodeHandle,
          EndOpCodeHandle
          );
      }

    }
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);
  }

  if (Action == EFI_BROWSER_ACTION_FORM_CLOSE) {
    if (QuestionId == FORM_ID_PSR_RECORD_EXPORT) {
      mUsbDeviceCount = 0;
      FreePsrUsbInfoLink ();
    }
  }

  if ((QuestionId >= KEY_PSR_EXPORT_BASE) && (QuestionId < (mUsbDeviceCount + KEY_PSR_EXPORT_BASE))) {
    if (Action == EFI_BROWSER_ACTION_CHANGING) {
      SearchNum = 0;
      if ((mExportInfoLinkHead != NULL) && (!IsListEmpty (&mExportInfoLinkHead->Link))) {
        PsrLink = GetFirstNode (&mExportInfoLinkHead->Link);
        if (!IsNull (&mExportInfoLinkHead->Link, PsrLink)) {
          TargetDevice = (UINT16) (QuestionId - KEY_PSR_EXPORT_BASE);
          //
          // Searching correspond data that user select.
          //
          while (SearchNum <= TargetDevice) {
            if (SearchNum != 0) {
              PsrLink = GetNextNode ( &mExportInfoLinkHead->Link, PsrLink);
            }
            SearchNum++;
          }
          TargetDev = PSR_EXPORT_INFO_FROM_LINK (PsrLink);
          ExportHandle = TargetDev->TargetblkHandle;
          DEBUG ((DEBUG_INFO, "User select: %s\n", TargetDev->TargetUsbDescription));

          //
          // Export PSR data to the selected USB Storage.
          //
          Status = mPlatformServiceRecordProtocol->PsrExportToUsbStorage (
                                                    mPlatformServiceRecordProtocol,
                                                    ExportHandle
                                                    );
          DEBUG((DEBUG_INFO, "Export PSR to Flash Status: %r\n", Status));
          PsrCreatePopUp (Status);
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  See UEFI spec for the general meaning of ExtractConfig function.
  PSR is unusual in that it doesn't store configuration entered by user in EFI flash variables.
  Instead, all(*) config is retrieved from CSME firmware using HECI messages.

  @param[in]  This                     Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Request                  A null-terminated Unicode string in <ConfigRequest> format.
  @param[out] Progress                 On return, points to a character in the Request string.
                                       Points to the string's null terminator if request was successful.
                                       Points to the most recent '&' before the first failing name/value pair (or
                                       the beginning of the string if the failure is in the first name/value pair)
                                       if the request was not successful.
  @param[out] Results                  A null-terminated Unicode string in <ConfigAltResp> format which has
                                       all values filled in for the names in the Request string. String to
                                       be allocated by the called function.

  @retval     EFI_SUCCESS              The Results is filled with the requested values.
  @retval     EFI_OUT_OF_RESOURCES     Not enough memory to store the results.
  @retval     EFI_INVALID_PARAMETER    Request is illegal syntax, or unknown name.
  @retval     EFI_NOT_FOUND            Routing data doesn't match any storage in this driver.
**/
EFI_STATUS
EFIAPI
PsrExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN  CONST EFI_STRING                        Request,
  OUT       EFI_STRING                        *Progress,
  OUT       EFI_STRING                        *Results
  )
{
  DEBUG ((DEBUG_INFO, "PSR Setup Extract Config:\n"));

  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *Progress = Request;
  return EFI_NOT_FOUND;
}

/**
  See UEFI spec for the general meaning of RouteConfig function.
  PSR is unusual in that it doesn't store configuration entered by user in EFI flash variables.
  Instead, all(*) config is retrieved from CSME firmware using HECI messages.

  @param[in]  This                     Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Configuration            A null-terminated Unicode string in <ConfigResp> format.
  @param[out] Progress                 A pointer to a string filled in with the offset of the most recent '&' before the first failing
                                       name/value pair (or the beginning of the string if the failure is in the first name/value pair) or
                                       the terminating NULL if all was successful.

  @retval     EFI_SUCCESS              The Results is processed successfully.
  @retval     EFI_INVALID_PARAMETER    Configuration is NULL.
  @retval     EFI_NOT_FOUND            Routing data doesn't match any storage in this driver.
**/
EFI_STATUS
EFIAPI
PsrRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN  CONST EFI_STRING                        Configuration,
  OUT       EFI_STRING                        *Progress
  )
{
  if ((NULL == This) || (NULL == Progress)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;

  return EFI_NOT_FOUND;

}

/**
  This function integrates PSR with BIOS setup browser by publishing PSR's formset
  and HII config access protocol.

  @retval EFI_SUCCESS    Successfully published PSR setup data
  @retval Others         Error while publishing PSR setup data
**/
EFI_STATUS
PsrSetupInit (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  DriverHandle;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  mPsrConfigAccess.ExtractConfig = PsrExtractConfig;
  mPsrConfigAccess.RouteConfig   = PsrRouteConfig;
  mPsrConfigAccess.Callback      = PsrDriverCallback;

  //
  //Locate required Hii protocols
  //
  Status = gBS->LocateProtocol (
                  &gEfiHiiConfigRoutingProtocolGuid,
                  NULL,
                  (VOID **) &mHiiConfigRouting
                  );

  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Install Device Path Protocol and Config Access protocol to driver handle.
  //
  DriverHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mPsrConfigAccess,
                  NULL
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a Publishing HII Data\n", __FUNCTION__));

  mPsrHiiHandle = HiiAddPackages (
                    &mPsrFormSetGuid,
                    DriverHandle,
                    PsrSetupStrings,
                    PsrSetupVfrBin,
                    NULL
                    );

  ASSERT (mPsrHiiHandle != NULL);

  if (mPsrHiiHandle == NULL) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return Status;
}

/**
  Driver's entry point.
  Publishes setup formset into setup browser.

  @param[in] ImageHandle     The firmware allocated handle for the EFI image.
  @param[in] SystemTable     A pointer to the EFI System Table.

  @retval EFI_SUCCESS        PSR driver initialized successfully
  @retval EFI_UNSUPPORTED    Feature is not supported by the FW
  @retval Others             PSR driver initialization failed
**/
EFI_STATUS
EFIAPI
PsrSetupEntry (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  // Locate protocol for querying the PSR data
  Status = gBS->LocateProtocol (
                  &gPlatformServiceRecordProtocolGuid,
                  NULL,
                  (VOID **) &mPlatformServiceRecordProtocol
                  );

  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  } else {
    Status = PsrSetupInit ();
  }

  return Status;
}
