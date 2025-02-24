/** @file
  Source file for common Base IGPU Info Library

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
#include <Library/DebugLib.h>
#include <IpIGpu.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <IGpuCfg.h>
#include <Library/DomainIGpuInit.h>
#include <IpWrapperCntxtInfoClient.h>
#include <Defines/IgdDefines.h>
#include <Library/NguInfoLib.h>
#include <IGpuDataHob.h>

//
// VGA Text Mode (Mode 3)
//
#define VGA_MODE3_BASE_ADDRESS  0xB8000
#define VGA_COLUMNS             80
#define VGA_LINES               25

#define PROGRESS_BAR_SIZE        100
#define VGA_COLOR_BLACK          0x00
#define VGA_COLOR_BLUE           0x01
#define VGA_COLOR_GREEN          0x02
#define VGA_COLOR_CYAN           0x03
#define VGA_COLOR_RED            0x04
#define VGA_COLOR_MAGENTA        0x05
#define VGA_COLOR_BROWN          0x06
#define VGA_COLOR_LIGHT_GRAY     0x07
#define VGA_COLOR_DARK_GRAY      0x08
#define VGA_COLOR_LIGHT_BLUE     0x09
#define VGA_COLOR_LIGHT_GREEN    0x0A
#define VGA_COLOR_LIGHT_CYAN     0x0B
#define VGA_COLOR_LIGHT_RED      0x0C
#define VGA_COLOR_LIGHT_MAGENTA  0x0D
#define VGA_COLOR_YELLOW         0x0E
#define VGA_COLOR_WHITE          0x0F
#define GREEN_ON_BLACK           0x02
#define WHITE_ON_BLACK           0x07

#define SOLID_BLOCK_CHAR  0xDB  // ASCII 219 solid block character
#define LIGHT_WHITE_BOX   0xB0  // ASCII 176, light white box
#define SOLID_WHITE_BOX   0xDB  // ASCII 219, solid white box
#define RED_ON_BLACK      0x04  // Red on black color attribute

//
// VGA Graphics Mode (Mode 12)
//
#define VGA_MODE12_BASE_ADDRESS         0xA0000 // VGA memory base for Mode 12 (640x480, 16 colors)
#define VGA_MODE12_WIDTH                640     // Width of the screen in pixels
#define VGA_MODE12_HEIGHT               480     // Height of the screen in pixels
#define VGA_MODE12_MAX_PLANE            4
#define VGA_MODE12_PROGRESS_BAR_HEIGHT  16
#define VGA_MODE12_CHAR_WIDTH           8
#define VGA_MODE12_CHAR_HEIGHT          16

/**
  Update the GttMmAdr if it's updated in register context.
  @param[in] pInst       Pointer to IP inst
**/
STATIC
VOID
IGpuCheckAndUpdateGttMmAdr (
  IP_IGPU_INST  *pInst
  )
{
  IP_WR_REG_INFO  *RegInfo;
  UINT64          CurrentGttMmAdr;

  RegInfo         = (IP_WR_REG_INFO *)(UINTN)pInst->MmioAccess;
  CurrentGttMmAdr = IpIGpuGetGttMmAdr (pInst);

  if (CurrentGttMmAdr != RegInfo->RegType.MmioBase) {
    DEBUG ((DEBUG_INFO, "Current GttMmAdr is not matched, updating it\n"));
    RegInfo->RegType.MmioBase = CurrentGttMmAdr;
  }
}

/**
  This function will check if Intel Graphics is Enabled or Supported

  @retval TRUE          Graphics Enabled or Supported
  @retval FALSE         Graphics not Enabled or not Supported
**/
BOOLEAN
EFIAPI
IGpuIsSupported (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;
  BOOLEAN            IgdEnable;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuSupported (IGpuInst) == FALSE) {
    DEBUG ((DEBUG_WARN, "IGPU is unsupported or disabled!\n"));
    return FALSE;
  }

  IgdEnable = UncoreDevEnRead (Graphics, 0);
  if (IgdEnable == FALSE) {
    DEBUG ((DEBUG_WARN, "IGPU is disabled from DevEn!\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  This function will check if Intel Graphics Display Present or not

  @retval TRUE          Graphics Display Present
  @retval FALSE         Graphics Display not Present
**/
BOOLEAN
EFIAPI
IGpuIsDisplayPresent (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  return (BOOLEAN)IGpuInst->IGpuPrivateConfig.IsDisplayPresent;
}

/**
  This function will check if Intel Graphics Media Present or not

  @retval TRUE          Graphics Media Present
  @retval FALSE         Graphics Media not Present
**/
BOOLEAN
EFIAPI
IGpuIsMediaPresent (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  return (BOOLEAN)IGpuInst->IGpuPrivateConfig.IsMediaPresent;
}

/**
  This function will check if Intel Graphics Gt Present or not

  @retval TRUE          Graphics Gt Present
  @retval FALSE         Graphics Gt not Present
**/
BOOLEAN
EFIAPI
IGpuIsGtPresent (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  return (BOOLEAN)IGpuInst->IGpuPrivateConfig.IsGtPresent;
}

/**
  This function will get the Intel Graphics Data Stolen Memory Size.

  @retval DsmSize          Graphics Data Stolen Memory Size
**/
UINT32
EFIAPI
IGpuGetDsmSizeInBytes (
  VOID
  )
{
  UINT32  DsmSizeSelector;
  UINT32  DsmSize;

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (!IGpuIsSupported ()) {
    return 0;
  }

  //
  // Get the GMS Value.
  //
  DsmSizeSelector = (UINT16)GetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgeGgcDsmSizeSelector);

  ///
  /// Graphics Stolen Size
  /// Graphics Stolen size above 64MB has a granularity of 32MB increments
  /// GMS values below 240 correspond to Sizes 32 * GSMValue
  /// Graphics Stolen size below 64MB has a higher granularity and can be set in 4MB increments
  /// GMS values ranging from 240-254 correspond to sizes 4MB to 60MB (excluding 32MB) which is 4*(GSMValue-239)
  ///
  if (DsmSizeSelector < 240 ) {
    DsmSize = (UINT32)DsmSizeSelector * 32;
  } else {
    DsmSize = 4 * (DsmSizeSelector - 239);
  }

  DEBUG ((DEBUG_INFO, "DsmSize = %d MB\n", DsmSize));

  return ((UINT32)DsmSize * SIZE_1MB);
}

/**
  This function will return LMem Bar Length

  @retval     LMemBar Size
**/
UINT64
EFIAPI
IGpuGetLMemBarSize (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;
  UINT64             LMemBarSize;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return SIZE_256MB;
  }

  LMemBarSize = IpIGpuGetLMemBarSize (IGpuInst);

  return LMemBarSize;
}

/**
  Get IGpu GSM Size in Mb
  @retval UINT32 - return the IGpu Ggms Size in Bytes
**/
UINT32
EFIAPI
IGpuGetGsmSizeInBytes (
  VOID
  )
{
  UINT32  GgsmSize;
  UINT16  Ggms;

  //
  // Get the GGSM Value.
  //
  Ggms = (UINT16)GetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgeGgcGgsmSize);
  if (Ggms == V_SA_GGC_GGMS_8MB) {
    GgsmSize = SIZE_8MB;
  } else {
    GgsmSize = 0;
  }

  DEBUG ((DEBUG_INFO, "GsmSize = %x\n", GgsmSize));
  return GgsmSize;
}

