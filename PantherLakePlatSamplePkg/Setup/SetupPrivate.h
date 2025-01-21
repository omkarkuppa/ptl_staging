/**@file

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

#ifndef __SetupPrivate__H__
#define __SetupPrivate__H__
#ifdef __cplusplus
extern "C" {
#endif

#ifndef VFRCOMPILE
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiPopup.h>
#include <Protocol/HiiString.h>
#include <Protocol/HiiDatabase.h>
#include <PlatformBoardType.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/IdeControllerInit.h>
#include <Protocol/PciIo.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/SmbusHc.h>
#include <Protocol/SystemMonitor.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeMeLib.h>
#include <Library/PciSegmentLib.h>
#include <Guid/GlobalVariable.h>
#include <Guid/MdeModuleHii.h>
#include <Guid/EventGroup.h>
#include <IndustryStandard/Pci30.h>
#include <IndustryStandard/SmBios.h>
#include <IndustryStandard/Acpi.h>
#include <BupMsgs.h>
#include <DebugTokenDataHob.h>

VOID
EFIAPI
InitString (
  EFI_HII_HANDLE HiiHandle,
  EFI_STRING_ID  StrRef,
  CHAR16         *sFormat,
  ...
  );

EFI_STATUS
HiiLibGetString (
  IN  EFI_HII_HANDLE                  PackageList,
  IN  EFI_STRING_ID                   StringId,
  OUT EFI_STRING                      String,
  IN  OUT UINTN                       *StringSize
  );

EFI_STRING
EFIAPI
HiiConstructRequestString (
  IN EFI_STRING      RequestString, OPTIONAL
  IN UINTN           Offset,
  IN UINTN           Width
  );

typedef struct {
  EFI_GUID                       *pGuid;
  EFI_HII_CONFIG_ACCESS_PROTOCOL *pFormCallback;
  UINT16                         Class, SubClass;
  EFI_HII_HANDLE                 HiiHandle;
} CALLBACK_INFO;

#define INVALID_HII_HANDLE NULL

#endif

#include <SetupStrTokens.h>
#include <Setup.h>
#include <SetupId.h>
#include <OemSetup.h>

extern SA_SETUP                  mSaSetup;
extern ME_SETUP                  mMeSetup;
extern CPU_SETUP                 mCpuSetup;
extern PCH_SETUP                 mPchSetup;
extern SETUP_DATA                mSetupData;
extern SETUP_VOLATILE_DATA       mSetupVolatileData;

#define PLATFORM_SETUP_VARSTORE_NAME PLATFORM_SETUP_VARIABLE_NAME

#define MAIN_FORM_SET_GUID     { 0x985eee91, 0xbcac, 0x4238, { 0x87, 0x78, 0x57, 0xef, 0xdc, 0x93, 0xf2, 0x4e } }
#define ADVANCED_FORM_SET_GUID { 0xe14f04fa, 0x8706, 0x4353, { 0x92, 0xf2, 0x9c, 0x24, 0x24, 0x74, 0x6f, 0x9f } }
#define CHIPSET_FORM_SET_GUID  { 0xadfe34c8, 0x9ae1, 0x4f8f, { 0xbe, 0x13, 0xcf, 0x96, 0xa2, 0xcb, 0x2c, 0x5b } }
#define BOOT_FORM_SET_GUID     { 0x8b33ffe0, 0xd71c, 0x4f82, { 0x9c, 0xeb, 0xc9, 0x70, 0x58, 0xc1, 0x3f, 0x8e } }
#define EXIT_FORM_SET_GUID     { 0xa43b03dc, 0xc18a, 0x41b1, { 0x91, 0xc8, 0x3f, 0xf9, 0xaa, 0xa2, 0x57, 0x13 } }
#define RESET_FORM_SET_GUID    { 0x46975fe8, 0x64f6, 0x4692, { 0x8d, 0x64, 0xd5, 0x6a, 0xa7, 0x52, 0xea, 0xa9 } }
#define TCG_FORM_SET_GUID      { 0xb0f901e4, 0xc424, 0x45de, { 0x90, 0x81, 0x95, 0xe2, 0xb,  0xde, 0x6f, 0xb5 } }
#define VTIO_FORM_SET_GUID     { 0xb318a3fb, 0xc98c, 0x43f4, { 0x86, 0x55, 0xc7, 0x61, 0x33, 0xac, 0xde, 0x44 } }

#define MAIN_FORM_SET_CLASS             0x01
#define ADVANCED_FORM_SET_CLASS         0x02
#define CHIPSET_FORM_SET_CLASS          0x04
#define BOOT_FORM_SET_CLASS             0x08
#define EXIT_FORM_SET_CLASS             0x20
#define RESET_FORM_SET_CLASS            0x40

#define LABEL_CPU_RATIO                 1

#define MAX_ICC_LIMIT                   2040 /// 510 * 4

#define LABEL_FORM_START                0xf000

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif
