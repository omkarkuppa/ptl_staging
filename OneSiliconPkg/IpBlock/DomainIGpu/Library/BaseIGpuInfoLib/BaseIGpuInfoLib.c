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