/**
  This function will check if DSM base is consistent in different memory scopes.
  @retval TRUE          DSM Base is consistent
  @retval FALSE         DSM Base is not consistent
**/
BOOLEAN
EFIAPI
IGpuIsDsmBaseAndImrMemoryConsistent (
  VOID
  )
{
  UINT64  BdsmBase;
  UINT64  ImrDsmBase;

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (!IGpuIsSupported ()) {
    return FALSE;
  }

  //
  // Get BdsmBase in MB.
  //
  BdsmBase   = GetHostBridgeRegisterData (HostBridgeBdsm, HostBridgeBdsmBase);
  ImrDsmBase = GetImrDsmBaseOffset ();

  if (BdsmBase == ImrDsmBase) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "DSM Base is not consistent\n"));
  return FALSE;
}

/**
  This function will check if GSM base is consistent in different memory scopes.
  @retval TRUE          GSM Base is consistent
  @retval FALSE         GSM Base is not consistent
**/
BOOLEAN
EFIAPI
IGpuIsGsmBaseAndImrMemoryConsistent (
  VOID
  )
{
  UINT64  GsmBase;
  UINT64  ImrGsmBase;

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (!IGpuIsSupported ()) {
    return FALSE;
  }

  //
  // Get BgsmBase in MB.
  //
  GsmBase    = GetHostBridgeRegisterData (HostBridgeBgsm, HostBridgeBgsmBase);
  ImrGsmBase = GetImrGsmBaseOffset ();

  if (GsmBase == ImrGsmBase) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "GSM Base is not consistent\n"));
  return FALSE;
}

/**
This function will check if GCD RC6 is in Range or not on IGD.
@param[in] GttMmAdr         IGD MMIO Base address value
@retval TRUE          Gcd RC6 is in Range
@retval FALSE         Gcd RC6 is not in Range OR GT is not present OR IGPU is not supported
**/
BOOLEAN
EFIAPI
IGpuIsGcdRc6RangeValid (
  VOID
  )
{
  UINT32             Tolud;
  UINT64             GtRc6CtxBaseVal;
  UINT32             GtRc6CtxSize;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  ///
  /// Return if Graphics not supported or not enabled or Media not present
  ///
  if (!IGpuIsSupported () && !IGpuIsGtPresent ()) {
    DEBUG ((DEBUG_INFO, "Exiting %a since IGPU is not supported or GT is not present.\n", __FUNCTION__));
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);
  //
  // GT RC6CTXBASE
  //
  GtRc6CtxBaseVal = IpIGpuGetRegistersData (IGpuInst, GtRc6CtxLowMemReg, GtRc6CtxLowMemBase);
  //
  // Get ToludBase in MB.
  //
  Tolud        = (UINT32)GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase);
  GtRc6CtxSize = SIZE_64KB;

  if (GtRc6CtxBaseVal == (Tolud - GtRc6CtxSize)) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "Gcd RC6 Range is not valid\n"));
  return FALSE;
}

/**
  This function will check if GCD RC6 Context Base is locked or not on IGD
  @param[in] GttMmAdr         IGD MMIO Base address value
  @retval TRUE          GCD RC6 is Locked
  @retval FALSE         If IGPU is not supported OR GCD RC6 is not locked OR GT not present
**/
BOOLEAN
EFIAPI
IGpuIsGcdRc6Locked (
  VOID
  )
{
  UINT64             GtRc6CtxMemLockVal;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (!IGpuIsSupported () && !IGpuIsGtPresent ()) {
    DEBUG ((DEBUG_INFO, "Exiting %a since IGPU is not supported or GT is not present.\n", __FUNCTION__));
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  GtRc6CtxMemLockVal = IpIGpuGetRegistersData (IGpuInst, GtRc6CtxLowMemReg, GtRc6CtxLowMemLock);
  if (GtRc6CtxMemLockVal) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "GCD RC6 Context Base is unlocked\n"));
  return FALSE;
}

/**
  This function will check if Media RC6 Context Base is locked or not on IGD
  @retval TRUE          Media RC6 is Locked
  @retval FALSE         If IGPU is not supported OR Media RC6 is not locked OR Media not present
**/
BOOLEAN
EFIAPI
IGpuIsMediaRc6Locked (
  VOID
  )
{
  UINT64             MediaRc6CtxMemLockValue;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (!IGpuIsSupported () && !IGpuIsMediaPresent ()) {
    DEBUG ((DEBUG_INFO, "Exiting %a since IGPU is not supported or Media is not present.\n", __FUNCTION__));
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  MediaRc6CtxMemLockValue = IpIGpuGetRegistersData (IGpuInst, MediaRc6CtxMemReg, MediaRc6CtxMemLock);
  if (MediaRc6CtxMemLockValue) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "Media RC6 Context Base is unlocked\n"));
  return FALSE;
}

/**
  Get GttMmAddr Base Address

  @retval UINT64 - return the GttMmAddr Base Address
**/
UINT64
EFIAPI
IGpuGetGttMmAdrBase (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0;
  }

  return IpIGpuGetGttMmAdr (IGpuInst);
}

/**
  Get Dsm Base Address.
  @retval 32 Bit Base Address of Dsm Register.
**/
UINT32
EFIAPI
IGpuGetDsmBase (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return 0;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  return (UINT32)IpIGpuGetRegistersData (IGpuInst, DsmBaseReg, DsmBaseAddr);
}

