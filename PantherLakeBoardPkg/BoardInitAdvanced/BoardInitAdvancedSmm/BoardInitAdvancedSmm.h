/** @file
  Header file for the Board Advanced SMM Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#ifndef _BOARD_INIT_ADVANCED_SMM_H_
#define _BOARD_INIT_ADVANCED_SMM_H_

#include "Platform.h"
#include <SetupVariable.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DxeSmmScriptLib/DxeSmmScriptLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <PlatformBoardType.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmPowerButtonDispatch2.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmIoTrapDispatch2.h>
#include <Protocol/SmmUsbDispatch2.h>
#include <Library/EcMiscLib.h>
#include <Protocol/PlatformNvsArea.h>
#include <IndustryStandard/Pci30.h>
#include <Protocol/PchEspiSmiDispatch.h>

#define  GUARD_BAND_TIME       15
#define  RTC_HOUR_INDEX        0x4
#define  RTC_MIN_INDEX         0x2
#define  RTC_SEC_INDEX         0x0
#define  RTC_HOUR_ALARM_INDEX  0x5
#define  RTC_MIN_ALARM_INDEX   0x3
#define  RTC_SEC_ALARM_INDEX   0x1
#define  RTC_REGISTER_B        0x0B
//
// Related data structures definition
//
typedef struct _EFI_ACPI_SMM_DEV {
  EFI_PHYSICAL_ADDRESS   RuntimeScriptTableBase;
  UINT32                 BootScriptSaved;
} EFI_ACPI_SMM_DEV;


//
// Callback function prototypes
//
EFI_STATUS
EFIAPI
PowerButtonCallback (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  );

EFI_STATUS
EFIAPI
S3SleepEntryCallBack (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  );

EFI_STATUS
EFIAPI
S4SleepEntryCallBack (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  );

EFI_STATUS
EFIAPI
S5SleepEntryCallBack (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  );

EFI_STATUS
IntelUsb20SmiHandler (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  EFI_SMM_USB_REGISTER_CONTEXT            *DispatchContext
);

VOID
WakeOnRTCS5 (
  VOID
);

UINT8
ReadRtcIndex (
  IN UINT8 Index
);

VOID
WriteRtcIndex (
  IN UINT8 Index,
  IN UINT8 Value
);

VOID
ClearRtcSts (
  VOID
);

VOID
RtcForceSetGuardBandAlarmTime (
  VOID
);

VOID
ConfigureGuardBandRtcReg (
  VOID
);

VOID
WadtForceSetGuardBandAlarmTime (
  VOID
);
#endif