/**
  Get Gsm Base Address.
  @retval 32 Bit Base Address of Gsm Register.
**/
UINT32
EFIAPI
IGpuGetGsmBase (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return 0;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  return (UINT32)IpIGpuGetRegistersData (IGpuInst, GsmBaseReg, GsmBaseAddr);
}

/**
  Get IGpuSgData After Setting Lock Bit Of IGpuSg Register .
  @retval 32 Bit Data After Setting Lock Bit Of IGpuSg Register.
**/
UINT32
EFIAPI
IGpuGetGgcSgData (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;
  UINT32             GgcIGpuSgRegVal;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return 0;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  GgcIGpuSgRegVal = (UINT32)IpIGpuGetRegistersData (IGpuInst, GgcIGpuSgReg, GgcIGpuSgData);
  return (GgcIGpuSgRegVal & B_SA_IGPU_GGC_MASK);
}

/**
  Print Graphics PCI Config space in Debug log.
  @retval None
**/
VOID
EFIAPI
IGpuPrintPciConfig (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return;
  }

  IpIGpuPrintConfigSpace (IGpuInst);
}

/**
  This function will check if PAVP is in Range or not on IGD.fv

  @retval TRUE          RC6 is in Range
  @retval FALSE         RC6 is not in Range
**/
BOOLEAN
EFIAPI
IGpuIsWoPcmRangeValid (
  VOID
  )
{
  UINT32             PcmBase;
  UINT32             PcmSize;
  UINT32             PcmTop;
  UINT32             ToludBase;
  UINT32             PcmTotalSize;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  if (!IGpuIsSupported ()) {
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  //
  // PCM Base (display) B0/D2/F0:R 10h (GTTMMADR), + 0x1082C0 (PAVPC0), bits[31:20]
  //
  PcmBase = (UINT32)IpIGpuGetRegistersData (IGpuInst, MediaPavpcReg, MediaPavpcBaseMB);
  PcmSize = (UINT32)IpIGpuGetRegistersData (IGpuInst, MediaPavpcReg, MediaPavpcSize);
  //
  // PCM Total size in MB
  //
  switch (PcmSize) {
    case 0:
      PcmTotalSize = 1;
      break;
    case 1:
      PcmTotalSize = 2;
      break;
    case 2:
      PcmTotalSize = 4;
      break;
    case 3:
      PcmTotalSize = 8;
      break;
    case 5:
      PcmTotalSize = 16;
      break;
    case 6:
      PcmTotalSize = 32;
      break;
    default:
      PcmTotalSize = 8;
  }

  PcmTop = PcmBase + PcmTotalSize;

  //
  // TOLUD (IOP B0/D0/F0:R BCh (TSEGMB), bits[31:20]
  // Get ToludBase in MB.
  //
  ToludBase   = (UINT32)GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase);
  ToludBase >>= 20;

  if (PcmTop == ToludBase) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "WOPCM Range is not Valid\n"));
  return FALSE;
}

/**
  This function will check if PAVP is within Gfx Stolen range
  @retval BOOLEAN - TRUE  WOPCM is within Gfx Stolen range
                    FALSE WOPCM is not within Gfx Stolen range
**/
BOOLEAN
IsWopcmWithinStolenGfx (
  VOID
  )
{
  PAVPC0_REG_IGPU_STRUCT  PcmBasePavpc;
  UINT32                  DsmBase;
  EFI_HOB_GUID_TYPE       *GuidHob;
  IP_IGPU_INST            *IGpuInst;

  if (!IGpuIsSupported ()) {
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  PcmBasePavpc.Data = (UINT32)IpIGpuGetRegistersData (IGpuInst, MediaPavpcReg, MediaPavpcData);
  DsmBase           = IGpuGetDsmBase ();

  if (PcmBasePavpc.Bits.pcme == 1) {
    if ((LShiftU64 (PcmBasePavpc.Bits.wopcmbase_0, 20) < DsmBase) ||
        ((LShiftU64 (PcmBasePavpc.Bits.wopcmbase_0, 20) + SIZE_4MB) >= (DsmBase + IGpuGetDsmSizeInBytes ())))
    {
      DEBUG ((DEBUG_ERROR, "        WOPCM is Enabled but is not fully inside of DSM\n"));
      DEBUG ((DEBUG_INFO, "        PAVPC0 BASE: 0x%08x LIMIT: 0x%08x\n", PcmBasePavpc.Bits.wopcmbase_0, PcmBasePavpc.Bits.wopcmbase_0 + SIZE_4MB));
      DEBUG ((DEBUG_INFO, "        DSM BASE: 0x%08x LIMIT: 0x%08x\n", DsmBase, DsmBase + IGpuGetDsmSizeInBytes ()));

      return FALSE;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "        WOPCM is Disabled\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  This function will check if Media CCS base is consistent in different memory scopes.

  @retval TRUE          Media CCS Base is consistent
  @retval FALSE         Media CCS Base is not consistent or IGPU is not supported or Media is not present
**/
BOOLEAN
EFIAPI
IGpuIsMediaCcsBaseAndImrMemoryConsistent (
  VOID
  )
{
  UINT64             CcsBaseMedia;
  UINT64             ImrCcsBase;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  ///
  /// Return if Graphics not supported or not enabled or Media not present
  ///
  if (!IGpuIsSupported () && !IGpuIsMediaPresent ()) {
    DEBUG ((DEBUG_INFO, "Exiting %a since IGPU is not supported or Media is not present.\n", __FUNCTION__));
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  //
  // Get CcsBase in Media
  //
  CcsBaseMedia = IpIGpuGetRegistersData (IGpuInst, MediaFlatCcsReg, MediaFlatCcsBase);
  ImrCcsBase   = GetImrCcsBase ();
  if (CcsBaseMedia == ImrCcsBase) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "Media CCS Base is not consistent\n"));
  return FALSE;
}

/**
  This function will check if SA Media RC6 is in Range or not on IGD.

  @retval TRUE          Media RC6 is in Range
  @retval FALSE         Media RC6 is not in Range OR IGPU is not supported OR Media is not present
**/
BOOLEAN
EFIAPI
IGpuIsMediaRc6RangeValid (
  VOID
  )
{
  UINT32             MediaRc6CtxBaseVal;
  UINT32             Tolud;
  UINT32             GcdWoPcmsize;
  UINT32             MediaRc6CtxSize;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (!IGpuIsSupported () && !IGpuIsMediaPresent ()) {
    DEBUG ((DEBUG_INFO, "Exiting %a since IGPU is not supported or Media is not present.\n", __FUNCTION__));
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  // Media RC6CTXBASE
  MediaRc6CtxBaseVal = (UINT32)IpIGpuGetRegistersData (IGpuInst, MediaRc6CtxMemReg, MediaRc6CtxMemBase);

  //
  // Get ToludBase in MB.
  //
  Tolud           = (UINT32)GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase);
  GcdWoPcmsize    = GCD_WOPCM_SIZE_2_MB * SIZE_1MB;
  MediaRc6CtxSize = SIZE_64KB;

  if (MediaRc6CtxBaseVal == (Tolud - GcdWoPcmsize - MediaRc6CtxSize)) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "Media RC6 Range is not valid\n"));
  return FALSE;
}

/**
  IGpuGetDisplayDsmBase : Get Display Dsm Base Address.

  @retval UINT32 - return Display Dsm Base
**/
UINT32
EFIAPI
IGpuGetDisplayDsmBase (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  return (UINT32)IpIGpuGetRegistersData (IGpuInst, DispDsmBaseReg, DispDsmBaseAddr);
}

/**
  IGpuGetDisplayGsmBase : Get Display Gsm Base Address.

  @retval UINT32 - return Display Gsm Base
**/
UINT32
EFIAPI
IGpuGetDisplayGsmBase (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  return (UINT32)IpIGpuGetRegistersData (IGpuInst, DispGsmBaseReg, DispGsmBaseAddr);
}

/**
  IGpuGetDisplayGgcData : Get Display Ggc Data

  @retval UINT32 - return Display Ggc Data
**/
UINT32
EFIAPI
IGpuGetDisplayGgcData (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;
  UINT32             DisplayGgc;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return 0;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  DisplayGgc = (UINT32)IpIGpuGetRegistersData (IGpuInst, DispGgcReg, DispGgcData);
  return (DisplayGgc & B_SA_IGPU_GGC_MASK);
}

/**
  This function will check if WOPCM base is consistent in different memory scopes.

  @retval TRUE          WOPCM Base is consistent
  @retval FALSE         WOPCM Base is not consistent
**/
BOOLEAN
EFIAPI
IGpuIsWoPcmBaseRegistersConsistent (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;
  UINT32             PavpMediaPcmBase;
  UINT32             PavpDisplayPcmBase;

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (!IGpuIsSupported ()) {
    DEBUG ((DEBUG_INFO, "Exiting %a since IGPU is not supported\n", __FUNCTION__));
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  PavpMediaPcmBase   = (UINT32)IpIGpuGetRegistersData (IGpuInst, MediaPavpcReg, MediaPavpcBase);
  PavpDisplayPcmBase = (UINT32)IpIGpuGetRegistersData (IGpuInst, DispPavpcReg, DispPavpcBase);

  if (PavpMediaPcmBase == PavpDisplayPcmBase) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "WOPCM Base is not consistent\n"));
  return FALSE;
}

/**
  This function will check if PAVP is locked in different memory scopes.

  @retval TRUE          PAVP is locked
  @retval FALSE         PAVP is not locked
**/
BOOLEAN
EFIAPI
IGpuIsPavpBitLocked (
  VOID
  )
{
  UINT32             MediaPavpcLockValue;
  UINT32             DisplayPavpcLockValue;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (!IGpuIsSupported ()) {
    DEBUG ((DEBUG_INFO, "Exiting %a since IGPU is not supported\n", __FUNCTION__));
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  MediaPavpcLockValue   = (UINT32)IpIGpuGetRegistersData (IGpuInst, MediaPavpcReg, MediaPavpcLock);
  DisplayPavpcLockValue = (UINT32)IpIGpuGetRegistersData (IGpuInst, DispPavpcReg, DispPavpcLock);

  if ((MediaPavpcLockValue == 1) && (DisplayPavpcLockValue == 1)) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "PAVP is unlocked\n"));
  return FALSE;
}

/**
  This function will check if Gt CCS base is consistent in different memory scopes.

  @retval TRUE          Gt CCS Base is consistent
  @retval FALSE         Gt CCS Base is not consistent or GT is not present or IGPU is not supported
**/
BOOLEAN
EFIAPI
IGpuIsGtCcsBaseAndImrMemoryConsistent (
  VOID
  )
{
  UINT64             CcsBaseGt;
  UINT64             ImrCcsBase;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  ///
  /// Return if Graphics not supported or not enabled or Media not present
  ///
  if (!IGpuIsSupported () && !IGpuIsGtPresent ()) {
    DEBUG ((DEBUG_INFO, "Exiting %a since IGPU is not supported or GT is not present.\n", __FUNCTION__));
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  //
  // Get CcsBase in Gt
  //
  CcsBaseGt  = IpIGpuGetRegistersData (IGpuInst, GtFlatCcsReg, GtFlatCcsBase);
  ImrCcsBase = GetImrCcsBase ();

  if (ImrCcsBase == CcsBaseGt) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "Gt CCS Base is not consistent\n"));
  return FALSE;
}

/**
  This function will check if Gt CCS is enabled

  @retval TRUE          CCS Enable
  @retval FALSE         CCS Disable
**/
BOOLEAN
EFIAPI
IGpuIsGtCcsEnabled (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;
  UINT32             FlatCcsEnable;

  ///
  /// Return if Graphics not supported or not enabled or Media not present
  ///
  if (!IGpuIsSupported () && !IGpuIsGtPresent ()) {
    DEBUG ((DEBUG_INFO, "Exiting %a since IGPU is not supported or GT is not present.\n", __FUNCTION__));
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  ///
  /// CCS GAM (GT) B0/D2/F0:R 10h (GTTMMADR), + 0x4910 (FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_REG), bit[0]
  ///
  FlatCcsEnable = (UINT32)IpIGpuGetRegistersData (IGpuInst, GtFlatCcsGamReg, GtFlatCcsEnable);
  if (FlatCcsEnable) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "Gt CCS is disabled\n"));
  return FALSE;
}

/**
  This function will check if Media CCS is enabled

  @retval TRUE          CCS Enable
  @retval FALSE         CCS Disable
**/
BOOLEAN
EFIAPI
IGpuIsMediaCcsEnabled (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;
  UINT32             FlatCcsEnable;

  ///
  /// Return if Graphics not supported or not enabled or Media not present
  ///
  if (!IGpuIsSupported () && !IGpuIsMediaPresent ()) {
    DEBUG ((DEBUG_INFO, "Exiting %a since IGPU is not supported or Media is not present.\n", __FUNCTION__));
    return FALSE;
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return FALSE;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  ///
  /// CCS GAM (GT)
  ///
  FlatCcsEnable = (UINT32)IpIGpuGetRegistersData (IGpuInst, MediaFlatCcsGamReg, MediaFlatCcsEnable);
  if (FlatCcsEnable) {
    return TRUE;
  }

  DEBUG ((DEBUG_WARN, "Media CCS is disabled\n"));
  return FALSE;
}

/**
  Get IGPU Flat CCS (Compression Control Surface) Size in Mb

  @retval UINT32 - return the Flat CCS Size in Mb
**/
UINT32
EFIAPI
IGpuGetFlatCcsSizeInMb (
  VOID
  )
{
  UINT64             Touud;
  UINT32             FlatCcsSizeInMb;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;
  UINT64             Remainder;

  Remainder = 0;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return 0;
  }

  //
  // Get the HOB for IGPU Data
  //
  if (IGpuIsSupported () && (IGpuInst->IGpuPreMemConfig.MemoryBandwidthCompression == 1)) {
    Touud = GetHostBridgeRegisterData (HostBridgeTouud, HostBridgeTouudBase);
    Touud = RShiftU64 (Touud, 20);

    //
    // Consider the Touud to GB aligned address so that CCS will not miss out the remaining portion of graphics allocation in DRAM while dividing by 512
    //
    DivU64x64Remainder (Touud, 1024, &Remainder);
    if (Remainder != 0) {
      Touud += (1024 - Remainder);
      DEBUG ((DEBUG_INFO, "Considered TOUUD for CCS Calulation = 0x%lx\n", Touud));
    }

    FlatCcsSizeInMb = (UINT32)(Touud / 512);

    if ((FlatCcsSizeInMb % 2) == 1) {
      FlatCcsSizeInMb += 1;
    }

    DEBUG ((DEBUG_INFO, "IGpuFlatCcsSizeInMb = 0x%x\n", FlatCcsSizeInMb));
    return FlatCcsSizeInMb;
  } else {
    DEBUG ((DEBUG_INFO, "MemoryBandwidthCompression is disabled\n"));
    return 0;
  }
}

/**
  This function will return the IGPU Vendor ID

  @retval VendorId        IGPU 0 2 0 Vendor ID
**/
UINT16
EFIAPI
IGpuGetVendorId (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0xFFFF;
  }

  return IpIGpuGetVendorId (IGpuInst);
}

/**
  This function will return the IGPU SVID

  @retval SubsystemVendorId        IGPU 0 2 0 SVID
**/
UINT16
EFIAPI
IGpuGetSubsystemVendorId (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0xFFFF;
  }

  return IpIGpuGetSubsystemVendorId (IGpuInst);
}

/**
  This function will return the IGPU Device ID

  @retval DeviceId        IGPU 0 2 0 Device ID
**/
UINT16
EFIAPI
IGpuGetDeviceId (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0xFFFF;
  }

  return IpIGpuGetDeviceId (IGpuInst);
}

/**
  This function will check if Bus Initiator and Memory access on 0:2:0 is enabled or not

  @retval TRUE          IGD Bus Initiator Access and Memory Space access is Enabled
  @retval FALSE         IGD Bus Initiator Access and Memory Space access is Disable
**/
BOOLEAN
EFIAPI
IGpuCmdRegEnabled (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return FALSE;
  }

  return IpIGpuCmdRegEnabled (IGpuInst);
}

/**
  Get LMemBar

  @retval         LMemBar
**/
UINT64
EFIAPI
IGpuGetLMemBar (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0;
  }

  return IpIGpuGetLMemBar (IGpuInst);
}

/**
  This function will return the IGPU Subsystem ID

  @retval SubsystemId        IGPU 0 2 0 Subsystem ID
**/
UINT16
EFIAPI
IGpuGetSubsystemId (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0xFFFF;
  }

  return IpIGpuGetSubsystemId (IGpuInst);
}

/**
  This function will enable Bus Initiator and Memory access on 0:2:0

**/
VOID
EFIAPI
IGpuEnableCmdReg (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  IpIGpuEnableCmdReg (IGpuInst);
}

/**
  This function will clear command register of 0:2:0

  @retval     cmd register
**/
VOID
EFIAPI
IGpuClearCmdReg (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  IpIGpuClearCmdReg (IGpuInst);
}

/**
  IGpuGetDevNumber: Get IGpu Dev Number

  @retval PCI dev number for IGpu
**/
UINT8
EFIAPI
IGpuGetDevNumber (
  VOID
  )
{
  return (UINT8)IGD_DEV_NUM;
}

/**
  IGpuGetFuncNumber: Get IGpu Fun Number

  @retval PCI fun number for IGpu
**/
UINT8
EFIAPI
IGpuGetFuncNumber (
  VOID
  )
{
  return (UINT8)IGD_FUN_NUM;
}

/**
  IGpuGetGttMmAdrOffset: Get GTTMMADR Offset value

  @retval PCI Offset for GTTMMADR
**/
UINT8
EFIAPI
IGpuGetGttMmAdrOffset (
  VOID
  )
{
  return (UINT8)GTTMMADR0_IGPU_REG;
}

/**
  IGpuGetLMemBarOffset: Get LMEMBAR Offset value

  @retval PCI Offset for LMEMBAR
**/
UINT8
EFIAPI
IGpuGetLMemBarOffset (
  VOID
  )
{
  return (UINT8)LMEMBAR0_IGPU_REG;
}

/**
  Get L3Bank register status

  @retval         Status of L3BANK lock bit
**/
BOOLEAN
IGpuIsL3BankLocked (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0;
  }

  return IpIGpuIsL3BankLocked (IGpuInst);
}

/**
  Get L3Bank Media register status

  @retval         Status of L3BANK lock bit
**/
BOOLEAN
IGpuIsL3BankMediaLocked (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return 0;
  }

  return IpIGpuIsL3BankMediaLocked (IGpuInst);
}

/**
  Enable Force Wake Up for GT.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
VOID
EFIAPI
IGpuForceWakeupGt (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  IpIGpuForceWakeupGt (IGpuInst);
}

/**
  Enable Idle mode for GT.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
VOID
EFIAPI
IGpuIdleGt (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  IpIGpuIdleGt (IGpuInst);
}

/**
  Enable Force Wake Up for media.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
VOID
EFIAPI
IGpuForceWakeupMedia (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  IpIGpuForceWakeupMedia (IGpuInst);
}

/**
  Enable Idle mode for Media.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
VOID
EFIAPI
IGpuIdleMedia (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  //
  // Update GttMmAdr if not updated
  //
  IGpuCheckAndUpdateGttMmAdr (IGpuInst);

  IpIGpuIdleMedia (IGpuInst);
}

/**
  Set a character at a specific (X, Y) position in VGA text mode.
  @note: This function is only applicable for VGA text mode (Mode 3) and it assumed VGA is enabled.

  @param[in] X       The column position (0-based).
  @param[in] Y       The row position (0-based).
  @param[in] Char    The ASCII character to display.
  @param[in] Color   The attribute byte for text color (foreground/background).
**/
VOID
SetChar (
  IN UINT16  X,
  IN UINT16  Y,
  IN CHAR8   Char,
  IN UINT8   Color
  )
{
  volatile UINT16  *VgaBuffer;

  //
  // Ensure X and Y are within bounds
  //
  if ((X >= VGA_COLUMNS) || (Y >= VGA_LINES)) {
    return; // Invalid coordinates
  }

  VgaBuffer                      = (UINT16 *)(UINTN)VGA_MODE3_BASE_ADDRESS;
  VgaBuffer[Y * VGA_COLUMNS + X] = (UINT16)((Color << 8) | Char);
}

/**
  Set the current VGA plane (0-3) for memory access.

  This function sets the VGA plane for memory access by writing to the plane mask register.
  The plane mask register is addressed at port 0x3C4 with index 0x02, and the specified plane
  is enabled by writing the appropriate value to port 0x3C5.

  @param[in]  Plane  The VGA plane to set (0-3).
  @note This function assumes VGA mode 12h is active and VGA is enabled.
**/
VOID
EFIAPI
SetVgaPlane (
  IN UINT8  Plane
  )
{
  IoWrite8 (0x3C4, 0x02);                // Address the plane mask register (index 0x02)
  IoWrite8 (0x3C5, (UINT8)(1 << Plane)); // Enable writing to the specified plane
}

/**
  Draw a filled rectangle on the screen.

  This function draws a filled rectangle on the screen using VGA mode 12h.
  It processes each of the 4 VGA planes and sets or clears the corresponding
  bits in VGA memory to draw the rectangle.

  @param[in] X       The X coordinate of the top-left corner of the rectangle.
  @param[in] Y       The Y coordinate of the top-left corner of the rectangle.
  @param[in] Width   The width of the rectangle.
  @param[in] Height  The height of the rectangle.
  @param[in] Color   The color of the rectangle (1 for filled, 0 for empty).

  @note This function assumes VGA mode 12h is active and VGA is enabled.
*/
VOID
FillRectangle (
  IN UINT16  X,
  IN UINT16  Y,
  IN UINT16  Width,
  IN UINT16  Height,
  IN UINT8   Color
  )
{
  UINT16          RowIndex;
  UINT16          ColumnIndex;
  volatile UINT8  *VgaMemory;
  UINT16          BytesPerRow;
  UINT16          StartOffset;
  UINT8           Plane;

  // Ensure coordinates are within bounds
  if ((X >= VGA_MODE12_WIDTH) || (Y >= VGA_MODE12_HEIGHT) || (X + Width > VGA_MODE12_WIDTH) || (Y + Height > VGA_MODE12_HEIGHT)) {
    return;
  }

  //
  // Calculate bytes per row and start offset
  // VGA Mode 12h uses a planar memory layout with 4 planes, each storing 1 bit per pixel.
  // Each byte in VGA memory represents 8 pixels (1 bit per pixel in each of the 4 planes),
  // so we divide the width by 8 to get the number of bytes per row.
  //
  BytesPerRow = VGA_MODE12_WIDTH / 8;
  //
  // Calculate start offset
  // Considering the Y coordinate and the X coordinate divided by 8.
  // This gives the starting byte offset in VGA memory for the top-left corner of the rectangle.
  //
  StartOffset = (Y * BytesPerRow) + (X / 8);

  //
  // Write to each plane
  //
  for (Plane = 0; Plane < VGA_MODE12_MAX_PLANE; Plane++) {
    SetVgaPlane (Plane);

    for (RowIndex = 0; RowIndex < Height; RowIndex++) {
      // Process each row
      VgaMemory = (volatile UINT8 *)(UINTN)(VGA_MODE12_BASE_ADDRESS + StartOffset + (RowIndex * BytesPerRow));

      for (ColumnIndex = 0; ColumnIndex < (Width + 7) / 8; ColumnIndex++) {
        // Write each byte in the row
        // Check if the current plane bit is set in the color value
        if ((Color & (1 << Plane)) != 0) {
          // Set bits for this plane
          *VgaMemory++ = 0xFF; // Set all 8 pixels in the byte
        } else {
          *VgaMemory++ = 0x00; // Clear all 8 pixels in the byte
        }
      }
    }
  }
}

/**
  Draw an image at a specific (X, Y) position in VGA Mode 12h.

  This function draws an image at the specified (X, Y) position in VGA Mode 12h (640x480, 16 colors).
  It processes each of the 4 VGA planes and writes the corresponding data to VGA memory to render the image.

  @param[in] X          The X coordinate of the top-left corner of the image.
  @param[in] Y          The Y coordinate of the top-left corner of the image.
  @param[in] Width      The width of the image in pixels.
  @param[in] Height     The height of the image in pixels.
  @param[in] VgaBuffer  Pointer to the buffer containing the VGA-compatible graphics data.
                        Ex: VgaPlanarImage200x58[4][58][25] for a 200x58 image.

  @note This function assumes VGA Mode 12h is active and properly initialized.
**/
VOID
EFIAPI
VgaMode12DrawImage (
  IN UINT32      X,
  IN UINT32      Y,
  IN UINT32      Width,
  IN UINT32      Height,
  IN const VOID  *VgaBuffer
  )
{
  UINT8           Plane;
  const UINT8     *Buffer;
  volatile UINT8  *VgaMemBase;
  UINT32          Offset;
  UINT32          Row;
  UINT32          BytesPerRow;
  IGPU_DATA_HOB   *IGpuDataHob;

  //
  // Ensure coordinates and dimensions are within bounds
  //
  if ((X >= VGA_MODE12_WIDTH) || (Y >= VGA_MODE12_HEIGHT) || (X + Width > VGA_MODE12_WIDTH) || (Y + Height > VGA_MODE12_HEIGHT)) {
    return; // Invalid coordinates or dimensions
  }

  //
  // Retrieve the IGPU data HOB to determine the VGA display configuration
  //
  IGpuDataHob = (IGPU_DATA_HOB *)GetFirstGuidHob (&gIGpuDataHobGuid);
  if (IGpuDataHob == NULL) {
    return; // Exit if the IGPU data HOB is not found
  }

  //
  // Exit if the VGA display configuration is not Mode 12
  //
  if (!IS_VGA_MODE12_ENABLED (IGpuDataHob->VgaDisplayConfig)) {
    return;
  }

  //
  // Calculate the number of bytes per row for the given width
  // Each byte in VGA memory represents 8 pixels (1 bit per pixel in each of the 4 planes),
  // so we divide the width by 8 to get the number of bytes per row.
  //
  BytesPerRow = Width / 8;
  Buffer      = (const UINT8 *)VgaBuffer;

  // Process each of the 4 VGA planes
  for (Plane = 0; Plane < VGA_MODE12_MAX_PLANE; Plane++) {
    SetVgaPlane (Plane); // Set the current VGA plane

    for (Row = 0; Row < Height; Row++) {
      //
      // - The buffer is structured as [Planes][Height][Width/8].
      // - Each plane contains (Height * BytesPerRow) bytes.
      // - To access the correct plane, multiply Plane by (Height * BytesPerRow).
      // - To access the correct row within that plane, add (Row * BytesPerRow).
      // - This results in a linear offset into the 3D buffer.
      //
      Offset = (Plane * Height * BytesPerRow) + (Row * BytesPerRow);
      //
      // VGA memory is arranged as a linear framebuffer.
      // Each row in VGA memory is (640 / 8) = 80 bytes wide.
      // (Y + Row) gives the vertical position.
      // (Y + Row) * (VGA_MODE12_WIDTH / 8) moves to the correct row in VGA memory.
      // (X / 8) finds the byte offset within that row (since each byte represents 8 pixels).
      //
      VgaMemBase = (volatile UINT8 *)(UINTN)(VGA_MODE12_BASE_ADDRESS + ((Y + Row) * (VGA_MODE12_WIDTH / 8)) + (X / 8));
      //
      // Copy the data from the buffer to VGA memory
      //
      CopyMem ((VOID *)VgaMemBase, &Buffer[Offset], BytesPerRow);
    }
  }
}

/**
  Draw a character at a specific (X, Y) position in VGA Mode 12h.

  This function draws a character at the specified (X, Y) position in VGA Mode 12h (640x480, 16 colors).
  It processes each of the 4 VGA planes and sets the corresponding bits in VGA memory to draw the character.

  @param[in] X         The X coordinate of the top-left corner of the character.
  @param[in] Y         The Y coordinate of the top-left corner of the character.
  @param[in] Char      The ASCII character to display.
  @param[in] CharData  Pointer to the 8x16 character bitmap data.

  @note This function assumes VGA Mode 12h is active and VGA is enabled.
**/
VOID
VgaMode12DrawChar (
  UINT16  X,
  UINT16  Y,
  CHAR8   Char,
  UINT8   *CharData
  )
{
  volatile UINT8  *VgaMemBase;
  UINT8           CurrentByte;
  UINT8           Plane;
  UINT8           Bit;
  UINT8           Bitmap;
  UINT8           Row;
  UINT32          ScreenOffset;
  UINT32          ScreenOffsetBase;

  //
  // Ensure the coordinates are within bounds and the character data is not null.
  //
  if ((X >= VGA_MODE12_WIDTH) || (Y >= VGA_MODE12_HEIGHT) || (CharData == NULL)) {
    return; // Invalid coordinates or null character data
  }

  //
  // Calculate the base offset in VGA memory for the character's position
  // VGA Mode 12h uses a planar memory layout with 4 planes, each storing 1 bit per pixel.
  // Each byte in VGA memory represents 8 pixels (1 bit per pixel), so we divide by 8 to get the byte offset.
  //
  ScreenOffsetBase = (UINT32)((Y * VGA_MODE12_WIDTH / 8) + (X / 8));

  //
  // Iterate over each row of the character bitmap
  //
  for (Row = 0; Row < VGA_MODE12_CHAR_HEIGHT; Row++) {
    //
    // Get the bitmap data for the current row
    //
    Bitmap = CharData[Row];

    //
    // Process each of the 4 VGA planes
    //
    for (Plane = 0; Plane < 4; Plane++) {
      SetVgaPlane (Plane); // Set the current VGA plane
      //
      // Calculate the screen offset for the current row and plane
      // Each byte in VGA memory represents 8 pixels (1 bit per pixel), so we divide by 8 to get the byte offset.
      // The offset is calculated by adding the base offset and the row offset.
      //
      ScreenOffset = ScreenOffsetBase + (Row * (VGA_MODE12_WIDTH / 8));
      VgaMemBase   = (volatile UINT8 *)VGA_MODE12_BASE_ADDRESS;

      // Read the existing value to preserve other bits
      CurrentByte = VgaMemBase[ScreenOffset];

      //
      // Iterate over each bit in the byte
      //
      for (Bit = 0; Bit < VGA_MODE12_CHAR_WIDTH; Bit++) {
        //
        // Check if the corresponding bit in the bitmap is set
        // Bitmap is an 8-bit value where each bit represents a pixel in the character row.
        // If the bit is set (1), we set the corresponding bit in the VGA memory byte.
        //
        if (Bitmap & (1 << (7 - Bit))) {
          CurrentByte |= (1 << (7 - Bit));  // Set the bit if the corresponding bit in the bitmap is set
        }
      }

      // Write back the modified byte to VGA memory
      VgaMemBase[ScreenOffset] = CurrentByte;
    }
  }
}

/**
  Updates the progress bar on the VGA display.

  This function is responsible for updating the progress bar displayed on the VGA screen.
  It ensures that the visual representation of the progress is accurately reflected based
  on the current progress state.

  @param[in] Percentage  The percentage of the progress bar to fill (0-100).
**/
VOID
UpdateProgressBar (
  IN UINT8  Percentage
  )
{
  UINT8          Index;
  IGPU_DATA_HOB  *IGpuDataHob;
  BOOLEAN        IsMode3;
  UINT8          MaxColumns;
  UINT16         FilledWidth;

  //
  // Ensure the percentage is within the valid range (0-100).
  //
  if (Percentage > 100) {
    Percentage = 100;
  }

  //
  // Retrieve the IGPU data HOB to determine the VGA display configuration
  //
  IGpuDataHob = (IGPU_DATA_HOB *)GetFirstGuidHob (&gIGpuDataHobGuid);
  if (IGpuDataHob == NULL) {
    return; // Exit if the IGPU data HOB is not found
  }

  //
  // Exit if the VGA display is disabled
  //
  if (IGpuDataHob->VgaDisplayConfig == VGA_DISPLAY_DISABLED) {
    return;
  }

  //
  // Check if it's VGA Mode3 or Mode12
  //
  if (IS_VGA_MODE3_ENABLED (IGpuDataHob->VgaDisplayConfig)) {
    IsMode3 = TRUE;
  } else if (IS_VGA_MODE12_ENABLED (IGpuDataHob->VgaDisplayConfig)) {
    IsMode3 = FALSE;
  } else {
    return; // Exit if the VGA display configuration is invalid
  }

  //
  // Print the progress bar.
  //
  if (IsMode3) {
    //
    // Calculate the maximum columns to fill based on the percentage.
    //
    MaxColumns = (UINT8)(((UINT16)Percentage * (UINT16)VGA_COLUMNS) / 100);
    for (Index = 0; Index < MaxColumns; Index++) {
      //
      // Set each character in the progress bar to a solid block character with white color.
      // Display the progress bar in the 2nd line from the bottom of the screen.
      //
      SetChar (Index, VGA_LINES - 2, SOLID_BLOCK_CHAR, VGA_COLOR_WHITE);
    }
  } else {
    //
    // Calculate the filled width for the progress bar based on the percentage.
    //
    FilledWidth = (UINT16)(((UINT16)Percentage * (UINT16)VGA_MODE12_WIDTH) / 100);
    //
    // Draw a filled rectangle for the progress bar with white color.
    // Considering a line size of 16 pixels, we will have 30 lines (480 / 16).
    // Display the progress bar in the 2nd line from the bottom of the screen.
    //
    FillRectangle (0, VGA_MODE12_HEIGHT - (2 * VGA_MODE12_PROGRESS_BAR_HEIGHT), FilledWidth, VGA_MODE12_PROGRESS_BAR_HEIGHT, VGA_COLOR_WHITE);
  }
}

/**
  Clear the VGA display based on the current video mode (Mode 3 or Mode 12).

  This function checks the current VGA mode, and depending on whether the mode is
  Mode 3 (80x25 text mode) or Mode 12 (640x480 graphics mode with 16 colors),
  it clears the display by setting all memory locations to zero, effectively
  resetting the display to a blank state (black background).
**/
VOID
ClearVgaDisplay (
  VOID
  )
{
  IGPU_DATA_HOB    *IGpuDataHob;
  volatile UINT16  *VgaBuffer;
  volatile UINT8   *VgaMode12Buffer;
  UINTN            BufferSize;
  UINT8            Plane;

  //
  // Retrieve the IGPU data HOB to determine the VGA display configuration
  //
  IGpuDataHob = (IGPU_DATA_HOB *)GetFirstGuidHob (&gIGpuDataHobGuid);
  if (IGpuDataHob == NULL) {
    return; // Exit if the IGPU data HOB is not found
  }

  BufferSize = 0;

  // Determine the VGA mode and set the buffer and size accordingly
  if (IS_VGA_MODE3_ENABLED (IGpuDataHob->VgaDisplayConfig)) {
    //
    // Point to the VGA framebuffer for Mode 3 (0xB8000)
    //
    VgaBuffer = (UINT16 *)(UINTN)VGA_MODE3_BASE_ADDRESS;
    //
    // Calculate the size of the framebuffer for Mode 3
    //
    // Mode 3 uses a resolution of 80x25 (80 columns and 25 rows).
    // - Each character cell on the screen is represented by 2 bytes:
    //   - 1 byte for the character data (ASCII code)
    //   - 1 byte for the attribute data (color and other settings).
    // - Therefore, each character occupies 2 bytes in memory.
    //
    // Total number of character cells: 80 columns * 25 rows = 2,000 cells
    // Since each character cell occupies 2 bytes, the total framebuffer size is:
    // 2,000 character cells * 2 bytes per cell = 4,000 bytes.
    //
    // Framebuffer size: 80 * 25 * 2 = 4,000 bytes (0xFA0 bytes).
    //
    BufferSize = VGA_COLUMNS * VGA_LINES * sizeof (UINT16);

    // Clear the display by setting all pixels or characters to zero (black background)
    ZeroMem ((VOID *)VgaBuffer, BufferSize);
  } else if (IS_VGA_MODE12_ENABLED (IGpuDataHob->VgaDisplayConfig)) {
    //
    // Point to the VGA framebuffer for Mode 12 (0xA0000)
    //
    VgaMode12Buffer = (volatile UINT8 *)(UINTN)VGA_MODE12_BASE_ADDRESS;
    //
    // Calculate the framebuffer size for Mode 12h (640x480x16).
    // Mode 12h uses a planar memory layout with 16 colors (4-bit color depth).
    // - The total number of pixels is 640 (width) * 480 (height) = 307,200 pixels.
    // - Each pixel is stored in 4 separate bit planes.
    // - Each plane stores 1 bit per pixel, meaning 8 pixels are packed into 1 byte.
    // - Therefore, the number of bytes per plane is:
    //     (640 * 480) / 8 = 38,400 bytes per plane.
    // - Since there are 4 planes, the total framebuffer size is:
    //     38,400 bytes * 4 planes = 153,600 bytes.
    //
    BufferSize = (VGA_MODE12_WIDTH * VGA_MODE12_HEIGHT) / 8; // 38,400 bytes
    for (Plane = 0; Plane < VGA_MODE12_MAX_PLANE; Plane++) {
      SetVgaPlane (Plane); // Activate the plane
      ZeroMem ((VOID *)VgaMode12Buffer, BufferSize);
    }
  }
}
